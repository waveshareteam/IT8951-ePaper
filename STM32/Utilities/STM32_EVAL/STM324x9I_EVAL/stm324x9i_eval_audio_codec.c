/**
  ******************************************************************************
  * @file    stm324x9i_eval_audio_codec.c
  * @author  MCD Application Team
  * @version V1.0.3
  * @date    13-November-2013
  * @brief   This file includes the low layer driver for WM8994 Audio Codec
  *          available on STM324x9I_EVAL evaluation board.  
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
 
/* File Info : ------------------------------------------------------------------------------------------
                                             User NOTES
1. How To use this driver:
--------------------------
   - This driver supports STM32F4xx devices on STM324x9I-EVAL (MB1045) Evaluation boards.

   - Configure the options in file STM324x9i_eval_audio_codec.h in the section CONFIGURATION.
      Refer to the sections 2 and 3 to have more details on the possible configurations.

   - Call the function EVAL_AUDIO_Init(
                                    OutputDevice : physical output mode (OUTPUT_DEVICE_SPEAKER, 
                                                   OUTPUT_DEVICE_HEADPHONE or OUTPUT_DEVICE_BOTH)                                                 
                                    Volume       : Initial volume to be set (0 is min (mute), 100 is max (100%)
                                    AudioFreq    : Audio frequency in Hz (8000, 16000, 22500, 32000...)
                                                   this parameter is relative to the audio file/stream type.
                                   )
      This function configures all the hardware required for the audio application (codec, I2C, SAI, 
      GPIOs, DMA and interrupt if needed). This function returns 0 if configuration is OK.
      if the returned value is different from 0 or the function is stuck then the communication with
      the codec or the IOExpander has failed (try to un-plug the power or reset device in this case).
      + OUTPUT_DEVICE_SPEAKER  : only speaker will be set as output for the audio stream.
      + OUTPUT_DEVICE_HEADPHONE: only headphones will be set as output for the audio stream.
      + OUTPUT_DEVICE_BOTH     : both Speaker and Headphone are used as outputs for the audio stream
                                 at the same time.

   - Call the function EVAL_AUDIO_Play(
                                  pBuffer : pointer to the audio data file address
                                  Size    : size of the buffer to be sent in Bytes
                                 )
      to start playing (for the first time) from the audio file/stream.

   - Call the function EVAL_AUDIO_PauseResume(
                                         Cmd: AUDIO_PAUSE (or 0) to pause playing or AUDIO_RESUME (or 
                                               any value different from 0) to resume playing.
                                         )
       Note. After calling EVAL_AUDIO_PauseResume() function for pause, only EVAL_AUDIO_PauseResume() should be called
          for resume (it is not allowed to call EVAL_AUDIO_Play() in this case).
       Note. This function should be called only when the audio file is played or paused (not stopped).

   - For each mode, you may need to implement the relative callback functions into your code.
      The Callback functions are named EVAL_AUDIO_XXX_CallBack() and only their prototypes are declared in 
      the STM324x9i_eval_audio_codec.h file. (refer to the example for more details on the callbacks implementations)

   - To Stop playing, to modify the volume level or to mute, use the functions
       EVAL_AUDIO_Stop(), EVAL_AUDIO_VolumeCtl() and EVAL_AUDIO_Mute().

   - The driver API and the callback functions are at the end of the STM324x9i_eval_audio_codec.h file.
 

 Driver architecture:
 --------------------
 This driver is composed of three main layers:
   o High Audio Layer: consists of the function API exported in the STM324x9i_eval_audio_codec.h file
     (EVAL_AUDIO_Init(), EVAL_AUDIO_Play() ...)
   o Codec Control layer: consists of the functions API controlling the audio codec (WM8994) and 
     included as local functions in file STM324x9i_eval_audio_codec.c (Codec_Init(), Codec_Play() ...)
   o Media Access Layer (MAL): which consists of functions allowing to access the media containing/
     providing the audio file/stream. These functions are also included as local functions into
     the STM324x9i_eval_audio_codec.c file (Audio_MAL_Init(), Audio_MAL_Play() ...)
  Each set of functions (layer) may be implemented independently of the others and customized when 
  needed.    

2. Modes description:
---------------------
     + AUDIO_MAL_MODE_NORMAL : is suitable when the audio file is in a memory location.
     + AUDIO_MAL_MODE_CIRCULAR: is suitable when the audio data are read either from a 
        memory location or from a device at real time (double buffer could be used).

3. DMA interrupts description:
------------------------------
     + EVAL_AUDIO_IT_TC_ENABLE: Enable this define to use the DMA end of transfer interrupt.
        then, a callback should be implemented by user to perform specific actions
        when the DMA has finished the transfer.
     + EVAL_AUDIO_IT_HT_ENABLE: Enable this define to use the DMA end of half transfer interrupt.
        then, a callback should be implemented by user to perform specific actions
        when the DMA has reached the half of the buffer transfer (generally, it is useful 
        to load the first half of buffer while DMA is loading from the second half).
     + EVAL_AUDIO_IT_ER_ENABLE: Enable this define to manage the cases of error on DMA transfer.

4. Known Limitations:
---------------------

  1- If the TDM Format used to paly in parallel 2 audio Stream (the first Stream is configured in codec SLOT0 and second 
     Stream in SLOT1) the Pause/Resume, volume and mute feature will control the both streams.
          
  2- Parsing of audio file is not implemented (in order to determine audio file properties: Mono/Stereo, Data size, 
     File size, Audio Frequency, Audio Data header size ...). The configuration is fixed for the given audio file.
     
  3- Supports only 16-bit audio data size.
  
--------------------------------------------------------------------------------------------------------*/

/* Includes ------------------------------------------------------------------*/
#include "stm324x9i_eval_audio_codec.h"

/** @addtogroup Utilities
  * @{
  */
  
/** @addtogroup STM32_EVAL
  * @{
  */ 

/** @addtogroup STM324x9I_EVAL
  * @{
  */

/** @addtogroup STM324x9I_EVAL_AUDIO_CODEC
  * @brief       This file includes the low layer driver for WM8994 Audio Codec
  *              available on STM324x9I_EVAL evaluation board.
  * @{
  */ 

/** @defgroup STM324x9I_EVAL_AUDIO_CODEC_Private_Types
  * @{
  */ 
/**
  * @}
  */ 
  
/** @defgroup STM324x9I_EVAL_AUDIO_CODEC_Private_Defines
  * @{
  */ 

/* The 7 bits Codec address (sent through I2C interface) */
#define CODEC_ADDRESS                 0x30  //0x34  /* b00110100 */

/**
  * @}
  */ 

/** @defgroup STM324x9I_EVAL_AUDIO_CODEC_Private_Macros
  * @{
  */
/**
  * @}
  */ 
  
/** @defgroup STM324x9I_EVAL_AUDIO_CODEC_Private_Variables
  * @{
  */

/* This structure is declared global because it is handled by two different functions */
static DMA_InitTypeDef DMA_InitStructure; 

/* This variable holds the total size of the audio file */
uint32_t AudioTotalSize = 0xFFFF;
/* This variable holds the remaining data in audio file */ 
uint32_t AudioRemSize   = 0xFFFF;
/* This variable holds the current position of audio pointer */ 
uint16_t *CurrentPos;             

__IO uint32_t  CODECTimeout = CODEC_LONG_TIMEOUT;   


/**
  * @}
  */ 

/** @defgroup STM324x9I_EVAL_AUDIO_CODEC_Private_Function_Prototypes
  * @{
  */ 
/**
  * @}
  */ 

/** @defgroup STM324x9I_EVAL_AUDIO_CODEC_Private_Functions
  * @{
  */ 

/*----------------------------------------------------------------------------
                           Audio Codec functions 
  ----------------------------------------------------------------------------*/
/* High Layer codec functions */
static uint32_t Codec_Init(uint16_t OutputDevice, uint8_t Volume, uint32_t AudioFreq);
static uint32_t Codec_DeInit(void);
static uint32_t Codec_Play(void);
static uint32_t Codec_PauseResume(uint32_t Cmd);
static uint32_t Codec_Stop(uint32_t Cmd);
static uint32_t Codec_VolumeCtrl(uint8_t Volume);
static uint32_t Codec_Mute(uint32_t Cmd);

/* Low layer codec functions */
static void     Codec_CtrlInterface_Init(void);
static void     Codec_CtrlInterface_DeInit(void);
static void     Codec_AudioInterface_Init(uint32_t AudioFreq);
static void     Codec_AudioInterface_DeInit(void);
static void     Codec_Reset(void);
static uint32_t Codec_WriteRegister(uint16_t RegisterAddr, uint16_t RegisterValue);
static uint32_t Codec_ReadRegister(uint16_t RegisterAddr);
static void     Codec_GPIO_Init(void);
static void     Codec_GPIO_DeInit(void);
static void     Delay(__IO uint32_t nCount);

