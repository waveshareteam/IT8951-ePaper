/**
  ******************************************************************************
  * @file    stm324x9i_eval_ioe16.c
  * @author  MCD Application Team
  * @version V1.0.3
  * @date    13-November-2013
  * @brief   This file provides a set of functions needed to manage the STMPE1600
  *          IO Expander devices mounted on STM324x9I-EVAL evaluation board.
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

/* File Info : -----------------------------------------------------------------
                                User NOTES

    - This driver doesn't use the DMA method for sending and receiving data on I2C 
      bus. It uses a direct I2C communication (polling).  
  
    - This driver is used to drive the io functionalities under the STM324x9I
      evaluation board. 
      
    SUPPORTED FEATURES:
      - IO Read/write : Set/Reset (Polling/Interrupt)

    UNSUPPORTED FEATURES:
      - Row ADC Feature is not supported (not implemented on STM324x9I_EVAL board)
      - Touch Screen Features: Single point mode (Polling/Interrupt)
      - Temperature Sensor Feature: accuracy not determined (Polling)
  ----------------------------------------------------------------------------*/

/* Includes ------------------------------------------------------------------*/
#include "stm324x9i_eval_ioe16.h"

/** @addtogroup Utilities
  * @{
  */

/** @addtogroup STM32_EVAL
  * @{
  */ 

/** @addtogroup STM324x9I_EVAL
  * @{
  */
    
/** @defgroup STM324x9I_EVAL_IOE16 
  * @brief  This file includes the IO Expander driver for STMPE1600 IO Expander 
  *         devices.
  * @{
  */ 


/** @defgroup STM324x9I_EVAL_IOE16_Private_Types_Definitions
  * @{
  */ 
/**
  * @}
  */ 


/** @defgroup STM324x9I_EVAL_IOE16_Private_Defines
  * @{
  */ 
#define TIMEOUT_MAX    0x3000 /*<! The value of the maximal timeout for I2C waiting loops */
/**
  * @}
  */ 


/** @defgroup STM324x9I_EVAL_IOE16_Private_Macros
  * @{
  */ 
/**
  * @}
  */ 

/** @defgroup STM324x9I_EVAL_IOE16_Private_Variables
  * @{
  */ 

uint32_t IOE16_TimeOut = TIMEOUT_MAX; /*<! Value of Timeout when I2C communication fails */
/**
  * @}
  */ 


/** @defgroup STM324x9I_EVAL_IOE16_Private_Function_Prototypes
  * @{
  */ 
static void IOE16_GPIO_Config(void);
static void IOE16_I2C_Config(void);
static void IOE16_EXTI_Config(void);

#ifndef USE_Delay
static void delay(__IO uint32_t nCount);
#endif /* USE_Delay*/
/**
  * @}
  */ 


/** @defgroup STM324x9I_EVAL_IOE16_Private_Functions
  * @{
  */ 


/**
  * @brief  Initializes and Configures the IO_Expander Functionalities and 
  *         configures all STM324x9I_EVAL necessary hardware resources
  *         (GPIOs, APB clocks..).
  * @param  None
  * @retval IOE16_OK if all initializations done correctly. Other value if error.
  */
uint8_t IOE16_Config(void)
{
  /* Configure the needed IO pins */
  IOE16_GPIO_Config(); 
  
  /* I2C initialization */
  IOE16_I2C_Config(); 
  
  /* Read IO Expander ID  */
  if(IOE16_IsOperational())
  {
    return IOE16_NOT_OPERATIONAL;
  }
  
  /* Generate IO Expander Software reset */
  IOE16_Reset();  
  
  /* Configuration is OK */
  return IOE16_OK; 
}

/**
  * @brief  Configures The selected IO pins in interrupt mode.
  * @param  IOE16_ITSRC_Source: the source of the interrupts. Could be one or a 
  *         combination of IT pins.
  * @retval IOE16_OK: if configuration is OK. Other value if error.
  */
uint8_t IOE16_ITConfig(uint32_t IOE16_ITSRC_Source)
{   
  /* Configure the GPIO Interrupt polarity */
  IOE16_ITOutConfig(IOE16_Polarity_Low);     
   
  /* Enable the Global interrupt */  
  IOE16_GITCmd(ENABLE);  
    
  /* Read the GPIO interrupt status to clear all pending bits if any */
  IOE16_I2C_ReadDeviceRegister(IOE16_REG_ISGPIOR_LSB);   

  /* Enable the IO pins to generate interrupt */
  IOE16_IOITConfig(IOE16_ITSRC_Source, ENABLE);

  /* Put GPIOs in input mode */
  IOE16_IOPinConfig(IOE16_ITSRC_Source, Direction_IN);

  /* Configure the Interrupt line as EXTI source */
  IOE16_EXTI_Config();    
  
  /* If all OK return IOE16_OK */
  return IOE16_OK;
}

