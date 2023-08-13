#ifndef _MOTION_BARRIER_H__
#define _MOTION_BARRIER_H__

/**Include Header Files**/
#include "sysconfig.h"

#include "Task_StateMachine.h"

/**Enum Definition**/


/**Extern Declaration**/
extern uint8_t Auto_Barrier_Flag;

/**Function Delaration**/
void Manual_Lift_Barrier(void);
void Manual_Sucker_Barrier(void);
void Manual_Forward_Barrier(void);
void Manual_Pitch_Barrier(void);

void Barrier_Init(void);
//void Manual_Roll_Barrier(void);





#endif