/*----------------------------------------------------------------------------
                   MAL (Media Access Layer) functions 
  ----------------------------------------------------------------------------*/
/* Peripherals configuration functions */
static void     Audio_MAL_Init(void);
static void     Audio_MAL_DeInit(void);
static void     Audio_MAL_Play(uint32_t Addr, uint32_t Size);
static void     Audio_MAL_PauseResume(uint32_t Cmd, uint32_t Addr);
static void     Audio_MAL_Stop(void);

/*----------------------------------------------------------------------------*/

/**
  * @brief  Configure the audio peripherals.
  * @param  OutputDevice: physical output mode (OUTPUT_DEVICE_SPEAKER, 
                          OUTPUT_DEVICE_HEADPHONE or OUTPUT_DEVICE_BOTH)
  * @param  Volume: Initial volume level (from 0 (Mute) to 100 (Max))
  * @param  AudioFreq: Audio frequency used to play the audio stream.
  * @retval 0 if correct communication, else wrong communication
  */
uint32_t EVAL_AUDIO_Init(uint16_t OutputDevice, uint8_t Volume, uint32_t AudioFreq)
{    
  /* Perform low layer Codec initialization */
  if (Codec_Init(OutputDevice, VOLUME_CONVERT(Volume), AudioFreq) != 0)
  {
    return 1;                
  }
  else
  {    
    /* SAI data transfer preparation:
    Prepare the Media to be used for the audio transfer from memory to SAI peripheral */
    Audio_MAL_Init();
    
    /* Return 0 when all operations are OK */
    return 0;
  }
}

/**
  * @brief  Deinitializes all the resources used by the codec (those initialized
  *         by EVAL_AUDIO_Init() function) EXCEPT the I2C resources since they are 
  *         eventually used by other modules. 
  * @param  None
  * @retval 0 if correct communication, else wrong communication
  */
uint32_t EVAL_AUDIO_DeInit(void)
{ 
  /* DeInitialize the Media layer */
  Audio_MAL_DeInit();
  
  /* DeInitialize Codec */  
  Codec_DeInit();  
  
  return 0;
}

/**
  * @brief  Starts playing audio stream from a data buffer for a determined size. 
  * @param  pBuffer: Pointer to the buffer 
  * @param  Size: Number of audio data BYTES.
  * @retval 0 if correct communication, else wrong communication
  */
uint32_t EVAL_AUDIO_Play(uint16_t* pBuffer, uint32_t Size)
{
  /* Set the total number of data to be played (count in half-word) */
  AudioTotalSize = Size/2;

  /* Call the audio Codec Play function */
  Codec_Play();
  
  /* Update the Media layer and enable it for play */  
  Audio_MAL_Play((uint32_t)pBuffer, (uint32_t)(DMA_MAX(AudioTotalSize / 2)));
  
  /* Update the remaining number of data to be played */
  AudioRemSize = (Size/2) - DMA_MAX(AudioTotalSize);
  
  /* Update the current audio pointer position */
  CurrentPos = pBuffer + DMA_MAX(AudioTotalSize);
  
  return 0;
}

/**
  * @brief  This function Pauses or Resumes the audio file stream. In case
  *         of using DMA, the DMA Pause feature is used. In all cases the SAI 
  *         peripheral is disabled. 
  * 
  * @WARNING When calling EVAL_AUDIO_PauseResume() function for pause, only
  *          this function should be called for resume (use of EVAL_AUDIO_Play() 
  *          function for resume could lead to unexpected behavior).
  * 
  * @param  Cmd: AUDIO_PAUSE (or 0) to pause, AUDIO_RESUME (or any value different
  *         from 0) to resume. 
  * @retval 0 if correct communication, else wrong communication
  */
uint32_t EVAL_AUDIO_PauseResume(uint32_t Cmd)
{    
  /* Call the Audio Codec Pause/Resume function */
  if (Codec_PauseResume(Cmd) != 0)
  {
    return 1;
  }
  else
  {
    /* Call the Media layer pause/resume function */
    Audio_MAL_PauseResume(Cmd, 0);
    
    /* Return 0 if all operations are OK */
    return 0;
  }
}

/**
  * @brief  Stops audio playing and enables the MUTE mode by software. 
  * @param  Option: could be one of the following parameters 
  *           - CODEC_PDWN_SW: To enable the MUTE mode.                   
  *           - CODEC_PDWN_HW: To reset the codec by writing in 0x0000 address.
  *                            All registers will be reset to their default state. 
  *                            Then need to reconfigure the Codec after that.  
  * @note   Codec will not be physically powered-down.
  * @retval 0 if correct communication, else wrong communication
  */
uint32_t EVAL_AUDIO_Stop(uint32_t Option)
{
  /* Call Audio Codec Stop function */
  if (Codec_Stop(Option) != 0)
  {
    return 1;
  }
  else
  {
    /* Call Media layer Stop function */
    Audio_MAL_Stop();
    
    /* Update the remaining data number */
    AudioRemSize = AudioTotalSize;    
    
    /* Return 0 when all operations are correctly done */
    return 0;
  }
}

/**
  * @brief  Controls the current audio volume level. 
  * @param  Volume: Volume level to be set in percentage from 0% to 100% (0 for 
  *         Mute and 100 for Max volume level).
  * @retval 0 if correct communication, else wrong communication
  */
uint32_t EVAL_AUDIO_VolumeCtl(uint8_t Volume)
{
  /* Call the codec volume control function with converted volume value */
  return (Codec_VolumeCtrl(VOLUME_CONVERT(Volume)));
}

/**
  * @brief  Enables or disables the MUTE mode by software 
  * @param  Command: could be AUDIO_MUTE_ON to mute sound or AUDIO_MUTE_OFF to 
  *         unmute the codec and restore previous volume level.
  * @retval 0 if correct communication, else wrong communication
  */
uint32_t EVAL_AUDIO_Mute(uint32_t Cmd)
{ 
  /* Call the Codec Mute function */
  return (Codec_Mute(Cmd));
}

/**
  * @brief  This function handles main Media layer interrupt. 
  * @param  None
  * @retval 0 if correct communication, else wrong communication
  */
void Audio_MAL_IRQHandler(void)
{    
#ifdef AUDIO_MAL_DMA_IT_TC_EN
  /* Transfer complete interrupt */
  if (DMA_GetFlagStatus(AUDIO_MAL_DMA_STREAM, AUDIO_MAL_DMA_FLAG_TC) != RESET)
  {     
 #ifdef AUDIO_MAL_MODE_NORMAL
    /* Check if the end of file has been reached */
    if (AudioRemSize > DMA_MAX_SZE)
    {      
      /* Wait the DMA Stream to be effectively disabled */
      while (DMA_GetCmdStatus(AUDIO_MAL_DMA_STREAM) != DISABLE)
      {}
      
      /* Clear the Interrupt flag */
      DMA_ClearFlag(AUDIO_MAL_DMA_STREAM, AUDIO_MAL_DMA_FLAG_ALL);  
      
      /* Re-Configure the buffer address and size */
      DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t) CurrentPos;
      DMA_InitStructure.DMA_BufferSize = (uint32_t) (DMA_MAX(AudioRemSize));
      
      /* Configure the DMA Stream with the new parameters */
      DMA_Init(AUDIO_MAL_DMA_STREAM, &DMA_InitStructure);
      
      /* Enable the SAI DMA Stream*/
      DMA_Cmd(AUDIO_MAL_DMA_STREAM, ENABLE);    
      
      /* Update the current pointer position */
      CurrentPos += DMA_MAX(AudioRemSize);        
      
      /* Update the remaining number of data to be played */
      AudioRemSize -= DMA_MAX(AudioRemSize);    
    }
    else
    {
      /* Disable the SAI DMA Stream*/
      DMA_Cmd(AUDIO_MAL_DMA_STREAM, DISABLE);   
      
      /* Clear the Interrupt flag */
      DMA_ClearFlag(AUDIO_MAL_DMA_STREAM, AUDIO_MAL_DMA_FLAG_TC);       
      
      /* Manage the remaining file size and new address offset: This function 
      should be coded by user (its prototype is already declared in stm32_eval_audio_codec.h) */  
      EVAL_AUDIO_TransferComplete_CallBack((uint32_t)CurrentPos, 0);       
    }
    
 #elif defined(AUDIO_MAL_MODE_CIRCULAR)
    /* Manage the remaining file size and new address offset: This function 
       should be coded by user (its prototype is already declared in stm32_eval_audio_codec.h) */  
    EVAL_AUDIO_TransferComplete_CallBack((uint32_t)CurrentPos, AudioRemSize);    
    
    /* Clear the Interrupt flag */
    DMA_ClearFlag(AUDIO_MAL_DMA_STREAM, AUDIO_MAL_DMA_FLAG_TC);
 #endif /* AUDIO_MAL_MODE_NORMAL */  
  }
