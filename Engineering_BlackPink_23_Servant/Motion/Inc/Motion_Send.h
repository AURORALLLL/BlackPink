#ifndef _MOTION_SEND_H__
#define _MOTION_SEND_H__

/**Include Header Files**/
#include "sysconfig.h"

#include "Task_StateMachine.h"

#include "Uart_Comm.h"

/**Macro Definition**/


/**Struct Definition**/
typedef struct
{
		uint8_t SoH;
		uint32_t FrameCounter;
	
		uint8_t LiftMotor_InPosition;
		uint8_t LiftMotor_BlockFinish;
	
		uint8_t EoT;
}UartSend_Data_t;

/**Extern Declaration**/


/**Function Delaration**/
void MCU_Control_CMD(void);
void Uart_Send(void);



#endif
