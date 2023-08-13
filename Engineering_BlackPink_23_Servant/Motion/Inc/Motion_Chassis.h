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
    int16_t Speed_X;								//底盘沿x轴的行驶速度（左右）
    int16_t Speed_Y;								//底盘沿y轴的行驶速度（前后）
    int16_t Speed_W;								//底盘绕w轴的自转速度

    int16_t RC_X;										//对接收到的遥控器摇杆数据进行处理后的数值，可以理解为摇杆的转动角度乘个系数
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

