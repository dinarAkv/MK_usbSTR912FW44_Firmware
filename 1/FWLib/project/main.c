/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : main.c
* Author             : MCD Application Team
* Version            : V2.1
* Date               : 12/22/2008
* Description        : Main program body:Configures CPU@96MHz and Toggle GPIO9.0
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

/*MCLK_Clock*/

#define PLL_48      0x0
#define PLL_66      0x1
#define PLL_96      0x2
#define OSC_Clock   0x3
#define RTC_Clock   0x4

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
GPIO_InitTypeDef GPIO_InitStructure;

/* Private function prototypes -----------------------------------------------*/
void MCLK_Config (u8 MCLK_Clock);
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

  /*Configure PLL as clock source @96MHZ*/
  MCLK_Config (PLL_96);

  /* Enable GPIO9 clock */
  SCU_APBPeriphClockConfig(__GPIO9 , ENABLE);
  
  /* GPIO Configuration */
  GPIO_DeInit(GPIO9);
  GPIO_InitStructure.GPIO_Direction = GPIO_PinOutput;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Type = GPIO_Type_PushPull ;
  GPIO_Init (GPIO9, &GPIO_InitStructure);

  while (1)
  {
    /* Turn OFF led connected to P9.0 pin */
    GPIO_WriteBit(GPIO9, GPIO_Pin_0, Bit_SET);

    /* Insert delay */
    Delay(0x7FFFF);

    /* Turn ON led connected to P9.0 pin */
    GPIO_WriteBit(GPIO9, GPIO_Pin_0, Bit_RESET);

    /* Insert delay */
    Delay(0x7FFFF);
  }


}

/*******************************************************************************
* Function Name  : MCLK_Config
* Description    : Configures the Master clock and the Main system configuration
*                  and Clocks (FMI, PLL, RCLK, HCLK, PCLK and MCLK )
* Input          : MCLK_Clock can be :PLL_48,PLL_66,PLL_96,OSC_Clock,RTC_Clock.
* Output         : None
* Return         : None
*******************************************************************************/
void MCLK_Config (u8 MCLK_Clock)
{
	 	
  SCU_MCLKSourceConfig(SCU_MCLK_OSC);	/* Default configuration */
  
  switch (MCLK_Clock)
  {
    case PLL_48:  /*Select PLL as clock source @48MHZ*/  
          
      /* This function should be executed from SRAM when*/
      /* booting from bank1 to avoid  Read-While-Write from the Same Bank.*/
      
      FMI_Config(FMI_READ_WAIT_STATE_1, FMI_WRITE_WAIT_STATE_0, FMI_PWD_ENABLE,\
                 FMI_LVD_ENABLE, FMI_FREQ_LOW);/*Insert 1 Wait State for read*/
                                                
      SCU_PLLFactorsConfig(192, 25, 3); /* PLL factors Configuration based on*/ 
                                        /* a OSC/Crystal value = 25Mhz*/    
      SCU_PLLCmd(ENABLE);  /* PLL Enable and wait for Locking*/     
      SCU_RCLKDivisorConfig(SCU_RCLK_Div1); /* RCLK @48Mhz */
      SCU_HCLKDivisorConfig(SCU_HCLK_Div1); /* AHB @48Mhz */
      SCU_FMICLKDivisorConfig(SCU_FMICLK_Div1);/* FMI @48Mhz */
      SCU_PCLKDivisorConfig(SCU_PCLK_Div1); /* APB @48Mhz */
      SCU_MCLKSourceConfig(SCU_MCLK_PLL);  /* MCLK @48Mhz */   
      break;
    case PLL_66:    /*Select PLL as clock source @66MHZ*/
      
      /*wait state insertion :This function should be executed from SRAM when*/
      /*booting from bank1 to avoid  Read-While-Write from the Same Bank.*/
      FMI_Config(FMI_READ_WAIT_STATE_1, FMI_WRITE_WAIT_STATE_0, FMI_PWD_ENABLE,\
                 FMI_LVD_ENABLE, FMI_FREQ_HIGH);/*Insert 1 Wait State for read*/
                          
      SCU_PLLFactorsConfig(132, 25, 2); /* PLL factors Configuration based on*/ 
                                        /* a OSC/Crystal value = 25Mhz*/     
      SCU_PLLCmd(ENABLE);  /* PLL Enable and wait for Locking*/     
      SCU_RCLKDivisorConfig(SCU_RCLK_Div1); /* RCLK @66Mhz */
      SCU_HCLKDivisorConfig(SCU_HCLK_Div1); /* AHB @66Mhz */
      SCU_FMICLKDivisorConfig(SCU_FMICLK_Div1);/* FMI @66Mhz */
      SCU_PCLKDivisorConfig(SCU_PCLK_Div2); /* APB @33Mhz */
      SCU_MCLKSourceConfig(SCU_MCLK_PLL);  /* MCLK @66Mhz */
      
      break;

    case PLL_96:   /*Select PLL as clock source @96MHZ*/

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
      
      break;

    case OSC_Clock:    /*Select OSC/Crystal as clock source , See 91x_conf.h */
    
      /* This function should be executed from SRAM when*/
      /* booting from bank1 to avoid  Read-While-Write from the Same Bank.*/    
      FMI_Config(FMI_READ_WAIT_STATE_1, FMI_WRITE_WAIT_STATE_0, FMI_PWD_ENABLE,\
                 FMI_LVD_ENABLE, FMI_FREQ_LOW);/*Insert 1 Wait State for read*/
                                                
      SCU_RCLKDivisorConfig(SCU_RCLK_Div1); /* RCLK @Osc/crystal Frequency*/
      SCU_HCLKDivisorConfig(SCU_HCLK_Div1); /* AHB @Osc/crystal Frequency*/
      SCU_FMICLKDivisorConfig(SCU_FMICLK_Div1);/* FMI @Osc/crystal Frequency*/
      SCU_PCLKDivisorConfig(SCU_PCLK_Div1); /* APB @Osc/crystal Frequency*/
      SCU_MCLKSourceConfig(SCU_MCLK_OSC);  /* MCLK @Osc/crystal Frequency*/          
      break;

    case RTC_Clock:   /*Select RTC as clock source */
      
      /* This function should be executed from SRAM when*/
      /* booting from bank1 to avoid  Read-While-Write from the Same Bank.*/    
      FMI_Config(FMI_READ_WAIT_STATE_1, FMI_WRITE_WAIT_STATE_0, FMI_PWD_ENABLE,\
                 FMI_LVD_ENABLE, FMI_FREQ_LOW);/*Insert 1 Wait State for read*/
                                                
      SCU_RCLKDivisorConfig(SCU_RCLK_Div1); /* RCLK @ RTC Frequency*/
      SCU_HCLKDivisorConfig(SCU_HCLK_Div1); /* AHB @ RTC  Frequency*/
      SCU_FMICLKDivisorConfig(SCU_FMICLK_Div1);/* FMI @ RTC  Frequency*/
      SCU_PCLKDivisorConfig(SCU_PCLK_Div1); /* APB @ RTC  Frequency*/
      SCU_MCLKSourceConfig(SCU_MCLK_RTC);  /* MCLK @ RTC  Frequency*/
     
      break;

    default:
      break;

  }
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

  for (j = nCount; j != 0; j--);
}
/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
