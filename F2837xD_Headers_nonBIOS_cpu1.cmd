


//.cmd 文件是嵌入式工程的链接命令文件，用于定义代码、数据、堆栈在 Flash 和 RAM 的存储布局，以及程序启动入口，是编译生成可执行文件的重要配置文件。
//F2837xD_Headers_nonBIOS_cpu1.cmd 是 CPU1 的裸机工程链接命令文件，用于定义代码、数据、堆栈在 Flash 和 RAM 中的存储布局，设置启动入口和段映射，是编译生成可执行程序的重要配置文件。


/*
 * DSP 内存映射与段分配配置文件
 * 适用于TI C2000系列高性能DSP（如F2837xD/F2838xD等）
 * MEMORY：定义芯片物理内存的地址范围、大小和属性
 * SECTIONS：将程序编译后的段映射到MEMORY定义的物理内存区域
 */

/* -------------------------- MEMORY 定义 -------------------------- */
MEMORY
{
    /* 
     * PAGE 0：程序内存 (Program Memory)
     * 用于存储代码、常量、中断向量表等只读数据
     * 本配置中暂未定义具体区域，需根据实际芯片Flash/RAM配置补充
     */
    PAGE 0:    /* Program Memory */

    /* 
     * PAGE 1：数据内存 (Data Memory)
     * 用于存储变量、寄存器映射、外设寄存器等可读写数据
     * 以下为各外设寄存器的物理地址映射定义
     */
    PAGE 1:    /* Data Memory */

    /* ADC结果寄存器区域 (16个通道×4个ADC，每个结果32字节) */
    ADCA_RESULT   : origin = 0x000B00, length = 0x000020  /* ADC A 结果寄存器，起始地址0xB00，长度32字节 */
    ADCB_RESULT   : origin = 0x000B20, length = 0x000020  /* ADC B 结果寄存器，起始地址0xB20，长度32字节 */
    ADCC_RESULT   : origin = 0x000B40, length = 0x000020  /* ADC C 结果寄存器，起始地址0xB40，长度32字节 */
    ADCD_RESULT   : origin = 0x000B60, length = 0x000020  /* ADC D 结果寄存器，起始地址0xB60，长度32字节 */

    /* ADC控制寄存器区域 (每个ADC占128字节) */
    ADCA          : origin = 0x007400, length = 0x000080  /* ADC A 控制寄存器，起始地址0x7400，长度128字节 */
    ADCB          : origin = 0x007480, length = 0x000080  /* ADC B 控制寄存器，起始地址0x7480，长度128字节 */
    ADCC          : origin = 0x007500, length = 0x000080  /* ADC C 控制寄存器，起始地址0x7500，长度128字节 */
    ADCD          : origin = 0x007580, length = 0x000080  /* ADC D 控制寄存器，起始地址0x7580，长度128字节 */

    /* 模拟子系统寄存器 */
    ANALOG_SUBSYS : origin = 0x05D180, length = 0x000080  /* 模拟子系统寄存器，起始地址0x5D180，长度128字节 */
    
    /* CLA (控制律加速器) 寄存器 */
    CLA1          : origin = 0x001400, length = 0x000040     /* CLA1 核心寄存器，起始地址0x1400，长度64字节 */

    /* CLB (可配置逻辑块) 交叉开关寄存器 */
    CLB_XBAR      : origin = 0x007A40, length = 0x000040      /* CLB交叉开关寄存器，起始地址0x7A40，长度64字节 */

    /* CMPSS (比较器子系统) 寄存器 (每个CMPSS占32字节) */
    CMPSS1        : origin = 0x005C80, length = 0x000020      /* CMPSS1 寄存器，起始地址0x5C80，长度32字节 */
    CMPSS2        : origin = 0x005CA0, length = 0x000020      /* CMPSS2 寄存器，起始地址0x5CA0，长度32字节 */
    CMPSS3        : origin = 0x005CC0, length = 0x000020      /* CMPSS3 寄存器，起始地址0x5CC0，长度32字节 */
    CMPSS4        : origin = 0x005CE0, length = 0x000020      /* CMPSS4 寄存器，起始地址0x5CE0，长度32字节 */
    CMPSS5        : origin = 0x005D00, length = 0x000020      /* CMPSS5 寄存器，起始地址0x5D00，长度32字节 */
    CMPSS6        : origin = 0x005D20, length = 0x000020      /* CMPSS6 寄存器，起始地址0x5D20，长度32字节 */
    CMPSS7        : origin = 0x005D40, length = 0x000020      /* CMPSS7 寄存器，起始地址0x5D40，长度32字节 */
    CMPSS8        : origin = 0x005D60, length = 0x000020      /* CMPSS8 寄存器，起始地址0x5D60，长度32字节 */

    /* CPU定时器寄存器 (每个定时器占8字节) */
    CPU_TIMER0    : origin = 0x000C00, length = 0x000008     /* CPU Timer0 寄存器，起始地址0xC00，长度8字节 */
    CPU_TIMER1    : origin = 0x000C08, length = 0x000008     /* CPU Timer1 寄存器，起始地址0xC08，长度8字节 */
    CPU_TIMER2    : origin = 0x000C10, length = 0x000008     /* CPU Timer2 寄存器，起始地址0xC10，长度8字节 */

    /* DAC (数模转换器) 寄存器 (每个DAC占16字节) */
    DACA          : origin = 0x005C00, length = 0x000010      /* DAC A 寄存器，起始地址0x5C00，长度16字节 */
    DACB          : origin = 0x005C10, length = 0x000010      /* DAC B 寄存器，起始地址0x5C10，长度16字节 */
    DACC          : origin = 0x005C20, length = 0x000010      /* DAC C 寄存器，起始地址0x5C20，长度16字节 */
    
    /* CAN (控制器局域网) 寄存器 (每个CAN占4KB) */
    CANA          : origin = 0x048000, length = 0x001000      /* DCAN A 寄存器，起始地址0x48000，长度4096字节 */
    CANB          : origin = 0x04A000, length = 0x001000      /* DCAN B 寄存器，起始地址0x4A000，长度4096字节 */

    /* DMA (直接内存访问) 寄存器 */
    DMA           : origin = 0x001000, length = 0x000200      /* DMA 核心寄存器，起始地址0x1000，长度512字节 */
    DMACLASRCSEL  : origin = 0x007980, length = 0x000040      /* DMA-CLA源选择寄存器，起始地址0x7980，长度64字节 */

    /* ECAP (增强型捕获模块) 寄存器 (每个ECAP占32字节) */
    ECAP1        : origin = 0x005000, length = 0x000020     /* Enhanced Capture 1 寄存器，起始地址0x5000，长度32字节 */
    ECAP2        : origin = 0x005020, length = 0x000020     /* Enhanced Capture 2 寄存器，起始地址0x5020，长度32字节 */
    ECAP3        : origin = 0x005040, length = 0x000020     /* Enhanced Capture 3 寄存器，起始地址0x5040，长度32字节 */
    ECAP4        : origin = 0x005060, length = 0x000020     /* Enhanced Capture 4 寄存器，起始地址0x5060，长度32字节 */
    ECAP5        : origin = 0x005080, length = 0x000020     /* Enhanced Capture 5 寄存器，起始地址0x5080，长度32字节 */
    ECAP6        : origin = 0x0050A0, length = 0x000020     /* Enhanced Capture 6 寄存器，起始地址0x50A0，长度32字节 */
    
    /* EMIF (外部存储器接口) 寄存器 (每个EMIF占2KB) */
    EMIF1        : origin = 0x047000, length = 0x000800      /* EMIF1 寄存器，起始地址0x47000，长度2048字节 */
    EMIF2        : origin = 0x047800, length = 0x000800      /* EMIF2 寄存器，起始地址0x47800，长度2048字节 */

    /* EQEP (增强型正交编码脉冲) 寄存器 (每个EQEP占64字节) */
    EQEP1        : origin = 0x005100, length = 0x000040     /* Enhanced QEP 1 寄存器，起始地址0x5100，长度64字节 */
    EQEP2        : origin = 0x005140, length = 0x000040     /* Enhanced QEP 2 寄存器，起始地址0x5140，长度64字节 */
    EQEP3        : origin = 0x005180, length = 0x000040     /* Enhanced QEP 3 寄存器，起始地址0x5180，长度64字节 */

    /* EPWM (增强型脉宽调制) 寄存器 (每个EPWM占256字节) */
    EPWM1        : origin = 0x004000, length = 0x000100     /* Enhanced PWM 1 寄存器，起始地址0x4000，长度256字节 */
    EPWM2        : origin = 0x004100, length = 0x000100     /* Enhanced PWM 2 寄存器，起始地址0x4100，长度256字节 */
    EPWM3        : origin = 0x004200, length = 0x000100     /* Enhanced PWM 3 寄存器，起始地址0x4200，长度256字节 */
    EPWM4        : origin = 0x004300, length = 0x000100     /* Enhanced PWM 4 寄存器，起始地址0x4300，长度256字节 */
    EPWM5        : origin = 0x004400, length = 0x000100     /* Enhanced PWM 5 寄存器，起始地址0x4400，长度256字节 */
    EPWM6        : origin = 0x004500, length = 0x000100     /* Enhanced PWM 6 寄存器，起始地址0x4500，长度256字节 */
    EPWM7        : origin = 0x004600, length = 0x000100     /* Enhanced PWM 7 寄存器，起始地址0x4600，长度256字节 */
    EPWM8        : origin = 0x004700, length = 0x000100     /* Enhanced PWM 8 寄存器，起始地址0x4700，长度256字节 */
    EPWM9        : origin = 0x004800, length = 0x000100     /* Enhanced PWM 9 寄存器，起始地址0x4800，长度256字节 */
    EPWM10       : origin = 0x004900, length = 0x000100     /* Enhanced PWM 10 寄存器，起始地址0x4900，长度256字节 */
    EPWM11       : origin = 0x004A00, length = 0x000100     /* Enhanced PWM 11 寄存器，起始地址0x4A00，长度256字节 */
    EPWM12       : origin = 0x004B00, length = 0x000100     /* Enhanced PWM 12 寄存器，起始地址0x4B00，长度256字节 */

    /* EPWM交叉开关寄存器 */
    EPWM_XBAR    : origin = 0x007A00, length = 0x000040      /* EPWM交叉开关寄存器，起始地址0x7A00，长度64字节 */

    /* FLASH控制与ECC寄存器 */
    FLASH0_CTRL  : origin = 0x05F800, length = 0x000300      /* FLASH0 控制寄存器，起始地址0x5F800，长度768字节 */
    FLASH0_ECC   : origin = 0x05FB00, length = 0x000040      /* FLASH0 ECC校验寄存器，起始地址0x5FB00，长度64字节 */

    /* GPIO (通用输入输出) 寄存器 */
    GPIOCTRL     : origin = 0x007C00, length = 0x000180     /* GPIO控制寄存器，起始地址0x7C00，长度384字节 */
    GPIODAT      : origin = 0x007F00, length = 0x000030     /* GPIO数据寄存器，起始地址0x7F00，长度48字节 */

    /* 输出交叉开关寄存器 */
    OUTPUT_XBAR  : origin = 0x007A80, length = 0x000040      /* 输出交叉开关寄存器，起始地址0x7A80，长度64字节 */
    /* HWBIST (硬件内建自测试) 寄存器 */
    HWBIST       : origin = 0x05E000, length = 0x000100     /* HWBIST寄存器，起始地址0x5E000，长度256字节 */
    /* I2C (集成电路总线) 寄存器 (每个I2C占64字节) */
    I2CA         : origin = 0x007300, length = 0x000040     /* I2C-A 寄存器，起始地址0x7300，长度64字节 */
    I2CB         : origin = 0x007340, length = 0x000040     /* I2C-B 寄存器，起始地址0x7340，长度64字节 */

    /* IPC (处理器间通信) 寄存器 */
    IPC          : origin = 0x050000, length = 0x001000      /* IPC寄存器，起始地址0x50000，长度4096字节 */

    /* ROM预取寄存器 */
    ROMPREFETCH  : origin = 0x05E608, length = 0x000002      /* ROM预取控制寄存器，起始地址0x5E608，长度2字节 */

    /* 内存配置相关寄存器 */
    MEMCFG       : origin = 0x05F400, length = 0x000080     /* 内存配置寄存器，起始地址0x5F400，长度128字节 */
    EMIF1CONFIG  : origin = 0x05F480, length = 0x000020     /* EMIF-1 配置寄存器，起始地址0x5F480，长度32字节 */
    EMIF2CONFIG  : origin = 0x05F4A0, length = 0x000020     /* EMIF-2 配置寄存器，起始地址0x5F4A0，长度32字节 */
    ACCESSPROTECTION  : origin = 0x05F4C0, length = 0x000040     /* 访问保护寄存器，起始地址0x5F4C0，长度64字节 */
    MEMORYERROR  : origin = 0x05F500, length = 0x000040     /* 内存错误寄存器，起始地址0x5F500，长度64字节 */
    ROMWAITSTATE : origin = 0x05F540, length = 0x000002     /* ROM等待状态寄存器，起始地址0x5F540，长度2字节 */

    /* McBSP (多通道缓冲串行端口) 寄存器 (每个McBSP占64字节) */
    MCBSPA       : origin = 0x006000, length = 0x000040     /* McBSP-A 寄存器，起始地址0x6000，长度64字节 */
    MCBSPB       : origin = 0x006040, length = 0x000040     /* McBSP-B 寄存器，起始地址0x6040，长度64字节 */

    /* NMI (不可屏蔽中断) 寄存器 */
    NMIINTRUPT   : origin = 0x007060, length = 0x000010     /* NMI看门狗中断寄存器，起始地址0x7060，长度16字节 */

    /* PIE (外设中断扩展) 寄存器 */
    PIE_CTRL     : origin = 0x000CE0, length = 0x000020     /* PIE控制寄存器，起始地址0xCE0，长度32字节 */
    PIE_VECT     : origin = 0x000D00, length = 0x000200     /* PIE中断向量表，起始地址0xD00，长度512字节 */
    /* PBIST (处理器内建自测试) 寄存器 */
    PBIST        : origin = 0x05E200, length = 0x000200     /* PBIST寄存器，起始地址0x5E200，长度512字节 */
    /* SCI (串行通信接口) 寄存器 (每个SCI占16字节) */
    SCIA         : origin = 0x007200, length = 0x000010     /* SCI-A 寄存器，起始地址0x7200，长度16字节 */
    SCIB         : origin = 0x007210, length = 0x000010     /* SCI-B 寄存器，起始地址0x7210，长度16字节 */
    SCIC         : origin = 0x007220, length = 0x000010     /* SCI-C 寄存器，起始地址0x7220，长度16字节 */
    SCID         : origin = 0x007230, length = 0x000010     /* SCI-D 寄存器，起始地址0x7230，长度16字节 */
    
    /* SDFM (西格玛-德尔塔滤波器模块) 寄存器 (每个SDFM占128字节) */
    SDFM1        : origin = 0x005E00, length = 0x000080     /* Sigma delta 1 寄存器，起始地址0x5E00，长度128字节 */
    SDFM2        : origin = 0x005E80, length = 0x000080     /* Sigma delta 2 寄存器，起始地址0x5E80，长度128字节 */

    /* SPI (串行外设接口) 寄存器 (每个SPI占16字节) */
    SPIA         : origin = 0x006100, length = 0x000010      /* SPIA寄存器，起始地址0x6100，长度16字节 */
    SPIB         : origin = 0x006110, length = 0x000010      /* SPIB寄存器，起始地址0x6110，长度16字节 */
    SPIC         : origin = 0x006120, length = 0x000010      /* SPIC寄存器，起始地址0x6120，长度16字节 */
    SPID         : origin = 0x006130, length = 0x000010      /* SPID寄存器，起始地址0x6130，长度16字节 */

    /* UPP (通用并行端口) 寄存器 */
    UPP          : origin = 0x006200, length = 0x000100     /* uPP寄存器，起始地址0x6200，长度256字节 */

    /* 设备配置寄存器 */
    DEV_CFG      : origin = 0x05D000, length = 0x000180     /* 设备配置寄存器，起始地址0x5D000，长度384字节 */
    CLK_CFG      : origin = 0x05D200, length = 0x000100     /* 时钟配置寄存器，起始地址0x5D200，长度256字节 */
    CPU_SYS      : origin = 0x05D300, length = 0x000100     /* CPU系统寄存器，起始地址0x5D300，长度256字节 */

    /* 输入交叉开关、触发、看门狗寄存器 */
    INPUT_XBAR   : origin = 0x007900, length = 0x000040      /* 输入交叉开关寄存器，起始地址0x7900，长度64字节 */
    TRIG         : origin = 0x007940, length = 0x000010      /* 触发寄存器，起始地址0x7940，长度16字节 */
    WD           : origin = 0x007000, length = 0x000040      /* 看门狗寄存器，起始地址0x7000，长度64字节 */

    /* 外部中断寄存器 */
    XINT         : origin = 0x007070, length = 0x000010      /* 外部中断寄存器，起始地址0x7070，长度16字节 */
                
    /* DCSM (双代码安全模块) 寄存器 */
    DCSM_Z1      : origin = 0x05F000, length = 0x000030     /* Zone 1 DCSM寄存器，起始地址0x5F000，长度48字节 */
    DCSM_Z2      : origin = 0x05F040, length = 0x000030     /* Zone 2 DCSM寄存器，起始地址0x5F040，长度48字节 */
    DCSM_COMMON  : origin = 0x05F070, length = 0x000010     /* 公共DCSM寄存器，起始地址0x5F070，长度16字节 */

    /* DCSM OTP (一次性可编程) 寄存器 */
    DCSM_Z1_OTP  : origin = 0x078000, length = 0x000020     /* Z1 OTP寄存器，起始地址0x78000，长度32字节 */
    DCSM_Z2_OTP  : origin = 0x078200, length = 0x000020     /* Z2 OTP寄存器，起始地址0x78200，长度32字节 */ 
}

