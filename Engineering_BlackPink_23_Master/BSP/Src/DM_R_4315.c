/** Include Headder Files **/
#include "DM_R_4315.h"

/**
Data[0]:协议头		
Data[1]:包序号
Data[2]:设备地址
Data[3]:命令码
Data[4]:数据包长度
Data[...]:数据内容
Data[n-2]:校验位CRC16低字节
Data[n-1]:校验位CRC16高字节
*/

/**Variable Definition**/
motor_t DMR4315;
uint8_t Basepoint_Flag = 0;
uint8_t Baud_rate = 0;
float PositionKp_Set = 0;
float TargetSpeed_Set = 0;
float SpeedKp_Set = 0;
float SpeedKi_Set = 0;
float FilterCoefficient_Set = 0;
uint8_t PowerPer_Set = 0;

uint8_t a[7];

/**
 * @description: DMR4315Send Tasks 
 * @param Motor_ID, huart
 * @return: void
 * @note: Read DMR4315 Information, command:0x0A
 */ 
void DMR4315_infoRead(uint8_t Motor_ID,UART_HandleTypeDef *huart)
{
	static RS485Send_Type RS485Send;
	uint8_t length = len_infoRead;

	RS485Send.Data[0] = 0x3E;		
	RS485Send.Data[1] = 0x00;
	RS485Send.Data[2] = Motor_ID;
	RS485Send.Data[3] = 0x0A;
	RS485Send.Data[4] = 0x00;
	RS485Send.Data[5] = 0;
	RS485Send.Data[6] = 0;
	
	CRC16_Modbus_Cal(&RS485Send,length-2);
	
	HAL_UART_Transmit(huart,RS485Send.Data,length,0xff);
	
	
}


/**
 * @description: DMR4315Send Tasks 
 * @param Motor_ID, huart
 * @return: void
 * @note: Read DMR4315 real-time Data, command:0x0B
 */ 
void DMR4315_realtimeDataRead(uint8_t Motor_ID,UART_HandleTypeDef *huart)
{
	static RS485Send_Type RS485Send;		
	uint8_t length = len_realtimeDataRead;

	RS485Send.Data[0] = 0x3E;		
	RS485Send.Data[1] = 0x00;
	RS485Send.Data[2] = Motor_ID;
	RS485Send.Data[3] = 0x0B;
	RS485Send.Data[4] = 0x00;
	RS485Send.Data[5] = 0;
	RS485Send.Data[6] = 0;
	
	CRC16_Modbus_Cal(&RS485Send,length-2);
	
	HAL_UART_Transmit(huart,RS485Send.Data,length,0xff);
	
	
}


/**
 * @description: DMR4315Send Tasks 
 * @param Motor_ID, huart
 * @return: void
 * @note: Read DMR4315 System Parameters, command:0x0C
 */ 
void DMR4315_systemParaRead(uint8_t Motor_ID,UART_HandleTypeDef *huart)
{
	static RS485Send_Type RS485Send;	
	uint8_t length = len_systemParaRead;

	RS485Send.Data[0] = 0x3E;		
	RS485Send.Data[1] = 0x00;
	RS485Send.Data[2] = Motor_ID;
	RS485Send.Data[3] = 0x0C;
	RS485Send.Data[4] = 0x00;
	RS485Send.Data[5] = 0;
	RS485Send.Data[6] = 0;
	
	CRC16_Modbus_Cal(&RS485Send,length-2);
	
	HAL_UART_Transmit(huart,RS485Send.Data,length,0xff);
	
	
}

/**
 * @description: DMR4315Send Tasks 
 * @param Motor_ID, huart, Angle_Kp, Target_Speed, Speed_Kp, Speed_Ki
 * @return: void
 * @note: Set DMR4315 System Parameters, Without Save, command:0x0D
 */ 
