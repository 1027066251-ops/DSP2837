


//includes.h 是集中头文件包含文件，统一管理标准库、驱动、应用模块和内核头文件，方便嵌入式系统各模块的依赖管理和维护。


// 全局变量/函数作用域控制宏：根据OS_GLOBALS是否定义，决定OS_EXT为空白/extern
// 场景1：定义OS_GLOBALS（如在OS内核实现文件中）→ OS_EXT为空，变量/函数为全局定义
// 场景2：未定义OS_GLOBALS（如在头文件中）→ OS_EXT为extern，变量/函数为外部声明
#ifdef	OS_GLOBALS
#define	OS_EXT
#else
#define	OS_EXT	extern
#endif

// 包含OS CPU相关头文件（定义基础数据类型、临界区/任务切换宏等）
#include	"Kernel\Ports\OS_CPU.h"
// 包含OS配置头文件（定义栈大小、优先级、定时器节拍等核心配置）
#include	"Kernel\Ports\OS_CONFIG.h"

// OS内核初始化函数声明
// 功能：初始化OS内核的所有全局变量、任务控制块（TCB）、事件、定时器等，为OS启动做准备
OS_EXT	void	OSInit(void);

// OS启动函数声明
// 功能：启动操作系统调度器，切换到最高优先级的就绪任务，开始任务调度
OS_EXT	void	OSStart(void);

// 任务创建函数声明
// 参数1 task：指向任务函数的指针（任务入口地址）
// 参数2 stksize：任务栈大小（单位：OS_STK类型长度）
// 参数3 prio：任务优先级（数值越大优先级越低）
// 功能：创建一个新任务，分配栈空间，初始化TCB，将任务加入就绪表
OS_EXT	void	OSTaskCreate(void (*task)(void), INTOS stksize, INTOS prio);

// 事件发送函数声明（任务级）
// 参数1 prio：接收事件的任务优先级
// 参数2 EventId：事件编号（如定时器事件、自定义事件）
// 功能：在任务上下文向指定优先级的任务发送事件，触发任务就绪
OS_EXT	void	OSEventSend(INTOS prio,INTOS EventId);

// 事件发送函数声明（中断级）
// 参数1 prio：接收事件的任务优先级
// 参数2 EventId：事件编号
// 功能：在中断上下文向指定优先级的任务发送事件，避免中断与任务的竞态
OS_EXT	void	OSISREventSend(INTOS prio,INTOS EventId);

// 中断退出函数声明
// 功能：中断服务程序（ISR）执行完成后调用，触发中断级任务调度，切换到最高优先级就绪任务
OS_EXT	void	OSIntExit(void);

// 定时器节拍处理函数声明
// 功能：处理系统时钟节拍（Tick），更新任务定时器计数器，触发定时事件
OS_EXT	void	OSTimerTick(void);

// 任务栈初始化函数声明
// 参数1 task：指向任务函数的指针
// 参数2 ptos：指向任务栈顶的指针（需结合栈增长方向）
// 返回值：初始化后的栈顶指针（供任务切换使用）
// 功能：初始化任务栈，将任务入口地址、寄存器初始值压入栈，适配CPU上下文切换
OS_EXT  void	*OSTaskStkInit(void (*task)(void),OS_STK *ptos);

// 调度器函数声明
// 功能：遍历就绪任务表（OSRdyMap），找到最高优先级的就绪任务，准备上下文切换
OS_EXT  void	OSSched(void);

// 任务上下文切换函数声明（任务级）
// 功能：保存当前任务的CPU上下文（寄存器），恢复最高优先级就绪任务的上下文，完成任务切换
OS_EXT  void	OSCtxSw(void);

// 任务上下文切换函数声明（中断级）
// 功能：在中断上下文完成任务切换，逻辑与OSCtxSw一致，适配中断环境
OS_EXT  void	OSIntCtxSw(void);

// 定时器节拍中断服务函数声明
// 功能：系统定时器中断的入口函数，调用OSTimerTick处理节拍，触发调度
OS_EXT  void	OSTickISR(void);

// 事件掩码等待函数声明
// 参数 mask：事件掩码（二进制位表示待等待的事件）
// 返回值：满足掩码的事件编号（或组合）
// 功能：任务等待指定掩码的事件，无匹配事件时任务挂起
OS_EXT	TASK_EVENT	OSMaskEventPend(TASK_EVENT mask);

// 事件获取函数声明
// 返回值：当前任务待处理的事件编号
// 功能：获取任务TCB中存储的待处理事件，清除事件标记
OS_EXT	TASK_EVENT	OSEventGet(void);

/********************************************************************************
*Task ready control								*
*										*
*Description: 	These two macros control the ready bit of a task in OSRdyMap	*
*		OSTaskRdy insert a ready task into the OSRdyMap			*
*		OSTaskSuspend	delete a redy task from OSRdyMap		*
********************************************************************************/
// 任务就绪宏
// 参数 prio：任务优先级
// 功能：将OSRdyMap的第prio位设为1，标记该优先级任务为就绪状态，参与调度
#define		OSTaskRdy(prio)			OSRdyMap |= (1<<(prio))

// 任务挂起宏
// 参数 prio：任务优先级
// 功能：将OSRdyMap的第prio位设为0，标记该优先级任务为挂起状态，退出调度
#define		OSTaskSuspend(prio)		OSRdyMap &= ~(1<<(prio))

/********************************************************************************
*Constant for stack check							*
********************************************************************************/
// 条件编译块：仅当OS_STK_CHK_EN>0（启用栈检查）时生效
#if	OS_STK_CHK_EN > 0
// 栈满常量定义
// 含义：栈检查函数返回该值，表示任务栈已溢出/满
#define		OS_STK_FULL			0
#endif

