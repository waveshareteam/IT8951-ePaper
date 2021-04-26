/**
  ******************************************************************************
  * @file    stm324x9i_eval_fmc_nor.c
  * @author  MCD Application Team
  * @version V1.0.3
  * @date    13-November-2013
  * @brief   This file provides a set of functions needed to drive the
  *          M29WV128G NOR memory mounted on STM324x9I-EVAL evaluation
  *          board.    
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
#include "stm324x9i_eval_fmc_nor.h"

/** @addtogroup Utilities
  * @{
  */ 

/** @addtogroup STM32_EVAL
  * @{
  */ 

/** @addtogroup STM324x9I_EVAL
  * @{
  */
  
/** @addtogroup STM324x9I_EVAL_FMC_NOR
  * @brief     This file provides a set of functions needed to drive the 
  *            M29WV128G NOR flash memory mounted on STM324x9I-EVAL board.
  * @{
  */ 

/** @defgroup STM324x9I_EVAL_FMC_NOR_Private_Macros
  * @{
  */
/**
  * @brief  FMC NOR address shift
  */  
#define ADDR_SHIFT(Address) (NOR_BANK_ADDR + (2 * (Address)))  

/**
  * @brief  FMC NOR write
  */ 
#define NOR_WRITE(Address, Data)  (*(__IO uint16_t *)(Address) = (Data))
/**
  * @}
  */

/** @defgroup STM324x9I_EVAL_FMC_NOR_Private_Functions
  * @{
  */
  
/**
  * @brief  Configures the FMC and GPIOs to interface with the NOR flash memory.
  *         This function must be called before any read/write operation
  *         on the NOR flash.
  * @param  None
  * @retval None
  */
void NOR_Init(void)
{
  FMC_NORSRAMInitTypeDef  FMC_NORSRAMInitStructure;
  FMC_NORSRAMTimingInitTypeDef  FMC_NORSRAMTimingStructure;

  /* GPIO configuration for FMC NOR bank */
  NOR_GPIOConfig();
  
  /* Enable FMC Clock */
  RCC_AHB3PeriphClockCmd(RCC_AHB3Periph_FMC, ENABLE); 

/* FMC Configuration ---------------------------------------------------------*/
  /* NOR memory timing configuration */
  FMC_NORSRAMTimingStructure.FMC_AddressSetupTime = 4;
  FMC_NORSRAMTimingStructure.FMC_AddressHoldTime = 3;
  FMC_NORSRAMTimingStructure.FMC_DataSetupTime = 7;
  FMC_NORSRAMTimingStructure.FMC_BusTurnAroundDuration = 1;
  FMC_NORSRAMTimingStructure.FMC_CLKDivision = 1;
  FMC_NORSRAMTimingStructure.FMC_DataLatency = 0;
  FMC_NORSRAMTimingStructure.FMC_AccessMode = FMC_AccessMode_A; 

  /* NOR memory control configuration */
  FMC_NORSRAMInitStructure.FMC_Bank = FMC_Bank1_NORSRAM1;
  FMC_NORSRAMInitStructure.FMC_DataAddressMux = FMC_DataAddressMux_Disable;
  FMC_NORSRAMInitStructure.FMC_MemoryType = FMC_MemoryType_NOR;
  FMC_NORSRAMInitStructure.FMC_MemoryDataWidth = FMC_NORSRAM_MemoryDataWidth_16b;
  FMC_NORSRAMInitStructure.FMC_BurstAccessMode = NOR_BURSTACCESS;  
  FMC_NORSRAMInitStructure.FMC_WaitSignalPolarity = FMC_WaitSignalPolarity_Low;
  FMC_NORSRAMInitStructure.FMC_WrapMode = FMC_WrapMode_Disable;
  FMC_NORSRAMInitStructure.FMC_WaitSignalActive = FMC_WaitSignalActive_BeforeWaitState;
  FMC_NORSRAMInitStructure.FMC_WriteOperation = FMC_WriteOperation_Enable;
  FMC_NORSRAMInitStructure.FMC_WaitSignal = FMC_WaitSignal_Disable;
  FMC_NORSRAMInitStructure.FMC_ExtendedMode = FMC_ExtendedMode_Disable;
  FMC_NORSRAMInitStructure.FMC_AsynchronousWait = FMC_AsynchronousWait_Disable;
  FMC_NORSRAMInitStructure.FMC_WriteBurst = NOR_WRITEBURST;
  FMC_NORSRAMInitStructure.FMC_ContinousClock = CONTINUOUSCLOCK_FEATURE;
  FMC_NORSRAMInitStructure.FMC_ReadWriteTimingStruct = &FMC_NORSRAMTimingStructure;
  FMC_NORSRAMInitStructure.FMC_WriteTimingStruct = &FMC_NORSRAMTimingStructure;

  /* FMC NOR memory de-initializtion */
  FMC_NORSRAMDeInit(FMC_Bank1_NORSRAM1);
  
  /* FMC NOR bank initialization */
  FMC_NORSRAMInit(&FMC_NORSRAMInitStructure);
  
  /* Enable the NOR memory bank */
  FMC_NORSRAMCmd(FMC_Bank1_NORSRAM1, ENABLE);
  
}