/**
  * @brief  Sets the value of the selected IO pins.
  * @param  IO_Pin: The output pin to be set or reset. This parameter could be any
  *                 combination of the following values:
  *   @arg  IO_Pin_x: where x can be from 0 to 15. 
  * @param  NewState: The value to be set. This parameter can be one of the
  *                   following values: IOE16_BitSet or IOE16_BitReset. 
  * @retval IOE16_OK or PARAM_ERROR
  */
uint8_t IOE16_SetIOPin(uint16_t IO_Pin, IOE16_BitValue_TypeDef NewState)
{
  uint16_t tmp = 0;
  
  /* Get the registers values */
  tmp = IOE16_I2C_ReadDeviceRegister(IOE16_REG_GPMR_LSB);
  tmp |= (((uint16_t)IOE16_I2C_ReadDeviceRegister(IOE16_REG_GPMR_MSB))<<8);
  
  if (NewState != IOE16_BitReset)
  {
    tmp |= (uint16_t)IO_Pin;
  }  
  else 
  {
    tmp = ~(uint16_t)IO_Pin;
  }
    
  /* Write the registers new values */
  IOE16_I2C_WriteDeviceRegister(IOE16_REG_GPSR_LSB, (uint8_t)(tmp & IO16_Pin_ALL_LSB));
  IOE16_I2C_WriteDeviceRegister(IOE16_REG_GPSR_MSB, (uint8_t)((tmp & IO16_Pin_ALL_MSB)>>8));
    
  /* If pins are correctly set return OK */
  return IOE16_OK;
}


/**
  * @brief  Monitors the status of the selected IO pins.
  * @param  IO_Pin: The input pin to be read. This parameter could be any
  *                 of the following values:
  *   @arg  IO_Pin_x: where x can be from 0 to 15. 
  * @retval Status of the selected input IO pin. Could be BitSet or BitReset.
  */
uint8_t IOE16_MonitorIOPin(uint16_t IO_Pin)
{
  uint8_t tmp = 0;
  
  if(IO_Pin & IO16_Pin_ALL_LSB)
  {
    /* Get LSB Pins status */
    tmp = IOE16_I2C_ReadDeviceRegister(IOE16_REG_GPMR_LSB);
    
    /* Return status */
    if ((tmp & (uint8_t)(IO_Pin & IO16_Pin_ALL_LSB)) != 0)
    {
      return IOE16_BitSet;
    }  
    else 
    {
      return IOE16_BitReset;
    }
  }
  else if(IO_Pin & IO16_Pin_ALL_MSB)
  {
    /* Get MSB Pins status */
    tmp = IOE16_I2C_ReadDeviceRegister(IOE16_REG_GPMR_MSB);
    
    /* Return status */
    if ((tmp & (uint8_t)((IO_Pin & IO16_Pin_ALL_MSB)>>8)) != 0)
    {
      return IOE16_BitSet;
    }  
    else 
    {
      return IOE16_BitReset;
    } 
  }

  /* Default return */
  return IOE16_PARAM_ERROR; 
  
}

/**
  * @brief  Enables or disables polarity inversion of the selected IO pins.
  * @param  IO_Pin: The input pin to be read. This parameter could be any
  *                 of the following values:
  *   @arg  IO_Pin_x: where x can be from 0 to 15.
  * @param  NewState: The value to be set. This parameter can be one of the
  *         following values: BitSet or BitReset. See IOE16_BitVal_TypeDef.
  * @retval Status of the selected input IO pin. Could be IOE16_BitSet or IOE16_BitReset.
  */
