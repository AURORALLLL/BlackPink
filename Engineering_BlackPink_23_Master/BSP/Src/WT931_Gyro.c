//ά������9��������
#include "WT931_Gyro.h"
#include "Encoder.h"
#include "Motion_Motor.h"

WT931_Data_t WT931_Data;

//������ʱ�洢��������δ��������
Angle_t stcAngle;

/**
 * @brief  WT931ͨѶ��ʼ��
 * @note   ʹ�ܴ����жϣ���ʼ���ش���
 * @param 
 * @retval 
 */
void BSP_Init_WT931(void)
{
		//���ݳ�ʼ��
		WT931_Data.WT931FrameCounter = 0;
		for(uint16_t i=0; i < RXBUFFER_LEN; i++)	
				WT931_Data.RxBuffer[i] = 0;
		WT931_Data.frame_head = 0x55;
		WT931_Data.Rx_flag = 0;
		WT931_Data.Rx_len = 0;
	
		Gimbal_Gyro.Realcirnum = 0;;
		Gimbal_Gyro.RealAngle = 0;
		Gimbal_Gyro.AllAngle = 0;
	
		//Ӳ����ʼ��
		__HAL_UART_ENABLE_IT(&huart2,UART_IT_IDLE);  //���������ж�
		HAL_UART_Receive_DMA(&huart2,WT931_Data.RxBuffer,RXBUFFER_LEN);  //����DMA�Ľ���	
}


/**
 * @brief  ͨѶ���ڽ����жϻص����������ڴ���ش�ֵ
 * @note   
 * @param 
 * @retval �ú�������JetsonComm_CallBack�Աȷ���
 */
void WT931_CallBack(UART_HandleTypeDef *huart)
{
	
		uint32_t temp_flag = 0;
		uint32_t temp;
		DMA_HandleTypeDef *hdma_uart_rx = huart->hdmarx; // ������ָ���˴��ڰѽ��յ��Ķ����ŵ���
	
		//����uart��ӦUSART_IT_IDLE�жϱ�־λ
		temp_flag = __HAL_UART_GET_FLAG(huart,UART_FLAG_IDLE);
	
		if((temp_flag!=RESET))		//�жϽ����ж�														
		{
				//����жϱ�־λ
				__HAL_UART_CLEAR_IDLEFLAG(huart);	
		
				HAL_UART_DMAStop(huart);   												//�ر�DMA����ֹ��ͻ
			
				temp = __HAL_DMA_GET_COUNTER(hdma_uart_rx);						//��ȡDMA���Ѵ�������ݸ���
			
				WT931_Data.Rx_len = temp;  					//���һ������ĸ���
				WT931_Data.Rx_flag = 1;
			
				WT931_Process();																	//������
				
				WT931_Data.Rx_len = 0;
				WT931_Data.Rx_flag = 0;
				
			

		}
				HAL_UART_Receive_DMA(huart,WT931_Data.RxBuffer,RXBUFFER_LEN);		//���¿�ʼ����DMA
		    
		
}

/**
 * @brief  ������յ�������
 * @note   
 * @param 
 * @retval 
 */
void WT931_Process(void)
{
	
		//���ݳ��Ȳ���
		if(WT931_Data.Rx_len < RXBUFFER_LEN)
				return;

		//֡ͷ����
		if(WT931_Data.RxBuffer[0]!= WT931_Data.frame_head)
				return;
		
		//ͨ��֡��
		WT931_Data.WT931FrameCounter++;
		
		//�������ͣ�0x53	�Ƕ�
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