/********************************************************************************
*Macro for CPU USEAGE CALCULATION						*
********************************************************************************/
// 条件编译块：仅当OS_TASK_IDLE_EN>0（启用空闲任务）时生效
#if	OS_TASK_IDLE_EN > 0
// CPU空闲时间获取宏
// 功能：返回OSCpuFree变量值，表征CPU空闲率（结合OS_CPUFREE_FACTOR计算）
#define		OSGetCPUTimeFree()		OSCpuFree
#endif

/********************************************************************************
*Macro for timer								*
********************************************************************************/
// 定时器启动宏
// 参数1 prio：任务优先级（关联任务TCB）
// 参数2 cnt：定时器初始计数值
// 参数3 period：定时器周期（0=单次定时，>0=周期定时）
// 功能：设置指定优先级任务的定时器计数器和周期，启动定时器
#define		OSTimerStart(prio,cnt,period)	OSTCBTbl[(prio)].TimerCnt=(cnt); OSTCBTbl[(prio)].TimerPeriod = (period)

// 定时器停止宏
// 参数 prio：任务优先级
// 功能：将指定优先级任务的定时器周期设为0，停止定时器
#define		OSTimerStop(prio)		OSTCBTbl[(prio)].TimerPeriod = 0

/********************************************************************************
*Macro for event								*
********************************************************************************/
// 事件等待宏（等待所有事件）
// 功能：调用OSMaskEventPend，传入全1掩码（~0），等待任意事件触发
#define		OSEventPend()			OSMaskEventPend(~0)

/********************************************************************************
*SYSTEM EVENT DEFINE								*
********************************************************************************/
// 系统事件编号：定时器事件
// 含义：定时器超时触发的系统内置事件编号，值为0
#define		OS_EVENTID_TIMER		0

/********************************************************************************
*CONSTANT FOR TIMER								*
********************************************************************************/
// 布尔常量：假（0）
#define		false				0
// 布尔常量：真（1）
#define		true				1

/********************************************************************************
*TASK CONTROL BLOCK								*
********************************************************************************/
// 任务控制块（TCB）结构体定义：存储单个任务的所有状态信息
typedef struct{
	OS_STK	*OSTCBStkPtr;			// 任务栈顶指针：指向当前任务栈的栈顶（上下文切换核心参数）

	// 条件编译：仅启用栈检查时包含栈底/栈大小字段
	#if	OS_STK_CHK_EN > 0
	OS_STK	*OSTCBStkBot;			// 任务栈底指针：指向任务栈的物理起始地址
	INTOS	OSTCBStkSize;			// 任务栈大小：栈的总容量（用于栈溢出检查）
	#endif

	INT16U	TimerPeriod;			// 定时器周期：0=停止，>0=周期定时（单位：Tick）
	INT16U	TimerCnt;				// 定时器计数器：当前剩余计数值，减到0触发定时事件

	TASK_EVENT	OSEvent;			// 任务待处理事件：存储当前未处理的事件编号
	TASK_EVENT	OSEventBitMask;		// 任务事件掩码：标记任务关注的事件类型
}OS_TCB;

/********************************************************************************
*TASK SCHED VARIABLES DEFINE							*
********************************************************************************/
// 就绪任务位图：每一位对应一个优先级，1=就绪，0=挂起（核心调度参数）
OS_EXT	INT16U	OSRdyMap;

// 任务控制块表：数组索引=任务优先级，存储所有任务的TCB（大小=OS_LOWEST_PRIO+1）
OS_EXT	OS_TCB	OSTCBTbl[OS_LOWEST_PRIO + 1];

// 最高优先级就绪任务：当前就绪任务中优先级最高的任务编号
OS_EXT	INTOS	OSPrioHighRdy;

// 当前运行任务优先级：正在执行的任务优先级
OS_EXT	INTOS	OSPrioCur;

// 最高优先级就绪任务TCB指针：指向OSPrioHighRdy对应的TCB
OS_EXT	OS_TCB	*OSTCBHighRdy;

// 当前运行任务TCB指针：指向OSPrioCur对应的TCB
OS_EXT	OS_TCB	*OSTCBCur;

// 【注释弃用】系统任务栈：原系统任务的栈数组（已注释，改用其他方式分配）
//OS_EXT	OS_STK	OSSystemStk[OS_MAX_STK_SIZE];

// 系统栈指针：全局栈指针，用于内核上下文切换
OS_EXT	OS_STK	*OSStkPtr;

/********************************************************************************
*CPU USAGE									*
********************************************************************************/
// 条件编译：仅启用空闲任务时包含CPU使用率相关变量
#if	OS_TASK_IDLE_EN > 0
OS_EXT	INT32U	OSIdleCtr;		// 空闲计数器：空闲任务运行时长累计值
OS_EXT	INT16U	OSTime;			// 系统时间：累计Tick数（单位：1/OS_TICKS_PER_SEC 秒）
OS_EXT	INTOS	OSCpuFree;		// CPU空闲率：计算后的CPU空闲百分比（或原始值）
#endif

/********************************************************************************
*SYSTEM FUNCTION LIST								*
********************************************************************************/
// 条件编译：仅启用栈检查时声明栈检查函数
#if		OS_STK_CHK_EN > 0
// 栈检查函数声明
// 参数 prio：任务优先级
// 返回值：栈使用情况（0=栈满/溢出，>0=剩余栈空间）
// 功能：检查指定优先级任务的栈使用情况，检测溢出
OS_EXT	INTOS	OSStkChk(INTOS	prio);
#endif


