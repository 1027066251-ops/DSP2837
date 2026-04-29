



//interrupt_cpu01.c 是 CPU1 的“中断控制中心”，负责配置 PIE、中断向量表、中断使能、以及实现所有 CPU1 侧的 ISR（中断服务程序）。“CPU1 的全部中断逻辑都在这里实现”。



// 定义全局中断宏，用于标识本文件为中断相关全局变量/函数的定义文件，通常配合头文件的条件编译使用
#define INTERRUPT_GLOBALS


/******************************************************************************
                                   INCLUDES
******************************************************************************/
// 应用层总头文件，包含系统基础宏定义、数据类型、全局枚举等核心内容
#include "App\include\head.h"
// F2837xD系列DSP的IPC（处理器间通信）驱动库头文件，用于多核间数据交互
#include "F2837xD_Ipc_drivers.h"
// CAN总线FIFO驱动头文件，实现CAN数据的先进先出缓存管理
#include "Driver\Can\CanFIFO\CanFIFO.h"
// CANOpen协议驱动头文件，实现CANOpen工业总线协议的相关功能
#include "Driver\Can\CanOpen\CanOpen.h"
// SCI（串行通信接口）驱动头文件，实现UART串行通信的收发功能
#include "Driver\Sci\SciDriver.h"


/******************************************************************************
                          PREPROCESSING INSTRUCTIONS
******************************************************************************/
// 编译器伪指令：将EPWM_Prd_isr_copy函数的代码段分配到"ramfuncs"RAM函数区，提升中断函数执行速度（RAM比Flash快）
#pragma CODE_SECTION(EPWM_Prd_isr_copy,"ramfuncs");
// 编译器伪指令：注释掉-将EPWM_Udf_isr_copy函数分配到RAM函数区，暂未启用该中断函数的RAM执行优化
//#pragma CODE_SECTION(EPWM_Udf_isr_copy,"ramfuncs");
// 编译器伪指令：注释掉-将多核IPC0中断处理函数分配到RAM函数区，暂未启用该多核通信中断的RAM执行优化
//#pragma CODE_SECTION(sCPU02toCPU01IPC0IntHandler,"ramfuncs");
// 编译器伪指令：将CANB总线接收消息中断函数sCanbMsgRxIsr分配到RAM函数区，提升CAN接收中断响应速度
#pragma CODE_SECTION(sCanbMsgRxIsr,"ramfuncs");
// 编译器伪指令：将交流数据拷贝函数sAcDataCopy分配到RAM函数区，提升数据拷贝效率
#pragma CODE_SECTION(sAcDataCopy,"ramfuncs");
// 编译器伪指令：将直流数据拷贝函数sDcDataCopy分配到RAM函数区，提升数据拷贝效率
#pragma CODE_SECTION(sDcDataCopy,"ramfuncs");
// 编译器伪指令：将软件数据接收处理函数swDataRecHdle分配到RAM函数区，提升软件数据处理速度
#pragma CODE_SECTION(swDataRecHdle,"ramfuncs");


/******************************************************************************
                                  DEFINITIONS
******************************************************************************/
// 定义100ms PWM中断计数变量（注释掉）：采样频率cSamplFrq乘以0.1秒，得到100ms对应的中断次数，暂未启用
//INT16U cwPwmIsr100ms = (INT16U)((float)cSamplFrq * 0.1);

///////***9.6KHz采样频率对应的25Hz低通滤波器系数***///////
/*
#define  cLFCoeffK1  0.9838  // 25Hz低通滤波器的系数K1，9.6KHz采样率适配
#define  cLFCoeffK2  0.0162  // 25Hz低通滤波器的系数K2，9.6KHz采样率适配，满足K1+K2≈1
*/

///////***7.2KHz采样频率对应的25Hz低通滤波器系数***///////
/*
#define  cLFCoeffK1  0.9784   // 25Hz低通滤波器的系数K1，7.2KHz采样率适配
#define  cLFCoeffK2  0.0216   // 25Hz低通滤波器的系数K2，7.2KHz采样率适配，满足K1+K2≈1
*/

///////***6.0KHz采样频率对应的25Hz低通滤波器系数***//////
/*
#define  cLFCoeffK1  0.9745   // 25Hz低通滤波器的系数K1，6.0KHz采样率适配
#define  cLFCoeffK2  0.0255   // 25Hz低通滤波器的系数K2，6.0KHz采样率适配，满足K1+K2≈1
*/

///////***5.4KHz采样频率对应的25Hz低通滤波器系数***//////
/*
#define  cLFCoeffK1  0.97174   // 25Hz低通滤波器的系数K1，5.4KHz采样率适配
#define  cLFCoeffK2  0.02826   // 25Hz低通滤波器的系数K2，5.4KHz采样率适配，满足K1+K2≈1
*/


///////***4.8KHz采样频率对应的25Hz低通滤波器系数***//////
/*
#define  cLFCoeffK1  0.9683   // 25Hz低通滤波器的系数K1，4.8KHz采样率适配
#define  cLFCoeffK2  0.0317   // 25Hz低通滤波器的系数K2，4.8KHz采样率适配，满足K1+K2≈1
*/

// 定义频率保护的移动窗口大小为200，用于电网频率的滑动平均计算，提升频率检测的稳定性
#define cFreMovWndSize    200

// 定义数据接收缓冲区的长度为10，用于交流/直流数据的接收缓存，做临时数据存储
#define cRecBufLen    10


/******************************************************************************
                                    TYPES
******************************************************************************/
// 定义DSP状态机联合类型：采用位域结构，16位无符号整型拆分为多个功能位，节省内存且方便位操作
typedef union {
    INT16U word;  // 整个16位的无符号整型值，用于整体操作状态机数据
    // 位域结构：按位分配功能，明确每一位的含义，bit0为最低位，bit15为最高位
    struct {
        INT16U CpldPwm      :4;  // bit0~3：CPLD的PWM相关状态/控制位，4位表示
        INT16U Rsvd1        :4;  // bit4~7：保留位，暂未使用，占4位
        INT16U DcSwSts      :2;  // bit8~9：直流开关状态位，2位表示（可区分4种状态）
        INT16U DcSwCmd      :2;  // bit10~11：直流开关控制指令位，2位表示（可下发4种指令）
        INT16U AcBrkerSts   :1;  // bit12：交流断路器状态位，1位（0=断开，1=闭合）
        INT16U MainRlySts   :1;  // bit13：主继电器状态位，1位（0=断开，1=闭合）
        INT16U AuxiRlyCmd   :1;  // bit14：辅助继电器控制指令位，1位（0=断开，1=闭合）
        INT16U MainRlyCmd   :1;  // bit15：主继电器控制指令位，1位（0=断开，1=闭合）
    } bit;  // 位域操作标识，用于单独操作某一位（如unDspFSM.bit.MainRlyCmd = 1;）
} DSP_FSM;  // 联合类型名：DSP状态机，命名为DSP_FSM


/******************************************************************************
                                 DECLARATIONS
******************************************************************************/
// 正弦波指针偏移量，整型有符号：用于正弦波查表的索引，控制正弦波相位和幅值输出
INT16S wSinpointer = 0;

// 相位实时切换标志位，整型有符号：0=未切换，非0=需要切换相位，用于相位调整控制
INT16S wPhaseRTChangeFlag = 0;

// 输出功率计算周期计数器，整型有符号：用于累计中断次数，达到指定值后执行功率计算
INT16S wOutputWattCalPrdCnt = 0;

// 模块故障计数器，整型有符号：累计模块故障的中断次数，用于故障防抖和判定
INT16S wModuleFaultCnt = 0;
// 输出继电器故障计数器，整型有符号：累计输出继电器故障的中断次数，故障防抖
INT16S wOutputRlyFaultCnt = 0;
// 交流开关操作故障计数器，整型有符号：累计交流开关动作故障的中断次数，故障防抖
INT16S wAcSwchOpFaultCnt = 0;
// 模块永久故障计数器，整型无符号：持续累计模块故障次数，用于故障统计和历史记录
INT16U wModuleFaultForeverCnt = 0;

/////频率保护相关变量定义区///////////
// 频率计数周期，整型无符号：累计中断次数，用于电网频率的周期检测
INT16U wFreCntCycle = 0;
// 频率移动窗口计数器，整型无符号：频率移动窗口的索引计数器，控制窗口数据的更新
INT16U wFreMovWindowCnt = 0;
// 频率移动窗口有效数据个数，整型无符号：记录移动窗口中实际存入的频率数据数量
INT16U wFreMovWindowNum = 0;
// 频率移动窗口半数阈值，整型无符号：cFreMovWndSize/2，用于窗口数据的半满判定
INT16U wFreqWindowNumHalf = 0;
// 频率保护移动窗口数组，浮点型：存储最近200个电网频率采样值，用于滑动平均计算
float  fFreMovWindowGrp[cFreMovWndSize];   // frecunency protection（原英文注释：频率保护）
// 频率移动窗口数据和，浮点型：实时累计fFreMovWindowGrp数组的所有元素和，用于快速计算平均值
float  fFreMovGrpSum = 0;
// 上一时刻频率移动窗口数据和，浮点型：保存上一次的数组和，用于差分更新平均值，提升计算效率
float  fFreMovGrpSum_1 = 0;
// 当前频率移动窗口计算值，浮点型：当前滑动平均后的电网频率值，用于频率保护判定
float  fFreMovWindowNow = 0;
// 频率移动窗口和增益系数，浮点型：用于频率和的比例缩放，适配不同采样率
float  fFreMovGrpSumGain;
// 电网频率累加和，浮点型：临时累计电网频率采样值，用于辅助计算
float  fFgSum = 0;
// 电网频率累加和临时变量，浮点型：fFgSum的缓存变量，防止计算过程中数据被覆盖
float  fFgSumTemp = 0;
// 电网频率最终值，浮点型：经过滑动平均后的电网频率有效值，供系统主逻辑使用
float  fGridFreq;

// 控制标志位上一状态，无符号字符型：保存上一时刻的控制标志状态，用于状态变化检测（false=0，true=1）
INT8U  bCtlFlagPre = false;
// 故障记录标志位，自定义枚举类型：存储各类故障的记录/清除标志，用于故障管理
FLT_REC_FLAG unFltRecFlag;
// 故障记录清除计数器，整型无符号：累计中断次数，达到指定值后清除故障记录，实现故障自动清除
INT16U wFaultRecClrCnt = 0;

// 交流数据接收延时计数器，整型无符号：控制交流数据接收的延时，防止数据接收过快导致溢出
INT16U wAcDataRecDly = 0;
// 直流数据接收延时计数器，整型无符号：控制直流数据接收的延时，防止数据接收过快导致溢出
INT16U wDcDataRecDly = 0;

// 交流数据接收缓冲区，整型有符号数组：长度10，临时存储交流采样数据，用于数据滤波/缓存
INT16S wAcDataRecBuffer[cRecBufLen];
// 直流数据接收缓冲区，整型有符号数组：长度10，临时存储直流采样数据，用于数据滤波/缓存
INT16S wDcDataRecBuffer[cRecBufLen];

// 数据计算临时结构体，自定义DATA_PROCESS类型：存储数据处理过程中的临时中间值，防止覆盖有效数据
DATA_PROCESS stDataCalcTemp;
// 数据计算主结构体，自定义DATA_PROCESS类型：存储数据处理后的最终有效结果，供系统主逻辑调用
DATA_PROCESS stDataCalc;
// 数据计算临时结构体指针，自定义DATA_PROCESS_HANDLE类型：指向stDataCalcTemp，方便函数传参和结构体操作
DATA_PROCESS_HANDLE pstDataCalcTemp = &stDataCalcTemp;
// 数据计算主结构体指针，自定义DATA_PROCESS_HANDLE类型：指向stDataCalc，方便函数传参和结构体操作
DATA_PROCESS_HANDLE pstDataCalc = &stDataCalc;

// CANA总线故障标志位，无符号字符型：0=CANA正常，1=CANA故障，用于CAN总线状态检测
INT8U bCanaFailFlag;

// R相电压偏移采样和，浮点型：累计R相电网电压的偏移采样值，用于电压偏移校准
float  fROffsetSampleSum;
// S相电压偏移采样和，浮点型：累计S相电网电压的偏移采样值，用于电压偏移校准
float  fSOffsetSampleSum;
// 采样和校验值，浮点型：校验fROffsetSampleSum和fSOffsetSampleSum的计算正确性，防止数据异常
float  fSumCheck;
// 电网电压偏移校准计数器，整型无符号：累计中断次数，达到指定值后执行电压偏移校准计算
INT16U wGridVoltOffsetCnt;

// 输出电流保护计数器，整型无符号：累计输出电流超标的中断次数，用于过流故障防抖和判定
INT16U wOutCurrProtCnt = 0;
// 输出电压保护计数器，整型无符号：累计输出电压超标的中断次数，用于过压/欠压故障防抖和判定
INT16U wOutVoltProtCnt = 0;
// 输出电压保护清除计数器，整型无符号：累计电压恢复正常的中断次数，达到指定值后清除过压/欠压故障
INT16U wOutVoltClrCnt = 0;

// 傅里叶变换缓冲区计数器，整型无符号：控制傅里叶变换（DFT）数据缓冲区的索引，用于数据采集和变换计算
INT16U wDftBuffCnt = 0;

// 电网跌落标志位，无符号字符型：0=电网正常，1=电网跌落（电压骤降），用于电网异常检测
INT8U  bGridDropFlag = false;
// 电网跌落滤波计数器，整型无符号：电网跌落判定的滤波计数器，实现故障防抖，防止误判
INT16U wGridDropFilter = 0;
// 频率检测屏蔽延时计数器，整型无符号：电网跌落后屏蔽频率检测的延时，防止频率检测异常
INT16U wFreqDetMaskDly = 0;

// 零点跌落标志位，无符号字符型：0=零点正常，1=零点跌落（电压为零），用于电网零点检测
INT8U  bZeroDropFlag = false;
// 零点跌落滤波计数器，整型无符号：零点跌落判定的滤波计数器，故障防抖
INT16U wZeroDropFilter = 0;
// 零点跌落恢复延时计数器，整型无符号：零点恢复后延时一段时间再清除标志，防止反复切换
INT16U wZeroDropBackDly = 0;
// 锁相环丢失标志位，无符号字符型：0=PLL锁相正常，1=PLL失锁，用于锁相环状态检测
INT8U  bPllLossFlag = false;
// 锁相环丢失恢复延时计数器，整型无符号：PLL重新锁相后延时清除失锁标志，防止反复切换
INT16U wPllLossBackDly = 0;

// 高压跌落标志位，无符号字符型：0=高压正常，1=高压跌落，用于高压侧电网异常检测
INT8U  bHvrtDropFlag = false;
// 高压跌落滤波计数器，整型无符号：高压跌落判定的滤波计数器，故障防抖
INT16U wHvrtDropFilter = 0;
// 高压跌落恢复延时计数器，整型无符号：高压恢复后延时清除跌落标志，防止反复切换
INT16U wHvrtDropBackDly = 0;

// 实际电压保护延时计数器，整型无符号：实际电压超标后的延时计数器，达到指定值后触发保护，防止瞬时波动误判
INT16U wVoltRealProtDlyCnt = 0;

// 25Hz低通滤波器系数K1，浮点型：动态计算版，适配任意采样频率cSamplFrq，公式为K1=1/(1+2π*25/cSamplFrq)
float fLFCoeffK1 = 1.0 / (1.0 + (PI2*25.0/(float)cSamplFrq));         //25Hz：低通滤波器截止频率25Hz
// 25Hz低通滤波器系数K2，浮点型：动态计算版，适配任意采样频率cSamplFrq，PI2_RCP为2π的倒数
float fLFCoeffK2 = 1.0 / (1.0 + (PI2_RCP*(float)cSamplFrq/25.0));

// 启动同步成功计数器，整型无符号：累计启动同步成功的中断次数，达到指定值后判定同步完成
INT16U wStartUpSycOkCnt = 0;
// 启动同步成功标志位，无符号字符型：0=未同步，1=同步完成，用于启动流程的状态判定
INT8U  bStartUpSycOkFlag = false;
// 主继电器吸合成功标志位，无符号字符型：0=未吸合，1=吸合成功，用于主继电器状态判定
INT8U  bMainRlyOnOk = false;
// 主继电器吸合计数器，整型无符号：累计主继电器吸合指令下发后的中断次数，用于吸合延时判定
INT16U wMainRlyOnCnt = 0;

// PM（功率模块）故障锁定码，整型无符号：存储功率模块的故障代码，用于故障定位和锁定
INT16U wPMFaultLockCode;

// 母线正电压值，浮点型：存储直流母线正端的电压采样值，用于母线电压检测和保护
float fBusVoltPos;
// 母线负电压值，浮点型：存储直流母线负端的电压采样值，用于母线电压检测和保护
float fBusVoltNeg;

// 中间故障计数器，整型无符号（注释掉）：暂未启用的故障计数器，用于预留的中间环节故障检测
//static INT16U wMidFaultCnt = 0;
// 直流电压保护计数器，静态整型无符号：仅在本文件内有效，累计直流电压超标的中断次数，用于直流过压/欠压保护
static INT16U wDcVoltProtCnt = 0;

// 负母线电压采样和，浮点型：累计负母线电压的采样值，用于负母线电压的平均值计算
float  fVinvNegSum;
// 负母线电压采样计数器，整型无符号：累计负母线电压的采样次数，用于平均值计算（平均值=和/次数）
INT16U wVinvNegCnt = 0;

// 一个正弦波周期的采样点数，整型无符号：采样频率cSamplFrq除以50Hz（工频），得到一个正弦波的采样点数量
INT16U wOneSinePrdPoint = (INT16U)(cSamplFrq/50);

// IGBT温度采样计数器，整型有符号：累计中断次数，控制IGBT温度的采样周期
INT16S wIgbtTempSamplCnt = 0;
// IGBT温度采样周期，整型有符号：采样频率cSamplFrq，即1秒采样一次IGBT温度
INT16S wIgbtTempSamplCntPrd = cSamplFrq;
// IGBT温度采样选择位，整型有符号：选择不同的IGBT温度传感器通道，用于多路温度采样
INT16S wIgbtTempSamplSel = 1;
// IGBT温度计算请求标志，整型有符号：0=无需计算，非0=需要计算IGBT温度，触发温度处理逻辑
INT16S wIgbtTptCalcReq = false;
// IGBT温度故障屏蔽标志，整型有符号：0=不屏蔽，非0=屏蔽IGBT温度故障，用于调试或特殊工况
INT16S wIgbtTempFltMsk = true;
// IGBT温度故障延时计数器，整型有符号：IGBT温度超标的延时计数器，故障防抖
INT16S wIgbtTempFltDlyCnt = 0;
// IGBT温度故障延时周期，整型有符号：故障延时判定的阈值，累计10次中断后判定为真实故障
INT16S wIgbtTempFltDlyCntPrd = 10;

// IGBT A1通道温度原始采样值，32位有符号整型：存储未处理的A1相IGBT温度采样值，高精度存储
INT32U dwIgbtA1TptRaw = 0;
// IGBT A2通道温度原始采样值，32位有符号整型：存储未处理的A2相IGBT温度采样值
INT32U dwIgbtA2TptRaw = 0;
// IGBT B1通道温度原始采样值，32位有符号整型：存储未处理的B1相IGBT温度采样值
INT32U dwIgbtB1TptRaw = 0;
// IGBT B2通道温度原始采样值，32位有符号整型：存储未处理的B2相IGBT温度采样值
INT32U dwIgbtB2TptRaw = 0;
// IGBT C1通道温度原始采样值，32位有符号整型：存储未处理的C1相IGBT温度采样值
INT32U dwIgbtC1TptRaw = 0;
// IGBT C2通道温度原始采样值，32位有符号整型：存储未处理的C2相IGBT温度采样值
INT32U dwIgbtC2TptRaw = 0;

// 逆变器电压与电网电压相等滤波计数器，整型无符号：判定逆变器电压和电网电压相等的滤波计数器，防抖
INT16U wVinvEqualVgFilter = 0;
// 逆变器电压与电网电压相等标志位，无符号字符型：0=不相等，1=相等，用于并网合闸判定
INT8U  bVinvEqualVgFlag = false;

// PWM输出禁止延时计数器，整型无符号：下发PWM禁止指令后的延时计数器，延时后真正关闭PWM，防止瞬时波动
INT16U wPwmDisDlyCnt = 0;

// DSP状态机实例化，自定义DSP_FSM类型：创建状态机变量，用于存储和操作DSP的各类状态/控制位（Liangc20180111：开发者标注+日期）
DSP_FSM unDspFSM;  // Liangc20180111
// 运行状态寄存器实例化，自定义RUN_INFO_REG类型：存储系统的整体运行状态，如运行/停机/故障等
RUN_INFO_REG unRunStsReg;

// 声明外部自动重启标志位，无符号字符型：该变量在其他文件定义，本文件可使用，0=不自动重启，1=故障后自动重启
extern INT8U  bAutoRestart;

