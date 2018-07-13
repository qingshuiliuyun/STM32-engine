/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __ENGINE_H
#define __ENGINE_H

#ifdef __cplusplus
 extern "C" {
#endif
                                              
/* Includes ------------------------------------------------------------------*/

typedef struct  
{										    
	unsigned int rpm;
	unsigned int egt;
	float 		v_pump;
	unsigned int state;
	float		th_postion;
	float		current;
// 	unsigned int time;
// 	unsigned short fThr_pwm;
// 	unsigned short fAux_pwm;
// 	
// 	unsigned short off_thr;
// 	unsigned short idle_thr;
// 	unsigned short max_thr;
// 	unsigned short off_aux;
// 	unsigned short center_aux;
// 	unsigned short pos2_aux;
// 	
// 	float 		v_battery;
// 	float		fThr_pos;
// 	unsigned short	fThr_pwmRC;
// 	unsigned short 	fAux_pwmRC;
// 	float		fAux_pos;

}EngineTypedf; 
	 
extern unsigned char g_EngineStateRAC[4];	 
extern unsigned char g_EngineStateWPE[4];	 
extern unsigned char g_EngineStateTCO[4];
extern EngineTypedf g_Engine[4];

extern unsigned int  engine_waitfor_echo[4];


void Engine_Rev(unsigned char ch);
void SendEngineMoniter(void);
void FK_Engine(void);
void SendCmdRAC(void);

     

#ifdef __cplusplus
}
#endif

#endif 	 
	 
	 

