#ifndef _CRC_CHECK_H__
#define _CRC_CHECK_H__

#include "sysconfig.h"

/*-------------------------------------------CRCУ��---------------------------------------------------*/
/**
* @brief  ����ϵͳ����У��
* @note
* @param  __RECEIVEBUFFER__��  ���յ��Ĳ���ϵͳ����ͷ֡���ڵ�ַ
* @param  __DATALENGTH__��     һ֡�����ڵ�������/Bytes�����ݣ�
* @retval 1��У����ȷ��0��У�����
*/
#define Verify_CRC_Check_Sum(__RECEIVEBUFFER__, __DATALENGTH__) (Verify_CRC8_Check_Sum(__RECEIVEBUFFER__, JUDGE_CRC8_OFFSET + 1) && Verify_CRC16_Check_Sum(__RECEIVEBUFFER__, __DATALENGTH__ + JUDGE_DATA_LENGTH(0)))

/**
* @brief  ����ϵͳ���У��
* @note
* @param  __TRANSMITBUFFER__�� ���͵�����ϵͳ��������ͷ֡���ڵ�ַ
* @param  __DATALENGTH__��     һ֡�����ڵ�������/Bytes�����ݣ�
* @retval None
  */
#define Append_CRC_Check_Sum(__TRANSMITBUFFER__, __DATALENGTH__)                       \
do                                                                                     \
{                                                                                      \
    Append_CRC8_Check_Sum(__TRANSMITBUFFER__, JUDGE_CRC8_OFFSET + 1);                  \
    Append_CRC16_Check_Sum(__TRANSMITBUFFER__, __DATALENGTH__ + JUDGE_DATA_LENGTH(0)); \
} while (0U)

/*--------------------------------------------------У�麯��--------------------------------------------------*/
unsigned char Get_CRC8_Check_Sum(unsigned char *pchMessage, unsigned int dwLength, unsigned char ucCRC8);
unsigned int Verify_CRC8_Check_Sum(unsigned char *pchMessage, unsigned int dwLength);
void Append_CRC8_Check_Sum(unsigned char *pchMessage, unsigned int dwLength);
uint16_t Get_CRC16_Check_Sum(uint8_t *pchMessage, uint32_t dwLength, uint16_t wCRC);
uint32_t Verify_CRC16_Check_Sum(uint8_t *pchMessage, uint32_t dwLength);
void Append_CRC16_Check_Sum(uint8_t *pchMessage, uint32_t dwLength);

#endif