// Liangc20160628 test begin：开发者标注+日期，测试代码开始
// 电压记录类型标志位，无符号字符型：存储电压记录的类型，cNoneRec为无记录，自定义枚举值，用于电压数据记录分类
INT8U bVrtRecType = cNoneRec;
// Liangc20160628 test end：开发者标注+日期，测试代码结束

// 输入功率跳闸计数器，整型无符号：累计输入功率超标的中断次数，达到指定值后触发跳闸保护
INT16U wInPowerTripCnt = 0;
// 输出电压超级保护计数器，整型无符号：更高优先级的输出电压保护计数器，用于严重过压/欠压的快速保护
INT16U wOutVoltProtSuperCnt = 0;
// 上一次电压超级保护计数器值，整型无符号：保存上一时刻的超级保护计数器值，用于状态变化检测
INT16U wVoltProtLastSuperCnt = 0;
// 电压超级保护清除计数器，整型无符号：电压恢复正常后，累计次数用于清除超级保护状态
INT16U wVoltProtClearSuperCnt = 0;
// 高压跳闸标志位，整型无符号：0=未跳闸，1=跳闸，用于高压过压后的跳闸状态判定
INT16U wVoltHighTripFlag = false;

// 开路保护电流值，整型无符号：存储开路故障判定的电流阈值，用于开路故障检测
INT16U wOpnLpProCurr;

// 平均值计算计数器，整型无符号：累计中断次数，达到指定值后执行三相电压/电流的平均值计算
INT16U wAvgCalCnt = 0;
// A相电流数组，浮点型：长度cOrder_N，存储A相电流的各次谐波分量，用于谐波分析/计算
float fAphaseCurr[cOrder_N];
// B相电流数组，浮点型：长度cOrder_N，存储B相电流的各次谐波分量
float fBphaseCurr[cOrder_N];
// C相电流数组，浮点型：长度cOrder_N，存储C相电流的各次谐波分量
float fCphaseCurr[cOrder_N];
// A相电压临时值，浮点型：存储A相电压的临时采样值，用于平均值/谐波计算
float fAphaseVoltTemp = 0;
// B相电压临时值，浮点型：存储B相电压的临时采样值
float fBphaseVoltTemp = 0;
// C相电压临时值，浮点型：存储C相电压的临时采样值
float fCphaseVoltTemp = 0;
// A相电流临时值，浮点型：存储A相电流的临时采样值
float fAphaseCurrTemp = 0;
// B相电流临时值，浮点型：存储B相电流的临时采样值
float fBphaseCurrTemp = 0;
// C相电流临时值，浮点型：存储C相电流的临时采样值
float fCphaseCurrTemp = 0;

// 有功功率值，32位有符号整型：存储系统计算后的有功功率，高精度存储（单位通常为W/0.1W）
INT32S dwActivePwr = 0;
// 无功功率值，32位有符号整型：存储系统计算后的无功功率，高精度存储（单位通常为Var/0.1Var）
INT32S dwReactivePwr = 0;

// 20毫秒计数器，整型无符号：累计中断次数，达到指定值后触发20ms周期的任务（如数据上报、状态刷新）
INT16U w20msCnt = 0;
// 功率中断管理标志位，无符号字符型：0=未触发，1=触发，用于管理功率计算相关的中断任务
INT8U bPwrIsrMngFlg;
// 中断周期计数器，整型无符号：累计PWM中断的总次数，用于系统周期任务的调度
INT16U wIsrCycleCnt = 0;
// 中断周期计数器满标志位，无符号字符型：0=未满，1=已满，计数器达到阈值后置1，触发周期任务
INT8U bIsrCycleCntFullFlg = false;

// 毫秒滴答计数器，整型无符号：系统毫秒级计时计数器，用于各类延时、定时任务的基础计时
INT16U wMsTickCnt = 0;


/******************************************************************************
                                  FUNCTIONS
******************************************************************************/
// 定时器2硬件中断服务函数，中断触发周期为500微秒
interrupt void Timer2Isr(void)	// 500us
{
	/***************Milli-second tick begin****************/
	// 毫秒滴答计数器自增1，判断是否累计达到2次（500us*2=1毫秒）
	if(++wMsTickCnt >= 2)
	{
		// 毫秒滴答计数器清零，重新开始累计
		wMsTickCnt = 0;
		// 系统UTC时间结构体的毫秒字段自增1
		stUtcTime.Msecond++;

		// 判断毫秒数是否达到1000，达到则完成1秒计时并进位
		if(stUtcTime.Msecond >= 1000)
		{
			// 毫秒字段清零
			stUtcTime.Msecond = 0;
			// UTC时间秒数字段自增1
			stUtcTime.Second++;

			// 条件编译：启用时间误差检查或上传时间检查时执行
			#if(TIME_ERR_CHECK || UPLOAD_TIME_CHECK)
			// Unix时间戳（秒级）自增1
			dwUnixTimestamp++;
			#endif
		}

		// 判断秒数是否达到60，达到则完成1分钟计时并进位
		if(stUtcTime.Second >= 60)
		{
			// 秒数字段清零
			stUtcTime.Second = 0;
			// UTC时间分钟字段自增1
			stUtcTime.Minute++;
		}

		// 判断分钟数是否达到60，达到则完成1小时计时并进位
		if(stUtcTime.Minute >= 60)
		{
			// 分钟字段清零
			stUtcTime.Minute = 0;
			// UTC时间小时字段自增1
			stUtcTime.Hour++;
		}

		// 判断小时数是否达到24，达到则完成1天计时并进位
		if(stUtcTime.Hour >= 24)
		{
			// 小时字段清零
			stUtcTime.Hour = 0;
			// UTC时间天数字段自增1
			stUtcTime.Day++;
		}

		// 英文注释：每个月的天数不同，日期进位处理复杂，此处忽略其影响
		// The number of days on each month is different, carrying difficultly. Ignore its influence.
	}
	/***************Milli-second tick end****************/

	// 判断参数通信启动标志位是否置1，注释：等待系统初始化完成
	if(bParaCommStartFlag == true)  //wait for initial complete
	{
		// 调用参数通信触发函数，注释：主机向从机下发设置/控制/查询指令
		sParaCommTrig();  // Master send Set/Ctrl/Query cmd to slaver
	}
    // 英文注释：CPU对本次中断进行应答
    // The CPU acknowledges the interrupt.
}

