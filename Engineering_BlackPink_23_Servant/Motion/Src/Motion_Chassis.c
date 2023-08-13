/**Include Header Files**/
#include "Motion_Chassis.h"

/**Tips**/
/*
ChassisFL:ChassisMotor[0]
ChassisFR:ChassisMotor[1]
ChassisBR:ChassisMotor[2]
ChassisBL:ChassisMotor[3]
*/

/**Variable Definition**/
Chassis_t Chassis;

Spin_State_t Spin_State;

int16_t Chassis_Follow_W = 0;
int16_t Chassis_Offset = 0;
uint8_t Auto_Chassis_Flag = 0;

/**Function Declaration**/
/**
 * @brief  Update the Speed of Chassis
 * @note
 * @retval None
 */
void UpdataChassisState(void)
{
    if (GetControlMode() == ControlMode_RC)
    {
        //ȷ�����̵Ĳ�ͬģʽ���Լ����е���Y�ᣨǰ��X�ᣨ���ң�Ŀ���ٶȼ���
        if (GetSubState() != SubState_Protect)
        {	
						if(Spin_State == Spin_State_ClockWise)
						{
								Chassis.RC_X = CH_LeftHori_Value * CHASSIS_SPEED_MAX * cos(Chassis_Offset * 3.14f / 180.0f) + CH_RightVert_Value * CHASSIS_SPEED_MAX * sin(Chassis_Offset * 3.14f / 180.0f);								
								Chassis.RC_Y = - CH_RightVert_Value * CHASSIS_SPEED_MAX * cos(Chassis_Offset * 3.14f / 180.0f) + CH_LeftHori_Value * CHASSIS_SPEED_MAX * sin(Chassis_Offset * 3.14f / 180.0f);
								Chassis.RC_W = 9000;
						}
						
						else if(Spin_State == Spin_State_Follow)
						{
								Chassis.RC_X = 0;
								Chassis.RC_Y = 0;
								Chassis.RC_W = Chassis_Follow_W;
						}
						else if(Spin_State == Spin_State_Static)
						{
								if(Gimbal_State == Gimbal_State_Front)
								{
										Chassis.RC_X = CH_LeftHori_Value * CHASSIS_SPEED_MAX;
										Chassis.RC_Y = -CH_RightVert_Value * CHASSIS_SPEED_MAX;
										Chassis.RC_W = CH_RightHori_Value * CHASSIS_SPEED_MAX;
								}
								else if(Gimbal_State == Gimbal_State_Back)
								{
										Chassis.RC_X = -CH_LeftHori_Value * CHASSIS_SPEED_MAX;
										Chassis.RC_Y = CH_RightVert_Value * CHASSIS_SPEED_MAX;
										Chassis.RC_W = CH_RightHori_Value * CHASSIS_SPEED_MAX;
								}
								else
								{
										Chassis.RC_X = 0;
										Chassis.RC_Y = 0;
										Chassis.RC_W = 0;
								
								}
						
						}
						else
						{
								Chassis.RC_X = 0;
								Chassis.RC_Y = 0;
								Chassis.RC_W = 0;
						}
        }
        else
        {
            Chassis.RC_X = 0;
            Chassis.RC_Y = 0;
            Chassis.RC_W = 0;
        }
    }

    /*====================����ģʽ====================*/
    if (GetControlMode() == ControlMode_PC)
    {
				//ȷ�����̵Ĳ�ͬģʽ���Լ����е���Y�ᣨǰ��X�ᣨ���ң�Ŀ���ٶȼ���
        if (GetSubState() != SubState_Protect)
        {
						if(Spin_State == Spin_State_ClockWise)
						{
								if(GetPCMode() == PCMode_Pick_Sky)
								{
										//����RC_X����
										if(Mouse_Right[0] == Pressed)
										{
												Chassis.RC_X = -0.26 * CHASSIS_SPEED_MAX * cos(Chassis_Offset * 3.14f / 180.0f) - Out_Y * 260 * sin(Chassis_Offset * 3.14f / 180.0f);//��
												Chassis.RC_Y = Out_Y * 260 * cos(Chassis_Offset * 3.14f / 180.0f) - 0.26 * CHASSIS_SPEED_MAX * sin(Chassis_Offset * 3.14f / 180.0f);
										}
										else if(D[0] == Pressed)
										{
												Chassis.RC_X = 0.26 * CHASSIS_SPEED_MAX * cos(Chassis_Offset * 3.14f / 180.0f) - Out_Y * 260 * sin(Chassis_Offset * 3.14f / 180.0f);//��
												Chassis.RC_Y = Out_Y * 260 * cos(Chassis_Offset * 3.14f / 180.0f) + 0.26 * CHASSIS_SPEED_MAX * sin(Chassis_Offset * 3.14f / 180.0f);
										}
										else if(Mouse_Left[0] == Pressed)
										{
												Chassis.RC_X = -0.06 * CHASSIS_SPEED_MAX * cos(Chassis_Offset * 3.14f / 180.0f) - Out_Y * 260 * sin(Chassis_Offset * 3.14f / 180.0f);//��
												Chassis.RC_Y = Out_Y * 260 * cos(Chassis_Offset * 3.14f / 180.0f) - 0.06 * CHASSIS_SPEED_MAX * sin(Chassis_Offset * 3.14f / 180.0f);
										}
										else if(A[0] == Pressed)
										{
												Chassis.RC_X = 0.06 * CHASSIS_SPEED_MAX * cos(Chassis_Offset * 3.14f / 180.0f) - Out_Y * 260 * sin(Chassis_Offset * 3.14f / 180.0f);//��
												Chassis.RC_Y = Out_Y * 260 * cos(Chassis_Offset * 3.14f / 180.0f) + 0.06 * CHASSIS_SPEED_MAX * sin(Chassis_Offset * 3.14f / 180.0f);
										}
										else
										{
												Chassis.RC_X = -Out_Y * 260 * sin(Chassis_Offset * 3.14f / 180.0f);//��
												Chassis.RC_Y = Out_Y * 260 * cos(Chassis_Offset * 3.14f / 180.0f);				
										}
										
										Chassis.RC_W = 9000;
								}
								else if(GetPCMode() == PCMode_Pick_Arm)
								{				
										//����RC_X����
										if(Mouse_Right[0] == Pressed)
										{
												Chassis.RC_X = 0.26 * CHASSIS_SPEED_MAX * cos(Chassis_Offset * 3.14f / 180.0f) + Out_Y * 260 * sin(Chassis_Offset * 3.14f / 180.0f);//��
												Chassis.RC_Y = - Out_Y * 260 * cos(Chassis_Offset * 3.14f / 180.0f) + 0.26 * CHASSIS_SPEED_MAX * sin(Chassis_Offset * 3.14f / 180.0f);
										}
										else if(D[0] == Pressed)
										{
												Chassis.RC_X = -0.26 * CHASSIS_SPEED_MAX * cos(Chassis_Offset * 3.14f / 180.0f) + Out_Y * 260 * sin(Chassis_Offset * 3.14f / 180.0f);//��
												Chassis.RC_Y = - Out_Y * 260 * cos(Chassis_Offset * 3.14f / 180.0f) - 0.26 * CHASSIS_SPEED_MAX * sin(Chassis_Offset * 3.14f / 180.0f);		
										}
										else if(Mouse_Left[0] == Pressed)
										{
												Chassis.RC_X = 0.06 * CHASSIS_SPEED_MAX * cos(Chassis_Offset * 3.14f / 180.0f) + Out_Y * 260 * sin(Chassis_Offset * 3.14f / 180.0f);//��
												Chassis.RC_Y = - Out_Y * 260 * cos(Chassis_Offset * 3.14f / 180.0f) + 0.06 * CHASSIS_SPEED_MAX * sin(Chassis_Offset * 3.14f / 180.0f);
										}
										else if(A[0] == Pressed)
										{
												Chassis.RC_X = -0.06 * CHASSIS_SPEED_MAX * cos(Chassis_Offset * 3.14f / 180.0f) + Out_Y * 260 * sin(Chassis_Offset * 3.14f / 180.0f);//��
												Chassis.RC_Y = - Out_Y * 260 * cos(Chassis_Offset * 3.14f / 180.0f) - 0.06 * CHASSIS_SPEED_MAX * sin(Chassis_Offset * 3.14f / 180.0f);
										}
										else
										{
												Chassis.RC_X = Out_Y * 260 * sin(Chassis_Offset * 3.14f / 180.0f);//��
												Chassis.RC_Y = - Out_Y * 260 * cos(Chassis_Offset * 3.14f / 180.0f);
										}
												
										Chassis.RC_W = 9000;
								}	
						}
						else if(Spin_State == Spin_State_Follow)
						{
								Chassis.RC_X = 0;
								Chassis.RC_Y = 0;
								Chassis.RC_W = Chassis_Follow_W;
						}
						else if(Spin_State == Spin_State_Static)
						{
								if(Auto_Chassis_Flag == 0)
								{
										if(GetPCMode() == PCMode_Exchange_Sky)
										{
												//����RC_X����
												if(Mouse_Right[0] == Pressed)
														Chassis.RC_X = -0.14 * CHASSIS_SPEED_MAX;//��
												else if(D[0] == Pressed)
														Chassis.RC_X = 0.14 * CHASSIS_SPEED_MAX;
												else if(Mouse_Left[0] == Pressed)
														Chassis.RC_X = -0.06 * CHASSIS_SPEED_MAX;
												else if(A[0] == Pressed)
														Chassis.RC_X = 0.06 * CHASSIS_SPEED_MAX;
												else
														Chassis.RC_X = 0;
									
												//����RC_Y����
												if(Out_Y > 10)
														Chassis.RC_Y = 0.06 * CHASSIS_SPEED_MAX;
												else if(Out_Y < -10)
														Chassis.RC_Y = -0.06 * CHASSIS_SPEED_MAX;
												else
														Chassis.RC_Y = 0;
										
												//����RC_W����								
												if(Out_X > 10)
														Chassis.RC_W = 0.06 * CHASSIS_SPEED_MAX;
												else if(Out_X < -10)
														Chassis.RC_W = -0.06 * CHASSIS_SPEED_MAX;
												else
														Chassis.RC_W = 0;							
										}
										else if(GetPCMode() == PCMode_Exchange_Arm)
										{
												if(R[0] == Pressed && Lift_State == Lift_State_Push)
												{
														//����RC_X����
														if(Mouse_Right[0] == Pressed)
																Chassis.RC_X = 0.14 * CHASSIS_SPEED_MAX + 700 * sin(Yaw_Incline * 3.14f / 180.0f);//��
														else if(D[0] == Pressed)
																Chassis.RC_X = -0.14 * CHASSIS_SPEED_MAX + 700 * sin(Yaw_Incline * 3.14f / 180.0f);
														else if(Mouse_Left[0] == Pressed)
																Chassis.RC_X = 0.06 * CHASSIS_SPEED_MAX + 700 * sin(Yaw_Incline * 3.14f / 180.0f);
														else if(A[0] == Pressed)
																Chassis.RC_X = -0.06 * CHASSIS_SPEED_MAX + 700 * sin(Yaw_Incline * 3.14f / 180.0f);
														else
																Chassis.RC_X = 700 * sin(Yaw_Incline * 3.14f / 180.0f);
											
														//����RC_Y����
														if(Out_Y > 10)
																Chassis.RC_Y = -0.06 * CHASSIS_SPEED_MAX;
														else if(Out_Y < -10)
																Chassis.RC_Y = 0.06 * CHASSIS_SPEED_MAX;
														else
																Chassis.RC_Y = 0;
												
														//����RC_W����								
														if(Out_X > 10)
																Chassis.RC_W = 0.06 * CHASSIS_SPEED_MAX;
														else if(Out_X < -10)
																Chassis.RC_W = -0.06 * CHASSIS_SPEED_MAX;
														else
																Chassis.RC_W = 0;		
												}
												else
												{
														//����RC_X����
														if(Mouse_Right[0] == Pressed)
																Chassis.RC_X = 0.14 * CHASSIS_SPEED_MAX;//��
														else if(D[0] == Pressed)
																Chassis.RC_X = -0.14 * CHASSIS_SPEED_MAX;
														else if(Mouse_Left[0] == Pressed)
																Chassis.RC_X = 0.06 * CHASSIS_SPEED_MAX;
														else if(A[0] == Pressed)
																Chassis.RC_X = -0.06 * CHASSIS_SPEED_MAX;
														else
																Chassis.RC_X = 0;
											
														//����RC_Y����
														if(Out_Y > 10)
																Chassis.RC_Y = -0.06 * CHASSIS_SPEED_MAX;
														else if(Out_Y < -10)
																Chassis.RC_Y = 0.06 * CHASSIS_SPEED_MAX;
														else
																Chassis.RC_Y = 0;
												
														//����RC_W����								
														if(Out_X > 10)
																Chassis.RC_W = 0.06 * CHASSIS_SPEED_MAX;
														else if(Out_X < -10)
																Chassis.RC_W = -0.06 * CHASSIS_SPEED_MAX;
														else
																Chassis.RC_W = 0;												
												}
										}
										else if(GetPCMode() == PCMode_Pick_Sky)
										{
												//����RC_X����
												if(Mouse_Right[0] == Pressed)
														Chassis.RC_X = -0.26 * CHASSIS_SPEED_MAX;//��
												else if(D[0] == Pressed)
														Chassis.RC_X = 0.26 * CHASSIS_SPEED_MAX;
												else if(Mouse_Left[0] == Pressed)
														Chassis.RC_X = -0.06 * CHASSIS_SPEED_MAX;
												else if(A[0] == Pressed)
														Chassis.RC_X = 0.06 * CHASSIS_SPEED_MAX;
												else
														Chassis.RC_X = 0;
							
												//����RC_Y����
												Chassis.RC_Y = Out_Y * 260;//325
						
												//����RC_W����
												Chassis.RC_W = Out_X * 70;//195
										}
										else if(GetPCMode() == PCMode_Pick_Arm)
										{
												//����RC_X����
												if(Mouse_Right[0] == Pressed)
														Chassis.RC_X = 0.26 * CHASSIS_SPEED_MAX;//��
												else if(D[0] == Pressed)
														Chassis.RC_X = -0.26 * CHASSIS_SPEED_MAX;
												else if(Mouse_Left[0] == Pressed)
														Chassis.RC_X = 0.06 * CHASSIS_SPEED_MAX;
												else if(A[0] == Pressed)
														Chassis.RC_X = -0.06 * CHASSIS_SPEED_MAX;
												else
														Chassis.RC_X = 0;
							
												//����RC_Y����
												Chassis.RC_Y = -Out_Y * 260;//325
						
												//����RC_W����
												Chassis.RC_W = Out_X * 70;//195
										}
								}
								else if(Auto_Chassis_Flag == 1)
								{
										if(GetPCMode() == PCMode_Exchange_Sky || GetPCMode() == PCMode_Pick_Sky)
										{
												//����RC_X����
												if(Mouse_Right[0] == Pressed)
														Chassis.RC_X = -0.22 * CHASSIS_SPEED_MAX;//��
												else if(D[0] == Pressed)
														Chassis.RC_X = 0.22 * CHASSIS_SPEED_MAX;
												else if(Mouse_Left[0] == Pressed)
														Chassis.RC_X = -0.06 * CHASSIS_SPEED_MAX;
												else if(A[0] == Pressed)
														Chassis.RC_X = 0.06 * CHASSIS_SPEED_MAX;
												else
														Chassis.RC_X = 0;	
												
												//����RC_Y����								
												if(Out_Y > 10)
														Chassis.RC_Y = -0.022 * CHASSIS_SPEED_MAX + 0.06 * CHASSIS_SPEED_MAX;
												else if(Out_Y < -10)
														Chassis.RC_Y = -0.022 * CHASSIS_SPEED_MAX - 0.06 * CHASSIS_SPEED_MAX;
												else
														Chassis.RC_Y = -0.022 * CHASSIS_SPEED_MAX ;
												
												//����RC_W����								
												if(Out_X > 10)
														Chassis.RC_W = 0.06 * CHASSIS_SPEED_MAX;
												else if(Out_X < -10)
														Chassis.RC_W = -0.06 * CHASSIS_SPEED_MAX;
												else
														Chassis.RC_W = 0;		
										}
										else if(GetPCMode() == PCMode_Exchange_Arm || GetPCMode() == PCMode_Pick_Arm)
										{
												//����RC_X����
												if(Mouse_Right[0] == Pressed)
														Chassis.RC_X = 0.22 * CHASSIS_SPEED_MAX;//��
												else if(D[0] == Pressed)
														Chassis.RC_X = -0.22 * CHASSIS_SPEED_MAX;
												else if(Mouse_Left[0] == Pressed)
														Chassis.RC_X = 0.06 * CHASSIS_SPEED_MAX;
												else if(A[0] == Pressed)
														Chassis.RC_X = -0.06 * CHASSIS_SPEED_MAX;
												else
														Chassis.RC_X = 0;	
												
												//����RC_Y����								
												if(Out_Y > 10)
														Chassis.RC_Y = 0.022 * CHASSIS_SPEED_MAX - 0.06 * CHASSIS_SPEED_MAX;
												else if(Out_Y < -10)
														Chassis.RC_Y = 0.022 * CHASSIS_SPEED_MAX + 0.06 * CHASSIS_SPEED_MAX;
												else
														Chassis.RC_Y = 0.022 * CHASSIS_SPEED_MAX ;
												
												//����RC_W����								
												if(Out_X > 10)
														Chassis.RC_W = 0.06 * CHASSIS_SPEED_MAX;
												else if(Out_X < -10)
														Chassis.RC_W = -0.06 * CHASSIS_SPEED_MAX;
												else
														Chassis.RC_W = 0;		
										}
								}
								else if(Auto_Chassis_Flag == 2)
								{
										if(GetPCMode() == PCMode_Pick_Arm)
										{
												//����RC_X����
												if(Mouse_Right[0] == Pressed)
														Chassis.RC_X = 0.22 * CHASSIS_SPEED_MAX;//��
												else if(D[0] == Pressed)
														Chassis.RC_X = -0.22 * CHASSIS_SPEED_MAX;
												else if(Mouse_Left[0] == Pressed)
														Chassis.RC_X = 0.06 * CHASSIS_SPEED_MAX;
												else if(A[0] == Pressed)
														Chassis.RC_X = -0.06 * CHASSIS_SPEED_MAX;
												else
														Chassis.RC_X = 0;	
												
												//����RC_Y����								
												if(Out_Y > 10)
														Chassis.RC_Y = -0.12 * CHASSIS_SPEED_MAX - 0.06 * CHASSIS_SPEED_MAX;
												else if(Out_Y < -10)
														Chassis.RC_Y = -0.12 * CHASSIS_SPEED_MAX + 0.06 * CHASSIS_SPEED_MAX;
												else
														Chassis.RC_Y = -0.12 * CHASSIS_SPEED_MAX ;
												
												//����RC_W����								
												if(Out_X > 10)
														Chassis.RC_W = 0.06 * CHASSIS_SPEED_MAX;
												else if(Out_X < -10)
														Chassis.RC_W = -0.06 * CHASSIS_SPEED_MAX;
												else
														Chassis.RC_W = 0;		
										}
										else
										{
												Chassis.RC_X = 0;
												Chassis.RC_Y = 0;
												Chassis.RC_W = 0;	
										}
								}
								else if(Auto_Chassis_Flag == 3)
								{
										if(GetPCMode() == PCMode_Pick_Sky)
										{
												//����RC_X����
												if(Mouse_Right[0] == Pressed)
														Chassis.RC_X = 0.22 * CHASSIS_SPEED_MAX;//��
												else if(D[0] == Pressed)
														Chassis.RC_X = -0.22 * CHASSIS_SPEED_MAX;
												else if(Mouse_Left[0] == Pressed)
														Chassis.RC_X = 0.06 * CHASSIS_SPEED_MAX;
												else if(A[0] == Pressed)
														Chassis.RC_X = -0.06 * CHASSIS_SPEED_MAX;
												else
														Chassis.RC_X = 0;	
												
												//����RC_Y����								
												if(Out_Y > 10)
														Chassis.RC_Y = -0.06 * CHASSIS_SPEED_MAX;
												else if(Out_Y < -10)
														Chassis.RC_Y = 0.06 * CHASSIS_SPEED_MAX;
												else
														Chassis.RC_Y = 0;
												
												//����RC_W����								
												if(Out_X > 10)
														Chassis.RC_W = 0.28 * CHASSIS_SPEED_MAX + 0.06 * CHASSIS_SPEED_MAX;
												else if(Out_X < -10)
														Chassis.RC_W = 0.28 * CHASSIS_SPEED_MAX - 0.06 * CHASSIS_SPEED_MAX;
												else
														Chassis.RC_W = 0.28 * CHASSIS_SPEED_MAX;		
										}
										else
										{
												Chassis.RC_X = 0;
												Chassis.RC_Y = 0;
												Chassis.RC_W = 0;	
										}
								}
								else
								{
										Chassis.RC_X = 0;
										Chassis.RC_Y = 0;
										Chassis.RC_W = 0;
								}	
						}
						else
						{
								Chassis.RC_X = 0;
								Chassis.RC_Y = 0;
								Chassis.RC_W = 0;
						}
				}
		}

		
		//�����ٶȸ�ֵ
    Chassis.Speed_W = Chassis.RC_W;
    Chassis.Speed_X = Chassis.RC_X;
    Chassis.Speed_Y = -Chassis.RC_Y;

		
    //���ݵ���Ŀ���ٶȺ������ķ����ѧ���ʣ�����������ӣ������ת��������ٶȡ�
    ChassisMotor[0].Targetrotationrate = (Chassis.Speed_X + Chassis.Speed_Y + Chassis.Speed_W);	//ǰ��
    ChassisMotor[1].Targetrotationrate = (Chassis.Speed_X - Chassis.Speed_Y + Chassis.Speed_W);	//ǰ��
    ChassisMotor[2].Targetrotationrate = (-Chassis.Speed_X - Chassis.Speed_Y + Chassis.Speed_W);	//����
    ChassisMotor[3].Targetrotationrate = (-Chassis.Speed_X + Chassis.Speed_Y + Chassis.Speed_W);	//����
		
}