/**
  * @brief  Configures all SDRAM memory I/Os pins. 
  * @param  None. 
  * @retval None.
  */
void NOR_GPIOConfig(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  
  /* Enable GPIO Clock */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOG | RCC_AHB1Periph_GPIOE |
                         RCC_AHB1Periph_GPIOF, ENABLE);

/* GPIOs Configuration -------------------------------------------------------*/
/*
 +-------------------+--------------------+--------------------+--------------------+
 +                       NOR flash pins assignment             +                    +
 +-------------------+--------------------+--------------------+--------------------+
 | PD0  <-> FMC_D2   | PE2  <-> FMC_A23   | PF0  <-> FMC_A0    | PG0 <-> FMC_A10    | 
 | PD1  <-> FMC_D3   | PE3  <-> FMC_A19   | PF1  <-> FMC_A1    | PG1 <-> FMC_A11    | 
 | PD3  <-> FMC_CLK  | PE4  <-> FMC_A20   | PF2  <-> FMC_A2    | PG2 <-> FMC_A12    | 
 | PD4  <-> FMC_NOE  | PE5  <-> FMC_A21   | PF3  <-> FMC_A3    | PG3 <-> FMC_A13    | 
 | PD5  <-> FMC_NWE  | PE6  <-> FMC_A22   | PF4  <-> FMC_A4    | PG4 <-> FMC_A14    | 
 | PD6  <-> FMC_NWAIT| PE7  <-> FMC_D4    | PF5  <-> FMC_A5    | PG5 <-> FMC_A15    |
 | PD7  <-> FMC_NE1  | PE8  <-> FMC_D5    | PF12 <-> FMC_A6    |--------------------+   
 | PD8  <-> FMC_D13  | PE9  <-> FMC_D6    | PF13 <-> FMC_A7    | 
 | PD9  <-> FMC_D14  | PE10 <-> FMC_D7    | PF14 <-> FMC_A8    | 
 | PD10 <-> FMC_D15  | PE11 <-> FMC_D8    | PF15 <-> FMC_A9    |                    
 | PD11 <-> FMC_A16  | PE12 <-> FMC_D9    | -------------------+                  
 | PD12 <-> FMC_A17  | PE13 <-> FMC_D10   |                     
 | PD13 <-> FMC_A18  | PE14 <-> FMC_D11   | 
 | PD14 <-> FMC_D0   | PE15 <-> FMC_D12   |
 | PD15 <-> FMC_D1   |--------------------+  
 +-------------------+
 
*/   
  
