/*
This is the default Startup for STR91x devices for the GNU toolchain

It has been designed by ST Microelectronics and modified by Raisonance.

You can use it, modify it, distribute it freely but without any waranty.

*/



.extern main


;/* the following are useful for initializing the .data section */
.extern _sidata ;/* start address for the initialization values of the .data section. defined in linker script */
.extern _sdata ;/* start address for the .data section. defined in linker script */
.extern _edata ;/* end address for the .data section. defined in linker script */

;/* the following are useful for initializing the .bss section */
.extern _sbss ;/* start address for the .bss section. defined in linker script */
.extern _ebss ;/* end address for the .bss section. defined in linker script */





/*Enable Only ONE of the following defines to select your STR91xFA Flash size*/ 
/*;and the Boot bank (Bank0 or Bank1).You have also to uncomment the appropriate */
/*defines in "91x_conf.h" file.*/
/*In this File/library the default size is 512Kbytes  and the boot bank is Bank0*/


.set  Flash_256KB_Bank0_Boot, 0
.set  Flash_512KB_Bank0_Boot, 1
.set  Flash_1MB_Bank0_Boot, 0
.set  Flash_2MB_Bank0_Boot, 0

.set  Flash_256KB_Bank1_Boot, 0
.set  Flash_512KB_Bank1_Boot, 0
.set  Flash_2MB_Bank1_Boot, 0
.set  Flash_1MB_Bank1_Boot, 0


/* Uncomment the following define when working in debug mode you have also to */
/* Uncomment the same define in "91x_conf.h" file*/

.set debug_mode, 1



/* Standard definitions of Mode bits and Interrupt (I & F) flags in PSRs */
.set  Mode_USR, 0x10            /* User Mode */
.set  Mode_FIQ, 0x11            /* FIQ Mode */
.set  Mode_IRQ, 0x12            /* IRQ Mode */
.set  Mode_SVC, 0x13            /* Supervisor Mode */
.set  Mode_ABT, 0x17            /* Abort Mode */
.set  Mode_UNDEF, 0x1B          /* Undefined Mode */
.set  Mode_SYS, 0x1F            /* System Mode */


.equ  I_Bit, 0x80               /* when I bit is set, IRQ is disabled */
.equ  F_Bit, 0x40               /* when F bit is set, FIQ is disabled */

/*--- STR9X SCU specific definitions */

.set SCU_BASE_Address, 0x5C002000 /* SCU Base Address*/ 
.set SCU_SCR0_OFST,    0x00000034 /* System Configuration Register 0 Offset*/

/* --- STR9X FMI specific definitions*/

.set FMI_BASE_Address, 0x54000000  /* FMI Base Address*/
.set FMI_BBSR_OFST,    0x00000000  /* Boot Bank Size Register offset*/
.set FMI_NBBSR_OFST,   0x00000004  /* Non-boot Bank Size Register offset*/
.set FMI_BBADR_OFST,   0x0000000C  /*Boot Bank Base Address Register offset*/
.set FMI_NBBADR_OFST,  0x00000010  /* Non-boot Bank Base Address Register offset*/
.set FMI_CR_OFST,      0x00000018  /* Control Register offset*/

.if Flash_2MB_Bank0_Boot

.set BOOT_BANK_Size,        0x6        /* Boot Bank Size Register*/
.set NON_BOOT_BANK_Size,    0x4        /* Non-boot Bank Size Register*/
.set BOOT_BANK_Address,     0x00000000 /* Boot Bank Base Address Register*/
.set NON_BOOT_BANK_Address, 0x00200000 /* Non-boot Bank Base Address Register*/

.endif

.if Flash_2MB_Bank1_Boot

.set BOOT_BANK_Size,         0x2        /* Boot Bank Size Register*/
.set NON_BOOT_BANK_Size,     0x8        /* Non-boot Bank Size Register*/
.set BOOT_BANK_Address,      0x00000000 /* Boot Bank Base Address Register*/
.set NON_BOOT_BANK_Address,  0x00200000 /* Non-boot Bank Base Address Register*/

.endif

.if Flash_1MB_Bank0_Boot

.set BOOT_BANK_Size,         0x5        /* Boot Bank Size Register*/
.set NON_BOOT_BANK_Size,     0x4        /* Non-boot Bank Size Register*/
.set BOOT_BANK_Address,      0x00000000 /* Boot Bank Base Address Register*/
.set NON_BOOT_BANK_Address,  0x00200000 /* Non-boot Bank Base Address Register*/

.endif

