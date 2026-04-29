


//SCI（Serial Communications Interface）通信任务
//SciTask.c 是逆变器 / PCS / 光伏储能系统的串口通信任务模块，实现串口数据接收、解析、命令响应与状态发送，负责上位机与系统之间的串口交互。


// 定义SCI任务的全局开关宏，结合前序头文件的#ifdef逻辑，使本文件内SCI_TASK宏置空，函数为全局定义
#define SCI_TASK_GLOBLES

/******************************************************************************
                                   INCLUDES 头文件包含
******************************************************************************/
// 应用层公共头文件：包含全局宏定义、数据类型、结构体、公共函数声明
#include "App\include\head.h"
// SCI驱动层头文件：包含SCI硬件驱动的初始化、发送、接收、缓冲区管理等函数声明
#include "Driver\Sci\SciDriver.h"

/******************************************************************************
                                  DEFINITIONS 宏定义
******************************************************************************/
// SCIA串口接收队列缓冲区大小（150字节）：为SCIA的485通信接收数据分配缓冲区，存储从站/外设的应答数据
#define cSciaRxQBufAlloc    150
// SCIB串口接收队列缓冲区大小（150字节）：为SCIB的485通信接收数据分配缓冲区，兼容多外设通信（风扇/电表/BCB）
#define cScibRxQBufAlloc    150

/******************************************************************************
                                 DECLARATIONS 全局变量声明
******************************************************************************/
// DSP上电延时计数器：用于逆变器上电后，延时启动并联控制等功能，防止上电初期硬件未稳定导致的通信异常
INT16U wDspPowerOnDlyCnt = 0;

// SCI任务800ms定时器：基于eSciTimer事件（100ms周期）计数，满8次为800ms，触发800ms周期的外设轮询/数据处理
INT16U wSciTask800msTimer = 0;
// SCI任务1s定时器：基于eSciTimer事件（100ms周期）计数，满10次为1s，触发1s周期的定时任务（绝缘检测/时长统计）
INT16U wSciTask1sTimer = 0;

// FRAM闪存重写计数器：FRAM写入失败时，用于计数重写次数，最多重写5次，防止无限重写占用系统资源
INT16U wRewriteFramCnt = 0;

// 主风扇类型历史值：存储上一次的主风扇类型（EBM/DELTA），用于检测风扇类型变更并重新初始化风扇参数
INT8U  bMFanTypePre;
// 通信主风扇ID：当前正在进行485通信的主风扇编号（如cMF1Id=1），实现多风扇的轮询通信
INT16U wCommMFId;

// 主风扇最大数量：初始化为全局宏cMFSize（风扇总数），后续根据设备单元数（wDevIdNum）动态更新为实际风扇数
INT16U wMFMaxSize = cMFSize;

// BCB参数设置看门狗计数器：BCB轮询时计数，满60s触发一次BCB参数重设，防止BCB参数丢失
INT16U wBCBSetWDCnt = 0;

// ADB参数设置看门狗计数器：ADB轮询时计数，初始化成功后触发ADB参数设置，保证ADB配置生效
INT16U wADBSetWDCnt = 0;

// 外部变量：ADB通信故障标志（其他模块定义，本模块读取），0=通信正常，1=通信故障，用于触发ADB通信告警
extern INT8U bADBCommFaultFlag;
// 外部变量：ADB设置指令标志（其他模块定义，本模块读取/修改），0=无设置指令，1=执行ADB参数设置，控制ADB配置流程
extern INT8U bADBSettingCmdFlag;

