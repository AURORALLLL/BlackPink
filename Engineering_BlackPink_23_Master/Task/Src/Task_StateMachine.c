/**Include Header Files**/
#include "Task_StateMachine.h"


/**Macro Definition**/
#define TASK_STATEMACHINE_INTERVAL (5)


/**Tips!!!**/
/*
	����֤����Ҫʹ��ͨ������ͨ�Ż�ȡ�����ݽ��в����������������������ͨ��д��һ����������ʹ�ò���ʱ���������³�������⡣
	������Ҫʹ��ң������������Ƶ�������ǵ�����ƴ��벢�Ǻ�ң��ͨ��д��һ�𣬾ͻ����ң�����������ǵ��û�з�Ӧ���ǵ�
����η�Ӧ�����⡣��Ϊƽ�����ǲ������˵��Ŀ��Ƕȸ�ֵ�������θ�ֵ��Ŀ��Ƕ���Ȼ��������Ԥ���ֵ������������ⲻ
�Ǻ����ԣ��Լ��ǳ�ż���ĸ�ֵʧ�ܣ������������һЩ����ϣ�����ִֻ��һ�εĲ�������ô�ͻ������⡣
	���ǵ�Task_StateMachine��ÿ���յ�һ��ң�������ݲŽ���һ�Σ����ԣ������и���ң�����ĵ��������д��Task_StateMachine
�У���ԭ����Task_Suck Task_Mineral����������������庯����������Task_Init�д������񣬼���Ϊ�򵥵�BSP�ļ���
*/


/**Variable Definition**/
//State
MainState_t MainState;
SubState_t SubState;
ControlMode_t ControlMode;
PCMode_t PCMode;
//Key_Value:Remote Control
Roller_State_t Roller_State[3] = {Roller_Mid, Roller_Mid, Roller_Mid};//���ַ���
Stick_Vert_t StickL_Vert[3] = {Stick_Mid_V, Stick_Mid_V, Stick_Mid_V};//��ҡ����ֱ����
Stick_Hori_t StickL_Hori[3] = {Stick_Mid_H, Stick_Mid_H, Stick_Mid_H};//��ҡ��ˮƽ����
Stick_Vert_t StickR_Vert[3] = {Stick_Mid_V, Stick_Mid_V, Stick_Mid_V};//��ҡ����ֱ����
Stick_Hori_t StickR_Hori[3] = {Stick_Mid_H, Stick_Mid_H, Stick_Mid_H};//��ҡ��ˮƽ����
//Key_Value:Keyboard
Key_Value_t W[3] = {UnPressed, UnPressed, UnPressed};
Key_Value_t S[3] = {UnPressed, UnPressed, UnPressed};
Key_Value_t D[3] = {UnPressed, UnPressed, UnPressed};
Key_Value_t A[3] = {UnPressed, UnPressed, UnPressed};
Key_Value_t Q[3] = {UnPressed, UnPressed, UnPressed};
Key_Value_t E[3] = {UnPressed, UnPressed, UnPressed};
Key_Value_t R[3] = {UnPressed, UnPressed, UnPressed};
Key_Value_t F[3] = {UnPressed, UnPressed, UnPressed};
Key_Value_t G[3] = {UnPressed, UnPressed, UnPressed};
Key_Value_t Z[3] = {UnPressed, UnPressed, UnPressed};
Key_Value_t X[3] = {UnPressed, UnPressed, UnPressed};
Key_Value_t C[3] = {UnPressed, UnPressed, UnPressed};
Key_Value_t V[3] = {UnPressed, UnPressed, UnPressed};
Key_Value_t B[3] = {UnPressed, UnPressed, UnPressed};
Key_Value_t CTRL[3] = {UnPressed, UnPressed, UnPressed};
Key_Value_t SHIFT[3] = {UnPressed, UnPressed, UnPressed};
Key_Value_t Mouse_Left[3] = {UnPressed, UnPressed, UnPressed};
Key_Value_t Mouse_Right[3] = {UnPressed, UnPressed, UnPressed};
//����ٶ�
float Out_X = 0;
float Out_Y = 0;
float XX = 0;
float YY = 0;
float Mouse_Speed_X[9] = {0};
float Mouse_Speed_Y[9] = {0};

//��ת���
SubState_t SubState_Last;//���ڶ�ת�����ж�
uint8_t SubState_flag = 0;//���ڼ�¼�ӱ���״̬�г��Ĵ���
uint8_t Protect_flag = 0;

uint8_t Change_Flag = 0;

/**Function Declaration**/
/**
  * @brief  Task StateMachine
  * @param  unused
  * @retval void
  * @note   Update StateMachine, Update Action Queue
  */