#endif /* AUDIO_MAL_DMA_IT_TC_EN */

#ifdef AUDIO_MAL_DMA_IT_HT_EN  
  /* Half Transfer complete interrupt */
  if (DMA_GetFlagStatus(AUDIO_MAL_DMA_STREAM, AUDIO_MAL_DMA_FLAG_HT) != RESET)
  {
    /* Manage the remaining file size and new address offset: This function 
       should be coded by user (its prototype is already declared in stm32_eval_audio_codec.h) */  
    EVAL_AUDIO_HalfTransfer_CallBack((uint32_t)CurrentPos, AudioRemSize);    
   
    /* Clear the Interrupt flag */
    DMA_ClearFlag(AUDIO_MAL_DMA_STREAM, AUDIO_MAL_DMA_FLAG_HT);    
  }
#endif /* AUDIO_MAL_DMA_IT_HT_EN */
  
#ifdef AUDIO_MAL_DMA_IT_TE_EN  
  /* FIFO Error interrupt */
  if ((DMA_GetFlagStatus(AUDIO_MAL_DMA_STREAM, AUDIO_MAL_DMA_FLAG_TE) != RESET) || \
     (DMA_GetFlagStatus(AUDIO_MAL_DMA_STREAM, AUDIO_MAL_DMA_FLAG_FE) != RESET) || \
     (DMA_GetFlagStatus(AUDIO_MAL_DMA_STREAM, AUDIO_MAL_DMA_FLAG_DME) != RESET))
    
  {
    /* Manage the error generated on DMA FIFO: This function 
       should be coded by user (its prototype is already declared in stm32_eval_audio_codec.h) */  
    EVAL_AUDIO_Error_CallBack((uint32_t*)&CurrentPos);    
    
    /* Clear the Interrupt flag */
    DMA_ClearFlag(AUDIO_MAL_DMA_STREAM, AUDIO_MAL_DMA_FLAG_TE | AUDIO_MAL_DMA_FLAG_FE | \
                                        AUDIO_MAL_DMA_FLAG_DME);
  }  
#endif /* AUDIO_MAL_DMA_IT_TE_EN */
}
/*---------------------------------------------------------------------------
                WM8994 Audio Codec Control Functions
 -----------------------------------------------------------------------------*/
/**
  * @brief  Initializes the audio codec and all related interfaces (control 
  *         interface: I2C and audio interface: SAI)
  * @param  OutputDevice: physical output mode (OUTPUT_DEVICE_SPEAKER, 
                          OUTPUT_DEVICE_HEADPHONE or OUTPUT_DEVICE_BOTH)
  * @param  Volume: Initial volume level (from 0 (Mute) to 100 (Max))
  * @param  AudioFreq: Audio frequency used to play the audio stream.
  * @retval 0 if correct communication, else wrong communication
  */
