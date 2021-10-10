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
    uint8_t taskPriority;
    
    //任务状态
    task_status taskStatus;

    //任务需要延时等待的时钟TICK数
    uint32_t taskDelayTick;
    
    //任务链表的下一个
    struct tcb* p_nextTcb;
    //任务链表的上一个
    struct tcb* p_lastTcb;

#if NPOS_TASK_USAGERATE_EN
    //任务总的运行时间数
    uint64_t taskRunTimeCount;
#endif

}Np_TCB;

typedef struct tcbb
{
    Np_TCB* taskNode;
}Np_tasklist;

typedef struct tcblist
{
    Np_tasklist taskReadyList[NPOS_TASK_PRIORITY_NUMBER];
    Np_TCB* taskPendList;

#if NPOS_OBJ_MESSAGE_EN
    Np_TCB* taskWaitList;
#endif

#if NPOS_TASK_PRIORITY_NUMBER == NPOS_TASK_PRIORITY_NUMBER_8
    TASK_PRIORITY_TYPE taskReadyflag;

#elif NPOS_TASK_PRIORITY_NUMBER <= NPOS_TASK_PRIORITY_NUMBER_64 && NPOS_TASK_PRIORITY_NUMBER > NPOS_TASK_PRIORITY_NUMBER_8
    TASK_PRIORITY_TYPE taskReadyflag2[NPOS_TASK_PRIORITY_NUMBER/8];
    TASK_PRIORITY_TYPE taskReadyflag1;

#elif NPOS_TASK_PRIORITY_NUMBER>=NPOS_TASK_PRIORITY_NUMBER_128
    TASK_PRIORITY_TYPE taskReadyflag3[NPOS_TASK_PRIORITY_NUMBER/8];
    TASK_PRIORITY_TYPE taskReadyflag2[NPOS_TASK_PRIORITY_NUMBER/64];
    TASK_PRIORITY_TYPE taskReadyflag1;
#endif

#if NPOS_TASK_TIMESLICE_SCHEDUL_EN
    uint8_t perTaskPriority;
#endif

}Np_tcblist;

extern void switch_to(Np_TCB* nexttcb);
extern void context_save();
extern void root_task_entry(Np_TCB* roottcb);



extern Np_tcblist g_TcbList;
extern Np_TCB* gp_currentTcb;
#if NPOS_ENTER_CIRTICAL_BY == NPOS_ENTER_CIRTICAL_BY_DISABLESCH
#define SchedulerClose 0
#define SchedulerOpen 1
extern uint8_t g_schedulerSwitch;
//进入临界区
#define NpOS_ENTER_CRITICAL()       g_schedulerSwitch = SchedulerClose
//退出临界区
#define NpOS_EXIT_CRITICAL()   g_schedulerSwitch = SchedulerOpen
#endif // 



void NpOS_roottask();
void NpOS_task_startSchedul();
void npos_task_setTaskReadyFlag(Np_TCB* tcb);
void npos_task_clearTaskReadyFlag(Np_TCB* tcb);
task_funcsta npos_task_insertIntotaskReadyList(
                        Np_TCB* _tcb,
                        TASK_PRIORITY_TYPE _taskpri
                        );
Np_TCB* npos_task_deleteFromtaskReadyList(
                        Np_TCB* _tcb,
                        TASK_PRIORITY_TYPE _taskpri
                        );

#endif // !NPOS_TASK_H