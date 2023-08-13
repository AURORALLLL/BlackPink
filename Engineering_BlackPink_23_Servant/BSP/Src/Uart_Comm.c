/**Include Header Files**/
#include "Uart_Comm.h"


/**Variable Definition**/
UartReceive_Data_t UartReceive_Data;
uint8_t UartReceive_Counter = 0;

/**Variable from External**/
extern TaskHandle_t STATEMACHINE_TASK_Handle;

/**
 * @brief  双板通信初始化
 * @note   使能串口中断；初始化回传量
 * @param 
 * @retval 
 */
void BSP_Init_UartComm(void)
{
		UartReceive_Data.SoH = 0;											//帧头
		UartReceive_Data.FrameCounter = 0;
		UartReceive_Data.MainState = MainState_Debug;
		UartReceive_Data.SubState = SubState_Protect;
		UartReceive_Data.ControlMode = ControlMode_RC;
		UartReceive_Data.PCMode = PCMode_Pick_Sky;
	
		UartReceive_Data.GimbalState = Gimbal_State_Front;
		UartReceive_Data.LiftState = Lift_State_Init;
		UartReceive_Data.SpinState = Spin_State_Static;
		UartReceive_Data.ChassisState = 0;
		UartReceive_Data.LiftMotor_BlockFlag = 0;
	
		UartReceive_Data.CH_LeftVert_Value = 0;
		UartReceive_Data.CH_LeftHori_Value = 0;
		UartReceive_Data.CH_RightVert_Value = 0;
		UartReceive_Data.CH_RightHori_Value = 0;
	
		UartReceive_Data.ChassisFollowW = 0;
		UartReceive_Data.ChassisOffset = 0;
		UartReceive_Data.PitchIncline = 0;
		UartReceive_Data.YawIncline = 0;
	
		UartReceive_Data.StickLVert0 = 0;
		UartReceive_Data.StickLVert1 = 0;
		UartReceive_Data.StickLVert2 = 0;
		UartReceive_Data.KeyBoard = 0;
	
		UartReceive_Data.PC_X = 0;
		UartReceive_Data.PC_Y = 0;	
		
		UartReceive_Data.EoT = 0;											//帧尾
	
   //硬件初始化
		__HAL_UART_ENABLE_IT(&UARTCOMM_HUART,UART_IT_IDLE);  //开启空闲中断
		HAL_UART_Receive_DMA(&UARTCOMM_HUART,(uint8_t*)&UartReceive_Data,UartCommLen);//开启DMA接收
}


/**
 * @brief  通讯串口接收中断回调函数，用于处理回传值
 * @note   
 * @param 
 * @retval 
 */
void UartComm_CallBack(UART_HandleTypeDef *huart)
{
			
		BaseType_t xHigherPriorityTaskWoken = pdFALSE;
		uint32_t success_flag = 0;	//接收成功标志位

		if(__HAL_UART_GET_IT_SOURCE(huart, UART_IT_IDLE) != RESET) //判断IDLE标志位情况
		{ 
					//  停止DMA传输
					__HAL_DMA_DISABLE(huart->hdmarx);
					
					//清除空闲中断标志位
					__HAL_UART_CLEAR_IDLEFLAG(huart);
					
					//记录接收到的字节数
					UartReceive_Counter = __HAL_DMA_GET_COUNTER(huart->hdmarx);//总计数减去未传输的数据个数，得到已经接收的数据个数
					
					
					//有接收到数据，而不是发送完成后的空闲！
					if (UartReceive_Counter > 0)
					{
							UartComm_Process();
							success_flag = 1;
					}
			
					//重新打开DMA接收
					__HAL_DMA_SET_COUNTER(huart->hdmarx, sizeof(UartReceive_Data_t));
				
					__HAL_DMA_ENABLE(huart->hdmarx);

		 } 
		
		if(success_flag == 1)
		{
				//发送任务通知给遥控任务
				vTaskNotifyGiveFromISR(STATEMACHINE_TASK_Handle,&xHigherPriorityTaskWoken);

				/*强制FreeRTOS任务切换*/
				portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
		}
}

/**
 * @brief  处理接收到的数据
 * @note   
 * @param 
 * @retval 
 */
void UartComm_Process(void)
{
	if(UartReceive_Data.SoH == Frame_Head && UartReceive_Data.EoT == Frame_Tail)
	{
			MainState = (MainState_t)UartReceive_Data.MainState;
			SubState = (SubState_t)UartReceive_Data.SubState;
			ControlMode = (ControlMode_t)UartReceive_Data.ControlMode;
			PCMode = (PCMode_t)UartReceive_Data.PCMode;
					
			Gimbal_State = (Gimbal_State_t)UartReceive_Data.GimbalState;
			Lift_State = (Lift_State_t)UartReceive_Data.LiftState;
			Spin_State = (Spin_State_t)UartReceive_Data.SpinState;
			Auto_Chassis_Flag = UartReceive_Data.ChassisState;
			Lift_Motor_BlockFlag = UartReceive_Data.LiftMotor_BlockFlag;
		
			CH_LeftVert_Value = UartReceive_Data.CH_LeftVert_Value / 100.0f;
			CH_LeftHori_Value = UartReceive_Data.CH_LeftHori_Value / 100.0f;
			CH_RightVert_Value = UartReceive_Data.CH_RightVert_Value / 100.0f;
			CH_RightHori_Value = UartReceive_Data.CH_RightHori_Value / 100.0f;
		
			Chassis_Follow_W = UartReceive_Data.ChassisFollowW;
			Chassis_Offset = UartReceive_Data.ChassisOffset;
			Pitch_Incline = UartReceive_Data.PitchIncline;
			Yaw_Incline = UartReceive_Data.YawIncline;
		
			StickL_Vert[0] = 	(Stick_Vert_t)UartReceive_Data.StickLVert0;
			StickL_Vert[1] = 	(Stick_Vert_t)UartReceive_Data.StickLVert1;
			StickL_Vert[2] = 	(Stick_Vert_t)UartReceive_Data.StickLVert2;
		
			R[0] = (Key_Value_t)((((uint8_t)UartReceive_Data.KeyBoard) & 0x40) >> 6);
			W[0] = (Key_Value_t)((((uint8_t)UartReceive_Data.KeyBoard) & 0x20) >> 5);
			S[0] = (Key_Value_t)((((uint8_t)UartReceive_Data.KeyBoard) & 0x10) >> 4);
			A[0] = (Key_Value_t)((((uint8_t)UartReceive_Data.KeyBoard) & 0x08) >> 3);
			D[0] = (Key_Value_t)((((uint8_t)UartReceive_Data.KeyBoard) & 0x04) >> 2);
			Mouse_Left[0] = (Key_Value_t)((((uint8_t)UartReceive_Data.KeyBoard) & 0x02) >> 1);
			Mouse_Right[0] = (Key_Value_t)(((uint8_t)UartReceive_Data.KeyBoard) & 0x01);
	
			Out_X = UartReceive_Data.PC_X;
			Out_Y = UartReceive_Data.PC_Y;
	}
	
}


