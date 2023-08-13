#ifndef _MOTION_PICK_H__
#define _MOTION_PICK_H__

/**Include Header Files**/
#include "sysconfig.h"

#include "Motion_Motor.h"

#include "Uart_Comm.h"

/**Macro Definition**/
#define Platform_Angle_Init 			(0)
#define Platform_Angle_AutoSilver	(150)
#define Platform_Angle_BackSilver	(80)
#define Platform_Angle_Silver			(125)
#define Platform_Angle_Gold				(235)
#define Platform_Angle_Hero				(0)
#define Platform_Angle_Sky				(450)
#define Platform_Angle_Rotate			(450)
#define Platform_Angle_Arm_Init		(0)
#define Platform_Angle_Arm_Ground	(-300)
#define Platform_Angle_Arm_Silver	(140)
#define Platform_Angle_Arm_AutoSilver	(110)
#define Platform_Angle_Arm_BackSilver	(145)
#define Platform_Angle_Arm_Store	(100)
#define Platform_Angle_Spin				(40)
#define Platform_Angle_Exchange		(-340)

#define SkyForward_Angle_Init 	(-3)
#define SkyForward_Angle_AutoSilver	(0)
#define SkyForward_Angle_BackSilver	(0)
#define SkyForward_Angle_Silver	(0)
#define SkyForward_Angle_Gold		(930)
#define SkyForward_Angle_Hero		(0)
#define SkyForward_Angle_Sky		(930)//880

#define SkyPitch_Angle_Init 	(0)
#define SkyPitch_Angle_Silver	(90)
#define SkyPitch_Angle_Gold		(90)
#define SkyPitch_Angle_Hero		(0)
#define SkyPitch_Angle_Sky		(0)
#define SkyPitch_Angle_Rotate	(140)
#define SkyPitch_Angle_Plane	(90)

#define ArmPitch_Angle_Init				(0)
#define ArmPitch_Angle_Hold				(-45)
#define ArmPitch_Angle_Ground			(20)
#define ArmPitch_Angle_Silver			(-70)
#define ArmPitch_Angle_Sky				(-75)
#define ArmPitch_Angle_Store			(28)
#define ArmPitch_Angle_Exchange		(-75)
#define ArmPitch_Angle_Protect		(-75)

#define ArmYaw_Angle_Init				(0)

#define Gimbal_Angle_Front		(32)
#define Gimbal_Angle_Back			(215)

/**Enum Definition**/
typedef enum
{
		PCState_Protect = 0,				/*PC状态——保护模式*/
		PCState_Pick_Init,
		PCState_Pick_Silver,
		PCState_Pick_Gold,
		PCState_Pick_Hero,
		PCState_Pick_Sky,
		PCState_Pick_Exchange,
		PCState_Pick_Arm_Init,
		PCState_Pick_Arm_Ground,
		PCState_Pick_Arm_Silver,
		PCState_Counter							/*无意义，用来计数到底几个状态*/
} PCState_t;

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
		Platform_State_Init = 0,
		Platform_State_AutoSilver,
		Platform_State_BackSilver,
		Platform_State_Silver,
		Platform_State_Gold,
		Platform_State_Hero,
		Platform_State_Sky,
		Platform_State_Rotate,
	  Platform_State_Arm_Init,
		Platform_State_Arm_Ground,
		Platform_State_Arm_Silver,
		Platform_State_Arm_AutoSilver,
		Platform_State_Arm_BackSilver,
		Platform_State_Arm_Store,
		Platform_State_Spin,
		Platform_State_Forward,
		Platform_State_Backward,
		Platform_State_Counter,
} Platform_State_t;

typedef enum
{
		SkyForward_State_Init = 0,
		SkyForward_State_AutoSilver,
		SkyForward_State_BackSilver,
		SkyForward_State_Silver,	
		SkyForward_State_Gold,	
		SkyForward_State_Hero,	
		SkyForward_State_Sky,	
		SkyForward_State_Forward,
		SkyForward_State_Backward,
		SkyForward_State_Counter,	
} SkyForward_State_t;

