////////////////////////////////////////////////////////////////////////////
//
//  File        :  arm.lsl
//
//  Version     :  @(#)arm.lsl  1.14  06/04/21
//
//  Description :  LSL file for ARM processors
//
//  Modified by :  Lue/Hitex Development Tools 07\03\20  	
//  	     for:  STR912		
//
//  Copyright 2003 Altium BV
//
////////////////////////////////////////////////////////////////////////////

#ifndef __STACK
#  define __STACK                       10k
#endif
#ifndef __HEAP
#  define __HEAP                        2k
#endif
#ifndef __STACK_FIQ
#  define __STACK_FIQ                   0x400
#endif
#ifndef __STACK_IRQ
#  define __STACK_IRQ                   0x800
#endif
#ifndef __STACK_SVC
#  define __STACK_SVC                   0x100
#endif
#ifndef __STACK_ABT
#  define __STACK_ABT                   0x100
#endif
#ifndef __STACK_UND
#  define __STACK_UND                   0x100
#endif
#ifndef __PROCESSOR_MODE
#  define __PROCESSOR_MODE              0x10            /* User mode */
#endif
#ifndef __IRQ_BIT
#  define __IRQ_BIT                     0x40           /* IRQ interrupts disabled */
#endif
#ifndef __FIQ_BIT
#  define __FIQ_BIT                     0x80           /* FIQ interrupts disabled */
#endif
//#ifndef __START
//#  define __START                       0x00000020
//#endif
#define __APPLICATION_MODE              (__PROCESSOR_MODE | __IRQ_BIT | __FIQ_BIT)

#ifndef __VECTOR_TABLE_ROM_ADDR
#  define __VECTOR_TABLE_ROM_ADDR       0x00000000
#endif

#ifndef __VECTOR_TABLE_RAM_ADDR
#  define __VECTOR_TABLE_RAM_ADDR       0x04000020
#endif

#ifdef __PIC_VECTORS
#  define __VECTOR_TABLE_SIZE           64
#else
#  ifdef __FIQ_HANDLER_INLINE
#    define __VECTOR_TABLE_SIZE         28
#    define __NR_OF_VECTORS             7
#  else
#    define __VECTOR_TABLE_SIZE         32
#    define __NR_OF_VECTORS             8
#  endif
#endif

#ifndef __VECTOR_TABLE_RAM_SPACE
#  undef __VECTOR_TABLE_RAM_COPY
#endif

////////////////////////////////////////////////////////////////////////////
//
// memory definition
//

memory xrom
{
    mau = 8;
    type = rom;
    size = 512k;
    map ( size = 512k, dest_offset=0x00000000, dest=bus:ARM:local_bus);
}

memory xram
{
    mau = 8;
    type = ram;
    size = 96k;
    map ( size = 96k, dest_offset=0x04000000, dest=bus:ARM:local_bus);
}

/////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////
// Define architecture

