/**
  ******************************************************************************
  * @file    stm324x9i_eval_fmc_sram.h
  * @author  MCD Application Team
  * @version V1.0.3
  * @date    13-November-2013
  * @brief   This file contains all the functions prototypes for the 
  *          stm324x9i_eval_fmc_sram.c driver.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM324X9I_EVAL_SRAM_H
#define __STM324X9I_EVAL_SRAM_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm324x9i_eval.h"

/** @defgroup STM324x9I_EVAL_FMC_SRAM_Private_Defines
  * @{
  */

/**
  * @brief  FMC SRAM bank address
  */   
#define SRAM_BANK_ADDR  ((uint32_t)0x64000000)  
  
/**
  * @brief  FMC SRAM Memory Width
  */  
/* #define SRAM_MEMORY_WIDTH    FMC_NORSRAM_MemoryDataWidth_8b  */
#define SRAM_MEMORY_WIDTH    FMC_NORSRAM_MemoryDataWidth_16b

/**
  * @brief  FMC SRAM Burst Access mode
  */  
#define SRAM_BURSTACCESS    FMC_BurstAccessMode_Disable  
/* #define SRAM_BURSTACCESS    FMC_BurstAccessMode_Enable*/

/**
  * @brief  FMC SRAM Write Burst feature
  */  
#define SRAM_WRITEBURST    FMC_WriteBurst_Disable  
/* #define SRAM_WRITEBURST   FMC_WriteBurst_Enable */

/**
  * @brief  FMC SRAM Continuous Clock feature
  */  
#define CONTINUOUSCLOCK_FEATURE    FMC_CClock_SyncOnly 
/* #define CONTINUOUSCLOCK_FEATURE     FMC_CClock_SyncAsync */

/**
  * @}
  */ 

/** @defgroup STM324x9I_EVAL_FMC_SDRAM_Exported_Functions
  * @{
  */ 
void  SRAM_Init(void);
void  SRAM_GPIOConfig(void);
void  SRAM_WriteBuffer(uint16_t* pBuffer, uint32_t uwWriteAddress, uint32_t uwBufferSize);
void  SRAM_ReadBuffer(uint16_t* pBuffer, uint32_t uwReadAddress, uint32_t uwBufferSize);

#ifdef __cplusplus
}
#endif

#endif /* __STM324X9I_EVAL_SRAM_H */

/**
  * @}
  */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