uint8_t IOE16_Polarity_Inversion(uint16_t IO_Pin, IOE16_BitValue_TypeDef NewState)
{
  uint16_t tmp = 0;
  
  /* Get the registers values */
  tmp = IOE16_I2C_ReadDeviceRegister(IOE16_REG_GPPIR_LSB);
  tmp |= (((uint16_t)IOE16_I2C_ReadDeviceRegister(IOE16_REG_GPPIR_MSB))<<8);

  /* Apply the bit value to the selected pin */
  if (NewState == IOE16_BitReset)
  {
    tmp &= ~ (uint16_t)IO_Pin;
  }
  else
  {
    tmp |= (uint16_t)IO_Pin;
  }
    
  /* Set register value */
  IOE16_I2C_WriteDeviceRegister(IOE16_REG_GPPIR_LSB, (uint8_t)tmp); 
  IOE16_I2C_WriteDeviceRegister(IOE16_REG_GPPIR_MSB, (uint8_t)(tmp >> 8)); 
  
  return IOE16_OK;
  
}

/**
  * @brief  Returns the current Joystick status.
  * @param  None
  * @retval The code of the Joystick key pressed: 
  *   @arg  JOY_NONE
  *   @arg  JOY_SEL
  *   @arg  JOY_DOWN
  *   @arg  JOY_LEFT
  *   @arg  JOY_RIGHT
  *   @arg  JOY_UP
  */
JOYState_TypeDef IOE16_JoyStickGetState(void)
{
  __IO uint8_t tmp = 0;
  
  /* Read the status of all pins */
  tmp = IOE16_I2C_ReadDeviceRegister(IOE16_REG_GPMR_MSB);
  tmp = ~(tmp & (uint8_t)(JOY_IO16_PINS>>8));
  
  /* Check the pressed keys */
  if ((tmp & (uint8_t)(JOY_IO16_NONE>>8)) == JOY_NONE)
  {
    return (JOYState_TypeDef) JOY_NONE;
  }
  else if (tmp & (uint8_t)(JOY_IO16_SEL>>8))
  {
    return (JOYState_TypeDef) JOY_SEL;
  }
  else if (tmp & (uint8_t)(JOY_IO16_DOWN>>8))
  {
    return (JOYState_TypeDef) JOY_DOWN;
  }
  else if (tmp & (uint8_t)(JOY_IO16_LEFT>>8))
  {
    return (JOYState_TypeDef) JOY_LEFT;
  }
  else if (tmp & (uint8_t)(JOY_IO16_RIGHT>>8))
  {
    return (JOYState_TypeDef) JOY_RIGHT;
  }
  else if (tmp & (uint8_t)(JOY_IO16_UP>>8))
  {
    return (JOYState_TypeDef) JOY_UP;
  }
  else
  { 
    return (JOYState_TypeDef) JOY_NONE;
  }
}

/**
  * @brief  Gets the IT status register value and clears all the pending bits.            
  * @retval Status register value.
  */
uint16_t IOE16_GetITStatus(void)
{
  uint16_t tmpsr, tmpsr2 = 0;
  
  /* Gets the IT status registers values */
  tmpsr = (uint16_t)IOE16_I2C_ReadDeviceRegister(IOE16_REG_ISGPIOR_LSB);
  tmpsr2 = (uint16_t)IOE16_I2C_ReadDeviceRegister(IOE16_REG_ISGPIOR_MSB);
  tmpsr2 = (tmpsr2 << 8);
  
  /* returns the status register value */
  return (tmpsr | tmpsr2);
}

/**
  * @brief  Checks if the device is correctly configured and communicates correctly 
  *         on the I2C bus.
  * @param  None.          
  * @retval IOE16_OK if IOE is operational. Other value if failure.
  */
uint8_t IOE16_IsOperational(void)
{
  /* Return Error if the ID is not correct */
  if( IOE16_ReadID() != (uint16_t)STMPE1600_ID )
  {
    /* Check if a Timeout occurred */
    if (IOE16_TimeOut == 0)
    {
      return(IOE16_TimeoutUserCallback());
    }
    else
    {
      return IOE16_FAILURE; /* ID is not Correct */
    }
  } 
  else 
  {
    return IOE16_OK; /* ID is correct */
  }
}

/**
  * @brief  Resets the IO Expander by Software (SYS_CTRL, RESET bit).
  * @param  None.
  * @retval IOE16_OK: if all initializations are OK. Other value if error.
  */
uint8_t IOE16_Reset(void)
{
  /* Power Down the IO_Expander */
  IOE16_I2C_WriteDeviceRegister(IOE16_REG_SYS_CTRL, 0x80);

  /* wait for a delay to ensure registers erasing */
  _delay_(2); 
  
  /* Power On the IOE after the power off => all registers are reinitialized*/
  IOE16_I2C_WriteDeviceRegister(IOE16_REG_SYS_CTRL, 0x00);
  
  /* If all OK return IOE16_OK */
  return IOE16_OK;    
}

