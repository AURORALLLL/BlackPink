#ifndef _MOTION_CHASSIS_H__
#define _MOTION_CHASSIS_H__

/**Include Header Files**/
#include "sysconfig.h"

#include "Task_StateMachine.h"


/**Macro Definition**/
#define CHASSIS_SPEED_MAX		10000


/**Struct Delaration**/
typedef struct
{
    int16_t Speed_X;								//������x�����ʻ�ٶȣ����ң�
    int16_t Speed_Y;								//������y�����ʻ�ٶȣ�ǰ��
    int16_t Speed_W;								//������w�����ת�ٶ�

    int16_t RC_X;										//�Խ��յ���ң����ҡ�����ݽ��д�������ֵ���������Ϊҡ�˵�ת���Ƕȳ˸�ϵ��
    int16_t RC_Y;
    int16_t RC_W;

}Chassis_t;

/**Enum Definition**/
typedef enum
{
	Spin_State_Static = 0,
	Spin_State_ClockWise,
	Spin_State_Follow,
	Spin_State_Counter,
} Spin_State_t;

/**Extern Declaration**/
extern Spin_State_t Spin_State;
extern int16_t Chassis_Follow_W;
extern int16_t Chassis_Offset;
extern uint8_t Auto_Chassis_Flag;

/**Function Delaration**/
void UpdataChassisState(void);


#endif

