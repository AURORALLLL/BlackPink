#ifndef __TASK_JETSONCOMM_H_
#define __TASK_JETSONCOMM_H_

#include "sysconfig.h"

#include "Motion_Send.h"

#define JetsonCommSOF       0x66
#define JetsonCommEOF       0x88
#define JetsonCommLen       sizeof(JetsonComm_Data_t)

typedef struct 
{
	uint8_t SoF;
  uint8_t FrameCounter;
	uint8_t FrameType;
			
  int8_t Flag; 
			
  int8_t X;
	int8_t Y;
	int8_t Z;
	int8_t Roll1;
	int8_t Yaw1;
	int8_t Pitch1;
			
	int8_t ChassisMove;
	int8_t PitchMove;
	int8_t RollMove;
			
	int8_t RotateDirection;
		
  uint8_t EoF;
}JetsonComm_Data_t;

extern JetsonComm_Data_t JetsonComm_Data;
extern JetsonComm_Data_t JetsonComm_Data_Temp;

void JetsonComm_Init(void);
void JetsonComm_CallBack(void);

#endif
