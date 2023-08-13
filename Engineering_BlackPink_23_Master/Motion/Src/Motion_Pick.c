/**Include Header Files**/
//抓取控制系列函数
#include "Motion_Pick.h"

/**Variable Definition**/
PCState_t PCState;
Lift_State_t Lift_State;
Platform_State_t Platform_State;
SkyForward_State_t SkyForward_State;
SkyPitch_State_t SkyPitch_State;

ArmPitch_State_t ArmPitch_State;
ArmYaw_State_t ArmYaw_State;

Sucker_State_t Arm_Sucker_State;
Sucker_State_t Sky_Sucker_State;
Gimbal_State_t Gimbal_State;

uint8_t Auto_Protect = 0;
uint8_t Auto_Sky_Flag = 0;
uint8_t Auto_Hold_Flag = 0;
uint8_t Auto_Back_Flag = 0;
uint8_t Auto_Store_Flag = 0;
uint8_t Auto_Take_Flag = 0;
uint8_t Auto_Silver_Flag = 0;
uint8_t Auto_Rotate_Flag = 0;
uint8_t Auto_Chassis_Flag = 0;
uint8_t Auto_Position_Flag = 0;

uint8_t LiftCounter = 0;
uint8_t WaitCounter = 0;

float Platform_Angle[Platform_State_Counter] = {Platform_Angle_Init, Platform_Angle_AutoSilver, Platform_Angle_BackSilver, Platform_Angle_Silver, Platform_Angle_Gold, Platform_Angle_Hero, Platform_Angle_Sky, Platform_Angle_Rotate, Platform_Angle_Arm_Init, Platform_Angle_Arm_Ground, Platform_Angle_Arm_Silver, Platform_Angle_Arm_AutoSilver, Platform_Angle_Arm_BackSilver, Platform_Angle_Arm_Store, Platform_Angle_Spin};
float SkyForward_Angle[SkyForward_State_Counter] = {SkyForward_Angle_Init, SkyForward_Angle_AutoSilver, SkyForward_Angle_BackSilver, SkyForward_Angle_Silver, SkyForward_Angle_Gold, SkyForward_Angle_Hero, SkyForward_Angle_Sky};
float SkyPitch_Angle[SkyPitch_State_Counter] = {SkyPitch_Angle_Init, SkyPitch_Angle_Silver, SkyPitch_Angle_Gold, SkyPitch_Angle_Hero, SkyPitch_Angle_Sky, SkyPitch_Angle_Rotate, SkyPitch_Angle_Plane};
float ArmPitch_Angle[ArmPitch_State_Counter] = {ArmPitch_Angle_Init, ArmPitch_Angle_Hold, ArmPitch_Angle_Ground, ArmPitch_Angle_Silver, ArmPitch_Angle_Sky, ArmPitch_Angle_Store, ArmPitch_Angle_Exchange, ArmPitch_Angle_Protect};
float ArmYaw_Angle[ArmYaw_State_Counter] = {ArmYaw_Angle_Init};

float Gimbal_Angle[Gimbal_State_Counter] = {Gimbal_Angle_Front, Gimbal_Angle_Back};
/**Function Declaration**/
/**
 * @brief  在抓取模式下使用W、S键调整抬升高度
 * @note	 保证pitch轴始终向上，手动流程
 * @param	 空接臂
 * @retval
 */
void Manual_Lift_Pick_Sky(void)
{
		if(S[1] == Pressed && S[0] == UnPressed && LiftCounter < 3)
		{
				LiftCounter++;
				if(LiftCounter == 0)
				{
						PCState = PCState_Pick_Init;
						Lift_State = Lift_State_Init;
					
						Platform_State = Platform_State_Init;
						SkyForward_State = SkyForward_State_Init;
						SkyPitch_State = SkyPitch_State_Init;
						ArmPitch_State = ArmPitch_State_Init;
					
						Set_Platform_Target(Platform_Angle[Platform_State]);
						Set_SkyForward_Target(SkyForward_Angle[SkyForward_State]);
						Set_SkyPitch_Target(SkyPitch_Angle[SkyPitch_State]);
						Set_ArmPitch_Target(ArmPitch_Angle[ArmPitch_State]);
				}
				if(LiftCounter == 1)
				{
						PCState = PCState_Pick_Silver;
						Lift_State = Lift_State_Silver;
					
						Platform_State = Platform_State_Init;
						SkyForward_State = SkyForward_State_Init;
						SkyPitch_State = SkyPitch_State_Init;
						ArmPitch_State = ArmPitch_State_Protect;
					
						Set_Platform_Target(Platform_Angle[Platform_State]);
						Set_SkyForward_Target(SkyForward_Angle[SkyForward_State]);
						Set_SkyPitch_Target(SkyPitch_Angle[SkyPitch_State]);
						Set_ArmPitch_Target(ArmPitch_Angle[ArmPitch_State]);
				}
				if(LiftCounter == 2)
				{
						PCState = PCState_Pick_Gold;
						Lift_State = Lift_State_Gold;
					
						Platform_State = Platform_State_Init;
						SkyForward_State = SkyForward_State_Init;
						SkyPitch_State = SkyPitch_State_Init;
						ArmPitch_State = ArmPitch_State_Protect;
					
						Set_Platform_Target(Platform_Angle[Platform_State]);
						Set_SkyForward_Target(SkyForward_Angle[SkyForward_State]);
						Set_SkyPitch_Target(SkyPitch_Angle[SkyPitch_State]);
						Set_ArmPitch_Target(ArmPitch_Angle[ArmPitch_State]);
				}
				if(LiftCounter == 3)
				{
						PCState = PCState_Pick_Hero;
						Lift_State = Lift_State_Hero;
					
						Platform_State = Platform_State_Init;
						SkyForward_State = SkyForward_State_Init;
						SkyPitch_State = SkyPitch_State_Init;
						ArmPitch_State = ArmPitch_State_Hold;
					
						Set_Platform_Target(Platform_Angle[Platform_State]);
						Set_SkyForward_Target(SkyForward_Angle[SkyForward_State]);
						Set_SkyPitch_Target(SkyPitch_Angle[SkyPitch_State]);
						Set_ArmPitch_Target(ArmPitch_Angle[ArmPitch_State]);
				}
		}

		if(W[1] == Pressed && W[0] == UnPressed && LiftCounter > 0)
		{
				LiftCounter--;
				if(LiftCounter == 0)
				{
						PCState = PCState_Pick_Init;
						Lift_State = Lift_State_Init;
					
						Platform_State = Platform_State_Init;
						SkyForward_State = SkyForward_State_Init;
						SkyPitch_State = SkyPitch_State_Init;
						ArmPitch_State = ArmPitch_State_Init;
					
						Set_Platform_Target(Platform_Angle[Platform_State]);
						Set_SkyForward_Target(SkyForward_Angle[SkyForward_State]);
						Set_SkyPitch_Target(SkyPitch_Angle[SkyPitch_State]);
						Set_ArmPitch_Target(ArmPitch_Angle[ArmPitch_State]);
				}
				if(LiftCounter == 1)
				{
						PCState = PCState_Pick_Silver;
						Lift_State = Lift_State_Silver;
					
						Platform_State = Platform_State_Init;
						SkyForward_State = SkyForward_State_Init;
						SkyPitch_State = SkyPitch_State_Init;
						ArmPitch_State = ArmPitch_State_Protect;
					
						Set_Platform_Target(Platform_Angle[Platform_State]);
						Set_SkyForward_Target(SkyForward_Angle[SkyForward_State]);
						Set_SkyPitch_Target(SkyPitch_Angle[SkyPitch_State]);
						Set_ArmPitch_Target(ArmPitch_Angle[ArmPitch_State]);
				}
				if(LiftCounter == 2)
				{
						PCState = PCState_Pick_Gold;
						Lift_State = Lift_State_Gold;
					
						Platform_State = Platform_State_Init;
						SkyForward_State = SkyForward_State_Init;
						SkyPitch_State = SkyPitch_State_Init;
						ArmPitch_State = ArmPitch_State_Protect;
					
						Set_Platform_Target(Platform_Angle[Platform_State]);
						Set_SkyForward_Target(SkyForward_Angle[SkyForward_State]);
						Set_SkyPitch_Target(SkyPitch_Angle[SkyPitch_State]);
						Set_ArmPitch_Target(ArmPitch_Angle[ArmPitch_State]);
				}
				if(LiftCounter == 3)
				{
						PCState = PCState_Pick_Hero;
						Lift_State = Lift_State_Hero;
					
						Platform_State = Platform_State_Init;
						SkyForward_State = SkyForward_State_Init;
						SkyPitch_State = SkyPitch_State_Init;
						ArmPitch_State = ArmPitch_State_Hold;
					
						Set_Platform_Target(Platform_Angle[Platform_State]);
						Set_SkyForward_Target(SkyForward_Angle[SkyForward_State]);
						Set_SkyPitch_Target(SkyPitch_Angle[SkyPitch_State]);
						Set_ArmPitch_Target(ArmPitch_Angle[ArmPitch_State]);
				}
		}
}

