#ifndef _TELERX_H
#define _TELERX_H

   
typedef struct
{
    unsigned char  cmd;
    unsigned char  data1;		
    unsigned char  data2;
} EchoStruct;

extern EchoStruct TeleEcho;
extern unsigned char  Tele_RxCnt;
extern unsigned char  Tele_RxFreq;
extern unsigned char  DGps_RxCnt;
extern unsigned char  DGps_RxFreq;
extern unsigned char  Tele_PackFreq;
extern unsigned char  Tele_PackCnt;
extern unsigned char  ValidTeleCnt;


void  Tele_Rev(void);    //“£øÿ÷°Ω” ’

extern unsigned int g_1ms_CNT;

#endif
