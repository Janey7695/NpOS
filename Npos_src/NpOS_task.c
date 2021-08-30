#include "../Npos_inc/NpOS.h"
#include "../Npos_cpu/systick.h"
#include "string.h"
#include "stdio.h"

Np_tcblist g_TcbList;
Np_TCB* gp_currentTcb;
Np_TCB* lp_circleTcb;
Np_TCB l_pendListRootNode;

//空闲任务相关
#define idleTask_StackSize  1024
TASK_STACK_TYPE idleTask_Stack[idleTask_StackSize];
Np_TCB idleTask_Tcb;
void idleTask(void);

/*****函数声明******/
void npos_task_lpendListInit();
void npos_task_gTcbListInit();
void npos_sp_init(
                Np_TCB* tcb,
                void* stackbut,
                uint32_t stacksize
                );
void npos_idletaskinit();
task_funcsta npos_task_insertIntotaskReadyList(
                        Np_TCB* _tcb,
                        TASK_PRIORITY_TYPE _taskpri
                        );
Np_TCB* npos_task_deleteFromtaskReadyList(
                        Np_TCB* _tcb,
                        TASK_PRIORITY_TYPE _taskpri
                        );
void NpOS_task_startSchedul();

void npos_task_setTaskReadyFlag(Np_TCB* tcb);
void npos_task_clearTaskReadyFlag(Np_TCB* tcb);

task_funcsta npos_insertIntoPendList();
Np_TCB* npos_deleteFromPendList(Np_TCB* tcbnode);
void npos_taskpendTick_dec();
void npos_get_highest_priority();
/******************/


//进入临界区
#define NpOS_ENTER_CRITICAL()       eclic_global_interrupt_disable()

//退出临界区
#define NpOS_EXIT_CRITICAL()    eclic_global_interrupt_enable()


const uint8_t c_taskPrioMask2Prio[256] = {
    0,0,1,1,2,2,2,2,3,3,3,3,3,3,3,3,
    4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,
    5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,
    5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,
    6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
    6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
    6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
    6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
    7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
    7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
    7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
    7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
    7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
    7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
    7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
    7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,

};


/**
    \brief  tcb list的初始化,并创建一个idle任务
    \param[in]  none
    \retval none
*/
void NpOS_task_tcblistInit(){

    npos_task_gTcbListInit();
    npos_task_lpendListInit();
    npos_idletaskinit();
}

/**
    \brief  create a task
    \param[in]  Np_TCB* tcb  任务所属的任务控制块的指针
    \param[in]  p_taskFunction taskfunc 任务的函数入口指针
    \param[in]  TASK_PRIORITY_TYPE taskpri  任务的优先级
    \param[in]  void* stackbut  任务的堆栈的栈底指针（即申请的数组的头指针）
    \param[in]  uint32_t stacksize  任务的堆栈大小（单位 Byte）
    \param[in]  task_status taskstatus 任务创建完后的初始状态
    \retval task_funcsta 返回任务的执行情况
*/
task_funcsta NpOS_task_createTask(
                        Np_TCB* tcb,
                        p_taskFunction taskfunc,
                        TASK_PRIORITY_TYPE taskpri,
                        void* stackbut,
                        uint32_t stacksize,
                        task_status taskstatus
                        )
{
    if(stacksize < NPOS_TASK_MIN_STACKSIZE)
    {
        LOG_ERR("system","The alloced stack is too small.");
        return Exc_ERROR;
    }
    tcb->pv_taskStack = (uint32_t)stackbut;

    tcb->taskStackSize = stacksize;

    tcb->taskStatus = taskstatus;

    tcb->p_taskFunction = taskfunc;
    
    tcb->p_nextTcb = NULL;

    if(taskstatus == TASK_READY)
    {
        tcb->taskPriority = taskpri;

        npos_task_setTaskReadyFlag(tcb);
    }
    else
    {
        tcb->taskPriority = taskpri;
    }
    
    npos_sp_init(tcb,stackbut,stacksize);

    if(!npos_task_insertIntotaskReadyList(tcb,taskpri))
        return Exc_ERROR;

    LOG_OK("system","task create successfully");
    return Exc_OK;

}

