#include "engine.h"
#include "usart.h"
#include "PWM.h"
#include "FKcomunication.h"

#define NONE	0
#define RAC 	1	
#define RI1		2	
#define RSP		3	
#define RFI		4	
#define RRC		5	
#define TCO		6
#define WPE		7
#define OC		0xff


#define ECU_OK	0X50
#define ECU_UC	0X51
#define ECU_PA	0X52
#define ECU_NA	0X53
#define ECU_PR	0X54
#define ECU_PL	0X55
#define ECU_DF	0X56
#define ENGINE_ODD	0x57
#define ENGINE_SUM	0X58
#define ENGINE_TIME	0X59

EngineTypedf g_Engine[4];
unsigned char g_EngineStateRAC[4] = {0,0,0,0};		//使用后清零
unsigned char g_EngineStateWPE[4] = {0,0,0,0};		//使用后清零
unsigned char g_EngineStateTCO[4] = {0,0,0,0};		//使用后清零

unsigned char g_currCmd = NONE;

unsigned char engine_start[]=	{'1',',','T','C','O',',','1',0x0d};
unsigned char engine_shut[]=		{'1',',','T','C','O',',','0',0x0d};
unsigned char engine_setRpm[]=	{'1',',','W','R','P',','};
unsigned char engine_setThr[]=	{'1',',','W','P','E',','};

unsigned char engine_232_RC[]=		{'1',',','T','C','O',',','2',0x0d};
unsigned char engine_RC_232[]=		{'1',',','T','C','O',',','3',0x0d};

unsigned char engine_RAC[] = {'1',',','R','A','C',',','1',0x0d};
unsigned char engine_RI1[] = {'1',',','R','I','1',',','1',0x0d};
unsigned char engine_RSP[] = {'1',',','R','S','P',',','1',0x0d};
unsigned char engine_RFI[] = {'1',',','R','F','I',',','1',0x0d};
unsigned char engine_RRC[] = {'1',',','R','R','C',',','1',0x0d};

unsigned int  engine_waitfor_echo[]={0,0,0,0};

void SendCmdRAC(void)
{
	USART_SendBuf_notDMA(USART3,engine_RAC,sizeof(engine_RAC));
	USART_SendBuf_notDMA(UART4,engine_RAC,sizeof(engine_RAC));
	USART_SendBuf_notDMA(UART5,engine_RAC,sizeof(engine_RAC));
	USART_SendBuf_notDMA(USART6,engine_RAC,sizeof(engine_RAC));
	
	engine_waitfor_echo[0] ++;
	engine_waitfor_echo[1] ++;
	engine_waitfor_echo[2] ++;
	engine_waitfor_echo[3] ++;
	
}


void FK_Engine(void)
{
	unsigned char buf[5];
	switch(FK_engine)
	{
		case 0:
			break;
		case 1:
			USART_SendBuf_notDMA(USART3,engine_start,sizeof(engine_start));
			USART_SendBuf_notDMA(UART4,engine_start,sizeof(engine_start));
			USART_SendBuf_notDMA(UART5,engine_start,sizeof(engine_start));
			USART_SendBuf_notDMA(USART6,engine_start,sizeof(engine_start));
		
		    engine_waitfor_echo[0] ++;
			engine_waitfor_echo[1] ++;
			engine_waitfor_echo[2] ++;
			engine_waitfor_echo[3] ++;
		
			break;
		case 2:
			USART_SendBuf_notDMA(USART3,engine_shut,sizeof(engine_shut));
			USART_SendBuf_notDMA(UART4,engine_shut,sizeof(engine_shut));
			USART_SendBuf_notDMA(UART5,engine_shut,sizeof(engine_shut));
			USART_SendBuf_notDMA(USART6,engine_shut,sizeof(engine_shut));
		  
            engine_waitfor_echo[0] ++;
			engine_waitfor_echo[1] ++;
			engine_waitfor_echo[2] ++;
			engine_waitfor_echo[3] ++;		
		
		case 3:
			USART_SendBuf_notDMA(USART3,engine_setThr,sizeof(engine_setThr));
			USART_SendBuf_notDMA(UART4,engine_setThr,sizeof(engine_setThr));
			USART_SendBuf_notDMA(UART5,engine_setThr,sizeof(engine_setThr));
			USART_SendBuf_notDMA(USART6,engine_setThr,sizeof(engine_setThr));
			if(FK_cmd >= 100)
			{
				buf[0] = '1';
				buf[1] = '0';
				buf[2] = '0';
				buf[3] = 0x0d;
				USART_SendBuf_notDMA(USART3,buf,4);
				USART_SendBuf_notDMA(UART4,buf,4);
				USART_SendBuf_notDMA(UART5,buf,4);
				USART_SendBuf_notDMA(USART6,buf,4);
			}else if(FK_cmd >=10)
			{
				buf[0] = FK_cmd/10 + '0';
				buf[1] = FK_cmd%10 + '0';
				buf[2] = 0x0d;
				USART_SendBuf_notDMA(USART3,buf,3);
				USART_SendBuf_notDMA(UART4,buf,3);
				USART_SendBuf_notDMA(UART5,buf,3);
				USART_SendBuf_notDMA(USART6,buf,3);
			}
			else
			{
				buf[0] = FK_cmd%10 + '0';
				buf[1] = 0x0d;
				USART_SendBuf_notDMA(USART3,buf,2);
				USART_SendBuf_notDMA(UART4,buf,2);
				USART_SendBuf_notDMA(UART5,buf,2);
				USART_SendBuf_notDMA(USART6,buf,2);
			}
			
			engine_waitfor_echo[0] ++;
			engine_waitfor_echo[1] ++;
			engine_waitfor_echo[2] ++;
			engine_waitfor_echo[3] ++;
			
			break;
		default:
			break;
	}
	
	FK_engine = 0;	
}
					  
