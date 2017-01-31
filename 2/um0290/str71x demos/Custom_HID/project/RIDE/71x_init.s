/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : 71x_init.s
* Author             : MCD Tools Team
* Version            : V2.0.0
* Date               : 09/29/2008
* Description        : This is the first code executed after RESET.
*                      This code used to initialize system stacks
*                      and critical peripherals before entering the C code.
*******************************************************************************
 THE PRESENT SOFTWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS WITH
 CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
 AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT, INDIRECT
 OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE CONTENT
 OF SUCH SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING INFORMATION
 CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/*;Depending on Your Application, Disable or Enable the following Defines */
 /* ----------------------------------------------------------------------------
  ;                      EMI Bank1 configuration
  ; ----------------------------------------------------------------------------*/
.set EMI_INIT, 0         /* Initialize EMI pins and configure bank1 if 1*/
 /*; ----------------------------------------------------------------------------
  ;                      EIC initialization
  ; ----------------------------------------------------------------------------*/
.set EIC_INIT, 1         /*; Configure and Initialize EIC if 1*/
  /*; ---------------------------------------------------------------------------- 
  ;                      Peripheral  configuration
  ; ---------------------------------------------------------------------------- */   
.set PERIPHERAL_INIT, 1  /* ; Disable all device peripherals except EIC if 1 */   
        
  /*; ---------------------------------------------------------------------------- 
  ;                      Memory remapping
  ; ----------------------------------------------------------------------------*/
.set remapping, 0        /* remap memory before entring the main program if 1 */       
 /* ; ----------------------------------------------------------------------------*/
.set remap_ram, 0        /*; remap SRAM at address 0x00  if 1*/
 /* ; ---------------------------------------------------------------------------*/
.set remap_flash, 0      /* ; remap FLASH at address 0x00 if 1*/


.extern main
.global T0TIMI_Addr

/* ---		the following are useful for initializing the .data section		---*/
.extern _sidata 	/* start address for the initialization values of the .data section. defined in linker script */
.extern _sdata 	/* start address for the .data section. defined in linker script */
.extern _edata 	/* end address for the .data section. defined in linker script */

/* --- 		the following are useful for initializing the .bss section		--- */
.extern _sbss 			/* start address for the .bss section. defined in linker script */
.extern _ebss 			/* end address for the .bss section. defined in linker script */



/* --- 		main oscillator frequency used on the board 	--- */
.equ	FOSC,		16000000 		/*Here set to 16MHz*/


/* --- 		Standard definitions of Mode bits AND Interrupt (I & F) flags in PSRs 		--- */
.set  Mode_USR, 0x10    /* User Mode */
.set  Mode_FIQ, 0x11    /* FIQ Mode */
.set  Mode_IRQ, 0x12    /* IRQ Mode */
.set  Mode_SVC, 0x13    /* Supervisor Mode */
.set  Mode_ABT, 0x17    /* Abort Mode */
.set  Mode_UNDEF, 0x1B  /* Undefined Mode */
.set  Mode_SYS, 0x1F    /* System Mode */

.equ  I_Bit, 0x80       /* when I bit is set, IRQ is disabled */
.equ  F_Bit, 0x40       /* when F bit is set, FIQ is disabled */

/* --- System memory locations */

/* init value for the stack pointer. defined in linker script */
.extern _estack

/* Stack Sizes. The default values are in the linker script, but they can be overriden. */
.extern _UND_Stack_Init
.extern _SVC_Stack_Init
.extern _ABT_Stack_Init
.extern _FIQ_Stack_Init
.extern _IRQ_Stack_Init
.extern _USR_Stack_Init

.extern _UND_Stack_Size
.extern _SVC_Stack_Size
.extern _ABT_Stack_Size
.extern _FIQ_Stack_Size
.extern _IRQ_Stack_Size
.extern _USR_Stack_Size

SVC_Stack           =     _SVC_Stack_Init /*_estack*/           /* 256 byte SVC stack at*/
                                              /* top of memory */
                                              
IRQ_Stack           =     _IRQ_Stack_Init /*SVC_Stack - 256*/     
USR_Stack           =     _USR_Stack_Init /*IRQ_Stack-1024*/    
FIQ_Stack           =     _FIQ_Stack_Init /*USR_Stack-1024*/   
ABT_Stack           =     _ABT_Stack_Init /*FIQ_Stack-256*/     
UNDEF_Stack         =     _UND_Stack_Init /*ABT_Stack-256*/     

.extern EIC_BASE           /* EIC base address */
.equ	ICR_OFFSET,		0x00	/*  Interrupt Control register offset*/
.equ	CICR_OFFSET,	0x04	/* Current Interrupt Channel Register.*/
.equ	CIPR_OFFSET,	0x08	/*  Current Interrupt Priority Register offset*/
.equ	IVR_OFFSET,		0x18	/*  Interrupt Vector Register offset */
.equ	FIR_OFFSET,		0x1C	/*  Fast Interrupt Register offset*/
.equ	IER_OFFSET,		0x20	/* Interrupt Enable Register offset*/
.equ	IPR_OFFSET,		0x40	/* Interrupt Pending Bit Register offset*/
.equ	SIR0_OFFSET,	0x60	/* Source Interrupt Register 0 */

