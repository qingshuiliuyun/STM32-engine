/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TIM_H
#define __TIM_H

#ifdef __cplusplus
 extern "C" {
#endif
                                              
/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"

void InitDeltaTimer(void);
uint64_t get_absolute_time(void);

extern unsigned short time_us;
extern float time;
	 
      
/**
  * @}
  */ 
//void TIM6_Config(void);

/**
  * @}
  */
  
#ifdef __cplusplus
}
#endif

#endif 
/**
  * @}
  */ 

/**
  * @}
  */ 

/**
  * @}
  */

 

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
