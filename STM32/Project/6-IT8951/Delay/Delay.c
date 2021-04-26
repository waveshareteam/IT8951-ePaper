#include "delay.h"
#include "stm32f4xx.h"

/* Private variables ---------------------------------------------------------*/	 
static uint8_t  delay_fac_us = 0;
static uint16_t delay_fac_ms = 0;
static FlagStatus  Status;


/*******************************************************************************
* Function Name  : delay_init
* Description    :  
* Input          : None
* Output         : None
* Return         : None
* Attention		 : 
*******************************************************************************/
void delay_init(void)
{
    RCC_ClocksTypeDef RCC_ClocksStatus;

    RCC_GetClocksFreq(&RCC_ClocksStatus);
    SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
	  SysTick_ITConfig(DISABLE);
    delay_fac_us = RCC_ClocksStatus.HCLK_Frequency / 8000000;
    delay_fac_ms = RCC_ClocksStatus.HCLK_Frequency / 8000;      
}
					
/*******************************************************************************
* Function Name  : delay_us
* Description    : 
* Input          : - Nus:
* Output         : None
* Return         : None
* Attention		 : 
*******************************************************************************/            
void delay_us(u32 Nus)
{ 
		SysTick_SetReload(delay_fac_us * Nus);
		SysTick_CounterCmd(SysTick_Counter_Clear);
		SysTick_CounterCmd(SysTick_Counter_Enable);
		do
		{
				Status = SysTick_GetFlagStatus(SysTick_FLAG_COUNT);
		}while (Status != SET);
		SysTick_CounterCmd(SysTick_Counter_Disable);
		SysTick_CounterCmd(SysTick_Counter_Clear);
}


/*******************************************************************************
* Function Name  : delay_ms
* Description    : 
* Input          : - nms:
* Output         : None
* Return         : None
* Attention		 : 
*******************************************************************************/  
void delay_ms(uint16_t nms)
{    
    uint32_t temp = delay_fac_ms * nms;

    if (temp > 0x00ffffff)
    {
        temp = 0x00ffffff;
    }
    SysTick_SetReload(temp);
    SysTick_CounterCmd(SysTick_Counter_Clear);
    SysTick_CounterCmd(SysTick_Counter_Enable);
    do
    {
        Status = SysTick_GetFlagStatus(SysTick_FLAG_COUNT);
    }while (Status != SET);
    SysTick_CounterCmd(SysTick_Counter_Disable);
		SysTick_CounterCmd(SysTick_Counter_Clear);
}

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
