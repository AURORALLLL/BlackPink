#ifndef __MOTOR_H__
#define __MOTOR_H__


/**Include Header Files**/
#include "sysconfig.h"
#include "PID.h"

#define MOTOR_MAX_BLOCKTIME	(10000)

/**Enum Definition**/
typedef enum
{
    MOTOR_NONE,
		MOTOR_2006,
    MOTOR_3508,
		MOTOR_4310,
		MOTOR_4315,
    MOTOR_6020,
    MOTOR_6623,
    MOTOR_9015,
		MOTOR_MF4005,
		MOTOR_MG4005,
    MOTOR_TYPE_COUNTER
}motor_type;


/**Struct Definition**/
typedef struct                            
{         
	uint8_t   Data[66];            
}RS485Send_Type;

typedef struct
{
	uint32_t    FrameCounter;           //帧率计数
	int16_t    	RealSpeed;       			  //rpm
	int16_t    	Current;
	int16_t			Voltage;
	uint16_t    Mechanical_Angle[2];    //电机返回机械角 [0]:当前 [1]:上一次
}Motor_Feedback_Data;


typedef enum
{
    noblock = 0,
    isblock,
    BlockState_Counter
}BlockState_t;

typedef struct
{
    BlockState_t BlockState;    //是否在堵转状态
    uint16_t Blocktime;         //单位ms
}BlockStruct_t;


typedef struct
{
	/* data */
	motor_type  type; 							//Motor_type
	uint32_t		id;									//Motor_ID
	uint8_t			Error_id;
	uint8_t			Mode;
	
	pid_type*   PositionPID;
	pid_type*   SpeedPID;
	Motor_Feedback_Data FeedbackData;
	
	uint8_t     Reductionratio; //减速比
	int16_t     Oricirnum;          //减速前转子转过圈数
	int16_t     Realcirnum;         //实际转过圈数
	int16_t     Targetcirnum;       //目标圈数

	int16_t     Realrotationrate;   //实际转速  rps
	float       Targetrotationrate; //目标转速  rps

	float   InitAngle;
	float   RealAngle;          //实际角度  单位°
	float   TargetAngle;        //目标角度  单位°
	int16_t TargetMechAngle;		//目标机械角度

	int16_t     	Out;                //PID计算后输出值
	
	BlockStruct_t Blockstate;       //电机堵转状态
	
	uint8_t First_Frame;				
}motor_t;


/**Function Delaration**/
void MotortypeRegister(motor_t * mot,motor_type type,pid_type* pospid,pid_type* speedpid,uint8_t Reductionratio, uint32_t Can_id);

#endif