/**
 * @brief  在抓取模式下使用Shift键使pitch轴、前伸到达抓取位置
 * @note	 手动流程
 * @param  空接臂
 * @retval
 */
void Manual_Out_Pick_Sky(void)
{
		if(SHIFT[1] == Pressed && SHIFT[0] == UnPressed && PCState == PCState_Pick_Silver)
		{
				Platform_State = Platform_State_Silver;
				SkyForward_State = SkyForward_State_Silver;
				SkyPitch_State = SkyPitch_State_Silver;
			
				Set_Platform_Target(Platform_Angle[Platform_State]);
				Set_SkyForward_Target(SkyForward_Angle[SkyForward_State]);
				Set_SkyPitch_Target(SkyPitch_Angle[SkyPitch_State]);
		}
		
		else if(SHIFT[1] == Pressed && SHIFT[0] == UnPressed && PCState == PCState_Pick_Gold)
		{
				Platform_State = Platform_State_Gold;
				SkyForward_State = SkyForward_State_Gold;
				SkyPitch_State = SkyPitch_State_Gold;
			
				Set_Platform_Target(Platform_Angle[Platform_State]);
				Set_SkyForward_Target(SkyForward_Angle[SkyForward_State]);
				Set_SkyPitch_Target(SkyPitch_Angle[SkyPitch_State]);
		}
}

/**
 * @brief  在抓取模式下使用Q键长按，进行前伸微调，实现抓取
 * @note	 手动流程
 * @param	 空接臂
 * @retval
 */
void Manual_Press_Pick_Sky(void)
{
		if(Q[1] == Pressed && Q[0]== Pressed  && PCState == PCState_Pick_Init)
		{
				Sky_Sucker_State = Sucker_On;
		}
		else if(Q[1] == Pressed && Q[0]== Pressed  && PCState == PCState_Pick_Silver)
		{
				Sky_Sucker_State = Sucker_On;
				
				if(Platform_State == Platform_State_Silver && SkyForward_State == SkyForward_State_Silver)
				{
						if(PlatformL_Motor.Targetcirnum * 360.0f + PlatformL_Motor.TargetAngle < PlatformL_Motor.InitAngle + 300)
            {
								PlatformL_Motor.TargetAngle = PlatformL_Motor.Targetcirnum * 360.0f + PlatformL_Motor.TargetAngle + 1.0f;
								PlatformR_Motor.TargetAngle = PlatformR_Motor.Targetcirnum * 360.0f + PlatformR_Motor.TargetAngle - 1.0f;
								Angle_Limit(&PlatformL_Motor);
								Angle_Limit(&PlatformR_Motor);
							
								//Auto_Chassis_Flag = 1;
						}
				}
		}
				
		else if(Q[1] == Pressed && Q[0]== Pressed  && PCState == PCState_Pick_Gold)
		{
				Sky_Sucker_State = Sucker_On;
					
				if(Platform_State == Platform_State_Gold && SkyForward_State == SkyForward_State_Gold)
				{		
						if(PlatformL_Motor.Targetcirnum * 360.0f + PlatformL_Motor.TargetAngle < PlatformL_Motor.InitAngle + 400)
            {
								PlatformL_Motor.TargetAngle = PlatformL_Motor.Targetcirnum * 360.0f + PlatformL_Motor.TargetAngle + 1.0f;
								PlatformR_Motor.TargetAngle = PlatformR_Motor.Targetcirnum * 360.0f + PlatformR_Motor.TargetAngle - 1.0f;
								Angle_Limit(&PlatformL_Motor);
								Angle_Limit(&PlatformR_Motor);
						}
				}
		}
}

/**
 * @brief  在抓取模式下使用Q键下降沿，使得前伸回到初始抓取位置
 * @note	 手动流程
 * @param  空接臂
 * @retval
 */
