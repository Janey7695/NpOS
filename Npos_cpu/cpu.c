#include "./cpu.h"



/**
    \brief  config system tick
    \param[in]  none
    \retval none
*/
uint32_t g_npos_systime_Ticks;
uint64_t g_npos_systemRunTimeCount;
void System_tickInit(){
   
	/* 初始化系统心跳时钟，使之每 NPOS_CountPerMS*NPOS_SchedulingInterval_MS 发生一次中断*/
	/* 系统心跳中断的优先级应该设为最低 */

	/* write you timer init code here*/


	/********/

    g_npos_systime_Ticks = 0;
    g_npos_systemRunTimeCount=0;
}

uint32_t get_sys_ticks(){
    return g_npos_systime_Ticks;
}

uint32_t get_sys_runtime(){
    return g_npos_systemRunTimeCount/NPOS_CountPerMS;
}

