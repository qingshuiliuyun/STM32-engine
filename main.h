/**
  ******************************************************************************
  * @file    SysTick/main.h 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    19-September-2011
  * @brief   Header for main.c module
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************  
  */ 

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/

#ifdef __cplusplus
 extern "C" {
#endif


#include "stm32f4xx_it.h"
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
//void TimingDelay_Decrement(void);
typedef enum{FALSE = 0, TRUE = !FALSE} bool;


#define Fabs(a) ((a)<(0.0f)?(-a):(a))


extern unsigned char tag_1ms;
extern unsigned char tag_2ms;
extern unsigned char tag_2_5ms;
extern unsigned char tag_5ms;
extern unsigned char tag_10ms;
extern unsigned char tag_20ms;
extern unsigned char tag_25ms;
extern unsigned char tag_40ms;
extern unsigned char tag_50ms;
extern unsigned char tag_80ms;
extern unsigned char tag_100ms;
extern unsigned char tag_125ms;
extern unsigned char tag_200ms;
extern unsigned char tag_500ms;
extern unsigned char tag_1s;

extern int time_d5ms;

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
