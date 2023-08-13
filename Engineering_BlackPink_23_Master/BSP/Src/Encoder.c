/**Include Header Files**/
#include "Encoder.h"

/**
 * @brief  Update the Angle and CircleNum of Incremental Encoder 
 * @note   Pulse number:1024
 * @param  None
 * @retval None
 */
void Encoder_Update(Encoder_t * encoder, TIM_HandleTypeDef time)
{
		encoder->FrameCounter++;
    encoder->AngleCounter_Last = encoder->AngleCounter;

    encoder->AngleCounter = (uint32_t)(__HAL_TIM_GET_COUNTER(&time));
    encoder->RealAngle = ((float)encoder->AngleCounter) / 1024.0f * 360.0f;//实际角度 = 任何时刻累积脉冲值 / 一圈1024个脉冲 * 一圈360°

    //这里用于判断是否过零的参数，924和100是测出来的，即让升降机构处于最高的运动速度，测量编码器数值的单次最大变化量
    if (encoder->AngleCounter_Last > 924 && encoder->AngleCounter < 100)//正向过零
        encoder->Realcirnum += 1;
    if (encoder->AngleCounter_Last < 100 && encoder->AngleCounter > 924)//反向过零
        encoder->Realcirnum -= 1;
		
		encoder->AllAngle = encoder->Realcirnum * 360.0f + encoder->RealAngle;

    return;
}
