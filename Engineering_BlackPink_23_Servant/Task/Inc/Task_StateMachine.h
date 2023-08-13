#ifndef _TASK_STATEMACHINE_H__
#define _TASK_STATEMACHINE_H__


/**Include Header Files**/
#include "sysconfig.h"
#include "RemoteControl_Comm.h"

#include "Motion_Motor.h"
#include "Motion_Chassis.h"
#include "Motion_Pick.h"


/**Enum Definition**/
//State
typedef enum
{
    MainState_Debug = 0,				/*��״̬��������ģʽ*/
    MainState_Vision,
    MainState_Extend,
		MainState_Counter
} MainState_t;

typedef enum
{
    SubState_Protect = 0,				/*��״̬��������ģʽ*/
    SubState_Test,
    SubState_PC,

    SubState_Hand,
    SubState_Exchange,
    SubState_Rotate,

    SubState_Move,
    SubState_Gimbal,
    SubState_Mineral,

    SubState_Counter						/*�����壬�����������׼���״̬*/
} SubState_t;

typedef enum
{
    ControlMode_RC = 0,
    ControlMode_PC
} ControlMode_t;

typedef enum
{
    PCMode_Pick_Sky = 0,
		PCMode_Pick_Arm,
    PCMode_Exchange_Sky,
		PCMode_Exchange_Arm,
} PCMode_t;


//Key_Value
typedef enum
{
    Roller_Mid = 0,								/*���˾���*/
    Roller_Small_Front,
    Roller_Big_Front,							/*������ǰ*/
    Roller_Small_Back,
    Roller_Big_Back								/*�������*/
} Roller_State_t;

typedef enum
{
    Stick_Mid_V = 0,					/*ҡ�˾���*/
    Stick_Small_Front,				/*ҡ��С��Χǰҡ*/
    Stick_Big_Front,					/*ҡ�˴�Χǰҡ*/
    Stick_Small_Back,					/*ҡ��С��Χ��ҡ*/
    Stick_Big_Back,						/*ҡ�˴�Χ��ҡ*/
} Stick_Vert_t;

typedef enum
{
    Stick_Mid_H = 0,					    /*ҡ�˾���*/
    Stick_Small_Right,           	/*ҡ��С��Χ��ҡ*/
    Stick_Big_Right,							/*ҡ�˴�Χ��ҡ*/
    Stick_Small_Left,					    /*ҡ��С��Χ��ҡ*/
    Stick_Big_Left,						    /*ҡ�˴�Χ��ҡ*/
} Stick_Hori_t;

typedef enum
{
    UnPressed = 0,
    Pressed,
} Key_Value_t;



/**Extern Declaration**/
extern MainState_t MainState;
extern SubState_t SubState;
extern ControlMode_t ControlMode;
extern PCMode_t PCMode;
//ҡ����ֵ
extern float CH_LeftVert_Value;
extern float CH_LeftHori_Value;
extern float CH_RightVert_Value;
extern float CH_RightHori_Value;
extern float PC_X;
extern float PC_Y;
//Key_Value:Remote Control
extern Roller_State_t Roller_State[3];	//���ַ���
extern Stick_Vert_t StickL_Vert[3];			//��ҡ����ֱ����
extern Stick_Hori_t StickL_Hori[3];			//��ҡ��ˮƽ����
extern Stick_Vert_t StickR_Vert[3];			//��ҡ����ֱ����
extern Stick_Hori_t StickR_Hori[3];			//��ҡ��ˮƽ����
//Key_Value:Keyboard
extern Key_Value_t W[3];
extern Key_Value_t S[3];
extern Key_Value_t D[3];
extern Key_Value_t A[3];
extern Key_Value_t Q[3];
extern Key_Value_t E[3];
extern Key_Value_t R[3];
extern Key_Value_t F[3];
extern Key_Value_t G[3];
extern Key_Value_t Z[3];
extern Key_Value_t X[3];
extern Key_Value_t C[3];
extern Key_Value_t V[3];
extern Key_Value_t B[3];
extern Key_Value_t CTRL[3];
extern Key_Value_t SHIFT[3];
extern Key_Value_t Mouse_Left[3];
extern Key_Value_t Mouse_Right[3];
//����ٶ�
extern float Out_X;
extern float Out_Y;



/**Function Declaration**/
//Task
void Task_StateMachine(void * parameters);
//Init
void StateMachine_Init(void);
void StatePara_Init(void);
//StateMachine Update
void StateMachine_Update(void);
void MainState_Update(void);
void SubState_Update(void);
void ControlMode_Update(void);
void PCMode_Update(void);
//Key Value Update
void Key_Update(void);
void Switch_Update(void);
void Mouse_Speed_Filter(void);
void Roller_PC_Update(void);
void LEDUI_Update(void);

float KalmanFilter(int16_t inData);

//State Read
MainState_t GetMainState(void);
SubState_t GetSubState(void);
ControlMode_t GetControlMode(void);
PCMode_t GetPCMode(void);
#endif
