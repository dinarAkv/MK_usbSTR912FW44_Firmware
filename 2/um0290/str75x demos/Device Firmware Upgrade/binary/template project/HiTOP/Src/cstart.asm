      .global  WAKUP_Addr                 ; imported from 75x_vect.s

  ; Depending on Your Application, Disable or Enable the following Defines
  ; ----------------------------------------------------------------------------
  ;                      SMI Bank0 configuration
  ; ----------------------------------------------------------------------------
      ; If you need to accees the SMI Bank0
      ; uncomment next line
        SMI_Bank0_EN  .equ 0

  ; ----------------------------------------------------------------------------
  ;                      Memory remapping
  ; ----------------------------------------------------------------------------
        Remap_SRAM .equ 1   ; remap SRAM at address 0x00
 
  ; ----------------------------------------------------------------------------
  ;                      EIC initialization
  ; ----------------------------------------------------------------------------
        EIC_INIT  .equ 1       ; Configure and Initialize EIC

; Standard definitions of mode bits and interrupt (I & F) flags in PSRs
Mode_USR    .equ    0x10
Mode_FIQ    .equ    0x11
Mode_IRQ    .equ    0x12
Mode_SVC    .equ    0x13
Mode_ABT    .equ    0x17
Mode_UND    .equ    0x1B
Mode_SYS    .equ    0x1F

I_Bit       .equ    0x80  ; when I bit is set, IRQ is disabled
F_Bit       .equ    0x40  ; when F bit is set, FIQ is disabled


; MRCC Register
MRCC_PCLKEN_Addr    .equ    0x60000030  ; Peripheral Clock Enable register base address

; CFG Register
CFG_GLCONF_Addr     .equ    0x60000010  ; Global Configuration register base address
SRAM_mask           .equ    0x0002      ; to remap RAM at 0x0

; GPIO Register
GPIOREMAP0R_Addr    .equ    0xFFFFE420
SMI_EN_Mask         .equ    0x00000001

; SMI Register
SMI_CR1_Addr        .equ    0x90000000

; EIC Registers offsets
EIC_base_addr    .equ    0xFFFFF800  ; EIC base address
ICR_off_addr     .equ    0x00        ; Interrupt Control register offset
CIPR_off_addr    .equ    0x08        ; Current Interrupt Priority Register offset
IVR_off_addr     .equ    0x18        ; Interrupt Vector Register offset
FIR_off_addr     .equ    0x1C        ; Fast Interrupt Register offset
IER_off_addr     .equ    0x20        ; Interrupt Enable Register offset
IPR_off_addr     .equ    0x40        ; Interrupt Pending Bit Register offset
SIR0_off_addr    .equ    0x60        ; Source Interrupt Register 0

EIC_Base_addr    .equ    0xFFFFF800 ; EIC base address
CICR_off_addr    .equ    0x04       ; Current Interrupt Channel Register
;IVR_off_addr     .equ    0x18       ; Interrupt Vector Register
;IPR_off_addr     .equ    0x40       ; Interrupt Pending Register





