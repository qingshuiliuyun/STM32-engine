/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include <stdio.h>

#include "bsp.h"
#include "TeleRx.h"

#include "engine.h"
#include "pwm.h"
#include "FKcomunication.h"

#include "stm32f4xx_conf.h"





void InitUSART_Config(void)
{
	USART_Config(USART1,115200,USART_WordLength_9b,USART_StopBits_1,USART_Parity_Odd);
	USART_Config(USART2,115200,USART_WordLength_9b,USART_StopBits_1,USART_Parity_Odd);
	USART_Config(USART3,9600,USART_WordLength_8b,USART_StopBits_1,USART_Parity_No);
	USART_Config(UART4,9600,USART_WordLength_8b,USART_StopBits_1,USART_Parity_No);
	USART_Config(UART5,9600,USART_WordLength_8b,USART_StopBits_1,USART_Parity_No);
	USART_Config(USART6,9600,USART_WordLength_8b,USART_StopBits_1,USART_Parity_No);
}


/**
  * @brief   Main program
  * @param  None
  * @retval None
  */
int main(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	uint64_t time_us;
	uint64_t pre_time_us;
	unsigned int detime=0;
    InitBSP();
    
    LEDOn();
	
    if (SysTick_Config(SystemCoreClock / 2000))
    { 
        /* Capture error */ 
        while (1);
    }
	
	PWM_OUT.CCR7_Val = 1000;
	PWM_OUT.CCR8_Val = 1000;
    while (1)
    {

        if(tag_1ms)          
        {
            tag_1ms = FALSE;
			time_us = get_absolute_time();
			
			Check_PrintfBuf_1ms();
			detime = time_us - pre_time_us;
			
			pre_time_us = time_us;
                
        }
        if(tag_2ms)          
        {
            tag_2ms = FALSE;    
        }
		
		if(tag_2_5ms)          
        {
            tag_2_5ms = FALSE; 
			//接收发动机发来的反馈
			Engine_Rev(0);		
			Engine_Rev(1);
			Engine_Rev(2);
			Engine_Rev(3);			
        }
		
        if(tag_5ms)          
        {
            tag_5ms = FALSE;
			//接收飞控发来的指令
			FK_Rev();
			//接收信息管理器发来的指令
			Message_Rev();
			
			//往发动机发送控制指令
			FK_Engine();
			//往信息管理器发送发动机反馈数据
			SendToMessage();
        }
                
        if(tag_10ms && (tag_5ms == FALSE))
        {
            tag_10ms = FALSE;
			
			
			PWM_OUT.CCR1_Val = 1500;
			PWM_OUT.CCR2_Val = 1500;
			PWM_OUT.CCR3_Val = 1500;
			PWM_OUT.CCR4_Val = 1500;
			PWM_OUT.CCR5_Val = 1500;
			PWM_OUT.CCR6_Val = 1500;
			//PWM_OUT.CCR7_Val = 1500;
			//PWM_OUT.CCR8_Val = 1500;
			PWM_OUT.CCR9_Val = 1500;
			PWM_OUT.CCR10_Val = 1500;
			PWM_OUT.CCR11_Val = 1500;
			PWM_OUT.CCR12_Val = 1500;
			PWM_OUT.CCR13_Val = 1500;

			PWMOUT_Set();
        }

        if(tag_20ms ) 
        {
            tag_20ms = FALSE;
        }

        if(tag_25ms) 
        {
            tag_25ms = FALSE; 
            
        }    
                
        if(tag_40ms) 
        {
            tag_40ms = FALSE;
        }

        if(tag_50ms)
        {
            tag_50ms = FALSE;
        }
       
        if(tag_80ms)
        {
            tag_80ms = FALSE;
        }
        
        if(tag_100ms)   
        {
            tag_100ms = FALSE;
        }    
      
        if(tag_125ms)
        {
            tag_125ms = FALSE; 
        }

        if(tag_200ms)
        {
            tag_200ms = FALSE; 
			//FK_HM_TEST();
			
			
        }
        
        if(tag_500ms && (tag_10ms == FALSE))
        {
            tag_500ms = FALSE;
            LEDToggle();
        }
        
        if(tag_1s && (tag_1ms == FALSE))
        {    
            
            tag_1s = FALSE;
			//往发动机发送状态查询指令
			SendCmdRAC();
			
        }
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

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
