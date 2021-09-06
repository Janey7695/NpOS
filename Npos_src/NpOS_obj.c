#include "../Npos_inc/NpOS.h"

/**
    \brief  向wait链表插入结点
    \param[in]  Np_TCB* tcb2wait 要插入的结点
    \retval task_funcsta 函数执行情况
*/
task_funcsta npos_insertIntoWaitList(Np_TCB* tcb2wait){
    Np_TCB* lp_waitNode;
    lp_waitNode = g_TcbList.taskWaitList;
    while(lp_waitNode->p_nextTcb!=NULL){
        lp_waitNode = lp_waitNode->p_nextTcb;
    }
    lp_waitNode->p_nextTcb = tcb2wait;
    tcb2wait->p_lastTcb = lp_waitNode;
    tcb2wait->p_nextTcb = NULL;
    
    return Exc_OK;
}

/**
    \brief  向wait链表拆离结点
    \param[in]  Np_TCB* tcbnode 要删除的结点
    \retval Np_TCB* 被删除的结点的tcb指针
*/
Np_TCB* npos_deleteFromWaitList(Np_TCB* tcbnode){
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


/**
    \brief  创建（初始化）一个消息的结构体
    \param[in]  Np_MSG* msg 要初始化的消息体
    \retval task_funcsta 函数执行情况
*/
task_funcsta NpOS_obj_createMsg(Np_MSG* msg){
    msg->messageStatu = MESSAGE_EMPTY;
    msg->p_message = NULL;
    LOG_OK("System","create message successfully");
    return Exc_OK;
}


/**
    \brief  接收一个来自指定任务的消息
            1.这个消息来自指定的任务
            2.如果没有收到消息，会挂起这个任务，使之成为等待态。
    \param[in]  Np_MSG* msg 承载消息的结构体
    \param[in]  Np_TCB* sendertcb 消息的发送者，用以核验消息的收发正确性
    \retval task_funcsta 函数执行情况
*/
task_funcsta NpOS_obj_receiveMsgFromTask(Np_MSG* msg,Np_TCB* sendertcb){
    NpOS_ENTER_CRITICAL();
    if(msg->messageStatu == MESSAGE_FULL && sendertcb == msg->senderTcb && msg->receiverTcb == gp_currentTcb){
        LOG_OK("System","receive a message from another task");
        msg->messageStatu = MESSAGE_EMPTY;
        NpOS_EXIT_CRITICAL();
        return Exc_OK;
    }

    LOG_WARING("System","waiting a message from another task");
    gp_currentTcb->taskStatus = TASK_WAIT;
    npos_task_deleteFromtaskReadyList(gp_currentTcb,gp_currentTcb->taskPriority);
    npos_insertIntoWaitList(gp_currentTcb);
    npos_task_clearTaskReadyFlag(gp_currentTcb);
    NpOS_EXIT_CRITICAL();
    NpOS_task_startSchedul();
    
    if(msg->messageStatu == MESSAGE_FULL && sendertcb == msg->senderTcb && msg->receiverTcb == gp_currentTcb){
        LOG_OK("System","receive a message from another task");
        msg->messageStatu = MESSAGE_EMPTY;
        NpOS_EXIT_CRITICAL();
        return Exc_OK;
    }

}


/**
    \brief  发送一个消息到指定任务中去
            1.这个消息会发送给指定的任务
            2.如果发送消息时，消息接收者正处在wait态，会将其恢复为就绪态，并进行一次任务调度
    \param[in]  Np_MSG* msg 承载消息的结构体
    \param[in]  Np_TCB* receivertcb 消息的接收者，用以核验消息的收发正确性
    \param[in]  void* p_msg 消息所携带的信息的指针
    \retval task_funcsta 函数执行情况
*/
task_funcsta NpOS_obj_sendMsgtoTask(Np_MSG* msg,Np_TCB* receivertcb,void* p_msg){
    NpOS_ENTER_CRITICAL();

    msg->p_message = p_msg;
    msg->messageStatu = MESSAGE_FULL;
    msg->receiverTcb = receivertcb;
    msg->senderTcb = gp_currentTcb;

    if(receivertcb->taskStatus == TASK_WAIT){
        npos_deleteFromWaitList(receivertcb);
        npos_task_insertIntotaskReadyList(receivertcb,receivertcb->taskPriority);
        npos_task_setTaskReadyFlag(receivertcb);
        LOG_OK("System","send a message to another task successfully");
        NpOS_EXIT_CRITICAL();
        NpOS_task_startSchedul();
        return Exc_OK;
    }
    NpOS_EXIT_CRITICAL();
    return Exc_OK;
}