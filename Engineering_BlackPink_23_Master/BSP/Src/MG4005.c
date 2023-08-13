#include "MG4005.h"
/*
warning:凡需写入ROM（相比RAM,断电不失效）的，
多次写入会影响芯片寿命，不能频繁使用。

帧头：
DATA[0]:帧头识别
DATA[1]:命令字节
DATA[2]:电机ID，1到32
DATA[3]:数据长度
DATA[4]:帧头校验checksum

帧数据：
DATA[4+1~4+N]:命令附带的N字节数据（0~60)
DATA[4+N+1]:帧数据校验字节checksum
*/

//G4005_Uartsend_Type MG4005_Send1;
RS485Send_Type RS485Send;
motor_t MG4005;


/**命令发送函数**/
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
 * @description: 三环的PI参数读取
 * @param 
 * @note: 文档内序号：（1）CMD：0x30
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
 * @description: 三环的PI参数写RAM（断电失效）
 * @param 
 * @note: （2）CMD：0x31
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
 * @description: 三环的PI参数写ROM（断电有效，勿频繁使用）
 * @param 
 * @note: （3）CMD：0x32
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
 * @description: 转矩闭环控制
 要让螺栓转360度，输入角度应与当前多圈角度差360 000
 * @param iqControl 目标转矩电流，-2000~2000对应-32A~32A
 * @note: （）CMD：0xA1
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
 * @description: 多圈角度闭环控制
 * @param 
 * @note: （）CMD：0xA3
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
 * @description: 多圈角度闭环控制,有限速
 * @param 
 * @note: （）CMD：0xA4
 */
void MG4005_MultiAngleMode2(uint8_t motor_id,UART_HandleTypeDef *huart,int64_t angle,uint32_t maxSpeed)
{
		//angle为实际角度数（°）realangle*1000，如90度对应angle 90000
		//maxSpeed为限制最大速度，效果估计和达妙的位置速度模式类似
		//目前取72000可以在阶跃响应达到一个不慢不快的速度。
	
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
 * @description: 多圈角度控制，往转子上插个螺栓,要让螺栓转360度，输入角度应与当前多圈角度差360 000
 * @param 
 * @note: （）CMD：0x92
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
 * @description: 电机开启
 * @param 
 * @note: 文档内序号：（1）CMD：0x88
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

/**接收处理函数**/
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

		//角度回传
		test_angle1 = ((Data[12] << 24 | Data[11] << 16) | Data[10] << 8) | Data[9];
		test_angle2 = ((Data[8] << 24 | Data[7] << 16) | Data[6] << 8) | Data[5];
		test_angle = (test_angle1 << 32) | test_angle2;
		
		if(mot->type == MOTOR_MG4005)
			test_angle = test_angle/1000;
		else if(mot->type == MOTOR_MF4005)
			test_angle = test_angle/100;//别问我为什么
		
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
		//“编码器值”每变化65536（16位最大值），实际度数变化360/8 degree
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
