/**
  ******************************************************************************
  * @file    STM324x9i_eval_audio_codec.h
  * @author  MCD Application Team
  * @version V1.0.3
  * @date    13-November-2013
  * @brief   This file contains all the functions prototypes for the 
  *          STM324x9i_eval_audio_codec.c driver.
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
#ifndef __STM324x9I_EVAL_AUDIOCODEC_H
#define __STM324x9I_EVAL_AUDIOCODEC_H

/* Includes ------------------------------------------------------------------*/
#include "stm324x9i_eval.h"

/** @addtogroup Utilities
  * @{
  */

/** @addtogroup STM32_EVAL
  * @{
  */ 

/** @addtogroup STM324x9i_EVAL
  * @{
  */
    
/** @defgroup STM324x9i_EVAL_AUDIO_CODEC 
  * @{
  */    


/** @defgroup STM324x9i_EVAL_AUDIO_CODEC_Exported_Types
  * @{
  */

/** @defgroup STM324x9i_EVAL_AUDIO_CODEC_Exported_Constants
  * @{
  */ 

/*----------------------------------------------------------------------------
             CONFIGURATION: Audio Codec Driver Configuration parameters
  ----------------------------------------------------------------------------*/
/* Audio Transfer mode (DMA, Interrupt or Polling) */
/* Uncomment this line to enable the audio Transfer using DMA in normal mode */
#define AUDIO_MAL_MODE_NORMAL

/* Uncomment this line to enable the audio Transfer using DMA in circular mode */         
/* #define AUDIO_MAL_MODE_CIRCULAR */ 

/* For the DMA modes select the interrupt that will be used */
/* Uncomment this line to enable DMA Transfer Complete interrupt */
#define AUDIO_MAL_DMA_IT_TC_EN  
/* Uncomment this line to enable DMA Half Transfer Complete interrupt */      
/* #define AUDIO_MAL_DMA_IT_HT_EN */
/* Uncomment this line to enable DMA Transfer Error interrupt */  
/* #define AUDIO_MAL_DMA_IT_TE_EN */  

/* Select the interrupt preemption priority and subpriority for the DMA interrupt */
/* Select the preemption priority level(0 is the highest) */
#define EVAL_AUDIO_IRQ_PREPRIO           0 
 /* Select the sub-priority level (0 is the highest) */  
#define EVAL_AUDIO_IRQ_SUBRIO            0  


/* Uncomment the following line to use the default Codec_TIMEOUT_UserCallback() 
   function implemented in stm324x9i_eval_audio_codec.c file.
   Codec_TIMEOUT_UserCallback() function is called whenever a timeout condition 
   occurs during communication (waiting on an event that doesn't occur, bus 
   errors, busy devices ...). */   
#define USE_DEFAULT_TIMEOUT_CALLBACK 

/*----------------------------------------------------------------------------
                    OPTIONAL Configuration defines parameters
  ----------------------------------------------------------------------------*/

#define SAI_AudioFreq_192k          ((uint32_t)192000)
#define SAI_AudioFreq_96k           ((uint32_t)96000)
#define SAI_AudioFreq_48k           ((uint32_t)48000)
#define SAI_AudioFreq_44k           ((uint32_t)44100)
#define SAI_AudioFreq_32k           ((uint32_t)32000)
#define SAI_AudioFreq_22k           ((uint32_t)22050)
#define SAI_AudioFreq_16k           ((uint32_t)16000)
#define SAI_AudioFreq_11k           ((uint32_t)11025)
#define SAI_AudioFreq_8k            ((uint32_t)8000) 

/*----------------------------------------------------------------------------
                    USER SAI defines parameters
 -----------------------------------------------------------------------------*/
/* In Main Application the PLL_SAI or PLL_I2S is configured to have this specific
   output, used to have a clean Audio Frequency */
#define SAI_ClockPLLSAI             ((uint32_t)11289600)
#define SAI_ClockPLLI2S             ((uint32_t)49152000)
#define SAI_ClockExtern             ((uint32_t)14000000)

/* Uncomment one these lines to configure SAI PLL source */
#define SAI_CLOCK_SOURCE            SAI_ClockPLLI2S       /* Default configuration */
/* #define SAI_CLOCK_SOURCE            SAI_ClockPLLSAI */
/* #define SAI_CLOCK_SOURCE            SAI_ClockExtern */

/* In W8994 codec the Audio frame contains 4 slots : TDM Mode */
/* TDM format :
 +------------------|------------------|--------------------|-------------------+ 
 | CODEC_SLOT0 Left | CODEC_SLOT1 Left | CODEC_SLOT0 Right  | CODEC_SLOT1 Right |
 +------------------------------------------------------------------------------+ */ 
 