u8 NMEA_Comma_Pos(unsigned char *buf,unsigned char cx)
{	 		    
	unsigned char *p=buf;
	while(cx)
	{		 
		if(*buf=='*'||*buf<' '||*buf>'z')return 0XFF;
		if(*buf==',')cx--;
		buf++;
	}
	return buf-p;	 
}

u32 NMEA_Pow(unsigned char m,unsigned char n)
{
	unsigned int result=1;	 
	while(n--)result*=m;    
	return result;
}

int NMEA_Str2num(unsigned char *buf,unsigned char*dx)
{
	unsigned char *p=buf;
	unsigned int ires=0,fres=0;
	unsigned char ilen=0,flen=0,i;
	unsigned char mask=0;
	int res;
	while(1) 
	{
		if(*p=='-'){mask|=0X02;p++;}
		
		if((*p==',') || (*p==0x0d))break;
		
		if(*p=='.'){mask|=0X01;p++;}
		else if(*p>'9'||(*p<'0'))	
		{	
			ilen=0;
			flen=0;
			break;
		}
		
		if(mask&0X01)flen++;
		else ilen++;
		p++;
	}
	if(mask&0X02)buf++;	
	for(i=0;i<ilen;i++)	
	{  
		ires+=NMEA_Pow(10,ilen-1-i)*(buf[i]-'0');
	}
	if(flen>5)flen=5;	
	*dx=flen;	 		
	for(i=0;i<flen;i++)
	{  
		fres+=NMEA_Pow(10,flen-1-i)*(buf[ilen+1+i]-'0');
	} 
	res=ires*NMEA_Pow(10,flen)+fres;
	if(mask&0X02)res=-res;		   
	return res;
}	
//RAC反馈
void RAC_decode(unsigned char ch, unsigned char *buf_use,unsigned char num)
{
	unsigned char tail1=0;
	unsigned char dx=0;
	int num_use=0;
	
	tail1 = NMEA_Comma_Pos(buf_use,3);
	if(tail1 != 0xff)
	{
		num_use = NMEA_Str2num(buf_use+tail1,&dx); 
		g_Engine[ch].rpm = num_use;
//		printf("rpm:%d,",g_Engine.rpm);
	}
	
	tail1 = NMEA_Comma_Pos(buf_use,4);
	if(tail1 != 0xff)
	{
		num_use = NMEA_Str2num(buf_use+tail1,&dx); 
		g_Engine[ch].egt = num_use;
//		printf("egt:%d,",g_Engine.egt);
	}
	
	tail1 = NMEA_Comma_Pos(buf_use,5);
	if(tail1 != 0xff)
	{
		num_use = NMEA_Str2num(buf_use+tail1,&dx); 
		g_Engine[ch].v_pump = ((float)num_use)/NMEA_Pow(10,dx);	
//		printf("v_pump:%f,",g_Engine.v_pump);
	}
	
	tail1 = NMEA_Comma_Pos(buf_use,6);
	if(tail1 != 0xff)
	{
		num_use = NMEA_Str2num(buf_use+tail1,&dx); 
		g_Engine[ch].state = num_use;
//		printf("state:%d,",g_Engine.state);
	}

	tail1 = NMEA_Comma_Pos(buf_use,7);
	if(tail1 != 0xff)
	{
		num_use = NMEA_Str2num(buf_use+tail1,&dx); 
		g_Engine[ch].th_postion = ((float)num_use)/NMEA_Pow(10,dx);	
//		printf("th:%f,",g_Engine.th_postion);
	}
	
	tail1 = NMEA_Comma_Pos(buf_use,8);
	if(tail1 != 0xff)
	{
		num_use = NMEA_Str2num(buf_use+tail1,&dx); 
		g_Engine[ch].current = ((float)num_use)/NMEA_Pow(10,dx);
//		printf("i:%d/%f,\r\n",num_use,g_Engine.current);
	}

}