/******************************************************************************
                                  FUNCTIONS 函数定义
******************************************************************************/
// SCI任务主函数：整个SCI模块的核心入口，基于事件驱动，循环等待并处理各类系统事件
void sSciTask(void)
{
    // 定义事件变量：存储从操作系统获取的待处理事件（如ePollMFan/eSciTimer等）
    TASK_EVENT event;

    // 第一步：从FRAM闪存读取设备累计运行小时数，存储到stLifeTime（设备生命周期）结构体的dwHour成员
    // 入参：FRAM的SPIID、运行小时数存储地址、接收数据的指针
    sbFramReadDWord(cFramSpiId,cRunHourAddr,&stLifeTime.dwHour);

    // 初始化FRAM重写计数器为0，准备处理后续FRAM写入事件
    wRewriteFramCnt = 0;

    // 条件编译：非cTNPC板卡执行此段代码（不同板卡的SCI串口分配不同）
  #if(BOARD_TYPE != cTNPC)
    // 初始化SCIA串口驱动：入参=SCIA编号、接收缓冲区大小、串口类型为SCI；完成SCIA硬件初始化、缓冲区分配、中断使能
    sInitSciDriver(cSciANum,cSciaRxQBufAlloc,cSciTypeSci);
    // 置位SCIA的485收发控制引脚为接收模式（RD=Receive Disable，此处为宏定义，实际是拉低/拉高引脚切换485为接收）
    mSCIA_PC485_RD;
  #endif

    // 初始化SCIB串口驱动：所有板卡均执行，入参=SCIB编号、接收缓冲区大小、串口类型为SCI；初始化SCIB硬件和缓冲区
    sInitSciDriver(cSciBNum,cScibRxQBufAlloc,cSciTypeSci);
    // 置位SCIB的485收发控制引脚为接收模式，准备接收外设的485应答数据
    mSCIB_PC485_RD;

    // 初始化电表通信协议：初始化电表的Modbus（或自定义）通信协议、帧格式、校验方式，准备与电表通信
    sInitMeterProtol();

    // 初始化BCB（电池控制板/并网控制板）参数：初始化BCB的通信参数、配置参数、状态变量，准备与BCB交互
    sBCBParasInit();
    // 初始化BCB设置看门狗计数器为0，开始BCB轮询计数
    wBCBSetWDCnt = 0;

    // 读取当前主风扇类型（EBM/DELTA），赋值给历史值bMFanTypePre，用于后续类型变更检测
    bMFanTypePre = sbGetMainFanType();
    // 初始化通信主风扇ID为第一路风扇（cMF1Id），开始从第一路风扇轮询通信
    wCommMFId = cMF1Id;
    // 初始化主风扇参数：初始化风扇的通信参数、转速控制参数、故障检测阈值，准备风扇的控制与轮询
    sMFParasInit();

    // 初始化数据记录模块：初始化发电量、运行时长、故障记录等数据的存储参数，准备数据统计
    sDataRecInit();

    // 初始化绝缘检测模块：初始化直流侧绝缘检测的参数、阈值、状态变量，准备绝缘电阻检测与保护
    sInitInsModu();

    // 初始化交流侧绝缘检测模块：初始化交流侧绝缘检测的硬件和参数，适配并网绝缘要求
    sInitAcInsDet();
    // 初始化交流侧绝缘单元检测：初始化交流侧绝缘检测单元的状态和检测逻辑
    sInitAcIsuCheck();
    // 初始化ADB设置看门狗计数器为0，开始ADB轮询计数
    wADBSetWDCnt = 0;

    // 第二步：无限循环，等待并处理操作系统事件（事件驱动核心逻辑）
    while(1)
    {
        // 等待操作系统事件：挂起任务，直到有事件触发，将触发的事件赋值给event变量
        event = OSEventPend();

        // 事件1：处理FRAM写入系数事件（eWriteFram）- 写入逆变器控制系数到FRAM，掉电保存
        if(event&(1 << eWriteFram))
        {
            // 调用FRAM写入系数函数，返回cSuccess表示写入成功，否则失败
            if(cSuccess != sbFramWriteCoeff())
            {
                // 写入失败，重写计数器自增，若小于5次则重新发送eWriteFram事件，再次尝试写入
                if(++wRewriteFramCnt < 5)
                {
                    OSEventSend(cPrioSixth,eWriteFram);
                }
                else
                {
                    // 重写5次仍失败，重置计数器，放弃写入（防止死循环）
                    wRewriteFramCnt = 0;
                }
            }
            else
            {
                // 写入成功，重置重写计数器
                wRewriteFramCnt = 0;
            }
        }

        // 事件2：处理FRAM写入设备类型事件（eFrWrMachTyp）- 写入设备型号到FRAM，掉电保存
        if(event&(1 << eFrWrMachTyp))
        {
            // 调用FRAM写入设备类型函数，失败则重写
            if(cSuccess != sbFramWriteMachineType())
            {
                if(++wRewriteFramCnt < 5)
                {
                    OSEventSend(cPrioSixth,eFrWrMachTyp);
                }
                else
                {
                    wRewriteFramCnt = 0;
                }
            }
            else
            {
                wRewriteFramCnt = 0;
            }

            // 若重写计数器为0（写入成功），则调用函数写入设备版本号到FRAM，完成设备信息的掉电保存
            if(wRewriteFramCnt == 0)
            {
                sFramWriteDevVer();
            }
        }

        // 事件3：处理FRAM写入关键参数事件（eFrWrKeyParas）- 写入逆变器关键运行参数到FRAM，掉电保存
        if(event&(1 << eFrWrKeyParas))
        {
            // 调用FRAM写入关键参数函数，失败则重写（逻辑同前）
            if(cSuccess != sbFramWriteKeyParas())
            {
                if(++wRewriteFramCnt < 5)
                {
                    OSEventSend(cPrioSixth,eFrWrKeyParas);
                }
                else
                {
                    wRewriteFramCnt = 0;
                }
            }
            else
            {
                wRewriteFramCnt = 0;
            }
        }

        // 事件4：处理主风扇轮询事件（ePollMFan）- 按cPollMFanPrd周期轮询风扇，读取风扇运行参数（运行时长/最大转速等）
        if(event&(1 << ePollMFan))  // cPollMFanPrd为风扇轮询周期宏
        {
            // 静态局部变量：风扇运行时长读取计数器，按风扇数量均分超时时间，避免单风扇通信超时阻塞整体
            static INT16U wMFRunHourRdCnt[cMFSize] = {0};
            // 静态局部变量：EBM风扇最大转速读取计数器，适配EBM风扇的转速参数读取
            static INT16U wEBMMFMaxSpdRdCnt[cMFSize] = {0};
            // 静态局部变量：DELTA风扇计数基准读取计数器，适配DELTA风扇的参数读取
            static INT16U wDELTAMFCntBaseRdCnt[cMFSize] = {0};
            // 静态局部变量：风扇运行时长查询计数器，累计到1800秒（0.5小时）触发一次所有风扇运行时长读取
            static INT16U wQueryMFRunHourCnt = 0;
            // 静态局部变量：EBM风扇最大转速查询计数器，累计到20秒触发一次所有EBM风扇最大转速读取
            static INT16U wQueryEBMMFMaxSpdCnt = 0;
            // 静态局部变量：DELTA风扇计数基准查询计数器，累计到20秒触发一次所有DELTA风扇计数基准读取
            static INT16U wQueryDELTAMFCntBaseCnt = 0;

            // 每0.5小时读取一次所有风扇的运行时长：计数器累计到1800秒对应的周期数，触发读取
            if(++wQueryMFRunHourCnt >= mSciTaskSec2Cnt(1800,cPollMFanPrd))  // mSciTaskSec2Cnt：秒数转任务周期数
            {
                INT16S i;
                // 重置计数器，准备下一次累计
                wQueryMFRunHourCnt = 0;
                // 遍历所有风扇，置位运行时长读取标志，触发后续读取操作
                for(i=0;i<cMFSize;i++)
                {
                    bMFRunHourRdFlag[i] = true;
                }
            }

            // 分支1：当前主风扇类型为EBM风扇
            if(sbGetMainFanType() == cEBMMainFan)
            {
            	// 重置DELTA风扇相关计数器，避免交叉触发
            	wQueryDELTAMFCntBaseCnt = 0;
            	// 清除当前通信风扇的DELTA计数基准读取标志
            	bDELTAMFCntBaseRdFlag[wCommMFId] = false;

                // 每20秒读取一次所有EBM风扇的最大转速
                if(++wQueryEBMMFMaxSpdCnt >= mSciTaskSec2Cnt(20,cPollMFanPrd))   // 20秒转周期数
                {
                	// 重置计数器
                	wQueryEBMMFMaxSpdCnt = 0;
                	INT16S j;
                	// 遍历所有风扇，置位EBM最大转速读取标志
                    for(j=0;j<cMFSize;j++)
                    {
                    	bEBMMFMaxSpdRdFlag[j] = true;
                    }
                }
            }
            // 分支2：当前主风扇类型为DELTA风扇
            else if(sbGetMainFanType() == cDELTAMainFan)
            {
            	// 重置EBM风扇相关计数器
            	wQueryEBMMFMaxSpdCnt = 0;
				// 清除当前通信风扇的EBM最大转速读取标志
				bEBMMFMaxSpdRdFlag[wCommMFId] = false;

                // 每20秒读取一次所有DELTA风扇的计数基准
                if(++wQueryDELTAMFCntBaseCnt >= mSciTaskSec2Cnt(20,cPollMFanPrd))   // 20秒转周期数
                {
                	// 重置计数器
                	wQueryDELTAMFCntBaseCnt = 0;
                	INT16S k;
                	// 遍历所有风扇，置位DELTA计数基准读取标志
                    for(k=0;k<cMFSize;k++)
                    {
                    	bDELTAMFCntBaseRdFlag[k] = true;
                    }
                }
            }
            // 分支3：未知风扇类型（异常）
            else
            {
            	// 重置所有风扇相关查询计数器
            	wQueryEBMMFMaxSpdCnt = 0;
            	wQueryDELTAMFCntBaseCnt = 0;
				// 清除当前通信风扇的所有风扇参数读取标志
				bEBMMFMaxSpdRdFlag[wCommMFId] = false;
            	bDELTAMFCntBaseRdFlag[wCommMFId] = false;
            }

            // 按标志位优先级，执行当前通信风扇的参数读取操作
            // 优先级1：读取风扇运行时长
            if(bMFRunHourRdFlag[wCommMFId] != false)
            {
                // 发送查询指令，读取指定ID风扇的运行时长
                sQueryMFRunHour(wCommMFId);
                // 读取计数器自增，若达到超时时间（按风扇数量均分），则重置计数器并清除标志
                if(++wMFRunHourRdCnt[wCommMFId] >= mSciTaskSec2Cnt(40,cPollMFanPrd)/cMFSize)
                {
                    wMFRunHourRdCnt[wCommMFId] = 0;
                    bMFRunHourRdFlag[wCommMFId] = false;
                }
            }
            // 优先级2：读取EBM风扇最大转速
            else if(bEBMMFMaxSpdRdFlag[wCommMFId] != false)
            {
            	// 发送查询指令，读取指定ID EBM风扇的最大转速
            	sQueryEBMMFMaxSpd(wCommMFId);
            	// 读取计数器自增，达到超时时间则重置并清除标志
            	if(++wEBMMFMaxSpdRdCnt[wCommMFId] >= mSciTaskSec2Cnt(8,cPollMFanPrd)/cMFSize)
            	{
            		wEBMMFMaxSpdRdCnt[wCommMFId] = 0;
            		bEBMMFMaxSpdRdFlag[wCommMFId] = false;
            	}
            }
            // 优先级3：读取DELTA风扇计数基准
            else if(bDELTAMFCntBaseRdFlag[wCommMFId] != false)
            {
            	// 发送查询指令，读取指定ID DELTA风扇的计数基准
            	sQueryDELTAMFCntBase(wCommMFId);
            	// 读取计数器自增，达到超时时间则重置并清除标志
            	if(++wDELTAMFCntBaseRdCnt[wCommMFId] >= mSciTaskSec2Cnt(8,cPollMFanPrd)/cMFSize)
            	{
            		wDELTAMFCntBaseRdCnt[wCommMFId] = 0;
            		bDELTAMFCntBaseRdFlag[wCommMFId] = false;
            	}
            }
            // 无参数读取需求时，查询风扇常规运行信息（转速/温度/故障状态）
            else
            {
            	// 重置所有读取计数器
            	wMFRunHourRdCnt[wCommMFId] = 0;
            	wEBMMFMaxSpdRdCnt[wCommMFId] = 0;
            	wDELTAMFCntBaseRdCnt[wCommMFId] = 0;
                // 发送查询指令，读取指定ID风扇的常规运行信息
                sQueryMFRunInfo(wCommMFId);
            }
        }

        // 事件5：处理主风扇控制事件（eCtrlMFan）- 按cCtrlMFanPrd周期控制风扇，执行转速模式/复位/常规转速控制
        if(event&(1 << eCtrlMFan))  // cCtrlMFanPrd为风扇控制周期宏
        {
            // 根据当前通信风扇的通信类型（bMFCommType），执行不同的风扇控制操作
            switch(bMFCommType[wCommMFId])
            {
                // 情况1：风扇转速模式设置（cMFSpdModeWr）- 配置风扇的转速控制模式（如恒速/温控）
                case cMFSpdModeWr:
                    // 风扇初始化通信使能标志为0xAA（表示初始化完成，可通信）
                    if(bMFInitCommEn[wCommMFId] == 0xAA)
                    {
                        // 发送指令，设置指定ID风扇的转速控制模式
                        sSetMFSpeedMode(wCommMFId);
                    }
                    else
                    {
                        // 初始化未完成，切换通信类型为风扇复位，重置使能标志
                        bMFCommType[wCommMFId] = cMFResetWr;
                        bMFInitCommEn[wCommMFId] = 0xAA;
                    }
                    break;

                // 情况2：风扇复位指令（cMFResetWr）- 风扇故障后，发送复位指令恢复
                case cMFResetWr:
                    if(bMFInitCommEn[wCommMFId] == 0xAA)
                    {
                        // 发送指令，复位指定ID的风扇
                        sMFanReset(wCommMFId);
                    }
                    else
                    {
                        // 复位完成，切换为常规控制模式，重置使能标志
                        bMFCommType[wCommMFId] = cMFNormalCtrl;
                        bMFInitCommEn[wCommMFId] = 0xAA;
                    }
                    break;

                // 情况3：风扇常规控制（cMFNormalCtrl）- 按温控/恒速策略，设置风扇实际转速
                case cMFNormalCtrl:
                    // 发送指令，设置指定ID风扇的当前目标转速
                    sSetMFSpeed(wCommMFId);
                    break;

                // 默认情况：无操作，防止未知通信类型导致的异常
                default:
                    break;
            }

            // 动态更新主风扇最大数量：根据设备单元数（wDevIdNum），更新为实际的风扇数量（单元1/单元2的风扇数）
            if(wDevIdNum == 1)
            {
            	wMFMaxSize = wUnit1FanNum;
            }
            else
            {
            	wMFMaxSize = wUnit2FanNum;
            }

            // 条件编译：非硬件在环测试（HiL_TEST=0）时，执行风扇故障/告警检测
          #if(HiL_TEST == 0)
            // 仅对有效风扇ID（小于最大数量）执行检测
            if(wMFMaxSize > wCommMFId)
            {
            	// 若风扇联合控制使能且风扇异常直接关断使能，执行风扇告警检测
            	if((bFanJointCtrlSw == cEnable) && (bFanAbnormDrtSw == cEnable))
            	{
            		sMFWarnCheck(wCommMFId);
            	}
            	else
            	{
                    // 否则执行风扇故障检测，触发故障保护
                    sMFFaultCheck(wCommMFId);
            	}
            }
          #endif
        }

        // 事件6：处理电表轮询事件（ePollMeter）- 按cPollMeterPrd周期轮询电表，读取电表功率/功率因数等参数
        if(event&(1 << ePollMeter))  // cPollMeterPrd为电表轮询周期宏
        {
            // 仅设备在工作模式（cWorkingMode）时，处理电表通信告警
            if(swGetMode() == cWorkingMode)
            {
                // 若当前无电表通信告警，且电表通信故障标志置位，则设置电表通信告警码
                if((swGetWarningCode() & cMeterCommErr) == 0)
                {
                    if(bMeterCommFaultFlag == true)
                    {
                        sSetWarningCode(cMeterCommErr);
                    }
                }
                else
                {
                    // 若当前有电表通信告警，且电表通信故障标志清除，则清除电表通信告警码
                    if(bMeterCommFaultFlag == false)
                    {
                        sClearWarningCode(cMeterCommErr);
                    }
                }
            }

            // 发送电表轮询指令，读取电表的实时参数（功率/功率因数/累计电量）
            sPollMeter();
        }

        // 事件7：处理BCB轮询事件（ePollBCB）- 仅主设备执行，按cPollBCBPrd周期轮询/设置BCB参数
        if(event&(1 << ePollBCB))  // cPollBCBPrd为BCB轮询周期宏
        {
            // 若BCB参数设置标志置位，执行BCB参数设置
            if(bSetBCBFlag != false)
            {
                sSetBCB();
            }
            else
            {
                // 否则轮询BCB，读取BCB的运行状态和参数
                sPollBCB();
                // BCB设置看门狗计数器自增，满60秒则置位设置标志，重设BCB参数（防止参数丢失）
                if(++wBCBSetWDCnt >= mSciTaskSec2Cnt(60,cPollBCBPrd))  // 60秒转周期数
                {
                    wBCBSetWDCnt = 0;
                    bSetBCBFlag = true;
                }
            }

            // 设备在工作模式时，处理BCB通信告警（逻辑同电表通信告警）
            if(swGetMode() == cWorkingMode)
            {
                if((swGetWarningCode1() & cBCBCommErr) == 0)
                {
                    if(bBCBCommFaultFlag == true)
                    {
                        sSetWarningCode1(cBCBCommErr);
                    }
                }
                else
                {
                    if(bBCBCommFaultFlag == false)
                    {
                        sClearWarningCode1(cBCBCommErr);
                    }
                }
            }

            // 若设备在并网参数测试模式/单机并网测试模式，执行直流漏电流检测（绝缘保护关键步骤）
            if((sbGetTestMode() == cParaOnGrid) || (sbGetTestMode() == cSingleOnGrid))
            {
                sDcLeakCurrCheck();
                //sDcSwChk();  // 直流开关检测函数，注释暂不执行
            }
        }

        // 事件8：处理ADB轮询事件（ePollADB）- 轮询/设置ADB（交流绝缘检测板）参数
        if(event&(1 << ePollADB))
        {
            // 若ADB设置指令标志置位，执行ADB参数设置
            if(bADBSettingCmdFlag != false)
            {
            	sSetADB();
            }
            else
            {
            	// 否则轮询ADB，读取ADB的绝缘检测参数
            	sPollADB();
            	// 若初始化设置成功标志为0x55（初始化完成），则置位ADB设置指令标志，执行参数设置
            	if(swGetInitSetSuccFlag() == 0x55)
            	{
            		bADBSettingCmdFlag = true;
            	}
            }

			// 设备在工作模式时，处理ADB通信告警（逻辑同电表/BCB）
			if(swGetMode() == cWorkingMode)
			{
				if((swGetWarningCode1() & cADBCommErr) == 0)
				{
					if(bADBCommFaultFlag == true)
					{
						sSetWarningCode1(cADBCommErr);
					}
				}
				else
				{
					if(bADBCommFaultFlag == false)
					{
						sClearWarningCode1(cADBCommErr);
					}
				}
			}
        }

        // 事件9：处理SCI定时器事件（eSciTimer）- 核心定时事件，100ms周期触发，管理485总线、800ms/1s定时器、1s周期任务
        // 注释：SCIA/SCIB的串口分配因板卡不同而差异：
        // 非cTNPC板卡：SCIA用于DSP-BCB通信，SCIB用于DSP-电表；cTNPC板卡：SCIA未用，SCIB复用为DSP-BCB/风扇/电表通信
        if(event&(1 << eSciTimer))
        {
            // 条件编译：非cTNPC板卡，定义SCIA发送忙计数器
          #if(BOARD_TYPE != cTNPC)
            static INT16U wSciATxBuzyCnt = 0;
          #endif
            // 定义SCIB发送忙计数器：所有板卡均使用，检测SCIB发送是否超时
            static INT16U wSciBTxBuzyCnt = 0;

            // 800ms定时器自增：满8次（8*100ms）为800ms，后重置
            if(++wSciTask800msTimer >= 8)  // 800ms
            {
                wSciTask800msTimer = 0;
            }

            // 1s定时器自增：满10次（10*100ms）为1s，后重置
            if(++wSciTask1sTimer >= 10)  // 1s
            {
                wSciTask1sTimer = 0;
            }

            // 并网测试模式下，执行直流开关检测，检查直流开关的状态是否正常
            if((sbGetTestMode() == cParaOnGrid) || (sbGetTestMode() == cSingleOnGrid))
    		{
    			sDcSwChk();
    		}

            /********************* SCIA总线管理开始 *********************/
          #if(BOARD_TYPE != cTNPC)  // 非cTNPC板卡执行SCIA管理
            // 检测SCIA串口发送状态：若为发送忙（cSciTxBusy），则启动超时计数
            if(sbGetSciTxStatus(cSciANum) == cSciTxBusy)
            {
                // 发送忙计数器自增，满50次（50*100ms=5s）表示发送超时
                if(++wSciATxBuzyCnt >= 50)  // 5s超时
                {
                    wSciATxBuzyCnt = 0;
                    mSCIA_PC485_RD;  // 强制将SCIA切换为接收模式
                    sClearSciBuff(cSciANum);  // 清除SCIA的发送/接收缓冲区，复位串口
                }
            }
            else
            {
                // 发送完成，重置超时计数器
                wSciATxBuzyCnt = 0;
            }

            // 主设备处理从站DSP的接收缓冲区（注释为原逻辑，实际直接执行）
            {
                // 800ms定时器为1时（100ms），执行以下操作
                if(wSciTask800msTimer == 1)
                {
                    // 若设备单元数为1，且交流绝缘单元使能标志为0xAA，处理ADB的接收缓冲区数据
                	if(wDevIdNum == 1)
                	{
                    	if(swGetAcIsuEnableFalg() == 0xAA)
                    	{
                    		sHandleADBRxBuf();
                    	}
                	}

                	// 若无分支升级标志，发送ePollBCB事件，800ms轮询一次BCB
                	if(bBranchIapFlag == false)
                	{
                		OSEventSend(cPrioSixth,ePollBCB);  // 800ms周期
                	}
                }
                // 800ms定时器为5时（500ms），执行以下操作
                else if(wSciTask800msTimer == 5)
                {
                	// 若无分支升级标志，处理BCB的接收缓冲区数据
                	if(bBranchIapFlag == false)
                	{
                		sHandleBCBRxBuf();
                	}

                	// 若设备单元数为1，且交流绝缘单元使能，发送ePollADB事件，800ms轮询一次ADB
                	if(wDevIdNum == 1)
                	{
                        if(swGetAcIsuEnableFalg() == 0xAA)
                        {
                        	OSEventSend(cPrioSixth,ePollADB);  // 800ms周期
                        }
                	}
                }

                // 若交流绝缘单元使能标志为0x55（未使能），初始化绝缘检测并清除ADB通信告警
                if(swGetAcIsuEnableFalg() == 0x55)
                {
                	sInitAcIsuCheck();
                	sClearWarningCode1(cADBCommErr);
                	bAcIsuFaultFlag = false;
                }
            }
          #endif
            /********************** SCIA总线管理结束 **********************/

            /********************* SCIB总线管理开始 **********************/
            // 检测SCIB串口发送状态，超时处理逻辑同SCIA（5s超时，强制切接收+清缓冲区）
            if(sbGetSciTxStatus(cSciBNum) == cSciTxBusy)
            {
                if(++wSciBTxBuzyCnt >= 50)  // 5s超时
                {
                    wSciBTxBuzyCnt = 0;
                    mSCIB_PC485_RD;
                    sClearSciBuff(cSciBNum);
                }
            }
            else
            {
                wSciBTxBuzyCnt = 0;
            }

            // 条件编译：cTNPC板卡的SCIB总线管理逻辑（SCIB复用为BCB/风扇/电表通信）
          #if(BOARD_TYPE == cTNPC)
            // 800ms定时器为0/4时（0ms/400ms），执行以下操作
            if((wSciTask800msTimer % 4) == 0)
            {
                // 处理当前通信风扇的接收缓冲区数据
                sHandleMFanRxBuf(wCommMFId);
                // 检测风扇类型是否变更：若当前类型与历史值不同，重新初始化风扇参数
                if(bMFanTypePre != sbGetMainFanType())
                {
                    bMFanTypePre = sbGetMainFanType();
                    sMFParasInit();
                }

                // 若电表板使能，发送ePollMeter事件，400ms轮询一次电表
                if(unDspCtlSwReg.bit.wMeterSw == cMeterBoard)
                {
                	OSEventSend(cPrioSixth,ePollMeter);  // 400ms周期
                }
            }
            // 800ms定时器为1时（100ms），执行以下操作
            else if(wSciTask800msTimer == 1)
            {
            	// 若电表板使能，处理电表的接收缓冲区数据
            	if(unDspCtlSwReg.bit.wMeterSw == cMeterBoard)
            	{
                    sHandleMeterRxBuf();
            	}
                // 非cTNPC板卡的风扇ID自增逻辑（此处注释，因cTNPC板卡单独处理）
                #if(BOARD_TYPE != cTNPC)
                if(++wCommMFId >= cMFSize)
                {
                    wCommMFId = cMF1Id;
                }
                #endif
                // 发送eCtrlMFan事件，800ms控制一次风扇
                OSEventSend(cPrioSixth,eCtrlMFan);  // 800ms周期
            }
            // 800ms定时器为2/6时（200ms/600ms），执行以下操作
            else if((wSciTask800msTimer % 4) == 2)
            {
                // 处理当前通信风扇的接收缓冲区数据
                sHandleMFanRxBuf(wCommMFId);
                // 检测风扇类型变更，变更则重新初始化
                if(bMFanTypePre != sbGetMainFanType())
                {
                    bMFanTypePre = sbGetMainFanType();
                    sMFParasInit();
                }
                // 若无分支升级标志，发送ePollBCB事件，800ms轮询一次BCB
                if(bBranchIapFlag == false)
                {
                     OSEventSend(cPrioSixth,ePollBCB);  // 800ms周期
                }
            }
            // 800ms定时器为5时（500ms），执行以下操作
            else if(wSciTask800msTimer == 5)
            {
            	// 若电表板使能，处理电表的接收缓冲区数据
            	if(unDspCtlSwReg.bit.wMeterSw == cMeterBoard)
            	{
            		sHandleMeterRxBuf();
            	}
                // 发送ePollMFan事件，800ms轮询一次风扇
                OSEventSend(cPrioSixth,ePollMFan);  // 800ms周期
            }

          // 条件编译：非cTNPC板卡的SCIB总线管理逻辑（SCIB专用于风扇/电表通信）
          #else
            // 800ms定时器为0/4时，处理风扇接收缓冲区+400ms轮询电表
            if((wSciTask800msTimer % 4) == 0)
            {
                sHandleMFanRxBuf(wCommMFId);
                if(bMFanTypePre != sbGetMainFanType())
                {
                    bMFanTypePre = sbGetMainFanType();
                    sMFParasInit();
                }
                if(unDspCtlSwReg.bit.wMeterSw == cMeterBoard)
                {
                	OSEventSend(cPrioSixth,ePollMeter);  // 400ms周期
                }
            }
            // 800ms定时器为2时，处理电表接收缓冲区+风扇ID自增+800ms控制风扇
            else if(wSciTask800msTimer == 2)
            {
            	if(unDspCtlSwReg.bit.wMeterSw == cMeterBoard)
            	{
                    sHandleMeterRxBuf();
            	}
                // 风扇ID自增，轮询所有风扇；若达到最大数量，重置为第一路风扇
                if(++wCommMFId >= cMFSize)
                {
                    wCommMFId = cMF1Id;
                }
                OSEventSend(cPrioSixth,eCtrlMFan);  // 800ms周期
            }
            // 800ms定时器为6时，处理电表接收缓冲区+800ms轮询风扇
            else if(wSciTask800msTimer == 6)
            {
            	if(unDspCtlSwReg.bit.wMeterSw == cMeterBoard)
            	{
            		sHandleMeterRxBuf();
            	}
                OSEventSend(cPrioSixth,ePollMFan);  // 800ms周期
            }
          #endif
            /********************** SCIB总线管理结束 **********************/

            // 1s定时器为0时（整1秒），执行1秒周期的核心任务：负极接地管理/交流绝缘检测/绝缘电阻更新
            if(wSciTask1sTimer == 0)
            {
            	// 仅主设备执行负极接地管理
            	if(psVarM2s->unModuleId.bit.FuncRole == cMaster)
            	{
                    // 若负极接地为自动模式，或PID功率模式有效，执行负极接地调度（绝缘保护核心）
                    if((sbGetNeg2GndType() == cAutoNeg2GndType) || (swGetPIDPowerMode() != cPIDInvalid))
                    {
                        sNegPoleSche();
                    }
                    else
                    {
                        // 否则执行无绝缘模块处理，清除绝缘相关状态
                        sNoneInsModu();
                        // 若为非负极接地模式，清除GFDI（接地故障检测）保护故障码
                        if(sbGetNeg2GndType() == cNotNeg2GndType)
                        {
                            if((swGetFaultCode2() & cGFDIProtectFault) != 0)
                            {
                                sClearFaultCode2(cGFDIProtectFault);
                            }
                        }
                    }
            	}
            	// 从设备执行：接收主设备的负极接地指令，同步执行
            	else
            	{
            	    eNegPoleCmd = (NEG_POLE_CMD)unCtrlCmdSec1Self.bit.NegPoleCmd;
            	}
				
              // 非cTNPC板卡，单元1且交流绝缘单元使能时，执行交流绝缘检测和故障检查
              #if(BOARD_TYPE != cTNPC)
                if(wDevIdNum == 1)
                {
                    if(swGetAcIsuEnableFalg() == 0xAA)
                    {
                    	sAcIsuDetPole();
                    	sAcIsuFaultCheck();
                    }
                }
			  #endif

                // 更新绝缘电阻值：重新计算直流侧绝缘电阻，为绝缘保护提供数据
                sInsResUpdate();
                // 绝缘电阻检查：判断绝缘电阻是否低于阈值，低于则触发绝缘告警/故障
                sInsResCheck();
            }
            // 1s定时器为3时（300ms），执行数据记录+设备运行时长统计
            else if(wSciTask1sTimer == 3)
            {
                // 数据记录：每秒检查并记录发电量、运行状态、故障信息等
                sDataRec();  // 每秒执行一次

                // 仅设备在工作模式时，统计运行时长（分/时）
                if(swGetMode() == cWorkingMode)
                {
                    // 秒数自增，满60秒则分钟数+1，秒数重置
                    if(++stLifeTime.wSecond >= 60)  // 60s
                    {
                        stLifeTime.wSecond = 0;
                        // 分钟数自增，满60分钟则小时数+1，分钟数重置
                        if(++stLifeTime.wMinute >= 60)  // 60min
                        {
                            stLifeTime.wMinute = 0;
                            stLifeTime.dwHour++;
                        }
                    }
                    // 每30分钟，将累计运行小时数+1写入FRAM（掉电保存，防止数据丢失）
                    if((stLifeTime.wMinute == 30)&&(stLifeTime.wSecond == 0))
                    {
                        sbFramWriteDWord(cFramSpiId,cRunHourAddr,stLifeTime.dwHour+1);
                    }
                }
            }
            // 1s定时器为8时（800ms），执行DSP上电延时+设备上电时长统计+参数异常计数清零
            else if(wSciTask1sTimer == 8)
            {
                // DSP上电延时处理：上电后延时cDspPowerOnDly周期，再关闭上电标志，允许并联控制
                if(bDspPowerOn == true)
                {
                    if(++wDspPowerOnDlyCnt >= cDspPowerOnDly)
                    {
                        wDspPowerOnDlyCnt = 0;
                        bDspPowerOn = false;
                    }
                }
                else
                {
                    // 设备上电时长统计：秒数自增，满60秒则分钟数+1，秒数重置
                    if(++stPowerOnTime.wSecond >= 60)
                    {
                        stPowerOnTime.wSecond = 0;
                        // 清零参数异常计数器，重置异常统计
                        sZeroParaAbnormCnt();
                        // 分钟数自增，满60分钟则小时数+1，分钟数重置
                        if(++stPowerOnTime.wMinute >= 60)
                        {
                            stPowerOnTime.wMinute = 0;
                            stPowerOnTime.dwHour++;
                            // 每20小时，执行参数运行顺序变更，防止主DSP长期运行不休眠导致的资源泄漏
                            if((stPowerOnTime.dwHour % 20) == 0)
                            {
                                sParaRunOrderChange();
                            }
                        }
                    }
                }
                // sBubbleSortArray();  // 冒泡排序函数，注释暂不执行
            }
        }
    }
}


