/**Include Header Files**/
#include "Motion_Exchange.h"


/**Variable Definition**/
int16_t Pitch_Incline;
int16_t Yaw_Incline;

/**Function Delaration**/
/**
 * @brief  在兑换模式下实现抬升运动
 * @note	 手动流程
 * @param  空接臂、兑换臂
 * @retval
 */
void Manual_Lift_Exchange(void)
{
		if(S[1] == Pressed && S[0] == Pressed)
		{
				Lift_State = Lift_State_Up;
		}
		
		if(W[1] == Pressed && W[0] == Pressed)
		{
				Lift_State = Lift_State_Down;
		}
}

/**
 * @brief  在兑换模式下控制吸盘
 * @note	 手动流程
 * @param  空接臂
 * @retval
 */
void Manual_Sucker_Exchange_Sky(void)
{
		if(Q[1] == Pressed && Q[0] == UnPressed && Sky_Sucker_State == Sucker_Off)
		{
				Sky_Sucker_State = Sucker_On;
		}
		
		if(E[1] == Pressed && E[0] == UnPressed && Sky_Sucker_State == Sucker_On)
		{
				Sky_Sucker_State = Sucker_Off;
		}
}

/**
 * @brief  在兑换模式下控制平台
 * @note	 手动流程
 * @param  空接臂
 * @retval
 */
void Manual_Forward_Exchange_Sky(void)
{
		if (SHIFT[1] == Pressed && SHIFT[0] == Pressed)
		{
			
				SkyForward_State = SkyForward_State_Forward;
				if(SkyForwardL_Motor.Targetcirnum * 360.0f + SkyForwardL_Motor.TargetAngle < SkyForwardL_Motor.InitAngle + SkyForward_Angle_Sky)
				{
						SkyForwardL_Motor.TargetAngle = SkyForwardL_Motor.Targetcirnum * 360.0f + SkyForwardL_Motor.TargetAngle + 2.0f;
						SkyForwardR_Motor.TargetAngle = SkyForwardR_Motor.Targetcirnum * 360.0f + SkyForwardR_Motor.TargetAngle - 2.0f;
						Angle_Limit(&SkyForwardL_Motor);
						Angle_Limit(&SkyForwardR_Motor);
				}	
		}
			
		if (CTRL[1] == Pressed && CTRL[0] == Pressed)
		{

				SkyForward_State = SkyForward_State_Backward;
				if(SkyForwardL_Motor.Targetcirnum * 360.0f + SkyForwardL_Motor.TargetAngle > SkyForwardL_Motor.InitAngle + SkyForward_Angle_Init)
				{
						SkyForwardL_Motor.TargetAngle = SkyForwardL_Motor.Targetcirnum * 360.0f + SkyForwardL_Motor.TargetAngle - 2.0f;
						SkyForwardR_Motor.TargetAngle = SkyForwardR_Motor.Targetcirnum * 360.0f + SkyForwardR_Motor.TargetAngle + 2.0f;
						Angle_Limit(&SkyForwardL_Motor);
						Angle_Limit(&SkyForwardR_Motor);
				}
		}
}

/**
 * @brief  抓取模式切换到兑换模式
 * @note	 空接臂
 * @param  
 * @retval
 */
void Pick_to_Exchange_Sky(void)
{
//		if(G[1] == Pressed && G[0] == UnPressed && Auto_Position_Flag == 0)
//		{
//				Auto_Position_Flag = 1;	
//				Auto_Protect = 1;
//			
//				Lift_State = Lift_State_Exchange;
//				Set_Lift_Target(Lift_Angle[Lift_State]);
//		}
		
		if(Auto_Position_Flag == 1)
		{
				if(Platform_State == Platform_State_Init)
				{
						if(Motor_InPosition(&PlatformL_Motor, &PlatformL_Encoder, 15) || (V[1] == Pressed && V[0]== UnPressed))
						{
								Lift_State = Lift_State_Arm_Exchange;
							
								SkyForward_State = SkyForward_State_Init;
								SkyPitch_State = SkyPitch_State_Plane;
								ArmPitch_State = ArmPitch_State_Protect;
								ArmYaw_State = ArmYaw_State_Init;
							
								Set_SkyForward_Target(SkyForward_Angle[SkyForward_State]);
								Set_SkyPitch_Target(SkyPitch_Angle[SkyPitch_State]);
								Set_ArmPitch_Target(ArmPitch_Angle[ArmPitch_State]);
								Set_ArmYaw_Target(ArmYaw_Angle[ArmYaw_State]);
							
								Auto_Position_Flag = 0;	
								Auto_Protect = 0;
						}	
				}	
		}	
}

/**
 * @brief  在兑换模式下控制平台
 * @note	 手动流程
 * @param  兑换臂
 * @retval
 */
