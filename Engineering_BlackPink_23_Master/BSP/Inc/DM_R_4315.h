#ifndef __DM_R_4315_H__
#define __DM_R_4315_H__


/**Include Header Files**/
#include "sysconfig.h"
#include "Motor.h"

/**Macro Definition**/
#define GIMBAL_HUART							huart8

#define len_infoRead						7
#define len_realtimeDataRead		7
#define len_systemParaRead			7
#define len_systemParaSet				33
#define len_factoryReset				7
#define len_encoderCalibrate		7
#define len_basepointSet				7
#define len_encoderDataRead			7
#define len_realtimeStateRead		7
#define len_errorClear					7
#define len_motorClose					7
#define len_Tobasepoint					7
#define len_openloopControl			9
#define len_speedClosedloopControl					9
#define len_positionClosedloopControlAbs		11
#define len_positionClosedloopControlRel		9
#define len_speedpositionClosedloop					10

#define MOTOR4315_Reductionratio 						1				//DMR4315¼õËÙ±È

/**Function Delaration**/
//MCU to DMR4315
void DMR4315_infoRead(uint8_t Motor_ID,UART_HandleTypeDef *huart);
void DMR4315_realtimeDataRead(uint8_t Motor_ID,UART_HandleTypeDef *huart);
void DMR4315_systemParaRead(uint8_t Motor_ID,UART_HandleTypeDef *huart);
void DMR4315_systemParaSetwithoutSave(uint8_t Motor_ID,UART_HandleTypeDef *huart,uint32_t Angle_Kp, uint32_t Target_Speed,uint32_t Speed_Kp, uint32_t Speed_Ki);
void DMR4315_systemParaSetwithSave(uint8_t Motor_ID,UART_HandleTypeDef *huart,uint32_t Angle_Kp, uint32_t Target_Speed,uint32_t Speed_Kp, uint32_t Speed_Ki);
void DMR4315_factoryReset(uint8_t Motor_ID,UART_HandleTypeDef *huart);
void DMR4315_encoderCalibrate(uint8_t Motor_ID,UART_HandleTypeDef *huart);
void DMR4315_basepointSet(uint8_t Motor_ID,UART_HandleTypeDef *huart);
void DMR4315_motorClose(uint8_t Motor_ID,UART_HandleTypeDef *huart);
void DMR4315_encoderDataRead(uint8_t Motor_ID,UART_HandleTypeDef *huart);
void DMR4315_realtimeStateRead(uint8_t Motor_ID,UART_HandleTypeDef *huart);
void DMR4315_errorClear(uint8_t Motor_ID,UART_HandleTypeDef *huart);
void DMR4315_TobasepointMulti(uint8_t Motor_ID,UART_HandleTypeDef *huart);
void DMR4315_TobasepointDirect(uint8_t Motor_ID,UART_HandleTypeDef *huart);
void DMR4315_openloopControl(uint8_t Motor_ID,UART_HandleTypeDef *huart,int16_t Power);
void DMR4315_speedClosedloopControl(uint8_t Motor_ID,UART_HandleTypeDef *huart,int16_t Target_Speed);
void DMR4315_positionClosedloopControlAbs(uint8_t Motor_ID,UART_HandleTypeDef *huart,uint32_t Target_Position);
void DMR4315_positionClosedloopControlRel(uint8_t Motor_ID,UART_HandleTypeDef *huart,int16_t Count);
void DMR4315_speedpositionClosedloopRead(uint8_t Motor_ID,UART_HandleTypeDef *huart);
void DMR4315_speedpositionClosedloopSet(uint8_t Motor_ID,UART_HandleTypeDef *huart,int16_t Target_Speed);

//DMR4315 to MCU
void DMR4315_infoHandle(motor_t* mot, uint8_t *Data,uint8_t length);
void DMR4315_realtimeDataHandle(motor_t* mot, uint8_t *Data,uint8_t length);
void DMR4315_systemParaHandle(motor_t* mot, uint8_t *Data,uint8_t length);
void DMR4315_basepointHandle(motor_t* mot, uint8_t *Data,uint8_t length);
void DMR4315_encoderDataHandle(motor_t* mot, uint8_t *Data,uint8_t length);
void DMR4315_realtimeStateHandle(motor_t* mot, uint8_t *Data,uint8_t length);
void DMR4315_speedpositionClosedloopHandle(motor_t* mot, uint8_t *Data,uint8_t length);

//CRC16 Check
void CRC16_Modbus_Cal(RS485Send_Type *RS485Send,uint8_t length);
int8_t CRC16_Modbus_Check(uint8_t* Content, uint8_t length);



#endif
