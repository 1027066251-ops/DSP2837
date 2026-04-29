//###########################################################################
//
// FILE:   F2837x_DefaultISR.c
//
// TITLE:  F2837x Device Default Interrupt Service Routines
//
//###########################################################################
// $TI Release: F2837xD Support Library v120 $
// $Release Date: Fri Aug 22 15:22:27 CDT 2014 $
//###########################################################################




//为 F2837xD MCU 提供默认中断服务程序，实现未定义中断的安全处理，并便于调试和用户替换。



/* 包含F2837xD器件主头文件，提供所有寄存器定义和基本器件宏定义 */
#include "F2837xD_Device.h"       // F2837x Header File Include File

/* 包含F2837xD示例头文件，提供示例代码相关的宏定义和函数原型 */
#include "F2837xD_Examples.h"     // F2837x Examples Include File

//---------------------------------------------------------------------------
// CPU Timer 1 Interrupt
/* 函数：TIMER1_ISR（CPU定时器1中断服务程序） */
interrupt void TIMER1_ISR(void)
{
    // Insert ISR Code here
    /* 在这里插入中断服务程序代码 */

    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
    /* 以下两行仅用于调试，在此处暂停处理器 */
    /* 插入中断服务程序代码后移除 */
    asm ("      ESTOP0");
    for(;;);
}

// CPU Timer 2 Interrupt
/* 函数：TIMER2_ISR（CPU定时器2中断服务程序） */
interrupt void TIMER2_ISR(void)
{
    // Insert ISR Code here
    /* 在这里插入中断服务程序代码 */

    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
    /* 以下两行仅用于调试，在此处暂停处理器 */
    /* 插入中断服务程序代码后移除 */
    asm ("      ESTOP0");
    for(;;);
}

// Datalogging Interrupt
/* 函数：DATALOG_ISR（数据记录中断服务程序） */
interrupt void DATALOG_ISR(void)
{
    // Insert ISR Code here
    /* 在这里插入中断服务程序代码 */

    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
    /* 以下两行仅用于调试，在此处暂停处理器 */
    /* 插入中断服务程序代码后移除 */
    asm ("      ESTOP0");
    for(;;);
}

// RTOS Interrupt
/* 函数：RTOS_ISR（实时操作系统中断服务程序） */
interrupt void RTOS_ISR(void)
{
    // Insert ISR Code here
    /* 在这里插入中断服务程序代码 */

    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
    /* 以下两行仅用于调试，在此处暂停处理器 */
    /* 插入中断服务程序代码后移除 */
    asm ("      ESTOP0");
    for(;;);
}

// Emulation Interrupt
/* 函数：EMU_ISR（仿真中断服务程序） */
interrupt void EMU_ISR(void)
{
    // Insert ISR Code here
    /* 在这里插入中断服务程序代码 */

    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
    /* 以下两行仅用于调试，在此处暂停处理器 */
    /* 插入中断服务程序代码后移除 */
    asm ("      ESTOP0");
    for(;;);
}

// Non-Maskable Interrupt
/* 函数：NMI_ISR（非屏蔽中断服务程序） */
interrupt void NMI_ISR(void)
{
    // Insert ISR Code here
    /* 在这里插入中断服务程序代码 */

    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
    /* 以下两行仅用于调试，在此处暂停处理器 */
    /* 插入中断服务程序代码后移除 */
    asm ("      ESTOP0");
    for(;;);
}

// Illegal Operation Trap
/* 函数：ILLEGAL_ISR（非法操作陷阱中断服务程序） */
interrupt void ILLEGAL_ISR(void)
{
    // Insert ISR Code here
    /* 在这里插入中断服务程序代码 */

    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
    /* 以下两行仅用于调试，在此处暂停处理器 */
    /* 插入中断服务程序代码后移除 */
    asm ("      ESTOP0");
    for(;;);
}

// User Defined Trap 1
/* 函数：USER1_ISR（用户定义陷阱1中断服务程序） */
interrupt void USER1_ISR(void)
{
    // Insert ISR Code here
    /* 在这里插入中断服务程序代码 */

    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
    /* 以下两行仅用于调试，在此处暂停处理器 */
    /* 插入中断服务程序代码后移除 */
    asm ("      ESTOP0");
    for(;;);
}

// User Defined Trap 2
/* 函数：USER2_ISR（用户定义陷阱2中断服务程序） */
interrupt void USER2_ISR(void)
{
    // Insert ISR Code here
    /* 在这里插入中断服务程序代码 */

    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
    /* 以下两行仅用于调试，在此处暂停处理器 */
    /* 插入中断服务程序代码后移除 */
    asm ("      ESTOP0");
    for(;;);
}

// User Defined Trap 3
/* 函数：USER3_ISR（用户定义陷阱3中断服务程序） */
interrupt void USER3_ISR(void)
{
    // Insert ISR Code here
    /* 在这里插入中断服务程序代码 */

    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
    /* 以下两行仅用于调试，在此处暂停处理器 */
    /* 插入中断服务程序代码后移除 */
    asm ("      ESTOP0");
    for(;;);
}

// User Defined Trap 4
/* 函数：USER4_ISR（用户定义陷阱4中断服务程序） */
interrupt void USER4_ISR(void)
{
    // Insert ISR Code here
    /* 在这里插入中断服务程序代码 */

    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
    /* 以下两行仅用于调试，在此处暂停处理器 */
    /* 插入中断服务程序代码后移除 */
    asm ("      ESTOP0");
    for(;;);
}

// User Defined Trap 5
/* 函数：USER5_ISR（用户定义陷阱5中断服务程序） */
interrupt void USER5_ISR(void)
{
    // Insert ISR Code here
    /* 在这里插入中断服务程序代码 */

    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
    /* 以下两行仅用于调试，在此处暂停处理器 */
    /* 插入中断服务程序代码后移除 */
    asm ("      ESTOP0");
    for(;;);
}

// User Defined Trap 6
/* 函数：USER6_ISR（用户定义陷阱6中断服务程序） */
interrupt void USER6_ISR(void)
{
    // Insert ISR Code here
    /* 在这里插入中断服务程序代码 */

    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
    /* 以下两行仅用于调试，在此处暂停处理器 */
    /* 插入中断服务程序代码后移除 */
    asm ("      ESTOP0");
    for(;;);
}

// User Defined Trap 7
/* 函数：USER7_ISR（用户定义陷阱7中断服务程序） */
interrupt void USER7_ISR(void)
{
    // Insert ISR Code here
    /* 在这里插入中断服务程序代码 */

    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
    /* 以下两行仅用于调试，在此处暂停处理器 */
    /* 插入中断服务程序代码后移除 */
    asm ("      ESTOP0");
    for(;;);
}

// User Defined Trap 8
/* 函数：USER8_ISR（用户定义陷阱8中断服务程序） */
interrupt void USER8_ISR(void)
{
    // Insert ISR Code here
    /* 在这里插入中断服务程序代码 */

    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
    /* 以下两行仅用于调试，在此处暂停处理器 */
    /* 插入中断服务程序代码后移除 */
    asm ("      ESTOP0");
    for(;;);
}

// User Defined Trap 9
/* 函数：USER9_ISR（用户定义陷阱9中断服务程序） */
interrupt void USER9_ISR(void)
{
    // Insert ISR Code here
    /* 在这里插入中断服务程序代码 */

    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
    /* 以下两行仅用于调试，在此处暂停处理器 */
    /* 插入中断服务程序代码后移除 */
    asm ("      ESTOP0");
    for(;;);
}

// User Defined Trap 10
/* 函数：USER10_ISR（用户定义陷阱10中断服务程序） */
interrupt void USER10_ISR(void)
{
    // Insert ISR Code here
    /* 在这里插入中断服务程序代码 */

    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
    /* 以下两行仅用于调试，在此处暂停处理器 */
    /* 插入中断服务程序代码后移除 */
    asm ("      ESTOP0");
    for(;;);
}

// User Defined Trap 11
/* 函数：USER11_ISR（用户定义陷阱11中断服务程序） */
interrupt void USER11_ISR(void)
{
    // Insert ISR Code here
    /* 在这里插入中断服务程序代码 */

    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
    /* 以下两行仅用于调试，在此处暂停处理器 */
    /* 插入中断服务程序代码后移除 */
    asm ("      ESTOP0");
    for(;;);
}

// User Defined Trap 12
/* 函数：USER12_ISR（用户定义陷阱12中断服务程序） */
interrupt void USER12_ISR(void)
{
    // Insert ISR Code here
    /* 在这里插入中断服务程序代码 */

    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
    /* 以下两行仅用于调试，在此处暂停处理器 */
    /* 插入中断服务程序代码后移除 */
    asm ("      ESTOP0");
    for(;;);
}

// 1.1 - ADCA Interrupt 1
/* 函数：ADCA1_ISR（ADC A中断1服务程序） */
interrupt void ADCA1_ISR(void)
{
    // Insert ISR Code here
    /* 在这里插入中断服务程序代码 */

    // To receive more interrupts from this PIE group,
    // acknowledge this interrupt.
    /* 要从此PIE组接收更多中断，确认此中断 */
    // PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;

    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
    /* 以下两行仅用于调试，在此处暂停处理器 */
    /* 插入中断服务程序代码后移除 */
    asm ("      ESTOP0");
    for(;;);
}

// 1.2 - ADCB Interrupt 1
/* 函数：ADCB1_ISR（ADC B中断1服务程序） */
interrupt void ADCB1_ISR(void)
{
    // Insert ISR Code here
    /* 在这里插入中断服务程序代码 */

    // To receive more interrupts from this PIE group,
    // acknowledge this interrupt.
    /* 要从此PIE组接收更多中断，确认此中断 */
    // PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;

    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
    /* 以下两行仅用于调试，在此处暂停处理器 */
    /* 插入中断服务程序代码后移除 */
    asm ("      ESTOP0");
    for(;;);
}

// 1.3 - ADCC Interrupt 1
/* 函数：ADCC1_ISR（ADC C中断1服务程序） */
interrupt void ADCC1_ISR(void)
{
    // Insert ISR Code here
    /* 在这里插入中断服务程序代码 */

    // To receive more interrupts from this PIE group,
    // acknowledge this interrupt.
    /* 要从此PIE组接收更多中断，确认此中断 */
    // PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;

    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
    /* 以下两行仅用于调试，在此处暂停处理器 */
    /* 插入中断服务程序代码后移除 */
    asm ("      ESTOP0");
    for(;;);
}

// 1.4 - XINT1 Interrupt
/* 函数：XINT1_ISR（外部中断1服务程序） */
interrupt void XINT1_ISR(void)
{
    // Insert ISR Code here
    /* 在这里插入中断服务程序代码 */

    // To receive more interrupts from this PIE group,
    // acknowledge this interrupt.
    /* 要从此PIE组接收更多中断，确认此中断 */
    // PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;

    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
    /* 以下两行仅用于调试，在此处暂停处理器 */
    /* 插入中断服务程序代码后移除 */
    asm ("      ESTOP0");
    for(;;);
}

