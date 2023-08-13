#ifndef _MOTION_MINERAL_H__
#define _MOTION_MINERAL_H__


/**Include Header Files**/
#include "sysconfig.h"

#include "Task_StateMachine.h"

#include "Motion_Motor.h"


/**Macro Definition**/
//#define ROTATE_SPEED_MAX		1250 //转矿电机转速大小，实际使用时需要根据方向改变正负(手动遥控代码中使用)
//#define ROTATE_SPEED				1000


/**Enum Definition**/
typedef enum
{
    Mineral_State_Static = 0,
    Mineral_State_Lowering,
    Mineral_State_Lifting,
		Mineral_State_Rotating,
    Mineral_State_Rotating_Left,
		Mineral_State_Rotating_Right,
		Mineral_State_Rotating_Forward,
		Mineral_State_Rotating_Back,
}Mineral_State_t;


/**Extern Declaration**/
extern Mineral_State_t Mineral_State;

/**Function Delaration**/
void MineralPara_Init(void);
void Mineral_State_Update(void);
void Rotate_Debug(void);

#endif

