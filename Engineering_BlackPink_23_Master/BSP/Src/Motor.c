/**Include Header Files**/
#include "Motor.h"


/**Function Delaration**/
/**
 * @brief  电机类型注册函数
 * @note   初始化电机结构体
 * @param  mot: 电机结构体；type: 电机类型；pospid：位置环pid；speedpid：速度环pid；
 * @param  Reductionratio:减速比；Can_id：电机CAN通信id号
 * @retval 注册成功1 失败0
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
