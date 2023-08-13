#include "MG4005.h"
/*
warning:����д��ROM�����RAM,�ϵ粻ʧЧ���ģ�
���д���Ӱ��оƬ����������Ƶ��ʹ�á�

֡ͷ��
DATA[0]:֡ͷʶ��
DATA[1]:�����ֽ�
DATA[2]:���ID��1��32
DATA[3]:���ݳ���
DATA[4]:֡ͷУ��checksum

֡���ݣ�
DATA[4+1~4+N]:�������N�ֽ����ݣ�0~60)
DATA[4+N+1]:֡����У���ֽ�checksum
*/

//G4005_Uartsend_Type MG4005_Send1;
RS485Send_Type RS485Send;
motor_t MG4005;


/**����ͺ���**/
/**
 * @description: Calculate CheckSumByte
 * @param 
 * @return: void
 * @note: 
 */ 
uint8_t calc_checksum(uint8_t const *p_data, int32_t data_len)
{
    uint8_t sum = 0;

    while (data_len--) {
        sum += *p_data++;
    }
    return sum;
}


/**
 * @description: ������PI������ȡ
 * @param 
 * @note: �ĵ�����ţ���1��CMD��0x30
 */ 
void MG4005_Pid_read(uint8_t motor_id,UART_HandleTypeDef *huart)
{
		
	RS485Send.Data[0] = 0x3E;
	RS485Send.Data[1] = 0x30;
	RS485Send.Data[2] = motor_id;
	RS485Send.Data[3] = 0;
	RS485Send.Data[4] = calc_checksum(RS485Send.Data,4);
	
	HAL_UART_Transmit(huart,RS485Send.Data,5,0xff);
	
}


/**
 * @description: ������PI����дRAM���ϵ�ʧЧ��
 * @param 
 * @note: ��2��CMD��0x31
 */ 
void MG4005_Pid_WR2RAM(uint8_t motor_id,UART_HandleTypeDef *huart,
uint8_t angleKp,uint8_t angleKi,uint8_t speedKp,uint8_t speedKi,uint8_t iqKp,uint8_t iqKi)
{

	RS485Send.Data[0] = 0x3E;
	RS485Send.Data[1] = 0x31;
	RS485Send.Data[2] = motor_id;
	RS485Send.Data[3] = 6;
	RS485Send.Data[4] = calc_checksum(RS485Send.Data,4);
	RS485Send.Data[5] = angleKp;
	RS485Send.Data[6] = angleKi;
	RS485Send.Data[7] = speedKp;
	RS485Send.Data[8] = speedKi;
	RS485Send.Data[9] = iqKp;
	RS485Send.Data[10] = iqKi;
	RS485Send.Data[11] = calc_checksum(RS485Send.Data+5,6);
	
	HAL_UART_Transmit(huart,RS485Send.Data,12,0xff);
	
}


/**
 * @description: ������PI����дROM���ϵ���Ч����Ƶ��ʹ�ã�
 * @param 
 * @note: ��3��CMD��0x32
 */ 
void MG4005_Pid_WR2ROM(uint8_t motor_id,UART_HandleTypeDef *huart,
uint8_t angleKp,uint8_t angleKi,uint8_t speedKp,uint8_t speedKi,uint8_t iqKp,uint8_t iqKi)
{
	
	RS485Send.Data[0] = 0x3E;
	RS485Send.Data[1] = 0x32;
	RS485Send.Data[2] = motor_id;
	RS485Send.Data[3] = 6;
	RS485Send.Data[4] = calc_checksum(RS485Send.Data,4);
	RS485Send.Data[5] = angleKp;
	RS485Send.Data[6] = angleKi;
	RS485Send.Data[7] = speedKp;
	RS485Send.Data[8] = speedKi;
	RS485Send.Data[9] = iqKp;
	RS485Send.Data[10] = iqKi;
	RS485Send.Data[11] = calc_checksum(RS485Send.Data+5,6);
	
	HAL_UART_Transmit(huart,RS485Send.Data,12,0xff);
	
}


/**
 * @description: ת�رջ�����
 Ҫ����˨ת360�ȣ�����Ƕ�Ӧ�뵱ǰ��Ȧ�ǶȲ�360 000
 * @param iqControl Ŀ��ת�ص�����-2000~2000��Ӧ-32A~32A
 * @note: ����CMD��0xA1
 */
