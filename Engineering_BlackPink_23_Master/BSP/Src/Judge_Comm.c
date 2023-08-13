#include "Judge_Comm.h"

/*--------------------变量定义--------------------*/
uint8_t Judge_Receive_Buffer[130];

ext_game_state_t ext_game_state;
ext_game_result_t ext_game_result;
ext_game_robot_HP_t ext_game_robot_HP;
ext_dart_status_t ext_dart_status;
ext_event_data_t ext_event_data;
ext_supply_projectile_action_t ext_supply_projectile_action;
ext_referee_warning_t ext_referee_warning;
ext_dart_remaining_time_t ext_dart_remaining_time;
ext_game_robot_state_t ext_game_robot_state;
ext_power_heat_data_t ext_power_heat_data;
ext_game_robot_pos_t ext_game_robot_pos;
ext_buff_t ext_buff;
aerial_robot_energy_t aerial_robot_energy;
ext_robot_hurt_t ext_robot_hurt;
ext_shoot_data_t ext_shoot_data;
ext_bullet_remaining_t ext_bullet_remaining;
ext_rfid_status_t ext_rfid_status;
ext_dart_client_cmd_t ext_dart_client_cmd;
ext_video_transmitter_t ext_video_transmitter;

/*--------------------函数定义--------------------*/
/**
* @brief  裁判系统通信串口初始化
* @note   Task_Init时调用
* @param  huart：UART的句柄，使用USART3
* @retval None
*/
void BSP_Init_JudgeComm(void)
{
    //下面两个写法应该等效，取其一即可
    /*HAL_UART_Receive_DMA(&huart3, (uint8_t*)Judge_Receive_Buffer, REFEREE_DMA_SIZE);
    __HAL_UART_ENABLE_IT(&huart3, UART_IT_IDLE);*/

    SET_BIT(REFEREE_HUART.Instance->CR3, USART_CR3_DMAR);
    HAL_DMA_Start_IT(REFEREE_HUART.hdmarx, (uint32_t)&(REFEREE_HUART.Instance->DR), (uint32_t)Judge_Receive_Buffer, REFEREE_DMA_SIZE);
    __HAL_UART_ENABLE_IT(&REFEREE_HUART, UART_IT_IDLE);
}

/**
* @brief  USART的空闲中断
* @note	  USART3_IRQHandler()调用
* @param  huart：UART的句柄
* @retval None
*/
void Referee_IDLECallback(UART_HandleTypeDef *huart)
{
//	BaseType_t xHigherPriorityTaskToWaken = pdFALSE;
    uint8_t counter;

    //判断空闲中断
    if (__HAL_UART_GET_IT_SOURCE(huart, UART_IT_IDLE))
    {
        //清除空闲中断标志位
        __HAL_UART_CLEAR_IDLEFLAG(huart);
        //关闭DMA接收
        __HAL_DMA_DISABLE(huart->hdmarx);
        //记录接收到的字节数
        counter = REFEREE_DMA_SIZE - __HAL_DMA_GET_COUNTER(huart->hdmarx);

        //有接收到数据，而不是发送完成后的空闲！
        if (counter > 0)
        {
            //接收裁判系统数据
            RefereeReceive(counter);
        }
        //重新打开DMA接收
        __HAL_DMA_SET_COUNTER(huart->hdmarx, 130);
        __HAL_DMA_ENABLE(huart->hdmarx);
    }
}

/**
* @brief  裁判系统接收数据函数
* @note   具体通信协议详见<裁判系统串口协议附录>
* @param  JudgeReceive_Counter: 当前接收数据长度
* @retval None
*/
void RefereeReceive(uint8_t JudgeReceive_Counter)
{
    uint8_t Judge_SOF = 0;
    uint16_t DataLength, Judge_CmdID;
    //循环扫描头帧
    while (Judge_SOF < JudgeReceive_Counter)
    {
        if (Judge_Receive_Buffer[Judge_SOF] == 0XA5)
        {
            //这一帧数据的指令ID和数据长度
            Judge_CmdID = Judge_Receive_Buffer[JUDGE_CMDID_OFFSET + Judge_SOF] | (Judge_Receive_Buffer[JUDGE_CMDID_OFFSET + 1 + Judge_SOF] << 8);
            DataLength = Judge_Receive_Buffer[JUDGE_DATALENGTH_OFFSET + Judge_SOF] | (Judge_Receive_Buffer[JUDGE_DATALENGTH_OFFSET + 1 + Judge_SOF] << 8);
            //校验一帧数据
            if (Verify_CRC_Check_Sum(Judge_Receive_Buffer + Judge_SOF, DataLength))
            {
                //数据处理，读取
                Referee_Receive_Data_Processing(Judge_SOF, Judge_CmdID);
                //跳到下一帧数据的头帧
                Judge_SOF += JUDGE_DATA_LENGTH(DataLength);
            }
            else
            {
                Judge_SOF += JUDGE_DATA_LENGTH(0);
            }
        }
        else
        {
            Judge_SOF++;
        }
    }
}