.if Flash_1MB_Bank1_Boot

.set BOOT_BANK_Size,         0x2        /* Boot Bank Size Register*/
.set NON_BOOT_BANK_Size,     0x7        /* Non-boot Bank Size Register*/
.set BOOT_BANK_Address,      0x00000000 /* Boot Bank Base Address Register*/
.set NON_BOOT_BANK_Address,  0x00200000 /* Non-boot Bank Base Address Register*/

.endif

.if Flash_512KB_Bank0_Boot

.set BOOT_BANK_Size,         0x4        /* Boot Bank Size Register*/
.set NON_BOOT_BANK_Size,     0x2        /* Non-boot Bank Size Register*/
.set BOOT_BANK_Address,      0x00000000 /* Boot Bank Base Address Register*/
.set NON_BOOT_BANK_Address,  0x00080000 /* Non-boot Bank Base Address Register*/

.endif

.if Flash_512KB_Bank1_Boot

.set BOOT_BANK_Size,         0x0        /* Boot Bank Size Register*/
.set NON_BOOT_BANK_Size,     0x6        /* Non-boot Bank Size Register*/
.set BOOT_BANK_Address,      0x00000000 /* Boot Bank Base Address Register*/
.set NON_BOOT_BANK_Address,  0x00080000 /* Non-boot Bank Base Address Register*/


.endif

.if Flash_256KB_Bank0_Boot

.set BOOT_BANK_Size,         0x3        /* Boot Bank Size Register*/
.set NON_BOOT_BANK_Size,     0x2        /* Non-boot Bank Size Register*/
.set BOOT_BANK_Address,      0x00000000 /* Boot Bank Base Address Register*/
.set NON_BOOT_BANK_Address,  0x00080000 /* Non-boot Bank Base Address Register*/

.endif

.if Flash_256KB_Bank1_Boot

.set BOOT_BANK_Size,         0x0        /* Boot Bank Size Register*/
.set NON_BOOT_BANK_Size,     0x5        /* Non-boot Bank Size Register*/
.set BOOT_BANK_Address,      0x00000000 /* Boot Bank Base Address Register*/
.set NON_BOOT_BANK_Address , 0x00080000 /* Non-boot Bank Base Address Register*/

.endif








/*; --- System memory locations */

/* init value for the stack pointer. defined in linker script */
.extern _estack

;/* Stack Sizes. The default values are in the linker script, but they can be overriden. */
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

SVC_Stack           =     _SVC_Stack_Init /*_estack*/           /*; 256 byte SVC stack at*/
                                              /*; top of memory */
                                              
IRQ_Stack           =     _IRQ_Stack_Init /*SVC_Stack - 256*/   /*; followed by IRQ stack */
USR_Stack           =     _USR_Stack_Init /*IRQ_Stack-1024*/    /*; followed by USR stack */
FIQ_Stack           =     _FIQ_Stack_Init /*USR_Stack-1024*/    /*; followed by FIQ stack*/
ABT_Stack           =     _ABT_Stack_Init /*FIQ_Stack-256*/     /*; followed by ABT stack */
UNDEF_Stack         =     _UND_Stack_Init /*ABT_Stack-256*/     /*; followed by UNDEF stack */

.equ VectorAddress,   0xFFFFF030  /* VIC Vector address register address.*/      

.globl start
.globl _start
.globl _startup
.section .flashtext,"ax",%progbits
start:
_start:
_startup:

	ldr     PC, =Reset_Handler
	ldr     PC, =UndefinedHandler
	ldr     PC, =SWIHandler
	ldr     PC, =PrefetchAbortHandler
	ldr     PC, =DataAbortHandler
	nop                          /*; Reserved vector*/
	ldr     PC, =IRQHandler


/********************************************************************************
* Function Name  : FIQHandler
* Description    : This function is called when FIQ exception is entered.
* Input          : none
* Output         : none
********************************************************************************/
FIQHandler:
       SUB    lr,lr,#4        /* Update the link register.*/
       STMFD  sp!,{r0-r3,lr}  /* Save The workspace plus the current return*/
                              /* address lr_fiq into the FIQ stack.*/       
       BL FIQ_Handler /*; Branch to FIQ_Handler.*/

      LDMFD   sp!,{r0-r3,pc}^ /*Return to the instruction following...*/
                              /* ...the exception interrupt.*/


.text

