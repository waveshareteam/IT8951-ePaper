/**
  ******************************************************************************
  * @file    stm324x9i_eval_fmc_nor.h
  * @author  MCD Application Team
  * @version V1.0.3
  * @date    13-November-2013
  * @brief   This file contains all the functions prototypes for the 
  *          stm324x9i_eval_fmc_nor.c driver.
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
#ifndef __STM324X9I_EVAL_FMC_NOR_H
#define __STM324X9I_EVAL_FMC_NOR_H

/* Includes ------------------------------------------------------------------*/
#include "stm324x9i_eval.h"

/** @defgroup STM324x9I_EVAL_FMC_NOR_Exported_Types
  * @{
  */
/**
  * @brief  FMC NOR ID typedef
  */  
typedef struct
{
  uint16_t Manufacturer_Code;
  uint16_t Device_Code1;
  uint16_t Device_Code2;
  uint16_t Device_Code3;
}NOR_IDTypeDef;

/**
  * @brief  FMC NOR Status typedef
  */
typedef enum
{
  NOR_SUCCESS = 0,
  NOR_ONGOING,
  NOR_ERROR,
  NOR_TIMEOUT
}NOR_Status;  

/**
  * @}
  */

/** @defgroup STM324x9I_EVAL_FMC_SDRAM_Private_Defines
  * @{
  */
/**
  * @brief  FMC NOR Bank Address
  */  
#define NOR_BANK_ADDR        ((uint32_t)0x60000000)
 
/**
  * @brief  FMC NOR block erase timeout
  */ 
#define BLOCKERASE_TIMEOUT   ((uint32_t)0x00A00000)

/**
  * @brief  FMC NOR chip erase timeout
  */
#define CHIPERASE_TIMEOUT    ((uint32_t)0x30000000) 

/**
  * @brief  FMC NOR program timeout
  */
#define PROGRAM_TIMEOUT      ((uint32_t)0x00004400)

/**
  * @brief  FMC NOR Burst Access mode
  */  
#define NOR_BURSTACCESS     FMC_BurstAccessMode_Disable  
/* #define NOR_BURSTACCESS      FMC_BurstAccessMode_Enable */

/**
  * @brief  FMC NOR Write Burst feature
  */  
#define NOR_WRITEBURST      FMC_WriteBurst_Disable 
/* #define NOR_WRITEBURST       FMC_WriteBurst_Enable */

/**
  * @brief  FMC NOR Continuous Clock feature
  */  
#define CONTINUOUSCLOCK_FEATURE     FMC_CClock_SyncOnly
/* #define CONTINUOUSCLOCK_FEATURE      FMC_CClock_SyncAsync */


/* NOR Ready/Busy signal GPIO definitions */
#define NOR_READY_BUSY_PIN    GPIO_Pin_6 
#define NOR_READY_BUSY_GPIO   GPIOD
#define NOR_READY_STATE       SET 
#define NOR_BUSY_STATE        RESET 

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */  
  
/** @defgroup STM324x9I_EVAL_FMC_NOR_Exported_Functions
  * @{
  */
void        NOR_Init(void);  
void        NOR_GPIOConfig(void);
void        NOR_ReadID(NOR_IDTypeDef* pNOR_ID);
NOR_Status  NOR_EraseBlock(uint32_t uwBlockAddress);
NOR_Status  NOR_EraseChip(void);
NOR_Status  NOR_WriteHalfWord(uint32_t uwWriteAddress, uint16_t uhData);
NOR_Status  NOR_WriteBuffer(uint16_t* pBuffer, uint32_t uwWriteAddress, uint32_t uwBufferSize);
NOR_Status  NOR_ProgramBuffer(uint16_t* pBuffer, uint32_t uwWriteAddress, uint32_t uwBufferSize);
uint16_t    NOR_ReadHalfWord(uint32_t uwReadAddress);
void        NOR_ReadBuffer(uint16_t* pBuffer, uint32_t uwReadAddress, uint32_t uwBufferSize);
NOR_Status  NOR_ReturnToReadMode(void);
NOR_Status  NOR_Reset(void);
NOR_Status  NOR_GetStatus(uint32_t Timeout);

#endif /* __STM324X9I_EVAL_FMC_NOR_H */

/**
  * @}
  */
/**
  * @}
  */ 


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
