/**Include Header Files**/
#include "DIJ_Motor.h"

/**Function Declaration**/
/**
 * @brief  Handle DIJ Motor Parameter
 * @note   
 * @param  mot, adata[]
 * @retval 
 */
void DIJMotor_ParaHandle(motor_t* mot,uint8_t adata[])
{
	if(mot->type == MOTOR_3508 || mot->type == MOTOR_2006)
	{
		mot->FeedbackData.FrameCounter++;

		if(mot->First_Frame == 1)
			mot->FeedbackData.Mechanical_Angle[1] = mot->FeedbackData.Mechanical_Angle[0];
		mot->FeedbackData.Mechanical_Angle[0] = (uint16_t)((adata[0] << 8) | adata[1]);

		if(mot->First_Frame == 0)
		{
			mot->FeedbackData.Mechanical_Angle[1] = mot->FeedbackData.Mechanical_Angle[0];
			mot->First_Frame = 1;
		}
		mot->FeedbackData.RealSpeed = (int16_t)((adata[2] << 8) | adata[3]);
		mot->FeedbackData.Current = (int16_t)((adata[4] << 8) | adata[5]);
		mot->Realrotationrate = mot->FeedbackData.RealSpeed;

		DIJMotor_AngleHandle(mot);
	}
	else if(mot->type == MOTOR_6020)
	{
		mot->FeedbackData.FrameCounter++;
		
		if(mot->First_Frame == 1)
			mot->FeedbackData.Mechanical_Angle[1] = mot->FeedbackData.Mechanical_Angle[0];
		mot->FeedbackData.Mechanical_Angle[0] = (uint16_t)((adata[0] << 8) | adata[1]);

		if(mot->First_Frame == 0)
		{
			mot->FeedbackData.Mechanical_Angle[1] = mot->FeedbackData.Mechanical_Angle[0];
			mot->First_Frame = 1;
		}
		mot->FeedbackData.RealSpeed = (int16_t)((adata[2] << 8) | adata[3]);
		mot->FeedbackData.Current = (int16_t)((adata[4] << 8) | adata[5]);
		mot->Realrotationrate = mot->FeedbackData.RealSpeed;
		
		DIJMotor_AngleHandle(mot);
	}
	else
		return;
}

/**
 * @brief  更新电机的角度以及圈数
 * @note   由于减速箱的存在，电机转多圈对应输出轴转一圈，此函数用于计算输出轴转过的角度。仅适用于一圈脉冲数为8192的电机！
 * @param  mot: 对应电机结构体指针
 * @retval -1：反向过零；1：正向过零；0 ：无变化or错误
 */
int8_t DIJMotor_AngleHandle(motor_t * mot)
{
    if(mot->type == MOTOR_3508 || mot->type ==  MOTOR_2006)
    {
        //用机械角+速度判断过零的方式不靠谱，怀疑是两者回传不同步，会导致电机多算或少算机械角过零，导致角度解算出错。
        //经测试，3508电机输出转速9000（工程代码里面一般达不到这个速度，如果达到了就速度限幅）时，1000Hz电调回传速率下机械角每次变化1250左右，余量放到1500，来判断机械角正向/反向过零
        
		/*根据机械角Mechanical_Angle解算当前转子转过圈数Oricirnum*/
		if(mot->FeedbackData.Mechanical_Angle[0] > 6700 && mot->FeedbackData.Mechanical_Angle[1] < 1500)
        {
            mot->Oricirnum -= 1;
            return -1;
        }
        else if(mot->FeedbackData.Mechanical_Angle[0] < 1500 && mot->FeedbackData.Mechanical_Angle[1] > 6700)
        {
            mot->Oricirnum += 1;
            return 1;
        }
		/*根据Oricirnum更新Realcirnum 根据减速比的定义，Oricirnum等于Reductionratio（减速比）时，对应输出轴转过一圈*/
        while(mot->Oricirnum >= mot->Reductionratio)
        {
            mot->Oricirnum -= mot->Reductionratio;
            mot->Realcirnum += 1;
        }
        while (mot->Oricirnum < 0)
        {
            mot->Oricirnum += mot->Reductionratio;
            mot->Realcirnum -= 1;
        }
		
		/*根据机械角Mechanical_Angle和转子转过圈数Oricirnum解算目前电机角度，范围在0~360之间*/
        float tempAngle = ((float)(mot->Oricirnum) / (float)(mot->Reductionratio) * 360.0f) \
                          + MechanicalAngle2RealAngle(mot->FeedbackData.Mechanical_Angle[0]) / (float)(mot->Reductionratio);
        while (tempAngle >= 360)
        {
            tempAngle -= 360;
        }
        while (tempAngle < 0)
        {
            tempAngle += 360;
        }
        mot->RealAngle = tempAngle;
        return 0;
    }
	else if(mot->type == MOTOR_6020)
	{
		/*6020圈数解算，由于电机减速比为1，可以直接利用机械角Mechanical_Angle来判断输出轴圈数Realcirnum*/
		if(mot->FeedbackData.Mechanical_Angle[0] > 6700 && mot->FeedbackData.Mechanical_Angle[1] < 1500)
        {
            mot->Realcirnum -= 1;
            return -1;
        }
        else if(mot->FeedbackData.Mechanical_Angle[0] < 1500 && mot->FeedbackData.Mechanical_Angle[1] > 6700)
        {
            mot->Realcirnum += 1;
            return 1;
        }
		
		/*6020角度解算，范围是0~360*/
		float tempAngle = MechanicalAngle2RealAngle(mot->FeedbackData.Mechanical_Angle[0]) / (float)(mot->Reductionratio);
        while (tempAngle >= 360)
        {
            tempAngle -= 360;
        }
        while (tempAngle < 0)
        {
            tempAngle += 360;
        }
        mot->RealAngle = tempAngle;
		return 0;
	}
    else
    {		
        return 0;
    }
}
