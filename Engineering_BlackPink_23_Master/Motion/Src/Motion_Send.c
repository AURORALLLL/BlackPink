/**Include Header Files**/
#include "Motion_Send.h"


/**Variable Definition**/
UartSend_Data_t UartSend_Data;


/**Function Declaration**/
/**
 * @brief  视觉发送数据更新
 * @note
 * @param  None
 * @retval
 */
void MCU_Control_CMD(void)
{
    //State
    UartSend_Data.MainState = (uint8_t)(GetMainState());
    UartSend_Data.SubState = (uint8_t)(GetSubState());
    UartSend_Data.ControlMode = (uint8_t)(GetControlMode());
    UartSend_Data.PCMode = (uint8_t)(GetPCMode());	
		
		UartSend_Data.GimbalState = (uint8_t)(Gimbal_State);
		UartSend_Data.LiftState = (uint8_t)(Lift_State);
		UartSend_Data.SpinState = (uint8_t)(Spin_State);
		UartSend_Data.ChassisState = (uint8_t)(Auto_Chassis_Flag);
		UartSend_Data.LiftMotor_BlockFlag = Lift_Motor_BlockFlag;
	
		UartSend_Data.CH_LeftVert_Value = Get_Channel_Value(CH_LeftVert) * 100;
		UartSend_Data.CH_LeftHori_Value = Get_Channel_Value(CH_LeftHori) * 100;
		UartSend_Data.CH_RightVert_Value = Get_Channel_Value(CH_RightVert) * 100;
		UartSend_Data.CH_RightHori_Value = Get_Channel_Value(CH_RightHori) * 100;
	
		UartSend_Data.ChassisFollowW = Chassis_Follow_W;
		UartSend_Data.ChassisOffset = Chassis_Offset;
		UartSend_Data.PitchIncline = Pitch_Incline;
		UartSend_Data.YawIncline = Yaw_Incline;
	
		UartSend_Data.StickLVert0 = StickL_Vert[0];
		UartSend_Data.StickLVert1 = StickL_Vert[1];
		UartSend_Data.StickLVert2 = StickL_Vert[2];
		UartSend_Data.KeyBoard =  (((uint8_t)R[0]) << 6) | (((uint8_t)W[0]) << 5) | (((uint8_t)S[0]) << 4) | (((uint8_t)A[0]) << 3) | \
															(((uint8_t)D[0]) << 2) | (((uint8_t)Mouse_Left[0]) << 1) | ((uint8_t)Mouse_Right[0]);
		
		UartSend_Data.PC_X = Out_X;
		UartSend_Data.PC_Y = Out_Y;
}

/**
 * @brief  帧头帧尾更新，数据发送
 * @note   双板通信
 * @param  None
 * @retval
 */
void Uart_Send(void)
{
    UartSend_Data.SoH = 0x01;
    UartSend_Data.EoT = 0x04;
    UartSend_Data.FrameCounter++;
	
		//CRC = CRC_SUM();
		

    HAL_UART_Transmit(&UARTCOMM_HUART,(uint8_t*)&UartSend_Data,sizeof(UartSend_Data),0xff);
}

/**
 * @brief  帧头帧尾更新，数据发送
 * @note   双板通信
 * @param  None
 * @retval
 */
void CRC_SUM(void)
{



}

