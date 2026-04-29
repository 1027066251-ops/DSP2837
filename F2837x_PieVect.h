//###########################################################################
//
// FILE:   F2837x_PieVect.h
//
// TITLE:  F2837x Device PIE Vector Table Definitions
//
//###########################################################################
// $TI Release: F2837xD Support Library v120 $
// $Release Date: Fri Aug 22 15:22:27 CDT 2014 $
//###########################################################################


//PIE Vect = Peripheral Interrupt Expansion Vector Table（中文：外设中断扩展向量表）
//F2837xD_pievect.h = 提供 F2837xD MCU PIE 向量表中断服务函数原型声明的头文件，用于建立外设中断源与 ISR 的对应关系。




#ifndef F2837x_PIE_VECT_H
#define F2837x_PIE_VECT_H
// 防止头文件被重复包含的预处理指令：
// 如果没有定义 F2837x_PIE_VECT_H 宏，则定义它并包含以下内容
// 如果已经定义过，则跳过整个文件内容

#ifdef __cplusplus
extern "C" {
#endif
// C++兼容性处理：
// 如果使用C++编译器编译，使用 extern "C" 确保函数名按C语言规则进行链接
// 防止C++编译器对函数名进行名称修饰（name mangling）

//---------------------------------------------------------------------------
// PIE Interrupt Vector Table Definition:
// PIE中断向量表定义：

// Create a user type called PINT (pointer to interrupt):
// 创建一个名为PINT的用户类型（指向中断函数的指针）：

typedef __interrupt void (*PINT)(void);
// 定义PINT类型为指向中断服务函数的指针
// __interrupt 是TI编译器特定的关键字，表示这是一个中断服务函数
// void (*PINT)(void) 表示PINT是一个指向无参数无返回值函数的指针
// 这个类型用于定义中断向量表中的各个向量项

// Define Vector Table:
// 定义向量表结构体：
struct PIE_VECT_TABLE {
    // PIE中断向量表结构体定义
    // 这个结构体包含了所有PIE中断向量的函数指针
    // 每个成员对应一个特定的中断源
    
    PINT  PIE1_RESERVED_INT;                          // Reserved 
    // 保留的中断向量1，位置：向量表索引0
    // 注释中的"Reserved"表示此向量位置保留未使用
    
    PINT  PIE2_RESERVED_INT;                          // Reserved 
    // 保留的中断向量2，位置：向量表索引1
    
    PINT  PIE3_RESERVED_INT;                          // Reserved 
    // 保留的中断向量3，位置：向量表索引2
    
    PINT  PIE4_RESERVED_INT;                          // Reserved 
    // 保留的中断向量4，位置：向量表索引3
    
    PINT  PIE5_RESERVED_INT;                          // Reserved 
    // 保留的中断向量5，位置：向量表索引4
    
    PINT  PIE6_RESERVED_INT;                          // Reserved 
    // 保留的中断向量6，位置：向量表索引5
    
    PINT  PIE7_RESERVED_INT;                          // Reserved 
    // 保留的中断向量7，位置：向量表索引6
    
    PINT  PIE8_RESERVED_INT;                          // Reserved 
    // 保留的中断向量8，位置：向量表索引7
    
    PINT  PIE9_RESERVED_INT;                          // Reserved 
    // 保留的中断向量9，位置：向量表索引8
    
    PINT  PIE10_RESERVED_INT;                         // Reserved 
    // 保留的中断向量10，位置：向量表索引9
    
    PINT  PIE11_RESERVED_INT;                         // Reserved 
    // 保留的中断向量11，位置：向量表索引10
    
    PINT  PIE12_RESERVED_INT;                         // Reserved 
    // 保留的中断向量12，位置：向量表索引11
    
    PINT  PIE13_RESERVED_INT;                         // Reserved 
    // 保留的中断向量13，位置：向量表索引12
    
    PINT  TIMER1_INT;                                 // CPU Timer 1 Interrupt 
    // CPU定时器1中断，位置：向量表索引13
    // CPU Timer 1的中断服务函数指针
    
    PINT  TIMER2_INT;                                 // CPU Timer 2 Interrupt 
    // CPU定时器2中断，位置：向量表索引14
    // CPU Timer 2的中断服务函数指针
    
    PINT  DATALOG_INT;                                // Datalogging Interrupt 
    // 数据记录中断，位置：向量表索引15
    // 数据记录模块的中断服务函数指针
    
    PINT  RTOS_INT;                                   // RTOS Interrupt 
    // 实时操作系统中断，位置：向量表索引16
    // RTOS相关的中断服务函数指针
    
    PINT  EMU_INT;                                    // Emulation Interrupt 
    // 仿真中断，位置：向量表索引17
    // 仿真器相关的中断服务函数指针
    
    PINT  NMI_INT;                                    // Non-Maskable Interrupt 
    // 不可屏蔽中断，位置：向量表索引18
    // NMI（Non-Maskable Interrupt）中断服务函数指针
    
    PINT  ILLEGAL_INT;                                // Illegal Operation Trap 
    // 非法操作陷阱，位置：向量表索引19
    // 当CPU执行非法操作时触发的中断
    
    PINT  USER1_INT;                                  // User Defined Trap 1 
    // 用户定义陷阱1，位置：向量表索引20
    // 用户自定义陷阱中断1的服务函数指针
    
    PINT  USER2_INT;                                  // User Defined Trap 2 
    // 用户定义陷阱2，位置：向量表索引21
    // 用户自定义陷阱中断2的服务函数指针
    
    PINT  USER3_INT;                                  // User Defined Trap 3 
    // 用户定义陷阱3，位置：向量表索引22
    // 用户自定义陷阱中断3的服务函数指针
    
    PINT  USER4_INT;                                  // User Defined Trap 4 
    // 用户定义陷阱4，位置：向量表索引23
    // 用户自定义陷阱中断4的服务函数指针
    
    PINT  USER5_INT;                                  // User Defined Trap 5 
    // 用户定义陷阱5，位置：向量表索引24
    // 用户自定义陷阱中断5的服务函数指针
    
    PINT  USER6_INT;                                  // User Defined Trap 6 
    // 用户定义陷阱6，位置：向量表索引25
    // 用户自定义陷阱中断6的服务函数指针
    
    PINT  USER7_INT;                                  // User Defined Trap 7 
    // 用户定义陷阱7，位置：向量表索引26
    // 用户自定义陷阱中断7的服务函数指针
    
    PINT  USER8_INT;                                  // User Defined Trap 8 
    // 用户定义陷阱8，位置：向量表索引27
    // 用户自定义陷阱中断8的服务函数指针
    
    PINT  USER9_INT;                                  // User Defined Trap 9 
    // 用户定义陷阱9，位置：向量表索引28
    // 用户自定义陷阱中断9的服务函数指针
    
    PINT  USER10_INT;                                 // User Defined Trap 10 
    // 用户定义陷阱10，位置：向量表索引29
    // 用户自定义陷阱中断10的服务函数指针
    
    PINT  USER11_INT;                                 // User Defined Trap 11 
    // 用户定义陷阱11，位置：向量表索引30
    // 用户自定义陷阱中断11的服务函数指针
    
    PINT  USER12_INT;                                 // User Defined Trap 12 
    // 用户定义陷阱12，位置：向量表索引31
    // 用户自定义陷阱中断12的服务函数指针
    
    PINT  ADCA1_INT;                                  // 1.1 - ADCA Interrupt 1 
    // ADCA中断1，位置：向量表索引32，对应PIE组1向量1
    // ADCA模块的第一个中断服务函数指针
    // 注释中的"1.1"表示PIE组1的第1个中断
    
    PINT  ADCB1_INT;                                  // 1.2 - ADCB Interrupt 1 
    // ADCB中断1，位置：向量表索引33，对应PIE组1向量2
    // ADCB模块的第一个中断服务函数指针
    
    PINT  ADCC1_INT;                                  // 1.3 - ADCC Interrupt 1 
    // ADCC中断1，位置：向量表索引34，对应PIE组1向量3
    // ADCC模块的第一个中断服务函数指针
    
    PINT  XINT1_INT;                                  // 1.4 - XINT1 Interrupt 
    // 外部中断1，位置：向量表索引35，对应PIE组1向量4
    // 外部中断1的服务函数指针
    
    PINT  XINT2_INT;                                  // 1.5 - XINT2 Interrupt 
    // 外部中断2，位置：向量表索引36，对应PIE组1向量5
    // 外部中断2的服务函数指针
    
    PINT  ADCD1_INT;                                  // 1.6 - ADCD Interrupt 1 
    // ADCD中断1，位置：向量表索引37，对应PIE组1向量6
    // ADCD模块的第一个中断服务函数指针
    
    PINT  TIMER0_INT;                                 // 1.7 - Timer 0 Interrupt 
    // 定时器0中断，位置：向量表索引38，对应PIE组1向量7
    // Timer 0的中断服务函数指针
    
    PINT  WAKE_INT;                                   // 1.8 - Standby and Halt Wakeup Interrupt 
    // 待机和停机唤醒中断，位置：向量表索引39，对应PIE组1向量8
    // 从低功耗模式唤醒的中断服务函数指针
    
    PINT  EPWM1_TZ_INT;                               // 2.1 - ePWM1 Trip Zone Interrupt 
    // ePWM1跳闸区中断，位置：向量表索引40，对应PIE组2向量1
    // ePWM1模块跳闸区保护中断服务函数指针
    
    PINT  EPWM2_TZ_INT;                               // 2.2 - ePWM2 Trip Zone Interrupt 
    // ePWM2跳闸区中断，位置：向量表索引41，对应PIE组2向量2
    // ePWM2模块跳闸区保护中断服务函数指针
    
    PINT  EPWM3_TZ_INT;                               // 2.3 - ePWM3 Trip Zone Interrupt 
    // ePWM3跳闸区中断，位置：向量表索引42，对应PIE组2向量3
    // ePWM3模块跳闸区保护中断服务函数指针
    
    PINT  EPWM4_TZ_INT;                               // 2.4 - ePWM4 Trip Zone Interrupt 
    // ePWM4跳闸区中断，位置：向量表索引43，对应PIE组2向量4
    // ePWM4模块跳闸区保护中断服务函数指针
    
    PINT  EPWM5_TZ_INT;                               // 2.5 - ePWM5 Trip Zone Interrupt 
    // ePWM5跳闸区中断，位置：向量表索引44，对应PIE组2向量5
    // ePWM5模块跳闸区保护中断服务函数指针
    
    PINT  EPWM6_TZ_INT;                               // 2.6 - ePWM6 Trip Zone Interrupt 
    // ePWM6跳闸区中断，位置：向量表索引45，对应PIE组2向量6
    // ePWM6模块跳闸区保护中断服务函数指针
    
    PINT  EPWM7_TZ_INT;                               // 2.7 - ePWM7 Trip Zone Interrupt 
    // ePWM7跳闸区中断，位置：向量表索引46，对应PIE组2向量7
    // ePWM7模块跳闸区保护中断服务函数指针
    
    PINT  EPWM8_TZ_INT;                               // 2.8 - ePWM8 Trip Zone Interrupt 
    // ePWM8跳闸区中断，位置：向量表索引47，对应PIE组2向量8
    // ePWM8模块跳闸区保护中断服务函数指针
    
    PINT  EPWM1_INT;                                  // 3.1 - ePWM1 Interrupt 
    // ePWM1中断，位置：向量表索引48，对应PIE组3向量1
    // ePWM1模块的一般中断服务函数指针
    
    PINT  EPWM2_INT;                                  // 3.2 - ePWM2 Interrupt 
    // ePWM2中断，位置：向量表索引49，对应PIE组3向量2
    // ePWM2模块的一般中断服务函数指针
    
    PINT  EPWM3_INT;                                  // 3.3 - ePWM3 Interrupt 
    // ePWM3中断，位置：向量表索引50，对应PIE组3向量3
    // ePWM3模块的一般中断服务函数指针
    
    PINT  EPWM4_INT;                                  // 3.4 - ePWM4 Interrupt 
    // ePWM4中断，位置：向量表索引51，对应PIE组3向量4
    // ePWM4模块的一般中断服务函数指针
    
    PINT  EPWM5_INT;                                  // 3.5 - ePWM5 Interrupt 
    // ePWM5中断，位置：向量表索引52，对应PIE组3向量5
    // ePWM5模块的一般中断服务函数指针
    
    PINT  EPWM6_INT;                                  // 3.6 - ePWM6 Interrupt 
    // ePWM6中断，位置：向量表索引53，对应PIE组3向量6
    // ePWM6模块的一般中断服务函数指针
    
    PINT  EPWM7_INT;                                  // 3.7 - ePWM7 Interrupt 
    // ePWM7中断，位置：向量表索引54，对应PIE组3向量7
    // ePWM7模块的一般中断服务函数指针
    
    PINT  EPWM8_INT;                                  // 3.8 - ePWM8 Interrupt 
    // ePWM8中断，位置：向量表索引55，对应PIE组3向量8
    // ePWM8模块的一般中断服务函数指针
    
    PINT  ECAP1_INT;                                  // 4.1 - eCAP1 Interrupt 
    // eCAP1中断，位置：向量表索引56，对应PIE组4向量1
    // eCAP1模块的中断服务函数指针
    
    PINT  ECAP2_INT;                                  // 4.2 - eCAP2 Interrupt 
    // eCAP2中断，位置：向量表索引57，对应PIE组4向量2
    // eCAP2模块的中断服务函数指针
    
    PINT  ECAP3_INT;                                  // 4.3 - eCAP3 Interrupt 
    // eCAP3中断，位置：向量表索引58，对应PIE组4向量3
    // eCAP3模块的中断服务函数指针
    
    PINT  ECAP4_INT;                                  // 4.4 - eCAP4 Interrupt 
    // eCAP4中断，位置：向量表索引59，对应PIE组4向量4
    // eCAP4模块的中断服务函数指针
    
    PINT  ECAP5_INT;                                  // 4.5 - eCAP5 Interrupt 
    // eCAP5中断，位置：向量表索引60，对应PIE组4向量5
    // eCAP5模块的中断服务函数指针
    
    PINT  ECAP6_INT;                                  // 4.6 - eCAP6 Interrupt 
    // eCAP6中断，位置：向量表索引61，对应PIE组4向量6
    // eCAP6模块的中断服务函数指针
    
    PINT  PIE14_RESERVED_INT;                         // 4.7 - Reserved 
    // 保留中断14，位置：向量表索引62，对应PIE组4向量7
    // 保留位置，未分配具体中断
    
    PINT  PIE15_RESERVED_INT;                         // 4.8 - Reserved 
    // 保留中断15，位置：向量表索引63，对应PIE组4向量8
    // 保留位置，未分配具体中断
    
    PINT  EQEP1_INT;                                  // 5.1 - eQEP1 Interrupt 
    // eQEP1中断，位置：向量表索引64，对应PIE组5向量1
    // eQEP1模块的中断服务函数指针
    
    PINT  EQEP2_INT;                                  // 5.2 - eQEP2 Interrupt 
    // eQEP2中断，位置：向量表索引65，对应PIE组5向量2
    // eQEP2模块的中断服务函数指针
    
    PINT  EQEP3_INT;                                  // 5.3 - eQEP3 Interrupt 
    // eQEP3中断，位置：向量表索引66，对应PIE组5向量3
    // eQEP3模块的中断服务函数指针
    
    PINT  PIE16_RESERVED_INT;                         // 5.4 - Reserved 
    // 保留中断16，位置：向量表索引67，对应PIE组5向量4
    // 保留位置，未分配具体中断
    
    PINT  PIE17_RESERVED_INT;                         // 5.5 - Reserved 
    // 保留中断17，位置：向量表索引68，对应PIE组5向量5
    // 保留位置，未分配具体中断
    
    PINT  PIE18_RESERVED_INT;                         // 5.6 - Reserved 
    // 保留中断18，位置：向量表索引69，对应PIE组5向量6
    // 保留位置，未分配具体中断
    
    PINT  PIE19_RESERVED_INT;                         // 5.7 - Reserved 
    // 保留中断19，位置：向量表索引70，对应PIE组5向量7
    // 保留位置，未分配具体中断
    
    PINT  PIE20_RESERVED_INT;                         // 5.8 - Reserved 
    // 保留中断20，位置：向量表索引71，对应PIE组5向量8
    // 保留位置，未分配具体中断
    
    PINT  SPIA_RX_INT;                                // 6.1 - SPIA Receive Interrupt 
    // SPIA接收中断，位置：向量表索引72，对应PIE组6向量1
    // SPIA模块接收完成中断服务函数指针
    
    PINT  SPIA_TX_INT;                                // 6.2 - SPIA Transmit Interrupt 
    // SPIA发送中断，位置：向量表索引73，对应PIE组6向量2
    // SPIA模块发送完成中断服务函数指针
    
    PINT  SPIB_RX_INT;                                // 6.3 - SPIB Receive Interrupt 
    // SPIB接收中断，位置：向量表索引74，对应PIE组6向量3
    // SPIB模块接收完成中断服务函数指针
    
    PINT  SPIB_TX_INT;                                // 6.4 - SPIB Transmit Interrupt 
    // SPIB发送中断，位置：向量表索引75，对应PIE组6向量4
    // SPIB模块发送完成中断服务函数指针
    
    PINT  MCBSPA_RX_INT;                              // 6.5 - McBSPA Receive Interrupt 
    // McBSPA接收中断，位置：向量表索引76，对应PIE组6向量5
    // McBSPA模块接收完成中断服务函数指针
    
    PINT  MCBSPA_TX_INT;                              // 6.6 - McBSPA Transmit Interrupt 
    // McBSPA发送中断，位置：向量表索引77，对应PIE组6向量6
    // McBSPA模块发送完成中断服务函数指针
    
    PINT  MCBSPB_RX_INT;                              // 6.7 - McBSPB Receive Interrupt 
    // McBSPB接收中断，位置：向量表索引78，对应PIE组6向量7
    // McBSPB模块接收完成中断服务函数指针
    
    PINT  MCBSPB_TX_INT;                              // 6.8 - McBSPB Transmit Interrupt 
    // McBSPB发送中断，位置：向量表索引79，对应PIE组6向量8
    // McBSPB模块发送完成中断服务函数指针
    
    PINT  DMA_CH1_INT;                                // 7.1 - DMA Channel 1 Interrupt 
    // DMA通道1中断，位置：向量表索引80，对应PIE组7向量1
    // DMA通道1传输完成中断服务函数指针
    
    PINT  DMA_CH2_INT;                                // 7.2 - DMA Channel 2 Interrupt 
    // DMA通道2中断，位置：向量表索引81，对应PIE组7向量2
    // DMA通道2传输完成中断服务函数指针
    
    PINT  DMA_CH3_INT;                                // 7.3 - DMA Channel 3 Interrupt 
    // DMA通道3中断，位置：向量表索引82，对应PIE组7向量3
    // DMA通道3传输完成中断服务函数指针
    
    PINT  DMA_CH4_INT;                                // 7.4 - DMA Channel 4 Interrupt 
    // DMA通道4中断，位置：向量表索引83，对应PIE组7向量4
    // DMA通道4传输完成中断服务函数指针
    
    PINT  DMA_CH5_INT;                                // 7.5 - DMA Channel 5 Interrupt 
    // DMA通道5中断，位置：向量表索引84，对应PIE组7向量5
    // DMA通道5传输完成中断服务函数指针
    
    PINT  DMA_CH6_INT;                                // 7.6 - DMA Channel 6 Interrupt 
    // DMA通道6中断，位置：向量表索引85，对应PIE组7向量6
    // DMA通道6传输完成中断服务函数指针
    
    PINT  PIE21_RESERVED_INT;                         // 7.7 - Reserved 
    // 保留中断21，位置：向量表索引86，对应PIE组7向量7
    // 保留位置，未分配具体中断
    
    PINT  PIE22_RESERVED_INT;                         // 7.8 - Reserved 
    // 保留中断22，位置：向量表索引87，对应PIE组7向量8
    // 保留位置，未分配具体中断
    
    PINT  I2CA_INT;                                   // 8.1 - I2CA Interrupt 1 
    // I2CA中断1，位置：向量表索引88，对应PIE组8向量1
    // I2CA模块的第一个中断服务函数指针
    
    PINT  I2CA_FIFO_INT;                              // 8.2 - I2CA Interrupt 2 
    // I2CA FIFO中断，位置：向量表索引89，对应PIE组8向量2
    // I2CA模块FIFO相关中断服务函数指针
    
    PINT  I2CB_INT;                                   // 8.3 - I2CB Interrupt 1 
    // I2CB中断1，位置：向量表索引90，对应PIE组8向量3
    // I2CB模块的第一个中断服务函数指针
    
    PINT  I2CB_FIFO_INT;                              // 8.4 - I2CB Interrupt 2 
    // I2CB FIFO中断，位置：向量表索引91，对应PIE组8向量4
    // I2CB模块FIFO相关中断服务函数指针
    
    PINT  SCIC_RX_INT;                                // 8.5 - SCIC Receive Interrupt 
    // SCIC接收中断，位置：向量表索引92，对应PIE组8向量5
    // SCIC模块接收完成中断服务函数指针
    
    PINT  SCIC_TX_INT;                                // 8.6 - SCIC Transmit Interrupt 
    // SCIC发送中断，位置：向量表索引93，对应PIE组8向量6
    // SCIC模块发送完成中断服务函数指针
    
    PINT  SCID_RX_INT;                                // 8.7 - SCID Receive Interrupt 
    // SCID接收中断，位置：向量表索引94，对应PIE组8向量7
    // SCID模块接收完成中断服务函数指针
    
    PINT  SCID_TX_INT;                                // 8.8 - SCID Transmit Interrupt 
    // SCID发送中断，位置：向量表索引95，对应PIE组8向量8
    // SCID模块发送完成中断服务函数指针
    
    PINT  SCIA_RX_INT;                                // 9.1 - SCIA Receive Interrupt 
    // SCIA接收中断，位置：向量表索引96，对应PIE组9向量1
    // SCIA模块接收完成中断服务函数指针
    
    PINT  SCIA_TX_INT;                                // 9.2 - SCIA Transmit Interrupt 
    // SCIA发送中断，位置：向量表索引97，对应PIE组9向量2
    // SCIA模块发送完成中断服务函数指针
    
    PINT  SCIB_RX_INT;                                // 9.3 - SCIB Receive Interrupt 
    // SCIB接收中断，位置：向量表索引98，对应PIE组9向量3
    // SCIB模块接收完成中断服务函数指针
    
    PINT  SCIB_TX_INT;                                // 9.4 - SCIB Transmit Interrupt 
    // SCIB发送中断，位置：向量表索引99，对应PIE组9向量4
    // SCIB模块发送完成中断服务函数指针
    
    PINT  DCANA_1_INT;                                // 9.5 - DCANA Interrupt 1 
    // DCANA中断1，位置：向量表索引100，对应PIE组9向量5
    // DCANA模块的第一个中断服务函数指针
    
    PINT  DCANA_2_INT;                                // 9.6 - DCANA Interrupt 2 
    // DCANA中断2，位置：向量表索引101，对应PIE组9向量6
    // DCANA模块的第二个中断服务函数指针
    
    PINT  DCANB_1_INT;                                // 9.7 - DCANB Interrupt 1 
    // DCANB中断1，位置：向量表索引102，对应PIE组9向量7
    // DCANB模块的第一个中断服务函数指针
    
    PINT  DCANB_2_INT;                                // 9.8 - DCANB Interrupt 2 
    // DCANB中断2，位置：向量表索引103，对应PIE组9向量8
    // DCANB模块的第二个中断服务函数指针
    
    PINT  ADCA_EVT_INT;                               // 10.1 - ADCA Event Interrupt 
    // ADCA事件中断，位置：向量表索引104，对应PIE组10向量1
    // ADCA模块事件中断服务函数指针
    
    PINT  ADCA2_INT;                                  // 10.2 - ADCA Interrupt 2 
    // ADCA中断2，位置：向量表索引105，对应PIE组10向量2
    // ADCA模块的第二个中断服务函数指针
    
    PINT  ADCA3_INT;                                  // 10.3 - ADCA Interrupt 3 
    // ADCA中断3，位置：向量表索引106，对应PIE组10向量3
    // ADCA模块的第三个中断服务函数指针
    
    PINT  ADCA4_INT;                                  // 10.4 - ADCA Interrupt 4 
    // ADCA中断4，位置：向量表索引107，对应PIE组10向量4
    // ADCA模块的第四个中断服务函数指针
    
    PINT  ADCB_EVT_INT;                               // 10.5 - ADCB Event Interrupt 
    // ADCB事件中断，位置：向量表索引108，对应PIE组10向量5
    // ADCB模块事件中断服务函数指针
    
    PINT  ADCB2_INT;                                  // 10.6 - ADCB Interrupt 2 
    // ADCB中断2，位置：向量表索引109，对应PIE组10向量6
    // ADCB模块的第二个中断服务函数指针
    
    PINT  ADCB3_INT;                                  // 10.7 - ADCB Interrupt 3 
    // ADCB中断3，位置：向量表索引110，对应PIE组10向量7
    // ADCB模块的第三个中断服务函数指针
    
    PINT  ADCB4_INT;                                  // 10.8 - ADCB Interrupt 4 
    // ADCB中断4，位置：向量表索引111，对应PIE组10向量8
    // ADCB模块的第四个中断服务函数指针
    
    PINT  CLA1_1_INT;                                 // 11.1 - CLA1 Interrupt 1 
    // CLA1中断1，位置：向量表索引112，对应PIE组11向量1
    // CLA1（控制律加速器）模块的第一个中断服务函数指针
    
    PINT  CLA1_2_INT;                                 // 11.2 - CLA1 Interrupt 2 
    // CLA1中断2，位置：向量表索引113，对应PIE组11向量2
    // CLA1模块的第二个中断服务函数指针
    
    PINT  CLA1_3_INT;                                 // 11.3 - CLA1 Interrupt 3 
    // CLA1中断3，位置：向量表索引114，对应PIE组11向量3
    // CLA1模块的第三个中断服务函数指针
    
    PINT  CLA1_4_INT;                                 // 11.4 - CLA1 Interrupt 4 
    // CLA1中断4，位置：向量表索引115，对应PIE组11向量4
    // CLA1模块的第四个中断服务函数指针
    
    PINT  CLA1_5_INT;                                 // 11.5 - CLA1 Interrupt 5 
    // CLA1中断5，位置：向量表索引116，对应PIE组11向量5
    // CLA1模块的第五个中断服务函数指针
    
    PINT  CLA1_6_INT;                                 // 11.6 - CLA1 Interrupt 6 
    // CLA1中断6，位置：向量表索引117，对应PIE组11向量6
    // CLA1模块的第六个中断服务函数指针
    
    PINT  CLA1_7_INT;                                 // 11.7 - CLA1 Interrupt 7 
    // CLA1中断7，位置：向量表索引118，对应PIE组11向量7
    // CLA1模块的第七个中断服务函数指针
    
    PINT  CLA1_8_INT;                                 // 11.8 - CLA1 Interrupt 8 
    // CLA1中断8，位置：向量表索引119，对应PIE组11向量8
    // CLA1模块的第八个中断服务函数指针
    
    PINT  XINT3_INT;                                  // 12.1 - XINT3 Interrupt 
    // 外部中断3，位置：向量表索引120，对应PIE组12向量1
    // 外部中断3的服务函数指针
    
    PINT  XINT4_INT;                                  // 12.2 - XINT4 Interrupt 
    // 外部中断4，位置：向量表索引121，对应PIE组12向量2
    // 外部中断4的服务函数指针
    
    PINT  XINT5_INT;                                  // 12.3 - XINT5 Interrupt 
    // 外部中断5，位置：向量表索引122，对应PIE组12向量3
    // 外部中断5的服务函数指针
    
    PINT  PBIST_INT;                                  // 12.4 - PBIST Interrupt 
    // PBIST中断，位置：向量表索引123，对应PIE组12向量4
    // PBIST（可编程内置自测试）模块中断服务函数指针
    
    PINT  PIE23_RESERVED_INT;                         // 12.5 - Reserved 
    // 保留中断23，位置：向量表索引124，对应PIE组12向量5
    // 保留位置，未分配具体中断
    
    PINT  VCU_INT;                                    // 12.6 - VCU Interrupt 
    // VCU中断，位置：向量表索引125，对应PIE组12向量6
    // VCU（Viterbi/Complex Unit）模块中断服务函数指针
    
    PINT  FPU_OVERFLOW_INT;                           // 12.7 - FPU Overflow Interrupt 
    // FPU溢出中断，位置：向量表索引126，对应PIE组12向量7
    // 浮点单元溢出中断服务函数指针
    
    PINT  FPU_UNDERFLOW_INT;                          // 12.8 - FPU Underflow Interrupt 
    // FPU下溢中断，位置：向量表索引127，对应PIE组12向量8
    // 浮点单元下溢中断服务函数指针
    
    PINT  PIE24_RESERVED_INT;                         // 1.9 - Reserved 
    // 保留中断24，位置：向量表索引128，对应PIE组1向量9
    // 保留位置，未分配具体中断，注意注释中的"1.9"表示PIE组1的第9个向量
    
    PINT  PIE25_RESERVED_INT;                         // 1.10 - Reserved 
    // 保留中断25，位置：向量表索引129，对应PIE组1向量10
    
    PINT  PIE26_RESERVED_INT;                         // 1.11 - Reserved 
    // 保留中断26，位置：向量表索引130，对应PIE组1向量11
    
    PINT  PIE27_RESERVED_INT;                         // 1.12 - Reserved 
    // 保留中断27，位置：向量表索引131，对应PIE组1向量12
    
    PINT  IPC0_INT;                                   // 1.13 - IPC Interrupt 1
    // IPC中断0，位置：向量表索引132，对应PIE组1向量13
    // IPC（处理器间通信）中断0的服务函数指针
    
    PINT  IPC1_INT;                                   // 1.14 - IPC Interrupt 2
    // IPC中断1，位置：向量表索引133，对应PIE组1向量14
    // IPC中断1的服务函数指针
    
    PINT  IPC2_INT;                                   // 1.15 - IPC Interrupt 3
    // IPC中断2，位置：向量表索引134，对应PIE组1向量15
    // IPC中断2的服务函数指针
    
    PINT  IPC3_INT;                                   // 1.16 - IPC Interrupt 4
    // IPC中断3，位置：向量表索引135，对应PIE组1向量16
    // IPC中断3的服务函数指针
    
    PINT  EPWM9_TZ_INT;                               // 2.9 - ePWM9 Trip Zone Interrupt 
    // ePWM9跳闸区中断，位置：向量表索引136，对应PIE组2向量9
    // ePWM9模块跳闸区保护中断服务函数指针
    
    PINT  EPWM10_TZ_INT;                              // 2.10 - ePWM10 Trip Zone Interrupt 
    // ePWM10跳闸区中断，位置：向量表索引137，对应PIE组2向量10
    // ePWM10模块跳闸区保护中断服务函数指针
    
    PINT  EPWM11_TZ_INT;                              // 2.11 - ePWM11 Trip Zone Interrupt 
    // ePWM11跳闸区中断，位置：向量表索引138，对应PIE组2向量11
    // ePWM11模块跳闸区保护中断服务函数指针
    
    PINT  EPWM12_TZ_INT;                              // 2.12 - ePWM12 Trip Zone Interrupt 
    // ePWM12跳闸区中断，位置：向量表索引139，对应PIE组2向量12
    // ePWM12模块跳闸区保护中断服务函数指针
    
    PINT  PIE28_RESERVED_INT;                         // 2.13 - Reserved 
    // 保留中断28，位置：向量表索引140，对应PIE组2向量13
    
    PINT  PIE29_RESERVED_INT;                         // 2.14 - Reserved 
    // 保留中断29，位置：向量表索引141，对应PIE组2向量14
    
    PINT  PIE30_RESERVED_INT;                         // 2.15 - Reserved 
    // 保留中断30，位置：向量表索引142，对应PIE组2向量15
    
    PINT  PIE31_RESERVED_INT;                         // 2.16 - Reserved 
    // 保留中断31，位置：向量表索引143，对应PIE组2向量16
    
    PINT  EPWM9_INT;                                  // 3.9 - ePWM9 Interrupt 
    // ePWM9中断，位置：向量表索引144，对应PIE组3向量9
    // ePWM9模块的一般中断服务函数指针
    
    PINT  EPWM10_INT;                                 // 3.10 - ePWM10 Interrupt 
    // ePWM10中断，位置：向量表索引145，对应PIE组3向量10
    // ePWM10模块的一般中断服务函数指针
    
    PINT  EPWM11_INT;                                 // 3.11 - ePWM11 Interrupt 
    // ePWM11中断，位置：向量表索引146，对应PIE组3向量11
    // ePWM11模块的一般中断服务函数指针
    
    PINT  EPWM12_INT;                                 // 3.12 - ePWM12 Interrupt 
    // ePWM12中断，位置：向量表索引147，对应PIE组3向量12
    // ePWM12模块的一般中断服务函数指针
    
    PINT  PIE32_RESERVED_INT;                         // 3.13 - Reserved 
    // 保留中断32，位置：向量表索引148，对应PIE组3向量13
    
    PINT  PIE33_RESERVED_INT;                         // 3.14 - Reserved 
    // 保留中断33，位置：向量表索引149，对应PIE组3向量14
    
    PINT  PIE34_RESERVED_INT;                         // 3.15 - Reserved 
    // 保留中断34，位置：向量表索引150，对应PIE组3向量15
    
    PINT  PIE35_RESERVED_INT;                         // 3.16 - Reserved 
    // 保留中断35，位置：向量表索引151，对应PIE组3向量16
    
    PINT  PIE36_RESERVED_INT;                         // 4.9 - Reserved 
    // 保留中断36，位置：向量表索引152，对应PIE组4向量9
    
    PINT  PIE37_RESERVED_INT;                         // 4.10 - Reserved 
    // 保留中断37，位置：向量表索引153，对应PIE组4向量10
    
    PINT  PIE38_RESERVED_INT;                         // 4.11 - Reserved 
    // 保留中断38，位置：向量表索引154，对应PIE组4向量11
    
    PINT  PIE39_RESERVED_INT;                         // 4.12 - Reserved 
    // 保留中断39，位置：向量表索引155，对应PIE组4向量12
    
    PINT  PIE40_RESERVED_INT;                         // 4.13 - Reserved 
    // 保留中断40，位置：向量表索引156，对应PIE组4向量13
    
    PINT  PIE41_RESERVED_INT;                         // 4.14 - Reserved 
    // 保留中断41，位置：向量表索引157，对应PIE组4向量14
    
    PINT  PIE42_RESERVED_INT;                         // 4.15 - Reserved 
    // 保留中断42，位置：向量表索引158，对应PIE组4向量15
    
    PINT  PIE43_RESERVED_INT;                         // 4.16 - Reserved 
    // 保留中断43，位置：向量表索引159，对应PIE组4向量16
    
    PINT  SD1_INT;                                    // 5.9 - SD1 Interrupt 
    // SD1中断，位置：向量表索引160，对应PIE组5向量9
    // SD1（Sigma-Delta）模块中断服务函数指针
    
    PINT  SD2_INT;                                    // 5.10 - SD2 Interrupt 
    // SD2中断，位置：向量表索引161，对应PIE组5向量10
    // SD2（Sigma-Delta）模块中断服务函数指针
    
    PINT  PIE44_RESERVED_INT;                         // 5.11 - Reserved 
    // 保留中断44，位置：向量表索引162，对应PIE组5向量11
    
    PINT  PIE45_RESERVED_INT;                         // 5.12 - Reserved 
    // 保留中断45，位置：向量表索引163，对应PIE组5向量12
    
    PINT  PIE46_RESERVED_INT;                         // 5.13 - Reserved 
    // 保留中断46，位置：向量表索引164，对应PIE组5向量13
    
    PINT  PIE47_RESERVED_INT;                         // 5.14 - Reserved 
    // 保留中断47，位置：向量表索引165，对应PIE组5向量14
    
    PINT  PIE48_RESERVED_INT;                         // 5.15 - Reserved 
    // 保留中断48，位置：向量表索引166，对应PIE组5向量15
    
    PINT  PIE49_RESERVED_INT;                         // 5.16 - Reserved 
    // 保留中断49，位置：向量表索引167，对应PIE组5向量16
    
    PINT  SPIC_RX_INT;                                // 6.9 - SPIC Receive Interrupt 
    // SPIC接收中断，位置：向量表索引168，对应PIE组6向量9
    // SPIC模块接收完成中断服务函数指针
    
    PINT  SPIC_TX_INT;                                // 6.10 - SPIC Transmit Interrupt 
    // SPIC发送中断，位置：向量表索引169，对应PIE组6向量10
    // SPIC模块发送完成中断服务函数指针
    
    PINT  PIE50_RESERVED_INT;                         // 6.11 - Reserved 
    // 保留中断50，位置：向量表索引170，对应PIE组6向量11
    
    PINT  PIE51_RESERVED_INT;                         // 6.12 - Reserved 
    // 保留中断51，位置：向量表索引171，对应PIE组6向量12
    
    PINT  PIE52_RESERVED_INT;                         // 6.13 - Reserved 
    // 保留中断52，位置：向量表索引172，对应PIE组6向量13
    
    PINT  PIE53_RESERVED_INT;                         // 6.14 - Reserved 
    // 保留中断53，位置：向量表索引173，对应PIE组6向量14
    
    PINT  PIE54_RESERVED_INT;                         // 6.15 - Reserved 
    // 保留中断54，位置：向量表索引174，对应PIE组6向量15
    
    PINT  PIE55_RESERVED_INT;                         // 6.16 - Reserved 
    // 保留中断55，位置：向量表索引175，对应PIE组6向量16
    
    PINT  PIE56_RESERVED_INT;                         // 7.9 - Reserved 
    // 保留中断56，位置：向量表索引176，对应PIE组7向量9
    
    PINT  PIE57_RESERVED_INT;                         // 7.10 - Reserved 
    // 保留中断57，位置：向量表索引177，对应PIE组7向量10
    
    PINT  PIE58_RESERVED_INT;                         // 7.11 - Reserved 
    // 保留中断58，位置：向量表索引178，对应PIE组7向量11
    
    PINT  PIE59_RESERVED_INT;                         // 7.12 - Reserved 
    // 保留中断59，位置：向量表索引179，对应PIE组7向量12
    
    PINT  PIE60_RESERVED_INT;                         // 7.13 - Reserved 
    // 保留中断60，位置：向量表索引180，对应PIE组7向量13
    
    PINT  PIE61_RESERVED_INT;                         // 7.14 - Reserved 
    // 保留中断61，位置：向量表索引181，对应PIE组7向量14
    
    PINT  PIE62_RESERVED_INT;                         // 7.15 - Reserved 
    // 保留中断62，位置：向量表索引182，对应PIE组7向量15
    
    PINT  PIE63_RESERVED_INT;                         // 7.16 - Reserved 
    // 保留中断63，位置：向量表索引183，对应PIE组7向量16
    
    PINT  PIE64_RESERVED_INT;                         // 8.9 - Reserved 
    // 保留中断64，位置：向量表索引184，对应PIE组8向量9
    
    PINT  PIE65_RESERVED_INT;                         // 8.10 - Reserved 
    // 保留中断65，位置：向量表索引185，对应PIE组8向量10
    
    PINT  PIE66_RESERVED_INT;                         // 8.11 - Reserved 
    // 保留中断66，位置：向量表索引186，对应PIE组8向量11
    
    PINT  PIE67_RESERVED_INT;                         // 8.12 - Reserved 
    // 保留中断67，位置：向量表索引187，对应PIE组8向量12
    
    PINT  PIE68_RESERVED_INT;                         // 8.13 - Reserved 
    // 保留中断68，位置：向量表索引188，对应PIE组8向量13
    
    PINT  PIE69_RESERVED_INT;                         // 8.14 - Reserved 
    // 保留中断69，位置：向量表索引189，对应PIE组8向量14
    
#ifdef CPU1
    // CPU1条件编译块：
    // 只有在定义了CPU1宏的情况下，才包含以下代码
    // 这表明某些中断向量是CPU1特有的
    
    PINT  UPPA_INT;                                   // 8.15 - uPPA Interrupt 
    // uPPA中断，位置：向量表索引190，对应PIE组8向量15
    // uPPA（通用并行端口A）模块中断服务函数指针
    // 仅CPU1有此中断
    
    PINT  PIE71_RESERVED_INT;                         // 8.16 - Reserved 
    // 保留中断71，位置：向量表索引191，对应PIE组8向量16
    // 保留位置，未分配具体中断，仅CPU1
    
#elif defined(CPU2)
    // CPU2条件编译块：
    // 只有在定义了CPU2宏的情况下，才包含以下代码
    // 这表明某些中断向量是CPU2特有的或与CPU1不同
    
    PINT  PIE70_RESERVED_INT;                         // 8.15 - Reserved 
    // 保留中断70，位置：向量表索引190，对应PIE组8向量15
    // 保留位置，未分配具体中断，仅CPU2
    
    PINT  PIE72_RESERVED_INT;                         // 8.16 - Reserved 
    // 保留中断72，位置：向量表索引191，对应PIE组8向量16
    // 保留位置，未分配具体中断，仅CPU2
    
#endif
    // 结束CPU1/CPU2条件编译块
    
    PINT  PIE73_RESERVED_INT;                         // 9.9 - Reserved 
    // 保留中断73，位置：向量表索引192，对应PIE组9向量9
    
    PINT  PIE74_RESERVED_INT;                         // 9.10 - Reserved 
    // 保留中断74，位置：向量表索引193，对应PIE组9向量10
    
    PINT  PIE75_RESERVED_INT;                         // 9.11 - Reserved 
    // 保留中断75，位置：向量表索引194，对应PIE组9向量11
    
    PINT  PIE76_RESERVED_INT;                         // 9.12 - Reserved 
    // 保留中断76，位置：向量表索引195，对应PIE组9向量12
    
    PINT  PIE77_RESERVED_INT;                         // 9.13 - Reserved 
    // 保留中断77，位置：向量表索引196，对应PIE组9向量13
    
    PINT  PIE78_RESERVED_INT;                         // 9.14 - Reserved 
    // 保留中断78，位置：向量表索引197，对应PIE组9向量14
    
#ifdef CPU1
    // CPU1条件编译块：
    
    PINT  USBA_INT;                                   // 9.15 - USBA Interrupt 
    // USBA中断，位置：向量表索引198，对应PIE组9向量15
    // USB模块A中断服务函数指针
    // 仅CPU1有此中断
    
#elif defined(CPU2)
    // CPU2条件编译块：
    
    PINT  PIE79_RESERVED_INT;                         // 9.15 - Reserved 
    // 保留中断79，位置：向量表索引198，对应PIE组9向量15
    // 保留位置，未分配具体中断，仅CPU2
    
#endif
    // 结束CPU1/CPU2条件编译块
    
    PINT  PIE80_RESERVED_INT;                         // 9.16 - Reserved 
    // 保留中断80，位置：向量表索引199，对应PIE组9向量16
    
    PINT  ADCC_EVT_INT;                               // 10.9 - ADCC Event Interrupt 
    // ADCC事件中断，位置：向量表索引200，对应PIE组10向量9
    // ADCC模块事件中断服务函数指针
    
    PINT  ADCC2_INT;                                  // 10.10 - ADCC Interrupt 2 
    // ADCC中断2，位置：向量表索引201，对应PIE组10向量10
    // ADCC模块的第二个中断服务函数指针
    
    PINT  ADCC3_INT;                                  // 10.11 - ADCC Interrupt 3 
    // ADCC中断3，位置：向量表索引202，对应PIE组10向量11
    // ADCC模块的第三个中断服务函数指针
    
    PINT  ADCC4_INT;                                  // 10.12 - ADCC Interrupt 4 
    // ADCC中断4，位置：向量表索引203，对应PIE组10向量12
    // ADCC模块的第四个中断服务函数指针
    
    PINT  ADCD_EVT_INT;                               // 10.13 - ADCD Event Interrupt 
    // ADCD事件中断，位置：向量表索引204，对应PIE组10向量13
    // ADCD模块事件中断服务函数指针
    
    PINT  ADCD2_INT;                                  // 10.14 - ADCD Interrupt 2 
    // ADCD中断2，位置：向量表索引205，对应PIE组10向量14
    // ADCD模块的第二个中断服务函数指针
    
    PINT  ADCD3_INT;                                  // 10.15 - ADCD Interrupt 3 
    // ADCD中断3，位置：向量表索引206，对应PIE组10向量15
    // ADCD模块的第三个中断服务函数指针
    
    PINT  ADCD4_INT;                                  // 10.16 - ADCD Interrupt 4 
    // ADCD中断4，位置：向量表索引207，对应PIE组10向量16
    // ADCD模块的第四个中断服务函数指针
    
    PINT  PIE81_RESERVED_INT;                         // 11.9 - Reserved 
    // 保留中断81，位置：向量表索引208，对应PIE组11向量9
    
    PINT  PIE82_RESERVED_INT;                         // 11.10 - Reserved 
    // 保留中断82，位置：向量表索引209，对应PIE组11向量10
    
    PINT  PIE83_RESERVED_INT;                         // 11.11 - Reserved 
    // 保留中断83，位置：向量表索引210，对应PIE组11向量11
    
    PINT  PIE84_RESERVED_INT;                         // 11.12 - Reserved 
    // 保留中断84，位置：向量表索引211，对应PIE组11向量12
    
    PINT  PIE85_RESERVED_INT;                         // 11.13 - Reserved 
    // 保留中断85，位置：向量表索引212，对应PIE组11向量13
    
    PINT  PIE86_RESERVED_INT;                         // 11.14 - Reserved 
    // 保留中断86，位置：向量表索引213，对应PIE组11向量14
    
    PINT  PIE87_RESERVED_INT;                         // 11.15 - Reserved 
    // 保留中断87，位置：向量表索引214，对应PIE组11向量15
    
    PINT  PIE88_RESERVED_INT;                         // 11.16 - Reserved 
    // 保留中断88，位置：向量表索引215，对应PIE组11向量16
    
    PINT  EMIF_ERROR_INT;                             // 12.9 - EMIF Error Interrupt 
    // EMIF错误中断，位置：向量表索引216，对应PIE组12向量9
    // EMIF（外部存储器接口）错误中断服务函数指针
    
    PINT  RAM_CORRECTABLE_ERROR_INT;                  // 12.10 - RAM Correctable Error Interrupt 
    // RAM可纠正错误中断，位置：向量表索引217，对应PIE组12向量10
    // RAM可纠正错误中断服务函数指针
    
    PINT  FLASH_CORRECTABLE_ERROR_INT;                // 12.11 - Flash Correctable Error Interrupt 
    // Flash可纠正错误中断，位置：向量表索引218，对应PIE组12向量11
    // Flash可纠正错误中断服务函数指针
    
    PINT  RAM_ACCESS_VIOLATION_INT;                   // 12.12 - RAM Access Violation Interrupt 
    // RAM访问违规中断，位置：向量表索引219，对应PIE组12向量12
    // RAM访问违规中断服务函数指针
    
    PINT  SYS_PLL_SLIP_INT;                           // 12.13 - System PLL Slip Interrupt 
    // 系统PLL滑移中断，位置：向量表索引220，对应PIE组12向量13
    // 系统PLL滑移错误中断服务函数指针
    
    PINT  AUX_PLL_SLIP_INT;                           // 12.14 - Auxiliary PLL Slip Interrupt 
    // 辅助PLL滑移中断，位置：向量表索引221，对应PIE组12向量14
    // 辅助PLL滑移错误中断服务函数指针
    
    PINT  CLA_OVERFLOW_INT;                           // 12.15 - CLA Overflow Interrupt 
    // CLA溢出中断，位置：向量表索引222，对应PIE组12向量15
    // CLA溢出中断服务函数指针
    
    PINT  CLA_UNDERFLOW_INT;                          // 12.16 - CLA Underflow Interrupt 
    // CLA下溢中断，位置：向量表索引223，对应PIE组12向量16
    // CLA下溢中断服务函数指针
};

//---------------------------------------------------------------------------
// PieVect External References & Function Declarations:
// PieVect外部引用和函数声明：

extern volatile struct PIE_VECT_TABLE PieVectTable;
// 声明一个外部的PIE_VECT_TABLE结构体变量PieVectTable
// volatile关键字确保编译器不会优化对此变量的访问
// 这表示PieVectTable是一个内存映射的中断向量表
// 在链接时，这个变量会被分配到特定的内存地址（通常是PIE向量表基地址）

#ifdef __cplusplus
}
#endif                                                /* extern "C" */
// C++兼容性处理结束：
// 如果使用C++编译器编译，结束extern "C"块
// 注释表示这是extern "C"的结束


#endif                                                // end of F2837x_PIEVECT_H definition
// 结束头文件条件编译，#ifndef F2837x_PIE_VECT_H的对应#endif
// 注释表示这是F2837x_PIEVECT_H定义的结束

//===========================================================================
// End of file.
//===========================================================================
// 文件结束标记，表示这是文件的结尾
// 通常用于指示文件内容已完全结束



