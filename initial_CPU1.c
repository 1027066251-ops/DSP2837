


//系统 CPU 1初始化模块
//initial_CPU.c 是整个 DSP 的 CPU 启动初始化文件，负责 CPU 核心环境、系统时钟、Flash/RAM、外设时钟、中断控制器、全局变量、状态机等基础启动流程的配置，
//是整个系统运行的起点。



// 定义全局变量初始化宏，用于标识当前文件执行全局变量的初始化逻辑
#define INITIAL_GLOBALS

// 引入应用层头文件，包含工程自定义的宏、结构体、函数声明等核心接口
#include "App\include\head.h"
// 引入F2837xD双核间IPC（处理器间通信）驱动库头文件，提供双核通信相关函数/寄存器定义
#include "F2837xD_Ipc_drivers.h"

/******************************************************************************************
* 函数名: sInitCpu
* 功能:   CPU1核心初始化总入口函数，完成双核资源分配、系统控制、外设初始化、中断配置、
*         双核同步、参数初始化等所有CPU1启动前的核心配置
* 输入:   无
* 输出:   无
* 注意:   该函数为CPU1上电后首个核心初始化函数，执行顺序严格不可随意调整
******************************************************************************************/
void sInitCpu(void)
{
    // 调用内存保护初始化函数，配置CPU的内存访问保护权限，防止非法内存读写
    sMemProt();

    // Step 1. 配置双核共享内存资源：分配GS-RAM（全局共享RAM）给CPU1和CPU2，划分私有/共享内存区域
    sAssignGSRam();

    // Step 2. 初始化系统控制模块：配置PLL锁相环（系统时钟倍频）、看门狗、使能外设时钟
    // 该示例函数在F2837xD_SysCtrl.c库文件中实现，为TI官方标准系统时钟配置函数
    sInitSysCtrl();

    // 解锁DCSM（数据加密安全模块）Z1区域，DCSM锁定时会限制部分寄存器/内存的访问权限，
    // 初始化阶段需先解锁才能进行后续配置
    DcsmZ1Unlock();

    // Step 3. 初始化GPIO模块：配置GPIO的输入/输出、上拉/下拉、复用功能等
    // 注：当前代码注释掉该函数，GPIO功能配置延后至Step 11执行（配合PWM启动时序要求）
    // InitGpio();
	
    // Step 4. 清除所有中断标志并禁用CPU中断，防止初始化过程中异常中断触发
    // 禁用CPU全局中断（INTM位），屏蔽所有可屏蔽中断
    DINT;

    // 初始化PIE（外设中断扩展）控制寄存器为默认状态：所有PIE中断禁用、所有中断标志位清零
    // 该函数在F2837xD_PieCtrl.c库文件中实现，为TI官方标准PIE控制器初始化函数
    InitPieCtrl();

    // 清空CPU中断使能寄存器IER，禁用所有CPU级中断
    IER = 0x0000;
    // 清空CPU中断标志寄存器IFR，清除所有未处理的CPU级中断标志
    IFR = 0x0000;

    // 分配双核外设资源：通过DevCfg寄存器配置每个外设归CPU1/CPU2独有控制，避免双核外设抢占
    sAssignPeripheral();

    // 全局EPWM同步初始化：配置EPWM模块的同步信号、时基同步模式，保证多EPWM通道时序一致
    sGlobalSyncEPwm();

    // 开启EALLOW保护（TI DSP特有的寄存器保护机制）：
    // 部分核心寄存器（如系统控制、GPIO、EPWM）写操作前需EALLOW，防止误写；操作后需EDIS关闭
    EALLOW;
    // 使能DSP内部温度传感器：配置模拟子系统寄存器TSNSCTL的ENABLE位为1，启动温度采集功能
    AnalogSubsysRegs.TSNSCTL.bit.ENABLE = 1;
    // 关闭EALLOW保护，恢复寄存器写保护
    EDIS;

    // Step 5. 启动CPU2核心：仅在_STANDALONE（独立运行）宏定义时执行，区分调试/量产模式
    #ifdef _STANDALONE
        // 复位看门狗计数器（喂狗），防止初始化过程中看门狗超时复位
        // ServiceDog();
        // 使能看门狗模块
        // EALLOW;
        // WdRegs.WDCR.all = 0x002F;  // 配置看门狗分频系数WDPS=111b，看门狗使能（0x0028为其他分频配置）
        // EDIS;

        // 根据_FLASH宏判断启动介质，分别从FLASH/RAM启动CPU2
        #ifdef _FLASH
    	    // 发送CPU2启动命令，指定CPU2从FLASH中启动应用程序
    	    // C1C2_BROM_BOOTMODE_BOOT_FROM_FLASH为TI定义的FLASH启动模式宏
    	    IPCBootCPU2(C1C2_BROM_BOOTMODE_BOOT_FROM_FLASH);

    	    // 设置IPC_FLAG18标志（CPU1到CPU2的标志位），通知CPU2从BootLoader跳转到应用程序执行
    	    IPCLtoRFlagSet(IPC_FLAG18);
        #else
    	    // 发送CPU2启动命令，指定CPU2从RAM中启动应用程序（调试模式常用，下载速度快）
    	    IPCBootCPU2(C1C2_BROM_BOOTMODE_BOOT_FROM_RAM);
        #endif
    #endif

    // Step 6. 初始化PIE中断向量表：将中断服务函数（ISR）的地址填充到PIE向量表中
    // 即使当前工程未使用某中断，也会填充默认ISR，便于调试时定位未处理中断
    // 默认ISR在F2837xD_DefaultIsr.c中实现，该初始化函数在F2837xD_PieVect.c中实现
    sInitPieVectTable();

    // 开启EALLOW保护，修改PIE向量表（向量表属于受保护寄存器，需EALLOW才能写）
    EALLOW;

    // 将用户自定义陷阱12的中断服务函数OSCtxSw（操作系统上下文切换函数）挂载到PIE向量表USER12_INT位置
    PieVectTable.USER12_INT = &OSCtxSw;  // User Defined Trap 12

    // 定时器0中断挂载OSTickISR（操作系统时钟节拍中断服务函数），对应PIE组1的第7个中断（1.7）
    PieVectTable.TIMER0_INT = &OSTickISR;                  // 1.7
    // IPC0中断挂载sCPU02toCPU01IPC0IntHandler（CPU2到CPU1的IPC0中断处理函数），对应PIE组1的第13个中断（1.13）
    PieVectTable.IPC0_INT = &sCPU02toCPU01IPC0IntHandler;  // 1.13

    // EPWM10周期中断挂载EPWM_Prd_isr_copy（EPWM10周期匹配中断服务函数），对应PIE组3的第9个中断（3.9）
    PieVectTable.EPWM10_INT = &EPWM_Prd_isr_copy;   // 3.9
    // 注释：EPWM10下溢中断挂载EPWM_Udf_isr_copy（EPWM10下溢匹配中断服务函数），对应PIE组3的第10个中断（3.10）
    //PieVectTable.EPWM10_INT = &EPWM_Udf_isr_copy;  // 3.10

    // 注释：ECAP2中断挂载ECAP1_ISR（捕获2中断服务函数），对应PIE组4的第2个中断（4.2）
	//PieVectTable.ECAP2_INT = &ECAP1_ISR;  // 4.2

    // SCIA接收中断挂载sSciaMsgRxIsr（串口A接收消息中断服务函数），对应PIE组9的第1个中断（9.1）
 	PieVectTable.SCIA_RX_INT = &sSciaMsgRxIsr;  // 9.1
    // SCIA发送中断挂载sSciaMsgTxIsr（串口A发送消息中断服务函数），对应PIE组9的第2个中断（9.2）
    PieVectTable.SCIA_TX_INT = &sSciaMsgTxIsr;  // 9.2
    // SCIB接收中断挂载sScibMsgRxIsr（串口B接收消息中断服务函数），对应PIE组9的第3个中断（9.3）
    PieVectTable.SCIB_RX_INT = &sScibMsgRxIsr;  // 9.3
    // SCIB发送中断挂载sScibMsgTxIsr（串口B发送消息中断服务函数），对应PIE组9的第4个中断（9.4）
    PieVectTable.SCIB_TX_INT = &sScibMsgTxIsr;  // 9.4
    // DCANA1中断挂载sCanaProtIsr（CAN A协议层中断服务函数），对应PIE组9的第5个中断（9.5）
    PieVectTable.DCANA_1_INT = &sCanaProtIsr;   // 9.5
    // DCANA2中断挂载sCanaMsgIsr（CAN A消息层中断服务函数），对应PIE组9的第6个中断（9.6）
    PieVectTable.DCANA_2_INT = &sCanaMsgIsr;    // 9.6
    // DCANB2中断挂载sCanbMsgRxIsr（CAN B消息接收中断服务函数），对应PIE组9的第8个中断（9.8）
    PieVectTable.DCANB_2_INT = &sCanbMsgRxIsr;  // 9.8

    // 定时器2中断挂载Timer2Isr（定时器2中断服务函数），对应PIE组14的中断（14）
    PieVectTable.TIMER2_INT = &Timer2Isr;  // 14

    // 关闭EALLOW保护，完成PIE向量表修改
    EDIS;

    // Step 8. 初始化关键参数：配置PWM相关的核心参数（频率、周期、采样率等）
    sPwmParasInit();

    // 初始化消息RAM：配置CAN模块的消息RAM（存储CAN收发报文），设置报文缓冲区、过滤表等
    sInitMsgRam();

    // 初始化CPU1固件版本号，将bCpu1FwVer（CPU1版本号数组）写入对应存储区域
	sInitFwVer(bCpu1FwVer);
	// 初始化CPLD固件版本号，将bCPLDFwVer（CPLD版本号数组）写入对应存储区域
	sInitCpldVer(bCPLDFwVer);
	// 初始化分支固件版本号，将bBrachFwVer（分支版本号数组）写入对应存储区域
	sInitBranchVer(bBrachFwVer);
	// 初始化交流故障检测固件版本号，将bAcIsuDetFwVer（交流故障检测版本号数组）写入对应存储区域
	sInitAcIsuDetVer(bAcIsuDetFwVer);

    // 初始化工程通用参数：配置系统运行的基础参数（如工作模式、保护阈值、校准参数等）
    sInitParas();

    // Step 9. 循环等待CPU2完成初始化：检测IPC_FLAG17标志（CPU2到CPU1的忙标志），
    // 直到CPU2置位该标志，表示CPU2初始化完成，防止双核同时初始化外设导致资源冲突
    while(!IPCRtoLFlagBusy(IPC_FLAG17));

    // Step 10. 初始化设备外设：双核初始化完成后，单独初始化CPU1管辖的外设，
    // 注：禁止双核同时初始化外设，避免共享寄存器（如CLKSEM）抢占
	// 初始化CPU定时器0，配置定时器0的周期、分频、中断使能等参数
	sInitCpuTimer0();
	// 启动CPU定时器0，开始计数
	StartCpuTimer0();

	// 初始化EPWM模块：配置CPU1管辖的EPWM通道（时基、比较、动作、死区等）
	sInitEPwm();
	// 全局启动EPWM计数器：在GPIO配置前启动PWM计数（时序要求，Liangc20170103添加注释）
	sGlobalStartEpwm();  // Liangc20170103, start pwm cnt before GPIO configured

	// 注释：初始化增强型捕获模块（ECapture），当前工程暂未使用
	//sInitECapture();

	// 初始化ECAP模块：配置增强型捕获模块的捕获模式、触发条件、中断等
	sInitECap();

    // 初始化SCI模块：配置串口A/B的波特率、数据位、停止位、校验位、中断等
    sInitSci();

    // 初始化I2C模块：配置I2C总线的时钟、地址、通信模式等
    sInitI2C();

    // 初始化SPI硬件模块：配置SPI的时钟极性、相位、数据位、主从模式等
    sInitSpiHw();

	// 初始化EQEP模块：配置增强型正交编码模块，用于电机转速/位置检测
	sInitEQep();

    // 初始化DAC A模块：配置DAC A的输出范围、触发方式、初始值等，cDacaId为DAC A设备ID
    sInitDac(cDacaId);
    // 注释：初始化DAC B模块，当前工程暂未使用
    //sInitDac(cDacbId);
    // 注释：初始化DAC C模块，当前工程暂未使用
    //sInitDac(cDaccId);

    // 延时100微秒：CPU定时器2与定时器0为异步时钟，需延时保证定时器2配置稳定（Liangc20160726添加注释）
    DELAY_US(100);     // Liangc20160726, cpu timer2 asynchronous with timer0
    // 初始化CPU定时器2，配置定时器2的周期、分频、中断使能等参数
    sInitCpuTimer2();
    // 启动CPU定时器2，开始计数
    StartCpuTimer2();

    // Step 11. 配置GPIO功能：设置GPIO的输入/输出、复用功能、中断等（延后配置配合PWM时序）
	sSetGpio();

    // 解锁CPLD的PDP保护：CPLD的功率器件保护（PDP）默认锁定，需解锁才能进行后续操作
    mCpldPdpUnlock();

    // 置位CPLD的WE（写使能）位：允许CPLD更新拨码开关的配置，将拨码开关的硬件配置写入CPLD寄存器
    mCpldWE;        // CPLD update dial switch

    // 延时500微秒：等待外设完成初始化，保证CPLD、传感器、功率器件等外设进入正常工作状态
    DELAY_US(500);  // wait peripherals normal

    // 置位CPLD的WD（写禁止）位：锁定CPLD的拨码开关配置，防止后续误操作修改硬件配置
    mCpldWD;        // CPLD lock dial switch

    // Step 12. 通知CPU2可以开始运行：清除/确认IPC_FLAG17标志，告知CPU1已完成外设初始化，CPU2可执行业务逻辑
    IPCRtoLFlagAcknowledge(IPC_FLAG17);

    // 延时1000微秒：等待CPU2完成数据更新，保证双核间共享数据的一致性
    DELAY_US(1000);  // CPU02 updating data

    // 注释：读取CPLD固件版本号，当前工程暂未在初始化阶段执行，延后至业务逻辑中读取
    //sReadCpldFwVer();

    // 注释：更新模块类型，wModuleSize（模块尺寸）依赖通信模块的更新，无需初始化阶段配置
    //sModuleTypeUpdate();  // wModuleSize relys on the update from communication and does not need initialization.
    // 更新模块ID：将硬件/配置的模块ID写入系统参数，用于设备识别、通信标识
    sModuleIdUpdate();
	// 初始化CAN A模块：配置CAN A的波特率、模式、过滤、中断等，完成CAN A通信初始化
	sInitCana();
	// 初始化CAN B模块：配置CAN B的波特率、模式、过滤、中断等，完成CAN B通信初始化
	sInitCanb();

    // 从FRAM（铁电存储器）读取设备类型：FRAM为掉电非易失存储器，存储设备硬件类型参数
	sFramReadMachType();
	// 从FRAM读取校准系数：读取传感器、功率器件的校准系数，用于系统精度补偿
	sFramReadCoeff();
    // 从FRAM读取关键参数：读取系统运行的关键配置参数（如保护阈值、工作参数等）
	sFramReadKeyParas();

    // 解析FRAM中的设备类型参数：将FRAM读取的原始数据解析为系统可识别的设备类型枚举/变量
    sDecdeFramMachType();
    // 解析FRAM中的控制参数：将FRAM读取的原始数据解析为系统控制用的参数（如PI参数、阈值等）
    sDecdeCtrlParas();

    // 将设备版本信息写入FRAM：将CPU1、CPLD等版本号写入FRAM，实现版本信息掉电保存
    sFramWriteDevVer();
    // 将IAP（在应用编程）信息写入FRAM：存储IAP升级的相关配置（如升级标志、目标地址等）
    sFramWriteIapInfo();
    // 设置FRAM中的IAP核心标识为0xFFFF：初始化IAP升级的核心选择标志，0xFFFF表示未指定升级核心
    sFramSetIapCore(0xFFFF);

    // 注释：直接配置CPU中断使能寄存器IER，使能指定PIE组的中断（当前改用逐位使能，更灵活）
    // IER = M_INT1|M_INT3|M_INT9|M_INT12|M_INT14;

    // 使能PIE组1的INTx7中断：对应定时器0中断（OSTickISR，操作系统节拍）
    PieCtrlRegs.PIEIER1.bit.INTx7 = 1;   // TIMER0_INT
    // 使能PIE组1的INTx13中断：对应IPC0中断（CPU2到CPU1的通信中断）
    PieCtrlRegs.PIEIER1.bit.INTx13 = 1;  // IPC0_INT

    // 使能PIE组3的INTx10中断：对应EPWM10中断（周期/下溢中断）
    PieCtrlRegs.PIEIER3.bit.INTx10 = 1;     // EPWM10_INT
    // 注释：重复使能PIE组3的INTx10中断，无实际意义，保留原始代码
    //PieCtrlRegs.PIEIER3.bit.INTx10 = 1;  // EPWM10_INT

    // 注释：使能PIE组4的INTx2中断：对应ECAP2中断，当前工程暂未使用
    //PieCtrlRegs.PIEIER4.bit.INTx2 = 1;  // ECAP2_INT
  // 条件编译：当BOARD_TYPE（板卡类型）不等于cTNPC时，使能SCIA的收发中断
  #if(BOARD_TYPE != cTNPC)
    // 使能PIE组9的INTx1中断：对应SCIA接收中断
    PieCtrlRegs.PIEIER9.bit.INTx1 = 1;    // SCIA_RX_INT
    // 使能PIE组9的INTx2中断：对应SCIA发送中断
    PieCtrlRegs.PIEIER9.bit.INTx2 = 1;    // SCIA_TX_INT
  #endif
    // 使能PIE组9的INTx3中断：对应SCIB接收中断
    PieCtrlRegs.PIEIER9.bit.INTx3 = 1;    // SCIB_RX_INT
    // 使能PIE组9的INTx4中断：对应SCIB发送中断
    PieCtrlRegs.PIEIER9.bit.INTx4 = 1;    // SCIB_TX_INT
    // 注释：使能PIE组9的INTx5中断：对应DCANA1中断，当前工程暂未使用
    //PieCtrlRegs.PIEIER9.bit.INTx5 = 1;  // DCANA_1_INT
    // 使能PIE组9的INTx6中断：对应DCANA2中断（CAN A消息中断）
    PieCtrlRegs.PIEIER9.bit.INTx6 = 1;    // DCANA_2_INT
    // 使能PIE组9的INTx8中断：对应DCANB2中断（CAN B消息接收中断）
    PieCtrlRegs.PIEIER9.bit.INTx8 = 1;    // DCANB_2_INT

    // 使能PIE组12的INTx12中断：对应RAM访问违例中断，用于检测非法内存访问（调试/故障定位）
    PieCtrlRegs.PIEIER12.bit.INTx12 = 1;  // RAM_ACCESS_VIOLATION_INT

    // 执行AD校准：对ADC模块进行零点、增益校准，补偿硬件误差，提高AD采样精度
    sADCalibration();

	// 注释：使能CPU全局中断和高优先级实时调试中断，当前工程暂未使能（延后至系统启动阶段）
    // EINT;   // Enable Global interrupt INTM：开启CPU全局可屏蔽中断
    // ERTM;   // Enable Global realtime interrupt DBGM：开启实时调试中断，调试时允许中断打断
}