void Manual_Init_Pick_Sky(void)
{
		
		if(Q[1] == Pressed && Q[0] == UnPressed  && PCState == PCState_Pick_Silver)
		{
				if(Platform_State == Platform_State_Silver && SkyForward_State == SkyForward_State_Silver)
				{
						Platform_State = Platform_State_Silver;
						Set_Platform_Target(Platform_Angle[Platform_State]);
					
						Auto_Chassis_Flag = 0;
				}
		}
		
		else if(Q[1] == Pressed && Q[0] == UnPressed  && PCState == PCState_Pick_Gold)
		{					
				if(Platform_State == Platform_State_Gold && SkyForward_State == SkyForward_State_Gold)
				{
						Platform_State = Platform_State_Gold;
						Set_Platform_Target(Platform_Angle[Platform_State]);
				}
		}
		
}

/**
 * @brief  在抓取模式下使用E键下降沿，关闭吸盘
 * @note	 手动流程
 * @param  空接臂
 * @retval
 */
void Manual_Suck_Sky(void)
{
		if(E[1] == Pressed && E[0] == UnPressed)
		{
				Sky_Sucker_State = Sucker_Off;
		}
}

/**
 * @brief  在抓取模式下使用CTRL	键实现pitch轴竖直保持
 * @note	 自动流程
 * @param  空接臂
 * @retval
 */
void Auto_Hold_Sky(void)
{
		if(PCState == PCState_Pick_Silver || PCState == PCState_Pick_Gold)
		{
				if(CTRL[1] == Pressed && CTRL[0]== UnPressed && Auto_Protect == 0)
				{
						Auto_Protect = 1;
						Auto_Hold_Flag = 1;
				}
		}
		
		if(Auto_Hold_Flag == 1)
    {
        if(PCState == PCState_Pick_Silver)
        {
            Auto_Hold_Silver_Sky();
        }
        if(PCState == PCState_Pick_Gold)
        {
						Auto_Hold_Gold_Sky();
        }
    }
}

/**
 * @brief  在抓取模式下小资源岛抓取时实现pitch轴竖直保持
 * @note	 自动流程
 * @param  空接臂
 * @retval
 */
void Auto_Hold_Silver_Sky(void)
{
		if(Lift_State == Lift_State_Silver)
    {
				//用于判断条件辅助，只进行状态切换，轴不进行运动	
				Platform_State = Platform_State_Silver;
				SkyForward_State = SkyForward_State_Silver;
			
				Lift_State = Lift_State_Safe;
    }
		
		else if(State_Confirm(Lift_State_Safe, Platform_State_Silver, SkyForward_State_Silver))
    {
				if((Lift_Motor_InPosition == 1) || (V[1] == Pressed && V[0]== UnPressed))
				{
						Platform_State = Platform_State_Init;
						SkyForward_State = SkyForward_State_Init;
						SkyPitch_State = SkyPitch_State_Init;

						Set_Platform_Target(Platform_Angle[Platform_State]);
						Set_SkyForward_Target(SkyForward_Angle[SkyForward_State]);
						Set_SkyPitch_Target(SkyPitch_Angle[SkyPitch_State]);
				}
				
    }
		else if(State_Confirm(Lift_State_Safe, Platform_State_Init, SkyForward_State_Init))
		{
				if(Motor_InPosition(&PlatformL_Motor, &PlatformL_Encoder, 20) || (V[1] == Pressed && V[0]== UnPressed))
				{
						Lift_State = Lift_State_Silver;
					
						Auto_Hold_Flag = 0;
						Auto_Protect = 0;
				}
		}
}

/**
 * @brief  在抓取模式下大资源岛抓取时实现pitch轴竖直保持
 * @note	 自动流程
 * @param  空接臂
 * @retval
 */
void Auto_Hold_Gold_Sky(void)
{
		if(Lift_State == Lift_State_Gold)
    {
				//用于判断条件辅助，只进行状态切换，轴不进行运动
				Platform_State = Platform_State_Gold;
				SkyForward_State = SkyForward_State_Gold;
			
				Lift_State = Lift_State_Safe;
    }
		
		else if(State_Confirm(Lift_State_Safe, Platform_State_Gold, SkyForward_State_Gold))
    {
				if(Lift_Motor_InPosition == 1 || (V[1] == Pressed && V[0]== UnPressed))
				{
						Platform_State = Platform_State_Init;
						SkyForward_State = SkyForward_State_Init;
						SkyPitch_State = SkyPitch_State_Init;

						Set_Platform_Target(Platform_Angle[Platform_State]);
						Set_SkyForward_Target(SkyForward_Angle[SkyForward_State]);
						Set_SkyPitch_Target(SkyPitch_Angle[SkyPitch_State]);
				}
				
    }
		else if(State_Confirm(Lift_State_Safe, Platform_State_Init, SkyForward_State_Init))
		{
			
				if((Motor_InPosition(&PlatformL_Motor, &PlatformL_Encoder, 20) && Motor_InPosition(&SkyForwardL_Motor, NULL, 20))|| \
					(V[1] == Pressed && V[0]== UnPressed))
				{
						Lift_State = Lift_State_Gold;
					
						Auto_Hold_Flag = 0;
						Auto_Protect = 0;
				}
		}
}

/**
 * @brief  在抓取模式下实现空接
 * @note	 自动流程
 * @param  空接臂
 * @retval
 */
void Auto_Sky_Sky(void)
{
		if(R[1] == Pressed && R[0] == UnPressed && Auto_Sky_Flag == 0)
		{
				Auto_Sky_Flag = 1;
				Auto_Chassis_Flag = 1;
				Auto_Protect = 1;

				Lift_State = Lift_State_Sky;

				Platform_State = Platform_State_Init;
				SkyForward_State = SkyForward_State_Init;
				SkyPitch_State = SkyPitch_State_Init;
			
				ArmPitch_State = ArmPitch_State_Sky;
				Set_ArmPitch_Target(ArmPitch_Angle[ArmPitch_State]);
		}
		
		if(Auto_Sky_Flag == 1)
		{
				if(State_Confirm(Lift_State_Sky, Platform_State_Init, SkyForward_State_Init) && SkyPitch_State == SkyPitch_State_Init)
				{
						if((Motor_InPosition(&ArmPitchL_Motor, NULL, 20) && Lift_Motor_InPosition == 2) || (V[1] == Pressed && V[0]== UnPressed))
						{
								Platform_State = Platform_State_Sky;
								SkyForward_State = SkyForward_State_Sky;
								SkyPitch_State = SkyPitch_State_Sky;
										
								Set_Platform_Target(Platform_Angle[Platform_State]);
								Set_SkyForward_Target(SkyForward_Angle[SkyForward_State]);
								Set_SkyPitch_Target(SkyPitch_Angle[SkyPitch_State]);

								Sky_Sucker_State = Sucker_On;	
						}
				}
				else if(R[1] == Pressed && R[0] == UnPressed && State_Confirm(Lift_State_Sky, Platform_State_Sky, SkyForward_State_Sky))
				{
						Platform_State = Platform_State_Init;
						SkyForward_State = SkyForward_State_Init;
					
						Set_Platform_Target(Platform_Angle[Platform_State]);
						Set_SkyForward_Target(SkyForward_Angle[SkyForward_State]);
					
						Auto_Chassis_Flag = 0;
				}

				else if(State_Confirm(Lift_State_Sky, Platform_State_Init, SkyForward_State_Init) && SkyPitch_State == SkyPitch_State_Sky)
				{
						if(Motor_InPosition(&PlatformL_Motor,&PlatformL_Encoder, 20)|| (V[1] == Pressed && V[0]== UnPressed))
						{
								Lift_State = Lift_State_Init;	
								SkyPitch_State = SkyPitch_State_Init;
								ArmPitch_State = ArmPitch_State_Init;		
							
								Set_SkyPitch_Target(SkyPitch_Angle[SkyPitch_State]);
								Set_ArmPitch_Target(ArmPitch_Angle[ArmPitch_State]);
							
								PCState = PCState_Pick_Init;
								LiftCounter = 0;
					
								Auto_Sky_Flag = 0;
								Auto_Protect = 0;
						}
				}
		}
}