/**
    \brief  delete a task
    \param[in]  Np_TCB* tcb  任务所属的任务控制块的指针
    \retval task_funcsta 返回函数的执行情况
*/
task_funcsta NpOS_task_deleteTask(Np_TCB* tcb){
    NpOS_ENTER_CRITICAL();
    if(tcb == g_TcbList.taskReadyList[0].taskNode){
        LOG_ERR("system","Sorry you couldn't delete idle task");
        NpOS_EXIT_CRITICAL();
        return Exc_ERROR;
    }
    if(tcb->taskStatus == TASK_PEND){
        npos_deleteFromPendList(tcb);
    }
    else{
        npos_task_deleteFromtaskReadyList(tcb,tcb->taskPriority);
    }
    
    tcb->taskStatus = TASK_UNKNOWN;
    npos_task_clearTaskReadyFlag(tcb);
    LOG_OK("system","task delete successfully");
    NpOS_EXIT_CRITICAL();
    NpOS_task_startSchedul();
    return Exc_OK;
}

/**
    \brief  pend a task
            
            若使用该函数挂起一个正在延时pend的任务，重新ready这个任务后会刷新其delayTicks
    \param[in]  Np_TCB* tcb  任务所属的任务控制块的指针
    \retval task_funcsta 返回任务的执行情况
*/
task_funcsta NpOS_task_pendTask(Np_TCB* tcb){
    NpOS_ENTER_CRITICAL();
    if(tcb == g_TcbList.taskReadyList[0].taskNode){
        LOG_ERR("system","Sorry you couldn't pend idle task");
        NpOS_EXIT_CRITICAL();
        return Exc_ERROR;
    }

    if(tcb->taskStatus == TASK_UNKNOWN){
        LOG_ERR("system","this task's statu is unknown,couldn't been set to pend");
        NpOS_EXIT_CRITICAL();
        return Exc_ERROR;
    }

    if(tcb->taskStatus == TASK_PEND){
        LOG_WARING("system","this task has been Pend statu");

        npos_deleteFromPendList(tcb);
    }
    npos_task_deleteFromtaskReadyList(tcb,tcb->taskPriority);

    tcb->taskStatus = TASK_PEND;
    npos_task_clearTaskReadyFlag(tcb);
    LOG_OK("system","task pend successfully");
    NpOS_EXIT_CRITICAL();
    NpOS_task_startSchedul();
    return Exc_OK;
}


/**
    \brief  ready a task
            BUG:对正在pend延时的任务使用这个函数会导致意料之外的错误
    \param[in]  Np_TCB* tcb  任务所属的任务控制块的指针
    \retval task_funcsta 返回任务的执行情况
*/
task_funcsta NpOS_task_readyTask(Np_TCB* tcb){
    NpOS_ENTER_CRITICAL();
    if(tcb->taskStatus == TASK_READY){
        LOG_ERR("system","this task has been ready");
        NpOS_EXIT_CRITICAL();
        return Exc_ERROR;
    }
    if(tcb->taskStatus == TASK_UNKNOWN){
        LOG_ERR("system","this task's statu is unknown,couldn't been set to ready");
        NpOS_EXIT_CRITICAL();
        return Exc_ERROR;
    }

    if(tcb->taskStatus == TASK_PEND){
        npos_deleteFromPendList(tcb);
    }
    npos_task_insertIntotaskReadyList(tcb,tcb->taskPriority);
    tcb->taskDelayTick = 0;
    tcb->taskStatus = TASK_READY;
    npos_task_setTaskReadyFlag(tcb);
    LOG_OK("system","task is set to be ready");
    NpOS_EXIT_CRITICAL();
    NpOS_task_startSchedul();
    return Exc_OK;
}

/**
    \brief  主动开始一次任务调度
            用以当前任务主动放弃cpu的使用权
    \param[in]  none
    \retval none
*/
void NpOS_task_startSchedul(){
    TIMER_WRITE_REG(TIMER_MSIP) = 1;
}

/**
    \brief  os延时函数 用以将当前任务挂起Ticks个时间
    \param[in]  uint32_t ticks  任务需要等待的tick数 每个ticks的持续时间由Npos_config.h中的 NPOS_SchedulingInterval_MS 决定
    \retval none
*/
void NpOS_task_pendDelayTicks(uint32_t ticks){

    NpOS_ENTER_CRITICAL();

    gp_currentTcb->taskDelayTick = ticks;
    gp_currentTcb->taskStatus = TASK_PEND;

    npos_task_clearTaskReadyFlag(gp_currentTcb);

    npos_task_deleteFromtaskReadyList(gp_currentTcb,gp_currentTcb->taskPriority);

    npos_insertIntoPendList();

    NpOS_EXIT_CRITICAL();

    NpOS_task_startSchedul();
}

