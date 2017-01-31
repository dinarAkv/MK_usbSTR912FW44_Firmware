/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : hw_config.c
* Author             : MCD Application Team
* Date First Issued  : 10/01/2008 : V1.0
* Description        : Hardware Configuration & Setup
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
#include "usb_prop.h"
#include "hw_config.h"
#include "usb_mem.h"
#include "usb_pwr.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
GPIO_InitTypeDef GPIO_InitStructure;
UART_InitTypeDef UART_InitStructure;
/* External variables --------------------------------------------------------*/
extern u8 buffer_in[];
extern u32 count_in;
extern LINE_CODING linecoding;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/*******************************************************************************
* Function Name  : Set_System.
* Description    : Configures Main system clocks & power
* Input          : None.
* Return         : None.
*******************************************************************************/




void Set_System(void)
{	  


  SCU_MCLKSourceConfig(SCU_MCLK_OSC);
   FMI_Config(FMI_READ_WAIT_STATE_2, FMI_WRITE_WAIT_STATE_0, FMI_PWD_ENABLE, \
             FMI_LVD_ENABLE, FMI_FREQ_HIGH);
  SCU_PLLFactorsConfig(192, 25, 2);
  SCU_PLLCmd(ENABLE);
  SCU_MCLKSourceConfig(SCU_MCLK_PLL);

  SCU_AHBPeriphClockConfig(__VIC, ENABLE);
  SCU_AHBPeriphReset(__VIC, DISABLE);




  /* USB clock = MCLK= 48MHz */
  SCU_USBCLKConfig(SCU_USBCLK_MCLK2);
  

  /* Enable USB clock */
  SCU_AHBPeriphClockConfig(__USB, ENABLE);
  SCU_AHBPeriphReset(__USB, DISABLE);

  SCU_AHBPeriphClockConfig(__USB48M, ENABLE);





  /** Configure 2.7 pin USBCLK. */
  
  SCU->GPIOOUT[0x02]  = 0x4000;

  
 




  /*configure UART0 interrupt in VIC*/
  VIC_DeInit();
  VIC_InitDefaultVectors(); /* initialize VICs default vector registers*/



}
/*******************************************************************************
* Function Name  : USB_Cable_Config
* Description    : Software Connection/Disconnection of USB Cable
* Input          : None.
* Return         : Status
*******************************************************************************/
void USB_Cable_Config (FunctionalState NewState)
{
  if (NewState == ENABLE)
  {
    GPIO_WriteBit(GPIO0, GPIO_Pin_1, Bit_RESET);
  }
  else
  {
    GPIO_WriteBit(GPIO0, GPIO_Pin_1, Bit_SET);
  }
}
/*******************************************************************************
* Function Name  : Enter_LowPowerMode.
* Description    :
* Input          : None.
* Return         : None.
*******************************************************************************/
void Enter_LowPowerMode(void)
{
  /* Set the device state to suspend */
  bDeviceState = SUSPENDED;
}
/*******************************************************************************
* Function Name  : Leave_LowPowerMode.
* Description    :
* Input          : None.
* Return         : None.
*******************************************************************************/
void Leave_LowPowerMode(void)
{
  DEVICE_INFO *pInfo = &Device_Info;

  /* Set the device state to the correct state */
  if (pInfo->Current_Configuration != 0)
  {
    /* Device configured */
    bDeviceState = CONFIGURED;
  }
  else
  {
    bDeviceState = ATTACHED;
  }
}
/*******************************************************************************
* Function Name  : USB_Interrupts_Config.
* Description    : configure and enable the USB interrupt Lines.
* Input          : None.
* Return         : None.
*******************************************************************************/
void USB_Interrupts_Config(void)
{
  VIC_Config(USBLP_ITLine, VIC_IRQ, 3);
  VIC_ITCmd(USBLP_ITLine, ENABLE);
}
/*******************************************************************************
* Function Name  :  UART0_Config_Default.
* Description    :  configure the UART 0 with default values.
* Input          :  None.
* Return         :  None.
*******************************************************************************/
void UART0_Config_Default (void)
{
  UART_DeInit(UART0);
  UART_InitStructure.UART_WordLength = UART_WordLength_8D;
  UART_InitStructure.UART_Parity = UART_Parity_No;
  UART_InitStructure.UART_StopBits = UART_StopBits_1;
  UART_InitStructure.UART_BaudRate = 115200;
  UART_InitStructure.UART_HardwareFlowControl = UART_HardwareFlowControl_None;
  UART_InitStructure.UART_Mode = UART_Mode_Tx_Rx;
  UART_InitStructure.UART_FIFO = UART_FIFO_Disable;
  
  UART_Init(UART0, &UART_InitStructure);

  /* Enable the UART Receive interrupt: this interrupt is generated when the UART0
     receive FIFO hold two data */
  UART_ITConfig(UART0, UART_IT_Receive, ENABLE);
  
  UART_Cmd(UART0, ENABLE);
}
/*******************************************************************************
* Function Name  :  UART0_Config.
* Description    :  Configure the UART 0 according to the linecoding structure.
* Input          :  None.
* Return         :  Configuration status
                    TRUE : configuration done with success
                    FALSE : configuration aborted.
*******************************************************************************/
bool UART0_Config(void)
{
  /* set the bit rate*/
  if (linecoding.bitrate > 460800)
  {
    UART0_Config_Default();
    return (FALSE);
  }
  /* set the Stop bit*/
  switch (linecoding.format)
  {
    case 0:
      UART_InitStructure.UART_StopBits = UART_StopBits_1;
      break;
    case 2:
      UART_InitStructure.UART_StopBits = UART_StopBits_2;
      break;
    default :
    {
      UART0_Config_Default();
      return (FALSE);
    }
  }
  /* set the parity bit*/
  switch (linecoding.paritytype)
  {
    case 0:
      UART_InitStructure.UART_Parity = UART_Parity_No;
      break;
    case 1:
      UART_InitStructure.UART_Parity = UART_Parity_Even;
      break;
    case 2:
      UART_InitStructure.UART_Parity = UART_Parity_Odd;
      break;
    case 3:
      UART_InitStructure.UART_Parity = UART_Parity_OddStick;
      break;
    case 4:
      UART_InitStructure.UART_Parity = UART_Parity_EvenStick;
      break;
    default :
    {
      UART0_Config_Default();
      return (FALSE);
    }
  }
  /*set the data type*/
  switch (linecoding.datatype)
  {
    case 0x08:
      UART_InitStructure.UART_WordLength = UART_WordLength_8D;
      break;
    case 0x07:
      UART_InitStructure.UART_WordLength = UART_WordLength_7D;
      break;
    case 0x06:
      UART_InitStructure.UART_WordLength = UART_WordLength_6D;
      break;
    case 0x05:
      UART_InitStructure.UART_WordLength = UART_WordLength_5D;
      break;
    default :
    {
      UART0_Config_Default();
      return (FALSE);
    }
  }

  UART_InitStructure.UART_BaudRate = linecoding.bitrate;
  UART_InitStructure.UART_HardwareFlowControl = UART_HardwareFlowControl_None;
  UART_InitStructure.UART_Mode = UART_Mode_Tx_Rx;
  UART_InitStructure.UART_FIFO = UART_FIFO_Disable;
 
  UART_Init(UART0, &UART_InitStructure);
  UART_Cmd(UART0, ENABLE);
  return (TRUE);
}
/*******************************************************************************
* Function Name  : USB_To_UART_Send_Data.
* Description    : send the received data from USB to the UART 0.
* Input          : data_buffer: data address.
                   Nb_bytes: number of bytes to send.
* Return         : none.
*******************************************************************************/
void USB_To_UART_Send_Data(u8* data_buffer, u8 Nb_bytes)
{
  u32 i;	

  for (i = 0;i < Nb_bytes;i++)
  {
    UART_SendData(UART0, *(data_buffer + i));
  }
}
/*******************************************************************************
* Function Name  : UART_To_USB_Send_Data.
* Description    : send the received data from UART 0 to USB.
* Input          : None.
* Return         : none.
*******************************************************************************/
void UART_To_USB_Send_Data(void)
{
  buffer_in[count_in] = UART_ReceiveData(UART0);
  count_in++;
  UserToPMABufferCopy(buffer_in, ENDP1_TXADDR, count_in);
  SetEPTxCount(ENDP1, count_in);
  SetEPTxValid(ENDP1);
}
/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/


