#ifndef _MOTION_EXCHANGE_H__
#define _MOTION_EXCHANGE_H__

/**Include Header Files**/
#include "sysconfig.h"

#include "Task_StateMachine.h"

/**Enum Definition**/


/**Extern Declaration**/
extern uint8_t Auto_Position_Flag;

/**Function Delaration**/
void Manual_Lift_Exchange(void);
void Manual_Sucker_Exchange(void);
void Manual_Forward_Exchange(void);
void Manual_Pitch_Exchange(void);
void Manual_Roll_Exchange(void);
void Manual_Rotate_Exchange(void);

void Position_Back(void);
void Manual_Push_Exchange(void);

#endif