/* To have 2 separate audio stream in Both headphone and speaker the 4 slot must be activated */
#define SAI_CODEC_SLOT0_1           SAI_SlotActive_0 | SAI_SlotActive_1 | SAI_SlotActive_2 | SAI_SlotActive_3
/* To have an audio stream in headphone only SAI Slot 0 and Slot 2 must be activated */ 
#define SAI_CODEC_SLOT0             SAI_SlotActive_0 | SAI_SlotActive_1 
/* To have an audio stream in speaker only SAI Slot 1 and Slot 3 must be activated */ 
#define SAI_CODEC_SLOT1             SAI_SlotActive_1 | SAI_SlotActive_3

/* Uncomment this line to enable SAI mono mode */
/* #define MONO_MODE */

/* Uncomment one of these lines to configure SAI active slot parameter */
/* #define USR_SAI_SlotActive          SAI_CODEC_TDM  */
#define USR_SAI_SlotActive          SAI_CODEC_SLOT0        /* Default configuration */
/* #define USR_SAI_SlotActive          SAI_CODEC_SOLT1 */

/* Uncomment this line to enable verifying data sent to codec after each write 
  operation */
#define VERIFY_WRITTENDATA

/*----------------------------------------------------------------------------
                    Hardware Configuration defines parameters
 -----------------------------------------------------------------------------*/
/* SAI peripheral configuration defines */
#define CODEC_SAI                      SAI1_Block_B
#define CODEC_SAI_CLK                  RCC_APB2Periph_SAI1
#define CODEC_SAI_ADDRESS              (uint32_t)(&SAI1_Block_B->DR)
#define CODEC_SAI_GPIO_AF              GPIO_AF_SAI1
#define CODEC_SAI_GPIO_CLOCK           (RCC_AHB1Periph_GPIOE | RCC_AHB1Periph_GPIOF)
#define CODEC_SAI_FS_PIN               GPIO_Pin_9
#define CODEC_SAI_SCK_PIN              GPIO_Pin_8
#define CODEC_SAI_SD_PIN               GPIO_Pin_6
#define CODEC_SAI_MCK_PIN              GPIO_Pin_7
#define CODEC_SAI_FS_PINSRC            GPIO_PinSource9
#define CODEC_SAI_SCK_PINSRC           GPIO_PinSource8
#define CODEC_SAI_SD_PINSRC            GPIO_PinSource6
#define CODEC_SAI_MCK_PINSRC           GPIO_PinSource7
#define CODEC_SAI_GPIO                 GPIOF

/* SAI DMA Stream definitions */
#define AUDIO_MAL_DMA_CLOCK            RCC_AHB1Periph_DMA2
#define AUDIO_MAL_DMA_STREAM           DMA2_Stream5
#define AUDIO_MAL_DMA_CHANNEL          DMA_Channel_0
#define AUDIO_MAL_DMA_IRQ              DMA2_Stream5_IRQn
#define AUDIO_MAL_DMA_FLAG_TC          DMA_FLAG_TCIF5
#define AUDIO_MAL_DMA_FLAG_HT          DMA_FLAG_HTIF5
#define AUDIO_MAL_DMA_FLAG_FE          DMA_FLAG_FEIF5
#define AUDIO_MAL_DMA_FLAG_TE          DMA_FLAG_TEIF5
#define AUDIO_MAL_DMA_FLAG_DME         DMA_FLAG_DMEIF5
#define AUDIO_MAL_DMA_FLAG_ALL         (uint32_t)(AUDIO_MAL_DMA_FLAG_TC | AUDIO_MAL_DMA_FLAG_HT | AUDIO_MAL_DMA_FLAG_FE | AUDIO_MAL_DMA_FLAG_TE | AUDIO_MAL_DMA_FLAG_DME)
#define AUDIO_MAL_DMA_PERIPH_DATA_SIZE DMA_PeripheralDataSize_HalfWord
#define AUDIO_MAL_DMA_MEM_DATA_SIZE    DMA_MemoryDataSize_HalfWord
#define DMA_MAX_SZE                    0xFFFF

#define Audio_MAL_IRQHandler           DMA2_Stream5_IRQHandler

