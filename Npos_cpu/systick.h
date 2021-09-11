#ifndef TICK_SWITCH
#define TICK_SWITCH

#include "../Npos_inc/NpOS.h"
#include "stdint.h"
#include "n200_func.h"

#define TIMER_WRITE_REG(offset)     *(uint32_t*)(TIMER_CTRL_ADDR+offset)

extern uint32_t g_npos_systime_Ticks;
extern uint64_t g_npos_systemRunTimeCount;
void System_tickInit();
uint32_t get_sys_ticks();
uint32_t get_sys_runtime();


#endif // !TICK_SWITCH