// 1.5 - XINT2 Interrupt
/* 函数：XINT2_ISR（外部中断2服务程序） */
interrupt void XINT2_ISR(void)
{
    // Insert ISR Code here
    /* 在这里插入中断服务程序代码 */

    // To receive more interrupts from this PIE group,
    // acknowledge this interrupt.
    /* 要从此PIE组接收更多中断，确认此中断 */
    // PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;

    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
    /* 以下两行仅用于调试，在此处暂停处理器 */
    /* 插入中断服务程序代码后移除 */
    asm ("      ESTOP0");
    for(;;);
}

// 1.6 - ADCD Interrupt 1
/* 函数：ADCD1_ISR（ADC D中断1服务程序） */
interrupt void ADCD1_ISR(void)
{
    // Insert ISR Code here
    /* 在这里插入中断服务程序代码 */

    // To receive more interrupts from this PIE group,
    // acknowledge this interrupt.
    /* 要从此PIE组接收更多中断，确认此中断 */
    // PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;

    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
    /* 以下两行仅用于调试，在此处暂停处理器 */
    /* 插入中断服务程序代码后移除 */
    asm ("      ESTOP0");
    for(;;);
}

// 1.7 - Timer 0 Interrupt
/* 函数：TIMER0_ISR（定时器0中断服务程序） */
interrupt void TIMER0_ISR(void)
{
    // Insert ISR Code here
    /* 在这里插入中断服务程序代码 */

    // To receive more interrupts from this PIE group,
    // acknowledge this interrupt.
    /* 要从此PIE组接收更多中断，确认此中断 */
    // PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;

    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
    /* 以下两行仅用于调试，在此处暂停处理器 */
    /* 插入中断服务程序代码后移除 */
    asm ("      ESTOP0");
    for(;;);
}

// 1.8 - Standby and Halt Wakeup Interrupt
/* 函数：WAKE_ISR（待机和暂停唤醒中断服务程序） */
interrupt void WAKE_ISR(void)
{
    // Insert ISR Code here
    /* 在这里插入中断服务程序代码 */

    // To receive more interrupts from this PIE group,
    // acknowledge this interrupt.
    /* 要从此PIE组接收更多中断，确认此中断 */
    // PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;

    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
    /* 以下两行仅用于调试，在此处暂停处理器 */
    /* 插入中断服务程序代码后移除 */
    asm ("      ESTOP0");
    for(;;);
}

// 2.1 - ePWM1 Trip Zone Interrupt
/* 函数：EPWM1_TZ_ISR（增强型PWM1跳闸区中断服务程序） */
interrupt void EPWM1_TZ_ISR(void)
{
    // Insert ISR Code here
    /* 在这里插入中断服务程序代码 */

    // To receive more interrupts from this PIE group,
    // acknowledge this interrupt.
    /* 要从此PIE组接收更多中断，确认此中断 */
    // PieCtrlRegs.PIEACK.all = PIEACK_GROUP2;

    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
    /* 以下两行仅用于调试，在此处暂停处理器 */
    /* 插入中断服务程序代码后移除 */
    asm ("      ESTOP0");
    for(;;);
}

// 2.2 - ePWM2 Trip Zone Interrupt
/* 函数：EPWM2_TZ_ISR（增强型PWM2跳闸区中断服务程序） */
interrupt void EPWM2_TZ_ISR(void)
{
    // Insert ISR Code here
    /* 在这里插入中断服务程序代码 */

    // To receive more interrupts from this PIE group,
    // acknowledge this interrupt.
    /* 要从此PIE组接收更多中断，确认此中断 */
    // PieCtrlRegs.PIEACK.all = PIEACK_GROUP2;

    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
    /* 以下两行仅用于调试，在此处暂停处理器 */
    /* 插入中断服务程序代码后移除 */
    asm ("      ESTOP0");
    for(;;);
}

// 2.3 - ePWM3 Trip Zone Interrupt
/* 函数：EPWM3_TZ_ISR（增强型PWM3跳闸区中断服务程序） */
interrupt void EPWM3_TZ_ISR(void)
{
    // Insert ISR Code here
    /* 在这里插入中断服务程序代码 */

    // To receive more interrupts from this PIE group,
    // acknowledge this interrupt.
    /* 要从此PIE组接收更多中断，确认此中断 */
    // PieCtrlRegs.PIEACK.all = PIEACK_GROUP2;

    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
    /* 以下两行仅用于调试，在此处暂停处理器 */
    /* 插入中断服务程序代码后移除 */
    asm ("      ESTOP0");
    for(;;);
}

// 2.4 - ePWM4 Trip Zone Interrupt
/* 函数：EPWM4_TZ_ISR（增强型PWM4跳闸区中断服务程序） */
interrupt void EPWM4_TZ_ISR(void)
{
    // Insert ISR Code here
    /* 在这里插入中断服务程序代码 */

    // To receive more interrupts from this PIE group,
    // acknowledge this interrupt.
    /* 要从此PIE组接收更多中断，确认此中断 */
    // PieCtrlRegs.PIEACK.all = PIEACK_GROUP2;

    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
    /* 以下两行仅用于调试，在此处暂停处理器 */
    /* 插入中断服务程序代码后移除 */
    asm ("      ESTOP0");
    for(;;);
}

// 2.5 - ePWM5 Trip Zone Interrupt
/* 函数：EPWM5_TZ_ISR（增强型PWM5跳闸区中断服务程序） */
interrupt void EPWM5_TZ_ISR(void)
{
    // Insert ISR Code here
    /* 在这里插入中断服务程序代码 */

    // To receive more interrupts from this PIE group,
    // acknowledge this interrupt.
    /* 要从此PIE组接收更多中断，确认此中断 */
    // PieCtrlRegs.PIEACK.all = PIEACK_GROUP2;

    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
    /* 以下两行仅用于调试，在此处暂停处理器 */
    /* 插入中断服务程序代码后移除 */
    asm ("      ESTOP0");
    for(;;);
}

// 2.6 - ePWM6 Trip Zone Interrupt
/* 函数：EPWM6_TZ_ISR（增强型PWM6跳闸区中断服务程序） */
interrupt void EPWM6_TZ_ISR(void)
{
    // Insert ISR Code here
    /* 在这里插入中断服务程序代码 */

    // To receive more interrupts from this PIE group,
    // acknowledge this interrupt.
    /* 要从此PIE组接收更多中断，确认此中断 */
    // PieCtrlRegs.PIEACK.all = PIEACK_GROUP2;

    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
    /* 以下两行仅用于调试，在此处暂停处理器 */
    /* 插入中断服务程序代码后移除 */
    asm ("      ESTOP0");
    for(;;);
}

// 2.7 - ePWM7 Trip Zone Interrupt
/* 函数：EPWM7_TZ_ISR（增强型PWM7跳闸区中断服务程序） */
interrupt void EPWM7_TZ_ISR(void)
{
    // Insert ISR Code here
    /* 在这里插入中断服务程序代码 */

    // To receive more interrupts from this PIE group,
    // acknowledge this interrupt.
    /* 要从此PIE组接收更多中断，确认此中断 */
    // PieCtrlRegs.PIEACK.all = PIEACK_GROUP2;

    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
    /* 以下两行仅用于调试，在此处暂停处理器 */
    /* 插入中断服务程序代码后移除 */
    asm ("      ESTOP0");
    for(;;);
}

// 2.8 - ePWM8 Trip Zone Interrupt
/* 函数：EPWM8_TZ_ISR（增强型PWM8跳闸区中断服务程序） */
interrupt void EPWM8_TZ_ISR(void)
{
    // Insert ISR Code here
    /* 在这里插入中断服务程序代码 */

    // To receive more interrupts from this PIE group,
    // acknowledge this interrupt.
    /* 要从此PIE组接收更多中断，确认此中断 */
    // PieCtrlRegs.PIEACK.all = PIEACK_GROUP2;

    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
    /* 以下两行仅用于调试，在此处暂停处理器 */
    /* 插入中断服务程序代码后移除 */
    asm ("      ESTOP0");
    for(;;);
}

// 3.1 - ePWM1 Interrupt
/* 函数：EPWM1_ISR（增强型PWM1中断服务程序） */
interrupt void EPWM1_ISR(void)
{
    // Insert ISR Code here
    /* 在这里插入中断服务程序代码 */

    // To receive more interrupts from this PIE group,
    // acknowledge this interrupt.
    /* 要从此PIE组接收更多中断，确认此中断 */
    // PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;

    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
    /* 以下两行仅用于调试，在此处暂停处理器 */
    /* 插入中断服务程序代码后移除 */
    asm ("      ESTOP0");
    for(;;);
}

// 3.2 - ePWM2 Interrupt
/* 函数：EPWM2_ISR（增强型PWM2中断服务程序） */
interrupt void EPWM2_ISR(void)
{
    // Insert ISR Code here
    /* 在这里插入中断服务程序代码 */

    // To receive more interrupts from this PIE group,
    // acknowledge this interrupt.
    /* 要从此PIE组接收更多中断，确认此中断 */
    // PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;

    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
    /* 以下两行仅用于调试，在此处暂停处理器 */
    /* 插入中断服务程序代码后移除 */
    asm ("      ESTOP0");
    for(;;);
}

// 3.3 - ePWM3 Interrupt
/* 函数：EPWM3_ISR（增强型PWM3中断服务程序） */
interrupt void EPWM3_ISR(void)
{
    // Insert ISR Code here
    /* 在这里插入中断服务程序代码 */

    // To receive more interrupts from this PIE group,
    // acknowledge this interrupt.
    /* 要从此PIE组接收更多中断，确认此中断 */
    // PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;

    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
    /* 以下两行仅用于调试，在此处暂停处理器 */
    /* 插入中断服务程序代码后移除 */
    asm ("      ESTOP0");
    for(;;);
}

// 3.4 - ePWM4 Interrupt
/* 函数：EPWM4_ISR（增强型PWM4中断服务程序） */
interrupt void EPWM4_ISR(void)
{
    // Insert ISR Code here
    /* 在这里插入中断服务程序代码 */

    // To receive more interrupts from this PIE group,
    // acknowledge this interrupt.
    /* 要从此PIE组接收更多中断，确认此中断 */
    // PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;

    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
    /* 以下两行仅用于调试，在此处暂停处理器 */
    /* 插入中断服务程序代码后移除 */
    asm ("      ESTOP0");
    for(;;);
}

// 3.5 - ePWM5 Interrupt
/* 函数：EPWM5_ISR（增强型PWM5中断服务程序） */
interrupt void EPWM5_ISR(void)
{
    // Insert ISR Code here
    /* 在这里插入中断服务程序代码 */

    // To receive more interrupts from this PIE group,
    // acknowledge this interrupt.
    /* 要从此PIE组接收更多中断，确认此中断 */
    // PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;

    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
    /* 以下两行仅用于调试，在此处暂停处理器 */
    /* 插入中断服务程序代码后移除 */
    asm ("      ESTOP0");
    for(;;);
}