void DMR4315_systemParaSetwithoutSave(uint8_t Motor_ID,UART_HandleTypeDef *huart,uint32_t Angle_Kp, uint32_t Target_Speed,uint32_t Speed_Kp, uint32_t Speed_Ki)
{
	static RS485Send_Type RS485Send;
	uint8_t length = len_systemParaSet;
	uint8_t temp[4];

	RS485Send.Data[0] = 0x3E;		
	RS485Send.Data[1] = 0x00;
	RS485Send.Data[2] = Motor_ID;
	RS485Send.Data[3] = 0x0D;
	RS485Send.Data[4] = 0x1A;
	
	RS485Send.Data[5] = 0x00;					//unused
	RS485Send.Data[6] = 0x22;
	RS485Send.Data[7] = 0x82;
	RS485Send.Data[8] = 0x00;					//high 4 bits unused
	
	/**Position PID Kp**/
	*(float*)temp = Angle_Kp;
	RS485Send.Data[9] = temp[0];
	RS485Send.Data[10] = temp[1];
	RS485Send.Data[11] = temp[2];
	RS485Send.Data[12] = temp[3];
	
	/**Position Close Loop Target Speed**/
	Target_Speed = Target_Speed/0.1f;
	*(float*)temp = Target_Speed;
	RS485Send.Data[13] = temp[0];
	RS485Send.Data[14] = temp[1];
	RS485Send.Data[15] = temp[2];
	RS485Send.Data[16] = temp[3];
	
	/**Speed PID Kp**/
	*(float*)temp = Speed_Kp;
	RS485Send.Data[17] = temp[0];
	RS485Send.Data[18] = temp[1];
	RS485Send.Data[19] = temp[2];
	RS485Send.Data[20] = temp[3];
	
	/**Speed PID Ki**/
	*(float*)temp = Speed_Ki;
	RS485Send.Data[21] = temp[0];
	RS485Send.Data[22] = temp[1];
	RS485Send.Data[23] = temp[2];
	RS485Send.Data[24] = temp[3];
	
	/**Reserved**/
	RS485Send.Data[25] = 0x00;
	RS485Send.Data[26] = 0x00;
	RS485Send.Data[27] = 0x00;
	RS485Send.Data[28] = 0x00;
	
	
	/**Speed Filter Coefficient**/
	RS485Send.Data[29] = 0x00;
	/**Motor power factor**/
	RS485Send.Data[30] = 0x5C;
	
	RS485Send.Data[31] = 0;
	RS485Send.Data[32] = 0;
	
	CRC16_Modbus_Cal(&RS485Send,length-2);
	
	HAL_UART_Transmit(huart,RS485Send.Data,length,0xff);
	
	
}



/**
 * @description: DMR4315Send Tasks 
 * @param Motor_ID, huart, Angle_Kp, Target_Speed, Speed_Kp, Speed_Ki
 * @return: void
 * @note: Set DMR4315 System Parameters, With Save, command:0x0E
 */ 
