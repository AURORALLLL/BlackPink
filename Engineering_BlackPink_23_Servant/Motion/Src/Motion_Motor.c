/**Include Header Files**/
//电机定义系列函数
#include "Motion_Motor.h"

/**Variable Definition**/
/*MOTOR*/
/**-----------CAN1--------------**/
motor_t ChassisMotor[4];
/**-----------------------------**/

/**-----------CAN2--------------**/
motor_t LiftL_Motor, LiftR_Motor;
/**-----------------------------**/

/*PID*/
pid_type Chassis_Pid[4];

pid_type LiftL_Speed_Pid, LiftL_Position_Pid;
pid_type LiftR_Speed_Pid, LiftR_Position_Pid;
pid_type LiftL_Twist_Pid, LiftR_Twist_Pid;

/*上线检测量*/
uint8_t ChassisMotor_Online = 0;
uint8_t LiftMotor_Online = 0;
uint8_t Motor_Define;

float TwistAngle = 0;
float AngleMax = 0;
uint8_t UIdebug = 0;

//堵转检测
uint8_t Lift_Motor_BlockFlag;
BlockState_t Lift_Motor_BlockFinish;

//掉线检测量
uint32_t Last_FrameCounter[14]={0};
uint8_t LiftR_G = 0;
uint8_t LiftL_G = 0;
uint8_t Chassis1_G = 0;
uint8_t Chassis2_G = 0;
uint8_t Chassis3_G = 0;
uint8_t Chassis4_G = 0;

/**Macro Definition**/
#define MOTION_MOTOR_INTERVAL (10)

//#define PLATFORM_NO_PID
#define PLAT_ONLINE_CHECK

//#define BOTTOM_NO_PID
#define BOTTOM_ONLINE_CHECK


/**Const Parameter Definition**/
#ifndef BOTTOM_NO_PID
	const float LiftLSpeedPIDparameter[3] = {15, 0.021, 0};//13, 0.008, 0
	const float LiftLPosPIDparameter[3] = {30, 0, 0.02};//16，0.005
		
	const float LiftRSpeedPIDparameter[3] = {15, 0.021, 0};
	const float LiftRPosPIDparameter[3] = {30, 0, 0.02};
	
	const float LiftLTwistPIDparameter[3] = {0, 0, 0.009};
	const float LiftRTwistPIDparameter[3] = {0, 0, 0.009};
#endif
		
#ifdef BOTTOM_NO_PID
	const float LiftLSpeedPIDparameter[3] = {0, 0, 0};
	const float LiftLPosPIDparameter[3] = {0, 0, 0};
		
	const float LiftRSpeedPIDparameter[3] = {0, 0, 0};
	const float LiftRPosPIDparameter[3] = {0, 0, 0};
	
	const float LiftLTwistPIDparameter[3] = {0, 0, 0};
	const float LiftRTwistPIDparameter[3] = {0, 0, 0};
#endif
	
/**Function Declaration**/
/**
 * @brief  Initialize BottomMotor
 * @note
 * @param
 * @retval
 */
