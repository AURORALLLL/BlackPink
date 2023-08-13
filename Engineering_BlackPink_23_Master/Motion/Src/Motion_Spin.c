/**Include Header Files**/
#include "Motion_Spin.h"

/**Tips**/


/**Variable Definition**/
int16_t Chassis_Follow_W = 0;
int16_t Chassis_Offset = 0;

Spin_State_t Spin_State;

pid_type Chassis_Follow_Pid;

uint8_t Auto_Follow_Flag = 0;
uint8_t SpinCounter = 0;

/**Macro Definition**/
//#define CHASSIS_FOLLOW_NO_PID


/**Const Parameter Definition**/
#ifndef CHASSIS_FOLLOW_NO_PID
	const float ChassisFollowPosPIDparameter[3] = {40, 0, 0};
#endif
		
#ifdef CHASSIS_FOLLOW_NO_PID
	const float ChassisFollowPosPIDparameter[3] = {0, 0, 0};
#endif
	
/**Function Declaration**/
/**
 * @brief  陀螺相关参数初始化
 * @note
 * @retval 
 */
void Spin_Init(void)
{
		PID_Parameter_Init(&Chassis_Follow_Pid, PID_POSITION, ChassisFollowPosPIDparameter, CHASSISFOLLOW_MAXOUT, CHASSISFOLLOW_MAXIOUT, CHASSISFOLLOW_DEADZONE);		
}

/**
 * @brief  陀螺相关参数初始化
 * @note
 * @retval 
 */
void Manual_Spin(void)
{
		if(X[1] == UnPressed && X[0] == Pressed)
		{
				Spin_State = Spin_State_ClockWise;
				Set_Gimbal_Target(Gimbal_Gyro.AllAngle);
			
				Platform_State = Platform_State_Spin;
				Set_Platform_Target(Platform_Angle[Platform_State]);
		}
		else if(X[1] == Pressed && X[0] == Pressed)
		{
				Spin_State = Spin_State_ClockWise;
		}
		else if(X[1] == Pressed && X[0] == UnPressed)
		{
				Spin_State = Spin_State_Follow;
			
				Auto_Follow_Flag = 1;
				Auto_Protect = 1;
		}
		
		
		if(Auto_Follow_Flag == 1)
		{
				SpinCounter++;
				if(fabs(Chassis_Offset) < 5 || SpinCounter > 150)
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
				if(GetPCMode() == PCMode_Pick_Sky)
				{
						Gimbal_State = 	Gimbal_State_Front;
						Set_Gimbal_Target(Gimbal_Angle[Gimbal_State]);
				}
				if(GetPCMode() == PCMode_Pick_Arm)
				{
						Gimbal_State = 	Gimbal_State_Back;
						Set_Gimbal_Target(Gimbal_Angle[Gimbal_State]);									
				}		
		}
}
	
/**
 * @brief  在陀螺情况下控制底盘
 * @note
 * @retval 底盘跟随云台进行运动
 */
void Spin_Chassis(void)
{
		if (GetSubState() != SubState_Protect)
    {	
				if (GetControlMode() == ControlMode_RC)
				{		
						if(Gimbal_State == Gimbal_State_Front)
						{
								Chassis_Offset = Gimbal_Motor.RealAngle - Gimbal_Angle_Front;
						}
						else if(Gimbal_State == Gimbal_State_Back)
						{
								Chassis_Offset = Gimbal_Motor.RealAngle - Gimbal_Angle_Back;
						}
        }
				if (GetControlMode() == ControlMode_PC)
				{
						if(GetPCMode() == PCMode_Pick_Sky)
						{
								Chassis_Offset = Gimbal_Motor.RealAngle - Gimbal_Angle_Front;
						}
						else if(GetPCMode() == PCMode_Pick_Arm)
						{
								Chassis_Offset = Gimbal_Motor.RealAngle - Gimbal_Angle_Back;
						}
        }				
		}
		
		if(Spin_State == Spin_State_Follow)
		{
			Chassis_Follow_W = (int16_t)(-Pid_cal(&Chassis_Follow_Pid, 0, Chassis_Offset));		
		}	
		
		
}

/**
 * @brief  在陀螺情况下控制云台
 * @note
 * @retval 操作手控制云台朝向
 */
void Spin_Gimbal(void)
{
		if (GetSubState() != SubState_Protect)
    {	
				if (GetControlMode() == ControlMode_RC)
				{
						if(Spin_State == Spin_State_ClockWise)
						{
								if(Get_Channel_Value(CH_RightHori) > 0.4f)
								{
										Gimbal_Motor.TargetAngle = Gimbal_Motor.Targetcirnum * 360.0f + Gimbal_Motor.TargetAngle - 1.5f;
										Angle_Limit(&Gimbal_Motor);
								}
								else if(Get_Channel_Value(CH_RightHori) < -0.4f)
								{
										Gimbal_Motor.TargetAngle = Gimbal_Motor.Targetcirnum * 360.0f + Gimbal_Motor.TargetAngle + 1.5f;
										Angle_Limit(&Gimbal_Motor);
								}
						}
        }
				if (GetControlMode() == ControlMode_PC)
				{

						if(Spin_State == Spin_State_ClockWise)
						{
								if(Out_X > 10)
								{
										Gimbal_Motor.TargetAngle = Gimbal_Motor.Targetcirnum * 360.0f + Gimbal_Motor.TargetAngle - 1.5f;
										Angle_Limit(&Gimbal_Motor);
								}
								else if(Out_X < -10)
								{
										Gimbal_Motor.TargetAngle = Gimbal_Motor.Targetcirnum * 360.0f + Gimbal_Motor.TargetAngle + 1.5f;
										Angle_Limit(&Gimbal_Motor);
								}
						}
        }				
		}
}