/* GPIOs Configuration --------------------------------------------------------*/
  /* Common GPIO configuration */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL; 
  
  /* GPIOD configuration */
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource0, GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource1, GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource3, GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource4, GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource5, GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource6, GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource7, GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource8, GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource9, GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource10, GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource11, GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource12, GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource13, GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource14, GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource15, GPIO_AF_FMC);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0  | GPIO_Pin_1  |GPIO_Pin_3  | GPIO_Pin_4 | 
                                GPIO_Pin_5  | GPIO_Pin_6  |GPIO_Pin_7  | GPIO_Pin_8 |
                                GPIO_Pin_9  | GPIO_Pin_10 |GPIO_Pin_11 |GPIO_Pin_12 |
                                GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;

  GPIO_Init(GPIOD, &GPIO_InitStructure);

  /* GPIOE configuration */
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource2, GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource3, GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource4, GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource5, GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource6, GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource7, GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource8, GPIO_AF_FMC); 
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource9, GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource10, GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource11, GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource12, GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource13, GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource14, GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource15, GPIO_AF_FMC);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2  |GPIO_Pin_3  | GPIO_Pin_4 |GPIO_Pin_5  | 
                                GPIO_Pin_6  |GPIO_Pin_7  | GPIO_Pin_8 |GPIO_Pin_9  | 
                                GPIO_Pin_10 |GPIO_Pin_11 |GPIO_Pin_12 |GPIO_Pin_13 |
                                GPIO_Pin_14 | GPIO_Pin_15;
                                
  GPIO_Init(GPIOE, &GPIO_InitStructure);

  /* GPIOF configuration */ 
  GPIO_PinAFConfig(GPIOF,GPIO_PinSource0, GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOF,GPIO_PinSource1, GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOF,GPIO_PinSource2, GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOF,GPIO_PinSource3, GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOF,GPIO_PinSource4, GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOF,GPIO_PinSource5, GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOF,GPIO_PinSource12, GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOF,GPIO_PinSource13, GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOF,GPIO_PinSource14, GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOF,GPIO_PinSource15, GPIO_AF_FMC);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0  |GPIO_Pin_1  | GPIO_Pin_2  | GPIO_Pin_3  | 
                                GPIO_Pin_4  |GPIO_Pin_5  | GPIO_Pin_12 | GPIO_Pin_13 | 
                                GPIO_Pin_14 |GPIO_Pin_15;

  GPIO_Init(GPIOF, &GPIO_InitStructure);

  /* GPIOG configuration */
  GPIO_PinAFConfig(GPIOG,GPIO_PinSource0, GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOG,GPIO_PinSource1, GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOG,GPIO_PinSource2, GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOG,GPIO_PinSource3, GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOG,GPIO_PinSource4, GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOG,GPIO_PinSource5, GPIO_AF_FMC);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 |GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | 
                                GPIO_Pin_4 |GPIO_Pin_5;

  GPIO_Init(GPIOG, &GPIO_InitStructure); 

}

/**
  * @brief  Reads NOR memory's Manufacturer and Device Code.
  * @param  pNOR_ID: pointer to a NOR_IDTypeDef structure which will hold
  *         the Manufacturer and Device Code.
  * @retval 0 if NOR IDs are correct, other value if it's incorrect. 
  */
void NOR_ReadID(NOR_IDTypeDef* pNOR_ID)
{ 
  NOR_WRITE(ADDR_SHIFT(0x0555), 0x00AA);
  NOR_WRITE(ADDR_SHIFT(0x02AA), 0x0055);
  NOR_WRITE(ADDR_SHIFT(0x0555), 0x0090);

  pNOR_ID->Manufacturer_Code = *(__IO uint16_t *) ADDR_SHIFT(0x0000);
  pNOR_ID->Device_Code1 = *(__IO uint16_t *) ADDR_SHIFT(0x0001);
  pNOR_ID->Device_Code2 = *(__IO uint16_t *) ADDR_SHIFT(0x000E);
  pNOR_ID->Device_Code3 = *(__IO uint16_t *) ADDR_SHIFT(0x000F);
  
}

/**
  * @brief  Performs a block erase operation giving its address.
  *         This function returns the NOR memory status after block erase operation.  
  * @param  uwBlockAddress: Address of the block to erase.
  * @retval NOR_Status: The returned value can be: NOR_SUCCESS, NOR_ERROR
  *         or NOR_TIMEOUT.
  */
NOR_Status NOR_EraseBlock(uint32_t uwBlockAddress)
{
  NOR_WRITE(ADDR_SHIFT(0x0555), 0x00AA);
  NOR_WRITE(ADDR_SHIFT(0x02AA), 0x0055);
  NOR_WRITE(ADDR_SHIFT(0x0555), 0x0080);
  NOR_WRITE(ADDR_SHIFT(0x0555), 0x00AA);
  NOR_WRITE(ADDR_SHIFT(0x02AA), 0x0055);
  NOR_WRITE((NOR_BANK_ADDR + uwBlockAddress), 0x30);

  return (NOR_GetStatus(BLOCKERASE_TIMEOUT));
}

/**
  * @brief  Erases the entire chip.
  * @param  None                      
  * @retval NOR_Status: The returned value can be: NOR_SUCCESS, NOR_ERROR
  *         or NOR_TIMEOUT
  */
NOR_Status NOR_EraseChip(void)
{
  NOR_WRITE(ADDR_SHIFT(0x0555), 0x00AA);
  NOR_WRITE(ADDR_SHIFT(0x02AA), 0x0055);
  NOR_WRITE(ADDR_SHIFT(0x0555), 0x0080);
  NOR_WRITE(ADDR_SHIFT(0x0555), 0x00AA);
  NOR_WRITE(ADDR_SHIFT(0x02AA), 0x0055);
  NOR_WRITE(ADDR_SHIFT(0x0555), 0x0010);

  return (NOR_GetStatus(CHIPERASE_TIMEOUT));
}

