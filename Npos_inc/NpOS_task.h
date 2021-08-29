#ifndef NPOS_TASK_H
#define NPOS_TASK_H
#include "stdint.h"


//任务状态
typedef enum {TASK_PEND=0,TASK_WAIT,TASK_READY,TASK_UNKNOWN}task_status;

//执行一些函数的返回情况
typedef enum {Exc_ERROR = 0,Exc_OK}task_funcsta;

typedef  void (*p_taskFunction)(void);


typedef struct tcb
{
    //任务当前的sp指针的地址值
    uint32_t pv_taskSp;

    //分配给任务的堆栈的本来的起始地址值
    uint32_t pv_taskStack;
    
    //任务堆栈大小
    uint32_t taskStackSize;//单位为 Byte

    //任务函数入口
    void (*p_taskFunction)(void);

    //任务优先级
    TASK_PRIORITY_TYPE taskPriority;
    
    //任务状态
    task_status taskStatus;

    //任务需要延时等待的时钟TICK数
    uint32_t taskDelayTick;
    
    //任务链表的下一个
    struct tcb* p_nextTcb;
    //任务链表的上一个
    struct tcb* p_lastTcb;

}Np_TCB;

typedef struct tcbb
{
    Np_TCB* taskNode;
}Np_tasklist;

typedef struct tcblist
{
    Np_tasklist taskReadyList[NPOS_TASK_PRIORITY_NUMBER];
    Np_TCB* taskPendList;
    TASK_PRIORITY_TYPE taskReadyflag;
}Np_tcblist;

extern void switch_to(Np_TCB* nexttcb);
extern void context_save();
extern void root_task_entry(Np_TCB* roottcb);

extern Np_tcblist g_TcbList;
extern Np_TCB* gp_currentTcb;

void NpOS_task_tcblistInit();

task_funcsta NpOS_task_createTask(Np_TCB* tcb,p_taskFunction taskfunc,TASK_PRIORITY_TYPE taskpri,void* stackbut,uint32_t stacksize,task_status taskstatus);
task_funcsta NpOS_task_deleteTask(Np_TCB* tcb);
task_funcsta NpOS_task_pendTask(Np_TCB* tcb);
task_funcsta NpOS_task_readyTask(Np_TCB* tcb);

void NpOS_task_pendDelayTicks(uint32_t ticks);
void NpOS_roottask();
void NpOS_Start();
void NpOS_task_startSchedul();

#endif // !NPOS_TASK_H