/**
  * @brief  Reads the IO Expander device's ID.
  * @param  None.
  * @retval The Device ID (two bytes).
  */
uint16_t IOE16_ReadID(void)
{
  uint16_t tmp = 0;

  /* Read the device ID  */
  tmp = IOE16_I2C_ReadDeviceRegister(1);
  tmp = (uint32_t)(tmp << 8);
  tmp |= (uint32_t)IOE16_I2C_ReadDeviceRegister(0);
  
  /* Return the ID */
  return (uint16_t)tmp;
}

/**
  * @brief  Configures the Interrupt line Polarity
  * @param  Polarity: could be one of these values; 
  *   @arg  Polarity_Low: Interrupt line is active Low/Falling edge      
  *   @arg  Polarity_High: Interrupt line is active High/Rising edge               
  * @retval IOE16_OK: if configuration is OK. Other value if error.
  */
uint8_t IOE16_ITOutConfig(uint8_t Polarity)
{
  uint8_t tmp = 0;
  
  /*  Get the system control register value */ 
  tmp = IOE16_I2C_ReadDeviceRegister(IOE16_REG_SYS_CTRL);
  
  /* Mask the polarity bit */
  tmp &= ~(uint8_t)0x01;
    
  /* Modify the Interrupt Output line configuration */
  tmp |= (uint8_t)Polarity;
  
  /* Set the register */
  IOE16_I2C_WriteDeviceRegister(IOE16_REG_SYS_CTRL, tmp);
  
  /* If all OK return IOE16_OK */
  return IOE16_OK;  
}

/**
  * @brief  Configures the selected pin direction (to be an input or an output)    
  * @param  IO_Pin: This parameter could be any combination of the following values:
  *   @arg  IO_Pin_x: where x can be from 0 to 15.  
  * @param  Direction: could be Direction_IN or Direction_OUT.      
  * @retval IOE16_OK: if all initializations are OK. Other value if error.
  */
uint8_t IOE16_IOPinConfig(uint16_t IO_Pin, uint8_t Direction)
{
  uint16_t tmp = 0; 
    
  /* Get all pins direction */
  tmp = (uint16_t)IOE16_I2C_ReadDeviceRegister(IOE16_REG_GPDR_LSB);
  tmp |= (((uint16_t)IOE16_I2C_ReadDeviceRegister(IOE16_REG_GPDR_MSB))>>8); 
  
  if (Direction != Direction_IN)
  {
    tmp |= (uint16_t)IO_Pin;
  }  
  else 
  {
    tmp &= ~(uint16_t)IO_Pin;
  }
    
  /* Write the registers new values */
  IOE16_I2C_WriteDeviceRegister(IOE16_REG_GPDR_LSB, (uint8_t)(tmp & IO16_Pin_ALL_LSB));
  IOE16_I2C_WriteDeviceRegister(IOE16_REG_GPDR_MSB, (uint8_t)((tmp & IO16_Pin_ALL_MSB)>>8));
  
  /* If all OK return IOE16_OK */
  return IOE16_OK;      
}

/**
  * @brief  Enables or disables the GPIO Global interrupt.
  * @param  NewState: could be ENABLE or DISABLE.        
  * @retval IOE16_OK: if all initializations are OK. Other value if error.
  */
uint8_t IOE16_GITCmd(FunctionalState NewState)
{
  uint8_t tmp = 0;
  
  /* Read the system control register  */
  tmp = IOE16_I2C_ReadDeviceRegister(IOE16_REG_SYS_CTRL);
  
  if (NewState != DISABLE)
  {
    /* Set the global interrupt to be Enabled */    
    tmp |= (uint8_t)IOE16_IT_EN;
  }
  else
  {
    /* Set the global interrupts to be Disabled */    
    tmp &= ~(uint8_t)IOE16_IT_EN;
  }  
  
  /* Write Back the system control register */
  IOE16_I2C_WriteDeviceRegister(IOE16_REG_SYS_CTRL, tmp);

  /* If all OK return IOE16_OK */
  return IOE16_OK;     
}

/**
  * @brief  Configures the selected IO pins to generate an interrupt or not.
  * @param  IO_IT: The IO interrupt to be configured. This parameter could be any
  *         combination of the following values:
  *   @arg  IO_IT_x: where x can be from 0 to 15.
  * @param  NewState: could be ENABLE or DISABLE.  
  * @retval IOE16_OK: if all initializations are OK. Other value if error.
  */
