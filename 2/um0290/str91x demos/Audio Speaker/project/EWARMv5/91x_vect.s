;******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
;* File Name          : 91x_vect.s
;* Author             : MCD Application Team
;* Version            : V2.0.0
;* Date               : 09/29/2008
;* Description        : This File used to initialize the exception and IRQ
;*                      vectors, and to enter/return to/from exceptions
;*                      handlers.
;*******************************************************************************
; THE PRESENT SOFTWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS WITH
; CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME. AS
; A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT, INDIRECT
; OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE CONTENT
; OF SUCH SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING INFORMATION
; CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
;******************************************************************************/

    SECTION .intvec:CODE:ROOT(2)		
    CODE32


VectorAddress          EQU    0xFFFFF030  ; VIC Vector address register address.


;*******************************************************************************
;              Import  the __iar_program_start address from 91x_init.s
;*******************************************************************************

   IMPORT  __iar_program_start

;*******************************************************************************
;                      Import exception handlers
;*******************************************************************************

        IMPORT  Undefined_Handler
        IMPORT  SWI_Handler
        IMPORT  Prefetch_Handler
        IMPORT  Abort_Handler
        IMPORT  FIQ_Handler

;*******************************************************************************
;                        Exception vectors
;*******************************************************************************

        LDR     PC, Reset_Addr
        LDR     PC, Undefined_Addr
        LDR     PC, SWI_Addr
        LDR     PC, Prefetch_Addr
        LDR     PC, Abort_Addr
        NOP                             ; Reserved vector
        LDR     PC, IRQ_Addr

;*******************************************************************************
;* Function Name  : FIQHandler
;* Description    : This function is called when FIQ exception is entered.
;* Input          : none
;* Output         : none
;*******************************************************************************
FIQHandler
       SUB    lr,lr,#4        ; Update the link register.
       STMFD  sp!,{r0-r4,lr}  ; Save The workspace plus the current return
                              ; address lr_fiq into the FIQ stack.
       ldr r0,=FIQ_Handler
       ldr lr,=FIQ_Handler_end
       bx r0                 ;Branch to FIQ_Handler.
FIQ_Handler_end:

       LDMFD   sp!,{r0-r4,pc}^; Return to the instruction following...
                              ; ...the exception interrupt.                             


;*******************************************************************************
;               Exception handlers address table
;*******************************************************************************

Reset_Addr      DCD     __iar_program_start
Undefined_Addr  DCD     UndefinedHandler
SWI_Addr        DCD     SWIHandler
Prefetch_Addr   DCD     PrefetchAbortHandler
Abort_Addr      DCD     DataAbortHandler
                DCD     0                   ; Reserved vector
IRQ_Addr        DCD     IRQHandler


;*******************************************************************************
;                                  MACRO
;*******************************************************************************
;*******************************************************************************
;* Macro Name     : SaveContext
;* Description    : This macro is used to save the context before entering
;                   an exception handler.
;* Input          : The range of registers to store.
;* Output         : none
;*******************************************************************************

SaveContext MACRO reg1,reg2  
       STMFD  sp!,{reg1-reg2,lr} ; Save The workspace plus the current return
                                  ; address lr_ mode into the stack. 
        ENDM

;*******************************************************************************
;* Macro Name     : RestoreContext
;* Description    : This macro is used to restore the context to return from
;                   an exception handler and continue the program execution.
;* Input          : The range of registers to restore.
;* Output         : none
;*******************************************************************************

RestoreContext MACRO reg1,reg2      
        LDMFD   sp!,{reg1-reg2,pc}^; Return to the instruction following...
                                ; ...the exception interrupt.                              
        ENDM


;*******************************************************************************
;                         Exception Handlers
;*******************************************************************************


;*******************************************************************************
;* Function Name  : UndefinedHandler
;* Description    : This function is called when undefined instruction
;                   exception is entered.
;* Input          : none
;* Output         : none
;*******************************************************************************

UndefinedHandler
        SaveContext r0,r4    ; Save the workspace plus the current
                              ; return address lr_ und.

       ldr r0,=Undefined_Handler
       ldr lr,=Undefined_Handler_end
       bx r0                 ; Branch to Undefined_Handler.

Undefined_Handler_end:
        RestoreContext r0,r4 ; Return to the instruction following...
                              ; ...the undefined instruction.

;*******************************************************************************
;* Function Name  : SWIHandler
;* Description    : This function is called when SWI instruction executed.
;* Input          : none
;* Output         : none
;*******************************************************************************

SWIHandler
        SaveContext r0,r4    ; Save the workspace plus the current
                              ; return address lr_ svc.

        ldr r0,=SWI_Handler
        ldr lr,=SWI_Handler_end
        bx r0                 ; Branch to SWI_Handler.

SWI_Handler_end:
        RestoreContext r0,r4 ; Return to the instruction following...
                              ; ...the SWI instruction.
;*******************************************************************************
;* Function Name  : PrefetchAbortHandler
;* Description    : This function is called when Prefetch Abort
;                   exception is entered.
;* Input          : none
;* Output         : none
;*******************************************************************************

PrefetchAbortHandler
        SUB    lr,lr,#4       ; Update the link register.
        SaveContext r0,r4    ; Save the workspace plus the current
                              ; return address lr_abt.

       ldr r0,=Prefetch_Handler
       ldr lr,=Prefetch_Handler_end
       bx r0                 ; Branch to Prefetch_Handler.

Prefetch_Handler_end:
        RestoreContext r0,r4 ; Return to the instruction following that...
                              ; ...has generated the prefetch abort exception.

;*******************************************************************************
;* Function Name  : DataAbortHandler
;* Description    : This function is called when Data Abort
;                   exception is entered.
;* Input          : none
;* Output         : none
;*******************************************************************************

DataAbortHandler
        SUB    lr,lr,#8       ; Update the link register.
        SaveContext r0,r4    ; Save the workspace plus the current
                              ; return address lr_ abt.
        ldr r0,=Abort_Handler
        ldr lr,=Abort_Handler_end
        bx r0                 ; Branch to Abort_Handler.

Abort_Handler_end:

        RestoreContext r0,r4 ; Return to the instruction following that...
                              ; ...has generated the data abort exception.
;*******************************************************************************
;* Function Name  : IRQHandler
;* Description    : This function is called when IRQ exception is entered.
;* Input          : none
;* Output         : none
;*******************************************************************************

IRQHandler
       SUB    lr,lr ,#4
       SaveContext r0,r4 
       LDR    r0, = VectorAddress
       LDR    r0, [r0]  ; Read the routine address from VIC0 Vector Address register
       
       BLX    r0        ; Branch with link to the IRQ handler.  
       RestoreContext r0,r4
       


       LTORG

       END
;******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****
