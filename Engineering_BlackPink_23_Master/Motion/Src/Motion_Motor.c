/**Include Header Files**/
//电机、编码器定义系列函数
#include "Motion_Motor.h"


/**Variable Definition**/
/*MOTOR and ENCODER*/
/**-----------UART--------------**/
motor_t ArmYaw_Motor;
motor_t Gimbal_Motor;
/**-----------------------------**/

/**-----------CAN1--------------**/
motor_t PlatformL_Motor, PlatformR_Motor;
motor_t SkyPitchL_Motor, SkyPitchR_Motor;

Encoder_t PlatformL_Encoder, PlatformR_Encoder;
Gyroscope_t Gimbal_Gyro;
/**-----------------------------**/

/**-----------CAN2--------------**/
motor_t SkyForwardL_Motor, SkyForwardR_Motor;
motor_t ArmPitchL_Motor, ArmPitchR_Motor;
motor_t ArmRoll_Motor;
/**-----------------------------**/

/*PID*/
pid_type PlatformL_Speed_Pid, PlatformL_Position_Pid;
pid_type PlatformR_Speed_Pid, PlatformR_Position_Pid;
pid_type PlatformL_Twist_Pid, PlatformR_Twist_Pid;

pid_type Gimbal_Speed_Pid, Gimbal_Position_Pid;

pid_type SkyPitchL_Speed_Pid, SkyPitchL_Position_Pid;
pid_type SkyPitchR_Speed_Pid, SkyPitchR_Position_Pid;

pid_type SkyForwardL_Speed_Pid, SkyForwardL_Position_Pid;
pid_type SkyForwardR_Speed_Pid, SkyForwardR_Position_Pid;
pid_type SkyForwardL_Twist_Pid, SkyForwardR_Twist_Pid;

pid_type ArmYaw_Speed_Pid, ArmYaw_Position_Pid;

pid_type ArmPitchL_Speed_Pid, ArmPitchL_Position_Pid;
pid_type ArmPitchR_Speed_Pid, ArmPitchR_Position_Pid;

pid_type ArmRoll_Speed_Pid, ArmRoll_Position_Pid;

//堵转检测
uint8_t Auto_Block_Flag = 0;
BlockState_t Lift_Motor_BlockFinish;
uint8_t Lift_Motor_BlockFlag;

/*上线检测量*/
uint8_t PlatformMotor_Online = 0;
uint8_t ArmMotor_Online = 0;
uint8_t SkyMotor_Online = 0;
uint8_t Motor_Define = 0;

//float TwistAngle = 0;
//float AngleMax = 0;
uint8_t UIdebug = 0;

uint8_t ArmYawInit = 2;

//掉线检测量
uint32_t Last_FrameCounter[14]={0};

uint8_t PlatformL_G = 0;
uint8_t PlatformR_G = 0;
uint8_t PlatformL_Encoder_G = 0;
uint8_t PlatformR_Encoder_G = 0;

uint8_t Gimbal_G = 0;

uint8_t SkyForwardL_G = 0;
uint8_t SkyForwardR_G = 0;
uint8_t SkyPitchL_G = 0;
uint8_t SkyPitchR_G = 0;

uint8_t ArmPitchL_G = 0;
uint8_t ArmPitchR_G = 0;
uint8_t ArmYaw_G = 0;
uint8_t ArmRoll_G = 0;

/**Macro Definition**/
#define MOTION_MOTOR_INTERVAL (10)

//#define PLATFORM_NO_PID
#define PLATFORM_ONLINE_CHECK

//#define SKY_NO_PID
#define SKY_ONLINE_CHECK

//#define ARM_NO_PID
#define ARM_ONLINE_CHECK


/**Const Parameter Definition**/
#ifndef PLATFORM_NO_PID
	const float PlatformLSpeedPIDparameter[3] = {19, 0.1, 0};
	const float PlatformLPosPIDparameter[3] = {60, 0, 20};
		
	const float PlatformRSpeedPIDparameter[3] = {19, 0.1, 0};
	const float PlatformRPosPIDparameter[3] = {60, 0, 20};
	
	const float PlatformLTwistPIDparameter[3] = {0, 0, 0};
	const float PlatformRTwistPIDparameter[3] = {0, 0, 0};
	
	
	const float GimbalSpeedPIDparameter[3] = {140, 0, 0};//140,0,0//100
	const float GimbalPosPIDparameter[3] = {3, 0, 0};//3,0,0//6
#endif
		
#ifdef PLATFORM_NO_PID
	const float PlatformLSpeedPIDparameter[3] = {0, 0, 0};
	const float PlatformLPosPIDparameter[3] = {0, 0, 0};
		
	const float PlatformRSpeedPIDparameter[3] = {0, 0, 0};
	const float PlatformRPosPIDparameter[3] = {0, 0, 0};
	
	const float PlatformLTwistPIDparameter[3] = {0, 0, 0};
	const float PlatformRTwistPIDparameter[3] = {0, 0, 0};
	
	
	const float GimbalSpeedPIDparameter[3] = {0, 0, 0};
	const float GimbalPosPIDparameter[3] = {0, 0, 0};
#endif

	
#ifndef SKY_NO_PID
//	const float SkyPitchLSpeedPIDparameter[3] = {80, 0, 0};//80, 0.022, 0
//	const float SkyPitchLPosPIDparameter[3] = {7, 0, 0};//7, 0, 0
//		
//	const float SkyPitchRSpeedPIDparameter[3] = {80, 0, 0};
//	const float SkyPitchRPosPIDparameter[3] = {7, 0, 0};

	const float SkyPitchLSpeedPIDparameter[3] = {60, 0.1, 0};//65, 0.02, 0//80, 0.022, 0
	const float SkyPitchLPosPIDparameter[3] = {17, 0, 8};//6, 0, 0//7, 0, 0
		
	const float SkyPitchRSpeedPIDparameter[3] = {60, 0.1, 0};
	const float SkyPitchRPosPIDparameter[3] = {17, 0, 8};

	
//	const float SkyForwardLSpeedPIDparameter[3] = {15, 0.009, 0};
//	const float SkyForwardLPosPIDparameter[3] = {41, 0, 0.016};
//		
//	const float SkyForwardRSpeedPIDparameter[3] = {15, 0.009, 0};
//	const float SkyForwardRPosPIDparameter[3] = {41, 0, 0.016};
	
	const float SkyForwardLSpeedPIDparameter[3] = {7, 0, 0};
	const float SkyForwardLPosPIDparameter[3] = {60, 0, 0};
		
	const float SkyForwardRSpeedPIDparameter[3] = {7, 0, 0};
	const float SkyForwardRPosPIDparameter[3] = {60, 0, 0};

	const float SkyForwardLTwistPIDparameter[3] = {0, 0, 0};
	const float SkyForwardRTwistPIDparameter[3] = {0, 0, 0};
#endif
	
#ifdef SKY_NO_PID
	const float SkyPitchLSpeedPIDparameter[3] = {0, 0, 0};
	const float SkyPitchLPosPIDparameter[3] = {0, 0, 0};
		
	const float SkyPitchRSpeedPIDparameter[3] = {0, 0, 0};
	const float SkyPitchRPosPIDparameter[3] = {0, 0, 0};

	
	const float SkyForwardLSpeedPIDparameter[3] = {0, 0, 0};
	const float SkyForwardLPosPIDparameter[3] = {0, 0, 0};
		
	const float SkyForwardRSpeedPIDparameter[3] = {0, 0, 0};
	const float SkyForwardRPosPIDparameter[3] = {0, 0, 0};

	const float SkyForwardLTwistPIDparameter[3] = {0, 0, 0};
	const float SkyForwardRTwistPIDparameter[3] = {0, 0, 0};
#endif	
	
	
#ifndef ARM_NO_PID
	const float ArmYawSpeedPIDparameter[3] = {0, 0, 0};
	const float ArmYawPosPIDparameter[3] = {0, 0, 0};
		
	const float ArmPitchLSpeedPIDparameter[3] = {0, 0, 0};
	const float ArmPitchLPosPIDparameter[3] = {0, 0, 0};
	
	const float ArmPitchRSpeedPIDparameter[3] = {0, 0, 0};
	const float ArmPitchRPosPIDparameter[3] = {0, 0, 0};

	const float ArmRollSpeedPIDparameter[3] = {7, 0.006, 0};
	const float ArmRollPosPIDparameter[3] = {28, 0, 0};
#endif

#ifdef ARM_NO_PID
	const float ArmYawSpeedPIDparameter[3] = {0, 0, 0};
	const float ArmYawPosPIDparameter[3] = {0, 0, 0};
		
	const float ArmPitchLSpeedPIDparameter[3] = {0, 0, 0};
	const float ArmPitchLPosPIDparameter[3] = {0, 0, 0};
	
	const float ArmPitchRSpeedPIDparameter[3] = {0, 0, 0};
	const float ArmPitchRPosPIDparameter[3] = {0, 0, 0};

	const float ArmRollSpeedPIDparameter[3] = {0, 0, 0};
	const float ArmRollPosPIDparameter[3] = {0, 0, 0};
#endif		

/**Function Declaration**/
/**
 * @brief  Initialize Platform and Gimbal Motor
 * @note
 * @param
 * @retval
 */
