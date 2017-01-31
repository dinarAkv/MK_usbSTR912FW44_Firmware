;******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
;* File Name          : 71x_init.s
;* Author             : MCD Application Team
;* Version            : V2.0.0
;* Date               : 09/29/2008
;* Description        : This module performs:
;*                      - Memory remapping (if required),
;*                      - Stack pointer initialisation for each mode ,
;*                      - Interrupt Controller Initialisation
;*                      - Branches to ?main in the C library (which eventually
;*                        calls main()).
;*			On reset, the ARM core starts up in Supervisor (SVC) mode,
;*			in ARM state,with IRQ and FIQ disabled.
;*******************************************************************************
; THE PRESENT SOFTWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
; WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
; AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
; INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
; CONTENT OF SUCH SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
; INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
;*******************************************************************************        

        IMPORT  T0TIMI_Addr
        
        EXPORT __user_initial_stackheap
   	    EXPORT __initial_sp

      	EXPORT  Reset_Handler
	  	
  
  ; Depending on Your Application, Disable or Enable the following Defines
  ; ----------------------------------------------------------------------------
  ;                      EMI Bank1 configuration
  ; ----------------------------------------------------------------------------
              ;GBLL EMI_INIT      ; Initialize EMI pins and configure bank1
  ; ----------------------------------------------------------------------------
  ;                      EIC initialization
  ; ----------------------------------------------------------------------------
               GBLL EIC_INIT      ; Configure and Initialize EIC
  ; ----------------------------------------------------------------------------
  ;                      Peripheral  configuration
  ; ----------------------------------------------------------------------------    
  ; Uncomment next ligne if you need to disable all device peripherals
              GBLL PERIPHERAL_INIT ; Disable all device peripherals	except EIC
                      
  ; ----------------------------------------------------------------------------
  ;                      Memory remapping
  ; ----------------------------------------------------------------------------
  ; If you need to remap memory before entring the main program
  ; uncomment next ligne
              GBLL  remapping          
  ; ----------------------------------------------------------------------------
              GBLL  remap_ram    ; remap SRAM at address 0x00 
  ; ----------------------------------------------------------------------------
              ;GBLL  remap_flash  ; remap FLASH at address 0x00
   
       
; --- Standard definitions of mode bits and interrupt (I & F) flags in PSRs       

Mode_USR            EQU    0x10
Mode_FIQ            EQU    0x11
Mode_IRQ            EQU    0x12
Mode_SVC            EQU    0x13
Mode_ABT            EQU    0x17
Mode_UNDEF          EQU    0x1B
Mode_SYS            EQU    0x1F ; available on ARM Arch 4 and later

I_Bit               EQU    0x80 ; when I bit is set, IRQ is disabled
F_Bit               EQU    0x40 ; when F bit is set, FIQ is disabled


; --- System memory locations

RAM_Base            EQU    0x20000000
RAM_Limit           EQU    0x20010000
SRAM_Base           EQU    0x60000000
Stack_Base          EQU    RAM_Limit
;// <h> Stack Configuration (Stack Sizes in Bytes)
;//   <o0> Undefined Mode      <0x0-0xFFFFFFFF:8>
;//   <o1> Supervisor Mode     <0x0-0xFFFFFFFF:8>
;//   <o2> Abort Mode          <0x0-0xFFFFFFFF:8>
;//   <o3> Fast Interrupt Mode <0x0-0xFFFFFFFF:8>
;//   <o4> Interrupt Mode      <0x0-0xFFFFFFFF:8>
;//   <o5> User/System Mode    <0x0-0xFFFFFFFF:8>
;// </h>

USR_Stack_Length    EQU    4096
IRQ_Stack_Length    EQU    1024
SVC_Stack_Length    EQU    256
FIQ_Stack_Length    EQU    256
ABT_Stack_Length    EQU    256
UNDEF_Stack_Length  EQU    256

ISR_Stack_Size  EQU     (USR_Stack_Length + IRQ_Stack_Length + SVC_Stack_Length + \
                         FIQ_Stack_Length + ABT_Stack_Length + UNDEF_Stack_Length)
USR_Stack           EQU    Stack_Base                 ; USR stack
IRQ_Stack           EQU    USR_Stack-USR_Stack_Length ; followed by IRQ stack
SVC_Stack           EQU    IRQ_Stack-IRQ_Stack_Length ; followed by SVC stack
FIQ_Stack           EQU    SVC_Stack-SVC_Stack_Length ; followed by FIQ stack
ABT_Stack           EQU    FIQ_Stack-FIQ_Stack_Length ; followed by ABT stack
UNDEF_Stack         EQU    ABT_Stack-ABT_Stack_Length ; followed by UNDEF stack

                AREA    STACK, NOINIT, READWRITE, ALIGN=3

Stack_Mem       SPACE   USR_Stack_Length
__initial_sp    SPACE   ISR_Stack_Size

Stack_Top


;// <h> Heap Configuration
;//   <o>  Heap Size (in Bytes) <0x0-0xFFFFFFFF>
;// </h>

