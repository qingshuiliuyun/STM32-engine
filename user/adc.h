/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __ADC_H
#define __ADC_H

#ifdef __cplusplus
 extern "C" {
#endif
                                              
/* Includes ------------------------------------------------------------------*/
     
     
#include "stm32f4xx.h"
#define ADC_CHANNEL    3

//extern vu16 AD_Value[ADC_CHANNEL];

void ADCInit(void);
void GetADCvalue(float ad_out5V[]);
     
#ifdef __cplusplus
}
#endif

#endif 



