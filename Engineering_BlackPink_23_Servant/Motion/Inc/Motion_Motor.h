#ifndef __MOTION_MOTOR_H_
#define __MOTION_MOTOR_H_


/**Include Header Files**/
#include "sysconfig.h"
#include "Motor.h"
#include "Encoder.h"

#include "DIJ_Motor.h"


#include "Task_StateMachine.h"
#include "Task_CAN.h"


/**Macro Definition**/
#define CHASSIS_MOTOR_NUM				4
/*=======CAN2=======*/

/*StdID = 0x200*/
#define LiftL_Motor_id          1
#define LiftR_Motor_id					2

//GM3508 Max:16384
#define LIFTL_MAXOUT 				12000
#define LIFTL_MAXIOUT 			7500
#define LIFTL_DEADZONE 			0 
//GM3508 Max:16384
#define LIFTR_MAXOUT 				12000
#define LIFTR_MAXIOUT 			7500
#define LIFTR_DEADZONE 			0 
//GM3508 Max:16384 used for twist
#define LIFTTWIST_MAXOUT 		2000
#define LIFTTWIST_MAXIOUT 	2000
#define LIFTTWIST_DEADZONE 	0 

//GM3508 Max:16384
#define CHASSIS_MAXOUT			12000
#define CHASSIS_MAXIOUT			5000
#define CHASSIS_DEADZONE		0 

#define FORWARD_BLOCK_KI_MAX	4500
#define LIFT_BLOCK_KI_MAX			5000

#define GG                   1
#define OK                  0


/**Extern Declaration**/
/**-----------CAN2--------------**/
extern motor_t LiftL_Motor, LiftR_Motor;
/**-----------------------------**/
extern motor_t ChassisMotor[4];

extern uint8_t Motor_Define;

extern uint8_t LiftR_G;
extern uint8_t LiftL_G;
extern uint8_t Chassis1_G;
extern uint8_t Chassis2_G;
extern uint8_t Chassis3_G;
extern uint8_t Chassis4_G;

//¶Â×ª¼ì²â
extern uint8_t Lift_Motor_BlockFlag;
extern BlockState_t Lift_Motor_BlockFinish;

/**Function Delaration**/
void BottomMotor_Init(void);
void ChassisMotor_Init(void);

void ChassisMotor_Online_Check(void);
void LiftMotor_Online_Check(void);

void Motor_Calibrate(void);
BlockState_t ForwardMotor_BlockDetect(void);
BlockState_t LiftMotor_BlockDetect(void);
BlockState_t MotorBlockDetect(motor_t *mot,uint16_t detectperiod,const uint16_t maxblocktime);

void Set_Lift_Target(float TargetAngle);

void LiftMotor_Control(void);
void ChassisControl(void);
	
void Angle_Limit(motor_t* Motor);
uint8_t Motor_InPosition(motor_t* Motor,Encoder_t* Encoder,float Region);

void Motor_G_Detect(void);

void BottomMotor_CMD(int16_t LifLOut, int16_t LifROut);
void ChassisMotor_CMD(int16_t FLOut, int16_t FROut, int16_t BROut, int16_t BLOut);

void Bottom_Pid_Clear(void);
void Chassis_Pid_Clear(void);

void Manual_Test(void);
void Manual_Control(void);

#endif
