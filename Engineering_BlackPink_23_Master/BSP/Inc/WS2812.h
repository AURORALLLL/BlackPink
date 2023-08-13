#ifndef _WS2812_H
#define _WS2812_H
#endif
//标识的命名规则一般是头文件名全大写，前后加下划线，并把文件名中的“.”也变成下划线
//头文件区
#include "main.h"
#include "dma.h"
#include "tim.h"
 
 
//用户修改参数区
#define ONE_PULSE        (59)                           //1 码计数个数
#define ZERO_PULSE       (29)                           //0 码计数个数
#define RESET_PULSE      (85)                           //80 复位电平个数（不能低于40）
#define LED_NUMS         (7)                            //led 个数
#define LED_DATA_LEN     (24)                           //led 长度，单个需要24个字节
#define WS2812_DATA_LEN  (LED_NUMS*LED_DATA_LEN)        //ws2812灯条需要的数组长度
#define DMA_LEN  (LED_NUMS*LED_DATA_LEN+RESET_PULSE)    //DMA发送的数组长度
#define RGBVALUE         0x03

typedef struct
{
	uint8_t    Red;           //帧率计数
	uint8_t    Green;       			  //rpm
	uint8_t    Blue;

} RGB_UI_t;
 
 
void ws2812_set_RGB(uint8_t R, uint8_t G, uint8_t B, uint16_t num);//设置彩灯颜色
void ws2812_Set(void);
void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim);
void Following_Lamp(uint8_t R, uint8_t G, uint8_t B);
void RGB_Bling(uint8_t Num, uint8_t red, uint8_t green, uint8_t blue);

extern uint16_t  RGB_buffur[RESET_PULSE + WS2812_DATA_LEN];
extern RGB_UI_t RGB_Value[7];