/**
 * @brief  自动换手
 * @note	 自动流程
 * @param	 空接臂
 * @retval
 */
void Auto_Rotate_Sky(void)
{
		if(G[1] == Pressed && G[0] == UnPressed && Auto_Rotate_Flag == 0)
		{
				Auto_Rotate_Flag = 1;
				Auto_Protect = 1;
			
				//只改变状态不赋值
				Platform_State = Platform_State_Init;
				SkyForward_State = SkyForward_State_Init;
			
				Lift_State = Lift_State_Init;

				ArmPitch_State = ArmPitch_State_Protect;
				Set_ArmPitch_Target(ArmPitch_Angle[ArmPitch_State]);
			
				//用于区分流程
				Sky_Sucker_State = Sucker_On;
				Auto_Chassis_Flag = 0;
		}	
	
		if(Auto_Rotate_Flag == 1)
		{
				if(State_Confirm(Lift_State_Init, Platform_State_Init, SkyForward_State_Init) && Auto_Chassis_Flag == 0)
				{
						if(Motor_InPosition(&ArmPitchL_Motor, NULL, 20) || (V[1] == Pressed && V[0]== UnPressed))
						{
								SkyPitch_State = SkyPitch_State_Rotate;
								Platform_State = Platform_State_Rotate;

								Set_SkyPitch_Target(SkyPitch_Angle[SkyPitch_State]);
								Set_Platform_Target(Platform_Angle[Platform_State]);
						}
				}
				else if (State_Confirm(Lift_State_Init, Platform_State_Rotate, SkyForward_State_Init) && Sky_Sucker_State == Sucker_On)
				{
						if(Motor_InPosition(&PlatformL_Motor,&PlatformL_Encoder, 20) || (V[1] == Pressed && V[0]== UnPressed))
						{
								Sky_Sucker_State = Sucker_Off;
						}
				}
				else if (State_Confirm(Lift_State_Init, Platform_State_Rotate, SkyForward_State_Init) && Sky_Sucker_State == Sucker_Off)
				{
						WaitCounter++;
						if(WaitCounter > 100)
						{
								Auto_Chassis_Flag = 3;
								
								SkyPitch_State = SkyPitch_State_Init;
								Platform_State = Platform_State_Init;

								Set_SkyPitch_Target(SkyPitch_Angle[SkyPitch_State]);
								Set_Platform_Target(Platform_Angle[Platform_State]);
							
								WaitCounter = 0;
						}
				}
				else if (State_Confirm(Lift_State_Init, Platform_State_Init, SkyForward_State_Init) && Auto_Chassis_Flag == 3)
				{
						WaitCounter++;
						if(WaitCounter > 80)
						{
								Auto_Chassis_Flag = 0;
								
								ArmPitch_State = ArmPitch_State_Init;
								Set_ArmPitch_Target(ArmPitch_Angle[ArmPitch_State]);
							
								PCMode = PCMode_Pick_Arm;
								Gimbal_State = 	Gimbal_State_Back;	
								Set_Gimbal_Target(Gimbal_Angle[Gimbal_State]);
							
								PCState = PCState_Pick_Arm_Ground;
								Lift_State = Lift_State_Arm_Ground;
							
								LiftCounter = 1;
								WaitCounter = 0;
							
								Platform_State = Platform_State_Arm_Ground;
								ArmPitch_State = ArmPitch_State_Ground;
				
								Set_ArmPitch_Target(ArmPitch_Angle[ArmPitch_State]);
								Set_Platform_Target(Platform_Angle[Platform_State]);

								Auto_Rotate_Flag = 0;
								Auto_Protect = 0;
						}
				}		
		}
}

/**
 * @brief  兑换模式切换到抓取模式
 * @note	 空接臂
 * @param  
 * @retval
 */
void Exchange_to_Pick_Sky(void)
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
								Lift_State = Lift_State_Init;
								SkyForward_State = SkyForward_State_Init;
								SkyPitch_State = SkyPitch_State_Init;
								ArmPitch_State = ArmPitch_State_Init;
								ArmYaw_State = ArmYaw_State_Init;
							
								Set_SkyForward_Target(SkyForward_Angle[SkyForward_State]);
								Set_SkyPitch_Target(SkyPitch_Angle[SkyPitch_State]);
								Set_ArmPitch_Target(ArmPitch_Angle[ArmPitch_State]);
								Set_ArmYaw_Target(ArmYaw_Angle[ArmYaw_State]);
								
								LiftCounter = 0;
							
								Auto_Position_Flag = 0;	
								Auto_Protect = 0;
						}	
				}	
		}
}


/**
 * @brief  在抓取模式下使用W、S键调整抬升高度
 * @note	 保证pitch轴始终向上，手动流程
 * @param	 兑换臂
 * @retval
 */