void PlatformMotor_Init(void)
{	
	
		MotortypeRegister(&PlatformL_Motor, MOTOR_3508, &PlatformL_Position_Pid, &PlatformL_Speed_Pid, MOTOR3508_Reductionratio, PlatformL_Motor_id);
    PID_Parameter_Init(PlatformL_Motor.PositionPID, PID_POSITION, PlatformLPosPIDparameter, 1000, PLATFORM_MAXIOUT, PLATFORM_DEADZONE);
    PID_Parameter_Init(PlatformL_Motor.SpeedPID, PID_POSITION, PlatformLSpeedPIDparameter, PLATFORM_MAXOUT, PLATFORM_MAXIOUT, PLATFORM_DEADZONE);
		
		MotortypeRegister(&PlatformR_Motor, MOTOR_3508, &PlatformR_Position_Pid, &PlatformR_Speed_Pid, MOTOR3508_Reductionratio, PlatformR_Motor_id);
    PID_Parameter_Init(PlatformR_Motor.PositionPID, PID_POSITION, PlatformRPosPIDparameter, 1000, PLATFORM_MAXIOUT, PLATFORM_DEADZONE);
    PID_Parameter_Init(PlatformR_Motor.SpeedPID, PID_POSITION, PlatformRSpeedPIDparameter, PLATFORM_MAXOUT, PLATFORM_MAXIOUT, PLATFORM_DEADZONE);
	
		PID_Parameter_Init(&PlatformL_Twist_Pid, PID_POSITION, PlatformLTwistPIDparameter, PLATFORMTWIST_MAXOUT, PLATFORMTWIST_MAXIOUT, PLATFORMTWIST_DEADZONE);
		PID_Parameter_Init(&PlatformR_Twist_Pid, PID_POSITION, PlatformRTwistPIDparameter, PLATFORMTWIST_MAXOUT, PLATFORMTWIST_MAXIOUT, PLATFORMTWIST_DEADZONE);

		
		MotortypeRegister(&Gimbal_Motor, MOTOR_4315, &Gimbal_Position_Pid, &Gimbal_Speed_Pid, MOTOR4315_Reductionratio, Gimbal_Motor_id);
		PID_Parameter_Init(Gimbal_Motor.PositionPID, PID_POSITION, GimbalPosPIDparameter, GIMBAL_MAXOUT, GIMBAL_MAXIOUT, GIMBAL_DEADZONE);
		PID_Parameter_Init(Gimbal_Motor.SpeedPID, PID_POSITION, GimbalSpeedPIDparameter, GIMBAL_MAXOUT, GIMBAL_MAXIOUT, GIMBAL_DEADZONE);		
	
		
#ifdef PLATFORM_ONLINE_CHECK
    PlatformMotor_Online_Check();
#endif

		//Platform电机理想初始位置
		
//		PlatformL_Motor.InitAngle = 889;
//		PlatformL_Motor.Targetcirnum = 2;
//    PlatformL_Motor.TargetAngle = 169;
//		
//		PlatformR_Motor.InitAngle = -508;
//		PlatformR_Motor.Targetcirnum = -2;
//    PlatformR_Motor.TargetAngle = 212;

		
		PlatformL_Motor.Targetcirnum = PlatformL_Encoder.Realcirnum;
    PlatformL_Motor.TargetAngle = 110;
		PlatformL_Motor.InitAngle = PlatformL_Encoder.Realcirnum *360 + PlatformL_Motor.TargetAngle;
		
		PlatformR_Motor.Targetcirnum = PlatformR_Encoder.Realcirnum;
    PlatformR_Motor.TargetAngle = 255;
		PlatformR_Motor.InitAngle = PlatformR_Encoder.Realcirnum *360 + PlatformR_Motor.TargetAngle;
		
		//Gimbal电机理想初始位置
		Gimbal_Motor.InitAngle = 34;
    Gimbal_Motor.TargetAngle = Gimbal_Motor.InitAngle;
    Gimbal_Motor.Targetcirnum = 0;
    Gimbal_Motor.Realcirnum = 0;
    Gimbal_Motor.Oricirnum = 0;
}

/**
 * @brief  Initialize Sky Motor
 * @note
 * @param
 * @retval
 */
void SkyMotor_Init(void)
{

		MotortypeRegister(&SkyPitchL_Motor, MOTOR_6020, &SkyPitchL_Position_Pid, &SkyPitchL_Speed_Pid, MOTOR6020_Reductionratio, SkyPitchL_Motor_id);
    PID_Parameter_Init(SkyPitchL_Motor.PositionPID, PID_POSITION, SkyPitchLPosPIDparameter, 300, SKYPITCH_MAXIOUT, SKYPITCH_DEADZONE);
    PID_Parameter_Init(SkyPitchL_Motor.SpeedPID, PID_POSITION, SkyPitchLSpeedPIDparameter, SKYPITCH_MAXOUT, SKYPITCH_MAXIOUT, SKYPITCH_DEADZONE);
		
		MotortypeRegister(&SkyPitchR_Motor, MOTOR_6020, &SkyPitchR_Position_Pid, &SkyPitchR_Speed_Pid, MOTOR6020_Reductionratio, SkyPitchR_Motor_id);
    PID_Parameter_Init(SkyPitchR_Motor.PositionPID, PID_POSITION, SkyPitchRPosPIDparameter, 300, SKYPITCH_MAXIOUT, SKYPITCH_DEADZONE);
    PID_Parameter_Init(SkyPitchR_Motor.SpeedPID, PID_POSITION, SkyPitchRSpeedPIDparameter, SKYPITCH_MAXOUT, SKYPITCH_MAXIOUT, SKYPITCH_DEADZONE);
		
		
		MotortypeRegister(&SkyForwardL_Motor, MOTOR_2006, &SkyForwardL_Position_Pid, &SkyForwardL_Speed_Pid, MOTOR2006_Reductionratio, SkyForwardL_Motor_id);
    PID_Parameter_Init(SkyForwardL_Motor.PositionPID, PID_POSITION, SkyForwardLPosPIDparameter, 3800, SKYFORWARD_MAXIOUT, SKYFORWARD_DEADZONE);
    PID_Parameter_Init(SkyForwardL_Motor.SpeedPID, PID_POSITION, SkyForwardLSpeedPIDparameter, SKYFORWARD_MAXOUT, SKYFORWARD_MAXIOUT, SKYFORWARD_DEADZONE);
		
		MotortypeRegister(&SkyForwardR_Motor, MOTOR_2006, &SkyForwardR_Position_Pid, &SkyForwardR_Speed_Pid, MOTOR2006_Reductionratio, SkyForwardR_Motor_id);
    PID_Parameter_Init(SkyForwardR_Motor.PositionPID, PID_POSITION, SkyForwardRPosPIDparameter, 3800, SKYFORWARD_MAXIOUT, SKYFORWARD_DEADZONE);
    PID_Parameter_Init(SkyForwardR_Motor.SpeedPID, PID_POSITION, SkyForwardRSpeedPIDparameter, SKYFORWARD_MAXOUT, SKYFORWARD_MAXIOUT, SKYFORWARD_DEADZONE);
		
    PID_Parameter_Init(&SkyForwardL_Twist_Pid, PID_POSITION, SkyForwardLTwistPIDparameter, SKYFORWARDTWIST_MAXOUT, SKYFORWARDTWIST_MAXIOUT, SKYFORWARDTWIST_DEADZONE);	
		PID_Parameter_Init(&SkyForwardR_Twist_Pid, PID_POSITION, SkyForwardRTwistPIDparameter, SKYFORWARDTWIST_MAXOUT, SKYFORWARDTWIST_MAXIOUT, SKYFORWARDTWIST_DEADZONE);	

#ifdef SKY_ONLINE_CHECK
		SkyMotor_Online_Check();
#endif

		SkyPitchL_Motor.InitAngle = 239;
    SkyPitchL_Motor.TargetAngle = SkyPitchL_Motor.RealAngle;
    SkyPitchL_Motor.Targetcirnum = 0;
    SkyPitchL_Motor.Realcirnum = 0;
    SkyPitchL_Motor.Oricirnum = 0;
		
		SkyPitchR_Motor.InitAngle = 239;
    SkyPitchR_Motor.TargetAngle = SkyPitchR_Motor.RealAngle;
    SkyPitchR_Motor.Targetcirnum = 0;
    SkyPitchR_Motor.Realcirnum = 0;
    SkyPitchR_Motor.Oricirnum = 0;
		
		SkyForwardL_Motor.InitAngle = SkyForwardL_Motor.RealAngle;
    SkyForwardL_Motor.TargetAngle = SkyForwardL_Motor.InitAngle;
    SkyForwardL_Motor.Targetcirnum = 0;
    SkyForwardL_Motor.Realcirnum = 0;
    SkyForwardL_Motor.Oricirnum = 0;
		
		SkyForwardR_Motor.InitAngle = SkyForwardR_Motor.RealAngle;
    SkyForwardR_Motor.TargetAngle = SkyForwardR_Motor.InitAngle;
    SkyForwardR_Motor.Targetcirnum = 0;
    SkyForwardR_Motor.Realcirnum = 0;
    SkyForwardR_Motor.Oricirnum = 0;

}

/**
 * @brief  Initialize Arm Motor
 * @note
 * @param
 * @retval
 */
void ArmMotor_Init(void)
{	

		MotortypeRegister(&ArmYaw_Motor, MOTOR_MG4005, NULL, NULL, MOTORMG4005_Reductionratio, ArmYaw_Motor_id);
    //PID_Parameter_Init(ArmYaw_Motor.PositionPID, PID_POSITION, ArmYawPosPIDparameter, ARMYAW_MAXOUT, ARMYAW_MAXIOUT, ARMYAW_DEADZONE);
    //PID_Parameter_Init(ArmYaw_Motor.SpeedPID, PID_POSITION, ArmYawSpeedPIDparameter, ARMYAW_MAXOUT, ARMYAW_MAXIOUT, ARMYAW_DEADZONE);
		

		MotortypeRegister(&ArmPitchL_Motor, MOTOR_4310, NULL, NULL, MOTOR4310_Reductionratio, ArmPitchL_Motor_id);
    //PID_Parameter_Init(ArmPitchL_Motor.PositionPID, PID_POSITION, ArmPitchLPosPIDparameter, ARMPITCH_MAXOUT, ARMPITCH_MAXIOUT, ARMPITCH_DEADZONE);
    //PID_Parameter_Init(ArmPitchL_Motor.SpeedPID, PID_POSITION, ArmPitchLSpeedPIDparameter, ARMPITCH_MAXOUT, ARMPITCH_MAXIOUT, ARMPITCH_DEADZONE);
	
		MotortypeRegister(&ArmPitchR_Motor, MOTOR_4310, NULL, NULL, MOTOR4310_Reductionratio, ArmPitchR_Motor_id);
    //PID_Parameter_Init(ArmPitchR_Motor.PositionPID, PID_POSITION, ArmPitchRPosPIDparameter, ARMPITCH_MAXOUT, ARMPITCH_MAXIOUT, ARMPITCH_DEADZONE);
    //PID_Parameter_Init(ArmPitchR_Motor.SpeedPID, PID_POSITION, ArmPitchRSpeedPIDparameter, ARMPITCH_MAXOUT, ARMPITCH_MAXIOUT, ARMPITCH_DEADZONE);

	
		MotortypeRegister(&ArmRoll_Motor, MOTOR_2006, &ArmRoll_Position_Pid, &ArmRoll_Speed_Pid, MOTOR2006_Reductionratio, ArmRoll_Motor_id);
		PID_Parameter_Init(ArmRoll_Motor.PositionPID, PID_POSITION, ArmRollPosPIDparameter, ARMROLL_MAXOUT, ARMROLL_MAXIOUT, ARMROLL_DEADZONE);
		PID_Parameter_Init(ArmRoll_Motor.SpeedPID, PID_POSITION, ArmRollSpeedPIDparameter, ARMROLL_MAXOUT, ARMROLL_MAXIOUT, ARMROLL_DEADZONE);		
		
#ifdef ARM_ONLINE_CHECK
    ArmMotor_Online_Check();
#endif

		ArmYaw_Motor.InitAngle = 5;
    ArmYaw_Motor.TargetAngle = ArmYaw_Motor.InitAngle;
    ArmYaw_Motor.Targetcirnum = 0;
    ArmYaw_Motor.Realcirnum = 0;
    ArmYaw_Motor.Oricirnum = 0;
		
		ArmPitchL_Motor.InitAngle = 60;
    ArmPitchL_Motor.TargetAngle = ArmPitchL_Motor.InitAngle;
    ArmPitchL_Motor.Targetcirnum = 0;
    ArmPitchL_Motor.Realcirnum = 0;
    ArmPitchL_Motor.Oricirnum = 0;
		
		ArmPitchR_Motor.InitAngle = -98;
    ArmPitchR_Motor.TargetAngle = 262;
    ArmPitchR_Motor.Targetcirnum = -1;
    ArmPitchR_Motor.Realcirnum = 0;
    ArmPitchR_Motor.Oricirnum = 0;
		
		ArmRoll_Motor.InitAngle = ArmRoll_Motor.RealAngle;
    ArmRoll_Motor.TargetAngle = ArmRoll_Motor.InitAngle;
    ArmRoll_Motor.Targetcirnum = 0;
    ArmRoll_Motor.Realcirnum = 0;
    ArmRoll_Motor.Oricirnum = 0;

}


