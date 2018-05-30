/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __PWM_H
#define __PWM_H

#ifdef __cplusplus
 extern "C" {
#endif
                                              
/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"

typedef struct
{
    uint16_t CCR1_Val;
    uint16_t CCR2_Val;
    uint16_t CCR3_Val;
    uint16_t CCR4_Val;
    uint16_t CCR5_Val; 
    uint16_t CCR6_Val;
    uint16_t CCR7_Val;
    uint16_t CCR8_Val;
    uint16_t CCR9_Val;
    uint16_t CCR10_Val;
    uint16_t CCR11_Val;
    uint16_t CCR12_Val;
    uint16_t CCR13_Val;
}PWMOUTTypeDef;      
/**
  * @}
  */ 

void PWMOUT_Set(void);
void PWMOUT_Init(void);

void InitPWMIN_GPIO(void);
void PWMIN_Config(void);

extern PWMOUTTypeDef PWM_OUT;

extern unsigned short			CH1Capture;
extern unsigned short 			Capture1_Val;
extern unsigned short			preCapture1_Val;
extern unsigned char 	Capture1_Sta;

extern unsigned short			CH2Capture;
extern unsigned short 			Capture2_Val;
extern unsigned short			preCapture2_Val;
extern unsigned char 	Capture2_Sta;

extern unsigned short			CH3Capture;
extern unsigned short 			Capture3_Val;
extern unsigned short			preCapture3_Val;
extern unsigned char 	Capture3_Sta;

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