int time_state = 0;
TickType_t start_time1;
TickType_t state;
void Task_StateMachine(void *parameters)
{
		//Initialize StateMachine Enum
		StateMachine_Init();
		
		//Initialize Motor
		PlatformMotor_Init();
		SkyMotor_Init();
		ArmMotor_Init();
		Spin_Init();
	
		//���ö��庯��
		Arm_Sucker_State = Sucker_Off;
		Sky_Sucker_State = Sucker_Off;
	
		Motor_Define = 1;
		

    while (1)
    {
				//start_time1 = xTaskGetTickCount();
        //����ң���������ݽ��㱾�ر���
        StateMachine_Update();
				Sucker_StateChange();
			
				if(GetSubState() == SubState_Protect && Protect_flag == 1)
				{
						Auto_Protect = 0;
						Auto_Sky_Flag = 0;
						Auto_Hold_Flag = 0;
						Auto_Back_Flag = 0;
						Auto_Store_Flag = 0;
						Auto_Take_Flag = 0;
						Auto_Silver_Flag = 0;
						Auto_Rotate_Flag = 0;
						Auto_Chassis_Flag = 0;
						Auto_Position_Flag = 0;
						Auto_Follow_Flag = 0;
					
						Auto_Protect = 0;
						LiftCounter = 0;
						WaitCounter = 0;
					
						if(PCMode == PCMode_Pick_Sky ||PCMode == PCMode_Exchange_Sky)
						{
								PCState = PCState_Pick_Init;
								Lift_State = Lift_State_Init;
						}

						if(PCMode == PCMode_Pick_Arm || PCMode == PCMode_Exchange_Arm)
						{
								PCState = PCState_Pick_Arm_Init;
								Lift_State = Lift_State_Arm_Init;
						}
						
						PlatformL_Motor.TargetAngle = PlatformL_Encoder.RealAngle;
						PlatformL_Motor.Targetcirnum = PlatformL_Encoder.Realcirnum;
						
						PlatformR_Motor.TargetAngle = PlatformR_Encoder.RealAngle;					
						PlatformR_Motor.Targetcirnum = PlatformR_Encoder.Realcirnum;

						SkyForwardL_Motor.TargetAngle = SkyForwardL_Motor.RealAngle;
						SkyForwardL_Motor.Targetcirnum = SkyForwardL_Motor.Realcirnum;
		
						SkyForwardR_Motor.TargetAngle = SkyForwardR_Motor.RealAngle;
						SkyForwardR_Motor.Targetcirnum = SkyForwardR_Motor.Realcirnum;					
						
						SkyPitchL_Motor.TargetAngle = SkyPitchL_Motor.RealAngle;
						SkyPitchL_Motor.Targetcirnum = SkyPitchL_Motor.Realcirnum;
		
						SkyPitchR_Motor.TargetAngle = SkyPitchR_Motor.RealAngle;
						SkyPitchR_Motor.Targetcirnum = SkyPitchR_Motor.Realcirnum;			
						
						ArmYaw_Motor.TargetAngle = ArmYaw_Motor.RealAngle;
						ArmYaw_Motor.Targetcirnum = ArmYaw_Motor.Realcirnum;
						
						ArmPitchL_Motor.TargetAngle = ArmPitchL_Motor.RealAngle;
						ArmPitchL_Motor.Targetcirnum = ArmPitchL_Motor.Realcirnum;
						
						ArmPitchR_Motor.TargetAngle = ArmPitchR_Motor.RealAngle;
						ArmPitchR_Motor.Targetcirnum = ArmPitchR_Motor.Realcirnum;
				}
				else
				{
						if(Auto_Block_Flag == 1)
						{
								Motor_Calibrate();
						}
						else
						{
								//������ؿ���
								Spin_Chassis();
								Spin_Gimbal();
							

								if(ControlMode == ControlMode_RC)
								{
										//ArmPitch_State = ArmPitch_State_Protect;
										//Set_ArmPitch_Target(ArmPitch_Angle[ArmPitch_State]);
										//Manual_Test();
										Manual_Control();
								}
								
								if(ControlMode == ControlMode_PC)
								{
										Platform_Motor_AngleInit();
										ArmYaw_Motor_AngleInit();
									
										FreeWheelBlock();
									
										//�ֶ�ģʽ
										if(Auto_Protect == 0)
										{
												//Update PCMode
												PCMode_Update();
												if(PCMode == PCMode_Pick_Sky)
												{
														Manual_Suck_Sky();
													
														Manual_Lift_Pick_Sky();
														Manual_Out_Pick_Sky();
														Manual_Press_Pick_Sky();
														Manual_Init_Pick_Sky();
												}
												if(PCMode == PCMode_Pick_Arm)
												{
														Manual_Suck_Arm();
													
														Manual_Lift_Pick_Arm();
														Manual_Out_Pick_Arm();
														Manual_Press_Pick_Arm();
														Manual_Init_Pick_Arm();
												}
												if(PCMode == PCMode_Exchange_Sky)
												{
														Manual_Lift_Exchange();
													
														Manual_Sucker_Exchange_Sky();
														Manual_Forward_Exchange_Sky();
												}
												if(PCMode == PCMode_Exchange_Arm)
												{
														Manual_Lift_Exchange();
													
														Manual_Sucker_Exchange_Arm();
														Manual_Forward_Exchange_Arm();
														Manual_Pitch_Exchange_Arm();
														Manual_Roll_Exchange_Arm();
														Manual_Yaw_Exchange_Arm();
													
														Manual_Push_Exchange_Arm();
												}
										}
										
										//�Զ�ģʽ
										if(PCMode == PCMode_Pick_Sky)
										{
												if(Auto_Hold_Flag == 0 && Auto_Position_Flag == 0 && Auto_Rotate_Flag == 0 && Auto_Follow_Flag == 0)
												{		
														Auto_Sky_Sky();
												}
												if(Auto_Sky_Flag == 0 && Auto_Position_Flag == 0 && Auto_Rotate_Flag == 0 && Auto_Follow_Flag == 0)
												{
														Auto_Hold_Sky();
												}	
												if(Auto_Hold_Flag == 0 && Auto_Position_Flag == 0 && Auto_Sky_Flag == 0 && Auto_Follow_Flag == 0)
												{
														Auto_Rotate_Sky();
												}
												if(Auto_Hold_Flag == 0 && Auto_Position_Flag == 0 && Auto_Sky_Flag == 0 && Auto_Rotate_Flag == 0)
												{
														Manual_Spin();
												}
												
												
												if(Auto_Position_Flag == 1)
												{
														Exchange_to_Pick_Sky();
												}
										}
										if(PCMode == PCMode_Pick_Arm)
										{
												if(Auto_Silver_Flag == 0 && Auto_Position_Flag == 0 && Auto_Take_Flag == 0 && Auto_Follow_Flag == 0)
												{
														Auto_HoldBackStore_Arm();
												}											
												if(Auto_Hold_Flag == 0 && Auto_Back_Flag == 0 && Auto_Store_Flag == 0 && Auto_Position_Flag == 0 && Auto_Take_Flag == 0 && Auto_Follow_Flag == 0)
												{
														Manual_Suck_Arm();
													
														Auto_Pick_Silver_Arm();
												}		
												if(Auto_Hold_Flag == 0 && Auto_Back_Flag == 0 && Auto_Store_Flag == 0 && Auto_Position_Flag == 0 && Auto_Silver_Flag == 0 && Auto_Follow_Flag == 0)
												{
														Auto_Take_Arm();
												}		
												if(Auto_Hold_Flag == 0 && Auto_Back_Flag == 0 && Auto_Store_Flag == 0 && Auto_Position_Flag == 0 && Auto_Silver_Flag == 0 && Auto_Take_Flag == 0)
												{
														Manual_Spin();
												}
												
												
												
												if(Auto_Position_Flag == 1)
												{
														Exchange_to_Pick_Arm();
												}
										}
										if(PCMode == PCMode_Exchange_Sky)
										{
												if(Auto_Position_Flag == 1)
												{
														Pick_to_Exchange_Sky();
												}
										}
										if(PCMode == PCMode_Exchange_Arm)
										{
												if(Auto_Position_Flag == 1)
												{
														Pick_to_Exchange_Arm();
												}
										}
								}
						}
				}
				
				//state = xTaskGetTickCount() - start_time1;
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

    }
}