.equ	EMI_Base_addr,	0x6C000000/*  EMI base address*/
.equ	BCON0_OFFSET,	0x00	/* Bank 0 configuration register offset */
.equ	BCON1_OFFSET,	0x04	/*  Bank 1 configuration register offset */
.equ	BCON2_OFFSET,	0x08	/*  Bank 2 configuration register offset */
.equ	BCON3_OFFSET,	0x0C	/*  Bank 3 configuration register offset*/

.equ	EMI_ENABLE,		0x8000
.equ	EMI_SIZE_16,	0x0001

.equ	GPIO2_Base_addr,0xE0005000		/*GPIO2 base address*/
.equ	PC0_OFFSET,		0x00				/* Port Configuration Register 0 offset*/
.equ	PC1_OFFSET,		0x04				/*  Port Configuration Register 1 offset */
.equ	PC2_OFFSET,		0x08				/* Port Configuration Register 2 offset*/
.equ	PD_OFFSET,		0x0C				/* Port Data Register offset*/

.equ	CPM_Base_addr,	0xA0000040		
.equ	BOOTCR_off_addr,0x0010			/* CPM - Boot Configuration Register*/
.equ	FLASH_mask,		0x0000			/* to remap FLASH at 0x0*/
.equ	RAM_mask,		0x0002			/* to remap RAM at 0x0*/

						
.equ  RCCU_PER_Base_addr,0xA0000000	/* RCCU_PER base address*/
.equ	RCCU_PER_Init,		0x14			/* to init RCCU_PER register to reduce power consumption*/
.equ	RCCU_PER_off_addr,	0x1C     /*RCCU_PER register offset*/
/*******************************************************************************
*				APB Bridge  (System Peripheral)
*******************************************************************************/
.extern	APB1_BASE						/*APB Bridge1 Base Address*/
.equ	CKDIS_off_addr,		0x10		/*APB Bridge1 - Clock Disable  Register*/
.equ	CKDIS1_config_all,	0x27FB	/*To disable clock of all APB1s peripherals*/

.extern	APB2_BASE						/*APB Bridge2 Base Address*/
.equ	CKDIS2_config_all,	0x1FDD	/*To disable clock of all APB2s peripherals*/
                                    /* except EIC */
/********************************************************************************
* Macro Name     : SaveContext
* Description    : This macro used to save the context before entering
                   an exception handler.
* Input          : The range of registers to store.
* Output         : none
********************************************************************************/

       .macro SaveContext $R0,$r12
        STMFD  sp!,{R0-r12,LR} /* Save The workspace plus the current return*/
                               /* address LR_ mode into the stack.*/
        MRS    r1,spsr         /* Save the spsr_mode into r1.*/
        STMFD  sp!,{r1}        /* Save spsr.*/
        .endm

/********************************************************************************
* Macro Name     : RestoreContext
* Description    : This macro used to restore the context to return from
                   an exception handler AND continue the program execution.
* Input          : The range of registers to restore.
* Output         : none
********************************************************************************/

        .macro RestoreContext $R0,$r12
        LDMFD   sp!,{r1}        /* Restore the saved spsr_mode into r1.*/
        MSR     spsr_cxsf,r1    /* Restore spsr_mode.*/
        LDMFD   sp!,{R0-r12,pc}^/* Return to the instruction following...*/
                                /* ...the exception interrupt.*/
        .endm
/******************************************************************************
*			Import exception handlers
*******************************************************************************/	
			.extern  Undefined_Handler
			.extern  SWI_Handler
			.extern  Prefetch_Handler
			.extern  Abort_Handler 
			.extern  FIQ_Handler
/******************************************************************************
*			Import IRQ handlers from 71x_it.c
*******************************************************************************/	
			  .extern  T0TIMI_IRQHandler			
	        .extern  FLASH_IRQHandler
	        .extern  RCCU_IRQHandler
	        .extern  RTC_IRQHandler
	        .extern  WDG_IRQHandler
	        .extern  XTI_IRQHandler
	        .extern  USBHP_IRQHandler
	        .extern  I2C0ITERR_IRQHandler
	        .extern  I2C1ITERR_IRQHandler
	        .extern  UART0_IRQHandler
	        .extern  UART1_IRQHandler
	        .extern  UART2_IRQHandler
	        .extern  UART3_IRQHandler
	        .extern  BSPI0_IRQHandler
	        .extern  BSPI1_IRQHandler
	        .extern  I2C0_IRQHandler
	        .extern  I2C1_IRQHandler
	        .extern  CAN_IRQHandler
	        .extern  ADC12_IRQHandler
	        .extern  T1TIMI_IRQHandler
	        .extern  T2TIMI_IRQHandler
	        .extern  T3TIMI_IRQHandler
	        .extern  HDLC_IRQHandler
	        .extern  USBLP_IRQHandler
	        .extern  T0TOI_IRQHandler
	        .extern  T0OC1_IRQHandler
	        .extern  T0OC2_IRQHandler
/******************************************************************************
*		Export Peripherals IRQ handlers table address
*******************************************************************************/


/* --- 			Copy the vector table in the flash 			--- */
			.section .text
			.global _start
			.global _startup
			.arm
_startup:
/* Note: LDR PC instructions are used here, though branch (B) instructions */
/* could also be used, unless the FLASH is at an address >32MB. */
/******************************************************************************
*				Exception vectors
*******************************************************************************/
				Reset_Vec:	LDR		PC, Reset_Addr
				Undef_Vec:	LDR		PC, Undefined_Addr
				SWI_Vec:		LDR		PC, SWI_Addr
				PAbt_Vec:	LDR		PC, Prefetch_Addr
				DAbt_Vec:	LDR		PC, Abort_Addr
								NOP		/* Reserved vector */
				IRQ_Vec:		LDR		PC, IRQ_Addr
				FIQ_Vec:		LDR		PC, FIQ_Addr				

