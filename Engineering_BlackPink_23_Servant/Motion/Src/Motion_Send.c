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
    UartSend_Data.LiftMotor_InPosition = Lift_Motor_InPosition;
		UartSend_Data.LiftMotor_BlockFinish = (uint8_t)Lift_Motor_BlockFinish;
}

/**
 * @brief  帧头帧尾更新，数据发送
 * @note   双板通信
 * @param  None
 * @retval
 */
void Uart_Send(void)
{
    UartSend_Data.SoH = 0x11;
    UartSend_Data.EoT = 0x14;
    UartSend_Data.FrameCounter++;

    HAL_UART_Transmit(&UARTCOMM_HUART,(uint8_t*)&UartSend_Data,sizeof(UartSend_Data),0xff);
}

