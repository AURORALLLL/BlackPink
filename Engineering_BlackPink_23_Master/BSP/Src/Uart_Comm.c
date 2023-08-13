/**Include Header Files**/
#include "Uart_Comm.h"


/**Variable Definition**/
UartReceive_Data_t UartReceive_Data;
uint8_t UartReceive_Counter = 0;
uint8_t Lift_Motor_InPosition = 0;

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
		UartReceive_Data.LiftMotor_InPosition = 0;	
		UartReceive_Data.LiftMotor_BlockFinish = 0;
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
	
		if(__HAL_UART_GET_IT_SOURCE(huart, UART_IT_IDLE) != RESET) //判断IDLE标志位情况
		{ 
					//  停止DMA传输
					__HAL_DMA_DISABLE(huart->hdmarx);
					
					//清除空闲中断标志位
					__HAL_UART_CLEAR_IDLEFLAG(huart);
					
					//记录接收到的字节数	
					UartReceive_Counter = UartCommLen - __HAL_DMA_GET_COUNTER(huart->hdmarx);
			
					//有接收到数据，而不是发送完成后的空闲！
					if (UartReceive_Counter > 0)
					{
							UartComm_Process();
					}
			
					//重新打开DMA接收
					__HAL_DMA_SET_COUNTER(huart->hdmarx, sizeof(UartReceive_Data_t));
				
					__HAL_DMA_ENABLE(huart->hdmarx);

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
				Lift_Motor_InPosition = UartReceive_Data.LiftMotor_InPosition;	
				Lift_Motor_BlockFinish = (BlockState_t)UartReceive_Data.LiftMotor_BlockFinish;	
		}
}