/**
  * @brief  Writes a half-word to the NOR memory.
  *         This function returns the NOR memory status after block erase operation.  
  * @param  uwWriteAddress : NOR memory internal address to write to.
  *         uhData : Data to write.
  * @retval NOR_Status: The returned value can be: NOR_SUCCESS, NOR_ERROR
  *         or NOR_TIMEOUT.
  */   
NOR_Status NOR_WriteHalfWord(uint32_t uwWriteAddress, uint16_t uhData)
{
  NOR_WRITE(ADDR_SHIFT(0x0555), 0x00AA);
  NOR_WRITE(ADDR_SHIFT(0x02AA), 0x0055);
  NOR_WRITE(ADDR_SHIFT(0x0555), 0x00A0);
  NOR_WRITE((NOR_BANK_ADDR + uwWriteAddress), uhData);

  return (NOR_GetStatus(PROGRAM_TIMEOUT));
}

/**
  * @brief  Writes a half-word to the NOR memory.
  *         This function returns the NOR memory status after block erase operation.  
  * @param  pBuffer : pointer to buffer.  
  *         uwWriteAddress : NOR memory internal address to write to.
  *         uwBufferSize : number of Half words to write.
  * @retval NOR_Status: The returned value can be: NOR_SUCCESS, NOR_ERROR
  *         or NOR_TIMEOUT.
  */   
NOR_Status NOR_WriteBuffer(uint16_t* pBuffer, uint32_t uwWriteAddress, uint32_t uwBufferSize)
{
  NOR_Status status = NOR_ONGOING; 

  do
  {
    /* Transfer data to the memory */
    status = NOR_WriteHalfWord(uwWriteAddress, *pBuffer++);
    uwWriteAddress = uwWriteAddress + 2;
    uwBufferSize--;
  }
  while((status == NOR_SUCCESS) && (uwBufferSize != 0));
  
  return (status); 
  
}

/**
  * @brief  Writes a half-word buffer to the FMC NOR memory. This function 
  *         must be used only with S29GL128P NOR memory. 
  * @param  pBuffer : pointer to buffer.  
  *         uwWriteAddress : NOR memory internal address from which the data 
  *         will be written.
  *         uwBufferSize : number of Half words to write. The maximum allowed 
  *         value is 32 Half words (64 bytes).
  * @retval NOR_Status: The returned value can be: NOR_SUCCESS, NOR_ERROR
  *         or NOR_TIMEOUT.
  */ 
NOR_Status NOR_ProgramBuffer(uint16_t* pBuffer, uint32_t uwWriteAddress, uint32_t uwBufferSize)
{
  uint32_t lastloadedaddress = 0;
  uint32_t currentaddress = 0;
  uint32_t endaddress = 0;

  /* Initialize variables */
  currentaddress = uwWriteAddress;
  endaddress = uwWriteAddress + uwBufferSize - 1;
  lastloadedaddress = uwWriteAddress;

  /* Issue unlock command sequence */
  NOR_WRITE(ADDR_SHIFT(0x00555), 0x00AA);
  NOR_WRITE(ADDR_SHIFT(0x02AA), 0x0055);  

  /* Write Buffer Load Command */
  NOR_WRITE(ADDR_SHIFT(uwWriteAddress), 0x0025);
  NOR_WRITE(ADDR_SHIFT(uwWriteAddress), (uwBufferSize - 1));

  /* Load Data into NOR Buffer */
  while(currentaddress <= endaddress)
  {
    /* Store last loaded address & data value (for polling) */
    lastloadedaddress = currentaddress;
 
    NOR_WRITE(ADDR_SHIFT(currentaddress), *pBuffer++);
    currentaddress += 1; 
  }

  NOR_WRITE(ADDR_SHIFT(lastloadedaddress), 0x29);
  
  return(NOR_GetStatus(PROGRAM_TIMEOUT));
}

/**
  * @brief  Reads a half-word from the NOR memory. 
  * @param  uwReadAddress : NOR memory internal address to read from. 
  * @retval Half-word read from the NOR memory.
  */
uint16_t NOR_ReadHalfWord(uint32_t uwReadAddress)
{
  NOR_WRITE(ADDR_SHIFT(0x00555), 0x00AA); 
  NOR_WRITE(ADDR_SHIFT(0x002AA), 0x0055);  
  
  /* Send read data command */
  NOR_WRITE((NOR_BANK_ADDR + uwReadAddress), 0x00F0 );

  /* Return the data read */
  return (*(__IO uint16_t *)((NOR_BANK_ADDR + uwReadAddress)));
}

