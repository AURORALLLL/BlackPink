#ifndef _REMOTE_CONTROL_H_
#define _REMOTE_CONTROL_H_

/* -------------------------------- Includes -------------------------------- */
#include "sysconfig.h"
/* ---------------------------- Defined constants --------------------------- */

#define RC_HUART huart1

#define RC_CH_MAX_RELATIVE 660.0f				//遥控器通道相对最大值

#define RC_FRAME_LEN        18U         //遥控器数据帧长
#define RC_FRAME_LEN_BACK   2U          //增加两个字节保持稳定

/* ----------------------- RC Channel Definition---------------------------- */
#define RC_CH_VALUE_MIN ((uint16_t)364 )
#define RC_CH_VALUE_OFFSET ((uint16_t)1024)
#define RC_CH_VALUE_MAX ((uint16_t)1684)
#define RC_CH0    ((uint8_t)0)
#define RC_CH1    ((uint8_t)1)
#define RC_CH2    ((uint8_t)2)
#define RC_CH3    ((uint8_t)3)
#define RC_CH4    ((uint8_t)4)
/* ----------------------- RC Switch Definition----------------------------- */
#define RC_SW_UP    ((uint16_t)1)
#define RC_SW_MID   ((uint16_t)3)
#define RC_SW_DOWN  ((uint16_t)2)
#define RC_SW_Right ((uint8_t)0)
#define RC_SW_Left  ((uint8_t)1)
/* ----------------------- PC Key Definition-------------------------------- */
#define KEY_PRESSED_OFFSET_W ((uint16_t)0x01<<0)//改成了uint16_t,原来是uint8_t
#define KEY_PRESSED_OFFSET_S ((uint16_t)0x01<<1)
#define KEY_PRESSED_OFFSET_A ((uint16_t)0x01<<2)
#define KEY_PRESSED_OFFSET_D ((uint16_t)0x01<<3)
#define KEY_PRESSED_OFFSET_SHIFT ((uint16_t)0x01<<4)
#define KEY_PRESSED_OFFSET_CTRL ((uint16_t)0x01<<5)
#define KEY_PRESSED_OFFSET_Q ((uint16_t)0x01<<6)
#define KEY_PRESSED_OFFSET_E ((uint16_t)0x01<<7)
#define KEY_PRESSED_OFFSET_R ((uint16_t)0x01<<8)
#define KEY_PRESSED_OFFSET_F ((uint16_t)0x01<<9)
#define KEY_PRESSED_OFFSET_G ((uint16_t)0x01<<10)
#define KEY_PRESSED_OFFSET_Z ((uint16_t)0x01<<11)
#define KEY_PRESSED_OFFSET_X ((uint16_t)0x01<<12)
#define KEY_PRESSED_OFFSET_C ((uint16_t)0x01<<13)
#define KEY_PRESSED_OFFSET_V ((uint16_t)0x01<<14)
#define KEY_PRESSED_OFFSET_B ((uint16_t)0x01<<15)


#define KEY_W         ((uint8_t)0) 
#define KEY_S         ((uint8_t)1)
#define KEY_A         ((uint8_t)2)
#define KEY_D         ((uint8_t)3)
#define KEY_SHIFT     ((uint8_t)4)
#define KEY_CTRL      ((uint8_t)5)
#define KEY_Q         ((uint8_t)6)
#define KEY_E         ((uint8_t)7)
#define KEY_R         ((uint8_t)8)
#define KEY_F         ((uint8_t)9)
#define KEY_G         ((uint8_t)10)
#define KEY_Z         ((uint8_t)11)
#define KEY_X         ((uint8_t)12)
#define KEY_C         ((uint8_t)13)
#define KEY_V         ((uint8_t)14)
#define KEY_B         ((uint8_t)15)
#define KEY_OFFSET    ((uint8_t)0)
/* ----------------------- PC Mouse Definition-------------------------------- */
#define MOUSE_X                 ((uint8_t)0)
#define MOUSE_Y                 ((uint8_t)1)
#define MOUSE_Z                 ((uint8_t)2)
#define MOUSE_SPEED_OFFSET      ((uint16_t)0)
#define MOUSE_LEFT              ((uint8_t)3)
#define MOUSE_RIGHT             ((uint8_t)4)
#define MOUSE_PRESSED_OFFSET    ((uint8_t)0)
/* ----------------------------- Macro functions ---------------------------- */