static uint32_t Codec_Init(uint16_t OutputDevice, uint8_t Volume, uint32_t AudioFreq)
{
//  uint32_t counter = 0; 
  
  /* Configure the Codec related IOs */
  Codec_GPIO_Init();   
  
  /* Configure the SAI peripheral */
  Codec_AudioInterface_Init(AudioFreq);  
  
  /* Enable SAI Block A to Start the communication */    
  SAI_Cmd(SAI1_Block_B, ENABLE); 
  SAI_SendData(SAI1_Block_B, 0x00);
  
  /* Initialize the Control interface of the Audio Codec */
  Codec_CtrlInterface_Init();  
	
	/*
	 *Enable all power for now
	 */	
  Codec_WriteRegister(0x02, 0xA5df); 
	/*
	 *CODEC ADC and DAC clock from WSPLL, all clocks enabled
	 */ 
	Codec_WriteRegister(0x00, 0x0f39);
		/*
	 *I2S bus data I/O formats, use digital mixer for output
	 *BCKO is slave
	 */
	Codec_WriteRegister(0x01, 0x0000);	 
	/*
	 *Full mixer analog input gain
	 */
	Codec_WriteRegister(0x03, 0x0000);		 
	/* 
	 *Enable headphone short circuit protection
	 */	 
	Codec_WriteRegister(0x04, 0x0202);
	/*
	 *Full master volume
	 */
	Codec_WriteRegister(0x10, 0x5555);	
	/*
	 *Enable full mixer volume on both channels
	 */
	Codec_WriteRegister(0x11, 0x0000);
	/*
	 *Bass and treble boost set to flat
	 */
	Codec_WriteRegister(0x12, 0x5515);
	/*
	 *Disable mute and de-emphasis
	 */	
	Codec_WriteRegister(0x13, 0x0000);
	/*
	 *Mixer off, other settings off
	 */
	Codec_WriteRegister(0x14, 0x0000);
		/*
	 *ADC decimator volume to max
	 */
	Codec_WriteRegister(0x20, 0x0000);
	/*
	 *No PGA mute, full gain
	 */ 
	Codec_WriteRegister(0x21, 0x0000);
	/*
	 *Select line in and MIC, max MIC gain
	 */	
	Codec_WriteRegister(0x22, 0x0f02);
	/*
	 *AGC
	 */	
	Codec_WriteRegister(0x23, 0x0000);
	
   return 0;
	 
//  /* Reset the Codec Registers */
//  Codec_Reset();
  
//  /* WM8994 Codec configuration  */
//  
//  /* Errata Work-Arounds */
//  
//  counter += Codec_WriteRegister(0x102, 0x0003);
//  counter += Codec_WriteRegister(0x817, 0x0000);
//  counter += Codec_WriteRegister(0x102, 0x0000);
//  
//  /* Analog Configuration */ 
//  
//  /* Enable VMID soft start (fast), Start-up Bias Current Enabled */
//  counter += Codec_WriteRegister(0x39, 0x0064);
//  
//  /* Enable bias generator, Enable VMID */
//  counter += Codec_WriteRegister(0x01, 0x0003);
//  
//  /* Add Delay */
//  Delay(50);
//  
//  /* Path Configurations */
//  
//  switch (OutputDevice)
//  {
//  case OUTPUT_DEVICE_BOTH:
//    /* Enable DAC1 (Left), Enable DAC1 (Right), 
//       also Enable DAC2 (Left), Enable DAC2 (Right)*/
//    counter += Codec_WriteRegister(0x05, 0x0303 | 0x0C0C);
//    
//    /* Enable the AIF1 Timeslot 0 (Left) to DAC 1 (Left) mixer path */
//    counter += Codec_WriteRegister(0x601, 0x0001);
//    
//    /* Enable the AIF1 Timeslot 0 (Right) to DAC 1 (Right) mixer path */
//    counter += Codec_WriteRegister(0x602, 0x0001);
//    
//    /* Enable the AIF1 Timeslot 1 (Left) to DAC 2 (Left) mixer path */
//    counter += Codec_WriteRegister(0x604, 0x0002);
//    
//    /* Enable the AIF1 Timeslot 1 (Right) to DAC 2 (Right) mixer path */
//    counter += Codec_WriteRegister(0x605, 0x0002);
//    break;
//  
//  case OUTPUT_DEVICE_SPEAKER:
//    /* Enable DAC1 (Left), Enable DAC1 (Right), 
//       Disable DAC2 (Left), Disable DAC2 (Right)*/
//    counter += Codec_WriteRegister(0x05, 0x0C0C);
//    
//    /* Enable the AIF1 Timeslot 0 (Left) to DAC 1 (Left) mixer path */
//    counter += Codec_WriteRegister(0x601, 0x0000);
//    
//    /* Enable the AIF1 Timeslot 0 (Right) to DAC 1 (Right) mixer path */
//    counter += Codec_WriteRegister(0x602, 0x0000);
//    
//    /* Disable the AIF1 Timeslot 1 (Left) to DAC 2 (Left) mixer path */
//    counter += Codec_WriteRegister(0x604, 0x0002);
//    
//    /* Disable the AIF1 Timeslot 1 (Right) to DAC 2 (Right) mixer path */
//    counter += Codec_WriteRegister(0x605, 0x0002);
//    break;
// 
//  case OUTPUT_DEVICE_HEADPHONE:
//    /* Disable DAC1 (Left), Disable DAC1 (Right), 
//       Enable DAC2 (Left), Enable DAC2 (Right)*/
//    counter += Codec_WriteRegister(0x05, 0x0303);
//    
//    /* Enable the AIF1 Timeslot 0 (Left) to DAC 1 (Left) mixer path */
//    counter += Codec_WriteRegister(0x601, 0x0001);
//    
//    /* Enable the AIF1 Timeslot 0 (Right) to DAC 1 (Right) mixer path */
//    counter += Codec_WriteRegister(0x602, 0x0001);
//    
//    /* Disable the AIF1 Timeslot 1 (Left) to DAC 2 (Left) mixer path */
//    counter += Codec_WriteRegister(0x604, 0x0000);
//    
//    /* Disable the AIF1 Timeslot 1 (Right) to DAC 2 (Right) mixer path */
//    counter += Codec_WriteRegister(0x605, 0x0000);
//    break;
//  
//  default:
//    /* Disable DAC1 (Left), Disable DAC1 (Right), 
//       Enable DAC2 (Left), Enable DAC2 (Right)*/
//    counter += Codec_WriteRegister(0x05, 0x0303);
//    
//    /* Enable the AIF1 Timeslot 0 (Left) to DAC 1 (Left) mixer path */
//    counter += Codec_WriteRegister(0x601, 0x0001);
//    
//    /* Enable the AIF1 Timeslot 0 (Right) to DAC 1 (Right) mixer path */
//    counter += Codec_WriteRegister(0x602, 0x0001);
//    
//    /* Disable the AIF1 Timeslot 1 (Left) to DAC 2 (Left) mixer path */
//    counter += Codec_WriteRegister(0x604, 0x0000);
//    
//    /* Disable the AIF1 Timeslot 1 (Right) to DAC 2 (Right) mixer path */
//    counter += Codec_WriteRegister(0x605, 0x0000);
//    break; 
//  }
//                                  
//                    
//  /*  Clock Configurations */
//  switch (AudioFreq)
//  {
//  case  SAI_AudioFreq_8k:
//    /* AIF1 Sample Rate = 8 (Khz), ratio=256 */ 
//    counter += Codec_WriteRegister(0x210, 0x0003);
//    break;
//    
//  case  SAI_AudioFreq_16k:
//    /* AIF1 Sample Rate = 16 (Khz), ratio=256 */ 
//    counter += Codec_WriteRegister(0x210, 0x0033);
//    break;
//    
//  case  SAI_AudioFreq_48k:
//    /* AIF1 Sample Rate = 48 (Khz), ratio=256 */ 
//    counter += Codec_WriteRegister(0x210, 0x0083);
//    break;
//    
//  case  SAI_AudioFreq_96k:
//    /* AIF1 Sample Rate = 96 (Khz), ratio=256 */ 
//    counter += Codec_WriteRegister(0x210, 0x00A3);
//    break;
//    
//  case  SAI_AudioFreq_11k:
//    /* AIF1 Sample Rate = 11.025 (Khz), ratio=256 */ 
//    counter += Codec_WriteRegister(0x210, 0x0013);
//    break;
//    
//  case  SAI_AudioFreq_22k:
//    /* AIF1 Sample Rate = 22.050 (Khz), ratio=256 */ 
//    counter += Codec_WriteRegister(0x210, 0x0043);
//    break;
//    
//  case  SAI_AudioFreq_44k:
//    /* AIF1 Sample Rate = 44.1 (Khz), ratio=256 */ 
//    counter += Codec_WriteRegister(0x210, 0x0073);
//    break; 
//    
//  default:
//    /* AIF1 Sample Rate = 48 (Khz), ratio=256 */ 
//    counter += Codec_WriteRegister(0x210, 0x0083);
//    break; 
//  }
//  
//  /* AIF1 Word Length = 16-bits, AIF1 Format = I2S (Default Register Value) */
//  counter += Codec_WriteRegister(0x300, 0x4010);
//  
//  /* slave mode */
//  counter += Codec_WriteRegister(0x302, 0x0000);
//  
//  /* Enable the DSP processing clock for AIF1, Enable the core clock */
//  counter += Codec_WriteRegister(0x208, 0x000A);
//  
//  /* Enable AIF1 Clock, AIF1 Clock Source = MCLK1 pin */
//  counter += Codec_WriteRegister(0x200, 0x0001);
//  
//  /* Analog Output Configuration */
//  
//  /* Enable SPKRVOL PGA, Enable SPKMIXR, Enable SPKLVOL PGA, Enable SPKMIXL */
//  counter += Codec_WriteRegister(0x03, 0x0300);
//  
//  /* Left Speaker Mixer Volume = 0dB */
//  counter += Codec_WriteRegister(0x22, 0x0000);
//  
//  /* Speaker output mode = Class D, Right Speaker Mixer Volume = 0dB ((0x23, 0x0100) = class AB)*/
//  counter += Codec_WriteRegister(0x23, 0x0000);
//  
//  /* Volume Control */
//  Codec_VolumeCtrl(Volume);
//  
//  /* Unmute DAC2 (Left) to Left Speaker Mixer (SPKMIXL) path, 
//     Unmute DAC2 (Right) to Right Speaker Mixer (SPKMIXR) path */
//  counter += Codec_WriteRegister(0x36, 0x0300);
//  
//  /* Enable bias generator, Enable VMID, Enable SPKOUTL, Enable SPKOUTR */
//  counter += Codec_WriteRegister(0x01, 0x3003);
//  
//  /* Headphone/Speaker Enable */
//  
//  /* Enable Class W, Class W Envelope Tracking = AIF1 Timeslot 0 */
//  counter += Codec_WriteRegister(0x51, 0x0005);
//  
//  /* Enable bias generator, Enable VMID, Enable HPOUT1 (Left) and Enable HPOUT1 (Right) input stages */
//  /* idem for Speaker */
//  counter += Codec_WriteRegister(0x01, 0x0303 | 0x3003);
//  
//  /* Enable HPOUT1 (Left) and HPOUT1 (Right) intermediate stages */
//  counter += Codec_WriteRegister(0x60, 0x0022);
//  
//  /* Enable Charge Pump */
//  counter += Codec_WriteRegister(0x4C, 0x9F25);
//  
//  /* Add Delay */
//  Delay(15);
//  
//  /* Select DAC1 (Left) to Left Headphone Output PGA (HPOUT1LVOL) path */
//  counter += Codec_WriteRegister(0x2D, 0x0001);
//  
//  /* Select DAC1 (Right) to Right Headphone Output PGA (HPOUT1RVOL) path */
//  counter += Codec_WriteRegister(0x2E, 0x0001);
//  
//  /* Enable Left Output Mixer (MIXOUTL), Enable Right Output Mixer (MIXOUTR) */
//  /* idem for SPKOUTL and SPKOUTR */
//  counter += Codec_WriteRegister(0x03, 0x0030 | 0x0300);
//  
//  /* Enable DC Servo and trigger start-up mode on left and right channels */
//  counter += Codec_WriteRegister(0x54, 0x0033);
//  
//  /* Add Delay */
//  Delay(250);
//  
//  /* Enable HPOUT1 (Left) and HPOUT1 (Right) intermediate and output stages. Remove clamps */
//  counter += Codec_WriteRegister(0x60, 0x00EE);
//  
//  /* Unmutes */
//  
//  /* Unmute DAC 1 (Left) */
//  counter += Codec_WriteRegister(0x610, 0x00C0);
//  
//  /* Unmute DAC 1 (Right) */
//  counter += Codec_WriteRegister(0x611, 0x00C0);
//  
//  /* Unmute the AIF1 Timeslot 0 DAC path */
//  counter += Codec_WriteRegister(0x420, 0x0000);
//  
//  /* Unmute DAC 2 (Left) */
//  counter += Codec_WriteRegister(0x612, 0x00C0);
//  
//  /* Unmute DAC 2 (Right) */
//  counter += Codec_WriteRegister(0x613, 0x00C0);
//  
//  /* Unmute the AIF1 Timeslot 1 DAC2 path */
//  counter += Codec_WriteRegister(0x422, 0x0000);
//  
//  /* Return communication control value */
//  return counter;  
}

/**
  * @brief  Restore the audio codec state to default state and free all used 
  *         resources.
  * @param  None
  * @retval 0 if correct communication, else wrong communication
  */
static uint32_t Codec_DeInit(void)
{
  uint32_t counter = 0; 

  /* Reset the Codec Registers */
  Codec_Reset();  
  
  /* Deinitialize all use GPIOs */
  Codec_GPIO_DeInit();

  /* Disable the Codec control interface */
  Codec_CtrlInterface_DeInit();
  
  /* Deinitialize the Codec audio interface (I2S) */
  Codec_AudioInterface_DeInit(); 
  
  /* Return communication control value */
  return counter;  
}

/**
  * @brief  Start the audio Codec play feature.
  * @note   For this codec no Play options are required.
  * @param  None
  * @retval 0 if correct communication, else wrong communication
  */
static uint32_t Codec_Play(void)
{
  /* 
     No actions required on Codec level for play command
     */  

  /* Return communication control value */
  return 0;  
}

/**
  * @brief  Pauses and resumes playing on the audio codec.
  * @param  Cmd: AUDIO_PAUSE (or 0) to pause, AUDIO_RESUME (or any value different
  *         from 0) to resume. 
  * @retval 0 if correct communication, else wrong communication
  */
