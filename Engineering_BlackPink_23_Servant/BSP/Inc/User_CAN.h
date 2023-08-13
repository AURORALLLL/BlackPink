#ifndef __USER_CAN_H__
#define __USER_CAN_H__


/**Include Header Files**/
#include "sysconfig.h"
#include "Motor.h"
#include "Motion_Motor.h"

#include "DIJ_Motor.h"

/**Function Delaration**/
void CAN_Init(CAN_HandleTypeDef *hcan);
void CAN_Recieve(CAN_HandleTypeDef *hcan);

void Can1Received_infoHandle(uint32_t stdid,uint8_t adata[]);
void Can2Received_infoHandle(uint32_t stdid,uint8_t adata[]);
#endif

