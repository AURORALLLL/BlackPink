/**Include Header Files**/
#include "Task_MotorControl.h"


/**Macro Definition**/
#define TASK_MOTORCONTROL_INTERVAL (1)


/**Variable Definition**/
uint8_t MotorCount = 0;

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
				start_time2 = xTaskGetTickCount();
			
				MotorCount++;
			
        if (Motor_Define == 1)//¼ÇµÃÐ´£¡
        {   
						Motor_Control();
					
						if(MotorCount%3 == 0)
						{
								MCU_Control_CMD();
								Uart_Send();
						}
					
            if(GetSubState() != SubState_Protect)
            {
								BottomMotor_CMD(LiftL_Motor.Out, LiftR_Motor.Out);
								ChassisMotor_CMD(ChassisMotor[0].Out, ChassisMotor[1].Out, ChassisMotor[2].Out, ChassisMotor[3].Out);
								//BottomMotor_CMD(0, 0);
								//ChassisMotor_CMD(0, 0, 0, 0);
							
            }
            else
            {			
								BottomMotor_CMD(0, 0);
								ChassisMotor_CMD(0, 0, 0, 0);
							
								Bottom_Pid_Clear();
								Chassis_Pid_Clear();
            }

        }
				control = xTaskGetTickCount() - start_time2;
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
		LiftMotor_Control();
		ChassisControl();
}