/**
    \brief  任务调度函数
    \param[in]  none
    \retval none
*/
void NpOS_task_schedul(){
    
    //获取下一个优先级最高的任务tcb
    npos_get_highest_priority();

    //恢复上下文
    switch_to(gp_currentTcb);
}

/**
    \brief  OS入口 
                调用此函数后cpu的使用权与控制器正式交给os
    \param[in]  none
    \retval none
*/
void NpOS_Start(){
    gp_currentTcb = g_TcbList.taskReadyList[0].taskNode;
    System_tickInit();
    root_task_entry(gp_currentTcb);
}


/**
    \brief  系统心跳中断服务函数
    \param[in]  none
    \retval none
*/
void eclic_mtip_handler(){

    context_save();

    g_npos_systime_Ticks += 1;
    npos_taskpendTick_dec();
    
    NpOS_task_schedul();

    TIMER_WRITE_REG(TIMER_MTIME) = 0x0;
    // TIMER_WRITE_REG(TIMER_MTIME+4) = 0x0;

}

/**
    \brief  msip中断服务函数
    \param[in]  none
    \retval none
*/
void eclic_msip_handler(){
    context_save();
    NpOS_task_schedul();
    TIMER_WRITE_REG(TIMER_MSIP) = 0;
}

/**
    \brief  系统空闲任务 避免系统无事可做
    \param[in]  none
    \retval none
*/
void idleTask(){
    #if NPOS_TASK_IDLE_USAGERATE_EN

    uint8_t lo_iftheFirstTimeCountExc;
    lo_iftheFirstTimeCountExc = 1;
    uint32_t l_excTimes,l_SecStartTime,l_excTimesPerSec;
    l_excTimes = 0;
    l_excTimesPerSec=0xffffffff;
    l_SecStartTime = get_sys_ticks()*NPOS_SchedulingInterval_MS;
    uint8_t l_usageRate;

    #endif

    while(1){
        #if NPOS_TASK_IDLE_USAGERATE_EN
        
        l_excTimes += 1;

        if(NPOS_SchedulingInterval_MS*get_sys_ticks() - l_SecStartTime > 1000)
        {
            
            if(lo_iftheFirstTimeCountExc){
                NpOS_ENTER_CRITICAL();
                l_excTimesPerSec = l_excTimes;
                lo_iftheFirstTimeCountExc = 0;
                NpOS_EXIT_CRITICAL();
            }
            
            NpOS_ENTER_CRITICAL();
            printf("l_excTimes is %d,l_excTimesPerSec is %d",l_excTimes,l_excTimesPerSec);
            l_usageRate = 100-l_excTimes*100/l_excTimesPerSec;
            l_SecStartTime = NPOS_SchedulingInterval_MS*(get_sys_ticks()+1);
            l_excTimes = 0;
            printf("[%ld]  [cpu][normal]: Now CPU usage is %.d %%. \n",NPOS_SchedulingInterval_MS*get_sys_ticks(),l_usageRate);
            NpOS_EXIT_CRITICAL();
            TIMER_WRITE_REG(TIMER_MTIMECMP) = 27000*NPOS_SchedulingInterval_MS;
        }

        #endif
        LOG_INFO("idle task","idle task run ...");
    }

}

/**
    \brief  在readylist中设置当前任务的就绪标志
    \param[in]  Np_TCB* tcb 要设置就绪标志的tcb
    \retval none
*/
void npos_task_setTaskReadyFlag(Np_TCB* tcb){
    g_TcbList.taskReadyflag |= (0x1 << tcb->taskPriority);
}

/**
    \brief  在readylist中清除当前任务的就绪标志
    \param[in]  Np_TCB* tcb 要清除就绪标志的tcb
    \retval none
*/
void npos_task_clearTaskReadyFlag(Np_TCB* tcb){
    g_TcbList.taskReadyflag &= ~( 0x1 << (tcb->taskPriority));
}