// 注意共享变量，其可能在被使用前被远端CPU更新
interrupt void EPWM_Prd_isr_copy(void)  // 函数关联采样频率参数wSamplFrq
{
    //mLed2On;  // 原始注释掉的代码：LED2点亮操作，暂未启用

  #if(PwmIsrTimeChk == 1)  // 条件编译：若使能PWM中断时间检测（PwmIsrTimeChk=1）
    sPrdIsrBegin(&PRDISRPWM);  // 调用PWM中断时间检测初始化函数，传入PRDISRPWM结构体地址，记录中断开始时间/状态
  #endif  // 结束PwmIsrTimeChk条件编译

    /* 原始注释掉的代码块：CAN总线接收处理，暂未启用
    while(sbCanMsgUpdated(cCanbId) == true)  // 循环判断CAN总线b通道（cCanbId）是否有新消息更新
    {
        sParaRxComm();  // 调用CAN接收参数处理函数，解析并更新接收到的参数
    }
    */

    /***************IGBT temperatrue sample begin****************/  // IGBT温度采样功能开始
    if(++wIgbtTempSamplCnt >= wSamplFrq)  // 温度采样计数器自增1，判断是否达到采样频率（wSamplFrq，配置为1秒）
    {
        wIgbtTempSamplCnt = 0;  // 计数器清零，为下一次1秒采样做准备

        if(wIgbtTempSamplSel == 1)  // 采样选择标志为1：采集1号模块IGBT温度，切换至2号模块采样
        {
            dwIgbtA1TptRaw = EQep1Regs.QPOSCNT;  // 读取QEP1寄存器的位置计数值，存入A1相IGBT温度原始值
            dwIgbtB1TptRaw = EQep2Regs.QPOSCNT;  // 读取QEP2寄存器的位置计数值，存入B1相IGBT温度原始值
            dwIgbtC1TptRaw = EQep3Regs.QPOSCNT;  // 读取QEP3寄存器的位置计数值，存入C1相IGBT温度原始值
            EQep1Regs.QEPCTL.bit.SWI = 1;  // 设置QEP1控制寄存器的软件初始化位SWI=1，触发QEP1软件复位/初始化
            EQep2Regs.QEPCTL.bit.SWI = 1;  // 设置QEP2控制寄存器的软件初始化位SWI=1，触发QEP2软件复位/初始化
            EQep3Regs.QEPCTL.bit.SWI = 1;  // 设置QEP3控制寄存器的软件初始化位SWI=1，触发QEP3软件复位/初始化
            wIgbtTptCalcReq = 1;  // IGBT温度计算请求置1，通知后台程序计算1号模块IGBT温度
            mModule2NtcSel();  // 调用硬件选择函数，切换NTC采样通道至2号IGBT模块
            wIgbtTempSamplSel = 2;  // 采样选择标志置2，下一次采集2号模块IGBT温度
            EQep1Regs.QEPCTL.bit.SWI = 0;  // 复位QEP1软件初始化位SWI=0，结束QEP1软件复位/初始化
            EQep2Regs.QEPCTL.bit.SWI = 0;  // 复位QEP2软件初始化位SWI=0，结束QEP2软件复位/初始化
            EQep3Regs.QEPCTL.bit.SWI = 0;  // 复位QEP3软件初始化位SWI=0，结束QEP3软件复位/初始化
        }
        else  // 采样选择标志非1（即2）：采集2号模块IGBT温度，切回1号模块采样
        {
            dwIgbtA2TptRaw = EQep1Regs.QPOSCNT;  // 读取QEP1寄存器的位置计数值，存入A2相IGBT温度原始值
            dwIgbtB2TptRaw = EQep2Regs.QPOSCNT;  // 读取QEP2寄存器的位置计数值，存入B2相IGBT温度原始值
            dwIgbtC2TptRaw = EQep3Regs.QPOSCNT;  // 读取QEP3寄存器的位置计数值，存入C2相IGBT温度原始值
            EQep1Regs.QEPCTL.bit.SWI = 1;  // 设置QEP1控制寄存器的软件初始化位SWI=1，触发QEP1软件复位/初始化
            EQep2Regs.QEPCTL.bit.SWI = 1;  // 设置QEP2控制寄存器的软件初始化位SWI=1，触发QEP2软件复位/初始化
            EQep3Regs.QEPCTL.bit.SWI = 1;  // 设置QEP3控制寄存器的软件初始化位SWI=1，触发QEP3软件复位/初始化
            wIgbtTptCalcReq = 2;  // IGBT温度计算请求置2，通知后台程序计算2号模块IGBT温度
            wIgbtTempSamplSel = 1;  // 采样选择标志置1，下一次采集1号模块IGBT温度
            mModule1NtcSel();  // 调用硬件选择函数，切换NTC采样通道至1号IGBT模块
            EQep1Regs.QEPCTL.bit.SWI = 0;  // 复位QEP1软件初始化位SWI=0，结束QEP1软件复位/初始化
            EQep2Regs.QEPCTL.bit.SWI = 0;  // 复位QEP2软件初始化位SWI=0，结束QEP2软件复位/初始化
            EQep3Regs.QEPCTL.bit.SWI = 0;  // 复位QEP3软件初始化位SWI=0，结束QEP3软件复位/初始化
        }
    }
    /***************IGBT temperatrue sample end****************/  // IGBT温度采样功能结束

    sExtIoStsUpdat();  // 调用外部IO状态更新函数，刷新所有外部IO口的输入/输出状态

    sUpdateAcLinkSts();  // 调用交流侧链路状态更新函数，更新交流接触器、交流链路连接状态等

    sOffGridDisPwmCtl();  // 调用离网模式PWM禁止控制函数，处理离网状态下的PWM输出使能/禁止逻辑

    if(++wDftBuffCnt >= 1)  // DFT数据缓冲计数器自增1，判断是否达到缓冲触发条件（每次中断都触发）
    {
        wDftBuffCnt = 0;  // 计数器清零，为下一次缓冲做准备
        sDFTDataBuffer(psMemS2m->fROutVolt, psMemS2m->fROutCurr);  // 调用DFT数据缓冲函数，将输出电压、输出电流存入DFT运算缓冲区
    }

////////////////////LOCK CPLD fault code////////////////////////  // CPLD故障码锁定功能
    if(psMemS2m->unCpldReg[2].word != 0xFFFF)  // 判断CPLD第2个寄存器值是否非0xFFFF（0xFFFF为无故障状态）
    {
        wPMFaultLockCode = ~psMemS2m->unCpldReg[2].word;    //20160219, DJ, 硬件低有效故障码取反为高有效，便于触摸屏显示故障
    }

    if(++wFreCntCycle >= 2)  // 频率计算周期计数器自增1，判断是否达到2次中断周期（频率计算基础周期）
    {
        wFreCntCycle = 0;  // 计数器清零，为下一次频率计算周期做准备

        if(++wFreMovWindowCnt >= wFreMovWindowNum)  // 频率移动窗口计数器自增1，判断是否超出窗口长度
        {
            wFreMovWindowCnt = 0;  // 窗口计数器清零，循环使用移动窗口数组
        }

        //fFreMovWindowNow = 2.0 * psMemS2m->fVoltQxErrOutFilter;  // 原始注释掉的代码：频率窗口当前值计算（2倍电压Q轴滤波误差），暂未启用

        fFreMovWindowNow = psMemS2m->fVoltQxErrOutFilter;  // 频率窗口当前值 = 电压Q轴误差滤波后值，作为频率计算原始数据

        // 移动窗口求和计算：当前总和 = 上一次总和 + 最新值 - 窗口中被挤出的旧值
        fFreMovGrpSum = fFreMovGrpSum_1 + fFreMovWindowNow - fFreMovWindowGrp[wFreMovWindowCnt];
        fFreMovWindowGrp[wFreMovWindowCnt] = fFreMovWindowNow;  // 将最新值存入移动窗口数组的当前位置
        fFreMovGrpSum_1 = fFreMovGrpSum;  // 更新上一次总和，为下一次计算做准备

        //fFreMovGrpSumGain = fFreMovGrpSum * 10000.0;  // 原始注释掉的代码：频率总和增益放大（10000倍），暂未启用
        fFreMovGrpSumGain = fFreMovGrpSum;  // 频率总和增益值 = 频率移动窗口总和，无额外放大

        if(psMemS2m->bPhaseLockOk != 0)  // 判断锁相环是否锁定成功（bPhaseLockOk非0为锁定）
        {
            fFgSumTemp += fFreMovGrpSumGain;  // 锁相锁定时，将频率增益值累加到电网频率临时总和
        }
        else  // 锁相环未锁定
        {
            fFgSumTemp = 0;  // 电网频率临时总和清零，重新开始累加
        }
        // 当窗口计数器为0 或 为窗口长度的一半时，更新电网频率总和并发送中断事件
        if((wFreMovWindowCnt == 0)||(wFreMovWindowCnt == wFreqWindowNumHalf))
        {
            fFgSum = fFgSumTemp;  // 将临时总和赋值给电网频率正式总和，完成一次频率累加计算
            fFgSumTemp = 0;  // 临时总和清零，为下一次累加做准备

            OSISREventSend(cPrioThird,eOutFreqZero);  // 调用系统中断事件发送函数，发送输出频率零检测事件（优先级三级）
        }
    }

#if(HiL_TEST == 0)  // 条件编译：非硬件在环测试模式（HiL_TEST=0）时执行以下代码
  #ifdef _STANDALONE  // 条件编译：独立运行模式（_STANDALONE宏定义）时执行以下代码
    // 判断当前故障码中无IGBT故障（cIBGTFault为IGBT故障掩码）
    if((swGetFaultCode() & cIBGTFault) == 0)
    {
        if(hiPdpPinSts == cLoLev)  // 判断PDP引脚状态为低电平（cLoLev为低电平宏定义，代表IGBT模块故障）
        {
            // IGBT模块故障计数器自增1，判断是否达到1秒（wSamplFrq为1秒计数阈值）
            if(++wModuleFaultForeverCnt >= wSamplFrq)
            {
                wModuleFaultForeverCnt = 0;  // 故障计数器清零

                sSetISRFaultCode(cIBGTFault);  // 调用中断级故障码设置函数，置位IGBT故障码
                //sSetIsrFastOffCmd(cFaultCmd);  // 原始注释掉的代码：设置故障快速关断命令，暂未启用
                OSISREventSend(cPrioSecond,eFaultEvent);  // 发送故障事件（优先级二级），通知系统处理IGBT故障
                //bAutoRestart = true;  // 原始注释掉的代码：使能自动重启功能，暂未启用
            }
        }
        else  // PDP引脚状态为高电平（无IGBT模块故障）
        {
            wModuleFaultForeverCnt = 0;  // IGBT模块故障计数器清零
        }
    }
  #endif  // 结束_STANDALONE条件编译
#endif  // 结束HiL_TEST条件编译

    // Clark convert  // 克拉克变换（三相静止坐标系→两相静止αβ坐标系），针对逆变器输出电压
    // α轴电压计算：(R相逆变器电压 - T相逆变器电压) * 1/3
    psVarM2s->fVinvAlfa = (psMemS2m->fRVinvReal - psMemS2m->fTVinvReal) * 0.33333333;
    // β轴电压计算：S相逆变器电压 * √3/3（0.57735027为√3/3的近似值）
    psVarM2s->fVinvBeta = psMemS2m->fSVinvReal * 0.57735027;
    // Postive park convert  // 正序派克变换（两相静止αβ坐标系→两相旋转dq坐标系）
    // 正序d轴电压计算：cosθ*α轴电压 + sinθ*β轴电压（θ为逆变器锁相角）
    psVarM2s->fVinvDxPlus = psMemS2m->fInvCostheta*psVarM2s->fVinvAlfa + psMemS2m->fInvSintheta * psVarM2s->fVinvBeta;
    // 正序q轴电压计算：cosθ*β轴电压 - sinθ*α轴电压
    psVarM2s->fVinvQxPlus = psMemS2m->fInvCostheta*psVarM2s->fVinvBeta - psMemS2m->fInvSintheta * psVarM2s->fVinvAlfa;
    // Negtive park convert  // 负序派克变换（两相静止αβ坐标系→两相旋转dq坐标系）
    // 负序d轴电压计算：cosθ*α轴电压 - sinθ*β轴电压
    psVarM2s->fVinvDxMinus = psMemS2m->fInvCostheta*psVarM2s->fVinvAlfa - psMemS2m->fInvSintheta * psVarM2s->fVinvBeta;
    // 负序q轴电压计算：cosθ*β轴电压 + sinθ*α轴电压
    psVarM2s->fVinvQxMinus = psMemS2m->fInvCostheta*psVarM2s->fVinvBeta + psMemS2m->fInvSintheta * psVarM2s->fVinvAlfa;
    // Decoupling  // 解耦控制，消除正序/负序分量之间的耦合影响，提升控制精度
    // 正序d轴解耦后电压计算：正序d轴原始值 - 负序d轴滤波值*cos2θ - 负序q轴滤波值*sin2θ
    psVarM2s->fVinvDxPos = psVarM2s->fVinvDxPlus - psVarM2s->fVinvDxNegFilter*psMemS2m->fInvCostheta_2 \
                 - psVarM2s->fVinvQxNegFilter*psMemS2m->fInvSintheta_2;
    // 正序d轴解耦电压一阶低通滤波：滤波值=上一次滤波值*K1 + 当前值*K2（K1+K2=1，滤波系数）
    psVarM2s->fVinvDxPosFilter = psVarM2s->fVinvDxPosFilter*fLFCoeffK1 + psVarM2s->fVinvDxPos*fLFCoeffK2;
    // 正序q轴解耦后电压计算：正序q轴原始值 + 负序d轴滤波值*sin2θ - 负序q轴滤波值*cos2θ
    psVarM2s->fVinvQxPos = psVarM2s->fVinvQxPlus + psVarM2s->fVinvDxNegFilter*psMemS2m->fInvSintheta_2 \
                 - psVarM2s->fVinvQxNegFilter*psMemS2m->fInvCostheta_2;
    // 正序q轴解耦电压一阶低通滤波
    psVarM2s->fVinvQxPosFilter = psVarM2s->fVinvQxPosFilter*fLFCoeffK1 + psVarM2s->fVinvQxPos*fLFCoeffK2;
    // 负序d轴解耦后电压计算：负序d轴原始值 - 正序d轴滤波值*cos2θ + 正序q轴滤波值*sin2θ
    psVarM2s->fVinvDxNeg = psVarM2s->fVinvDxMinus - psVarM2s->fVinvDxPosFilter*psMemS2m->fInvCostheta_2 \
                 + psVarM2s->fVinvQxPosFilter*psMemS2m->fInvSintheta_2;
    // 负序d轴解耦电压一阶低通滤波
    psVarM2s->fVinvDxNegFilter = psVarM2s->fVinvDxNegFilter*fLFCoeffK1 + psVarM2s->fVinvDxNeg*fLFCoeffK2;
    // 负序q轴解耦后电压计算：负序q轴原始值 - 正序d轴滤波值*sin2θ - 正序q轴滤波值*cos2θ
    psVarM2s->fVinvQxNeg = psVarM2s->fVinvQxMinus - psVarM2s->fVinvDxPosFilter*psMemS2m->fInvSintheta_2 \
                 - psVarM2s->fVinvQxPosFilter*psMemS2m->fInvCostheta_2;
    // 负序q轴解耦电压一阶低通滤波
    psVarM2s->fVinvQxNegFilter = psVarM2s->fVinvQxNegFilter*fLFCoeffK1 + psVarM2s->fVinvQxNeg*fLFCoeffK2;

    // 判断逆变器正序d轴滤波电压 与 电网正序d轴滤波电压的差值绝对值小于30V（电压同步阈值）
    if(fabs(psVarM2s->fVinvDxPosFilter - psMemS2m->fVgDxPosFilter) < 30.0)
    {
        // 电压同步计数器自增1，判断是否达到一个正弦波周期的计数点（wOneSinePrdPoint）
        if(++wVinvEqualVgFilter >= wOneSinePrdPoint)
        {
            wVinvEqualVgFilter = 0;  // 同步计数器清零
            bVinvEqualVgFlag = true;  // 逆变器电压与电网电压同步标志置位（已同步）
        }
    }
    else  // 逆变器与电网电压差值超出阈值，未同步
    {
        wVinvEqualVgFilter = 0;  // 电压同步计数器清零
        bVinvEqualVgFlag = false;  // 电压同步标志清零（未同步）
    }

    // 判断当前测试模式为并网模式（cParaOnGrid）或单相并网模式（cSingleOnGrid）
    if((sbGetTestMode() == cParaOnGrid)||(sbGetTestMode() == cSingleOnGrid))
    {
        sGridAbnormJudge();  // 调用电网异常判断函数，检测电网过压、欠压、过频、欠频、相位异常等故障

        if(swGetMode() != cWorkingMode)  // 判断当前系统模式非工作模式（未进入正式并网运行）
        {
            // 判断软启动标志置位（psVarM2s->unCntlFlag.bit.wSoftStart为软启动控制位）
            if(psVarM2s->unCntlFlag.bit.wSoftStart == true)
            {
                // 再次判断逆变器正序d轴滤波电压与电网正序d轴滤波电压差值绝对值小于30V
                if(fabs(psVarM2s->fVinvDxPosFilter - psMemS2m->fVgDxPosFilter) < 30.0)
                {
                    wStartUpSycOkCnt++;  // 启动同步成功计数器自增1
                    // 判断同步成功计数达到1秒（wSamplFrq为1秒阈值），即持续同步1秒
                    if(wStartUpSycOkCnt >= wSamplFrq)
                    {
                        wStartUpSycOkCnt = 0;  // 启动同步计数器清零

                        if(bStartUpSycOkFlag == false)  // 判断启动同步成功标志未置位（首次同步成功）
						{
							sAcCapValCalTrig();        // 触发交流电容值计算函数，开始计算交流侧电容参数
						}

                        bStartUpSycOkFlag = true;  // 启动同步成功标志置位，代表软启动同步完成
                    }
                }
                else  // 逆变器与电网电压未同步
                {
                    if(wStartUpSycOkCnt > 10)  // 同步计数器值大于10，做递减处理（抗干扰，避免单次波动清零）
                    {
                        wStartUpSycOkCnt -= 10;  // 计数器减10
                    }
                    else  // 同步计数器值小于等于10，直接清零
                    {
                        wStartUpSycOkCnt = 0;
                    }

                    // 计算逆变器负序电压幅值的平方：负序d轴滤波值² + 负序q轴滤波值²（判断负序分量是否过大）
                    fVinvNegSum = psVarM2s->fVinvDxNegFilter * psVarM2s->fVinvDxNegFilter + psVarM2s->fVinvQxNegFilter * psVarM2s->fVinvQxNegFilter;

                    if(fVinvNegSum > 40000.0)  // 负序电压幅值平方大于40000（即幅值大于200V，负序分量过大故障）
                    {
                        // 负序故障计数器自增1，判断是否达到PWM频率周期（wPwmFrq为PWM周期计数阈值）
                        if(++wVinvNegCnt >= wPwmFrq)
                        {
                            wVinvNegCnt = 0;  // 负序故障计数器清零

                            //sIPCDisPwmOut();  // 原始注释掉的代码：通过IPC禁止PWM输出，暂未启用
                            //psVarM2s->unCntlFlag.bit.wSoftStart = false;  // 原始注释掉的代码：清除软启动标志，暂未启用

                            sSetISRFaultCode4(cSoftStartFault);  // 调用4字节故障码设置函数，置位软启动故障码
                            //sSetIsrFastOffCmd(cFaultCmd);  // 原始注释掉的代码：设置故障快速关断命令，暂未启用
                            OSISREventSend(cPrioSecond,eFaultEvent);  // 发送故障事件（优先级二级），处理软启动故障
                            bAutoRestart = false;  // 禁止自动重启功能，软启动故障需手动复位
                        }
                    }
                    else  // 逆变器负序分量在正常范围内，无故障
                    {
                        wVinvNegCnt = 0;  // 负序故障计数器清零
                    }
                }

                // 判断交流链路状态为低电平（交流接触器已吸合）且启动同步成功标志置位
                if((sbGetAcLinkSts() == cLoLev)&&(bStartUpSycOkFlag != false))
                {
                    //if(++wMainRlyOnCnt >= 36)  // 原始注释掉的代码：主接触器吸合计数器达到36次（原延时配置）
                    if(++wMainRlyOnCnt >= 108)    // 主接触器吸合计数器自增1，达到108次（对应15ms延时，接触器动作裕量）
                    {
                        wMainRlyOnCnt = 0;  // 接触器计数器清零
                        //sIPCDisPwmOut();  // 原始注释掉的代码：禁止PWM输出，暂未启用
                        //psVarM2s->unCntlFlag.bit.wSoftStart = false;  // 原始注释掉的代码：清除软启动标志，暂未启用
                        bMainRlyOnOk = true;  // 主交流接触器吸合成功标志置位，代表链路准备完成
                    }
                }
                else  // 交流链路未吸合 或 启动同步未完成
                {
                    wMainRlyOnCnt = 0;  // 主接触器计数器清零
                }
            }
            else  // 软启动标志未置位（未进入软启动流程）
            {
                wStartUpSycOkCnt = 0;  // 启动同步计数器清零
                wMainRlyOnCnt = 0;  // 主接触器计数器清零
            }
        }
    }



else
{
    // 进入非前置条件的分支，执行输出过流保护核心逻辑
    if(sbGetTestMode() == cOpenLoop)  // 判断当前测试模式为开环模式（cOpenLoop为开环模式宏定义）
    {
        if(sbGetOffGridMode() == cNoOffGrid)  // 判断当前非离网模式（cNoOffGrid为非离网宏定义，即并网/待机）
        {
            // 开环模式过流保护阈值设为开环低电平保护电流定值（项目自定义定值）
            wOpnLpProCurr = cOpnLpLvProCurr;
        }
        else  // 当前为离网模式
        {
            // 开环模式过流保护阈值设为逆变器最大线电流峰值的1.15倍，动态匹配离网负载电流
            wOpnLpProCurr = 1.15 * psVarM2s->fIgLnPkMax;
        }

        // 判断逆变器六路输出电流（R1/S1/T1/R2/S2/T2）任意一相绝对值超过开环过流保护阈值
        if((fabs(psMemS2m->fR1OutCurr) > wOpnLpProCurr) \
        || (fabs(psMemS2m->fS1OutCurr) > wOpnLpProCurr) \
        || (fabs(psMemS2m->fT1OutCurr) > wOpnLpProCurr) \
        || (fabs(psMemS2m->fR2OutCurr) > wOpnLpProCurr) \
        || (fabs(psMemS2m->fS2OutCurr) > wOpnLpProCurr) \
        || (fabs(psMemS2m->fT2OutCurr) > wOpnLpProCurr))
        {
            // 输出过流保护计数器自增1，判断是否达到触发条件（每次检测到过流即触发，阈值=1）
            if(++wOutCurrProtCnt >= 1)
            {
                wOutCurrProtCnt = 0;  // 过流保护计数器清零，为下一次检测做准备

                sAcLinkOff();  // 调用交流链路断开函数，断开交流接触器/关闭交流侧链路
                sSetISRFaultCode2(cGridCurrOverFault);  // 设置二级中断故障码，置位电网过流故障（cGridCurrOverFault为过流故障宏）
                //sSetIsrFastOffCmd(cFaultCmd);  // 原始注释掉的代码：发送故障快速关断命令，暂未启用
                OSISREventSend(cPrioSecond,eFaultEvent);  // 发送二级优先级故障事件，通知系统处理过流故障

                if(sbGetOffGridMode() == cNoOffGrid)  // 非离网模式下的过流故障处理
                {
                    bAutoRestart = false;  // 禁止自动重启，并网过流需手动复位
                }
                else  // 离网模式下的过流故障处理
                {
                    //bAutoRestart = true;  // 原始注释掉的代码：使能自动重启，暂未启用
                }
            }
        }
        else  // 六路输出电流均在保护阈值范围内，无过流故障
        {
            wOutCurrProtCnt = 0;  // 输出过流保护计数器清零
        }
    }
    else  // 当前测试模式非开环模式（如闭环/并网/测试模式等）
    {
        // 判断逆变器六路输出电流任意一相绝对值超过短路模式PWM禁止电流阈值（项目自定义短路保护定值）
        if((fabs(psMemS2m->fR1OutCurr) > cShortModeDisPwmInvCurr) \
        || (fabs(psMemS2m->fS1OutCurr) > cShortModeDisPwmInvCurr) \
        || (fabs(psMemS2m->fT1OutCurr) > cShortModeDisPwmInvCurr) \
        || (fabs(psMemS2m->fR2OutCurr) > cShortModeDisPwmInvCurr) \
        || (fabs(psMemS2m->fS2OutCurr) > cShortModeDisPwmInvCurr) \
        || (fabs(psMemS2m->fT2OutCurr) > cShortModeDisPwmInvCurr))
        {
            // 输出过流保护计数器自增1，判断是否达到触发条件（每次检测到短路过流即触发）
            if(++wOutCurrProtCnt >= 1)
            {
                wOutCurrProtCnt = 0;  // 过流保护计数器清零

                sAcLinkOff();  // 调用交流链路断开函数，断开交流侧链路
                sSetISRFaultCode2(cGridCurrOverFault);  // 设置二级中断故障码，置位电网过流故障
                //sSetIsrFastOffCmd(cFaultCmd);  // 原始注释掉的代码：发送故障快速关断命令，暂未启用
                OSISREventSend(cPrioSecond,eFaultEvent);  // 发送二级优先级故障事件
                bAutoRestart = false;  // 禁止自动重启，短路过流故障需手动复位
            }
        }
        else  // 六路输出电流均在短路保护阈值范围内，无短路故障
        {
            wOutCurrProtCnt = 0;  // 输出过流保护计数器清零
        }
    }

    // 判断直流母线电压超过PWM禁止的母线过压阈值（cDisPwmDcVolt为母线过压定值）
    if(psMemS2m->fDCVolt > cDisPwmDcVolt)
    {
        sAcLinkOff();  // 调用交流链路断开函数，断开交流侧链路
        sSetISRFaultCode2(cBusOverVoltFault);  // 设置二级中断故障码，置位母线过压故障（cBusOverVoltFault为过压故障宏）
        //sSetIsrFastOffCmd(cFaultCmd);  // 原始注释掉的代码：发送故障快速关断命令，暂未启用
        OSISREventSend(cPrioSecond,eFaultEvent);  // 发送二级优先级故障事件
        bAutoRestart = false;  // 禁止自动重启，母线过压故障需手动复位
    }
}

// 判断C2模块四路故障码（wC2FaultCode/2/3/4）任意一路非0（存在C2模块故障）
if(psMemS2m->wC2FaultCode | psMemS2m->wC2FaultCode2 | psMemS2m->wC2FaultCode3 | psMemS2m->wC2FaultCode4)
{
    sSetISRFaultCode(psMemS2m->wC2FaultCode);    // 将C2一级故障码同步至系统中断级故障码
    sSetISRFaultCode2(psMemS2m->wC2FaultCode2);  // 将C2二级故障码同步至系统中断级故障码
    sSetISRFaultCode3(psMemS2m->wC2FaultCode3);  // 将C2三级故障码同步至系统中断级故障码
    sSetISRFaultCode4(psMemS2m->wC2FaultCode4);  // 将C2四级故障码同步至系统中断级故障码
}

//////////////////Fault Record Trigger/////////////////////////  // 故障记录触发功能区，用于触发波形/故障数据记录
// 判断逆变器控制标志前置状态为真 且 当前逆变器控制标志为假（控制标志由使能→禁止，发生故障关断）
if((bCtlFlagPre != false)&&(psMemS2m->bInvCtrlFlag == false))
{
    unFltRecFlag.word = 3;  // 设置故障记录标志字为3，触发故障记录（3为项目自定义记录类型定值）
}
bCtlFlagPre = psMemS2m->bInvCtrlFlag;  // 更新逆变器控制标志前置状态为当前状态，为下一次判断做准备

// 判断故障记录标志字非0（存在故障记录触发请求）
if(unFltRecFlag.word != 0)
{
    // 故障记录清除计数器自增1，判断是否达到600次（对应83ms，故障记录持续时间）
    if(++wFaultRecClrCnt >= 600)  // 83ms
    {
        wFaultRecClrCnt = 0;  // 故障记录清除计数器清零
        unFltRecFlag.word = 0;  // 清零故障记录标志字，结束故障记录
    }
}
else  // 无故障记录触发请求
{
    wFaultRecClrCnt = 0;  // 故障记录清除计数器清零
}

/**************Rms and Avg value precalculate end***************/  // 有效值/平均值预计算功能结束标记

// 计算母线正半侧电压：(直流母线总电压 + 母线PN压差) * 0.5，用于母线分压均衡检测
fBusVoltPos = (psMemS2m->fDCVolt + psMemS2m->fBusPNDiff) * 0.5;
// 计算母线负半侧电压：(直流母线总电压 - 母线PN压差) * 0.5，用于母线分压均衡检测
fBusVoltNeg = (psMemS2m->fDCVolt - psMemS2m->fBusPNDiff) * 0.5;

// 判断当前系统模式为工作模式（cWorkingMode为正常工作模式宏定义，正式并网/离网运行）
if(swGetMode() == cWorkingMode)
{
    /* 原始注释掉的代码块：母线中点不平衡故障保护，暂未启用
    // 判断母线PN压差超过阈值 或 正/负半侧母线电压超过半母线高压阈值
    if((fabs(psMemS2m->fBusPNDiff) > cBusPNDiffHigh) \
    || (fBusVoltPos > cBusHalfHigh) || (fBusVoltNeg > cBusHalfHigh))
    {
        // 中点故障计数器自增1，判断是否达到3次（抗干扰，连续3次检测到故障才触发）
        if(++wMidFaultCnt >= 3)
        {
            wMidFaultCnt = 0;  // 中点故障计数器清零

            sAcLinkOff();  // 断开交流侧链路
            sSetISRFaultCode4(cNPUnbalFault);  // 设置四级中断故障码，置位母线中点不平衡故障
            //sSetIsrFastOffCmd(cFaultCmd);  // 注释掉的代码：发送故障快速关断命令
            OSISREventSend(cPrioSecond,eFaultEvent);  // 发送二级优先级故障事件
            //bAutoRestart = true;  // 注释掉的代码：使能自动重启
        }
    }
    else  // 母线分压均衡，无中点不平衡故障
    {
        wMidFaultCnt = 0;  // 中点故障计数器清零
    }
    */

    // 判断当前测试模式为并网模式 或 单相并网模式
    if((sbGetTestMode() == cParaOnGrid)||(sbGetTestMode() == cSingleOnGrid))
    {
        // 判断直流母线电压低于紧急低压保护阈值（cEmergDcVoltLow为母线紧急欠压定值）
        if(psMemS2m->fDCVolt < cEmergDcVoltLow)
        {
            // 母线欠压保护计数器自增1，判断是否达到3次（抗干扰，连续3次检测才触发）
            if(++wDcVoltProtCnt >= 3)
            {
                wDcVoltProtCnt = 0;  // 母线欠压计数器清零
                sAcLinkOff();  // 断开交流侧链路
                sSetISRFaultCode(cBusVoltLowFault);  // 设置一级中断故障码，置位母线欠压故障
                //sSetIsrFastOffCmd(cFaultCmd);  // 注释掉的代码：发送故障快速关断命令
                OSISREventSend(cPrioSecond,eFaultEvent);  // 发送二级优先级故障事件
                //bAutoRestart = true;  // 注释掉的代码：使能自动重启
            }
        }
        else  // 直流母线电压在正常范围内，无欠压故障
        {
            wDcVoltProtCnt = 0;  // 母线欠压计数器清零
        }

        sDcRevCurrOverChk();  // 调用直流反向过流检测函数，检测直流侧逆流故障并执行对应保护
    }

    // 判断低电压穿越标志置位（bGridDropFlag=true，电网发生低电压故障）
    if(bGridDropFlag != false)
    {
        wGridDropFilter = 0;  // 低电压穿越滤波计数器清零

        // VRT exit wave record  // 电压穿越退出波形记录：电网故障恢复时的波形记录
        if(bVrtRecType == cExitRec)  // 判断电压穿越记录类型为退出记录（cExitRec为退出记录宏）
        {
            if(psMemS2m->wC2Test9 == 1)  // C2模块测试位9置1（项目自定义触发条件）
            {
                unFltRecFlag.bit.InstRec = 1;  // 设置故障记录标志位，触发瞬时波形记录
            }
        }

        // 判断电网跌落类型为正常模式（cNorm，电网故障恢复，退出LVRT）
        if(swGetGridDropType() == cNorm)
        {
            // LVRT恢复延时计数器自增1，判断是否达到0.5秒（采样频率*0.5，项目自定义延时）
            if(++wFreqDetMaskDly >= (INT16U)((float)wSamplFrq*0.5))  // 0.5s
            {
                wFreqDetMaskDly = 0;  // 恢复延时计数器清零
                bGridDropFlag = false;  // 清零低电压穿越标志，退出LVRT模式

                psVarM2s->fVmAmpMax = fVmAmpHiLim;  // 恢复逆变器电压幅值最大值为高限值（额定值）
            }
        }
        else  // 电网跌落类型非正常模式（仍处于LVRT故障中）
        {
            wFreqDetMaskDly = 0;  // LVRT恢复延时计数器清零
        }
    }
    else  // 低电压穿越标志未置位（电网无低压故障）
    {
        wFreqDetMaskDly = 0;  // LVRT恢复延时计数器清零

        // 判断电网跌落类型为低电压穿越（cLvrt，电网发生低压故障，进入LVRT）
        if(swGetGridDropType() == cLvrt)
        {
            // LVRT触发滤波计数器自增1，判断是否达到10ms（抗干扰，连续10ms检测到低压才触发）
            if(++wGridDropFilter >= (INT16U)((float)wSamplFrq*0.01))  // 10ms
            {
                wGridDropFilter = 0;  // LVRT触发滤波计数器清零
                bGridDropFlag = true;  // 置位低电压穿越标志，进入LVRT模式

                unFltRecFlag.bit.RmsRec = 1;  // 设置故障记录标志位，触发有效值波形记录

                // VRT enter wave record  // 电压穿越进入波形记录：电网故障发生时的波形记录
                if(bVrtRecType == cEnterRec)  // 判断电压穿越记录类型为进入记录（cEnterRec为进入记录宏）
                {
                    unFltRecFlag.bit.InstRec = 1;  // 设置故障记录标志位，触发瞬时波形记录
                }

                // Liangc20200518 for unblance LVRT  // 20200518 梁工开发：不平衡电网低电压穿越处理
                // 判断电网负序电压幅值超过额定相电压峰值的20%（电网三相不平衡）
                if(psMemS2m->fVgAmpNeg > fVgPhPkRtd_20PCT)
                {
                    // 逆变器电压幅值最大值设为容错值，降低输出幅值适应不平衡电网
                    psVarM2s->fVmAmpMax = fVmAmpToler;
                }
            }
        }
        else  // 电网跌落类型非LVRT（电网正常）
        {
            wGridDropFilter = 0;  // LVRT触发滤波计数器清零
        }
    }

    // 判断高电压穿越标志置位（bHvrtDropFlag=true，电网发生高电压故障）
    if(bHvrtDropFlag != false)
    {
        wHvrtDropFilter = 0;  // 高电压穿越滤波计数器清零

        // VRT exit wave record  // 电压穿越退出波形记录：高压故障恢复时记录
        if(bVrtRecType == cExitRec)  // 判断电压穿越记录类型为退出记录
        {
            if(psMemS2m->wC2Test9 == 1)  // C2模块测试位9置1
            {
                unFltRecFlag.bit.InstRec = 1;  // 触发瞬时波形记录
            }
        }

        // 判断电网跌落类型为正常模式（高压故障恢复，退出HVRT）
        if(swGetGridDropType() == cNorm)
        {
            // HVRT恢复延时计数器自增1，判断是否达到0.5秒
            if(++wHvrtDropBackDly >= (INT16U)((float)wSamplFrq*0.5))  // 0.5s
            {
                wHvrtDropBackDly = 0;  // 恢复延时计数器清零
                bHvrtDropFlag = false;  // 清零高压穿越标志，退出HVRT模式
            }
        }
        else  // 仍处于HVRT故障中
        {
            wHvrtDropBackDly = 0;  // HVRT恢复延时计数器清零
        }
    }
    else  // 高电压穿越标志未置位（电网无高压故障）
    {
        wHvrtDropBackDly = 0;  // HVRT恢复延时计数器清零

        // 判断电网跌落类型为高电压穿越（cHvrt，电网发生高压故障，进入HVRT）
        if(swGetGridDropType() == cHvrt)
        {
            // HVRT触发滤波计数器自增1，判断是否达到10ms（抗干扰）
            if(++wHvrtDropFilter >= (INT16U)((float)wSamplFrq*0.01))  // 10ms
            {
                wHvrtDropFilter = 0;  // HVRT触发滤波计数器清零
                bHvrtDropFlag = true;  // 置位高压穿越标志，进入HVRT模式

                unFltRecFlag.bit.RmsRec = 1;  // 触发有效值波形记录

                // VRT enter wave record  // 电压穿越进入波形记录：高压故障发生时记录
                if(bVrtRecType == cEnterRec)  // 判断为进入记录类型
                {
                    unFltRecFlag.bit.InstRec = 1;  // 触发瞬时波形记录
                }
            }
        }
        else  // 电网跌落类型非HVRT（电网正常）
        {
            wHvrtDropFilter = 0;  // HVRT触发滤波计数器清零
        }
    }

    // Liangc20180406 for island emergency frequency protect  // 20180406 梁工开发：孤岛运行紧急频率保护
    // 判断零电压跌落标志置位（bZeroDropFlag=true，电网发生零电压孤岛故障）
    if(bZeroDropFlag != false)
    {
        wZeroDropFilter = 0;  // 零电压跌落滤波计数器清零

        // 判断电网跌落状态位的零电压跌落功能为禁止（cDisable，孤岛故障恢复）
        if(psMemS2m->unGridDropSts.bit.wZeroDrop == cDisable)
        {
            // 零电压跌落恢复延时计数器自增1，判断是否达到0.5秒
            if(++wZeroDropBackDly >= (INT16U)((float)wSamplFrq*0.5))  // 0.5s
            {
                wZeroDropBackDly = 0;  // 恢复延时计数器清零
                bZeroDropFlag = false;  // 清零零电压跌落标志，退出孤岛保护
                fGridIslandVoltSet = fVgLnPkHiLim;  // 孤岛电网电压设定值恢复为高限值（额定值）
            }
        }
        else  // 零电压跌落功能为使能（仍处于孤岛故障中）
        {
            wZeroDropBackDly = 0;  // 零电压跌落恢复延时计数器清零
        }
    }
    else  // 零电压跌落标志未置位（电网无零电压孤岛故障）
    {
        wZeroDropBackDly = 0;  // 零电压跌落恢复延时计数器清零

        // 判断电网跌落状态位的零电压跌落功能为使能（cEnable，发生零电压孤岛故障）
        if(psMemS2m->unGridDropSts.bit.wZeroDrop == cEnable)
        {
            // 零电压跌落触发滤波计数器自增1，判断是否达到10ms（抗干扰）
            if(++wZeroDropFilter >= (INT16U)((float)wSamplFrq*0.01))  // 10ms
            {
                wZeroDropFilter = 0;  // 零电压跌落触发滤波计数器清零
                bZeroDropFlag = true;  // 置位零电压跌落标志，进入孤岛保护
                fGridIslandVoltSet = fVgLnPkToler;  // 孤岛电网电压设定值设为容错值，降低输出电压
            }
        }
        else  // 零电压跌落功能为禁止（电网正常）
        {
            wZeroDropFilter = 0;  // 零电压跌落触发滤波计数器清零
        }
    }

    // Tianzisi20220406 for avoiding false trigger of 8Hz/s frequency protect  // 20220406 田工开发：避免8Hz/s频率保护误触发
    // 判断锁相丢失标志置位（bPllLossFlag=true，锁相环失锁，电网频率/相位异常）
    if(bPllLossFlag != false)
    {
        // 判断电网跌落状态位的锁相丢失标志为禁止（cDisable，锁相环恢复正常）
        if(psMemS2m->unGridDropSts.bit.wPllLossFlg == cDisable)
        {
            // 锁相丢失恢复延时计数器自增1，判断是否达到0.5秒（防抖，确认锁相恢复）
            if(++wPllLossBackDly >= (INT16U)((float)wSamplFrq*0.5))  // 0.5s
            {
                wPllLossBackDly = 0;  // 恢复延时计数器清零
                bPllLossFlag = false;  // 清零锁相丢失标志，恢复正常锁相
            }
        }
        else  // 锁相丢失功能为使能（仍处于失锁状态）
        {
            wPllLossBackDly = 0;  // 锁相丢失恢复延时计数器清零
        }
    }
    else  // 锁相丢失标志未置位（锁相环正常）
    {
        wPllLossBackDly = 0;  // 锁相丢失恢复延时计数器清零

        // 判断电网跌落状态位的锁相丢失标志为使能（cEnable，锁相环失锁）
        if(psMemS2m->unGridDropSts.bit.wPllLossFlg == cEnable)
        {
            bPllLossFlag = true;  // 置位锁相丢失标志，触发锁相丢失保护
        }
    }

    // 累加R相电网电压采样值至偏移求和变量，用于电网电压直流偏移校准
    fROffsetSampleSum += psMemS2m->fRVgSample;
    // 累加S相电网电压采样值至偏移求和变量，用于电网电压直流偏移校准
    fSOffsetSampleSum += psMemS2m->fSVgSample;
    wGridVoltOffsetCnt++;  // 电网电压偏移采样计数器自增1

    // 判断偏移采样计数器达到2倍采样频率（wSamplFrq*2，对应2秒，完成一次偏移校准周期）
    if(wGridVoltOffsetCnt >= wSamplFrq*2)     //7200/50*100  原始备注：采样频率相关定值参考
    {
        // 计算偏移求和阈值：采样次数*5.0（5.0为项目自定义偏移阈值，过滤微小偏移）
        fSumCheck = (float)wGridVoltOffsetCnt*5.0;  // 72000  原始备注：阈值参考值

        // R相偏移求和值超过正阈值，说明R相电压采样存在正直流偏移
        if(fROffsetSampleSum > fSumCheck)
        {
            psVarM2s->wROutVoltDiff -= 1;  // R相输出电压补偿值减1，抵消正偏移
        }
        // R相偏移求和值小于负阈值，说明R相电压采样存在负直流偏移
        else if(fROffsetSampleSum < -fSumCheck)
        {
            psVarM2s->wROutVoltDiff += 1;  // R相输出电压补偿值加1，抵消负偏移
        }

        // S相偏移求和值超过正阈值，说明S相电压采样存在正直流偏移
        if(fSOffsetSampleSum > fSumCheck)
        {
            psVarM2s->wSOutVoltDiff -= 1;  // S相输出电压补偿值减1，抵消正偏移
        }
        // S相偏移求和值小于负阈值，说明S相电压采样存在负直流偏移
        else if(fSOffsetSampleSum < -fSumCheck)
        {
            psVarM2s->wSOutVoltDiff += 1;  // S相输出电压补偿值加1，抵消负偏移
        }

        // R相电压补偿值上限限制：最大不超过600（项目自定义补偿量程）
        if(psVarM2s->wROutVoltDiff > 600)
        {
            psVarM2s->wROutVoltDiff = 600;
        }
        // R相电压补偿值下限限制：最小不低于-600
        else if(psVarM2s->wROutVoltDiff < -600)
        {
            psVarM2s->wROutVoltDiff = -600;
        }

        // S相电压补偿值上限限制：最大不超过600
        if(psVarM2s->wSOutVoltDiff > 600)
        {
            psVarM2s->wSOutVoltDiff = 600;
        }
        // S相电压补偿值下限限制：最小不低于-600
        else if(psVarM2s->wSOutVoltDiff < -600)
        {
            psVarM2s->wSOutVoltDiff = -600;
        }

        fROffsetSampleSum = 0;  // 清零R相偏移求和变量，为下一次校准做准备
        fSOffsetSampleSum = 0;  // 清零S相偏移求和变量，为下一次校准做准备
        wGridVoltOffsetCnt = 0;  // 清零电网电压偏移采样计数器，为下一次校准做准备
    }

    // 判断电网跌落类型为正常模式 且 电网跌落状态的临界区为正常（电网无任何故障，正常运行）
    if((swGetGridDropType() == cNorm) \
    && (psMemS2m->unGridDropSts.bit.wCessZone == cNorm))
    {
        sClearISRWarningCode(cGFRTAbnorm);  // 清除中断级警告码，清零电网故障穿越异常警告
    }
    else  // 电网存在故障（LVRT/HVRT/孤岛等）或处于临界区
    {
        sSetISRWarningCode(cGFRTAbnorm);  // 设置中断级警告码，置位电网故障穿越异常警告
    }
}
else  // 当前系统模式非工作模式（待机/故障/停机等，未正式运行）
{
    bGridDropFlag = false;    // 清零低电压穿越标志，强制退出LVRT
    bZeroDropFlag = false;    // 清零零电压跌落标志，强制退出孤岛保护
    bHvrtDropFlag = false;    // 清零高电压穿越标志，强制退出HVRT
    bPllLossFlag  = false;    // 清零锁相丢失标志，强制恢复锁相状态

    fGridIslandVoltSet = fVgLnPkHiLim;  // 孤岛电网电压设定值恢复为额定高限值
    psVarM2s->fVmAmpMax = fVmAmpHiLim;  // 逆变器电压幅值最大值恢复为额定高限值
}

// 零压跌落标志置位时，执行孤岛保护电压阈值的动态调整逻辑
//if(bZeroDropFlag == true)
//{
//    // 电压实际保护延时计数器为0时，进入初始判断逻辑
//    if(wVoltRealProtDlyCnt == 0)
//    {
//        // 电网跌落状态寄存器中零压跌落位为禁用状态时
//        if(psMemS2m->unGridDropSts.bit.wZeroDrop == cDisable)
//        {
//            // 电压实际保护延时计数器自增1，启动延时
//            wVoltRealProtDlyCnt++;
//            // 将孤岛保护电压阈值设置为电网线电压峰值容忍值
//            fGridIslandVoltSet = fVgLnPkToler;
//        }
//    }
//    // 电压实际保护延时计数器非0时，进入延时递增判断逻辑
//    else
//    {
//	    // 计数器自增1后，判断是否达到500ms延时（采样频率*0.5，转为无符号16位整数）
//	    if(++wVoltRealProtDlyCnt >= (INT16U)((float)wSamplFrq*0.5))  // 500ms 延时阈值
//	    {
//	        // 计数器钳位在500ms阈值，防止溢出
//	        wVoltRealProtDlyCnt = (INT16U)((float)wSamplFrq*0.5);
//	        // 孤岛保护电压阈值切换为电网线电压峰值高限值
//	        fGridIslandVoltSet = fVgLnPkHiLim;
//	    }
//	}
//}
// 零压跌落标志未置位时，恢复默认保护配置
//else
//{
//    // 清零电压实际保护延时计数器
//    wVoltRealProtDlyCnt = 0;
//    // 孤岛保护电压阈值恢复为电网线电压峰值高限值
//    fGridIslandVoltSet = fVgLnPkHiLim;
//
// }

/**********快速孤岛保护逻辑**********/
// 判断R/S/T三相输出电压的绝对值是否超过孤岛保护电压阈值，任一相超阈值即触发保护判断
if((fabs(psMemS2m->fROutVolt) > fGridIslandVoltSet) \
|| (fabs(psMemS2m->fSOutVolt) > fGridIslandVoltSet) \
|| (fabs(psMemS2m->fTOutVolt) > fGridIslandVoltSet))
{
    // 输出电压保护计数器自增1，连续超阈值时累加计数
    if(++wOutVoltProtCnt >= 15)
    {
        // 计数器清零，为下一次保护判断做准备
        wOutVoltProtCnt = 0;

        // 故障记录标志字置3，标识捕获到过压故障（巨幅电压）
        unFltRecFlag.word = 3;  // capture huge voltage：捕获到巨幅电压故障

        // 执行交流侧链路断开操作（如断开并网继电器、关断PWM等）
        sAcLinkOff();
        // 设置中断服务程序故障码为孤岛故障
        sSetISRFaultCode(cIslandFault);
        //// 注释：设置快速关断命令为故障关断（暂未启用）
        //sSetIsrFastOffCmd(cFaultCmd);
        // 向操作系统中断服务发送二级优先级的故障事件，触发上层故障处理
        OSISREventSend(cPrioSecond,eFaultEvent);
        //// 注释：置位自动重启标志（暂未启用）
        //bAutoRestart = true;
        // 中断服务程序记录电网故障事件，故障类型为逆变器过压
        sIsrRecGridFltEvent(cInvOverVolt);
    }

    // 输出电压保护清零计数器清零，连续超阈值时禁止清零保护计数
    wOutVoltClrCnt = 0;
}
// 三相输出电压均未超过孤岛保护电压阈值时，执行计数器清零逻辑
else
{
    // 零压跌落标志置位时，直接清零输出电压保护计数器（零压跌落时不累计保护计数）
    if(bZeroDropFlag == true)
    {
        wOutVoltProtCnt = 0;
    }
    // 零压跌落标志未置位时，执行延时清零逻辑
    else
    {
        // 清零计数器自增1，判断是否达到100ms延时（采样频率*0.1，转为无符号16位整数）
        if(++wOutVoltClrCnt >= (INT16U)((float)wSamplFrq*0.1))  // 100ms 延时清零阈值
        {
            // 清零计数器清零，为下一次延时做准备
            wOutVoltClrCnt = 0;
            // 输出电压保护计数器清零，解除保护计数
            wOutVoltProtCnt = 0;
        }
    }
}

// 输入电源电压保护功能使能时（寄存器对应位为启用状态）
if(unInPowerProtReg.bit.VoltProt == cEnable)
{
    // 设备当前工作模式非正常工作模式时（如待机、故障、调试模式）
    if(swGetMode() != cWorkingMode)
    {
        // 判断R/S/T三相输出电压绝对值是否超过额定线电压峰值的150%，任一相超阈值即触发超级过压保护
	    if((fabs(psMemS2m->fROutVolt) > fVgLnPkRtd_150PCT)||(fabs(psMemS2m->fSOutVolt) > fVgLnPkRtd_150PCT)||\
	       (fabs(psMemS2m->fTOutVolt) > fVgLnPkRtd_150PCT))
	    {
	        // 超级输出电压保护计数器自增1，连续超阈值累加
	        if(++wOutVoltProtSuperCnt >= 20)
	        {
	            // 直流开关事件寄存器置位直流断路器闭合命令，触发直流侧保护
	            DcSwEvent.bit.DcBrkOnCmd = 1;
	            // 超级保护计数器清零
	            wOutVoltProtSuperCnt = 0;
	            // 设置输入电源跳闸标志为真，触发输入电源保护动作
	            sSetInPowerTrip(true);
	            // 执行输入电源断路器跳闸操作（宏定义，底层硬件/驱动操作）
	            mInPowerBreakTrip;
	            // 设置高压跳闸标志为真，记录高压故障
	            sSetVoltHighTripFlag(true);
	        }
	    }

	    // 每40ms检查一次超级电压保护计数器的清零逻辑（定时判断，防止误触发）
        if(++wVoltProtClearSuperCnt >= (INT16U)((float)wSamplFrq*0.04))   //check 40ms：40ms定时检查
        {
        	// 定时检查计数器清零，为下一次40ms计时做准备
        	wVoltProtClearSuperCnt = 0;

            // 判断当前超级保护计数器是否小于等于上一次的计数值（无递增，说明电压已恢复）
            if(wOutVoltProtSuperCnt <= wVoltProtLastSuperCnt)
            {
            	// 清零超级保护计数器，解除超级保护计数
            	wOutVoltProtSuperCnt = 0;
            }

            // 记录本次超级保护计数器值，作为下一次40ms检查的对比基准
            wVoltProtLastSuperCnt = wOutVoltProtSuperCnt;
        }
    }
    // 设备当前为正常工作模式时，清零所有超级保护相关计数器
    else
    {
    	wOutVoltProtSuperCnt = 0;
    	wVoltProtLastSuperCnt = 0;
    	wVoltProtClearSuperCnt = 0;
    }
}
// 输入电源电压保护功能禁用时，清零所有超级保护相关计数器
else
{
	wOutVoltProtSuperCnt = 0;
	wVoltProtLastSuperCnt = 0;
	wVoltProtClearSuperCnt = 0;
}


/* 中断服务程序电源管理配置 */
// 条件编译：电源计算功能在中断服务程序中执行时（PWR_CAL宏定义为cIsr）
#if(PWR_CAL == cIsr)
    // 20ms计数器自增1，判断是否达到20ms定时（采样频率*0.02，转为无符号16位整数）
    if(++w20msCnt >= (INT16U)((float)wSamplFrq * 0.02))
    {
        // 置位电源中断管理标志，触发20ms一次的电源管理操作
        bPwrIsrMngFlg = true;
        // 20ms计数器清零，为下一次计时做准备
        w20msCnt = 0;
    }
    
    // 中断周期计数器自增1，判断是否达到设定的中断周期数
    if(++wIsrCycleCnt >= cIsrCycleNum)
    {
        // 中断周期计数器清零
        wIsrCycleCnt = 0;
        // 置位中断周期计数器满标志，触发周期型操作
        bIsrCycleCntFullFlg = true;
    }

    // 设备测试模式非开环测试、非短路环测试时（正常并网/离网测试模式）
    if((sbGetTestMode() != cOpenLoop)&&(sbGetTestMode() != cShortLoop))
    {
        // 执行有功/无功功率计算/调节操作
        sDratWattOrVar();
    }

    // 设备测试模式为并网参数测试、或单相并网测试时
    if((sbGetTestMode() == cParaOnGrid)||(sbGetTestMode() == cSingleOnGrid))
    {
        // 电压环控制使能时（寄存器对应位为启用状态）
        if(psVarM2s->unCntlFlag.bit.wVoltLoopEn == cEnable)
        {
            // LCD屏控制为0xCE（特定指令/状态） 或 参数重启标志置位时
            if((sbGetLCDOnOffCtl() == 0xCE)||(bParaReStartFlag == true))
            {
                // 空操作：满足条件时无执行动作
            }
            // 最大功率点跟踪（MPPT）功能使能时
            else if(psVarM2s->unCntlFlag.bit.wMpptEn == cEnable)
            {
                // 执行电源管理操作，入参为20ms电源管理标志（控制20ms一次的MPPT调节）
                sPwrManip(bPwrIsrMngFlg);
            }
            // 既非LCD特定状态/参数重启，也非MPPT使能时
            else
            {
                // 空操作：无执行动作
            }
        }
        // 电压环控制禁用时
        else
        {
            // 空操作：无执行动作
        }
    }

    // 清零电源中断管理标志，结束本次20ms电源管理周期
    bPwrIsrMngFlg = false;
    // 清零中断周期计数器满标志，结束本次中断周期
    bIsrCycleCntFullFlg = false;
// 条件编译结束：电源计算不在中断中执行时，跳过上述逻辑
#endif
/* 电源管理配置结束 */

// 交流数据记录延时计数器自增1，判断是否达到交流数据记录周期
if(++wAcDataRecDly >= wAcDataRecPrd)
{
    // 交流数据记录延时计数器清零，为下一次记录计时
    wAcDataRecDly = 0;

    // 将实时交流数据拷贝到交流数据记录缓冲区
    sAcDataCopy(wAcDataRecBuffer);
    // 对交流数据记录缓冲区的数据进行处理（如缓存、格式化、上传等）
    sAcDataBuffer(wAcDataRecBuffer);
}

// 直流数据记录延时计数器自增1，判断是否达到直流数据记录周期
if(++wDcDataRecDly >= wDcDataRecPrd)
{
    // 直流数据记录延时计数器清零，为下一次记录计时
    wDcDataRecDly = 0;

    // 将实时直流数据拷贝到直流数据记录缓冲区
    sDcDataCopy(wDcDataRecBuffer);
    // 对直流数据记录缓冲区的数据进行处理（如缓存、格式化、上传等）
    sDcDataBuffer(wDcDataRecBuffer);
}

// 注释：设置带符号DAC输出值（Daca通道），值为直流电流*0.01（暂未启用）
//sSetSignedDacVal(cDacaId,psMemS2m->fDCCurr*0.01);

// 快速关断命令非无关断命令时（存在快速关断请求：故障关断/正常关断）
if(swGetFastOffCmd() != cNoneOffCmd)
{
    // 执行交流侧链路断开操作（关断并网回路、PWM驱动等）
	sAcLinkOff();
    // PWM禁用延时计数器自增1，启动关断延时
    wPwmDisDlyCnt++;

    // 交流链路状态为高电平（已断开） 或 PWM禁用延时达到100ms时，执行后续关断动作
    if((sbGetAcLinkSts() == cHiLev)||(wPwmDisDlyCnt >= (INT16U)((float)wSamplFrq * 0.1)))
    {
        // PWM禁用延时计数器清零
        wPwmDisDlyCnt = 0;

        // 快速关断命令为故障关断时
        if(swGetFastOffCmd() == cFaultCmd)
        {
            // 向操作系统中断服务发送二级优先级的故障事件，触发上层故障处理
            OSISREventSend(cPrioSecond,eFaultEvent);
        }
        // 快速关断命令为非故障关断（正常关断）时
        else
        {
            // 向操作系统中断服务发送二级优先级的逆变器关断事件，触发上层正常关断处理
            OSISREventSend(cPrioSecond,eOffInverter);
        }

        // 清零快速关断命令，恢复为无关断命令状态
        sSetIsrFastOffCmd(cNoneOffCmd);
    }
}
// 无快速关断命令时
else
{
    // 清零PWM禁用延时计数器
    wPwmDisDlyCnt = 0;
}

// 注释：LED2灯关断操作（宏定义，底层GPIO操作，暂未启用）
//mLed2Off;

// 将示波器内存数据拷贝到示波器地址缓冲区（底层调试数据缓存，供上位机示波器解析）
ScopeMemCpy(ScopeMem,ScopeAddr);
// 根据参数内存的示波器密码，设置示波器数据到示波器地址缓冲区（加密/验证后的数据更新）
ScopeDataSet(&(psVarM2s->ScopePassWord),ScopeSet,ScopeAddr);

/**************有效值和平均值预计算开始***************/
// 直流母线电压累加：实时值加入到临时计算结构体的母线电压和，用于后续平均值计算
pstDataCalcTemp->fBusVoltSum += psMemS2m->fDCVolt;
// 直流母线电压累加计数自增1，记录累加次数
pstDataCalcTemp->wBusVoltSumCnt++;
// 光伏电压累加：实时值加入到临时计算结构体的光伏电压和，用于后续平均值计算
pstDataCalcTemp->fPvVoltSum += psMemS2m->fPvVolt;
// 光伏电压累加计数自增1
pstDataCalcTemp->wPvVoltSumCnt++;
// 光伏电流累加：实时值加入到临时计算结构体的光伏电流和，用于后续平均值计算
pstDataCalcTemp->fPvCurrSum += psMemS2m->fDCCurr;
// 光伏电流累加计数自增1
pstDataCalcTemp->wPvCurrSumCnt++;

// 外部温度累加：实时外部温度值加入到临时计算结构体的外部温度和，用于后续平均值计算
pstDataCalcTemp->fExtTptSum += psMemS2m->wExtTpt;
// 外部温度累加计数自增1
pstDataCalcTemp->wExtTptSumCnt++;

// R相输出电压平方累加：实时值平方后加入和值，用于后续有效值计算（有效值=根号(平方和/次数)）
pstDataCalcTemp->fROutVolt2Sum += psMemS2m->fROutVolt * psMemS2m->fROutVolt;
// S相输出电压平方累加
pstDataCalcTemp->fSOutVolt2Sum += psMemS2m->fSOutVolt * psMemS2m->fSOutVolt;
// T相输出电压平方累加
pstDataCalcTemp->fTOutVolt2Sum += psMemS2m->fTOutVolt * psMemS2m->fTOutVolt;
// R相输出电流平方累加
pstDataCalcTemp->fROutCurr2Sum += psMemS2m->fROutCurr * psMemS2m->fROutCurr;
// S相输出电流平方累加
pstDataCalcTemp->fSOutCurr2Sum += psMemS2m->fSOutCurr * psMemS2m->fSOutCurr;
// T相输出电流平方累加
pstDataCalcTemp->fTOutCurr2Sum += psMemS2m->fTOutCurr * psMemS2m->fTOutCurr;
// R1相输出电流平方累加
pstDataCalcTemp->fR1OutCurr2Sum += psMemS2m->fR1OutCurr * psMemS2m->fR1OutCurr;
// S1相输出电流平方累加
pstDataCalcTemp->fS1OutCurr2Sum += psMemS2m->fS1OutCurr * psMemS2m->fS1OutCurr;
// T1相输出电流平方累加
pstDataCalcTemp->fT1OutCurr2Sum += psMemS2m->fT1OutCurr * psMemS2m->fT1OutCurr;
// R2相输出电流平方累加
pstDataCalcTemp->fR2OutCurr2Sum += psMemS2m->fR2OutCurr * psMemS2m->fR2OutCurr;
// S2相输出电流平方累加
pstDataCalcTemp->fS2OutCurr2Sum += psMemS2m->fS2OutCurr * psMemS2m->fS2OutCurr;
// T2相输出电流平方累加
pstDataCalcTemp->fT2OutCurr2Sum += psMemS2m->fT2OutCurr * psMemS2m->fT2OutCurr;
// 共模电流平方累加
pstDataCalcTemp->fCmCCurr2Sum += psMemS2m->fCmCCurr * psMemS2m->fCmCCurr;
// 差模Alpha轴电流平方累加
pstDataCalcTemp->fDmAlphaCCurr2Sum += psMemS2m->fDmAlphaCCurr * psMemS2m->fDmAlphaCCurr;
// 差模Beta轴电流平方累加
pstDataCalcTemp->fDmBetaCCurr2Sum += psMemS2m->fDmBetaCCurr * psMemS2m->fDmBetaCCurr;
// 外部共模电流平方累加
pstDataCalcTemp->fExtCmCCurr2Sum += psMemS2m->fExtCmCCurr * psMemS2m->fExtCmCCurr;
// 漏电流平方累加
pstDataCalcTemp->fLeakCurr2Sum += psMemS2m->fLeakCurr * psMemS2m->fLeakCurr;
// R相逆变器电压平方累加
pstDataCalcTemp->fRInvVolt2Sum += psMemS2m->fRVinvReal * psMemS2m->fRVinvReal;
// S相逆变器电压平方累加
pstDataCalcTemp->fSInvVolt2Sum += psMemS2m->fSVinvReal * psMemS2m->fSVinvReal;
// T相逆变器电压平方累加
pstDataCalcTemp->fTInvVolt2Sum += psMemS2m->fTVinvReal * psMemS2m->fTVinvReal;
// 输出电压平方累加计数自增1，记录所有电压/电流平方的累加次数
pstDataCalcTemp->wOutVolt2SumCnt++;
// 并网电流d轴正参考值累加，用于后续平均值计算
pstDataCalcTemp->fParaCurrDxPosRefSum += (float)psMemS2m->wParaCurrDxPosRef;
// 电网d轴正滤波电压累加，用于后续平均值计算
pstDataCalcTemp->fVgDxPosFilterSum += psMemS2m->fVgDxPosFilter;

// R相输出有功功率累加：相电压*相电流，实时值加入和值，用于后续平均值计算
pstDataCalcTemp->fROutWattSum += psMemS2m->fROutPhaseVolt * psMemS2m->fROutCurr;
// S相输出有功功率累加
pstDataCalcTemp->fSOutWattSum += psMemS2m->fSOutPhaseVolt * psMemS2m->fSOutCurr;
// T相输出有功功率累加
pstDataCalcTemp->fTOutWattSum += psMemS2m->fTOutPhaseVolt * psMemS2m->fTOutCurr;
// 输出功率累加计数自增1，记录功率累加次数
pstDataCalcTemp->wOutWattSumCnt++;

// 光伏功率累加：直流母线电压*直流电流，实时值加入和值，用于后续平均值计算
pstDataCalcTemp->fPvPowerSum += psMemS2m->fDCVolt * psMemS2m->fDCCurr;
// 光伏功率累加计数自增1
pstDataCalcTemp->wPvPowerSumCnt++;

// 中性点对电网电压累加，用于后续平均值计算
pstDataCalcTemp->fN2GVoltSum += psMemS2m->fN2GVoltReal;
// 漏电流累加，用于后续平均值计算
pstDataCalcTemp->fLeakCurrSum += psMemS2m->fLeakCurr;
// 中性点电压/漏电流累加计数自增1
pstDataCalcTemp->wN2GVoltSumCnt++;

// R相输出平均电压平方累加：平均电压值平方后加入和值，用于后续有效值计算
pstDataCalcTemp->fROutVoltAvg2Sum += psMemS2m->fROutVoltAvg * psMemS2m->fROutVoltAvg;
// S相输出平均电压平方累加
pstDataCalcTemp->fSOutVoltAvg2Sum += psMemS2m->fSOutVoltAvg * psMemS2m->fSOutVoltAvg;
// T相输出平均电压平方累加
pstDataCalcTemp->fTOutVoltAvg2Sum += psMemS2m->fTOutVoltAvg * psMemS2m->fTOutVoltAvg;
// 输出平均电压平方累加计数自增1
pstDataCalcTemp->wOutVolt2SumAvgCnt++;

// A相（R相）电压临时累加：平均相电压加入临时变量，用于后续均值/功率计算
fAphaseVoltTemp += psMemS2m->fROutPhaseVoltAvg;
// B相（S相）电压临时累加
fBphaseVoltTemp += psMemS2m->fSOutPhaseVoltAvg;
// C相（T相）电压临时累加
fCphaseVoltTemp += psMemS2m->fTOutPhaseVoltAvg;
// A相（R相）电流临时累加：平均相电流加入临时变量，用于后续均值/功率计算
fAphaseCurrTemp += psMemS2m->fROutCurrAvg;
// B相（S相）电流临时累加
fBphaseCurrTemp += psMemS2m->fSOutCurrAvg;
// C相（T相）电流临时累加
fCphaseCurrTemp += psMemS2m->fTOutCurrAvg;

// 条件编译：硬件板卡类型非TNPC拓扑时（TNPC为三电平逆变器拓扑，跳过此段逻辑）
#if(BOARD_TYPE != cTNPC)
    // 平均值计算计数器自增1，判断是否达到2次累加（每2次中断做一次均值计算）
    if(++wAvgCalCnt >= 2)
    {
    	// 平均值计算计数器清零，为下一次累加做准备
    	wAvgCalCnt = 0;
        // A相有功功率计算结构体的电压输入赋值为：电压临时累加值*0.5（2次累加的平均值）
        pStReaPowerCal_A->fVoltIn = fAphaseVoltTemp * 0.5;
        // B相有功功率计算结构体的电压输入赋值为2次累加平均值
        pStReaPowerCal_B->fVoltIn = fBphaseVoltTemp * 0.5;
        // C相有功功率计算结构体的电压输入赋值为2次累加平均值
        pStReaPowerCal_C->fVoltIn = fCphaseVoltTemp * 0.5;

        // 定义16位有符号整型变量i，用于循环计数
        INT16S i;

        // 循环：将A/B/C相电流缓存数组进行左移（滑窗滤波），保留最新的cOrder_N-1个值
        for(i=0;i<cOrder_N-1;i++)
        {
        	// A相电流数组：第i位 = 第i+1位（左移一位，丢弃最旧值）
        	fAphaseCurr[i] = fAphaseCurr[i+1];
        	// B相电流数组：左移一位
        	fBphaseCurr[i] = fBphaseCurr[i+1];
        	// C相电流数组：左移一位
        	fCphaseCurr[i] = fCphaseCurr[i+1];
        }

        // A相电流数组最后一位赋值为：电流临时累加值*0.5（2次累加的平均值，最新值）
        fAphaseCurr[cOrder_N-1] = fAphaseCurrTemp * 0.5;
        // B相电流数组最后一位赋值为2次累加平均值
        fBphaseCurr[cOrder_N-1] = fBphaseCurrTemp * 0.5;
        // C相电流数组最后一位赋值为2次累加平均值
        fCphaseCurr[cOrder_N-1] = fCphaseCurrTemp * 0.5;

        // A相有功功率计算结构体的电流输入赋值为电流数组中间值（滑窗滤波后的值，提高稳定性）
        pStReaPowerCal_A->fCurrIn = fAphaseCurr[(cOrder_N-1)/2];
        // B相有功功率计算结构体的电流输入赋值为电流数组中间值
        pStReaPowerCal_B->fCurrIn = fBphaseCurr[(cOrder_N-1)/2];
        // C相有功功率计算结构体的电流输入赋值为电流数组中间值
        pStReaPowerCal_C->fCurrIn = fCphaseCurr[(cOrder_N-1)/2];

        // 清零A/B/C相电压临时累加值，为下一次2次累加做准备
        fAphaseVoltTemp = 0;
        fBphaseVoltTemp = 0;
        fCphaseVoltTemp = 0;
        // 清零A/B/C相电流临时累加值
        fAphaseCurrTemp = 0;
        fBphaseCurrTemp = 0;
        fCphaseCurrTemp = 0;

        // 根据并网电流q轴正给定值，执行电容参数系数计算（补偿电容影响）
        sCapVarCoffCal(psMemS2m->fCurrQxPosGiven);

        // 对A相有功功率计算结构体执行希尔伯特变换（用于无功功率/相位计算）
        sHilbertTrans(pStReaPowerCal_A);
        // 对B相执行希尔伯特变换
        sHilbertTrans(pStReaPowerCal_B);
        // 对C相执行希尔伯特变换
        sHilbertTrans(pStReaPowerCal_C);

        // A相输出平均电流平方累加：希尔伯特变换后的实时电流平方加入和值
        pstDataCalcTemp->fROutCurrAvg2Sum += pStReaPowerCal_A->fCurrReal * pStReaPowerCal_A->fCurrReal;
        // B相输出平均电流平方累加
        pstDataCalcTemp->fSOutCurrAvg2Sum += pStReaPowerCal_B->fCurrReal * pStReaPowerCal_B->fCurrReal;
        // C相输出平均电流平方累加
        pstDataCalcTemp->fTOutCurrAvg2Sum += pStReaPowerCal_C->fCurrReal * pStReaPowerCal_C->fCurrReal;
        // 输出平均电流平方累加计数自增1
        pstDataCalcTemp->wOutCurr2SumAvgCnt++;

        // A相输出平均功率累加：电压数组中间值 * 希尔伯特变换后的实时电流
        pstDataCalcTemp->fROutWattSumAvg += pStReaPowerCal_A->fVolt[(cOrder_N-1)/2] * pStReaPowerCal_A->fCurrReal;
        // B相输出平均功率累加
        pstDataCalcTemp->fSOutWattSumAvg += pStReaPowerCal_B->fVolt[(cOrder_N-1)/2] * pStReaPowerCal_B->fCurrReal;
        // C相输出平均功率累加
        pstDataCalcTemp->fTOutWattSumAvg += pStReaPowerCal_C->fVolt[(cOrder_N-1)/2] * pStReaPowerCal_C->fCurrReal;

        // 输出平均功率累加计数自增1
        pstDataCalcTemp->wOutWattAvgSumCnt++;
    }
// 条件编译结束：板卡为TNPC拓扑时，跳过上述均值/功率计算逻辑
#endif

// 正弦波指针计数器自增1，判断是否达到一个正弦波周期的采样点数（电周期计时）
if(++wSinpointer >= wOneSinePrdPoint)
{
    // 正弦波指针计数器清零，为下一个正弦波周期计时做准备
    wSinpointer = 0;

    // 将临时计算结构体的R相输出电压平方和，赋值到正式计算结构体（保存一个电周期的和值）
    pstDataCalc->fROutVolt2Sum = pstDataCalcTemp->fROutVolt2Sum;
    // S相输出电压平方和赋值
    pstDataCalc->fSOutVolt2Sum = pstDataCalcTemp->fSOutVolt2Sum;
    // T相输出电压平方和赋值
    pstDataCalc->fTOutVolt2Sum = pstDataCalcTemp->fTOutVolt2Sum;
    // R相输出电流平方和赋值
    pstDataCalc->fROutCurr2Sum = pstDataCalcTemp->fROutCurr2Sum;
    // S相输出电流平方和赋值
    pstDataCalc->fSOutCurr2Sum = pstDataCalcTemp->fSOutCurr2Sum;
    // T相输出电流平方和赋值
    pstDataCalc->fTOutCurr2Sum = pstDataCalcTemp->fTOutCurr2Sum;
    // R1相输出电流平方和赋值
    pstDataCalc->fR1OutCurr2Sum = pstDataCalcTemp->fR1OutCurr2Sum;
    // S1相输出电流平方和赋值
    pstDataCalc->fS1OutCurr2Sum = pstDataCalcTemp->fS1OutCurr2Sum;
    // T1相输出电流平方和赋值
    pstDataCalc->fT1OutCurr2Sum = pstDataCalcTemp->fT1OutCurr2Sum;
    // R2相输出电流平方和赋值
    pstDataCalc->fR2OutCurr2Sum = pstDataCalcTemp->fR2OutCurr2Sum;
    // S2相输出电流平方和赋值
    pstDataCalc->fS2OutCurr2Sum = pstDataCalcTemp->fS2OutCurr2Sum;
    // T2相输出电流平方和赋值
    pstDataCalc->fT2OutCurr2Sum = pstDataCalcTemp->fT2OutCurr2Sum;
    // 共模电流平方和赋值
    pstDataCalc->fCmCCurr2Sum = pstDataCalcTemp->fCmCCurr2Sum;
    // 差模Alpha轴电流平方和赋值
    pstDataCalc->fDmAlphaCCurr2Sum = pstDataCalcTemp->fDmAlphaCCurr2Sum;
    // 差模Beta轴电流平方和赋值
    pstDataCalc->fDmBetaCCurr2Sum = pstDataCalcTemp->fDmBetaCCurr2Sum;
    // 外部共模电流平方和赋值
    pstDataCalc->fExtCmCCurr2Sum = pstDataCalcTemp->fExtCmCCurr2Sum;
    // 漏电流平方和赋值
    pstDataCalc->fLeakCurr2Sum = pstDataCalcTemp->fLeakCurr2Sum;
    // R相逆变器电压平方和赋值
    pstDataCalc->fRInvVolt2Sum = pstDataCalcTemp->fRInvVolt2Sum;
    // S相逆变器电压平方和赋值
    pstDataCalc->fSInvVolt2Sum = pstDataCalcTemp->fSInvVolt2Sum;
    // T相逆变器电压平方和赋值
    pstDataCalc->fTInvVolt2Sum = pstDataCalcTemp->fTInvVolt2Sum;
    // 输出电压平方累加次数赋值
    pstDataCalc->wOutVolt2SumCnt = pstDataCalcTemp->wOutVolt2SumCnt;
    // 并网电流d轴正参考值和赋值
    pstDataCalc->fParaCurrDxPosRefSum = pstDataCalcTemp->fParaCurrDxPosRefSum;
    // 电网d轴正滤波电压和赋值
    pstDataCalc->fVgDxPosFilterSum = pstDataCalcTemp->fVgDxPosFilterSum;

    // 清零临时计算结构体的R相输出电压平方和，为下一个电周期累加做准备
    pstDataCalcTemp->fROutVolt2Sum = 0;
    // 清零S相输出电压平方和
    pstDataCalcTemp->fSOutVolt2Sum = 0;
    // 清零T相输出电压平方和
    pstDataCalcTemp->fTOutVolt2Sum = 0;
    // 清零R相输出电流平方和
    pstDataCalcTemp->fROutCurr2Sum = 0;
    // 清零S相输出电流平方和
    pstDataCalcTemp->fSOutCurr2Sum = 0;
    // 清零T相输出电流平方和
    pstDataCalcTemp->fTOutCurr2Sum = 0;
    // 清零R1相输出电流平方和
    pstDataCalcTemp->fR1OutCurr2Sum = 0;
    // 清零S1相输出电流平方和
    pstDataCalcTemp->fS1OutCurr2Sum = 0;
    // 清零T1相输出电流平方和
    pstDataCalcTemp->fT1OutCurr2Sum = 0;
    // 清零R2相输出电流平方和
    pstDataCalcTemp->fR2OutCurr2Sum = 0;
    // 清零S2相输出电流平方和
    pstDataCalcTemp->fS2OutCurr2Sum = 0;
    // 清零T2相输出电流平方和
    pstDataCalcTemp->fT2OutCurr2Sum = 0;
    // 清零共模电流平方和
    pstDataCalcTemp->fCmCCurr2Sum = 0;
    // 清零差模Alpha轴电流平方和
    pstDataCalcTemp->fDmAlphaCCurr2Sum = 0;
    // 清零差模Beta轴电流平方和
    pstDataCalcTemp->fDmBetaCCurr2Sum = 0;
    // 清零外部共模电流平方和
    pstDataCalcTemp->fExtCmCCurr2Sum = 0;
    // 清零漏电流平方和
    pstDataCalcTemp->fLeakCurr2Sum = 0;
    // 清零R相逆变器电压平方和
    pstDataCalcTemp->fRInvVolt2Sum = 0;
    // 清零S相逆变器电压平方和
    pstDataCalcTemp->fSInvVolt2Sum = 0;
    // 清零T相逆变器电压平方和
    pstDataCalcTemp->fTInvVolt2Sum = 0;
    // 清零输出电压平方累加次数
    pstDataCalcTemp->wOutVolt2SumCnt = 0;
    // 清零并网电流d轴正参考值和
    pstDataCalcTemp->fParaCurrDxPosRefSum = 0;
    // 清零电网d轴正滤波电压和
    pstDataCalcTemp->fVgDxPosFilterSum = 0;
	
    // 输出功率计算周期计数器自增1，判断是否达到5个正弦波周期（累计5个电周期计算一次功率）
    if(++wOutputWattCalPrdCnt >= 5)
    {
        // 输出功率计算周期计数器清零，为下一次5周期计时做准备
        wOutputWattCalPrdCnt = 0;

        // 将临时计算结构体的R相输出功率和赋值到正式计算结构体
        pstDataCalc->fROutWattSum = pstDataCalcTemp->fROutWattSum;
        // S相输出功率和赋值
        pstDataCalc->fSOutWattSum = pstDataCalcTemp->fSOutWattSum;
        // T相输出功率和赋值
        pstDataCalc->fTOutWattSum = pstDataCalcTemp->fTOutWattSum;
        // 输出功率累加次数赋值
        pstDataCalc->wOutWattSumCnt = pstDataCalcTemp->wOutWattSumCnt;

        // 中性点对电网电压和赋值
        pstDataCalc->fN2GVoltSum = pstDataCalcTemp->fN2GVoltSum;
        // 漏电流和赋值
        pstDataCalc->fLeakCurrSum = pstDataCalcTemp->fLeakCurrSum;
        // 中性点电压/漏电流累加次数赋值
        pstDataCalc->wN2GVoltSumCnt = pstDataCalcTemp->wN2GVoltSumCnt;

        // A相输出平均功率和赋值
        pstDataCalc->fROutWattSumAvg = pstDataCalcTemp->fROutWattSumAvg;
        // B相输出平均功率和赋值
        pstDataCalc->fSOutWattSumAvg = pstDataCalcTemp->fSOutWattSumAvg;
        // C相输出平均功率和赋值
        pstDataCalc->fTOutWattSumAvg = pstDataCalcTemp->fTOutWattSumAvg;
        // 输出平均功率累加次数赋值
        pstDataCalc->wOutWattAvgSumCnt = pstDataCalcTemp->wOutWattAvgSumCnt;

        // R相输出平均电压平方和赋值
        pstDataCalc->fROutVoltAvg2Sum = pstDataCalcTemp->fROutVoltAvg2Sum;
        // S相输出平均电压平方和赋值
        pstDataCalc->fSOutVoltAvg2Sum = pstDataCalcTemp->fSOutVoltAvg2Sum;
        // T相输出平均电压平方和赋值
        pstDataCalc->fTOutVoltAvg2Sum = pstDataCalcTemp->fTOutVoltAvg2Sum;
        // A相输出平均电流平方和赋值
        pstDataCalc->fROutCurrAvg2Sum = pstDataCalcTemp->fROutCurrAvg2Sum;
        // B相输出平均电流平方和赋值
        pstDataCalc->fSOutCurrAvg2Sum = pstDataCalcTemp->fSOutCurrAvg2Sum;
        // C相输出平均电流平方和赋值
        pstDataCalc->fTOutCurrAvg2Sum = pstDataCalcTemp->fTOutCurrAvg2Sum;
        // 输出平均电压平方累加次数赋值
        pstDataCalc->wOutVolt2SumAvgCnt = pstDataCalcTemp->wOutVolt2SumAvgCnt;
        // 输出平均电流平方累加次数赋值
        pstDataCalc->wOutCurr2SumAvgCnt = pstDataCalcTemp->wOutCurr2SumAvgCnt;
        
        // 清零临时计算结构体的R相输出功率和
        pstDataCalcTemp->fROutWattSum = 0;
        // 清零S相输出功率和
        pstDataCalcTemp->fSOutWattSum = 0;
        // 清零T相输出功率和
        pstDataCalcTemp->fTOutWattSum = 0;
        // 清零输出功率累加次数
        pstDataCalcTemp->wOutWattSumCnt = 0;

        // 清零中性点对电网电压和
        pstDataCalcTemp->fN2GVoltSum = 0;
        // 清零漏电流和
        pstDataCalcTemp->fLeakCurrSum = 0;
        // 清零中性点电压/漏电流累加次数
        pstDataCalcTemp->wN2GVoltSumCnt = 0;
        // 清零A相输出平均功率和
        pstDataCalcTemp->fROutWattSumAvg = 0;
        // 清零B相输出平均功率和
        pstDataCalcTemp->fSOutWattSumAvg = 0;
        // 清零C相输出平均功率和
        pstDataCalcTemp->fTOutWattSumAvg = 0;
        // 清零输出平均功率累加次数
        pstDataCalcTemp->wOutWattAvgSumCnt = 0;

        // 清零R相输出平均电压平方和
        pstDataCalcTemp->fROutVoltAvg2Sum = 0;
        // 清零S相输出平均电压平方和
        pstDataCalcTemp->fSOutVoltAvg2Sum = 0;
        // 清零T相输出平均电压平方和
        pstDataCalcTemp->fTOutVoltAvg2Sum = 0;
        // 清零A相输出平均电流平方和
        pstDataCalcTemp->fROutCurrAvg2Sum = 0;
        // 清零B相输出平均电流平方和
        pstDataCalcTemp->fSOutCurrAvg2Sum = 0;
        // 清零C相输出平均电流平方和
        pstDataCalcTemp->fTOutCurrAvg2Sum = 0;
        // 清零输出平均电压平方累加次数
        pstDataCalcTemp->wOutVolt2SumAvgCnt = 0;
        // 清零输出平均电流平方累加次数
        pstDataCalcTemp->wOutCurr2SumAvgCnt = 0;

        // 执行总无功功率计算（基于各相有功/无功功率，计算三相总无功）
        sTotalReacPowerCal();
    }

    // 向操作系统中断服务发送三级优先级的正弦波过零事件，触发电周期同步操作
    OSISREventSend(cPrioThird,eOutSinZero);
}

// 更新快速控制信息（如PWM占空比、电流环/电压环控制参数、状态标志等）
sUpdateFastCtrlInfo();

// 条件编译：使能PWM中断时间检测时（PwmIsrTimeChk宏定义为1）
#if(PwmIsrTimeChk == 1)
    // 记录PWM中断服务程序的结束时间，用于检测中断执行时长（防止中断溢出）
    sPrdIsrEnd(&PRDISRPWM);
// 条件编译结束：未使能时跳过
#endif

// 注释：LED2灯关断操作（宏定义，底层GPIO操作，暂未启用）
//mLed2Off;

// 清零本次定时器中断的中断标志位（清除硬件中断挂起，允许下一次中断触发）
PRDISRPWM.ETCLR.bit.INT = 1;
// 确认PIE组3的中断（向PIE控制器发送确认信号，允许接收该组后续的中断请求）
PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;



}