void Manual_Forward_Exchange_Arm(void)
{
		if (CTRL[1] == Pressed && CTRL[0] == Pressed)
		{
				//平台前移
				Platform_State = Platform_State_Forward;
				if(PlatformL_Motor.Targetcirnum * 360.0f + PlatformL_Motor.TargetAngle < PlatformL_Motor.InitAngle + Platform_Angle_Sky)
				{
						PlatformL_Motor.TargetAngle = PlatformL_Motor.Targetcirnum * 360.0f + PlatformL_Motor.TargetAngle + 2.0f;
						PlatformR_Motor.TargetAngle = PlatformR_Motor.Targetcirnum * 360.0f + PlatformR_Motor.TargetAngle - 2.0f;
						Angle_Limit(&PlatformL_Motor);
						Angle_Limit(&PlatformR_Motor);
				}
		}
		if (SHIFT[1] == Pressed && SHIFT[0] == Pressed)
		{

				//平台后移
				Platform_State = Platform_State_Backward;
				if(PlatformL_Motor.Targetcirnum * 360.0f + PlatformL_Motor.TargetAngle > PlatformL_Motor.InitAngle + Platform_Angle_Exchange)
				{
						PlatformL_Motor.TargetAngle = PlatformL_Motor.Targetcirnum * 360.0f + PlatformL_Motor.TargetAngle - 2.0f;
						PlatformR_Motor.TargetAngle = PlatformR_Motor.Targetcirnum * 360.0f + PlatformR_Motor.TargetAngle + 2.0f;
						Angle_Limit(&PlatformL_Motor);
						Angle_Limit(&PlatformR_Motor);
				}
		}
}

/**
 * @brief  在兑换模式下控制pitch
 * @note	 手动流程
 * @param  兑换臂
 * @retval
 */
void Manual_Pitch_Exchange_Arm(void)
{
		if(B[1] == Pressed && B[0] == Pressed)
		{
				ArmPitch_State = ArmPitch_State_Up;
				if(ArmPitchL_Motor.Targetcirnum * 360.0f + ArmPitchL_Motor.TargetAngle > ArmPitchL_Motor.InitAngle - 97.0f)
				{
						ArmPitchL_Motor.TargetAngle = ArmPitchL_Motor.Targetcirnum * 360.0f + ArmPitchL_Motor.TargetAngle - 0.4f;
						ArmPitchR_Motor.TargetAngle = ArmPitchR_Motor.Targetcirnum * 360.0f + ArmPitchR_Motor.TargetAngle + 0.4f;
						Angle_Limit(&ArmPitchL_Motor);
						Angle_Limit(&ArmPitchR_Motor);
				}
		}
			
		if(V[1] == Pressed && V[0] == Pressed)
		{
				ArmPitch_State = ArmPitch_State_Down;
				if(ArmPitchL_Motor.Targetcirnum * 360.0f + ArmPitchL_Motor.TargetAngle < ArmPitchL_Motor.InitAngle + 20.0f)
				{
						ArmPitchL_Motor.TargetAngle = ArmPitchL_Motor.Targetcirnum * 360.0f + ArmPitchL_Motor.TargetAngle + 0.4f;
						ArmPitchR_Motor.TargetAngle = ArmPitchR_Motor.Targetcirnum * 360.0f + ArmPitchR_Motor.TargetAngle - 0.4f;
						Angle_Limit(&ArmPitchL_Motor);
						Angle_Limit(&ArmPitchR_Motor);
				}
		}
}

/**
 * @brief  在兑换模式下控制roll
 * @note	 手动流程
 * @param  兑换臂
 * @retval
 */
void Manual_Roll_Exchange_Arm(void)
{
		if(C[1] == Pressed && C[0] == Pressed)
		{
				ArmRoll_Motor.TargetAngle = ArmRoll_Motor.Targetcirnum * 360.0f + ArmRoll_Motor.TargetAngle - 1.0f;
				Angle_Limit(&ArmRoll_Motor);
		}
			
		if(X[1] == Pressed && X[0] == Pressed)
		{
				ArmRoll_Motor.TargetAngle = ArmRoll_Motor.Targetcirnum * 360.0f + ArmRoll_Motor.TargetAngle + 1.0f;
				Angle_Limit(&ArmRoll_Motor);
		}
}

/**
 * @brief  在兑换模式下控制yaw
 * @note	 手动流程
 * @param  兑换臂
 * @retval
 */