// 3.6 - ePWM6 Interrupt
/* 函数：EPWM6_ISR（增强型PWM6中断服务程序） */
interrupt void EPWM6_ISR(void)
{
    // Insert ISR Code here
    /* 在这里插入中断服务程序代码 */

    // To receive more interrupts from this PIE group,
    // acknowledge this interrupt.
    /* 要从此PIE组接收更多中断，确认此中断 */
    // PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;

    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
    /* 以下两行仅用于调试，在此处暂停处理器 */
    /* 插入中断服务程序代码后移除 */
    asm ("      ESTOP0");
    for(;;);
}

// 3.7 - ePWM7 Interrupt
/* 函数：EPWM7_ISR（增强型PWM7中断服务程序） */
interrupt void EPWM7_ISR(void)
{
    // Insert ISR Code here
    /* 在这里插入中断服务程序代码 */

    // To receive more interrupts from this PIE group,
    // acknowledge this interrupt.
    /* 要从此PIE组接收更多中断，确认此中断 */
    // PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;

    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
    /* 以下两行仅用于调试，在此处暂停处理器 */
    /* 插入中断服务程序代码后移除 */
    asm ("      ESTOP0");
    for(;;);
}

// 3.8 - ePWM8 Interrupt
/* 函数：EPWM8_ISR（增强型PWM8中断服务程序） */
interrupt void EPWM8_ISR(void)
{
    // Insert ISR Code here
    /* 在这里插入中断服务程序代码 */

    // To receive more interrupts from this PIE group,
    // acknowledge this interrupt.
    /* 要从此PIE组接收更多中断，确认此中断 */
    // PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;

    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
    /* 以下两行仅用于调试，在此处暂停处理器 */
    /* 插入中断服务程序代码后移除 */
    asm ("      ESTOP0");
    for(;;);
}

// 4.1 - eCAP1 Interrupt
/* 函数：ECAP1_ISR（增强型捕获1中断服务程序） */
interrupt void ECAP1_ISR(void)
{
    // Insert ISR Code here
    /* 在这里插入中断服务程序代码 */

    // To receive more interrupts from this PIE group,
    // acknowledge this interrupt.
    /* 要从此PIE组接收更多中断，确认此中断 */
    // PieCtrlRegs.PIEACK.all = PIEACK_GROUP4;

    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
    /* 以下两行仅用于调试，在此处暂停处理器 */
    /* 插入中断服务程序代码后移除 */
    asm ("      ESTOP0");
    for(;;);
}

// 4.2 - eCAP2 Interrupt
/* 函数：ECAP2_ISR（增强型捕获2中断服务程序） */
interrupt void ECAP2_ISR(void)
{
    // Insert ISR Code here
    /* 在这里插入中断服务程序代码 */

    // To receive more interrupts from this PIE group,
    // acknowledge this interrupt.
    /* 要从此PIE组接收更多中断，确认此中断 */
    // PieCtrlRegs.PIEACK.all = PIEACK_GROUP4;

    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
    /* 以下两行仅用于调试，在此处暂停处理器 */
    /* 插入中断服务程序代码后移除 */
    asm ("      ESTOP0");
    for(;;);
}

// 4.3 - eCAP3 Interrupt
/* 函数：ECAP3_ISR（增强型捕获3中断服务程序） */
interrupt void ECAP3_ISR(void)
{
    // Insert ISR Code here
    /* 在这里插入中断服务程序代码 */

    // To receive more interrupts from this PIE group,
    // acknowledge this interrupt.
    /* 要从此PIE组接收更多中断，确认此中断 */
    // PieCtrlRegs.PIEACK.all = PIEACK_GROUP4;

    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
    /* 以下两行仅用于调试，在此处暂停处理器 */
    /* 插入中断服务程序代码后移除 */
    asm ("      ESTOP0");
    for(;;);
}

// 4.4 - eCAP4 Interrupt
/* 函数：ECAP4_ISR（增强型捕获4中断服务程序） */
interrupt void ECAP4_ISR(void)
{
    // Insert ISR Code here
    /* 在这里插入中断服务程序代码 */

    // To receive more interrupts from this PIE group,
    // acknowledge this interrupt.
    /* 要从此PIE组接收更多中断，确认此中断 */
    // PieCtrlRegs.PIEACK.all = PIEACK_GROUP4;

    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
    /* 以下两行仅用于调试，在此处暂停处理器 */
    /* 插入中断服务程序代码后移除 */
    asm ("      ESTOP0");
    for(;;);
}

// 4.5 - eCAP5 Interrupt
/* 函数：ECAP5_ISR（增强型捕获5中断服务程序） */
interrupt void ECAP5_ISR(void)
{
    // Insert ISR Code here
    /* 在这里插入中断服务程序代码 */

    // To receive more interrupts from this PIE group,
    // acknowledge this interrupt.
    /* 要从此PIE组接收更多中断，确认此中断 */
    // PieCtrlRegs.PIEACK.all = PIEACK_GROUP4;

    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
    /* 以下两行仅用于调试，在此处暂停处理器 */
    /* 插入中断服务程序代码后移除 */
    asm ("      ESTOP0");
    for(;;);
}

// 4.6 - eCAP6 Interrupt
/* 函数：ECAP6_ISR（增强型捕获6中断服务程序） */
interrupt void ECAP6_ISR(void)
{
    // Insert ISR Code here
    /* 在这里插入中断服务程序代码 */

    // To receive more interrupts from this PIE group,
    // acknowledge this interrupt.
    /* 要从此PIE组接收更多中断，确认此中断 */
    // PieCtrlRegs.PIEACK.all = PIEACK_GROUP4;

    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
    /* 以下两行仅用于调试，在此处暂停处理器 */
    /* 插入中断服务程序代码后移除 */
    asm ("      ESTOP0");
    for(;;);
}

// 5.1 - eQEP1 Interrupt
/* 函数：EQEP1_ISR（增强型正交编码器1中断服务程序） */
interrupt void EQEP1_ISR(void)
{
    // Insert ISR Code here
    /* 在这里插入中断服务程序代码 */

    // To receive more interrupts from this PIE group,
    // acknowledge this interrupt.
    /* 要从此PIE组接收更多中断，确认此中断 */
    // PieCtrlRegs.PIEACK.all = PIEACK_GROUP5;

    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
    /* 以下两行仅用于调试，在此处暂停处理器 */
    /* 插入中断服务程序代码后移除 */
    asm ("      ESTOP0");
    for(;;);
}

// 5.2 - eQEP2 Interrupt
/* 函数：EQEP2_ISR（增强型正交编码器2中断服务程序） */
interrupt void EQEP2_ISR(void)
{
    // Insert ISR Code here
    /* 在这里插入中断服务程序代码 */

    // To receive more interrupts from this PIE group,
    // acknowledge this interrupt.
    /* 要从此PIE组接收更多中断，确认此中断 */
    // PieCtrlRegs.PIEACK.all = PIEACK_GROUP5;

    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
    /* 以下两行仅用于调试，在此处暂停处理器 */
    /* 插入中断服务程序代码后移除 */
    asm ("      ESTOP0");
    for(;;);
}

// 5.3 - eQEP3 Interrupt
/* 函数：EQEP3_ISR（增强型正交编码器3中断服务程序） */
interrupt void EQEP3_ISR(void)
{
    // Insert ISR Code here
    /* 在这里插入中断服务程序代码 */

    // To receive more interrupts from this PIE group,
    // acknowledge this interrupt.
    /* 要从此PIE组接收更多中断，确认此中断 */
    // PieCtrlRegs.PIEACK.all = PIEACK_GROUP5;

    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
    /* 以下两行仅用于调试，在此处暂停处理器 */
    /* 插入中断服务程序代码后移除 */
    asm ("      ESTOP0");
    for(;;);
}

// 6.1 - SPIA Receive Interrupt
/* 函数：SPIA_RX_ISR（SPI A接收中断服务程序） */
interrupt void SPIA_RX_ISR(void)
{
    // Insert ISR Code here
    /* 在这里插入中断服务程序代码 */

    // To receive more interrupts from this PIE group,
    // acknowledge this interrupt.
    /* 要从此PIE组接收更多中断，确认此中断 */
    // PieCtrlRegs.PIEACK.all = PIEACK_GROUP6;

    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
    /* 以下两行仅用于调试，在此处暂停处理器 */
    /* 插入中断服务程序代码后移除 */
    asm ("      ESTOP0");
    for(;;);
}

// 6.2 - SPIA Transmit Interrupt
/* 函数：SPIA_TX_ISR（SPI A发送中断服务程序） */
interrupt void SPIA_TX_ISR(void)
{
    // Insert ISR Code here
    /* 在这里插入中断服务程序代码 */

    // To receive more interrupts from this PIE group,
    // acknowledge this interrupt.
    /* 要从此PIE组接收更多中断，确认此中断 */
    // PieCtrlRegs.PIEACK.all = PIEACK_GROUP6;

    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
    /* 以下两行仅用于调试，在此处暂停处理器 */
    /* 插入中断服务程序代码后移除 */
    asm ("      ESTOP0");
    for(;;);
}

// 6.3 - SPIB Receive Interrupt
/* 函数：SPIB_RX_ISR（SPI B接收中断服务程序） */
interrupt void SPIB_RX_ISR(void)
{
    // Insert ISR Code here
    /* 在这里插入中断服务程序代码 */

    // To receive more interrupts from this PIE group,
    // acknowledge this interrupt.
    /* 要从此PIE组接收更多中断，确认此中断 */
    // PieCtrlRegs.PIEACK.all = PIEACK_GROUP6;

    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
    /* 以下两行仅用于调试，在此处暂停处理器 */
    /* 插入中断服务程序代码后移除 */
    asm ("      ESTOP0");
    for(;;);
}

// 6.4 - SPIB Transmit Interrupt
/* 函数：SPIB_TX_ISR（SPI B发送中断服务程序） */
interrupt void SPIB_TX_ISR(void)
{
    // Insert ISR Code here
    /* 在这里插入中断服务程序代码 */

    // To receive more interrupts from this PIE group,
    // acknowledge this interrupt.
    /* 要从此PIE组接收更多中断，确认此中断 */
    // PieCtrlRegs.PIEACK.all = PIEACK_GROUP6;

    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
    /* 以下两行仅用于调试，在此处暂停处理器 */
    /* 插入中断服务程序代码后移除 */
    asm ("      ESTOP0");
    for(;;);
}

// 6.5 - McBSPA Receive Interrupt
/* 函数：MCBSPA_RX_ISR（多通道缓冲串口A接收中断服务程序） */
interrupt void MCBSPA_RX_ISR(void)
{
    // Insert ISR Code here
    /* 在这里插入中断服务程序代码 */

    // To receive more interrupts from this PIE group,
    // acknowledge this interrupt.
    /* 要从此PIE组接收更多中断，确认此中断 */
    // PieCtrlRegs.PIEACK.all = PIEACK_GROUP6;

    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
    /* 以下两行仅用于调试，在此处暂停处理器 */
    /* 插入中断服务程序代码后移除 */
    asm ("      ESTOP0");
    for(;;);
}

