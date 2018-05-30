#ifndef __FKCOMUNICATION_H
#define __FKCOMUNICATION_H



extern unsigned char FK_engine;			//0 nc  1: start, 2: shut,  3: cmd state
extern unsigned char FK_cmd;
extern unsigned char FK_cnt;



void Message_Rev(void);
void SendToMessage(void);
void FK_Rev(void);



#endif