/**
 * @brief  PlatformMotor Online Check
 * @note   Check Platform Motor and Platform Encoder are online or not
 * @param
 * @retval
 */
void PlatformMotor_Online_Check(void)
{
		TickType_t xLastWakeUpTime;
		xLastWakeUpTime = xTaskGetTickCount();
		static uint16_t Online_Flag = 0;
		
		while (PlatformL_Motor.FeedbackData.FrameCounter == 0 && Online_Flag <= 1000)
		{
				PlatformMotor_Online = 0;
				Online_Flag++;
				vTaskDelayUntil(&xLastWakeUpTime, MOTION_MOTOR_INTERVAL);
		}
		if(Online_Flag > 990)
		{
				PlatformL_G = GG;
				UIdebug++;
		}
		Online_Flag = 0;
	
		
		while (PlatformR_Motor.FeedbackData.FrameCounter == 0 && Online_Flag <= 1000)
		{
				PlatformMotor_Online = 0;
				Online_Flag++;
				vTaskDelayUntil(&xLastWakeUpTime, MOTION_MOTOR_INTERVAL);
		}
		if(Online_Flag > 990)
		{
				PlatformR_G = GG;
				UIdebug++;
		}
		Online_Flag = 0;
		

		while (PlatformL_Encoder.FrameCounter == 0 && Online_Flag <= 1000)
		{
				PlatformMotor_Online = 0;
				Online_Flag++;
				vTaskDelayUntil(&xLastWakeUpTime, MOTION_MOTOR_INTERVAL);
		}
		if(Online_Flag > 990)
		{
				PlatformL_Encoder_G = GG;
				UIdebug++;
		}
		Online_Flag = 0;
	
	
		while (PlatformR_Encoder.FrameCounter == 0 && Online_Flag <= 1000)
		{
				PlatformMotor_Online = 0;
				Online_Flag++;
				vTaskDelayUntil(&xLastWakeUpTime, MOTION_MOTOR_INTERVAL);
		}
		if(Online_Flag > 990)
		{
				PlatformR_Encoder_G = GG;
				UIdebug++;
		}
		Online_Flag = 0;
		
	
//		while (Gimbal_Motor.FeedbackData.FrameCounter == 0 && Online_Flag <= 1000)
//		{
//				PlatformMotor_Online = 0;
//				Online_Flag++;
//				vTaskDelayUntil(&xLastWakeUpTime, MOTION_MOTOR_INTERVAL);
//		}
//		if(Online_Flag>990)
//		{
//				Gimbal_G = GG;
//				UIdebug++;
//		}
//		Online_Flag = 0;
	
		
		PlatformMotor_Online = 1;
}


/**
 * @brief  SkyMotor Online Check
 * @note   Check Sky Motor is online or not
 * @param
 * @retval
 */
void SkyMotor_Online_Check(void)
{
		TickType_t xLastWakeUpTime;
		xLastWakeUpTime = xTaskGetTickCount();
		static uint16_t Online_Flag = 0;
		
		while (SkyPitchL_Motor.FeedbackData.FrameCounter == 0 && Online_Flag <= 700)
		{
				SkyMotor_Online = 0;
				Online_Flag++;
				vTaskDelayUntil(&xLastWakeUpTime, MOTION_MOTOR_INTERVAL);
		}
		if(Online_Flag > 690)
		{
				SkyPitchL_G = GG;
		}
		Online_Flag = 0;
		
		
		while (SkyPitchR_Motor.FeedbackData.FrameCounter == 0 && Online_Flag <= 700)
		{
				SkyMotor_Online = 0;
				Online_Flag++;
				vTaskDelayUntil(&xLastWakeUpTime, MOTION_MOTOR_INTERVAL);
		}
		if(Online_Flag > 690)
		{
				SkyPitchR_G = GG;
		}
		Online_Flag = 0;
		
		
		while (SkyForwardL_Motor.FeedbackData.FrameCounter == 0 && Online_Flag <= 700)
		{
				SkyMotor_Online = 0;
				Online_Flag++;
				vTaskDelayUntil(&xLastWakeUpTime, MOTION_MOTOR_INTERVAL);
		}
		if(Online_Flag > 690)
		{
				SkyForwardL_G = GG;
		}
		Online_Flag = 0;
		
		
		while (SkyForwardR_Motor.FeedbackData.FrameCounter == 0 && Online_Flag <= 700)
		{
				SkyMotor_Online = 0;
				Online_Flag++;
				vTaskDelayUntil(&xLastWakeUpTime, MOTION_MOTOR_INTERVAL);
		}
		if(Online_Flag > 690)
		{
				SkyForwardR_G = GG;
		}
		Online_Flag = 0;
		
		SkyMotor_Online = 1;
}



/**
 * @brief  ArmMotor Online Check
 * @note   Check Lift Motor and Lift Encoder are online or not
 * @param
 * @retval
 */
void ArmMotor_Online_Check(void)
{
		TickType_t xLastWakeUpTime;
		xLastWakeUpTime = xTaskGetTickCount();
		static uint16_t Online_Flag = 0;
		
//		while (ArmYaw_Motor.FeedbackData.FrameCounter == 0 && Online_Flag <= 700)
//		{
//			ArmMotor_Online = 0;
//			Online_Flag++;
//			vTaskDelayUntil(&xLastWakeUpTime, MOTION_MOTOR_INTERVAL);
//		}
//		if(Online_Flag > 695)
//		{
//				ArmYaw_G = GG;
//		}	
//		Online_Flag = 0;
//		
//		while (ArmPitchL_Motor.FeedbackData.FrameCounter == 0 && Online_Flag <= 700)
//		{
//			ArmMotor_Online = 0;
//			Online_Flag++;
//			vTaskDelayUntil(&xLastWakeUpTime, MOTION_MOTOR_INTERVAL);
//		}
//		if(Online_Flag > 695)
//		{
//				ArmPitchL_G = GG;
//		}	
//		Online_Flag = 0;
//		
//		while (ArmPitchR_Motor.FeedbackData.FrameCounter == 0 && Online_Flag <= 700)
//		{
//			ArmMotor_Online = 0;
//			Online_Flag++;
//			vTaskDelayUntil(&xLastWakeUpTime, MOTION_MOTOR_INTERVAL);
//		}
//		if(Online_Flag > 695)
//		{
//				ArmPitchR_G = GG;
//		}	
//		Online_Flag = 0;

		while (ArmRoll_Motor.FeedbackData.FrameCounter == 0 && Online_Flag <= 700)
		{
			ArmMotor_Online = 0;
			Online_Flag++;
			vTaskDelayUntil(&xLastWakeUpTime, MOTION_MOTOR_INTERVAL);
		}
		if(Online_Flag > 695)
		{
				ArmRoll_G = GG;
		}
		Online_Flag = 0;
		
		
		ArmMotor_Online = 1;
}

/**
 * @brief  电机堵转校准
 * @note	 只在第一次从保护切出时才会运行
 * @param	 抬升抬起，前伸堵转，抬升堵转
 * @retval 
 */
void Motor_Calibrate(void)
{
		//抬升抬起
		if(Lift_State == Lift_State_Init && ForwardMotor_BlockDetect() == noblock)
    {
				Lift_State = Lift_State_Block;
			
				Platform_State = Platform_State_Init;
				ArmPitch_State = ArmPitch_State_Protect;
			
				Set_Platform_Target(Platform_Angle[Platform_State]);
				Set_ArmPitch_Target(ArmPitch_Angle[ArmPitch_State]);
    }
		
		//前伸堵转
		else if(Lift_State == Lift_State_Block && ForwardMotor_BlockDetect() == noblock)
    {
				SkyForwardL_Motor.Targetrotationrate = -800;
				SkyForwardR_Motor.Targetrotationrate = 800;
    }
		
		//前伸堵转完成
		else if(Lift_State == Lift_State_Block && ForwardMotor_BlockDetect() == isblock)
		{
				Lift_State = Lift_State_Init;
			
				//设置前伸长度
				SkyForward_State = SkyForward_State_Init;
				Set_SkyForward_Target(SkyForward_Angle[SkyForward_State]);
		}
		//抬升堵转完成
		else if(Lift_State == Lift_State_Init && ForwardMotor_BlockDetect() == isblock && Lift_Motor_BlockFinish == noblock)
		{
				Lift_Motor_BlockFlag = 1;
		}
		if(Lift_State == Lift_State_Init && ForwardMotor_BlockDetect() == isblock && Lift_Motor_BlockFinish == isblock)
		{
				Auto_Block_Flag = 0;
				Lift_Motor_BlockFlag = 0;
	
				if(GetPCMode() == PCMode_Pick_Sky || GetPCMode() == PCMode_Exchange_Sky)
				{
						PCState = PCState_Pick_Init;
						Lift_State = Lift_State_Init;
						LiftCounter = 0;
				}
				if(GetPCMode() == PCMode_Pick_Arm || GetPCMode() == PCMode_Exchange_Arm)
				{
						PCState = PCState_Pick_Arm_Init;
						Lift_State = Lift_State_Arm_Init;
						LiftCounter = 0;
				}

				ArmPitch_State = ArmPitch_State_Init;
				Set_ArmPitch_Target(ArmPitch_Angle[ArmPitch_State]);
		}
}

