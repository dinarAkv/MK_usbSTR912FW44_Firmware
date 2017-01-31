/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : usb_desc.h
* Author             : MCD Application Team
* Version            : V2.0.0
* Date               : 09/29/2008
* Description        : Descriptor Header for Device Firmware Upgrade (DFU)
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USB_DESC_H
#define __USB_DESC_H
/* Includes ------------------------------------------------------------------*/
#include "91x_conf.h"
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
/* External variables --------------------------------------------------------*/

#define DFU_SIZ_DEVICE_DESC             18

#define DFU_SIZ_CONFIG_DESC             36


#define DFU_SIZ_STRING_LANGID           4
#define DFU_SIZ_STRING_VENDOR           38
#define DFU_SIZ_STRING_PRODUCT          22
#define DFU_SIZ_STRING_SERIAL           8

#ifdef Flash_512KB_256KB
#define DFU_SIZ_STRING_INTERFACE0       74     /* Flash Bank 0 */
#endif

#ifdef Flash_2MB_1MB
#define DFU_SIZ_STRING_INTERFACE0       76     /* Flash Bank 0 */
#endif

#define DFU_SIZ_STRING_INTERFACE1       74     /* Flash Bank 1 */

extern const u8 DFU_DeviceDescriptor[DFU_SIZ_DEVICE_DESC];
extern const u8 DFU_ConfigDescriptor[DFU_SIZ_CONFIG_DESC];


extern const u8 DFU_StringLangId     [DFU_SIZ_STRING_LANGID];
extern const u8 DFU_StringVendor     [DFU_SIZ_STRING_VENDOR];
extern const u8 DFU_StringProduct    [DFU_SIZ_STRING_PRODUCT];
extern const u8 DFU_StringSerial     [DFU_SIZ_STRING_SERIAL];
extern const u8 DFU_StringInterface0 [DFU_SIZ_STRING_INTERFACE0];
extern const u8 DFU_StringInterface1 [DFU_SIZ_STRING_INTERFACE1];

#define bMaxPacketSize0             0x40     /* bMaxPacketSize0 = 64 bytes   */
#define wTransferSize               0x0400   /* wTransferSize   = 1024 bytes */
/* bMaxPacketSize0 <= wTransferSize <= 32kbytes */
#define wTransferSizeB0             0x00
#define wTransferSizeB1             0x04


#endif /* __USB_DESC_H */

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