void BottomMotor_Init(void)
{
		MotortypeRegister(&LiftL_Motor, MOTOR_3508, &LiftL_Position_Pid, &LiftL_Speed_Pid, MOTOR3508Lift_Reductionratio, LiftL_Motor_id);
    PID_Parameter_Init(LiftL_Motor.PositionPID, PID_POSITION, LiftLPosPIDparameter, 3500, LIFTL_MAXIOUT, LIFTL_DEADZONE);
    PID_Parameter_Init(LiftL_Motor.SpeedPID, PID_POSITION, LiftLSpeedPIDparameter, LIFTL_MAXOUT, LIFTL_MAXIOUT, LIFTL_DEADZONE);
    PID_Parameter_Init(&LiftL_Twist_Pid, PID_POSITION, LiftLTwistPIDparameter, LIFTTWIST_MAXOUT, LIFTTWIST_MAXIOUT, LIFTTWIST_DEADZONE);
		
		MotortypeRegister(&LiftR_Motor, MOTOR_3508, &LiftR_Position_Pid, &LiftR_Speed_Pid, MOTOR3508Lift_Reductionratio, LiftR_Motor_id);
    PID_Parameter_Init(LiftR_Motor.PositionPID, PID_POSITION, LiftRPosPIDparameter, 3500, LIFTR_MAXIOUT, LIFTR_DEADZONE);
    PID_Parameter_Init(LiftR_Motor.SpeedPID, PID_POSITION, LiftRSpeedPIDparameter, LIFTR_MAXOUT, LIFTR_MAXIOUT, LIFTR_DEADZONE);
    PID_Parameter_Init(&LiftR_Twist_Pid, PID_POSITION, LiftRTwistPIDparameter, LIFTTWIST_MAXOUT, LIFTTWIST_MAXIOUT, LIFTTWIST_DEADZONE);	
		
		
	#ifdef BOTTOM_ONLINE_CHECK
		LiftMotor_Online_Check();
	#endif

		LiftL_Motor.InitAngle = LiftL_Motor.RealAngle;
    LiftL_Motor.TargetAngle = LiftL_Motor.InitAngle;
    LiftL_Motor.Targetcirnum = 0;
    LiftL_Motor.Realcirnum = 0;
    LiftL_Motor.Oricirnum = 0;

    LiftR_Motor.InitAngle = LiftR_Motor.RealAngle;
    LiftR_Motor.TargetAngle = LiftR_Motor.InitAngle;
    LiftR_Motor.Targetcirnum = 0;
    LiftR_Motor.Realcirnum = 0;
    LiftR_Motor.Oricirnum = 0;
		
}

/**
 * @brief  底盘电机初始化，并检测上线与否
 * @note   Check Chassis Motor is online or not
 * @param
 * @retval
 */
void ChassisMotor_Init(void)
{

	#ifndef CHASSIS_NO_PID
	  const float ChassisSpeedPIDparameter[3] = {19, 0, 0};//18, 0.014, 0
	#endif
		
	#ifdef CHASSIS_NO_PID
		const float ChassisSpeedPIDparameter[3] = {0, 0, 0};
	#endif

		for (int i = 0; i < CHASSIS_MOTOR_NUM; i++)
    {
			MotortypeRegister(&(ChassisMotor[i]), MOTOR_3508, NULL, &Chassis_Pid[i], MOTOR3508_Reductionratio, i+1);//
      PID_Parameter_Init((ChassisMotor[i].SpeedPID), PID_POSITION, ChassisSpeedPIDparameter, CHASSIS_MAXOUT, CHASSIS_MAXIOUT, CHASSIS_DEADZONE);
    }
		
	#ifdef GIMBAL_ONLINE_CHECK
    ChassisMotor_Online_Check();
	#endif
		
		return;
		
}

/**
 * @brief  ChassisMotor Online Check
 * @note   Check Chassis Motor is online or not
 * @param
 * @retval
 */
void ChassisMotor_Online_Check(void)
{
		TickType_t xLastWakeUpTime;
		xLastWakeUpTime = xTaskGetTickCount();
		static uint16_t Online_Flag = 0;
		
		while (ChassisMotor[0].FeedbackData.FrameCounter == 0 && Online_Flag <= 700)
		{
			ChassisMotor_Online = 0;
			Online_Flag++;
			vTaskDelayUntil(&xLastWakeUpTime, MOTION_MOTOR_INTERVAL);
		}
		if(Online_Flag > 690)
				Chassis1_G = GG;
		Online_Flag = 0;
		
		while (ChassisMotor[1].FeedbackData.FrameCounter == 0 && Online_Flag <= 700)
		{
			ChassisMotor_Online = 0;
			Online_Flag++;
			vTaskDelayUntil(&xLastWakeUpTime, MOTION_MOTOR_INTERVAL);
		}
		if(Online_Flag > 690)
				Chassis2_G = GG;
		Online_Flag = 0;
		
		while (ChassisMotor[2].FeedbackData.FrameCounter == 0 && Online_Flag <= 700)
		{
			ChassisMotor_Online = 0;
			Online_Flag++;
			vTaskDelayUntil(&xLastWakeUpTime, MOTION_MOTOR_INTERVAL);
		}
		if(Online_Flag > 690)
				Chassis3_G = GG;
		Online_Flag = 0;
		
		while (ChassisMotor[3].FeedbackData.FrameCounter == 0 && Online_Flag <= 700)
		{
			ChassisMotor_Online = 0;
			Online_Flag++;
			vTaskDelayUntil(&xLastWakeUpTime, MOTION_MOTOR_INTERVAL);
		}
		if(Online_Flag > 690)
				Chassis4_G = GG;
		Online_Flag = 0;
		
		ChassisMotor_Online = 1;
}



