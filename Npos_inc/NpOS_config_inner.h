#ifndef NPOS_CONFIG_INNER_H
#define NPOS_CONFIG_INNER_H

#include "./NpOS_config_user.h"
/*******************system config 非用户更改区********************/

/*           Task               */

// 单个任务最小需要的堆栈大小
#define NPOS_TASK_MIN_STACKSIZE     140

//优先级列表使用的数据类型
#define TASK_PRIORITY_TYPE  uint8_t

//任务堆栈使用的数据类型
#define TASK_STACK_TYPE __attribute__ ((aligned (8))) uint8_t

//系统保留的最小优先级
#define TASK_SYSTEMKEEP_LOWEST_PRIORITY     0

//系统保留的最大优先级
#define TASK_SYSTEMKEEP_HIGHEST_PRIORITY     NPOS_TASK_PRIORITY_NUMBER-1

//可供用户使用的最小优先级
#define TASK_USER_LOWEST_PRIORITY       TASK_SYSTEMKEEP_LOWEST_PRIORITY+1
//可供用户使用的最高优先级
#define TASK_USER_HIGHEST_PRIORITY       TASK_SYSTEMKEEP_HIGHEST_PRIORITY-1
#endif // !NPOS_CONFIG_INNER_H
