/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : main.c
* Author             : MCD application team
* Version            : V2.1
* Date               : 12/22/2008
* Description        : RTC Periodic Interrupt Example
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
 GPIO_InitTypeDef     GPIO_Struct;
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

int main (void)
 {
  #ifdef DEBUG
  debug();
  #endif
  
  /* Configure the system clocks */
   SCU_Configuration();
  
  VIC_DeInit();              /* VIC Deinitialization */
  /* initialize VICs default vector registers*/
  VIC_InitDefaultVectors();             

 /*Config GPIO pin P9.2 for software togling*/
  
  GPIO_Struct.GPIO_Pin= GPIO_Pin_2;
  GPIO_Struct.GPIO_Direction=GPIO_PinOutput;
  GPIO_Struct.GPIO_Type=GPIO_Type_PushPull;
  GPIO_Init(GPIO9,&GPIO_Struct);

  /*Configure Periodic interrupt clock = 1024Hz*/
  RTC_PeriodicIntConfig(RTC_Per_1024Hz);
  
  /*Enable RTC Periodic interrupt*/
  RTC_ITConfig(RTC_IT_Per,ENABLE);
  
  /*Configure and enable RTC interrupt in VIC*/
  VIC_Config(RTC_ITLine, VIC_IRQ, 0);
  VIC_ITCmd(RTC_ITLine, ENABLE);
  
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
  
  /*Enable GPIO9 clock*/
  SCU_APBPeriphClockConfig(__GPIO9,ENABLE);
  SCU_APBPeriphReset(__GPIO9,DISABLE);
  
   /*Enable RTC and VIC clocks*/
  SCU_APBPeriphClockConfig(__RTC,ENABLE); /*enable RTC clock*/
  SCU_APBPeriphReset(__RTC,DISABLE);      /*RTC out of RESET state*/
  
  SCU_AHBPeriphClockConfig(__VIC,ENABLE); /*enable VIC*/
  SCU_AHBPeriphReset(__VIC,DISABLE);      /*VIC out of RESET state*/ 
      
}  
/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
