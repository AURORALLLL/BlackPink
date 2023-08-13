#ifndef _UART_COMM_H__
#define _UART_COMM_H__


/**Include Header Files**/
#include "sysconfig.h"

#include "Task_StateMachine.h"

#include "Motion_Exchange.h"
#include "Motion_Mineral.h"


/**Macro Definition**/
#define UARTCOMM_HUART 		huart3
#define Frame_Head   			0x11
#define Frame_Tail   			0x14
#define UartCommLen       sizeof(UartReceive_Data_t) + 10
	

/**Extern Declaration**/
extern uint8_t Lift_Motor_InPosition;


/**Struct Definition**/
typedef struct
{
		uint8_t SoH;
		uint32_t FrameCounter;
	
		uint8_t LiftMotor_InPosition;
		uint8_t LiftMotor_BlockFinish;
	
		uint8_t EoT;
}UartReceive_Data_t;


/**Function Delaration**/
void BSP_Init_UartComm(void);
void UartComm_CallBack(UART_HandleTypeDef *huart);
void UartComm_Process(void);

#endif