uint8_t IOE16_IOITConfig(uint16_t IO_IT, FunctionalState NewState)
{
  uint16_t tmp = 0;
 
  /* Get the registers values */
  tmp = (uint16_t)IOE16_I2C_ReadDeviceRegister(IOE16_REG_IEGPIOR_LSB);
  tmp |= ((uint16_t)IOE16_I2C_ReadDeviceRegister(IOE16_REG_IEGPIOR_MSB)>>8);
  
  if (NewState != DISABLE)
  {
    /* Set the interrupts to be Enabled */    
    tmp |= (uint16_t)IO_IT;
  }
  else
  {
    /* Set the interrupts to be Disabled */    
    tmp &= ~(uint16_t)IO_IT;
  }
  
  /* Write the registers new values */
  IOE16_I2C_WriteDeviceRegister(IOE16_REG_IEGPIOR_LSB, (uint8_t)(tmp & ALL_IT_LSB));
  IOE16_I2C_WriteDeviceRegister(IOE16_REG_IEGPIOR_MSB, (uint8_t)((tmp & ALL_IT_MSB)>>8));
  
  /* If all OK return IOE16_OK */
  return IOE16_OK;   
}

/**
  * @brief  Writes a value in a register of the device through I2C without DMA.
  * @param  RegisterAddr: The target register address
  * @param  RegisterValue: The target register value to be written 
  * @retval IOE16_OK if all initializations are OK. Other value if error.     
  */
uint8_t IOE16_I2C_WriteDeviceRegister(uint8_t RegisterAddr, uint8_t RegisterValue)
{
  /* Begin the configuration sequence */
  I2C_GenerateSTART(IOE16_I2C, ENABLE);

  /* Test on EV5 and clear it */
  IOE16_TimeOut = TIMEOUT_MAX;
  while (!I2C_GetFlagStatus(IOE16_I2C, I2C_FLAG_SB))
  {
    if (IOE16_TimeOut-- == 0) return(IOE16_TimeoutUserCallback());
  }

  /* Transmit the slave address and enable writing operation */
  I2C_Send7bitAddress(IOE16_I2C, IOE16_ADDR, I2C_Direction_Transmitter);
  
  /* Test on EV6 and clear it */
  IOE16_TimeOut = TIMEOUT_MAX;  
  while (!I2C_GetFlagStatus(IOE16_I2C, I2C_FLAG_ADDR))
  {
    if (IOE16_TimeOut-- == 0) return(IOE16_TimeoutUserCallback());
  }
  
  /* Read status register 2 to clear ADDR flag */
  IOE16_I2C->SR2;
  
  /* Test on EV8_1 and clear it */
  IOE16_TimeOut = TIMEOUT_MAX;
  while (!I2C_GetFlagStatus(IOE16_I2C, I2C_FLAG_TXE))
  {
    if (IOE16_TimeOut-- == 0) return(IOE16_TimeoutUserCallback());
  }
  
  /* Transmit the first address for r/w operations */
  I2C_SendData(IOE16_I2C, RegisterAddr);
  
  /* Test on EV8 and clear it */
  IOE16_TimeOut = TIMEOUT_MAX;
  while (!I2C_GetFlagStatus(IOE16_I2C, I2C_FLAG_TXE))
  {
    if (IOE16_TimeOut-- == 0) return(IOE16_TimeoutUserCallback());
  }
  
  /* Prepare the register value to be sent */
  I2C_SendData(IOE16_I2C, RegisterValue);
  
  /* Test on EV8_2 and clear it */
  IOE16_TimeOut = TIMEOUT_MAX;
  while ((!I2C_GetFlagStatus(IOE16_I2C, I2C_FLAG_TXE)) || (!I2C_GetFlagStatus(IOE16_I2C, I2C_FLAG_BTF)))
  {
    if (IOE16_TimeOut-- == 0) return(IOE16_TimeoutUserCallback());
  }
  
  /* End the configuration sequence */
  I2C_GenerateSTOP(IOE16_I2C, ENABLE);
  
  return IOE16_OK;
}

/**
  * @brief  Reads a register of the device through I2C without DMA.
  * @param  RegisterAddr: The target register address (between 00x and 0x24)
  * @retval The value of the read register (0xAA if Timeout occurred)   
  */ 