/******************************************************************************************
* 函数名: sAssignPeripheral
* 功能:   双核外设资源分配函数，通过DevCfg寄存器配置每个外设的主控CPU（0=CPU1，1=CPU2）
*         避免双核同时访问外设导致的资源冲突，是F2837xD双核开发的核心步骤
* 输入:   无
* 输出:   无
* 注意:   外设分配后不可动态修改，需在初始化阶段一次性配置完成
******************************************************************************************/
void sAssignPeripheral(void)
{
    // 0表示外设归CPU1主控，1表示外设归CPU2主控
    // 开启EALLOW保护，DevCfg寄存器为受保护寄存器，需EALLOW才能写操作
    EALLOW;

    // 配置EPWM1~EPWM9归CPU2主控
    DevCfgRegs.CPUSEL0.bit.EPWM1 = 1;
    DevCfgRegs.CPUSEL0.bit.EPWM2 = 1;
    DevCfgRegs.CPUSEL0.bit.EPWM3 = 1;
    DevCfgRegs.CPUSEL0.bit.EPWM4 = 1;
    DevCfgRegs.CPUSEL0.bit.EPWM5 = 1;
    DevCfgRegs.CPUSEL0.bit.EPWM6 = 1;
    DevCfgRegs.CPUSEL0.bit.EPWM7 = 1;
    DevCfgRegs.CPUSEL0.bit.EPWM8 = 1;
    DevCfgRegs.CPUSEL0.bit.EPWM9 = 1;
    // 配置EPWM10~EPWM12归CPU1主控
    DevCfgRegs.CPUSEL0.bit.EPWM10 = 0;
    DevCfgRegs.CPUSEL0.bit.EPWM11 = 0;
    DevCfgRegs.CPUSEL0.bit.EPWM12 = 0;

    // 配置ECAP1~ECAP6（增强型捕获）全部归CPU1主控
    DevCfgRegs.CPUSEL1.bit.ECAP1 = 0;
    DevCfgRegs.CPUSEL1.bit.ECAP2 = 0;
    DevCfgRegs.CPUSEL1.bit.ECAP3 = 0;
    DevCfgRegs.CPUSEL1.bit.ECAP4 = 0;
    DevCfgRegs.CPUSEL1.bit.ECAP5 = 0;
    DevCfgRegs.CPUSEL1.bit.ECAP6 = 0;

    // 配置EQEP1~EQEP3（增强型正交编码）全部归CPU1主控
    DevCfgRegs.CPUSEL2.bit.EQEP1 = 0;
    DevCfgRegs.CPUSEL2.bit.EQEP2 = 0;
    DevCfgRegs.CPUSEL2.bit.EQEP3 = 0;

    // 配置SD1~SD2（SD卡控制器）全部归CPU1主控
    DevCfgRegs.CPUSEL4.bit.SD1 = 0;
    DevCfgRegs.CPUSEL4.bit.SD2 = 0;

    // 配置SCI_A~SCI_D（串口A~D）全部归CPU1主控
    DevCfgRegs.CPUSEL5.bit.SCI_A = 0;
    DevCfgRegs.CPUSEL5.bit.SCI_B = 0;
    DevCfgRegs.CPUSEL5.bit.SCI_C = 0;
    DevCfgRegs.CPUSEL5.bit.SCI_D = 0;

    // 配置SPI_A~SPI_C（SPI总线A~C）全部归CPU1主控
    DevCfgRegs.CPUSEL6.bit.SPI_A = 0;
    DevCfgRegs.CPUSEL6.bit.SPI_B = 0;
    DevCfgRegs.CPUSEL6.bit.SPI_C = 0;

    // 配置I2C_A~I2C_B（I2C总线A~B）全部归CPU1主控
    DevCfgRegs.CPUSEL7.bit.I2C_A = 0;
    DevCfgRegs.CPUSEL7.bit.I2C_B = 0;

    // 配置CAN_A~CAN_B（CAN总线A~B）全部归CPU1主控
    DevCfgRegs.CPUSEL8.bit.CAN_A = 0;
    DevCfgRegs.CPUSEL8.bit.CAN_B = 0;

    // 配置McBSP_A~McBSP_B（多通道串行端口）全部归CPU1主控
    DevCfgRegs.CPUSEL9.bit.McBSP_A = 0;
    DevCfgRegs.CPUSEL9.bit.McBSP_B = 0;

    // 配置ADC_A~ADC_D（AD转换器A~D）全部归CPU2主控
    DevCfgRegs.CPUSEL11.bit.ADC_A = 1;
    DevCfgRegs.CPUSEL11.bit.ADC_B = 1;
    DevCfgRegs.CPUSEL11.bit.ADC_C = 1;
    DevCfgRegs.CPUSEL11.bit.ADC_D = 1;

    // 配置CMPSS1~CMPSS8（比较器子系统）全部归CPU1主控
    DevCfgRegs.CPUSEL12.bit.CMPSS1 = 0;
    DevCfgRegs.CPUSEL12.bit.CMPSS2 = 0;
    DevCfgRegs.CPUSEL12.bit.CMPSS3 = 0;
    DevCfgRegs.CPUSEL12.bit.CMPSS4 = 0;
    DevCfgRegs.CPUSEL12.bit.CMPSS5 = 0;
    DevCfgRegs.CPUSEL12.bit.CMPSS6 = 0;
    DevCfgRegs.CPUSEL12.bit.CMPSS7 = 0;
    DevCfgRegs.CPUSEL12.bit.CMPSS8 = 0;

    // 配置DAC_A~DAC_C（数模转换器A~C）全部归CPU1主控
    DevCfgRegs.CPUSEL14.bit.DAC_A = 0;
    DevCfgRegs.CPUSEL14.bit.DAC_B = 0;
    DevCfgRegs.CPUSEL14.bit.DAC_C = 0;

    // 关闭EALLOW保护，完成外设资源分配
    EDIS;
}

