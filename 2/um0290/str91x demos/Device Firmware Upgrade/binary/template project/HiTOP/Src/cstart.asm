;/******************** (C) COPYRIGHT 2007 STMicroelectronics ********************
;* File Name          : 91x_init.s
;* Author             : MCD Application Team
;* Version            : V2.0
;* Date               : 12/07/2007
;* Description        : This module performs:
;*                      - FLASH/RAM initialization,
;*                      - Stack pointer initialization for each mode ,
;*                      - Branches to ?main in the C library (which eventually 
;*                        calls main()).
;*		      On reset, the ARM core starts up in Supervisor (SVC) mode,
;*		      in ARM state,with IRQ and FIQ disabled.
;********************************************************************************
;* THE PRESENT SOFTWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS WITH
;* CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME. AS
;* A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT, INDIRECT
;* OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE CONTENT
;* OF SUCH SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING INFORMATION
;* CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
;*******************************************************************************/

;Uncomment Only ONE of the following lines to select your STR91xFA Flash size 
;and the Boot bank (Bank0 or Bank1).You have also to uncomment the appropriate 
;defines in "91x_conf.h" file.
;In this File/library the default size is 512Kbytes  and the boot bank is Bank0

Flash_256KB_Bank0_Boot .equ 0
Flash_512KB_Bank0_Boot .equ 0
Flash_1MB_Bank0_Boot   .equ 0
Flash_2MB_Bank0_Boot   .equ 0

Flash_256KB_Bank1_Boot .equ 0
Flash_512KB_Bank1_Boot .equ 0
Flash_2MB_Bank1_Boot   .equ 0
Flash_1MB_Bank1_Boot   .equ 0


; Uncomment the following define when working in debug mode you have also to 
; Uncomment the same define in "91x_conf.h" file

debug_mode .equ 1


; --- Standard definitions of mode bits and interrupt (I & F) flags in PSRs

Mode_USR           .equ      0x10
Mode_FIQ           .equ      0x11
Mode_IRQ           .equ      0x12
Mode_SVC           .equ      0x13
Mode_ABT           .equ      0x17
Mode_UND           .equ      0x1B
Mode_SYS           .equ      0x1F ; available on ARM Arch 4 and later

I_Bit              .equ      0x80 ; when I bit is set, IRQ is disabled
F_Bit              .equ      0x40 ; when F bit is set, FIQ is disabled

VectorAddress      .equ      0xFFFFF030  ; VIC Vector address register address.
; --- STR9X SCU specific definitions

SCU_BASE_Address    .equ     0x5C002000 ; SCU Base Address 
SCU_SCR0_OFST       .equ     0x00000034 ; System Configuration Register 0 Offset

; --- STR9X FMI specific definitions

FMI_BASE_Address    .equ      0x54000000 ; FMI Base Address
FMI_BBSR_OFST       .equ      0x00000000 ; Boot Bank Size Register offset
FMI_NBBSR_OFST      .equ      0x00000004 ; Non-boot Bank Size Register offset
FMI_BBADR_OFST      .equ      0x0000000C ; Boot Bank Base Address Register offset
FMI_NBBADR_OFST     .equ      0x00000010 ; Non-boot Bank Base Address Register offset
FMI_CR_OFST         .equ      0x00000018 ; Control Register offset



.if Flash_2MB_Bank0_Boot

BOOT_BANK_Size        .equ        0x6 ; Boot Bank Size Register
NON_BOOT_BANK_Size    .equ        0x4 ; Non-boot Bank Size Register
BOOT_BANK_Address     .equ        0x00000000 ; Boot Bank Base Address Register
NON_BOOT_BANK_Address .equ        0x00200000 ; Non-boot Bank Base Address Register

.endif

.if Flash_2MB_Bank1_Boot

BOOT_BANK_Size        .equ         0x2 ; Boot Bank Size Register
NON_BOOT_BANK_Size    .equ         0x8 ; Non-boot Bank Size Register
BOOT_BANK_Address     .equ         0x00000000 ; Boot Bank Base Address Register
NON_BOOT_BANK_Address .equ         0x00200000 ; Non-boot Bank Base Address Register

.endif

.if Flash_1MB_Bank0_Boot

