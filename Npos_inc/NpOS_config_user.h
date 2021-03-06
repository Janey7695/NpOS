#ifndef NPOS_CONFIG_USER_H
#define NPOS_CONFIG_USER_H
/*******************user can config 用户更改区********************/
/*           OS config            */

//系统心跳 每个tick的时间间隔
#define NPOS_CountPerMS         27000
#define NPOS_SchedulingInterval_MS  5

//是否启用系统信息打印
#define NPOS_logInfoPrintf_EN       1

//进入临界区的方式
#define NPOS_ENTER_CIRTICAL_BY_DISABLEIRQ   0   //通过关闭全局中断
#define NPOS_ENTER_CIRTICAL_BY_DISABLESCH   1   //通过关闭调度器
//设定进入临界区的方式
#define NPOS_ENTER_CIRTICAL_BY NPOS_ENTER_CIRTICAL_BY_DISABLESCH

/*           Task               */

// 是否启用任务运行时长统计
#define NPOS_TASK_USAGERATE_EN 1
// 是否启用idle任务来统计cpu的利用率
#define NPOS_TASK_CPUUSAGE_RATE_EN 1
// 是否启用同优先级时间片轮转调度
#define NPOS_TASK_TIMESLICE_SCHEDUL_EN 1

// 允许的最多优先级数量
#define NPOS_TASK_PRIORITY_NUMBER_8         8
#define NPOS_TASK_PRIORITY_NUMBER_16        16
#define NPOS_TASK_PRIORITY_NUMBER_32        32
#define NPOS_TASK_PRIORITY_NUMBER_64        64

//128往上的优先级存在bug 暂时不建议使用
#define NPOS_TASK_PRIORITY_NUMBER_128       128
#define NPOS_TASK_PRIORITY_NUMBER_256       256

//在这里填写对应数量级的宏
#define NPOS_TASK_PRIORITY_NUMBER   NPOS_TASK_PRIORITY_NUMBER_64

/*           Object               */
//是否使用消息功能
#define NPOS_OBJ_MESSAGE_EN         1




#endif // !NPOS_CONFIG_USER_H
