/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : main.c
* Author             : MCD Application Team
* Version            : V2.1
* Date               : 12/22/2008
* Description        : main program for wdg example1.
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

/* Include of other module interface headers ---------------------------------*/
/* Local includes ------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
WDG_InitTypeDef WDG_InitStructure;
GPIO_InitTypeDef  GPIO9_InitStruct;
/* Private function prototypes -----------------------------------------------*/
void SCU_Configuration(void);
/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
* Function Name  : main
* Description    : Main program
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
int main(void)
{

#ifdef DEBUG
    debug();
#endif

  /* Configure the system clocks */
  SCU_Configuration();
   
   /* Configure the GPIO9 pin P9.1 */
  GPIO_DeInit(GPIO9);
  GPIO9_InitStruct.GPIO_Pin= GPIO_Pin_1;  
  GPIO9_InitStruct.GPIO_Direction=GPIO_PinOutput; 
  GPIO_Init(GPIO9,&GPIO9_InitStruct); 

  /* Configure the WDG to generate an EOC interrupt each  0.35s */
  WDG_InitStructure.WDG_Preload = 0xFFFF;
  WDG_InitStructure.WDG_Prescaler = 0xFF;
  WDG_InitStructure.WDG_ClockSource=WDG_ClockSource_Apb;
  WDG_Init(&WDG_InitStructure);  

  /* Configure the VIC */
  VIC_DeInit(); /* Initialize the VIC*/
  VIC_InitDefaultVectors(); /* Initialize default vector registers*/

  /* Configure the VIC interrupt */
  VIC_Config(WDG_ITLine,VIC_IRQ,0);
  VIC_ITCmd(WDG_ITLine, ENABLE);  /* Enable the VIC */
  WDG_ITConfig(ENABLE); /* Enable the WDG interrupt */

  /* Enable the WDG in Timer mode*/
  WDG_TimerModeCmd(ENABLE);


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
  
  SCU_APBPeriphClockConfig(__GPIO9, ENABLE); /* Enable the clock for GPIO9*/
  SCU_APBPeriphClockConfig(__WDG, ENABLE);  /* Enable the clock for WDG*/
  SCU_AHBPeriphClockConfig(__VIC,ENABLE);   /* Enable the clock to VIC*/
}  
/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/







