/**Include Header Files**/
#include "User_CAN.h"

/**Function Declaration**/
/**
 * @brief  CAN��ʼ��
 * @note   �˲�
 * @param  hcan1 hcan2�ĵ�ַ
 * @retval None
 */
void CAN_Init(CAN_HandleTypeDef *hcan)
{
    uint32_t FilterBank, FilterFIFO;
    CAN_FilterTypeDef sFilterConfig;
    if(hcan == &hcan1)
    {
        FilterBank = 0;
        FilterFIFO = CAN_RX_FIFO0;
    }
    else if(hcan == &hcan2)
    {
        FilterBank = 14;
        FilterFIFO = CAN_RX_FIFO1;
    }
    else
    {
        return;
    }
    //�����˲���
    sFilterConfig.FilterBank = FilterBank;
    sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
    sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
    sFilterConfig.FilterIdHigh = 0x0000;
    sFilterConfig.FilterIdLow = 0x0000;
    sFilterConfig.FilterMaskIdHigh = 0x0000;
    sFilterConfig.FilterMaskIdLow = 0x0000;
    sFilterConfig.FilterFIFOAssignment = FilterFIFO;
    sFilterConfig.FilterActivation = ENABLE;
    sFilterConfig.SlaveStartFilterBank = 14;
    HAL_CAN_ConfigFilter(hcan, &sFilterConfig);
    HAL_CAN_Start(hcan);
}

/**
 * @brief  ʹ��CAN�ж�
 * @note   can ���ж�ʹ��
 * @param  *hcan: ��Ӧcanָ��
 * @retval None
 */
void CAN_Recieve(CAN_HandleTypeDef *hcan)
{
    uint32_t ActiveITs;

    if(hcan == &hcan1)
    {
        ActiveITs = CAN_IT_RX_FIFO0_MSG_PENDING;
    }
    else if(hcan == &hcan2)
    {
        ActiveITs = CAN_IT_RX_FIFO1_MSG_PENDING;
    }
    else
    {
        return;
    }
    HAL_CAN_ActivateNotification(hcan, ActiveITs);
}

/**
 * @brief  FIFO0��CAN���ջص�����
 * @note   CAN1
 * @param  ��Ӧ��CAN���
 * @retval None
 */
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
    CAN_RxHeaderTypeDef RxHeader;
    uint8_t aData[8];	/*�������ݻ�������*/
    HAL_CAN_GetRxMessage(&hcan1, CAN_RX_FIFO0, &RxHeader, aData); /*��������ȡ������*/
    Can1Received_infoHandle(RxHeader.StdId,aData);
}

/**
 * @brief  FIFO1��CAN���ջص�����
 * @note   CAN2
 * @param  ��Ӧ��CAN���
 * @retval None
 */
void HAL_CAN_RxFifo1MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
    CAN_RxHeaderTypeDef RxHeader;
    uint8_t aData[8];	/*�������ݻ�������*/
    HAL_CAN_GetRxMessage(&hcan2, CAN_RX_FIFO1, &RxHeader, aData); /*��������ȡ������*/
    Can2Received_infoHandle(RxHeader.StdId,aData);
}

/**
 * @brief  Handle the Received Information though Can1
 * @note   CAN1
 * @param  stdid, adata
 * @retval None
 */
void Can1Received_infoHandle(uint32_t stdid,uint8_t adata[])
{
		switch (stdid)
		{
			case 0x201:
				DIJMotor_ParaHandle(&ChassisMotor[0],adata);
			break;
			
			case 0x202:
				DIJMotor_ParaHandle(&ChassisMotor[1],adata);
			break;
			
			case 0x203:
				DIJMotor_ParaHandle(&ChassisMotor[2],adata);
			break;
			
			case 0x204:
				DIJMotor_ParaHandle(&ChassisMotor[3],adata);
			break;
			
			default:
			break;
		}
}


/**
 * @brief  Handle the Received Information though Can2
 * @note   CAN2
 * @param  stdid, adata
 * @retval None
 */
void Can2Received_infoHandle(uint32_t stdid,uint8_t adata[])
{
		switch (stdid)
		{
			case M3508_M2006_RX_OFFSETID + LiftL_Motor_id:
				DIJMotor_ParaHandle(&LiftL_Motor,adata);
			break;
			
			case M3508_M2006_RX_OFFSETID + LiftR_Motor_id:
				DIJMotor_ParaHandle(&LiftR_Motor,adata);
			break;
			
			default:
			break;
		}
}