/******************************************************************************************
* 函数名: sAssignGSRam
* 功能:   双核共享内存（GS-RAM）分配函数，配置GSxMSEL寄存器划分CPU1/CPU2的私有GS-RAM
*         及双核共享GS-RAM，避免内存访问冲突
* 输入:   无
* 输出:   无
* 注意:   GS-RAM分配后，CPU仅能访问自身分配的区域，非法访问会触发RAM访问违例中断
******************************************************************************************/
void sAssignGSRam(void)
{
    // 开启EALLOW保护，MemCfg寄存器为受保护寄存器，需EALLOW才能写操作
    EALLOW;

    // 分配GS-RAM给CPU1主控（0=CPU1，1=CPU2）
    // CPU1私有CPU_PRAM区域：GS0、GS1、GS2
    MemCfgRegs.GSxMSEL.bit.MSEL_GS0 = 0;
    MemCfgRegs.GSxMSEL.bit.MSEL_GS1 = 0;
    MemCfgRegs.GSxMSEL.bit.MSEL_GS2 = 0;
    // CPU1私有CPU_DRAM区域：GS6、GS7、GS8
    MemCfgRegs.GSxMSEL.bit.MSEL_GS6 = 0;
    MemCfgRegs.GSxMSEL.bit.MSEL_GS7 = 0;
    MemCfgRegs.GSxMSEL.bit.MSEL_GS8 = 0;
    // 双核共享区域SHARERAMGSC1TOC2：GS12（CPU1到CPU2的共享内存）
    MemCfgRegs.GSxMSEL.bit.MSEL_GS12 = 0;
    // DFT数据缓冲区DFTDataBuff：GS14、GS15
    MemCfgRegs.GSxMSEL.bit.MSEL_GS14 = 0;
    MemCfgRegs.GSxMSEL.bit.MSEL_GS15 = 0;

    // 分配GS-RAM给CPU2主控（0=CPU1，1=CPU2）
    // CPU2私有CPU_PRAM区域：GS3、GS4、GS5
    MemCfgRegs.GSxMSEL.bit.MSEL_GS3 = 1;
    MemCfgRegs.GSxMSEL.bit.MSEL_GS4 = 1;
    MemCfgRegs.GSxMSEL.bit.MSEL_GS5 = 1;
    // CPU2私有CPU_DRAM区域：GS9、GS10、GS11
    MemCfgRegs.GSxMSEL.bit.MSEL_GS9 = 1;
    MemCfgRegs.GSxMSEL.bit.MSEL_GS10 = 1;
    MemCfgRegs.GSxMSEL.bit.MSEL_GS11 = 1;
    // 双核共享区域SHARERAMGSC2TOC1：GS13（CPU2到CPU1的共享内存）
    MemCfgRegs.GSxMSEL.bit.MSEL_GS13 = 1;

    // 关闭EALLOW保护，完成GS-RAM分配
    EDIS;

    // 汇编指令延时：执行100次空操作（NOP），保证内存配置寄存器写入完成并稳定
    asm(" RPT #100 || NOP");
}

