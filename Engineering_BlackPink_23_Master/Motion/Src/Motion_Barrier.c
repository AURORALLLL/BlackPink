/**Include Header Files**/
#include "Motion_Barrier.h"


/**Variable Definition**/
uint8_t Auto_Barrier_Flag = 0;

/**Function Delaration**/
void Manual_Lift_Barrier(void)
{
		if(S[1] == Pressed && S[0] == Pressed)
		{
				if(LiftL_Motor.Targetcirnum * 360.0f + LiftL_Motor.TargetAngle < 1510.0f)
        {
						Lift_State = Lift_State_Up;
							
						LiftL_Motor.TargetAngle = LiftL_Motor.Targetcirnum * 360.0f + LiftL_Motor.TargetAngle + 3.0f;
            LiftR_Motor.TargetAngle = LiftR_Motor.Targetcirnum * 360.0f + LiftR_Motor.TargetAngle - 3.0f;
            Angle_Limit(&LiftL_Motor);
		        Angle_Limit(&LiftR_Motor);
        }
		}
		
		if(W[1] == Pressed && W[0] == Pressed)
		{
				if(LiftL_Motor.Targetcirnum * 360.0f + LiftL_Motor.TargetAngle > 0.1f)
        {
						Lift_State = Lift_State_Down;
							
						LiftL_Motor.TargetAngle = LiftL_Motor.Targetcirnum * 360.0f + LiftL_Motor.TargetAngle - 3.0f;
            LiftR_Motor.TargetAngle = LiftR_Motor.Targetcirnum * 360.0f + LiftR_Motor.TargetAngle + 3.0f;
            Angle_Limit(&LiftL_Motor);
		        Angle_Limit(&LiftR_Motor);
        }
		}
			
}

void Manual_Sucker_Barrier(void)
{
		if(Q[1] == Pressed && Q[0] == UnPressed && Sucker_State == Sucker_Off)
		{
				Sucker_State = Sucker_On;
		}
		
		if(E[1] == Pressed && E[0] == UnPressed && Sucker_State == Sucker_On)
		{
				Sucker_State = Sucker_Off;
		}
}

void Manual_Forward_Barrier(void)
{
		if (SHIFT[1] == Pressed && SHIFT[0] == Pressed)
		{
				Forward_State = Forward_State_Out;
				
				if(ForwardL_Motor.Targetcirnum * 360.0f + ForwardL_Motor.TargetAngle < 855.0f + ForwardL_Motor.InitAngle)
        {
						ForwardL_Motor.TargetAngle = ForwardL_Motor.Targetcirnum * 360.0f + ForwardL_Motor.TargetAngle + 2.0f;
            ForwardR_Motor.TargetAngle = ForwardR_Motor.Targetcirnum * 360.0f + ForwardR_Motor.TargetAngle - 2.0f;
            Angle_Limit(&ForwardL_Motor);
		        Angle_Limit(&ForwardR_Motor);
        }
		}
			
		if (CTRL[1] == Pressed && CTRL[0] == Pressed)
		{
				Forward_State = Forward_State_Back;
				
				if(ForwardL_Motor.Targetcirnum * 360.0f + ForwardL_Motor.TargetAngle > 200.0f + ForwardL_Motor.InitAngle)
        {
						ForwardL_Motor.TargetAngle = ForwardL_Motor.Targetcirnum * 360.0f + ForwardL_Motor.TargetAngle - 2.0f;
            ForwardR_Motor.TargetAngle = ForwardR_Motor.Targetcirnum * 360.0f + ForwardR_Motor.TargetAngle + 2.0f;
            Angle_Limit(&ForwardL_Motor);
		        Angle_Limit(&ForwardR_Motor);
        }
				if(ForwardL_Motor.Targetcirnum * 360.0f + ForwardL_Motor.TargetAngle > 0.1f + ForwardL_Motor.InitAngle && \
						ForwardL_Motor.Targetcirnum * 360.0f + ForwardL_Motor.TargetAngle <= 200.0f + ForwardL_Motor.InitAngle && \
						PitchStatus.TargetAngle > 232 )
        {
						ForwardL_Motor.TargetAngle = ForwardL_Motor.Targetcirnum * 360.0f + ForwardL_Motor.TargetAngle - 3.0f;
            ForwardR_Motor.TargetAngle = ForwardR_Motor.Targetcirnum * 360.0f + ForwardR_Motor.TargetAngle + 3.0f;
            Angle_Limit(&ForwardL_Motor);
		        Angle_Limit(&ForwardR_Motor);
        }
		}
}

void Barrier_Init(void)
{
		if(Auto_Barrier_Flag == 1)
		{
				if(Lift_State == Lift_State_Safe)
				{
						if(Motor_InPosition(&LiftL_Motor, &LiftL_Encoder, 20) || (Z[1] != Pressed && Z[0] != Pressed && V[1] == Pressed && V[0]== UnPressed))
						{
								Lift_State = Lift_State_Barrier;
								Forward_State = Forward_State_Barrier;
								Pitch_State = Pitch_State_Barrier;
								Roll_State = Roll_State_Init;
							
								Set_Lift_Target(Lift_Angle[Lift_State]);
								Set_Roll_Target(Roll_State);
								Set_Forward_Target(Forward_Angle[Forward_State]);
								Set_Pitch_Target(Pitch_Angle[Pitch_State]);
					
								Auto_Barrier_Flag = 0;	
								Auto_Protect = 0;
						}
				}	
		}
}

					
					





//void Manual_Pitch_Barrier(void)
//{
//		if(B[1] == Pressed && B[0] == Pressed)
//		{
//				if(PitchStatus.TargetAngle < 308.0f)
//				{
//						Pitch_State = Pitch_State_Back;
//					
//						PitchStatus.TargetAngle = PitchStatus.TargetAngle + 0.5f;	
//				}
//				
//		}
//		if(V[1] == Pressed && V[0] == Pressed)
//		{
//				if(PitchStatus.TargetAngle > 30.0f)
//				{
//						Pitch_State = Pitch_State_Out;
//					
//						PitchStatus.TargetAngle = PitchStatus.TargetAngle - 0.5f;	
//				}
//		}
//}
