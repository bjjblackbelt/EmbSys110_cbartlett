/****************************************************************************
*  Copyright (c) 2006 by Michael Fischer. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without
*  modification, are permitted provided that the following conditions
*  are met:
*
*  1. Redistributions of source code must retain the above copyright
*     notice, this list of conditions and the following disclaimer.
*  2. Redistributions in binary form must reproduce the above copyright
*     notice, this list of conditions and the following disclaimer in the
*     documentation and/or other materials provided with the distribution.
*  3. Neither the name of the author nor the names of its contributors may
*     be used to endorse or promote products derived from this software
*     without specific prior written permission.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
*  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
*  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
*  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL
*  THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
*  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
*  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
*  OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
*  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
*  OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
*  THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
*  SUCH DAMAGE.
*
****************************************************************************
*
*  History:
*
*  31.03.06  mifi   First Version
*                   This version based on an example from Ethernut and
*                   "ARM Cross Development with Eclipse" from James P. Lynch
*  20141214: Chad Bartlett
*    Added constants to identify types of exceptions
*    Declare abortPrint for printing during exceptions
****************************************************************************/

/*
 * Some defines for the program status registers
 */
   ARM_MODE_USER  = 0x10      /* Normal User Mode                             */
   ARM_MODE_FIQ   = 0x11      /* FIQ Fast Interrupts Mode                     */
   ARM_MODE_IRQ   = 0x12      /* IRQ Standard Interrupts Mode                 */
   ARM_MODE_SVC   = 0x13      /* Supervisor Interrupts Mode                   */
   ARM_MODE_ABORT = 0x17      /* Abort Processing memory Faults Mode          */
   ARM_MODE_UNDEF = 0x1B      /* Undefined Instructions Mode                  */
   ARM_MODE_SYS   = 0x1F      /* System Running in Priviledged Operating Mode */
   ARM_MODE_MASK  = 0x1F

   I_BIT          = 0x80      /* disable IRQ when I bit is set */
   F_BIT          = 0x40      /* disable IRQ when I bit is set */

/*
 * Some defines for exception type
 */
   EXCEPTION_TYPE_DABORT = 0x00  /* Data Abort        */
   EXCEPTION_TYPE_PABORT = 0x01  /* Prefetch Abort    */

/*
 * Register Base Address
 */

    .section .vectors,"ax"
  .global _startup  /* the linker wants this symbol */
    .code 32
_startup:

/****************************************************************************/
/*               Vector table and reset entry                               */
/****************************************************************************/
_vectors:
   ldr pc, ResetAddr    /* Reset                 */
   ldr pc, UndefAddr    /* Undefined instruction */
   ldr pc, SWIAddr      /* Software interrupt    */
   ldr pc, PAbortAddr   /* Prefetch abort        */
   ldr pc, DAbortAddr   /* Data abort            */
   ldr pc, ReservedAddr /* Reserved              */
   ldr pc, IRQAddr      /* IRQ interrupt         */
   ldr pc, FIQAddr      /* FIQ interrupt         */


ResetAddr:     .word ResetHandler
UndefAddr:     .word UndefHandler
SWIAddr:       .word SWIHandler
PAbortAddr:    .word PAbortHandler
DAbortAddr:    .word DAbortHandler
ReservedAddr:  .word 0
IRQAddr:       .word IRQHandler
FIQAddr:       .word FIQHandler

UndefHandler:   B UndefHandler
SWIHandler:     B SWIHandler
@@ PAbortHandler:  B PAbortHandler
@@ DAbortHandler:  B DAbortHandler
@@ IRQ_Handler:    B IRQ_Handler
@@ FIQ_Handler:    B FIQ_Handler

   .ltorg

   .section .init, "ax"
   .code 32

   .global ResetHandler
   .global ExitFunction
   .extern main
   .extern abortPrint

