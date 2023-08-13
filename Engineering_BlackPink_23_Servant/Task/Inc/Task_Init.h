#ifndef __TASK_INIT_H_
#define __TASK_INIT_H_

/**Include Header Files**/
#include "Task_Can.h"
#include "Task_StateMachine.h"
#include "Task_MotorControl.h"

#include "User_CAN.h"
#include "Judge_Comm.h"
#include "Uart_Comm.h"
#include "RemoteControl_Comm.h"

#include "sysconfig.h"

/**Extern Declaration**/
extern QueueHandle_t Queue_CANSend;

/**Function Delaration**/
void Task_Init(void *parameters);
void BSP_Init(void);


#endif





 
     

