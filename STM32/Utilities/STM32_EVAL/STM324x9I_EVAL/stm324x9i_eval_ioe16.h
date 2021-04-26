/**
  ******************************************************************************
  * @file    stm324x9i_eval_ioe16.h
  * @author  MCD Application Team
  * @version V1.0.3
  * @date    13-November-2013
  * @brief   This file contains all the functions prototypes for the
  *          stm324x9i_eval_ioe16.c driver.
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
#ifndef __STM324X9I_EVAL_IOE16_H
#define __STM324X9I_EVAL_IOE16_H

#ifdef __cplusplus
 extern "C" {
#endif   
   
/* Includes ------------------------------------------------------------------*/
#include "stm324x9i_eval.h"

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
  * @{
  */    


/** @defgroup STM324x9I_EVAL_IOE16_Exported_Types
  * @{
  */ 
  
/** 
  * @brief  Joystick State definitions  
  */ 
#ifndef __STM324X9I_EVAL_H

typedef enum 
{ 
  JOY_NONE = 0,
  JOY_SEL = 1,
  JOY_DOWN = 2,
  JOY_LEFT = 3,
  JOY_RIGHT = 4,
  JOY_UP = 5
} JOYState_TypeDef;
#endif /* __STM324X9I_EVAL_H */
 
/** 
  * @brief  IO_Expander Error codes  
  */ 
typedef enum
{
  IOE16_OK = 0,
  IOE16_FAILURE, 
  IOE16_TIMEOUT,
  IOE16_PARAM_ERROR,
  IOE16_NOT_OPERATIONAL
}IOE16_Status_TypDef;

/** 
  * @brief  IO bit values  
  */ 
typedef enum
{
  IOE16_BitReset = 0,
  IOE16_BitSet = 1
}IOE16_BitValue_TypeDef;


/**
  * @}
  */ 


/** @defgroup STM324x9I_EVAL_IOE16_Exported_Constants
  * @{
  */ 

/**
 * @brief Uncomment the line below if you want to use user timeout callback.
 *        Function prototypes is declared in this file but function body may be
 *        implemented into user application.  
 */
/* #define USE_TIMEOUT_USER_CALLBACK */

/**
 * @brief Uncomment the line below if you want to use user defined Delay function
 *        (for precise timing), otherwise default _delay_ function defined within
 *         this driver is used (less precise timing).  
 */
/* #define USE_Delay */

#ifdef USE_Delay
#include "main.h"
 
  #define _delay_     Delay   /* !< User can provide more timing precise _delay_ function
                                   (with 10ms time base), using SysTick for example */
#else
  #define _delay_     delay   /* !< Default _delay_ function with less precise timing */
#endif    

/*------------------------------------------------------------------------------
    Hardware Configuration 
------------------------------------------------------------------------------*/
/** 
  * @brief  I2C port definitions  
  */ 
#define IOE16_I2C                  I2C1
#define IOE16_I2C_CLK              RCC_APB1Periph_I2C1
#define IOE16_I2C_SCL_PIN          GPIO_Pin_6
#define IOE16_I2C_SCL_GPIO_PORT    GPIOB
#define IOE16_I2C_SCL_GPIO_CLK     RCC_AHB1Periph_GPIOB
#define IOE16_I2C_SCL_SOURCE       GPIO_PinSource6
#define IOE16_I2C_SCL_AF           GPIO_AF_I2C1
#define IOE16_I2C_SDA_PIN          GPIO_Pin_9
#define IOE16_I2C_SDA_GPIO_PORT    GPIOB
#define IOE16_I2C_SDA_GPIO_CLK     RCC_AHB1Periph_GPIOB
#define IOE16_I2C_SDA_SOURCE       GPIO_PinSource9
#define IOE16_I2C_SDA_AF           GPIO_AF_I2C1
#define IOE16_I2C_DR               ((uint32_t)0x40005410)

/** 
  * @brief  IO Expander Interrupt line on EXTI  
  */ 
#define IOE16_IT_PIN               GPIO_Pin_8
#define IOE16_IT_GPIO_PORT         GPIOI
#define IOE16_IT_GPIO_CLK          RCC_AHB1Periph_GPIOI
#define IOE16_IT_EXTI_PORT_SOURCE  EXTI_PortSourceGPIOI
#define IOE16_IT_EXTI_PIN_SOURCE   EXTI_PinSource8
#define IOE16_IT_EXTI_LINE         EXTI_Line8
#define IOE16_IT_EXTI_IRQn         EXTI9_5_IRQn   

/** 
  * @brief IO Expanders adresse and chip IDs  
  */ 
  
#define IOE16_ADDR                 0x84 
#define STMPE1600_ID               0x1600


/*------------------------------------------------------------------------------
    Functional and Interrupt Management
------------------------------------------------------------------------------*/

/** 
  * @brief  Interrupt enable  
  */ 
#define IOE16_IT_EN                0x04



/*------------------------------------------------------------------------------
    STMPE1600 device register definition
------------------------------------------------------------------------------*/
/** 
  * @brief  Identification registers  
  */ 
#define IOE16_REG_CHP_ID_LSB       0x00
#define IOE16_REG_CHP_ID_MSB       0x01
#define IOE16_REG_ID_VER           0x02

/** 
  * @brief  System Control Register  
  */ 
#define IOE16_REG_SYS_CTRL         0x03

/** 
  * @brief  Interrupt Control register  
  */ 
#define IOE16_REG_IEGPIOR_LSB      0x08
#define IOE16_REG_IEGPIOR_MSB      0x09
#define IOE16_REG_ISGPIOR_LSB      0x0A
#define IOE16_REG_ISGPIOR_MSB      0x0B


/** 
  * @brief  GPIO Registers  
  */ 

#define IOE16_REG_GPMR_LSB         0x10
#define IOE16_REG_GPMR_MSB         0x11
#define IOE16_REG_GPSR_LSB         0x12
#define IOE16_REG_GPSR_MSB         0x13
#define IOE16_REG_GPDR_LSB         0x14
#define IOE16_REG_GPDR_MSB         0x15
#define IOE16_REG_GPPIR_LSB        0x16
#define IOE16_REG_GPPIR_MSB        0x17


/*------------------------------------------------------------------------------
    Functions parameters defines
------------------------------------------------------------------------------*/


/**
  * @brief JOYSTICK Pins definition 
  */ 
#define JOY_IO16_SEL               IO16_Pin_14
#define JOY_IO16_DOWN              IO16_Pin_13
#define JOY_IO16_LEFT              IO16_Pin_12
#define JOY_IO16_RIGHT             IO16_Pin_11
#define JOY_IO16_UP                IO16_Pin_10
#define JOY_IO16_NONE              JOY_IO16_PINS
#define JOY_IO16_PINS              (IO16_Pin_10 | IO16_Pin_11 | IO16_Pin_12 | IO16_Pin_13 | IO16_Pin_14)

/** 
  * @brief  IO Pins  
  */ 
#define IO16_Pin_0                 0x0001
#define IO16_Pin_1                 0x0002
#define IO16_Pin_2                 0x0004
#define IO16_Pin_3                 0x0008
#define IO16_Pin_4                 0x0010
#define IO16_Pin_5                 0x0020
#define IO16_Pin_6                 0x0040
#define IO16_Pin_7                 0x0080

#define IO16_Pin_8                 0x0100
#define IO16_Pin_9                 0x0200
#define IO16_Pin_10                0x0400
#define IO16_Pin_11                0x0800
#define IO16_Pin_12                0x1000
#define IO16_Pin_13                0x2000
#define IO16_Pin_14                0x4000
#define IO16_Pin_15                0x8000

#define IO16_Pin_ALL_LSB           0x00FF
#define IO16_Pin_ALL_MSB           0xFF00

/** 
  * @brief  IO Pin directions  
  */ 
#define Direction_IN               0x00
#define Direction_OUT              0x01

/** 
  * @brief  Interrupt Line output parameters  
  */ 
#define IOE16_Polarity_Low         0x00
#define IOE16_Polarity_High        0x01


/** 
  * @brief IO Interrupts  
  */ 
#define IO16_IT_0                  0x0001
#define IO16_IT_1                  0x0002
#define IO16_IT_2                  0x0004
#define IO16_IT_3                  0x0008
#define IO16_IT_4                  0x0010
#define IO16_IT_5                  0x0020
#define IO16_IT_6                  0x0040
#define IO16_IT_7                  0x0080

#define IO16_IT_8                  0x0100
#define IO16_IT_9                  0x0200
#define IO16_IT_10                 0x0400
#define IO16_IT_11                 0x0800
#define IO16_IT_12                 0x1000
#define IO16_IT_13                 0x2000
#define IO16_IT_14                 0x4000
#define IO16_IT_15                 0x8000

#define ALL_IT_LSB                 0x00FF
#define ALL_IT_MSB                 0xFF00
#define IOE16_JOY_IT               (IO16_IT_10 | IO16_IT_11 | IO16_IT_12 | IO16_IT_13 | IO16_IT_14)
#define IOE16_TS_IT                IO16_IT_4

/** 
  * @brief  Edge detection value  
  */ 
#define EDGE_FALLING               0x01
#define EDGE_RISING                0x02

/**
  * @}
  */ 



/** @defgroup STM324x9I_EVAL_IOE16_Exported_Macros
  * @{
  */ 

/** @defgroup STM324x9I_EVAL_IOE16_Exported_Functions
  * @{
  */ 
  
/** 
  * @brief  Configuration and initialization functions  
  */
uint8_t IOE16_Config(void);
uint8_t IOE16_ITConfig(uint32_t IOE16_ITSRC_Source);

/** 
  * @brief IO pins control functions
  */
uint8_t          IOE16_SetIOPin(uint16_t IO_Pin, IOE16_BitValue_TypeDef BitVal);
uint8_t          IOE16_MonitorIOPin(uint16_t IO_Pin);
uint8_t          IOE16_Polarity_Inversion(uint16_t IO_Pin, IOE16_BitValue_TypeDef BitVal);
JOYState_TypeDef IOE16_JoyStickGetState(void);

/** 
  * @brief Interrupts Management functions
  */
uint16_t IOE16_GetITStatus(void);

/** 
  * @brief IO Expander Control functions
  */
uint8_t  IOE16_IsOperational(void);
uint8_t  IOE16_Reset(void);
uint16_t IOE16_ReadID(void);
uint8_t  IOE16_ITOutConfig(uint8_t Polarity);
uint8_t  IOE16_IOPinConfig(uint16_t IO_Pin, uint8_t Direction);
uint8_t  IOE16_GITCmd(FunctionalState NewState);
uint8_t  IOE16_IOITConfig(uint16_t IO_IT, FunctionalState NewState);

/** 
  * @brief Low Layer functions
  */
uint8_t  IOE16_I2C_WriteDeviceRegister(uint8_t RegisterAddr, uint8_t RegisterValue);
uint8_t  IOE16_I2C_ReadDeviceRegister(uint8_t RegisterAddr);
uint16_t IOE16_I2C_ReadDataBuffer(uint32_t RegisterAddr);

/** 
  * @brief  Timeout user callback function. This function is called when a timeout
  *         condition occurs during communication with IO Expander. Only prototype
  *         of this function is declared in IO Expander driver. Its implementation
  *         may be done into user application. This function may typically stop
  *         current operations and reset the I2C peripheral and IO Expander.
  *         To enable this function use uncomment the define USE_TIMEOUT_USER_CALLBACK
  *         at the top of this file.          
  */
uint8_t IOE16_TimeoutUserCallback(void);

#ifdef __cplusplus
}
#endif
#endif /* __STM324X9I_EVAL_IOE16_H */

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