/****************************************************************************/
/*                           Reset handler                                  */
/****************************************************************************/
ResetHandler:
/*
 * Wait for the oscillator is stable
 */
   nop
   nop
   nop
   nop
   nop
   nop
   nop
   nop

   /*
    * Setup a stack for each mode
    */
   msr   CPSR_c, #ARM_MODE_UNDEF | I_BIT | F_BIT   /* Undefined Instruction Mode */
   ldr   sp, =__stack_und_end

   msr   CPSR_c, #ARM_MODE_ABORT | I_BIT | F_BIT   /* Abort Mode */
   ldr   sp, =__stack_abt_end

   msr   CPSR_c, #ARM_MODE_FIQ | I_BIT | F_BIT     /* FIQ Mode */
   ldr   sp, =__stack_fiq_end

   msr   CPSR_c, #ARM_MODE_IRQ | I_BIT | F_BIT     /* IRQ Mode */
   ldr   sp, =__stack_irq_end

   msr   CPSR_c, #ARM_MODE_SVC | I_BIT | F_BIT     /* Supervisor Mode */
   ldr   sp, =__stack_svc_end



/*
 * Copy .data section (copy from ROM to RAM)
 */
     LDR     r1, =_etext
     LDR     r2, =__data_start
     LDR     r3, =__data_end
copyloop:
     CMP     r2, r3
     LDRLO   r0, [r1], #4
     STRLO   r0, [r2], #4
     BLO     copyloop

/*
 * zero .bss section
 */
   ldr   r1, =__bss_start
   ldr   r2, =__bss_end
   ldr   r3, =0
bss_clear_loop:
   cmp   r1, r2
   strne r3, [r1], #+4
   bne   bss_clear_loop


   /*
    * Jump to main
    */
   mrs   r0, cpsr
   bic   r0, r0, #I_BIT | F_BIT     /* Enable FIQ and IRQ interrupt */
   msr   cpsr, r0

   mov   r0, #0 /* No arguments */
   mov   r1, #0 /* No arguments */
   ldr   r2, =main
   mov   lr, pc
   bx    r2     /* And jump... */

ExitFunction:
   nop
   nop
   nop
   b ExitFunction


/****************************************************************************/
/*                         Default interrupt handler                        */
/****************************************************************************/

/*
    Write a prefetch abort handler.
    (Hint, do the data abort one first. It's easier.)

    Retrieve the original lr from the SVC mode.
        Save R1 to stack so we can use it as a temporary variable.
        Switch to SVC mode so we can the retrieve the orignal lr.
        Save the SVC mode's lr to r1.
        Switch back the ABORT mode.
        Set lr to the original lr before the call that caused the prefetch abort.
        Restore r1 from the stack.

   Save all registers and lr to the stack.

    Call the abort print function.
        Set the first parameter (R0) to 0 for data abort or 1 for prefetch abort.
        Set the second parameter (R1) to address where the abort occured.
        Branch with link to the abortPrint function.

    Return from this exception.
        Restore registers and pc from the stack
*/
PAbortHandler:
    /* Retrieve the original lr from the SVC mode. */
   STMFD r13!, {r1,r2}           /* Save R1 & R2 to the stack to use as temp variables */
   MRS r1, CPSR                  /* Save the CPSR to r1 */
   BIC r1, r1, #ARM_MODE_MASK    /* Clear the mode bits in CPSR */
   ORR r1, r1, #ARM_MODE_SVC     /* Set the supervisor mode bits */
   MSR CPSR, r1                  /* Save r1 to CPSR */
   MOV r2, r14                   /* Save the SVC mode's lr to r2 */
   BIC r1, r1, #ARM_MODE_MASK    /* Clear the mode bits in CPSR */
   ORR r1, r1, #ARM_MODE_ABORT   /* Set the ABORT mode bits */
   MSR CPSR, r1                  /* Save r1 to CPSR */
   MOV r14, r2                   /* set LR to the original LR before the call */
   LDMFD r13!, {r1,r2}           /* Pop R1 & R2 from the stack */

   /* Save all registers and lr to the stack. */
   STMFD r13!, {r0-r3, r14}

   /* Call the abort print function. */
   MOV r0, #EXCEPTION_TYPE_PABORT /* abortPrint type parameter    */
   MOV r1, r14                    /* abortPrint address parameter. Copy of LR. */
   SUB r1, #4                     /* Adjust copy of LR for the instruction that */
                                  /* actually caused the exception. */
   BL abortPrint                  /* Print the exception info. */

   /* Return from this exception. */
   LDMFD r13!, {r0-r3, pc}^       /* Restore registers and pc from the stack. */

