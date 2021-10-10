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



#endif // !NPOS_OBJ_H