// 6.6 - McBSPA Transmit Interrupt
/* 函数：MCBSPA_TX_ISR（多通道缓冲串口A发送中断服务程序） */
interrupt void MCBSPA_TX_ISR(void)
{
    // Insert ISR Code here
    /* 在这里插入中断服务程序代码 */

    // To receive more interrupts from this PIE group,
    // acknowledge this interrupt.
    /* 要从此PIE组接收更多中断，确认此中断 */
    // PieCtrlRegs.PIEACK.all = PIEACK_GROUP6;

    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
    /* 以下两行仅用于调试，在此处暂停处理器 */
    /* 插入中断服务程序代码后移除 */
    asm ("      ESTOP0");
    for(;;);
}

// 6.7 - McBSPB Receive Interrupt
/* 函数：MCBSPB_RX_ISR（多通道缓冲串口B接收中断服务程序） */
interrupt void MCBSPB_RX_ISR(void)
{
    // Insert ISR Code here
    /* 在这里插入中断服务程序代码 */

    // To receive more interrupts from this PIE group,
    // acknowledge this interrupt.
    /* 要从此PIE组接收更多中断，确认此中断 */
    // PieCtrlRegs.PIEACK.all = PIEACK_GROUP6;

    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
    /* 以下两行仅用于调试，在此处暂停处理器 */
    /* 插入中断服务程序代码后移除 */
    asm ("      ESTOP0");
    for(;;);
}

// 6.8 - McBSPB Transmit Interrupt
/* 函数：MCBSPB_TX_ISR（多通道缓冲串口B发送中断服务程序） */
interrupt void MCBSPB_TX_ISR(void)
{
    // Insert ISR Code here
    /* 在这里插入中断服务程序代码 */

    // To receive more interrupts from this PIE group,
    // acknowledge this interrupt.
    /* 要从此PIE组接收更多中断，确认此中断 */
    // PieCtrlRegs.PIEACK.all = PIEACK_GROUP6;

    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
    /* 以下两行仅用于调试，在此处暂停处理器 */
    /* 插入中断服务程序代码后移除 */
    asm ("      ESTOP0");
    for(;;);
}

// 7.1 - DMA Channel 1 Interrupt
/* 函数：DMA_CH1_ISR（DMA通道1中断服务程序） */
interrupt void DMA_CH1_ISR(void)
{
    // Insert ISR Code here
    /* 在这里插入中断服务程序代码 */

    // To receive more interrupts from this PIE group,
    // acknowledge this interrupt.
    /* 要从此PIE组接收更多中断，确认此中断 */
    // PieCtrlRegs.PIEACK.all = PIEACK_GROUP7;

    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
    /* 以下两行仅用于调试，在此处暂停处理器 */
    /* 插入中断服务程序代码后移除 */
    asm ("      ESTOP0");
    for(;;);
}

// 7.2 - DMA Channel 2 Interrupt
/* 函数：DMA_CH2_ISR（DMA通道2中断服务程序） */
interrupt void DMA_CH2_ISR(void)
{
    // Insert ISR Code here
    /* 在这里插入中断服务程序代码 */

    // To receive more interrupts from this PIE group,
    // acknowledge this interrupt.
    /* 要从此PIE组接收更多中断，确认此中断 */
    // PieCtrlRegs.PIEACK.all = PIEACK_GROUP7;

    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
    /* 以下两行仅用于调试，在此处暂停处理器 */
    /* 插入中断服务程序代码后移除 */
    asm ("      ESTOP0");
    for(;;);
}

// 7.3 - DMA Channel 3 Interrupt
/* 函数：DMA_CH3_ISR（DMA通道3中断服务程序） */
interrupt void DMA_CH3_ISR(void)
{
    // Insert ISR Code here
    /* 在这里插入中断服务程序代码 */

    // To receive more interrupts from this PIE group,
    // acknowledge this interrupt.
    /* 要从此PIE组接收更多中断，确认此中断 */
    // PieCtrlRegs.PIEACK.all = PIEACK_GROUP7;

    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
    /* 以下两行仅用于调试，在此处暂停处理器 */
    /* 插入中断服务程序代码后移除 */
    asm ("      ESTOP0");
    for(;;);
}

// 7.4 - DMA Channel 4 Interrupt
/* 函数：DMA_CH4_ISR（DMA通道4中断服务程序） */
interrupt void DMA_CH4_ISR(void)
{
    // Insert ISR Code here
    /* 在这里插入中断服务程序代码 */

    // To receive more interrupts from this PIE group,
    // acknowledge this interrupt.
    /* 要从此PIE组接收更多中断，确认此中断 */
    // PieCtrlRegs.PIEACK.all = PIEACK_GROUP7;

    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
    /* 以下两行仅用于调试，在此处暂停处理器 */
    /* 插入中断服务程序代码后移除 */
    asm ("      ESTOP0");
    for(;;);
}

// 7.5 - DMA Channel 5 Interrupt
/* 函数：DMA_CH5_ISR（DMA通道5中断服务程序） */
interrupt void DMA_CH5_ISR(void)
{
    // Insert ISR Code here
    /* 在这里插入中断服务程序代码 */

    // To receive more interrupts from this PIE group,
    // acknowledge this interrupt.
    /* 要从此PIE组接收更多中断，确认此中断 */
    // PieCtrlRegs.PIEACK.all = PIEACK_GROUP7;

    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
    /* 以下两行仅用于调试，在此处暂停处理器 */
    /* 插入中断服务程序代码后移除 */
    asm ("      ESTOP0");
    for(;;);
}

// 7.6 - DMA Channel 6 Interrupt
/* 函数：DMA_CH6_ISR（DMA通道6中断服务程序） */
interrupt void DMA_CH6_ISR(void)
{
    // Insert ISR Code here
    /* 在这里插入中断服务程序代码 */

    // To receive more interrupts from this PIE group,
    // acknowledge this interrupt.
    /* 要从此PIE组接收更多中断，确认此中断 */
    // PieCtrlRegs.PIEACK.all = PIEACK_GROUP7;

    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
    /* 以下两行仅用于调试，在此处暂停处理器 */
    /* 插入中断服务程序代码后移除 */
    asm ("      ESTOP0");
    for(;;);
}

// 8.1 - I2CA Interrupt 1
/* 函数：I2CA_ISR（I2C A中断1服务程序） */
interrupt void I2CA_ISR(void)
{
    // Insert ISR Code here
    /* 在这里插入中断服务程序代码 */

    // To receive more interrupts from this PIE group,
    // acknowledge this interrupt.
    /* 要从此PIE组接收更多中断，确认此中断 */
    // PieCtrlRegs.PIEACK.all = PIEACK_GROUP8;

    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
    /* 以下两行仅用于调试，在此处暂停处理器 */
    /* 插入中断服务程序代码后移除 */
    asm ("      ESTOP0");
    for(;;);
}

// 8.2 - I2CA Interrupt 2
/* 函数：I2CA_FIFO_ISR（I2C A FIFO中断服务程序） */
interrupt void I2CA_FIFO_ISR(void)
{
    // Insert ISR Code here
    /* 在这里插入中断服务程序代码 */

    // To receive more interrupts from this PIE group,
    // acknowledge this interrupt.
    /* 要从此PIE组接收更多中断，确认此中断 */
    // PieCtrlRegs.PIEACK.all = PIEACK_GROUP8;

    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
    /* 以下两行仅用于调试，在此处暂停处理器 */
    /* 插入中断服务程序代码后移除 */
    asm ("      ESTOP0");
    for(;;);
}

// 8.3 - I2CB Interrupt 1
/* 函数：I2CB_ISR（I2C B中断1服务程序） */
interrupt void I2CB_ISR(void)
{
    // Insert ISR Code here
    /* 在这里插入中断服务程序代码 */

    // To receive more interrupts from this PIE group,
    // acknowledge this interrupt.
    /* 要从此PIE组接收更多中断，确认此中断 */
    // PieCtrlRegs.PIEACK.all = PIEACK_GROUP8;

    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
    /* 以下两行仅用于调试，在此处暂停处理器 */
    /* 插入中断服务程序代码后移除 */
    asm ("      ESTOP0");
    for(;;);
}

// 8.4 - I2CB Interrupt 2
/* 函数：I2CB_FIFO_ISR（I2C B FIFO中断服务程序） */
interrupt void I2CB_FIFO_ISR(void)
{
    // Insert ISR Code here
    /* 在这里插入中断服务程序代码 */

    // To receive more interrupts from this PIE group,
    // acknowledge this interrupt.
    /* 要从此PIE组接收更多中断，确认此中断 */
    // PieCtrlRegs.PIEACK.all = PIEACK_GROUP8;

    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
    /* 以下两行仅用于调试，在此处暂停处理器 */
    /* 插入中断服务程序代码后移除 */
    asm ("      ESTOP0");
    for(;;);
}

// 8.5 - SCIC Receive Interrupt
/* 函数：SCIC_RX_ISR（SCI C接收中断服务程序） */
interrupt void SCIC_RX_ISR(void)
{
    // Insert ISR Code here
    /* 在这里插入中断服务程序代码 */

    // To receive more interrupts from this PIE group,
    // acknowledge this interrupt.
    /* 要从此PIE组接收更多中断，确认此中断 */
    // PieCtrlRegs.PIEACK.all = PIEACK_GROUP8;

    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
    /* 以下两行仅用于调试，在此处暂停处理器 */
    /* 插入中断服务程序代码后移除 */
    asm ("      ESTOP0");
    for(;;);
}

// 8.6 - SCIC Transmit Interrupt
/* 函数：SCIC_TX_ISR（SCI C发送中断服务程序） */
interrupt void SCIC_TX_ISR(void)
{
    // Insert ISR Code here
    /* 在这里插入中断服务程序代码 */

    // To receive more interrupts from this PIE group,
    // acknowledge this interrupt.
    /* 要从此PIE组接收更多中断，确认此中断 */
    // PieCtrlRegs.PIEACK.all = PIEACK_GROUP8;

    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
    /* 以下两行仅用于调试，在此处暂停处理器 */
    /* 插入中断服务程序代码后移除 */
    asm ("      ESTOP0");
    for(;;);
}

// 8.7 - SCID Receive Interrupt
/* 函数：SCID_RX_ISR（SCI D接收中断服务程序） */
interrupt void SCID_RX_ISR(void)
{
    // Insert ISR Code here
    /* 在这里插入中断服务程序代码 */

    // To receive more interrupts from this PIE group,
    // acknowledge this interrupt.
    /* 要从此PIE组接收更多中断，确认此中断 */
    // PieCtrlRegs.PIEACK.all = PIEACK_GROUP8;

    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
    /* 以下两行仅用于调试，在此处暂停处理器 */
    /* 插入中断服务程序代码后移除 */
    asm ("      ESTOP0");
    for(;;);
}

