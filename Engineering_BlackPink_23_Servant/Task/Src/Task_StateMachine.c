/**Include Header Files**/
#include "Task_StateMachine.h"


/**Macro Definition**/
#define TASK_STATEMACHINE_INTERVAL (5)


/**Tips!!!**/
/*
	经验证，若要使用通过外来通信获取的数据进行操作，不将操作代码和外来通信写在一起会出现数据使用不及时、变量更新出错的问题。
	比如需要使用遥控器的命令控制电机，但是电机控制代码并非和遥控通信写在一起，就会出现遥控器拨动但是电机没有反应或是电
机多次反应的问题。因为平常都是拨动拨杆电机目标角度赋值，即便多次赋值，目标角度仍然还是我们预设的值，所以这个问题不
是很明显（以及非常偶尔的赋值失败）。但是如果是一些我们希望真的只执行一次的操作，那么就会有问题。
	考虑到Task_StateMachine是每接收到一次遥控器数据才进入一次，所以，将所有根据遥控器的电机操作都写在Task_StateMachine
中，而原来的Task_Suck Task_Mineral等任务仅仅拿来定义函数，不再在Task_Init中创建任务，即作为简单的BSP文件。
*/


/**Variable Definition**/
//State
MainState_t MainState;
SubState_t SubState;
ControlMode_t ControlMode;
PCMode_t PCMode;
//摇杆数值
float CH_LeftVert_Value;
float CH_LeftHori_Value;
float CH_RightVert_Value;
float CH_RightHori_Value;
float PC_X;
float PC_Y;
//Key_Value:Remote Control
Roller_State_t Roller_State[3] = {Roller_Mid, Roller_Mid, Roller_Mid};//拨轮方向
Stick_Vert_t StickL_Vert[3] = {Stick_Mid_V, Stick_Mid_V, Stick_Mid_V};//左摇杆竖直方向
Stick_Hori_t StickL_Hori[3] = {Stick_Mid_H, Stick_Mid_H, Stick_Mid_H};//左摇杆水平方向
Stick_Vert_t StickR_Vert[3] = {Stick_Mid_V, Stick_Mid_V, Stick_Mid_V};//右摇杆竖直方向
Stick_Hori_t StickR_Hori[3] = {Stick_Mid_H, Stick_Mid_H, Stick_Mid_H};//右摇杆水平方向
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
//鼠标速度
float Out_X = 0;
float Out_Y = 0;
float XX = 0;
float YY = 0;
float Mouse_Speed_X[9] = {0};
float Mouse_Speed_Y[9] = {0};

//堵转检测
SubState_t SubState_Last;//用于堵转运行判断
uint8_t SubState_flag = 0;//用于记录从保护状态切出的次数
uint8_t Protect_flag = 0;

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
		BottomMotor_Init();
		ChassisMotor_Init();
	
		Motor_Define = 1;
		

    while (1)
    {
				start_time1 = xTaskGetTickCount();
        //根据遥控器的数据解算本地变量
        //StateMachine_Update();
				if(GetSubState() == SubState_Protect)// && Protect_flag == 1)
				{
					
					
					
				}
				if(GetSubState() != SubState_Protect)
				{
						UpdataChassisState();
						if(Lift_Motor_BlockFlag == 1)
						{
								Motor_Calibrate();
						}
						else
						{
								Lift_Motor_BlockFinish = noblock;
								LiftL_Motor.Blockstate.BlockState = noblock;
								LiftR_Motor.Blockstate.BlockState = noblock;
								if(ControlMode == ControlMode_RC)
								{
										//Manual_Test();
										Manual_Control();
										Manual_InPosition();
								}
								if(ControlMode == ControlMode_PC)
								{
										Manual_Lift_Control();
										Manual_InPosition();
								}
						}
				}
				
				state = xTaskGetTickCount() - start_time1;
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
 * @retval 注意函数之间调用顺序！！！确保实时响应
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
    //遥控器主状态更新
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
        if (Get_Switch_Value(Switch_Right) == sw_up)//双拨上位保护
            SubState = SubState_Protect;
        else if (Get_Switch_Value(Switch_Right) == sw_mid)
            SubState = SubState_Test;
        else if (Get_Switch_Value(Switch_Right) == sw_down)//左上右下为键鼠控制
            SubState = SubState_Mineral;
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
            SubState = SubState_PC;
        break;

    default:
        break;
    }
		
//		//对从状态变化情况进行记录
//		if(SubState_Last == SubState_Protect && SubState != SubState_Protect)
//		{
//				SubState_flag++;
//				if(SubState_flag == 1)
//				{
//						Auto_Block_Flag = 1;
//				}
//		}
//		if(SubState_Last != SubState_Protect && SubState == SubState_Protect)
//		{
//				Protect_flag = 1;
//		}
		
		
//		if(SubState == SubState_Protect)
//		{
//				Auto_Block_Flag = 0;
//		}
//		if(SubState != SubState_Protect)
//		{
//				Protect_flag = 0;
//		}
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
	
}


