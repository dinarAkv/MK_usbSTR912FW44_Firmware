/*
Common part of the linker scripts for STR91x devices
Copyright RAISONANCE 2006
You can use, modify and distribute thisfile freely, but without any waranty.
*/
/*
Linker subscript for STR91xFx44 definitions
Copyright RAISONANCE 2006
You can use, modify and distribute this file freely, but without any warranty.
*/
EXTMEM_BASE     = 0x30000000 ;

FMI_BASE        = 0x54000000 ;
APB0_BASE       = 0x58000000 ;
APB1_BASE       = 0x5C000000 ;

USB_BASE        = 0x70000000 ;
EMI_BASE        = 0x74000000 ;
DMA_BASE        = 0x78000000 ;

VIC0_BASE       = 0xFFFFF000 ;
VIC1_BASE       = 0xFC000000 ;


PROVIDE( _DMA      = (DMA_BASE) );
PROVIDE( _EMI      = (EMI_BASE) );

PROVIDE( _VIC0     = (VIC0_BASE) );
PROVIDE( VIC0VECT  = (VIC0_BASE + 0x0030) );
PROVIDE( _VIC1     = (VIC1_BASE) );
PROVIDE( VIC1VECT  = (VIC1_BASE + 0x0030) );

PROVIDE( _USB      = (USB_BASE) );

PROVIDE( _AHBAPB0  = (APB0_BASE) );
PROVIDE( _WIU      = (APB0_BASE + 0x1000) );
PROVIDE( _TIM0     = (APB0_BASE + 0x2000) );
PROVIDE( _TIM1     = (APB0_BASE + 0x3000) );
PROVIDE( _TIM2     = (APB0_BASE + 0x4000) );
PROVIDE( _TIM3     = (APB0_BASE + 0x5000) );
PROVIDE( _GPIO0    = (APB0_BASE + 0x6000) );
PROVIDE( _GPIO1    = (APB0_BASE + 0x7000) );
PROVIDE( _GPIO2    = (APB0_BASE + 0x8000) );
PROVIDE( _GPIO3    = (APB0_BASE + 0x9000) );
PROVIDE( _GPIO4    = (APB0_BASE + 0xA000) );
PROVIDE( _GPIO5    = (APB0_BASE + 0xB000) );
PROVIDE( _GPIO6    = (APB0_BASE + 0xC000) );
PROVIDE( _GPIO7    = (APB0_BASE + 0xD000) );
PROVIDE( _GPIO8    = (APB0_BASE + 0xE000) );
PROVIDE( _GPIO9    = (APB0_BASE + 0xF000) );

PROVIDE( _AHBAPB1  = (APB1_BASE) );
PROVIDE( _RTC      = (APB1_BASE + 0x1000) );
PROVIDE( _SCU      = (APB1_BASE + 0x2000) );
PROVIDE( _MC       = (APB1_BASE + 0x3000) );
PROVIDE( _UART0    = (APB1_BASE + 0x4000) );
PROVIDE( _UART1    = (APB1_BASE + 0x5000) );
PROVIDE( _UART2    = (APB1_BASE + 0x6000) );
PROVIDE( _SPI0     = (APB1_BASE + 0x7000) );
PROVIDE( _SPI1     = (APB1_BASE + 0x8000) );
PROVIDE( _CAN      = (APB1_BASE + 0x9000) );
PROVIDE( _ADC      = (APB1_BASE + 0xA000) );
PROVIDE( _WDG      = (APB1_BASE + 0xB000) );
PROVIDE( _I2C0     = (APB1_BASE + 0xC000) );
PROVIDE( _I2C1     = (APB1_BASE + 0xD000) );

/* Memory Spaces Definitions */

MEMORY
{
  FLASH (rx) : ORIGIN = 0, LENGTH = 32K
  FLASHB1 (rx) : ORIGIN = 0x80000, LENGTH = 512K
  EXTMEMB0 (rx) : ORIGIN = 0x30000000, LENGTH = 64M
  EXTMEMB1 (rx) : ORIGIN = 0x34000000, LENGTH = 64M
  EXTMEMB2 (rx) : ORIGIN = 0x38000000, LENGTH = 64M
  EXTMEMB3 (rx) : ORIGIN = 0x3C000000, LENGTH = 64M 
  RAM (xrw) : ORIGIN = 0x50000000, LENGTH = 96K
}

/* higher address of the user mode stack */
_estack = 0x50018000;

/*PROVIDE (  _bb_size = <bb_size> ) ;
PROVIDE ( _nbb_size = <nbb_size> ) ;
PROVIDE ( _nbb_addr = <nbb_addr> ) ;*/




/* default stack sizes. 

These are used by the startup in order to allocate stacks for the different modes.
*/

__SVC_Stack_Size = 256 ;
__IRQ_Stack_Size = 1024 ;
__USR_Stack_Size = 4096 ;
__FIQ_Stack_Size = 256 ;
__ABT_Stack_Size = 256 ;
__UND_Stack_Size = 256 ;

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

PROVIDE( Undefined_Handler = _start );
PROVIDE( SWI_Handler = _start );
PROVIDE( Prefetch_Handler = _start );
PROVIDE( Abort_Handler = _start );
PROVIDE( FIQ_Handler = _start );
PROVIDE( T0TIMI_IRQHandler = _start );
PROVIDE( FLASH_IRQHandler = _start );
PROVIDE( RCCU_IRQHandler = _start );
PROVIDE( RTC_IRQHandler = _start );
PROVIDE( WDG_IRQHandler = _start );
PROVIDE( XTI_IRQHandler = _start );
PROVIDE( USBHP_IRQHandler = _start );
PROVIDE( I2C0ITERR_IRQHandler = _start );
PROVIDE( I2C1ITERR_IRQHandler = _start );
PROVIDE( UART0_IRQHandler = _start );
PROVIDE( UART1_IRQHandler = _start );
PROVIDE( UART2_IRQHandler = _start );
PROVIDE( UART3_IRQHandler = _start );
PROVIDE( BSPI0_IRQHandler = _start );
PROVIDE( BSPI1_IRQHandler = _start );
PROVIDE( I2C0_IRQHandler = _start );
PROVIDE( I2C1_IRQHandler = _start );
PROVIDE( CAN_IRQHandler = _start );
PROVIDE( ADC12_IRQHandler = _start );
PROVIDE( T1TIMI_IRQHandler = _start );
PROVIDE( T2TIMI_IRQHandler = _start );
PROVIDE( T3TIMI_IRQHandler = _start );
PROVIDE( HDLC_IRQHandler = _start );
PROVIDE( USBLP_IRQHandler = _start );
PROVIDE( T0TOI_IRQHandler = _start );
PROVIDE( T0OC1_IRQHandler = _start );
PROVIDE( T0OC2_IRQHandler = _start );




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