/**
    \brief  pendlist的初始化
    \param[in]  none
    \retval none
*/
void npos_task_lpendListInit(){
    l_pendListRootNode.p_nextTcb = NULL;
    l_pendListRootNode.p_lastTcb = &l_pendListRootNode;
}

/**
    \brief  tcblist初始化的核心步骤
    \param[in]  none
    \retval none
*/
void npos_task_gTcbListInit(){
    for(int i = 0;i<NPOS_TASK_PRIORITY_NUMBER;i++){
        g_TcbList.taskReadyList[i].taskNode = NULL;
    }
    g_TcbList.taskPendList = &l_pendListRootNode;
    g_TcbList.taskReadyflag = 0;
}

/**
    \brief  空闲任务的创建
    \param[in]  none
    \retval none
*/
void npos_idletaskinit(){
    NpOS_task_createTask(&idleTask_Tcb,idleTask,0,idleTask_Stack,idleTask_StackSize,TASK_READY);
}


/**
    \brief  将任务插入任务列表中
    \param[in]  Np_TCB* tcb 需要初始化的tcb的指针
    \param[in]  TASK_PRIORITY_TYPE _taskpri 任务优先级
    \retval task_funcsta 返回任务执行状态代码
*/
task_funcsta npos_task_insertIntotaskReadyList(
                        Np_TCB* _tcb,
                        TASK_PRIORITY_TYPE _taskpri
                        ){

    if(g_TcbList.taskReadyList[TASK_SYSTEMKEEP_LOWEST_PRIORITY].taskNode == NULL){
        g_TcbList.taskReadyList[TASK_SYSTEMKEEP_LOWEST_PRIORITY].taskNode = _tcb;
        _tcb->p_nextTcb = g_TcbList.taskReadyList[TASK_SYSTEMKEEP_LOWEST_PRIORITY].taskNode;
    }
    else{
        if(g_TcbList.taskReadyList[_taskpri].taskNode==NULL){
            g_TcbList.taskReadyList[_taskpri].taskNode = _tcb;
            _tcb->p_nextTcb = NULL;
            _tcb->p_lastTcb = _tcb;
        }

        Np_TCB* lp_perNdoe;
        lp_perNdoe = g_TcbList.taskReadyList[_taskpri].taskNode;
        while(lp_perNdoe->p_nextTcb != NULL){
            lp_perNdoe = lp_perNdoe->p_nextTcb;
        }
        lp_perNdoe->p_nextTcb = _tcb;
        _tcb->p_lastTcb = lp_perNdoe;
        _tcb->p_nextTcb = NULL;
        
    }
    return Exc_OK;
}

/**
    \brief  从就绪列表中删除一个结点
    \param[in]  Np_TCB* tcb 需要初始化的tcb的指针
    \param[in]  TASK_PRIORITY_TYPE _taskpri 任务优先级
    \retval Np_TCB* 返回删除的结点的指针
*/
Np_TCB* npos_task_deleteFromtaskReadyList(
                        Np_TCB* _tcb,
                        TASK_PRIORITY_TYPE _taskpri
                        ){
    Np_TCB* lp_perNdoe;
    lp_perNdoe = g_TcbList.taskReadyList[_taskpri].taskNode;
    if(lp_perNdoe == _tcb){
        lp_perNdoe = lp_perNdoe->p_nextTcb;
        if(lp_perNdoe!=NULL)    lp_perNdoe->p_lastTcb = lp_perNdoe;
        _tcb->p_nextTcb = NULL;
        return _tcb;
    }

    while(lp_perNdoe->p_nextTcb != _tcb){
        lp_perNdoe = lp_perNdoe->p_nextTcb;
    }
    lp_perNdoe->p_nextTcb = _tcb->p_nextTcb;
    if(lp_perNdoe->p_nextTcb != NULL)   lp_perNdoe->p_nextTcb->p_lastTcb = lp_perNdoe;
    _tcb->p_nextTcb = NULL;
    _tcb->p_lastTcb = NULL;
    return _tcb;
}

