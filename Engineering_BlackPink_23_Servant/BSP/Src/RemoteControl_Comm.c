/* -------------------------------- Includes -------------------------------- */
#include "RemoteControl_Comm.h"
/* ----------------------------- Private Defines ---------------------------- */
#define RC_INTERVAL 14
/*------------------------ Private macro Function ----------------------------*/

/* ---------------------- Static Functions Reference ------------------------ */
static uint8_t GetRcIndex(void);

static uint8_t RcDataUpdate(void);

static void Updata_Press_time(void);

static void Updata_Trigger(void);
/* ---------------------------- Global Variables ---------------------------- */
uint8_t RCBuffer[2][RC_FRAME_LEN+RC_FRAME_LEN_BACK];
extern UART_HandleTypeDef huart3;
extern DMA_HandleTypeDef hdma_usart3_rx;
extern TaskHandle_t STATEMACHINE_TASK_Handle;

/* ---------------------------- Static Variables ---------------------------- */
typedef __packed struct
{
    uint8_t buffer_index; //当前使用的缓存区
    uint8_t offline_check;//离线计数，用于判断遥控是否离线

    int32_t framecounter; //用于计数包数

    uint8_t swtrigger[2]; //拨码开关切换状态
    uint8_t mousetrigger[2];//鼠标切换状态
    uint16_t mousePresstime[2];//鼠标按下时间

    uint16_t keyPresstime[16]; //键盘按键按下时间
    uint16_t keytrigger;   //键盘按键切换状态
    __packed struct
    {
        __packed struct
        {
            /* data */
            switch_state sw[2];
            uint16_t ch[5];
        } rc;

        __packed struct
        {
            /* data */
            int16_t x;
            int16_t y;
            int16_t z;
            uint8_t press_l;
            uint8_t press_r;
        } mouse;

        __packed struct
        {
            /* data */
            uint16_t key_bit;
        } keyboard;
    } buffer[2];

} RCDecoding_Type;

RCDecoding_Type Remote_controler;
/* ---------------------- Static Functions Declarations   --------------------- */
uint8_t GetRcIndex(void)
{
    return Remote_controler.buffer_index;
}

/**
 * @brief  遥控数据更新
 * @note   RC_IRQHandler中调用
 * @retval 更新成功返回1 失败返回0
 */
