//维特智能9轴陀螺仪
#include "WT931_Gyro.h"
#include "Encoder.h"
#include "Motion_Motor.h"

WT931_Data_t WT931_Data;

//用于临时存储传输来的未处理数据
Angle_t stcAngle;

/**
 * @brief  WT931通讯初始化
 * @note   使能串口中断；初始化回传量
 * @param 
 * @retval 
 */
void BSP_Init_WT931(void)
{
		//数据初始化
		WT931_Data.WT931FrameCounter = 0;
		for(uint16_t i=0; i < RXBUFFER_LEN; i++)	
				WT931_Data.RxBuffer[i] = 0;
		WT931_Data.frame_head = 0x55;
		WT931_Data.Rx_flag = 0;
		WT931_Data.Rx_len = 0;
	
		Gimbal_Gyro.Realcirnum = 0;;
		Gimbal_Gyro.RealAngle = 0;
		Gimbal_Gyro.AllAngle = 0;
	
		//硬件初始化
		__HAL_UART_ENABLE_IT(&huart2,UART_IT_IDLE);  //开启空闲中断
		HAL_UART_Receive_DMA(&huart2,WT931_Data.RxBuffer,RXBUFFER_LEN);  //开启DMA的接收	
}


/**
 * @brief  通讯串口接收中断回调函数，用于处理回传值
 * @note   
 * @param 
 * @retval 该函数可与JetsonComm_CallBack对比分析
 */
void WT931_CallBack(UART_HandleTypeDef *huart)
{
	
		uint32_t temp_flag = 0;
		uint32_t temp;
		DMA_HandleTypeDef *hdma_uart_rx = huart->hdmarx; // 这个句柄指向了串口把接收到的东西放到那
	
		//接收uart对应USART_IT_IDLE中断标志位
		temp_flag = __HAL_UART_GET_FLAG(huart,UART_FLAG_IDLE);
	
		if((temp_flag!=RESET))		//判断进入中断														
		{
				//清除中断标志位
				__HAL_UART_CLEAR_IDLEFLAG(huart);	
		
				HAL_UART_DMAStop(huart);   												//关闭DMA，防止冲突
			
				temp = __HAL_DMA_GET_COUNTER(hdma_uart_rx);						//获取DMA中已传输的数据个数
			
				WT931_Data.Rx_len = temp;  					//获得一共传输的个数
				WT931_Data.Rx_flag = 1;
			
				WT931_Process();																	//处理函数
				
				WT931_Data.Rx_len = 0;
				WT931_Data.Rx_flag = 0;
				
			

		}
				HAL_UART_Receive_DMA(huart,WT931_Data.RxBuffer,RXBUFFER_LEN);		//重新开始接收DMA
		    
		
}

/**
 * @brief  处理接收到的数据
 * @note   
 * @param 
 * @retval 
 */
void WT931_Process(void)
{
	
		//数据长度不对
		if(WT931_Data.Rx_len < RXBUFFER_LEN)
				return;

		//帧头不对
		if(WT931_Data.RxBuffer[0]!= WT931_Data.frame_head)
				return;
		
		//通信帧数
		WT931_Data.WT931FrameCounter++;
		
		//数据类型：0x53	角度
		if(WT931_Data.RxBuffer[1] == 0x53)
		{
				memcpy(&stcAngle,&WT931_Data.RxBuffer[2],6);
			
				Gimbal_Gyro.FrameCounter++;
				if(Gimbal_Gyro.FrameCounter == 1)
				{
						WT931_Data.Angle.Angle_X = (float)stcAngle.Angle_X/32768*180;
						WT931_Data.Angle.Angle_Y = (float)stcAngle.Angle_Y/32768*180;
						WT931_Data.Angle.Angle_Z = (float)stcAngle.Angle_Z/32768*180;
					
						Gimbal_Gyro.Angle_X = WT931_Data.Angle.Angle_X;
						Gimbal_Gyro.Angle_Y = WT931_Data.Angle.Angle_Y;
						Gimbal_Gyro.Angle_Z = WT931_Data.Angle.Angle_Z + 180;	
					
						Gimbal_Gyro.Angle_X_Last = Gimbal_Gyro.Angle_X;
						Gimbal_Gyro.Angle_Y_Last = Gimbal_Gyro.Angle_Y;
						Gimbal_Gyro.Angle_Z_Last = Gimbal_Gyro.Angle_Z;
				}
				else
				{
						Gimbal_Gyro.Angle_X_Last = Gimbal_Gyro.Angle_X;
						Gimbal_Gyro.Angle_Y_Last = Gimbal_Gyro.Angle_Y;
						Gimbal_Gyro.Angle_Z_Last = Gimbal_Gyro.Angle_Z;
					
						WT931_Data.Angle.Angle_X = (float)stcAngle.Angle_X/32768*180;
						WT931_Data.Angle.Angle_Y = (float)stcAngle.Angle_Y/32768*180;
						WT931_Data.Angle.Angle_Z = (float)stcAngle.Angle_Z/32768*180;
					
						Gimbal_Gyro.Angle_X = WT931_Data.Angle.Angle_X;
						Gimbal_Gyro.Angle_Y = WT931_Data.Angle.Angle_Y;
						Gimbal_Gyro.Angle_Z = (WT931_Data.Angle.Angle_Z + 180);				
				}
			
				if(Gimbal_Gyro.Angle_Z_Last > 330 &&  Gimbal_Gyro.Angle_Z < 30)
				{
						Gimbal_Gyro.Realcirnum++;
				}
				if(Gimbal_Gyro.Angle_Z_Last < 30 &&  Gimbal_Gyro.Angle_Z > 330)
				{
						Gimbal_Gyro.Realcirnum--;
				}
				
				Gimbal_Gyro.RealAngle = Gimbal_Gyro.Angle_Z;
				Gimbal_Gyro.AllAngle = Gimbal_Gyro.Realcirnum * 360 + Gimbal_Gyro.RealAngle;
		}
}