/* -------------------------- SECTIONS 分配 -------------------------- */
SECTIONS
{
/*** PIE Vect Table and Boot ROM Variables Structures ***/
    /* PIE向量表与Boot ROM变量联合分配到PIE_VECT区域 (PAGE 1) */
    UNION run = PIE_VECT, PAGE = 1
    {
        PieVectTableFile                  /* PIE中断向量表文件 */
        GROUP                             /* 组内变量：Boot ROM相关 */
        {
            EmuKeyVar                     /* 仿真器密钥变量 */
            EmuBModeVar                   /* 仿真器引导模式变量 */
            FlashCallbackVar              /* Flash回调函数变量 */
            FlashScalingVar               /* Flash缩放变量 */
        }
    }

    /* ADC结果寄存器段映射 */
    AdcaResultFile        : > ADCA_RESULT,  PAGE = 1        /* ADC A结果段映射到ADCA_RESULT地址 */
    AdcbResultFile        : > ADCB_RESULT,  PAGE = 1        /* ADC B结果段映射到ADCB_RESULT地址 */
    AdccResultFile        : > ADCC_RESULT,  PAGE = 1        /* ADC C结果段映射到ADCC_RESULT地址 */
    AdcdResultFile        : > ADCD_RESULT,  PAGE = 1        /* ADC D结果段映射到ADCD_RESULT地址 */

    /* ADC控制寄存器段映射 */
    AdcaRegsFile          : > ADCA,         PAGE = 1        /* ADC A控制段映射到ADCA地址 */
    AdcbRegsFile          : > ADCB,         PAGE = 1        /* ADC B控制段映射到ADCB地址 */
    AdccRegsFile          : > ADCC,         PAGE = 1        /* ADC C控制段映射到ADCC地址 */
    AdcdRegsFile          : > ADCD,         PAGE = 1        /* ADC D控制段映射到ADCD地址 */

    /* 模拟子系统寄存器段映射 */
    AnalogSubsysRegsFile : > ANALOG_SUBSYS, PAGE = 1        /* 模拟子系统段映射到ANALOG_SUBSYS地址 */
    
    /* CLA寄存器段映射 */
    Cla1RegsFile          : > CLA1,         PAGE = 1        /* CLA1寄存器段映射到CLA1地址 */
    Cla1SoftIntRegsFile   : > PIE_CTRL,     PAGE = 1, type=DSECT  /* CLA1软中断段映射到PIE_CTRL，类型为DSECT(仅声明) */

    /* CLB交叉开关寄存器段映射 */
    ClbXbarRegsFile      : > CLB_XBAR     PAGE = 1          /* CLB交叉开关段映射到CLB_XBAR地址 */

    /* CMPSS寄存器段映射 */
    Cmpss1RegsFile        : > CMPSS1,      PAGE = 1        /* CMPSS1段映射到CMPSS1地址 */
    Cmpss2RegsFile        : > CMPSS2,      PAGE = 1        /* CMPSS2段映射到CMPSS2地址 */
    Cmpss3RegsFile        : > CMPSS3,      PAGE = 1        /* CMPSS3段映射到CMPSS3地址 */
    Cmpss4RegsFile        : > CMPSS4,      PAGE = 1        /* CMPSS4段映射到CMPSS4地址 */
    Cmpss5RegsFile        : > CMPSS5,      PAGE = 1        /* CMPSS5段映射到CMPSS5地址 */
    Cmpss6RegsFile        : > CMPSS6,      PAGE = 1        /* CMPSS6段映射到CMPSS6地址 */
    Cmpss7RegsFile        : > CMPSS7,      PAGE = 1        /* CMPSS7段映射到CMPSS7地址 */
    Cmpss8RegsFile        : > CMPSS8,      PAGE = 1        /* CMPSS8段映射到CMPSS8地址 */

    /* CPU定时器寄存器段映射 */
    CpuTimer0RegsFile     : > CPU_TIMER0,    PAGE = 1      /* Timer0段映射到CPU_TIMER0地址 */
    CpuTimer1RegsFile     : > CPU_TIMER1,    PAGE = 1      /* Timer1段映射到CPU_TIMER1地址 */
    CpuTimer2RegsFile     : > CPU_TIMER2,    PAGE = 1      /* Timer2段映射到CPU_TIMER2地址 */

    /* DAC寄存器段映射 */
    DacaRegsFile          : > DACA          PAGE = 1       /* DAC A段映射到DACA地址 */
    DacbRegsFile          : > DACB          PAGE = 1       /* DAC B段映射到DACB地址 */
    DaccRegsFile          : > DACC          PAGE = 1       /* DAC C段映射到DACC地址 */
    
    /* CAN寄存器段映射 */
    CanaRegsFile          : > CANA,         PAGE = 1       /* CAN A段映射到CANA地址 */
    CanbRegsFile          : > CANB,         PAGE = 1       /* CAN B段映射到CANB地址 */

    /* DCSM寄存器段映射 */
    DcsmZ1RegsFile        : > DCSM_Z1,      PAGE = 1       /* DCSM Z1段映射到DCSM_Z1地址 */
    DcsmZ2RegsFile        : > DCSM_Z2,      PAGE = 1       /* DCSM Z2段映射到DCSM_Z2地址 */
    DcsmCommonRegsFile    : > DCSM_COMMON,  PAGE = 1       /* DCSM公共段映射到DCSM_COMMON地址 */

    /* DCSM OTP寄存器段映射 */
    DcsmZ1OtpFile         : > DCSM_Z1_OTP,  PAGE = 1       /* DCSM Z1 OTP段映射到DCSM_Z1_OTP地址 */
    DcsmZ2OtpFile         : > DCSM_Z2_OTP,  PAGE = 1       /* DCSM Z2 OTP段映射到DCSM_Z2_OTP地址 */

    /* DMA寄存器段映射 */
    DmaRegsFile           : > DMA           PAGE = 1       /* DMA段映射到DMA地址 */
    DmaClaSrcSelRegsFile  : > DMACLASRCSEL  PAGE = 1       /* DMA-CLA源选择段映射到DMACLASRCSEL地址 */

    /* ECAP寄存器段映射 */
    ECap1RegsFile         : > ECAP1,        PAGE = 1       /* ECAP1段映射到ECAP1地址 */
    ECap2RegsFile         : > ECAP2,        PAGE = 1       /* ECAP2段映射到ECAP2地址 */
    ECap3RegsFile         : > ECAP3,        PAGE = 1       /* ECAP3段映射到ECAP3地址 */
    ECap4RegsFile         : > ECAP4,        PAGE = 1       /* ECAP4段映射到ECAP4地址 */
    ECap5RegsFile         : > ECAP5,        PAGE = 1       /* ECAP5段映射到ECAP5地址 */
    ECap6RegsFile         : > ECAP6,        PAGE = 1       /* ECAP6段映射到ECAP6地址 */
    
    /* EMIF寄存器段映射 */
    Emif1RegsFile         : > EMIF1         PAGE = 1       /* EMIF1段映射到EMIF1地址 */
    Emif2RegsFile         : > EMIF2         PAGE = 1       /* EMIF2段映射到EMIF2地址 */

    /* EPWM寄存器段映射 */
    EPwm1RegsFile         : > EPWM1,        PAGE = 1       /* EPWM1段映射到EPWM1地址 */
    EPwm2RegsFile         : > EPWM2,        PAGE = 1       /* EPWM2段映射到EPWM2地址 */
    EPwm3RegsFile         : > EPWM3,        PAGE = 1       /* EPWM3段映射到EPWM3地址 */
    EPwm4RegsFile         : > EPWM4,        PAGE = 1       /* EPWM4段映射到EPWM4地址 */
    EPwm5RegsFile         : > EPWM5,        PAGE = 1       /* EPWM5段映射到EPWM5地址 */
    EPwm6RegsFile         : > EPWM6,        PAGE = 1       /* EPWM6段映射到EPWM6地址 */
    EPwm7RegsFile         : > EPWM7,        PAGE = 1       /* EPWM7段映射到EPWM7地址 */
    EPwm8RegsFile         : > EPWM8,        PAGE = 1       /* EPWM8段映射到EPWM8地址 */
    EPwm9RegsFile         : > EPWM9,        PAGE = 1       /* EPWM9段映射到EPWM9地址 */
    EPwm10RegsFile        : > EPWM10,       PAGE = 1       /* EPWM10段映射到EPWM10地址 */
    EPwm11RegsFile        : > EPWM11,       PAGE = 1       /* EPWM11段映射到EPWM11地址 */
    EPwm12RegsFile        : > EPWM12,       PAGE = 1       /* EPWM12段映射到EPWM12地址 */

    /* EPWM交叉开关寄存器段映射 */
    EPwmXbarRegsFile      : > EPWM_XBAR     PAGE = 1       /* EPWM交叉开关段映射到EPWM_XBAR地址 */

    /* EQEP寄存器段映射 */
    EQep1RegsFile         : > EQEP1,        PAGE = 1       /* EQEP1段映射到EQEP1地址 */
    EQep2RegsFile         : > EQEP2,        PAGE = 1       /* EQEP2段映射到EQEP2地址 */
    EQep3RegsFile         : > EQEP3,        PAGE = 1       /* EQEP3段映射到EQEP3地址 */

    /* FLASH寄存器段映射 */
    Flash0CtrlRegsFile    : > FLASH0_CTRL   PAGE = 1       /* FLASH0控制段映射到FLASH0_CTRL地址 */
    Flash0EccRegsFile     : > FLASH0_ECC    PAGE = 1       /* FLASH0 ECC段映射到FLASH0_ECC地址 */

    /* GPIO寄存器段映射 */
    GpioCtrlRegsFile      : > GPIOCTRL,     PAGE = 1       /* GPIO控制段映射到GPIOCTRL地址 */
    GpioDataRegsFile      : > GPIODAT,      PAGE = 1       /* GPIO数据段映射到GPIODAT地址 */

    /* 输出交叉开关、HWBIST、I2C、输入交叉开关、IPC寄存器段映射 */
    OutputXbarRegsFile    : > OUTPUT_XBAR   PAGE = 1       /* 输出交叉开关段映射到OUTPUT_XBAR地址 */
    HwbistRegsFile        : > HWBIST        PAGE = 1       /* HWBIST段映射到HWBIST地址 */
    I2caRegsFile          : > I2CA,         PAGE = 1       /* I2C A段映射到I2CA地址 */
    I2cbRegsFile          : > I2CB,         PAGE = 1       /* I2C B段映射到I2CB地址 */
    InputXbarRegsFile     : > INPUT_XBAR    PAGE = 1       /* 输入交叉开关段映射到INPUT_XBAR地址 */
    IpcRegsFile           : > IPC,          PAGE = 1       /* IPC段映射到IPC地址 */

    /* ROM预取、内存配置相关寄存器段映射 */
    RomPrefetchRegsFile   : > ROMPREFETCH,  PAGE = 1       /* ROM预取段映射到ROMPREFETCH地址 */
    MemCfgRegsFile        : > MEMCFG,       PAGE = 1       /* 内存配置段映射到MEMCFG地址 */
    Emif1ConfigRegsFile   : > EMIF1CONFIG,  PAGE = 1       /* EMIF1配置段映射到EMIF1CONFIG地址 */
    Emif2ConfigRegsFile   : > EMIF2CONFIG,  PAGE = 1       /* EMIF2配置段映射到EMIF2CONFIG地址 */
    AccessProtectionRegsFile  : > ACCESSPROTECTION, PAGE = 1  /* 访问保护段映射到ACCESSPROTECTION地址 */
    MemoryErrorRegsFile   : > MEMORYERROR,  PAGE = 1       /* 内存错误段映射到MEMORYERROR地址 */
    RomWaitStateRegsFile  : > ROMWAITSTATE, PAGE = 1       /* ROM等待状态段映射到ROMWAITSTATE地址 */

    /* McBSP寄存器段映射 */
    McbspaRegsFile        : > MCBSPA,       PAGE = 1       /* McBSP A段映射到MCBSPA地址 */
    McbspbRegsFile        : > MCBSPB,       PAGE = 1       /* McBSP B段映射到MCBSPB地址 */

    /* UPP寄存器段映射 */
    UppRegsFile           : > UPP,          PAGE = 1       /* UPP段映射到UPP地址 */

    /* NMI、PBIST、PIE控制寄存器段映射 */
    NmiIntruptRegsFile    : > NMIINTRUPT,   PAGE = 1       /* NMI中断段映射到NMIINTRUPT地址 */
    PbistRegsFile         : > PBIST,        PAGE = 1       /* PBIST段映射到PBIST地址 */
    PieCtrlRegsFile       : > PIE_CTRL,     PAGE = 1       /* PIE控制段映射到PIE_CTRL地址 */

    /* SCI寄存器段映射 */
    SciaRegsFile          : > SCIA,         PAGE = 1       /* SCI A段映射到SCIA地址 */
    ScibRegsFile          : > SCIB,         PAGE = 1       /* SCI B段映射到SCIB地址 */
    ScicRegsFile          : > SCIC,         PAGE = 1       /* SCI C段映射到SCIC地址 */
    ScidRegsFile          : > SCID,         PAGE = 1       /* SCI D段映射到SCID地址 */
    
    /* SDFM寄存器段映射 */
    Sdfm1RegsFile         : > SDFM1,        PAGE = 1       /* SDFM1段映射到SDFM1地址 */
    Sdfm2RegsFile         : > SDFM2,        PAGE = 1       /* SDFM2段映射到SDFM2地址 */

    /* SPI寄存器段映射 */
    SpiaRegsFile          : > SPIA,         PAGE = 1       /* SPI A段映射到SPIA地址 */
    SpibRegsFile          : > SPIB,         PAGE = 1       /* SPI B段映射到SPIB地址 */
    SpicRegsFile          : > SPIC,         PAGE = 1       /* SPI C段映射到SPIC地址 */
    SpidRegsFile          : > SPID,         PAGE = 1       /* SPI D段映射到SPID地址 */

    /* 设备配置、时钟配置、CPU系统寄存器段映射 */
    DevCfgRegsFile        : > DEV_CFG,      PAGE = 1       /* 设备配置段映射到DEV_CFG地址 */
    ClkCfgRegsFile        : > CLK_CFG,      PAGE = 1       /* 时钟配置段映射到CLK_CFG地址 */
    CpuSysRegsFile        : > CPU_SYS,      PAGE = 1       /* CPU系统段映射到CPU_SYS地址 */

    /* 触发、看门狗、外部中断、内存配置寄存器段映射 */
    TrigRegsFile          : > TRIG,         PAGE = 1       /* 触发段映射到TRIG地址 */
    WdRegsFile            : > WD,           PAGE = 1       /* 看门狗段映射到WD地址 */
    XintRegsFile          : > XINT          PAGE = 1       /* 外部中断段映射到XINT地址 */
    MemCfgRegs            : > MEMCFG        PAGE = 1       /* 内存配置段映射到MEMCFG地址 (冗余映射) */

}

/*
//===========================================================================
// End of file.
//===========================================================================
*/


