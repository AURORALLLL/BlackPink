#ifndef _MOTION_SPIN_H__
#define _MOTION_SPIN_H__

/**Include Header Files**/
#include "sysconfig.h"

#include "Task_StateMachine.h"



/**Macro Definition**/
#define CHASSISFOLLOW_MAXOUT		6000
#define CHASSISFOLLOW_MAXIOUT		4000
#define CHASSISFOLLOW_DEADZONE	0

/**Struct Delaration**/


/**Enum Definition**/
typedef enum
{
	Spin_State_Static = 0,
	Spin_State_ClockWise,
	Spin_State_Follow,
	Spin_State_Counter,
} Spin_State_t;



/**Extern Declaration**/
extern Spin_State_t Spin_State;
extern int16_t Chassis_Follow_W;
extern int16_t Chassis_Offset;
extern uint8_t SpinCounter;
extern uint8_t Auto_Follow_Flag;

/**Function Delaration**/
void Spin_Init(void);
void Manual_Spin(void);
void Spin_Chassis(void);
void Spin_Gimbal(void);

#endif
