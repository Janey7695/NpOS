#ifndef NPOS_CONFIG_H
#define NPOS_CONFIG_H





/*           OS config            */

//系统心跳 每个tick的时间间隔
#define NPOS_SchedulingInterval_MS  5


/************* Task ********************/

// 单个任务最小需要的堆栈大小
#define NPOS_TASK_MIN_STACKSIZE     4*32

// 是否启用idle任务来统计cpu的利用率
#define NPOS_TASK_IDLE_USAGERATE_EN 0

// 允许的最多优先级数量
#define NPOS_TASK_PRIORITY_NUMBER   8

//优先级列表使用的数据类型
#define TASK_PRIORITY_TYPE  uint8_t
#define TASK_STACK_TYPE  uint8_t

#endif // !NPOS_CONFIG_H
