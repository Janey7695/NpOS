#ifndef NPOS_H
#define NPOS_H

#include "./NpOS_config.h"
#include "./NpOS_task.h"
#include "./NpOS_log.h"
#if NPOS_OBJ_MESSAGE_EN
#include "./NpOS_obj.h"
#endif


#define OS_NAME      "Not_Popular Real-Time Operate System"
#define OS_VERSION    "1.7.11"
#define OS_RELEASED   "2021-8-25"
#define OS_DESIGNED    "JaneyXu"


void NpOS_task_tcblistInit();
void NpOS_Start();
task_funcsta NpOS_task_createTask(
                    Np_TCB* tcb,
                    p_taskFunction taskfunc,
                    TASK_PRIORITY_TYPE taskpri,
                    void* stackbut,
                    uint32_t stacksize,
                    task_status taskstatus);
task_funcsta NpOS_task_deleteTask(Np_TCB* tcb);
task_funcsta NpOS_task_pendTask(Np_TCB* tcb);
task_funcsta NpOS_task_readyTask(Np_TCB* tcb);
void NpOS_task_pendDelayTicks(uint32_t ticks);


task_funcsta NpOS_obj_createMsg(Np_MSG* msg);
task_funcsta NpOS_obj_receiveMsgFromTask(Np_MSG* msg,Np_TCB* sendertcb);
task_funcsta NpOS_obj_sendMsgtoTask(Np_MSG* msg,Np_TCB* receivertcb,void* p_msg);

/*

变量命名规范建议：
    允许不同变量进行组合，如 pv_,lp_,lpv_,gp_等
    排列的顺序最好按照 ： 作用范围 - 变量类型 - 类型附属  如 ： 写 gpv_ 而不是 pvg_这样子 

    作用范围：
    g_ 该变量是一个全局变量。
    l_ 该变量是一个局部local变量  如：  void func1(){
                                            uint8_t l_value;
                                        }

    变量类型：
    p_ 该变量是一个指针 如： uint32_t* p_value;
    c_ 该变量是一个 const修饰的变量 如 ： const uint32_t c_dontChange;


    类型附属：
    v_ 该变量是某种类型的值 如 ： uint32_t* p_aPointer;
                                uint32_t pv_ThePointerValue;
                                    pv_ThePointerValue = (uint32_t)p_aPointer;
    o_ 该变量为只使用一次的变量 如 ： bool o_OnlyUseOnce=true;
                                    while(1){
                                        if(o_OnlyUseOnce){
                                            ...
                                            o_OnlyUseOnce = false;
                                        }
                                    }
*/

/*

task相关命名规范建议：
    任务堆栈大小的定义 ： #define taskName_StackSize  size
                    如： #define idleTask_StackSize  1024

    任务堆栈的声明  : TASK_STACK_TYPE taskName_Stack[taskName_StackSize];
                    如 ： TASK_STACK_TYPE task1_Stack[idleTask_StackSize];
    
    任务控制块声明 ： Np_TCB taskTcbName_Tcb;
                    如 ： Np_TCB task1_Tcb;

    任务函数声明 ： void taskName(void){
                        while(1){
                            ...
                        }
                    }
                    如：
                    void task1(void){
                        while(1){
                            ...
                        }
                    }
*/

/*

log报告规范建议：
        [systime]['tab'][who log]][log statu]:[ content ]
        如 [2.005]  [task][error]: couldn't create this task. 

        [systime] 自开启心跳后到打印log 运行的时间
        [who log] 是什么导致了这次log上报
        [log statu] 导致执行这次log的任务的运行情况
        [ content ] 详细原因
*/



#endif // !NPOS_H