uint8_t IOE16_I2C_ReadDeviceRegister(uint8_t RegisterAddr)
{
  uint8_t tmp = 0;
  
  /* Enable the I2C peripheral */
  I2C_GenerateSTART(IOE16_I2C, ENABLE);
 
  /* Test on EV5 and clear it */
  IOE16_TimeOut = TIMEOUT_MAX;
  while (!I2C_GetFlagStatus(IOE16_I2C, I2C_FLAG_SB))
  {
    if (IOE16_TimeOut-- == 0) return(IOE16_TimeoutUserCallback());
  }
  /* Disable Acknowledgement */
  I2C_AcknowledgeConfig(IOE16_I2C, DISABLE);
  
  /* Transmit the slave address and enable writing operation */
  I2C_Send7bitAddress(IOE16_I2C, IOE16_ADDR, I2C_Direction_Transmitter);
  
  /* Test on EV6 and clear it */
  IOE16_TimeOut = TIMEOUT_MAX;  
  while (!I2C_GetFlagStatus(IOE16_I2C, I2C_FLAG_ADDR))
  {
    if (IOE16_TimeOut-- == 0) return(IOE16_TimeoutUserCallback());
  }
  
  /* Read status register 2 to clear ADDR flag */
  IOE16_I2C->SR2;
  
  /* Test on EV8 and clear it */
  IOE16_TimeOut = TIMEOUT_MAX;
  while (!I2C_GetFlagStatus(IOE16_I2C, I2C_FLAG_TXE))
  {
    if (IOE16_TimeOut-- == 0) return(IOE16_TimeoutUserCallback());
  }
  
  /* Transmit the first address for r/w operations */
  I2C_SendData(IOE16_I2C, RegisterAddr);
  
  /* Test on EV8 and clear it */
  IOE16_TimeOut = TIMEOUT_MAX;
  while ((!I2C_GetFlagStatus(IOE16_I2C, I2C_FLAG_TXE)) || (!I2C_GetFlagStatus(IOE16_I2C, I2C_FLAG_BTF)))
  {
    if (IOE16_TimeOut-- == 0) return(IOE16_TimeoutUserCallback());
  }
  
  /* Regenerate a start condition */
  I2C_GenerateSTART(IOE16_I2C, ENABLE);
  
  /* Test on EV5 and clear it */
  IOE16_TimeOut = TIMEOUT_MAX;
  while (!I2C_GetFlagStatus(IOE16_I2C, I2C_FLAG_SB))
  {
    if (IOE16_TimeOut-- == 0) return(IOE16_TimeoutUserCallback());
  }
  
  /* Transmit the slave address and enable writing operation */
  I2C_Send7bitAddress(IOE16_I2C, IOE16_ADDR, I2C_Direction_Receiver);
  
  /* Test on EV6 and clear it */
  IOE16_TimeOut = TIMEOUT_MAX;
  while (!I2C_GetFlagStatus(IOE16_I2C, I2C_FLAG_ADDR))
  {
    if (IOE16_TimeOut-- == 0) return(IOE16_TimeoutUserCallback());
  }
  
  /* Read status register 2 to clear ADDR flag */
  IOE16_I2C->SR2;
  
  /* Test on EV7 and clear it */
  IOE16_TimeOut = TIMEOUT_MAX;
  while (!I2C_GetFlagStatus(IOE16_I2C, I2C_FLAG_RXNE))
  {
    if (IOE16_TimeOut-- == 0) return(IOE16_TimeoutUserCallback());
  }
  
  /* End the configuration sequence */
  I2C_GenerateSTOP(IOE16_I2C, ENABLE);
  
  /* Load the register value */
  tmp = I2C_ReceiveData(IOE16_I2C);
  
  /* Enable Acknowledgement */
  I2C_AcknowledgeConfig(IOE16_I2C, ENABLE);
  
  /* Return the read value */
  return tmp;

}

/**
  * @brief  Reads a buffer of 2 bytes from the device registers.
  * @param  RegisterAddr: The target register address (between 00x and 0x24)
  * @retval The data in the buffer containing the two returned bytes (in halfword).  
  */
