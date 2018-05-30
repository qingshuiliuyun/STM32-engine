/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USART_H
#define __USART_H

#ifdef __cplusplus
 extern "C" {
#endif
                                              
/* Includes ------------------------------------------------------------------*/
     
     
#include "stm32f4xx.h"
#include <stdio.h>

#define SEND1NUM 150
#define SEND6NUM 256


void Init_USART(void);
void Check_PrintfBuf_1ms(void);

unsigned char USART6_ReadLen(void);
void USART6_ReadIn(unsigned char *pdata,unsigned char num);

unsigned char UART5_ReadLen(void);
void UART5_ReadIn(unsigned char *pdata,unsigned char num);

unsigned char UART4_ReadLen(void);
void UART4_ReadIn(unsigned char *pdata,unsigned char num);

unsigned char USART3_ReadLen(void);
void USART3_ReadIn(unsigned char *pdata,unsigned char num);

unsigned char USART1_ReadLen(void);
void USART1_ReadIn(unsigned char *pdata,unsigned char num);

unsigned char USART2_ReadLen(void);
void USART2_ReadIn(unsigned char *pdata,unsigned char num);

void USART_SendBuf_notDMA(USART_TypeDef* USARTx, unsigned char *sendbuf, unsigned char num);

#define RECENUM 256 

void USART_Config(USART_TypeDef* USARTx,uint32_t baund, uint16_t WordLength, uint16_t StopBits, uint16_t Parity );
     
#ifdef __cplusplus
}
#endif

#endif 
