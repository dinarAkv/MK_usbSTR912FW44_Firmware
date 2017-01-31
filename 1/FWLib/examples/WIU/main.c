/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : main.c
* Author             : MCD Application Team
* Version            : V2.1
* Date               : 12/22/2008
* Description        : Main program body
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

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
  WIU_InitTypeDef WIU_InitStructure;
/* Private function prototypes -----------------------------------------------*/
void SCU_Configuration(void);
void GPIO_Configuration(void);
void VIC_Configuration(void);


/* Private functions ---------------------------------------------------------*/
/*******************************************************************************
* Function Name  : main
* Description    : Main program
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
int main()
{
  #ifdef DEBUG
    debug();      
  #endif
  
  /* Configure the system clocks */
  SCU_Configuration();

  /* Configure the GPIO ports */
  GPIO_Configuration();

  /* Enable the WIU & Clear the WIU line 28 pending bit */
  WIU_Cmd(ENABLE );
  WIU_ClearITPendingBit(WIU_Line28);

  /* Configure external interrupt line */
  WIU_DeInit();
  WIU_StructInit(&WIU_InitStructure);
  WIU_InitStructure.WIU_Line = WIU_Line28 ;
  WIU_InitStructure.WIU_TriggerEdge = WIU_FallingEdge ;
  WIU_Init(&WIU_InitStructure);

  /* Select WIU line 28 as EXTIT3_ITLine interrupt source */
  SCU_WakeUpLineConfig(28);

  /* Configure and enable the interrupt controller */
  VIC_Configuration();

  while(1);
}

/*******************************************************************************
* Function Name  : SCU_Configuration
* Description    : Configures the Master clock @96MHz and the Main system 
*                  configuration and Clocks (FMI,PLL,RCLK,HCLK,PCLK ,MCLK )
*                  and enable clocks for peripherals.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SCU_Configuration(void)
{
  SCU_MCLKSourceConfig(SCU_MCLK_OSC);	/* Default configuration */
  
  /*wait state insertion :This function should be executed from SRAM when*/
  /*booting from bank1 to avoid  Read-While-Write from the Same Bank.*/
  FMI_Config(FMI_READ_WAIT_STATE_2, FMI_WRITE_WAIT_STATE_0, FMI_PWD_ENABLE,\
                 FMI_LVD_ENABLE, FMI_FREQ_HIGH);/*Insert 2 Wait States for read*/
                                                
  SCU_PLLFactorsConfig(192, 25, 2); /* PLL factors Configuration based on*/
                                        /* a OSC/Crystal value = 25Mhz*/     
  SCU_PLLCmd(ENABLE);  /* PLL Enable and wait for Locking*/     
  SCU_RCLKDivisorConfig(SCU_RCLK_Div1); /* RCLK @96Mhz */
  SCU_HCLKDivisorConfig(SCU_HCLK_Div1); /* AHB @96Mhz */
  SCU_FMICLKDivisorConfig(SCU_FMICLK_Div1);/* FMI @96Mhz */
  SCU_PCLKDivisorConfig(SCU_PCLK_Div2); /* APB @48Mhz */
  SCU_MCLKSourceConfig(SCU_MCLK_PLL);  /* MCLK @96Mhz */
  
  
  /* Enable VIC clock */
  SCU_AHBPeriphClockConfig(__VIC,ENABLE);
  
  /* Enable WIU clock */
  SCU_APBPeriphClockConfig(__WIU, ENABLE);
  
  /* GPIO 7 clock source enable */
  SCU_APBPeriphClockConfig(__GPIO7, ENABLE);

  /* GPIO 9 clock source enable */
  SCU_APBPeriphClockConfig(__GPIO9, ENABLE);
  

}

/*******************************************************************************
* Function Name  : GPIO_Configuration
* Description    : This function configures the different GPIO ports.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /* GPIO pin P7.4 configuration (on the MB460 board, this pin goes low when 
  pressing joystick or PB3 button)*/

  GPIO_DeInit(GPIO7);
  GPIO_StructInit(&GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Direction = GPIO_PinInput;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
  GPIO_Init (GPIO7, &GPIO_InitStructure);

  /* GPIO pin P9.0 (connected to LD2 on MB460 board) configuration */
  GPIO_DeInit(GPIO9);
  GPIO_StructInit(&GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Direction = GPIO_PinOutput;
  GPIO_Init(GPIO9, &GPIO_InitStructure);
}

/*******************************************************************************
* Function Name  : VIC_Configuration
* Description    : This function configures and enables the interrupt controller.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void VIC_Configuration(void)
{
  VIC_DeInit();
  
  /* Initialize VICs Default vector address registers */
  VIC_InitDefaultVectors();
  
  /* Configure the External interrupt group 3 priority as FIQ interrupt */
  VIC_Config(EXTIT3_ITLine, VIC_FIQ, 0);
  /* Enable the External interrupt group 3 */
  VIC_ITCmd(EXTIT3_ITLine, ENABLE);
}
/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