/**
 * @brief  检测前伸电机是否堵转
 * @note   左右两个电机，均要堵转
 * @param  
 * @retval 是否堵转
 */
BlockState_t ForwardMotor_BlockDetect(void)
{
		BlockState_t ForwardL_Block, ForwardR_Block;
		ForwardL_Block = MotorBlockDetect(&SkyForwardL_Motor, MOTION_MOTOR_INTERVAL, MOTOR_MAX_BLOCKTIME);
		ForwardR_Block = MotorBlockDetect(&SkyForwardR_Motor, MOTION_MOTOR_INTERVAL, MOTOR_MAX_BLOCKTIME);
		if (ForwardL_Block  == isblock && ForwardR_Block == isblock)
			return isblock;
		else
			return noblock;
}

/**
 * @brief  更新电机数据判断是否堵转
 * @note   用于更新电机堵转状态
 * @param  mot: 对应电机结构体指针；detectperiod:检测周期ms；maxblocktime:最大堵转时间ms
 * @retval 对应电机堵转状态
 */
BlockState_t MotorBlockDetect(motor_t *mot,uint16_t detectperiod,const uint16_t maxblocktime)
{
		if(mot == NULL)
		{
				return noblock;
		}
		
		if (mot == &SkyForwardL_Motor)
		{		
				if(mot->Blockstate.BlockState == noblock)
				{
						//与初始角度夹角小于90度时才开始进行堵转校准，按照电机安装方向，应用负值进行判断而不能用绝对值，因为机械臂抵抗重力时的Iout就可能达到+7000，但单方向堵转对应的应该是-7000）
						if ((mot->FeedbackData.RealSpeed > -2) && (mot->Out <= -FORWARD_BLOCK_KI_MAX))
						{
								if(mot->Blockstate.BlockState == noblock)
									mot->Blockstate.Blocktime += detectperiod;
								if(mot->Blockstate.Blocktime >= maxblocktime||(SkyForwardR_Motor.Blockstate.BlockState == isblock))
								{
										mot->Blockstate.BlockState = isblock;
										
                    //mot->InitAngle = mot->RealAngle +  mot->Realcirnum*360.0f;
										//mot->TargetAngle = mot->InitAngle;
									  //Angle_Limit(mot);
										mot->InitAngle = MechanicalAngle2RealAngle(mot->FeedbackData.Mechanical_Angle[0]) / (float)(mot->Reductionratio);
										mot->TargetAngle = mot->InitAngle;
										mot->Targetcirnum = 0;
										mot->Realcirnum = 0;
										mot->Oricirnum = 0;
									
										SkyForwardR_Motor.InitAngle = MechanicalAngle2RealAngle(SkyForwardR_Motor.FeedbackData.Mechanical_Angle[0]) / (float)(SkyForwardR_Motor.Reductionratio);
										SkyForwardR_Motor.TargetAngle = SkyForwardR_Motor.InitAngle;
										SkyForwardR_Motor.Targetcirnum = 0;
										SkyForwardR_Motor.Realcirnum = 0;
										SkyForwardR_Motor.Oricirnum = 0;
								}
						}
						else
						{
							mot->Blockstate.Blocktime = 0;
							mot->Blockstate.BlockState = noblock;
						}
				}	
		}
		else if (mot == &SkyForwardR_Motor)
		{
				if(mot->Blockstate.BlockState == noblock)
				{
						if ((mot->FeedbackData.RealSpeed < 2) && (mot->Out >= FORWARD_BLOCK_KI_MAX))
						{
								if(mot->Blockstate.BlockState == noblock)
									mot->Blockstate.Blocktime += detectperiod;
								if((mot->Blockstate.Blocktime >= maxblocktime)||(SkyForwardL_Motor.Blockstate.BlockState == isblock))
								{
									mot->Blockstate.BlockState = isblock;
									
										// mot->InitAngle = mot->RealAngle +  mot->Realcirnum*360.0f;
										//mot->TargetAngle = mot->InitAngle;
									  //Angle_Limit(mot);
										mot->InitAngle = MechanicalAngle2RealAngle(mot->FeedbackData.Mechanical_Angle[0]) / (float)(mot->Reductionratio);
										mot->TargetAngle = mot->InitAngle;
										mot->Targetcirnum = 0;
										mot->Realcirnum = 0;
										mot->Oricirnum = 0;
									
									  SkyForwardL_Motor.InitAngle = MechanicalAngle2RealAngle(SkyForwardL_Motor.FeedbackData.Mechanical_Angle[0]) / (float)(SkyForwardL_Motor.Reductionratio);
									  SkyForwardL_Motor.TargetAngle = SkyForwardL_Motor.InitAngle;
									  SkyForwardL_Motor.Targetcirnum = 0;
										SkyForwardL_Motor.Realcirnum = 0;
										SkyForwardL_Motor.Oricirnum = 0;
								}
						}
						else
						{
								mot->Blockstate.Blocktime = 0;
								mot->Blockstate.BlockState = noblock;
						}
				}
		}
		
		return mot->Blockstate.BlockState;
}

/**
 * @brief  Set Platform Motor Angle 
 * @note	 
 * @param	 
 * @retval 
 */
void Set_Platform_Target(float TargetAngle)
{
    PlatformL_Motor.SpeedPID->sum_error = 0;
	  PlatformR_Motor.SpeedPID->sum_error = 0;  
	
  	PlatformL_Motor.TargetAngle = PlatformL_Motor.InitAngle + TargetAngle;
    PlatformR_Motor.TargetAngle = PlatformR_Motor.InitAngle - TargetAngle;
	
		Angle_Limit(&PlatformL_Motor);
		Angle_Limit(&PlatformR_Motor);
}

/**
 * @brief  Set Gimbal Motor Angle 
 * @note	 
 * @param	 
 * @retval 
 */
void Set_Gimbal_Target(float TargetAngle)
{
    Gimbal_Motor.SpeedPID->sum_error = 0;
  	Gimbal_Motor.TargetAngle = TargetAngle;
		Angle_Limit(&Gimbal_Motor);
}

/**
 * @brief  Set the Angle of Pitch Motor on the part of Sky 
 * @note	 
 * @param	 
 * @retval 
 */
void Set_SkyPitch_Target(float TargetAngle)
{
    SkyPitchL_Motor.SpeedPID->sum_error = 0;
	  SkyPitchR_Motor.SpeedPID->sum_error = 0;  
	
  	SkyPitchL_Motor.TargetAngle = SkyPitchL_Motor.InitAngle - TargetAngle;
    SkyPitchR_Motor.TargetAngle = SkyPitchL_Motor.InitAngle - TargetAngle;
	
		Angle_Limit(&SkyPitchL_Motor);
		Angle_Limit(&SkyPitchR_Motor);
}

/**
 * @brief  Set the Angle of Forward Motor on the part of Sky 
 * @note	 
 * @param	 
 * @retval 运动范围：884
 */
void Set_SkyForward_Target(float TargetAngle)
{
    SkyForwardL_Motor.SpeedPID->sum_error = 0;
	  SkyForwardR_Motor.SpeedPID->sum_error = 0;  
	
  	SkyForwardL_Motor.TargetAngle = SkyForwardL_Motor.InitAngle + TargetAngle;
    SkyForwardR_Motor.TargetAngle = SkyForwardR_Motor.InitAngle - TargetAngle;//目标角度的赋值也是给的相对于初始角度的偏移量
    
		Angle_Limit(&SkyForwardL_Motor);
		Angle_Limit(&SkyForwardR_Motor);
}

/**
 * @brief  Set the Angle of Pitch Motor on the part of Arm
 * @note	 
 * @param	 
 * @retval 
 */
void Set_ArmPitch_Target(float TargetAngle)
{
    ArmPitchL_Motor.SpeedPID->sum_error = 0;
	  ArmPitchR_Motor.SpeedPID->sum_error = 0;  
	
  	ArmPitchL_Motor.TargetAngle = ArmPitchL_Motor.InitAngle + TargetAngle;
    ArmPitchR_Motor.TargetAngle = ArmPitchR_Motor.InitAngle - TargetAngle;
	
		Angle_Limit(&ArmPitchL_Motor);
		Angle_Limit(&ArmPitchR_Motor);
}

/**
 * @brief  Set the Angle of Roll Motor on the part of Arm
 * @note	 
 * @param	 
 * @retval 
 */
void Set_ArmRoll_Target(float TargetAngle)
{
    ArmRoll_Motor.SpeedPID->sum_error = 0;
  	ArmRoll_Motor.TargetAngle = ArmRoll_Motor.InitAngle + TargetAngle;
		Angle_Limit(&ArmRoll_Motor);
}

/**
 * @brief  Set the Angle of Yaw Motor on the part of Arm
 * @note	 
 * @param	 
 * @retval 
 */
void Set_ArmYaw_Target(float TargetAngle)
{
    ArmYaw_Motor.SpeedPID->sum_error = 0;
  	ArmYaw_Motor.TargetAngle = ArmYaw_Motor.InitAngle + TargetAngle;
		Angle_Limit(&ArmYaw_Motor);
}


/**
 * @brief  Calculate Platform Motor Out
 * @note   
 * @param
 * @retval
 */
