/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : usb_endp.c
* Author             : MCD Application Team
* Version            : V2.0.0
* Date               : 09/29/2008
* Description        : Endpoint routines
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "91x_lib.h"
#include "usb_lib.h"
#include "usb_mem.h"
#include "hw_config.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
u8 Stream_Buff[24];
u16 Out_Data_Offset;

/* Extern variables ----------------------------------------------------------*/
extern u16 point;
extern u8 selector;

/* Private function prototypes -----------------------------------------------*/
/* Extern function prototypes ------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/*******************************************************************************
* Function Name  : EP1_OUT_Callback
* Description    : Endpoint 1 out callback routine.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void EP1_OUT_Callback(void)
{
  point = GetENDPOINT(ENDP1);
  if (point&EP_DTOG_TX)
  {
    selector = 2;
  }
  else
  {
    selector = 1;
  }
  Out_Data_Offset = 22;
}

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