architecture ARM
{
        endianness
        {
                little;
                big;
        }

        space linear
        {
                id = 1;
                mau = 8;
                map (size = 4G, dest = bus:local_bus);

                copytable
                (
                        align = 4,
                        copy_unit = 1,
                        dest = linear
                );

                start_address
                (
                        // It is not strictly necessary to define a run_addr for _START
                        // because hardware starts execution at address 0x0 which should
                        // be the vector table with a jump to the relocatable _START, but
                        // an absolute address can prevent the branch to be out-of-range.
                        // Or _START may be the entry point at reset and the reset handler
                        // copies the vector table to address 0x0 after some ROM/RAM memory
                        // re-mapping. In that case _START should be at a fixed address
                        // in ROM, specifically the alias of address 0x0 before memory
                        // re-mapping.
#ifdef __START
                        run_addr = __START,
#endif
                        symbol = "_START"
                );

                stack "stack"
                (
#ifdef __STACK_FIXED
                        fixed,
#endif
                        align = 4,
                        min_size = __STACK,
                        grows = high_to_low
                );

                heap "heap"
                (
#ifdef __HEAP_FIXED
                        fixed,
#endif
                        align = 4,
                        min_size=__HEAP
                );

                stack "stack_fiq"
                (
                        fixed,
                        align = 4,
                        min_size = __STACK_FIQ,
                        grows = high_to_low
                );
                stack "stack_irq"
                (
                        fixed,
                        align = 4,
                        min_size = __STACK_IRQ,
                        grows = high_to_low
                );
                stack "stack_svc"
                (
                        fixed,
                        align = 4,
                        min_size = __STACK_SVC,
                        grows = high_to_low
                );
                stack "stack_abt"
                (
                        fixed,
                        align = 4,
                        min_size = __STACK_ABT,
                        grows = high_to_low
                );
                stack "stack_und"
                (
                        fixed,
                        align = 4,
                        min_size = __STACK_UND,
                        grows = high_to_low
                );

#ifndef __NO_AUTO_VECTORS
#  ifdef __PIC_VECTORS
                // vector table with ldrpc instructions from handler table
                vector_table "vector_table" ( vector_size = 4, size = 8, run_addr = __VECTOR_TABLE_ROM_ADDR,
                                              template = ".text.vector_ldrpc",
                                              template_symbol = "_lc_vector_ldrpc",
                                              vector_prefix = "_vector_ldrpc_",
                                              fill = loop
                                            )
                {
                }
                // subsequent vector table (data pool) with addresses of handlers
                vector_table "handler_table" ( vector_size = 4, size = 8, run_addr = __VECTOR_TABLE_ROM_ADDR + 32,
                                               template = ".text.handler_address",
                                               template_symbol = "_lc_vector_handler",
                                               vector_prefix = "_vector_",
                                               fill = loop[-32],
                                               no_inline
                                             )
                {
                        vector ( id = 0, fill = "_START" );
                }
#  else
                // vector table with branch instructions to handlers
                
                vector_table "vector_table" ( vector_size = 4, size = __NR_OF_VECTORS, run_addr = __VECTOR_TABLE_ROM_ADDR,
                                              template = ".text.vector_branch",
                                              template_symbol = "_lc_vector_handler",
                                              vector_prefix = "_vector_",
                                              fill = loop
                                            )
                {
                        vector ( id = 0, fill = "_START" );
                }
#  endif
#endif

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Locate sections

  section_layout ARM:ARM:Linear
                {
#ifdef __NO_AUTO_VECTORS
                        "_lc_ub_vector_table" = __VECTOR_TABLE_ROM_ADDR;
                        "_lc_ue_vector_table" = __VECTOR_TABLE_ROM_ADDR + __VECTOR_TABLE_SIZE;
#endif
#ifdef __VECTOR_TABLE_RAM_SPACE
                        // reserve space to copy vector table from ROM to RAM
                        group ( ordered, run_addr = __VECTOR_TABLE_RAM_ADDR )
                                reserved "vector_table_space" ( size = __VECTOR_TABLE_SIZE, attributes = rwx );
#endif
#ifdef __VECTOR_TABLE_RAM_COPY
                        // provide copy address symbols for copy routine
                        "_lc_ub_vector_table_copy" := "_lc_ub_vector_table_space";
                        "_lc_ue_vector_table_copy" := "_lc_ue_vector_table_space";
#else
                        // prevent copy: copy address equals orig address
                        "_lc_ub_vector_table_copy" := "_lc_ub_vector_table";
                        "_lc_ue_vector_table_copy" := "_lc_ue_vector_table";
#endif

                        // define labels for begin and end symbols as used in C library
                        group grp_bounds ( ordered, contiguous )
                        {
                                select "bounds";
                        }
                        "_lc_ub_bounds" := addressof(group:grp_bounds);
                        "_lc_ue_bounds" := addressof(group:grp_bounds) + sizeof(group:grp_bounds);

#ifdef __HEAPADDR
                        group ( ordered, run_addr=__HEAPADDR )
                        {
                                select "heap";
                        }
#endif
#ifdef __STACKADDR
                        group ( ordered, run_addr=__STACKADDR )
                        {
                                select "stack";
                        }
#endif
                        group ( contiguous )
                        {
                                select "stack_fiq";
                                select "stack_irq";
                                select "stack_svc";
                                select "stack_abt";
                                select "stack_und";
			}

                        // symbol to set mode bits and interrupt disable bits
                        // in cstart module before calling the application (main)
                        "_APPLICATION_MODE_" = __APPLICATION_MODE;
		
                }

        }

        bus local_bus
        {
                mau = 8;
                width = 32;
        }
}




