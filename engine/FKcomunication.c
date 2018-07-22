#include "FKcomunication.h"
#include "usart.h"
#include "engine.h"

#define TRUE 1
#define FALSE 0

unsigned char FK_engine = 0;			//0 nc  1: start, 2: shut,  3: cmd state
unsigned char FK_cmd = 0;
unsigned char FK_cnt = 0;

unsigned char M_cnt = 0;
unsigned char M_state = 0;				//0: FK     1: MessageController

unsigned char  FK_RxCheckSum(unsigned char buf[], unsigned char len)  
{
	
	unsigned char  sum=0;
	unsigned char  idx = 0;
    unsigned char  check = FALSE;
	
	for (idx=0; idx<len-1; idx++)  
	    sum += buf[idx];
	
	if (buf[len-1] == sum)
	    check = TRUE;	
	else
	    check = FALSE;
	
	return check;
}

unsigned char  FK_TxCheckSum(unsigned char buf[], unsigned char len)
{  
    
    unsigned char  idx = 0;
    unsigned char  sum = 0;
    
	for (idx=0; idx<len-1; idx++ )      
	    sum += buf[idx];	
    
    return sum;
    
}

void  sort(unsigned char a[], unsigned char len)
{
    unsigned char  idx, jdx;
    unsigned char  tmp;
    
    for (jdx=(unsigned char)(len-1); jdx>0; jdx--) 
    {
        for (idx=0; idx<jdx; idx++) 
        {
            if (a[idx] > a[idx+1]) 
            {
                tmp = a[idx];
                a[idx] = a[idx+1];
                a[idx+1] = tmp;
            }
        }
    }
}




void FK_Rx(unsigned char buf[], unsigned char len)
{
	unsigned char a[3];
	unsigned char buf_error[9];
	
	a[0] = buf[1];
	a[1] = buf[2];
	a[2] = buf[3];
	
	sort(a, 3);
    if((a[0]==a[1]) || (a[1]==a[2]))  
    {
		if(buf[0] == 0x00)
		{
			if(a[1] == 0)
			{
				FK_engine = 2;
			}
			else if(a[1] == 1)
			{
				FK_engine = 1;
			}
			else
			{
				FK_engine = 0;
			}	
		}
		else if(buf[0] == 0xff)
		{
			FK_engine = 3;
			FK_cmd = a[1];
		}		
	}
	else
	{
		buf_error[0] = 0xeb;
		buf_error[1] = 0x90;
		buf_error[2] = buf[0];
		buf_error[3] = 0x59;
		buf_error[4] = 0x59;
		buf_error[5] = 0x59;
		buf_error[6] = 0x59;
		buf_error[7] = FK_cnt++;
		buf_error[8] = FK_TxCheckSum( (buf_error+2), (sizeof(buf_error)-2) );
		
		USART_SendBuf_notDMA(USART1,buf_error,sizeof(buf_error));
	}
}

