/**
  ******************************************************************************
  * @file    stm324x9i_eval_lcd.h
  * @author  MCD Application Team
  * @version V1.0.3
  * @date    13-November-2013
  * @brief   This file contains all the functions prototypes for the stm324x9i_eval_lcd.c
  *          driver.
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
#ifndef __STM324x9I_EVAL_LCD_H
#define __STM324x9I_EVAL_LCD_H

#ifdef __cplusplus
 extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/
#include "stm324x9i_eval.h"
#include "stm324x9i_eval_fmc_sdram.h"
#include "../Common/fonts.h"
   
/** @addtogroup Utilities
  * @{
  */

/** @addtogroup STM32_EVAL
  * @{
  */ 

/** @addtogroup STM324x9I_EVAL
  * @{
  */
    
/** @addtogroup STM324x9I_EVAL_LCD
  * @{
  */ 


/** @defgroup STM324x9I_EVAL_LCD_Exported_Types
  * @{
  */
typedef struct 
{
  int16_t X;
  int16_t Y;
} Point, * pPoint;   
/**
  * @}
  */ 

/** @defgroup STM324x9I_EVAL_LCD_Exported_Constants
  * @{
  */ 
  
/**
 * @brief Used LCD  
 */

typedef enum 
{
    USE_LCD_AM640480        = 0,      /*!<  LCD Ampire 640x480 */
    USE_LCD_AM480272        = 1,      /*!<  LCD Ampire 480x272 */
} LCD_Used;

/** 
  * @brief  LCD Control pin  
  */ 
#define LCD_NCS_PIN             GPIO_Pin_4                  
#define LCD_NCS_GPIO_PORT       GPIOD                     
#define LCD_NCS_GPIO_CLK        RCC_AHB1Periph_GPIOD

/** 
  * @brief  LCD color  
  */ 
#define LCD_COLOR_WHITE          0xFFFF
#define LCD_COLOR_BLACK          0x0000
#define LCD_COLOR_GREY           0xF7DE
#define LCD_COLOR_BLUE           0x001F
#define LCD_COLOR_BLUE2          0x051F
#define LCD_COLOR_RED            0xF800
#define LCD_COLOR_MAGENTA        0xF81F
#define LCD_COLOR_GREEN          0x07E0
#define LCD_COLOR_CYAN           0x7FFF
#define LCD_COLOR_YELLOW         0xFFE0

/** 
  * @brief  LCD Lines depending on the chosen fonts.  
  */
#define LCD_LINE_0               LINE(0)
#define LCD_LINE_1               LINE(1)
#define LCD_LINE_2               LINE(2)
#define LCD_LINE_3               LINE(3)
#define LCD_LINE_4               LINE(4)
#define LCD_LINE_5               LINE(5)
#define LCD_LINE_6               LINE(6)
#define LCD_LINE_7               LINE(7)
#define LCD_LINE_8               LINE(8)
#define LCD_LINE_9               LINE(9)
#define LCD_LINE_10              LINE(10)
#define LCD_LINE_11              LINE(11)
#define LCD_LINE_12              LINE(12)
#define LCD_LINE_13              LINE(13)
#define LCD_LINE_14              LINE(14)
#define LCD_LINE_15              LINE(15)
#define LCD_LINE_16              LINE(16)
#define LCD_LINE_17              LINE(17)
#define LCD_LINE_18              LINE(18)
#define LCD_LINE_19              LINE(19)
#define LCD_LINE_20              LINE(20)
#define LCD_LINE_21              LINE(21)
#define LCD_LINE_22              LINE(22)
#define LCD_LINE_23              LINE(23)
#define LCD_LINE_24              LINE(24)
#define LCD_LINE_25              LINE(25)
#define LCD_LINE_26              LINE(26)
#define LCD_LINE_27              LINE(27)
#define LCD_LINE_28              LINE(28)
#define LCD_LINE_29              LINE(29)
#define LCD_LINE_30              LINE(30)
#define LCD_LINE_31              LINE(31)
#define LCD_LINE_32              LINE(32)
#define LCD_LINE_33              LINE(33)
#define LCD_LINE_34              LINE(34)
#define LCD_LINE_35              LINE(35)
#define LCD_LINE_36              LINE(36)
#define LCD_LINE_37              LINE(37)
#define LCD_LINE_38              LINE(38)
#define LCD_LINE_39              LINE(39)
#define LCD_LINE_40              LINE(40)
#define LCD_LINE_41              LINE(41)
#define LCD_LINE_42              LINE(42)
#define LCD_LINE_43              LINE(43)
#define LCD_LINE_44              LINE(44)
#define LCD_LINE_45              LINE(45)
#define LCD_LINE_46              LINE(46)
#define LCD_LINE_47              LINE(47)
#define LCD_LINE_48              LINE(48)
#define LCD_LINE_49              LINE(49)
#define LCD_LINE_50              LINE(50)
#define LCD_LINE_51              LINE(51)
#define LCD_LINE_52              LINE(52)
#define LCD_LINE_53              LINE(53)
#define LCD_LINE_54              LINE(54)
#define LCD_LINE_55              LINE(55)
#define LCD_LINE_56              LINE(56)
#define LCD_LINE_57              LINE(57)
#define LCD_LINE_58              LINE(58)
#define LCD_LINE_59              LINE(59)    
    
/** 
  * @brief LCD default font 
  */ 
#define LCD_DEFAULT_FONT         Font16x24

/** 
  * @brief  LCD Direction  
  */ 
