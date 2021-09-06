#ifndef NPOS_CONFIG_H
#define NPOS_CONFIG_H
/*******************user can config 用户更改区********************/
/*           OS config            */

//系统心跳 每个tick的时间间隔
#define NPOS_SchedulingInterval_MS  5

//是否启用系统信息打印
#define NPOS_logInfoPrintf_EN       1

/*           Task               */

// 是否启用idle任务来统计cpu的利用率
#define NPOS_TASK_IDLE_USAGERATE_EN 0

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
#define NPOS_OBJ_MESSAGE_EN         0


/*******************system config 非用户更改区********************/

/*           Task               */

// 单个任务最小需要的堆栈大小
#define NPOS_TASK_MIN_STACKSIZE     4*32+16

//优先级列表使用的数据类型
#define TASK_PRIORITY_TYPE  uint8_t

//任务堆栈使用的数据类型
#define TASK_STACK_TYPE __attribute__ ((aligned (8))) uint8_t

//系统保留的最小优先级
#define TASK_SYSTEMKEEP_LOWEST_PRIORITY     0
//可供用户使用的最小优先级
#define TASK_USER_LOWEST_PRIORITY       TASK_SYSTEMKEEP_LOWEST_PRIORITY+1

#endif // !NPOS_CONFIG_H