uint8_t RcDataUpdate(void)
{
    if(GetRcIndex() == MEMORYRESET)
    {
        Remote_controler.buffer[0].rc.sw[0] = sw_offset;
        Remote_controler.buffer[0].rc.sw[1] = sw_offset;

        Remote_controler.buffer[0].rc.ch[0] = RC_CH_VALUE_OFFSET;
        Remote_controler.buffer[0].rc.ch[1] = RC_CH_VALUE_OFFSET;
        Remote_controler.buffer[0].rc.ch[2] = RC_CH_VALUE_OFFSET;
        Remote_controler.buffer[0].rc.ch[3] = RC_CH_VALUE_OFFSET;
        Remote_controler.buffer[0].rc.ch[4] = RC_CH_VALUE_OFFSET;

        Remote_controler.buffer[0].mouse.x = MOUSE_SPEED_OFFSET;
        Remote_controler.buffer[0].mouse.y = MOUSE_SPEED_OFFSET;
        Remote_controler.buffer[0].mouse.z = MOUSE_SPEED_OFFSET;
        Remote_controler.buffer[0].mouse.press_l = MOUSE_PRESSED_OFFSET;
        Remote_controler.buffer[0].mouse.press_r = MOUSE_PRESSED_OFFSET;
        Remote_controler.buffer[0].keyboard.key_bit = KEY_OFFSET;

        Remote_controler.buffer[1] = Remote_controler.buffer[0];

        Remote_controler.mousePresstime[0] = Remote_controler.mousePresstime[1] = 0;
        Remote_controler.keyPresstime[0] = Remote_controler.keyPresstime[1] = Remote_controler.keyPresstime[2] = \
                                           Remote_controler.keyPresstime[3] = Remote_controler.keyPresstime[4] = Remote_controler.keyPresstime[5] = \
                                                   Remote_controler.keyPresstime[6] = Remote_controler.keyPresstime[7] = Remote_controler.keyPresstime[8] = \
                                                           Remote_controler.keyPresstime[9] = Remote_controler.keyPresstime[10] = Remote_controler.keyPresstime[11] = \
                                                                   Remote_controler.keyPresstime[12] = Remote_controler.keyPresstime[13] = Remote_controler.keyPresstime[14] = \
                                                                           Remote_controler.keyPresstime[15] = 0;
        Remote_controler.offline_check = 1;
        return 0;
    }
    else
    {
        Remote_controler.buffer[GetRcIndex()].rc.sw[0] = (switch_state)(((RCBuffer[0][5] >> 4) & 0x000C) >> 2); //!< Switch left
        Remote_controler.buffer[GetRcIndex()].rc.sw[1] = (switch_state)((RCBuffer[0][5] >> 4) & 0x0003);     //!< Switch right

        Remote_controler.buffer[GetRcIndex()].rc.ch[0] = ((RCBuffer[0][0] | (RCBuffer[0][1] << 8)) & 0x07ff);        //!< Channel 0
        Remote_controler.buffer[GetRcIndex()].rc.ch[1] = (((RCBuffer[0][1] >> 3) | (RCBuffer[0][2] << 5)) & 0x07ff); //!< Channel 1
        Remote_controler.buffer[GetRcIndex()].rc.ch[2] = (((RCBuffer[0][2] >> 6) | (RCBuffer[0][3] << 2) | (RCBuffer[0][4] << 10)) & 0x07ff);
        Remote_controler.buffer[GetRcIndex()].rc.ch[3] = (((RCBuffer[0][4] >> 1) | (RCBuffer[0][5] << 7)) & 0x07ff); //!< Channel 3
        Remote_controler.buffer[GetRcIndex()].rc.ch[4] = ((RCBuffer[0][16] | (RCBuffer[0][17] << 8)) & 0x07ff); //!< Channel 4

        Remote_controler.buffer[GetRcIndex()].mouse.x = RCBuffer[0][6] | (RCBuffer[0][7] << 8);   //!< Mouse X axis
        Remote_controler.buffer[GetRcIndex()].mouse.y = RCBuffer[0][8] | (RCBuffer[0][9] << 8);   //!< Mouse Y axis
        Remote_controler.buffer[GetRcIndex()].mouse.z = RCBuffer[0][10] | (RCBuffer[0][11] << 8); //!< Mouse Z axis
        Remote_controler.buffer[GetRcIndex()].mouse.press_l = RCBuffer[0][12];                    //!< Mouse Left Is Press ?
        Remote_controler.buffer[GetRcIndex()].mouse.press_r = RCBuffer[0][13];                    //!< Mouse Right Is Press ?
        Remote_controler.buffer[GetRcIndex()].keyboard.key_bit = RCBuffer[0][14] | (RCBuffer[0][15] << 8);//!< KeyBoard value

    }
    Updata_Press_time();
    Updata_Trigger();
    return 1;
}

/* ---------------------------- global functions ---------------------------- */

/**
 * @brief  中断服务函数
 * @note   在串口中断服务函数中调用，判断接收状态并重置dma
 * @retval None
 */
void RC_IRQHandler(UART_HandleTypeDef *huart)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;                   /*通知值为二进制信号量，用这个通知会使通知只进行一次*/

    static uint8_t this_time_rx_len = 0;                             /*本次接收长度*/
    if (__HAL_UART_GET_IT_SOURCE(&huart3, UART_IT_IDLE) != RESET)
    {
        /*清除空闲中断标志位*/
        (void)USART1->SR;
        (void)USART1->DR;

        __HAL_UART_CLEAR_IDLEFLAG(huart);

        /*关闭DMA接收*/
        __HAL_DMA_DISABLE(&hdma_usart3_rx);

        /*计算本次帧长度*/
        this_time_rx_len = (RC_FRAME_LEN+RC_FRAME_LEN_BACK) - __HAL_DMA_GET_COUNTER(&hdma_usart3_rx);

        /*记录本次的DMA内存*/
        if ((hdma_usart3_rx.Instance->CR & DMA_SxCR_CT) != RESET)
        {
            /* Current memory buffer used is Memory 1 */
            hdma_usart3_rx.Instance->CR &= (uint32_t)(~DMA_SxCR_CT);
            Remote_controler.buffer_index = MEMORY1;
        }
        else
        {
            /* Current memory buffer used is Memory 0 */
            hdma_usart3_rx.Instance->CR |= (uint32_t)DMA_SxCR_CT;
            Remote_controler.buffer_index = MEMORY0;
        }

        /*如果本次帧长度与RC帧长度不等，重启遥控接收内存*/
        if (this_time_rx_len != RC_FRAME_LEN)
            Remote_controler.buffer_index = MEMORYRESET;

        /*设定DMA发送的长度*/
        __HAL_DMA_SET_COUNTER(&hdma_usart3_rx, (RC_FRAME_LEN+RC_FRAME_LEN_BACK));

        /*重启DMA接收*/
        __HAL_DMA_ENABLE(&hdma_usart3_rx);

        RcDataUpdate();

        //发送任务通知给遥控任务
        vTaskNotifyGiveFromISR(STATEMACHINE_TASK_Handle,&xHigherPriorityTaskWoken);

        /*强制FreeRTOS任务切换*/
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
}
/**
 * @brief  获取遥控拨杆的值 百分比值
 * @note   CH_RightHori:右水平 CH_RightVert:右竖直 CH_LeftHori:左水平 CH_LeftVert:左竖直 CH_Roll:左侧拨轮
 * @param  channel: enum channel_num( CH_RightHori,CH_RightVert,CH_LeftHori,CH_LeftVert)
* @retval  对应通道数值百分比，-100% ~ 100%(右上正)
 */