void DMR4315_systemParaSetwithSave(uint8_t Motor_ID,UART_HandleTypeDef *huart,uint32_t Angle_Kp, uint32_t Target_Speed,uint32_t Speed_Kp, uint32_t Speed_Ki)
{
	static RS485Send_Type RS485Send;	
	uint8_t length = len_systemParaSet;
	uint8_t temp[4];

	RS485Send.Data[0] = 0x3E;		
	RS485Send.Data[1] = 0x00;
	RS485Send.Data[2] = Motor_ID;
	RS485Send.Data[3] = 0x0E;
	RS485Send.Data[4] = 0x1A;
	
	RS485Send.Data[5] = 0x00;					//unused
	RS485Send.Data[6] = 0x1E;
	RS485Send.Data[7] = 0x82;
	RS485Send.Data[8] = 0x00;					//high 4 bits unused
	
	/**Position PID Kp**/
	*(float*)temp = Angle_Kp;
	RS485Send.Data[9] = temp[0];
	RS485Send.Data[10] = temp[1];
	RS485Send.Data[11] = temp[2];
	RS485Send.Data[12] = temp[3];
	
	/**Position Close Loop Target Speed**/
	Target_Speed = Target_Speed/0.1f;
	*(float*)temp = Target_Speed;
	RS485Send.Data[13] = temp[0];
	RS485Send.Data[14] = temp[1];
	RS485Send.Data[15] = temp[2];
	RS485Send.Data[16] = temp[3];
	
	/**Speed PID Kp**/
	*(float*)temp = Speed_Kp;
	RS485Send.Data[17] = temp[0];
	RS485Send.Data[18] = temp[1];
	RS485Send.Data[19] = temp[2];
	RS485Send.Data[20] = temp[3];
	
	/**Speed PID Ki**/
	*(float*)temp = Speed_Ki;
	RS485Send.Data[21] = temp[0];
	RS485Send.Data[22] = temp[1];
	RS485Send.Data[23] = temp[2];
	RS485Send.Data[24] = temp[3];
	
	/**Reserved**/
	RS485Send.Data[25] = 0x00;
	RS485Send.Data[26] = 0x00;
	RS485Send.Data[27] = 0x00;
	RS485Send.Data[28] = 0x00;
	
	
	/**Speed Filter Coefficient**/
	RS485Send.Data[29] = 0;
	/**Motor power factor**/
	RS485Send.Data[30] = 0x5C;
	
	RS485Send.Data[31] = 0;
	RS485Send.Data[32] = 0;
	
	CRC16_Modbus_Cal(&RS485Send,length-2);
	
	HAL_UART_Transmit(huart,RS485Send.Data,length,0xff);
	
	
}


/**
 * @description: DMR4315Send Tasks 
 * @param Motor_ID, huart
 * @return: void
 * @note: Restore the factory settings, command:0x0F
 */ 
void DMR4315_factoryReset(uint8_t Motor_ID,UART_HandleTypeDef *huart)
{
	static RS485Send_Type RS485Send;	
	uint8_t length = len_factoryReset;

	RS485Send.Data[0] = 0x3E;		
	RS485Send.Data[1] = 0x00;
	RS485Send.Data[2] = Motor_ID;
	RS485Send.Data[3] = 0x0F;
	RS485Send.Data[4] = 0x00;
	RS485Send.Data[5] = 0;
	RS485Send.Data[6] = 0;
	
	CRC16_Modbus_Cal(&RS485Send,length-2);
	
	HAL_UART_Transmit(huart,RS485Send.Data,length,0xff);
	
	
}


/**
 * @description: DMR4315Send Tasks 
 * @param Motor_ID, huart
 * @return: void
 * @note: Calibrate Encoder, command:0x20, without test!!!
 */ 
void DMR4315_encoderCalibrate(uint8_t Motor_ID,UART_HandleTypeDef *huart)
{
	static RS485Send_Type RS485Send;	
	uint8_t length = len_encoderCalibrate;

	RS485Send.Data[0] = 0x3E;		
	RS485Send.Data[1] = 0x00;
	RS485Send.Data[2] = Motor_ID;
	RS485Send.Data[3] = 0x20;
	RS485Send.Data[4] = 0x00;
	RS485Send.Data[5] = 0;
	RS485Send.Data[6] = 0;
	
	CRC16_Modbus_Cal(&RS485Send,length-2);
	
	HAL_UART_Transmit(huart,RS485Send.Data,length,0xff);
	
	
}


/**
 * @description: DMR4315Send Tasks 
 * @param Motor_ID, huart
 * @return: void
 * @note: Set base point, command:0x21
 */ 
void DMR4315_basepointSet(uint8_t Motor_ID,UART_HandleTypeDef *huart)
{
	static RS485Send_Type RS485Send;	
	uint8_t length = len_basepointSet;

	RS485Send.Data[0] = 0x3E;		
	RS485Send.Data[1] = 0x00;
	RS485Send.Data[2] = Motor_ID;
	RS485Send.Data[3] = 0x21;
	RS485Send.Data[4] = 0x00;
	RS485Send.Data[5] = 0;
	RS485Send.Data[6] = 0;
	
	CRC16_Modbus_Cal(&RS485Send,length-2);
	
	HAL_UART_Transmit(huart,RS485Send.Data,length,0xff);
	
	
}


