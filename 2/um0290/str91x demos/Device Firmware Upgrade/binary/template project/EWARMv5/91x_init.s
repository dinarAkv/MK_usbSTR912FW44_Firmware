/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
;* File Name          :91x_init.s
;* Author             : MCD Application Team
;* Version            : V2.0.0.0
;* Date               : 08/08/2008
;* Description        : This module performs:
;*                      - FLASH/RAM initialization,
;*                      - Stack pointer initialization for each mode ,
;*                      - Branches to ?main in the C library (which eventually 
;*                        calls main()).
;*		      On reset, the ARM core starts up in Supervisor (SVC) mode,
;*		      in ARM state,with IRQ and FIQ disabled.
;********************************************************************************
;* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
;* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
;* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
;* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
;* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
;* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
;*******************************************************************************/
;Uncomment Only ONE of the following lines to select your STR91xFA Flash size 
;and the Boot bank (Bank0 or Bank1).You have also to uncomment the appropriate 
;defines in "91x_conf.h" file.
;In this File/library the default size is 512Kbytes  and the boot bank is Bank0

;#define Flash_256KB_Bank0_Boot
;#define Flash_512KB_Bank0_Boot
;#define Flash_1MB_Bank0_Boot
;#define Flash_2MB_Bank0_Boot

;#define Flash_256KB_Bank1_Boot
;#define Flash_512KB_Bank1_Boot
;#define Flash_2MB_Bank1_Boot
;#define Flash_1MB_Bank1_Boot


; Uncomment the following define when working in debug mode you have also to 
; Uncomment the same define in "91x_conf.h" file

#define debug_mode


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



#ifdef Flash_2MB_Bank0_Boot

BOOT_BANK_Size        EQU       0x6 ; Boot Bank Size Register
NON_BOOT_BANK_Size    EQU       0x4 ; Non-boot Bank Size Register
BOOT_BANK_Address     EQU       0x00000000 ; Boot Bank Base Address Register
NON_BOOT_BANK_Address EQU       0x00200000 ; Non-boot Bank Base Address Register

#endif

#ifdef Flash_2MB_Bank1_Boot

BOOT_BANK_Size        EQU        0x2 ; Boot Bank Size Register
NON_BOOT_BANK_Size    EQU        0x8 ; Non-boot Bank Size Register
BOOT_BANK_Address     EQU        0x00000000 ; Boot Bank Base Address Register
NON_BOOT_BANK_Address EQU        0x00200000 ; Non-boot Bank Base Address Register

#endif

#ifdef Flash_1MB_Bank0_Boot

BOOT_BANK_Size         EQU       0x5 ; Boot Bank Size Register
NON_BOOT_BANK_Size     EQU       0x4 ; Non-boot Bank Size Register
BOOT_BANK_Address      EQU       0x00000000 ; Boot Bank Base Address Register
NON_BOOT_BANK_Address  EQU       0x00200000 ; Non-boot Bank Base Address Register

#endif

#ifdef Flash_1MB_Bank1_Boot

BOOT_BANK_Size           EQU     0x2 ; Boot Bank Size Register
NON_BOOT_BANK_Size       EQU     0x7 ; Non-boot Bank Size Register
BOOT_BANK_Address        EQU     0x00000000 ; Boot Bank Base Address Register
NON_BOOT_BANK_Address    EQU     0x00200000 ; Non-boot Bank Base Address Register

#endif

#ifdef Flash_512KB_Bank0_Boot

BOOT_BANK_Size           EQU     0x4 ; Boot Bank Size Register
NON_BOOT_BANK_Size       EQU     0x2 ; Non-boot Bank Size Register
BOOT_BANK_Address        EQU     0x00000000 ; Boot Bank Base Address Register
NON_BOOT_BANK_Address    EQU     0x00080000 ; Non-boot Bank Base Address Register

#endif

#ifdef Flash_512KB_Bank1_Boot

BOOT_BANK_Size        EQU       0x0 ; Boot Bank Size Register
NON_BOOT_BANK_Size    EQU       0x6 ; Non-boot Bank Size Register
BOOT_BANK_Address     EQU       0x00000000 ; Boot Bank Base Address Register
NON_BOOT_BANK_Address EQU       0x00080000 ; Non-boot Bank Base Address Register


#endif