/*;*******************************************************************************
;* Macro Name     : SaveContext
;* Description    : This macro used to save the context before entering
;                   an exception handler.
;* Input          : The range of registers to store.
;* Output         : none
;*******************************************************************************/
  
	.macro Savecontext $r0,$r3
    STMFD  sp!,{r0-r3,lr}     /* Save The workspace plus the current return*/
                                  /* address lr_ mode into the stack.*/
	.endm
/*;*******************************************************************************
;* Macro Name     : RestoreContext
;* Description    : This macro used to restore the context to return from
;                   an exception handler and continue the program execution.
;* Input          : The range of registers to restore.
;* Output         : none
;*******************************************************************************/
	.macro RestoreContext $r0,$r3  
      
   LDMFD   sp!,{r0-r3,pc}^  /* Return to the instruction following...*/
                                /*...the exception interrupt.*/
	
	.endm          




/*;*******************************************************************************
;* Function Name  : UndefinedHandler
;* Description    : This function called when undefined instruction
;                   exception is entered.
;* Input          : none
;* Output         : none
;*******************************************************************************/

UndefinedHandler:
	SaveContext r0,r3      /*; Save the workspace plus the current*/
	BL Undefined_Handler /*; Branch to Undefined_Handler.*/
	RestoreContext r0,r3 /*; Return to the instruction following..*/
                           /*; ...the undefined instruction.*/

/*;*******************************************************************************
;* Function Name  : SWIHandler
;* Description    : This function called when SWI instruction executed.
;* Input          : none
;* Output         : none
;*******************************************************************************/

SWIHandler:
	SaveContext r0,r3     /*; Save the workspace plus the current*/
                          /*; return address lr_ svc and spsr_svc.*/
   BL SWI_Handler  /*; Branch to SWI_Handler.*/
	RestoreContext r0,r3 /*; Return to the instruction following...*/
                         /*; ...the SWI instruction.*/

                                                       
/*;*******************************************************************************
;* Function Name  : PrefetchAbortHandler
;* Description    : This function called when Prefetch Abort
;                   exception is entered.
;* Input          : none
;* Output         : none
;*******************************************************************************/

PrefetchAbortHandler:
	SUB    lr,lr,#4      /* ; Update the link register.*/
   SaveContext r0,r3     /*; Save the workspace plus the current*/
                          /*; return address lr_abt and spsr_abt.*/
   BL Prefetch_Handler /*; Branch to Prefetch_Handler. */
   RestoreContext r0,r3 /*; Return to the instruction following that... */
                         /*; ...has generated the prefetch abort exception.*/

/*;*******************************************************************************
;* Function Name  : DataAbortHandler
;* Description    : This function is called when Data Abort
;                   exception is entered.
;* Input          : none
;* Output         : none
;********************************************************************************/

DataAbortHandler:
   SUB    lr,lr,#8       /*; Update the link register.*/
   SaveContext r0,r3   /*; Save the workspace plus the current*/
                         /*; return address lr_ abt and spsr_abt.*/
   BL Abort_Handler  /*; Branch to Abort_Handler.*/
   RestoreContext r0,r3 /*; Return to the instruction following that...*/
                         /*; ...has generated the data abort exception.*/

/********************************************************************************
* Function Name  : IRQHandler
* Description    : This function is called when IRQ exception is entered.
* Input          : none
* Output         : none
********************************************************************************/

IRQHandler:
       SUB    lr,lr ,#4
       SaveContext r0,r3 
       LDR    r0, = VectorAddress
       LDR    r0, [r0]  /* Read the routine address from VIC0 Vector Address register  */   
                        
       BLX    r0        /* Branch with link to the IRQ handler. */       
       RestoreContext r0,r3

/***********************************************************************************************/
Reset_Handler:
	ldr    PC, =NextInst

NextInst:
/* ------------------------------------------------------------------------------
 Description  : This delay is added to let JTAG debuggers able to connect
                to STR91x micro then load the program even if a previously 
                flash code is halting the CPU core. This may happen when the
                flash content is corrupt by containing a "bad" code like entering
                soon to IDLE or SLEEP Low power modes.
                When going to production/Release code and to remove this Start-up
                delay, Please comment the "Debug_mode" define above.
 --------------------------------------------------------------------------------*/

.if  debug_mode               
                                                                  
      MOV     r0, #0x4000        
Loop:                              
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

/* ------------------------------------------------------------------------------
 Description  :  Enable the Buffered mode.
                 To use buffered mode access you have to uncomment Buffered  
                 define on the 91x_conf.h file
 ------------------------------------------------------------------------------*/

       MRC     p15, 0, r0, c1, c0, 0   /* Read CP15 register 1 into r0*/
       ORR     r0, r0, #0x8            /* Enable Write Buffer on AHB*/
       MCR     p15, 0, r0, c1, c0, 0  /* Write CP15 register 1 */ 