/**
 * @brief  StateMachine Init
 * @note   
 * @param  None
 * @retval Initialize MainState, SubState, ControlMode, PCMode
 */
void StateMachine_Init(void)
{
    MainState = MainState_Debug;
    SubState = SubState_Protect;
	
		ControlMode = ControlMode_RC;
    PCMode = PCMode_Pick_Sky;
}

void StatePara_Init(void)
{
//		Sucker_State = Sucker_Off;
}

/**
 * @brief  StateMachine Update
 * @note
 * @param  None
 * @retval ע�⺯��֮�����˳�򣡣���ȷ��ʵʱ��Ӧ
 */
void StateMachine_Update(void)
{
    //Update State
    MainState_Update();
    SubState_Update();
    ControlMode_Update();

		//Update Key_Value
    if(ControlMode == ControlMode_RC)
    {
        Switch_Update();
    }
    if(ControlMode == ControlMode_PC)
    {
        Key_Update();
				Mouse_Speed_Filter();
			  //Roller_PC_Update();
				Switch_Update();
				//XX = KalmanFilter(Out_X);
				//YY = KalmanFilter(Out_Y);
    }
		
}

/**
 * @brief  Update MainState
 * @note   According to Remote Control
 * @param  None
 * @retval 
 */
void MainState_Update(void)
{
    //ң������״̬����
    switch (Get_Switch_Value(Switch_Left))
    {
    case sw_up:
        MainState = MainState_Debug;
        CleanSwTrigger(Switch_Left);
        CleanSwTrigger(Switch_Right);
        break;

    case sw_mid:
        MainState = MainState_Vision;
        CleanSwTrigger(Switch_Left);
        CleanSwTrigger(Switch_Right);
        break;

    case sw_down:
        MainState = MainState_Extend;
        CleanSwTrigger(Switch_Left);
        CleanSwTrigger(Switch_Right);
        break;

    default:
        MainState = MainState_Debug;
        break;
    }
}

/**
 * @brief  Update SubState
 * @note   According to Remote Control and MainState
 * @param  None
 * @retval 
 */
void SubState_Update(void)
{
   SubState_Last = SubState;
		
    switch (MainState)
    {
    case MainState_Debug:
        if (Get_Switch_Value(Switch_Right) == sw_up)//˫����λ����
            SubState = SubState_Protect;
        else if (Get_Switch_Value(Switch_Right) == sw_mid)
            SubState = SubState_Test;
        else if (Get_Switch_Value(Switch_Right) == sw_down)//��������Ϊ�������
            SubState = SubState_PC;
        break;

    case MainState_Vision:
        if (Get_Switch_Value(Switch_Right) == sw_up)
            SubState = SubState_Hand;
        else if (Get_Switch_Value(Switch_Right) == sw_mid)
            SubState = SubState_Exchange;
        else if (Get_Switch_Value(Switch_Right) == sw_down)
            SubState = SubState_Rotate;
        break;

    case MainState_Extend:
        if (Get_Switch_Value(Switch_Right) == sw_up)
            SubState = SubState_Move;
        else if (Get_Switch_Value(Switch_Right) == sw_mid)
            SubState = SubState_Gimbal;
        else if (Get_Switch_Value(Switch_Right) == sw_down)
            SubState = SubState_Mineral;
        break;

    default:
        break;
    }
		
		//�Դ�״̬�仯������м�¼
		if(SubState_Last == SubState_Protect && SubState != SubState_Protect)
		{
				SubState_flag++;
				if(SubState_flag == 1)
				{
						Auto_Block_Flag = 1;
				}
		}
		if(SubState_Last != SubState_Protect && SubState == SubState_Protect)
		{
				Protect_flag = 1;
		}
		if(SubState_Last != SubState_PC && SubState == SubState_PC)
		{	
				if(GetPCMode() == PCMode_Exchange_Sky)
				{
						Gimbal_State = 	Gimbal_State_Front;
						Set_Gimbal_Target(Gimbal_Angle[Gimbal_State]);
				}
				if(GetPCMode() == PCMode_Exchange_Arm)
				{
						Gimbal_State = 	Gimbal_State_Back;
						Set_Gimbal_Target(Gimbal_Angle[Gimbal_State]);									
				}		
		}
		
		
		if(SubState == SubState_Protect)
		{
				Auto_Block_Flag = 0;
				Lift_Motor_BlockFlag = 0;
		}
		if(SubState != SubState_Protect)
		{
				Protect_flag = 0;
		}
}