/**
* @brief  裁判系统数据处理
* @note	  具体通信协议详见<裁判系统串口协议附录>
* @param  SOF：头帧偏移量；CmdID：数据帧的ID
* @retval None
*/
void Referee_Receive_Data_Processing(uint8_t SOF, uint16_t CmdID)
{
    switch (CmdID)
    {
        //1.	比赛状态(0x0001)	1Hz
    case GAME_STATE:
    {
        memcpy(&ext_game_state, (Judge_Receive_Buffer + JUDGE_DATA_OFFSET + SOF), GAME_STATE_DATA_SIZE);
        break;
    }
    //2.	比赛结果(0x0002)
    case GAME_RESULT:
    {
        memcpy(&ext_game_result, (Judge_Receive_Buffer + JUDGE_DATA_OFFSET + SOF), GAME_RESULT_DATA_SIZE);
        break;
    }
    //3.	比赛机器人血量数据（0x0003）
    case ROBOT_HP:
    {
        memcpy(&ext_game_robot_HP, (Judge_Receive_Buffer + JUDGE_DATA_OFFSET + SOF), ROBOT_HP_DATA_SIZE);
        break;
    }
    //4.    飞镖发射状态(0x0004)
    case DART_STATUS:
    {
        memcpy(&ext_dart_status, (Judge_Receive_Buffer + JUDGE_DATA_OFFSET + SOF), DART_STATUS_DATE_SIZE);
        break;
    }
    //6.	场地事件数据（0x0101）
    case EVENT_DATA:
    {
        memcpy(&ext_event_data, (Judge_Receive_Buffer + JUDGE_DATA_OFFSET + SOF), EVENTDATA_DATA_SIZE);
        break;
    }
    //7.	场地补给站动作标识数据（0x0102）
    case SUPPLY_ACTION:
    {
        memcpy(&ext_supply_projectile_action, (Judge_Receive_Buffer + JUDGE_DATA_OFFSET + SOF), SUPPLY_ACTION_DATA_SIZE);
        break;
    }
    //8.    裁判警告数据(0x0104)
    case REFEREE_WARNING:
    {
        memcpy(&ext_referee_warning, (Judge_Receive_Buffer + JUDGE_DATA_OFFSET + SOF), REFEREE_WARNING_DATA_SIZE);
        break;
    }
    //9.    飞镖发射口倒计时(0x0105)
    case DART_REMAINING_TIME:
    {
        memcpy(&ext_dart_remaining_time, (Judge_Receive_Buffer + JUDGE_DATA_OFFSET + SOF), DART_REMAINING_TIME_DATA_SIZE);
        break;
    }
    //10.	机器人状态数据（0x0201）
    case GAME_ROBOT_STATE:
    {
        memcpy(&ext_game_robot_state, (Judge_Receive_Buffer + JUDGE_DATA_OFFSET + SOF), GAMEROBOT_STATE_DATA_SIZE);
        break;
    }
    //11.	实时功率热量数据(0x0202)
    case POWER_HEAT_DATA:
    {
        memcpy(&ext_power_heat_data, (Judge_Receive_Buffer + JUDGE_DATA_OFFSET + SOF), POWER_HEAT_DATA_SIZE);
        break;
    }
    //12.机器人位置数据(0X0203)
    case GAME_ROBOT_POS:
    {
        memcpy(&ext_game_robot_pos, (Judge_Receive_Buffer + JUDGE_DATA_OFFSET + SOF), GAME_ROBOT_POS_DATA_SIZE);
        break;
    }
    //13.机器人增益数据(0X0204)
    case BUFF_MUSK:
    {
        memcpy(&ext_buff, (Judge_Receive_Buffer + JUDGE_DATA_OFFSET + SOF), BUFF_DATA_SIZE);
        break;
    }
    //14.空中机器人能量状态数据(0X0205)
    case AERIAL_ROBOT_ENERGY:
    {
        memcpy(&aerial_robot_energy, (Judge_Receive_Buffer + JUDGE_DATA_OFFSET + SOF), AERIAL_ROBOT_ENERGY_DATA_SIZE);
        break;
    }
    //15.伤害状态数据(0X0206)
    case ROBOT_HURT:
    {
        memcpy(&ext_robot_hurt, (Judge_Receive_Buffer + JUDGE_DATA_OFFSET + SOF), ROBOT_HURT_DATA_SIZE);
        break;
    }
    //16.实时射击数据(0X0207)
    case SHOOT_DATA:
    {
        memcpy(&ext_shoot_data, (Judge_Receive_Buffer + JUDGE_DATA_OFFSET + SOF), SHOOTDATA_DATA_SIZE);
        break;
    }
    //17.子弹剩余发送数(0X0208)
    case REMAIN_BULLET:
    {
        memcpy(&ext_bullet_remaining, (Judge_Receive_Buffer + JUDGE_DATA_OFFSET + SOF), REMAIN_BULLET_DATA_SIZE);
        break;
    }
    //18.机器人 RFID 状态(0X0209)
    case RFID_STATE:
    {
        memcpy(&ext_rfid_status, (Judge_Receive_Buffer + JUDGE_DATA_OFFSET + SOF), RFID_STATE_DATA_SIZE);
        break;
    }
    //19.飞镖机器人客户端指令数据(0X020A)
    case DART_CLIENT_CMD:
    {
        memcpy(&ext_dart_client_cmd, (Judge_Receive_Buffer + JUDGE_DATA_OFFSET + SOF), DART_CLIENT_CMD_DATA_SIZE);
        break;
    }
    //20.图传遥控信息数据(0x0304)
    case VIDEO_TRANSMITTER:
    {
        memcpy(&ext_video_transmitter, (Judge_Receive_Buffer + JUDGE_DATA_OFFSET + SOF), VIDEO_TRANSMITTER_DATA_SIZE);
        break;
    }
    }
}