/*------------------------------------------------------------------------------
 Description  :  Write Buffer in ITCM may cause the Flash “write then read”    
                 command order reversed and cause flash error.
                 To maintain the right order, bit 18 (Instruction TCM order bit)
                 in the Configuration Registers of the ARM966E-S core must be set.           
 ------------------------------------------------------------------------------*/
    
       MOV     r0, #0x40000             
       MCR     p15,0x1,r0,c15,c1,0

/*------------------------------------------------------------------------------
 Description  : FMI Registers configuration depending on the Flash size selected, 
                and the boot bank.

     After reset, the application program has to write the size and start
     address of Bank 1 in the FMI_BBSR and FMI_BBADR registers and the size and 
     start address of Bank 0 in the FMI_NBBSR and FMI_NBBADR registers.
 ------------------------------------------------------------------------------*/

        LDR     R6, =FMI_BASE_Address   
        LDR     R7, = BOOT_BANK_Size          /* BOOT BANK Size=*/
        STR     R7, [R6, #FMI_BBSR_OFST]      /* (2^BOOT_BANK_Size) * 32KBytes  */     
        LDR     R7, = NON_BOOT_BANK_Size     /*  NON BOOT BANK Size =*/
        STR     R7, [R6, #FMI_NBBSR_OFST]    /*  (2^NON_BOOT_BANK_Size) * 8KBytes*/
        LDR     R7, =BOOT_BANK_Address       /*  BOOT BANK Address*/
        MOV     R7, R7 ,LSR #0x2   
        STR     R7, [R6, #FMI_BBADR_OFST]
        LDR     R7, =NON_BOOT_BANK_Address    /* BOOT BANK Address*/
        MOV     R7, R7 ,LSR #0x2              
        STR     R7, [R6, #FMI_NBBADR_OFST]    
  
       
        LDR     R7, = 0x19                    /* Enable Both banks*/
        STR     R7, [R6, #FMI_CR_OFST]        
                                              
      
/* --- Enable 96K of RAM  & Disable DTCM & AHB wait-states*/

        LDR     R0, = SCU_BASE_Address  
        LDR     R1, = 0x0191
        STR     R1, [R0, #SCU_SCR0_OFST]

  
/*; Initialize Stack pointer for various CPU Modes.*/
  
   
   msr    CPSR_c, #Mode_FIQ|I_Bit|F_Bit     /* No interrupts */
   ldr     SP, =FIQ_Stack           

	msr     CPSR_c, #Mode_IRQ|I_Bit|F_Bit    /* No interrupts */
   ldr     SP, =IRQ_Stack    
   
   MSR     CPSR_c, #Mode_ABT|I_Bit|F_Bit     /* No interrupts */
   LDR     SP, =ABT_Stack

   MSR     CPSR_c, #Mode_UNDEF|I_Bit|F_Bit   /* No interrupts */
   LDR     SP, =UNDEF_Stack

   MSR     CPSR_c, #Mode_SVC|I_Bit|F_Bit     /* No interrupts */
   LDR     SP, =_estack /*RAM_Limit*/
      
       
/* --- Now change to USR/SYS mode and set up User mode stack, */             
   MSR     CPSR_c, #Mode_SYS        /* IRQs & FIQs are now enable*/
   ldr     SP, =USR_Stack 
   
  	/* copy the initial values for .data section from FLASH to RAM */
	ldr	R1, =_sidata
	ldr	R2, =_sdata
	ldr	R3, =_edata
_reset_inidata_loop:
	cmp	R2, R3
	ldrlO	R0, [R1], #4
	strlO	R0, [R2], #4
	blO	_reset_inidata_loop

	/* Clear the .bss section */
	mov   r0,#0						/* get a zero */
	ldr   r1,=_sbss				/* point to bss start */
	ldr   r2,=_ebss				/* point to bss end */
_reset_inibss_loop:
	cmp   r1,r2						/* check if some data remains to clear */
	strlo r0,[r1],#4				/* clear 4 bytes */
	blo   _reset_inibss_loop	/* loop until done */


        
        
/************************************************************************************************/


/*; --- Now enter the C code */
             ldr    PC, =main
             
        
/*
END
/**************
****** (C) COPYRIGHT 2008 STMicroelectronics *****
****** (C) COPYRIGHT 2008 RAISONANCE *****

END OF FILE****/


