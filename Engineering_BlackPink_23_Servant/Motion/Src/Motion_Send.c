/**Include Header Files**/
#include "Motion_Send.h"

/**Variable Definition**/
UartSend_Data_t UartSend_Data;

/**Function Declaration**/
/**
 * @brief  �Ӿ��������ݸ���
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
 * @brief  ֡ͷ֡β���£����ݷ���
 * @note   ˫��ͨ��
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

