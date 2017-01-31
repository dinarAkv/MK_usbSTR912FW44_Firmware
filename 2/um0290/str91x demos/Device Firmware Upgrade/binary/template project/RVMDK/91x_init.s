;/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
;* File Name          : 91x_init.s
;* Author             : MCD Application Team
;/* <<< Use Configuration Wizard in Context Menu >>> 
;* Version            : V2.0.0
;* Date               : 09/29/2008
;* Description        : This module performs:
;*                      - FLASH/RAM initialization,
;*                      - Stack pointer initialization for each mode ,
;*                      - Branches to ?main in the C library (which eventually 
;*                        calls main()).
;*		      On reset, the ARM core starts up in Supervisor (SVC) mode,
;*		      in ARM state,with IRQ and FIQ disabled.
;********************************************************************************
;
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

; Uncomment the following define when working in debug mode you have also to 
; Uncomment the same define in "91x_conf.h" file
Debug_Mode        EQU      1

; --- Standard definitions of mode bits and interrupt (I & F) flags in PSRs

Mode_USR           EQU     0x10
Mode_FIQ           EQU     0x11
Mode_IRQ           EQU     0x12
Mode_SVC           EQU     0x13
Mode_ABT           EQU     0x17
Mode_UND           EQU     0x1B
Mode_SYS           EQU     0x1F ; available on ARM Arch 4 and later

I_Bit              EQU     0x80 ; when I bit is set, IRQ is disabled
F_Bit              EQU     0x40 ; when F bit is set, FIQ is disabled

; --- Initialize Stack pointer registers
;// <h> Stack Configuration (Stack Sizes in Bytes)
;//   <o0> Undefined Mode      <0x0-0xFFFFFFFF:8>
;//   <o1> Supervisor Mode     <0x0-0xFFFFFFFF:8>
;//   <o2> Abort Mode          <0x0-0xFFFFFFFF:8>
;//   <o3> Fast Interrupt Mode <0x0-0xFFFFFFFF:8>
;//   <o4> Interrupt Mode      <0x0-0xFFFFFFFF:8>
;//   <o5> User/System Mode    <0x0-0xFFFFFFFF:8>
;// </h> Stack Configuration

UND_Stack_Size  EQU     0x00000000
SVC_Stack_Size  EQU     0x00000008
ABT_Stack_Size  EQU     0x00000000
FIQ_Stack_Size  EQU     0x00000020
IRQ_Stack_Size  EQU     0x00000100
USR_Stack_Size  EQU     0x00000400

ISR_Stack_Size  EQU     (UND_Stack_Size + SVC_Stack_Size + ABT_Stack_Size + \
                         FIQ_Stack_Size + IRQ_Stack_Size)

                AREA    STACK, NOINIT, READWRITE, ALIGN=3

Stack_Mem       SPACE   USR_Stack_Size
__initial_sp    SPACE   ISR_Stack_Size
Stack_Top


;// <h> Heap Configuration
;//   <o>  Heap Size (in Bytes) <0x0-0xFFFFFFFF>
;// </h>

Heap_Size       EQU     0x00000200

                AREA    HEAP, NOINIT, READWRITE, ALIGN=3
__heap_base
Heap_Mem        SPACE   Heap_Size
__heap_limit

; --- STR9X SCU specific definitions

SCU_BASE_Address    EQU     0x5C002000 ; SCU Base Address 
SCU_SCR0_OFST       EQU     0x00000034 ; System Configuration Register 0 Offset

; --- STR9X FMI specific definitions

FMI_BASE_Address    EQU     0x54000000 ; FMI Base Address
FMI_BBSR_OFST       EQU     0x00000000 ; Boot Bank Size Register offset
FMI_NBBSR_OFST      EQU     0x00000004 ; Non-boot Bank Size Register offset
FMI_BBADR_OFST      EQU     0x0000000C ; Boot Bank Base Address Register offset
FMI_NBBADR_OFST     EQU     0x00000010 ; Non-boot Bank Base Address Register offset
FMI_CR_OFST         EQU     0x00000018 ; Control Register offset

;// <e0> Flash Memory Interface (FMI)
;//   <e1.3> Flash BOOT Bank Enable
;//   <h> BOOT Bank Size Configuration (BBSR)
;//     <o2.0..3>  BBSIZE: Memory size 
;//       <i> Default 32KB
;//                     <0=>   32KB
;//                     <1=>   64KB
;//                     <2=>  128KB
;//                     <3=>  256KB
;//                     <4=>  512KB
;//                     <5=>    1MB
;//                     <6=>    2MB
;//   </h>
;//   <h> BOOT Bank Base Address Configuration (BBADR)
;//     <o3.0..23> BBADDR: Address <0x0-0xFFFFFF>
;//       <i> Default: 0x000000
;//   </h>
;//   </e>
;//   <e1.4> Flash Bank 1 Enable
;//   <h> Non-BOOT Bank Size Configuration (NBBSR)
;//     <o4.0..3>  NBBSIZE: Memory size 
;//       <i> Default 32KB
;//                     <2=>   32KB
;//                     <3=>   64KB
;//                     <4=>  128KB
;//                     <5=>  256KB
;//                     <6=>  512KB
;//                     <7=>    1MB
;//                     <8=>    2MB
;//   </h>
;//   <h> BOOT Bank Base Address Configuration (NBBADR)
;//     <o5.0..23> NBBADDR: Address <0x0-0xFFFFFF>
;//       <i> Default: 0x000000
;//   </h>
;// </e0> End of FMI
FMI_SETUP       EQU     0
FMI_CR_Val      EQU     0x00000018
FMI_BBSR_Val    EQU     0x00000000
FMI_BBADR_Val   EQU     0x00000000
FMI_NBBSR_Val   EQU     0x00000006
FMI_NBBADR_Val  EQU     0x00080000
FLASH_CFG_Val   EQU     0x00000000

;---------------------------------------------------------------
; Reset Handler
;---------------------------------------------------------------

                PRESERVE8


; Area Definition and Entry Point
;  Startup Code must be linked first at Address at which it expects to run.

                AREA    Init, CODE, READONLY
                ARM
                EXPORT  Reset_Handler
Reset_Handler
        LDR     pc, =NextInst
NextInst
; ------------------------------------------------------------------------------
; Description  : This delay is added to let JTAG debuggers able to connect
;                to STR91x micro then load the program even if a previously 
;                flash code is halting the CPU core. This may happen when the
;                flash content is corrupt by containing a "bad" code like entering
;                soon to IDLE or SLEEP Low power modes.
;                When going to production/Release code and to remove this Start-up
;                delay, Please comment the "Debug_mode" define above.
; ------------------------------------------------------------------------------
 IF Debug_Mode = 1                                                        
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
 ENDIF

; ------------------------------------------------------------------------------
; Description  :  Enable the Buffered mode.
;                 To use buffered mode access you have to uncomment Buffered  
;                 define on the 91x_conf.h file
; ------------------------------------------------------------------------------

        MRC     p15, 0, r0, c1, c0, 0   ; Read CP15 register 1 into r0
        ORR     r0, r0, #0x8            ; Enable Write Buffer on AHB
        MCR     p15, 0, r0, c1, c0, 0   ; Write CP15 register 1  
;------------------------------------------------------------------------------
; Description  :  Write Buffer in ITCM may cause the Flash “write then read”    
;                 command order reversed and cause flash error.
;                 To maintain the right order, bit 18 (Instruction TCM order bit)
;                 in the Configuration Registers of the ARM966E-S core must be set.          
; ------------------------------------------------------------------------------
    
        MOV     r0, #0x40000             
	    MCR     p15,0x1,r0,c15,c1,0

;------------------------------------------------------------------------------
; Description  : FMI Registers configuration depending on the Flash size selected, 
;                and the boot bank.
;
;     After reset, the application program has to write the size and start
;     address of Bank 1 in the FMI_BBSR and FMI_BBADR registers and the size and 
;     start address of Bank 0 in the FMI_NBBSR and FMI_NBBADR registers.
; ------------------------------------------------------------------------------

        LDR     R6, =FMI_BASE_Address   
        LDR     R7, = FMI_BBSR_Val            ; BOOT BANK Size=
        STR     R7, [R6, #FMI_BBSR_OFST]      ; (2^BOOT_BANK_Size) * 32KBytes       
        LDR     R7, = FMI_NBBSR_Val           ; NON BOOT BANK Size =
        STR     R7, [R6, #FMI_NBBSR_OFST]     ; (2^NON_BOOT_BANK_Size) * 8KBytes
        LDR     R7, =FMI_BBADR_Val            ; BOOT BANK Address
        MOV     R7, R7 ,LSR #0x2   
        STR     R7, [R6, #FMI_BBADR_OFST]
        LDR     R7, =FMI_NBBADR_Val           ; BOOT BANK Address
        MOV     R7, R7 ,LSR #0x2              
        STR     R7, [R6, #FMI_NBBADR_OFST]    
  
       
        LDR     R7, = 0x19                    ; Enable Both banks
        STR     R7, [R6, #FMI_CR_OFST]        

      
; --- Enable 96K of RAM  & Disable DTCM & AHB wait-states

        LDR     R0, = SCU_BASE_Address  
        LDR     R1, = 0x0191
        STR     R1, [R0, #SCU_SCR0_OFST]

; Setup Stack for each mode

                LDR     R0, =Stack_Top

;  Enter Undefined Instruction Mode and set its Stack Pointer
                MSR     CPSR_c, #Mode_UND:OR:I_Bit:OR:F_Bit
                MOV     SP, R0
                SUB     R0, R0, #UND_Stack_Size

;  Enter Abort Mode and set its Stack Pointer
                MSR     CPSR_c, #Mode_ABT:OR:I_Bit:OR:F_Bit
                MOV     SP, R0
                SUB     R0, R0, #ABT_Stack_Size

;  Enter FIQ Mode and set its Stack Pointer
                MSR     CPSR_c, #Mode_FIQ:OR:I_Bit:OR:F_Bit
                MOV     SP, R0
                SUB     R0, R0, #FIQ_Stack_Size

;  Enter IRQ Mode and set its Stack Pointer
                MSR     CPSR_c, #Mode_IRQ:OR:I_Bit:OR:F_Bit
                MOV     SP, R0
                SUB     R0, R0, #IRQ_Stack_Size

;  Enter Supervisor Mode and set its Stack Pointer
                MSR     CPSR_c, #Mode_SVC:OR:I_Bit:OR:F_Bit
                MOV     SP, R0
                SUB     R0, R0, #SVC_Stack_Size

;  Enter User Mode and set its Stack Pointer
                MSR     CPSR_c, #Mode_SYS
			   IF      :DEF:__MICROLIB

                EXPORT __initial_sp

                ELSE

                MOV     SP, R0
                SUB     SL, SP, #USR_Stack_Size

                ENDIF


; Enter the C code

                IMPORT  __main
				 B __main

                IF      :DEF:__MICROLIB

                EXPORT  __heap_base
                EXPORT  __heap_limit

                ELSE
; User Initial Stack & Heap
                AREA    |.text|, CODE, READONLY

                IMPORT  __use_two_region_memory
                EXPORT  __user_initial_stackheap
__user_initial_stackheap

                LDR     R0, =  Heap_Mem
                LDR     R1, =(Stack_Mem + USR_Stack_Size)
                LDR     R2, = (Heap_Mem +      Heap_Size)
                LDR     R3, = Stack_Mem
                BX      LR
                ENDIF


                END
  

;******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****




