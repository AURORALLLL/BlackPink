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
	uint32_t    FrameCounter;           //֡�ʼ���
	int16_t    	RealSpeed;       			  //rpm
	int16_t    	Current;
	int16_t			Voltage;
	uint16_t    Mechanical_Angle[2];    //������ػ�е�� [0]:��ǰ [1]:��һ��
}Motor_Feedback_Data;


typedef enum
{
    noblock = 0,
    isblock,
    BlockState_Counter
}BlockState_t;

typedef struct
{
    BlockState_t BlockState;    //�Ƿ��ڶ�ת״̬
    uint16_t Blocktime;         //��λms
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
	
	uint8_t     Reductionratio; //���ٱ�
	int16_t     Oricirnum;          //����ǰת��ת��Ȧ��
	int16_t     Realcirnum;         //ʵ��ת��Ȧ��
	int16_t     Targetcirnum;       //Ŀ��Ȧ��

	int16_t     Realrotationrate;   //ʵ��ת��  rps
	float       Targetrotationrate; //Ŀ��ת��  rps

	float   InitAngle;
	float   RealAngle;          //ʵ�ʽǶ�  ��λ��
	float   TargetAngle;        //Ŀ��Ƕ�  ��λ��
	int16_t TargetMechAngle;		//Ŀ���е�Ƕ�

	int16_t     	Out;                //PID��������ֵ
	
	BlockStruct_t Blockstate;       //�����ת״̬
	
	uint8_t First_Frame;				
}motor_t;


/**Function Delaration**/
void MotortypeRegister(motor_t * mot,motor_type type,pid_type* pospid,pid_type* speedpid,uint8_t Reductionratio, uint32_t Can_id);

#endif
