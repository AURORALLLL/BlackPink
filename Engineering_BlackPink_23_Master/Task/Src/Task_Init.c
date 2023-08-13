/**Include Header Files**/
#include "Task_Init.h"

/**Variable Definition**/
#define STATEMACHINE_PRIORITY   13
#define STATEMACHINE_STACK_SIZE 256
TaskHandle_t STATEMACHINE_TASK_Handle;

#define CAN_PRIORITY 14
#define CAN_STACK_SIZE 256
TaskHandle_t CAN_TASK_Handle;

#define MOTORCONTROL_PRIORITY 12
#define MOTORCONTROL_STACK_SIZE 256
TaskHandle_t MOTORCONTROL_TASK_Handle;

//#define Ui_PRIORITY 10
//#define Ui_STACK_SIZE 256
//TaskHandle_t Ui_TASK_Handle;


QueueHandle_t Queue_CANSend;

/**Function Declaration**/
/**
 * @description: Create all task
 * @param {unused} 
 * @return: void
 * @note: 
 */
void Task_Init(void *parameters)
{
    taskENTER_CRITICAL(); // 初始化过程中禁止其他任务进行
    BSP_Init();


    xTaskCreate((TaskFunction_t)Task_CAN,
                (const char *)"Task_CAN",
                (uint16_t)CAN_STACK_SIZE,
                (void *)NULL,
                (UBaseType_t)CAN_PRIORITY,
                (TaskHandle_t *)&CAN_TASK_Handle);
								

    xTaskCreate((TaskFunction_t)Task_StateMachine,
                (const char *)"Task_StateMachine",
                (uint16_t)STATEMACHINE_STACK_SIZE,
                (void *)NULL,
                (UBaseType_t)STATEMACHINE_PRIORITY,
                (TaskHandle_t *)&STATEMACHINE_TASK_Handle);
								
								
    xTaskCreate((TaskFunction_t)Task_MotorControl,
                (const char *)"Task_MotorControl",
                (uint16_t)MOTORCONTROL_STACK_SIZE,
                (void *)NULL,
                (UBaseType_t)MOTORCONTROL_PRIORITY,
                (TaskHandle_t *)&MOTORCONTROL_TASK_Handle);		
								
								
//    xTaskCreate((TaskFunction_t)Task_Ui,
//                (const char *)"Task_Ui",
//                (uint16_t)Ui_STACK_SIZE,
//                (void *)NULL,
//                (UBaseType_t)Ui_PRIORITY,
//                (TaskHandle_t *)&Ui_TASK_Handle);


    HAL_Delay(1000);
    vTaskDelete(NULL);			//Delete Task_Init
    taskEXIT_CRITICAL();
}

/**
 * @description: Initialize Peripheral 
 * @param {unused} 
 * @return: void
 * @note: Queue, CAN, UART,
 */ 
void BSP_Init(void)
{
	  //BSP_Init_WT931();
    Queue_CANSend = xQueueCreate(30, sizeof(CanSend_Type)); //创建发送队列
    BSP_Init_RemoteControl();
		
		//图传链路
    //BSP_Init_JudgeComm();
		BSP_Init_UartComm();
		BSP_Init_WT931();
	
		BSP_Init_MG4005Comm();
		BSP_Init_DMR4315Comm();
	
    CAN_Init(&hcan1);
    CAN_Init(&hcan2);

    CAN_Recieve(&hcan1);
    CAN_Recieve(&hcan2);
}