/**
 * @brief  Update ControlMode
 * @note   According to SubState
 * @param  PC or RC
 * @retval 
 */
void ControlMode_Update(void)
{
    if (SubState == SubState_PC)
    {
        ControlMode = ControlMode_PC;
    }
    else
    {
        ControlMode = ControlMode_RC;
    }
}

/**
 * @brief  Update PCMode
 * @note   
 * @param  
 * @retval 
 */
void PCMode_Update(void)
{
		if(C[1] == UnPressed && C[0] == UnPressed)
		{
				if(F[1] == Pressed && F[0] == UnPressed)
				{
						if(PCMode == PCMode_Pick_Sky)
						{
								PCMode = PCMode_Exchange_Sky;
							
								Gimbal_State = 	Gimbal_State_Front;
								Set_Gimbal_Target(Gimbal_Angle[Gimbal_State]);
							
								Auto_Position_Flag = 1;	
								Auto_Protect = 1;
							
								Platform_State = Platform_State_Init;
								Set_Platform_Target(Platform_Angle[Platform_State]);
						}
						else if(PCMode == PCMode_Exchange_Sky)
						{
								PCMode = PCMode_Pick_Sky;
							
								Gimbal_State = 	Gimbal_State_Front;
								Set_Gimbal_Target(Gimbal_Angle[Gimbal_State]);
							
								Auto_Position_Flag = 1;	
								Auto_Protect = 1;
							
								Platform_State = Platform_State_Init;
								Set_Platform_Target(Platform_Angle[Platform_State]);
						}
						else if(PCMode == PCMode_Pick_Arm)
						{
								PCMode = PCMode_Exchange_Arm;
							
								Gimbal_State = 	Gimbal_State_Back;
								Set_Gimbal_Target(Gimbal_Angle[Gimbal_State]);
							
								Auto_Position_Flag = 1;	
								Auto_Protect = 1;
							
								Platform_State = Platform_State_Init;
								Set_Platform_Target(Platform_Angle[Platform_State]);
						}
						else if(PCMode == PCMode_Exchange_Arm)
						{
								PCMode = PCMode_Pick_Arm;
								Change_Flag = 2;
							
								Gimbal_State = 	Gimbal_State_Back;	
								Set_Gimbal_Target(Gimbal_Angle[Gimbal_State]);
							
								Auto_Position_Flag = 1;	
								Auto_Protect = 1;
							
								Platform_State = Platform_State_Init;
								Set_Platform_Target(Platform_Angle[Platform_State]);

						}
				}
		}
		
		
		if(PCMode == PCMode_Pick_Sky || PCMode == PCMode_Exchange_Sky)
		{
				if(C[1] == Pressed && C[0] == UnPressed)
				{
						PCMode = PCMode_Pick_Arm;
						Change_Flag = 1;
					
						Gimbal_State = 	Gimbal_State_Back;
						Set_Gimbal_Target(Gimbal_Angle[Gimbal_State]);
					
						Auto_Position_Flag = 1;	
						Auto_Protect = 1;
					
						Platform_State = Platform_State_Init;
						Set_Platform_Target(Platform_Angle[Platform_State]);
				}
		}
		else if(PCMode == PCMode_Pick_Arm)
		{
				if(C[1] == Pressed && C[0] == UnPressed)
				{
						PCMode = PCMode_Pick_Sky;
					
						Gimbal_State = 	Gimbal_State_Front;
						Set_Gimbal_Target(Gimbal_Angle[Gimbal_State]);
					
						Auto_Position_Flag = 1;	
						Auto_Protect = 1;
					
						Platform_State = Platform_State_Init;
						Set_Platform_Target(Platform_Angle[Platform_State]);
				}
		}
		else if(PCMode == PCMode_Exchange_Arm)
		{
				if(C[1] == Pressed && C[0] == Pressed)
				{
						if(F[1] == Pressed && F[0] == UnPressed)
						{					
								PCMode = PCMode_Pick_Sky;
							
								Gimbal_State = 	Gimbal_State_Front;
								Set_Gimbal_Target(Gimbal_Angle[Gimbal_State]);
							
								Auto_Position_Flag = 1;	
								Auto_Protect = 1;
							
								Platform_State = Platform_State_Init;
								Set_Platform_Target(Platform_Angle[Platform_State]);
						}	
				}
		}
}