/*********************************************************************
 * 函数名: EPWM_Udf_isr_copy
 * 功能: EPWM用户自定义中断服务函数（已注释）
 * 说明: 该函数原本用于处理EPWM定时器中断，目前暂未启用
 *********************************************************************/
/*interrupt void EPWM_Udf_isr_copy(void)
{

	// Clear INT flag for this timer
    // 清除该EPWM定时器的中断标志位
    //ZROISRPWM.ETCLR.bit.INT = 1;
    // Acknowledge this interrupt to receive more interrupts from group 3
    // 确认PIE组3的中断，以便接收该组更多的中断请求
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;
}*/

/*********************************************************************
 * 函数名: sCPU02toCPU01IPC0IntHandler
 * 功能: CPU02到CPU01的IPC0中断处理函数
 * 说明: 处理双核间IPC通信中断，解析并响应IPC命令
 *********************************************************************/
interrupt void sCPU02toCPU01IPC0IntHandler(void)
{
    // Continue processing messages
    // 继续处理IPC通信消息
    uint32_t command;                  // 定义存储IPC命令的变量
    command = IpcRegs.IPCRECVCOM;      // 读取接收到的IPC命令

    // 根据IPC命令类型执行不同操作
    switch (command)
    {
        // IPC函数调用命令
        case IPC_FUNC_CALL:
            // 调用IPC Lite库函数处理函数调用请求，使用FLAG0和FLAG31
            IPCLiteRtoLFunctionCall(IPC_FLAG0, IPC_FLAG31);
            break;
        // 默认情况（未知命令）
        default:
            // 确认IPC_FLAG0中断，避免重复触发
            IpcRegs.IPCACK.all |= IPC_FLAG0;
            break;
    }

    // IPC Lite Driver Functions acknowledge the IPC interrupt.
    // IPC Lite驱动函数会自动确认IPC中断，无需单独确认IPC中断标志
    // There is no need to ACK the IPC interrupt flag separately.
    // 确认PIE组1的中断，允许接收该组后续中断
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;   // Acknowledge interrupt to PIE

}

