/**Include Header Files**/
#include "Motion_Exchange.h"


/**Variable Definition**/
uint8_t Auto_Position_Flag = 0;
float Pitch_Incline;

/**Function Delaration**/
void Manual_Lift_Exchange(void)
{
		if(S[1] == Pressed && S[0] == Pressed)
		{
				Lift_State = Lift_State_Up;
				
				if(LiftL_Motor.Targetcirnum * 360.0f + LiftL_Motor.TargetAngle < 1430.0f)
        {	
						LiftL_Motor.TargetAngle = LiftL_Motor.Targetcirnum * 360.0f + LiftL_Motor.TargetAngle + 3.0f;
            LiftR_Motor.TargetAngle = LiftR_Motor.Targetcirnum * 360.0f + LiftR_Motor.TargetAngle - 3.0f;
            Angle_Limit(&LiftL_Motor);
		        Angle_Limit(&LiftR_Motor);
        }
		}
		
		if(W[1] == Pressed && W[0] == Pressed)
		{
				Lift_State = Lift_State_Down;
			
				if(LiftL_Motor.Targetcirnum * 360.0f + LiftL_Motor.TargetAngle > 0.1f)
        {
						LiftL_Motor.TargetAngle = LiftL_Motor.Targetcirnum * 360.0f + LiftL_Motor.TargetAngle - 3.0f;
            LiftR_Motor.TargetAngle = LiftR_Motor.Targetcirnum * 360.0f + LiftR_Motor.TargetAngle + 3.0f;
            Angle_Limit(&LiftL_Motor);
		        Angle_Limit(&LiftR_Motor);
        }
		}
			
}


void Manual_Sucker_Exchange(void)
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

void Manual_Forward_Exchange(void)
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
void Manual_Pitch_Exchange(void)
{
		if(Z[0] != Pressed && Z[1] != Pressed)
		{
				if(B[1] == Pressed && B[0] == Pressed)
				{
						if(PitchStatus.TargetAngle < 308.0f)
						{
								Pitch_State = Pitch_State_Back;
							
								PitchStatus.TargetAngle = PitchStatus.TargetAngle + 0.5f;	
						}
				}
				if(V[1] == Pressed && V[0] == Pressed)
				{
						if(PitchStatus.TargetAngle > 30.0f)
						{
								Pitch_State = Pitch_State_Out;
							
								PitchStatus.TargetAngle = PitchStatus.TargetAngle - 0.5f;	
						}
				}
		}
}

void Manual_Roll_Exchange(void)
{
		if(Z[0] != Pressed && Z[1] != Pressed)
		{
				if(X[1] == Pressed && X[0] == Pressed)
				{
						//if(Roll_Motor.TargetAngle < 150.0f)
						//{
								Roll_State = Roll_State_Left;	
							
								Roll_Motor.TargetAngle = Roll_Motor.TargetAngle + 0.5f;	
						//}
				}
				if(C[1] == Pressed && C[0] == Pressed)
				{
						//if(Roll_Motor.TargetAngle > -150.0f)
						//{
								Roll_State = Roll_State_Right;
							
								Roll_Motor.TargetAngle = Roll_Motor.TargetAngle - 0.5f;	
						//}
				}
		}
}


void Manual_Rotate_Exchange(void)
{
		if(Z[1] == Pressed && Z[0] == Pressed)
		{
				if(V[0] == Pressed && V[1]==Pressed)
				{
						Set_Rotate_Speed(-ROTATE_SPEED, -ROTATE_SPEED);
				}
				else if(X[0] == Pressed && X[1]==Pressed)
				{		
						Set_Rotate_Speed(-ROTATE_SPEED, ROTATE_SPEED);
				}
				else if(C[0] == Pressed && C[1]==Pressed)
				{
						Set_Rotate_Speed(ROTATE_SPEED, -ROTATE_SPEED);
				}
				else if(B[0] == Pressed && B[1]==Pressed)
				{
						Set_Rotate_Speed(ROTATE_SPEED, ROTATE_SPEED);
				}
				else
				{
						Set_Rotate_Speed(0,0);
				}	
		}
		else
		{
				Set_Rotate_Speed(0,0);
		}
		
}

void Position_Back(void)
{
		if(G[1] == Pressed && G[0] == UnPressed && Auto_Position_Flag == 0)
		{
				Auto_Position_Flag = 1;	
				Auto_Protect = 1;
			
				Lift_State = Lift_State_Exchange;
				Set_Lift_Target(Lift_Angle[Lift_State]);
		}
		
		if(Auto_Position_Flag == 1)
		{
				if(Lift_State == Lift_State_Exchange)
				{
						if(Motor_InPosition(&LiftL_Motor, &LiftL_Encoder, 40) || (Z[1] != Pressed && Z[0] != Pressed && V[1] == Pressed && V[0]== UnPressed))
						{
								Forward_State = Forward_State_Exchange;
								Pitch_State = Pitch_State_Exchange;
								Roll_State = Roll_State_Init;	
								
								Set_Roll_Target(Roll_State);
								Set_Forward_Target(Forward_Angle[Forward_State]);
								Set_Pitch_Target(Pitch_Angle[Pitch_State]);
							
								Auto_Position_Flag = 0;	
								Auto_Protect = 0;
						}
						
				}	
		}
}
		
void Manual_Push_Exchange(void)
{
		if(R[1] == Pressed && R[0] == Pressed)
		{
				Pitch_Incline = 134.0f - PitchStatus.RealAngle;
			
				Forward_State = Forward_State_Out;
				if(ForwardL_Motor.Targetcirnum * 360.0f + ForwardL_Motor.TargetAngle < 855.0f + ForwardL_Motor.InitAngle)
				{
						ForwardL_Motor.TargetAngle = ForwardL_Motor.Targetcirnum * 360.0f + ForwardL_Motor.TargetAngle + 1.5*cos(Pitch_Incline * 3.14f / 180.0f);
						ForwardR_Motor.TargetAngle = ForwardR_Motor.Targetcirnum * 360.0f + ForwardR_Motor.TargetAngle - 1.5*cos(Pitch_Incline * 3.14f / 180.0f);
						Angle_Limit(&ForwardL_Motor);
						Angle_Limit(&ForwardR_Motor);
				}
				Lift_State = Lift_State_Down;
				if(LiftL_Motor.Targetcirnum * 360.0f + LiftL_Motor.TargetAngle > 0.1f)
				{
						LiftL_Motor.TargetAngle = LiftL_Motor.Targetcirnum * 360.0f + LiftL_Motor.TargetAngle - 2.6*sin(Pitch_Incline * 3.14f / 180.0f);
						LiftR_Motor.TargetAngle = LiftR_Motor.Targetcirnum * 360.0f + LiftR_Motor.TargetAngle + 2.6*sin(Pitch_Incline * 3.14f / 180.0f);
						Angle_Limit(&LiftL_Motor);
						Angle_Limit(&LiftR_Motor);
				}

		}	
}
		
