/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : Readme.txt
* Author             : MCD Application Team
* Version            : V2.1
* Date               : 12/22/2008
* Description        : Description of the ADC Example 2.
********************************************************************************
* THE PRESENT SOFTWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

Example description
===================
Program using the ADC peripheral triggered through the external Pin to start the
conversion.
TIM0 is configured to generate the external trigger signal and injected to pin P4.7.
TIM1 is configured to generate a PWM signal to be the input for the ADC channel6, 
The frequency of this signal is the half of the external trigger’s frequency.
When 16 samples are transferred to internal SRAM (using the DMA capability) the ADC
is disabled.  


                                |----|     |----|    |----|    |----|        
                                |    |     |    |    |    |    |    | 
                                ^    |     ^    |    ^    |    ^    | 
        External Trigger : -----|    |-----|    |----|    |----|    |----
                           
                                       3.3 Volts              3.3 Volts
                                      |----------|         |----------|
                                      |          |         |          |
                             0 Volt   |          | 0 Volt  |          | 0 Volt 
            Input signal : -----------|          |---------|          |-------
                                        
          
for each external trigger's rising edge one sample is captured and transferred to 
internal SRAM "Dest_Buffer" through DMA.





Directory contents
==================
91x_conf.h  Library Configuration file
main.c      Main program
91x_it.c    Interrupt handlers
91x_it.h    Interrupt handlers header file
readme.txt   This file

Hardware environment
====================
 - Connect pin P4.7 to pin6.0 (on STR91x-EVAL board).
 - Connect pin P4.2 to pin4.6 (on STR91x-EVAL board).
 - Remove jumper JP1.
   

How to use it
=============
In order to make the program work, you must do the following :
- Create a project and setup all your toolchain's start-up files
- Compile the directory content files and required Library files :
  + 91x_adc.c
  + 91x_lib.c
  + 91x_tim.c
  + 91x_gpio.c
  + 91x_scu.c
  + 91x_vic.c
  + 91x_fmi.c
      
- Link all compiled files and load your image into Flash
- Run the example


******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****