/*********************************************************************
 * 函数名: sCanaProtIsr
 * 功能: CANA总线协议层中断处理函数
 * 说明: 检测CANA总线错误状态（总线关闭/警告），设置故障标志
 *********************************************************************/
interrupt void sCanaProtIsr(void)
{
    // 将CANA错误状态寄存器值复制到影子寄存器（防止寄存器值被覆盖）
	CanShadow.CAN_ES.all = CanaRegs.CAN_ES.all;
    // 检查是否发生总线关闭(BOff)或错误警告(EWarn)
	if((CanShadow.CAN_ES.bit.BOff == 1)||(CanShadow.CAN_ES.bit.EWarn == 1))
    {
        // 设置CANA故障标志为真
        bCanaFailFlag = true;
    }
    // 检查是否无错误（LEC=0表示无错误）
	else if(CanShadow.CAN_ES.bit.LEC == 0)
	{
        // 清除CANA故障标志
		bCanaFailFlag = false;
	}

    // 清除CANA全局中断0标志（CANn_INT0）
    CanaRegs.CAN_GLB_INT_CLR.all = 1;  
    // 确认PIE组9的中断，允许接收该组后续中断
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP9;
}

/*********************************************************************
 * 函数名: sCanaMsgIsr
 * 功能: CANA消息中断处理函数
 * 说明: 处理CANA收发消息中断，根据消息ID分发到对应处理函数
 *********************************************************************/
