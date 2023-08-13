#ifndef __MG4005_H__
#define __MG4005_H__

/**Include Header Files**/
#include "sysconfig.h"
#include "MG4005_Comm.h"
#include "Motor.h"

#define MOTORMG4005_Reductionratio 8
#define MOTORMF4005_Reductionratio 8

/**Extern Variables**/
extern motor_t MG4005;	


uint8_t calc_checksum(uint8_t const *p_data, int32_t data_len);
void MG4005_FrameHeader_Load(uint8_t motor_id,uint8_t data_len,uint8_t cmd);
void MG4005_Pid_read(uint8_t motor_id,UART_HandleTypeDef *huart);
void MG4005_Pid_WR2RAM(uint8_t motor_id,UART_HandleTypeDef *huart,uint8_t angleKp,uint8_t angleKi,uint8_t speedKp,uint8_t speedKi,uint8_t iqKp,uint8_t iqKi);
void MG4005_Pid_WR2ROM(uint8_t motor_id,UART_HandleTypeDef *huart,uint8_t angleKp,uint8_t angleKi,uint8_t speedKp,uint8_t speedKi,uint8_t iqKp,uint8_t iqKi);

void MG4005_Open(uint8_t motor_id,UART_HandleTypeDef *huart);
void MG4005_iqControl(uint8_t motor_id,UART_HandleTypeDef *huart,int16_t iqControl);
void MG4005_AngleControl(uint8_t motor_id,UART_HandleTypeDef *huart,int64_t angle);
void MG4005_MultiAngleMode2(uint8_t motor_id,UART_HandleTypeDef *huart,int64_t angle,uint32_t maxSpeed);
void MG4005_AllAngle_Read(uint8_t motor_id,UART_HandleTypeDef *huart);


/**接收处理函数**/
void MG4005_encoderDataHandle(motor_t* mot, uint8_t *Data,uint8_t length);
void MG4005_iqControlDataHandle(motor_t* mot, uint8_t *Data,uint8_t length);





#endif