void MG4005_iqControl(uint8_t motor_id,UART_HandleTypeDef *huart,int16_t iqControl)
{
	RS485Send.Data[0] = 0x3E;
	RS485Send.Data[1] = 0xA1;
	RS485Send.Data[2] = motor_id;
	RS485Send.Data[3] = 2;
	RS485Send.Data[4] = calc_checksum(RS485Send.Data,4);
	RS485Send.Data[5] = iqControl & 0xff;
	RS485Send.Data[6] = iqControl >> 8;
	RS485Send.Data[7] = calc_checksum(RS485Send.Data+5,2);
	
	HAL_UART_Transmit(huart,RS485Send.Data,8,0xff);
	
}

/**
 * @description: ��Ȧ�Ƕȱջ�����
 * @param 
 * @note: ����CMD��0xA3
 */
void MG4005_AngleControl(uint8_t motor_id,UART_HandleTypeDef *huart,int64_t angle)
{
		RS485Send.Data[0] = 0x3E;
		RS485Send.Data[1] = 0xA3;
		RS485Send.Data[2] = motor_id;
		RS485Send.Data[3] = 8;
		RS485Send.Data[4] = calc_checksum(RS485Send.Data,4);
		RS485Send.Data[5] = *(uint8_t*)(&angle);
		RS485Send.Data[6] = *((uint8_t*)(&angle)+1);
		RS485Send.Data[7] = *((uint8_t*)(&angle)+2);
		RS485Send.Data[8] = *((uint8_t*)(&angle)+3);
		RS485Send.Data[9] = *((uint8_t*)(&angle)+4);
		RS485Send.Data[10] = *((uint8_t*)(&angle)+5);
		RS485Send.Data[11] = *((uint8_t*)(&angle)+6);
		RS485Send.Data[12] = *((uint8_t*)(&angle)+7);
		RS485Send.Data[13] = calc_checksum(RS485Send.Data+5,8);
		
		HAL_UART_Transmit(huart,RS485Send.Data,14,0xff);
	
}

/**
 * @description: ��Ȧ�Ƕȱջ�����,������
 * @param 
 * @note: ����CMD��0xA4
 */
void MG4005_MultiAngleMode2(uint8_t motor_id,UART_HandleTypeDef *huart,int64_t angle,uint32_t maxSpeed)
{
		//angleΪʵ�ʽǶ������㣩realangle*1000����90�ȶ�Ӧangle 90000
		//maxSpeedΪ��������ٶȣ�Ч�����ƺʹ����λ���ٶ�ģʽ����
		//Ŀǰȡ72000�����ڽ�Ծ��Ӧ�ﵽһ������������ٶȡ�
	
		RS485Send.Data[0] = 0x3E;
		RS485Send.Data[1] = 0xA4;
		RS485Send.Data[2] = motor_id;
		RS485Send.Data[3] = 0x0C;
		RS485Send.Data[4] = calc_checksum(RS485Send.Data,4);
	
		angle = angle *1000;
	
		RS485Send.Data[5] = *(uint8_t*)(&angle);
		RS485Send.Data[6] = *((uint8_t*)(&angle)+1);
		RS485Send.Data[7] = *((uint8_t*)(&angle)+2);
		RS485Send.Data[8] = *((uint8_t*)(&angle)+3);
		RS485Send.Data[9] = *((uint8_t*)(&angle)+4);
		RS485Send.Data[10] = *((uint8_t*)(&angle)+5);
		RS485Send.Data[11] = *((uint8_t*)(&angle)+6);
		RS485Send.Data[12] = *((uint8_t*)(&angle)+7);
	
		RS485Send.Data[13] = *((uint8_t*)(&maxSpeed));
		RS485Send.Data[14] = *((uint8_t*)(&maxSpeed)+1);
		RS485Send.Data[15] = *((uint8_t*)(&maxSpeed)+2);
		RS485Send.Data[16] = *((uint8_t*)(&maxSpeed)+3);
		
		RS485Send.Data[17] = calc_checksum(RS485Send.Data+5,12);
		
		HAL_UART_Transmit(huart,RS485Send.Data,18,0xff);
	
}