/**
 * @brief  灯珠UI更新 
 * @note	 使用数组传递
 * @param
 * @retval According to Keyboard
 */
void LEDUI_Update(void)
{
//  if(Auto_Block_Flag ==1)Following_Lamp(RGBVALUE,RGBVALUE,0);
//	else if(Auto_Protect == 1)Following_Lamp(0,RGBVALUE,0);

//	else
//	{
//		//抬升状态显示：LED1
//		if(LiftL_G == GG)RGB_Bling(5,RGBVALUE,0,0);
//		if(LiftR_G == GG)RGB_Bling(1,RGBVALUE,0,0);
//		if(LiftR_G != GG&&LiftR_G != GG)
//		{
//		   if(Lift_State == Lift_State_Gold)
//        {
//					for(int i = 0;i<=6;i++){RGB_Value[i].Green = RGBVALUE; RGB_Value[i].Red = RGBVALUE+4;RGB_Value[i].Blue = 0;}
//				
//				}
//		   else if(Lift_State == Lift_State_Silver)
//				 {
//					for(int i = 0;i<=6;i++){RGB_Value[i].Green = RGBVALUE; RGB_Value[i].Red = RGBVALUE;RGB_Value[i].Blue = RGBVALUE;}
//				
//				 }
//			else if(Lift_State == Lift_State_Ground)
//				 {
//					for(int i = 0;i<=6;i++){RGB_Value[i].Green = 0; RGB_Value[i].Red = 2;RGB_Value[i].Blue = 2;}
//				
//				 }
//			else if(Lift_State == Lift_State_Hero)
//				{
//					for(int i = 0;i<=6;i++){RGB_Value[i].Green = 0; RGB_Value[i].Red = 0;RGB_Value[i].Blue = 2;}
//				
//				 }
//		   else 
//			 {
//				 for(int i = 0;i<=6;i++){RGB_Value[i].Green = 0; RGB_Value[i].Red = 0;RGB_Value[i].Blue = 0;}
//			 }
//	  }
//		
//		//平台电机异常显示：LED2
//		if(ForwardL_G==GG)RGB_Bling(4,RGBVALUE,0,0);
//		if(ForwardR_G==GG)RGB_Bling(2,RGBVALUE,0,0);
//		if(Rotate_G==GG||Pitch_G == GG)RGB_Bling(3,RGBVALUE,0,0);
//		
//		if(Chassis4_G==GG)RGB_Bling(0,RGBVALUE+1,RGBVALUE,0);//红为前面 橙色后面
//		else if(Chassis1_G==GG)RGB_Bling(0,RGBVALUE+1,0,0);
//		if(Chassis2_G==GG)RGB_Bling(6,RGBVALUE+1,0,0);
//		else if(Chassis3_G==GG)RGB_Bling(6,RGBVALUE+1,RGBVALUE,0);
//		
//		
//		
//		
//	}
//	
//	
//	ws2812_Set();
}



/**
 * @brief  Update Key Value 
 * @note	 使用数组传递，防止在实际使用过程中操作手多次重复按键产生重复效果
 * @param
 * @retval According to Keyboard
 */
void Key_Update(void)
{
    //每次更新[0]中数据，其余依次存储
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
		
    

    //获取按键按下情况
    if (GetControlMode() == ControlMode_PC) //PC控制模式
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
 * @note   拨轮返回-1~1的float，这里把它转换成前-中-后的状态量；拨杆即正常的前-中-后
 * @param  None
 * @retval According to Remote Control
 */
void Switch_Update(void)
{
    //每次更新[0]中数据，其余依次存储
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

		//拨轮
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

		//左摇杆
		if ((Get_Channel_Value(CH_LeftHori)) < 0.2f && (Get_Channel_Value(CH_LeftHori)) > -0.2f) // 避免误拨杆，当前后摇杆的时候限制左右摇杆在+-20%
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
		if ((Get_Channel_Value(CH_LeftVert)) < 0.2f && (Get_Channel_Value(CH_LeftVert)) > -0.2f) // 避免误拨杆，当前后摇杆的时候限制左右摇杆在+-20%
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

		//右摇杆
		if ((Get_Channel_Value(CH_RightHori)) < 0.2f && (Get_Channel_Value(CH_RightHori)) > -0.2f) // 避免误拨杆，当前后摇杆的时候限制左右摇杆在+-20%
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
		if ((Get_Channel_Value(CH_RightVert)) < 0.2f && (Get_Channel_Value(CH_RightVert)) > -0.2f) // 避免误拨杆，当前后摇杆的时候限制左右摇杆在+-20%
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
 * @brief  鼠标速度的移动平均滤波
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
		static float prevData = 0;                                 //先前数值
		static float p = 10, q = 0.001, r = 0.001, kGain = 0;      // q控制误差  r控制响应速度 
	
		p = p + q;
		kGain = p / ( p + r );                                     //计算卡尔曼增益
		inData = prevData + ( kGain * ( inData - prevData ) );     //计算本次滤波估计值
		p = ( 1 - kGain ) * p;                                     //更新测量方差
		prevData = inData;
		return inData;                                             //返回滤波值
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