uint16_t IOE16_I2C_ReadDataBuffer(uint32_t RegisterAddr)
{
  uint8_t IOE16_BufferRX[2] = {0x00, 0x00};  
  
  /* Enable the I2C peripheral */
  I2C_GenerateSTART(IOE16_I2C, ENABLE);
 
  /* Test on EV5 and clear it */
  IOE16_TimeOut = TIMEOUT_MAX;
  while (!I2C_GetFlagStatus(IOE16_I2C, I2C_FLAG_SB))
  {
    if (IOE16_TimeOut-- == 0) return(IOE16_TimeoutUserCallback());
  }
   
  /* Send device address for write */
  I2C_Send7bitAddress(IOE16_I2C, IOE16_ADDR, I2C_Direction_Transmitter);
  
  /* Test on EV6 and clear it */
  IOE16_TimeOut = TIMEOUT_MAX;  
  while (!I2C_GetFlagStatus(IOE16_I2C, I2C_FLAG_ADDR))
  {
    if (IOE16_TimeOut-- == 0) return(IOE16_TimeoutUserCallback());
  }
  
  /* Read status register 2 to clear ADDR flag */
  IOE16_I2C->SR2;
  
  /* Test on EV8 and clear it */
  IOE16_TimeOut = TIMEOUT_MAX;
  while (!I2C_GetFlagStatus(IOE16_I2C, I2C_FLAG_TXE))
  {
    if (IOE16_TimeOut-- == 0) return(IOE16_TimeoutUserCallback());
  }
  
  /* Send the device's internal address to write to */
  I2C_SendData(IOE16_I2C, RegisterAddr);  
    
  /* Send START condition a second time */  
  I2C_GenerateSTART(IOE16_I2C, ENABLE);
  
  /* Test on EV5 and clear it */
  IOE16_TimeOut = TIMEOUT_MAX;
  while (!I2C_GetFlagStatus(IOE16_I2C, I2C_FLAG_SB))
  {
    if (IOE16_TimeOut-- == 0) return(IOE16_TimeoutUserCallback());
  }
  
  /* Send IO Expander address for read */
  I2C_Send7bitAddress(IOE16_I2C, IOE16_ADDR, I2C_Direction_Receiver);
  
  /* Test on EV6 and clear it */
  IOE16_TimeOut = TIMEOUT_MAX;
  while (!I2C_GetFlagStatus(IOE16_I2C, I2C_FLAG_ADDR))
  {
    if (IOE16_TimeOut-- == 0) return(IOE16_TimeoutUserCallback());
  }
 
  /* Disable Acknowledgement and set Pos bit */
  I2C_AcknowledgeConfig(IOE16_I2C, DISABLE);       
  I2C_NACKPositionConfig(IOE16_I2C, I2C_NACKPosition_Next);
  
  /* Read status register 2 to clear ADDR flag */
  IOE16_I2C->SR2;

  /* Test on EV7 and clear it */
  IOE16_TimeOut = TIMEOUT_MAX;
  while (!I2C_GetFlagStatus(IOE16_I2C, I2C_FLAG_BTF))
  {
    if (IOE16_TimeOut-- == 0) return(IOE16_TimeoutUserCallback());
  }
 
  /* Send STOP Condition */
  I2C_GenerateSTOP(IOE16_I2C, ENABLE);
   
  /* Read the first byte from the IO Expander */
  IOE16_BufferRX[1] = I2C_ReceiveData(IOE16_I2C);
    
  /* Read the second byte from the IO Expander */
  IOE16_BufferRX[0] = I2C_ReceiveData(IOE16_I2C);
                                         
  /* Enable Acknowledgement and reset POS bit to be ready for another reception */
  I2C_AcknowledgeConfig(IOE16_I2C, ENABLE);
  I2C_NACKPositionConfig(IOE16_I2C, I2C_NACKPosition_Current);
   
  /* return the data */
  return ((uint16_t) IOE16_BufferRX[0] | ((uint16_t)IOE16_BufferRX[1]<< 8));

}

/**
  * @brief  Initializes the GPIO pins used by the IO expander.
  * @param  None
  * @retval None
  */
