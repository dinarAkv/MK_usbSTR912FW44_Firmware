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
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
u8 Receive_Buffer[2];

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/*******************************************************************************
* Function Name  : EP1_OUT_Callback.
* Description    : EP1 OUT Callback Routine.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void EP1_OUT_Callback(void)
{
  BitAction Led_State;
  
  PMAToUserBufferCopy(Receive_Buffer, ENDP1_RXADDR, 2);
  
  if (Receive_Buffer[1] == 0)
  {
    Led_State = Bit_SET;
  }
  else 
  {
    Led_State = Bit_RESET;
  }
 
 
  switch (Receive_Buffer[0])
  {
    case 1: /* Led 2 */
      GPIO_WriteBit(GPIO9, GPIO_Pin_0, Led_State);
     break;
    case 2: /* Led 3 */
      GPIO_WriteBit(GPIO9, GPIO_Pin_1, Led_State);
      break;
    case 3: /* Led 4 */
      GPIO_WriteBit(GPIO9, GPIO_Pin_2, Led_State);
      break;
    case 4: /* Led 5 */
      GPIO_WriteBit(GPIO9, GPIO_Pin_3, Led_State);
      break;
  default:
    GPIO_Write(GPIO9, 0xFF);
    break;
  }
  
  SetEPRxStatus(ENDP1, EP_RX_VALID);
}

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/

