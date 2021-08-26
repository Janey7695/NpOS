#include "gd32vf103.h"
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

//默认usart0开启
void usart0_config(void);

/*!
    \brief      main function
    \param[in]  none
    \param[out] none
    \retval     none
*/
int main(void)
{
    usart0_config();
    eclic_global_interrupt_enable();
    eclic_set_nlbits(ECLIC_GROUP_LEVEL3_PRIO1);
    NpOS_task_tcblistInit();

    NpOS_task_createTask(
        &task_Tcb,task,
        7,
        task_Stack,
        task_StackSize,
        TASK_READY
        );

    NpOS_Start();

    while (1){

    }
}

//串口初始化
void usart0_config(void)
{
    /* enable USART clock */
    rcu_periph_clock_enable(RCU_USART0);
    
#if defined USART0_REMAP
    /* enable GPIO clock */
    rcu_periph_clock_enable(RCU_GPIOB);
    rcu_periph_clock_enable(RCU_AF);
    /* USART0 remapping */
    gpio_pin_remap_config(GPIO_USART0_REMAP, ENABLE);
    /* connect port to USARTx_Tx */
    gpio_init(GPIOB, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_6);
    /* connect port to USARTx_Rx */
    gpio_init(GPIOB, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_7);
#else
    /* enable GPIO clock */
    rcu_periph_clock_enable(RCU_GPIOA);
        
    /* connect port to USARTx_Tx */
    gpio_init(GPIOA, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_9);
    /* connect port to USARTx_Rx */
    gpio_init(GPIOA, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_10);
#endif

    /* USART configure */
    usart_deinit(USART0);
    usart_baudrate_set(USART0, 115200U);
    usart_word_length_set(USART0, USART_WL_8BIT);
    usart_stop_bit_set(USART0, USART_STB_1BIT);
    usart_parity_config(USART0, USART_PM_NONE);
    usart_hardware_flow_rts_config(USART0, USART_RTS_DISABLE);
    usart_hardware_flow_cts_config(USART0, USART_CTS_DISABLE);
    usart_receive_config(USART0, USART_RECEIVE_ENABLE);
    usart_transmit_config(USART0, USART_TRANSMIT_ENABLE);
    usart_enable(USART0);
}

/* retarget the C library printf function to the USART */
int _put_char(int ch)
{
    usart_data_transmit(USART0, (uint8_t) ch );
    while ( usart_flag_get(USART0, USART_FLAG_TBE)== RESET){
    }

    return ch;
}