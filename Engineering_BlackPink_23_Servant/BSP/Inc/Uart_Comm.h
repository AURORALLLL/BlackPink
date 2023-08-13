#ifndef _UART_COMM_H__
#define _UART_COMM_H__


/**Include Header Files**/
#include "sysconfig.h"

#include "Task_StateMachine.h"


/**Macro Definition**/
#define UARTCOMM_HUART 		huart1
#define Frame_Head   			0x01
#define Frame_Tail   			0x04
#define UartCommLen       sizeof(UartReceive_Data_t) + 10


/**Struct Definition**/
typedef struct
{
		uint8_t SoH;											//start of frame
		uint32_t FrameCounter;
	
		//State
		uint8_t MainState;
		uint8_t SubState;
		uint8_t ControlMode;
		uint8_t PCMode;
	
		uint8_t GimbalState;
		uint8_t LiftState;
		uint8_t SpinState;
		uint8_t ChassisState;
		uint8_t LiftMotor_BlockFlag;
	
		//KEY Value
		int16_t CH_LeftVert_Value;
		int16_t CH_LeftHori_Value;
		int16_t CH_RightVert_Value;
		int16_t CH_RightHori_Value;
	
		int16_t ChassisFollowW;
		int16_t ChassisOffset;
		int16_t PitchIncline;
		int16_t YawIncline;
	
		int8_t StickLVert0;
		int8_t StickLVert1;
		int8_t StickLVert2;
		int8_t KeyBoard;
	
		int16_t PC_X;
		int16_t PC_Y;

		uint8_t EoT;											//end of frame
}UartReceive_Data_t;


/**Function Delaration**/
void BSP_Init_UartComm(void);
void UartComm_CallBack(UART_HandleTypeDef *huart);
void UartComm_Process(void);

#endif