// 8.8 - SCID Transmit Interrupt
/* 函数：SCID_TX_ISR（SCI D发送中断服务程序） */
interrupt void SCID_TX_ISR(void)
{
    // Insert ISR Code here
    /* 在这里插入中断服务程序代码 */

    // To receive more interrupts from this PIE group,
    // acknowledge this interrupt.
    /* 要从此PIE组接收更多中断，确认此中断 */
    // PieCtrlRegs.PIEACK.all = PIEACK_GROUP8;

    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
    /* 以下两行仅用于调试，在此处暂停处理器 */
    /* 插入中断服务程序代码后移除 */
    asm ("      ESTOP0");
    for(;;);
}

// 9.1 - SCIA Receive Interrupt
/* 函数：SCIA_RX_ISR（SCI A接收中断服务程序） */
interrupt void SCIA_RX_ISR(void)
{
    // Insert ISR Code here
    /* 在这里插入中断服务程序代码 */

    // To receive more interrupts from this PIE group,
    // acknowledge this interrupt.
    /* 要从此PIE组接收更多中断，确认此中断 */
    // PieCtrlRegs.PIEACK.all = PIEACK_GROUP9;

    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
    /* 以下两行仅用于调试，在此处暂停处理器 */
    /* 插入中断服务程序代码后移除 */
    asm ("      ESTOP0");
    for(;;);
}

// 9.2 - SCIA Transmit Interrupt
/* 函数：SCIA_TX_ISR（SCI A发送中断服务程序） */
interrupt void SCIA_TX_ISR(void)
{
    // Insert ISR Code here
    /* 在这里插入中断服务程序代码 */

    // To receive more interrupts from this PIE group,
    // acknowledge this interrupt.
    /* 要从此PIE组接收更多中断，确认此中断 */
    // PieCtrlRegs.PIEACK.all = PIEACK_GROUP9;

    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
    /* 以下两行仅用于调试，在此处暂停处理器 */
    /* 插入中断服务程序代码后移除 */
    asm ("      ESTOP0");
    for(;;);
}

// 9.3 - SCIB Receive Interrupt
/* 函数：SCIB_RX_ISR（SCI B接收中断服务程序） */
interrupt void SCIB_RX_ISR(void)
{
    // Insert ISR Code here
    /* 在这里插入中断服务程序代码 */

    // To receive more interrupts from this PIE group,
    // acknowledge this interrupt.
    /* 要从此PIE组接收更多中断，确认此中断 */
    // PieCtrlRegs.PIEACK.all = PIEACK_GROUP9;

    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
    /* 以下两行仅用于调试，在此处暂停处理器 */
    /* 插入中断服务程序代码后移除 */
    asm ("      ESTOP0");
    for(;;);
}

// 9.4 - SCIB Transmit Interrupt
/* 函数：SCIB_TX_ISR（SCI B发送中断服务程序） */
interrupt void SCIB_TX_ISR(void)
{
    // Insert ISR Code here
    /* 在这里插入中断服务程序代码 */

    // To receive more interrupts from this PIE group,
    // acknowledge this interrupt.
    /* 要从此PIE组接收更多中断，确认此中断 */
    // PieCtrlRegs.PIEACK.all = PIEACK_GROUP9;

    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
    /* 以下两行仅用于调试，在此处暂停处理器 */
    /* 插入中断服务程序代码后移除 */
    asm ("      ESTOP0");
    for(;;);
}

// 9.5 - DCANA Interrupt 1
/* 函数：DCANA_1_ISR（CAN A中断1服务程序） */
interrupt void DCANA_1_ISR(void)
{
    // Insert ISR Code here
    /* 在这里插入中断服务程序代码 */

    // To receive more interrupts from this PIE group,
    // acknowledge this interrupt.
    /* 要从此PIE组接收更多中断，确认此中断 */
    // PieCtrlRegs.PIEACK.all = PIEACK_GROUP9;

    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
    /* 以下两行仅用于调试，在此处暂停处理器 */
    /* 插入中断服务程序代码后移除 */
    asm ("      ESTOP0");
    for(;;);
}

// 9.6 - DCANA Interrupt 2
/* 函数：DCANA_2_ISR（CAN A中断2服务程序） */
interrupt void DCANA_2_ISR(void)
{
    // Insert ISR Code here
    /* 在这里插入中断服务程序代码 */

    // To receive more interrupts from this PIE group,
    // acknowledge this interrupt.
    /* 要从此PIE组接收更多中断，确认此中断 */
    // PieCtrlRegs.PIEACK.all = PIEACK_GROUP9;

    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
    /* 以下两行仅用于调试，在此处暂停处理器 */
    /* 插入中断服务程序代码后移除 */
    asm ("      ESTOP0");
    for(;;);
}

// 9.7 - DCANB Interrupt 1
/* 函数：DCANB_1_ISR（CAN B中断1服务程序） */
interrupt void DCANB_1_ISR(void)
{
    // Insert ISR Code here
    /* 在这里插入中断服务程序代码 */

    // To receive more interrupts from this PIE group,
    // acknowledge this interrupt.
    /* 要从此PIE组接收更多中断，确认此中断 */
    // PieCtrlRegs.PIEACK.all = PIEACK_GROUP9;

    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
    /* 以下两行仅用于调试，在此处暂停处理器 */
    /* 插入中断服务程序代码后移除 */
    asm ("      ESTOP0");
    for(;;);
}

// 9.8 - DCANB Interrupt 2
/* 函数：DCANB_2_ISR（CAN B中断2服务程序） */
interrupt void DCANB_2_ISR(void)
{
    // Insert ISR Code here
    /* 在这里插入中断服务程序代码 */

    // To receive more interrupts from this PIE group,
    // acknowledge this interrupt.
    /* 要从此PIE组接收更多中断，确认此中断 */
    // PieCtrlRegs.PIEACK.all = PIEACK_GROUP9;

    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
    /* 以下两行仅用于调试，在此处暂停处理器 */
    /* 插入中断服务程序代码后移除 */
    asm ("      ESTOP0");
    for(;;);
}

// 10.1 - ADCA Event Interrupt
/* 函数：ADCA_EVT_ISR（ADC A事件中断服务程序） */
interrupt void ADCA_EVT_ISR(void)
{
    // Insert ISR Code here
    /* 在这里插入中断服务程序代码 */

    // To receive more interrupts from this PIE group,
    // acknowledge this interrupt.
    /* 要从此PIE组接收更多中断，确认此中断 */
    // PieCtrlRegs.PIEACK.all = PIEACK_GROUP10;

    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
    /* 以下两行仅用于调试，在此处暂停处理器 */
    /* 插入中断服务程序代码后移除 */
    asm ("      ESTOP0");
    for(;;);
}

// 10.2 - ADCA Interrupt 2
/* 函数：ADCA2_ISR（ADC A中断2服务程序） */
interrupt void ADCA2_ISR(void)
{
    // Insert ISR Code here
    /* 在这里插入中断服务程序代码 */

    // To receive more interrupts from this PIE group,
    // acknowledge this interrupt.
    /* 要从此PIE组接收更多中断，确认此中断 */
    // PieCtrlRegs.PIEACK.all = PIEACK_GROUP10;

    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
    /* 以下两行仅用于调试，在此处暂停处理器 */
    /* 插入中断服务程序代码后移除 */
    asm ("      ESTOP0");
    for(;;);
}

// 10.3 - ADCA Interrupt 3
/* 函数：ADCA3_ISR（ADC A中断3服务程序） */
interrupt void ADCA3_ISR(void)
{
    // Insert ISR Code here
    /* 在这里插入中断服务程序代码 */

    // To receive more interrupts from this PIE group,
    // acknowledge this interrupt.
    /* 要从此PIE组接收更多中断，确认此中断 */
    // PieCtrlRegs.PIEACK.all = PIEACK_GROUP10;

    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
    /* 以下两行仅用于调试，在此处暂停处理器 */
    /* 插入中断服务程序代码后移除 */
    asm ("      ESTOP0");
    for(;;);
}

// 10.4 - ADCA Interrupt 4
/* 函数：ADCA4_ISR（ADC A中断4服务程序） */
interrupt void ADCA4_ISR(void)
{
    // Insert ISR Code here
    /* 在这里插入中断服务程序代码 */

    // To receive more interrupts from this PIE group,
    // acknowledge this interrupt.
    /* 要从此PIE组接收更多中断，确认此中断 */
    // PieCtrlRegs.PIEACK.all = PIEACK_GROUP10;

    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
    /* 以下两行仅用于调试，在此处暂停处理器 */
    /* 插入中断服务程序代码后移除 */
    asm ("      ESTOP0");
    for(;;);
}

// 10.5 - ADCB Event Interrupt
/* 函数：ADCB_EVT_ISR（ADC B事件中断服务程序） */
interrupt void ADCB_EVT_ISR(void)
{
    // Insert ISR Code here
    /* 在这里插入中断服务程序代码 */

    // To receive more interrupts from this PIE group,
    // acknowledge this interrupt.
    /* 要从此PIE组接收更多中断，确认此中断 */
    // PieCtrlRegs.PIEACK.all = PIEACK_GROUP10;

    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
    /* 以下两行仅用于调试，在此处暂停处理器 */
    /* 插入中断服务程序代码后移除 */
    asm ("      ESTOP0");
    for(;;);
}

// 10.6 - ADCB Interrupt 2
/* 函数：ADCB2_ISR（ADC B中断2服务程序） */
interrupt void ADCB2_ISR(void)
{
    // Insert ISR Code here
    /* 在这里插入中断服务程序代码 */

    // To receive more interrupts from this PIE group,
    // acknowledge this interrupt.
    /* 要从此PIE组接收更多中断，确认此中断 */
    // PieCtrlRegs.PIEACK.all = PIEACK_GROUP10;

    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
    /* 以下两行仅用于调试，在此处暂停处理器 */
    /* 插入中断服务程序代码后移除 */
    asm ("      ESTOP0");
    for(;;);
}

// 10.7 - ADCB Interrupt 3
/* 函数：ADCB3_ISR（ADC B中断3服务程序） */
interrupt void ADCB3_ISR(void)
{
    // Insert ISR Code here
    /* 在这里插入中断服务程序代码 */

    // To receive more interrupts from this PIE group,
    // acknowledge this interrupt.
    /* 要从此PIE组接收更多中断，确认此中断 */
    // PieCtrlRegs.PIEACK.all = PIEACK_GROUP10;

    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
    /* 以下两行仅用于调试，在此处暂停处理器 */
    /* 插入中断服务程序代码后移除 */
    asm ("      ESTOP0");
    for(;;);
}

// 10.8 - ADCB Interrupt 4
/* 函数：ADCB4_ISR（ADC B中断4服务程序） */
interrupt void ADCB4_ISR(void)
{
    // Insert ISR Code here
    /* 在这里插入中断服务程序代码 */

    // To receive more interrupts from this PIE group,
    // acknowledge this interrupt.
    /* 要从此PIE组接收更多中断，确认此中断 */
    // PieCtrlRegs.PIEACK.all = PIEACK_GROUP10;

    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
    /* 以下两行仅用于调试，在此处暂停处理器 */
    /* 插入中断服务程序代码后移除 */
    asm ("      ESTOP0");
    for(;;);
}

