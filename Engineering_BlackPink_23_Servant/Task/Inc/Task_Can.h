#ifndef _TASK_CAN_H__
#define _TASK_CAN_H__

/**Include Header Files**/
#include "sysconfig.h"

/** Macro Definition **/
#define CANSEND_1 1                //CANSEND的编号 1 0x200 2 0x1ff
#define CANSEND_2 2								 //CANSEND的编号 1 0x200 2 0x1ff

/**Struct Definition**/
typedef struct
{
    uint8_t            CANx;               //CAN发送结构体
    uint32_t           stdid;              
		uint8_t            Data[8];
}CanSend_Type;

/**Function Declaration**/
void Task_CAN(void *parameters);
void CANTransmit(CAN_HandleTypeDef *hcan, uint32_t std_id, uint8_t aData[]);

/**Task Function Declaration**/
extern QueueHandle_t Queue_CANSend;
#endif