/**
 * @brief  ����UI���� 
 * @note	 ʹ�����鴫��
 * @param
 * @retval According to Keyboard
 */
void LEDUI_Update(void)
{
//							for(int i = 0;i<=6;i++)
//							{
//									RGB_Value[i].Green = 0; 
//									RGB_Value[i].Red = 0x01;
//									RGB_Value[i].Blue = 0;
//							}
	
	
  if(Auto_Block_Flag ==1)
		Following_Lamp(RGBVALUE,RGBVALUE,0);
	else if(Auto_Protect == 1)
		Following_Lamp(0,RGBVALUE,0);

	else
	{
			//̧��״̬��ʾ��LED1
//			if(LiftL_G == GG)
//				RGB_Bling(5,RGBVALUE,0,0);
//			if(LiftR_G == GG)
//				RGB_Bling(1,RGBVALUE,0,0);
//			if(LiftR_G != GG&&LiftR_G != GG)
//			{
					if(Lift_State == Lift_State_Gold)
					{
							for(int i = 0;i<=6;i++)
							{
									RGB_Value[i].Green = RGBVALUE; 
									RGB_Value[i].Red = RGBVALUE+4;
									RGB_Value[i].Blue = 0;
							}
					}
					else if(Lift_State == Lift_State_Silver || Lift_State == Lift_State_Arm_Silver)
					{
							for(int i = 0;i<=6;i++)
							{
									RGB_Value[i].Green = RGBVALUE;
									RGB_Value[i].Red = RGBVALUE;
									RGB_Value[i].Blue = RGBVALUE;
							}
					}
					else if(Lift_State == Lift_State_Arm_Ground)
					{
							for(int i = 0;i<=6;i++)
							{
									RGB_Value[i].Green = 0; 
									RGB_Value[i].Red = RGBVALUE;
									RGB_Value[i].Blue = RGBVALUE;
							}
					
					}
					else if(Lift_State == Lift_State_Hero)
					{
							for(int i = 0;i<=6;i++)
							{
									RGB_Value[i].Green = 0;
									RGB_Value[i].Red = 0;
									RGB_Value[i].Blue = RGBVALUE;
							}
					}
					else 
					{
							for(int i = 0;i<=6;i++)
							{
									RGB_Value[i].Green = 0; 
									RGB_Value[i].Red = 0;
									RGB_Value[i].Blue = 0;
							}
					}
	  }
		
//		//ƽ̨����쳣��ʾ��LED2
//		if(ForwardL_G==GG)
//			RGB_Bling(4,RGBVALUE,0,0);
//		if(ForwardR_G==GG)
//			RGB_Bling(2,RGBVALUE,0,0);
//		if(Rotate_G==GG||Pitch_G == GG)
//			RGB_Bling(3,RGBVALUE,0,0);
//		
//		if(Chassis4_G==GG)
//			RGB_Bling(0,RGBVALUE+1,RGBVALUE,0);//��Ϊǰ�� ��ɫ����
//		else if(Chassis1_G==GG)
//			RGB_Bling(0,RGBVALUE+1,0,0);
//		if(Chassis2_G==GG)
//			RGB_Bling(6,RGBVALUE+1,0,0);
//		else if(Chassis3_G==GG)
//			RGB_Bling(6,RGBVALUE+1,RGBVALUE,0);
	
	//}
	ws2812_Set();
}

/**
 * @brief  Update Key Value 
 * @note	 ʹ�����鴫�ݣ���ֹ��ʵ��ʹ�ù����в����ֶ���ظ����������ظ�Ч��
 * @param
 * @retval According to Keyboard
 */
void Key_Update(void)
{
    //ÿ�θ���[0]�����ݣ��������δ洢
    W[2] = W[1];
    W[1] = W[0];
    S[2] = S[1];
    S[1] = S[0];
    D[2] = D[1];
    D[1] = D[0];
    A[2] = A[1];
    A[1] = A[0];
    Q[2] = Q[1];
    Q[1] = Q[0];
    E[2] = E[1];
    E[1] = E[0];
    R[2] = R[1];
    R[1] = R[0];
    F[2] = F[1];
    F[1] = F[0];
    G[2] = G[1];
    G[1] = G[0];
    Z[2] = Z[1];
    Z[1] = Z[0];
    X[2] = X[1];
    X[1] = X[0];
    C[2] = C[1];
    C[1] = C[0];
    V[2] = V[1];
    V[1] = V[0];
    B[2] = B[1];
    B[1] = B[0];
		CTRL[2] = CTRL[1];
    CTRL[1] = CTRL[0];
    SHIFT[2] = SHIFT[1];
    SHIFT[1] = SHIFT[0];
		Mouse_Left[2] = Mouse_Left[1];
		Mouse_Left[1] = Mouse_Left[0];
		Mouse_Right[2] = Mouse_Right[1];
		Mouse_Right[1] = Mouse_Right[0];
		
    

    //��ȡ�����������
    if (GetControlMode() == ControlMode_PC) //PC����ģʽ
    {
        W[0] = (Key_Value_t)(Get_KeyBoard_Value(KEY_W));
        S[0] = (Key_Value_t)(Get_KeyBoard_Value(KEY_S));
        D[0] = (Key_Value_t)(Get_KeyBoard_Value(KEY_D));
        A[0] = (Key_Value_t)(Get_KeyBoard_Value(KEY_A));
        Q[0] = (Key_Value_t)(Get_KeyBoard_Value(KEY_Q));
        E[0] = (Key_Value_t)(Get_KeyBoard_Value(KEY_E));
        R[0] = (Key_Value_t)(Get_KeyBoard_Value(KEY_R));
        F[0] = (Key_Value_t)(Get_KeyBoard_Value(KEY_F));
        G[0] = (Key_Value_t)(Get_KeyBoard_Value(KEY_G));
        Z[0] = (Key_Value_t)(Get_KeyBoard_Value(KEY_Z));
        X[0] = (Key_Value_t)(Get_KeyBoard_Value(KEY_X));
        C[0] = (Key_Value_t)(Get_KeyBoard_Value(KEY_C));
        V[0] = (Key_Value_t)(Get_KeyBoard_Value(KEY_V));
        B[0] = (Key_Value_t)(Get_KeyBoard_Value(KEY_B));
        CTRL[0] =(Key_Value_t)(Get_KeyBoard_Value(KEY_CTRL));
        SHIFT[0] = (Key_Value_t)(Get_KeyBoard_Value(KEY_SHIFT));
        Mouse_Left[0] = (Key_Value_t)(Get_Mouse_State(mouse_left));
        Mouse_Right[0] = (Key_Value_t)(Get_Mouse_State(mouse_right));
    }
}

