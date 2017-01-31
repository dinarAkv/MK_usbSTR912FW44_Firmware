
/*
Common part of the linker scripts for STR75x devices
Copyright RAISONANCE 2006
You can use, modify and distribute this file freely, but without any waranty.
*/


/* Registers mapping */
FLASHR_BASE     = 0x20100000 ;
FLASHPR_BASE    = 0x2010DFB0 ;
CFG_BASE        = 0x60000000 ;
MRCC_BASE       = 0x60000020 ;
EXTMEM_BASE     = 0x80000000 ;
SMI_BASE        = 0x90000000 ;
DMA_BASE        = 0xFFFFEC00 ;
EIC_BASE        = 0xFFFFF800 ;
APB_BASE        = 0xFFFF8000 ;

PROVIDE( _MRCC  =  MRCC_BASE );
PROVIDE( _CFG   =  CFG_BASE  );
PROVIDE( _SMI   =  SMI_BASE  );
PROVIDE( _DMA   =  DMA_BASE  );
PROVIDE( _EIC   =  EIC_BASE  );

PROVIDE( _ADC   = (APB_BASE + 0x0400) );
PROVIDE( _TB    = (APB_BASE + 0x0800) );
PROVIDE( _TIM0  = (APB_BASE + 0x0C00) );
PROVIDE( _TIM1  = (APB_BASE + 0x1000) );
PROVIDE( _TIM2  = (APB_BASE + 0x1400) );
PROVIDE( _PWM   = (APB_BASE + 0x1800) );
PROVIDE( _USB   = (APB_BASE + 0x2800) ); /* USB registers */
PROVIDE( _WDG   = (APB_BASE + 0x3000) );
PROVIDE( _SSP0  = (APB_BASE + 0x3800) );
PROVIDE( _SSP1  = (APB_BASE + 0x3C00) );
PROVIDE( _CAN   = (APB_BASE + 0x4400) );
PROVIDE( _I2C   = (APB_BASE + 0x4C00) );
PROVIDE( _UART0 = (APB_BASE + 0x5400) );
PROVIDE( _UART1 = (APB_BASE + 0x5800) );
PROVIDE( _UART2 = (APB_BASE + 0x5C00) );
PROVIDE( _GPIO0 = (APB_BASE + 0x6400) );
PROVIDE( _GPIO1 = (APB_BASE + 0x6440) );
PROVIDE( _GPIO2 = (APB_BASE + 0x6480) );
PROVIDE( _RTC   = (APB_BASE + 0x7000) );
PROVIDE( _EXTIT = (APB_BASE + 0x7400) );

/* default stack sizes. 

These are used by the startup in order to allocate stacks for the different modes.
*/

__SVC_Stack_Size = 32 ;
__IRQ_Stack_Size = 2048 ;
__USR_Stack_Size = 2048 ;
__FIQ_Stack_Size = 64 ;
__ABT_Stack_Size = 0 ;
__UND_Stack_Size = 0 ;

__SVC_Stack_Init = _estack ;
__IRQ_Stack_Init = __SVC_Stack_Init - __SVC_Stack_Size ;
__USR_Stack_Init = __IRQ_Stack_Init - __IRQ_Stack_Size ;
__FIQ_Stack_Init = __USR_Stack_Init - __USR_Stack_Size ;
__ABT_Stack_Init = __FIQ_Stack_Init - __FIQ_Stack_Size ;
__UND_Stack_Init = __ABT_Stack_Init - __ABT_Stack_Size ;

/*"PROVIDE" allows to easily override these values from an object file or the commmand line.*/
PROVIDE ( _SVC_Stack_Init = __SVC_Stack_Init ) ;
PROVIDE ( _IRQ_Stack_Init = __IRQ_Stack_Init ) ;
PROVIDE ( _USR_Stack_Init = __USR_Stack_Init ) ;
PROVIDE ( _FIQ_Stack_Init = __FIQ_Stack_Init ) ;
PROVIDE ( _ABT_Stack_Init = __ABT_Stack_Init ) ;
PROVIDE ( _UND_Stack_Init = __UND_Stack_Init ) ;

/*
the user mode stack is an exception because we want it at the end of the RAM.
therefore, we just check against a minimum.
The value below is the minimum memory required for ALL THE STACKS.
There will be a link error if there is not this amount of RAM free at the end.
*/
_Minimum_Stack_Size = 0xC00 ;



/* default ISR addresses. 

The startup needs these addresses defined from another object file.
In case they are not, these PROVIDEs redirect them to the Reset.

_start must be defined, usually in the startup.
*/

