/******************** (C) COPYRIGHT 2007 STMicroelectronics ********************
* File Name          : main.c
* Author             : MCD Application Team
* Version            : V4.0
* Date               : 10/09/2007
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
#include "71x_lib.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
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

  /* System clocks configuration ---------------------------------------------*/
  /* MCLK = PCLK1 = PCLK2 = 24MHz*/ 
  
  /* Configure PCLK1 = RCLK / 1 */
  RCCU_PCLK1Config (RCCU_DEFAULT);

  /* Configure PCLK2 = RCLK / 1 */
  RCCU_PCLK2Config (RCCU_DEFAULT);

  /* Configure MCLK clock for the CPU, RCCU_DEFAULT = RCLK /1 */
  RCCU_MCLKConfig (RCCU_DEFAULT);
   
  /* Configure the PLL1 ( * 12 , / 4 ) */
  RCCU_PLL1Config (RCCU_PLL1_Mul_12, RCCU_Div_4) ;

  while(RCCU_FlagStatus(RCCU_PLL1_LOCK) == RESET)
  {
    /* Wait PLL to lock */
  }
  /* Select PLL1_Output as RCLK clock */
  RCCU_RCLKSourceConfig (RCCU_PLL1_Output) ;

  /* Enable XTI and GPIO0 clocks on APB2 */
  APB_ClockConfig (APB2, ENABLE, GPIO0_Periph | XTI_Periph );
  
  /* XTI configuration -------------------------------------------------------*/

  /* Initialize the XTI */
  XTI_Init();

  /* Set Line 15 edge */
  XTI_LineModeConfig(XTI_Line15, XTI_FallingEdge);

  /* Enable the External interrupts on line 15 */
  XTI_LineConfig(XTI_Line15, ENABLE);

  /* Set the XTI mode */
  XTI_ModeConfig(XTI_Interrupt, ENABLE);

  /* Configure the XTI IRQ channel */

  /*  Set the XTI IRQ Channel priority to 1*/
  EIC_IRQChannelPriorityConfig(XTI_IRQChannel, 1);

  /* Enable XTI IRQ Interrupts */
  EIC_IRQChannelConfig(XTI_IRQChannel, ENABLE);

  /* Enable IRQ interrupts on EIC */
  EIC_IRQConfig(ENABLE);

  /* GPIO configuration ------------------------------------------------------*/

  /* Configure the P0.15 in IPUPD WP mode */
  GPIO_Config(GPIO0, 0x0001 << 0x0F, GPIO_IPUPD_WP);

  /* Configure the P0.0 in Push-Pull Output mode */
  GPIO_Config(GPIO0, 0x0001, GPIO_OUT_PP);

  while(1)
  {
  /*  infinite loop */
  }
}

/******************* (C) COPYRIGHT 2007 STMicroelectronics *****END OF FILE****/