void Manual_Yaw_Exchange_Arm(void)
{
		if(Z[1] == Pressed && Z[0] == Pressed)
		{
				if(ArmYaw_Motor.Targetcirnum * 360.0f + ArmYaw_Motor.TargetAngle > ArmYaw_Motor.InitAngle - 65.0f)
				{
						ArmYaw_Motor.TargetAngle = ArmYaw_Motor.Targetcirnum * 360.0f + ArmYaw_Motor.TargetAngle - 0.5f;
						Angle_Limit(&ArmYaw_Motor);
					
						Gimbal_Motor.TargetAngle = Gimbal_Motor.Targetcirnum * 360.0f + Gimbal_Motor.TargetAngle + 0.3f;
						Angle_Limit(&Gimbal_Motor);
				}
		}
			
		if(G[1] == Pressed && G[0] == Pressed)
		{
				if(ArmYaw_Motor.Targetcirnum * 360.0f + ArmYaw_Motor.TargetAngle < ArmYaw_Motor.InitAngle + 65.0f)
				{
						ArmYaw_Motor.TargetAngle = ArmYaw_Motor.Targetcirnum * 360.0f + ArmYaw_Motor.TargetAngle + 0.5f;
						Angle_Limit(&ArmYaw_Motor);
					
						Gimbal_Motor.TargetAngle = Gimbal_Motor.Targetcirnum * 360.0f + Gimbal_Motor.TargetAngle - 0.3f;
						Angle_Limit(&Gimbal_Motor);
				}
		}
}

/**
 * @brief  在兑换模式下控制吸盘
 * @note	 手动流程
 * @param  兑换臂
 * @retval
 */
void Manual_Sucker_Exchange_Arm(void)
{
		if(Q[1] == Pressed && Q[0] == UnPressed && Arm_Sucker_State == Sucker_Off)
		{
				Arm_Sucker_State = Sucker_On;
		}
		
		if(E[1] == Pressed && E[0] == UnPressed && Arm_Sucker_State == Sucker_On)
		{
				Arm_Sucker_State = Sucker_Off;
		}
}

/**
 * @brief  在兑换模式下沿P轴前伸
 * @note	 手动流程
 * @param  兑换臂
 * @retval
 */
void Manual_Push_Exchange_Arm(void)
{
		if(R[1] == Pressed && R[0] == Pressed)
		{
				Pitch_Incline = ArmPitchL_Motor.Realcirnum * 360.0f + ArmPitchL_Motor.RealAngle + 50;
				Yaw_Incline = ArmYaw_Motor.Realcirnum * 360.0f + ArmYaw_Motor.RealAngle - 5;
				
				Lift_State = Lift_State_Push;
			
				Platform_State = Platform_State_Backward;
				if(PlatformL_Motor.Targetcirnum * 360.0f + PlatformL_Motor.TargetAngle < PlatformL_Motor.InitAngle + Platform_Angle_Sky)
				{
						PlatformL_Motor.TargetAngle = PlatformL_Motor.Targetcirnum * 360.0f + PlatformL_Motor.TargetAngle - 4*cos(Pitch_Incline * 3.14f / 180.0f);
						PlatformR_Motor.TargetAngle = PlatformR_Motor.Targetcirnum * 360.0f + PlatformR_Motor.TargetAngle + 4*cos(Pitch_Incline * 3.14f / 180.0f);
						Angle_Limit(&PlatformL_Motor);
						Angle_Limit(&PlatformR_Motor);
				}
		}	
}

/**
 * @brief  抓取模式切换到兑换模式
 * @note	 兑换臂
 * @param  
 * @retval
 */
void Pick_to_Exchange_Arm(void)
{
//		if(G[1] == Pressed && G[0] == UnPressed && Auto_Position_Flag == 0)
//		{
//				Auto_Position_Flag = 1;	
//				Auto_Protect = 1;
//			
//				Lift_State = Lift_State_Exchange;
//				Set_Lift_Target(Lift_Angle[Lift_State]);
//		}
		
		if(Auto_Position_Flag == 1)
		{
				if(Platform_State == Platform_State_Init)
				{
						if(Motor_InPosition(&PlatformL_Motor, &PlatformL_Encoder, 15) || (V[1] == Pressed && V[0]== UnPressed))
						{
								Lift_State = Lift_State_Arm_Exchange;
							
								SkyForward_State = SkyForward_State_Init;
								SkyPitch_State = SkyPitch_State_Init;
								ArmPitch_State = ArmPitch_State_Exchange;
								ArmYaw_State = ArmYaw_State_Init;
							
								Set_SkyForward_Target(SkyForward_Angle[SkyForward_State]);
								Set_SkyPitch_Target(SkyPitch_Angle[SkyPitch_State]);
								Set_ArmPitch_Target(ArmPitch_Angle[ArmPitch_State]);
								Set_ArmYaw_Target(ArmYaw_Angle[ArmYaw_State]);
							
								Auto_Position_Flag = 0;	
								Auto_Protect = 0;
						}	
				}	
		}	
}