#define LCD_DIR_HORIZONTAL       0x0000
#define LCD_DIR_VERTICAL         0x0001

/** 
  * @brief  LCD Layer  
  */ 
#define LCD_BACKGROUND_LAYER     0x0000
#define LCD_FOREGROUND_LAYER     0x0001


/** 
  * @brief  I2C port definitions  
  */ 
#define LCD_I2C                          I2C1
#define LCD_I2C_CLK                      RCC_APB1Periph_I2C1
#define LCD_I2C_SCL_PIN                  GPIO_Pin_6
#define LCD_I2C_SCL_GPIO_PORT            GPIOB
#define LCD_I2C_SCL_GPIO_CLK             RCC_AHB1Periph_GPIOB
#define LCD_I2C_SCL_SOURCE               GPIO_PinSource6
#define LCD_I2C_SCL_AF                   GPIO_AF_I2C1
#define LCD_I2C_SDA_PIN                  GPIO_Pin_9
#define LCD_I2C_SDA_GPIO_PORT            GPIOB
#define LCD_I2C_SDA_GPIO_CLK             RCC_AHB1Periph_GPIOB
#define LCD_I2C_SDA_SOURCE               GPIO_PinSource9
#define LCD_I2C_SDA_AF                   GPIO_AF_I2C1
#define LCD_I2C_DR                       ((uint32_t)0x40005410)    

/** 
  * @brief  The Touch panel connector for LCD panel addresses  
  */ 
#define LCD_ADDR                   0x80    

/**
  * @}
  */ 

/** @defgroup STM324x9I_EVAL_LCD_Exported_Macros
  * @{
  */
#define ASSEMBLE_RGB(R ,G, B)    ((((R)& 0xF8) << 8) | (((G) & 0xFC) << 3) | (((B) & 0xF8) >> 3)) 
/**
  * @}
  */ 

/** @defgroup STM324x9I_EVAL_LCD_Exported_Functions
  * @{
  */

void     LCD_DeInit(void);   
void     LCD_Init(void);
void     LCD_LayerInit(void);
void     LCD_SetLayer(uint32_t Layerx);
void     LCD_SetColors(uint16_t _TextColor, uint16_t _BackColor); 
void     LCD_GetColors(uint16_t *_TextColor, uint16_t *_BackColor);
void     LCD_SetTextColor(uint16_t Color);
void     LCD_SetBackColor(uint16_t Color);
void     LCD_SetTransparency(uint8_t transparency);
void     LCD_SetPixelFormat(uint32_t PixelFormat);
void     LCD_SetAddress(uint32_t Address);
void     LCD_ClearLine(uint16_t Line);
void     LCD_Clear(uint16_t Color);
uint32_t LCD_SetCursor(uint16_t Xpos, uint16_t Ypos);
void     LCD_SetColorKeying(uint32_t RGBValue);
void     LCD_ReSetColorKeying(void);
void     LCD_DrawChar(uint16_t Xpos, uint16_t Ypos, const uint16_t *c);
void     LCD_DisplayChar(uint16_t Line, uint16_t Column, uint8_t Ascii);
void     LCD_SetFont(sFONT *fonts);
sFONT *  LCD_GetFont(void);
void     LCD_DisplayStringLine(uint16_t Line, uint8_t *ptr);
void     LCD_SetDisplayWindow(uint16_t Xpos, uint16_t Ypos, uint16_t Height, uint16_t Width);
void     LCD_WindowModeDisable(void);
void     LCD_DrawLine(uint16_t Xpos, uint16_t Ypos, uint16_t Length, uint8_t Direction);
void     LCD_DrawRect(uint16_t Xpos, uint16_t Ypos, uint16_t Height, uint16_t Width);
void     LCD_DrawCircle(uint16_t Xpos, uint16_t Ypos, uint16_t Radius);
void     LCD_DrawEllipse(int Xpos, int Ypos, int Radius, int Radius2);
void     LCD_DrawFullEllipse(int Xpos, int Ypos, int Radius, int Radius2);
void     LCD_DrawMonoPict(const uint32_t *Pict);
void     LCD_WriteBMP(uint32_t BmpAddress);
void     LCD_DrawUniLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
void     LCD_DrawFullRect(uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height);
void     LCD_DrawFullCircle(uint16_t Xpos, uint16_t Ypos, uint16_t Radius);
void     LCD_PolyLine(pPoint Points, uint16_t PointCount);
void     LCD_PolyLineRelative(pPoint Points, uint16_t PointCount);
void     LCD_ClosedPolyLine(pPoint Points, uint16_t PointCount);
void     LCD_ClosedPolyLineRelative(pPoint Points, uint16_t PointCount);
void     LCD_FillPolyLine(pPoint Points, uint16_t PointCount);
void     LCD_Triangle(pPoint Points, uint16_t PointCount);
void     LCD_FillTriangle(uint16_t x1, uint16_t x2, uint16_t x3, uint16_t y1, uint16_t y2, uint16_t y3);

uint32_t LCD_GetPixelHeight(void);
uint32_t LCD_GetPixelWidth(void);
uint8_t  LCD_GetPosition(uint8_t* ReadBuffer);
uint8_t  LCD_TimeOutUserCallback(void);

void     LCD_CheckDevice(void);

void     LCD_DisplayOn(void);
void     LCD_DisplayOff(void);



/**
  * @}
  */    
#ifdef __cplusplus
}
#endif

#endif /* __STM324x9I_EVAL_LCD_H */
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

/*********************** (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
