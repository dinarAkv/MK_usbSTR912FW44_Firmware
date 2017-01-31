 .global  T0TIMI_Addr
 .global  IRQHandler

; Depending on Your Application, Disable or Enable the following Defines
  ; ----------------------------------------------------------------------------
  ;                      EMI Bank1 configuration
  ; ----------------------------------------------------------------------------
         EMI_INIT .equ 1        ; Initialize EMI pins and configure bank1
  ; ----------------------------------------------------------------------------
  ;                      EIC initialization
  ; ----------------------------------------------------------------------------
         EIC_INIT .equ 1        ; Configure and Initialize EIC
  ; ---------------------------------------------------------------------------- 
  ;                      Peripheral  configuration
  ; ----------------------------------------------------------------------------    
  ; Uncomment next ligne if you need to disable all device peripherals
          PERIPHERAL_INIT .equ 1  ; Disable all device peripherals except EIC   
        
  ; ---------------------------------------------------------------------------- 
  ;                      Memory remapping
  ; ----------------------------------------------------------------------------
  ; If you need to remap memory before entring the main program
  ; uncomment next ligne
           remapping .equ 1           
  ; ----------------------------------------------------------------------------
          remap_ram .equ 1     ; remap SRAM at address 0x00 
  ; ----------------------------------------------------------------------------
         remap_flash .equ 0    ; remap FLASH at address 0x00
        
 		
     
; --- Standard definitions of mode bits and interrupt (I & F) flags in PSRs

Mode_USR        .equ            0x10
Mode_FIQ        .equ            0x11
Mode_IRQ        .equ            0x12
Mode_SVC        .equ            0x13
Mode_ABT        .equ            0x17
Mode_UND        .equ            0x1B
Mode_SYS        .equ            0x1F

I_Bit           .equ            0x80    ; when I bit is set, IRQ is disabled
F_Bit           .equ            0x40    ; when F bit is set, FIQ is disabled

EIC_Base_addr      .equ   0xFFFFF800; EIC base address
ICR_off_addr       .equ   0x00      ; Interrupt Control register offset
CIPR_off_addr      .equ   0x08      ; Current Interrupt Priority Register offset
IVR_off_addr       .equ   0x18      ; Interrupt Vector Register offset
FIR_off_addr       .equ   0x1C      ; Fast Interrupt Register offset
IER_off_addr       .equ   0x20      ; Interrupt Enable Register offset
IPR_off_addr       .equ   0x40      ; Interrupt Pending Bit Register offset
SIR0_off_addr      .equ   0x60      ; Source Interrupt Register 0

EMI_Base_addr      .equ   0x6C000000; EMI base address
BCON0_off_addr     .equ   0x00      ; Bank 0 configuration register offset
BCON1_off_addr     .equ   0x04      ; Bank 1 configuration register offset
BCON2_off_addr     .equ   0x08      ; Bank 2 configuration register offset
BCON3_off_addr     .equ   0x0C      ; Bank 3 configuration register offset

EMI_ENABLE         .equ   0x8000
EMI_SIZE_16        .equ   0x0001

GPIO2_Base_addr    .equ   0xE0005000; GPIO2 base address
PC0_off_addr       .equ   0x00      ; Port Configuration Register 0 offset
PC1_off_addr       .equ   0x04      ; Port Configuration Register 1 offset
PC2_off_addr       .equ   0x08      ; Port Configuration Register 2 offset
PD_off_addr        .equ   0x0C      ; Port Data Register offset

CPM_Base_addr      .equ   0xA0000040; CPM Base Address
BOOTCR_off_addr    .equ   0x10      ; CPM - Boot Configuration Register
FLASH_mask         .equ   0x0000    ; to remap FLASH at 0x0
RAM_mask           .equ   0x0002    ; to remap RAM at 0x0

RCCU_PER_Init      .equ   0x14      ; to init RCCU_PER register to reduce power consumption
RCCU_PER_Base_addr .equ   0xA0000000; RCCU_PER base address
RCCU_PER_off_addr  .equ   0x1C      ; RCCU_PER register offset


