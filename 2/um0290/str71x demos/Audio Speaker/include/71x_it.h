/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : 71x_it.h
* Author             : MCD Application Team
* Version            : V2.0.0
* Date               : 09/29/2008
* Description        : Main Interrupt Service Routines header.
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _71x_IT_H
#define _71x_IT_H

/* Includes ------------------------------------------------------------------*/
#include "71x_lib.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void Undefined_Handler   (void);
void FIQ_Handler         (void);
void SWI_Handler         (void);
void Prefetch_Handler    (void);
void Abort_Handler       (void);
void T0TIMI_IRQHandler   (void);
void FLASH_IRQHandler    (void);
void RCCU_IRQHandler     (void);
void RTC_IRQHandler      (void);
void WDG_IRQHandler      (void);
void XTI_IRQHandler      (void);
void USBHP_IRQHandler    (void);
void I2C0ITERR_IRQHandler(void);
void I2C1ITERR_IRQHandler(void);
void UART0_IRQHandler    (void);
void UART1_IRQHandler    (void);
void UART2_IRQHandler    (void);
void UART3_IRQHandler    (void);
void BSPI0_IRQHandler    (void);
void BSPI1_IRQHandler    (void);
void I2C0_IRQHandler     (void);
void I2C1_IRQHandler     (void);
void CAN_IRQHandler      (void);
void ADC12_IRQHandler    (void);
void T1TIMI_IRQHandler   (void);
void T2TIMI_IRQHandler   (void);
void T3TIMI_IRQHandler   (void);
void HDLC_IRQHandler     (void);
void USBLP_IRQHandler    (void);
void T0TOI_IRQHandler    (void);
void T0OC1_IRQHandler    (void);
void T0OC2_IRQHandler    (void);
#endif /* _71x_IT_H */

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