/******************************************************************************************
* 函数名: sADCalibration
* 功能:   AD校准初始化函数，清零AD采样相关的电压差参数，为后续AD校准做准备
* 输入:   无
* 输出:   无
* 注意:   该函数为AD校准的前置步骤，实际校准逻辑在其他函数中实现
******************************************************************************************/
void sADCalibration(void)
{
	// 清零psVarM2s（主到从变量结构体）中的wROutVoltDiff参数：R相输出电压差清零
	psVarM2s->wROutVoltDiff = 0;
	// 清零psVarM2s（主到从变量结构体）中的wSOutVoltDiff参数：S相输出电压差清零
	psVarM2s->wSOutVoltDiff = 0;
}

/******************************************************************************************
* 函数名: sInitParas
* 功能:   系统通用参数初始化函数，配置示波器、数据采集、工作模式等基础运行参数
* 输入:   无
* 输出:   无
******************************************************************************************/
void sInitParas(void)
{
    // 示波器初始化：配置系统示波器的采样通道、采样率、缓冲区等（用于调试/数据采集）
    ScopeInit();
    // 数据采集选择初始化：配置需要采集的系统数据（如电压、电流、转速等）及采集方式
    sDataRecSel();

    // 设置系统测试模式为并网模式（cParaOnGrid）：默认工作在电网并网模式
    sSetTestMode(cParaOnGrid);
    // 设置离网模式为无离网（cNoOffGrid）：默认不启用离网工作模式
    sSetOffGridMode(cNoOffGrid);
    // 设置离网频率为50Hz（cOffGridFreq50Hz）：离网模式下的默认输出频率
    sSetOffGridFreq(cOffGridFreq50Hz);
}

