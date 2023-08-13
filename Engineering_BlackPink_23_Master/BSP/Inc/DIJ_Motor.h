#ifndef __DIJ_MOTOR_H__
#define __DIJ_MOTOR_H__

/**Include Header Files**/
#include "sysconfig.h"
#include "Motor.h"

#define MAXBLOCKTIME 500

/*3508，2006相关宏定义*/
#define MOTOR3508_Reductionratio 				19				//3508安装减速箱后近似减速比
#define MOTOR3508Lift_Reductionratio    27
#define M3508KIMAX											10000			//3508电机Ki环节最大值
#define M3508SPEEDMAX 									10000		  	//M3508速度限制
#define M3508OUTMAX 										15999 			//C620电调输出最大值

#define MOTOR2006_Reductionratio 				36				//2006安装减速箱后近似减速比
#define M2006KIMAX											5000			//2006电机Ki环节最大值
#define M2006OUTMAX 										9999 		  //C610电调输出最大值

/*6020相关宏定义*/
#define MOTOR6020_Reductionratio				1
#define M6020OutMax											29999

#define MechanicalAngle2RealAngle(Mechanical_Angle) (Mechanical_Angle / 8191.0f * 360.0f) //机械角（0~8191）->（0~360°）



/*所有电机id号宏定义*/
#define Non_id                  				0x000       //部分电机的CAN发送由从板完成，主板只进行输出值计算，因此这些电机不需要id
#define M3508_M2006_RX_OFFSETID					0x200 			//M3508	电机接收can的偏移id 
#define M3508_M2006_CH1_TX_OFFSETID	    0x200 			//M3508 M2006通道一发送can的偏移id
#define M3508_M2006_CH2_TX_OFFSETID     0x1FF       //M3508 M2006通道二发送can的偏移id

#define M6020_RX_OFFSETID								0x204				//M6020	电机接收can的偏移id 
#define M6020_CH1_TX_OFFSETID						0x1FF				//M6020	通道一发送can的偏移id
#define M6020_CH2_TX_OFFSETID						0x2FF				//M6020	通道二发送can的偏移id



/**Function Declaration**/
void DIJMotor_ParaHandle(motor_t* mot,uint8_t adata[]);
int8_t DIJMotor_AngleHandle(motor_t * mot);

#endif
