/**Include Header Files**/
#include "Task_Can.h"


/**Variable Definition**/
CanSend_Type CAN_Tx_Msg;


/**Function Declaration**/
/**
  * @brief  CAN发送函数，读取需要发送的数据进行发送
  * @param  unused
  * @retval void
  * @note   优先级高,无时无刻不在发送
  */
int time_can = 0;
TickType_t start_time3;
TickType_t can;
void Task_CAN(void *parameters)
{
    while(1)
    {		
				//start_time3 = xTaskGetTickCount();
        xQueueReceive(Queue_CANSend, &CAN_Tx_Msg, portMAX_DELAY); //从CAN队列接收需要发送的数据
        switch (CAN_Tx_Msg.CANx)																	//根据数据选定的CAN接口进行数据发送
        {
        case CANSEND_1:
            CANTransmit(&hcan1,CAN_Tx_Msg.stdid,CAN_Tx_Msg.Data);
            break;
        case CANSEND_2:
            CANTransmit(&hcan2,CAN_Tx_Msg.stdid,CAN_Tx_Msg.Data);
            break;
        default:
            break;
        }
				//can = xTaskGetTickCount() - start_time3;
    }
}

/**
  * @brief  CAN发送的封装函数，具体变量的意义建议百度
  * @param  hcan1, stdid, aData
  * @retval void
  * @note   CAN发送函数
  */
void CANTransmit(CAN_HandleTypeDef *hcan, uint32_t std_id, uint8_t aData[])
{
    CAN_TxHeaderTypeDef TxHeader;
    TxHeader.StdId = std_id;			//标准标识符赋值
    TxHeader.IDE = CAN_ID_STD;		//标准帧
    TxHeader.RTR = CAN_RTR_DATA;	//数据帧
    TxHeader.DLC = 8;							//数据长度
    if(hcan == &hcan1)						//给邮箱加入数据，进行发送
    {
        HAL_CAN_AddTxMessage(hcan, &TxHeader, aData, (uint32_t*)CAN_TX_MAILBOX0);
    }
    else if(hcan == &hcan2)
        HAL_CAN_AddTxMessage(hcan, &TxHeader, aData, (uint32_t*)CAN_TX_MAILBOX1);
    else return;
}