/**
 * @description: DMR4315Send Tasks 
 * @param Motor_ID, huart
 * @return: void
 * @note: Read Encoder real-time Data, command:0x2F
 */ 
void DMR4315_encoderDataRead(uint8_t Motor_ID,UART_HandleTypeDef *huart)
{
	static RS485Send_Type RS485Send;	
	uint8_t length = len_encoderDataRead;

	RS485Send.Data[0] = 0x3E;		
	RS485Send.Data[1] = 0x00;
	RS485Send.Data[2] = Motor_ID;
	RS485Send.Data[3] = 0x2F;
	RS485Send.Data[4] = 0x00;
	RS485Send.Data[5] = 0;
	RS485Send.Data[6] = 0;
	
	CRC16_Modbus_Cal(&RS485Send,length-2);
	
	HAL_UART_Transmit(huart,RS485Send.Data,length,0xff);
	
	
}


/**
 * @description: DMR4315Send Tasks 
 * @param Motor_ID, huart
 * @return: void
 * @note: Read DMR4315 real-time State, command:0x40
 */ 
void DMR4315_realtimeStateRead(uint8_t Motor_ID,UART_HandleTypeDef *huart)
{
	static RS485Send_Type RS485Send;	
	uint8_t length = len_realtimeStateRead;

	RS485Send.Data[0] = 0x3E;		
	RS485Send.Data[1] = 0x00;
	RS485Send.Data[2] = Motor_ID;
	RS485Send.Data[3] = 0x40;
	RS485Send.Data[4] = 0x00;
	RS485Send.Data[5] = 0;
	RS485Send.Data[6] = 0;
	
	CRC16_Modbus_Cal(&RS485Send,length-2);
	           
	HAL_UART_Transmit(huart,RS485Send.Data,length,0xff);
	
	
}


/**
 * @description: DMR4315Send Tasks 
 * @param Motor_ID, huart
 * @return: void
 * @note: Clear System Errors, command:0x41
 */ 
void DMR4315_errorClear(uint8_t Motor_ID,UART_HandleTypeDef *huart)
{
	static RS485Send_Type RS485Send;	
	uint8_t length = len_errorClear;

	RS485Send.Data[0] = 0x3E;		
	RS485Send.Data[1] = 0x00;
	RS485Send.Data[2] = Motor_ID;
	RS485Send.Data[3] = 0x41;
	RS485Send.Data[4] = 0x00;
	RS485Send.Data[5] = 0;
	RS485Send.Data[6] = 0;
	
	CRC16_Modbus_Cal(&RS485Send,length-2);
	
	HAL_UART_Transmit(huart,RS485Send.Data,length,0xff);
	
	
}



/**
 * @description: DMR4315Send Tasks 
 * @param Motor_ID, huart
 * @return: void
 * @note: Close Motor, command:0x50
 */ 
void DMR4315_motorClose(uint8_t Motor_ID,UART_HandleTypeDef *huart)
{
	static RS485Send_Type RS485Send;	
	uint8_t length = len_motorClose;

	RS485Send.Data[0] = 0x3E;		
	RS485Send.Data[1] = 0x00;
	RS485Send.Data[2] = Motor_ID;
	RS485Send.Data[3] = 0x50;
	RS485Send.Data[4] = 0x00;
	RS485Send.Data[5] = 0;
	RS485Send.Data[6] = 0;
	
	CRC16_Modbus_Cal(&RS485Send,length-2);
	
	HAL_UART_Transmit(huart,RS485Send.Data,length,0xff);
	
	
}



/**
 * @description: DMR4315Send Tasks 
 * @param Motor_ID, huart
 * @return: void
 * @note: Go back to base point according to Multiloop Encoder, command:0x51
 */ 
