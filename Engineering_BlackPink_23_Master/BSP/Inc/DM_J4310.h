#ifndef __DM_J_4310_H__
#define __DM_J_4310_H__

/**Include Header Files**/
#include "sysconfig.h"
#include "Motor.h"

#include "Task_Init.h"

/**Macro Definition**/
#define DMJ_STDID														0x100

#define MOTOR4310_Reductionratio 						1				//DMJ4310¼õËÙ±È

#define P_MIN -12.5f
#define P_MAX 12.5f
#define V_MIN -30.0f
#define V_MAX 30.0f
#define KP_MIN 0.0f
#define KP_MAX 500.0f
#define KD_MIN 0.0f
#define KD_MAX 5.0f
#define T_MIN -10.0f
#define T_MAX 10.0f


/**Struct Definition**/
typedef struct
{
		int id;
		int state;
		int p_int;
		int v_int;
		int t_int;
		int kp_int;
		int kd_int;
		float pos;
		float vel;
		float toq;
		float Kp;
		float Kd;
		float Tmos;
		float Tcoil;
}Motor_Inf;



/**Function Delaration**/
//MCU to DMJ4310
void DMJ4310_motorOpen(uint8_t Motor_ID, uint8_t CAN_ID);
void DMJ4310_motorClose(uint8_t Motor_ID, uint8_t CAN_ID);
void DMJ4310_speedpositionControl(uint8_t Motor_ID, uint8_t CAN_ID, float _pos, float _vel);

//DMJ4310 to MCU
void DMJ4310_infoHandle(motor_t* mot, uint8_t *Data);

//convert type
int float_to_uint(float x, float x_min, float x_max, int bits);
float uint_to_float(int x_int, float x_min, float x_max, int bits);

#endif