/**
  * @brief  Reads a block of data from the FMC NOR memory.
  * @param  pBuffer: pointer to the buffer that receives the data read from the 
  *         NOR memory.
  * @param  uwReadAddress: NOR memory internal address to read from.
  * @param  uwBufferSize : number of Half word to read.
  * @retval None
  */
void NOR_ReadBuffer(uint16_t* pBuffer, uint32_t uwReadAddress, uint32_t uwBufferSize)
{
  NOR_WRITE(ADDR_SHIFT(0x0555), 0x00AA);
  NOR_WRITE(ADDR_SHIFT(0x02AA), 0x0055);
  NOR_WRITE((NOR_BANK_ADDR + uwReadAddress), 0x00F0);

  for(; uwBufferSize != 0x00; uwBufferSize--) /* while there is data to read */
  {
    /* Read a Halfword from the NOR */
    *pBuffer++ = *(__IO uint16_t *)((NOR_BANK_ADDR + uwReadAddress));
    uwReadAddress = uwReadAddress + 2; 
  }  
}

/**
  * @brief  Returns the NOR memory to Read mode.
  * @param  None
  * @retval NOR_SUCCESS
  */
NOR_Status NOR_ReturnToReadMode(void)
{
  NOR_WRITE(NOR_BANK_ADDR, 0x00F0);

  return (NOR_SUCCESS);
}

/**
  * @brief  Returns the NOR memory to Read mode and resets the errors in the NOR 
  *         memory Status Register.  
  * @param  None
  * @retval NOR_SUCCESS
  */
NOR_Status NOR_Reset(void)
{
  NOR_WRITE(ADDR_SHIFT(0x00555), 0x00AA); 
  NOR_WRITE(ADDR_SHIFT(0x002AA), 0x0055); 
  
  /* Send read mode command */
  NOR_WRITE(NOR_BANK_ADDR, 0x00F0); 
  
  /* Return status success operation */
  return (NOR_SUCCESS);
}

/**
  * @brief  Returns the NOR operation status.
  * @param  Timeout: NOR progamming Timeout
  * @retval NOR_Status: The returned value can be: NOR_SUCCESS, NOR_ERROR
  *         or NOR_TIMEOUT
  */
NOR_Status NOR_GetStatus(uint32_t Timeout)
{ 
  uint16_t val1 = 0, val2 = 0;
  NOR_Status status = NOR_ONGOING; 
  uint32_t timeout = Timeout;

  /* Poll on NOR memory Ready/Busy signal ------------------------------------*/
  while((GPIO_ReadInputDataBit(NOR_READY_BUSY_GPIO, NOR_READY_BUSY_PIN) != NOR_BUSY_STATE) && (timeout > 0)) 
  {
    timeout--;
  }

  timeout = Timeout;
  
  while((GPIO_ReadInputDataBit(NOR_READY_BUSY_GPIO, NOR_READY_BUSY_PIN) == NOR_BUSY_STATE) && (timeout > 0))   
  {
    timeout--;
  }
  
  /* Get the NOR memory operation status -------------------------------------*/
  while((Timeout != 0x00) && (status != NOR_SUCCESS))
  {
    Timeout--;

    /*!< Read DQ6 and DQ5 */
    val1 = *(__IO uint16_t *)(NOR_BANK_ADDR);
    val2 = *(__IO uint16_t *)(NOR_BANK_ADDR);

    /* If DQ6 did not toggle between the two reads then return NOR_Success */
    if((val1 & 0x0040) == (val2 & 0x0040)) 
    {
      return NOR_SUCCESS;
    }

    if((val1 & 0x0020) != 0x0020)
    {
      status = NOR_ONGOING;
    }

    val1 = *(__IO uint16_t *)(NOR_BANK_ADDR);
    val2 = *(__IO uint16_t *)(NOR_BANK_ADDR);
    
    if((val1 & 0x0040) == (val2 & 0x0040)) 
    {
      return NOR_SUCCESS;
    }
    else if((val1 & 0x0020) == 0x0020)
    {
      return NOR_ERROR;
    }
  }

  if(Timeout == 0)
  {
    status = NOR_TIMEOUT;
  } 

  /* Return the operation status */
  return (status);
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
  *
  */ 

/**
  * @}
  */       
 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