static uint32_t Codec_PauseResume(uint32_t Cmd)
{
  uint32_t counter = 0;   
  
  /* Pause the audio file playing */
  if (Cmd == AUDIO_PAUSE)
  { 
    /* Mute the output */
    counter += Codec_Mute(AUDIO_MUTE_ON);
  }
  else /* AUDIO_RESUME */
  { 
    /* Unmute the output */
    counter += Codec_Mute(AUDIO_MUTE_OFF);
  }
  return counter;
}

/**
  * @brief  Stops audio playing and enables the MUTE mode by software.
  * @param  CodecPdwnMode: could be one of the following parameters
  *          - CODEC_PDWN_SW: only mutes the audio codec. When resuming from this 
  *                           mode the codec keeps the previous initialization
  *                           (no need to re-Initialize the codec registers).
  *          - CODEC_PDWN_HW: Reset the codec (Software Reset). When resuming from this
  *                           mode, the codec is set to default configuration 
  *                           (user should re-Initialize the codec in order to 
  *                            play again the audio stream).
  * @note   Codec will not be physically powered-down.
  * @retval 0 if correct communication, else wrong communication
  */
static uint32_t Codec_Stop(uint32_t CodecPdwnMode)
{
  uint32_t counter = 0;   

  /* Mute the output first */
  counter += Codec_Mute(AUDIO_MUTE_ON);
  
  if (CodecPdwnMode == CODEC_PDWN_SW)
  {    
     /* Only output mute required*/
  }
  else /* CODEC_PDWN_HW */
  { 
    /* Mute the AIF1 Timeslot 0 DAC1 path */
    counter += Codec_WriteRegister(0x420, 0x0200);
    
    /* Mute the AIF1 Timeslot 1 DAC2 path */
    counter += Codec_WriteRegister(0x422, 0x0200);
    
    /* Disable DAC1L_TO_HPOUT1L */
    counter += Codec_WriteRegister(0x2D, 0x0000);
    
    /* Disable DAC1R_TO_HPOUT1R */
    counter += Codec_WriteRegister(0x2E, 0x0000);
    
    /* Disable DAC1 and DAC2 */
    counter += Codec_WriteRegister(0x05, 0x0000);
    
    /* Reset Codec by wrinting in 0x0000 address register */
    counter += Codec_WriteRegister(0x0000, 0x0000);  
    
    /* Delay ms */
    Delay(10);
  }
  
  return counter;    
}

/**
  * @brief  Sets higher or lower the codec volume level.
  * @param  Volume: a byte value from 0 to 63 (refer to codec registers 
  *         description for more details).
  * @retval 0 if correct communication, else wrong communication
  */
static uint32_t Codec_VolumeCtrl(uint8_t Volume)
{
  uint32_t counter = 0;
  
  if (Volume > 0x3E)
  {
    /* Unmute audio codec */
    Codec_Mute(AUDIO_MUTE_OFF); 
    
    /* Left Headphone Volume */
    counter += Codec_WriteRegister(0x1C, 0x3F | 0x140); 
    
    /* Right Headphone Volume */
    counter += Codec_WriteRegister(0x1D, 0x3F | 0x140);  
    
    /* Left Speaker Volume */
    counter += Codec_WriteRegister(0x26, 0x3F | 0x140); 
    
    /* Right Speaker Volume */
    counter += Codec_WriteRegister(0x27, 0x3F | 0x140);  
  }
  else if (Volume == 0)
  {
    /* Mute audio codec */
    Codec_Mute(AUDIO_MUTE_ON); 
  }
  else
  {
    /* Unmute audio codec */
    Codec_Mute(AUDIO_MUTE_OFF); 
    
    /* Left Headphone Volume */
    counter += Codec_WriteRegister(0x1C, Volume | 0x140); 
    
    /* Right Headphone Volume */
    counter += Codec_WriteRegister(0x1D, Volume | 0x140); 
    
    /* Left Speaker Volume */
    counter += Codec_WriteRegister(0x26, Volume | 0x140); 
    
    /* Right Speaker Volume */
    counter += Codec_WriteRegister(0x27, Volume | 0x140);  
  }

  return counter;  
}
/**
  * @brief  Enables or disables the mute feature on the audio codec.
  * @param  Cmd: AUDIO_MUTE_ON to enable the mute or AUDIO_MUTE_OFF to disable the
  *             mute mode.
  * @retval 0 if correct communication, else wrong communication
  */
static uint32_t Codec_Mute(uint32_t Cmd)
{
  uint32_t counter = 0;  
  
  /* Set the Mute mode */
  if (Cmd == AUDIO_MUTE_ON)
  {
    /* Soft Mute the AIF1 Timeslot 0 DAC1 path L&R */
    counter += Codec_WriteRegister(0x420, 0x0200);
    
    /* Soft Mute the AIF1 Timeslot 1 DAC2 path L&R */
    counter += Codec_WriteRegister(0x422, 0x0200);
  }
  else /* AUDIO_MUTE_OFF Disable the Mute */
  {
    /* Unmute the AIF1 Timeslot 0 DAC1 path L&R */
    counter += Codec_WriteRegister(0x420, 0x0000);
    
    /* Unmute the AIF1 Timeslot 1 DAC2 path L&R */
    counter += Codec_WriteRegister(0x422, 0x0000);
  }
  
  return counter; 
}

/**
  * @brief  Resets the audio codec. It restores the default configuration of the 
  *         codec (this function shall be called before initializing the codec).
  * @param  None
  * @retval None
  */

static void Codec_Reset(void)
{
  uint32_t counter = 0; 

  /* Reset Codec by wrinting in 0x0000 address register */
  counter += Codec_WriteRegister(0x0000, 0x0000);  
  
  /* Delay ms */
  Delay(10);
}


/**
  * @brief  Writes a Byte to a given register into the audio codec through the 
            control interface (I2C)
  * @param  RegisterAddr: The address (location) of the register to be written.
  * @param  RegisterValue: the Byte value to be written into destination register.
  * @retval 0 if correct communication, else wrong communication
  */