void PlatformMotor_Control(void)
{
		//Calculate Platform Angle
		float Dst_PlatformLAngle, Rel_PlatformLAngle;
		Dst_PlatformLAngle = PlatformL_Motor.Targetcirnum * 360.0f + PlatformL_Motor.TargetAngle;
		Rel_PlatformLAngle = PlatformL_Encoder.Realcirnum * 360.0f + PlatformL_Encoder.RealAngle;

		float Dst_PlatformRAngle, Rel_PlatformRAngle;
		Dst_PlatformRAngle = PlatformR_Motor.Targetcirnum * 360.0f + PlatformR_Motor.TargetAngle;
		Rel_PlatformRAngle = PlatformR_Encoder.Realcirnum * 360.0f + PlatformR_Encoder.RealAngle;

		//Calculate Move Speed PID
		float PlatformL_MoveSpeed, PlatformR_MoveSpeed;
		PlatformL_MoveSpeed = Pid_cal(PlatformL_Motor.PositionPID, Dst_PlatformLAngle, Rel_PlatformLAngle);
		PlatformR_MoveSpeed = Pid_cal(PlatformR_Motor.PositionPID, Dst_PlatformRAngle, Rel_PlatformRAngle);

		//Calculate Twist Angle
		float Platform_Twist_Angle;
		//可能有问题，如果要添抗扭需要修改
		Platform_Twist_Angle = fabsf(Rel_PlatformLAngle - Rel_PlatformRAngle) - Platform_Twist_Init;//大于零则左侧超前，小于零则右侧超前
		
		//Calculate Twist Speed PID
		float PlatformL_TwistSpeed, PlatformR_TwistSpeed;
		PlatformL_TwistSpeed = -Pid_cal(&PlatformL_Twist_Pid, 0, Platform_Twist_Angle);
		PlatformR_TwistSpeed = -Pid_cal(&PlatformR_Twist_Pid, 0, Platform_Twist_Angle);
		
		PlatformL_Motor.Targetrotationrate = PlatformL_MoveSpeed + PlatformL_TwistSpeed;
		PlatformR_Motor.Targetrotationrate = PlatformR_MoveSpeed + PlatformR_TwistSpeed;

		//Calculate Motor Out
		PlatformL_Motor.Out = (int16_t)(Pid_cal(PlatformL_Motor.SpeedPID, PlatformL_Motor.Targetrotationrate, PlatformL_Motor.Realrotationrate));
		PlatformR_Motor.Out = (int16_t)(Pid_cal(PlatformR_Motor.SpeedPID, PlatformR_Motor.Targetrotationrate, PlatformR_Motor.Realrotationrate));

		return;
}

/**
 * @brief  Calculate Gimbal Motor Out
 * @note   
 * @param
 * @retval
 */			
void GimbalMotor_Control(void)
{
	  float Dst_GimbalAngle, Rel_GimbalAngle;
		if(Spin_State == Spin_State_Static)
		{
//				Dst_GimbalAngle = Gimbal_Motor.Targetcirnum * 360.0f + Gimbal_Motor.TargetAngle;
//				Rel_GimbalAngle = Gimbal_Motor.Realcirnum * 360.0f + Gimbal_Motor.RealAngle;
			
				Dst_GimbalAngle = Gimbal_Motor.TargetAngle;
				Rel_GimbalAngle = Gimbal_Motor.RealAngle;
			
				
				Gimbal_Motor.Targetrotationrate = Pid_cal(Gimbal_Motor.PositionPID, Dst_GimbalAngle, Rel_GimbalAngle);
				Gimbal_Motor.Out = (int16_t)(Pid_cal(Gimbal_Motor.SpeedPID, Gimbal_Motor.Targetrotationrate, Gimbal_Motor.Realrotationrate));	
		}
		
		if(Spin_State == Spin_State_ClockWise || Spin_State == Spin_State_Follow)
		{		
				Dst_GimbalAngle = Gimbal_Motor.Targetcirnum * 360.0f + Gimbal_Motor.TargetAngle;
				Rel_GimbalAngle = Gimbal_Gyro.Realcirnum * 360 + Gimbal_Gyro.RealAngle;
			
				Gimbal_Motor.Targetrotationrate = -Pid_cal(Gimbal_Motor.PositionPID, Dst_GimbalAngle, Rel_GimbalAngle);
				Gimbal_Motor.Out = (int16_t)(Pid_cal(Gimbal_Motor.SpeedPID, Gimbal_Motor.Targetrotationrate, Gimbal_Motor.Realrotationrate));	
		}
}

/**
 * @brief  Calculate SkyPitch Motor Out
 * @note   
 * @param
 * @retval
 */			
void SkyPitchMotor_Control(void)
{
			float Dst_SkyPitchAngle, Rel_SkyPitchAngle;
			Dst_SkyPitchAngle = SkyPitchL_Motor.Targetcirnum * 360.0f + SkyPitchL_Motor.TargetAngle;
			Rel_SkyPitchAngle = SkyPitchL_Motor.Realcirnum * 360.0f + SkyPitchL_Motor.RealAngle;
		
			SkyPitchL_Motor.Targetrotationrate = Pid_cal(SkyPitchL_Motor.PositionPID, Dst_SkyPitchAngle, Rel_SkyPitchAngle);
			SkyPitchR_Motor.Targetrotationrate = -SkyPitchL_Motor.Targetrotationrate;
	
//		SkyPitchL_Motor.Out = -5000 * sin((Rel_SkyPitchAngle - 244)*3.14f / 180.0f); 
//		SkyPitchR_Motor.Out = 5000 * sin((Rel_SkyPitchAngle - 244)*3.14f / 180.0f);
	
//		SkyPitchL_Motor.Out = (int16_t)(-5000* sin((Rel_SkyPitchAngle - 239)*3.14f / 180.0f)) + (int16_t)(Pid_cal(SkyPitchL_Motor.SpeedPID, SkyPitchL_Motor.Targetrotationrate, SkyPitchL_Motor.Realrotationrate));
//		SkyPitchR_Motor.Out = (int16_t)(5000 * sin((Rel_SkyPitchAngle - 239)*3.14f / 180.0f)) + (int16_t)(Pid_cal(SkyPitchR_Motor.SpeedPID, SkyPitchR_Motor.Targetrotationrate, SkyPitchR_Motor.Realrotationrate));

			SkyPitchL_Motor.Out = (int16_t)(Pid_cal(SkyPitchL_Motor.SpeedPID, SkyPitchL_Motor.Targetrotationrate, SkyPitchL_Motor.Realrotationrate));
			SkyPitchR_Motor.Out = (int16_t)(Pid_cal(SkyPitchR_Motor.SpeedPID, SkyPitchR_Motor.Targetrotationrate, SkyPitchR_Motor.Realrotationrate));

}

/**
 * @brief  Calculate SkyForward Motor Out
 * @note   
 * @param
 * @retval
 */
void SkyForwardMotor_Control(void)
{
		//Calculate Forward Angle
	  float Dst_SkyForwardLAngle, Rel_SkyForwardLAngle;
		Dst_SkyForwardLAngle = SkyForwardL_Motor.Targetcirnum * 360.0f + SkyForwardL_Motor.TargetAngle;
		Rel_SkyForwardLAngle = SkyForwardL_Motor.Realcirnum * 360.0f + SkyForwardL_Motor.RealAngle;

		float Dst_SkyForwardRAngle, Rel_SkyForwardRAngle;
		Dst_SkyForwardRAngle = SkyForwardR_Motor.Targetcirnum * 360.0f + SkyForwardR_Motor.TargetAngle;
		Rel_SkyForwardRAngle = SkyForwardR_Motor.Realcirnum * 360.0f + SkyForwardR_Motor.RealAngle;
	
		//Calculate Move Speed PID
		float SkyForwardL_MoveSpeed, SkyForwardR_MoveSpeed;
    SkyForwardL_MoveSpeed = Pid_cal(SkyForwardL_Motor.PositionPID, Dst_SkyForwardLAngle, Rel_SkyForwardLAngle);
    SkyForwardR_MoveSpeed = Pid_cal(SkyForwardR_Motor.PositionPID, Dst_SkyForwardRAngle, Rel_SkyForwardRAngle);
	
		//Calculate Twist Angle
		float SkyForward_Twist_Angle;
    SkyForward_Twist_Angle = fabsf(Rel_SkyForwardLAngle - SkyForwardL_Motor.InitAngle) - fabsf(Rel_SkyForwardRAngle - SkyForwardR_Motor.InitAngle);//大于零则左侧超前，小于零则右侧超前
	
		//Calculate Twist Speed PID
		float SkyForwardL_TwistSpeed, SkyForwardR_TwistSpeed;
    SkyForwardL_TwistSpeed = Pid_cal(&SkyForwardL_Twist_Pid, 0, SkyForward_Twist_Angle);
    SkyForwardR_TwistSpeed = Pid_cal(&SkyForwardR_Twist_Pid, 0, SkyForward_Twist_Angle);
	
		if(ForwardMotor_BlockDetect() == isblock)
		{
				SkyForwardL_Motor.Targetrotationrate = SkyForwardL_MoveSpeed + SkyForwardL_TwistSpeed;
				SkyForwardR_Motor.Targetrotationrate = SkyForwardR_MoveSpeed + SkyForwardR_TwistSpeed;
		}

		//Calculate Motor Out
    SkyForwardL_Motor.Out = (int16_t)(Pid_cal(SkyForwardL_Motor.SpeedPID, SkyForwardL_Motor.Targetrotationrate, SkyForwardL_Motor.Realrotationrate));
    SkyForwardR_Motor.Out = (int16_t)(Pid_cal(SkyForwardR_Motor.SpeedPID, SkyForwardR_Motor.Targetrotationrate, SkyForwardR_Motor.Realrotationrate));

    return;
}

/**
 * @brief  Calculate ArmRoll Motor Out
 * @note   
 * @param
 * @retval
 */
void ArmRollMotor_Control(void)
{
		float Dst_ArmRollAngle, Rel_ArmRollAngle;
    Dst_ArmRollAngle = ArmRoll_Motor.Targetcirnum * 360.0f + ArmRoll_Motor.TargetAngle;
	  Rel_ArmRollAngle = ArmRoll_Motor.Realcirnum * 360.0f + ArmRoll_Motor.RealAngle;
	

		ArmRoll_Motor.Targetrotationrate = Pid_cal(ArmRoll_Motor.PositionPID, Dst_ArmRollAngle, Rel_ArmRollAngle);
		ArmRoll_Motor.Out = (int16_t)(Pid_cal(ArmRoll_Motor.SpeedPID, ArmRoll_Motor.Targetrotationrate, ArmRoll_Motor.Realrotationrate));

		return;
}

/**
 * @brief  Change the target angle to cirnum and angle
 * @note   720 degrees to cirnum:2 angle:0
 * @param
 * @retval
 */
