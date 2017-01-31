/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : main.c
* Author             : MCD Application Team
* Version            : V2.1
* Date               : 12/22/2008
* Description        : Main program body
********************************************************************************
* THE PRESENT SOFTWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS WITH
* CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME. AS
* A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT, INDIRECT
* OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE CONTENT
* OF SUCH SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING INFORMATION
* CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/


/* Includes ------------------------------------------------------------------*/
#include "91x_lib.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
  GPIO_InitTypeDef GPIO_InitStructure;

/* Private function prototypes -----------------------------------------------*/
  void SCU_Configuration(void);
  static void Delay(u32 nCount);

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

/* GPIO Configuration --------------------------------------------------------*/
  GPIO_DeInit(GPIO9);
  GPIO_InitStructure.GPIO_Direction = GPIO_PinOutput;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Type = GPIO_Type_PushPull ;
  GPIO_Init (GPIO9, &GPIO_InitStructure);

  while(1)
  {
    /* Turn OFF leds connected to P9.0, P9.1, P9.2 and P9.3 pins */
    GPIO_WriteBit(GPIO9, GPIO_Pin_0, Bit_SET);
    GPIO_WriteBit(GPIO9, GPIO_Pin_1, Bit_SET);
    GPIO_WriteBit(GPIO9, GPIO_Pin_2, Bit_SET);
    GPIO_WriteBit(GPIO9, GPIO_Pin_3, Bit_SET);

    /* Insert delay */
    Delay(0x7FFFF);

    /* Turn ON leds connected to P9.0, P9.1, P9.2 and P9.3 pins */
    GPIO_WriteBit(GPIO9, GPIO_Pin_0, Bit_RESET);
    GPIO_WriteBit(GPIO9, GPIO_Pin_1, Bit_RESET);
    GPIO_WriteBit(GPIO9, GPIO_Pin_2, Bit_RESET);
    GPIO_WriteBit(GPIO9, GPIO_Pin_3, Bit_RESET);

    /* Insert delay */
    Delay(0x7FFFF);
  }
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
  /* Enable the __GPIO9 */
  SCU_APBPeriphClockConfig(__GPIO9 ,ENABLE);
}

/*******************************************************************************
* Function Name  : Delay
* Description    : Inserts a delay time.
* Input          : nCount: specifies the delay time length.
* Output         : None
* Return         : None
*******************************************************************************/
static void Delay(u32 nCount)
{
  vu32 j = 0;

  for(j = nCount; j != 0; j--);
}
/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/

