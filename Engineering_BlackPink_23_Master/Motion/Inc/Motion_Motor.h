#ifndef __MOTION_MOTOR_H_
#define __MOTION_MOTOR_H_


/**Include Header Files**/
#include "sysconfig.h"
#include "motor.h"
#include "Encoder.h"

#include "MG4005.h"
#include "DM_R_4315.h"
#include "DM_J4310.H"
#include "DIJ_Motor.h"
//#include "WT931_Gyro.h"

#include "Motion_Pick.h"
#include "Motion_Spin.h"

#include "Task_StateMachine.h"

/**Macro Definition**/
/*=======RS485======*/
#define Gimbal_Motor_id					1
#define ArmYaw_Motor_id					1
/*=======CAN1=======*/
/*StdID = 0x50*/
#define PlatformL_Encoder_id		0x50
/*StdID = 0x51*/
#define PlatformR_Encoder_id		0x51
/*StdID = 0x1FF*/
#define SkyPitchL_Motor_id			1
#define SkyPitchR_Motor_id			3
/*StdID = 0x200*/
#define PlatformL_Motor_id			2
#define PlatformR_Motor_id			4

/*=======CAN2=======*/
/*StdID = 0x100*/
#define ArmPitchL_Motor_id			5
#define ArmPitchR_Motor_id			6
/*StdID = 0x200*/
#define ArmRoll_Motor_id				1
#define SkyForwardL_Motor_id		3
#define SkyForwardR_Motor_id		4


//GM3508 Max:16384
#define PLATFORM_MAXOUT					12000
#define PLATFORM_MAXIOUT				7500
#define PLATFORM_DEADZONE				0

//GM3508 Max:16384 used for twist
#define PLATFORMTWIST_MAXOUT		2000
#define PLATFORMTWIST_MAXIOUT		2000
#define PLATFORMTWIST_DEADZONE	0

//DMR4315 Max:32767
#define GIMBAL_MAXOUT						32767
#define GIMBAL_MAXIOUT					32767
#define GIMBAL_DEADZONE					0

//GM6020 Max:30000
#define SKYPITCH_MAXOUT					25000
#define SKYPITCH_MAXIOUT				25000
#define SKYPITCH_DEADZONE				0

//GM2006 Max:10000
#define SKYFORWARD_MAXOUT				10000
#define SKYFORWARD_MAXIOUT			10000
#define SKYFORWARD_DEADZONE			0

//GM2006 Max:10000
#define SKYFORWARDTWIST_MAXOUT		10000
#define SKYFORWARDTWIST_MAXIOUT		10000
#define SKYFORWARDTWIST_DEADZONE 	0

//MG4005
#define ARMYAW_MAXOUT			2000
#define ARMYAW_MAXIOUT		2000
#define ARMYAW_DEADZONE		0

//GM2006 Max:10000
#define ARMPITCH_MAXOUT		4096
#define ARMPITCH_MAXIOUT	4096
#define ARMPITCH_DEADZONE	0

//GM2006 Max:10000
#define ARMROLL_MAXOUT		10000
#define ARMROLL_MAXIOUT		10000
#define ARMROLL_DEADZONE	0

#define FORWARD_BLOCK_KI_MAX	3000
#define LIFT_BLOCK_KI_MAX			5000

#define Platform_Twist_Init		1397

#define GG                   	1
#define OK                  	0


/**Extern Declaration**/
extern motor_t ArmYaw_Motor;
extern motor_t Gimbal_Motor;
extern Gyroscope_t Gimbal_Gyro;
/**-----------------------------**/

/**-----------CAN1--------------**/
extern motor_t PlatformL_Motor;
extern motor_t PlatformR_Motor;
extern motor_t SkyPitchL_Motor;
extern motor_t SkyPitchR_Motor;
extern Encoder_t PlatformL_Encoder;
extern Encoder_t PlatformR_Encoder;
/**-----------------------------**/

/**-----------CAN2--------------**/
extern motor_t SkyForwardL_Motor;
extern motor_t SkyForwardR_Motor;
extern motor_t ArmPitchL_Motor;
extern motor_t ArmPitchR_Motor;
extern motor_t ArmRoll_Motor;
/**-----------------------------**/

extern uint8_t Motor_Define;

extern uint8_t Auto_Block_Flag;
extern BlockState_t Lift_Motor_BlockFinish;
extern uint8_t Lift_Motor_BlockFlag;

extern uint8_t PlatformL_G;
extern uint8_t PlatformR_G;
extern uint8_t PlatformL_Encoder_G;
extern uint8_t PlatformR_Encoder_G;

extern uint8_t SkyForwardL_G;
extern uint8_t SkyForwardR_G;
extern uint8_t SkyPitchL_G;
extern uint8_t SkyPitchR_G;

extern uint8_t ArmPitchL_G;
extern uint8_t ArmPitchR_G;
extern uint8_t ArmYaw_G;
extern uint8_t ArmRoll_G;

extern uint8_t Gimbal_G;

/**Function Delaration**/
void PlatformMotor_Init(void);
void SkyMotor_Init(void);
void ArmMotor_Init(void);

void PlatformMotor_Online_Check(void);
void SkyMotor_Online_Check(void);
void ArmMotor_Online_Check(void);

void Motor_Calibrate(void);
BlockState_t ForwardMotor_BlockDetect(void);
BlockState_t LiftMotor_BlockDetect(void);
BlockState_t MotorBlockDetect(motor_t *mot,uint16_t detectperiod,const uint16_t maxblocktime);

void Set_Platform_Target(float TargetAngle);
void Set_Gimbal_Target(float TargetAngle);
void Set_SkyPitch_Target(float TargetAngle);
void Set_SkyForward_Target(float TargetAngle);
void Set_ArmPitch_Target(float TargetAngle);
void Set_ArmRoll_Target(float TargetAngle);
void Set_ArmYaw_Target(float TargetAngle);
	
void PlatformMotor_Control(void);
void GimbalMotor_Control(void);
void SkyPitchMotor_Control(void);
void SkyForwardMotor_Control(void);
void ArmRollMotor_Control(void);
	
void Angle_Limit(motor_t* Motor);
uint8_t Motor_InPosition(motor_t* Motor,Encoder_t* Encoder,float Region);
uint8_t IMU_InPosition(motor_t* Motor, float IMU_Yaw, float Region);

void Motor_G_Detect(void);

void PlatformMotor_CMD(int16_t PlatLOut, int16_t PlatROut);
void SkyMotor_CMD(int16_t SkyPitLOut, int16_t SkyPitROut);
void ArmMotor_CMD(int16_t SkyForLOut, int16_t SkyForROut, int16_t RollOut);

void Platform_Pid_Clear(void);
void Sky_Pid_Clear(void);
void Arm_Pid_Clear(void);

void Manual_Test(void);
void Manual_Control(void);

void Platform_Motor_AngleInit(void);
void ArmYaw_Motor_AngleInit(void);
void FreeWheelBlock(void);

#endif