/******************************************************************************************
* 函数名: sReadCpldFwVer
* 功能:   读取CPLD固件版本号函数，从双核共享内存中读取CPLD的版本号并存储到全局数组
* 输入:   无
* 输出:   无
* 注意:   psMemS2m为从到主共享内存结构体，存储CPLD寄存器的原始数据
******************************************************************************************/
void sReadCpldFwVer(void)
{
    // 读取共享内存中CPLD寄存器0的16位数据，存入wCpldFwVer[0]（CPLD版本号高16位）
    wCpldFwVer[0] = psMemS2m->unCpldReg[0].word;
    // 读取共享内存中CPLD寄存器1的16位数据，存入wCpldFwVer[1]（CPLD版本号低16位）
    wCpldFwVer[1] = psMemS2m->unCpldReg[1].word;
}

/******************************************************************************************
* 函数名: swPackCpldFwVer
* 功能:   打包CPLD固件版本号函数，返回CPLD完整的版本号（原始16位数据）
* 输入:   无
* 输出:   INT16U - CPLD固件版本号的16位原始数据
******************************************************************************************/
INT16U swPackCpldFwVer(void)
{
    // 返回共享内存中CPLD寄存器0的16位数据，即CPLD固件版本号
    return(psMemS2m->unCpldReg[0].word);
}