static void IOE16_GPIO_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  
  /* Enable I2C, I2C GPIO_PORT & Alternate Function clocks */
  RCC_APB1PeriphClockCmd(IOE16_I2C_CLK, ENABLE);
  RCC_AHB1PeriphClockCmd(IOE16_I2C_SCL_GPIO_CLK | IOE16_I2C_SDA_GPIO_CLK |
                         IOE16_IT_GPIO_CLK, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
  
  /* If the I2C peripheral is already enabled, don't reconfigure it */
  if ((IOE16_I2C->CR1 & I2C_CR1_PE) == 0)
  {
    /* Reset I2C */
    RCC_APB1PeriphResetCmd(IOE16_I2C_CLK, ENABLE);
  
    /* Release reset signal of I2C */
    RCC_APB1PeriphResetCmd(IOE16_I2C_CLK, DISABLE); 
    
    /* Connect PXx to I2C_SCL*/
    GPIO_PinAFConfig(IOE16_I2C_SCL_GPIO_PORT, IOE16_I2C_SCL_SOURCE, IOE16_I2C_SCL_AF);
  
    /* Connect PXx to I2C_SDA*/
    GPIO_PinAFConfig(IOE16_I2C_SDA_GPIO_PORT, IOE16_I2C_SDA_SOURCE, IOE16_I2C_SDA_AF); 
    
    /* I2C SCL and SDA pins configuration */
    GPIO_InitStructure.GPIO_Pin = IOE16_I2C_SCL_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
    GPIO_Init(IOE16_I2C_SCL_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = IOE16_I2C_SDA_PIN;
    GPIO_Init(IOE16_I2C_SDA_GPIO_PORT, &GPIO_InitStructure);
  }
}

/**
  * @brief  Configure the I2C Peripheral used to communicate with IO_Expanders.
  * @param  None
  * @retval None
  */
static void IOE16_I2C_Config(void)
{
  I2C_InitTypeDef I2C_InitStructure;

  /* If the I2C peripheral is already enabled, don't reconfigure it */
  if ((IOE16_I2C->CR1 & I2C_CR1_PE) == 0)
  {   
    /* IOE16_I2C configuration */
    I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
    I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
    I2C_InitStructure.I2C_OwnAddress1 = 0;
    I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
    I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_InitStructure.I2C_ClockSpeed = I2C_SPEED;
  
    /* Initialize the I2C peripheral */
    I2C_Init(IOE16_I2C, &I2C_InitStructure);
  
    /* Enable the I2C peripheral */
    I2C_Cmd(IOE16_I2C, ENABLE);
  }
}


/**
  * @brief  Configures the IO expander Interrupt line and GPIO in EXTI mode.
  * @param  None        
  * @retval None
  */
static void IOE16_EXTI_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  EXTI_InitTypeDef EXTI_InitStructure;
 
  /* Enable GPIO clock */
  RCC_AHB1PeriphClockCmd(IOE16_IT_GPIO_CLK, ENABLE);
  
  /* Enable SYSCFG clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

  /* Configure Button pin as input floating */
  GPIO_InitStructure.GPIO_Pin = IOE16_IT_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(IOE16_IT_GPIO_PORT, &GPIO_InitStructure);  
  
  /* Connect Button EXTI Line to Button GPIO Pin */
  SYSCFG_EXTILineConfig(IOE16_IT_EXTI_PORT_SOURCE, IOE16_IT_EXTI_PIN_SOURCE);  
  
  /* Configure Button EXTI line */
  EXTI_InitStructure.EXTI_Line = IOE16_IT_EXTI_LINE;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
  
  /* Enable and set Button EXTI Interrupt to the lowest priority */
  NVIC_InitStructure.NVIC_IRQChannel = IOE16_IT_EXTI_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

#ifndef USE_TIMEOUT_USER_CALLBACK 
/**
  * @brief  IOE16_TIMEOUT_UserCallback
  * @param  None
  * @retval 0
  */
uint8_t IOE16_TimeoutUserCallback(void)
{
  I2C_InitTypeDef I2C_InitStructure;

  I2C_GenerateSTOP(IOE16_I2C, ENABLE);
  I2C_SoftwareResetCmd(IOE16_I2C, ENABLE);
  I2C_SoftwareResetCmd(IOE16_I2C, DISABLE);
  
  IOE16_GPIO_Config();

  /* IOE16_I2C peripheral configuration */
  I2C_DeInit(IOE16_I2C);
  
  I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
  I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
  I2C_InitStructure.I2C_OwnAddress1 = 0x33;
  I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
  I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
  I2C_InitStructure.I2C_ClockSpeed = I2C_SPEED;
  
  /* Enable the I2C peripheral */
  I2C_Cmd(IOE16_I2C, ENABLE);  
  I2C_Init(IOE16_I2C, &I2C_InitStructure);
  
  return 0;
  
}
#endif /* !USE_TIMEOUT_USER_CALLBACK */

#ifndef USE_Delay
/**
  * @brief  Inserts a delay time.
  * @param  nCount: specifies the delay time length.
  * @retval None
  */
static void delay(__IO uint32_t nCount)
{
  __IO uint32_t index = 0; 
  for(index = (100000 * nCount); index != 0; index--)
  {
  }
}
#endif /* USE_Delay*/
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