/**
 * @brief  Update Switch Value 
 * @note   ���ַ���-1~1��float���������ת����ǰ-��-���״̬�������˼�������ǰ-��-��
 * @param  None
 * @retval According to Remote Control
 */
void Switch_Update(void)
{
    //ÿ�θ���[0]�����ݣ��������δ洢
    Roller_State[2] = Roller_State[1];
    Roller_State[1] = Roller_State[0];

    StickL_Vert[2] = StickL_Vert[1];
    StickL_Vert[1] = StickL_Vert[0];
	
    StickL_Hori[2] = StickL_Hori[1];
    StickL_Hori[1] = StickL_Hori[0];
	
    StickR_Vert[2] = StickR_Vert[1];
    StickR_Vert[1] = StickR_Vert[0];
	
    StickR_Hori[2] = StickR_Hori[1];
    StickR_Hori[1] = StickR_Hori[0];

		//����
		if (Get_Channel_Value(CH_Roll) < -0.2f && Get_Channel_Value(CH_Roll) >= -0.6f)//
				Roller_State[0] = Roller_Small_Front;
		else if (Get_Channel_Value(CH_Roll) > 0.2f && Get_Channel_Value(CH_Roll) <= 0.6f)//
				Roller_State[0] = Roller_Small_Back;
		else if (Get_Channel_Value(CH_Roll) < -0.6f)
				Roller_State[0] = Roller_Big_Front;
		else if (Get_Channel_Value(CH_Roll) > 0.6f)
				Roller_State[0] = Roller_Big_Back;
		else
				Roller_State[0] = Roller_Mid;

		//��ҡ��
		if ((Get_Channel_Value(CH_LeftHori)) < 0.2f && (Get_Channel_Value(CH_LeftHori)) > -0.2f) // �����󲦸ˣ���ǰ��ҡ�˵�ʱ����������ҡ����+-20%
		{
				if (Get_Channel_Value(CH_LeftVert) < -0.2f && Get_Channel_Value(CH_LeftVert) >= -0.6f)
						StickL_Vert[0] = Stick_Small_Back;
				else if (Get_Channel_Value(CH_LeftVert) > 0.2f && Get_Channel_Value(CH_LeftVert) <= 0.6f)
						StickL_Vert[0] = Stick_Small_Front;
				else if (Get_Channel_Value(CH_LeftVert) < -0.6f)
						StickL_Vert[0] = Stick_Big_Back;
				else if (Get_Channel_Value(CH_LeftVert) > 0.6f)
						StickL_Vert[0] = Stick_Big_Front;
				else
						StickL_Vert[0] = Stick_Mid_V;
		}
		if ((Get_Channel_Value(CH_LeftVert)) < 0.2f && (Get_Channel_Value(CH_LeftVert)) > -0.2f) // �����󲦸ˣ���ǰ��ҡ�˵�ʱ����������ҡ����+-20%
		{
				if (Get_Channel_Value(CH_LeftHori) < -0.2f && Get_Channel_Value(CH_LeftHori) >= -0.6f)
						StickL_Hori[0] = Stick_Small_Left;
				else if (Get_Channel_Value(CH_LeftHori) > 0.2f && Get_Channel_Value(CH_LeftHori) <= 0.6f)
						StickL_Hori[0] = Stick_Small_Right;
				else if (Get_Channel_Value(CH_LeftHori) < -0.6f)
						StickL_Hori[0] = Stick_Big_Left;
				else if (Get_Channel_Value(CH_LeftHori) > 0.6f)
						StickL_Hori[0] = Stick_Big_Right;
				else
						StickL_Hori[0] = Stick_Mid_H;
		}

		//��ҡ��
		if ((Get_Channel_Value(CH_RightHori)) < 0.2f && (Get_Channel_Value(CH_RightHori)) > -0.2f) // �����󲦸ˣ���ǰ��ҡ�˵�ʱ����������ҡ����+-20%
		{
				if (Get_Channel_Value(CH_RightVert) < -0.2f && Get_Channel_Value(CH_RightVert) >= -0.6f)
						StickR_Vert[0] = Stick_Small_Back;
				else if (Get_Channel_Value(CH_RightVert) > 0.2f && Get_Channel_Value(CH_RightVert) <= 0.6f)
						StickR_Vert[0] = Stick_Small_Front;
				else if (Get_Channel_Value(CH_RightVert) < -0.6f)
						StickR_Vert[0] = Stick_Big_Back;
				else if (Get_Channel_Value(CH_RightVert) > 0.6f)
						StickR_Vert[0] = Stick_Big_Front;
				else
						StickR_Vert[0] = Stick_Mid_V;
		}
		if ((Get_Channel_Value(CH_RightVert)) < 0.2f && (Get_Channel_Value(CH_RightVert)) > -0.2f) // �����󲦸ˣ���ǰ��ҡ�˵�ʱ����������ҡ����+-20%
		{
				if (Get_Channel_Value(CH_RightHori) < -0.2f && Get_Channel_Value(CH_RightHori) >= -0.6f)
						StickR_Hori[0] = Stick_Small_Left;
				else if (Get_Channel_Value(CH_RightHori) > 0.2f && Get_Channel_Value(CH_RightHori) <= 0.6f)
						StickR_Hori[0] = Stick_Small_Right;
				else if (Get_Channel_Value(CH_RightHori) < -0.6f)
						StickR_Hori[0] = Stick_Big_Left;
				else if (Get_Channel_Value(CH_RightHori) > 0.6f)
						StickR_Hori[0] = Stick_Big_Right;
				else
						StickR_Hori[0] = Stick_Mid_H;
		}
}

