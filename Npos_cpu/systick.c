#include "./systick.h"



/**
    \brief  config system tick
    \param[in]  none
    \retval none
*/
uint32_t g_npos_systime_Ticks;
uint64_t g_npos_systemRunTimeCount;
void System_tickInit(){
    

    //初始化mtime的中断
    //设置系统心跳中断为最低优先级与等级
    eclic_enable_interrupt(7);
    eclic_set_irq_lvl_abs(7, 0);
    eclic_set_irq_priority(7, 0);

    //初始化msip中断
    eclic_enable_interrupt(3);
    eclic_set_irq_lvl_abs(3, 1);
    eclic_set_irq_priority(3, 0);

    //设置mtime的比较值
    TIMER_WRITE_REG(TIMER_MTIMECMP) = 27000*NPOS_SchedulingInterval_MS;
    TIMER_WRITE_REG(TIMER_MTIMECMP+4) = 0x0;
    TIMER_WRITE_REG(TIMER_MTIME) = 0x0;
    TIMER_WRITE_REG(TIMER_MTIME+4) = 0x0;

    g_npos_systime_Ticks = 0;
    g_npos_systemRunTimeCount=0;
}

uint32_t get_sys_ticks(){
    return g_npos_systime_Ticks;
}

uint32_t get_sys_runtime(){
    return g_npos_systemRunTimeCount/27000;
}