// 11.1 - CLA1 Interrupt 1
/* 函数：CLA1_1_ISR（控制律加速器1中断1服务程序） */
interrupt void CLA1_1_ISR(void)
{
    // Insert ISR Code here
    /* 在这里插入中断服务程序代码 */

    // To receive more interrupts from this PIE group,
    // acknowledge this interrupt.
    /* 要从此PIE组接收更多中断，确认此中断 */
    // PieCtrlRegs.PIEACK.all = PIEACK_GROUP11;

    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
    /* 以下两行仅用于调试，在此处暂停处理器 */
    /* 插入中断服务程序代码后移除 */
    asm ("      ESTOP0");
    for(;;);
}

// 11.2 - CLA1 Interrupt 2
/* 函数：CLA1_2_ISR（控制律加速器1中断2服务程序） */
interrupt void CLA1_2_ISR(void)
{
    // Insert ISR Code here
    /* 在这里插入中断服务程序代码 */

    // To receive more interrupts from this PIE group,
    // acknowledge this interrupt.
    /* 要从此PIE组接收更多中断，确认此中断 */
    // PieCtrlRegs.PIEACK.all = PIEACK_GROUP11;

    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
    /* 以下两行仅用于调试，在此处暂停处理器 */
    /* 插入中断服务程序代码后移除 */
    asm ("      ESTOP0");
    for(;;);
}

// 11.3 - CLA1 Interrupt 3
/* 函数：CLA1_3_ISR（控制律加速器1中断3服务程序） */
interrupt void CLA1_3_ISR(void)
{
    // Insert ISR Code here
    /* 在这里插入中断服务程序代码 */

    // To receive more interrupts from this PIE group,
    // acknowledge this interrupt.
    /* 要从此PIE组接收更多中断，确认此中断 */
    // PieCtrlRegs.PIEACK.all = PIEACK_GROUP11;

    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
    /* 以下两行仅用于调试，在此处暂停处理器 */
    /* 插入中断服务程序代码后移除 */
    asm ("      ESTOP0");
    for(;;);
}

// 11.4 - CLA1 Interrupt 4
/* 函数：CLA1_4_ISR（控制律加速器1中断4服务程序） */
interrupt void CLA1_4_ISR(void)
{
    // Insert ISR Code here
    /* 在这里插入中断服务程序代码 */

    // To receive more interrupts from this PIE group,
    // acknowledge this interrupt.
    /* 要从此PIE组接收更多中断，确认此中断 */
    // PieCtrlRegs.PIEACK.all = PIEACK_GROUP11;

    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
    /* 以下两行仅用于调试，在此处暂停处理器 */
    /* 插入中断服务程序代码后移除 */
    asm ("      ESTOP0");
    for(;;);
}

// 11.5 - CLA1 Interrupt 5
/* 函数：CLA1_5_ISR（控制律加速器1中断5服务程序） */
interrupt void CLA1_5_ISR(void)
{
    // Insert ISR Code here
    /* 在这里插入中断服务程序代码 */

    // To receive more interrupts from this PIE group,
    // acknowledge this interrupt.
    /* 要从此PIE组接收更多中断，确认此中断 */
    // PieCtrlRegs.PIEACK.all = PIEACK_GROUP11;

    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
    /* 以下两行仅用于调试，在此处暂停处理器 */
    /* 插入中断服务程序代码后移除 */
    asm ("      ESTOP0");
    for(;;);
}

// 11.6 - CLA1 Interrupt 6
/* 函数：CLA1_6_ISR（控制律加速器1中断6服务程序） */
interrupt void CLA1_6_ISR(void)
{
    // Insert ISR Code here
    /* 在这里插入中断服务程序代码 */

    // To receive more interrupts from this PIE group,
    // acknowledge this interrupt.
    /* 要从此PIE组接收更多中断，确认此中断 */
    // PieCtrlRegs.PIEACK.all = PIEACK_GROUP11;

    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
    /* 以下两行仅用于调试，在此处暂停处理器 */
    /* 插入中断服务程序代码后移除 */
    asm ("      ESTOP0");
    for(;;);
}

// 11.7 - CLA1 Interrupt 7
/* 函数：CLA1_7_ISR（控制律加速器1中断7服务程序） */
interrupt void CLA1_7_ISR(void)
{
    // Insert ISR Code here
    /* 在这里插入中断服务程序代码 */

    // To receive more interrupts from this PIE group,
    // acknowledge this interrupt.
    /* 要从此PIE组接收更多中断，确认此中断 */
    // PieCtrlRegs.PIEACK.all = PIEACK_GROUP11;

    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
    /* 以下两行仅用于调试，在此处暂停处理器 */
    /* 插入中断服务程序代码后移除 */
    asm ("      ESTOP0");
    for(;;);
}

// 11.8 - CLA1 Interrupt 8
/* 函数：CLA1_8_ISR（控制律加速器1中断8服务程序） */
interrupt void CLA1_8_ISR(void)
{
    // Insert ISR Code here
    /* 在这里插入中断服务程序代码 */

    // To receive more interrupts from this PIE group,
    // acknowledge this interrupt.
    /* 要从此PIE组接收更多中断，确认此中断 */
    // PieCtrlRegs.PIEACK.all = PIEACK_GROUP11;

    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
    /* 以下两行仅用于调试，在此处暂停处理器 */
    /* 插入中断服务程序代码后移除 */
    asm ("      ESTOP0");
    for(;;);
}

// 12.1 - XINT3 Interrupt
/* 函数：XINT3_ISR（外部中断3服务程序） */
interrupt void XINT3_ISR(void)
{
    // Insert ISR Code here
    /* 在这里插入中断服务程序代码 */

    // To receive more interrupts from this PIE group,
    // acknowledge this interrupt.
    /* 要从此PIE组接收更多中断，确认此中断 */
    // PieCtrlRegs.PIEACK.all = PIEACK_GROUP12;

    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
    /* 以下两行仅用于调试，在此处暂停处理器 */
    /* 插入中断服务程序代码后移除 */
    asm ("      ESTOP0");
    for(;;);
}

// 12.2 - XINT4 Interrupt
/* 函数：XINT4_ISR（外部中断4服务程序） */
interrupt void XINT4_ISR(void)
{
    // Insert ISR Code here
    /* 在这里插入中断服务程序代码 */

    // To receive more interrupts from this PIE group,
    // acknowledge this interrupt.
    /* 要从此PIE组接收更多中断，确认此中断 */
    // PieCtrlRegs.PIEACK.all = PIEACK_GROUP12;

    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
    /* 以下两行仅用于调试，在此处暂停处理器 */
    /* 插入中断服务程序代码后移除 */
    asm ("      ESTOP0");
    for(;;);
}

// 12.3 - XINT5 Interrupt
/* 函数：XINT5_ISR（外部中断5服务程序） */
interrupt void XINT5_ISR(void)
{
    // Insert ISR Code here
    /* 在这里插入中断服务程序代码 */

    // To receive more interrupts from this PIE group,
    // acknowledge this interrupt.
    /* 要从此PIE组接收更多中断，确认此中断 */
    // PieCtrlRegs.PIEACK.all = PIEACK_GROUP12;

    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
    /* 以下两行仅用于调试，在此处暂停处理器 */
    /* 插入中断服务程序代码后移除 */
    asm ("      ESTOP0");
    for(;;);
}

// 12.4 - PBIST Interrupt
/* 函数：PBIST_ISR（内建自测试中断服务程序） */
interrupt void PBIST_ISR(void)
{
    // Insert ISR Code here
    /* 在这里插入中断服务程序代码 */

    // To receive more interrupts from this PIE group,
    // acknowledge this interrupt.
    /* 要从此PIE组接收更多中断，确认此中断 */
    // PieCtrlRegs.PIEACK.all = PIEACK_GROUP12;

    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
    /* 以下两行仅用于调试，在此处暂停处理器 */
    /* 插入中断服务程序代码后移除 */
    asm ("      ESTOP0");
    for(;;);
}

// 12.6 - VCU Interrupt
/* 函数：VCU_ISR（向量计算单元中断服务程序） */
interrupt void VCU_ISR(void)
{
    // Insert ISR Code here
    /* 在这里插入中断服务程序代码 */

    // To receive more interrupts from this PIE group,
    // acknowledge this interrupt.
    /* 要从此PIE组接收更多中断，确认此中断 */
    // PieCtrlRegs.PIEACK.all = PIEACK_GROUP12;

    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
    /* 以下两行仅用于调试，在此处暂停处理器 */
    /* 插入中断服务程序代码后移除 */
    asm ("      ESTOP0");
    for(;;);
}

// 12.7 - FPU Overflow Interrupt
/* 函数：FPU_OVERFLOW_ISR（浮点单元溢出中断服务程序） */
interrupt void FPU_OVERFLOW_ISR(void)
{
    // Insert ISR Code here
    /* 在这里插入中断服务程序代码 */

    // To receive more interrupts from this PIE group,
    // acknowledge this interrupt.
    /* 要从此PIE组接收更多中断，确认此中断 */
    // PieCtrlRegs.PIEACK.all = PIEACK_GROUP12;

    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
    /* 以下两行仅用于调试，在此处暂停处理器 */
    /* 插入中断服务程序代码后移除 */
    asm ("      ESTOP0");
    for(;;);
}

// 12.8 - FPU Underflow Interrupt
/* 函数：FPU_UNDERFLOW_ISR（浮点单元下溢中断服务程序） */
interrupt void FPU_UNDERFLOW_ISR(void)
{
    // Insert ISR Code here
    /* 在这里插入中断服务程序代码 */

    // To receive more interrupts from this PIE group,
    // acknowledge this interrupt.
    /* 要从此PIE组接收更多中断，确认此中断 */
    // PieCtrlRegs.PIEACK.all = PIEACK_GROUP12;

    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
    /* 以下两行仅用于调试，在此处暂停处理器 */
    /* 插入中断服务程序代码后移除 */
    asm ("      ESTOP0");
    for(;;);
}

// 1.13 - IPC Interrupt 0
/* 函数：IPC0_ISR（IPC中断0服务程序） */
interrupt void IPC0_ISR(void)
{
    // Insert ISR Code here
    /* 在这里插入中断服务程序代码 */

    // To receive more interrupts from this PIE group,
    // acknowledge this interrupt.
    /* 要从此PIE组接收更多中断，确认此中断 */
    // PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;

    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
    /* 以下两行仅用于调试，在此处暂停处理器 */
    /* 插入中断服务程序代码后移除 */
    asm ("      ESTOP0");
    for(;;);
}

// 1.14 - IPC Interrupt 1
/* 函数：IPC1_ISR（IPC中断1服务程序） */
interrupt void IPC1_ISR(void)
{
    // Insert ISR Code here
    /* 在这里插入中断服务程序代码 */

    // To receive more interrupts from this PIE group,
    // acknowledge this interrupt.
    /* 要从此PIE组接收更多中断，确认此中断 */
    // PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;

    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
    /* 以下两行仅用于调试，在此处暂停处理器 */
    /* 插入中断服务程序代码后移除 */
    asm ("      ESTOP0");
    for(;;);
}