static uint32_t Codec_WriteRegister(uint16_t RegisterAddr, uint16_t RegisterValue)
{
//  uint32_t result = 0;

  /* While the bus is busy */
//  CODECTimeout = CODEC_LONG_TIMEOUT;
//  while(I2C_GetFlagStatus(CODEC_I2C, I2C_FLAG_BUSY))
//  {
//    if((CODECTimeout--) == 0) return Codec_TIMEOUT_UserCallback();
//  }
  
  /* Start the config sequence */
  I2C_GenerateSTART(CODEC_I2C, ENABLE);

  /* Test on EV5 and clear it */
  CODECTimeout = CODEC_FLAG_TIMEOUT;
  while (!I2C_CheckEvent(CODEC_I2C, I2C_EVENT_MASTER_MODE_SELECT))
  {
    if((CODECTimeout--) == 0) return Codec_TIMEOUT_UserCallback();
  }
  
  /* Transmit the slave address and enable writing operation */
  I2C_Send7bitAddress(CODEC_I2C, CODEC_ADDRESS, I2C_Direction_Transmitter);

  /* Test on EV6 and clear it */
  CODECTimeout = CODEC_FLAG_TIMEOUT;
  while (!I2C_CheckEvent(CODEC_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
  {
    if((CODECTimeout--) == 0) return Codec_TIMEOUT_UserCallback();
  }

//  /* Transmit the MSB address first for write operation */
//  I2C_SendData(CODEC_I2C, (uint8_t)(RegisterAddr >> 8));

//  /* Test on EV8 and clear it */
//  CODECTimeout = CODEC_FLAG_TIMEOUT;
//  while (!I2C_CheckEvent(CODEC_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTING))
//  {
//    if((CODECTimeout--) == 0) return Codec_TIMEOUT_UserCallback();
//  }
//  
  /* Transmit the LSB address for write operation */
  I2C_SendData(CODEC_I2C, (uint8_t)(RegisterAddr));

  /* Test on EV8 and clear it */
  CODECTimeout = CODEC_FLAG_TIMEOUT;
  while (!I2C_CheckEvent(CODEC_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTING))
  {
    if((CODECTimeout--) == 0) return Codec_TIMEOUT_UserCallback();
  }
  
  /* Prepare the register value to be sent (MSB fisrt)*/
  I2C_SendData(CODEC_I2C, (uint8_t)(RegisterValue >> 8));
  
    /* Test on EV8 and clear it */
  CODECTimeout = CODEC_FLAG_TIMEOUT;
  while (!I2C_CheckEvent(CODEC_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTING))
  {
    if((CODECTimeout--) == 0) return Codec_TIMEOUT_UserCallback();
  }
  
  /* Prepare the register value to be sent (LSB)*/
  I2C_SendData(CODEC_I2C, (uint8_t)(RegisterValue));
  
  /*!< Wait till all data have been physically transferred on the bus */
  CODECTimeout = CODEC_LONG_TIMEOUT;
  while(!I2C_GetFlagStatus(CODEC_I2C, I2C_FLAG_BTF))
  {
    if((CODECTimeout--) == 0) Codec_TIMEOUT_UserCallback();
  }
  
  /* End the configuration sequence */
  I2C_GenerateSTOP(CODEC_I2C, ENABLE);  
  
//#ifdef VERIFY_WRITTENDATA
//  /* Verify that the data has been correctly written */  
//  result = (Codec_ReadRegister(RegisterAddr) == RegisterValue)? 0:1;
//#endif /* VERIFY_WRITTENDATA */

//  /* Return the verifying value: 0 (Passed) or 1 (Failed) */
//  return result;  
	return 0;
}

/**
  * @brief  Reads and returns the value of an audio codec register through the
  *         control interface (I2C).
  * @param  RegisterAddr: Address of the register to be read.
  * @retval Value of the register to be read or dummy value if the communication
  *         fails.
  */
static uint32_t Codec_ReadRegister(uint16_t RegisterAddr)
{
  uint32_t result = 0;

  /* While the bus is busy */
  CODECTimeout = CODEC_LONG_TIMEOUT;
  while(I2C_GetFlagStatus(CODEC_I2C, I2C_FLAG_BUSY))
  {
    if((CODECTimeout--) == 0) return Codec_TIMEOUT_UserCallback();
  }
  
  /* Start the config sequence */
  I2C_GenerateSTART(CODEC_I2C, ENABLE);

  /* Test on EV5 and clear it */
  CODECTimeout = CODEC_FLAG_TIMEOUT;
  while (!I2C_CheckEvent(CODEC_I2C, I2C_EVENT_MASTER_MODE_SELECT))
  {
    if((CODECTimeout--) == 0) return Codec_TIMEOUT_UserCallback();
  }
  
  /* Transmit the slave address and enable writing operation */
  I2C_Send7bitAddress(CODEC_I2C, CODEC_ADDRESS, I2C_Direction_Transmitter);

  /* Test on EV6 and clear it */
  CODECTimeout = CODEC_FLAG_TIMEOUT;
  while (!I2C_CheckEvent(CODEC_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
  {
    if((CODECTimeout--) == 0) return Codec_TIMEOUT_UserCallback();
  }

  /* Transmit the register address to be read (MSB fist)*/
  I2C_SendData(CODEC_I2C, (uint8_t)(RegisterAddr >> 8));
  
    /* Test on EV6 and clear it */
  CODECTimeout = CODEC_FLAG_TIMEOUT;
  while (!I2C_CheckEvent(CODEC_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTING))
  {
    if((CODECTimeout--) == 0) return Codec_TIMEOUT_UserCallback();
  }

  /* Transmit the register address to be read (LSB)*/
  I2C_SendData(CODEC_I2C, (uint8_t)(RegisterAddr));

  /* Test on EV8 and clear it */
  CODECTimeout = CODEC_FLAG_TIMEOUT;
  while (I2C_GetFlagStatus(CODEC_I2C, I2C_FLAG_BTF) == RESET)
  {
    if((CODECTimeout--) == 0) return Codec_TIMEOUT_UserCallback();
  }
  
  /* Send START condition a second time */  
  I2C_GenerateSTART(CODEC_I2C, ENABLE);
  
  /*!< Test on EV5 and clear it (cleared by reading SR1 then writing to DR) */
  CODECTimeout = CODEC_FLAG_TIMEOUT;
  while(!I2C_CheckEvent(CODEC_I2C, I2C_EVENT_MASTER_MODE_SELECT))
  {
    if((CODECTimeout--) == 0) return Codec_TIMEOUT_UserCallback();
  } 
  
  /* Send Codec address for read */
  I2C_Send7bitAddress(CODEC_I2C, CODEC_ADDRESS, I2C_Direction_Receiver);  
  
  /* Wait on ADDR flag to be set (ADDR is still not cleared at this level */
  CODECTimeout = CODEC_FLAG_TIMEOUT;
  while(I2C_GetFlagStatus(CODEC_I2C, I2C_FLAG_ADDR) == RESET)
  {
    if((CODECTimeout--) == 0) return Codec_TIMEOUT_UserCallback();
  }     
  
  /* Disable Acknowledgment */
  I2C_AcknowledgeConfig(CODEC_I2C, DISABLE); 
  I2C_NACKPositionConfig(CODEC_I2C, I2C_NACKPosition_Next);
  
  /* Clear ADDR register by reading SR1 then SR2 register (SR1 has already been read) */
  (void)CODEC_I2C->SR2;
  
  /* Test on EV8 and clear it */
  CODECTimeout = CODEC_FLAG_TIMEOUT;
  while (I2C_GetFlagStatus(CODEC_I2C, I2C_FLAG_BTF) == RESET)
  {
    if((CODECTimeout--) == 0) return Codec_TIMEOUT_UserCallback();
  }
  
  /* Send STOP Condition */
  I2C_GenerateSTOP(CODEC_I2C, ENABLE);
  
  /* Receive MSB first from the Codec */
  result = I2C_ReceiveData(CODEC_I2C);
  result = result << 8;

  /* Receive LSB from the Codec */
  result |= I2C_ReceiveData(CODEC_I2C);
  
  /* Wait to make sure that STOP flag has been cleared */
  CODECTimeout = CODEC_FLAG_TIMEOUT;
  while(CODEC_I2C->CR1 & I2C_CR1_STOP)
  {
    if((CODECTimeout--) == 0) return Codec_TIMEOUT_UserCallback();
  }  
  
  /* Re-Enable Acknowledgment to be ready for another reception */
  I2C_AcknowledgeConfig(CODEC_I2C, ENABLE);  
  
  /* Disable Acknowledgment position */
  I2C_NACKPositionConfig(CODEC_I2C, I2C_NACKPosition_Current);
    
  /* Clear AF flag for next communication */
  I2C_ClearFlag(CODEC_I2C, I2C_FLAG_AF); 
  
  /* Return the byte read from Codec */
  return result;
}

/**
  * @brief  Initializes the Audio Codec control interface (I2C).
  * @param  None
  * @retval None
  */
static void Codec_CtrlInterface_Init(void)
{
  I2C_InitTypeDef I2C_InitStructure;
  
  /* Enable the CODEC_I2C peripheral clock */
  RCC_APB1PeriphClockCmd(CODEC_I2C_CLK, ENABLE);

  /* If the I2C peripheral is already enabled, don't reconfigure it */
  if ((CODEC_I2C->CR1 & I2C_CR1_PE) == 0)
  {   
    /* CODEC_I2C peripheral configuration */
    I2C_DeInit(CODEC_I2C);
    I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
    I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
    I2C_InitStructure.I2C_OwnAddress1 =0xfe; //0;
    I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
    I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_InitStructure.I2C_ClockSpeed = I2C_SPEED;
  
    /* Initialize the I2C peripheral */
    I2C_Init(CODEC_I2C, &I2C_InitStructure);    
  
    /* Enable the I2C peripheral */
    I2C_Cmd(CODEC_I2C, ENABLE);
  }  
}

/**
  * @brief  Restore the Audio Codec control interface to its default state.
  *         This function doesn't de-initialize the I2C because the I2C peripheral
  *         may be used by other modules.
  * @param  None
  * @retval None
  */
static void Codec_CtrlInterface_DeInit(void)
{
  /* Disable the I2C peripheral */ 
  /* This step is not done here because the I2C interface can be used by other modules */
  /* I2C_DeInit(CODEC_I2C); */
}

/**
  * @brief  Initializes the Audio Codec audio interface (SAI)
  * @note   This function assumes that the SAI input clock (through PLLSAI)
  *         is already configured and ready to be used.    
  * @param  AudioFreq: Audio frequency to be configured for the SAI peripheral. 
  * @retval None
  */
static void Codec_AudioInterface_Init(uint32_t AudioFreq)
{
 uint32_t tmpdiv;
  
 SAI_InitTypeDef       SAI_InitStructure;
 SAI_FrameInitTypeDef  SAI_FrameInitStructure;
 SAI_SlotInitTypeDef   SAI_SlotInitStructure;
 
  /* Enable the CODEC_SAI peripheral clock */
  RCC_APB2PeriphClockCmd(CODEC_SAI_CLK, ENABLE);
   
  /* Configure Master Clock using the following formula :
  MCLK_x = SAI_CK_x / (MCKDIV[3:0] * 2) with MCLK_x = 256 * FS    //MCLK=11.294400
  FS = SAI_CK_x / (MCKDIV[3:0] * 2) * 256
  MCKDIV[3:0] = SAI_CK_x / FS * 512 */  
  tmpdiv = SAI_CLOCK_SOURCE / (AudioFreq * 512);
  //tmpdiv=0;
  SAI_InitStructure.SAI_NoDivider = SAI_MasterDivider_Enabled;
  SAI_InitStructure.SAI_MasterDivider = tmpdiv;
  
  /* Configure SAI_Block_x 
  LSBFirst : Disabled 
  DataSize : 16 */
  SAI_InitStructure.SAI_AudioMode = SAI_Mode_MasterTx;
  SAI_InitStructure.SAI_Protocol = SAI_Free_Protocol;
  SAI_InitStructure.SAI_DataSize = SAI_DataSize_16b;
  SAI_InitStructure.SAI_FirstBit = SAI_FirstBit_MSB;   //
  SAI_InitStructure.SAI_ClockStrobing = SAI_ClockStrobing_FallingEdge;  //
  SAI_InitStructure.SAI_Synchro = SAI_Asynchronous;
  SAI_InitStructure.SAI_OUTDRIV = SAI_OutputDrive_Enabled;  
  SAI_InitStructure.SAI_FIFOThreshold = SAI_FIFOThreshold_HalfFull;
  SAI_Init(CODEC_SAI, &SAI_InitStructure);
  
  /* Configure SAI_Block_x Frame 
  Frame Length : 32
  Frame active Length: 16
  FS Definition : Start frame + Channel Side identification
  FS Polarity: FS active Low
  FS Offset: FS asserted one bit before the first bit of slot 0 */ 
  SAI_FrameInitStructure.SAI_FrameLength = 32;
  SAI_FrameInitStructure.SAI_ActiveFrameLength = 16;  
  SAI_FrameInitStructure.SAI_FSDefinition = I2S_FS_ChannelIdentification;
  SAI_FrameInitStructure.SAI_FSPolarity = SAI_FS_ActiveLow;  
  SAI_FrameInitStructure.SAI_FSOffset = SAI_FS_FirstBit;     //
  SAI_FrameInit(CODEC_SAI, &SAI_FrameInitStructure);
  
  /* Configure SAI Block_x Slot 
  Slot First Bit Offset : 0
  Slot Size   : 16
  Slot Number : 4
  Slot Active : All slot actives */
  SAI_SlotInitStructure.SAI_FirstBitOffset = 0;
  SAI_SlotInitStructure.SAI_SlotSize = SAI_SlotSize_16b; 
  //SAI_SlotInitStructure.SAI_SlotSize = SAI_SlotSize_DataSize;
  SAI_SlotInitStructure.SAI_SlotNumber = 2;
  SAI_SlotInitStructure.SAI_SlotActive = USR_SAI_SlotActive;
  SAI_SlotInit(CODEC_SAI, &SAI_SlotInitStructure); 
  
  SAI_FlushFIFO(CODEC_SAI);
	

  /* Mono Mode Config */
#ifdef MONO_MODE
    SAI_MonoModeConfig(CODEC_SAI, SAI_MonoMode);
#endif /* MONO_MODE */
  
  /* The SAI peripheral will be enabled only in the EVAL_AUDIO_Play() function 
       or by user functions if DMA mode not enabled */  
}

/**
  * @brief  Restores the Audio Codec audio interface to its default state.
  * @param  None
  * @retval None
  */
static void Codec_AudioInterface_DeInit(void)
{
  /* Disable the CODEC_SAI peripheral (in case it hasn't already been disabled) */
  SAI_Cmd(CODEC_SAI, DISABLE);
  
  /* Deinitialize the CODEC_SAI peripheral */
  SAI_DeInit(SAI1);
  
  /* Disable the CODEC_SAI peripheral clock */
  RCC_APB2PeriphClockCmd(CODEC_SAI_CLK, DISABLE);
}

/**
  * @brief Initializes IOs used by the Audio Codec (on the control and audio 
  *        interfaces).
  * @param  None
  * @retval None
  */
static void Codec_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  
  /* Enable SAI and I2C GPIO clocks */
  RCC_AHB1PeriphClockCmd(CODEC_I2C_GPIO_CLOCK | CODEC_SAI_GPIO_CLOCK, ENABLE);
  
  /* CODEC_I2C SCL and SDA pins configuration -------------------------------------*/
  /* Connect pins to Periph */
  /* If the I2C peripheral is already enabled, don't reconfigure it */
  if ((CODEC_I2C->CR1 & I2C_CR1_PE) == 0)
  { 
    GPIO_PinAFConfig(CODEC_I2C_GPIO, CODEC_I2C_SCL_PINSRC, CODEC_I2C_GPIO_AF);  
    GPIO_PinAFConfig(CODEC_I2C_GPIO, CODEC_I2C_SDA_PINSRC, CODEC_I2C_GPIO_AF);
  
    GPIO_InitStructure.GPIO_Pin = CODEC_I2C_SCL_PIN | CODEC_I2C_SDA_PIN; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
    GPIO_Init(CODEC_I2C_GPIO, &GPIO_InitStructure);      
  }
  /*CODEC_SAI SD MCLK SCK and FS pins configuration --------------------------------*/
  
  /* SAI1_Block_B Pins configuration  *****************************************/
  /* Connect pin to Periph */
  /* Configuration SAI1_BLockB_SD Alternate */
  GPIO_PinAFConfig(CODEC_SAI_GPIO, CODEC_SAI_SD_PINSRC, GPIO_AF_SAI1); 
  /* Configuration SAI1_BLockB_MCLK Alternate */
  GPIO_PinAFConfig(CODEC_SAI_GPIO, CODEC_SAI_MCK_PINSRC, GPIO_AF_SAI1);  
  /* Configuration SAI1_BLockB_SCK Alternate */  
  GPIO_PinAFConfig(CODEC_SAI_GPIO, CODEC_SAI_SCK_PINSRC, GPIO_AF_SAI1); 
  /* Configuration SAI1_BLockB_FS Alternate */  
  GPIO_PinAFConfig(CODEC_SAI_GPIO, CODEC_SAI_FS_PINSRC, GPIO_AF_SAI1); 
 
    /* Configure pins as AF pushpull */
  GPIO_InitStructure.GPIO_Pin = CODEC_SAI_MCK_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
  GPIO_Init(CODEC_SAI_GPIO, &GPIO_InitStructure); 
  
  /* Configure pins as AF pushpull */
  GPIO_InitStructure.GPIO_Pin = CODEC_SAI_FS_PIN | CODEC_SAI_SD_PIN | CODEC_SAI_SCK_PIN ;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(CODEC_SAI_GPIO, &GPIO_InitStructure); 
}

/**
  * @brief  Restores the IOs used by the Audio Codec interface to their default state.
  * @param  None
  * @retval None
  */
static void Codec_GPIO_DeInit(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  
  /* Deinitialize all the GPIOs used by the driver */
  GPIO_InitStructure.GPIO_Pin = CODEC_SAI_FS_PIN | CODEC_SAI_SD_PIN | CODEC_SAI_SCK_PIN | CODEC_SAI_MCK_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_Init(CODEC_SAI_GPIO, &GPIO_InitStructure);     
  /* Disconnect pins from SAI peripheral */
  GPIO_PinAFConfig(CODEC_SAI_GPIO, CODEC_SAI_SD_PINSRC, 0x00); 
  GPIO_PinAFConfig(CODEC_SAI_GPIO, CODEC_SAI_MCK_PINSRC, 0x00);  
  GPIO_PinAFConfig(CODEC_SAI_GPIO, CODEC_SAI_SCK_PINSRC, 0x00);
  GPIO_PinAFConfig(CODEC_SAI_GPIO, CODEC_SAI_FS_PINSRC, 0x00); 
}

/**
  * @brief  Inserts a delay time (not accurate timing).
  * @param  nCount: specifies the delay time length.
  * @retval None
  */
static void Delay( __IO uint32_t nCount)
{
  for (; nCount != 0; nCount--);
}

#ifdef USE_DEFAULT_TIMEOUT_CALLBACK
/**
  * @brief  Basic management of the timeout situation.
  * @param  None
  * @retval None
  */
uint32_t Codec_TIMEOUT_UserCallback(void)
{
  /* The following code allows I2C error recovery and return to normal communication
     if the error source doesn’t still exist (ie. hardware issue..) */
  I2C_InitTypeDef I2C_InitStructure;
  
  /* LCD_ErrLog("> I2C Timeout error occurred\n"); */

  I2C_GenerateSTOP(CODEC_I2C, ENABLE);
  I2C_SoftwareResetCmd(CODEC_I2C, ENABLE);
  I2C_SoftwareResetCmd(CODEC_I2C, DISABLE);
    
  /* CODEC_I2C peripheral configuration */
  I2C_DeInit(CODEC_I2C);
  I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
  I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
  I2C_InitStructure.I2C_OwnAddress1 =0xfe;        // 0x34;
  I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
  I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
  I2C_InitStructure.I2C_ClockSpeed = I2C_SPEED;
  /* Enable the I2C peripheral */
  I2C_Cmd(CODEC_I2C, ENABLE);  
  I2C_Init(CODEC_I2C, &I2C_InitStructure);  

  /* At this stage the I2C error should be recovered and device can communicate
     again (except if the error source still exist).
     User can implement mechanism (ex. test on max trial number) to manage situation
     when the I2C can't recover from current error. */

  /* LCD_UsrLog("> I2C error recovered.\n"); */

  return 0;
}
#endif /* USE_DEFAULT_TIMEOUT_CALLBACK */


/*-----------------------------------------------------------------------------
                    Audio MAL Interface Control Functions
  ----------------------------------------------------------------------------*/

/**
  * @brief  Initializes and prepares the Media to perform audio data transfer 
  *         from Media to the SAI peripheral.
  * @param  None
  * @retval None
  */
static void Audio_MAL_Init(void)  
{   
#if defined(AUDIO_MAL_DMA_IT_TC_EN) || defined(AUDIO_MAL_DMA_IT_HT_EN) || defined(AUDIO_MAL_DMA_IT_TE_EN)
  NVIC_InitTypeDef NVIC_InitStructure;
#endif
  /* Enable the SAI  */
  SAI_Cmd(CODEC_SAI, DISABLE);
  
  /* Enable the DMA clock */
  RCC_AHB1PeriphClockCmd(AUDIO_MAL_DMA_CLOCK, ENABLE); 
  
  /* Configure the DMA Stream */
  DMA_Cmd(AUDIO_MAL_DMA_STREAM, DISABLE);
  DMA_DeInit(AUDIO_MAL_DMA_STREAM);
  /* Set the parameters to be configured */
  DMA_InitStructure.DMA_Channel = AUDIO_MAL_DMA_CHANNEL;  
  DMA_InitStructure.DMA_PeripheralBaseAddr = CODEC_SAI_ADDRESS;
  DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)0;      /* This field will be configured in play function */
  DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
  DMA_InitStructure.DMA_BufferSize = (uint32_t)0xFFFe;      /* This field will be configured in play function */
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = AUDIO_MAL_DMA_PERIPH_DATA_SIZE;
  DMA_InitStructure.DMA_MemoryDataSize = AUDIO_MAL_DMA_MEM_DATA_SIZE; 
#ifdef AUDIO_MAL_MODE_NORMAL
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
#elif defined(AUDIO_MAL_MODE_CIRCULAR)
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
#else
  #error "AUDIO_MAL_MODE_NORMAL or AUDIO_MAL_MODE_CIRCULAR should be selected !!"
#endif /* AUDIO_MAL_MODE_NORMAL */  
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;  
  DMA_Init(AUDIO_MAL_DMA_STREAM, &DMA_InitStructure);  

  /* Enable the selected DMA interrupts (selected in "stm32_eval_audio_codec.h" defines) */
#ifdef AUDIO_MAL_DMA_IT_TC_EN
  DMA_ITConfig(AUDIO_MAL_DMA_STREAM, DMA_IT_TC, ENABLE);
#endif /* AUDIO_MAL_DMA_IT_TC_EN */
#ifdef AUDIO_MAL_DMA_IT_HT_EN
  DMA_ITConfig(AUDIO_MAL_DMA_STREAM, DMA_IT_HT, ENABLE);
#endif /* AUDIO_MAL_DMA_IT_HT_EN */
#ifdef AUDIO_MAL_DMA_IT_TE_EN
  DMA_ITConfig(AUDIO_MAL_DMA_STREAM, DMA_IT_TE | DMA_IT_FE | DMA_IT_DME, ENABLE);
#endif /* AUDIO_MAL_DMA_IT_TE_EN */
  
  /* Enable the SAI DMA request */
  SAI_DMACmd(CODEC_SAI, ENABLE);
  
#if defined(AUDIO_MAL_DMA_IT_TC_EN) || defined(AUDIO_MAL_DMA_IT_HT_EN) || defined(AUDIO_MAL_DMA_IT_TE_EN)
  /* SAI DMA IRQ Channel configuration */
  NVIC_InitStructure.NVIC_IRQChannel = AUDIO_MAL_DMA_IRQ;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = EVAL_AUDIO_IRQ_PREPRIO;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = EVAL_AUDIO_IRQ_SUBRIO;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
#endif 
}

/**
  * @brief  Restore default state of the used Media.
  * @param  None
  * @retval None
  */
static void Audio_MAL_DeInit(void)  
{   
#if defined(AUDIO_MAL_DMA_IT_TC_EN) || defined(AUDIO_MAL_DMA_IT_HT_EN) || defined(AUDIO_MAL_DMA_IT_TE_EN)
  NVIC_InitTypeDef NVIC_InitStructure;  
  
  /* Deinitialize the NVIC interrupt for the SAI DMA Stream */
  NVIC_InitStructure.NVIC_IRQChannel = AUDIO_MAL_DMA_IRQ;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = EVAL_AUDIO_IRQ_PREPRIO;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = EVAL_AUDIO_IRQ_SUBRIO;
  NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
  NVIC_Init(&NVIC_InitStructure);  
#endif 
  
  /* Disable the DMA stream before the deinit */
  DMA_Cmd(AUDIO_MAL_DMA_STREAM, DISABLE);
  
  /* Dinitialize the DMA Stream */
  DMA_DeInit(AUDIO_MAL_DMA_STREAM);
  
  /* 
     The DMA clock is not disabled, since it can be used by other streams 
                                                                          */ 
}

/**
  * @brief  Starts playing audio stream from the audio Media.
  * @param  None
  * @retval None
  */
static void Audio_MAL_Play(uint32_t Addr, uint32_t Size)
{   
  /* Configure the buffer address and size */
  DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)Addr;
  DMA_InitStructure.DMA_BufferSize = (uint32_t)Size;

  /* Configure the DMA Stream with the new parameters */
  DMA_Init(AUDIO_MAL_DMA_STREAM, &DMA_InitStructure);
  
  /* Enable the SAI DMA Stream*/
  DMA_Cmd(AUDIO_MAL_DMA_STREAM, ENABLE);

  
  /* If the SAI peripheral is still not enabled, enable it */
  if (SAI_GetCmdStatus(CODEC_SAI) == DISABLE)
  {
    SAI_Cmd(CODEC_SAI, ENABLE);
  }   
}

/**
  * @brief  Pauses or Resumes the audio stream playing from the Media.
  * @param  Cmd: AUDIO_PAUSE (or 0) to pause, AUDIO_RESUME (or any value different
  *              from 0) to resume. 
  * @param  Addr: Address from/at which the audio stream should resume/pause.
  * @retval None
  */
static void Audio_MAL_PauseResume(uint32_t Cmd, uint32_t Addr)
{
  /* Pause the audio file playing */
  if (Cmd == AUDIO_PAUSE)
  {   
    /* Disable the SAI DMA request */
    SAI_DMACmd(CODEC_SAI, DISABLE);

    /* Pause the SAI DMA Stream 
        Note. For the STM32F4xx devices, the DMA implements a pause feature, 
              by disabling the stream, all configuration is preserved and data 
              transfer is paused till the next enable of the stream.*/
    DMA_Cmd(AUDIO_MAL_DMA_STREAM, DISABLE);
  }
  else /* AUDIO_RESUME */
  {
    /* Enable the I2S DMA request */
    SAI_DMACmd(CODEC_SAI, ENABLE);
  
    /* Resume the SAI DMA Stream 
        Note. For the STM32F4xx devices, the DMA implements a pause feature, 
              by disabling the stream, all configuration is preserved and data 
              transfer is paused till the next enable of the stream.*/
    DMA_Cmd(AUDIO_MAL_DMA_STREAM, ENABLE);
    
    /* If the SAI peripheral is still not enabled, enable it */
    if (SAI_GetCmdStatus(CODEC_SAI) == DISABLE)
    {
      SAI_Cmd(CODEC_SAI, ENABLE);
    }     
  }  
}

/**
  * @brief  Stops audio stream playing on the used Media.
  * @param  None
  * @retval None
  */
static void Audio_MAL_Stop(void)
{   
  /* Stop the Transfer on the SAI side: Stop and disable the DMA stream */
  DMA_Cmd(AUDIO_MAL_DMA_STREAM, DISABLE);

  /* Clear all the DMA flags for the next transfer */
  DMA_ClearFlag(AUDIO_MAL_DMA_STREAM, AUDIO_MAL_DMA_FLAG_TC |AUDIO_MAL_DMA_FLAG_HT | \
                                  AUDIO_MAL_DMA_FLAG_FE | AUDIO_MAL_DMA_FLAG_TE);
  
  /*  
           The SAI DMA requests are not disabled here.
                                                            */
  
  /* In all modes, disable the SAI peripheral */
  SAI_Cmd(CODEC_SAI, DISABLE);
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