typedef enum
{
		SkyPitch_State_Init = 0,
		SkyPitch_State_Silver,	
		SkyPitch_State_Gold,	
		SkyPitch_State_Hero,
		SkyPitch_State_Sky,
		SkyPitch_State_Rotate,
		SkyPitch_State_Plane,		
		SkyPitch_State_Counter,
} SkyPitch_State_t;

typedef enum
{
		ArmPitch_State_Init = 0,
		ArmPitch_State_Hold,
		ArmPitch_State_Ground,
		ArmPitch_State_Silver,
		ArmPitch_State_Sky,		
		ArmPitch_State_Store,
		ArmPitch_State_Exchange,
		ArmPitch_State_Protect,
		ArmPitch_State_Up,
		ArmPitch_State_Down,
		ArmPitch_State_Counter,
} ArmPitch_State_t;

typedef enum
{
		ArmYaw_State_Init = 0,
		ArmYaw_State_Counter,
} ArmYaw_State_t;

typedef enum
{
		Sucker_Off = 0,
		Sucker_On,
} Sucker_State_t;

typedef enum
{
		Gimbal_State_Front = 0,
		Gimbal_State_Back,
		Gimbal_State_Counter,
} Gimbal_State_t;


/**Extern Declaration**/
extern PCState_t PCState;
extern Lift_State_t Lift_State;
extern Platform_State_t Platform_State;
extern SkyForward_State_t SkyForward_State;
extern SkyPitch_State_t SkyPitch_State;
extern ArmPitch_State_t ArmPitch_State;
extern ArmYaw_State_t ArmYaw_State;

extern Sucker_State_t Arm_Sucker_State;
extern Sucker_State_t Sky_Sucker_State;
extern Gimbal_State_t Gimbal_State;

extern uint8_t Auto_Protect;
extern uint8_t Auto_Sky_Flag;
extern uint8_t Auto_Hold_Flag;
extern uint8_t Auto_Back_Flag;
extern uint8_t Auto_Store_Flag;
extern uint8_t Auto_Take_Flag;
extern uint8_t Auto_Silver_Flag;
extern uint8_t Auto_Rotate_Flag;
extern uint8_t Auto_Chassis_Flag;
extern uint8_t Auto_Position_Flag;

extern float Platform_Angle[Platform_State_Counter];
extern float SkyForward_Angle[SkyForward_State_Counter];
extern float SkyPitch_Angle[SkyPitch_State_Counter];
extern float ArmPitch_Angle[ArmPitch_State_Counter];
extern float ArmYaw_Angle[ArmYaw_State_Counter];

extern float Gimbal_Angle[Gimbal_State_Counter];

extern uint8_t LiftCounter;
extern uint8_t WaitCounter;

/**Function Delaration**/
//空接臂手动流程
void Manual_Lift_Pick_Sky(void);
void Manual_Out_Pick_Sky(void);
void Manual_Press_Pick_Sky(void);
void Manual_Init_Pick_Sky(void);
void Manual_Suck_Sky(void);

//兑换臂手动流程
void Manual_Lift_Pick_Arm(void);
void Manual_Out_Pick_Arm(void);
void Manual_Press_Pick_Arm(void);
void Manual_Init_Pick_Arm(void);
void Manual_Suck_Arm(void);

//空接臂自动流程
void Auto_Hold_Sky(void);
void Auto_Hold_Silver_Sky(void);
void Auto_Hold_Gold_Sky(void);
void Auto_Sky_Sky(void);
void Auto_Rotate_Sky(void);

//兑换臂自动流程
void Auto_HoldBackStore_Arm(void);
void Auto_Hold_Ground_Arm(void);
void Auto_Hold_Silver_Arm(void);
void Auto_Back_Arm(void);
void Auto_Store_Arm(void);
void Auto_Take_Arm(void);
void Auto_Pick_Silver_Arm(void);

void Exchange_to_Pick_Sky(void);
void Exchange_to_Pick_Arm(void);


uint8_t State_Confirm(Lift_State_t Expected_Lift_State, Platform_State_t Expected_Platform_State, SkyForward_State_t Expected_SkyForward_State);

void Sucker_StateChange(void);
void FreeWheelBlock(void);

#endif

