



//OS：Operating System（操作系统）或实时内核 RTOS    CPU：CPU 相关接口和宏定义
//OS_CPU.h 是操作系统 CPU 适配头文件，提供任务切换、上下文管理、临界区和 Tick 配置接口，实现 RTOS 或内核与特定 CPU 架构的绑定



// 全局变量作用域控制宏：根据OS_GLOBALS是否定义，决定OS_EXT为空白/extern
// 场景1：定义OS_GLOBALS（如在OS全局变量实现文件中）→ OS_EXT为空，变量为全局定义
// 场景2：未定义OS_GLOBALS（如在头文件中）→ OS_EXT为extern，变量为外部声明
#ifdef	OS_GLOBALS
#define	OS_EXT
#else
#define	OS_EXT	extern
#endif

/********************************************************************************
*Basic variables type definition						*
********************************************************************************/
// 布尔类型别名：映射为无符号字符型（1字节），用于表示真/假（0/非0）
#define		BOOLEAN		unsigned char
// 8位无符号整型别名：映射为无符号字符型（1字节，取值范围0~255）
#define		INT8U		unsigned char
// 8位有符号整型别名：映射为有符号字符型（1字节，取值范围-128~127）
#define		INT8S		signed char
// CPU最小整型单位别名：映射为无符号整型（适配目标CPU的最小整型长度，通常2字节）
// 注释说明：该类型为CPU中数据处理的最小单位
#define		INTOS		unsigned int		// The smallest unit int the cpu
// 16位无符号整型别名：映射为无符号整型（通常2字节，取值范围0~65535）
#define		INT16U		unsigned int
// 16位有符号整型别名：映射为有符号整型（通常2字节，取值范围-32768~32767）
#define		INT16S		signed int
// 32位无符号整型别名：映射为无符号长整型（4字节，取值范围0~4294967295）
#define		INT32U		unsigned long
// 32位有符号整型别名：映射为有符号长整型（4字节，取值范围-2147483648~2147483647）
#define		INT32S		signed long
// 64位无符号整型别名：映射为无符号长长整型（8字节，超大数值存储）
#define		INT64U		unsigned long long
// 64位有符号整型别名：映射为有符号长长整型（8字节，超大数值存储）
#define		INT64S		signed long long
// OS栈类型别名：映射为INTOS类型（适配CPU最小整型单位，确保栈操作效率）
#define		OS_STK		INTOS

/********************************************************************************
*Macro definitions								*
********************************************************************************/
// 进入临界区宏：通过汇编指令SETC INTM置位中断屏蔽位，关闭所有可屏蔽中断
// 功能：保护临界代码段（如全局变量操作）不被中断打断，避免数据竞争
#define		OS_ENTER_CRITICAL()		asm("	SETC	INTM")

// 退出临界区宏：通过汇编指令CLRC INTM清零中断屏蔽位，恢复中断响应
// 功能：临界代码段执行完成后，重新启用中断，保证系统实时性
#define		OS_EXIT_CRITICAL()		asm("	CLRC	INTM")

// 任务切换宏：通过汇编指令TRAP #31触发31号软中断，执行任务切换逻辑
// 功能：主动触发操作系统的任务调度器，切换到最高优先级的就绪任务
#define		OS_TASK_SW()			asm("	TRAP	#31")