void Manual_Lift_Pick_Arm(void)
{
		if(S[1] == Pressed && S[0] == UnPressed && LiftCounter < 2)
		{
				LiftCounter++;
				if(LiftCounter == 0)
				{
						PCState = PCState_Pick_Arm_Init;
						Lift_State = Lift_State_Arm_Init;
					
						Platform_State = Platform_State_Arm_Init;
						ArmPitch_State = ArmPitch_State_Hold;
					
						Set_Platform_Target(Platform_Angle[Platform_State]);
						Set_ArmPitch_Target(ArmPitch_Angle[ArmPitch_State]);
				}
				if(LiftCounter == 1)
				{
					
						PCState = PCState_Pick_Arm_Ground;
						Lift_State = Lift_State_Arm_Ground;
					
						Platform_State = Platform_State_Arm_Init;
						ArmPitch_State = ArmPitch_State_Hold;
					
						Set_Platform_Target(Platform_Angle[Platform_State]);
						Set_ArmPitch_Target(ArmPitch_Angle[ArmPitch_State]);
				}
				
				if(LiftCounter == 2)
				{
					
						PCState = PCState_Pick_Arm_Silver;
						Lift_State = Lift_State_Arm_Silver;
					
						Platform_State = Platform_State_Arm_Init;
						ArmPitch_State = ArmPitch_State_Hold;
					
						Set_Platform_Target(Platform_Angle[Platform_State]);
						Set_ArmPitch_Target(ArmPitch_Angle[ArmPitch_State]);
				}
		}

		if(W[1] == Pressed && W[0] == UnPressed && LiftCounter > 0)
		{
				LiftCounter--;
				if(LiftCounter == 0)
				{
						PCState = PCState_Pick_Arm_Init;
						Lift_State = Lift_State_Arm_Init;
					
						Platform_State = Platform_State_Arm_Init;
						ArmPitch_State = ArmPitch_State_Hold;

						Set_Platform_Target(Platform_Angle[Platform_State]);
						Set_ArmPitch_Target(ArmPitch_Angle[ArmPitch_State]);

				}
				if(LiftCounter == 1)
				{
					
						PCState = PCState_Pick_Arm_Ground;
						Lift_State = Lift_State_Arm_Ground;
					
						Platform_State = Platform_State_Arm_Init;
						ArmPitch_State = ArmPitch_State_Hold;
					
						Set_Platform_Target(Platform_Angle[Platform_State]);
						Set_ArmPitch_Target(ArmPitch_Angle[ArmPitch_State]);

				}
				
				if(LiftCounter == 2)
				{
					
						PCState = PCState_Pick_Arm_Silver;
						Lift_State = Lift_State_Arm_Silver;
					
						Platform_State = Platform_State_Arm_Init;
						ArmPitch_State = ArmPitch_State_Hold;
					
						Set_Platform_Target(Platform_Angle[Platform_State]);
						Set_ArmPitch_Target(ArmPitch_Angle[ArmPitch_State]);
				}
		}
}

/**
 * @brief  在抓取模式下使用Shift键使pitch轴、前伸到达抓取位置
 * @note	 手动流程
 * @param  兑换臂
 * @retval
 */
void Manual_Out_Pick_Arm(void)
{
		if(SHIFT[1] == Pressed && SHIFT[0] == UnPressed && PCState == PCState_Pick_Arm_Ground)
		{
				Platform_State = Platform_State_Arm_Ground;
				ArmPitch_State = ArmPitch_State_Ground;
			
				Set_ArmPitch_Target(ArmPitch_Angle[ArmPitch_State]);
				Set_Platform_Target(Platform_Angle[Platform_State]);
		}
		else if(SHIFT[1] == Pressed && SHIFT[0] == UnPressed && PCState == PCState_Pick_Arm_Silver)
		{
				Platform_State = Platform_State_Arm_Silver;
				ArmPitch_State = ArmPitch_State_Silver;
			
				Set_Platform_Target(Platform_Angle[Platform_State]);
				Set_ArmPitch_Target(ArmPitch_Angle[ArmPitch_State]);
		}
}

/**
 * @brief  在兑换模式下使用Q键长按，进行抬升、前伸微调，实现抓取
 * @note	 手动流程
 * @param	 兑换臂
 * @retval
 */
void Manual_Press_Pick_Arm(void)
{
	
		if(Q[1] == Pressed && Q[0]== Pressed  && PCState == PCState_Pick_Arm_Init)
		{
				Arm_Sucker_State = Sucker_On;
		}
		else if(Q[1] == Pressed && Q[0]== Pressed && PCState == PCState_Pick_Arm_Ground)
		{
				Arm_Sucker_State = Sucker_On;
				
				if(Platform_State == Platform_State_Arm_Ground && ArmPitch_State == ArmPitch_State_Ground)
				{
						Lift_State = Lift_State_Press;
				}
		}
				
		else if(Q[1] == Pressed && Q[0]== Pressed && PCState == PCState_Pick_Arm_Silver)
		{
				Arm_Sucker_State = Sucker_On;
				
				if(Platform_State == Platform_State_Arm_Silver && ArmPitch_State == ArmPitch_State_Silver)
				{
						if(PlatformL_Motor.Targetcirnum * 360.0f + PlatformL_Motor.TargetAngle > PlatformL_Motor.InitAngle + Platform_Angle_Exchange)
						{
								PlatformL_Motor.TargetAngle = PlatformL_Motor.Targetcirnum * 360.0f + PlatformL_Motor.TargetAngle - 1.0f;
								PlatformR_Motor.TargetAngle = PlatformR_Motor.Targetcirnum * 360.0f + PlatformR_Motor.TargetAngle + 1.0f;
								Angle_Limit(&PlatformL_Motor);
								Angle_Limit(&PlatformR_Motor);
							
								//Auto_Chassis_Flag = 1;
						}
				}
		}		
}

/**
 * @brief  在兑换模式下使用Q键下降沿，使得抬升、前伸回到初始抓取位置
 * @note	 手动流程
 * @param
 * @retval
 */
void Manual_Init_Pick_Arm(void)
{
		if(Q[1] == Pressed && Q[0] == UnPressed  && PCState == PCState_Pick_Arm_Ground)
		{
				if(Platform_State == Platform_State_Arm_Ground && ArmPitch_State == ArmPitch_State_Ground)
				{
						Lift_State = Lift_State_Arm_Ground;
				}
		}
				
		else if(Q[1] == Pressed && Q[0]== UnPressed && PCState == PCState_Pick_Arm_Silver)
		{	
				if(Platform_State == Platform_State_Arm_Silver && ArmPitch_State == ArmPitch_State_Silver)
				{
						Platform_State = Platform_State_Arm_Silver;
						Set_Platform_Target(Platform_Angle[Platform_State]);
					
						Auto_Chassis_Flag = 0;
				}
		}	
}

/**
 * @brief  在抓取模式下使用E键下降沿，关闭吸盘
 * @note	 手动流程
 * @param  兑换臂
 * @retval
 */
void Manual_Suck_Arm(void)
{
		if(E[1] == Pressed && E[0] == UnPressed)
		{
				Arm_Sucker_State = Sucker_Off;
		}
}

