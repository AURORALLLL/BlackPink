#ifndef __RMD_9015_H_
#define __RMD_9015_H_


/** Include Header Files **/
#include "sysconfig.h"
#include "motor.h"

#include "Task_CAN.h"


/** Macro Definition **/
#define RMD_STDID 0x140
#define MOTOR9015_Reductionratio 						1				//RMD9015¼õËÙ±È

/** Function Declaration **/
void RMD_pidRead(uint8_t Motor_ID);
void RMD_pidWrite_RAM(uint8_t Motor_ID, uint8_t angle_Kp, uint8_t angle_Ki,
																				uint8_t speed_Kp, uint8_t speed_Ki,
																				uint8_t iq_Kp,    uint8_t iq_Ki    );	
void RMD_pidWrite_ROM(uint8_t Motor_ID, uint8_t angle_Kp, uint8_t angle_Ki,
																				uint8_t speed_Kp, uint8_t speed_Ki,
																				uint8_t iq_Kp,    uint8_t iq_Ki    );
void RMD_accelRead(uint8_t Motor_ID);
void RMD_accelWrite(uint8_t Motor_ID, int32_t Accel);
void RMD_encoderRead(uint8_t Motor_ID);
void RMD_encoderSet(uint8_t Motor_ID, uint16_t encoderOffset);
void RMD_MultiLoop_angleRead(uint8_t Motor_ID);
void RMD_SingleLoop_angleRead(uint8_t Motor_ID);
void RMD_stateRead_1(uint8_t Motor_ID);
void RMD_stateRead_2(uint8_t Motor_ID);
void RMD_stateRead_3(uint8_t Motor_ID);
void RMD_errorClear(uint8_t Motor_ID);
void RMD_Motor_off(uint8_t Motor_ID);
void RMD_Motor_stop(uint8_t Motor_ID);
void RMD_Motor_run(uint8_t Motor_ID);
void RMD_iqControl(uint8_t Motor_ID, int16_t iqControl);
void RMD_speedControl(uint8_t Motor_ID, int32_t speedControl);
void RMD_MultiLoop_angleControl(uint8_t Motor_ID, int32_t angleControl);
void RMD_MultiLoop_angleControl_limited(uint8_t Motor_ID, uint16_t maxSpeed, int32_t angleControl);
void RMD_SingleLoop_angleControl(uint8_t Motor_ID, uint8_t spinDir, uint16_t angleControl);
void RMD_SingleLoop_angleControl_limited(uint8_t Motor_ID, uint8_t spinDir, uint16_t maxSpeed, uint16_t angleControl);
void RMD_Increment_angleControl(uint8_t Motor_ID, int32_t angleIncrement);
void RMD_Increment_angleControl_limited(uint8_t Motor_ID, uint16_t maxSpeed, int32_t angleIncrement);

void RMDMotor_ParaHandle(motor_t* mot, uint8_t *Data);
void RMD_pidWrite_ROMHandle(motor_t* mot, uint8_t *Data);
void RMD_EncoderDataHandle(motor_t* mot, uint8_t *Data);
void RMD_basepointHandle(motor_t* mot, uint8_t *Data);
void RMD_motorClose(motor_t* mot, uint8_t *Data);
void RMD_MotorDataHandle(motor_t* mot, uint8_t *Data);
void RMD_MultiLoop_angleReadHandle(motor_t* mot, uint8_t *Data);
void RMD_pidReadHandle(motor_t* mot, uint8_t *Data);

#endif