/* ------------------------------- Enum types ------------------------------- */
typedef enum 
{
    CH_RightHori,   //right horizon;	channel0
    CH_RightVert,   //right vertical;	channel1
    CH_LeftHori,   	//left  horizon;	channel2
    CH_LeftVert,   	//left  vertical;	channel3
		CH_Roll					//left wheel;			channel4
}channel_num;

typedef enum 
{
    Switch_Left,
    Switch_Right,
		swcounter
}rc_switch;

typedef enum
{
    sw_offset, //未赋值
    sw_up = 1,
    sw_down,
    sw_mid
}switch_state;

//鼠标键状态位序
typedef enum
{
  mouse_left = 0,
  mouse_right,
  mouse_counter
}mouse_key;
//鼠标轴位序
typedef enum
{
  mouse_axis_x = 0,
  mouse_axis_y,
  mouse_axis_z
}mouse_axis;

/* --------------------------------- Structs -------------------------------- */

/* ------------------------- Global variables extern ------------------------ */

/* ---------------------------- Global Functions ---------------------------- */

/**
 * @brief 初始化RC
 * @note  创建任务前调用
 * @retval
 */
void BSP_Init_RemoteControl(void);

 /**
  * @brief  中断服务函数
  * @note   在串口中断服务函数中调用，判断接收状态并重置dma
  * @retval None
  */
void RC_IRQHandler(UART_HandleTypeDef *huart);

 /**
  * @brief  获取遥控拨杆的值 百分比值
  * @note   CH_RightHori:右水平 CH_RightVert:右竖直 CH_LeftHori:左水平 CH_LeftVert:左竖直
  * @param  channel: enum channel_num( CH_RightHori,CH_RightVert,CH_LeftHori,CH_LeftVert)
  * @retval 对应通道数值百分比，-100% ~ 100%(右下正)
  */
float Get_Channel_Value(channel_num channel);

 /**
  * @brief  遥控拨码开关值
  * @note   Switch_Left 左 Switch_Right右
  * @param  sw: enum rc_switch(Switch_Left,Switch_Right)
  * @retval switch_state(sw_up:1,sw_down:2,sw_mid:3) 
  */
switch_state Get_Switch_Value(rc_switch sw);

 /**
  * @brief  获取键盘按键状态
  * @note   
  * @param  key: 需判断的按键状态
  * @retval pressed:0  UnPressed:1 
  */
uint8_t Get_KeyBoard_Value(uint16_t key);

 /**
  * @brief  获取键盘按住时间
  * @note   单位:ms  以及遥控器发送是14ms一帧
  * @param  key: 
  * @retval 0-255 按键对应按下时间 单位:ms
  */
uint16_t Get_KeyBoard_Presstime(uint16_t key);

 /**
  * @brief  获取鼠标按下状态
  * @note   
  * @param  key: 对应鼠标按键
  * @retval 
  */
uint8_t Get_Mouse_State(mouse_key key);
 /**
  * @brief  获取鼠标移动速度
  * @note   
  * @param  axis: 鼠标三轴速度
  * @retval 鼠标对应方向移动速度(单位忘了。。。)
  */
int16_t Get_Mouse_Speed(mouse_axis axis);

 /**
  * @brief  
  * @note   
  * @param  key: 
  * @retval 鼠标对应按键按压时间
  */
uint16_t Get_Mouse_Presstime(mouse_key key);

/**
 * @brief  获取所有键盘状态
 * @note   
 * @retval 键盘状态位向量
 */
uint16_t Get_Key_All(void);

uint8_t GetSwTrigger(rc_switch sw);
uint8_t CleanSwTrigger(rc_switch sw);

uint8_t GetMouseTrigger(mouse_key mouse);
uint8_t CleanMouseTrigger(mouse_key mouse);

uint8_t GetKeyTrigger(uint8_t key);
uint8_t CleanKeyTrigger(uint8_t key);

#endif
