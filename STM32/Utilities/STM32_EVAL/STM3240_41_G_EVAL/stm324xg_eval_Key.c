/**
  ******************************************************************************
  * @file    stm324xg_eval_ioe.c
  * @author  MCD Application Team
  * @version V1.0.2
  * @date    05-March-2012
  * @brief   This file provides a set of functions needed to manage the STMPE811
  *          IO Expander devices mounted on STM324xG-EVAL evaluation board.
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

/* Includes ------------------------------------------------------------------*/
#include "stm324xg_eval_Key.h"

void Key_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	/*--------------------------------------------------------------------------------
	
	Key GPIO Config
	
	--------------------------------------------------------------------------------*/
	/* Periph clock enable */
	RCC_AHB1PeriphClockCmd(JOY_A_KEY_RCC_AHBPeriph | JOY_B_KEY_RCC_AHBPeriph |  
						   JOY_C_KEY_RCC_AHBPeriph | JOY_D_KEY_RCC_AHBPeriph| JOY_CTR_KEY_RCC_AHBPeriph, ENABLE);
	
	/* Configure Input pushpull mode */
	

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;

	GPIO_InitStructure.GPIO_Pin = JOY_A_KEY_Pin;
	GPIO_Init(JOY_A_KEY_Port, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = JOY_B_KEY_Pin;
	GPIO_Init(JOY_B_KEY_Port, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = JOY_C_KEY_Pin;
	GPIO_Init(JOY_C_KEY_Port, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = JOY_D_KEY_Pin;
	GPIO_Init(JOY_D_KEY_Port, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = JOY_CTR_KEY_Pin;
	GPIO_Init(JOY_CTR_KEY_Port, &GPIO_InitStructure);
}

__IO JOYState_TypeDef keydone = JOY_NONE;

JOYState_TypeDef Key_ReadIOPin(void)
{
	if(keydone == JOY_NONE)
	{ 
		if(!GPIO_ReadInputDataBit(JOY_A_KEY_Port,JOY_A_KEY_Pin))
		{
			keydone = JOY_LEFT;
			return JOY_NONE;
		}   
		else if(!GPIO_ReadInputDataBit(JOY_B_KEY_Port,JOY_B_KEY_Pin))
		{
			keydone = JOY_UP;
			return JOY_NONE;
		}
		else if(!GPIO_ReadInputDataBit(JOY_C_KEY_Port,JOY_C_KEY_Pin))
		{	
			keydone = JOY_DOWN;
			return JOY_NONE;
		}
		else if(!GPIO_ReadInputDataBit(JOY_D_KEY_Port,JOY_D_KEY_Pin))
		{
			keydone = JOY_RIGHT;
			return JOY_NONE;
		}
		else if(!GPIO_ReadInputDataBit(JOY_CTR_KEY_Port,JOY_CTR_KEY_Pin))
		{
			keydone = JOY_SEL;
			return JOY_NONE;
		}	  
	}
	else if(keydone != JOY_NONE)
	{
		if(GPIO_ReadInputDataBit(JOY_A_KEY_Port,JOY_A_KEY_Pin) && keydone == JOY_LEFT)
		{
			keydone = JOY_NONE;
			return JOY_LEFT;
		}
		else if(GPIO_ReadInputDataBit(JOY_B_KEY_Port,JOY_B_KEY_Pin) && keydone == JOY_UP)
		{
			keydone = JOY_NONE;
			return JOY_UP;
		}
		else if(GPIO_ReadInputDataBit(JOY_C_KEY_Port,JOY_C_KEY_Pin) && keydone == JOY_DOWN)
		{
			keydone = JOY_NONE;
			return JOY_DOWN;
		}
		else if(GPIO_ReadInputDataBit(JOY_D_KEY_Port,JOY_D_KEY_Pin) && keydone == JOY_RIGHT)
		{
			keydone = JOY_NONE;
			return JOY_RIGHT;
		}
		else if(GPIO_ReadInputDataBit(JOY_CTR_KEY_Port,JOY_CTR_KEY_Pin) && keydone == JOY_SEL)
		{
			keydone = JOY_NONE;
			return JOY_SEL;
		}
	}
	return JOY_NONE;
}

JOYState_TypeDef Key_ReadIOPin_continuous(void)
{
	if(!GPIO_ReadInputDataBit(JOY_A_KEY_Port,JOY_A_KEY_Pin))
		return JOY_LEFT;
	else if(!GPIO_ReadInputDataBit(JOY_B_KEY_Port,JOY_B_KEY_Pin))
		return JOY_UP;
	else if(!GPIO_ReadInputDataBit(JOY_C_KEY_Port,JOY_C_KEY_Pin))
		return JOY_DOWN;
	else if(!GPIO_ReadInputDataBit(JOY_D_KEY_Port,JOY_D_KEY_Pin))
		return JOY_RIGHT;
	else if(!GPIO_ReadInputDataBit(JOY_CTR_KEY_Port,JOY_CTR_KEY_Pin))
		return JOY_SEL;
	return JOY_NONE;
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