BOOT_BANK_Size         .equ        0x5 ; Boot Bank Size Register
NON_BOOT_BANK_Size     .equ        0x4 ; Non-boot Bank Size Register
BOOT_BANK_Address      .equ        0x00000000 ; Boot Bank Base Address Register
NON_BOOT_BANK_Address  .equ        0x00200000 ; Non-boot Bank Base Address Register

.endif

.if  Flash_1MB_Bank1_Boot

BOOT_BANK_Size           .equ      0x2 ; Boot Bank Size Register
NON_BOOT_BANK_Size       .equ      0x7 ; Non-boot Bank Size Register
BOOT_BANK_Address        .equ      0x00000000 ; Boot Bank Base Address Register
NON_BOOT_BANK_Address    .equ      0x00200000 ; Non-boot Bank Base Address Register

.endif

.if Flash_512KB_Bank0_Boot

BOOT_BANK_Size           .equ      0x4 ; Boot Bank Size Register
NON_BOOT_BANK_Size       .equ      0x2 ; Non-boot Bank Size Register
BOOT_BANK_Address        .equ      0x00000000 ; Boot Bank Base Address Register
NON_BOOT_BANK_Address    .equ      0x00080000 ; Non-boot Bank Base Address Register

.endif

.if Flash_512KB_Bank1_Boot

BOOT_BANK_Size        .equ        0x0 ; Boot Bank Size Register
NON_BOOT_BANK_Size    .equ        0x6 ; Non-boot Bank Size Register
BOOT_BANK_Address     .equ        0x00000000 ; Boot Bank Base Address Register
NON_BOOT_BANK_Address .equ        0x00080000 ; Non-boot Bank Base Address Register


.endif

.if Flash_256KB_Bank0_Boot

BOOT_BANK_Size        .equ       0x3 ; Boot Bank Size Register
NON_BOOT_BANK_Size    .equ       0x2 ; Non-boot Bank Size Register
BOOT_BANK_Address     .equ       0x00000000 ; Boot Bank Base Address Register
NON_BOOT_BANK_Address .equ       0x00080000 ; Non-boot Bank Base Address Register

.endif

.if Flash_256KB_Bank1_Boot

BOOT_BANK_Size        .equ       0x0 ; Boot Bank Size Register
NON_BOOT_BANK_Size    .equ       0x5 ; Non-boot Bank Size Register
BOOT_BANK_Address     .equ       0x00000000 ; Boot Bank Base Address Register
NON_BOOT_BANK_Address .equ       0x00080000 ; Non-boot Bank Base Address Register

.endif


   
        .extern    Undefined_Handler
        .extern    SWI_Handler
        .extern    Prefetch_Handler
        .extern    Abort_Handler
        .extern    FIQ_Handler

        .extern _lc_ub_stack            ; usr/sys mode stack pointer
        .extern _lc_ue_stack            ; symbol required by debugger
        .extern _lc_ub_table            ; ROM to RAM copy table
        .extern _lc_ub_stack_fiq
        .extern _lc_ub_stack_irq
        .extern _lc_ub_stack_svc
        .extern _lc_ub_stack_abt
        .extern _lc_ub_stack_und
        .extern main
        .extern _Exit
        .extern exit
        .weak   exit
        .extern __init_hardware
        .extern __init_vector_table
        
        .extern _APPLICATION_MODE_      ; symbol defined in LSL file
        
    .if @defined('__PROF_ENABLE__')
        .extern __prof_init
    .endif
    .if @defined('__POSIX__')
        .extern posix_main
        .extern _posix_boot_stack_top
    .endif

        .global _START
		.section .text.cstart

        .code32
        .align  4
_START: 
        ;; anticipate possible ROM/RAM remapping
        ;; by loading the 'real' program address
        ldr     pc,=_Next
_Next:
; ------------------------------------------------------------------------------
; Description  : This delay is added to let JTAG debuggers able to connect
;                to STR91x micro then load the program even if a previously 
;                flash code is halting the CPU core. This may happen when the
;                flash content is corrupt by containing a "bad" code like entering
;                soon to IDLE or SLEEP Low power modes.
;                When going to production/Release code and to remove this Start-up
;                delay, Please comment the "Debug_mode" define above.
; ------------------------------------------------------------------------------
         
