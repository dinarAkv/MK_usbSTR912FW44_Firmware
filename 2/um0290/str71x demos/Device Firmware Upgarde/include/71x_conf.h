/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : 71x_conf.h
* Author             : MCD Application Team
* Version            : V2.0.0
* Date               : 09/29/2008
* Description        : Library configuration file.
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __71x_CONF_H
#define __71x_CONF_H

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Uncomment the line below to compile the library in DEBUG mode */
/* #define DEBUG  1 */


#ifdef __IAR_SYSTEMS_ICC__     /* IAR Compiler */
#define RAM_exe  __ramfunc
#endif

#ifdef  __CC_ARM               /* ARM Compiler */
#define RAM_exe
#endif

#ifdef  __GNUC__              /* GNU Compiler */
#define RAM_exe
#define RIDE_REMAP_RAM  *(vu32 *)(0xA0000050) &= 0xFFFFFFFC; \
                        *(vu32 *)(0xA0000050) |= 0x0002;
#endif

#ifdef __TASKING__          /*TASKING compiler*/
#define RAM_exe
#endif

/* Comment the line below to disable the specific peripheral inclusion */
/************************************* ADC ************************************/
//#define _ADC12

/************************************* APB ************************************/
#define _APB 
#define _APB1 
#define _APB2 

/************************************* BSPI ***********************************/
//#define _BSPI 
//#define _BSPI0 
//#define _BSPI1 

/************************************* CAN ************************************/
//#define _CAN 

/************************************* EIC ************************************/
#define _EIC 

/************************************* EMI ************************************/
#define _EMI 

/************************************* GPIO ***********************************/
#define _GPIO
#define _GPIO0 
#define _GPIO1
#define _GPIO2

/************************************* I2C ************************************/
//#define _I2C 
//#define _I2C0 
//#define _I2C1

/************************************* PCU ************************************/
#define _PCU

/************************************* RCCU ***********************************/
#define _RCCU

/************************************* RTC ************************************/
//#define _RTC

/************************************* TIM ************************************/
//#define _TIM
//#define _TIM0
//#define _TIM1
//#define _TIM2 
//#define _TIM3 

/************************************* UART ***********************************/
//#define _UART 
//#define _UART0 
//#define _UART1 
//#define _UART2 
//#define _UART3

/************************************* Serial Port ****************************/
/* Uncomment only one line*/
//#define USE_SERIAL_PORT  
//#define USE_UART0 
//#define USE_UART1 
//#define USE_UART2 
//#define USE_UART3 

/************************************* WDG ************************************/
#define _WDG 

/************************************* XTI ************************************/
#define _XTI

/*  Main Oscillator Frequency value = 16 MHz */
#define RCCU_Main_Osc 16000000

//#define _IRQVectors

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#endif /* __71x_CONF_H */

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/

