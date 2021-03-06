/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : hw_config.c
* Author             : MCD Application Team
* Version            : V2.0.0
* Date               : 09/29/2008
* Description        : Hardware Configuration & Setup
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
#include "hw_config.h"
#include "usb_pwr.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
GPIO_InitTypeDef GPIO_InitStructure;
DMA_InitTypeDef  DMA_InitStruct;
u8 Stream_Buff[24];

/* External variables --------------------------------------------------------*/
extern u8 selector;

/* Private function prototypes -----------------------------------------------*/
/* Extern function prototypes ------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/*******************************************************************************
* Function Name  : Set_System
* Description    : Configures Main system clocks & power.
* Input          : None.
* Return         : None.
*******************************************************************************/
void Set_System(void)
{
  /*-------------------- System Clock Config --------------------*/
  FMI_BankRemapConfig(4, 2, 0, 0x80000); /* Set Flash banks size & address */
  FMI_Config(FMI_READ_WAIT_STATE_2, FMI_WRITE_WAIT_STATE_0, FMI_PWD_ENABLE, \
             FMI_LVD_ENABLE, FMI_FREQ_HIGH); /* FMI Waite States */

  /* Set the PCLK Clock to MCLK/2 */
  SCU_PCLKDivisorConfig(SCU_PCLK_Div2);
  /* USB clock = MCLK/2 = 48MHz */
  SCU_USBCLKConfig(SCU_USBCLK_MCLK2);

  SCU_PLLFactorsConfig(192, 25, 2); /* Configure Factors FPLL = 96MHz */
  SCU_PLLCmd(ENABLE);
  SCU_MCLKSourceConfig(SCU_MCLK_PLL);

  /* Enable USB clock */
  SCU_AHBPeriphClockConfig(__USB, ENABLE);
  SCU_AHBPeriphReset(__USB, DISABLE);

  SCU_AHBPeriphClockConfig(__USB48M, ENABLE);
  /*------------------ Peripheral Clock Enable ------------------*/

  SCU_APBPeriphClockConfig(__TIM01, ENABLE);
  SCU_APBPeriphReset(__TIM01, DISABLE);

  SCU_APBPeriphClockConfig(__TIM23, ENABLE);
  SCU_APBPeriphReset(__TIM23, DISABLE);

  /*Enable VIC clock*/
  SCU_AHBPeriphClockConfig(__VIC, ENABLE);
  SCU_AHBPeriphReset(__VIC, DISABLE);
  /*USB clock = MCLK= 48MHz*/
  SCU_USBCLKConfig(SCU_USBCLK_MCLK2);
  /*Enable USB clock*/
  SCU_AHBPeriphClockConfig(__USB, ENABLE);
  SCU_AHBPeriphReset(__USB, DISABLE);

  /*Configure GPIO0 (D+ Pull-Up on P0.1)*/

  SCU_APBPeriphClockConfig(__GPIO0 , ENABLE);
  SCU_APBPeriphReset(__GPIO0, DISABLE);

  GPIO_DeInit(GPIO0);
  GPIO_InitStructure.GPIO_Direction = GPIO_PinOutput;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Type = GPIO_Type_PushPull ;
  GPIO_InitStructure.GPIO_Alternate = GPIO_OutputAlt1;
  GPIO_Init (GPIO0, &GPIO_InitStructure);

  /* Enable WIU clock */
  SCU_APBPeriphClockConfig(__WIU, ENABLE);
  SCU_APBPeriphReset(__WIU, DISABLE);

  SCU_AHBPeriphClockConfig(__DMA, ENABLE); /* Enable the clock for DMA*/
  SCU_AHBPeriphReset(__DMA, DISABLE);
  DMA_Config();

  /* Enable and configure the priority of the USB_LP IRQ Channel*/
  VIC_DeInit();
  VIC_InitDefaultVectors(); /* Initialize default vector registers*/
  VIC_Config(TIM0_ITLine, VIC_IRQ, 3);
  VIC_ITCmd(TIM0_ITLine, ENABLE);
  VIC_Config(DMA_ITLine, VIC_IRQ, 1);
  VIC_ITCmd(DMA_ITLine, ENABLE);
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
* Function Name  : Enter_LowPowerMode
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
* Function Name  : Leave_LowPowerMode
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
* Function Name  : USB_Interrupts_Config
* Description    : configure and enable the USB interrupt Lines
* Input          : None.
* Return         : None.
*******************************************************************************/
void USB_Interrupts_Config(void)
{
  VIC_Config(USBHP_ITLine, VIC_IRQ, 0);
  VIC_ITCmd(USBHP_ITLine, ENABLE);
  VIC_Config(USBLP_ITLine, VIC_IRQ, 2);
  VIC_ITCmd(USBLP_ITLine, ENABLE);
}

/*******************************************************************************
* Function Name  : Speaker_Timer_Config
* Description    : configure and enable the timer
* Input          : None.
* Return         : None.
*******************************************************************************/
void Speaker_Timer_Config()
{
  TIM_InitTypeDef TIM_InitStructure;

  GPIO_DeInit(GPIO4);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Direction = GPIO_PinOutput;
  GPIO_InitStructure.GPIO_Type = GPIO_Type_PushPull;
  //GPIO_InitStructure.GPIO_IPConnected = GPIO_IPConnected_Disable;
  GPIO_InitStructure.GPIO_Alternate = GPIO_OutputAlt2;
  GPIO_Init(GPIO4, &GPIO_InitStructure);

  TIM_StructInit(&TIM_InitStructure);

  /*22.050 KHz generation*/
  TIM_InitStructure.TIM_Mode = TIM_OCM_CHANNEL_1;
  TIM_InitStructure.TIM_OC1_Modes = TIM_TIMING;
  TIM_InitStructure.TIM_Clock_Source = TIM_CLK_APB;
  TIM_InitStructure.TIM_Prescaler = 0x1;
  TIM_InitStructure.TIM_Pulse_Length_1 = TIMER_PULSE_LENGTH_22KHZ;

  /* Initialize the Timer 0 */
  TIM_Init (TIM0, &TIM_InitStructure);

  TIM_StructInit(&TIM_InitStructure);

  /* PWM configuration */
  TIM_InitStructure.TIM_Mode = TIM_PWM;
  TIM_InitStructure.TIM_Clock_Source = TIM_CLK_APB;
  TIM_InitStructure.TIM_Prescaler = 0x0;
  TIM_InitStructure.TIM_Pulse_Level_1 = TIM_HIGH;
  TIM_InitStructure.TIM_Period_Level = TIM_LOW;
  TIM_InitStructure.TIM_Pulse_Length_1 = 0x7F;
  TIM_InitStructure.TIM_Full_Period = 0xFF;

  /* Initialize the Timer 3 */
  TIM_Init (TIM3, &TIM_InitStructure);

  /* Enable the Timer Overflow interrupt */
  TIM_ITConfig(TIM0, TIM_IT_OC1, ENABLE);
  /* Start the Timer counter */
  TIM_CounterCmd(TIM3, TIM_START);
  /* Start the Timer counter */
  TIM_CounterCmd(TIM0, TIM_START);
}

/*******************************************************************************
* Function Name  : Switch_DMA_Src_Addr
* Description    : Switch the DMA source buffer between the ENDP1_BUF0Addr and
*                  the ENDP1_BUF1Addr
* Input          : None.
* Return         : None.
*******************************************************************************/
void Switch_DMA_Src_Addr(void)
{
  DMA_Cmd(ENABLE);/*Enable the DMA*/
  if (selector == 1)
  {
    DMA_InitStruct.DMA_Channel_SrcAdd = (u32)(PMAAddr + (u32)((GetEPDblBuf1Addr(ENDP1)))); /* source address Endpoint 1 Rx*/
  }
  else
  {
    DMA_InitStruct.DMA_Channel_SrcAdd = (u32)(PMAAddr + (u32)((GetEPDblBuf0Addr(ENDP1)))); /*source address Endpoint 1 Tx*/
  }
  /* Configure the DMA channel0 "the chosen channel to perform the transfer" */
  DMA_Init(DMA_Channel0, &DMA_InitStruct);/* update the DMA channel0 registers*/

  DMA_ChannelCmd (DMA_Channel0, ENABLE);/*Enable the DMA channel0*/
  DMALinkedModeRxEnable();
}

/*******************************************************************************
* Function Name  : DMA_Config
* Description    : Configure and enable the DMA
*                 table.
* Input          : None.
* Return         : None.
*******************************************************************************/
void DMA_Config(void)
{
  DMA_DeInit(); /* DMA default configuration : Reset configuration*/
  DMA_Cmd(ENABLE);/* Enable the DMA*/
  DMA_StructInit(&DMA_InitStruct);
  DMA_InitStruct.DMA_Channel_LLstItm = 0; /* only 1 descriptor is used*/
  /* source address Endpoint 1 Buffer 0*/
  DMA_InitStruct.DMA_Channel_SrcAdd = (u32)(PMAAddr + (u32)((GetEPDblBuf0Addr(ENDP1)))); 
  /* Destination address  */
  DMA_InitStruct.DMA_Channel_DesAdd = (u32)(&Stream_Buff); 
  /* The source bus width is a Word */
  DMA_InitStruct.DMA_Channel_SrcWidth = DMA_SrcWidth_Word;
  /* The Destination bus width is a word*/
  DMA_InitStruct.DMA_Channel_DesWidth = DMA_DesWidth_Word; 
  /* USB is The flow controller*/
  DMA_InitStruct.DMA_Channel_FlowCntrl = DMA_FlowCntrl_Perip2; 
  DMA_InitStruct.DMA_Channel_Src = DMA_SRC_USB_RX;
  DMA_InitStruct.DMA_Channel_TrsfSize = 0; /* Transfer size*/
  /* Configure the DMA channel0  */
  DMA_Init(DMA_Channel0, &DMA_InitStruct);

  DMALinkedModeRxEnable();
  DMASynchEnable();
  DMALinkedModeRxConfig(ENDP1);
  SetDMALLIRxPacketNum(1);
  SetDMALLIRxLength(1); /*only one descriptor*/
  DMA_ChannelCmd (DMA_Channel0, ENABLE);/*Enable the DMA channel0*/
  DMA_ChannelDESIncConfig (DMA_Channel0, ENABLE);
  DMA_ChannelSRCIncConfig (DMA_Channel0, ENABLE);
  DMA_ITMaskConfig(DMA_Channel0, DMA_ITMask_ITC, ENABLE);
  DMA_ITConfig(DMA_Channel0, ENABLE);
}

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