Heap_Size       EQU     0x00000400

                AREA    HEAP, NOINIT, READWRITE, ALIGN=3
__heap_base
Heap_Mem        SPACE   Heap_Size
__heap_limit

EIC_Base_addr       EQU    0xFFFFF800; EIC base address
ICR_off_addr        EQU    0x00      ; Interrupt Control register offset
CIPR_off_addr       EQU    0x08      ; Current Interrupt Priority Register offset
IVR_off_addr        EQU    0x18      ; Interrupt Vector Register offset
FIR_off_addr        EQU    0x1C      ; Fast Interrupt Register offset
IER_off_addr        EQU    0x20      ; Interrupt Enable Register offset
IPR_off_addr        EQU    0x40      ; Interrupt Pending Bit Register offset
SIR0_off_addr       EQU    0x60      ; Source Interrupt Register 0

EMI_Base_addr       EQU    0x6C000000; EMI base address
BCON0_off_addr      EQU    0x00      ; Bank 0 configuration register offset
BCON1_off_addr      EQU    0x04      ; Bank 1 configuration register offset
BCON2_off_addr      EQU    0x08      ; Bank 2 configuration register offset
BCON3_off_addr      EQU    0x0C      ; Bank 3 configuration register offset

EMI_ENABLE          EQU    0x8000
EMI_SIZE_16         EQU    0x0001

GPIO2_Base_addr     EQU    0xE0005000; GPIO2 base address
PC0_off_addr        EQU    0x00      ; Port Configuration Register 0 offset
PC1_off_addr        EQU    0x04      ; Port Configuration Register 1 offset
PC2_off_addr        EQU    0x08      ; Port Configuration Register 2 offset
PD_off_addr         EQU    0x0C      ; Port Data Register offset

CPM_Base_addr       EQU    0xA0000040; CPM Base Address
BOOTCR_off_addr     EQU    0x10      ; CPM - Boot Configuration Register
FLASH_mask          EQU    0x0000    ; to remap FLASH at 0x0
RAM_mask            EQU    0x0002    ; to remap RAM at 0x0
EXTMEM_mask         EQU    0x0003    ; to remap EXTMEM at 0x0

RCCU_PER_Init      EQU   0x14      ; to init RCCU_PER register to reduce power consumption
RCCU_PER_Base_addr EQU   0xA0000000; RCCU_PER base address
RCCU_PER_off_addr  EQU   0x1C      ; RCCU_PER register offset

; APB Bridge  (System Peripheral) 
APB1_base_addr      EQU    0xC0000000          ; APB Bridge1 Base Address
APB2_base_addr      EQU    0xE0000000          ; APB Bridge2 Base Address
CKDIS_off_addr      EQU    0x10                ; APB Bridge1 - Clock Disable  Register
CKDIS1_config_all   EQU    0x27FB              ; To disable clock of all APB1's peripherals
CKDIS2_config_all   EQU    0x1FDD              ; To disable clock of all APB2's peripherals
                                               ; except EIC    
 

        PRESERVE8
        AREA   RESET, CODE, READONLY

Reset_Handler
         LDR     pc, =NextInst
NextInst
		NOP		; Wait for OSC stabilization
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
      
; ------------------------------------------------------------------------------
; Description  : This delay is added to let JTAG debuggers able to connect
;                to STR71x micro then load the program even if a previously 
;                flash code is halting the CPU core. This may happen when the
;                flash content is corrupt by containing a "bad" code like entering
;                soon to IDLE or SLEEP Low power modes.
; ------------------------------------------------------------------------------
   
	  MOV     r0, #0x10000        
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

        MSR     CPSR_c, #Mode_FIQ:OR:I_Bit:OR:F_Bit ; No interrupts
        LDR     SP, = FIQ_Stack & 0xFFFFFFF8        ; End of FIQ_STACK

        MSR     CPSR_c, #Mode_IRQ:OR:I_Bit:OR:F_Bit ; No interrupts
        LDR     SP, = IRQ_Stack & 0xFFFFFFF8        ; End of FIQ_STACK

        MSR     CPSR_c, #Mode_ABT:OR:I_Bit:OR:F_Bit ; No interrupts
        LDR     SP, = ABT_Stack & 0xFFFFFFF8        ; End of FIQ_STACK

        MSR     CPSR_c, #Mode_UNDEF:OR:I_Bit:OR:F_Bit ; No interrupts
        LDR     SP, = UNDEF_Stack & 0xFFFFFFF8        ; End of FIQ_STACK

        MSR     CPSR_c, #Mode_SVC:OR:I_Bit:OR:F_Bit ; No interrupts
        LDR     SP, = SVC_Stack & 0xFFFFFFF8        ; End of FIQ_STACK  

