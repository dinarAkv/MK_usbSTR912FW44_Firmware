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
#define TxBufferSize   (countof(TxBuffer) - 1)


/* Private macro -------------------------------------------------------------*/
#define countof(a)   (sizeof(a) / sizeof(*(a)))

/* Private variables ---------------------------------------------------------*/
  UART_InitTypeDef UART_InitStructure;
  u8 TxBuffer[] = "UART Example1: UART - Hyperterminal communication using hardware flow control\n\r";
  u8 NbrOfDataToTransfer = TxBufferSize;
  u8 TxCounter = 0;
  
/* Private function prototypes -----------------------------------------------*/
void SCU_Configuration(void);
void GPIO_Configuration(void);

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

  /* UART0 configuration -----------------------------------------------------*/
  /* UART0 configured as follow:
        - Word Length = 7 Bits
        - Two Stop Bit
        - No parity
        - BaudRate = 115200 baud
        - Hardware flow control enabled (RTS and CTS signals)
        - Receive and transmit enabled
        - Receive and transmit FIFOs are enabled
        - Transmit and Receive FIFOs levels have 8 bytes depth
  */
  UART_InitStructure.UART_WordLength = UART_WordLength_7D;
  UART_InitStructure.UART_StopBits = UART_StopBits_2;
  UART_InitStructure.UART_Parity = UART_Parity_No ;
  UART_InitStructure.UART_BaudRate = 115200;
  UART_InitStructure.UART_HardwareFlowControl = UART_HardwareFlowControl_RTS_CTS;
  UART_InitStructure.UART_Mode = UART_Mode_Tx_Rx;
  UART_InitStructure.UART_FIFO = UART_FIFO_Disable;
  

  UART_DeInit(UART0);
  UART_Init(UART0, &UART_InitStructure);

  /*enable UART Rx Interrupt*/
  UART_ITConfig(UART0, UART_IT_Receive	, ENABLE);
  
  /*configure UART0 interrupt in VIC*/
  VIC_DeInit();
  VIC_InitDefaultVectors(); /* initialize VICs default vector registers*/
  VIC_Config(UART0_ITLine, VIC_IRQ, 0);
 
  /* Enable the UART0 */
  UART_Cmd(UART0, ENABLE);
 

  /* Communication  hyperterminal-UART0 using the hardware flow control */
  /* Send a buffer from UART to hyperterminal */
  while(NbrOfDataToTransfer--)
  {
    UART_SendData(UART0, TxBuffer[TxCounter++]);
    while(UART_GetFlagStatus(UART0, UART_FLAG_TxFIFOFull) != RESET);

  }
  
  /*enable UART0 interrupt for data reception*/
  VIC_ITCmd(UART0_ITLine,ENABLE);
  
  while (1);
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

  /* Enable UART0 Clock */
  SCU_APBPeriphClockConfig(__UART0, ENABLE);
  
  /* Enable VIC clock */
  SCU_AHBPeriphClockConfig(__VIC, ENABLE);

  /* Enable GPIO2 Clock */
  SCU_APBPeriphClockConfig(__GPIO2, ENABLE);

  /* Enable GPIO3 Clock */
  SCU_APBPeriphClockConfig(__GPIO3, ENABLE);

  /* Enable GPIO5 Clock */
  SCU_APBPeriphClockConfig(__GPIO5, ENABLE);

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
  /*Gonfigure UART0_Rx pin GPIO5.1*/
  GPIO_InitStructure.GPIO_Direction = GPIO_PinInput;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
  GPIO_InitStructure.GPIO_IPInputConnected = GPIO_IPInputConnected_Enable;
  GPIO_InitStructure.GPIO_Alternate = GPIO_InputAlt1  ;
  GPIO_Init (GPIO5, &GPIO_InitStructure);

   GPIO_DeInit(GPIO3);
  /*Gonfigure UART0_Tx pin GPIO3.4*/
  GPIO_InitStructure.GPIO_Direction = GPIO_PinOutput;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Type = GPIO_Type_PushPull ;
  GPIO_InitStructure.GPIO_Alternate = GPIO_OutputAlt3  ;
  GPIO_Init (GPIO3, &GPIO_InitStructure);

  /*Gonfigure UART0_RTS pin GPIO3.3*/
  GPIO_InitStructure.GPIO_Direction = GPIO_PinOutput;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Type = GPIO_Type_PushPull ;
  GPIO_InitStructure.GPIO_Alternate = GPIO_OutputAlt3  ;
  GPIO_Init (GPIO3, &GPIO_InitStructure);
}
/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/

