/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BSP_H
#define __BSP_H

#ifdef __cplusplus
 extern "C" {
#endif
                                              
/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "usart.h"
#include "nivc.h"
#include "Tim.h"
#include "SPI.h"
#include "delay.h"
#include "adc.h"
      
/**
  * @}
  */ 
 
void LEDInit(void);
void LEDOn(void);
void LEDOff(void);
void LEDToggle(void);
     
void InitBSP(void);


/**
  * @}
  */
  
#ifdef __cplusplus
}
#endif

#endif /* __STM32F4_DISCOVERY_H */
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