void FK_Rev(void)
{
	unsigned char buf_error[9];
	unsigned char  rxBufLen, numread;
    unsigned char  idx,  temp[256],buf_use[7];//buf_use[6];
    unsigned char  tail1, tail2, prtail; 

    static unsigned char  head=0, tail=0, buff[256];
    
    rxBufLen = USART1_ReadLen();
    numread = (unsigned char)(rxBufLen>256 ? 256:rxBufLen);
    if (numread) 
    {              
		USART1_ReadIn(temp, numread);
		
		for (idx=0; idx<numread; idx++)
		{
			buff[head++] = temp[idx];
		}
	}
	
	//while (((unsigned char)(head-tail)) >= 8) 
	while (((unsigned char)(head-tail)) >= 9) 
    {
		tail1 = (unsigned char)(tail + 1);
		tail2 = (unsigned char)(tail + 2);
		
		if( (buff[tail]==0xEB) && (buff[tail1]==0x90) )
		{
			prtail = (unsigned char)(tail+2);
            for (idx=0; idx<sizeof(buf_use); idx++) 
                buf_use[idx] = buff[prtail++];
			
			if (FK_RxCheckSum(buf_use, sizeof(buf_use)))  //满足校验和
			{
			   //tail += 8;
               tail += 9;				
			   FK_Rx(buf_use,sizeof(buf_use));
			}
			else
			{
				buf_error[0] = 0xeb;
				buf_error[1] = 0x90;
				buf_error[2] = buff[tail2];
				buf_error[3] = 0x58;
				buf_error[4] = 0x58;
				buf_error[5] = 0x58;
				buf_error[6] = 0x58;
				buf_error[7] = FK_cnt++;
				buf_error[8] = FK_TxCheckSum( (buf_error+2), (sizeof(buf_error)-2) );
				
				USART_SendBuf_notDMA(USART1,buf_error,sizeof(buf_error));
				tail ++; 
			}				
			
		}
		else
			tail++;
	}
	
	
	
	//反馈给飞控
	
	if( (M_state == 0) && ((g_EngineStateTCO[0] != 0) || (g_EngineStateTCO[1] != 0) || (g_EngineStateTCO[2] != 0) || (g_EngineStateTCO[3] != 0)))
	{
		
		if((engine_waitfor_echo[0] > 20)&&(g_EngineStateTCO[0] == 0))//当发送指令数量累计超过20个没有回应时，给飞控超时反馈
		{
			g_EngineStateTCO[0] = 0x4f;
		}
		
		if((engine_waitfor_echo[1] > 20)&&(g_EngineStateTCO[1] == 0))
		{
			g_EngineStateTCO[1] = 0x4f;
		}
		
		if((engine_waitfor_echo[2] > 20)&&(g_EngineStateTCO[2] == 0))
		{
			g_EngineStateTCO[2] = 0x4f;
		}
		
		if((engine_waitfor_echo[3] > 20)&&(g_EngineStateTCO[3] == 0))
		{
			g_EngineStateTCO[3] = 0x4f;
		}
		
		
		buf_error[0] = 0xeb;
		buf_error[1] = 0x90;
		buf_error[2] = 0x00;
		buf_error[3] = g_EngineStateTCO[0];
		buf_error[4] = g_EngineStateTCO[1];
		buf_error[5] = g_EngineStateTCO[2];
		buf_error[6] = g_EngineStateTCO[3];
		buf_error[7] = FK_cnt++;
		buf_error[8] = FK_TxCheckSum( (buf_error+2), (sizeof(buf_error)-2) );
		
		USART_SendBuf_notDMA(USART1,buf_error,sizeof(buf_error));
		
		g_EngineStateTCO[0] = 0;
		g_EngineStateTCO[1] = 0;
		g_EngineStateTCO[2] = 0;
		g_EngineStateTCO[3] = 0;
	}
	
	if( (g_EngineStateWPE[0] != 0) || (g_EngineStateWPE[1] != 0) || (g_EngineStateWPE[2] != 0) || (g_EngineStateWPE[3] != 0))
	{
		
		if((engine_waitfor_echo[0] > 20)&&(g_EngineStateWPE[0] == 0))//当发送指令数量累计超过20个没有回应时，给飞控超时反馈
		{
			g_EngineStateWPE[0] = 0x4f;
		}
		
		if((engine_waitfor_echo[1] > 20)&&(g_EngineStateWPE[1] == 0))
		{
			g_EngineStateWPE[1] = 0x4f;
		}
		
		if((engine_waitfor_echo[2] > 20)&&(g_EngineStateWPE[2] == 0))
		{
			g_EngineStateWPE[2] = 0x4f;
		}
		
		if((engine_waitfor_echo[3] > 20)&&(g_EngineStateWPE[3] == 0))
		{
			g_EngineStateWPE[3] = 0x4f;
		}
		
		
		buf_error[0] = 0xeb;
		buf_error[1] = 0x90;
		buf_error[2] = 0xFF;
		buf_error[3] = g_EngineStateWPE[0];
		buf_error[4] = g_EngineStateWPE[1];
		buf_error[5] = g_EngineStateWPE[2];
		buf_error[6] = g_EngineStateWPE[3];
		buf_error[7] = FK_cnt++;
		buf_error[8] = FK_TxCheckSum( (buf_error+2), (sizeof(buf_error)-2) );
		
		USART_SendBuf_notDMA(USART1,buf_error,sizeof(buf_error));
		
		g_EngineStateWPE[0] = 0;
		g_EngineStateWPE[1] = 0;
		g_EngineStateWPE[2] = 0;
		g_EngineStateWPE[3] = 0;
	}
}