float Get_Channel_Value(channel_num channel)
{
    switch (channel)
    {
    case CH_RightHori:
        if(abs(Remote_controler.buffer[GetRcIndex()].rc.ch[0] - RC_CH_VALUE_OFFSET) < 10)
            return 0;
        else
            return ((Remote_controler.buffer[GetRcIndex()].rc.ch[0] - RC_CH_VALUE_OFFSET)/660.0f);
    case CH_RightVert:
        if(abs((Remote_controler.buffer[GetRcIndex()].rc.ch[1] - RC_CH_VALUE_OFFSET)) < 10)
            return 0;
        else
            return ((Remote_controler.buffer[GetRcIndex()].rc.ch[1] - RC_CH_VALUE_OFFSET)/660.0f);
    case CH_LeftHori:
        if(abs((Remote_controler.buffer[GetRcIndex()].rc.ch[2] - RC_CH_VALUE_OFFSET)) < 10)
            return 0;
        else
            return ((Remote_controler.buffer[GetRcIndex()].rc.ch[2] - RC_CH_VALUE_OFFSET)/660.0f);
    case CH_LeftVert:
        if(abs((Remote_controler.buffer[GetRcIndex()].rc.ch[3] - RC_CH_VALUE_OFFSET)) < 10)
            return 0;
        else
            return ((Remote_controler.buffer[GetRcIndex()].rc.ch[3] - RC_CH_VALUE_OFFSET)/660.0f);
    case CH_Roll:
        if(abs((Remote_controler.buffer[GetRcIndex()].rc.ch[4] - RC_CH_VALUE_OFFSET)) < 10)
            return 0;
        else
            return ((Remote_controler.buffer[GetRcIndex()].rc.ch[4] - RC_CH_VALUE_OFFSET)/660.0f);
    default:
        return RC_CH_VALUE_OFFSET;
    }
}

/**
 * @brief  遥控拨码开关值
 * @note   Switch_Left 左 Switch_Right右
 * @param  sw: enum rc_switch(Switch_Left,Switch_Right)
 * @retval switch_state(sw_up:1,sw_down:2,sw_mid:3)
 */
switch_state Get_Switch_Value(rc_switch sw)
{
    switch (sw)
    {
    case Switch_Left:
        return Remote_controler.buffer[Remote_controler.buffer_index].rc.sw[0];
    case Switch_Right:
        return Remote_controler.buffer[Remote_controler.buffer_index].rc.sw[1];
    default:
        return sw_mid;
    }
}

/**
 * @brief  获取键盘按键状态
 * @note   返回值有待检验！！！
 * @param  key: 需判断的按键
 * @retval pressed:1  UnPressed:0
 */