/******************************************************************************************
* 函数名: swPackCpldMajorVer
* 功能:   提取CPLD主版本号函数，从版本号中解析出主版本号（高8位）
* 输入:   无
* 输出:   INT16U - CPLD主版本号（0~255）
******************************************************************************************/
INT16U swPackCpldMajorVer(void)
{
    // 将CPLD版本号右移8位，与0x00FF做与运算，提取高8位作为主版本号
    return((psMemS2m->unCpldReg[0].word >> 8)&0x00FF);
}

/******************************************************************************************
* 函数名: swPackCpldMinorVer
* 功能:   提取CPLD次版本号函数，从版本号中解析出次版本号（低8位）
* 输入:   无
* 输出:   INT16U - CPLD次版本号（0~255）
******************************************************************************************/
INT16U swPackCpldMinorVer(void)
{
    // 将CPLD版本号与0x00FF做与运算，提取低8位作为次版本号
    return((psMemS2m->unCpldReg[0].word)&0x00FF);
}

/******************************************************************************************
* 函数名: sPwmParasInit
* 功能:   PWM核心参数初始化函数，配置PWM频率、采样率、时间常数、滤波系数等基础参数
* 输入:   无
* 输出:   无
* 注意:   基于宏定义cPwmFrq/cSamplFrq初始化，为系统默认PWM参数
******************************************************************************************/
void sPwmParasInit(void)
{
	// 初始化PWM频率为宏定义cPwmFrq：设置全局PWM频率变量
	wPwmFrq = cPwmFrq;
	// 初始化采样频率为宏定义cSamplFrq：设置全局采样频率变量
	wSamplFrq = cSamplFrq;
	// 计算PWM周期时间：fPwmTs = 1/PWM频率，单位为秒
	fPwmTs = 1.0 / (float)cPwmFrq;
	// 计算采样周期时间：fSamplTs = 1/采样频率，单位为秒
	fSamplTs = 1.0 / (float)cSamplFrq;
	// 初始化PWM四分之一周期的逆时间常数为宏定义cQrtInvPwmTs：用于PWM时序计算
	wQrtInvPwmTs = cQrtInvPwmTs;
	// 初始化PWM二分之一周期的逆时间常数为宏定义cHalfInvPwmTs：用于PWM时序计算
	wHalfInvPwmTs = cHalfInvPwmTs;
	// 初始化PWM整周期的逆时间常数为宏定义cInvPwmTs：用于PWM时序计算
	wInvPwmTs = cInvPwmTs;
	// 初始化锁相环步长参考值为宏定义cPllStepRef：用于电网锁相环（PLL）的步长计算
	fPllStepRef = cPllStepRef;
	// 将PWM频率写入主到从变量结构体，供双核共享PWM频率参数
	psVarM2s->wPwmFreq = wPwmFrq;

	// 计算交流缓冲分辨率：基于交流缓冲点数、采样周期计算，用于交流信号的缓冲处理
	fAcBufferRes = ((float)cAcBufferPoint * 1000.0) * fSamplTs;
	// 计算直流缓冲分辨率：基于直流缓冲点数、采样周期计算，用于直流信号的缓冲处理
	fDcBufferRes = ((float)cDcBufferPoint * 1000.0) * fSamplTs;

	// 计算25Hz低通滤波器K1系数：一阶低通滤波器系数，用于25Hz信号滤波
	fLFCoeffK1 = 1.0 / (1.0 + (PI2*25.0*fSamplTs));      //25Hz
	// 计算25Hz低通滤波器K2系数：K2 = 1-K1，一阶低通滤波器的互补系数
	fLFCoeffK2 = 1.0 - fLFCoeffK1;
}