void DMR4315_TobasepointMulti(uint8_t Motor_ID,UART_HandleTypeDef *huart)
{
	static RS485Send_Type RS485Send;	
	uint8_t length = len_Tobasepoint;

	RS485Send.Data[0] = 0x3E;		
	RS485Send.Data[1] = 0x00;
	RS485Send.Data[2] = Motor_ID;
	RS485Send.Data[3] = 0x51;
	RS485Send.Data[4] = 0x00;
	RS485Send.Data[5] = 0;
	RS485Send.Data[6] = 0;
	
	CRC16_Modbus_Cal(&RS485Send,length-2);
	
	HAL_UART_Transmit(huart,RS485Send.Data,length,0xff);
	
	
}


/**
 * @description: DMR4315Send Tasks 
 * @param Motor_ID, huart
 * @return: void
 * @note: Go back to base point directly, rotation angle < 180, command:0x52
 */ 
void DMR4315_TobasepointDirect(uint8_t Motor_ID,UART_HandleTypeDef *huart)
{
	static RS485Send_Type RS485Send;	
	uint8_t length = len_Tobasepoint;

	RS485Send.Data[0] = 0x3E;		
	RS485Send.Data[1] = 0x00;
	RS485Send.Data[2] = Motor_ID;
	RS485Send.Data[3] = 0x52;
	RS485Send.Data[4] = 0x00;
	RS485Send.Data[5] = 0;
	RS485Send.Data[6] = 0;
	
	CRC16_Modbus_Cal(&RS485Send,length-2);
	
	HAL_UART_Transmit(huart,RS485Send.Data,length,0xff);
	
	
}



/**
 * @description: DMR4315Send Tasks 
 * @param Motor_ID, huart, Power：-32768~32767
 * @return: void
 * @note: Motor Open Loop Control, command:0x53
 */ 
void DMR4315_openloopControl(uint8_t Motor_ID,UART_HandleTypeDef *huart,int16_t Power)
{
	static RS485Send_Type RS485Send;	
	uint8_t length = len_openloopControl;

	RS485Send.Data[0] = 0x3E;		
	RS485Send.Data[1] = 0x00;
	RS485Send.Data[2] = Motor_ID;
	RS485Send.Data[3] = 0x53;
	RS485Send.Data[4] = 0x02;
	RS485Send.Data[5] = Power & 0xff;
	RS485Send.Data[6] = Power >> 8;
	RS485Send.Data[7] = 0;
	RS485Send.Data[8] = 0;
	
	CRC16_Modbus_Cal(&RS485Send,length-2);
	
	HAL_UART_Transmit(huart,RS485Send.Data,length,0xff);
	
	
}


/**
 * @description: DMR4315Send Tasks 
 * @param Motor_ID, huart, Target_Speed：-32768~32767
 * @return: void
 * @note: Motor Speed Closed Loop Control, command:0x54
 */ 
void DMR4315_speedClosedloopControl(uint8_t Motor_ID,UART_HandleTypeDef *huart,int16_t Target_Speed)
{
	static RS485Send_Type RS485Send;	
	uint8_t length = len_speedClosedloopControl;

	Target_Speed = Target_Speed/0.1f;
	
	RS485Send.Data[0] = 0x3E;		
	RS485Send.Data[1] = 0x00;
	RS485Send.Data[2] = Motor_ID;
	RS485Send.Data[3] = 0x54;
	RS485Send.Data[4] = 0x02;
	RS485Send.Data[5] = Target_Speed & 0xff;
	RS485Send.Data[6] = Target_Speed >> 8;
	RS485Send.Data[7] = 0;
	RS485Send.Data[8] = 0;
	
	CRC16_Modbus_Cal(&RS485Send,length-2);
	
	HAL_UART_Transmit(huart,RS485Send.Data,length,0xff);
	
	
}

/**
 * @description: DMR4315Send Tasks 
 * @param Motor_ID, huart, Target_Position：-32768~32767
 * @return: void
 * @note: Motor Absolute Position Closed Loop Control, command:0x55
 */ 