; APB Bridge  (System Peripheral)                                                
APB1_base_addr     .equ   0xC0000000          ; APB Bridge1 Base Address
APB2_base_addr     .equ   0xE0000000          ; APB Bridge2 Base Address
CKDIS_off_addr     .equ   0x10                ; APB Bridge1 - Clock Disable  Register
CKDIS1_config_all  .equ   0x27FB              ; To enable/disable clock of all APB1's peripherals
CKDIS2_config_all  .equ   0x1FDD              ; To enable/disable clock of all APB2's peripherals
                                             ; except EIC

EIC_base_addr        .equ    0xFFFFF800; EIC base address.
CICR_off_addr        .equ    0x04      ; Current Interrupt Channel Register.

		
		
		
		
		
		
;*******************************************************************************
;                      Import exception handlers
;*******************************************************************************
         
        ;.extern  Undefined_Handler
        ;.extern  SWI_Handler
        ;.extern  Prefetch_Handler
        ;.extern  Abort_Handler
        ;.extern  FIQ_Handler
		
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

        NOP		; Wait for OSC stabilization
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP

        ;; initialize the stack pointer for each mode
        ;; while keeping interrupts disabled

        msr     cpsr_c,#Mode_FIQ | F_Bit | I_Bit        ; change to FIQ mode
        ldr     sp,=_lc_ub_stack_fiq                    ; initialize FIQ stack pointer

        msr     cpsr_c,#Mode_IRQ | F_Bit | I_Bit        ; change to IRQ mode
        ldr     sp,=_lc_ub_stack_irq                    ; initialize IRQ stack pointer

       
        msr     cpsr_c,#Mode_ABT | F_Bit | I_Bit        ; change to Abort mode
        ldr     sp,=_lc_ub_stack_abt                    ; initialize ABT stack pointer

        msr     cpsr_c,#Mode_UND | F_Bit | I_Bit        ; change to Undefined mode
        ldr     sp,=_lc_ub_stack_und                    ; initialize UND stack pointer

         msr     cpsr_c,#Mode_SVC | F_Bit | I_Bit        ; change to Supervisor mode
        ldr     sp,=_lc_ub_stack_svc                    ; initialize SVC stack pointer
		; ------------------------------------------------------------------------------
; Description  : EMI_INIT. Initialize EMI bank 1: 16-bit 15 wait state
; ------------------------------------------------------------------------------
.if EMI_INIT 
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
.endif

;-------------------------------------------------------------------------------
;Description  : Initialize the EIC as following :
;              - IRQ disabled
;              - FIQ disabled
;              - IVR contains the load PC opcode
;              - All channels are disabled
;              - All channels priority equal to 0
;              - All SIR registers contains offset to the related IRQ table entry
;-------------------------------------------------------------------------------
.if EIC_INIT 
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
        SUB     r4,r0,#8               ; subtract 8 for prefetch
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
.endif

; ------------------------------------------------------------------------------
; Description  : PERIPHERAL_INIT. Disable all device peripherals except EIC.
; ------------------------------------------------------------------------------
.if PERIPHERAL_INIT 
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
.endif   