void SendToMessage(void)
{
	unsigned char buf[45];
	unsigned char i=0,j=0;
	union {
        unsigned char   B[4];
        signed short   W[2];
		unsigned int    uI;
    } src;
	static unsigned char cnt=0;
	
	if( (g_EngineStateRAC[0] != 0) || (g_EngineStateRAC[1] != 0) || (g_EngineStateRAC[2] != 0) || (g_EngineStateRAC[3] != 0))
	{
		
		if((engine_waitfor_echo[0] > 20)&&(g_EngineStateRAC[0] == 0))//当发送指令数量累计超过20个没有回应时，给飞控超时反馈
		{
			g_EngineStateRAC[0] = 0x4f;
		}
		
		if((engine_waitfor_echo[1] > 20)&&(g_EngineStateRAC[1] == 0))
		{
			g_EngineStateRAC[1] = 0x4f;
		}
		
		if((engine_waitfor_echo[2] > 20)&&(g_EngineStateRAC[2] == 0))
		{
			g_EngineStateRAC[2] = 0x4f;
		}
		
		if((engine_waitfor_echo[3] > 20)&&(g_EngineStateRAC[3] == 0))
		{
			g_EngineStateRAC[3] = 0x4f;
		}
		
		
		buf[0] = 0x9a;
		buf[1] = 0xbc;
		buf[2] = 0x00;
		
		i=3;
		for(j=0;j<4;j++)
		{
			buf[i++] = g_EngineStateRAC[j];
			buf[i++] = g_Engine[j].state;
			
			src.uI = g_Engine[j].rpm;
			buf[i++] = src.B[0];
			buf[i++] = src.B[1];
			buf[i++] = src.B[2];
			buf[i++] = src.B[3];
			
			src.W[0] = g_Engine[j].egt;
			buf[i++] = src.B[0];
			buf[i++] = src.B[1];
			
			buf[i++] = (unsigned char)g_Engine[j].v_pump;
			buf[i++] = (unsigned char)g_Engine[j].current;
		}
		
		buf[i++] = cnt++;
		buf[i++] = FK_TxCheckSum( (buf+2), (sizeof(buf)-2) );
		
		USART_SendBuf_notDMA(USART2,buf,sizeof(buf));
		
		g_EngineStateRAC[0] = 0;
		g_EngineStateRAC[1] = 0;
		g_EngineStateRAC[2] = 0;
		g_EngineStateRAC[3] = 0;
	}
}