/**
 * @brief  ����ٶȵ��ƶ�ƽ���˲�
 * @note
 * @param
 * @retval
 */
void Mouse_Speed_Filter(void)
{
//		static int16_t X_Memory = 0;
//		static int16_t Y_Memory = 0;
		int Zero_X = 0;
//		int Zero_Y = 0;
		int x,y;
//	
//		//FilterX
//    Mouse_Speed_X[8] = Mouse_Speed_X[7];
//    Mouse_Speed_X[7] = Mouse_Speed_X[6];
//    Mouse_Speed_X[6] = Mouse_Speed_X[5];
//    Mouse_Speed_X[5] = Mouse_Speed_X[4];
//    Mouse_Speed_X[4] = Mouse_Speed_X[3];
//    Mouse_Speed_X[3] = Mouse_Speed_X[2];
//    Mouse_Speed_X[2] = Mouse_Speed_X[1];
//    Mouse_Speed_X[1] = Mouse_Speed_X[0];
//    Mouse_Speed_X[0] = Get_Mouse_Speed(mouse_axis_x);
//	
//		if(Get_Mouse_Speed(mouse_axis_x) == 0)
//		{
//				X_Memory = X_Memory;
//		}
//		else
//		{
//				if(abs(Get_Mouse_Speed(mouse_axis_x))>15)
//				{
//						X_Memory = Get_Mouse_Speed(mouse_axis_x)/2;
//				}
//				else
//				{
//						X_Memory = Get_Mouse_Speed(mouse_axis_x);
//				}
//		}
//		
//		if(abs(Get_Mouse_Speed(mouse_axis_x))>15)
//		{
//				x = Get_Mouse_Speed(mouse_axis_x)/2;
//		}
//		else
//		{
//				if(Get_Mouse_Speed(mouse_axis_x)==0)
//				{
//						x = X_Memory;
//				}
//				else
//				{
//						x = Get_Mouse_Speed(mouse_axis_x);
//				}
//		}
//		
//	  for(int i = 0;i<9;i++)
//		{
//				if(Mouse_Speed_X[i] == 0)
//				{
//						Zero_X++;
//				}
//						
//		}
//		Out_X = Zero_X>6?0:x;


//		//FilterY
//    Mouse_Speed_Y[8] = Mouse_Speed_Y[7];
//    Mouse_Speed_Y[7] = Mouse_Speed_Y[6];
//    Mouse_Speed_Y[6] = Mouse_Speed_Y[5];
//    Mouse_Speed_Y[5] = Mouse_Speed_Y[4];
//    Mouse_Speed_Y[4] = Mouse_Speed_Y[3];
//    Mouse_Speed_Y[3] = Mouse_Speed_Y[2];
//    Mouse_Speed_Y[2] = Mouse_Speed_Y[1];
//    Mouse_Speed_Y[1] = Mouse_Speed_Y[0];
//    Mouse_Speed_Y[0] = Get_Mouse_Speed(mouse_axis_y);
//		if(Get_Mouse_Speed(mouse_axis_y) == 0)
//		{
//				Y_Memory = Y_Memory;
//		}
//		else
//		{
//				if(abs(Get_Mouse_Speed(mouse_axis_y))>15)
//				{
//						Y_Memory = Get_Mouse_Speed(mouse_axis_y)/2;
//				}
//				else
//				{
//						Y_Memory = Get_Mouse_Speed(mouse_axis_y);
//				}
//		}
//		
//		if(abs(Get_Mouse_Speed(mouse_axis_y))>15)
//		{
//				y=Get_Mouse_Speed(mouse_axis_y)/2;
//		}
//		else
//		{
//				if(Get_Mouse_Speed(mouse_axis_y)==0)
//				{
//					y = Y_Memory;
//				}
//				else
//				{
//					y = Get_Mouse_Speed(mouse_axis_y);
//				}
//		}

//	  for(int i = 0;i<9;i++)
//		{
//				if(Mouse_Speed_Y[i] == 0)
//						Zero_Y++;
//		}
//		Out_Y = Zero_Y>6?0:y;

		static int16_t X_Memory = 0;
		static int16_t Y_Memory = 0;
		static int16_t X_Max = 0;
		static int16_t Y_Max = 0;
    
		if(abs(Get_Mouse_Speed(mouse_axis_x))>X_Max)X_Max = abs(Get_Mouse_Speed(mouse_axis_x));
    Mouse_Speed_X[8] = Mouse_Speed_X[7];
    Mouse_Speed_X[7] = Mouse_Speed_X[6];
    Mouse_Speed_X[6] = Mouse_Speed_X[5];
    Mouse_Speed_X[5] = Mouse_Speed_X[4];
    Mouse_Speed_X[4] = Mouse_Speed_X[3];
    Mouse_Speed_X[3] = Mouse_Speed_X[2];
    Mouse_Speed_X[2] = Mouse_Speed_X[1];
    Mouse_Speed_X[1] = Mouse_Speed_X[0];
    Mouse_Speed_X[0] = Get_Mouse_Speed(mouse_axis_x);
		X_Memory = Get_Mouse_Speed(mouse_axis_x) == 0?X_Memory:(abs(Get_Mouse_Speed(mouse_axis_x))>X_Max?Get_Mouse_Speed(mouse_axis_x)/2:Get_Mouse_Speed(mouse_axis_x));
		x = abs(Get_Mouse_Speed(mouse_axis_x))>X_Max?Get_Mouse_Speed(mouse_axis_x)/2:(Get_Mouse_Speed(mouse_axis_x)==0?X_Memory:Get_Mouse_Speed(mouse_axis_x));
		
	  for(int i = 0;i<9;i++)
		{if(Mouse_Speed_X[i] == 0)Zero_X++;}
		Out_X = Zero_X>(5)?0:x;
		Zero_X = 0;




    if(abs(Get_Mouse_Speed(mouse_axis_y))>Y_Max)Y_Max = abs(Get_Mouse_Speed(mouse_axis_y));
    Mouse_Speed_Y[8] = Mouse_Speed_Y[7];
    Mouse_Speed_Y[7] = Mouse_Speed_Y[6];
    Mouse_Speed_Y[6] = Mouse_Speed_Y[5];
    Mouse_Speed_Y[5] = Mouse_Speed_Y[4];
    Mouse_Speed_Y[4] = Mouse_Speed_Y[3];
    Mouse_Speed_Y[3] = Mouse_Speed_Y[2];
    Mouse_Speed_Y[2] = Mouse_Speed_Y[1];
    Mouse_Speed_Y[1] = Mouse_Speed_Y[0];
    Mouse_Speed_Y[0] = Get_Mouse_Speed(mouse_axis_y);

		Y_Memory = Get_Mouse_Speed(mouse_axis_y) == 0?Y_Memory:(abs(Get_Mouse_Speed(mouse_axis_y))>Y_Max/2?Get_Mouse_Speed(mouse_axis_y)/2:Get_Mouse_Speed(mouse_axis_y));
		y = abs(Get_Mouse_Speed(mouse_axis_y))>Y_Max/2?Get_Mouse_Speed(mouse_axis_y)/2:(Get_Mouse_Speed(mouse_axis_y)==0?Y_Memory:Get_Mouse_Speed(mouse_axis_y));
		
	  for(int i = 0;i<9;i++)
		{if(Mouse_Speed_Y[i] == 0)Zero_X++;}
		Out_Y = Zero_X>(5)?0:y;
		Zero_X = 0;
		
}