; ------------------------------------------------------------------------------
; Description  :  Remapping SRAM, FLASH at address 0x00 after the application has 
;                 started executing. 
; ------------------------------------------------------------------------------
.if remapping 
    .if remap_flash  
        MOV     r0, #FLASH_mask
    .endif
    .if remap_ram  
        MOV     r0, #RAM_mask
    .endif

        LDR     r1, =CPM_Base_addr
        LDRH    r2, [r1, #BOOTCR_off_addr]   ; Read BOOTCR Register
        BIC     r2, r2, #0x03                ; Reset the two LSB bits of BOOTCR
        ORR     r2, r2, r0                   ; change the two LSB bits of BOOTCR
        STRH    r2, [r1, #BOOTCR_off_addr]   ; Write BOOTCR Register
.endif

; --- Now change to USR/SYS mode and set up User mode stack,


        msr     cpsr_c,#Mode_SYS                        ; change to System mode
        ldr     sp,=_lc_ub_stack                        ; initialize USR/SYS stack pointer  


;copy the vector table from FLASH to RAM 

     ldr   r1,=0x40004000

     ldr   r2,=0x20000000

     ldr   r3,=0x800

copy_vect:

      
    ldr   r0,[r1]

    str  r0, [r2]

    sub  r3, r3, #4

    add r1, r1, #4

    add r2, r2, #4

   cmp r3,#0

 
   bne   copy_vect
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









;*******************************************************************************
;                   Import IRQ handlers from 71x_it.c
;*******************************************************************************


        ;.extern  T0TIMI_IRQHandler
        ;.extern  FLASH_IRQHandler
        ;.extern  RCCU_IRQHandler
        ;.extern  RTC_IRQHandler
        ;.extern  WDG_IRQHandler
        ;.extern  XTI_IRQHandler
        ;.extern  USBHP_IRQHandler
        ;.extern  I2C0ITERR_IRQHandler
        ;.extern  I2C1ITERR_IRQHandler
        ;.extern  UART0_IRQHandler
        ;.extern  UART1_IRQHandler
        ;.extern  UART2_IRQHandler
        ;.extern  UART3_IRQHandler
        ;.extern  BSPI0_IRQHandler
        ;.extern  BSPI1_IRQHandler
        ;.extern  I2C0_IRQHandler
        ;.extern  I2C1_IRQHandler
        ;.extern  CAN_IRQHandler
        ;.extern  ADC12_IRQHandler
        ;.extern  T1TIMI_IRQHandler
        ;.extern  T2TIMI_IRQHandler
        ;.extern  T3TIMI_IRQHandler
        ;.extern  HDLC_IRQHandler
        .extern  USBLP_IRQHandler
        ;.extern  T0TOI_IRQHandler
        ;.extern  T0OC1_IRQHandler
        ;.extern  T0OC2_IRQHandler

;*******************************************************************************
;            Export Peripherals IRQ handlers table address
;*******************************************************************************

        
         
;*******************************************************************************
;                        Exception vectors
;*******************************************************************************
     .section .text.vectors, at(0x40004000)

           .code32
           .align  4
		   
.Vectors:	      
		
		LDR     PC, Reset_Addr
        LDR     PC, Undefined_Addr
        LDR     PC, SWI_Addr
        LDR     PC, Prefetch_Addr
        LDR     PC, Abort_Addr
        NOP                             ; Reserved vector
        LDR     PC, IRQ_Addr
        LDR     PC, FIQ_Addr

;*******************************************************************************
;               Exception handlers address table
;*******************************************************************************

Reset_Addr      .DW     _START
Undefined_Addr  .DW     0
SWI_Addr        .DW     0
Prefetch_Addr   .DW     0
Abort_Addr      .DW     0
                .DW     0               ; Reserved vector
IRQ_Addr        .DW     IRQHandler
FIQ_Addr        .DW     0

;*******************************************************************************
;              Peripherals IRQ handlers address table
;*******************************************************************************

T0TIMI_Addr     .DW  0
FLASH_Addr      .DW  0
RCCU_Addr       .DW  0
RTC_Addr        .DW  0
WDG_Addr        .DW  0
XTI_Addr        .DW  0
USBHP_Addr      .DW  0
I2C0ITERR_Addr  .DW  0
I2C1ITERR_ADDR  .DW  0
UART0_Addr      .DW  0
UART1_Addr      .DW  0
UART2_ADDR      .DW  0
UART3_ADDR      .DW  0
BSPI0_ADDR      .DW  0
BSPI1_Addr      .DW  0
I2C0_Addr       .DW  0
I2C1_Addr       .DW  0
CAN_Addr        .DW  0
ADC12_Addr      .DW  0
T1TIMI_Addr     .DW  0
T2TIMI_Addr     .DW  0
T3TIMI_Addr     .DW  0
                .DW  0                  ; reserved
                .DW  0                  ; reserved
                .DW  0                  ; reserved
HDLC_Addr       .DW  0
USBLP_Addr      .DW  USBLPIRQHandler
                .DW  0                  ; reserved
                .DW  0                  ; reserved
T0TOI_Addr      .DW  0
T0OC1_Addr      .DW  0
T0OC2_Addr      .DW  0


;*******************************************************************************
;                         Exception Handlers
;*******************************************************************************

;*******************************************************************************
;* Macro Name     : SaveContext
;* Description    : This macro used to save the context before entering
;                   an exception handler.
;* Input          : The range of registers to store.
;* Output         : none
;*******************************************************************************

SaveContext .MACRO   r0,r12 ;[r0,r3]
            ;STMFD  sp!,{r0-r3,lr} ; Save The workspace plus the current return
                                  ; address lr_ mode into the stack.
          
 		 STMFD  sp!,{r0-r12,lr} ; Save The workspace plus the current return
                              ; address lr_ mode into the stack.
        MRS    r1,spsr        ; Save the spsr_mode into r1.
        STMFD  sp!,{r1}       ; Save spsr.
 
         .ENDM

;*******************************************************************************
;* Macro Name     : RestoreContext
;* Description    : This macro is used to restore the context to return from
;                   an exception handler and continue the program execution.
;* Input          : The range of registers to restore.
;* Output         : none
;*******************************************************************************

RestoreContext .MACRO r0,r12

        LDMFD   sp!,{r1}        ; Restore the saved spsr_mode into r1.
        MSR     spsr_cxsf,r1    ; Restore spsr_mode.
        LDMFD   sp!,{r0-r12,pc}^; Return to the instruction following...
                                ; ...the exception interrupt.
        .ENDM



;*******************************************************************************
;* Function Name  : IRQHandler
;* Description    : This function called when IRQ exception is entered.
;* Input          : none
;* Output         : none
;*******************************************************************************

IRQHandler
        SUB    lr,lr,#4       ; Update the link register
        SaveContext r0,r12    ; Save the workspace plus the current
                              ; return address lr_ irq and spsr_irq.
        LDR    lr, =ReturnAddress ; Read the return address.
        LDR    r0, =EIC_base_addr
        LDR    r1, =IVR_off_addr
        ADD    pc,r0,r1      ; Branch to the IRQ handler.
ReturnAddress:
                             ; Clear pending bit in EIC (using the proper IPRx)
        LDR    r0, =EIC_base_addr
        LDR    r2, [r0, #CICR_off_addr] ; Get the IRQ channel number.
        MOV    r3,#1
        MOV    r3,r3,LSL r2
        STR    r3,[r0, #IPR_off_addr]   ; Clear the corresponding IPR bit.
        RestoreContext r0,r12  ; Restore the context and return to the...
                               ; ...program execution.

;*******************************************************************************
;* Macro Name     : IRQ_to_SYS
;* Description    : This macro used to switch form IRQ mode to SYS mode
;* Input          : none.
;* Output         : none
;*******************************************************************************
IRQ_to_SYS .MACRO
        MSR    cpsr_c,#0x1F   ; Switch to SYS mode
        STMFD  sp!,{lr}       ; Save the link register.
       .ENDM
;*******************************************************************************
;* Macro Name     : SYS_to_IRQ
;* Description    : This macro used to switch from SYS mode to IRQ mode
;                   then to return to IRQHnadler routine.
;* Input          : none.
;* Output         : none.
;*******************************************************************************
SYS_to_IRQ .MACRO
        LDMFD  sp!,{lr}      ; Restore the link register.
        MSR    cpsr_c,#0xD2  ; Switch to IRQ mode.
        MOV    pc,lr         ; Return to IRQHandler routine to clear the
                             ; pending bit.
       .ENDM
;*******************************************************************************
;* Function Name  : USBLPIRQHandler
;* Description    : This function used to switch to SYS mode before entering
;                   the USBLP_IRQHandler function located in 71x_it.c.
;                   Then to return to IRQ mode after the
;                   USBLP_IRQHandler function termination.
;* Input          : none
;* Output         : none
;*******************************************************************************
USBLPIRQHandler:
        IRQ_to_SYS
        ldr r0,=USBLP_IRQHandler
        ldr lr,=USBLP_IRQHandler_end
        bx  r0
USBLP_IRQHandler_end:
        SYS_to_IRQ
  
  	
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
