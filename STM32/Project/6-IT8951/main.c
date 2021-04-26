/**
  ******************************************************************************
  * @author  Waveshare Team
  ******************************************************************************
**/

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usart.h"
#include "delay.h"
#include "IT8951.h"

/** @addtogroup STM32F4xx_StdPeriph_Examples
  * @{
  */

/** @addtogroup USART_Printf
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

//Global varivale
extern I80IT8951DevInfo gstI80DevInfo;

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  /*!< At this stage the microcontroller clock setting is already configured, 
       this is done through SystemInit() function which is called from startup
       files (startup_stm32f40_41xxx.s/startup_stm32f427_437xx.s/startup_stm32f429_439xx.s)
       before to branch to application main. 
       To reconfigure the default setting of SystemInit() function, refer to
       system_stm32f4xx.c file.
  */     
	RCC_ClocksTypeDef SYS_Clocks;

  /* USART configuration */
	USART_Config();
  
	RCC_GetClocksFreq(&SYS_Clocks);
	printf("\r\nSYSCLK:%dM\r\n",SYS_Clocks.SYSCLK_Frequency/1000000);
	printf("HCLK:%dM\r\n",SYS_Clocks.HCLK_Frequency/1000000);
	printf("PCLK1:%dM\r\n",SYS_Clocks.PCLK1_Frequency/1000000);
	printf("PCLK2:%dM\r\n",SYS_Clocks.PCLK2_Frequency/1000000);	
	printf("IT8951 Example \r\n");

	SDRAM_Init();
	
	delay_init();
		
	InterfaceInit();
	
	printf("Example \r\n");
	IT8951DisplayExample();
	
	printf("Example 02 \r\n");
 	IT8951DisplayExample2(); //16 Gray stripe

/*
	printf("LISAR=%x \r\n",IT8951ReadReg(LISAR));//8340
	IT8951WriteReg(LISAR, 0x1234);
	printf("LISAR=%x \r\n",IT8951ReadReg(LISAR));//1234
*/
	while (1)
	{
		printf("image... \r\n");
		IT8951DisplayExample3();
		printf("Clear... \r\n");
		IT8951DisplayClear();
		printf("16Gray... \r\n");
		IT8951DisplayExample2();
		printf("Clear... \r\n");
		IT8951DisplayClear();
	}
}



#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */ 

/**
  * @}
  */ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
