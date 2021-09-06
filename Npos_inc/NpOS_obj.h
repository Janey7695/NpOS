#ifndef NPOS_OBJ_H
#define NPOS_OBJ_H
#include <stdint.h>

typedef enum {MESSAGE_EMPTY=0,MESSAGE_FULL}msg_status;


typedef struct msg
{
    msg_status messageStatu;
    void* p_message;
    Np_TCB* senderTcb;
    Np_TCB* receiverTcb;
}Np_MSG;

task_funcsta NpOS_obj_createMsg(Np_MSG* msg);
task_funcsta NpOS_obj_receiveMsgFromTask(Np_MSG* msg,Np_TCB* sendertcb);
task_funcsta NpOS_obj_sendMsgtoTask(Np_MSG* msg,Np_TCB* receivertcb,void* p_msg);

#endif // !NPOS_OBJ_H