/******************************************************************************************
* 函数名: sPwmFreqSetting
* 功能:   PWM频率设置函数，修改全局PWM频率变量为指定值
* 输入:   wPwmFreqTemp - 待设置的PWM频率（16位无符号整数）
* 输出:   无
******************************************************************************************/
void sPwmFreqSetting(INT16U wPwmFreqTemp)
{
	// 将全局PWM频率变量更新为输入的指定频率
	wPwmFrq = wPwmFreqTemp;
}

/******************************************************************************************
* 函数名: sPwmParasReset
* 功能:   PWM参数重置函数，当PWM频率异常时恢复默认值，并重新计算所有PWM相关衍生参数
*         同时重新初始化EPWM模块，保证参数修改后PWM正常工作
* 输入:   无
* 输出:   无
******************************************************************************************/
void sPwmParasReset(void)
{
    // 判读PWM频率是否异常（小于等于0），若异常则恢复为默认宏定义cPwmFrq
    if(wPwmFrq <= 0)
    {
        wPwmFrq = cPwmFrq;
    }

	// 重新计算采样频率：采样频率 = 2 * PWM频率（固定2倍频关系，保证采样精度）
	wSamplFrq = 2 * wPwmFrq;
    // 重新计算PWM周期时间：fPwmTs = 1/PWM频率
    fPwmTs = 1.0 / (float)wPwmFrq;
    // 重新计算采样周期时间：fSamplTs = 1/采样频率
    fSamplTs = 1.0 / (float)wSamplFrq;

	// 重新计算PWM四分之一周期逆时间常数：25000000L / PWM频率（基于系统时钟的时序常数）
	wQrtInvPwmTs = ((INT16U)(25000000L / wPwmFrq));
	// 重新计算PWM二分之一周期逆时间常数：2 * 四分之一周期常数
	wHalfInvPwmTs = 2 * wQrtInvPwmTs ;
	// 重新计算PWM整周期逆时间常数：2 * 二分之一周期常数
	wInvPwmTs = 2 * wHalfInvPwmTs;

	// 重新计算锁相环步长参考值：(100.0*PI) * 采样周期，用于电网锁相环步长更新
	fPllStepRef = (100.0 * PI) * fSamplTs;

	// 重新计算交流缓冲分辨率：基于新的采样周期更新
	fAcBufferRes = ((float)cAcBufferPoint * 1000.0) * fSamplTs;
	// 重新计算直流缓冲分辨率：基于新的采样周期更新
	fDcBufferRes = ((float)cDcBufferPoint * 1000.0) * fSamplTs;

	// 重新计算25Hz低通滤波器K1系数：基于新的采样周期更新滤波系数
	fLFCoeffK1 = 1.0 / (1.0 + (PI2*25.0*fSamplTs));      //25Hz
	// 重新计算25Hz低通滤波器K2系数：K2=1-K1
	fLFCoeffK2 = 1.0 - fLFCoeffK1;

	// 判断当前电网频率类型，若为60Hz则保持60Hz，否则恢复为50Hz
	if(sbGetGridFreqType() == cRateFreq60Hz)
	{
		sSetGridFreqType(cRateFreq60Hz);
	}
	else
	{
		sSetGridFreqType(cRateFreq50Hz);
	}

	// 将更新后的PWM频率写入双核共享结构体，保证CPU2获取最新PWM频率
	psVarM2s->wPwmFreq = wPwmFrq;

	// 重新初始化EPWM模块：加载新的PWM参数到EPWM寄存器，使参数修改生效
	sInitEPwm();

    // 初始化SCU（系统控制单元）初始化标志为0x5555：表示所有SCU参数均未完成初始化
    unSCUInitSetFlag.word = 0x5555;  // all none set
    // 初始化通用参数设置标志为0：表示所有通用参数均未完成设置
    unComnParaSetFlag.word = 0;      // all none set
}

