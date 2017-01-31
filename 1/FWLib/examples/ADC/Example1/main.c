/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : main.c
* Author             : MCD Application Team
* Version            : V2.1
* Date               : 12/22/2008
* Description        : Program using the ADC peripheral.The purpose of this 
*                      program is to convert an analog input using the ADC. 
*                      The c library "printf" function is retargeted
*                      to the UART , this implementation output the converted 
*                      result on the Hyperterminal using UART0
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
#include "stdio.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (should be set) calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

ADC_InitTypeDef   ADC_InitStructure;
GPIO_InitTypeDef  GPIO_InitStructure;
UART_InitTypeDef UART_InitStructure;
extern vu16 Conversion_Value;
float Conversion_Result=0;

/* Private function prototypes -----------------------------------------------*/
void SCU_Configuration(void);
void GPIO_Configuration(void);
static void Delay(u32 nCount);
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

  /* Configure the GPIO ports */
  GPIO_Configuration();

  VIC_DeInit();              /* VIC Deinitialization */
  VIC_InitDefaultVectors(); /* initialize VICs default vector registers*/
  
  /* ADC Structure Initialization */
  ADC_StructInit(&ADC_InitStructure);

  /* Configure the ADC  structure in continuous mode conversion */
  ADC_DeInit();             /* ADC Deinitialization */
  ADC_InitStructure.ADC_Channel_6_Mode = ADC_NoThreshold_Conversion;
  ADC_InitStructure.ADC_Select_Channel = ADC_Channel_6;
  ADC_InitStructure.ADC_Scan_Mode = DISABLE;
  ADC_InitStructure.ADC_Conversion_Mode = ADC_Single_Mode;


  /* UART0 configuration -----------------------------------------------------*/
  /* UART0 configured as follow:
        - Word Length = 7 Bits
        - Two Stop Bit
        - No parity
        - BaudRate = 115200 baud
        - Hardware flow control enabled (RTS and CTS signals)
        - Receive and transmit enabled
        - Receive and transmit FIFOs are enabled
        - Transmit and Receive FIFOs levels have 8 bytes depth*/
  
  UART_InitStructure.UART_WordLength = UART_WordLength_7D;
  UART_InitStructure.UART_StopBits = UART_StopBits_2;
  UART_InitStructure.UART_Parity = UART_Parity_No ;
  UART_InitStructure.UART_BaudRate = 115200;
  UART_InitStructure.UART_HardwareFlowControl = UART_HardwareFlowControl_RTS_CTS;
  UART_InitStructure.UART_Mode = UART_Mode_Tx_Rx;
  UART_InitStructure.UART_FIFO = UART_FIFO_Enable;
  UART_InitStructure.UART_TxFIFOLevel = UART_FIFOLevel_1_2; /* FIFO size 16 bytes, FIFO level 8 bytes */
  UART_InitStructure.UART_RxFIFOLevel = UART_FIFOLevel_1_2; /* FIFO size 16 bytes, FIFO level 8 bytes */


  UART_DeInit(UART0);
  UART_Init(UART0, &UART_InitStructure);

  /* Enable the UART0 */
  UART_Cmd(UART0, ENABLE);


  /* Communication  hyperterminal-UART0 using the hardware flow control */
 

  printf("\r\n======================================================================");
  printf("\r\n=              (C) Copyright 2008 STMicroelectronics                 =");
  printf("\r\n=                                                                    =");
  printf("\r\n=                    ADC Example1  (Version 2.1)                     =");
  printf("\r\n=                                                                    =");
  printf("\r\n=                      By MCD Application Team                       =");
  printf("\r\n======================================================================");
  printf("\r\n\r\n");

  /* Enable the ADC */
  ADC_Cmd(ENABLE);

  /* Prescaler config */
  ADC_PrescalerConfig(0x2);

  /* Configure the ADC */
  ADC_Init(&ADC_InitStructure);

  /* VIC interrupt configuration */
  VIC_Config(ADC_ITLine, VIC_IRQ, 0);
  VIC_ITCmd(ADC_ITLine, ENABLE);

  /* ADC interrupt config */
  ADC_ITConfig(ADC_IT_ECV, ENABLE);
  /* Start the conversion */
  ADC_ConversionCmd(ADC_Conversion_Start);


  while (1){  
   
    /* Start the single conversion */
    ADC_ConversionCmd(ADC_Conversion_Start);
    
    /* Insert delay */
    Delay(0x7FFFF);
    
    /*Get the conversion value in volts*/
    Conversion_Result = (3.3*Conversion_Value)/1023 ;
    
    /*Output the converted result on the Hyperterminal using UART0*/
    printf("\r The converted value in <Volt> is : %f\n\r ",Conversion_Result);
    
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
  
  /* Enable the clock for the ADC */
  SCU_APBPeriphClockConfig(__ADC, ENABLE);    
 
  /* Enable the clock for the VIC */  
  SCU_AHBPeriphClockConfig(__VIC, ENABLE);    
      
  /* Enable the UART0 Clock */
  SCU_APBPeriphClockConfig(__UART0, ENABLE);

  /* Enable the GPIO2 Clock */
  SCU_APBPeriphClockConfig(__GPIO2, ENABLE);

  /* Enable the GPIO3 Clock */
  SCU_APBPeriphClockConfig(__GPIO3, ENABLE);

  /* Enable the GPIO4 Clock */
  SCU_APBPeriphClockConfig(__GPIO4, ENABLE);

  /* Enable the GPIO5 Clock */
  SCU_APBPeriphClockConfig(__GPIO5, ENABLE);

  /* Enable the GPIO6 Clock */
  SCU_APBPeriphClockConfig(__GPIO6, ENABLE);

}

/*******************************************************************************
* Function Name  : GPIO_Configuration
* Description    : Configures the different GPIO ports.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /*Configure UART0_CTS P2.0*/
  GPIO_DeInit(GPIO2);
  /*After DeInit function P2.0 = UART0_CTS (defaut configuration)*/

  GPIO_DeInit(GPIO5);
  /*Configure UART0_Rx pin GPIO5.1*/
  GPIO_InitStructure.GPIO_Direction = GPIO_PinInput;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
  GPIO_InitStructure.GPIO_IPInputConnected = GPIO_IPInputConnected_Enable;
  GPIO_InitStructure.GPIO_Alternate = GPIO_InputAlt1;
  GPIO_Init (GPIO5, &GPIO_InitStructure);

  GPIO_DeInit(GPIO3);
  /*Configure UART0_Tx pin GPIO3.4*/
  GPIO_InitStructure.GPIO_Direction = GPIO_PinOutput;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Alternate = GPIO_OutputAlt3;
  GPIO_Init (GPIO3, &GPIO_InitStructure);

  /* Configure the GPIO4 pin 6 as analog input */
  GPIO_DeInit(GPIO4);
  GPIO_ANAPinConfig(GPIO_ANAChannel6, ENABLE);

  /*Configure UART0_RTS pin GPIO3.3*/
  GPIO_InitStructure.GPIO_Direction = GPIO_PinOutput;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Type = GPIO_Type_PushPull;
  GPIO_InitStructure.GPIO_Alternate = GPIO_OutputAlt3;
  GPIO_Init (GPIO3, &GPIO_InitStructure);


}
/*******************************************************************************
* Function Name  : PUTCHAR_PROTOTYPE
* Description    : Retargets the C library printf function to the USART.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the UART */

  UART_SendData(UART0, (u8) ch);
  while (UART_GetFlagStatus(UART0, UART_FLAG_TxFIFOFull) != RESET);

  return ch;
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
  u32 j = 0;

  for(j = nCount; j != 0; j--);
}
/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
