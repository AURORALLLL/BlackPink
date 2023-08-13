/**Include Header Files**/
//抓取控制系列函数
#include "Motion_Pick.h"

/**Variable Definition**/
Lift_State_t Lift_State;
Gimbal_State_t Gimbal_State;


float Lift_Angle[Lift_State_Counter] = {Lift_Angle_Init, Lift_Angle_Silver, Lift_Angle_Gold, Lift_Angle_Safe, Lift_Angle_Hero, Lift_Angle_Sky, Lift_Angle_Exchange, Lift_Angle_Arm_Init, Lift_Angle_Arm_Ground, Lift_Angle_Arm_Silver, Lift_Angle_Arm_Exchange, Lift_Angle_Store, Lift_Angle_StoreDown, Lift_Angle_StoreUp, Lift_Angle_Take, Lift_Angle_Block};


float Pitch_Incline;
float Yaw_Incline;
uint8_t Lift_Motor_InPosition;
//float Gimbal_Angle[Gimbal_State_Counter] = {Gimbal_Angle_Front, Gimbal_Angle_Back};


/**Function Delaration**/
/**
 * @brief  根据抬升状态调整抬升电机运动情况
 * @note	 
 * @param
 * @retval
 */
void Manual_Lift_Control(void)
{
		if(Lift_State == Lift_State_Init || Lift_State == Lift_State_Silver || Lift_State == Lift_State_Gold || Lift_State == Lift_State_Safe  || \
			Lift_State == Lift_State_Hero || Lift_State == Lift_State_Sky || Lift_State == Lift_State_Exchange || Lift_State == Lift_State_Arm_Init || \
			Lift_State == Lift_State_Arm_Ground || Lift_State == Lift_State_Arm_Silver || Lift_State == Lift_State_Arm_Exchange || \
			Lift_State == Lift_State_Store || Lift_State == Lift_State_StoreDown || Lift_State == Lift_State_StoreUp || Lift_State == Lift_State_Take || \
		Lift_State == Lift_State_Block)
		{
				Set_Lift_Target(Lift_Angle[Lift_State]);
		}

		if(Lift_State == Lift_State_Down)
		{
				if(PCMode == PCMode_Exchange_Sky || PCMode == PCMode_Exchange_Arm)
				{
						if(W[0] == Pressed)
						{
								if(LiftL_Motor.Targetcirnum * 360.0f + LiftL_Motor.TargetAngle < 0.1f)
								{
										LiftL_Motor.TargetAngle = LiftL_Motor.Targetcirnum * 360.0f + LiftL_Motor.TargetAngle + 1.5f;
										LiftR_Motor.TargetAngle = LiftR_Motor.Targetcirnum * 360.0f + LiftR_Motor.TargetAngle - 1.5f;
										Angle_Limit(&LiftL_Motor);
										Angle_Limit(&LiftR_Motor);
								}										
						}
				}
		}
		if(Lift_State == Lift_State_Up)
		{
				if(PCMode == PCMode_Exchange_Sky || PCMode == PCMode_Exchange_Arm)
				{
						if(S[0] == Pressed)
						{
								if(LiftL_Motor.Targetcirnum * 360.0f + LiftL_Motor.TargetAngle > -1111.0f -LiftL_Motor.InitAngle)
								{
										LiftL_Motor.TargetAngle = LiftL_Motor.Targetcirnum * 360.0f + LiftL_Motor.TargetAngle - 1.5f;
										LiftR_Motor.TargetAngle = LiftR_Motor.Targetcirnum * 360.0f + LiftR_Motor.TargetAngle + 1.5f;
										Angle_Limit(&LiftL_Motor);
										Angle_Limit(&LiftR_Motor);
								}	
						}
				}
		}
		
		if(Lift_State == Lift_State_Press)
		{
				if(LiftL_Motor.Targetcirnum * 360.0f + LiftL_Motor.TargetAngle < 0.1f)
				{
						LiftL_Motor.TargetAngle = LiftL_Motor.Targetcirnum * 360.0f + LiftL_Motor.TargetAngle + 1.0f;
						LiftR_Motor.TargetAngle = LiftR_Motor.Targetcirnum * 360.0f + LiftR_Motor.TargetAngle - 1.0f;
						Angle_Limit(&LiftL_Motor);
						Angle_Limit(&LiftR_Motor);
				}	
		}	
		
		if(Lift_State == Lift_State_Push)
		{
				if(PCMode == PCMode_Exchange_Arm)
				{
						if(R[0] == Pressed)
						{
								if(LiftL_Motor.Targetcirnum * 360.0f + LiftL_Motor.TargetAngle < 0.1f)
								{
										LiftL_Motor.TargetAngle = LiftL_Motor.Targetcirnum * 360.0f + LiftL_Motor.TargetAngle + 0.7 * sin(Pitch_Incline * 3.14f / 180.0f);
										LiftR_Motor.TargetAngle = LiftR_Motor.Targetcirnum * 360.0f + LiftR_Motor.TargetAngle - 0.7 * sin(Pitch_Incline * 3.14f / 180.0f);
										Angle_Limit(&LiftL_Motor);
										Angle_Limit(&LiftR_Motor);
								}
						}
				}	
		}
}

void Manual_InPosition(void)
{
		if(Motor_InPosition(&LiftL_Motor, NULL, 30))
		{
				Lift_Motor_InPosition = 1;
		}
		else
		{		
				if(Motor_InPosition(&LiftL_Motor, NULL, 200))
				{
						Lift_Motor_InPosition = 2;
				}
				else
				{
						Lift_Motor_InPosition = 0;
				}	
		}
}