/**
 * @brief  在抓取模式下使用CTRL	键实现pitch轴倾斜保持/存矿
 * @note	 自动流程
 * @param  兑换臂
 * @retval
 */
void Auto_HoldBackStore_Arm(void)
{
		if(PCState == PCState_Pick_Arm_Ground)
		{
				if(CTRL[1] == Pressed && CTRL[0]== UnPressed && Lift_State != Lift_State_Safe && Auto_Protect == 0)
				{
						Auto_Protect = 1;
						Auto_Hold_Flag = 1;
				}
		}
		else if(PCState == PCState_Pick_Arm_Silver)
		{
				if(CTRL[1] == Pressed && CTRL[0]== UnPressed && ArmPitch_State == ArmPitch_State_Hold && Auto_Protect == 0)	
				{
						Auto_Protect = 1;
						Auto_Store_Flag = 1;
				}
				else
				{
						if(CTRL[1] == Pressed && CTRL[0]== UnPressed && Lift_State != Lift_State_Safe && Auto_Protect == 0)
						{
								Auto_Protect = 1;
								Auto_Hold_Flag = 1;
						}
						if(CTRL[1] == Pressed && CTRL[0]== UnPressed && Lift_State == Lift_State_Safe && Auto_Protect == 0)
						{
								Auto_Protect = 1;
								Auto_Back_Flag = 1;
						}	
				}
		}

		
		if(Auto_Hold_Flag == 1)
    {
        if(PCState == PCState_Pick_Arm_Ground)
        {
            Auto_Hold_Ground_Arm();
        }
        if(PCState == PCState_Pick_Arm_Silver)
        {		
						Auto_Hold_Silver_Arm();
        }
    }
		
		if(Auto_Back_Flag == 1)
    {
        if(PCState == PCState_Pick_Arm_Silver)
        {
            Auto_Back_Arm();
        }
    }
		
		if(Auto_Store_Flag == 1)
    {
        if(PCState == PCState_Pick_Arm_Silver)
        {
            Auto_Store_Arm();
        }
    }
}

/**
 * @brief  在抓取模式下地面抓取时实现pitch轴倾斜保持
 * @note	 自动流程
 * @param  兑换臂
 * @retval
 */
void Auto_Hold_Ground_Arm(void)
{
		Platform_State = Platform_State_Arm_Init;
		ArmPitch_State = ArmPitch_State_Hold;

		Set_Platform_Target(Platform_Angle[Platform_State]);
		Set_ArmPitch_Target(ArmPitch_Angle[ArmPitch_State]);
	
		Auto_Protect = 0;
		Auto_Hold_Flag = 0;
}

/**
 * @brief  在抓取模式下小资源岛抓取时实现pitch轴水平保持
 * @note	 自动流程
 * @param  兑换臂
 * @retval
 */
void Auto_Hold_Silver_Arm(void)
{		
		Lift_State = Lift_State_Safe;
			
		Auto_Protect = 0;
		Auto_Hold_Flag = 0;
}

/**
 * @brief  在抓取模式下小资源岛抓取时实现pitch轴倾斜保持
 * @note	 自动流程
 * @param  兑换臂
 * @retval
 */
void Auto_Back_Arm(void)
{		
		Lift_State = Lift_State_Arm_Silver;
	
		ArmPitch_State = ArmPitch_State_Hold;
		Platform_State = Platform_State_Arm_Init;
		
		Set_Platform_Target(Platform_Angle[Platform_State]);
		Set_ArmPitch_Target(ArmPitch_Angle[ArmPitch_State]);

		Auto_Protect = 0;
		Auto_Back_Flag = 0;

}
/**
 * @brief  在抓取模式下小资源岛抓取时实现存矿
 * @note	 自动流程
 * @param  兑换臂
 * @retval
 */
void Auto_Store_Arm(void)
{
		if(ArmPitch_State == ArmPitch_State_Hold)
		{
			//只改变状态不改变角度
			SkyForward_State = SkyForward_State_Init;
			Platform_State = Platform_State_Arm_Init;
			ArmPitch_State = ArmPitch_State_Init;
			
			Lift_State = Lift_State_Store;
			
			Arm_Sucker_State = Sucker_On;
		}
		else if(State_Confirm(Lift_State_Store, Platform_State_Arm_Init, SkyForward_State_Init))
		{
				if(Lift_Motor_InPosition == 1 || (V[1] == Pressed && V[0]== UnPressed))
				{
						Platform_State = Platform_State_Arm_Store;
						ArmPitch_State = ArmPitch_State_Store;
						
						Set_ArmPitch_Target(ArmPitch_Angle[ArmPitch_State]);
						Set_Platform_Target(Platform_Angle[Platform_State]);
				}
		}
		else if(State_Confirm(Lift_State_Store, Platform_State_Arm_Store, SkyForward_State_Init))
		{
				if(Motor_InPosition(&PlatformL_Motor, &PlatformL_Encoder, 20) && Lift_Motor_InPosition == 1)
				{
							WaitCounter++;
				}
				if(WaitCounter > 100 || (V[1] == Pressed && V[0]== UnPressed))
				{
						Lift_State = Lift_State_StoreDown;
				}
		}
		else if(State_Confirm(Lift_State_StoreDown, Platform_State_Arm_Store, SkyForward_State_Init) && Arm_Sucker_State == Sucker_On)
		{	
				WaitCounter++;
			
				if(WaitCounter > 150)
				{
						Arm_Sucker_State = Sucker_Off;
						WaitCounter = 0;
				}
		}
		else if(State_Confirm(Lift_State_StoreDown, Platform_State_Arm_Store, SkyForward_State_Init) && Arm_Sucker_State == Sucker_Off)
		{	
				WaitCounter++;
			
				if(WaitCounter > 150)
				{
						Lift_State = Lift_State_StoreUp;
					
						WaitCounter = 0;
				}
		}
		else if(State_Confirm(Lift_State_StoreUp, Platform_State_Arm_Store, SkyForward_State_Init))
		{	
				if(Lift_Motor_InPosition == 1)
				{
						WaitCounter++;
				}	
				if(WaitCounter > 40 || (V[1] == Pressed && V[0]== UnPressed))
				{
						ArmPitch_State = ArmPitch_State_Hold;
						Platform_State = Platform_State_Init;
						Set_ArmPitch_Target(ArmPitch_Angle[ArmPitch_State]);
						Set_Platform_Target(Platform_Angle[Platform_State]);
					
						PCState = PCState_Pick_Arm_Silver;
						Lift_State = Lift_State_Arm_Silver;
					
						LiftCounter = 2;
						WaitCounter = 0;
					
						Auto_Protect = 0;
						Auto_Store_Flag = 0;							
				}
				
		}
}