.if debug_mode               
                                                                  
      MOV     r0, #0x4000        
Loop                              
      SUBS    r0,r0, #1 
      SUBS    r0,r0, #1
      SUBS    r0,r0, #1
      SUBS    r0,r0, #1
      SUBS    r0,r0, #1 
      SUBS    r0,r0, #1
      SUBS    r0,r0, #1
      SUBS    r0,r0, #1                              
      BNE     Loop
.endif

; ------------------------------------------------------------------------------
; Description  :  Enable the Buffered mode.
;                 To use buffered mode access you have to uncomment Buffered  
;                 define on the 91x_conf.h file
; ------------------------------------------------------------------------------

       ;MRC     p15, 0, r0, c1, c0, 0   ; Read CP15 register 1 into r0
       ;ORR     r0, r0, #0x8            ; Enable Write Buffer on AHB
       ;MCR     p15, 0, r0, c1, c0, 0   ; Write CP15 register 1  
;------------------------------------------------------------------------------
; Description  :  Write Buffer in ITCM may cause the Flash “write then read”    
;                 command order reversed and cause flash error.
;                 To maintain the right order, bit 18 (Instruction TCM order bit)
;                 in the Configuration Registers of the ARM966E-S core must be set.          
; ------------------------------------------------------------------------------
    
        ;MOV     r0, #0x40000             
        ;MCR     p15,0x1,r0,c15,c1,0