void DMR4315_positionClosedloopControlAbs(uint8_t Motor_ID,UART_HandleTypeDef *huart,uint32_t Target_Position)
{
	static RS485Send_Type RS485Send;	
	uint8_t length = len_positionClosedloopControlAbs;

	RS485Send.Data[0] = 0x3E;		
	RS485Send.Data[1] = 0x00;
	RS485Send.Data[2] = Motor_ID;
	RS485Send.Data[3] = 0x55;
	RS485Send.Data[4] = 0x04;
	RS485Send.Data[5] = Target_Position & 0xff;
	RS485Send.Data[6] = (Target_Position >> 8) & 0xff;
	RS485Send.Data[7] = (Target_Position >> 16) & 0xff;
	RS485Send.Data[8] = (Target_Position >> 24) & 0xff;
	RS485Send.Data[9] = 0;
	RS485Send.Data[10] = 0;
	
	CRC16_Modbus_Cal(&RS485Send,length-2);
	
	HAL_UART_Transmit(huart,RS485Send.Data,length,0xff);
	
	
}

/**
 * @description: DMR4315Send Tasks 
 * @param Motor_ID, huart, Count：-32768~32767
 * @return: void
 * @note: Motor Relative Position Closed Loop Control, Count：16384 to one circle, command:0x56
 */ 
void DMR4315_positionClosedloopControlRel(uint8_t Motor_ID,UART_HandleTypeDef *huart,int16_t Count)
{
	static RS485Send_Type RS485Send;	
	uint8_t length = len_positionClosedloopControlRel;

	RS485Send.Data[0] = 0x3E;		
	RS485Send.Data[1] = 0x00;
	RS485Send.Data[2] = Motor_ID;
	RS485Send.Data[3] = 0x56;
	RS485Send.Data[4] = 0x02;
	RS485Send.Data[5] = Count & 0xff;
	RS485Send.Data[6] = Count >> 8;
	RS485Send.Data[7] = 0;
	RS485Send.Data[8] = 0;
	
	CRC16_Modbus_Cal(&RS485Send,length-2);
	
	HAL_UART_Transmit(huart,RS485Send.Data,length,0xff);
	
	
}

/**
 * @description: DMR4315Send Tasks 
 * @param Motor_ID, huart
 * @return: void
 * @note: Read Speed under Position Closed Loop, flag:0x00, command:0x57
 */ 
void DMR4315_speedpositionClosedloopRead(uint8_t Motor_ID,UART_HandleTypeDef *huart)
{
	static RS485Send_Type RS485Send;	
	uint8_t length = len_speedpositionClosedloop;

	RS485Send.Data[0] = 0x3E;		
	RS485Send.Data[1] = 0x00;
	RS485Send.Data[2] = Motor_ID;
	RS485Send.Data[3] = 0x57;
	RS485Send.Data[4] = 0x03;
	RS485Send.Data[5] = 0x00;
	RS485Send.Data[6] = 0x00;						//unused
	RS485Send.Data[7] = 0x00;						//unused
	RS485Send.Data[8] = 0;
	RS485Send.Data[9] = 0;
	
	CRC16_Modbus_Cal(&RS485Send,length-2);
	
	HAL_UART_Transmit(huart,RS485Send.Data,length,0xff);
	
	
}

/**
 * @description: DMR4315Send Tasks 
 * @param Motor_ID, huart, Target_Speed
 * @return: void
 * @note: Set Speed under Position Closed Loop, flag:0x01, command:0x57
 */ 
void DMR4315_speedpositionClosedloopSet(uint8_t Motor_ID,UART_HandleTypeDef *huart,int16_t Target_Speed)
{
	static RS485Send_Type RS485Send;	
	uint8_t length = len_speedpositionClosedloop;
	Target_Speed = Target_Speed/0.1f;
	
	RS485Send.Data[0] = 0x3E;		
	RS485Send.Data[1] = 0x00;
	RS485Send.Data[2] = Motor_ID;
	RS485Send.Data[3] = 0x57;
	RS485Send.Data[4] = 0x03;
	RS485Send.Data[5] = 0x01;
	RS485Send.Data[6] = Target_Speed & 0xff;
	RS485Send.Data[7] = Target_Speed >> 8;
	RS485Send.Data[8] = 0;
	RS485Send.Data[9] = 0;
	
	CRC16_Modbus_Cal(&RS485Send,length-2);
	
	HAL_UART_Transmit(huart,RS485Send.Data,length,0xff);
	
	
}