/**
 * @brief  LiftMotor Online Check
 * @note   Check Lift Motor and Lift Encoder are online or not
 * @param
 * @retval
 */
void LiftMotor_Online_Check(void)
{
		TickType_t xLastWakeUpTime;
		xLastWakeUpTime = xTaskGetTickCount();
		static uint16_t Online_Flag = 0;
		
		while (LiftL_Motor.FeedbackData.FrameCounter == 0 && Online_Flag <= 700)
		{
			LiftMotor_Online = 0;
			Online_Flag++;
			vTaskDelayUntil(&xLastWakeUpTime, MOTION_MOTOR_INTERVAL);
		}
		if(Online_Flag > 695)
			LiftL_G = GG;
		Online_Flag = 0;
		
		while (LiftR_Motor.FeedbackData.FrameCounter == 0 && Online_Flag <= 700)
		{
			LiftMotor_Online = 0;
			Online_Flag++;
			vTaskDelayUntil(&xLastWakeUpTime, MOTION_MOTOR_INTERVAL);
		}
		if(Online_Flag > 695)
			LiftR_G = GG;
		Online_Flag = 0;
		
		LiftMotor_Online = 1;
}

/**
 * @brief  电机堵转校准
 * @note	 只在第一次从保护切出时才会运行
 * @param	 抬升抬起，前伸堵转，抬升堵转
 * @retval 
 */
void Motor_Calibrate(void)
{
		//抬升堵转完成
		if(LiftMotor_BlockDetect() == noblock)
		{
				LiftL_Motor.Targetrotationrate = 500;
				LiftR_Motor.Targetrotationrate = -500;
		}
		if(LiftMotor_BlockDetect() == isblock)
		{
			LiftL_Motor.SpeedPID->sum_error = 0;
			LiftR_Motor.SpeedPID->sum_error = 0;
			Lift_Motor_BlockFinish = isblock;
		}
}

/**
 * @brief  检测抬升电机是否堵转
 * @note   左右两个电机，均要堵转
 * @param  
 * @retval 是否堵转
 */