uint8_t Get_KeyBoard_Value(uint16_t key)
{
    switch (key)
    {
    case KEY_W:
        return ((Remote_controler.buffer[GetRcIndex()].keyboard.key_bit & KEY_PRESSED_OFFSET_W)&&1);
    case KEY_S:
        return ((Remote_controler.buffer[GetRcIndex()].keyboard.key_bit & KEY_PRESSED_OFFSET_S)&&1);
    case KEY_A:
        return ((Remote_controler.buffer[GetRcIndex()].keyboard.key_bit & KEY_PRESSED_OFFSET_A)&&1);
    case KEY_D:
        return ((Remote_controler.buffer[GetRcIndex()].keyboard.key_bit & KEY_PRESSED_OFFSET_D)&&1);
    case KEY_SHIFT:
        return ((Remote_controler.buffer[GetRcIndex()].keyboard.key_bit & KEY_PRESSED_OFFSET_SHIFT)&&1);
    case KEY_CTRL:
        return ((Remote_controler.buffer[GetRcIndex()].keyboard.key_bit & KEY_PRESSED_OFFSET_CTRL)&&1);
    case KEY_Q:
        return ((Remote_controler.buffer[GetRcIndex()].keyboard.key_bit & KEY_PRESSED_OFFSET_Q)&&1);
    case KEY_E:
        return ((Remote_controler.buffer[GetRcIndex()].keyboard.key_bit & KEY_PRESSED_OFFSET_E)&&1);
    case KEY_R:
        return ((Remote_controler.buffer[GetRcIndex()].keyboard.key_bit & KEY_PRESSED_OFFSET_R)&&1);
    case KEY_F:
        return ((Remote_controler.buffer[GetRcIndex()].keyboard.key_bit & KEY_PRESSED_OFFSET_F)&&1);
    case KEY_G:
        return ((Remote_controler.buffer[GetRcIndex()].keyboard.key_bit & KEY_PRESSED_OFFSET_G)&&1);
    case KEY_Z:
        return ((Remote_controler.buffer[GetRcIndex()].keyboard.key_bit & KEY_PRESSED_OFFSET_Z)&&1);
    case KEY_X:
        return ((Remote_controler.buffer[GetRcIndex()].keyboard.key_bit & KEY_PRESSED_OFFSET_X)&&1);
    case KEY_C:
        return ((Remote_controler.buffer[GetRcIndex()].keyboard.key_bit & KEY_PRESSED_OFFSET_C)&&1);
    case KEY_V:
        return ((Remote_controler.buffer[GetRcIndex()].keyboard.key_bit & KEY_PRESSED_OFFSET_V)&&1);
    case KEY_B:
        return ((Remote_controler.buffer[GetRcIndex()].keyboard.key_bit & KEY_PRESSED_OFFSET_B)&&1);
    default:
        return KEY_OFFSET;
    }
}

/**
 * @brief  获取键盘按住时间
 * @note   单位:ms  以及遥控器发送是14ms一帧
 * @param  key:
 * @retval 0-255 按键对应按下时间 单位:ms
 */
uint16_t Get_KeyBoard_Presstime(uint16_t key)
{
//	if(Get_KeyBoard_Value(key)) Remote_controler.kPressTime++;
//	uint8_t kt=Remote_controler.kPressTime;
//	Remote_controler.kPressTime = 0;
//	return kt;
    return 0;
}

/**
 * @brief  获取鼠标按下状态
 * @note
 * @param  key: 对应鼠标按键
 * @retval
 */
uint8_t Get_Mouse_State(mouse_key key)
{
    switch (key)
    {
    case mouse_left:
        return Remote_controler.buffer[GetRcIndex()].mouse.press_l;
    case mouse_right:
        return Remote_controler.buffer[GetRcIndex()].mouse.press_r;
    default:
        return MOUSE_PRESSED_OFFSET;
    }
}

/**
 * @brief  获取鼠标移动速度
 * @note
 * @param  axis: 鼠标三轴速度
 * @retval 鼠标对应方向移动速度(单位忘了。。。)
 */
int16_t Get_Mouse_Speed(mouse_axis axis)
{
    switch (axis)
    {
    case mouse_axis_x:
        return Remote_controler.buffer[GetRcIndex()].mouse.x;
    case mouse_axis_y:
        return Remote_controler.buffer[GetRcIndex()].mouse.y;
    case mouse_axis_z:
        return Remote_controler.buffer[GetRcIndex()].mouse.z;
    default:
        break;
    }
    return 0;
}

/**
 * @brief
 * @note
 * @param  key:
 * @retval 鼠标对应按键按压时间
 */
uint16_t Get_Mouse_Presstime(mouse_key key)
{
    return 0;
}

/**
 * @brief  获取所有键盘状态
 * @note
 * @retval 键盘状态位向量
 */
uint16_t Get_Key_All(void)
{
    return 0;
}

uint8_t GetSwTrigger(rc_switch sw)
{
    if(sw >= swcounter)
    {
        return 0;
    }

    return Remote_controler.swtrigger[sw];
}

