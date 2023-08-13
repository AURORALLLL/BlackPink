/** Include Headder Files **/
#include "DM_R_4315_Comm.h"


/**Variable Definition**/
uint8_t DMR4315Receive[DMR4315Receive_Size]={0};  	//接收数据缓存数组
int j= 0;

/**
 * @description: DMR4315Receive Uart Init
 * @param Motor_ID
 * @return: void
 * @note: DMR4315_HUART
 */ 
void BSP_Init_DMR4315Comm(void)
{
    //开启DMA数据搬运
    SET_BIT(DMR4315_HUART.Instance->CR3, USART_CR3_DMAR);/*串口控制寄存器置位DMA接收标志位*/
    HAL_DMA_Start_IT(DMR4315_HUART.hdmarx, (uint32_t)&(DMR4315_HUART.Instance->DR), (uint32_t)&DMR4315Receive, DMR4315Receive_Size);
    __HAL_UART_ENABLE_IT(&DMR4315_HUART, UART_IT_IDLE);
}
	uint8_t counter;
/**
 * @description: DMR4315Receive Uart Callback
 * @param huart
 * @return: void
 * @note: Receive random length through DMA
 */
void DMR4315Comm_CallBack(UART_HandleTypeDef *huart)
{

	if(__HAL_UART_GET_IT_SOURCE(huart, UART_IT_IDLE) != RESET) //判断IDLE标志位情况
	{ 
		//清除空闲中断标志位
		__HAL_UART_CLEAR_IDLEFLAG(huart);
		//  停止DMA传输
		__HAL_DMA_DISABLE(huart->hdmarx);
		
    //记录接收到的字节数
    counter = DMR4315Receive_Size - __HAL_DMA_GET_COUNTER(huart->hdmarx);//总计数减去未传输的数据个数，得到已经接收的数据个数
		
		//有接收到数据，而不是发送完成后的空闲！
    if (counter > 0)
    {
			DMR4315_dataHandle(counter);
    }
		
   //重新打开DMA接收
   __HAL_DMA_SET_COUNTER(huart->hdmarx, DMR4315Receive_Size);
   __HAL_DMA_ENABLE(huart->hdmarx);
	 } 
	 
}


/**
 * @description: DMR4315Receive dataHandle
 * @param length
 * @return: void
 * @note: Deal with the received data of DMR4315
 */
void DMR4315_dataHandle(uint8_t len)
{
	//Data package length, command ID
	uint8_t DMR4315_SOF = 0;
  uint16_t DataLength, CmdID;
	
  //Scan to find the start of Frame
  while (DMR4315_SOF < len)
	{
		if (DMR4315Receive[DMR4315_SOF] == 0x3C)
    {
			CmdID = DMR4315Receive[DMR4315_SOF + CommandID_Offset];
			DataLength = DMR4315Receive[DMR4315_SOF + DataLength_Offset];
			//CRC16_Modbus_Check
			if(len >= DMR4315_SOF + DataLength + DataHeader_Size + CRCLength_Size)
			{
				if(CRC16_Modbus_Check(&DMR4315Receive[DMR4315_SOF], DataLength + DataHeader_Size))
				{
					switch (CmdID)
					{
						case 0x0A:
							j++;
							DMR4315_infoHandle(&Gimbal_Motor, &DMR4315Receive[DMR4315_SOF], DataLength);
							break;
						
						case 0x0B:
							DMR4315_realtimeDataHandle(&Gimbal_Motor, &DMR4315Receive[DMR4315_SOF], DataLength);
							break;
						
						case 0x0C:case 0x0D:case 0x0E:case 0x0F:
							DMR4315_systemParaHandle(&Gimbal_Motor, &DMR4315Receive[DMR4315_SOF], DataLength);
							break;
						
						case 0x20:
							break;
						
						case 0x21:
							DMR4315_basepointHandle(&Gimbal_Motor, &DMR4315Receive[DMR4315_SOF], DataLength);
							break;
							
						case 0x2F:case 0x50:case 0x51:case 0x52:case 0x53:case 0x54:case 0x55:case 0x56:
							DMR4315_encoderDataHandle(&Gimbal_Motor, &DMR4315Receive[DMR4315_SOF], DataLength);
							break;
						
						case 0x40:case 0x41:
							DMR4315_realtimeStateHandle(&Gimbal_Motor, &DMR4315Receive[DMR4315_SOF], DataLength);
							break;
						
						case 0x57:
							DMR4315_speedpositionClosedloopHandle(&Gimbal_Motor, &DMR4315Receive[DMR4315_SOF], DataLength);
							break;
					}	
					DMR4315_SOF += (DataLength + DataHeader_Size + CRCLength_Size);
				}
				else
				{
					DMR4315_SOF += (DataHeader_Size + CRCLength_Size);
        }					
			}
			else
			{
				break;
			}
		}			
		else
		{
			DMR4315_SOF++;
		}
	}
}


