/**
  ******************************************************************************
  * @file    stm324xg_eval_ioe.h
  * @author  MCD Application Team
  * @version V1.0.2
  * @date    05-March-2012
  * @brief   This file contains all the functions prototypes for the
  *          stm324xg_eval_ioe.c driver.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2012 STMicroelectronics</center></h2>
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

  /* File Info : ---------------------------------------------------------------
    SUPPORTED FEATURES:
      - IO Read/write : Set/Reset and Read (Polling/Interrupt)
      - Joystick: config and Read (Polling/Interrupt)
      - Touch Screen Features: Single point mode (Polling/Interrupt)
      - TempSensor Feature: accuracy not determined (Polling).

    UNSUPPORTED FEATURES:
      - Row ADC Feature is not supported (not implemented on STM324xG_EVAL board)
  ----------------------------------------------------------------------------*/


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM324xG_EVAL_IOE_H
#define __STM324xG_EVAL_IOE_H

/* Includes ------------------------------------------------------------------*/
#include "stm324xg_eval.h"


/** 
  * @brief  Touch Screen Information structure  
  */
   
#define JOY_A_KEY_Port					GPIOE
#define JOY_A_KEY_Pin					GPIO_Pin_2
#define JOY_A_KEY_RCC_AHBPeriph			RCC_AHB1Periph_GPIOE

#define JOY_B_KEY_Port					GPIOE
#define JOY_B_KEY_Pin					GPIO_Pin_3
#define JOY_B_KEY_RCC_AHBPeriph			RCC_AHB1Periph_GPIOE

#define JOY_C_KEY_Port					GPIOE
#define JOY_C_KEY_Pin					GPIO_Pin_4
#define JOY_C_KEY_RCC_AHBPeriph			RCC_AHB1Periph_GPIOE

#define JOY_D_KEY_Port					GPIOE
#define JOY_D_KEY_Pin					GPIO_Pin_5
#define JOY_D_KEY_RCC_AHBPeriph			RCC_AHB1Periph_GPIOE

#define JOY_CTR_KEY_Port				GPIOE
#define JOY_CTR_KEY_Pin					GPIO_Pin_6
#define JOY_CTR_KEY_RCC_AHBPeriph		RCC_AHB1Periph_GPIOE


/** 
  * @brief  Configuration and initialization functions  
  */
void Key_Config(void);
JOYState_TypeDef Key_ReadIOPin(void);
JOYState_TypeDef Key_ReadIOPin_continuous(void);

#endif
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