//NO1->U3  NO2->U6  NO3->U4  NO4->U5	

void Engine_Rev(unsigned char ch)
{
	unsigned char  rxBufLen=0, numread;
    unsigned char  idx, len, temp[256],buf_use[256];
    unsigned char  tail1, tail2, tail3, tail4, tail5, tail6, prtail;

    static unsigned char  head[4]={0,0,0,0}, tail[4]={0,0,0,0}, buff[4][256],state[4]={NONE,NONE,NONE,NONE};
    
	switch(ch)
	{
		case 0:
			rxBufLen = USART3_ReadLen();
			break;
		case 1:
			rxBufLen = USART6_ReadLen();
			break;
		case 2:
			rxBufLen = UART4_ReadLen();
			break;
		case 3:
			rxBufLen = UART5_ReadLen();
			break;
		default:
			break;
	}
	
    numread = (unsigned char)(rxBufLen>256 ? 256:rxBufLen);
    
    if (numread) 
    {              
		switch(ch)
		{
			case 0:
				USART3_ReadIn(temp, numread);
				break;
			case 1:
				USART6_ReadIn(temp, numread);
				break;
			case 2:
				UART4_ReadIn(temp, numread);
				break;
			case 3:
				UART5_ReadIn(temp, numread);
				break;
			default:
				break;
		}
		
        for (idx=0; idx<numread; idx++)
		{
			buff[ch][head[ch]] = temp[idx];

//===========================================================================================			
			if(buff[ch][head[ch]++] == 0x0d)
			{
				while( (len =(unsigned char)(head[ch]-tail[ch]))>0 )
				{
					tail1 = (unsigned char)(tail[ch] + 1);
					tail2 = (unsigned char)(tail[ch] + 2);
					tail3 = (unsigned char)(tail[ch] + 3);
					tail4 = (unsigned char)(tail[ch] + 4);
					tail5 = (unsigned char)(tail[ch] + 5);
					tail6 = (unsigned char)(tail[ch] + 6);
					
					if ((buff[ch][tail[ch]]=='1') && (buff[ch][tail1]==','))
					{   
						prtail = tail[ch];
						
						if( state[ch] != NONE )
						{
							if((buff[ch][tail2] == 'H') && (buff[ch][tail3] == 'S'))
							{
								if((buff[ch][tail5] == 'O') && (buff[ch][tail6] == 'K'))
								{
									
									
									for(idx = 0; idx < len; idx++)
									{
										buf_use[idx] = buff[ch][prtail++];
									}

									switch (state[ch])
									{
										case RAC:
											//RCA
											RAC_decode(ch, buf_use, len);	
											g_EngineStateRAC[ch] = ECU_OK;
										    engine_waitfor_echo[ch] = 0;
											tail[ch] = tail[ch]+len;
											break;
										case WPE:
											g_EngineStateWPE[ch] = ECU_OK;
											engine_waitfor_echo[ch] = 0;
											tail[ch] = tail[ch]+len;
											break;
										case TCO:
											g_EngineStateTCO[ch] = ECU_OK;
										    engine_waitfor_echo[ch] = 0;
											tail[ch] = tail[ch]+len;
											break;
										default:
											///////////////////////other cmd
											tail[ch] = tail[ch]+len;
											///////////////////////
											break;
									}
									
									state[ch] = NONE;
								}
								/*	
									#define ECU_UC	0X51
									#define ECU_PA	0X52
									#define ECU_NA	0X53
									#define ECU_PR	0X54
									#define ECU_PL	0X55
									#define ECU_DF	0X56
								*/						
								else if((buff[ch][tail5] == 'U') && (buff[ch][tail6] == 'C'))
								{
									switch (state[ch])
									{
										case RAC:
											g_EngineStateRAC[ch] = ECU_UC;
										    engine_waitfor_echo[ch] = 0;
											break;
										case WPE:
											g_EngineStateWPE[ch] = ECU_UC;
										    engine_waitfor_echo[ch] = 0;
											break;
										case TCO:
											g_EngineStateTCO[ch] = ECU_UC;
										    engine_waitfor_echo[ch] = 0;
											break;
										default:
											break;
									}
									state[ch] = ECU_UC;
									tail[ch] = tail[ch]+len;
								}
								else if((buff[ch][tail5] == 'P') && (buff[ch][tail6] == 'A'))
								{
									switch (state[ch])
									{
										case RAC:
											g_EngineStateRAC[ch] = ECU_PA;
										    engine_waitfor_echo[ch] = 0;
											break;
										case WPE:
											g_EngineStateWPE[ch] = ECU_PA;
										    engine_waitfor_echo[ch] = 0;
											break;
										case TCO:
											g_EngineStateTCO[ch] = ECU_PA;
										    engine_waitfor_echo[ch] = 0;
											break;
										default:
											break;
									}
									state[ch] = ECU_PA;
									tail[ch] = tail[ch]+len;
								}
								else if((buff[ch][tail5] == 'N') && (buff[ch][tail6] == 'A'))
								{
									switch (state[ch])
									{
										case RAC:
											g_EngineStateRAC[ch] = ECU_NA;
										    engine_waitfor_echo[ch] = 0;
											break;
										case WPE:
											g_EngineStateWPE[ch] = ECU_NA;
										    engine_waitfor_echo[ch] = 0;
											break;
										case TCO:
											g_EngineStateTCO[ch] = ECU_NA;
										    engine_waitfor_echo[ch] = 0;
											break;
										default:
											break;
									}
									state[ch] = ECU_NA;
									tail[ch] = tail[ch]+len;
								}
								else if((buff[ch][tail5] == 'P') && (buff[ch][tail6] == 'R'))
								{
									switch (state[ch])
									{
										case RAC:
											g_EngineStateRAC[ch] = ECU_PR;
										    engine_waitfor_echo[ch] = 0;
											break;
										case WPE:
											g_EngineStateWPE[ch] = ECU_PR;
										    engine_waitfor_echo[ch] = 0;
											break;
										case TCO:
											g_EngineStateTCO[ch] = ECU_PR;
										    engine_waitfor_echo[ch] = 0;
											break;
										default:
											break;
									}
									state[ch] = ECU_PR;
									tail[ch] = tail[ch]+len;
								}
								else if((buff[ch][tail5] == 'P') && (buff[ch][tail6] == 'L'))
								{
									switch (state[ch])
									{
										case RAC:
											g_EngineStateRAC[ch] = ECU_PL;
										    engine_waitfor_echo[ch] = 0;
											break;
										case WPE:
											g_EngineStateWPE[ch] = ECU_PL;
										    engine_waitfor_echo[ch] = 0;
											break;
										case TCO:
											g_EngineStateTCO[ch] = ECU_PL;
										    engine_waitfor_echo[ch] = 0;
											break;
										default:
											break;
									}
									state[ch] = ECU_PL;
									tail[ch] = tail[ch]+len;
								}
								else if((buff[ch][tail5] == 'D') && (buff[ch][tail6] == 'F'))
								{
									switch (state[ch])
									{
										case RAC:
											g_EngineStateRAC[ch] = ECU_DF;
										    engine_waitfor_echo[ch] = 0;
											break;
										case WPE:
											g_EngineStateWPE[ch] = ECU_DF;
										    engine_waitfor_echo[ch] = 0;
											break;
										case TCO:
											g_EngineStateTCO[ch] = ECU_DF;
										    engine_waitfor_echo[ch] = 0;
											break;
										default:
											break;
									}
									state[ch] = ECU_DF;
									tail[ch] = tail[ch]+len;
								}
								else 
								{
									//========feedback unknow=============================
									tail[ch] = tail[ch]+len;
									state[ch] = NONE;
									//===========================-========================
								}
							}
							else
							{
								//========return data ERROR===========================
								tail[ch]++;
								//===========================-========================
							}
						}
						else
						{
							if( (buff[ch][tail2]=='R') && (buff[ch][tail3]=='A') && (buff[ch][tail4]=='C'))
							{
								state[ch] = RAC;
								tail[ch] = tail[ch]+len;
							}
							else if( (buff[ch][tail2]=='W') && (buff[ch][tail3]=='P') && (buff[ch][tail4]=='E') )
							{
								state[ch] = WPE;
								tail[ch] = tail[ch]+len;
							}
							else if( (buff[ch][tail2]=='T') && (buff[ch][tail3]=='C') && (buff[ch][tail4]=='O') )
							{
								state[ch] = TCO;
								tail[ch] = tail[ch]+len;
							}
							else
							{
								/////////////////////////other cmd
								state[ch] = NONE;
								tail[ch] = tail[ch]+len;
								/////////////////////////
							}
						}						
					}        	
					else
						tail[ch] ++;
				}
				
			}
//=============================================================================================		

		}
            	
    } 

	//ECU接收指令错误处理
	if(state[ch] > ECU_OK)
	{
		switch(state[ch])
		{
			case ECU_UC:
				break;
			case ECU_PA:
				break;
			case ECU_NA:
				break;
			case ECU_PR:
				break;
			case ECU_PL:
				break;
			case ECU_DF:
				break;
			default:
				break;
		}
		
		state[ch] = NONE;
	}

}








