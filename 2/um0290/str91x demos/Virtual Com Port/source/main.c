/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : main.c
* Author             : MCD Application Team
* Date First Issued  : 10/01/2008 : V1.0
* Description        : Virtual Com Port demo main file
********************************************************************************
* History:
* 10/01/2008 : V1.0
********************************************************************************
* THE PRESENT SOFTWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/
/* Includes ------------------------------------------------------------------*/
#include "91x_lib.h"
#include "usb_lib.h"
#include "usb_conf.h"
#include "usb_prop.h"
#include "usb_pwr.h"
#include "hw_config.h"

#include "usb_regs.h"



/*******************************************************************************
* Function Name  : main
* Description    : This project show work of virtual com port for STR912FW44.
				   It contain special usb user library for send and
				   recieve data through usb interface
* Input          : None.
* Return         : None.
*******************************************************************************/









int main()
{
	


#ifdef DEBUG
  debug();
#endif
  

  Set_System();
  USB_Interrupts_Config();
  USB_Init();
	

 






  while (1)
  {

	

  }
}
/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