void Message_Rev(void)
{
	unsigned char  rxBufLen, numread;
    unsigned char  idx,  temp[256],buf_use[5];
    unsigned char  tail1, prtail; 
	
	unsigned char a[3];
	unsigned char buf_error[9];

    static unsigned char  head=0, tail=0, buff[256];
    
    rxBufLen = USART2_ReadLen();
    numread = (unsigned char)(rxBufLen>256 ? 256:rxBufLen);
    if (numread) 
    {              
		USART2_ReadIn(temp, numread);
		
		for (idx=0; idx<numread; idx++)
		{
			buff[head++] = temp[idx];
		}
	}
	
	while (((unsigned char)(head-tail)) >= 7) 
    {
		tail1 = (unsigned char)(tail + 1);
		
		if( (buff[tail]==0x9a) && (buff[tail1]==0xbc) )
		{
			prtail = (unsigned char)(tail+2);
            for (idx=0; idx<sizeof(buf_use); idx++) 
                buf_use[idx] = buff[prtail++];
			
			if (FK_RxCheckSum(buf_use, sizeof(buf_use)))  
			{
			   tail += 7;			       

				a[0] = buf_use[0];
				a[1] = buf_use[1];
				a[2] = buf_use[2];
				
				sort(a, 3);
				if((a[0]==a[1]) || (a[1]==a[2]))  
				{
					if(a[1] == 0)
					{
						FK_engine = 2;
						M_state = 1;
					}
					else if(a[1] == 1)
					{
						FK_engine = 1;
						M_state = 1;
					}
					else
					{
						M_state = 0;
					}	
				}
				else
				{
					buf_error[0] = 0x9a;
					buf_error[1] = 0xbc;
					buf_error[2] = 0xff;
					buf_error[3] = 0x59;
					buf_error[4] = 0x59;
					buf_error[5] = 0x59;
					buf_error[6] = 0x59;
					buf_error[7] = M_cnt++;
					buf_error[8] = FK_TxCheckSum( (buf_error+2), (sizeof(buf_error)-2) );
					
					USART_SendBuf_notDMA(USART2,buf_error,sizeof(buf_error));
				}
			}
			else
			{
				buf_error[0] = 0x9a;
				buf_error[1] = 0xbc;
				buf_error[2] = 0xff;
				buf_error[3] = 0x58;
				buf_error[4] = 0x58;
				buf_error[5] = 0x58;
				buf_error[6] = 0x58;
				buf_error[7] = M_cnt++;
				buf_error[8] = FK_TxCheckSum( (buf_error+2), (sizeof(buf_error)-2) );
				
				USART_SendBuf_notDMA(USART2,buf_error,sizeof(buf_error));
				tail ++; 
			}				
			
		}
		else 
			tail++;
	}
	
	if( (M_state == 1) && ((g_EngineStateTCO[0] != 0) || (g_EngineStateTCO[1] != 0) || (g_EngineStateTCO[2] != 0) || (g_EngineStateTCO[3] != 0)))
	{
		M_state = 0;
		
		if((engine_waitfor_echo[0] > 20)&&(g_EngineStateTCO[0] == 0))//当发送指令数量累计超过20个没有回应时，给信息管理器超时反馈
		{
			g_EngineStateTCO[0] = 0x4f;
		}
		
		if((engine_waitfor_echo[1] > 20)&&(g_EngineStateTCO[1] == 0))
		{
			g_EngineStateTCO[1] = 0x4f;
		}
		
		if((engine_waitfor_echo[2] > 20)&&(g_EngineStateTCO[2] == 0))
		{
			g_EngineStateTCO[2] = 0x4f;
		}
		
		if((engine_waitfor_echo[3] > 20)&&(g_EngineStateTCO[3] == 0))
		{
			g_EngineStateTCO[3] = 0x4f;
		}
		
		
		buf_error[0] = 0x9a;
		buf_error[1] = 0xbc;
		buf_error[2] = 0xff;
		buf_error[3] = g_EngineStateTCO[0];
		buf_error[4] = g_EngineStateTCO[1];
		buf_error[5] = g_EngineStateTCO[2];
		buf_error[6] = g_EngineStateTCO[3];
		buf_error[7] = M_cnt++;
		buf_error[8] = FK_TxCheckSum( (buf_error+2), (sizeof(buf_error)-2) );
		
		USART_SendBuf_notDMA(USART2,buf_error,sizeof(buf_error));
		
		g_EngineStateTCO[0] = 0;
		g_EngineStateTCO[1] = 0;
		g_EngineStateTCO[2] = 0;
		g_EngineStateTCO[3] = 0;
	}
}

