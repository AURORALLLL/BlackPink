/**Include Header Files**/
#include "Motor.h"


/**Function Delaration**/
/**
 * @brief  �������ע�ắ��
 * @note   ��ʼ������ṹ��
 * @param  mot: ����ṹ�壻type: ������ͣ�pospid��λ�û�pid��speedpid���ٶȻ�pid��
 * @param  Reductionratio:���ٱȣ�Can_id�����CANͨ��id��
 * @retval ע��ɹ�1 ʧ��0
 */
void MotortypeRegister(motor_t * mot,motor_type type,pid_type* pospid,pid_type* speedpid,uint8_t Reductionratio, uint32_t Can_id)
{
    if(mot != NULL)
    {
			mot->id = Can_id;
			mot->type = type;
			mot->Reductionratio = Reductionratio;
			mot->PositionPID = pospid;
			mot->SpeedPID = speedpid;
    }
    
}