/**
 * @brief  在抓取模式下实现取出矿石
 * @note	 自动流程
 * @param  兑换臂
 * @retval
 */
void Auto_Take_Arm(void)
{
		if(G[1] == Pressed && G[0] == UnPressed && Auto_Take_Flag == 0)
		{
				Auto_Take_Flag = 1;
				Auto_Protect = 1;

				//只改变状态不改变角度
				SkyForward_State = SkyForward_State_Init;
			
				Lift_State = Lift_State_Store;
			
				Platform_State = Platform_State_Arm_Store;
				ArmPitch_State = ArmPitch_State_Store;
				
				Set_ArmPitch_Target(ArmPitch_Angle[ArmPitch_State]);
				Set_Platform_Target(Platform_Angle[Platform_State]);
				
				Arm_Sucker_State = Sucker_Off;
		}
		if(Auto_Take_Flag == 1)
		{
				if(State_Confirm(Lift_State_Store, Platform_State_Arm_Store, SkyForward_State_Init) && Arm_Sucker_State == Sucker_Off)
				{
						if((Motor_InPosition(&PlatformL_Motor, &PlatformL_Encoder, 20) && Lift_Motor_InPosition == 1) || \
							(V[1] == Pressed && V[0]== UnPressed))
						{
								Lift_State = Lift_State_Take;
								
								Arm_Sucker_State = Sucker_On;
						}
				}
				else if(State_Confirm(Lift_State_Take, Platform_State_Arm_Store, SkyForward_State_Init))
				{	
						WaitCounter++;
					
						if(WaitCounter > 150)
						{
								Lift_State = Lift_State_Store;
								WaitCounter = 0;
						}
				}
				else if(State_Confirm(Lift_State_Store, Platform_State_Arm_Store, SkyForward_State_Init) && Arm_Sucker_State == Sucker_On)
				{	
						if(Lift_Motor_InPosition == 1 || (V[1] == Pressed && V[0]== UnPressed))
						{
								Platform_State = Platform_State_Arm_Init;
								ArmPitch_State = ArmPitch_State_Hold;
						
								Set_ArmPitch_Target(ArmPitch_Angle[ArmPitch_State]);
								Set_Platform_Target(Platform_Angle[Platform_State]);
						}
				}
				else if(State_Confirm(Lift_State_Store, Platform_State_Arm_Init, SkyForward_State_Init))
				{	
						if((Motor_InPosition(&ArmPitchL_Motor, NULL, 20) && Motor_InPosition(&PlatformL_Motor, &PlatformL_Encoder, 20)) || \
							(V[1] == Pressed && V[0]== UnPressed))
						{
								PCState = PCState_Pick_Arm_Silver;
								Lift_State = Lift_State_Arm_Silver;
								
								LiftCounter = 2;
							
								Auto_Take_Flag = 0;
								Auto_Protect = 0;
						}
				}
		}
}
		

/**
 * @brief  在抓取模式下抓取小资源岛
 * @note	 自动流程
 * @param  兑换臂
 * @retval
 */
//void Auto_Pick_Silver_Arm()
//{
//		if(B[1] == Pressed && B[0] == UnPressed && Auto_Silver_Flag == 0)
//		{
//				Auto_Silver_Flag = 1;
//				Auto_Protect = 1;

//				//只改变状态不改变角度
//				SkyForward_State = SkyForward_State_Init;
//			
//				Lift_State = Lift_State_Arm_Silver;
//			
//				Platform_State = Platform_State_Arm_Init;
//				ArmPitch_State = ArmPitch_State_Hold;
//				
//				Set_ArmPitch_Target(ArmPitch_Angle[ArmPitch_State]);
//				Set_Platform_Target(Platform_Angle[Platform_State]);
//		}
//		if(Auto_Silver_Flag == 1)
//		{
//				if(State_Confirm(Lift_State_Arm_Silver, Platform_State_Arm_Init, SkyForward_State_Init))
//				{
//						if((Lift_Motor_InPosition == 1 && Motor_InPosition(&ArmPitchL_Motor, NULL, 20)) || \
//							(V[1] == Pressed && V[0]== UnPressed))
//						{
//								Platform_State = Platform_State_Arm_Silver;
//								ArmPitch_State = ArmPitch_State_Silver;
//						
//								Set_ArmPitch_Target(ArmPitch_Angle[ArmPitch_State]);
//								Set_Platform_Target(Platform_Angle[Platform_State]);
//						}
//				}
//				else if(B[1] == Pressed && B[0] == UnPressed && State_Confirm(Lift_State_Arm_Silver, Platform_State_Arm_Silver, SkyForward_State_Init))
//				{
//						Arm_Sucker_State = Sucker_On;
//						Platform_State = Platform_State_Arm_AutoSilver;
//						Set_Platform_Target(Platform_Angle[Platform_State]);
//				}
//				else if(State_Confirm(Lift_State_Arm_Silver, Platform_State_Arm_AutoSilver, SkyForward_State_Init))
//				{
//						WaitCounter++;
//					
//						if(WaitCounter > 80)
//						{
//								Platform_State = Platform_State_Arm_BackSilver;
//									Set_Platform_Target(Platform_Angle[Platform_State]);
//								WaitCounter = 0;
//						}
//				}
//				else if(State_Confirm(Lift_State_Arm_Silver, Platform_State_Arm_BackSilver, SkyForward_State_Init))
//				{
//						if(Motor_InPosition(&PlatformL_Motor, &PlatformL_Encoder, 20) || (V[1] == Pressed && V[0]== UnPressed))
//						{
//								Lift_State = Lift_State_Safe;
//						}
//				}
//				else if(State_Confirm(Lift_State_Safe, Platform_State_Arm_BackSilver, SkyForward_State_Init) && Auto_Chassis_Flag == 0)
//				{
//						if(Lift_Motor_InPosition == 1 || (V[1] == Pressed && V[0]== UnPressed))
//						{
//								Auto_Chassis_Flag = 2;
//						}
//				}
//				else if(State_Confirm(Lift_State_Safe, Platform_State_Arm_BackSilver, SkyForward_State_Init) && Auto_Chassis_Flag == 2)
//				{
//						WaitCounter++;
//					
//						if(WaitCounter > 150)
//						{
//								Auto_Chassis_Flag = 0;
//							
//								PCState = PCState_Pick_Arm_Silver;
//								Lift_State = Lift_State_Arm_Silver;
//			
//								ArmPitch_State = ArmPitch_State_Hold;
//								Platform_State = Platform_State_Arm_Init;
//								
//								Set_Platform_Target(Platform_Angle[Platform_State]);
//								Set_ArmPitch_Target(ArmPitch_Angle[ArmPitch_State]);
//							
//								LiftCounter = 2;
//								WaitCounter = 0;
//							
//								Auto_Protect = 0;
//								Auto_Silver_Flag = 0;
//						}
//				}
//		}
//}

