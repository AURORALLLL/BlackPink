#ifndef _MOTION_PICK_H__
#define _MOTION_PICK_H__

/**Include Header Files**/
#include "sysconfig.h"

#include "Motion_Motor.h"

#define Lift_Angle_Init					(0)
#define Lift_Angle_Silver				(-315)
#define Lift_Angle_Gold					(-340)
#define Lift_Angle_Safe					(-705)
#define Lift_Angle_Hero					(-805)
#define Lift_Angle_Sky					(-1195)
#define Lift_Angle_Exchange			(-405)
#define Lift_Angle_Arm_Init			(0)
#define Lift_Angle_Arm_Ground		(-155)
#define Lift_Angle_Arm_Silver		(-420)
#define Lift_Angle_Arm_Exchange	(-805)
#define Lift_Angle_Store				(-805)
#define Lift_Angle_StoreDown		(-505)
#define Lift_Angle_StoreUp			(-705)
#define Lift_Angle_Take					(-455)
#define Lift_Angle_Block				(-205)

//#define Lift_Angle_Sky	(-1104)

/**Enum Definition**/
typedef enum
{
		Lift_State_Init = 0,
		Lift_State_Silver,
		Lift_State_Gold,
		Lift_State_Safe, 
		Lift_State_Hero,
		Lift_State_Sky,
		Lift_State_Exchange,
		Lift_State_Arm_Init,
		Lift_State_Arm_Ground,
		Lift_State_Arm_Silver,
		Lift_State_Arm_Exchange,
		Lift_State_Store,
		Lift_State_StoreDown,
		Lift_State_StoreUp,
		Lift_State_Take,
		Lift_State_Block,
		Lift_State_Up,		
		Lift_State_Down,
		Lift_State_Press,
		Lift_State_Push,
		Lift_State_Counter,
} Lift_State_t;

typedef enum
{
	Gimbal_State_Front = 0,
	Gimbal_State_Back,
	Gimbal_State_Counter
} Gimbal_State_t;

/**Extern Declaration**/
extern Lift_State_t Lift_State;
extern Gimbal_State_t Gimbal_State;

extern float Lift_Angle[Lift_State_Counter];

extern float Pitch_Incline;
extern float Yaw_Incline;

extern uint8_t Lift_Motor_InPosition;
/**Function Delaration**/
void Manual_Lift_Control(void);
void Manual_InPosition(void);

#endif