void Angle_Limit(motor_t* Motor)
{
    int16_t temp_Targetcirnum = 0;
    while (Motor->TargetAngle >= 360)
    {
        Motor->TargetAngle -= 360;
        temp_Targetcirnum += 1;
    }
    while (Motor->TargetAngle < 0)
    {
        Motor->TargetAngle += 360;
        temp_Targetcirnum -= 1;
    }
    Motor->Targetcirnum = temp_Targetcirnum;
		
    return;
}

/**
 * @brief  判断电机是否到位，可否进入下一步
 * @note   
 * @param
 * @retval
 */
uint8_t Motor_InPosition(motor_t* Motor, Encoder_t* Encoder, float Region)
{
		if(Encoder == NULL)
		{
			if(fabs(Motor->Targetcirnum * 360 + Motor->TargetAngle - Motor->Realcirnum * 360 - Motor->RealAngle) < fabsf(Region))
				return 1;
			else
				return 0;
		}
		else
		{
			if(fabs(Motor->Targetcirnum * 360 + Motor->TargetAngle - Encoder->Realcirnum * 360 - Encoder->RealAngle) < fabsf(Region))
				return 1;
      else
        return 0;
		}

}


/**
 * @brief  判断电机是否到位，可否进入下一步
 * @note   
 * @param
 * @retval
 */
uint8_t IMU_InPosition(motor_t* Motor, float IMU_Yaw, float Region)
{
		if(fabs(Motor->TargetAngle - IMU_Yaw) < fabsf(Region))
				return 1;
		else
				return 0;
}



void Motor_G_Detect(void)
{
//	if(LiftL_Motor.FeedbackData.FrameCounter == Last_FrameCounter[0])LiftL_G = GG;
//	else LiftL_G = OK;
//	if(LiftR_Motor.FeedbackData.FrameCounter == Last_FrameCounter[1])LiftR_G = GG;
//	else LiftR_G = OK;
//	Last_FrameCounter[0] = LiftL_Motor.FeedbackData.FrameCounter;
//	Last_FrameCounter[1] = LiftR_Motor.FeedbackData.FrameCounter;
//	
//	if(ForwardL_Motor.FeedbackData.FrameCounter == Last_FrameCounter[2])ForwardL_G = GG;
//	else ForwardL_G = OK;
//	if(ForwardR_Motor.FeedbackData.FrameCounter == Last_FrameCounter[3])ForwardR_G = GG;
//	else ForwardR_G = OK;
//	Last_FrameCounter[2] = ForwardL_Motor.FeedbackData.FrameCounter;
//	Last_FrameCounter[3] = ForwardR_Motor.FeedbackData.FrameCounter;
//	
//	if(Roll_Motor.FeedbackData.FrameCounter == Last_FrameCounter[4])Roll_G = GG;
//	else Roll_G = OK;
//	Last_FrameCounter[4] = Roll_Motor.FeedbackData.FrameCounter;
//	
//	if(PitchR_Motor.FeedbackData.FrameCounter == Last_FrameCounter[5])Pitch_G = GG;
//	else if(PitchL_Motor.FeedbackData.FrameCounter == Last_FrameCounter[6])Pitch_G = GG;
//	else Pitch_G = OK;
//	Last_FrameCounter[5] = PitchR_Motor.FeedbackData.FrameCounter;
//	Last_FrameCounter[6] = PitchL_Motor.FeedbackData.FrameCounter;
//	
//	if(RotateB_Motor.FeedbackData.FrameCounter == Last_FrameCounter[7])Rotate_G = GG;
//	else if(RotateF_Motor.FeedbackData.FrameCounter == Last_FrameCounter[8])Rotate_G = GG;
//	else Rotate_G = OK;
//	Last_FrameCounter[7] = RotateB_Motor.FeedbackData.FrameCounter;
//	Last_FrameCounter[8] = RotateF_Motor.FeedbackData.FrameCounter;
//	
//	if(Gimbal_Motor.FeedbackData.FrameCounter == Last_FrameCounter[9])Gimbal_G = GG;
//	else Gimbal_G = OK;
//	Last_FrameCounter[9] = Gimbal_Motor.FeedbackData.FrameCounter;
//	
//	if(ChassisMotor[0].FeedbackData.FrameCounter == Last_FrameCounter[10])Chassis1_G = GG;
//	else Chassis1_G = OK;
//	if(ChassisMotor[1].FeedbackData.FrameCounter == Last_FrameCounter[11])Chassis2_G = GG;
//	else Chassis2_G = OK;
//	if(ChassisMotor[2].FeedbackData.FrameCounter == Last_FrameCounter[12])Chassis3_G = GG;
//	else Chassis3_G = OK;
//	if(ChassisMotor[3].FeedbackData.FrameCounter == Last_FrameCounter[13])Chassis4_G = GG;
//	else Chassis4_G = OK;
//	
//	
//	Last_FrameCounter[10] = ChassisMotor[0].FeedbackData.FrameCounter;
//	Last_FrameCounter[11] = ChassisMotor[1].FeedbackData.FrameCounter;
//	Last_FrameCounter[12] = ChassisMotor[2].FeedbackData.FrameCounter;
//	Last_FrameCounter[13] = ChassisMotor[3].FeedbackData.FrameCounter;
}




/**
 * @brief  Platform Motor Out Send
 * @note   CAN1
 * @param  None
 * @retval
 */
void PlatformMotor_CMD(int16_t PlatLOut, int16_t PlatROut)
{
    static CanSend_Type CANSend;

    CANSend.CANx = CANSEND_1;

    CANSend.stdid = M3508_M2006_CH1_TX_OFFSETID;

    CANSend.Data[0] = (uint8_t)(0);
    CANSend.Data[1] = (uint8_t)(0);
    CANSend.Data[2] = (uint8_t)(PlatLOut >> 8);
    CANSend.Data[3] = (uint8_t)PlatLOut;
    CANSend.Data[4] = (uint8_t)(0);
    CANSend.Data[5] = (uint8_t)(0);
    CANSend.Data[6] = (uint8_t)(PlatROut >> 8);
    CANSend.Data[7] = (uint8_t)PlatROut;

    xQueueSend(Queue_CANSend, &CANSend, 3 );
}

/**
 * @brief  Sky Motor Out Send
 * @note   CAN1
 * @param  None
 * @retval
 */
void SkyMotor_CMD(int16_t SkyPitLOut, int16_t SkyPitROut)
{
	  static CanSend_Type CANSend;

    CANSend.CANx = CANSEND_1;

		CANSend.stdid = M6020_CH1_TX_OFFSETID;

    CANSend.Data[0] = (uint8_t)(SkyPitLOut >> 8);
    CANSend.Data[1] = (uint8_t)SkyPitLOut;
    CANSend.Data[2] = (uint8_t)(0);
    CANSend.Data[3] = (uint8_t)(0);
    CANSend.Data[4] = (uint8_t)(SkyPitROut >> 8);;
    CANSend.Data[5] = (uint8_t)SkyPitROut;
    CANSend.Data[6] = (uint8_t)(0);
    CANSend.Data[7] = (uint8_t)(0);

    xQueueSend(Queue_CANSend, &CANSend, 3 );
}

/**
 * @brief  Sky and Arm Motor Out Send
 * @note   CAN2
 * @param  None
 * @retval
 */
void ArmMotor_CMD(int16_t SkyForLOut, int16_t SkyForROut, int16_t RollOut)
{
	  static CanSend_Type CANSend;

    CANSend.CANx = CANSEND_2;

		CANSend.stdid = M3508_M2006_CH1_TX_OFFSETID;

    CANSend.Data[0] = (uint8_t)(RollOut >> 8);
    CANSend.Data[1] = (uint8_t)RollOut;
    CANSend.Data[2] = (uint8_t)(0);
    CANSend.Data[3] = (uint8_t)(0);
    CANSend.Data[4] = (uint8_t)(SkyForLOut >> 8);
    CANSend.Data[5] = (uint8_t)SkyForLOut;
    CANSend.Data[6] = (uint8_t)(SkyForROut >> 8);
    CANSend.Data[7] = (uint8_t)SkyForROut;

    xQueueSend(Queue_CANSend, &CANSend, 3 );

}

/**
 * @brief  Clear the result of Platform PID calculation
 * @note	 
 * @param
 * @retval
 */
void Platform_Pid_Clear(void)
{
		PID_clear(PlatformL_Motor.SpeedPID);
    PID_clear(PlatformL_Motor.PositionPID);
	
		PID_clear(PlatformR_Motor.SpeedPID);
    PID_clear(PlatformR_Motor.PositionPID);
	
		PID_clear(&PlatformL_Twist_Pid);
    PID_clear(&PlatformR_Twist_Pid);

		PID_clear(Gimbal_Motor.SpeedPID);
    PID_clear(Gimbal_Motor.PositionPID);  
}

/**
 * @brief  Clear the result of Sky PID calculation
 * @note	 
 * @param
 * @retval
 */
void Sky_Pid_Clear(void)
{
		PID_clear(SkyPitchL_Motor.SpeedPID);
    PID_clear(SkyPitchL_Motor.PositionPID);
	
		PID_clear(SkyPitchR_Motor.SpeedPID);
    PID_clear(SkyPitchR_Motor.PositionPID);
	
		PID_clear(SkyForwardL_Motor.SpeedPID);
    PID_clear(SkyForwardL_Motor.PositionPID);
	
		PID_clear(SkyForwardR_Motor.SpeedPID);
    PID_clear(SkyForwardR_Motor.PositionPID);
	
		PID_clear(&SkyForwardL_Twist_Pid);
    PID_clear(&SkyForwardR_Twist_Pid);
}

/**
 * @brief  Clear the result of Arm PID calculation
 * @note	 
 * @param
 * @retval
 */
void Arm_Pid_Clear(void)
{
		PID_clear(ArmYaw_Motor.SpeedPID);
    PID_clear(ArmYaw_Motor.PositionPID);
	
		PID_clear(ArmPitchL_Motor.SpeedPID);
    PID_clear(ArmPitchL_Motor.PositionPID);
	
		PID_clear(ArmPitchR_Motor.SpeedPID);
    PID_clear(ArmPitchR_Motor.PositionPID);
	
		PID_clear(ArmRoll_Motor.SpeedPID);
    PID_clear(ArmRoll_Motor.PositionPID);
}

/**
 * @brief  手动测试代码
 * @note	 
 * @param
 * @retval
 */