;------------------------------------------------------------------------------
; Description  : FMI Registers configuration depending on the Flash size selected, 
;                and the boot bank.
;
;     After reset, the application program has to write the size and start
;     address of Bank 1 in the FMI_BBSR and FMI_BBADR registers and the size and 
;     start address of Bank 0 in the FMI_NBBSR and FMI_NBBADR registers.
; ------------------------------------------------------------------------------

        ;LDR     R6, =FMI_BASE_Address   
        ;LDR     R7, = BOOT_BANK_Size          ; BOOT BANK Size=
        ;STR     R7, [R6, #FMI_BBSR_OFST]      ; (2^BOOT_BANK_Size) * 32KBytes       
        ;LDR     R7, = NON_BOOT_BANK_Size      ; NON BOOT BANK Size =
        ;STR     R7, [R6, #FMI_NBBSR_OFST]     ; (2^NON_BOOT_BANK_Size) * 8KBytes
        ;LDR     R7, =BOOT_BANK_Address        ; BOOT BANK Address
        ;MOV     R7, R7 ,LSR #0x2   
        ;STR     R7, [R6, #FMI_BBADR_OFST]
        ;LDR     R7, =NON_BOOT_BANK_Address    ; BOOT BANK Address
        ;MOV     R7, R7 ,LSR #0x2              
        ;STR     R7, [R6, #FMI_NBBADR_OFST]    
  
       
       ; LDR     R7, = 0x18                    ; Enable Both banks
        ;STR     R7, [R6, #FMI_CR_OFST]        
                                              
      
; --- Enable 96K of RAM  & Disable DTCM & AHB wait-states

        LDR     R0, = SCU_BASE_Address  
        LDR     R1, = 0x0196
        STR     R1, [R0, #SCU_SCR0_OFST]
 
        ;; initialize the stack pointer for each mode
        ;; while keeping interrupts disabled

        msr     cpsr_c,#Mode_FIQ | F_Bit | I_Bit        ; change to FIQ mode
        ldr     sp,=_lc_ub_stack_fiq                    ; initialize FIQ stack pointer

        msr     cpsr_c,#Mode_IRQ | F_Bit | I_Bit        ; change to IRQ mode
        ldr     sp,=_lc_ub_stack_irq                    ; initialize IRQ stack pointer

        msr     cpsr_c,#Mode_SVC | F_Bit | I_Bit        ; change to Supervisor mode
        ldr     sp,=_lc_ub_stack_svc                    ; initialize SVC stack pointer

        msr     cpsr_c,#Mode_ABT | F_Bit | I_Bit        ; change to Abort mode
        ldr     sp,=_lc_ub_stack_abt                    ; initialize ABT stack pointer

        msr     cpsr_c,#Mode_UND | F_Bit | I_Bit        ; change to Undefined mode
        ldr     sp,=_lc_ub_stack_und                    ; initialize UND stack pointer

        msr     cpsr_c,#Mode_SYS                        ; change to System mode
        ldr     sp,=_lc_ub_stack                        ; initialize USR/SYS stack pointer  


      
;; copy initialized sections from ROM to RAM
      ;; and clear uninitialized data sections in RAM

      ldr     r3,=_lc_ub_table
      movs    r0,#0
cploop:
      ldr     r4,[r3,#0]      ; load type
      ldr     r5,[r3,#4]      ; dst address
      ldr     r6,[r3,#8]      ; src address
      ldr     r7,[r3,#12]     ; size

      cmp     r4,#1
      beq     copy
      cmp     r4,#2
      beq     clear
      b       done

copy:
      subs    r7,r7,#1
      ldrb    r1,[r6,r7]
      strb    r1,[r5,r7]
      bne     copy

      adds    r3,r3,#16
      b       cploop

clear:
      subs    r7,r7,#1
      strb    r0,[r5,r7]
      bne     clear

      adds    r3,r3,#16
      b       cploop

done:
      ;; initialize or copy the vector table

      .if @defined('__POSIX__')

         ;; posix stack buffer for system upbringing
         ldr     r0,=_posix_boot_stack_top
         ldr     r0, [r0]
         mov     sp,r0

      .else

         ;; load r10 with end of USR/SYS stack, which is
         ;; needed in case stack overflow checking is on
         ;; NOTE: use 16-bit instructions only, for ARMv6M
         ldr     r0,=_lc_ue_stack
         mov     r10,r0

      .endif

      .if @defined('__PROF_ENABLE__')
         bl      __prof_init
      .endif

      .if @defined('__POSIX__')
         ;; call posix_main with no arguments
         bl      posix_main
      .else
         ;; call main with argv[0]==NULL & argc==0
         movs     r0,#0
         movs     r1,#0
         bl       main
      .endif

         ;; call exit using the return value from main()
         ;; Note. Calling exit will also run all functions
         ;; that were supplied through atexit().
         bl      exit

      .ltorg
      .endsec

      .calls  '_START','_init_hardware'
      .calls  '_START','_init_vector_table'
      .if @defined('__PROF_ENABLE__')
         .calls  '_START','__prof_init'
      .endif
      .if @defined('__POSIX__')
         .calls  '_START','posix_main'
      .else
         .calls  '_START','main'
      .endif
         .calls  '_START','exit'
         .calls  '_START','',0

      ;.end

       ; b      main
   ;; .endif

        ;; call exit using the return value from main()
        ;; Note. Calling exit will also run all functions
        ;; that were supplied through atexit().
        bl      exit

        .ltorg                  ; assemble literal data pool here
        
        .endsec
   
		   
		   

;*************************************************************************
; Exception Vectors
;*************************************************************************
.section .text.vectors, at(0)

           .code32
           .align  4

Vectors:


        LDR     PC, Reset_Addr
        LDR     PC, Undefined_Addr
        LDR     PC, SWI_Addr
        LDR     PC, Prefetch_Addr
        LDR     PC, Abort_Addr
        NOP                         
       ; LDR     PC, [PC, #-0xFF0]
        LDR     PC, IRQ_Addr

;*******************************************************************************
;* Function Name  : FIQHandler
;* Description    : This function is called when FIQ exception is entered.
;* Input          : none
;* Output         : none
;*******************************************************************************
FIQHandler:
       SUB    lr,lr,#4        ; Update the link register.
       STMFD  sp!,{r0-r3,lr}  ; Save The workspace plus the current return
                              ; address lr_fiq into the FIQ stack.
       ldr r0,=FIQ_Handler
       ldr lr,=FIQ_Handler_end
       bx r0                 ;Branch to FIQ_Handler.
FIQ_Handler_end:

      LDMFD   sp!,{r0-r3,pc}^; Return to the instruction following...
                              ; ...the exception interrupt.

;*******************************************************************************
;               Exception handlers address table
;*******************************************************************************    

Reset_Addr      .DW _START
Undefined_Addr  .DW UndefinedHandler 
SWI_Addr        .DW SWIHandler
Prefetch_Addr   .DW PrefetchAbortHandler
Abort_Addr      .DW DataAbortHandler
                .DW  0
IRQ_Addr        .DW IRQHandler


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

SaveContext .MACRO   [r0,r3]
        STMFD  sp!,{r0-r3,lr} ; Save The workspace plus the current return
                                  ; address lr_ mode into the stack.
 
        .ENDM

;*******************************************************************************
;* Macro Name     : RestoreContext
;* Description    : This macro is used to restore the context to return from
;                   an exception handler and continue the program execution.
;* Input          : The range of registers to restore.
;* Output         : none
;*******************************************************************************

RestoreContext .MACRO [r0,r3]
      
        LDMFD   sp!,{r0-r3,pc}^; Return to the instruction following...
                                ; ...the exception interrupt.
        .ENDM


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

UndefinedHandler:
        SaveContext r0,r3    ; Save the workspace plus the current
                              ; return address lr_ und.

       ldr r0,=Undefined_Handler
       ldr lr,=Undefined_Handler_end
       bx r0                 ; Branch to Undefined_Handler.

Undefined_Handler_end:
        RestoreContext r0,r3 ; Return to the instruction following...
                              ; ...the undefined instruction.

;*******************************************************************************
;* Function Name  : SWIHandler
;* Description    : This function is called when SWI instruction executed.
;* Input          : none
;* Output         : none
;*******************************************************************************

SWIHandler:
        SaveContext r0,r3    ; Save the workspace plus the current
                              ; return address lr_ svc.

        ldr r0,=SWI_Handler
        ldr lr,=SWI_Handler_end
        bx r0                 ; Branch to SWI_Handler.

SWI_Handler_end:
        RestoreContext r0,r3 ; Return to the instruction following...
                              ; ...the SWI instruction.
;*******************************************************************************
;* Function Name  : PrefetchAbortHandler
;* Description    : This function is called when Prefetch Abort
;                   exception is entered.
;* Input          : none
;* Output         : none
;*******************************************************************************

PrefetchAbortHandler:
        SUB    lr,lr,#4       ; Update the link register.
        SaveContext r0,r3    ; Save the workspace plus the current
                              ; return address lr_abt.

       ldr r0,=Prefetch_Handler
       ldr lr,=Prefetch_Handler_end
       bx r0                 ; Branch to Prefetch_Handler.

Prefetch_Handler_end:
        RestoreContext r0,r3 ; Return to the instruction following that...
                              ; ...has generated the prefetch abort exception.

;*******************************************************************************
;* Function Name  : DataAbortHandler
;* Description    : This function is called when Data Abort
;                   exception is entered.
;* Input          : none
;* Output         : none
;*******************************************************************************

DataAbortHandler:
        SUB    lr,lr,#8       ; Update the link register.
        SaveContext r0,r3    ; Save the workspace plus the current
                              ; return address lr_ abt.
        ldr r0,=Abort_Handler
        ldr lr,=Abort_Handler_end
        bx r0                 ; Branch to Abort_Handler.

Abort_Handler_end:

        RestoreContext r0,r3 ; Return to the instruction following that...
                              ; ...has generated the data abort exception.
;*******************************************************************************
;* Function Name  : IRQHandler
;* Description    : This function is called when IRQ exception is entered.
;* Input          : none
;* Output         : none
;*******************************************************************************

IRQHandler:
       SUB    lr,lr ,#4
       SaveContext r0,r3 
       LDR    r0, = VectorAddress
       LDR    r0, [r0]  ; Read the routine address from VIC0 Vector Address register     
                        
       BLX    r0        ; Branch with link to the IRQ handler.        
       RestoreContext r0,r3
       
  .endsec


        .calls  '_START','__init_hardware'
        .calls  '_START','__init_vector_table'
    .if @defined('__PROF_ENABLE__')
        .calls  '_START','__prof_init'
    .endif
    .if @defined('__POSIX__')
        .calls  '_START','posix_main'
    .else
        .calls  '_START','main'
    .endif
        .calls  '_START','exit'
        .calls  '_START','',0,0

        .end