interrupt void sCanaMsgIsr(void)
{
    // 定义存储CAN消息ID的变量
	INT16U wMsgId;

    //wCanaFailCnt = 0;  // 注释：清零CANA故障计数（暂未启用）

    // 将CANA中断寄存器值复制到影子寄存器
	CanShadow.CAN_INT.all = CanaRegs.CAN_INT.all;
    // 读取中断对应的消息ID
	wMsgId = CanShadow.CAN_INT.bit.INT1ID;

    // 根据消息ID分类处理
    // 消息ID=1：发送到SCU设置的消息中断
    if(wMsgId == 1)  
    {
        // 调用CANA发送中断处理函数（cCanaId为CANA控制器ID，wMsgId为消息ID）
        sCanTxIsr(cCanaId,wMsgId);
    }
    // 消息ID=2：发送到SCU查询的消息中断
    else if(wMsgId == 2)  
    {
        sCanTxIsr(cCana2Id,wMsgId);
    }
    // 消息ID=3：主动上报的发送消息中断
    else if(wMsgId == 3)  
    {
        sCanTxIsr(cCana3Id,wMsgId);
    }
    // 消息ID=4：预留
    else if(wMsgId == 4)
    {
    }
    // 消息ID=5：预留
    else if(wMsgId == 5)
    {
    }
    // 消息ID=6：预留
    else if(wMsgId == 6)
    {
    }
    // 消息ID=9~16：接收SCU设置的消息中断
    else if((wMsgId >= 9)&&(wMsgId <= 16))  
    {
        // 调用CANA硬件缓冲区消息读取函数
        sCanHwBufMsgGet(cCanaId,wMsgId);
    }
    // 消息ID=17~20：接收SCU查询的消息中断
    else if((wMsgId >= 17)&&(wMsgId <= 20))  
    {
        sCanHwBufMsgGet(cCana2Id,wMsgId);
    }
    // 消息ID=21~24：接收SCU超级设置的消息中断
    else if((wMsgId >= 21)&&(wMsgId <= 24))  
    {
        sCanHwBufMsgGet(cCana4Id,wMsgId);
    }
    // 消息ID=25~28：预留
    else if((wMsgId >= 25)&&(wMsgId <= 28))
    {
    }
    // 消息ID=29~32：预留
    else if((wMsgId >= 29)&&(wMsgId <= 32))
    {
    }
    // 其他消息ID：预留
    else
    {
    }

    // 清除CANA全局中断1标志（CANn_INT1）
	CanaRegs.CAN_GLB_INT_CLR.all = 2;  
    // 确认PIE组9的中断
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP9;
}

/*********************************************************************
 * 全局变量: wCanbRxBuzyCnt
 * 功能: CANB接收忙计数，防止接收处理死循环
 * 初始值: 0
 *********************************************************************/
static INT16U wCanbRxBuzyCnt = 0;

/*********************************************************************
 * 函数名: sCanbMsgRxIsr
 * 功能: CANB消息接收中断处理函数
 * 说明: 处理CANB接收消息中断，循环读取消息直到无新消息或达到计数上限
 *********************************************************************/
interrupt void sCanbMsgRxIsr(void)
{
    // 延时指令：等待数据传输到IF3寄存器（10个NOP周期）
    asm(" RPT #10 || NOP");  

    // 清零接收忙计数
    wCanbRxBuzyCnt = 0;
    // 循环检查是否有新的CANB消息更新
	while(sbCanMsgUpdated(cCanbId) == true)
	{
        // 接收忙计数自增
	    if(++wCanbRxBuzyCnt > 100)
	    {
            // 计数超过100则退出循环，防止死循环
	        break;
	    }
        // 处理接收到的参数通信消息
	    sParaRxComm();
	}

    // 清除CANB全局中断1标志（CANn_INT1）
	CanbRegs.CAN_GLB_INT_CLR.all = 2;  
    // 确认PIE组9的中断
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP9;
}

/*********************************************************************
 * 函数名: sSciaMsgRxIsr
 * 功能: SCIA串口接收中断处理函数
 * 说明: 处理SCIA串口接收中断，调用通用串口接收处理函数
 *********************************************************************/
interrupt void sSciaMsgRxIsr(void)
{
    // 调用SCIA串口数据接收中断处理函数（cSciANum为SCIA设备号）
    sSciDataRxISR(cSciANum);

    // 确认PIE组9的中断，允许接收后续中断
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP9;
}

/*********************************************************************
 * 函数名: sSciaMsgTxIsr
 * 功能: SCIA串口发送中断处理函数
 * 说明: 处理SCIA串口发送中断，完成发送后重置485方向为接收
 *********************************************************************/
interrupt void sSciaMsgTxIsr(void)
{
    // 调用SCIA串口数据发送中断处理函数
    sSciDataTxISR(cSciANum);

    //reset SCIA to read status.
    // 检查SCIA发送状态是否非忙
    if(sbGetSciTxStatus(cSciANum) != cSciTxBusy)
    {
        // 将SCIA的485引脚设置为接收模式（宏定义）
        mSCIA_PC485_RD;
    }

    // 确认PIE组9的中断
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP9;
}

/*********************************************************************
 * 函数名: sScibMsgRxIsr
 * 功能: SCIB串口接收中断处理函数
 * 说明: 处理SCIB串口接收中断，调用通用串口接收处理函数
 *********************************************************************/
interrupt void sScibMsgRxIsr(void)
{
    // 调用SCIB串口数据接收中断处理函数（cSciBNum为SCIB设备号）
    sSciDataRxISR(cSciBNum);

    // 确认PIE组9的中断
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP9;
}

/*********************************************************************
 * 函数名: sScibMsgTxIsr
 * 功能: SCIB串口发送中断处理函数
 * 说明: 处理SCIB串口发送中断，完成发送后重置485方向为接收
 *********************************************************************/
interrupt void sScibMsgTxIsr(void)
{
    // 调用SCIB串口数据发送中断处理函数
    sSciDataTxISR(cSciBNum);

    //reset SCIB to read status.
    // 检查SCIB发送状态是否非忙
    if(sbGetSciTxStatus(cSciBNum) != cSciTxBusy)
    {
        // 将SCIB的485引脚设置为接收模式（宏定义）
        mSCIB_PC485_RD;
    }

    // 确认PIE组9的中断
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP9;
}

/*********************************************************************
 * 函数名: sScibDataCheck
 * 功能: SCIB串口数据检查函数
 * 说明: 轮询检查SCIB串口接收/发送就绪状态，主动触发数据处理
 *********************************************************************/
void sScibDataCheck(void)
{
    // 检查SCIB接收是否就绪
    if(sbGetSciRxRdy(cSciBNum) == cSciRxRdy)
    {
        // 调用SCIB接收处理函数
        sSciDataRxISR(cSciBNum);
    }
    // 检查SCIB发送是否就绪（注：此处宏定义名cSciRxRdy疑似笔误，应为cSciTxRdy）
    if(sbGetSciTxRdy(cSciBNum) == cSciRxRdy)
    {
        // 调用SCIB发送处理函数
        sSciDataTxISR(cSciBNum);
    }
}

/*********************************************************************
 * 函数名: OSIPCEventSend
 * 功能: OS IPC事件发送函数
 * 说明: 将32位参数拆分后调用OS中断事件发送函数
 * 参数: dwParam - 32位参数，高16位为源OS，低16位为目标OS
 *********************************************************************/
void OSIPCEventSend(INT32U dwParam)
{
    // 调用OS中断事件发送函数：高16位>>16，低16位直接取
    OSISREventSend((INTOS)(dwParam>>16),(INTOS)dwParam);
}

/*********************************************************************
 * 函数名: sfRmsCal
 * 功能: 有效值（RMS）计算函数
 * 说明: 根据平方和与采样次数计算有效值
 * 参数: fSquareSum - 采样值平方和，wSumCnt - 采样次数
 * 返回: 计算得到的有效值
 *********************************************************************/
float sfRmsCal(float fSquareSum, INT16U wSumCnt)
{
    // 定义临时存储有效值的变量
    float fRmsTemp;

    // 采样次数大于0时计算有效值
    if (wSumCnt > 0)
    {
        // 有效值公式：√(平方和/采样次数)
    	fRmsTemp = sqrt(fSquareSum / (float)wSumCnt);
    }
    // 采样次数为0时返回0（防止除零错误）
    else
    {
    	fRmsTemp = 0;
    }

    // 返回计算结果
    return(fRmsTemp);
}

/*********************************************************************
 * 函数名: sfAvgCal
 * 功能: 平均值计算函数
 * 说明: 根据总和与采样次数计算平均值
 * 参数: fSum - 采样值总和，wSumCnt - 采样次数
 * 返回: 计算得到的平均值
 *********************************************************************/
float sfAvgCal(float fSum, INT16U wSumCnt)
{
    // 定义临时存储平均值的变量
    float fAvgTemp;

    // 采样次数大于0时计算平均值
	if (wSumCnt > 0)
	{
        // 平均值公式：总和/采样次数
        fAvgTemp = fSum / (float)wSumCnt;
    }
    // 采样次数为0时返回0（防止除零错误）
    else
    {
        fAvgTemp = 0;
    }

    // 返回计算结果
    return(fAvgTemp);
}

/*********************************************************************
 * 函数名: sUpdateMpptSumValue
 * 功能: 更新MPPT（最大功率点跟踪）求和值
 * 说明: 将临时缓存的MPPT采样值复制到正式变量，并清零临时缓存
 *********************************************************************/
