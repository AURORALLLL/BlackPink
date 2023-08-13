#ifndef __ABSOLUTE_ENCODER_H__
#define __ABSOLUTE_ENCODER_H__
/** Include Header Files **/
#include "sysconfig.h"
#include "Task_CAN.h"

#include "Encoder.h"

/**Function Delaration**/
void Encoder_DataRead(uint8_t Encoder_ID, uint8_t Can_ID);
void Encoder_IDSet(uint8_t Encoder_ID_Old, uint8_t Can_ID, uint8_t Encoder_ID_New);
void Encoder_BaudRateSet(uint8_t Encoder_ID, uint8_t Can_ID);
void Encoder_ModeAASet(uint8_t Encoder_ID, uint8_t Can_ID);
void Encoder_Mode00Set(uint8_t Encoder_ID, uint8_t Can_ID);
void Encoder_ReturnTimeSet(uint8_t Encoder_ID, uint8_t Can_ID, uint16_t Return_Time);
void Encoder_BasePointSet(uint8_t Encoder_ID, uint8_t Can_ID);

void Encoder_Handle(Encoder_t * mot,uint8_t adata[]);
#endif
