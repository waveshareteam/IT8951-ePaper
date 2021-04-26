/**
  ******************************************************************************
  * @file    USART/USART_Printf/main.c 
  * @author  MCD Application Team
  * @version V1.3.0
  * @date    13-November-2013
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2013 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "usart.h"

/** @addtogroup STM32F4xx_StdPeriph_Examples
  * @{
  */

/** @addtogroup USART_Printf
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/


#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

typedef enum 
{
  COM1 = 0,
  COM2 = 1,
	COM3 = 2
} COM_TypeDef;

#define COMn                             3

#define EVAL_COM1                        USART1
#define EVAL_COM1_CLK                    RCC_APB2Periph_USART1
#define EVAL_COM1_TX_PIN                 GPIO_Pin_9
#define EVAL_COM1_TX_GPIO_PORT           GPIOA
#define EVAL_COM1_TX_GPIO_CLK            RCC_AHB1Periph_GPIOA
#define EVAL_COM1_TX_SOURCE              GPIO_PinSource9
#define EVAL_COM1_TX_AF                  GPIO_AF_USART1
#define EVAL_COM1_RX_PIN                 GPIO_Pin_10
#define EVAL_COM1_RX_GPIO_PORT           GPIOA
#define EVAL_COM1_RX_GPIO_CLK            RCC_AHB1Periph_GPIOA
#define EVAL_COM1_RX_SOURCE              GPIO_PinSource10
#define EVAL_COM1_RX_AF                  GPIO_AF_USART1
#define EVAL_COM1_IRQn                   USART1_IRQn

#define EVAL_COM2                        USART2
#define EVAL_COM2_CLK                    RCC_APB1Periph_USART2
#define EVAL_COM2_TX_PIN                 GPIO_Pin_5
#define EVAL_COM2_TX_GPIO_PORT           GPIOD
#define EVAL_COM2_TX_GPIO_CLK            RCC_AHB1Periph_GPIOD
#define EVAL_COM2_TX_SOURCE              GPIO_PinSource5
#define EVAL_COM2_TX_AF                  GPIO_AF_USART2
#define EVAL_COM2_RX_PIN                 GPIO_Pin_6
#define EVAL_COM2_RX_GPIO_PORT           GPIOD
#define EVAL_COM2_RX_GPIO_CLK            RCC_AHB1Periph_GPIOD
#define EVAL_COM2_RX_SOURCE              GPIO_PinSource6
#define EVAL_COM2_RX_AF                  GPIO_AF_USART2
#define EVAL_COM2_IRQn                   USART2_IRQn

#define EVAL_COM3                        USART3
#define EVAL_COM3_CLK                    RCC_APB1Periph_USART3
#define EVAL_COM3_TX_PIN                 GPIO_Pin_8
#define EVAL_COM3_TX_GPIO_PORT           GPIOD
#define EVAL_COM3_TX_GPIO_CLK            RCC_AHB1Periph_GPIOD
#define EVAL_COM3_TX_SOURCE              GPIO_PinSource8
#define EVAL_COM3_TX_AF                  GPIO_AF_USART3
#define EVAL_COM3_RX_PIN                 GPIO_Pin_9
#define EVAL_COM3_RX_GPIO_PORT           GPIOD
#define EVAL_COM3_RX_GPIO_CLK            RCC_AHB1Periph_GPIOD
#define EVAL_COM3_RX_SOURCE              GPIO_PinSource9
#define EVAL_COM3_RX_AF                  GPIO_AF_USART3
#define EVAL_COM3_IRQn                   USART3_IRQn


USART_TypeDef* COM_USART[COMn] = {EVAL_COM1,
                                  EVAL_COM2,
                                  EVAL_COM3}; 

GPIO_TypeDef* COM_TX_PORT[COMn] = {EVAL_COM1_TX_GPIO_PORT,
                                   EVAL_COM2_TX_GPIO_PORT,
                                   EVAL_COM3_TX_GPIO_PORT};
 
GPIO_TypeDef* COM_RX_PORT[COMn] = {EVAL_COM1_RX_GPIO_PORT,
                                   EVAL_COM2_RX_GPIO_PORT,
                                   EVAL_COM3_RX_GPIO_PORT};

const uint32_t COM_USART_CLK[COMn] = {EVAL_COM1_CLK,
                                      EVAL_COM2_CLK,
                                      EVAL_COM3_CLK};

const uint32_t COM_TX_PORT_CLK[COMn] = {EVAL_COM1_TX_GPIO_CLK,
                                        EVAL_COM2_TX_GPIO_CLK,
                                        EVAL_COM3_TX_GPIO_CLK};
 
const uint32_t COM_RX_PORT_CLK[COMn] = {EVAL_COM1_RX_GPIO_CLK,
                                        EVAL_COM2_RX_GPIO_CLK,
                                        EVAL_COM3_RX_GPIO_CLK};

const uint16_t COM_TX_PIN[COMn] = {EVAL_COM1_TX_PIN,
                                   EVAL_COM2_TX_PIN,
                                   EVAL_COM3_TX_PIN};

const uint16_t COM_RX_PIN[COMn] = {EVAL_COM1_RX_PIN,
                                   EVAL_COM2_RX_PIN,
                                   EVAL_COM3_RX_PIN};
 
const uint16_t COM_TX_PIN_SOURCE[COMn] = {EVAL_COM1_TX_SOURCE,
                                          EVAL_COM2_TX_SOURCE,
                                          EVAL_COM3_TX_SOURCE};

const uint16_t COM_RX_PIN_SOURCE[COMn] = {EVAL_COM1_RX_SOURCE,
                                          EVAL_COM2_RX_SOURCE,
                                          EVAL_COM3_RX_SOURCE};
 
const uint16_t COM_TX_AF[COMn] = {EVAL_COM1_TX_AF,
                                  EVAL_COM2_TX_AF,
                                  EVAL_COM3_TX_AF};
 
const uint16_t COM_RX_AF[COMn] = {EVAL_COM1_RX_AF,
                                  EVAL_COM2_RX_AF,
                                  EVAL_COM3_RX_AF};


void STM_EVAL_COMInit(COM_TypeDef COM, USART_InitTypeDef* USART_InitStruct)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /* Enable GPIO clock */
  RCC_AHB1PeriphClockCmd(COM_TX_PORT_CLK[COM] | COM_RX_PORT_CLK[COM], ENABLE);

  if (COM == COM1)
  {
    /* Enable UART clock */
    RCC_APB2PeriphClockCmd(COM_USART_CLK[COM], ENABLE);
  }
	else
  RCC_APB1PeriphClockCmd(COM_USART_CLK[COM], ENABLE);
  /* Connect PXx to USARTx_Tx*/
  GPIO_PinAFConfig(COM_TX_PORT[COM], COM_TX_PIN_SOURCE[COM], COM_TX_AF[COM]);

  /* Connect PXx to USARTx_Rx*/
  GPIO_PinAFConfig(COM_RX_PORT[COM], COM_RX_PIN_SOURCE[COM], COM_RX_AF[COM]);

  /* Configure USART Tx as alternate function  */
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;

  GPIO_InitStructure.GPIO_Pin = COM_TX_PIN[COM];
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(COM_TX_PORT[COM], &GPIO_InitStructure);

  /* Configure USART Rx as alternate function  */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Pin = COM_RX_PIN[COM];
  GPIO_Init(COM_RX_PORT[COM], &GPIO_InitStructure);

  /* USART configuration */
  USART_Init(COM_USART[COM], USART_InitStruct);
    
  /* Enable USART */
  USART_Cmd(COM_USART[COM], ENABLE);
}

void USART_Config(void)
{
  USART_InitTypeDef USART_InitStructure;
  
  /* USARTx configured as follows:
        - BaudRate = 115200 baud  
        - Word Length = 8 Bits
        - One Stop Bit
        - No parity
        - Hardware flow control disabled (RTS and CTS signals)
        - Receive and transmit enabled
  */
  USART_InitStructure.USART_BaudRate = 115200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

  STM_EVAL_COMInit(COM1, &USART_InitStructure);
}

/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART */
  USART_SendData(EVAL_COM1, (uint8_t) ch);

  /* Loop until the end of transmission */
  while (USART_GetFlagStatus(EVAL_COM1, USART_FLAG_TC) == RESET)
  {}

  return ch;
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */ 

/**
  * @}
  */ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
