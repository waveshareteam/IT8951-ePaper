/**
  ******************************************************************************
  * @file    STM324x7i_eval_fmc_sram.c
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    19-September-2013
  * @brief   This file provides a set of functions needed to drive the
  *          IS61WV102416BLL SRAM memory mounted on STM324x7I-EVAL evaluation
  *          board(MB786).    
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
#include "STM324x7i_eval_fmc_sram.h"

/** @addtogroup Utilities
  * @{
  */

/** @addtogroup STM32_EVAL
  * @{
  */

/** @addtogroup STM324x7I_EVAL
  * @{
  */

/** @addtogroup STM324x7I_EVAL_FMC_SRAM
  * @brief     This file provides a set of functions needed to drive the 
  *            CY7C1071DV33-12BAXI SRAM memory mounted on STM324x7I-EVAL board.
  * @{
  */

/** @defgroup STM324x7I_EVAL_FMC_SRAM_Private_Types
  * @{
  */
/**
  * @}
  */


/** @defgroup STM324x7I_EVAL_FMC_SRAM_Private_Defines
  * @{
  */
/**
  * @brief  FMC Bank 1 NOR/SRAM2
  */
#define Bank1_SRAM2_ADDR  ((uint32_t)0x64000000)  

/**
  * @}
  */


/** @defgroup STM324x7I_EVAL_FMC_SRAM_Private_Macros
  * @{
  */
/**
  * @}
  */


/** @defgroup STM324x7I_EVAL_FMC_SRAM_Private_Variables
  * @{
  */
/**
  * @}
  */


/** @defgroup STM324x7I_EVAL_FMC_SRAM_Private_Function_Prototypes
  * @{
  */
/**
  * @}
  */


/** @defgroup STM324x7I_EVAL_FMC_SRAM_Private_Functions
  * @{
  */

/**
  * @brief  Configures the FMC and GPIOs to interface with the SRAM memory.
  *         This function must be called before any write/read operation
  *         on the SRAM.
  * @param  None
  * @retval None
  */
void SRAM_Init(void)
{
  FMC_NORSRAMInitTypeDef  FMC_NORSRAMInitStructure;
  FMC_NORSRAMTimingInitTypeDef  p;
  GPIO_InitTypeDef GPIO_InitStructure; 
  
  /* Enable GPIOs clock */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOE | RCC_AHB1Periph_GPIOF |
                         RCC_AHB1Periph_GPIOG, ENABLE);

  /* Enable FMC clock */
  RCC_AHB3PeriphClockCmd(RCC_AHB3Periph_FMC, ENABLE); 
  
/*-- GPIOs Configuration -----------------------------------------------------*/
/*
 +-------------------+--------------------+------------------+------------------+
 | PD0  <-> FMC_D2  | PE0  <-> FMC_NBL0 | PF0 <-> FMC_A0  | PG0 <-> FMC_A10 |
 | PD1  <-> FMC_D3  | PE1  <-> FMC_NBL1 | PF1 <-> FMC_A1  | PG1 <-> FMC_A11 |
 | PD4  <-> FMC_NOE | PE2  <-> FMC_A23  | PF2 <-> FMC_A2  | PG2 <-> FMC_A12 |
 | PD5  <-> FMC_NWE | PE3  <-> FMC_A19  | PF3 <-> FMC_A3  | PG3 <-> FMC_A13 |
 | PD8  <-> FMC_D13 | PE4  <-> FMC_A20  | PF4 <-> FMC_A4  | PG4 <-> FMC_A14 |
 | PD9  <-> FMC_D14 | PE5  <-> FMC_A21  | PF5 <-> FMC_A5  | PG5 <-> FMC_A15 |
 | PD10 <-> FMC_D15 | PE6  <-> FMC_A22  | PF12 <-> FMC_A6 | PG9 <-> FMC_NE2 |
 | PD11 <-> FMC_A16 | PE7  <-> FMC_D4   | PF13 <-> FMC_A7 |------------------+
 | PD12 <-> FMC_A17 | PE8  <-> FMC_D5   | PF14 <-> FMC_A8 |
 | PD13 <-> FMC_A18 | PE9  <-> FMC_D6   | PF15 <-> FMC_A9 |
 | PD14 <-> FMC_D0  | PE10 <-> FMC_D7   |------------------+
 | PD15 <-> FMC_D1  | PE11 <-> FMC_D8   |
 +-------------------| PE12 <-> FMC_D9   |
                     | PE13 <-> FMC_D10  |
                     | PE14 <-> FMC_D11  |
                     | PE15 <-> FMC_D12  |
                     +--------------------+
*/

  /* GPIOD configuration */
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource0, GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource1, GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource4, GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource5, GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource8, GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource9, GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource10, GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource11, GPIO_AF_FMC); 
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource12, GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource13, GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource14, GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource15, GPIO_AF_FMC);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0  | GPIO_Pin_1  | GPIO_Pin_4  | GPIO_Pin_5  | 
                                GPIO_Pin_8  | GPIO_Pin_9  | GPIO_Pin_10 | GPIO_Pin_11 |
                                GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;

  GPIO_Init(GPIOD, &GPIO_InitStructure);


  /* GPIOE configuration */
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource0 , GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource1 , GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource2 , GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource3 , GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource4 , GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource5 , GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource6 , GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource7 , GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource8 , GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource9 , GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource10 , GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource11 , GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource12 , GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource13 , GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource14 , GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource15 , GPIO_AF_FMC);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0  | GPIO_Pin_1  | GPIO_Pin_2  | GPIO_Pin_3 |  
                                GPIO_Pin_4  | GPIO_Pin_5  | GPIO_Pin_6  | GPIO_Pin_7 |
                                GPIO_Pin_8  | GPIO_Pin_9  | GPIO_Pin_10 | GPIO_Pin_11|
                                GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;

  GPIO_Init(GPIOE, &GPIO_InitStructure);


  /* GPIOF configuration */
  GPIO_PinAFConfig(GPIOF, GPIO_PinSource0 , GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOF, GPIO_PinSource1 , GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOF, GPIO_PinSource2 , GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOF, GPIO_PinSource3 , GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOF, GPIO_PinSource4 , GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOF, GPIO_PinSource5 , GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOF, GPIO_PinSource12 , GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOF, GPIO_PinSource13 , GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOF, GPIO_PinSource14 , GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOF, GPIO_PinSource15 , GPIO_AF_FMC);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0  | GPIO_Pin_1  | GPIO_Pin_2  | GPIO_Pin_3  | 
                                GPIO_Pin_4  | GPIO_Pin_5  | GPIO_Pin_12 | GPIO_Pin_13 |
                                GPIO_Pin_14 | GPIO_Pin_15;      

  GPIO_Init(GPIOF, &GPIO_InitStructure);


  /* GPIOG configuration */
  GPIO_PinAFConfig(GPIOG, GPIO_PinSource0 , GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOG, GPIO_PinSource1 , GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOG, GPIO_PinSource2 , GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOG, GPIO_PinSource3 , GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOG, GPIO_PinSource4 , GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOG, GPIO_PinSource5 , GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOG, GPIO_PinSource9 , GPIO_AF_FMC);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0  | GPIO_Pin_1  | GPIO_Pin_2  | GPIO_Pin_3 | 
                                GPIO_Pin_4  | GPIO_Pin_5  |GPIO_Pin_9;      

  GPIO_Init(GPIOG, &GPIO_InitStructure);