void Manual_Test(void)
{
		if (GetSubState() != SubState_Protect)
		{
//				//测试用		
//				if(StickR_Vert[0] == Stick_Big_Front && StickR_Vert[1] != Stick_Big_Front)
//				{
//						Arm_Sucker_State = Sucker_On;
//				}
//				if(StickR_Vert[0] == Stick_Big_Back && StickR_Vert[1] != Stick_Big_Back)
//				{
//						Arm_Sucker_State = Sucker_Off;
//				}
			
//				//测试用		
//				if(StickL_Vert[0] == Stick_Big_Front && StickL_Vert[1] != Stick_Big_Front)
//				{
//						Spin_State = Spin_State_ClockWise;
//						Set_Gimbal_Target(Gimbal_Gyro.AllAngle);
//				}
//				if(StickL_Vert[0] == Stick_Big_Back && StickL_Vert[1] != Stick_Big_Back)
//				{
//						Spin_State = Spin_State_Follow;
//					
//				}
//				if(StickR_Hori[0] == Stick_Big_Right && StickR_Hori[1] != Stick_Big_Right)
//				{
//						Spin_State = Spin_State_Static;
//					
//						Gimbal_State = Gimbal_State_Front;			
//						Set_Gimbal_Target(Gimbal_Angle[Gimbal_State]);
//				}

				
//				//测试用		
//				if(StickR_Vert[0] == Stick_Big_Front)
//				{
//						Spin_State = Spin_State_ClockWise;
//					Set_Gimbal_Target(Gimbal_Gyro.AllAngle);
////						Gimbal_Motor.TargetAngle = Gimbal_Motor.Targetcirnum * 360.0f + Gimbal_Motor.TargetAngle + 3.0f;
////						Angle_Limit(&Gimbal_Motor);
//				}
//				if(StickR_Vert[0] == Stick_Big_Back)
//				{
//						Spin_State = Spin_State_Static;
//						
//					
////						Gimbal_Motor.TargetAngle = Gimbal_Motor.Targetcirnum * 360.0f + Gimbal_Motor.TargetAngle - 3.0f;
////						Angle_Limit(&Gimbal_Motor);
//				}
				
				
				
//				//测试用		
//				if(StickR_Vert[0] == Stick_Big_Front)
//				{
//						PlatformL_Motor.Targetrotationrate = 300;
//						PlatformR_Motor.Targetrotationrate = -300;
//				}
//				else if(StickR_Vert[0] == Stick_Big_Back)
//				{
//						PlatformL_Motor.Targetrotationrate = -300;
//						PlatformR_Motor.Targetrotationrate = 300;
//				}
//				else
//				{
//						PlatformL_Motor.Targetrotationrate = 0;
//						PlatformR_Motor.Targetrotationrate = 0;
//				}

//				//测试用		
//				if(StickR_Vert[0] == Stick_Big_Front)
//				{
//						SkyPitchL_Motor.Targetrotationrate = 100;
//						SkyPitchR_Motor.Targetrotationrate = -100;
//				}
//				else if(StickR_Vert[0] == Stick_Big_Back)
//				{
//						SkyPitchL_Motor.Targetrotationrate = -100;
//						SkyPitchR_Motor.Targetrotationrate = 100;
//				}
//				else
//				{
//						SkyPitchL_Motor.Targetrotationrate = 0;
//						SkyPitchR_Motor.Targetrotationrate = 0;
//				}


			
//				//测试用		
//				if(StickR_Vert[0] == Stick_Big_Front && StickR_Vert[1] != Stick_Big_Front)
//				{
//						Set_Gimbal_Target(34);
//				}
//				if(StickR_Vert[0] == Stick_Big_Back && StickR_Vert[1] != Stick_Big_Back)
//				{
//						Set_Gimbal_Target(213);
//				}
				
//				if(StickL_Hori[0] == Stick_Big_Right && StickL_Hori[1] != Stick_Big_Right)
//				{
//						Set_ArmPitch_Target(0);
//				}
//				if(StickL_Hori[0] == Stick_Big_Left && StickL_Hori[1] != Stick_Big_Left)
//				{
//						Set_ArmPitch_Target(-100);
//				}
				
//				if(StickL_Vert[0] == Stick_Big_Front && StickL_Vert[1] != Stick_Big_Front)
//				{
//						Set_ArmYaw_Target(0);
//				}
//				if(StickL_Vert[0] == Stick_Big_Back && StickL_Vert[1] != Stick_Big_Back)
//				{
//						Set_ArmYaw_Target(50);
//				}
//				
//				
//				if(StickR_Hori[0] == Stick_Big_Right && StickR_Hori[1] != Stick_Big_Right)
//				{
//						Set_ArmRoll_Target(0);
//				}
//				if(StickR_Hori[0] == Stick_Big_Left && StickR_Hori[1] != Stick_Big_Left)
//				{
//						Set_ArmRoll_Target(110);
//				}
				
			
//				if(StickR_Hori[0] == Stick_Big_Right && StickR_Hori[1] != Stick_Big_Right)
//				{
//						Set_SkyForward_Target(-3);
//				}
//				if(StickR_Hori[0] == Stick_Big_Left && StickR_Hori[1] != Stick_Big_Left)
//				{
//						Set_SkyForward_Target(913);
//				}		
				
				if(StickL_Hori[0] == Stick_Big_Right && StickL_Hori[1] != Stick_Big_Right)
				{
						Set_SkyPitch_Target(45);
				}
				if(StickL_Hori[0] == Stick_Big_Left && StickL_Hori[1] != Stick_Big_Left)
				{
						Set_SkyPitch_Target(90);
				}	
				if(StickL_Vert[0] == Stick_Big_Front && StickL_Vert[1] != Stick_Big_Front)
				{
						Set_SkyPitch_Target(0);
				}
				if(StickL_Vert[0] == Stick_Big_Back && StickL_Vert[1] != Stick_Big_Back)
				{
						Set_SkyPitch_Target(130);
				}				
				
//				if(StickL_Vert[0] == Stick_Big_Front && StickL_Vert[1] != Stick_Big_Front)
//				{
//						Set_Platform_Target(0);
//				}
//				if(StickL_Vert[0] == Stick_Big_Back && StickL_Vert[1] != Stick_Big_Back)
//				{
//						Set_Platform_Target(-300);
//				}				
//				if(StickR_Hori[0] == Stick_Big_Right && StickR_Hori[1] != Stick_Big_Right)
//				{
//						Set_Platform_Target(350);
//				}
//				if(StickR_Hori[0] == Stick_Big_Left && StickR_Hori[1] != Stick_Big_Left)
//				{
//						Set_Platform_Target(100);
//				}		
				
//				if(StickL_Vert[0] == Stick_Big_Front && StickL_Vert[1] != Stick_Big_Front)
//				{
//						Lift_State = Lift_State_Init;	
//				}
//				if(StickL_Vert[0] == Stick_Big_Back && StickL_Vert[1] != Stick_Big_Back)
//				{
//						Lift_State = Lift_State_Sky;	
//				}
				
//				if(StickL_Vert[0] == Stick_Big_Front)
//				{
//            if(PlatformL_Motor.Targetcirnum * 360.0f + PlatformL_Motor.TargetAngle > 517)
//            {
//								PlatformL_Motor.TargetAngle = PlatformL_Motor.Targetcirnum * 360.0f + PlatformL_Motor.TargetAngle - 3.0f;
//								PlatformR_Motor.TargetAngle = PlatformR_Motor.Targetcirnum * 360.0f + PlatformR_Motor.TargetAngle + 3.0f;
//								Angle_Limit(&PlatformL_Motor);
//								Angle_Limit(&PlatformR_Motor);
//						}
//				}
//				if(StickL_Vert[0] == Stick_Big_Back)
//				{
//						if(PlatformL_Motor.Targetcirnum * 360.0f + PlatformL_Motor.TargetAngle < 1276)
//            {
//								PlatformL_Motor.TargetAngle = PlatformL_Motor.Targetcirnum * 360.0f + PlatformL_Motor.TargetAngle + 3.0f;
//								PlatformR_Motor.TargetAngle = PlatformR_Motor.Targetcirnum * 360.0f + PlatformR_Motor.TargetAngle - 3.0f;
//								Angle_Limit(&PlatformL_Motor);
//								Angle_Limit(&PlatformR_Motor);
//						}
//				}
				
				
		}
}

/**
 * @brief  遥控器控制
 * @note	 
 * @param
 * @retval
 */