/*******************************************************************************
*				Exception handlers address table
*******************************************************************************/
				Reset_Addr:		.long	_start
				Undefined_Addr:.long	UndefinedHandler
				SWI_Addr:		.long	SWIHandler
				Prefetch_Addr:	.long	PrefetchHandler
				Abort_Addr:		.long	AbortHandler
									.long	0/* reserved */
				IRQ_Addr:		.long	IRQHandler
				FIQ_Addr:		.long	FIQHandler				

/*******************************************************************************
*				Peripherals IRQ handlers address table
*******************************************************************************/
				T0TIMI_Addr:	.long	T0TIMIIRQHandler
				FLASH_Addr:		.long	FLASHIRQHandler
				RCCU_Addr:		.long	RCCUIRQHandler
				RTC_Addr:		.long	RTCIRQHandler
				WDG_Addr:		.long	WDGIRQHandler
				XTI_Addr:		.long	XTIIRQHandler
				USBHP_Addr:		.long	USBHPIRQHandler
				I2C0ITERR_Addr:	.long	I2C0ITERRIRQHandler
				I2C1ITERR_ADDR:	.long	I2C1ITERRIRQHandler
				UART0_Addr:		.long	UART0IRQHandler
				UART1_Addr:		.long	UART1IRQHandler
				UART2_ADDR:		.long	UART2IRQHandler
				UART3_ADDR:		.long	UART3IRQHandler
				BSPI0_ADDR:		.long	BSPI0IRQHandler
				BSPI1_Addr:		.long	BSPI1IRQHandler
				I2C0_Addr:		.long	I2C0IRQHandler
				I2C1_Addr:		.long	I2C1IRQHandler
				CAN_Addr:		.long	CANIRQHandler
				ADC12_Addr:		.long	ADC12IRQHandler
				T1TIMI_Addr:	.long	T1TIMIIRQHandler
				T2TIMI_Addr:	.long	T2TIMIIRQHandler
				T3TIMI_Addr:	.long	T3TIMIIRQHandler
									.long	0				/* reserved */
									.long	0				/* reserved */
									.long	0				/* reserved */
				HDLC_Addr:		.long	HDLCIRQHandler
				USBLP_Addr:		.long	USBLPIRQHandler
									.long	0				/* reserved */
									.long	0				/* reserved */
				T0TOI_Addr:		.long	T0TOIIRQHandler
				T0OC1_Addr:		.long	T0OC1IRQHandler
				T0OC2_Addr:		.long	T0OC2IRQHandler
.text
/********************************************************************************
* Function Name  : UndefinedHandler
* Description    : This function called when undefined instruction
                   exception is entered.
* Input          : none
* Output         : none
*********************************************************************************/
UndefinedHandler:
        SaveContext R0,r12  	/* Save the workspace plus the current*/
								      /* return address LR_ und AND spsr_und.*/
        LDR		R0,=Undefined_Handler
        LDR		LR,=Undefined_Handler_end
        BX		R0             /* Branch to Undefined_Handler*/
Undefined_Handler_end:
        RestoreContext R0,r12  /* Return to the instruction following...*/
                               /* ...the undefined instruction.*/
/********************************************************************************
* Function Name  : SWIHandler
* Description    : This function called when SWI instruction executed.
* Input          : none
* Output         : none
********************************************************************************/
SWIHandler:
        SaveContext R0,r12    /* Save the workspace plus the current*/
                              /* return address LR_ svc AND spsr_svc.*/
        LDR		R0,=SWI_Handler
        LDR		LR,=SWI_Handler_end
        BX		R0             /* Branch to SWI_Handler*/
SWI_Handler_end:
        RestoreContext R0,r12 /* Return to the instruction following...*/
                              /* ...the SWI instruction.*/
/********************************************************************************
* Function Name  : IRQHandler
* Description    : This function called when IRQ exception is entered.
* Input          : none
* Output         : none
********************************************************************************/
IRQHandler:
        SUB    LR,LR,#4		       /* Update the link register*/
        SaveContext R0,r12        /* Save the workspace plus the current*/
                                  /* return address LR_ irq AND spsr_irq.*/
        LDR    LR, =ReturnAddress /* Read the return address.*/
        LDR    R0, =EIC_BASE
        LDR    r1, =IVR_OFFSET
        ADD    pc,R0,r1           /* Branch to the IRQ handler.*/
