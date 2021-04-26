
#include "KEY&LED.h"
#include "usart.h"


static void Delay(__IO uint32_t nCount);
void JOYState_LED_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* GPIOF Periph clock enable */
	RCC_AHB1PeriphClockCmd(LED1_RCC_AHBPeriph | LED2_RCC_AHBPeriph | LED3_RCC_AHBPeriph | LED4_RCC_AHBPeriph, ENABLE);
	
	/* Configure PF6 PF7 PF8 PF9 in output pushpull mode */
	GPIO_InitStructure.GPIO_Pin = LED1_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(LED1_Port, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = LED2_Pin;
	GPIO_Init(LED2_Port, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = LED3_Pin;
	GPIO_Init(LED3_Port, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = LED4_Pin;
	GPIO_Init(LED4_Port, &GPIO_InitStructure);
	
	/*--------------------------------------------------------------------------------
	
	Key GPIO Config
	
	--------------------------------------------------------------------------------*/
	/* Periph clock enable */
	RCC_AHB1PeriphClockCmd(USER_KEY_RCC_AHBPeriph | WAKEUP_KEY_RCC_AHBPeriph | JOY_A_KEY_RCC_AHBPeriph |
							 JOY_B_KEY_RCC_AHBPeriph |  JOY_C_KEY_RCC_AHBPeriph |  JOY_D_KEY_RCC_AHBPeriph |
							  JOY_CTR_KEY_RCC_AHBPeriph, ENABLE);
	
	/* Configure Input pushpull mode */
	GPIO_InitStructure.GPIO_Pin = USER_KEY_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(USER_KEY_Port, &GPIO_InitStructure);
	

	GPIO_InitStructure.GPIO_Pin = WAKEUP_KEY_Pin;
	GPIO_Init(WAKEUP_KEY_Port, &GPIO_InitStructure);

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

uint8_t Read_JOYState(void)
{
  uint8_t vulua;
  if(!GPIO_ReadInputDataBit(JOY_A_KEY_Port,JOY_A_KEY_Pin))
		vulua=0;
	else if(!GPIO_ReadInputDataBit(JOY_B_KEY_Port,JOY_B_KEY_Pin))
		vulua=1;
	else if(!GPIO_ReadInputDataBit(JOY_C_KEY_Port,JOY_C_KEY_Pin))
		vulua=2;
	else if(!GPIO_ReadInputDataBit(JOY_D_KEY_Port,JOY_D_KEY_Pin))
		vulua=3;
	else if(!GPIO_ReadInputDataBit(JOY_CTR_KEY_Port,JOY_CTR_KEY_Pin))
		vulua=4;
	else
	 vulua=5;
	Delay(0xffff);
	
	while((!GPIO_ReadInputDataBit(JOY_A_KEY_Port,JOY_A_KEY_Pin))||
		    (!GPIO_ReadInputDataBit(JOY_B_KEY_Port,JOY_B_KEY_Pin))||
     	  (!GPIO_ReadInputDataBit(JOY_C_KEY_Port,JOY_C_KEY_Pin))||
	      (!GPIO_ReadInputDataBit(JOY_D_KEY_Port,JOY_D_KEY_Pin))||
	      (!GPIO_ReadInputDataBit(JOY_CTR_KEY_Port,JOY_CTR_KEY_Pin)));
	Delay(0xfffff);
	
	return vulua;
}

void Led_Toggle(uint8_t keyvulua)
{
	switch(keyvulua)
		{
			case 0:
        printf("KEY A \r\n");
			  GPIO_SetBits(LED1_Port,LED1_Pin);
// 			  GPIO_SetBits(JOY_A_KEY_Port,JOY_A_KEY_Pin);
// 			  GPIO_SetBits(JOY_A_KEY_Port,JOY_A_KEY_Pin);
// 			  GPIO_SetBits(JOY_A_KEY_Port,JOY_A_KEY_Pin);
			    
				break;
			case 1:
        printf("KEY B \r\n");
			  GPIO_SetBits(LED2_Port,LED2_Pin);
				break;
			case 2:
				printf("KEY C \r\n");
			  GPIO_SetBits(LED3_Port,LED3_Pin);
				break;
			case 3:
				printf("KEY D \r\n");
			  GPIO_SetBits(LED4_Port,LED4_Pin);
				break;
			case 4:
			  printf("KEY Center \r\n");
			  GPIO_SetBits(LED1_Port,LED1_Pin);
			  GPIO_SetBits(LED2_Port,LED2_Pin);
			  GPIO_SetBits(LED3_Port,LED3_Pin);
			  GPIO_SetBits(LED4_Port,LED4_Pin);
				break;
			case 5:
			  GPIO_ResetBits(LED1_Port,LED1_Pin);
			  GPIO_ResetBits(LED2_Port,LED2_Pin);
			  GPIO_ResetBits(LED3_Port,LED3_Pin);
			  GPIO_ResetBits(LED4_Port,LED4_Pin);
				break;

		}
			Delay(0xfffff);
}

/**
  * @brief  Delay Function.
  * @param  nCount:specifies the Delay time length.
  * @retval None
  */
static void Delay(__IO uint32_t nCount)
{
  while(nCount--)
  {
  }
}