/**
 * @description: DMR4315Receive Tasks 
 * @param mot, Data, length
 * @return: void
 * @note: Handle Received DMR4315 Information, command:0x0A
 */ 
void DMR4315_infoHandle(motor_t* mot, uint8_t *Data,uint8_t length)
{
	mot->FeedbackData.FrameCounter++;
	mot->type = (Data[6] << 8 | Data[5]);
}

/**
 * @description: DMR4315Receive Tasks 
 * @param mot, Data, length
 * @return: void
 * @note: Handle Received DMR4315 real-time Data, command:0x0B
 */ 
void DMR4315_realtimeDataHandle(motor_t* mot, uint8_t *Data,uint8_t length)
{
	int32_t temp_angle = 0;
	int16_t temp_cirnum = 0;
	
	mot->FeedbackData.FrameCounter++;
	//Update RealAngle
	mot->RealAngle = (Data[6] << 8 | Data[5])*360/16384;
	
	//Update RealCirnum
	temp_angle = (((Data[10] << 24 | Data[9] << 16) | Data[8] << 8) | Data[7])*360/16384;
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
	
	//Update RealSpeed
	mot->FeedbackData.RealSpeed = (int16_t)(((Data[12] << 8) | Data[11]))* 0.1;
	
	//Update Voltage
	mot->FeedbackData.Voltage = (uint8_t)(Data[13])* 0.2;
	
	//Update Current
	mot->FeedbackData.Current = (uint8_t)(Data[14])* 0.03;
	
	//Update Error_id
	mot->Error_id = Data[16];
	//Update Mode
	mot->Mode = Data[17];
	
}

/**
 * @description: DMR4315Receive Tasks 
 * @param mot, Data, length
 * @return: void
 * @note: Handle Received DMR4315 System Parameters, command:0x0C 0x0D 0x0E 0x0F
 */ 
void DMR4315_systemParaHandle(motor_t* mot, uint8_t *Data,uint8_t length)
{	
	uint8_t temp[4];
	
	mot->FeedbackData.FrameCounter++;
	
	Baud_rate = Data[8];
	
	//不能用float直接转换！！！
	temp[0] = Data[9];
	temp[1] = Data[10];
	temp[2] = Data[11];
	temp[3] = Data[12];
	memcpy(&PositionKp_Set, temp, 4);
	
	temp[0] = Data[13];
	temp[1] = Data[14];
	temp[2] = Data[15];
	temp[3] = Data[16];
	memcpy(&TargetSpeed_Set, temp, 4);
	TargetSpeed_Set = TargetSpeed_Set*0.1f;
	
	temp[0] = Data[17];
	temp[1] = Data[18];
	temp[2] = Data[19];
	temp[3] = Data[20];
	memcpy(&SpeedKp_Set, temp, 4);
	
	temp[0] = Data[21];
	temp[1] = Data[22];
	temp[2] = Data[23];
	temp[3] = Data[24];
	memcpy(&SpeedKi_Set, temp, 4);
	
	FilterCoefficient_Set = (uint8_t)Data[29]/100.0f;
	PowerPer_Set = Data[30];
	
}

/**
 * @description: DMR4315Receive Tasks 
 * @param mot, Data, length
 * @return: void
 * @note: Handle Received DMR4315 Base Point Data, command:0x21
 */ 
void DMR4315_basepointHandle(motor_t* mot, uint8_t *Data,uint8_t length)
{
	mot->FeedbackData.FrameCounter++;
	Basepoint_Flag = Data[7];
}