/**
 * @description: ��Ȧ�Ƕȿ��ƣ���ת���ϲ����˨,Ҫ����˨ת360�ȣ�����Ƕ�Ӧ�뵱ǰ��Ȧ�ǶȲ�360 000
 * @param 
 * @note: ����CMD��0x92
 */
void MG4005_AllAngle_Read(uint8_t motor_id,UART_HandleTypeDef *huart)
{
	
	RS485Send.Data[0] = 0x3E;
	RS485Send.Data[1] = 0x92;
	RS485Send.Data[2] = motor_id;
	RS485Send.Data[3] = 0;
	RS485Send.Data[4] = calc_checksum(RS485Send.Data,4);
	
	HAL_UART_Transmit(huart,RS485Send.Data,5,0xff);

}

/**
 * @description: �������
 * @param 
 * @note: �ĵ�����ţ���1��CMD��0x88
 */ 
void MG4005_Open(uint8_t motor_id,UART_HandleTypeDef *huart)
{
		
	RS485Send.Data[0] = 0x3E;
	RS485Send.Data[1] = 0x88;
	RS485Send.Data[2] = motor_id;
	RS485Send.Data[3] = 0;
	RS485Send.Data[4] = calc_checksum(RS485Send.Data,4);
	
	HAL_UART_Transmit(huart,RS485Send.Data,5,0xff);
	
}

/**���մ�����**/
/**
 * @description: MG4005Receive Tasks 
 * @param mot, Data, length
 * @return: void
 * @note: Handle Received MG4005 Encoder Data, command:0x92
 */ 
void MG4005_encoderDataHandle(motor_t* mot, uint8_t *Data,uint8_t length)
{
		mot->FeedbackData.FrameCounter++;
	
		int64_t test_angle1 = 0;
		int64_t test_angle2 = 0;
		int64_t test_angle = 0;
		int16_t test_cirnum = 0;

		//�ǶȻش�
		test_angle1 = ((Data[12] << 24 | Data[11] << 16) | Data[10] << 8) | Data[9];
		test_angle2 = ((Data[8] << 24 | Data[7] << 16) | Data[6] << 8) | Data[5];
		test_angle = (test_angle1 << 32) | test_angle2;
		
		if(mot->type == MOTOR_MG4005)
			test_angle = test_angle/1000;
		else if(mot->type == MOTOR_MF4005)
			test_angle = test_angle/100;//������Ϊʲô
		
		if(test_angle < 1000 && test_angle > -1000)
		{
				while (test_angle >= 360)
				{
						test_angle -= 360;
						test_cirnum += 1;
				}
				while (test_angle < 0)
				{
					test_angle += 360;
					test_cirnum -= 1;
				}
				
				mot->Realcirnum = test_cirnum;
				mot->RealAngle = test_angle;	
		}


}

/**
 * @description: MG4005Receive Tasks 
 * @param mot, Data, length
 * @return: void
 * @note: Handle Received MG4005 iq Control Data, command:0xA1
 */ 
void MG4005_iqControlDataHandle(motor_t* mot, uint8_t *Data,uint8_t length)
{
		mot->FeedbackData.FrameCounter++;
		//��������ֵ��ÿ�仯65536��16λ���ֵ����ʵ�ʶ����仯360/8 degree
		//Update RealAngle
//		int64_t temp_angle = 0;
//		int16_t temp_cirnum = 0;
	
//		temp_angle = Data[11] << 8 | Data[10];
		//temp_angle = temp_angle/1000.0f;
		
//		while (temp_angle >= 360)
//		{
//				temp_angle -= 360;
//				temp_cirnum += 1;
//		}
//		while (temp_angle < 0)
//		{
//			temp_angle += 360;
//			temp_cirnum -= 1;
//		}
//		mot->Realcirnum = temp_cirnum;
//		mot->RealAngle = temp_angle;
	
	
		//Update RealSpeed
		mot->FeedbackData.RealSpeed = (int16_t)(((Data[9] << 8) | Data[8]))* 0.1;
		mot->Realrotationrate = mot->FeedbackData.RealSpeed;

}