uint8_t SkyPitch_Flag = 0;
uint8_t Sky_Flag = 0;
uint8_t Spin_Flag = 0;
uint8_t Sky_Protect_Count1 = 0;
uint8_t Sky_Protect_Count2 = 0;
void Manual_Control()
{
		if(GetSubState() != SubState_Protect)
		{
				if(GetSubState() == SubState_Test)
				{
						Gimbal_State = Gimbal_State_Front;			
						Set_Gimbal_Target(Gimbal_Angle[Gimbal_State]);
					
						//吸盘开合
						if(Roller_State[1] == Roller_Big_Front && Roller_State[0] != Roller_Big_Front)
						{
								if(Sky_Sucker_State == Sucker_On)
								{
										Sky_Sucker_State = Sucker_Off;
										Arm_Sucker_State = Sucker_Off;
								}
								else if(Sky_Sucker_State == Sucker_Off)
								{
										Sky_Sucker_State = Sucker_On;
										Arm_Sucker_State = Sucker_On;
								}		
						}
						
						//空接动作
						if(Roller_State[1] == Roller_Big_Back && Roller_State[0] != Roller_Big_Back)
						{
								if(Sky_Flag == 0)
								{
										Sky_Flag = 1;
								}
								else if(Sky_Flag == 1)
								{
										Sky_Flag = 2;
								}
						}	
						
						if(Sky_Flag == 2)
						{		
								Lift_State = Lift_State_Init;	
							
								Platform_State = Platform_State_Init;
								SkyForward_State = SkyForward_State_Init;
								SkyPitch_State = SkyPitch_State_Init;
								ArmPitch_State = ArmPitch_State_Protect;
								
								Set_Platform_Target(Platform_Angle[Platform_State]);
								Set_SkyForward_Target(SkyForward_Angle[SkyForward_State]);
								Set_SkyPitch_Target(SkyPitch_Angle[SkyPitch_State]);
								Set_ArmPitch_Target(ArmPitch_Angle[ArmPitch_State]);
							
								Sky_Flag = 0;
						}
						else if(Sky_Flag == 1)
						{
								Lift_State = Lift_State_Sky;	
							
								Platform_State = Platform_State_Sky;
								SkyForward_State = SkyForward_State_Sky;
								SkyPitch_State = SkyPitch_State_Sky;
								ArmPitch_State = ArmPitch_State_Sky;
								
								Set_Platform_Target(Platform_Angle[Platform_State]);
								Set_SkyForward_Target(SkyForward_Angle[SkyForward_State]);
								Set_SkyPitch_Target(SkyPitch_Angle[SkyPitch_State]);
								Set_ArmPitch_Target(ArmPitch_Angle[ArmPitch_State]);

								Sky_Sucker_State = Sucker_On;	
						}
				}
				
				if(GetSubState() == SubState_Hand)
				{

						if(Roller_State[0] == Roller_Big_Front && Roller_State[1] != Roller_Big_Front)
						{
								Spin_State = Spin_State_ClockWise;
								Set_Gimbal_Target(Gimbal_Gyro.AllAngle);
							
								Platform_State = Platform_State_Spin;
								Set_Platform_Target(Platform_Angle[Platform_State]);
						}
						else if(Roller_State[0] == Roller_Big_Front && Roller_State[1] == Roller_Big_Front)
						{
								Spin_State = Spin_State_ClockWise;
						}
						else if(Roller_State[0] != Roller_Big_Front && Roller_State[1] == Roller_Big_Front)
						{
								Spin_State = Spin_State_Follow;
							
								Auto_Follow_Flag = 1;
								Auto_Protect = 1;
						}
						
						
						if(Auto_Follow_Flag == 1)
						{
								SpinCounter++;
								if(fabs(Chassis_Offset) < 5 ||SpinCounter > 200)
								{
										Spin_State = Spin_State_Static;
									
										Platform_State = Platform_State_Arm_Init;
										Set_Platform_Target(Platform_Angle[Platform_State]);
									
										SpinCounter = 0;
									
										Auto_Follow_Flag = 0;
										Auto_Protect = 0;
								}	
						}
						
						if(Spin_State == Spin_State_Static)
						{
								Gimbal_State = Gimbal_State_Front;			
								Set_Gimbal_Target(Gimbal_Angle[Gimbal_State]);
						}
				}
				
				if(GetSubState() == SubState_Exchange)
				{	
					
						if(Roller_State[0] == Roller_Big_Front && Roller_State[1] != Roller_Big_Front)
						{
								Spin_State = Spin_State_ClockWise;
								Set_Gimbal_Target(Gimbal_Gyro.AllAngle);
							
								Platform_State = Platform_State_Spin;
								Set_Platform_Target(Platform_Angle[Platform_State]);
						}
						if(Roller_State[0] == Roller_Big_Front && Roller_State[1] == Roller_Big_Front)
						{
								Spin_State = Spin_State_ClockWise;
						}
						else if(Roller_State[0] != Roller_Big_Front && Roller_State[1] == Roller_Big_Front)
						{
								Spin_State = Spin_State_Follow;
							
								Auto_Follow_Flag = 1;
								Auto_Protect = 1;
						}
						
						
						if(Auto_Follow_Flag == 1)
						{
								SpinCounter++;
								if(fabs(Chassis_Offset) < 5 || SpinCounter > 200)
								{
										Spin_State = Spin_State_Static;
									
										Platform_State = Platform_State_Arm_Init;
										Set_Platform_Target(Platform_Angle[Platform_State]);
									
										SpinCounter = 0;
									
										Auto_Follow_Flag = 0;
										Auto_Protect = 0;
								}	
						}
						
						if(Spin_State == Spin_State_Static)
						{
								Gimbal_State = Gimbal_State_Front;			
								Set_Gimbal_Target(Gimbal_Angle[Gimbal_State]);
						}
				}
				
				if(GetSubState() == SubState_Rotate)
				{
						if(Roller_State[0] == Roller_Big_Front && Roller_State[1] != Roller_Big_Front)
						{
								Spin_State = Spin_State_ClockWise;
								Set_Gimbal_Target(Gimbal_Gyro.AllAngle);
							
								Platform_State = Platform_State_Spin;
								Set_Platform_Target(Platform_Angle[Platform_State]);
						}
						if(Roller_State[0] == Roller_Big_Front && Roller_State[1] == Roller_Big_Front)
						{
								Spin_State = Spin_State_ClockWise;
						}
						else if(Roller_State[0] != Roller_Big_Front && Roller_State[1] == Roller_Big_Front)
						{
								Spin_State = Spin_State_Follow;
							
								Auto_Follow_Flag = 1;
								Auto_Protect = 1;
						}
						
						
						if(Auto_Follow_Flag == 1)
						{
								SpinCounter++;
								if(fabs(Chassis_Offset) < 5 || SpinCounter > 200)
								{
										Spin_State = Spin_State_Static;
									
										Platform_State = Platform_State_Arm_Init;
										Set_Platform_Target(Platform_Angle[Platform_State]);
									
										SpinCounter = 0;
									
										Auto_Follow_Flag = 0;
										Auto_Protect = 0;
								}	
						}	
						
						if(Spin_State == Spin_State_Static)
						{
								Gimbal_State = Gimbal_State_Front;			
								Set_Gimbal_Target(Gimbal_Angle[Gimbal_State]);
						}
				}
				
				if(GetSubState() == SubState_Move)
				{
						Gimbal_State = 	Gimbal_State_Back;	
						Set_Gimbal_Target(Gimbal_Angle[Gimbal_State]);
				}
				
				if(GetSubState() == SubState_Gimbal)
				{
						Gimbal_State = Gimbal_State_Back;			
						Set_Gimbal_Target(Gimbal_Angle[Gimbal_State]);
				}

				if(GetSubState() == SubState_Mineral)
				{
						Gimbal_State = Gimbal_State_Back;			
						Set_Gimbal_Target(Gimbal_Angle[Gimbal_State]);
				}
				
				
				
				if(StickL_Vert[0] == Stick_Big_Front)
				{
						Lift_State = Lift_State_Down;	
				}
				else if(StickL_Vert[0] == Stick_Big_Back)
				{
						Lift_State = Lift_State_Up;
				}
		}		
}






void ArmYaw_Motor_AngleInit(void)
{
		if(StickR_Hori[0] == Stick_Big_Right && StickR_Hori[1] != Stick_Big_Right && ArmYawInit < 4)
		{
				ArmYawInit++;
				if(ArmYawInit == 0)
				{
						ArmYaw_Motor.InitAngle = -67;
						ArmYaw_Motor.TargetAngle = 293;
						ArmYaw_Motor.Targetcirnum = -1;
				}
				if(ArmYawInit == 1)
				{
						ArmYaw_Motor.InitAngle = -31;
						ArmYaw_Motor.TargetAngle = 329;
						ArmYaw_Motor.Targetcirnum = -1;
				}
				if(ArmYawInit == 2)
				{
						ArmYaw_Motor.InitAngle = 5;
						ArmYaw_Motor.TargetAngle = ArmYaw_Motor.InitAngle;
						ArmYaw_Motor.Targetcirnum = 0;
				}
				if(ArmYawInit == 3)
				{
						ArmYaw_Motor.InitAngle = 41;
						ArmYaw_Motor.TargetAngle = ArmYaw_Motor.InitAngle;
						ArmYaw_Motor.Targetcirnum = 0;
				}
				if(ArmYawInit == 4)
				{
						ArmYaw_Motor.InitAngle = 77;
						ArmYaw_Motor.TargetAngle = ArmYaw_Motor.InitAngle;
						ArmYaw_Motor.Targetcirnum = 0;
				}
		}
		if(StickR_Hori[0] == Stick_Big_Left && StickR_Hori[1] != Stick_Big_Left && ArmYawInit > 0)
		{
				ArmYawInit--;
				if(ArmYawInit == 0)
				{
						ArmYaw_Motor.InitAngle = -67;
						ArmYaw_Motor.TargetAngle = 293;
						ArmYaw_Motor.Targetcirnum = -1;
				}
				if(ArmYawInit == 1)
				{
						ArmYaw_Motor.InitAngle = -31;
						ArmYaw_Motor.TargetAngle = 329;
						ArmYaw_Motor.Targetcirnum = -1;
				}
				if(ArmYawInit == 2)
				{
						ArmYaw_Motor.InitAngle = 5;
						ArmYaw_Motor.TargetAngle = ArmYaw_Motor.InitAngle;
						ArmYaw_Motor.Targetcirnum = 0;
				}
				if(ArmYawInit == 3)
				{
						ArmYaw_Motor.InitAngle = 41;
						ArmYaw_Motor.TargetAngle = ArmYaw_Motor.InitAngle;
						ArmYaw_Motor.Targetcirnum = 0;
				}
				if(ArmYawInit == 4)
				{
						ArmYaw_Motor.InitAngle = 77;
						ArmYaw_Motor.TargetAngle = ArmYaw_Motor.InitAngle;
						ArmYaw_Motor.Targetcirnum = 0;
				}
		}	
}


void Platform_Motor_AngleInit(void)
{
		if(StickL_Vert[0] == Stick_Big_Front && StickL_Vert[1] != Stick_Big_Front)
		{
				PlatformL_Motor.Targetcirnum = PlatformL_Motor.Targetcirnum++;
				PlatformL_Motor.InitAngle = 360 + PlatformL_Motor.InitAngle;
				
				PlatformR_Motor.Targetcirnum = PlatformR_Motor.Targetcirnum--;
				PlatformR_Motor.InitAngle = -360 + PlatformR_Motor.InitAngle;
		}
		else if(StickL_Vert[0] == Stick_Big_Back && StickL_Vert[1] != Stick_Big_Back)
		{
				PlatformL_Motor.Targetcirnum = PlatformL_Motor.Targetcirnum--;
				PlatformL_Motor.InitAngle = -360 + PlatformL_Motor.InitAngle;
				
				PlatformR_Motor.Targetcirnum = PlatformR_Motor.Targetcirnum++;
				PlatformR_Motor.InitAngle = 360 + PlatformR_Motor.InitAngle;
		}
}

void FreeWheelBlock(void)
{
		if(Roller_State[1] == Roller_Big_Front && Roller_State[0] != Roller_Big_Front)
		{  
			Lift_State = Lift_State_Init;
			SkyForwardL_Motor.Blockstate.BlockState = noblock;
			SkyForwardR_Motor.Blockstate.BlockState = noblock;
			Auto_Block_Flag = 1;
		}
}


