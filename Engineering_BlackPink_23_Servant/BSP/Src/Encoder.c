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
    encoder->RealAngle = ((float)encoder->AngleCounter) / 1024.0f * 360.0f;//ʵ�ʽǶ� = �κ�ʱ���ۻ�����ֵ / һȦ1024������ * һȦ360��

    //���������ж��Ƿ����Ĳ�����924��100�ǲ�����ģ�������������������ߵ��˶��ٶȣ�������������ֵ�ĵ������仯��
    if (encoder->AngleCounter_Last > 924 && encoder->AngleCounter < 100)//�������
        encoder->Realcirnum += 1;
    if (encoder->AngleCounter_Last < 100 && encoder->AngleCounter > 924)//�������
        encoder->Realcirnum -= 1;
		
		encoder->AllAngle = encoder->Realcirnum * 360.0f + encoder->RealAngle;

    return;
}
