/*****************************************************************************/
/** \file mc_pic.h
**
** 
**
** History:
** 
*****************************************************************************/

#ifndef __MC_PIC_H__
#define __MC_PIC_H__

/*****************************************************************************/
/* Include files */
/*****************************************************************************/
#include  "base_define.h"

/*****************************************************************************/
/* Global pre-processor symbols/macros ('#define') */
/*****************************************************************************/


/*****************************************************************************/
/* Global type definitions ('typedef') */
/*****************************************************************************/
typedef struct 
{ 	
  s32 Kp;
  s32 Ki;
  s32 Lower_Output;     //Lower Limit for Output limitation
  s32 Upper_Output;     //Lower Limit for Output limitation
  s32 Lower_Integral;   //Lower Limit for Integral term limitation
  s32 Upper_Integral;   //Lower Limit for Integral term limitation
  s32 Integral;
  s32 Out;
  s32 KiSum;
  s32 Error;
} PID_Components;

/*****************************************************************************/
/* Global variable declarations ('extern', definition in C source) */
/*****************************************************************************/
extern  PID_Components   PID_ID, PID_IQ, PID_WL, PID_FW;
extern  s32 PI_SpeedLoop_Kd;

/*****************************************************************************/
/* Global function prototypes ('extern', definition in C source) */
/*****************************************************************************/
void 	FOC_PID_Parameter_Init(void);
s32 	PID_CALC(PID_Components *PID_Struct,s32 hReference, s32 hPresentFeedback);
s32 	PID_CALC2(PID_Components *PID_Struct,s32 hReference, s32 hPresentFeedback);
s32 	PI_Regulator(PID_Components *PID_Struct,s32 hReference, s32 hPresentFeedback);
s32 	HL_PI_AntiDump(PID_Components *PID_Struct,s32 hReference, s32 hPresentFeedback);
s32 	PID_SpeedLoop(PID_Components *PID_Struct,s32 hReference, s32 hPresentFeedback);

#endif	/* __MC_PIC_H__ */