void sUpdateMpptSumValue(void)
{
    // 进入临界区：禁止中断，防止数据读写冲突
	OS_ENTER_CRITICAL();

    // 复制PV电压平方和：临时缓存 -> 正式变量
	pstDataCalc->fPvVoltSum = pstDataCalcTemp->fPvVoltSum;
    // 清零PV电压平方和临时缓存
	pstDataCalcTemp->fPvVoltSum = 0;
    // 复制PV电压采样次数：临时缓存 -> 正式变量
	pstDataCalc->wPvVoltSumCnt = pstDataCalcTemp->wPvVoltSumCnt;
    // 清零PV电压采样次数临时缓存
	pstDataCalcTemp->wPvVoltSumCnt = 0;

    // 复制PV电流平方和：临时缓存 -> 正式变量
	pstDataCalc->fPvCurrSum = pstDataCalcTemp->fPvCurrSum;
    // 清零PV电流平方和临时缓存
	pstDataCalcTemp->fPvCurrSum = 0;
    // 复制PV电流采样次数：临时缓存 -> 正式变量
	pstDataCalc->wPvCurrSumCnt = pstDataCalcTemp->wPvCurrSumCnt;
    // 清零PV电流采样次数临时缓存
	pstDataCalcTemp->wPvCurrSumCnt = 0;

    // 复制PV功率平方和：临时缓存 -> 正式变量
	pstDataCalc->fPvPowerSum = pstDataCalcTemp->fPvPowerSum;
    // 清零PV功率平方和临时缓存
	pstDataCalcTemp->fPvPowerSum = 0;
    // 复制PV功率采样次数：临时缓存 -> 正式变量
	pstDataCalc->wPvPowerSumCnt = pstDataCalcTemp->wPvPowerSumCnt;
    // 清零PV功率采样次数临时缓存
	pstDataCalcTemp->wPvPowerSumCnt = 0;

    // 退出临界区：恢复中断
	OS_EXIT_CRITICAL();
}

/*********************************************************************
 * 函数名: sUpdateBusSumValue
 * 功能: 更新母线电压求和值
 * 说明: 将临时缓存的母线电压采样值复制到正式变量，并清零临时缓存
 *********************************************************************/
void sUpdateBusSumValue(void)
{
    // 进入临界区：禁止中断
	OS_ENTER_CRITICAL();

    // 复制母线电压平方和：临时缓存 -> 正式变量
	pstDataCalc->fBusVoltSum = pstDataCalcTemp->fBusVoltSum;
    // 清零母线电压平方和临时缓存
	pstDataCalcTemp->fBusVoltSum=0;
    // 复制母线电压采样次数：临时缓存 -> 正式变量
	pstDataCalc->wBusVoltSumCnt = pstDataCalcTemp->wBusVoltSumCnt;
    // 清零母线电压采样次数临时缓存
	pstDataCalcTemp->wBusVoltSumCnt = 0;

    // 退出临界区：恢复中断
	OS_EXIT_CRITICAL();
}

/*********************************************************************
 * 函数名: sfExtPTTempCal
 * 功能: 外部PT（电压互感器）温度计算函数
 * 说明: 计算外部PT温度平均值，并确保温度值非负
 * 返回: 外部PT温度平均值
 *********************************************************************/
float sfExtPTTempCal(void)
{
    // 定义临时存储温度的变量
    float fTempTemp;

    // 进入临界区：禁止中断
    OS_ENTER_CRITICAL();

    // 复制外部PT温度总和：临时缓存 -> 正式变量
    pstDataCalc->fExtTptSum = pstDataCalcTemp->fExtTptSum;
    // 清零外部PT温度总和临时缓存
    pstDataCalcTemp->fExtTptSum = 0;

    // 复制外部PT温度采样次数：临时缓存 -> 正式变量
    pstDataCalc->wExtTptSumCnt = pstDataCalcTemp->wExtTptSumCnt;
    // 清零外部PT温度采样次数临时缓存
    pstDataCalcTemp->wExtTptSumCnt = 0;

    // 退出临界区：恢复中断
    OS_EXIT_CRITICAL();

    // 采样次数大于0时计算平均值
    if(pstDataCalc->wExtTptSumCnt > 0)
    {
        fTempTemp = pstDataCalc->fExtTptSum /(float)(pstDataCalc->wExtTptSumCnt);
    }

    // 确保温度值非负（小于0则置0）
    if(fTempTemp < 0)
    {
        fTempTemp=0;
    }

    // 返回温度平均值
    return(fTempTemp);
}

/*********************************************************************
 * 函数名: swGetROutputVoltReal
 * 功能: 获取R相输出电压实际值
 * 说明: 将浮点电压值乘以10转换为整型（保留1位小数）
 * 返回: R相输出电压实际值（整型，单位：0.1V）
 *********************************************************************/
INT16S swGetROutputVoltReal(void)
{
    return((INT16S)(psMemS2m->fROutVolt*10));
}

/*********************************************************************
 * 函数名: swGetSOutputVoltReal
 * 功能: 获取S相输出电压实际值
 * 说明: 将浮点电压值乘以10转换为整型（保留1位小数）
 * 返回: S相输出电压实际值（整型，单位：0.1V）
 *********************************************************************/
INT16S swGetSOutputVoltReal(void)
{
    return((INT16S)(psMemS2m->fSOutVolt*10));
}

/*********************************************************************
 * 函数名: swGetTOutputVoltReal
 * 功能: 获取T相输出电压实际值
 * 说明: 将浮点电压值乘以10转换为整型（保留1位小数）
 * 返回: T相输出电压实际值（整型，单位：0.1V）
 *********************************************************************/
INT16S swGetTOutputVoltReal(void)
{
    return((INT16S)(psMemS2m->fTOutVolt*10));
}

/*********************************************************************
 * 函数名: swGetOutputPeriodNew
 * 功能: 获取新的输出周期
 * 说明: 预留函数，当前返回0
 * 返回: 0
 *********************************************************************/
INT16U swGetOutputPeriodNew(void)
{
    return(0);
}

/*********************************************************************
 * 函数名: swGetROutputCurrReal
 * 功能: 获取R相输出电流实际值
 * 说明: 将浮点电流值乘以10转换为整型（保留1位小数）
 * 返回: R相输出电流实际值（整型，单位：0.1A）
 *********************************************************************/
INT16S swGetROutputCurrReal(void)
{
    return((INT16S)(psMemS2m->fROutCurr*10));
}

/*********************************************************************
 * 函数名: swGetSOutputCurrReal
 * 功能: 获取S相输出电流实际值
 * 说明: 将浮点电流值乘以10转换为整型（保留1位小数）
 * 返回: S相输出电流实际值（整型，单位：0.1A）
 *********************************************************************/
INT16S swGetSOutputCurrReal(void)
{
    return((INT16S)(psMemS2m->fSOutCurr*10));
}

/*********************************************************************
 * 函数名: swGetTOutputCurrReal
 * 功能: 获取T相输出电流实际值
 * 说明: 将浮点电流值乘以10转换为整型（保留1位小数）
 * 返回: T相输出电流实际值（整型，单位：0.1A）
 *********************************************************************/
INT16S swGetTOutputCurrReal(void)
{
    return((INT16S)(psMemS2m->fTOutCurr*10));
}

/*********************************************************************
 * 函数名: swGetDCCurrReal
 * 功能: 获取直流电流实际值
 * 说明: 将浮点电流值乘以10转换为整型（保留1位小数）
 * 返回: 直流电流实际值（整型，单位：0.1A）
 *********************************************************************/
INT16S swGetDCCurrReal(void)
{
    return((INT16S)(psMemS2m->fDCCurr*10));
}

/*********************************************************************
 * 函数名: sSetPhaseRTChangeFlag
 * 功能: 设置相序RT切换标志
 * 说明: 更新相序切换标志，并同步到共享内存变量
 * 参数: changeflag - 相序切换标志值
 *********************************************************************/
void sSetPhaseRTChangeFlag(INT16S changeflag)
{
    // 更新本地相序切换标志
    wPhaseRTChangeFlag = changeflag;
    // 同步到共享内存的控制标志位
    psVarM2s->unCntlFlag.bit.wPhaseSeqBit = changeflag;
}

/*********************************************************************
 * 函数名: sSetModuleFaultCnt
 * 功能: 设置模块故障计数
 * 说明: 在临界区内更新模块故障计数值，防止中断干扰
 * 参数: FaultCnt - 模块故障计数值
 *********************************************************************/
void sSetModuleFaultCnt(INT16S FaultCnt)
{
    // 进入临界区：禁止中断
    OS_ENTER_CRITICAL();
    // 更新模块故障计数
    wModuleFaultCnt = FaultCnt;
    // 退出临界区：恢复中断
    OS_EXIT_CRITICAL();
}

/*********************************************************************
 * 函数名: swGetModuleFaultCnt
 * 功能: 获取模块故障计数
 * 返回: 模块故障计数值
 *********************************************************************/
INT16S swGetModuleFaultCnt(void)
{
    return(wModuleFaultCnt);
}

/*********************************************************************
 * 函数名: sSetOutputRlyFaultCnt
 * 功能: 设置输出继电器故障计数
 * 说明: 在临界区内更新输出继电器故障计数值
 * 参数: RlyFaultCnt - 输出继电器故障计数值
 *********************************************************************/
void sSetOutputRlyFaultCnt(INT16S RlyFaultCnt)
{
    // 进入临界区：禁止中断
    OS_ENTER_CRITICAL();
    // 更新输出继电器故障计数
    wOutputRlyFaultCnt = RlyFaultCnt;
    // 退出临界区：恢复中断
    OS_EXIT_CRITICAL();
}

/*********************************************************************
 * 函数名: swGetOutputRlyFaultCnt
 * 功能: 获取输出继电器故障计数
 * 返回: 输出继电器故障计数值
 *********************************************************************/
INT16S swGetOutputRlyFaultCnt(void)
{
    return(wOutputRlyFaultCnt);
}

/*********************************************************************
 * 函数名: sSetAcSwchOpFaultCnt
 * 功能: 设置交流开关操作故障计数
 * 说明: 在临界区内更新交流开关操作故障计数值
 * 参数: SwchOpFaultCnt - 交流开关操作故障计数值
 *********************************************************************/
void sSetAcSwchOpFaultCnt(INT16S SwchOpFaultCnt)
{
    // 进入临界区：禁止中断
    OS_ENTER_CRITICAL();
    // 更新交流开关操作故障计数
    wAcSwchOpFaultCnt = SwchOpFaultCnt;
    // 退出临界区：恢复中断
    OS_EXIT_CRITICAL();
}

/*********************************************************************
 * 函数名: swGetAcSwchOpFaultCnt
 * 功能: 获取交流开关操作故障计数
 * 返回: 交流开关操作故障计数值
 *********************************************************************/
INT16S swGetAcSwchOpFaultCnt(void)
{
    return(wAcSwchOpFaultCnt);
}

/*********************************************************************
 * 函数名: sSetInPowerTripCnt
 * 功能: 设置输入电源跳闸计数
 * 说明: 在临界区内更新输入电源跳闸计数值
 * 参数: wTripCntTemp - 输入电源跳闸计数值
 *********************************************************************/
void sSetInPowerTripCnt(INT16U wTripCntTemp)
{
    // 进入临界区：禁止中断
    OS_ENTER_CRITICAL();
    // 更新输入电源跳闸计数
    wInPowerTripCnt = wTripCntTemp;
    // 退出临界区：恢复中断
    OS_EXIT_CRITICAL();
}

/*********************************************************************
 * 函数名: swGetInPowerTripCnt
 * 功能: 获取输入电源跳闸计数
 * 返回: 输入电源跳闸计数值
 *********************************************************************/
INT16U swGetInPowerTripCnt(void)
{
    return(wInPowerTripCnt);
}

/*********************************************************************
 * 函数名: sSetVoltHighTripFlag
 * 功能: 设置过压跳闸标志
 * 说明: 更新过压跳闸标志值
 * 参数: wFlagTemp - 过压跳闸标志值
 *********************************************************************/
void sSetVoltHighTripFlag(INT16U wFlagTemp)
{
    // 更新过压跳闸标志
	wVoltHighTripFlag = wFlagTemp;
}

/*********************************************************************
 * 函数名: swGetVoltHighTripFlag
 * 功能: 获取过压跳闸标志
 * 返回: 过压跳闸标志值
 *********************************************************************/
INT16U swGetVoltHighTripFlag(void)
{
    // 返回过压跳闸标志
	return(wVoltHighTripFlag);
}

/*********************************************************************
 * 函数名: sInitialMovWinVar
 * 功能: 初始化移动窗口变量
 * 说明: 清零频率移动窗口数组及相关变量，初始化滤波参数
 *********************************************************************/
void sInitialMovWinVar(void)
{
    // 定义循环变量
	INT16S i;

    // 进入临界区：禁止中断
    OS_ENTER_CRITICAL();

    // 循环清零频率移动窗口数组（cFreMovWndSize为窗口大小）
    for(i=0;i<cFreMovWndSize;i++)
    {
        fFreMovWindowGrp[i] = 0;
    }
    // 清零频率移动窗口总和
    fFreMovGrpSum = 0;
    // 清零频率移动窗口总和（备用）
    fFreMovGrpSum_1 = 0;
    // 清零当前频率移动窗口索引
    fFreMovWindowNow = 0;
    // 清零频率移动窗口计数
    wFreMovWindowCnt = 0;
    // 清零频率计数周期
    wFreCntCycle = 0;

    // 初始化电网电压d轴正序滤波值
    psVarM2s->fVinvDxPosFilter = 0;
    // 初始化电网电压q轴正序滤波值
    psVarM2s->fVinvQxPosFilter = 0;
    // 初始化电网电压d轴负序滤波值
    psVarM2s->fVinvDxNegFilter = 0;
    // 初始化电网电压q轴负序滤波值
    psVarM2s->fVinvQxNegFilter = 0;

    // 退出临界区：恢复中断
    OS_EXIT_CRITICAL();
}

/*********************************************************************
 * 函数名: sSetOutVoltRation
 * 功能: 设置输出电压比例系数
 * 说明: 更新共享内存中的交流电压比例系数
 * 参数: AcVoltRation - 交流电压比例系数
 *********************************************************************/
void sSetOutVoltRation(float AcVoltRation)
{
    // 设置交流电压比例系数
	psVarM2s->fVgRatio = AcVoltRation;
}

/*********************************************************************
 * 函数名: sSetDCVoltRation
 * 功能: 设置直流电压比例系数
 * 说明: 更新共享内存中的直流电压比例系数
 * 参数: DcVoltRation - 直流电压比例系数
 *********************************************************************/
void sSetDCVoltRation(float DcVoltRation)
{
    // 设置直流电压比例系数
	psVarM2s->fVdcRatio = DcVoltRation;
}

/*********************************************************************
 * 函数名: sSetOutCurrRation
 * 功能: 设置输出电流比例系数
 * 说明: 更新共享内存中的交流电流比例系数
 * 参数: AcCurrRation - 交流电流比例系数
 *********************************************************************/
void sSetOutCurrRation(float AcCurrRation)
{
    // 设置交流电流比例系数
	psVarM2s->fIgRatio = AcCurrRation;
}

/*********************************************************************
 * 函数名: sSetDCCurrRation
 * 功能: 设置直流电流比例系数
 * 说明: 更新共享内存中的直流电流比例系数
 * 参数: DcCurrRation - 直流电流比例系数
 *********************************************************************/
void sSetDCCurrRation(float DcCurrRation)
{
    // 设置直流电流比例系数
	psVarM2s->fIdcRatio = DcCurrRation;
}

/*********************************************************************
 * 函数名: sSetLeakCurrRation
 * 功能: 设置漏电流比例系数
 * 说明: 更新共享内存中的漏电流比例系数
 * 参数: LeakCurrRation - 漏电流比例系数
 *********************************************************************/
void sSetLeakCurrRation(float LeakCurrRation)
{
    // 设置漏电流比例系数
	psVarM2s->fLeakRatio = LeakCurrRation;
}

/*********************************************************************
 * 函数名: sfGetFreMovGrpSum
 * 功能: 获取频率移动窗口总和
 * 返回: 频率移动窗口总和值
 *********************************************************************/
float sfGetFreMovGrpSum(void)
{
    return(fFreMovGrpSum);
}

/*********************************************************************
 * 函数名: sbGetFaultRecFlag
 * 功能: 获取故障记录标志
 * 返回: 故障记录标志值（整型）
 *********************************************************************/
INT8U sbGetFaultRecFlag(void)
{
    return(unFltRecFlag.word);
}

/*********************************************************************
 * 函数名: sClearFaultRecFlag
 * 功能: 清除故障记录标志
 * 说明: 在临界区内清零故障记录标志，防止中断干扰
 *********************************************************************/
void sClearFaultRecFlag(void)
{
    // 进入临界区：禁止中断
    OS_ENTER_CRITICAL();
    // 清零故障记录标志
    unFltRecFlag.word = 0;
    // 退出临界区：恢复中断
    OS_EXIT_CRITICAL();
}

/*********************************************************************
 * 函数名: sfGetFreqDelta
 * 功能: 获取频率偏差值
 * 说明: 根据采样参数计算电网频率偏差
 * 返回: 电网频率偏差值
 *********************************************************************/
float sfGetFreqDelta(void)
{
    // 定义临时存储电网频率的变量
    float fGridFreqTemp;

    /* 注释：原频率计算逻辑（按50/60Hz区分），当前未启用
    if(sbGetGridFreqType() == cRateFreq60Hz)
    {
    	fGridFreqTemp = fFgSum * fSamplTs * (1.44 * PI2_RCP);
    }
    else
    {
    	fGridFreqTemp = fFgSum * fSamplTs * (1.0 * PI2_RCP);
    }
    */

    // 频率偏差计算公式：fFgSum * 采样频率 / (移动窗口数 * 半窗口数 * 2π)
    fGridFreqTemp = fFgSum * (float)wSamplFrq /((float)wFreMovWindowNum * (float)wFreqWindowNumHalf * PI2);

    // 返回频率偏差值
    return(fGridFreqTemp);
}

/*********************************************************************
 * 函数名: swGetVgFreq
 * 功能: 获取电网频率值
 * 说明: 根据频率偏差计算实际电网频率（50/60Hz基准）
 * 返回: 电网频率值（整型，单位：0.01Hz）
 *********************************************************************/
INT16U swGetVgFreq(void)
{
    // 定义临时存储频率的变量
    INT16U wFreqTemp;

    // 判断电网频率类型是否为60Hz
    if(sbGetGridFreqType() == cRateFreq60Hz)
    {
        // 60Hz基准：60 + 频率偏差
    	fGridFreq = 60.0 + sfGetFreqDelta();
    }
    else
    {
        // 50Hz基准：50 + 频率偏差
    	fGridFreq = 50.0 + sfGetFreqDelta();
    }
    // 转换为整型（乘以100，保留2位小数）
    wFreqTemp = (INT16U)(fGridFreq * 100);

    // 返回电网频率值
    return(wFreqTemp);
}

/*********************************************************************
 * 函数名: sbGetGridDropFlag
 * 功能: 获取电网跌落标志
 * 返回: 电网跌落标志值（0/1）
 *********************************************************************/
INT8U sbGetGridDropFlag(void)
{
    return(bGridDropFlag);
}

/*********************************************************************
 * 函数名: sbGetZeroDropFlag
 * 功能: 获取零点跌落标志
 * 返回: 零点跌落标志值（0/1）
 *********************************************************************/
INT8U sbGetZeroDropFlag(void)
{
    return(bZeroDropFlag);
}

/*********************************************************************
 * 函数名: sbGetPllLossFlag
 * 功能: 获取PLL（锁相环）丢失标志
 * 返回: PLL丢失标志值（0/1）
 *********************************************************************/
INT8U sbGetPllLossFlag(void)
{
    return(bPllLossFlag);
}

/*********************************************************************
 * 函数名: sbGetStartUpSycOkFlag
 * 功能: 获取启动同步完成标志
 * 返回: 启动同步完成标志值（0/1）
 *********************************************************************/
INT8U sbGetStartUpSycOkFlag(void)
{
	return(bStartUpSycOkFlag);
}

/*********************************************************************
 * 函数名: sResetStartUpSycOkFlag
 * 功能: 重置启动同步完成标志
 * 说明: 将标志置为false
 *********************************************************************/
void sResetStartUpSycOkFlag(void)
{
	bStartUpSycOkFlag = false;
}

/*********************************************************************
 * 函数名: sbGetMainRlyOnOk
 * 功能: 获取主继电器吸合完成标志
 * 返回: 主继电器吸合完成标志值（0/1）
 *********************************************************************/
INT8U sbGetMainRlyOnOk(void)
{
	return(bMainRlyOnOk);
}

/*********************************************************************
 * 函数名: sResetMainRlyOnOk
 * 功能: 重置主继电器吸合完成标志
 * 说明: 将标志置为false
 *********************************************************************/
void sResetMainRlyOnOk(void)
{
	bMainRlyOnOk = false;
}

/*********************************************************************
 * 函数名: sfGetBusVoltPos
 * 功能: 获取正母线电压值
 * 说明: 返回正母线电压浮点值（注释中保留了备用读取方式）
 * 返回: 正母线电压值
 *********************************************************************/
float sfGetBusVoltPos(void)
{
	return(fBusVoltPos);
    // 注释：备用读取方式（从共享内存读取）
    //return(psMemS2m->fPBusVoltReal);
}

/*********************************************************************
 * 函数名: sfGetBusVoltNeg
 * 功能: 获取负母线电压值
 * 说明: 返回负母线电压浮点值（注释中保留了备用读取方式）
 * 返回: 负母线电压值
 *********************************************************************/
float sfGetBusVoltNeg(void)
{
	return(fBusVoltNeg);
    // 注释：备用读取方式（从共享内存读取）
    //return(psMemS2m->fNBusVoltReal);
}