/**
 * @description: DMR4315Receive Tasks 
 * @param mot, Data, length
 * @return: void
 * @note: Handle Received DMR4315 Base Point Data, command:0x2F 0x50 0x51 0x52 0x53 0x54 0x55 0x56:
 */ 
void DMR4315_encoderDataHandle(motor_t* mot, uint8_t *Data,uint8_t length)
{
	float temp_angle = 0;
	int16_t temp_cirnum = 0;
	
	mot->FeedbackData.FrameCounter++;
	//Update RealAngle
	mot->RealAngle = (Data[6] << 8 | Data[5])*360.0f/16384.0f;
	
	//Update RealCirnum
	temp_angle = (((Data[10] << 24 | Data[9] << 16) | Data[8] << 8) | Data[7])*360.0f/16384.0f;
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
	
	//Update RealSpeed
	mot->FeedbackData.RealSpeed = ((int16_t)(Data[12] << 8 | Data[11])) * 0.1;
	//mot->FeedbackData.RealSpeed = ((int16_t)(Data[12] << 8 | Data[11]));
	mot->Realrotationrate = mot->FeedbackData.RealSpeed;
	
}

/**
 * @description: DMR4315Receive Tasks 
 * @param mot, Data, length
 * @return: void
 * @note: Handle Received DMR4315 Base Point Data, command:0x40 0x41
 */ 
void DMR4315_realtimeStateHandle(motor_t* mot, uint8_t *Data,uint8_t length)
{
	mot->FeedbackData.FrameCounter++;
	
	//Update Voltage
	mot->FeedbackData.Voltage = (uint8_t)(Data[5])* 0.2;
	
	//Update Current
	mot->FeedbackData.Current = (uint16_t)(Data[6])* 0.03;
	
	//Update Error_id
	mot->Error_id = Data[8];
	//Update Mode
	mot->Mode = Data[9];
	
}

/**
 * @description: DMR4315Receive Tasks 
 * @param mot, Data, length
 * @return: void
 * @note: Handle Received DMR4315 Base Point Data, command:0x57
 */ 
void DMR4315_speedpositionClosedloopHandle(motor_t* mot, uint8_t *Data,uint8_t length)
{
	mot->FeedbackData.FrameCounter++;
	TargetSpeed_Set = (Data[6] << 8 | Data[5]) * 0.1;
}


/**
 * @description: CRC16_Modbus Check Calculate
 * @param RS485Send.Data, length
 * @return: void
 * @note: Calculate CRC16_Modbus Check bits
 */ 
void CRC16_Modbus_Cal(RS485Send_Type *RS485Send,uint8_t length)
{
	unsigned short tmp = 0xFFFF;
	
	for(int n = 0; n < length; n++){
		tmp = RS485Send->Data[n] ^ tmp;
    for(int i = 0;i < 8;i++){
			if(tmp & 0x01){
				tmp = tmp >> 1;
				tmp = tmp ^ 0xA001;
			}   
			else{
				tmp = tmp >> 1;
			}   
		}   
	}
	RS485Send->Data[length] = tmp & 0xFF;
  RS485Send->Data[length + 1] = tmp >> 8;
		
}

/**
 * @description: CRC16_Modbus Check
 * @param RS485Send.Data
 * @return: bool
 * @note: Calculate CRC16_Modbus Check bits
 */ 
int8_t CRC16_Modbus_Check(uint8_t* Content, uint8_t length)
{
	unsigned short tmp = 0xFFFF;
	unsigned short CRC16_High, CRC16_Low;
	
	for(int n = 0; n <= length; n++){
		tmp = Content[n] ^ tmp;
		for(int i = 0;i < 8;i++){
			if(tmp & 0x01){
				tmp = tmp >> 1;
        tmp = tmp ^ 0xA001;
			}   
			else{
				tmp = tmp >> 1;
			}   
		}   
	}
	
	CRC16_Low = tmp & 0xFF;
	CRC16_High = tmp >> 8;
	
	if(Content[length + 1] != CRC16_Low)
		return 0;
	if(Content[length + 2] != CRC16_High)
		return 0;
	
	return 1;

}



 