ReturnAddress:				          /* Clear pending bit in EIC (using the proper IPRx)*/
        LDR    R0, =EIC_BASE
        LDR    r2, [R0, #CICR_OFFSET] /* Get the IRQ channel number*/
        MOV    r3,#1
        MOV    r3,r3,LSL r2
        STR    r3,[R0, #IPR_OFFSET]   
							              /* Clear the corresponding IPR bit.*/
        RestoreContext R0,r12      /*  Restore the context AND return to the...*/
                                   /*  ...program execution. */						
/********************************************************************************
* Function Name  :PrefetchHandler
* Description    : This function called when Prefetch Abort
                   exception is entered.
* Input          : none
* Output         : none
*********************************************************************************/
PrefetchHandler:
        SUB		LR,LR,#4          /* Update the link register.*/
        SaveContext R0,r12       /* Save the workspace plus the current*/
                                 /* return address LR_abt AND spsr_abt.*/
        LDR		R0,=Prefetch_Handler
        LDR		LR,=Prefetch_Handler_end
        BX		R0                /* Branch to Prefetch_Handler.*/
Prefetch_Handler_end:
        RestoreContext R0,r12 /* Return to the instruction following that...*/
                              /* ...has generated the prefetch abort exception.*/						  
/********************************************************************************
* Function Name  : AbortHandler
* Description    : This function is called when Data Abort
                   exception is entered.
* Input          : none
* Output         : none
********************************************************************************/
AbortHandler:
        SUB		LR,LR,#8       /* Update the link register.*/
        SaveContext R0,r12    /* Save the workspace plus the current*/
                              /* return address LR_ abt AND spsr_abt.*/
        LDR		R0,=Abort_Handler
        LDR		LR,=Abort_Handler_end
        BX		R0             /* Branch to Abort_Handler.*/
Abort_Handler_end:
        RestoreContext R0,r12 /* Return to the instruction following that...*/
                              /* ...has generated the data abort exception.*/	
/*******************************************************************************
* Function Name  : FIQHandler
* Description    : This function is called when FIQ
*                      exception is entered.
* Input          : none
* Output         : none
*******************************************************************************/
FIQHandler:
		SUB		LR,LR,#4		/* Update the link register.*/
		STMFD	sp!,{R0-r7,LR}	/* Save The workSPace plus the current return*/
									/* address LR_fiq into the FIQ stack.*/
		LDR		R0,=FIQ_Handler
		LDR		LR,=FIQ_Handler_end
		BX		R0						/*Branch to FIQ_Handler.*/
FIQ_Handler_end:
		LDMFD	sp!,{R0-r7,PC}^	/* Return to the instruction following...*/
										/* the exception interrupt.*/

/********************************************************************************
* Macro Name     : IRQ_to_SYS
* Description    : This macro used to switch form IRQ mode to SYS mode
* Input          : none.
* Output         : none
*******************************************************************************/
       .macro IRQ_to_SYS
        MSR    cpsr_c,#0x1F
        STMFD  sp!,{LR}
       .endm
/********************************************************************************
* Macro Name     : SYS_to_IRQ
* Description    : This macro used to switch from SYS mode to IRQ mode
                   then to return to IRQHnadler routine.
* Input          : none.
* Output         : none.
*******************************************************************************/
      .macro SYS_to_IRQ
       LDMFD  sp!,{LR}       /* Restore the link register. */
        MSR    cpsr_c,#0xD2  /* Switch to IRQ mode.*/
        MOV    pc,LR         /* Return to IRQHandler routine to clear the*/
                             /* pending bit.*/
       .endm
/********************************************************************************
* Function Name  : T0TIMIIRQHandler
* Description    : This function used to switch to SYS mode before entering
                   the T0TIMI_IRQHandler function located in 71x_it.c.
                   Then to return to IRQ mode after the
                   T0TIMI_IRQHandler function termination.
* Input          : none.
* Output         : none.
********************************************************************************/
T0TIMIIRQHandler:
        IRQ_to_SYS
        LDR		R0,=T0TIMI_IRQHandler
        LDR		LR,=T0TIMI_IRQHandler_end
        BX		R0
T0TIMI_IRQHandler_end:
        SYS_to_IRQ	   
/********************************************************************************
* Function Name  : FLASHIRQHandler
* Description    : This function used to switch to SYS mode before entering
                   the FLASH_IRQHandler function located in 71x_it.c.
                   Then to return to IRQ mode after the
                   FLASH_IRQHandler function termination.
* Input          : none
* Output         : none
********************************************************************************/
FLASHIRQHandler:
        IRQ_to_SYS
        LDR		R0,=FLASH_IRQHandler
        LDR		LR,=FLASH_IRQHandler_end
        BX		R0
FLASH_IRQHandler_end:
        SYS_to_IRQ		
/********************************************************************************
* Function Name  : RCCUIRQHandler
* Description    : This function used to switch to SYS mode before entering
                   the RCCU_IRQHandler function located in 71x_it.c.
                   Then to return to IRQ mode after the
                   RCCU_IRQHandler function termination.
* Input          : none
* Output         : none
*******************************************************************************/
RCCUIRQHandler:
        IRQ_to_SYS
        LDR		R0,=RCCU_IRQHandler
        LDR		LR,=RCCU_IRQHandler_end
        BX		R0
RCCU_IRQHandler_end:
        SYS_to_IRQ
/********************************************************************************
* Function Name  : RTCIRQHandler
* Description    : This function used to switch to SYS mode before entering
                   the RTC_IRQHandler function located in 71x_it.c.
                   Then to return to IRQ mode after the
                   RTC_IRQHandler function termination.
* Input          : none
* Output         : none
*******************************************************************************/
RTCIRQHandler:
        IRQ_to_SYS
        LDR		R0,=RTC_IRQHandler
        LDR		LR,=RTC_IRQHandler_end
        BX		R0
RTC_IRQHandler_end:
        SYS_to_IRQ
/********************************************************************************
* Function Name  : WDGIRQHandler
* Description    : This function used to switch to SYS mode before entering
                   the WDG_IRQHandler function located in 71x_it.c.
                   Then to return to IRQ mode after the
                   WDG_IRQHandler function termination.
* Input          : none
* Output         : none
/********************************************************************************/
WDGIRQHandler:
        IRQ_to_SYS
        LDR		R0,=WDG_IRQHandler
        LDR		LR,=WDG_IRQHandler_end
        BX 		R0
WDG_IRQHandler_end:
        SYS_to_IRQ
/******************************************************************************** /
* Function Name  : XTIIRQHandler
* Description    : This function used to switch to SYS mode before entering
                   the XTI_IRQHandler function located in 71x_it.c.
                   Then to return to IRQ mode after the
                   XTI_IRQHandler function termination.
* Input          : none
* Output         : none 
**************************************************************************************/
XTIIRQHandler:
        IRQ_to_SYS
        LDR		R0,=XTI_IRQHandler
        LDR		LR,=XTI_IRQHandler_end
        BX		R0
XTI_IRQHandler_end:
        SYS_to_IRQ
/********************************************************************************
* Function Name  : USBHPIRQHandler
* Description    : This function used to switch to SYS mode before entering
                   the USBHP_IRQHandler function located in 71x_it.c.
                   Then to return to IRQ mode after the
                   USBHP_IRQHandler function termination.
* Input          : none
* Output         : none
*******************************************************************************/
USBHPIRQHandler:
        IRQ_to_SYS
        LDR		R0,=USBHP_IRQHandler
        LDR		LR,=USBHP_IRQHandler_end
        BX		R0
USBHP_IRQHandler_end:
        SYS_to_IRQ
/********************************************************************************
* Function Name  : I2C0ITERRIRQHandler
* Description    : This function used to switch to SYS mode before entering
                   the I2C0ITERR_IRQHandler function located in 71x_it.c.
                   Then to return to IRQ mode after the
                   I2C0ITERR_IRQHandler function termination.
* Input          : none
* Output         : none
*******************************************************************************/
I2C0ITERRIRQHandler:
        IRQ_to_SYS
        LDR		R0,=I2C0ITERR_IRQHandler
        LDR		LR,=I2C0ITERR_IRQHandler_end
        BX		R0
I2C0ITERR_IRQHandler_end:
        SYS_to_IRQ
/********************************************************************************
* Function Name  : I2C1ITERRIRQHandler
* Description    : This function used to switch to SYS mode before entering
                   the I2C1ITERR_IRQHandler function located in 71x_it.c.
                   Then to return to IRQ mode after the
                   I2C1ITERR_IRQHandler function termination.
* Input          : none
* Output         : none
*******************************************************************************/
I2C1ITERRIRQHandler:
        IRQ_to_SYS
        LDR		R0,=I2C1ITERR_IRQHandler
        LDR		LR,=I2C1ITERR_IRQHandler_end
        BX		R0
I2C1ITERR_IRQHandler_end:
        SYS_to_IRQ
/********************************************************************************
* Function Name  : UART0IRQHandler
* Description    : This function used to switch to SYS mode before entering
                   the UART0_IRQHandler function located in 71x_it.c.
                   Then to return to IRQ mode after the
                   UART0_IRQHandler function termination.
* Input          : none
* Output         : none
*******************************************************************************/
UART0IRQHandler:
        IRQ_to_SYS
        LDR		R0,=UART0_IRQHandler
        LDR		LR,=UART0_IRQHandler_end
        BX		R0
UART0_IRQHandler_end:
        SYS_to_IRQ
/********************************************************************************
* Function Name  : UART1IRQHandler
* Description    : This function used to switch to SYS mode before entering
                   the UART1_IRQHandler function located in 71x_it.c.
                   Then to return to IRQ mode after the
                   UART1_IRQHandler function termination.
* Input          : none
* Output         : none
*******************************************************************************/
UART1IRQHandler:
        IRQ_to_SYS
        LDR		R0,=UART1_IRQHandler
        LDR		LR,=UART1_IRQHandler_end
        BX		R0
UART1_IRQHandler_end:
        SYS_to_IRQ
/********************************************************************************
* Function Name  : UART2IRQHandler
* Description    : This function used to switch to SYS mode before entering
                   the UART2_IRQHandler function located in 71x_it.c.
                   Then to return to IRQ mode after the
                   UART2_IRQHandler function termination.
* Input          : none
* Output         : none
*******************************************************************************/
UART2IRQHandler:
        IRQ_to_SYS
        LDR		R0,=UART2_IRQHandler
        LDR		LR,=UART2_IRQHandler_end
        BX		R0
UART2_IRQHandler_end:
        SYS_to_IRQ
/********************************************************************************
* Function Name  : UART3IRQHandler
* Description    : This function used to switch to SYS mode before entering
                   the UART3IRQHandler function located in 71x_it.c.
                   Then to return to IRQ mode after the
                   UART3IRQHandler function termination.
* Input          : none
* Output         : none
*******************************************************************************/
UART3IRQHandler:
        IRQ_to_SYS
        LDR		R0,=UART3_IRQHandler
        LDR		LR,=UART3_IRQHandler_end
        BX		R0
UART3_IRQHandler_end:
        SYS_to_IRQ
/********************************************************************************
* Function Name  : BSPI0IRQHandler
* Description    : This function used to switch to SYS mode before entering
                   the BSPI0IRQHandler function located in 71x_it.c.
                   Then to return to IRQ mode after the
                   BSPI0IRQHandler function termination.
* Input          : none
* Output         : none
*******************************************************************************/
BSPI0IRQHandler:
        IRQ_to_SYS
        LDR		R0,=BSPI0_IRQHandler
        LDR		LR,=BSPI0_IRQHandler_end
        BX		R0
BSPI0_IRQHandler_end:
        SYS_to_IRQ
/********************************************************************************
* Function Name  : BSPI1IRQHandler
* Description    : This function used to switch to SYS mode before entering
                   the BSPI1_IRQHandler function located in 71x_it.c.
                   Then to return to IRQ mode after the
                   BSPI1_IRQHandler function termination.
* Input          : none
* Output         : none
*******************************************************************************/
BSPI1IRQHandler:
        IRQ_to_SYS
        LDR		R0,=BSPI1_IRQHandler
        LDR		LR,=BSPI1_IRQHandler_end
        BX		R0
BSPI1_IRQHandler_end:
        SYS_to_IRQ
/********************************************************************************
* Function Name  : I2C0IRQHandler
* Description    : This function used to switch to SYS mode before entering
                   the I2C0_IRQHandler function located in 71x_it.c.
                   Then to return to IRQ mode after the
                   I2C0_IRQHandler function termination.
* Input          : none
* Output         : none
*******************************************************************************/
I2C0IRQHandler:
        IRQ_to_SYS
        LDR		R0,=I2C0_IRQHandler
        LDR		LR,=I2C0_IRQHandler_end
        BX		R0
I2C0_IRQHandler_end:
        SYS_to_IRQ
/********************************************************************************
* Function Name  : I2C1IRQHandler
* Description    : This function used to switch to SYS mode before entering
                   the I2C1_IRQHandler function located in 71x_it.c.
                   Then to return to IRQ mode after the
                   I2C1_IRQHandler function termination.
* Input          : none
* Output         : none
*******************************************************************************/
I2C1IRQHandler:
        IRQ_to_SYS
        LDR		R0,=I2C1_IRQHandler
        LDR		LR,=I2C1_IRQHandler_end
        BX		R0
I2C1_IRQHandler_end:
        SYS_to_IRQ
/********************************************************************************
* Function Name  : CANIRQHandler
* Description    : This function used to switch to SYS mode before entering
                   the CAN_IRQHandler function located in 71x_it.c.
                   Then to return to IRQ mode after the
                   CAN_IRQHandler function termination.
* Input          : none
* Output         : none
*******************************************************************************/
CANIRQHandler:
        IRQ_to_SYS
        LDR		R0,=CAN_IRQHandler
        LDR		LR,=CAN_IRQHandler_end
        BX		R0
CAN_IRQHandler_end:
        SYS_to_IRQ
/********************************************************************************
* Function Name  : ADC12IRQHandler
* Description    : This function used to switch to SYS mode before entering
                   the ADC12_IRQHandler function located in 71x_it.c.
                   Then to return to IRQ mode after the
                   ADC12_IRQHandler function termination.
* Input          : none
* Output         : none
*******************************************************************************/
ADC12IRQHandler:
        IRQ_to_SYS
        LDR		R0,=ADC12_IRQHandler
        LDR		LR,=ADC12_IRQHandler_end
        BX		R0
ADC12_IRQHandler_end:
        SYS_to_IRQ
/********************************************************************************
* Function Name  : T1TIMIIRQHandler
* Description    : This function used to switch to SYS mode before entering
                   the T1TIMI_IRQHandler function located in 71x_it.c.
                   Then to return to IRQ mode after the
                   T1TIMI_IRQHandler function termination.
* Input          : none
* Output         : none
*******************************************************************************/
T1TIMIIRQHandler:
        IRQ_to_SYS
        LDR		R0,=T1TIMI_IRQHandler
        LDR		LR,=T1TIMI_IRQHandler_end
        BX		R0
T1TIMI_IRQHandler_end:
        SYS_to_IRQ
/********************************************************************************
* Function Name  : T2TIMIIRQHandler
* Description    : This function used to switch to SYS mode before entering
                   the T2TIMI_IRQHandler function located in 71x_it.c.
                   Then to return to IRQ mode after the
                   T2TIMI_IRQHandler function termination.
* Input          : none
* Output         : none
*******************************************************************************/
T2TIMIIRQHandler:
        IRQ_to_SYS
        LDR		R0,=T2TIMI_IRQHandler
        LDR		LR,=T2TIMI_IRQHandler_end
        BX		R0
T2TIMI_IRQHandler_end:
        SYS_to_IRQ
/********************************************************************************
* Function Name  : T3TIMIIRQHandler
* Description    : This function used to switch to SYS mode before entering
                   the T3TIMI_IRQHandler function located in 71x_it.c.
                   Then to return to IRQ mode after the
                   T3TIMI_IRQHandler function termination.
* Input          : none
* Output         : none
*******************************************************************************/
T3TIMIIRQHandler:
        IRQ_to_SYS
        LDR		R0, =T3TIMI_IRQHandler
        LDR		LR,=T3TIMI_IRQHandler_end
        BX		R0
T3TIMI_IRQHandler_end:
        SYS_to_IRQ
/********************************************************************************
* Function Name  : HDLCIRQHandler
* Description    : This function used to switch to SYS mode before entering
                   the HDLC_IRQHandler function located in 71x_it.c.
                   Then to return to IRQ mode after the
                   HDLC_IRQHandler function termination.
* Input          : none
* Output         : none
*******************************************************************************/
HDLCIRQHandler:
        IRQ_to_SYS
        LDR		R0,=HDLC_IRQHandler
        LDR		LR,=HDLC_IRQHandler_end
        BX		R0
HDLC_IRQHandler_end:
        SYS_to_IRQ
/********************************************************************************
* Function Name  : USBLPIRQHandler
* Description    : This function used to switch to SYS mode before entering
                   the USBLP_IRQHandler function located in 71x_it.c.
                   Then to return to IRQ mode after the
                   USBLP_IRQHandler function termination.
* Input          : none
* Output         : none
*******************************************************************************/
USBLPIRQHandler:
        IRQ_to_SYS
        LDR		R0,=USBLP_IRQHandler
        LDR		LR,=USBLP_IRQHandler_end
        BX		R0
USBLP_IRQHandler_end:
        SYS_to_IRQ
/********************************************************************************
* Function Name  : T0TOIIRQHandler
* Description    : This function used to switch to SYS mode before entering
                   the T0TOI_IRQHandler function located in 71x_it.c.
                   Then to return to IRQ mode after the
                   T0TOI_IRQHandler function termination.
* Input          : none
* Output         : none
*******************************************************************************/
T0TOIIRQHandler:
        IRQ_to_SYS
        LDR		R0,=T0TOI_IRQHandler
        LDR		LR,=T0TOI_IRQHandler_end
        BX		R0
T0TOI_IRQHandler_end:
        SYS_to_IRQ
/********************************************************************************
* Function Name  : T0OC1IRQHandler
* Description    : This function used to switch to SYS mode before entering
                   the T0OC1_IRQHandler function located in 71x_it.c.
                   Then to return to IRQ mode after the
                   T0OC1_IRQHandler function termination.
* Input          : none
* Output         : none
*******************************************************************************/
T0OC1IRQHandler:
        IRQ_to_SYS
        LDR		R0,=T0OC1_IRQHandler
        LDR		LR,=T0OC1_IRQHandler_end
        BX		R0
T0OC1_IRQHandler_end:
        SYS_to_IRQ
/********************************************************************************
* Function Name  : T0OC2IRQHandler
* Description    : This function used to switch to SYS mode before entering
                   the T0OC2_IRQHandler function located in 71x_it.c.
                   Then to return to IRQ mode after the
                   T0OC2_IRQHandler function termination.
* Input          : none
* Output         : none
*******************************************************************************/
T0OC2IRQHandler:
        IRQ_to_SYS
        LDR		R0,=T0OC2_IRQHandler
        LDR		LR,=T0OC2_IRQHandler_end
        BX		R0
T0OC2_IRQHandler_end:
        SYS_to_IRQ



_start:	
        LDR     pc, =NextInst

NextInst:
 
        NOP   /* Wait for OSC stabilization*/
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
       
           
        MSR    CPSR_c, #Mode_FIQ|I_Bit|F_Bit	/*No interrupts     */
        LDR     SP, =FIQ_Stack           /* Initialize FIQ stack pointer */
        
		  MSR     CPSR_c, #Mode_IRQ|I_Bit|F_Bit	/*No interrupts     */
        LDR     SP, =IRQ_Stack			/* Initialize IRQ stack pointer*/
		
		  MSR     CPSR_c, #Mode_ABT|F_Bit|I_Bit	/*No interrupts*/
        LDR     SP, =ABT_Stack			/* Initialize ABT stack pointer*/

        MSR     CPSR_c, #Mode_UNDEF|F_Bit|I_Bit	/*No interrupts*/
        LDR     SP, =UNDEF_Stack

        MSR     CPSR_c, #Mode_SVC|F_Bit|I_Bit	/*No interrupts*/
        LDR     SP, =_estack          /*SVC_Stack*/
 /********************************************************************************
* Macro Name     : EMI_INIT
* Description    : Initialize EMI bank 1: 16-bit 15 wait state
* Input          : None.
* Output         : None.
*******************************************************************************/
.if EMI_INIT
        LDR     r0, =GPIO2_Base_addr      /*Configure P2.0 to P2.3 in AF_PP mode*/
        LDR     r2, [r0, #PC0_OFFSET]
        ORR     r2, r2,#0x0000000F
        STR     r2, [r0, #PC0_OFFSET]
        LDR     r2, [r0, #PC1_OFFSET]
        ORR     r2, r2,#0x0000000F
        STR     r2, [r0, #PC1_OFFSET]
        LDR     r2, [r0, #PC2_OFFSET]
        ORR     r2, r2,#0x0000000F
        STR     r2, [r0, #PC2_OFFSET]
        LDR     r0, =EMI_Base_addr
        LDR     r1, =EMI_ENABLE|EMI_SIZE_16
        STR     r1, [r0, #BCON0_OFFSET] /* Enable bank 1 16-bit 15 wait states*/
.endif

/******************************************************************************
*EIC initialization
*Description  : Initialize the EIC as following :
              - IRQ disabled
	   - FIQ disabled
              - IVR contain the load PC opcode (0xF59FF00)
              - Current priority level equal to 0
              - All channels are disabled
              - All channels priority equal to 0
              - All SIR registers contain offset to the related IRQ table entry
******************************************************************************/

 .if EIC_INIT 
        LDR     r3, =EIC_BASE
        LDR     r4, =0x00000000
        STR   	 r4, [r3, #ICR_OFFSET]     /* Disable FIQ AND IRQ */
        STR     r4, [r3, #IER_OFFSET]     /* Disable all channels interrupts */
        LDR     r4, =0xFFFFFFFF
        STR     r4, [r3, #IPR_OFFSET]     /* Clear all IRQ pending bits */
        LDR     r4, =0x0C
        STR     r4, [r3, #FIR_OFFSET]     /* Disable FIQ channels AND clear FIQ pending bits */
        LDR     r4, =0x00000000
        STR     r4, [r3, #CIPR_OFFSET]    /* Reset the current priority register*/
        LDR     r4, =0xE59F0000
        STR     r4, [r3, #IVR_OFFSET]     /* Write the LDR pc,pc,#offset instruction code in IVR[31:16] */
        LDR     r2, =32                   /* 32 Channel to initialize*/
        LDR     R0, =T0TIMI_Addr          /* Read the address of the IRQs address table*/
        LDR     r1, =0x00000FFF
        AND     R0,R0,r1
        LDR     r5, =SIR0_OFFSET          /* Read SIR0 address */
        SUB     r4,R0,#8                  /* subtract 8 for prefetch*/
        LDR     r1, =0xF7E8               /* add the offset to the 0x00000000 address(IVR address + 7E8 = 0x00000000)*/
                                          /* 0xF7E8 used to complete the LDR pc,pc,#offset opcode */
        add     r1,r4,r1                  /* compute the jump offset*/
EIC_INI :
        MOV     r4, r1, LSL #16           /* Left shift the result*/
        STR     r4, [r3, r5]              /* Store the result in SIRx register*/
        ADD     r1, r1, #4                /* Next IRQ address*/
        ADD     r5, r5, #4                /* Next SIR*/
        SUBS    r2, r2, #1                /* Decrement the number of SIR registers to initialize*/
        BNE     EIC_INI                   /* If more then continue*/
 .endif

/*******************************************************************************
* Macro Name     : PERIPHERAL_INIT
* Description    : Disable all device peripherals except EIC.
* Input          : None.
* Output         : None.
*******************************************************************************/
.if PERIPHERAL_INIT
		  LDR     r1, =APB1_BASE          		/*r1= APB1 base address*/
        LDR     r2, =APB2_BASE					/*r2= APB2 base address*/
        LDR     r0, =CKDIS1_config_all
        STRH    r0, [r1, #CKDIS_off_addr]		/*Disable clock for all APB1 peripherals*/
        LDR     r0, =CKDIS2_config_all 
        STRH    r0, [r2, #CKDIS_off_addr]    /*Disable clock for all APB2 peripherals*/
        LDR     r1, =RCCU_PER_Base_addr      /*r1= RCCU_PER base address*/
        LDR     r0, =RCCU_PER_Init           
        STRH    r0, [r1, #RCCU_PER_off_addr] /*To reduce power consumption clear all
                                             unused bits in the RCCU_PER register
                                             and keep EMI and USB KERNEL clocks enabled*/
.endif

/*******************************************************************************
*  			Change to System mode	
******************************************************************************/
		MSR     CPSR_c, #Mode_SYS
      LDR     SP, =USR_Stack           /* Initialize USR/SYS stack pointer*/

/*******************************************************************************
*REMAPPING
*Description  : Remapping  memory whether RAM,FLASH
               at Address 0x0 after the application has started executing.
               Remapping is generally done to allow RAM  to replace FLASH
               at 0x0.
               the remapping of RAM allow copying of vector table into RAM
               To enable the memory remapping uncomment: (see above)
               #define  remapping to enable memory remapping
                  AND
               #define  remap_ram to remap RAM
                  OR
               #define  remap_flash to remap FLASH
******************************************************************************/
.if remapping
    .if remap_flash
        MOV     R0, #FLASH_mask
    .endif
    .if remap_ram
        MOV     R0, #RAM_mask
    .endif

        LDR     r1, =CPM_Base_addr
        LDRH    r2, [r1, #BOOTCR_off_addr]/* Read BOOTCR Register*/
        BIC     r2, r2, #0x03             /* Reset the two LSB bits of BOOTCR*/
        ORR     r2, r2, R0                /* change the two LSB bits of BOOTCR*/
        STRH    r2, [r1, #BOOTCR_off_addr]/* Write BOOTCR Register*/
.endif

	/* copy the initial values for .data section from FLASH to RAM */
		LDR		R1, =_sidata
		LDR		R2, =_sdata
		LDR		R3, =_edata
_reset_inidata_loop:
		CMP		R2, R3
		LDRLO	R0, [R1], #4
		STRLO	R0, [R2], #4
		BLO		_reset_inidata_loop

	/* Clear the .bss section */
		MOV   	R0,#0					   /* get a zero */
		LDR   	r1,=_sbss				/* point to bss start */
		LDR  	r2,=_ebss				   /* point to bss end */
_reset_inibss_loop:
		CMP   	r1,r2					   /* check if some data remains to clear */
		STRLO 	R0,[r1],#4			   /* clear 4 bytes */
		BLO   	_reset_inibss_loop	/* loop until done */
		

/* --- 		Enter the C code, use B instruction so as to never return 		--- */
     B		main								     


	.end	

/****** (C) COPYRIGHT 2008 STMicroelectronics *********/















