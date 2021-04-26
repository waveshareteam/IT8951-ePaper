#ifndef _KEY_LED_H
#define _KEY_LED_H

#include <stdio.h>
#include "stm32f4xx.h"

#define USER_KEY_Port					GPIOF
#define USER_KEY_Pin					GPIO_Pin_10
#define USER_KEY_RCC_AHBPeriph			RCC_AHB1Periph_GPIOF
/****************************************************************/
#define WAKEUP_KEY_Port					    GPIOA
#define WAKEUP_KEY_Pin					    GPIO_Pin_0
#define WAKEUP_KEY_RCC_AHBPeriph		RCC_AHB1Periph_GPIOA
/****************************************************************/
#define JOY_A_KEY_Port					    GPIOG
#define JOY_A_KEY_Pin					      GPIO_Pin_2
#define JOY_A_KEY_RCC_AHBPeriph			RCC_AHB1Periph_GPIOG
 
#define JOY_B_KEY_Port					    GPIOG
#define JOY_B_KEY_Pin					      GPIO_Pin_3
#define JOY_B_KEY_RCC_AHBPeriph			RCC_AHB1Periph_GPIOG

#define JOY_C_KEY_Port					    GPIOG
#define JOY_C_KEY_Pin				        GPIO_Pin_9
#define JOY_C_KEY_RCC_AHBPeriph			RCC_AHB1Periph_GPIOG

#define JOY_D_KEY_Port					    GPIOG
#define JOY_D_KEY_Pin					      GPIO_Pin_10
#define JOY_D_KEY_RCC_AHBPeriph			RCC_AHB1Periph_GPIOG

#define JOY_CTR_KEY_Port				    GPIOD
#define JOY_CTR_KEY_Pin					    GPIO_Pin_13
#define JOY_CTR_KEY_RCC_AHBPeriph		RCC_AHB1Periph_GPIOD
/****************************************************************/
#define LED1_Port						        GPIOF
#define LED1_Pin						        GPIO_Pin_6
#define LED1_RCC_AHBPeriph				  RCC_AHB1Periph_GPIOF

#define LED2_Port						        GPIOF
#define LED2_Pin						        GPIO_Pin_7
#define LED2_RCC_AHBPeriph				  RCC_AHB1Periph_GPIOF

#define LED3_Port						        GPIOF
#define LED3_Pin						        GPIO_Pin_8
#define LED3_RCC_AHBPeriph				  RCC_AHB1Periph_GPIOF

#define LED4_Port						        GPIOF
#define LED4_Pin						        GPIO_Pin_9
#define LED4_RCC_AHBPeriph				  RCC_AHB1Periph_GPIOF


void JOYState_LED_GPIO_Init(void);
uint8_t Read_JOYState(void);
void Led_Toggle(uint8_t key);

#endif /*_KEY_LED_H*/