/*********************************************************************
 * 函数名: sSetFreMovWindowNum
 * 功能: 设置频率移动窗口数量
 * 说明: 在临界区内更新半窗口数和总窗口数（总窗口数=半窗口数*2）
 * 参数: wWindowNumTemp - 半窗口数
 *********************************************************************/
void sSetFreMovWindowNum(INT16U wWindowNumTemp)
{
    // 进入临界区：禁止中断
    OS_ENTER_CRITICAL();

    // 设置频率窗口半数量
    wFreqWindowNumHalf = wWindowNumTemp;
    // 总窗口数 = 半窗口数 << 1（即乘以2）
	wFreMovWindowNum = (INT16U)(wFreqWindowNumHalf << 1);

    // 退出临界区：恢复中断
	OS_EXIT_CRITICAL();
}

/*********************************************************************
 * 函数名: sSetOneSinePrdPoint
 * 功能: 设置单个正弦周期采样点数
 * 说明: 在临界区内更新单个正弦周期采样点数
 * 参数: wPrdPointTemp - 单个正弦周期采样点数
 *********************************************************************/
void sSetOneSinePrdPoint(INT16U wPrdPointTemp)
{
    // 进入临界区：禁止中断
    OS_ENTER_CRITICAL();
    // 设置单个正弦周期采样点数
	wOneSinePrdPoint = wPrdPointTemp;
    // 退出临界区：恢复中断
    OS_EXIT_CRITICAL();
}

// Fault recorder channel online setting begin
// 故障录波通道在线配置 开始

/*********************************************************************
 * 宏定义: 数据类型枚举
 * FLOAT_TYPE - 浮点型
 * WORD_TYPE  - 字类型（16位整型）
 * DWORD_TYPE - 双字类型（32位整型）
 *********************************************************************/
#define FLOAT_TYPE    0
#define WORD_TYPE     1
#define DWORD_TYPE    2

/*********************************************************************
 * 全局数组: stC1ScopeDeal/stC2ScopeDeal
 * 功能: 故障录波通道处理结构体数组
 * 说明: 存储录波通道的数据类型、缩放系数等信息
 * 大小: cScopeSize（录波通道数量）
 *********************************************************************/
DATA_REC_DEAL stC1ScopeDeal[cScopeSize];
DATA_REC_DEAL stC2ScopeDeal[cScopeSize];

/*********************************************************************
 * 外部变量: wOutputFreq
 * 功能: 输出频率值
 *********************************************************************/
extern INT16U wOutputFreq;

/*********************************************************************
 * 函数名: swDataRecHdle
 * 功能: 数据录波处理函数
 * 说明: 根据数据类型读取录波数据，缩放后转换为整型
 * 参数: pdwScopeMem - 录波数据内存地址，pDataRecDeal - 录波处理结构体
 * 返回: 缩放后的整型录波数据
 *********************************************************************/
INT16S swDataRecHdle(INT32U *pdwScopeMem,DATA_REC_DEAL *pDataRecDeal)
{
    // 定义临时变量
    float  fTemp;    // 浮点临时变量
    INT16S wTemp;    // 16位整型临时变量
    INT32S dwTemp;   // 32位整型临时变量

    // 根据数据类型读取数据
    if(pDataRecDeal->bDataType == FLOAT_TYPE)
    {
        // 浮点型：复制2个字节（注：实际浮点型应为4字节，此处可能为平台特殊处理）
        memcpy((void *)(&fTemp),(void *)pdwScopeMem,2);
    }
    else if(pDataRecDeal->bDataType == WORD_TYPE)
    {
        // 字类型：复制1个字节，转换为浮点型
        memcpy((void *)(&wTemp),(void *)pdwScopeMem,1);
        fTemp = (float)wTemp;
    }
    else
    {
        // 双字类型：复制2个字节，转换为浮点型
        memcpy((void *)(&dwTemp),(void *)pdwScopeMem,2);
        fTemp = (float)dwTemp;
    }

    // 缩放后转换为16位整型并返回
    return((INT16S)(fTemp * pDataRecDeal->fDataZoom));
}

/*********************************************************************
 * 条件编译: AC通道配置（默认启用第一个配置）
 * 说明: 定义AC录波通道的数据源、缩放系数、数据类型
 *********************************************************************/
#if(1)
  // AC通道0：R相输出电压（Uab）
  #define ACCH0    (psMemS2m->fROutVolt)   
  // AC通道1：S相输出电压（Ubc）
  #define ACCH1    (psMemS2m->fSOutVolt)   
  // AC通道2：DSP状态机（Uca）
  #define ACCH2    (unDspFSM.word)         
  // AC通道3：R相输出电流1（Ia）
  #define ACCH3    (psMemS2m->fR1OutCurr)  
  // AC通道4：S相输出电流1（Ib）
  #define ACCH4    (psMemS2m->fS1OutCurr)  
  // AC通道5：T相输出电流1（Ic）
  #define ACCH5    (psMemS2m->fT1OutCurr)  
  // AC通道6：R相调制电压实部（Tmod1）
  #define ACCH6    (psMemS2m->fRVinvReal)  
  // AC通道7：S相调制电压实部（Tmod2）
  #define ACCH7    (psMemS2m->fSVinvReal)  
  // AC通道8：T相调制电压实部（Tmod3）
  #define ACCH8    (psMemS2m->fTVinvReal)  

  // AC通道缩放系数（乘以10，保留1位小数）
  #define ACCH0_ZOOM    10
  #define ACCH1_ZOOM    10
  #define ACCH2_ZOOM    1
  #define ACCH3_ZOOM    10
  #define ACCH4_ZOOM    10
  #define ACCH5_ZOOM    10
  #define ACCH6_ZOOM    10
  #define ACCH7_ZOOM    10
  #define ACCH8_ZOOM    10

  // AC通道数据类型
  #define ACCH0_TYPE    FLOAT_TYPE
  #define ACCH1_TYPE    FLOAT_TYPE
  #define ACCH2_TYPE    WORD_TYPE
  #define ACCH3_TYPE    FLOAT_TYPE
  #define ACCH4_TYPE    FLOAT_TYPE
  #define ACCH5_TYPE    FLOAT_TYPE
  #define ACCH6_TYPE    FLOAT_TYPE
  #define ACCH7_TYPE    FLOAT_TYPE
  #define ACCH8_TYPE    FLOAT_TYPE
#else
  // 备用AC通道配置（当前未启用）
  #define ACCH0    (psMemS2m->fROutVolt)   
  #define ACCH1    (psMemS2m->fSOutVolt)   
  #define ACCH2    (unDspFSM.word)         
  #define ACCH3    (psMemS2m->fR1OutCurr)  
  #define ACCH4    (psMemS2m->fVgAmpNeg)   
  #define ACCH5    (psMemS2m->fVoltQxErrInteg)    
  #define ACCH6    (psMemS2m->unGridDropSts.all)  
  #define ACCH7    (psMemS2m->fVgDxPosFilter)     
  #define ACCH8    (psMemS2m->fVgAmp)            

  #define ACCH0_ZOOM    10
  #define ACCH1_ZOOM    10
  #define ACCH2_ZOOM    1
  #define ACCH3_ZOOM    10
  #define ACCH4_ZOOM    10
  #define ACCH5_ZOOM    10
  #define ACCH6_ZOOM    1
  #define ACCH7_ZOOM    10
  #define ACCH8_ZOOM    10

  #define ACCH0_TYPE    FLOAT_TYPE
  #define ACCH1_TYPE    FLOAT_TYPE
  #define ACCH2_TYPE    WORD_TYPE
  #define ACCH3_TYPE    FLOAT_TYPE
  #define ACCH4_TYPE    FLOAT_TYPE
  #define ACCH5_TYPE    FLOAT_TYPE
  #define ACCH6_TYPE    WORD_TYPE
  #define ACCH7_TYPE    FLOAT_TYPE
  #define ACCH8_TYPE    FLOAT_TYPE
#endif

/*********************************************************************
 * 条件编译: DC通道配置（默认启用第一个配置）
 * 说明: 定义DC录波通道的数据源、缩放系数、数据类型
 *********************************************************************/
#if(1)
  // DC通道0：直流电压（Udc）
  #define DCCH0    (psMemS2m->fDCVolt)     
  // DC通道1：直流电流（IpvA）
  #define DCCH1    (psMemS2m->fDCCurr)     
  // DC通道2：输出频率（Fgrid）
  #define DCCH2    (wOutputFreq)           
  // DC通道3：母线正负压差（Ileak）
  #define DCCH3    (psMemS2m->fBusPNDiff)  

  // DC通道缩放系数
  #define DCCH0_ZOOM    10
  #define DCCH1_ZOOM    10
  #define DCCH2_ZOOM    1
  #define DCCH3_ZOOM    10

  // DC通道数据类型
  #define DCCH0_TYPE    FLOAT_TYPE
  #define DCCH1_TYPE    FLOAT_TYPE
  #define DCCH2_TYPE    WORD_TYPE
  #define DCCH3_TYPE    FLOAT_TYPE
#else
  // 备用DC通道配置（当前未启用）
  #define DCCH0    (psMemS2m->fDCVolt)     
  #define DCCH1    (psMemS2m->fDCCurr)     
  #define DCCH2    (wOutputFreq)           
  #define DCCH3    (psMemS2m->fBusPNDiff)  

  #define DCCH0_ZOOM    10
  #define DCCH1_ZOOM    10
  #define DCCH2_ZOOM    1
  #define DCCH3_ZOOM    10

  #define DCCH0_TYPE    FLOAT_TYPE
  #define DCCH1_TYPE    FLOAT_TYPE
  #define DCCH2_TYPE    WORD_TYPE
  #define DCCH3_TYPE    FLOAT_TYPE
#endif

/*********************************************************************
 * 函数名: sDataRecSel
 * 功能: 数据录波通道选择函数
 * 说明: 配置录波通道的内存地址、数据类型、缩放系数，清零故障记录标志
 *********************************************************************/
void sDataRecSel(void)
{
    // 配置C2Scope通道0：ACCH3（Ia）
    psVarM2s->C2ScopeAddr[cScopeCH0] = (INT32U)(&ACCH3);
    stC2ScopeDeal[cScopeCH0].bDataType = ACCH3_TYPE;
	stC2ScopeDeal[cScopeCH0].fDataZoom = ACCH3_ZOOM;

    // 配置C2Scope通道1：ACCH4（Ib）
    psVarM2s->C2ScopeAddr[cScopeCH1] = (INT32U)(&ACCH4);
    stC2ScopeDeal[cScopeCH1].bDataType = ACCH4_TYPE;
	stC2ScopeDeal[cScopeCH1].fDataZoom = ACCH4_ZOOM;

    // 配置C2Scope通道2：ACCH5（Ic）
    psVarM2s->C2ScopeAddr[cScopeCH2] = (INT32U)(&ACCH5);
    stC2ScopeDeal[cScopeCH2].bDataType = ACCH5_TYPE;
	stC2ScopeDeal[cScopeCH2].fDataZoom = ACCH5_ZOOM;

    // 配置C2Scope通道3：DCCH0（Udc）
    psVarM2s->C2ScopeAddr[cScopeCH3] = (INT32U)(&DCCH0);
    stC2ScopeDeal[cScopeCH3].bDataType = DCCH0_TYPE;
	stC2ScopeDeal[cScopeCH3].fDataZoom = DCCH0_ZOOM;

    // 配置C2Scope通道4：DCCH1（IpvA）
    psVarM2s->C2ScopeAddr[cScopeCH4] = (INT32U)(&DCCH1);
    stC2ScopeDeal[cScopeCH4].bDataType = DCCH1_TYPE;
	stC2ScopeDeal[cScopeCH4].fDataZoom = DCCH1_ZOOM;

    // 配置Scope通道0：ACCH6（Tmod1）
    ScopeAddr[cScopeCH0] = (INT32U)(&ACCH6);
    stC1ScopeDeal[cScopeCH0].bDataType = ACCH6_TYPE;
	stC1ScopeDeal[cScopeCH0].fDataZoom = ACCH6_ZOOM;

    // 配置Scope通道1：ACCH7（Tmod2）
    ScopeAddr[cScopeCH1] = (INT32U)(&ACCH7);
    stC1ScopeDeal[cScopeCH1].bDataType = ACCH7_TYPE;
	stC1ScopeDeal[cScopeCH1].fDataZoom = ACCH7_ZOOM;

    // 配置Scope通道2：ACCH8（Tmod3）
    ScopeAddr[cScopeCH2] = (INT32U)(&ACCH8);
    stC1ScopeDeal[cScopeCH2].bDataType = ACCH8_TYPE;
	stC1ScopeDeal[cScopeCH2].fDataZoom = ACCH8_ZOOM;

    // 配置Scope通道3：DCCH2（Fgrid）
    ScopeAddr[cScopeCH3] = (INT32U)(&DCCH2);
    stC1ScopeDeal[cScopeCH3].bDataType = DCCH2_TYPE;
	stC1ScopeDeal[cScopeCH3].fDataZoom = DCCH2_ZOOM;

    // 配置Scope通道4：DCCH3（Ileak）
    ScopeAddr[cScopeCH4] = (INT32U)(&DCCH3);
    stC1ScopeDeal[cScopeCH4].bDataType = DCCH3_TYPE;
	stC1ScopeDeal[cScopeCH4].fDataZoom = DCCH3_ZOOM;

    // 清零故障记录标志
	unFltRecFlag.word = 0;
}

/*********************************************************************
 * 函数名: sAcDataCopy
 * 功能: AC数据复制函数
 * 说明: 组装DSP状态机数据，根据密码状态复制AC录波数据到缓冲区
 * 参数: pwAcBufTemp - AC数据缓冲区指针
 *********************************************************************/
void sAcDataCopy(INT16S *pwAcBufTemp)
{
    // 清零DSP状态机
    unDspFSM.word = 0;

    // 填充DSP状态机各位数据
    // CPLD PWM寄存器值
    unDspFSM.bit.CpldPwm = psMemS2m->unCpldReg[1].word;
    // DC开关状态
    unDspFSM.bit.DcSwSts = hiDcSwSts;
    // DC开关命令
    unDspFSM.bit.DcSwCmd = wDcSwCmd;
    // 交流断路器状态
    unDspFSM.bit.AcBrkerSts = AC_BRK_STS;
    // 主继电器状态
    unDspFSM.bit.MainRlySts = AC_RLY_STS;
    // 辅助继电器命令
    unDspFSM.bit.AuxiRlyCmd = hoAuxiRlyCmd;
    // 主继电器命令
    unDspFSM.bit.MainRlyCmd = hoAcLinkCmd;

    // 状态机高4位取反（可能为通信校验）
    unDspFSM.word ^= 0xF000;

    // 判断录波密码是否正确（未解锁）
    if(psVarM2s->ScopePassWord != cScopePassKey)
    {
        // 直接缩放后复制到缓冲区
        pwAcBufTemp[0] = (INT16S)(ACCH0 * ACCH0_ZOOM);  // Uab
        pwAcBufTemp[1] = (INT16S)(ACCH1 * ACCH1_ZOOM);  // Ubc
        pwAcBufTemp[2] = (INT16S)(ACCH2 * ACCH2_ZOOM);  // Uca
        pwAcBufTemp[3] = (INT16S)(ACCH3 * ACCH3_ZOOM);  // Ia
        pwAcBufTemp[4] = (INT16S)(ACCH4 * ACCH4_ZOOM);  // Ib
        pwAcBufTemp[5] = (INT16S)(ACCH5 * ACCH5_ZOOM);  // Ic
        pwAcBufTemp[6] = (INT16S)(ACCH6 * ACCH6_ZOOM);  // Tmod1
        pwAcBufTemp[7] = (INT16S)(ACCH7 * ACCH7_ZOOM);  // Tmod2
        pwAcBufTemp[8] = (INT16S)(ACCH8 * ACCH8_ZOOM);  // Tmod3
    }
    else
    {
        // 密码正确（已解锁）：前3通道直接复制，后6通道调用录波处理函数
        pwAcBufTemp[0] = (INT16S)(ACCH0 * ACCH0_ZOOM);  // Uab
        pwAcBufTemp[1] = (INT16S)(ACCH1 * ACCH1_ZOOM);  // Ubc
        pwAcBufTemp[2] = (INT16S)(ACCH2 * ACCH2_ZOOM);  // Uca

        // 调用录波处理函数获取通道数据
        pwAcBufTemp[3] = swDataRecHdle(psMemS2m->C2ScopeMem+cScopeCH0,stC2ScopeDeal+cScopeCH0);  // Ia
        pwAcBufTemp[4] = swDataRecHdle(psMemS2m->C2ScopeMem+cScopeCH1,stC2ScopeDeal+cScopeCH1);  // Ib
        pwAcBufTemp[5] = swDataRecHdle(psMemS2m->C2ScopeMem+cScopeCH2,stC2ScopeDeal+cScopeCH2);  // Ic
        pwAcBufTemp[6] = swDataRecHdle(ScopeMem+cScopeCH0,stC1ScopeDeal+cScopeCH0);              // Tmod1
        pwAcBufTemp[7] = swDataRecHdle(ScopeMem+cScopeCH1,stC1ScopeDeal+cScopeCH1);              // Tmod2
        pwAcBufTemp[8] = swDataRecHdle(ScopeMem+cScopeCH2,stC1ScopeDeal+cScopeCH2);              // Tmod3
    }
}

/*********************************************************************
 * 函数名: sDcDataCopy
 * 功能: DC数据复制函数
 * 说明: 根据密码状态复制DC录波数据到缓冲区
 * 参数: pwDcBufTemp - DC数据缓冲区指针
 *********************************************************************/
void sDcDataCopy(INT16S *pwDcBufTemp)
{
    // 判断录波密码是否正确（未解锁）
    if(psVarM2s->ScopePassWord != cScopePassKey)
    {
        // 直接缩放后复制到缓冲区
        pwDcBufTemp[0] = (INT16S)(DCCH0 * DCCH0_ZOOM);  // Udc
        pwDcBufTemp[1] = (INT16S)(DCCH1 * DCCH1_ZOOM);  // IpvA
        pwDcBufTemp[2] = (INT16S)(DCCH2 * DCCH2_ZOOM);  // Fgrid
        pwDcBufTemp[3] = (INT16S)(DCCH3 * DCCH3_ZOOM);  // Ileak
    }
    else
    {
        // 密码正确（已解锁）：调用录波处理函数获取通道数据
        pwDcBufTemp[0] = swDataRecHdle(psMemS2m->C2ScopeMem+cScopeCH3,stC2ScopeDeal+cScopeCH3);  // Udc
        pwDcBufTemp[1] = swDataRecHdle(psMemS2m->C2ScopeMem+cScopeCH4,stC2ScopeDeal+cScopeCH4);  // IpvA
        pwDcBufTemp[2] = swDataRecHdle(ScopeMem+cScopeCH3,stC1ScopeDeal+cScopeCH3);              // Fgrid
        pwDcBufTemp[3] = swDataRecHdle(ScopeMem+cScopeCH4,stC1ScopeDeal+cScopeCH4);              // Ileak
    }
}
// Fault recorder channel online setting end
// 故障录波通道在线配置 结束

/*********************************************************************
 * 函数名: sUpdateFastCtrlInfo
 * 功能: 更新快速控制信息
 * 说明: 组装运行状态寄存器，计算有功/无功功率
 *********************************************************************/
void sUpdateFastCtrlInfo(void)
{
    // 设置运行状态寄存器：工作模式标志（是否为工作模式）
	unRunStsReg.bit.WorkingSts = swGetMode() == cWorkingMode;

    // 设置HVRT（高压穿越）状态标志
	unRunStsReg.bit.HvrtSts = psMemS2m->unGridDropSts.bit.wGridDrop == cHvrt;

    // 设置LVRT（低压穿越）状态标志
	unRunStsReg.bit.LvrtSts = psMemS2m->unGridDropSts.bit.wGridDrop == cLvrt;

    // 设置有功功率限制状态标志（电压环饱和）
	unRunStsReg.bit.ActPwrLimtSts = bVoltLoopSatFlag == true;

    // 设置交流断路器状态标志（是否闭合）
	unRunStsReg.bit.AcBrkSts = cLoLev == AC_BRK_STS;

    // 判断是否为工作模式
    if(swGetMode() == cWorkingMode)
    {
        // 计算有功功率：1.5 * 电网电压d轴正序 * 电流d轴正序
        dwActivePwr = (INT32S)(1.5 * psMemS2m->fVgDxPosFilter * psMemS2m->fIgDxPosFilter);
        // 计算无功功率：1.5 * 电网电压d轴正序 * (电流q轴正序 - 电容电流补偿项)
        dwReactivePwr = (INT32S)(1.5 * psMemS2m->fVgDxPosFilter * (psMemS2m->fIgQxPosFilter - (psVarM2s->fKCapCurr * psVarM2s->fVgPhPkRtd)));
    }
    else
    {
        // 非工作模式：有功/无功功率置0
        dwActivePwr = 0;
        dwReactivePwr = 0;
    }
}


