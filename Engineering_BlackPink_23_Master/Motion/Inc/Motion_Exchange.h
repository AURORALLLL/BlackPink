#ifndef _MOTION_EXCHANGE_H__
#define _MOTION_EXCHANGE_H__

/**Include Header Files**/
#include "sysconfig.h"

#include "Task_StateMachine.h"

/**Enum Definition**/


/**Extern Declaration**/
extern int16_t Pitch_Incline;
extern int16_t Yaw_Incline;

/**Function Delaration**/
void Manual_Lift_Exchange(void);

void Manual_Sucker_Exchange_Sky(void);
void Manual_Forward_Exchange_Sky(void);

void Manual_Sucker_Exchange_Arm(void);
void Manual_Forward_Exchange_Arm(void);
void Manual_Pitch_Exchange_Arm(void);
void Manual_Roll_Exchange_Arm(void);
void Manual_Yaw_Exchange_Arm(void);
void Manual_Push_Exchange_Arm(void);

void Pick_to_Exchange_Sky(void);
void Pick_to_Exchange_Arm(void);

#endif