uint8_t CleanSwTrigger(rc_switch sw)
{
    if(sw >= swcounter)
    {
        return 0;
    }
    Remote_controler.swtrigger[sw] = 0;
    return 1;
}

uint8_t GetMouseTrigger(mouse_key mouse)
{
    if(mouse >= mouse_counter)
    {
        return 0;
    }
    return Remote_controler.mousetrigger[mouse];
}

uint8_t CleanMouseTrigger(mouse_key mouse)
{
    if(mouse >= mouse_counter)
    {
        return 0;
    }
    Remote_controler.mousetrigger[mouse] = 0;
    return 1;
}

uint8_t GetKeyTrigger(uint8_t key)
{
    if(key >= 16)
    {
        return 0;
    }
    uint8_t trigger = (Remote_controler.keytrigger >> key) & 0x01;
    return trigger;
}

uint8_t CleanKeyTrigger(uint8_t key)
{
    if(key >= 16)
    {
        return 0;
    }
    Remote_controler.keytrigger = Remote_controler.keytrigger & (~((uint16_t)0x01 << key));
    return 1;
}

/**
 * @brief 初始化RC
 * @note  创建任务前调用
 * @retval
 */
void BSP_Init_RemoteControl(void)
{
    Remote_controler.framecounter = 0;                 /*帧率计数*/
    Remote_controler.offline_check = 0;                /*离线计数*/
    Remote_controler.buffer_index = MEMORYRESET;       /*串口缓存区默认值*/
    SET_BIT(RC_HUART.Instance->CR3, USART_CR3_DMAR);
    HAL_DMAEx_MultiBufferStart(RC_HUART.hdmarx,(uint32_t)&(RC_HUART.Instance->DR),(uint32_t)&RCBuffer[0][0],(uint32_t)&RCBuffer[1][0],(RC_FRAME_LEN+RC_FRAME_LEN_BACK));
    __HAL_UART_ENABLE_IT(&RC_HUART,UART_IT_IDLE);
}
/* ----------------------- Static Function Definitions ---------------------- */
/**
 * @brief  更新键盘按压时间
 * @note   每次更新数据后执行
 * @retval none
 */
void Updata_Press_time(void)
{
    //鼠标按压时间更新
    if(Get_Mouse_State(mouse_left) == 1)
    {
        if(Remote_controler.mousePresstime[mouse_left] < 50000)
            Remote_controler.mousePresstime[mouse_left] += RC_INTERVAL;
    }
    else
    {
        Remote_controler.mousePresstime[mouse_left] = 0;
    }

    if(Get_Mouse_State(mouse_right) == 1)
    {
        if(Remote_controler.mousePresstime[mouse_right] < 50000)
            Remote_controler.mousePresstime[mouse_right] += RC_INTERVAL;
    }
    else
    {
        Remote_controler.mousePresstime[mouse_right] = 0;
    }
    //键盘按压时间更新
    for(int key = 0; key < 16; key++)
    {
        if(Get_KeyBoard_Value(key) == 1)
        {
            if(Remote_controler.keyPresstime[key] < 50000)
                Remote_controler.keyPresstime[key] += RC_INTERVAL;
        }
        else
        {
            Remote_controler.mousePresstime[key] = 0;
        }
    }

}


/**
 * @brief  更新切换状态
 * @note   如果拨杆或者按键状态发生切换，则标志位制1
 * @retval None
 */
void Updata_Trigger(void)
{
    if(Remote_controler.buffer[0].rc.sw[0] != Remote_controler.buffer[1].rc.sw[0])
    {
        Remote_controler.swtrigger[0] = 1;
    }
    if(Remote_controler.buffer[0].rc.sw[1] != Remote_controler.buffer[1].rc.sw[1])
    {
        Remote_controler.swtrigger[1] = 1;
    }


    if(Remote_controler.buffer[0].mouse.press_l != Remote_controler.buffer[1].mouse.press_l)
    {
        Remote_controler.mousetrigger[0] = 1;
    }
    if(Remote_controler.buffer[0].mouse.press_r != Remote_controler.buffer[1].mouse.press_r)
    {
        Remote_controler.mousetrigger[1] = 1;
    }
    Remote_controler.keytrigger = (Remote_controler.buffer[0].keyboard.key_bit) ^ (Remote_controler.buffer[1].keyboard.key_bit);
}