/**
    \brief  任务堆栈的初始化
                完成对任务上下文的初始化 对sp,mpec,msubm,macuse赋初值
    \param[in]  Np_TCB* tcb 需要初始化的tcb的指针
    \retval none
*/
void npos_sp_init(
                Np_TCB* tcb,
                void* stackbut,
                uint32_t stacksize
                ){

    memset(stackbut,0,sizeof(uint8_t)*stacksize);
    tcb->pv_taskSp = (uint32_t)stackbut + stacksize;

    tcb->pv_taskSp -= 32*4;

    *(uint32_t*)(tcb->pv_taskSp + 108 ) = (uint32_t)tcb->pv_taskSp;
    
    //初始化 mcause 
    *(uint32_t*)(tcb->pv_taskSp + 116 ) = (uint32_t)0xb8000007;
    //初始化 mpec 和 ra
    *(uint32_t*)(tcb->pv_taskSp + 112 ) = (uint32_t)tcb->p_taskFunction;
    *(uint32_t*)(tcb->pv_taskSp + 120 ) = (uint32_t)tcb->p_taskFunction;
    //初始化 msubm
    *(uint32_t*)(tcb->pv_taskSp + 124 ) = (uint32_t)0x40;

    //人为模拟堆栈在进入eclic_mtip_handler时 被自动申请的16个Byte
    tcb->pv_taskSp -= 16;
    
}



/**
    \brief  获取当前readylist中优先级最高的任务
    \param[in]  none
    \retval none
*/
void npos_get_highest_priority(){
    NpOS_ENTER_CRITICAL();
    TASK_PRIORITY_TYPE l_highestPri =0;
    Np_TCB* lp_nexttcb;
    l_highestPri = c_taskPrioMask2Prio[g_TcbList.taskReadyflag];
    lp_nexttcb = g_TcbList.taskReadyList[l_highestPri].taskNode;
    if(gp_currentTcb == lp_nexttcb) return;
    else gp_currentTcb = lp_nexttcb;
    NpOS_EXIT_CRITICAL();
}



/**
    \brief  用以减少pendlist中的任务的ticks数
            当ticks减为0时，任务完成延时，从pend态变回ready态
    \param[in]  none
    \retval none
*/
void npos_taskpendTick_dec(){
    Np_TCB* lp_pendNode;
    Np_TCB* lp_pendOverNode;
    lp_pendNode = g_TcbList.taskPendList;

    while(lp_pendNode->p_nextTcb!=NULL){
        lp_pendNode->p_nextTcb->taskDelayTick-=1;

        if(lp_pendNode->p_nextTcb->taskDelayTick == 0){
            lp_pendOverNode = npos_deleteFromPendList(lp_pendNode->p_nextTcb);

            npos_task_insertIntotaskReadyList(lp_pendOverNode,lp_pendOverNode->taskPriority);

            lp_pendOverNode->taskStatus = TASK_READY;

            npos_task_setTaskReadyFlag(lp_pendOverNode);

        }
        else
        {
            lp_pendNode = lp_pendNode->p_nextTcb;
        }

        
    }
}

/**
    \brief  插入结点到pendlist
    \param[in]  none
    \retval task_funcsta 返回函数执行情况
*/
task_funcsta npos_insertIntoPendList(){
    Np_TCB* lp_pendNode;
    lp_pendNode = g_TcbList.taskPendList;
    while(lp_pendNode->p_nextTcb!=NULL){
        lp_pendNode = lp_pendNode->p_nextTcb;
    }
    lp_pendNode->p_nextTcb = gp_currentTcb;
    gp_currentTcb->p_lastTcb = lp_pendNode;
    gp_currentTcb->p_nextTcb = NULL;
    
    return Exc_OK;
}

/**
    \brief  删除pendlist节点函数
    \param[in]  Np_TCB* tcbnode 要删除的结点
    \retval Np_TCB* 被删除的结点指针
*/
Np_TCB* npos_deleteFromPendList(Np_TCB* tcbnode){
    Np_TCB* lp_lastNode;
    if(tcbnode->p_lastTcb!=NULL){
        lp_lastNode = tcbnode->p_lastTcb;
        lp_lastNode->p_nextTcb = tcbnode->p_nextTcb;
        

        if(tcbnode->p_nextTcb!=NULL){
            tcbnode->p_nextTcb->p_lastTcb = lp_lastNode;
        }

        tcbnode->p_lastTcb = NULL;
        tcbnode->p_nextTcb = NULL;
    }

    return tcbnode;

}

