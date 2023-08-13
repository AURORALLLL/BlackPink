/** Include Headder Files **/
#include "DM_J4310.h"

/**Variable Definition**/
Motor_Inf mtr;

/**Function Declaration**/
/**
 * @description: DMJ4310Send Tasks 
 * @param Motor_ID, CAN
 * @return: void
 * @note: Start the DMJ4310 Motor
 */ 
void DMJ4310_motorOpen(uint8_t Motor_ID, uint8_t CAN_ID)
{
		static CanSend_Type CANSend;

    CANSend.CANx = CAN_ID;

    CANSend.stdid = DMJ_STDID + Motor_ID;

    CANSend.Data[0] = 0xFF;
    CANSend.Data[1] = 0xFF;
    CANSend.Data[2] = 0xFF;
    CANSend.Data[3] = 0xFF;
    CANSend.Data[4] = 0xFF;
    CANSend.Data[5] = 0xFF;
    CANSend.Data[6] = 0xFF;
    CANSend.Data[7] = 0xFC;

    xQueueSend(Queue_CANSend, &CANSend, 3 );
}

/**
 * @description: DMJ4310Send Tasks 
 * @param Motor_ID, CAN
 * @return: void
 * @note: Close the DMJ4310 Motor
 */ 
void DMJ4310_motorClose(uint8_t Motor_ID, uint8_t CAN_ID)
{
		static CanSend_Type CANSend;

    CANSend.CANx = CAN_ID;

    CANSend.stdid = DMJ_STDID + Motor_ID;

    CANSend.Data[0] = 0xFF;
    CANSend.Data[1] = 0xFF;
    CANSend.Data[2] = 0xFF;
    CANSend.Data[3] = 0xFF;
    CANSend.Data[4] = 0xFF;
    CANSend.Data[5] = 0xFF;
    CANSend.Data[6] = 0xFF;
    CANSend.Data[7] = 0xFD;

    xQueueSend(Queue_CANSend, &CANSend, 3 );
}

/**
 * @description: DMJ4310Send Tasks 
 * @param Motor_ID, CAN
 * @return: void
 * @note: Control the Motor with position and speed
 */ 
void DMJ4310_speedpositionControl(uint8_t Motor_ID, uint8_t CAN_ID, float _pos, float _vel)
{
		uint8_t *pbuf,*vbuf;
	
		_pos = _pos * 12.5f / 720.0f;
		pbuf=(uint8_t*)&_pos;
		vbuf=(uint8_t*)&_vel;

		static CanSend_Type CANSend;

    CANSend.CANx = CAN_ID;

    CANSend.stdid = DMJ_STDID + Motor_ID;
		
    CANSend.Data[0] = *pbuf;
    CANSend.Data[1] = *(pbuf+1);
    CANSend.Data[2] = *(pbuf+2);
    CANSend.Data[3] = *(pbuf+3);
    CANSend.Data[4] = *vbuf;
    CANSend.Data[5] = *(vbuf+1);
    CANSend.Data[6] = *(vbuf+2);
    CANSend.Data[7] = *(vbuf+3);

    xQueueSend(Queue_CANSend, &CANSend, 3 );	
}

/**
 * @description: DMJ4310Receive Tasks 
 * @param Motor, Data
 * @return: void
 * @note: Receive the Data of the Motor
 */ 
void DMJ4310_infoHandle(motor_t* mot, uint8_t *Data)
{
			int32_t temp_angle = 0;
			int16_t temp_cirnum = 0;
	
			//�ش����ݴ���stdid����ν�ġ�master id��
			//id��ʾ��������idȡCAN_ID�ĵͰ�λ
			mtr.id = (Data[0])&0x0F;
			//ERR�������룬8��ѹ��9Ƿѹ��A��������BMos���£�C�����Ȧ���£�DͨѶ��ʧ��E����
			mtr.state = (Data[0])>>4;

			//λ��
			mtr.p_int=(Data[1]<<8)|Data[2];
			//�ٶ�
			mtr.v_int=(Data[3]<<4)|(Data[4]>>4);
			//Ť��
			mtr.t_int=((Data[4]&0xF)<<8)|Data[5];

			mtr.pos = uint_to_float(mtr.p_int, P_MIN, P_MAX, 16); 
			mtr.vel = uint_to_float(mtr.v_int, V_MIN, V_MAX, 12);
			mtr.toq = uint_to_float(mtr.t_int, T_MIN, T_MAX, 12);  

			//����mos�¶�
			mtr.Tmos = (float)(Data[6]);

			//��Ȧ�¶����϶�
			mtr.Tcoil = (float)(Data[7]);
			
			mot->FeedbackData.FrameCounter++;
			
			//ԭ��λΪrad/s,תΪrps
			mot->Realrotationrate = mtr.vel;
			//ԭ��λΪrad,Ҫ��720/12.5
			temp_angle = mtr.pos * 720 / 12.5f;
			
			while (temp_angle >= 360)
			{
				temp_angle -= 360;
				temp_cirnum += 1;
			}
			while (temp_angle < 0)
			{
				temp_angle += 360;
				temp_cirnum -= 1;
			}
			mot->Realcirnum = temp_cirnum;
			mot->RealAngle = temp_angle;
}

/**
 * @description: Convert float to uint
 * @param 
 * @return: int
 * @note: 
 */ 
int float_to_uint(float x, float x_min, float x_max, int bits)
{
    /// Converts a float to an unsigned int, given range and number of bits ///
    float span = x_max - x_min;
    float offset = x_min;
    return (int) ((x-offset)*((float)((1<<bits)-1))/span);
}
    
/**
 * @description: Convert uint to float
 * @param 
 * @return: float
 * @note: 
 */   
float uint_to_float(int x_int, float x_min, float x_max, int bits)
{
    /// converts unsigned int to float, given range and number of bits ///
    float span = x_max - x_min;
    float offset = x_min;
    return ((float)x_int)*span/((float)((1<<bits)-1)) + offset;
}