BlockState_t LiftMotor_BlockDetect(void)
{
		BlockState_t LiftL_Block, LiftR_Block;
		LiftL_Block = MotorBlockDetect(&LiftL_Motor, MOTION_MOTOR_INTERVAL, MOTOR_MAX_BLOCKTIME);
		LiftR_Block = MotorBlockDetect(&LiftR_Motor, MOTION_MOTOR_INTERVAL, MOTOR_MAX_BLOCKTIME);
		if (LiftL_Block  == isblock && LiftR_Block == isblock)
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
		
		if (mot == &LiftL_Motor)//腕部电机堵转校准用
		{
				if(mot->Blockstate.BlockState == noblock)
				{
						//肘部校准到位之后，腕部电机只需转动较小角度即可,跟肘部相比负载较小
						if ((mot->FeedbackData.RealSpeed < 10) && (mot->Out >= LIFT_BLOCK_KI_MAX))
						{

							if(mot->Blockstate.BlockState == noblock)
								mot->Blockstate.Blocktime += detectperiod;
							if(mot->Blockstate.Blocktime >= maxblocktime||(LiftR_Motor.Blockstate.BlockState == isblock))
							{
										mot->Blockstate.BlockState = isblock;
									 	mot->InitAngle = MechanicalAngle2RealAngle(mot->FeedbackData.Mechanical_Angle[0]) / (float)(mot->Reductionratio);
									 	mot->TargetAngle = mot->InitAngle;
									  mot->Targetcirnum = 0;
										mot->Realcirnum = 0;
										mot->Oricirnum = 0;
									
									  LiftR_Motor.InitAngle = MechanicalAngle2RealAngle(LiftR_Motor.FeedbackData.Mechanical_Angle[0]) / (float)(LiftR_Motor.Reductionratio);
									  LiftR_Motor.TargetAngle = LiftR_Motor.InitAngle;
									  LiftR_Motor.Targetcirnum = 0;
										LiftR_Motor.Realcirnum = 0;
										LiftR_Motor.Oricirnum = 0;
							}
								
						}
						else
						{
								mot->Blockstate.Blocktime = 0;
								mot->Blockstate.BlockState = noblock;
						}
				}
		}
		else if (mot == &LiftR_Motor)
		{
				if(mot->Blockstate.BlockState == noblock)
				{
						//右侧电机与左侧电机相反
						if ((mot->FeedbackData.RealSpeed > -10) && (mot->Out <= -LIFT_BLOCK_KI_MAX))
						{
								if(mot->Blockstate.BlockState == noblock)
									mot->Blockstate.Blocktime += detectperiod;
								if(mot->Blockstate.Blocktime >= maxblocktime||(LiftL_Motor.Blockstate.BlockState == isblock))
								{
										mot->Blockstate.BlockState = isblock;
									 	mot->InitAngle = MechanicalAngle2RealAngle(mot->FeedbackData.Mechanical_Angle[0]) / (float)(mot->Reductionratio);
									 	mot->TargetAngle = mot->InitAngle;
									  mot->Targetcirnum = 0;
										mot->Realcirnum = 0;
										mot->Oricirnum = 0;
									
									  LiftL_Motor.InitAngle = MechanicalAngle2RealAngle(LiftL_Motor.FeedbackData.Mechanical_Angle[0]) / (float)(LiftL_Motor.Reductionratio);
									  LiftL_Motor.TargetAngle = LiftL_Motor.InitAngle;
									  LiftL_Motor.Targetcirnum = 0;
										LiftL_Motor.Realcirnum = 0;
										LiftL_Motor.Oricirnum = 0;
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
 * @brief  Set Lift Motor Angle 
 * @note	 For Mechanical Structure contolled by Two Motors
 * @param	
 * @retval 两电机的变化范围不相同，跟机械结构和电机性能有关
 */
void Set_Lift_Target(float TargetAngle)
{
    LiftL_Motor.SpeedPID->sum_error = 0;
	  LiftR_Motor.SpeedPID->sum_error = 0;   
	
	  LiftL_Motor.TargetAngle = LiftL_Motor.InitAngle + TargetAngle;
    LiftR_Motor.TargetAngle = LiftR_Motor.InitAngle - TargetAngle;//目标角度的赋值也是给的相对于初始角度的偏移量
    
		Angle_Limit(&LiftL_Motor);
		Angle_Limit(&LiftR_Motor);
}

/**
 * @brief  Calculate Catch Motor Out
 * @note   
 * @param
 * @retval
 */
float Twist_Angle;
void LiftMotor_Control(void)
{
			//Calculate Lift Angle
			float Dst_LiftLAngle, Rel_LiftLAngle;
			Dst_LiftLAngle = LiftL_Motor.Targetcirnum * 360.0f + LiftL_Motor.TargetAngle;
			Rel_LiftLAngle = LiftL_Motor.Realcirnum * 360.0f + LiftL_Motor.RealAngle;

			float Dst_LiftRAngle, Rel_LiftRAngle;
			Dst_LiftRAngle = LiftR_Motor.Targetcirnum * 360.0f + LiftR_Motor.TargetAngle;
			Rel_LiftRAngle = LiftR_Motor.Realcirnum * 360.0f + LiftR_Motor.RealAngle;

			//Calculate Move Speed PID
			float LiftL_MoveSpeed, LiftR_MoveSpeed;
			LiftL_MoveSpeed = Pid_cal(LiftL_Motor.PositionPID, Dst_LiftLAngle, Rel_LiftLAngle);
			LiftR_MoveSpeed = Pid_cal(LiftR_Motor.PositionPID, Dst_LiftRAngle, Rel_LiftRAngle);

			//Calculate Twist Angle
			float Lift_Twist_Angle;
			Lift_Twist_Angle = fabsf(Rel_LiftLAngle) - fabsf(Rel_LiftRAngle);//大于零则左侧超前，小于零则右侧超前
			Twist_Angle = Lift_Twist_Angle;
	
			//Calculate Twist Speed PID
			float LiftL_TwistSpeed, LiftR_TwistSpeed;
			LiftL_TwistSpeed = -Pid_cal(&LiftL_Twist_Pid, 0, Lift_Twist_Angle);
			LiftR_TwistSpeed = -Pid_cal(&LiftR_Twist_Pid, 0, Lift_Twist_Angle);
	
			if(Lift_Motor_BlockFlag == 0)
			{
					//Calculate Motor Speed
					LiftL_Motor.Targetrotationrate = LiftL_MoveSpeed + LiftL_TwistSpeed;
					LiftR_Motor.Targetrotationrate = LiftR_MoveSpeed + LiftR_TwistSpeed;			
			}

			//Calculate Motor Out	
			LiftL_Motor.Out = -2000 + (int16_t)(Pid_cal(LiftL_Motor.SpeedPID, LiftL_Motor.Targetrotationrate, LiftL_Motor.Realrotationrate));
			LiftR_Motor.Out = 2000 + (int16_t)(Pid_cal(LiftR_Motor.SpeedPID, LiftR_Motor.Targetrotationrate, LiftR_Motor.Realrotationrate));
		
		
		
//		LiftL_Motor.Out = 1000;
//		LiftR_Motor.Out = -1000;
    return;

}




void ChassisControl(void)
{
		for (int i = 0; i < CHASSIS_MOTOR_NUM; i++)
		{
				ChassisMotor[i].Out = (int16_t)(Pid_cal(ChassisMotor[i].SpeedPID, ChassisMotor[i].Targetrotationrate, ChassisMotor[i].Realrotationrate));
		}
    
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
 * @brief  Lift Motor Out Send
 * @note   CAN2
 * @param  None
 * @retval
 */
void BottomMotor_CMD(int16_t LifLOut, int16_t LifROut)
{
    static CanSend_Type CANSend;

    CANSend.CANx = CANSEND_2;

    CANSend.stdid = M3508_M2006_CH1_TX_OFFSETID;

    CANSend.Data[0] = (uint8_t)(LifLOut >> 8);
    CANSend.Data[1] = (uint8_t)LifLOut;
    CANSend.Data[2] = (uint8_t)(LifROut >> 8);
    CANSend.Data[3] = (uint8_t)LifROut;
    CANSend.Data[4] = (uint8_t)(0);
    CANSend.Data[5] = (uint8_t)(0);
    CANSend.Data[6] = (uint8_t)(0);
    CANSend.Data[7] = (uint8_t)(0);
		
    xQueueSend(Queue_CANSend, &CANSend, 3 );/// portTICK_RATE_MS
	
    return;
}


/**
 * @brief  Chassis Motor Out Send
 * @note   CAN2
 * @param  None
 * @retval
 */

/**Tips**/
/*
ChassisFL:ChassisMotor[0]
ChassisFR:ChassisMotor[1]
ChassisBR:ChassisMotor[2]
ChassisBL:ChassisMotor[3]
*/
void ChassisMotor_CMD(int16_t FLOut, int16_t FROut, int16_t BROut, int16_t BLOut)
{
    static CanSend_Type CANSend;

    CANSend.CANx = CANSEND_1;

    CANSend.stdid = M3508_M2006_CH1_TX_OFFSETID;

    CANSend.Data[0] = (uint8_t)(FLOut >> 8);
    CANSend.Data[1] = (uint8_t)FLOut;
    CANSend.Data[2] = (uint8_t)(FROut >> 8);
    CANSend.Data[3] = (uint8_t)FROut;
    CANSend.Data[4] = (uint8_t)(BROut >> 8);
    CANSend.Data[5] = (uint8_t)BROut;
    CANSend.Data[6] = (uint8_t)(BLOut >> 8);
    CANSend.Data[7] = (uint8_t)BLOut;
		
    xQueueSend(Queue_CANSend, &CANSend, 3 );/// portTICK_RATE_MS
	
    return;
}

/**
 * @brief  Clear the result of Arm Catch PID calculation with Mineral
 * @note	 
 * @param
 * @retval
 */
void Bottom_Pid_Clear(void)
{
		PID_clear(LiftL_Motor.SpeedPID);
    PID_clear(LiftL_Motor.PositionPID);
	
		PID_clear(LiftR_Motor.SpeedPID);
    PID_clear(LiftR_Motor.PositionPID);    
}

/**
 * @brief  Clear the result of Chassis PID calculation
 * @note	 底盘PID清零函数
 * @param
 * @retval
 */
void Chassis_Pid_Clear(void)
{
		PID_clear(ChassisMotor[0].SpeedPID);
		PID_clear(ChassisMotor[1].SpeedPID);
		PID_clear(ChassisMotor[2].SpeedPID);
		PID_clear(ChassisMotor[3].SpeedPID);
	
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
//			//测试用		
      if(StickL_Vert[0] == Stick_Big_Front && StickL_Vert[2] != Stick_Big_Front)
      {
					Set_Lift_Target(-1000);
			}
			if(StickL_Vert[0] == Stick_Big_Back && StickL_Vert[2] != Stick_Big_Back)
      {
					Set_Lift_Target(0);
			}
			
			if(StickR_Vert[0] == Stick_Big_Front && StickR_Vert[2] != Stick_Big_Front)
      {
					Set_Lift_Target(-300);
			}
			if(StickR_Vert[0] == Stick_Big_Back && StickR_Vert[2] != Stick_Big_Back)
      {
					Set_Lift_Target(-800);
			}
			
			
//			if(StickL_Vert[2] == Stick_Big_Front)
//			{
//            if(LiftL_Motor.Targetcirnum * 360.0f + LiftL_Motor.TargetAngle > -1130.0f -LiftL_Motor.InitAngle)
//            {
//                LiftL_Motor.TargetAngle = LiftL_Motor.Targetcirnum * 360.0f + LiftL_Motor.TargetAngle - 3.0f;
//                LiftR_Motor.TargetAngle = LiftR_Motor.Targetcirnum * 360.0f + LiftR_Motor.TargetAngle + 3.0f;
//                Angle_Limit(&LiftL_Motor);
//		            Angle_Limit(&LiftR_Motor);
//            }
//      }
//      if(StickL_Vert[2] == Stick_Big_Back)
//      {
//						if(LiftL_Motor.Targetcirnum * 360.0f + LiftL_Motor.TargetAngle < 0.1f)
//            {
//                LiftL_Motor.TargetAngle = LiftL_Motor.Targetcirnum * 360.0f + LiftL_Motor.TargetAngle + 3.0f;
//                LiftR_Motor.TargetAngle = LiftR_Motor.Targetcirnum * 360.0f + LiftR_Motor.TargetAngle - 3.0f;
//                Angle_Limit(&LiftL_Motor);
//		            Angle_Limit(&LiftR_Motor);
//            }
//			}
			
		}
		  
}

/**
 * @brief  遥控器控制
 * @note	 
 * @param
 * @retval
 */
void Manual_Control(void)
{
		if(GetSubState() != SubState_Protect)
		{
				if(Lift_State == Lift_State_Init)
				{
						Set_Lift_Target(Lift_Angle[Lift_State]);
				}
				if(Lift_State == Lift_State_Sky)
				{
						Set_Lift_Target(Lift_Angle[Lift_State]);
				}
				if(Lift_State == Lift_State_Block)
				{
						Set_Lift_Target(Lift_Angle[Lift_State]);
				}
				
				if(Lift_State == Lift_State_Down)
				{
						if(StickL_Vert[0] == Stick_Big_Front)
						{

								if(LiftL_Motor.Targetcirnum * 360.0f + LiftL_Motor.TargetAngle < 0.1f)
								{
										LiftL_Motor.TargetAngle = LiftL_Motor.Targetcirnum * 360.0f + LiftL_Motor.TargetAngle + 2.0f;
										LiftR_Motor.TargetAngle = LiftR_Motor.Targetcirnum * 360.0f + LiftR_Motor.TargetAngle - 2.0f;
										Angle_Limit(&LiftL_Motor);
										Angle_Limit(&LiftR_Motor);
								}										
						}
				}
				if(Lift_State == Lift_State_Up)
				{
						if(StickL_Vert[0] == Stick_Big_Back)
						{
								if(LiftL_Motor.Targetcirnum * 360.0f + LiftL_Motor.TargetAngle > -1111.0f -LiftL_Motor.InitAngle)
								{
										LiftL_Motor.TargetAngle = LiftL_Motor.Targetcirnum * 360.0f + LiftL_Motor.TargetAngle - 2.0f;
										LiftR_Motor.TargetAngle = LiftR_Motor.Targetcirnum * 360.0f + LiftR_Motor.TargetAngle + 2.0f;
										Angle_Limit(&LiftL_Motor);
										Angle_Limit(&LiftR_Motor);
								}	
						}
				}
		}
}

	




