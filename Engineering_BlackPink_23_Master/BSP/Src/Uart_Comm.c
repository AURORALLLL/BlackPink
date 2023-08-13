/**Include Header Files**/
#include "Uart_Comm.h"


/**Variable Definition**/
UartReceive_Data_t UartReceive_Data;
uint8_t UartReceive_Counter = 0;
uint8_t Lift_Motor_InPosition = 0;

/**
 * @brief  ˫��ͨ�ų�ʼ��
 * @note   ʹ�ܴ����жϣ���ʼ���ش���
 * @param 
 * @retval 
 */
void BSP_Init_UartComm(void)
{
		UartReceive_Data.SoH = 0;											//֡ͷ
		UartReceive_Data.FrameCounter = 0;
		UartReceive_Data.LiftMotor_InPosition = 0;	
		UartReceive_Data.LiftMotor_BlockFinish = 0;
		UartReceive_Data.EoT = 0;											//֡β

   //Ӳ����ʼ��
		__HAL_UART_ENABLE_IT(&UARTCOMM_HUART,UART_IT_IDLE);  //���������ж�
		HAL_UART_Receive_DMA(&UARTCOMM_HUART,(uint8_t*)&UartReceive_Data,UartCommLen);//����DMA����
}


/**
 * @brief  ͨѶ���ڽ����жϻص����������ڴ���ش�ֵ
 * @note   
 * @param 
 * @retval 
 */
void UartComm_CallBack(UART_HandleTypeDef *huart)
{
	
		if(__HAL_UART_GET_IT_SOURCE(huart, UART_IT_IDLE) != RESET) //�ж�IDLE��־λ���
		{ 
					//  ֹͣDMA����
					__HAL_DMA_DISABLE(huart->hdmarx);
					
					//��������жϱ�־λ
					__HAL_UART_CLEAR_IDLEFLAG(huart);
					
					//��¼���յ����ֽ���	
					UartReceive_Counter = UartCommLen - __HAL_DMA_GET_COUNTER(huart->hdmarx);
			
					//�н��յ����ݣ������Ƿ�����ɺ�Ŀ��У�
					if (UartReceive_Counter > 0)
					{
							UartComm_Process();
					}
			
					//���´�DMA����
					__HAL_DMA_SET_COUNTER(huart->hdmarx, sizeof(UartReceive_Data_t));
				
					__HAL_DMA_ENABLE(huart->hdmarx);

		 } 
}

/**
 * @brief  ������յ�������
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