// 1.15 - IPC Interrupt 2
/* 函数：IPC2_ISR（IPC中断2服务程序） */
interrupt void IPC2_ISR(void)
{
    // Insert ISR Code here
    /* 在这里插入中断服务程序代码 */

    // To receive more interrupts from this PIE group,
    // acknowledge this interrupt.
    /* 要从此PIE组接收更多中断，确认此中断 */
    // PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;

    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
    /* 以下两行仅用于调试，在此处暂停处理器 */
    /* 插入中断服务程序代码后移除 */
    asm ("      ESTOP0");
    for(;;);
}

// 1.16 - IPC Interrupt 3
/* 函数：IPC3_ISR（IPC中断3服务程序） */
interrupt void IPC3_ISR(void)
{
    // Insert ISR Code here
    /* 在这里插入中断服务程序代码 */

    // To receive more interrupts from this PIE group,
    // acknowledge this interrupt.
    /* 要从此PIE组接收更多中断，确认此中断 */
    // PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;

    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
    /* 以下两行仅用于调试，在此处暂停处理器 */
    /* 插入中断服务程序代码后移除 */
    asm ("      ESTOP0");
    for(;;);
}

// 2.9 - ePWM9 Trip Zone Interrupt
/* 函数：EPWM9_TZ_ISR（增强型PWM9跳闸区中断服务程序） */
interrupt void EPWM9_TZ_ISR(void)
{
    // Insert ISR Code here
    /* 在这里插入中断服务程序代码 */

    // To receive more interrupts from this PIE group,
    // acknowledge this interrupt.
    /* 要从此PIE组接收更多中断，确认此中断 */
    // PieCtrlRegs.PIEACK.all = PIEACK_GROUP2;

    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
    /* 以下两行仅用于调试，在此处暂停处理器 */
    /* 插入中断服务程序代码后移除 */
    asm ("      ESTOP0");
    for(;;);
}

// 2.10 - ePWM10 Trip Zone Interrupt
/* 函数：EPWM10_TZ_ISR（增强型PWM10跳闸区中断服务程序） */
interrupt void EPWM10_TZ_ISR(void)
{
    // Insert ISR Code here
    /* 在这里插入中断服务程序代码 */

    // To receive more interrupts from this PIE group,
    // acknowledge this interrupt.
    /* 要从此PIE组接收更多中断，确认此中断 */
    // PieCtrlRegs.PIEACK.all = PIEACK_GROUP2;

    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
    /* 以下两行仅用于调试，在此处暂停处理器 */
    /* 插入中断服务程序代码后移除 */
    asm ("      ESTOP0");
    for(;;);
}

// 2.11 - ePWM11 Trip Zone Interrupt
/* 函数：EPWM11_TZ_ISR（增强型PWM11跳闸区中断服务程序） */
interrupt void EPWM11_TZ_ISR(void)
{
    // Insert ISR Code here
    /* 在这里插入中断服务程序代码 */

    // To receive more interrupts from this PIE group,
    // acknowledge this interrupt.
    /* 要从此PIE组接收更多中断，确认此中断 */
    // PieCtrlRegs.PIEACK.all = PIEACK_GROUP2;

    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
    /* 以下两行仅用于调试，在此处暂停处理器 */
    /* 插入中断服务程序代码后移除 */
    asm ("      ESTOP0");
    for(;;);
}

// 2.12 - ePWM12 Trip Zone Interrupt
/* 函数：EPWM12_TZ_ISR（增强型PWM12跳闸区中断服务程序） */
interrupt void EPWM12_TZ_ISR(void)
{
    // Insert ISR Code here
    /* 在这里插入中断服务程序代码 */

    // To receive more interrupts from this PIE group,
    // acknowledge this interrupt.
    /* 要从此PIE组接收更多中断，确认此中断 */
    // PieCtrlRegs.PIEACK.all = PIEACK_GROUP2;

    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
    /* 以下两行仅用于调试，在此处暂停处理器 */
    /* 插入中断服务程序代码后移除 */
    asm ("      ESTOP0");
    for(;;);
}

// 3.9 - ePWM9 Interrupt
/* 函数：EPWM9_ISR（增强型PWM9中断服务程序） */
interrupt void EPWM9_ISR(void)
{
    // Insert ISR Code here
    /* 在这里插入中断服务程序代码 */

    // To receive more interrupts from this PIE group,
    // acknowledge this interrupt.
    /* 要从此PIE组接收更多中断，确认此中断 */
    // PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;

    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
    /* 以下两行仅用于调试，在此处暂停处理器 */
    /* 插入中断服务程序代码后移除 */
    asm ("      ESTOP0");
    for(;;);
}

// 3.10 - ePWM10 Interrupt
/* 函数：EPWM10_ISR（增强型PWM10中断服务程序） */
interrupt void EPWM10_ISR(void)
{
    // Insert ISR Code here
    /* 在这里插入中断服务程序代码 */

    // To receive more interrupts from this PIE group,
    // acknowledge this interrupt.
    /* 要从此PIE组接收更多中断，确认此中断 */
    // PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;

    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
    /* 以下两行仅用于调试，在此处暂停处理器 */
    /* 插入中断服务程序代码后移除 */
    asm ("      ESTOP0");
    for(;;);
}

// 3.11 - ePWM11 Interrupt
/* 函数：EPWM11_ISR（增强型PWM11中断服务程序） */
interrupt void EPWM11_ISR(void)
{
    // Insert ISR Code here
    /* 在这里插入中断服务程序代码 */

    // To receive more interrupts from this PIE group,
    // acknowledge this interrupt.
    /* 要从此PIE组接收更多中断，确认此中断 */
    // PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;

    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
    /* 以下两行仅用于调试，在此处暂停处理器 */
    /* 插入中断服务程序代码后移除 */
    asm ("      ESTOP0");
    for(;;);
}

// 3.12 - ePWM12 Interrupt
/* 函数：EPWM12_ISR（增强型PWM12中断服务程序） */
interrupt void EPWM12_ISR(void)
{
    // Insert ISR Code here
    /* 在这里插入中断服务程序代码 */

    // To receive more interrupts from this PIE group,
    // acknowledge this interrupt.
    /* 要从此PIE组接收更多中断，确认此中断 */
    // PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;

    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
    /* 以下两行仅用于调试，在此处暂停处理器 */
    /* 插入中断服务程序代码后移除 */
    asm ("      ESTOP0");
    for(;;);
}

// 5.9 - SD1 Interrupt
/* 函数：SD1_ISR（Sigma-Delta滤波器1中断服务程序） */
interrupt void SD1_ISR(void)
{
    // Insert ISR Code here
    /* 在这里插入中断服务程序代码 */

    // To receive more interrupts from this PIE group,
    // acknowledge this interrupt.
    /* 要从此PIE组接收更多中断，确认此中断 */
    // PieCtrlRegs.PIEACK.all = PIEACK_GROUP5;

    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
    /* 以下两行仅用于调试，在此处暂停处理器 */
    /* 插入中断服务程序代码后移除 */
    asm ("      ESTOP0");
    for(;;);
}

// 5.10 - SD2 Interrupt
/* 函数：SD2_ISR（Sigma-Delta滤波器2中断服务程序） */
interrupt void SD2_ISR(void)
{
    // Insert ISR Code here
    /* 在这里插入中断服务程序代码 */

    // To receive more interrupts from this PIE group,
    // acknowledge this interrupt.
    /* 要从此PIE组接收更多中断，确认此中断 */
    // PieCtrlRegs.PIEACK.all = PIEACK_GROUP5;

    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
    /* 以下两行仅用于调试，在此处暂停处理器 */
    /* 插入中断服务程序代码后移除 */
    asm ("      ESTOP0");
    for(;;);
}

// 6.9 - SPIC Receive Interrupt
/* 函数：SPIC_RX_ISR（SPI C接收中断服务程序） */
interrupt void SPIC_RX_ISR(void)
{
    // Insert ISR Code here
    /* 在这里插入中断服务程序代码 */

    // To receive more interrupts from this PIE group,
    // acknowledge this interrupt.
    /* 要从此PIE组接收更多中断，确认此中断 */
    // PieCtrlRegs.PIEACK.all = PIEACK_GROUP6;

    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
    /* 以下两行仅用于调试，在此处暂停处理器 */
    /* 插入中断服务程序代码后移除 */
    asm ("      ESTOP0");
    for(;;);
}

// 6.10 - SPIC Transmit Interrupt
/* 函数：SPIC_TX_ISR（SPI C发送中断服务程序） */
interrupt void SPIC_TX_ISR(void)
{
    // Insert ISR Code here
    /* 在这里插入中断服务程序代码 */

    // To receive more interrupts from this PIE group,
    // acknowledge this interrupt.
    /* 要从此PIE组接收更多中断，确认此中断 */
    // PieCtrlRegs.PIEACK.all = PIEACK_GROUP6;

    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
    /* 以下两行仅用于调试，在此处暂停处理器 */
    /* 插入中断服务程序代码后移除 */
    asm ("      ESTOP0");
    for(;;);
}

#ifdef CPU1
// 8.15 - uPPA Interrupt
/* 函数：UPPA_ISR（通用并行端口A中断服务程序） */
interrupt void UPPA_ISR(void)
{
    // Insert ISR Code here
    /* 在这里插入中断服务程序代码 */

    // To receive more interrupts from this PIE group,
    // acknowledge this interrupt.
    /* 要从此PIE组接收更多中断，确认此中断 */
    // PieCtrlRegs.PIEACK.all = PIEACK_GROUP8;

    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
    /* 以下两行仅用于调试，在此处暂停处理器 */
    /* 插入中断服务程序代码后移除 */
    asm ("      ESTOP0");
    for(;;);
}
#endif

#ifdef CPU1
// 9.15 - USBA Interrupt
/* 函数：USBA_ISR（USB A中断服务程序） */
interrupt void USBA_ISR(void)
{
    // Insert ISR Code here
    /* 在这里插入中断服务程序代码 */

    // To receive more interrupts from this PIE group,
    // acknowledge this interrupt.
    /* 要从此PIE组接收更多中断，确认此中断 */
    // PieCtrlRegs.PIEACK.all = PIEACK_GROUP9;

    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
    /* 以下两行仅用于调试，在此处暂停处理器 */
    /* 插入中断服务程序代码后移除 */
    asm ("      ESTOP0");
    for(;;);
}
#endif

// 10.9 - ADCC Event Interrupt
/* 函数：ADCC_EVT_ISR（ADC C事件中断服务程序） */
interrupt void ADCC_EVT_ISR(void)
{
    // Insert ISR Code here
    /* 在这里插入中断服务程序代码 */

    // To receive more interrupts from this PIE group,
    // acknowledge this interrupt.
    /* 要从此PIE组接收更多中断，确认此中断 */
    // PieCtrlRegs.PIEACK.all = PIEACK_GROUP10;

    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
    /* 以下两行仅用于调试，在此处暂停处理器 */
    /* 插入中断服务程序代码后移除 */
    asm ("      ESTOP0");
    for(;;);
}