/*-- FMC Configuration ------------------------------------------------------*/
  p.FMC_AddressSetupTime = 1;
  p.FMC_AddressHoldTime = 0;
  p.FMC_DataSetupTime = 2;
  p.FMC_BusTurnAroundDuration = 0;
  p.FMC_CLKDivision = 0;
  p.FMC_DataLatency = 0;
  p.FMC_AccessMode = FMC_AccessMode_A;

  FMC_NORSRAMInitStructure.FMC_Bank = FMC_Bank1_NORSRAM2;
  FMC_NORSRAMInitStructure.FMC_DataAddressMux = FMC_DataAddressMux_Disable;
  FMC_NORSRAMInitStructure.FMC_MemoryType = FMC_MemoryType_SRAM;
  FMC_NORSRAMInitStructure.FMC_MemoryDataWidth = FMC_NORSRAM_MemoryDataWidth_16b;
  FMC_NORSRAMInitStructure.FMC_BurstAccessMode = FMC_BurstAccessMode_Disable;
  FMC_NORSRAMInitStructure.FMC_AsynchronousWait = FMC_AsynchronousWait_Disable;  
  FMC_NORSRAMInitStructure.FMC_WaitSignalPolarity = FMC_WaitSignalPolarity_Low;
  FMC_NORSRAMInitStructure.FMC_WrapMode = FMC_WrapMode_Disable;
  FMC_NORSRAMInitStructure.FMC_WaitSignalActive = FMC_WaitSignalActive_BeforeWaitState;
  FMC_NORSRAMInitStructure.FMC_WriteOperation = FMC_WriteOperation_Enable;
  FMC_NORSRAMInitStructure.FMC_WaitSignal = FMC_WaitSignal_Disable;
  FMC_NORSRAMInitStructure.FMC_ExtendedMode = FMC_ExtendedMode_Disable;
  FMC_NORSRAMInitStructure.FMC_WriteBurst = FMC_WriteBurst_Disable;
  FMC_NORSRAMInitStructure.FMC_ContinousClock = FMC_CClock_SyncOnly;
  FMC_NORSRAMInitStructure.FMC_ReadWriteTimingStruct = &p;
  FMC_NORSRAMInitStructure.FMC_WriteTimingStruct = &p;

  FMC_NORSRAMInit(&FMC_NORSRAMInitStructure); 

  /*!< Enable FMC Bank1_SRAM2 Bank */
  FMC_NORSRAMCmd(FMC_Bank1_NORSRAM2, ENABLE); 

}

/**
  * @brief  Writes a Half-word buffer to the FMC SRAM memory.
  * @param  pBuffer : pointer to buffer.
  * @param  WriteAddr : SRAM memory internal address from which the data will be
  *         written.
  * @param  NumHalfwordToWrite : number of half-words to write.
  * @retval None
  */
void SRAM_WriteBuffer(uint16_t* pBuffer, uint32_t WriteAddr, uint32_t NumHalfwordToWrite)
{
  for (; NumHalfwordToWrite != 0; NumHalfwordToWrite--) /* while there is data to write */
  {
    /* Transfer data to the memory */
    *(uint16_t *) (Bank1_SRAM2_ADDR + WriteAddr) = *pBuffer++;

    /* Increment the address*/
    WriteAddr += 2;
  }
}

/**
  * @brief  Reads a block of data from the FMC SRAM memory.
  * @param  pBuffer : pointer to the buffer that receives the data read from the
  *         SRAM memory.
  * @param  ReadAddr : SRAM memory internal address to read from.
  * @param  NumHalfwordToRead : number of half-words to read.
  * @retval None
  */
void SRAM_ReadBuffer(uint16_t* pBuffer, uint32_t ReadAddr, uint32_t NumHalfwordToRead)
{
  for (; NumHalfwordToRead != 0; NumHalfwordToRead--) /* while there is data to read */
  {
    /* Read a half-word from the memory */
    *pBuffer++ = *(__IO uint16_t*) (Bank1_SRAM2_ADDR + ReadAddr);

    /* Increment the address*/
    ReadAddr += 2;
  }
}

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
