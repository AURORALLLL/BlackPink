/**Include Header Files**/
#include "Motion_Mineral.h"


/**Variable Definition**/
Mineral_State_t Mineral_State;

/**Function Declaration**/
/**
 * @brief  Update the Parameter of Rotate
 * @note
 * @param
 * @retval
 */
void MineralPara_Init(void)
{
    Mineral_State = Mineral_State_Static;
}


/**
 * @brief  Update the State of Rotate
 * @note	 Rotate Manual under PC Mode
 * @param
 * @retval
 */
void Mineral_State_Update(void)
{
    //判断转矿情况
    if(B[0] == Pressed && CTRL[0] == Pressed)
    {
        Mineral_State = Mineral_State_Lifting;
    }
    else if(B[0] == Pressed && Q[0] == Pressed)
    {
        Mineral_State = Mineral_State_Lowering;
    }
    else if(B[0] == Pressed && R[0] == Pressed)
    {
        Mineral_State = Mineral_State_Rotating_Forward;
    }
    else if(B[0] == Pressed && F[0] == Pressed)
    {
        Mineral_State = Mineral_State_Rotating_Back;
    }
    else if(B[0] == Pressed && G[0] == Pressed)
    {
        Mineral_State = Mineral_State_Rotating_Left;
    }
    else if(B[0] == Pressed && Z[0] == Pressed)
    {
        Mineral_State = Mineral_State_Rotating_Right;
    }
    else
    {
        Mineral_State = Mineral_State_Static;
    }

}


/**
 * @brief  转矿单独调试
 * @note
 * @param
 * @retval
 */
void Rotate_Debug(void)
{
    if(Roller_State[0] != Roller_Big_Front)
    {
        RotateB_Motor.Targetrotationrate = 400;
			  RotateF_Motor.Targetrotationrate = -400;
			
    }
    if(Roller_State[0] != Roller_Big_Back)
    {
        RotateB_Motor.Targetrotationrate = -400;
			  RotateF_Motor.Targetrotationrate = 400;
    }
}
