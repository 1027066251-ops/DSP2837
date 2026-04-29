


//Main.c = 系统主程序入口文件




// 定义主函数全局标识宏，用于标识该文件为主程序入口，管理全局变量/函数的链接属性
#define  MAIN_GLOBALS

// 引入应用层公共头文件，包含基础数据类型、宏定义、全局函数声明、硬件寄存器定义等核心内容
#include "App\include\head.h"
// 引入主程序专属头文件，包含main函数中调用的任务函数、优先级、定时器等相关声明
#include "App\include\main.h"

/**
 * @brief 程序入口主函数
 * @note 嵌入式系统中main函数为系统启动后的第一个用户级函数，负责初始化系统并启动操作系统
 */


 //各子任务  实时操作系统(RTOS)基于优先级和时间片进行抢占式调度。

void main(void)
{



    // 初始化CPU核心：包含时钟配置、外设时钟使能、GPIO初始化、中断向量表配置等底层硬件初始化
    sInitCpu();

    // 初始化嵌入式操作系统(OS)内核：初始化任务控制块、定时器、就绪列表、事件标志等OS核心数据结构
    OSInit();

    // 创建MPPT(最大功率点跟踪)任务
    // 参数说明：
    // sMpptTask - 任务函数名（光伏/储能系统中负责追踪最大功率输出的核心任务）
    // 200       - 任务栈大小（单位：字节/字，根据系统定义）
    // cPrioFirst - 任务优先级（cPrioFirst为宏定义，代表第一优先级，数值越小优先级越高）
    OSTaskCreate(sMpptTask,200,cPrioFirst);
    // 启动MPPT任务定时器
    // 参数说明：
    // cPrioFirst - 关联的任务优先级
    // 0          - 定时器初始延时（0表示立即启动）
    // cT10ms     - 定时器周期（宏定义，代表10毫秒，即该任务每10ms执行一次）
    OSTimerStart(cPrioFirst,0,cT10ms);

    // 创建模式管理任务
    // 参数说明：
    // sModeTask   - 任务函数名（负责系统运行模式切换，如并网/离网、待机/运行、故障模式等）
    // 200         - 任务栈大小
    // cPrioSecond - 任务优先级（第二优先级）
    OSTaskCreate(sModeTask,200,cPrioSecond);
    // 启动模式任务定时器
    // 参数说明：
    // cPrioSecond - 关联的任务优先级
    // 4           - 初始延时（单位：系统时基，根据定时器配置，此处为延时后启动）
    // cT20ms      - 定时器周期（宏定义，代表20毫秒，任务每20ms执行一次）
    OSTimerStart(cPrioSecond,4,cT20ms);

    // 创建输出控制任务
    // 参数说明：
    // sOutputTask - 任务函数名（负责逆变器PWM输出、电压/电流闭环控制、功率调节等核心控制逻辑）
    // 200         - 任务栈大小
    // cPrioThird  - 任务优先级（第三优先级）
    OSTaskCreate(sOutputTask,200,cPrioThird);
    // 启动输出控制任务定时器
    // 参数说明：
    // cPrioThird - 关联的任务优先级
    // 2          - 初始延时
    // cT20ms     - 定时器周期（20毫秒，任务每20ms执行一次）
    OSTimerStart(cPrioThird,2,cT20ms);

    // 创建母线保护任务
    // 参数说明：
    // sBusProtTask - 任务函数名（负责母线电压/电流保护、过压/欠压/过流检测、故障处理等）
    // 200          - 任务栈大小
    // cPrioForth   - 任务优先级（第四优先级）
    OSTaskCreate(sBusProtTask,200,cPrioForth);
    // 启动母线保护任务定时器
    // 参数说明：
    // cPrioForth - 关联的任务优先级
    // 3          - 初始延时
    // cT10ms     - 定时器周期（10毫秒，任务每10ms执行一次）
    OSTimerStart(cPrioForth,3,cT10ms);

    // 创建接口管理任务
    // 参数说明：
    // sInterfaceTask - 任务函数名（负责外部接口管理，如通信接口、IO接口、传感器接口数据采集/解析等）
    // 200            - 任务栈大小
    // cPrioFifth     - 任务优先级（第五优先级）
    OSTaskCreate(sInterfaceTask,200,cPrioFifth);
    // 启动接口管理任务定时器
    // 参数说明：
    // cPrioFifth - 关联的任务优先级
    // 1          - 初始延时
    // cT1ms      - 定时器周期（宏定义，代表1毫秒，任务每1ms执行一次，高频接口数据处理）
    OSTimerStart(cPrioFifth,1,cT1ms);

    // 创建SCI(串行通信接口)任务
    // 参数说明：
    // sSciTask    - 任务函数名（负责SCI串口通信，如与上位机/触摸屏/外设的数据交互、指令解析/响应等）
    // 200         - 任务栈大小
    // cPrioSixth  - 任务优先级（第六优先级）
    OSTaskCreate(sSciTask,200,cPrioSixth);
    // 启动SCI通信任务定时器
    // 参数说明：
    // cPrioSixth - 关联的任务优先级
    // 5          - 初始延时
    // cT100ms    - 定时器周期（宏定义，代表100毫秒，任务每100ms执行一次，低频通信处理）
    OSTimerStart(cPrioSixth,5,cT100ms);

    // 注释：启动操作系统内核
    //Start OS
    // 启动OS内核，开始任务调度：OS将根据任务优先级和定时器配置，循环调度各任务执行
    OSStart();
}



