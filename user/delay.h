/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DELAY_H
#define __DELAY_H

#ifdef __cplusplus
 extern "C" {
#endif
                                              
/* Includes ------------------------------------------------------------------*/
      
/**
  * @}
  */ 
void InitDelayTimer(void);
void delay_us(unsigned int data_us);
void delay_ms(unsigned int data_ms);
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