/* I2C peripheral configuration defines (control interface of the audio codec) */
#define CODEC_I2C                      I2C1
#define CODEC_I2C_CLK                  RCC_APB1Periph_I2C1
#define CODEC_I2C_GPIO_CLOCK           RCC_AHB1Periph_GPIOB
#define CODEC_I2C_GPIO_AF              GPIO_AF_I2C1
#define CODEC_I2C_GPIO                 GPIOB
#define CODEC_I2C_SCL_PIN              GPIO_Pin_8
#define CODEC_I2C_SDA_PIN              GPIO_Pin_9
#define CODEC_I2C_SCL_PINSRC           GPIO_PinSource8
#define CODEC_I2C_SDA_PINSRC           GPIO_PinSource9

/* Maximum Timeout values for flags and events waiting loops. These timeouts are
   not based on accurate values, they just guarantee that the application will 
   not remain stuck if the I2C communication is corrupted.
   You may modify these timeout values depending on CPU frequency and application
   conditions (interrupts routines ...). */   
#define CODEC_FLAG_TIMEOUT             ((uint32_t)0x1000)
#define CODEC_LONG_TIMEOUT             ((uint32_t)(300 * CODEC_FLAG_TIMEOUT))


/*----------------------------------------------------------------------------
                        Audio Codec User defines
 -----------------------------------------------------------------------------*/
/* Codec output DEVICE */
#define OUTPUT_DEVICE_SPEAKER         1
#define OUTPUT_DEVICE_HEADPHONE       2
#define OUTPUT_DEVICE_BOTH            3
                                       
#define AUDIO_PAUSE                   0
#define AUDIO_RESUME                  1

/* Codec POWER DOWN modes */
#define CODEC_PDWN_HW                 1
#define CODEC_PDWN_SW                 2
                                       
/* MUTE commands */
#define AUDIO_MUTE_ON                 1
#define AUDIO_MUTE_OFF                0
/*----------------------------------------------------------------------------*/
/**
  * @}
  */ 

/** @defgroup STM324x9i_EVAL_AUDIO_CODEC_Exported_Macros
  * @{
  */ 
#define VOLUME_CONVERT(Volume)    ((Volume > 100)? 100:((uint8_t)((Volume * 63) / 100)))
#define DMA_MAX(x)                (((x) <= DMA_MAX_SZE)? (x):DMA_MAX_SZE)

/**
  * @}
  */ 

/** @defgroup STM324x9i_EVAL_AUDIO_CODEC_Exported_Functions
  * @{
  */ 

uint32_t EVAL_AUDIO_Init(uint16_t OutputDevice, uint8_t Volume, uint32_t AudioFreq);
uint32_t EVAL_AUDIO_DeInit(void);
uint32_t EVAL_AUDIO_Play(uint16_t* pBuffer, uint32_t Size);
uint32_t EVAL_AUDIO_PauseResume(uint32_t Cmd);
uint32_t EVAL_AUDIO_Stop(uint32_t CodecPowerDown_Mode);
uint32_t EVAL_AUDIO_VolumeCtl(uint8_t Volume);
uint32_t EVAL_AUDIO_Mute(uint32_t Command);

/* User Callbacks: user has to implement these functions in his code if
  they are needed. -----------------------------------------------------------*/

/* This function is called when the requested data has been completely transferred.
   In Normal mode (when  the define AUDIO_MAL_MODE_NORMAL is enabled) this function
   is called at the end of the whole audio file.
   In circular mode (when  the define AUDIO_MAL_MODE_CIRCULAR is enabled) this 
   function is called at the end of the current buffer transmission. */
void EVAL_AUDIO_TransferComplete_CallBack(uint32_t pBuffer, uint32_t Size);

/* This function is called when half of the requested buffer has been transferred 
   This callback is useful in Circular mode only (when AUDIO_MAL_MODE_CIRCULAR 
   define is enabled)*/
void EVAL_AUDIO_HalfTransfer_CallBack(uint32_t pBuffer, uint32_t Size);

/* This function is called when an Interrupt due to transfer error on or peripheral
   error occurs. */
void EVAL_AUDIO_Error_CallBack(void* pData);

/* Codec_TIMEOUT_UserCallback() function is called whenever a timeout condition 
   occurs during communication (waiting on an event that doesn't occur, bus 
   errors, busy devices ...) on the Codec control interface (I2C).
   You can use the default timeout callback implementation by uncommenting the 
   define USE_DEFAULT_TIMEOUT_CALLBACK in stm324x9i_eval_audio_codec.h file.
   Typically the user implementation of this callback should reset I2C peripheral
   and re-initialize communication or in worst case reset all the application. */
uint32_t Codec_TIMEOUT_UserCallback(void);
 
#endif /* __STM324x9i_EVAL_AUDIOCODEC_H */

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
