#ifndef _MOTION_SEND_H__
#define _MOTION_SEND_H__

/**Include Header Files**/
#include "sysconfig.h"

#include "Task_StateMachine.h"

/**Macro Definition**/


/**Struct Definition**/
typedef struct
{
		uint8_t SoH;
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
		uint8_t KeyBoard;
	
		int16_t PC_X;
		int16_t PC_Y;
		
		//uint8_t CRC;
		
		uint8_t EoT;
		
}UartSend_Data_t;

/**Extern Declaration**/


/**Function Delaration**/
void MCU_Control_CMD(void);
void Uart_Send(void);
void CRC_SUM(void);



#endif
