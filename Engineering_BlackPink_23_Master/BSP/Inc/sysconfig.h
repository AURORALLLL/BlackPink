#ifndef __SYSCONFIG__H_
#define __SYSCONFIG__H_

/**Include Header Files**/
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "main.h"
#include "stm32f4xx_hal.h"
#include "can.h"
#include "gpio.h"
#include "usart.h"
#include "tim.h"
//#include "i2c.h"
//#include "adc.h"

#include "cmsis_os.h"
#include "task.h"


/********** Private  Macro *********/
#define Toggle_LED_Green()  HAL_GPIO_TogglePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin)
#define TurnOn_LED_Green()  HAL_GPIO_WritePin(LED_GREEN_GPIO_Port,LED_GREEN_Pin,GPIO_PIN_RESET)
#define TurnOff_LED_Green()  HAL_GPIO_WritePin(LED_GREEN_GPIO_Port,LED_GREEN_Pin,GPIO_PIN_SET)

#define Toggle_LED_Red()    HAL_GPIO_TogglePin(LED_RED_GPIO_Port,LED_RED_Pin)
#define TurnOn_LED_Red()  HAL_GPIO_WritePin(LED_RED_GPIO_Port,LED_RED_Pin,GPIO_PIN_RESET)
#define TurnOff_LED_Red()  HAL_GPIO_WritePin(LED_RED_GPIO_Port,LED_RED_Pin,GPIO_PIN_SET)

/********** Mode Select **********/
#define DEBUG_MODE
//#define RELEASE_MODE

#ifdef DEBUG_MODE
#define MODE_SELECTED
#endif

#ifdef RELEASE_MODE
#define MODE_SELECTED
#endif

#ifndef MODE_SELECTED
#error Please select your current mode(Debug or Release)
#endif

/************* Uart buffer ***************/
//串口缓存区
#define MEMORY0 0
#define MEMORY1 1
#define MEMORYRESET 2

/********** Math limit **********/
#define LIMIT(data,min,max) (data = data > max ? max : (data < min ? min : data))

#endif