// 10.10 - ADCC Interrupt 2
/* 函数：ADCC2_ISR（ADC C中断2服务程序） */
interrupt void ADCC2_ISR(void)
{
    // Insert ISR Code here
    /* 在这里插入中断服务程序代码 */

    // To receive more interrupts from this PIE group,
    // acknowledge this interrupt.
    /* 要从此PIE组接收更多中断，确认此中断 */
    // PieCtrlRegs.PIEACK.all = PIEACK_GROUP10;

    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
    /* 以下两行仅用于调试，在此处暂停处理器 */
    /* 插入中断服务程序代码后移除 */
    asm ("      ESTOP0");
    for(;;);
}

// 10.11 - ADCC Interrupt 3
/* 函数：ADCC3_ISR（ADC C中断3服务程序） */
interrupt void ADCC3_ISR(void)
{
    // Insert ISR Code here
    /* 在这里插入中断服务程序代码 */

    // To receive more interrupts from this PIE group,
    // acknowledge this interrupt.
    /* 要从此PIE组接收更多中断，确认此中断 */
    // PieCtrlRegs.PIEACK.all = PIEACK_GROUP10;

    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
    /* 以下两行仅用于调试，在此处暂停处理器 */
    /* 插入中断服务程序代码后移除 */
    asm ("      ESTOP0");
    for(;;);
}

// 10.12 - ADCC Interrupt 4
/* 函数：ADCC4_ISR（ADC C中断4服务程序） */
interrupt void ADCC4_ISR(void)
{
    // Insert ISR Code here
    /* 在这里插入中断服务程序代码 */

    // To receive more interrupts from this PIE group,
    // acknowledge this interrupt.
    /* 要从此PIE组接收更多中断，确认此中断 */
    // PieCtrlRegs.PIEACK.all = PIEACK_GROUP10;

    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
    /* 以下两行仅用于调试，在此处暂停处理器 */
    /* 插入中断服务程序代码后移除 */
    asm ("      ESTOP0");
    for(;;);
}

// 10.13 - ADCD Event Interrupt
/* 函数：ADCD_EVT_ISR（ADC D事件中断服务程序） */
interrupt void ADCD_EVT_ISR(void)
{
    // Insert ISR Code here
    /* 在这里插入中断服务程序代码 */

    // To receive more interrupts from this PIE group,
    // acknowledge this interrupt.
    /* 要从此PIE组接收更多中断，确认此中断 */
    // PieCtrlRegs.PIEACK.all = PIEACK_GROUP10;

    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
    /* 以下两行仅用于调试，在此处暂停处理器 */
    /* 插入中断服务程序代码后移除 */
    asm ("      ESTOP0");
    for(;;);
}

// 10.14 - ADCD Interrupt 2
/* 函数：ADCD2_ISR（ADC D中断2服务程序） */
interrupt void ADCD2_ISR(void)
{
    // Insert ISR Code here
    /* 在这里插入中断服务程序代码 */

    // To receive more interrupts from this PIE group,
    // acknowledge this interrupt.
    /* 要从此PIE组接收更多中断，确认此中断 */
    // PieCtrlRegs.PIEACK.all = PIEACK_GROUP10;

    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
    /* 以下两行仅用于调试，在此处暂停处理器 */
    /* 插入中断服务程序代码后移除 */
    asm ("      ESTOP0");
    for(;;);
}

// 10.15 - ADCD Interrupt 3
/* 函数：ADCD3_ISR（ADC D中断3服务程序） */
interrupt void ADCD3_ISR(void)
{
    // Insert ISR Code here
    /* 在这里插入中断服务程序代码 */

    // To receive more interrupts from this PIE group,
    // acknowledge this interrupt.
    /* 要从此PIE组接收更多中断，确认此中断 */
    // PieCtrlRegs.PIEACK.all = PIEACK_GROUP10;

    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
    /* 以下两行仅用于调试，在此处暂停处理器 */
    /* 插入中断服务程序代码后移除 */
    asm ("      ESTOP0");
    for(;;);
}

// 10.16 - ADCD Interrupt 4
/* 函数：ADCD4_ISR（ADC D中断4服务程序） */
interrupt void ADCD4_ISR(void)
{
    // Insert ISR Code here
    /* 在这里插入中断服务程序代码 */

    // To receive more interrupts from this PIE group,
    // acknowledge this interrupt.
    /* 要从此PIE组接收更多中断，确认此中断 */
    // PieCtrlRegs.PIEACK.all = PIEACK_GROUP10;

    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
    /* 以下两行仅用于调试，在此处暂停处理器 */
    /* 插入中断服务程序代码后移除 */
    asm ("      ESTOP0");
    for(;;);
}

// 12.9 - EMIF Error Interrupt
/* 函数：EMIF_ERROR_ISR（外部存储器接口错误中断服务程序） */
interrupt void EMIF_ERROR_ISR(void)
{
    // Insert ISR Code here
    /* 在这里插入中断服务程序代码 */

    // To receive more interrupts from this PIE group,
    // acknowledge this interrupt.
    /* 要从此PIE组接收更多中断，确认此中断 */
    // PieCtrlRegs.PIEACK.all = PIEACK_GROUP12;

    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
    /* 以下两行仅用于调试，在此处暂停处理器 */
    /* 插入中断服务程序代码后移除 */
    asm ("      ESTOP0");
    for(;;);
}

// 12.10 - RAM Correctable Error Interrupt
/* 函数：RAM_CORRECTABLE_ERROR_ISR（RAM可纠正错误中断服务程序） */
interrupt void RAM_CORRECTABLE_ERROR_ISR(void)
{
    // Insert ISR Code here
    /* 在这里插入中断服务程序代码 */

    // To receive more interrupts from this PIE group,
    // acknowledge this interrupt.
    /* 要从此PIE组接收更多中断，确认此中断 */
    // PieCtrlRegs.PIEACK.all = PIEACK_GROUP12;

    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
    /* 以下两行仅用于调试，在此处暂停处理器 */
    /* 插入中断服务程序代码后移除 */
    asm ("      ESTOP0");
    for(;;);
}

// 12.11 - Flash Correctable Error Interrupt
/* 函数：FLASH_CORRECTABLE_ERROR_ISR（闪存可纠正错误中断服务程序） */
interrupt void FLASH_CORRECTABLE_ERROR_ISR(void)
{
    // Insert ISR Code here
    /* 在这里插入中断服务程序代码 */

    // To receive more interrupts from this PIE group,
    // acknowledge this interrupt.
    /* 要从此PIE组接收更多中断，确认此中断 */
    // PieCtrlRegs.PIEACK.all = PIEACK_GROUP12;

    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
    /* 以下两行仅用于调试，在此处暂停处理器 */
    /* 插入中断服务程序代码后移除 */
    asm ("      ESTOP0");
    for(;;);
}

// 12.12 - RAM Access Violation Interrupt
/* 函数：RAM_ACCESS_VIOLATION_ISR（RAM访问违规中断服务程序） */
interrupt void RAM_ACCESS_VIOLATION_ISR(void)
{
    // Insert ISR Code here
    /* 在这里插入中断服务程序代码 */

    // To receive more interrupts from this PIE group,
    // acknowledge this interrupt.
    /* 要从此PIE组接收更多中断，确认此中断 */
    // PieCtrlRegs.PIEACK.all = PIEACK_GROUP12;

    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
    /* 以下两行仅用于调试，在此处暂停处理器 */
    /* 插入中断服务程序代码后移除 */
    asm ("      ESTOP0");
    for(;;);
}

// 12.13 - System PLL Slip Interrupt
/* 函数：SYS_PLL_SLIP_ISR（系统PLL滑移中断服务程序） */
interrupt void SYS_PLL_SLIP_ISR(void)
{
    // Insert ISR Code here
    /* 在这里插入中断服务程序代码 */

    // To receive more interrupts from this PIE group,
    // acknowledge this interrupt.
    /* 要从此PIE组接收更多中断，确认此中断 */
    // PieCtrlRegs.PIEACK.all = PIEACK_GROUP12;

    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
    /* 以下两行仅用于调试，在此处暂停处理器 */
    /* 插入中断服务程序代码后移除 */
    asm ("      ESTOP0");
    for(;;);
}

// 12.14 - Auxiliary PLL Slip Interrupt
/* 函数：AUX_PLL_SLIP_ISR（辅助PLL滑移中断服务程序） */
interrupt void AUX_PLL_SLIP_ISR(void)
{
    // Insert ISR Code here
    /* 在这里插入中断服务程序代码 */

    // To receive more interrupts from this PIE group,
    // acknowledge this interrupt.
    /* 要从此PIE组接收更多中断，确认此中断 */
    // PieCtrlRegs.PIEACK.all = PIEACK_GROUP12;

    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
    /* 以下两行仅用于调试，在此处暂停处理器 */
    /* 插入中断服务程序代码后移除 */
    asm ("      ESTOP0");
    for(;;);
}

// 12.15 - CLA Overflow Interrupt
/* 函数：CLA_OVERFLOW_ISR（控制律加速器溢出中断服务程序） */
interrupt void CLA_OVERFLOW_ISR(void)
{
    // Insert ISR Code here
    /* 在这里插入中断服务程序代码 */

    // To receive more interrupts from this PIE group,
    // acknowledge this interrupt.
    /* 要从此PIE组接收更多中断，确认此中断 */
    // PieCtrlRegs.PIEACK.all = PIEACK_GROUP12;

    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
    /* 以下两行仅用于调试，在此处暂停处理器 */
    /* 插入中断服务程序代码后移除 */
    asm ("      ESTOP0");
    for(;;);
}

// 12.16 - CLA Underflow Interrupt
/* 函数：CLA_UNDERFLOW_ISR（控制律加速器下溢中断服务程序） */
interrupt void CLA_UNDERFLOW_ISR(void)
{
    // Insert ISR Code here
    /* 在这里插入中断服务程序代码 */

    // To receive more interrupts from this PIE group,
    // acknowledge this interrupt.
    /* 要从此PIE组接收更多中断，确认此中断 */
    // PieCtrlRegs.PIEACK.all = PIEACK_GROUP12;

    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
    /* 以下两行仅用于调试，在此处暂停处理器 */
    /* 插入中断服务程序代码后移除 */
    asm ("      ESTOP0");
    for(;;);
}

//---------------------------------------------------------------------------
// Catch-all Default ISRs:

// Reserved ISR
/* 函数：PIE_RESERVED_ISR（PIE保留中断服务程序） */
interrupt void PIE_RESERVED_ISR(void)
{
    asm ("      ESTOP0");
    for(;;);
}

// Only does a return
/* 函数：EMPTY_ISR（空中断服务程序） */
interrupt void EMPTY_ISR(void)
{

}

// Unused ISR
/* 函数：NOTUSED_ISR（未使用中断服务程序） */
interrupt void NOTUSED_ISR(void)
{
    asm ("      ESTOP0");
    for(;;);
}



