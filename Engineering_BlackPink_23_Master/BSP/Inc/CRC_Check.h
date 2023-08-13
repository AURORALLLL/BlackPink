#ifndef _CRC_CHECK_H__
#define _CRC_CHECK_H__

#include "sysconfig.h"

/*-------------------------------------------CRC校验---------------------------------------------------*/
/**
* @brief  裁判系统数据校验
* @note
* @param  __RECEIVEBUFFER__：  接收到的裁判系统数据头帧所在地址
* @param  __DATALENGTH__：     一帧数据内的数据量/Bytes（内容）
* @retval 1：校验正确；0：校验错误
*/
#define Verify_CRC_Check_Sum(__RECEIVEBUFFER__, __DATALENGTH__) (Verify_CRC8_Check_Sum(__RECEIVEBUFFER__, JUDGE_CRC8_OFFSET + 1) && Verify_CRC16_Check_Sum(__RECEIVEBUFFER__, __DATALENGTH__ + JUDGE_DATA_LENGTH(0)))

/**
* @brief  裁判系统添加校验
* @note
* @param  __TRANSMITBUFFER__： 发送到裁判系统的数据中头帧所在地址
* @param  __DATALENGTH__：     一帧数据内的数据量/Bytes（内容）
* @retval None
  */
#define Append_CRC_Check_Sum(__TRANSMITBUFFER__, __DATALENGTH__)                       \
do                                                                                     \
{                                                                                      \
    Append_CRC8_Check_Sum(__TRANSMITBUFFER__, JUDGE_CRC8_OFFSET + 1);                  \
    Append_CRC16_Check_Sum(__TRANSMITBUFFER__, __DATALENGTH__ + JUDGE_DATA_LENGTH(0)); \
} while (0U)

/*--------------------------------------------------校验函数--------------------------------------------------*/
unsigned char Get_CRC8_Check_Sum(unsigned char *pchMessage, unsigned int dwLength, unsigned char ucCRC8);
unsigned int Verify_CRC8_Check_Sum(unsigned char *pchMessage, unsigned int dwLength);
void Append_CRC8_Check_Sum(unsigned char *pchMessage, unsigned int dwLength);
uint16_t Get_CRC16_Check_Sum(uint8_t *pchMessage, uint32_t dwLength, uint16_t wCRC);
uint32_t Verify_CRC16_Check_Sum(uint8_t *pchMessage, uint32_t dwLength);
void Append_CRC16_Check_Sum(uint8_t *pchMessage, uint32_t dwLength);

#endif