;*******************************************************************************
;                      Import exception handlers
;*******************************************************************************
         
        .extern  Undefined_Handler
        .extern  SWI_Handler
        .extern  Prefetch_Handler
        .extern  Abort_Handler
        .extern  FIQ_Handler
		
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
; Reset all Peripheral Clocks
; This is usefull only when using debugger to Reset\Run the application

     .if SMI_Bank0_EN
        LDR     r0, =0x01000000          ; Disable peripherals clock (except GPIO)
     .else
        LDR     r0, =0x00000000          ; Disable peripherals clock        
     .endif
        LDR     r1, =MRCC_PCLKEN_Addr
        STR     r0, [r1]

     .if SMI_Bank0_EN
        LDR     r0, =0x1875623F          ; Peripherals kept under reset (except GPIO)
     .else
        LDR     r0, =0x1975623F          ; Peripherals kept under reset
     .endif     
           
        STR     r0, [r1,#4]              
        MOV     r0, #0
        NOP                              ; Wait
        NOP
        NOP
        NOP
        STR     r0, [r1,#4]              ; Disable peripherals reset

; Initialize stack pointer registers
; Enter each mode in turn and set up the stack pointer


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
; Description  :  Enable SMI Bank0: enable GPIOs clock in MRCC_PCLKEN register, 
;                 enable SMI alternate function in GPIO_REMAP register and enable 
;                 Bank0 in SMI_CR1 register.
; ------------------------------------------------------------------------------
  .if SMI_Bank0_EN
        MOV     r0, #0x01000000
        LDR     r1, =MRCC_PCLKEN_Addr
        STR     r0, [r1]                 ; Enable GPIOs clock

        LDR     r1, =GPIOREMAP0R_Addr
        MOV     r0, #SMI_EN_Mask
        LDR     r2, [r1]
        ORR     r2, r2, r0
        STR     r2, [r1]                 ; Enable SMI alternate function  

        LDR     r0, =0x251               ; SMI Bank0 enabled, Prescaler = 2, Deselect Time = 5
        LDR     r1, =SMI_CR1_Addr
        STR     r0, [r1]                 ; Configure CR1 register 
        LDR     r0, =0x00
        STR     r0, [r1,#4]              ; Reset CR2 register             
  .endif

; ------------------------------------------------------------------------------
; Description  :  Remapping SRAM at address 0x00 after the application has 
;                 started executing. 
; ------------------------------------------------------------------------------
 .if  Remap_SRAM
        MOV     r0, #SRAM_mask
        LDR     r1, =CFG_GLCONF_Addr
        LDR     r2, [r1]                  ; Read GLCONF Register
        BIC     r2, r2, #0x03             ; Reset the SW_BOOT bits
        ORR     r2, r2, r0                ; Change the SW_BOOT bits
        STR     r2, [r1]                  ; Write GLCONF Register
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
        LDR     r4, =0x00000000
        STR     r4, [r3, #ICR_off_addr]   ; Disable FIQ and IRQ
        STR     r4, [r3, #IER_off_addr]   ; Disable all interrupts channels

        LDR     r4, =0xFFFFFFFF
        STR     r4, [r3, #IPR_off_addr]   ; Clear all IRQ pending bits

        LDR     r4, =0x18
        STR     r4, [r3, #FIR_off_addr]   ; Disable FIQ channels and clear FIQ pending bits

        LDR     r4, =0x00000000
        STR     r4, [r3, #CIPR_off_addr]  ; Reset the current priority register

        LDR     r4, =0xE59F0000           ; Write the LDR pc,pc,#offset..
        STR     r4, [r3, #IVR_off_addr]   ; ..instruction code in IVR[31:16]


        LDR     r2,= 32                   ; 32 Channel to initialize
        LDR     r0, =WAKUP_Addr           ; Read the address of the IRQs address table
        LDR     r1, =0x00000FFF
        AND     r0,r0,r1
        LDR     r5,=SIR0_off_addr         ; Read SIR0 address
        SUB     r4,r0,#8                  ; subtract 8 for prefetch
        LDR     r1, =0xF7E8               ; add the offset to the 0x00 address..
                                          ; ..(IVR address + 7E8 = 0x00)
                                          ; 0xF7E8 used to complete the LDR pc,offset opcode
        ADD     r1,r4,r1                  ; compute the jump offset
EIC_INI
        MOV     r4, r1, LSL #16           ; Left shift the result
        STR     r4, [r3, r5]              ; Store the result in SIRx register
        ADD     r1, r1, #4                ; Next IRQ address
        ADD     r5, r5, #4                ; Next SIR
        SUBS    r2, r2, #1                ; Decrement the number of SIR registers to initialize
        BNE     EIC_INI                   ; If more then continue

 .endif



; --- Now change to USR/SYS mode and set up User mode stack,

      
        msr     cpsr_c,#Mode_SYS                        ; change to System mode
        ldr     sp,=_lc_ub_stack                        ; initialize USR/SYS stack pointer  

;copy the vector table from FLASH to RAM 

     ldr   r1,=0x20004000

     ldr   r2,=0x40000000

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


        .extern WAKUP_IRQHandler
        .extern TIM2_OC2_IRQHandler
        .extern TIM2_OC1_IRQHandler
        .extern TIM2_IC12_IRQHandler
        .extern TIM2_UP_IRQHandler
        .extern TIM1_OC2_IRQHandler
        .extern TIM1_OC1_IRQHandler
        .extern TIM1_IC12_IRQHandler
        .extern TIM1_UP_IRQHandler
        .extern TIM0_OC2_IRQHandler
        .extern TIM0_OC1_IRQHandler
        .extern TIM0_IC12_IRQHandler
        .extern TIM0_UP_IRQHandler
        .extern  PWM_OC123_IRQHandler
        .extern PWM_EM_IRQHandler
        .extern PWM_UP_IRQHandler
        .extern I2C_IRQHandler
        .extern SSP1_IRQHandler
        .extern SSP0_IRQHandler
        .extern UART2_IRQHandler
        .extern UART1_IRQHandler
        .extern UART0_IRQHandler
        .extern CAN_IRQHandler
        .extern USB_LP_IRQHandler
        .extern USB_HP_IRQHandler
        .extern ADC_IRQHandler
        .extern DMA_IRQHandler
        .extern EXTIT_IRQHandler
        .extern MRCC_IRQHandler
        .extern FLASHSMI_IRQHandler
        .extern RTC_IRQHandler
        .extern TB_IRQHandler

        

;*************************************************************************
; Exception Vectors
;*************************************************************************
        .section .text.vectors, at(0x20004000)

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
Undefined_Addr  .DW     UndefinedHandler
SWI_Addr        .DW     SWIHandler
Prefetch_Addr   .DW     PrefetchAbortHandler
Abort_Addr      .DW     DataAbortHandler
                .DW     0               ; Reserved vector
IRQ_Addr        .DW     IRQHandler
FIQ_Addr        .DW     FIQHandler

;*******************************************************************************
;              Peripherals IRQ handlers address table
;*******************************************************************************

WAKUP_Addr         .DW     	WAKUPIRQHandler
TIM2_OC2_Addr      .DW     	TIM2_OC2IRQHandler
TIM2_OC1_Addr      .DW     	TIM2_OC1IRQHandler
TIM2_IC12_Addr     .DW     	TIM2_IC12IRQHandler
TIM2_UP_Addr       .DW     	TIM2_UPIRQHandler
TIM1_OC2_Addr      .DW     	TIM1_OC2IRQHandler
TIM1_OC1_Addr      .DW     	TIM1_OC1IRQHandler
TIM1_IC12_Addr     .DW     	TIM1_IC12IRQHandler
TIM1_UP_Addr       .DW     	TIM1_UPIRQHandler
TIM0_OC2_Addr      .DW     	TIM0_OC2IRQHandler
TIM0_OC1_Addr      .DW     	TIM0_OC1IRQHandler
TIM0_IC12_Addr     .DW     	TIM0_IC12IRQHandler
TIM0_UP_Addr       .DW     	TIM0_UPIRQHandler
PWM_OC123_Addr     .DW     	PWM_OC123IRQHandler
PWM_EM_Addr        .DW     	PWM_EMIRQHandler
PWM_UP_Addr        .DW     	PWM_UPIRQHandler
I2C_Addr           .DW     	I2CIRQHandler
SSP1_Addr          .DW     	SSP1IRQHandler
SSP0_Addr          .DW     	SSP0IRQHandler
UART2_Addr         .DW     	UART2IRQHandler
UART1_Addr         .DW     	UART1IRQHandler
UART0_Addr         .DW     	UART0IRQHandler
CAN_Addr           .DW     	CANIRQHandler
USB_LP_Addr        .DW     	USB_LPIRQHandler
USB_HP_Addr        .DW     	USB_HPIRQHandler
ADC_Addr           .DW     	ADCIRQHandler
DMA_Addr           .DW     	DMAIRQHandler
EXTIT_Addr         .DW     	EXTITIRQHandler
MRCC_Addr          .DW     	MRCCIRQHandler
FLASHSMI_Addr      .DW     	FLASHSMIIRQHandler
RTC_Addr           .DW     	RTCIRQHandler
TB_Addr            .DW     	TBIRQHandler

;*******************************************************************************
;                         Exception Handlers
;*******************************************************************************

;*******************************************************************************
;* Macro Name     : SaveContext
;* Description    : This macro used to save the context before entering
;*                  an exception handler.
;* Input          : The range of registers to store.
;* Output         : none
;*******************************************************************************
SaveContext .MACRO r0,r12
        STMFD  sp!,{r0-r12,lr} ; Save The workspace plus the current return
                              ; address lr_ mode into the stack.
        MRS    r1,spsr        ; Save the spsr_mode into r1.
        STMFD  sp!,{r1}       ; Save spsr.
        .ENDM

;*******************************************************************************
;* Macro Name     : RestoreContext
;* Description    : This macro used to restore the context to return from
;*                  an exception handler and continue the program execution.
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
;* Function Name  : UndefinedHandler
;* Description    : This function called when undefined instruction exception
;*                  is entered.
;* Input          : none
;* Output         : none
;*******************************************************************************
UndefinedHandler
        SaveContext r0,r12         ; Save the workspace plus the current
                                   ; return address lr_ und and spsr_und.
        ldr r0,=Undefined_Handler
        ldr lr,=Undefined_Handler_end
        bx r0                       ;Branch to Undefined_Handler
Undefined_Handler_end:
        RestoreContext r0,r12      ; Return to the instruction following...
                                    ; ...the undefined instruction.

;*******************************************************************************
;* Function Name  : SWIHandler
;* Description    : This function called when SWI instruction executed.
;* Input          : none
;* Output         : none
;*******************************************************************************
SWIHandler
        SaveContext r0,r12         ; Save the workspace plus the current
                                   ; return address lr_ svc and spsr_svc.
        ldr r0,= SWI_Handler
        ldr lr,= SWI_Handler_end
        bx r0                       ;Branch to  SWI_Handler
SWI_Handler_end:
        RestoreContext r0,r12     ; Return to the instruction following...
                                  ; ...the SWI instruction.

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
ReturnAddress
                             ; Clear pending bit in EIC (using the proper IPRx)
        LDR    r0, =EIC_base_addr
        LDR    r2, [r0, #CICR_off_addr] ; Get the IRQ channel number
        MOV    r3,#1
        MOV    r3,r3,LSL r2
        STR    r3,[r0, #IPR_off_addr]   ; Clear the corresponding IPR bit.

        RestoreContext r0,r12  ; Restore the context and return to the...
                               ; ...program execution.

;*******************************************************************************
;* Function Name  : PrefetchAbortHandler
;* Description    : This function called when Prefetch Abort exception is entered.
;* Input          : none
;* Output         : none
;*******************************************************************************
PrefetchAbortHandler
        SUB    lr,lr,#4        ; Update the link register.
        SaveContext r0,r7      ; Save the workspace plus the current
                               ; return address lr_abt and spsr_abt.
        ldr r0,= Prefetch_Handler
        ldr lr,= Prefetch_Handler_end
        bx r0                       ;Branch to  Prefetch_Handler
Prefetch_Handler_end:
        RestoreContext r0,r7   ; Return to the instruction following that...
                               ; ...has generated the prefetch abort exception.

;*******************************************************************************
;* Function Name  : DataAbortHandler
;* Description    : This function is called when Data Abort exception is entered.
;* Input          : none
;* Output         : none
;*******************************************************************************
DataAbortHandler
        SUB    lr,lr,#8            ; Update the link register.
        SaveContext r0,r12         ; Save the workspace plus the current
                                   ; return address lr_ abt and spsr_abt.
        ldr r0,= Abort_Handler
        ldr lr,= Abort_Handler_end
        bx r0                       ;Branch to  Abort_Handler
Abort_Handler_end:
        RestoreContext r0,r12       ; Return to the instruction following that...
                                    ; ...has generated the data abort exception.

;*******************************************************************************
;* Function Name  : FIQHandler
;* Description    : This function is called when FIQ exception is entered.
;* Input          : none
;* Output         : none
;*******************************************************************************
FIQHandler
        SUB    lr,lr,#4            ; Update the link register.
        SaveContext r0,r7          ; Save the workspace plus the current
                                   ; return address lr_ fiq and spsr_fiq.
        ldr r0,= FIQ_Handler
        ldr lr,= FIQ_Handler_end
        bx r0                       ;Branch to  FIQ_Handler
FIQ_Handler_end:
        RestoreContext r0,r7        ; Restore the context and return to the...
                                    ; ...program execution.

;*******************************************************************************
;* Macro Name     : IRQ_to_SYS
;* Description    : This macro used to switch form IRQ mode to SYS mode.
;* Input          : none.
;* Output         : none
;*******************************************************************************
IRQ_to_SYS .MACRO
        MSR    cpsr_c,#0x1F
        STMFD  sp!,{lr}
       .ENDM

;*******************************************************************************
;* Macro Name     : SYS_to_IRQ
;* Description    : This macro used to switch from SYS mode to IRQ mode.
;* Input          : none.
;* Output         : none
;*******************************************************************************
SYS_to_IRQ .MACRO
        LDMFD  sp!,{lr}
        MSR    cpsr_c,#0xD2
        MOV    pc,lr
       .ENDM

;*******************************************************************************
;* Function Name  : WAKUPIRQHandler
;* Description    : This function used to switch to SYS mode before entering
;*                  the WAKUP_IRQHandler function located in 75x_it.c.
;*                  Then to return to IRQ mode after the WAKUP_IRQHandler
;*                  function termination.
;* Input          : none
;* Output         : none
;*******************************************************************************
WAKUPIRQHandler
        IRQ_to_SYS
        ldr r0,=WAKUP_IRQHandler
        ldr lr,=WAKUP_IRQHandler_end
        bx r0
WAKUP_IRQHandler_end:
        SYS_to_IRQ

;*******************************************************************************
;* Function Name  : TIM2_OC2IRQHandler
;* Description    : This function used to switch to SYS mode before entering
;*                  the TIM2_OC2_IRQHandler function located in 75x_it.c.
;*                  Then to return to IRQ mode after the TIM2_OC2_IRQHandler
;*                  function termination.
;* Input          : none
;* Output         : none
;*******************************************************************************
TIM2_OC2IRQHandler
        IRQ_to_SYS
        ldr r0,=TIM2_OC2_IRQHandler
        ldr lr,=TIM2_OC2_IRQHandler_end
        bx r0
TIM2_OC2_IRQHandler_end:
        SYS_to_IRQ

;*******************************************************************************
;* Function Name  : TIM2_OC1IRQHandler
;* Description    : This function used to switch to SYS mode before entering
;*                  the TIM2_OC1_IRQHandler function located in 75x_it.c.
;*                  Then to return to IRQ mode after the TIM2_OC1_IRQHandler
;*                  function termination.
;* Input          : none
;* Output         : none
;*******************************************************************************
TIM2_OC1IRQHandler
        IRQ_to_SYS
        ldr r0,=TIM2_OC1_IRQHandler
        ldr lr,=TIM2_OC1_IRQHandler_end
        bx r0
TIM2_OC1_IRQHandler_end:
        SYS_to_IRQ

;*******************************************************************************
;* Function Name  : TIM2_IC12IRQHandler
;* Description    : This function used to switch to SYS mode before entering
;*                  the TIM2_IC12_IRQHandler function located in 75x_it.c.
;*                  Then to return to IRQ mode after the TIM2_IC12_IRQHandler
;*                  function termination.
;* Input          : none
;* Output         : none
;*******************************************************************************
TIM2_IC12IRQHandler
        IRQ_to_SYS
        ldr r0,=TIM2_IC12_IRQHandler
        ldr lr,=TIM2_IC12_IRQHandler_end
        bx r0
TIM2_IC12_IRQHandler_end:
        SYS_to_IRQ

;*******************************************************************************
;* Function Name  : TIM2_UPIRQHandler
;* Description    : This function used to switch to SYS mode before entering
;*                  the TIM2_UP_IRQHandler function located in 75x_it.c.
;*                  Then to return to IRQ mode after the TIM2_UP_IRQHandler
;*                  function termination.
;* Input          : none
;* Output         : none
;*******************************************************************************
TIM2_UPIRQHandler
        IRQ_to_SYS
        ldr r0,=TIM2_UP_IRQHandler
        ldr lr,=TIM2_UP_IRQHandler_end
        bx r0
TIM2_UP_IRQHandler_end:
        SYS_to_IRQ

;*******************************************************************************
;* Function Name  : TIM1_OC2IRQHandler
;* Description    : This function used to switch to SYS mode before entering
;*                  the TIM1_OC2_IRQHandler function located in 75x_it.c.
;*                  Then to return to IRQ mode after the TIM1_OC2_IRQHandler
;*                  function termination.
;* Input          : none
;* Output         : none
;*******************************************************************************
TIM1_OC2IRQHandler
        IRQ_to_SYS
        ldr r0,=TIM1_OC2_IRQHandler
        ldr lr,=TIM1_OC2_IRQHandler_end
        bx r0
TIM1_OC2_IRQHandler_end:
        SYS_to_IRQ

;*******************************************************************************
;* Function Name  : TIM1_OC1IRQHandler
;* Description    : This function used to switch to SYS mode before entering
;*                  the TIM1_OC1_IRQHandler function located in 75x_it.c.
;*                  Then to return to IRQ mode after the TIM1_OC1_IRQHandler
;*                  function termination.
;* Input          : none
;* Output         : none
;*******************************************************************************
TIM1_OC1IRQHandler
        IRQ_to_SYS
        ldr r0,=TIM1_OC1_IRQHandler
        ldr lr,=TIM1_OC1_IRQHandler_end
        bx r0
TIM1_OC1_IRQHandler_end:
        SYS_to_IRQ

;*******************************************************************************
;* Function Name  : TIM1_IC12IRQHandler
;* Description    : This function used to switch to SYS mode before entering
;*                  the TIM1_IC12_IRQHandler function located in 75x_it.c.
;*                  Then to return to IRQ mode after the TIM1_IC12_IRQHandler
;*                  function termination.
;* Input          : none
;* Output         : none
;*******************************************************************************
TIM1_IC12IRQHandler
        IRQ_to_SYS
        ldr r0,=TIM1_IC12_IRQHandler
        ldr lr,=TIM1_IC12_IRQHandler_end
        bx r0
TIM1_IC12_IRQHandler_end:
        SYS_to_IRQ

;*******************************************************************************
;* Function Name  : TIM1_UPIRQHandler
;* Description    : This function used to switch to SYS mode before entering
;*                  the TIM1_UP_IRQHandler function located in 75x_it.c.
;*                  Then to return to IRQ mode after the TIM1_UP_IRQHandler
;*                  function termination.
;* Input          : none
;* Output         : none
;*******************************************************************************
TIM1_UPIRQHandler
        IRQ_to_SYS
        ldr r0,=TIM1_UP_IRQHandler
        ldr lr,=TIM1_UP_IRQHandler_end
        bx r0
TIM1_UP_IRQHandler_end:
        SYS_to_IRQ

;*******************************************************************************
;* Function Name  : TIM0_OC2IRQHandler
;* Description    : This function used to switch to SYS mode before entering
;*                  the TIM0_OC2_IRQHandler function located in 75x_it.c.
;*                  Then to return to IRQ mode after the TIM0_OC2_IRQHandler
;*                  function termination.
;* Input          : none
;* Output         : none
;*******************************************************************************
TIM0_OC2IRQHandler
        IRQ_to_SYS
        ldr r0,=TIM0_OC2_IRQHandler
        ldr lr,=TIM0_OC2_IRQHandler_end
        bx r0
TIM0_OC2_IRQHandler_end:
        SYS_to_IRQ

;*******************************************************************************
;* Function Name  : TIM0_OC1IRQHandler
;* Description    : This function used to switch to SYS mode before entering
;*                  the TIM0_OC1_IRQHandler function located in 75x_it.c.
;*                  Then to return to IRQ mode after the TIM0_OC1_IRQHandler
;*                  function termination.
;* Input          : none
;* Output         : none
;*******************************************************************************
TIM0_OC1IRQHandler
        IRQ_to_SYS
        ldr r0,=TIM0_OC1_IRQHandler
        ldr lr,=TIM0_OC1_IRQHandler_end
        bx r0
TIM0_OC1_IRQHandler_end:
        SYS_to_IRQ

;*******************************************************************************
;* Function Name  : TIM0_IC12IRQHandler
;* Description    : This function used to switch to SYS mode before entering
;*                  the TIM0_IC12_IRQHandler function located in 75x_it.c.
;*                  Then to return to IRQ mode after the TIM0_IC12_IRQHandler
;*                  function termination.
;* Input          : none
;* Output         : none
;*******************************************************************************
TIM0_IC12IRQHandler
        IRQ_to_SYS
        ldr r0,=TIM0_IC12_IRQHandler
        ldr lr,=TIM0_IC12_IRQHandler_end
        bx r0
TIM0_IC12_IRQHandler_end:
        SYS_to_IRQ

;*******************************************************************************
;* Function Name  : TIM0_UPIRQHandler
;* Description    : This function used to switch to SYS mode before entering
;*                  the TIM0_UP_IRQHandler function located in 75x_it.c.
;*                  Then to return to IRQ mode after the TIM0_UP_IRQHandler
;*                  function termination.
;* Input          : none
;* Output         : none
;*******************************************************************************
TIM0_UPIRQHandler
        IRQ_to_SYS
        ldr r0,=TIM0_UP_IRQHandler
        ldr lr,=TIM0_UP_IRQHandler_end
        bx r0
TIM0_UP_IRQHandler_end:
        SYS_to_IRQ

;*******************************************************************************
;* Function Name  : PWM_OC123IRQHandler
;* Description    : This function used to switch to SYS mode before entering
;*                  the PWM_OC123_IRQHandler function located in 75x_it.c.
;*                  Then to return to IRQ mode after the PWM_OC123_IRQHandler
;*                  function termination.
;* Input          : none
;* Output         : none
;*******************************************************************************
PWM_OC123IRQHandler
        IRQ_to_SYS
        ldr r0,=PWM_OC123_IRQHandler
        ldr lr,=PWM_OC123_IRQHandler_end
        bx r0
PWM_OC123_IRQHandler_end:
        SYS_to_IRQ

;*******************************************************************************
;* Function Name  : PWM_EMIRQHandler
;* Description    : This function used to switch to SYS mode before entering
;*                  the PWM_EM_IRQHandler function located in 75x_it.c.
;*                  Then to return to IRQ mode after the PWM_EM_IRQHandler
;*                  function termination.
;* Input          : none
;* Output         : none
;*******************************************************************************
PWM_EMIRQHandler
        IRQ_to_SYS
        ldr r0,=PWM_EM_IRQHandler
        ldr lr,=PWM_EM_IRQHandler_end
        bx r0
PWM_EM_IRQHandler_end:
        SYS_to_IRQ

;*******************************************************************************
;* Function Name  : PWM_UPIRQHandler
;* Description    : This function used to switch to SYS mode before entering
;*                  the PWM_UP_IRQHandler function located in 75x_it.c.
;*                  Then to return to IRQ mode after the PWM_UP_IRQHandler
;*                  function termination.
;* Input          : none
;* Output         : none
;*******************************************************************************
PWM_UPIRQHandler
        IRQ_to_SYS
        ldr r0,=PWM_UP_IRQHandler
        ldr lr,=PWM_UP_IRQHandler_end
        bx r0
PWM_UP_IRQHandler_end:
        SYS_to_IRQ

;*******************************************************************************
;* Function Name  : I2CIRQHandler
;* Description    : This function used to switch to SYS mode before entering
;*                  the I2C_IRQHandler function located in 75x_it.c.
;*                  Then to return to IRQ mode after the I2C_IRQHandler function
;*                  termination.
;* Input          : none
;* Output         : none
;*******************************************************************************
I2CIRQHandler
        IRQ_to_SYS
        ldr r0,=I2C_IRQHandler
        ldr lr,=I2C_IRQHandler_end
        bx r0
I2C_IRQHandler_end:
        SYS_to_IRQ

;*******************************************************************************
;* Function Name  : SSP1IRQHandler
;* Description    : This function used to switch to SYS mode before entering
;*                  the SSP1_IRQHandler function located in 75x_it.c.
;*                  Then to return to IRQ mode after the SSP1_IRQHandler
;*                  function termination.
;* Input          : none
;* Output         : none
;*******************************************************************************
SSP1IRQHandler
        IRQ_to_SYS
        ldr r0,=SSP1_IRQHandler
        ldr lr,=SSP1_IRQHandler_end
        bx r0
SSP1_IRQHandler_end:
        SYS_to_IRQ

;*******************************************************************************
;* Function Name  : SSP0IRQHandler
;* Description    : This function used to switch to SYS mode before entering
;*                  the SSP0_IRQHandler function located in 75x_it.c.
;*                  Then to return to IRQ mode after the SSP0_IRQHandler
;*                  function termination.
;* Input          : none
;* Output         : none
;*******************************************************************************
SSP0IRQHandler
        IRQ_to_SYS
        ldr r0,=SSP0_IRQHandler
        ldr lr,=SSP0_IRQHandler_end
        bx r0
SSP0_IRQHandler_end:
        SYS_to_IRQ

;*******************************************************************************
;* Function Name  : UART2IRQHandler
;* Description    : This function used to switch to SYS mode before entering
;*                  the UART2_IRQHandler function located in 75x_it.c.
;*                  Then to return to IRQ mode after the UART2_IRQHandler
;*                  function termination.
;* Input          : none
;* Output         : none
;*******************************************************************************
UART2IRQHandler
        IRQ_to_SYS
        ldr r0,=UART2_IRQHandler
        ldr lr,=UART2_IRQHandler_end
        bx r0
UART2_IRQHandler_end:
        SYS_to_IRQ

;*******************************************************************************
;* Function Name  : UART1IRQHandler
;* Description    : This function used to switch to SYS mode before entering
;*                  the UART1_IRQHandler function located in 75x_it.c.
;*                  Then to return to IRQ mode after the UART1_IRQHandler
;*                  function termination.
;* Input          : none
;* Output         : none
;*******************************************************************************
UART1IRQHandler
        IRQ_to_SYS
        ldr r0,=UART1_IRQHandler
        ldr lr,=UART1_IRQHandler_end
        bx r0
UART1_IRQHandler_end:
        SYS_to_IRQ

;*******************************************************************************
;* Function Name  : UART0IRQHandler
;* Description    : This function used to switch to SYS mode before entering
;*                  the UART0_IRQHandler function located in 75x_it.c.
;*                  Then to return to IRQ mode after the UART0_IRQHandler
;*                  function termination.
;* Input          : none
;* Output         : none
;*******************************************************************************
UART0IRQHandler
        IRQ_to_SYS
        ldr r0,=UART0_IRQHandler
        ldr lr,=UART0_IRQHandler_end
        bx r0
UART0_IRQHandler_end:
        SYS_to_IRQ

;*******************************************************************************
;* Function Name  : CANIRQHandler
;* Description    : This function used to switch to SYS mode before entering
;*                  the CAN_IRQHandler function located in 75x_it.c.
;*                  Then to return to IRQ mode after the CAN_IRQHandler function
;*                  termination.
;* Input          : none
;* Output         : none
;*******************************************************************************
CANIRQHandler
        IRQ_to_SYS
        ldr r0,=CAN_IRQHandler
        ldr lr,=CAN_IRQHandler_end
        bx r0
CAN_IRQHandler_end:
        SYS_to_IRQ

;*******************************************************************************
;* Function Name  : USB_LPIRQHandler
;* Description    : This function used to switch to SYS mode before entering
;*                  the USB_LP_IRQHandler function located in 75x_it.c.
;*                  Then to return to IRQ mode after the USB_LP_IRQHandler
;*                  function termination.
;* Input          : none
;* Output         : none
;*******************************************************************************
USB_LPIRQHandler
        IRQ_to_SYS
        ldr r0,=USB_LP_IRQHandler
        ldr lr,=USB_LP_IRQHandler_end
        bx r0
USB_LP_IRQHandler_end:
        SYS_to_IRQ

;*******************************************************************************
;* Function Name  : USB_HPIRQHandler
;* Description    : This function used to switch to SYS mode before entering
;*                  the USB_HP_IRQHandler function located in 75x_it.c.
;*                  Then to return to IRQ mode after the USB_HP_IRQHandler
;*                  function termination.
;* Input          : none
;* Output         : none
;*******************************************************************************
USB_HPIRQHandler
        IRQ_to_SYS
        ldr r0,=USB_HP_IRQHandler
        ldr lr,=USB_HP_IRQHandler_end
        bx r0
USB_HP_IRQHandler_end:
        SYS_to_IRQ

;*******************************************************************************
;* Function Name  : ADCIRQHandler
;* Description    : This function used to switch to SYS mode before entering
;*                  the ADC_IRQHandler function located in 75x_it.c.
;*                  Then to return to IRQ mode after the ADC_IRQHandler
;*                  function termination.
;* Input          : none
;* Output         : none
;*******************************************************************************
ADCIRQHandler
        IRQ_to_SYS
        ldr r0,=ADC_IRQHandler
        ldr lr,=ADC_IRQHandler_end
        bx r0
ADC_IRQHandler_end:
        SYS_to_IRQ

;*******************************************************************************
;* Function Name  : DMAIRQHandler
;* Description    : This function used to switch to SYS mode before entering
;*                  the DMA_IRQHandler function located in 75x_it.c.
;*                  Then to return to IRQ mode after the DMA_IRQHandler
;*                  function termination.
;* Input          : none
;* Output         : none
;*******************************************************************************
DMAIRQHandler
        IRQ_to_SYS
        ldr r0,=DMA_IRQHandler
        ldr lr,=DMA_IRQHandler_end
        bx r0
DMA_IRQHandler_end:
        SYS_to_IRQ

;*******************************************************************************
;* Function Name  : EXTITIRQHandler
;* Description    : This function used to switch to SYS mode before entering
;*                  the EXTIT_IRQHandler function located in 75x_it.c.
;*                  Then to return to IRQ mode after the EXTIT_IRQHandler
;*                  function termination.
;* Input          : none
;* Output         : none
;*******************************************************************************
EXTITIRQHandler
        IRQ_to_SYS
        ldr r0,=EXTIT_IRQHandler
        ldr lr,=EXTIT_IRQHandler_end
        bx r0
EXTIT_IRQHandler_end:
        SYS_to_IRQ

;*******************************************************************************
;* Function Name  : MRCCIRQHandler
;* Description    : This function used to switch to SYS mode before entering
;*                  the MRCC_IRQHandler function located in 75x_it.c.
;*                  Then to return to IRQ mode after the MRCC_IRQHandler
;*                  function termination.
;* Input          : none
;* Output         : none
;*******************************************************************************
MRCCIRQHandler
        IRQ_to_SYS
        ldr r0,=MRCC_IRQHandler
        ldr lr,=MRCC_IRQHandler_end
        bx r0
MRCC_IRQHandler_end:
        SYS_to_IRQ

;*******************************************************************************
;* Function Name  : FLASHSMIIRQHandler
;* Description    : This function used to switch to SYS mode before entering
;*                  the FLASHSMI_IRQHandler function located in 75x_it.c.
;*                  Then to return to IRQ mode after the FLASHSMI_IRQHandler
;*                  function termination.
;* Input          : none
;* Output         : none
;*******************************************************************************
FLASHSMIIRQHandler
        IRQ_to_SYS
        ldr r0,=FLASHSMI_IRQHandler
        ldr lr,=FLASHSMI_IRQHandler_end
        bx r0
FLASHSMI_IRQHandler_end:
        SYS_to_IRQ

;*******************************************************************************
;* Function Name  : RTCIRQHandler
;* Description    : This function used to switch to SYS mode before entering
;*                  the RTC_IRQHandler function located in 75x_it.c.
;*                  Then to return to IRQ mode after the RTC_IRQHandler function
;*                  termination.
;* Input          : none
;* Output         : none
;*******************************************************************************
RTCIRQHandler
        IRQ_to_SYS
        ldr r0,=RTC_IRQHandler
        ldr lr,=RTC_IRQHandler_end
        bx r0
RTC_IRQHandler_end:
        SYS_to_IRQ

;*******************************************************************************
;* Function Name  : TBIRQHandler
;* Description    : This function used to switch to SYS mode before entering
;*                  the TB_IRQHandler function located in 75x_it.c.
;*                  Then to return to IRQ mode after the TB_IRQHandler function
;*                  termination.
;* Input          : none
;* Output         : none
;*******************************************************************************
TBIRQHandler
        IRQ_to_SYS
        ldr r0,=TB_IRQHandler
        ldr lr,=TB_IRQHandler_end
        bx r0
TB_IRQHandler_end:
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