; ------------------------------------------------------------------------------
; Description  : EMI_INIT. Initialize EMI bank 1: 16-bit 15 wait state
; ------------------------------------------------------------------------------
  IF :DEF: EMI_INIT
        LDR     r0, =GPIO2_Base_addr     ; Configure P2.0 to P2.3 in AF_PP mode
        LDR     r2, [r0, #PC0_off_addr]
        ORR     r2, r2,#0x0000000F
        STR     r2, [r0, #PC0_off_addr]
        LDR     r2, [r0, #PC1_off_addr]
        ORR     r2, r2,#0x0000000F
        STR     r2, [r0, #PC1_off_addr]
        LDR     r2, [r0, #PC2_off_addr]
        ORR     r2, r2,#0x0000000F
        STR     r2, [r0, #PC2_off_addr]
        LDR     r0, =EMI_Base_addr
        LDR     r1, =EMI_ENABLE|EMI_SIZE_16
        STR     r1, [r0, #BCON0_off_addr] ; Enable bank 1 16-bit 15 wait states
  ENDIF
  
;-------------------------------------------------------------------------------
;Description  : Initialize the EIC as following :
;              - IRQ disabled
;              - FIQ disabled
;              - IVR contains the load PC opcode
;              - All channels are disabled
;              - All channels priority equal to 0
;              - All SIR registers contains offset to the related IRQ table entry
;-------------------------------------------------------------------------------
  IF :DEF: EIC_INIT
        LDR     r3, =EIC_Base_addr
        LDR     r4, =0xE59F0000

        STR     r4, [r3, #IVR_off_addr]; Write the LDR pc,[pc,#offset]
                                       ; instruction code in IVR[31:16]
        LDR     r2, =32                ; 32 Channel to initialize
        LDR     r0, =T0TIMI_Addr       ; Read the address of the IRQs
                                       ; address table
        LDR     r1, =0x00000FFF
        AND     r0,r0,r1
        LDR     r5, =SIR0_off_addr     ; Read SIR0 address
        SUB     r4,r0,#8               ; Subtract 8 for prefetch
        LDR     r1, =0xF7E8            ; Add the offset from IVR to 0x00000000
                                       ; address(IVR address + 7E8 = 0x00000000)
                                       ; 0xF7E8 used to complete the
                                       ; LDR pc,[pc,#offset] opcode (0xE59FFXXX)
        ADD     r1,r4,r1               ; Compute the jump offset from IVR to the
                                       ; IRQ table entry.
EIC_INI MOV     r4, r1, LSL #16        ; Left shift the result
        STR     r4, [r3, r5]           ; Store the result in SIRx register
        ADD     r1, r1, #4             ; Next IRQ address
        ADD     r5, r5, #4             ; Next SIR
        SUBS    r2, r2, #1             ; Decrement the number of SIR registers
                                       ; to initialize
        BNE     EIC_INI                ; If more then continue
  ENDIF
  
; ------------------------------------------------------------------------------
; Description  : PERIPHERAL_INIT. Disable all device peripherals except EIC.
; ------------------------------------------------------------------------------
  IF :DEF: PERIPHERAL_INIT
        LDR     r1, =APB1_base_addr          ; r1= APB1 base address
        LDR     r2, =APB2_base_addr          ; r2= APB2 base address
        LDR     r0, =CKDIS1_config_all
        STRH    r0, [r1, #CKDIS_off_addr]    ; Disable clock for all APB1 peripherals
        LDR     r0, =CKDIS2_config_all
        STRH    r0, [r2, #CKDIS_off_addr]    ; Disable clock for all APB2 peripherals
        LDR     r1, =RCCU_PER_Base_addr      ; r1= RCCU_PER base address
        LDR     r0, =RCCU_PER_Init           
        STRH    r0, [r1, #RCCU_PER_off_addr] ; To reduce power consumption clear all
                                             ; unused bits in the RCCU_PER register
                                             ; and keep EMI and USB KERNEL enabled                                         
  ENDIF
  
; ------------------------------------------------------------------------------
; Description  :  Remapping SRAM, FLASH at address 0x00 after the application has 
;                 started executing. 
; ------------------------------------------------------------------------------
  IF :DEF: remapping
    IF :DEF: remap_flash
        MOV     r0, #FLASH_mask
    ENDIF
    IF :DEF: remap_ram
        MOV     r0, #RAM_mask
    ENDIF
    IF :DEF: remap_ext
        MOV     r0, #EXTMEM_mask
    ENDIF
        LDR     r1, =CPM_Base_addr
        LDRH    r2, [r1, #BOOTCR_off_addr]; Read BOOTCR Register
        BIC     r2, r2, #0x03             ; Reset the two LSB bits of BOOTCR
        ORR     r2, r2, r0                ; change the two LSB bits of BOOTCR
        STRH    r2, [r1, #BOOTCR_off_addr]; Write BOOTCR Register
  ENDIF

; --- Now change to USR/SYS mode and set up User mode stack,

        MSR     CPSR_c, #Mode_SYS               ; IRQs & FIQs are now enabled
        LDR     SP, =USR_Stack                 ; Initialize USR stack pointer

; --- Branch to C Library entry point
      
                IMPORT  __main
                B       __main
       

; Implementation of __user_initial_stackheap 

__user_initial_stackheap    

    LDR   r0, = RAM_Limit - Heap_Size
    MOV   pc,lr 


    
        LTORG


        END
;******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****