float KalmanFilter(int16_t inData)
{
		static float prevData = 0;                                 //��ǰ��ֵ
		static float p = 10, q = 0.001, r = 0.001, kGain = 0;      // q�������  r������Ӧ�ٶ� 
	
		p = p + q;
		kGain = p / ( p + r );                                     //���㿨��������
		inData = prevData + ( kGain * ( inData - prevData ) );     //���㱾���˲�����ֵ
		p = ( 1 - kGain ) * p;                                     //���²�������
		prevData = inData;
		return inData;                                             //�����˲�ֵ
}

/**
 * @brief  Get MainState 
 * @note
 * @param  None
 * @retval 
 */
MainState_t GetMainState(void)
{
    return MainState;
}

/**
 * @brief  Get SubState 
 * @note
 * @param  None
 * @retval 
 */
SubState_t GetSubState(void)
{
    return SubState;
}

/**
 * @brief  Get ControlMode
 * @note
 * @param  None
 * @retval 
 */
ControlMode_t GetControlMode(void)
{
    return ControlMode;
}

/**
 * @brief  Get PCMode
 * @note
 * @param  None
 * @retval 
 */
PCMode_t GetPCMode(void)
{
    return PCMode;
}


void Roller_PC_Update(void)
{
		Roller_State[2] = Roller_State[1];
		Roller_State[1] = Roller_State[0];
	
		if (Get_Channel_Value(CH_Roll) < -0.2f && Get_Channel_Value(CH_Roll) >= -0.6f)//
				Roller_State[0] = Roller_Small_Front;
		else if (Get_Channel_Value(CH_Roll) > 0.2f && Get_Channel_Value(CH_Roll) <= 0.6f)//
				Roller_State[0] = Roller_Small_Back;
		else if (Get_Channel_Value(CH_Roll) < -0.6f)
				Roller_State[0] = Roller_Big_Front;
		else if (Get_Channel_Value(CH_Roll) > 0.6f)
				Roller_State[0] = Roller_Big_Back;
		else
				Roller_State[0] = Roller_Mid;
		return;
}