PROVIDE( SWI_Handler = _start );
PROVIDE( Undefined_Handler = _start );
PROVIDE( Prefetch_Handler = _start );
PROVIDE( Abort_Handler = _start );
PROVIDE( IRQ_Handler = _start ); /* <-- needed ? */
PROVIDE( FIQ_Handler = _start );
PROVIDE( WAKUP_IRQHandler = _start );
PROVIDE( TIM2_OC2_IRQHandler = _start );
PROVIDE( TIM2_OC1_IRQHandler = _start );
PROVIDE( TIM2_IC12_IRQHandler = _start );
PROVIDE( TIM2_UP_IRQHandler = _start );
PROVIDE( TIM1_OC2_IRQHandler = _start );
PROVIDE( TIM1_OC1_IRQHandler = _start );
PROVIDE( TIM1_IC12_IRQHandler = _start );
PROVIDE( TIM1_UP_IRQHandler = _start );
PROVIDE( TIM0_OC2_IRQHandler = _start );
PROVIDE( TIM0_OC1_IRQHandler = _start );
PROVIDE( TIM0_IC12_IRQHandler = _start );
PROVIDE( TIM0_UP_IRQHandler = _start );
PROVIDE( PWM_OC123_IRQHandler = _start );
PROVIDE( PWM_EM_IRQHandler = _start );
PROVIDE( PWM_UP_IRQHandler = _start );
PROVIDE( I2C_IRQHandler = _start );
PROVIDE( SSP1_IRQHandler = _start );
PROVIDE( SSP0_IRQHandler = _start );
PROVIDE( UART2_IRQHandler = _start );
PROVIDE( UART1_IRQHandler = _start );
PROVIDE( UART0_IRQHandler = _start );
PROVIDE( CAN_IRQHandler = _start );
PROVIDE( USB_LPI_RQHandler = _start );
PROVIDE( USB_HP_IRQHandler = _start );
PROVIDE( ADC_IRQHandler = _start );
PROVIDE( DMA_IRQHandler = _start );
PROVIDE( EXTIT_IRQHandler = _start );
PROVIDE( MRCC_IRQHandler = _start );
PROVIDE( FLASHSMI_IRQHandler = _start );
PROVIDE( RTC_IRQHandler = _start );
PROVIDE( TB_IRQHandler = _start );
PROVIDE( USB_LP_IRQHandler = _start );
PROVIDE( USB_HP_IRQHandler = _start );


/*
Linker subscript for STR75xFx2 definitions
Copyright RAISONANCE 2006
You can use, modify and distribute this file freely, but without any warranty.
*/


/*  Memory Spaces Definitions */


MEMORY
{
  FLASH (rx) : ORIGIN = 0x20006000, LENGTH = 256K-0x6000
  FLASHB1 (rx) : ORIGIN = 0x200C0000, LENGTH = 16K
  RAM (rwx) : ORIGIN = 0x40000800, LENGTH = 16K-0x800
  EXTMEMB0 (rx) : ORIGIN = 0x80000000, LENGTH = 16M
  EXTMEMB1 (rx) : ORIGIN = 0x82000000, LENGTH = 16M
  EXTMEMB2 (rx) : ORIGIN = 0x84000000, LENGTH = 16M
  EXTMEMB3 (rx) : ORIGIN = 0x86000000, LENGTH = 16M
}

/* higher address of the user mode stack */
_estack = 0x40004000;




/*
Common part of the linker scripts for STR71x devices in FLASH mode
(that is, the FLASH is seen at 0)
Copyright RAISONANCE 2005
You can use, modify and distribute thisfile freely, but without any waranty.
*/



/* Sections Definitions */

