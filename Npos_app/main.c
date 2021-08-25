#include "gd32vf103.h"
#include <stdio.h>
// #include "systick.h"
#include "../Npos_inc/NpOS.h"

/* configure the GPIO ports */
void gpio_config(void);
void usart0_config(void);

#define task1_StackSize 2048
#define task2_StackSize 1024
TASK_STACK_TYPE task1_Stack[task1_StackSize];
TASK_STACK_TYPE task2_Stack[task2_StackSize];
Np_TCB task1_Tcb;
Np_TCB task2_Tcb;

/**
    \brief      configure the GPIO ports
    \param[in]  none
    \param[out] none
    \retval     none
  */
void gpio_config(void)
{
    rcu_periph_clock_enable(RCU_GPIOA);
    gpio_init(GPIOA, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_1 | GPIO_PIN_2);
    gpio_bit_set(GPIOA, GPIO_PIN_1 | GPIO_PIN_2);
}

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


void task1(){
uint32_t haha;
    while(1){
    haha+=1;
    eclic_global_interrupt_disable();
    LOG_INFO("task1","task1 run ...");
    printf("task1 run ,haha is %d ...\n",haha);
    eclic_global_interrupt_enable();
    gpio_bit_reset(GPIOA, GPIO_PIN_1);
    NpOS_task_pendDelayTicks(100);
    gpio_bit_set(GPIOA, GPIO_PIN_1);
    NpOS_task_pendDelayTicks(100);
    }
}

void task2(){
    while(1){
        
        LOG_INFO("task2","task2 run ...");
        NpOS_task_pendDelayTicks(75);
        gpio_bit_reset(GPIOA, GPIO_PIN_2);
        NpOS_task_pendDelayTicks(75);
        gpio_bit_set(GPIOA, GPIO_PIN_2);
    }
}

void task_over(){
    gpio_bit_reset(GPIOA, GPIO_PIN_1 | GPIO_PIN_2);
}


/*!
    \brief      main function
    \param[in]  none
    \param[out] none
    \retval     none
*/
int main(void)
{
    gpio_config();
    usart0_config();
    eclic_global_interrupt_enable();
    eclic_set_nlbits(ECLIC_GROUP_LEVEL3_PRIO1);

    NpOS_task_tcblistInit();
    NpOS_task_createTask(&task1_Tcb,task1,7,task1_Stack,task1_StackSize,TASK_READY);
    NpOS_task_createTask(&task2_Tcb,task2,6,task2_Stack,task2_StackSize,TASK_READY);
    NpOS_Start();

    while (1){

    }
}


