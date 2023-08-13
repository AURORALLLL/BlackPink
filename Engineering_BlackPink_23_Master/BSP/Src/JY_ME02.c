/** Include Headder Files **/
#include "JY_ME02.h"

/**Function Declaration**/
/**
 * @description: JY_ME02 Encoder Data Handle
 * @param Encoder, Data
 * @return: void
 * @note: Receive the Data of the Motor
 */ 
void Encoder_DataHandler(Encoder_t * encoder, uint8_t *Data)
{
		if(Data[0]!=0x55)
				return;
		if(Data[1]!=0x55)
				return;
		
		encoder->FrameCounter++;
		encoder->RealAngle = (((Data[3]<<8)|Data[2])*360)/32768;
		encoder->Realcirnum = (Data[7]<<8)|Data[6];
		encoder->AllAngle = encoder->Realcirnum * 360 + encoder->RealAngle;
}