/*
    Write a data abort handler.

    Adjust lr back by 4
    Save all registers and lr to the stack. You will use them to return.

    Call the abort print function.
        Set the first parameter (R0) to 0 for data abort or 1 for prefetch abort.
        Set the second parameter (R1) to address where the abort occured.
        Branch with link to the abortPrint function.

    Return from this exception.
        Restore registers and pc from the stack

*/
DAbortHandler:
                                  /* b  DAbortHandler */
   SUB r14, #4                    /* Adjust LR. Instruction after instruction */
                                  /* that triggered abort. */
   STMFD r13!, {r0-r3, r14}       /* Push context onto the ABT (abort) stack */
                                  /* When calling a C function you must store r0-r3 */
                                  /* and r14 (LR) before calling and restore after. */
   MOV r0, #EXCEPTION_TYPE_DABORT /* abortPrint type parameter    */
   MOV r1, r14                    /* abortPrint address parameter. Copy of LR. */
   SUB r1, #4                     /* Adjust copy of LR for the instruction that */
                                  /* actually caused the exception. */

   BL abortPrint                  /* Print the exception info     */
   LDMFD r13!, {r0-r3, pc}^       /* Pop the context and return. */
                                  /* The caret (^) after the register list tells */
                                  /* the CPU to restore the CPSR from the SPSR_abt */

FIQHandler:
   b FIQHandler

   .weak ExitFunction
   .weak UndefHandler, PAbortHandler, DAbortHandler
   .weak IRQHandler, FIQHandler

   .ltorg

@@ IRQ_Wrapper based on Examples for
@@ AT91-ARM7TDMI AIC from Atmel,
@@ adapted to LPC23xx/24xx VIC by M. Thomas
@@ This wrapper avoids compiler-dependencies.

.set LPC_BASE_VIC, 0xFFFFF000
.set VIC_VectAddr, 0xF00

.arm
IRQ_Handler:

@- Manage Exception Entry
@- Adjust and save LR_irq in IRQ stack
            sub         lr, lr, #4
            stmfd       sp!, {lr}

@- Save SPSR need to be saved for nested interrupt
            mrs         r14, SPSR
            stmfd       sp!, {r14}

@- Save and r0 in IRQ stack
            stmfd       sp!, {r0}

@- Load the ISR-Address from VICVectAddr
            ldr         r14, =LPC_BASE_VIC
            ldr         r0 , [r14, #VIC_VectAddr]

@- Enable Interrupt and Switch in Supervisor Mode
           msr         CPSR_c, #ARM_MODE_SVC

@- Save scratch/used registers and LR in User Stack
            stmfd       sp!, { r1-r3, r12, r14 }

@- Branch to the routine pointed by the VIC_VectAddr
            mov         r14, pc
            bx          r0

@- Disable Interrupt and switch back in IRQ mode
			mov 		r1, #0
			orr 		r1, #I_BIT
			orr 		r1, #ARM_MODE_IRQ
            msr         CPSR_c, r1

@- Restore scratch/used registers and LR from User Stack
            ldmia       sp!, { r1-r3, r12, r14 }

@-  Mark the End of Interrupt on the VIC
@   by writing to VICVectAddr - not needed
@   here since already done in the ISRs
@@           ldr         r14, =LPC_BASE_VIC
@@           str         r14, [r14, #VIC_VectAddr]

@- Restore R0
            ldmia       sp!, {r0}

@- Restore SPSR_irq and r0 from IRQ stack
            ldmia       sp!, {r14}
            msr         SPSR_cxsf, r14

@- Restore adjusted  LR_irq from IRQ stack directly in the PC
            ldmia       sp!, {pc}^
.end
