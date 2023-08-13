/**Include Header Files**/
#include "Task_Can.h"


/**Variable Definition**/
CanSend_Type CAN_Tx_Msg;


/**Function Declaration**/
/**
  * @brief  CAN���ͺ�������ȡ��Ҫ���͵����ݽ��з���
  * @param  unused
  * @retval void
  * @note   ���ȼ���,��ʱ�޿̲��ڷ���
  */
int time_can = 0;
TickType_t start_time3;
TickType_t can;
void Task_CAN(void *parameters)
{
    while(1)
    {		
				//start_time3 = xTaskGetTickCount();
        xQueueReceive(Queue_CANSend, &CAN_Tx_Msg, portMAX_DELAY); //��CAN���н�����Ҫ���͵�����
        switch (CAN_Tx_Msg.CANx)																	//��������ѡ����CAN�ӿڽ������ݷ���
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
  * @brief  CAN���͵ķ�װ������������������彨��ٶ�
  * @param  hcan1, stdid, aData
  * @retval void
  * @note   CAN���ͺ���
  */
void CANTransmit(CAN_HandleTypeDef *hcan, uint32_t std_id, uint8_t aData[])
{
    CAN_TxHeaderTypeDef TxHeader;
    TxHeader.StdId = std_id;			//��׼��ʶ����ֵ
    TxHeader.IDE = CAN_ID_STD;		//��׼֡
    TxHeader.RTR = CAN_RTR_DATA;	//����֡
    TxHeader.DLC = 8;							//���ݳ���
    if(hcan == &hcan1)						//������������ݣ����з���
    {
        HAL_CAN_AddTxMessage(hcan, &TxHeader, aData, (uint32_t*)CAN_TX_MAILBOX0);
    }
    else if(hcan == &hcan2)
        HAL_CAN_AddTxMessage(hcan, &TxHeader, aData, (uint32_t*)CAN_TX_MAILBOX1);
    else return;
}
