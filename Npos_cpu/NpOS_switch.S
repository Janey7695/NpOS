
// See LICENSE for license details

#ifndef CONTEXTSWITCH_S
#define CONTEXTSWITCH_S

#include "riscv_encoding.h"
#include "riscv_bits.h"
#include "n200_eclic.h"
#include "n200_timer.h"


.macro DISABLE_MIE
  csrc CSR_MSTATUS, MSTATUS_MIE  
.endm


###############################################
###############################################
// IRQ entry point
//

  .section      .text.entry
  .align 2     /* In ECLIC mode, the trap entry must be 64bytes aligned */
  .global irq_entry
irq_entry: // -------------> This label will be set to MTVT2 register
  // Allocate the stack space
    addi sp,sp,-32*4

    sw a7,0(sp)
    sw a6,4(sp)
    sw a5,8(sp)
    sw a4,12(sp)
    sw a3,16(sp)
    sw a2,20(sp)
    sw a1,24(sp)
    sw a0,28(sp)
    sw s11,32(sp)
    sw s10,36(sp)
    sw s9,40(sp)
    sw s8,44(sp)
    sw s7,48(sp)
    sw s6,52(sp)
    sw s5,56(sp)
    sw s4,60(sp)
    sw s3,64(sp)
    sw s2,68(sp)
    sw s1,72(sp)
    sw s0,76(sp)
    sw t6,80(sp)
    sw t5,84(sp)
    sw t4,88(sp)
    sw t3,92(sp)
    sw t2,96(sp)
    sw t1,100(sp)
    sw t0,104(sp)
    sw sp,108(sp)
    sw ra,112(sp)

  //------This special CSR read operation, which is actually use mcause as operand to directly store it to memory
  csrrwi  x0, CSR_PUSHMCAUSE, 29
  //------This special CSR read operation, which is actually use mepc as operand to directly store it to memory
  csrrwi  x0, CSR_PUSHMEPC, 30
  //------This special CSR read operation, which is actually use Msubm as operand to directly store it to memory
  csrrwi  x0, CSR_PUSHMSUBM, 31
 
 //若需要任务切换，则在这里面完成上下文切换
irq_service_loop:
  //------This special CSR read/write operation, which is actually Claim the CLIC to find its pending highest
  // ID, if the ID is not 0, then automatically enable the mstatus.MIE, and jump to its vector-entry-label, and
  // update the link register 
  csrrw ra, CSR_JALMNXTI, ra
  
  //RESTORE_CONTEXT_EXCPT_X5

  #---- Critical section with interrupts disabled -----------------------
  DISABLE_MIE # Disable interrupts 

  LOAD x5,  124(sp)
  csrw CSR_MSUBM, x5
  LOAD x5,  120(sp)
  csrw CSR_MEPC, x5
  LOAD x5,  116(sp)
  csrw CSR_MCAUSE, x5

    lw ra,112(sp)
    lw sp,108(sp)
    lw a7,0(sp)
    lw a6,4(sp)
    lw a5,8(sp)
    lw a4,12(sp)
    lw a3,16(sp)
    lw a2,20(sp)
    lw a1,24(sp)
    lw a0,28(sp)
    lw s11,32(sp)
    lw s10,36(sp)
    lw s9,40(sp)
    lw s8,44(sp)
    lw s7,48(sp)
    lw s6,52(sp)
    lw s5,56(sp)
    lw s4,60(sp)
    lw s3,64(sp)
    lw s2,68(sp)
    lw s1,72(sp)
    lw s0,76(sp)
    lw t6,80(sp)
    lw t5,84(sp)
    lw t4,88(sp)
    lw t3,92(sp)
    lw t2,96(sp)
    lw t1,100(sp)
    lw t0,104(sp)
    
    
    addi sp,sp,32*4
  // Return to regular code
  mret

    .global context_save
context_save:

    csrrw t6,mscratch,x0

    sw sp,0(t6)

    ret

    .global switch_to
switch_to:
    csrw mscratch,a0

    lw sp,0(a0)

    //addi sp,sp,-16
    la t6,irq_service_loop
    sw t6,12(sp)

    ret

  .global root_task_entry
root_task_entry:
  csrw mscratch,a0
  
  lw sp,0(a0)
  addi sp,sp,16

  lw ra,120(sp)
  addi sp,sp,-16
  ret



#endif
