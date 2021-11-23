#ifndef TICK_SWITCH
#define TICK_SWITCH

#include "../Npos_inc/NpOS.h"
#include "stdint.h"

#if  NPOS_ENTER_CIRTICAL_BY == NPOS_ENTER_CIRTICAL_BY_DISABLEIRQ  //使用关中断的方式

//进入临界区
#define NpOS_ENTER_CRITICAL()       
//退出临界区
#define NpOS_EXIT_CRITICAL()    

#endif

extern uint32_t g_npos_systime_Ticks;
extern uint64_t g_npos_systemRunTimeCount;
void System_tickInit();
uint32_t get_sys_ticks();
uint32_t get_sys_runtime();


#endif // !TICK_SWITCH
