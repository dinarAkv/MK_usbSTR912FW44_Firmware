/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : main.c
* Author             : MCD Application Team
* Version            : V2.0.0
* Date               : 09/29/2008
* Description        : Microphone Demo main file
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "usb_lib.h"
#include "hw_config.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
u32 Micro_Start = 0;

/* Private function prototypes -----------------------------------------------*/
/* Extern function prototypes ------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/*******************************************************************************
* Function Name  : main
* Description    : main program.
* Input          : None.
* Return         : None.
*******************************************************************************/
int main(void)
{
#ifdef DEBUG
  debug();
#endif

  Set_System();
  Set_USBClock();
  USB_Interrupts_Config();
  USB_Init();
  /* Main loop */
  while (1)
  {
    if ((pInformation->Current_Interface == 1) 
        && (pInformation->Current_AlternateSetting == 1))
    {
      Micro_Start = 1;
      Voice_Micro_Start();
    }
    else
    {
      Micro_Start = 0;
      Voice_Micro_Stop();
    }
  }
}
/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