SECTIONS
{
    /* for Cortex devices, the beginning of the startup code is stored in the .isr_vector section, which goes to FLASH */
    .isr_vector :
    {
	. = ALIGN(4);
        KEEP(*(.isr_vector))            /* Startup code */
	. = ALIGN(4);
    } >FLASH
 
    /* for some STRx devices, the beginning of the startup code is stored in the .flashtext section, which goes to FLASH */
    .flashtext :
    {
	. = ALIGN(4);
        *(.flashtext)            /* Startup code */
	. = ALIGN(4);
    } >FLASH
 
    
    /* the program code is stored in the .text section, which goes to Flash */
    .text :
    {
	    . = ALIGN(4);
	    
        *(.text)                   /* remaining code */
        *(.text.*)                   /* remaining code */
        *(.rodata)                 /* read-only data (constants) */
        *(.rodata*)
        *(.glue_7)
        *(.glue_7t)

	    . = ALIGN(4);
   	 _etext = .;
	    /* This is used by the startup in order to initialize the .data secion */
   	 _sidata = _etext;
    } >FLASH
    
 

    /* This is the initialized data section
    The program executes knowing that the data is in the RAM
    but the loader puts the initial values in the FLASH (inidata).
    It is one task of the startup to copy the initial values from FLASH to RAM. */
    .data  : AT ( _sidata )
    {
	    . = ALIGN(4);
        /* This is used by the startup in order to initialize the .data secion */
        _sdata = . ;
        *(.coderam)
        *(.data)
        *(.data.*)      
         . = ALIGN(4);
	    /* This is used by the startup in order to initialize the .data secion */
   	 _edata = . ;
    } >RAM
    
    

    /* This is the uninitialized data section */
    .bss :
    {
	    . = ALIGN(4);
        /* This is used by the startup in order to initialize the .bss secion */
        _sbss = .;
        
        *(.bss)
        *(COMMON)
        
	    . = ALIGN(4);
	    /* This is used by the startup in order to initialize the .bss secion */
   	 _ebss = . ;
    } >RAM
    
    PROVIDE ( end = _ebss );
    PROVIDE ( _end = _ebss );
    
    /* This is the user stack section 
    This is just to check that there is enough RAM left for the User mode stack
    It should generate an error if it's full.
     */
    ._usrstack :
    {
	    . = ALIGN(4);
        _susrstack = . ;
        
        . = . + _Minimum_Stack_Size ;
        
	    . = ALIGN(4);
        _eusrstack = . ;
    } >RAM
    

   
    /* this is the FLASH Bank1 */
    /* the C or assembly source must explicitly place the code or data there
    using the "section" attribute */
    .b1text :
    {
        *(.b1text)                   /* remaining code */
        *(.b1rodata)                 /* read-only data (constants) */
        *(.b1rodata*)
    } >FLASHB1
    
    /* this is the EXTMEM */
    /* the C or assembly source must explicitly place the code or data there
    using the "section" attribute */
    
    /* EXTMEM Bank0 */
    .eb0text :
    {
        *(.eb0text)                   /* remaining code */
        *(.eb0rodata)                 /* read-only data (constants) */
        *(.eb0rodata*)
    } >EXTMEMB0
    
    /* EXTMEM Bank1 */
    .eb1text :
    {
        *(.eb1text)                   /* remaining code */
        *(.eb1rodata)                 /* read-only data (constants) */
        *(.eb1rodata*)
    } >EXTMEMB1
    
    /* EXTMEM Bank2 */
    .eb2text :
    {
        *(.eb2text)                   /* remaining code */
        *(.eb2rodata)                 /* read-only data (constants) */
        *(.eb2rodata*)
    } >EXTMEMB2
    
    /* EXTMEM Bank0 */
    .eb3text :
    {
        *(.eb3text)                   /* remaining code */
        *(.eb3rodata)                 /* read-only data (constants) */
        *(.eb3rodata*)
    } >EXTMEMB3
    
    __exidx_start = .;
    __exidx_end = .;
    
    /* after that it's only debugging information. */
    
    /* remove the debugging information from the standard libraries */
    /DISCARD/ :
    {
     libc.a ( * )
     libm.a ( * )
     libgcc.a ( * )
     }

    /* Stabs debugging sections.  */
    .stab          0 : { *(.stab) }
    .stabstr       0 : { *(.stabstr) }
    .stab.excl     0 : { *(.stab.excl) }
    .stab.exclstr  0 : { *(.stab.exclstr) }
    .stab.index    0 : { *(.stab.index) }
    .stab.indexstr 0 : { *(.stab.indexstr) }
    .comment       0 : { *(.comment) }
    /* DWARF debug sections.
       Symbols in the DWARF debugging sections are relative to the beginning
       of the section so we begin them at 0.  */
    /* DWARF 1 */
    .debug          0 : { *(.debug) }
    .line           0 : { *(.line) }
    /* GNU DWARF 1 extensions */
    .debug_srcinfo  0 : { *(.debug_srcinfo) }
    .debug_sfnames  0 : { *(.debug_sfnames) }
    /* DWARF 1.1 and DWARF 2 */
    .debug_aranges  0 : { *(.debug_aranges) }
    .debug_pubnames 0 : { *(.debug_pubnames) }
    /* DWARF 2 */
    .debug_info     0 : { *(.debug_info .gnu.linkonce.wi.*) }
    .debug_abbrev   0 : { *(.debug_abbrev) }
    .debug_line     0 : { *(.debug_line) }
    .debug_frame    0 : { *(.debug_frame) }
    .debug_str      0 : { *(.debug_str) }
    .debug_loc      0 : { *(.debug_loc) }
    .debug_macinfo  0 : { *(.debug_macinfo) }
    /* SGI/MIPS DWARF 2 extensions */
    .debug_weaknames 0 : { *(.debug_weaknames) }
    .debug_funcnames 0 : { *(.debug_funcnames) }
    .debug_typenames 0 : { *(.debug_typenames) }
    .debug_varnames  0 : { *(.debug_varnames) }
}