void Auto_Pick_Silver_Arm()
{
		if(B[1] == Pressed && B[0] == UnPressed && Auto_Silver_Flag == 0)
		{
				Auto_Silver_Flag = 1;
				Auto_Protect = 1;

				//只改变状态不改变角度
				SkyForward_State = SkyForward_State_Init;
			
				Lift_State = Lift_State_Arm_Silver;
			
				Platform_State = Platform_State_Arm_Init;
				ArmPitch_State = ArmPitch_State_Hold;
				
				Set_ArmPitch_Target(ArmPitch_Angle[ArmPitch_State]);
				Set_Platform_Target(Platform_Angle[Platform_State]);
		}
		if(Auto_Silver_Flag == 1)
		{
				if(State_Confirm(Lift_State_Arm_Silver, Platform_State_Arm_Init, SkyForward_State_Init))
				{
						if((Lift_Motor_InPosition == 1 && Motor_InPosition(&ArmPitchL_Motor, NULL, 20)) || \
							(V[1] == Pressed && V[0]== UnPressed))
						{
								Platform_State = Platform_State_Arm_Silver;
								ArmPitch_State = ArmPitch_State_Silver;
						
								Set_ArmPitch_Target(ArmPitch_Angle[ArmPitch_State]);
								Set_Platform_Target(Platform_Angle[Platform_State]);
							
								Auto_Chassis_Flag = 1;
						}
				}
				else if(B[1] == Pressed && B[0] == UnPressed && State_Confirm(Lift_State_Arm_Silver, Platform_State_Arm_Silver, SkyForward_State_Init))
				{
						Arm_Sucker_State = Sucker_On;
						Platform_State = Platform_State_Arm_AutoSilver;
						Set_Platform_Target(Platform_Angle[Platform_State]);
				}
				else if(State_Confirm(Lift_State_Arm_Silver, Platform_State_Arm_AutoSilver, SkyForward_State_Init))
				{
						WaitCounter++;
					
						if(WaitCounter > 80)
						{
								Lift_State = Lift_State_Safe;
							
								Platform_State = Platform_State_Arm_BackSilver;
								Set_Platform_Target(Platform_Angle[Platform_State]);
								
								Auto_Chassis_Flag = 0;
							
								WaitCounter = 0;
						}
				}
				else if(State_Confirm(Lift_State_Safe, Platform_State_Arm_BackSilver, SkyForward_State_Init) && Auto_Chassis_Flag == 0)
				{
						if(Lift_Motor_InPosition == 1 || (V[1] == Pressed && V[0]== UnPressed))
						{
								Auto_Chassis_Flag = 2;
						}
				}
				else if(State_Confirm(Lift_State_Safe, Platform_State_Arm_BackSilver, SkyForward_State_Init) && Auto_Chassis_Flag == 2)
				{
						WaitCounter++;
					
						if(WaitCounter > 50)
						{
								Auto_Chassis_Flag = 0;
							
								PCState = PCState_Pick_Arm_Silver;
								Lift_State = Lift_State_Arm_Silver;
			
								ArmPitch_State = ArmPitch_State_Hold;
								Platform_State = Platform_State_Arm_Init;
								
								Set_Platform_Target(Platform_Angle[Platform_State]);
								Set_ArmPitch_Target(ArmPitch_Angle[ArmPitch_State]);
							
								LiftCounter = 2;
								WaitCounter = 0;
							
								Auto_Protect = 0;
								Auto_Silver_Flag = 0;
						}
				}
		}
}

/**
 * @brief  兑换模式切换到抓取模式
 * @note	 兑换臂
 * @param  
 * @retval
 */
void Exchange_to_Pick_Arm(void)
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
								if(Change_Flag == 1)
								{
										PCState = PCState_Pick_Arm_Init;
										Lift_State = Lift_State_Arm_Init;
										
										LiftCounter = 0;								
								}
								if(Change_Flag == 2)
								{
										PCState = PCState_Pick_Arm_Silver;
										Lift_State = Lift_State_Arm_Silver;
									
										LiftCounter = 2;	
								}	
									
								SkyForward_State = SkyForward_State_Init;
								SkyPitch_State = SkyPitch_State_Init;
								ArmPitch_State = ArmPitch_State_Hold;
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
 * @brief  状态确认
 * @note	 用于自动流程切换
 * @param
 * @retval
 */
uint8_t State_Confirm(Lift_State_t Expected_Lift_State, Platform_State_t Expected_Platform_State, SkyForward_State_t Expected_SkyForward_State)
{
    if(Lift_State == Expected_Lift_State && Platform_State == Expected_Platform_State && SkyForward_State == Expected_SkyForward_State)
        return 1;
    else
        return 0;
}


/**
 * @brief  吸盘控制
 * @note	 
 * @param
 * @retval
 */
void Sucker_StateChange(void)
{
		if(Sky_Sucker_State == Sucker_On)
		{
			HAL_GPIO_WritePin(Sky_Sucker_GPIO_Port,Sky_Sucker_Pin,GPIO_PIN_SET);
			HAL_GPIO_WritePin(Air2_GPIO_Port,Air2_Pin,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(Air3_GPIO_Port,Air3_Pin,GPIO_PIN_RESET);
		}
		if(Sky_Sucker_State == Sucker_Off)
		{
			HAL_GPIO_WritePin(Sky_Sucker_GPIO_Port,Sky_Sucker_Pin,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(Air2_GPIO_Port,Air2_Pin,GPIO_PIN_SET);
			HAL_GPIO_WritePin(Air3_GPIO_Port,Air3_Pin,GPIO_PIN_SET);
		}
		
		if(Arm_Sucker_State == Sucker_On)
		{
			HAL_GPIO_WritePin(Arm_Sucker_GPIO_Port,Arm_Sucker_Pin,GPIO_PIN_SET);
			HAL_GPIO_WritePin(Air1_GPIO_Port,Air1_Pin,GPIO_PIN_RESET);
			
		}
		if(Arm_Sucker_State == Sucker_Off)
		{
			HAL_GPIO_WritePin(Arm_Sucker_GPIO_Port,Arm_Sucker_Pin,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(Air1_GPIO_Port,Air1_Pin,GPIO_PIN_SET);	
		}
}



