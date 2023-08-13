#ifndef __MG4005_COMM_H__
#define __MG4005_COMM_H__

#include "sysconfig.h"
#include "MG4005.h"
#include "Motion_Motor.h"

#define MG4005_HUART    					huart6
#define MG4005Receive_Size 66//DMA传输字节数（最大）



void BSP_Init_MG4005Comm(void);
void MG4005Comm_CallBack(UART_HandleTypeDef *huart);
void MG4005_DataHandle(uint8_t* rx_data);

#endif
