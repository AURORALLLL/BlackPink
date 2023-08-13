/** Include Headder Files **/
#include "sysconfig.h"
#include "MG4005_Comm.h"


/**Variable Definition**/
uint8_t MG4005_Counter = 0;
uint8_t MG4005_Receive_Counter = 0;
uint8_t MG4005Receive[70];

/**
 * @description:  Uart Init
 * @param Motor_ID
 * @return: void
 * @note: DMR4315_HUART
 */ 
void BSP_Init_MG4005Comm()
{
    //����DMA���ݰ���
    SET_BIT(MG4005_HUART.Instance->CR3, USART_CR3_DMAR);/*���ڿ��ƼĴ�����λDMA���ձ�־λ*/
    HAL_DMA_Start_IT(MG4005_HUART.hdmarx, (uint32_t)&(MG4005_HUART.Instance->DR), (uint32_t)&MG4005Receive, MG4005Receive_Size);
    __HAL_UART_ENABLE_IT(&MG4005_HUART, UART_IT_IDLE);
}
/**
 * @description: DMR4315Receive Uart Callback
 * @param huart
 * @return: void
 * @note: Receive random length through DMA
 */

void MG4005Comm_CallBack(UART_HandleTypeDef *huart)
{
	
	if(__HAL_UART_GET_IT_SOURCE(huart, UART_IT_IDLE) != RESET) //�ж�IDLE��־λ���
	{ 
		//  ֹͣDMA����
		__HAL_DMA_DISABLE(huart->hdmarx);
		
		//��������жϱ�־λ
		__HAL_UART_CLEAR_IDLEFLAG(huart);
		
    //��¼���յ����ֽ���
    MG4005_Counter = MG4005Receive_Size - __HAL_DMA_GET_COUNTER(huart->hdmarx);//�ܼ�����ȥδ��������ݸ������õ��Ѿ����յ����ݸ���
		
		
		//�н��յ����ݣ������Ƿ�����ɺ�Ŀ��У�
    if (MG4005_Counter > 0)
    {
				MG4005_DataHandle(MG4005Receive);
    }
		
				//���´�DMA����
				__HAL_DMA_SET_COUNTER(huart->hdmarx, MG4005Receive_Size);
			
				__HAL_DMA_ENABLE(huart->hdmarx);

	 } 
	 	 
}

/**
 * @description: MG4005Receive dataHandle
 * @param length
 * @return: void
 * @note: Deal with the received data of DMR4315
 */
void MG4005_DataHandle(uint8_t* rx_data)
{
		
		//У�������Ƿ���ȷ������ֱ���˳�����
		//data[2]���id�ݲ�����
		if(rx_data[0]!=0x3E)
				return;
		
		if(calc_checksum(rx_data,4)!=rx_data[4])
				return;
		
		if(rx_data[1] == 0x92)
			if(calc_checksum(rx_data+5,8)!=rx_data[13])
					return;
			
		else if(rx_data[1] == 0xA1 || rx_data[1] == 0xA4)
			if(calc_checksum(rx_data+5,7)!=rx_data[12])
					return;
					
					
		switch(rx_data[1])//�����ֽ�
		{
				case 0x92:
						MG4005_encoderDataHandle(&ArmYaw_Motor, rx_data, 14);//motor_t MG4005
				break;
				
				case 0xA1:case 0xA4:
						MG4005_iqControlDataHandle(&ArmYaw_Motor, rx_data, 13);
				break;

				default:
					break;

		}
		
}


