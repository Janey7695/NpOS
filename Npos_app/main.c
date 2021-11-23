#include <stdio.h>
#include "../Npos_inc/NpOS.h"

/****在这里定义任务堆栈大小（单位 ：字节Byte）******/

#define task_StackSize 512

/************************/


/****在这里创建任务堆栈实体******/

TASK_STACK_TYPE task_Stack[task_StackSize];

/************************/

/****在这里创建任务控制块******/

Np_TCB task_Tcb;

/************************/

/****在这里创建任务******/
void task(){
    while(1){

    }
}

void uart_irq_init(){
	/* write you init code here  */

}

int main(void)
{
	
	uart_irq_init();

    NpOS_task_tcblistInit();

    NpOS_task_createTask(
        &task_Tcb,
        task,
        7,
        task_Stack,
        task_StackSize,
        TASK_READY
        );

    NpOS_Start();

    while (1){

    }
}


