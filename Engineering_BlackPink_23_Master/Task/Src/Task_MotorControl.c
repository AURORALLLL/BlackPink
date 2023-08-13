/**Include Header Files**/
#include "Task_MotorControl.h"


/**Macro Definition**/
#define TASK_MOTORCONTROL_INTERVAL (1)


/**Variable Definition**/
uint8_t MGMotorCount = 0;
uint8_t DMJMotorCount = 0;
uint8_t DMJ4310_OpenFlag = 0;
uint8_t AngleorIq_Flag = 0;

/**Function Declaration**/
/**
  * @brief  Task Motor Control
  * @param  unused
  * @retval void
  * @note   
  */
TickType_t start_time2;
TickType_t control;
void Task_MotorControl(void *parameters)
{
	
    TickType_t xLastWakeUpTime;
    xLastWakeUpTime = xTaskGetTickCount();
	
    while (1)
    {
				LEDUI_Update();
				//start_time2 = xTaskGetTickCount();
				if(MGMotorCount%3 == 0)
				{
						MCU_Control_CMD();
						Uart_Send();
						
				}
						
        if (Motor_Define == 1)//¼ÇµÃÐ´£¡
        {   
					  MGMotorCount++;
						DMJMotorCount++;
					
						Motor_Control();
						

            if(GetSubState() != SubState_Protect)
            {
								if(MGMotorCount%3 == 0)
								{
										if(!AngleorIq_Flag)
										{
												MG4005_MultiAngleMode2(ArmYaw_Motor_id,&MG4005_HUART, ArmYaw_Motor.Targetcirnum * 360.0f + ArmYaw_Motor.TargetAngle, 72000);
												DMR4315_openloopControl(Gimbal_Motor_id, &GIMBAL_HUART,Gimbal_Motor.Out);
											
												AngleorIq_Flag = 1;
										}
										else
										{
												MG4005_AllAngle_Read(ArmYaw_Motor_id,&MG4005_HUART);
												AngleorIq_Flag = 0;
										}
								}	

								if(DMJMotorCount%30 == 0)
								{
										DMJ4310_motorOpen(ArmPitchL_Motor_id, CANSEND_2);
										DMJ4310_motorOpen(ArmPitchR_Motor_id, CANSEND_2);
								}
								else
								{
//										DMJ4310_speedpositionControl(ArmPitchL_Motor_id, CANSEND_2, ArmPitchL_Motor.Targetcirnum * 360.0f + ArmPitchL_Motor.TargetAngle, 0);
//										DMJ4310_speedpositionControl(ArmPitchR_Motor_id, CANSEND_2, ArmPitchR_Motor.Targetcirnum * 360.0f + ArmPitchR_Motor.TargetAngle, 0);
										
										DMJ4310_speedpositionControl(ArmPitchL_Motor_id, CANSEND_2, ArmPitchL_Motor.Targetcirnum * 360.0f + ArmPitchL_Motor.TargetAngle, 3);
										DMJ4310_speedpositionControl(ArmPitchR_Motor_id, CANSEND_2, ArmPitchR_Motor.Targetcirnum * 360.0f + ArmPitchR_Motor.TargetAngle, 3);
								}
											
							
									PlatformMotor_CMD(PlatformL_Motor.Out, PlatformR_Motor.Out);
									SkyMotor_CMD(SkyPitchL_Motor.Out, SkyPitchR_Motor.Out);
									ArmMotor_CMD(SkyForwardL_Motor.Out, SkyForwardR_Motor.Out, ArmRoll_Motor.Out);								
            		
									//PlatformMotor_CMD(0, 0);
									//SkyMotor_CMD(0, 0);
									//ArmMotor_CMD(0, 0, 0);
								
									//ArmMotor_CMD(0, 0, ArmRoll_Motor.Out);			
							
						}
            else
            {		
								if(MGMotorCount%3 == 0)
								{		
										if(!AngleorIq_Flag)
										{
												MG4005_iqControl(ArmYaw_Motor_id,&MG4005_HUART,0);
												DMR4315_openloopControl(Gimbal_Motor_id, &GIMBAL_HUART,0);
											
												AngleorIq_Flag = 1;
										}
										else
										{
												MG4005_AllAngle_Read(ArmYaw_Motor_id,&MG4005_HUART);
												AngleorIq_Flag = 0;
										}
								}
								
								DMJ4310_motorClose(ArmPitchL_Motor_id, CANSEND_2);
								DMJ4310_motorClose(ArmPitchR_Motor_id, CANSEND_2);
								
								PlatformMotor_CMD(0, 0);
								SkyMotor_CMD(0, 0);
								ArmMotor_CMD(0, 0, 0);			
								
								Platform_Pid_Clear();
								Sky_Pid_Clear();
								Arm_Pid_Clear();
            }

        }
				//control = xTaskGetTickCount() - start_time2;
        vTaskDelayUntil(&xLastWakeUpTime, TASK_MOTORCONTROL_INTERVAL);
    }

}

/**
 * @brief  Calculate Motor Out
 * @note	 unused
 * @param
 * @retval
 */
void Motor_Control(void)
{
		PlatformMotor_Control();
		GimbalMotor_Control();
		SkyPitchMotor_Control();
		SkyForwardMotor_Control();
		ArmRollMotor_Control();
}
