#ifndef TICK_SWITCH
#define TICK_SWITCH

#include "../Npos_inc/NpOS.h"
#include "stdint.h"
#include "n200_func.h"

#define TIMER_WRITE_REG(offset)     *(uint32_t*)(TIMER_CTRL_ADDR+offset)

extern uint32_t g_npos_systime_Ticks;
void System_tickInit();
uint32_t get_sys_ticks();


#endif // !TICK_SWITCH