#ifdef Flash_256KB_Bank0_Boot

BOOT_BANK_Size        EQU      0x3 ; Boot Bank Size Register
NON_BOOT_BANK_Size    EQU      0x2 ; Non-boot Bank Size Register
BOOT_BANK_Address     EQU      0x00000000 ; Boot Bank Base Address Register
NON_BOOT_BANK_Address EQU      0x00080000 ; Non-boot Bank Base Address Register

#endif

#ifdef Flash_256KB_Bank1_Boot

BOOT_BANK_Size        EQU      0x0 ; Boot Bank Size Register
NON_BOOT_BANK_Size    EQU      0x5 ; Non-boot Bank Size Register
BOOT_BANK_Address     EQU      0x00000000 ; Boot Bank Base Address Register
NON_BOOT_BANK_Address EQU      0x00080000 ; Non-boot Bank Base Address Register

#endif

;---------------------------------------------------------------
; ?program_start
;---------------------------------------------------------------
       MODULE  ?program_start       
       SECTION	   IRQ_STACK:DATA:NOROOT(3)
       SECTION	   FIQ_STACK:DATA:NOROOT(3)
       SECTION	   UND_STACK:DATA:NOROOT(3)
       SECTION	   ABT_STACK:DATA:NOROOT(3)		
       SECTION	   SVC_STACK:DATA:NOROOT(3)
       SECTION	   CSTACK:DATA:NOROOT(3)
       SECTION .icode:CODE:NOROOT(2)
       PUBLIC __iar_program_start
       EXTERN  ?main
       CODE32     
                

__iar_program_start:
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
         
#ifdef  debug_mode               
                                                                  
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
#endif

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

      /*  LDR     R6, =FMI_BASE_Address   
        LDR     R7, = BOOT_BANK_Size          ; BOOT BANK Size=
        STR     R7, [R6, #FMI_BBSR_OFST]      ; (2^BOOT_BANK_Size) * 32KBytes       
        LDR     R7, = NON_BOOT_BANK_Size      ; NON BOOT BANK Size =
        STR     R7, [R6, #FMI_NBBSR_OFST]     ; (2^NON_BOOT_BANK_Size) * 8KBytes
        LDR     R7, =BOOT_BANK_Address        ; BOOT BANK Address
        MOV     R7, R7 ,LSR #0x2   
        STR     R7, [R6, #FMI_BBADR_OFST]
        LDR     R7, =NON_BOOT_BANK_Address    ; BOOT BANK Address
        MOV     R7, R7 ,LSR #0x2              
        STR     R7, [R6, #FMI_NBBADR_OFST]    
  
       
        LDR     R7, = 0x19                    ; Enable Both banks
        STR     R7, [R6, #FMI_CR_OFST]    */    
                                              
      
; --- Enable 96K of RAM  & Disable DTCM & AHB wait-states

        LDR     R0, = SCU_BASE_Address  
        LDR     R1, = 0x0191
        STR     R1, [R0, #SCU_SCR0_OFST]

; --- Initialize Stack pointer registers

; Enter each mode in turn and set up the stack pointer
        
       MSR     CPSR_c, #Mode_FIQ|I_Bit|F_Bit    ; No interrupts
       LDR     SP, =SFE(FIQ_STACK) 

       MSR     CPSR_c, #Mode_IRQ|I_Bit|F_Bit    ; No interrupts
       LDR     SP, = SFE(IRQ_STACK) 

       MSR     CPSR_c, #Mode_ABT|I_Bit|F_Bit    ; No interrupts
       LDR     SP, = SFE(ABT_STACK) 
	
       MSR     CPSR_c, #Mode_UND|I_Bit|F_Bit    ; No interrupts
       LDR     SP, = SFE(UND_STACK) 

       MSR     CPSR_c, #Mode_SVC|I_Bit|F_Bit    ; No interrupts
       LDR     SP, = SFE(SVC_STACK) 
    
     
; --- Now change to USR/SYS mode and set up User mode stack,
        MSR     CPSR_c, #Mode_SYS               ; IRQs & FIQs are now enabled
        LDR     SP, = SFE(CSTACK) 

; --- Now enter the C code
        B       ?main   ; Note : use B not BL, because an application will
                         ; never return this way

        LTORG


        END
        
;******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****





