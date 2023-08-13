#ifndef __ENCODER_H__
#define __ENCODER_H__

/**Include Header Files**/
#include "sysconfig.h"

/**Struct Definition**/
typedef struct
{
	uint32_t FrameCounter;
	uint8_t	Received_data;				//Abs_Encoder
	uint8_t id;										//Abs_Encoder
	
	uint16_t AngleCounter;				//Inc_Encoder
	uint16_t AngleCounter_Last;		//Inc_Encoder
	
	int8_t Realcirnum;
	int16_t RealAngle;
	int32_t AllAngle;

}Encoder_t;

//��������
void Encoder_Update(Encoder_t * encoder, TIM_HandleTypeDef time);//��������

#endif


