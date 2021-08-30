#ifndef NPOS_CONFIG_H
#define NPOS_CONFIG_H


/*           OS config            */

//系统心跳 每个tick的时间间隔
#define NPOS_SchedulingInterval_MS  5

//是否启用系统信息打印
#define NPOS_logInfoPrintf_EN       0


/************* Task ********************/

// 单个任务最小需要的堆栈大小
#define NPOS_TASK_MIN_STACKSIZE     4*32+16

// 是否启用idle任务来统计cpu的利用率
#define NPOS_TASK_IDLE_USAGERATE_EN 0

// 允许的最多优先级数量
#define NPOS_TASK_PRIORITY_NUMBER   8

//优先级列表使用的数据类型
#define TASK_PRIORITY_TYPE  uint8_t

//任务堆栈使用的数据类型
#define TASK_STACK_TYPE __attribute__ ((aligned (8))) uint8_t

//系统保留的最小优先级
#define TASK_SYSTEMKEEP_LOWEST_PRIORITY     0
//可供用户使用的最小优先级
#define TASK_USER_LOWEST_PRIORITY       TASK_SYSTEMKEEP_LOWEST_PRIORITY+1

#endif // !NPOS_CONFIG_H
