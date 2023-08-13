#include "Task_Ui.h"
int Middle_Line_Width=260;
int Middle_Line_Hidth=400;
int Sky_Width = 60;
int Ladder_Down = 120;
int Ladder_Up = 40;



union
{
    UiFrame_t Frame;
    uint8_t UiFrameBuffer[128];
} UiFrame;
/* --------------------------   变量定义    ----------------------------------*/
static uint8_t send_seq; //包序号
uint8_t Ui_Cycle_Num = 0;


ext_student_interactive_header_data_t header_data;
robot_interactive_data_t robot_interactive_data;
ext_client_custom_character_t custom_character;
graphic_data_struct_t graphic_data[GRAPHIC_NUM];


void Task_Ui(void *parameters)
{
    InitPeripheral_UI();
    TickType_t xLastWakeUpTime;
    xLastWakeUpTime = xTaskGetTickCount();
    while (1)
    {
        FrameUpdate();
        vTaskDelayUntil(&xLastWakeUpTime, TASK_UI_INTERVAL);
    }
}
/* --------------------------   函数定义    ----------------------------------*/
/**
 * @brief  数据帧内容更新
 * @note   需要画什么，怎么画在这里面更新
 * @retval 0 不成功，此时不发送 1：更新成功
 */
uint8_t FrameUpdate(void)
{
    static uint8_t FirstDraw = 1; //判断是初始化UI界面还是更新UI界面
    //uint8_t *uname;               //uint8_t数组，必须三字节

    //循环创建
    if (Ui_Cycle_Num > 5)
    {
        Ui_Cycle_Num = 0;
        FirstDraw = 1;
    }
    else
    {
        Ui_Cycle_Num ++;
    }

    if (FirstDraw == 1)
    {
        /*第一次创建图形*/

        Init_Shape_Build(1);
        Lift_State_Show(1);
        Operate_Mode_Show(1);//1
		  	Block_Prevent_Show(1);
        send_graphic(); //以上七个图形，调用一次send_graphic()
        FirstDraw = 0;
    }

    else
    {
        Lift_State_Show(2);
        Operate_Mode_Show(2);
			  Block_Prevent_Show(2);
        send_graphic(); //以上七个图形，调用一次send_graphic()

    }

    if (header_data.sender_ID == 0 || header_data.receiver_ID == 0)
    {
        if (InitPeripheral_UI() == 0)
        {
            return 0;
        }
    }

    return 1;
}
/**
 * @brief 初始化UI
 * @note
 * @retval
 */
uint8_t InitPeripheral_UI(void)
{
    /*
    	|SOF1|data_length2|seq1|CRC81|
    	|frame_header-----5----------|cmd_id 2|data n|
    	|UiFrame_t(UiFrameBuffer[128])-----------|
      */
    UiFrame.UiFrameBuffer[0] = 0xA5;
    UiFrame.Frame.frame_header.Frame.seq = 0;               //包序号初始化
    UiFrame.Frame.frame_header.Frame.data_length.Frame = 0; //数据长度初始化
    UiFrame.Frame.frame_header.Frame.CRC8 = 0;              //CRC校验初始化
    UiFrame.UiFrameBuffer[5] = 0X01;
    UiFrame.UiFrameBuffer[6] = 0X03;                       //cmd_id设置为0x0301机器人间交互
    memset(UiFrame.Frame.data, 0, 121);                    //数据初始化
    if (ext_game_robot_state.robot_id == 2)
    {
        header_data.sender_ID = 2;//蓝工程
        header_data.receiver_ID = 0x0102;
    }
    else if (ext_game_robot_state.robot_id == 102)
    {
        header_data.sender_ID = 102;//红工程
        header_data.receiver_ID = 0x0166;
    }
    return 1;
}

void Init_Shape_Build(uint8_t mode)
{
    uint8_t *uname1;
    uname1=(unsigned char *)"IN1";               //uint8_t数组，必须三字节
    draw_rect(mode,1595,432,1825,648,uname1,2,6,COLOR_TEAM);
    uname1=(unsigned char *)"IN2";               //uint8_t数组，必须三字节
    draw_rect(mode,1624,450,1796,630,uname1,2,6,COLOR_TEAM);
    uname1=(unsigned char *)"IN3";               //uint8_t数组，必须三字节
    draw_rect(mode,1624,495,1796,585,uname1,2,6,COLOR_TEAM);
    uname1=(unsigned char *)"IN4";               //uint8_t数组，必须三字节
    draw_line(mode,1624,540,1796,540,uname1,2,6,COLOR_TEAM);
	  
    send_graphic();

		
		
	


}




void Lift_State_Show(uint8_t mode)
{

    static uint8_t *uname=(unsigned char *)"LS1";               //uint8_t数组，必须三字节
    
    if (Lift_State == Lift_State_Init)
        draw_rect(mode,1624,450,1796,495,uname,10,7,COLOR_GREEN);
    else if (Lift_State == Lift_State_Ground)
        draw_rect(mode,1624,495,1796,540,uname,10,7,COLOR_GREEN);
    else if (Lift_State == Lift_State_Silver)
        draw_rect(mode,1624,540,1796,585,uname,10,7,COLOR_GREEN);
    else if (Lift_State == Lift_State_Gold)
        draw_rect(mode,1624,585,1796,630,uname,10,7,COLOR_GREEN);
		
}







void Operate_Mode_Show(uint8_t mode)
{
	uint8_t *uname6;
	uint8_t *chars;
    
	if (PCMode==PCMode_Exchange)
		{
			chars=(uint8_t*)"Change";
			uname6=(unsigned char *)"OM1";
			write_chars(mode,1680,425,uname6,3,25,6,COLOR_AMARANTH,chars,6);
		}
    else if (PCMode==PCMode_Pick)
		{  
			chars=(uint8_t*)"Pick";
			uname6=(unsigned char *)"OM1";
			write_chars(mode,1650,425,uname6,3,25,6,COLOR_AMARANTH,chars,4);
		}

}



void Block_Prevent_Show(uint8_t mode)
{
	static uint8_t *uname=(unsigned char *)"BPS";               //uint8_t数组，必须三字节
    
    if (Auto_Block_Flag == 1)
        draw_circle(mode,1710,540,65,uname,14,4,COLOR_AMARANTH);
    else if (Auto_Block_Flag == 0)
         draw_circle(mode,2000,2000,4,uname,4,4,COLOR_AMARANTH);
		
		static uint8_t *uname1=(unsigned char *)"BP2";
		if (Auto_Protect == 1)
        draw_rect(mode,1595,432,1825,648,uname1,6,8,COLOR_YELLOW);
    else if (Auto_Protect == 0)
        draw_rect(mode,1595,432,1825,648,uname1,2,8,COLOR_TEAM);
		
		static uint8_t *uname2=(unsigned char *)"BP3";
		if (Auto_Sky_Flag == 1)
			draw_rect(mode,960-Sky_Width,0,960+Sky_Width,1080,uname2,2,8,COLOR_YELLOW);
		else if (Auto_Sky_Flag == 0)
		  draw_line(mode,960,0,960,1080,uname2,2,6,COLOR_YELLOW);
		uint8_t *uname6;
		uname1=(unsigned char *)"IN5";  
	  draw_line(mode,960+Ladder_Down,0,960+Ladder_Up,400,uname6,2,6,COLOR_WHITE);
	  uname1=(unsigned char *)"IN6";  
	  draw_line(mode,960-Ladder_Down,0,960-Ladder_Up,400,uname6,2,6,COLOR_WHITE);
		
   
}


/**
 * @brief 发送图形
 * @note
 * @retval 1:发送成功,0:没有发送
 */
uint8_t send_graphic(void)
{
    uint8_t graphic_num;
    graphic_num = check_empty_graphic();

    uint8_t data_size;
    if (graphic_num == 0)
    {
        //全是空的不要发
        return 0;
    }
    else if (graphic_num == 1)
    {
        header_data.data_cmd_id = 0X0101;                        //发送一个图形的内容id
        UiFrame.Frame.frame_header.Frame.data_length.Frame = 21; //发送一个图形的长度
    }
    else if (graphic_num == 2)
    {
        header_data.data_cmd_id = 0X0102;
        UiFrame.Frame.frame_header.Frame.data_length.Frame = 36;
    }
    else if (graphic_num > 2 && graphic_num <= 5)
    {
        header_data.data_cmd_id = 0X0103;
        UiFrame.Frame.frame_header.Frame.data_length.Frame = 81;
    }
    else if (graphic_num > 5 && graphic_num <= 7)
    {
        header_data.data_cmd_id = 0X0104;
        UiFrame.Frame.frame_header.Frame.data_length.Frame = 111;
    }
    else
    {
        header_data.data_cmd_id = 0X0104;
        UiFrame.Frame.frame_header.Frame.data_length.Frame = 111;
    }
    send_seq++;
    UiFrame.UiFrameBuffer[3] = send_seq;
    data_size = 15 * graphic_num;

    memcpy(UiFrame.UiFrameBuffer + JUDGE_DATA_OFFSET, &header_data, sizeof(ext_student_interactive_header_data_t));
    memcpy(UiFrame.UiFrameBuffer + JUDGE_DATA_OFFSET + sizeof(ext_student_interactive_header_data_t), graphic_data, data_size);

    Append_CRC_Check_Sum(UiFrame.UiFrameBuffer, UiFrame.UiFrameBuffer[1]);
    HAL_UART_Transmit(&REFEREE_HUART, UiFrame.UiFrameBuffer, (UiFrame.UiFrameBuffer[1] + 9), 0xffff); //头帧加校验共9字节

    //以下俩操作相当于清空数据（操作类型写成NULL就相当于不产生作用了）
    for (int i = 0; i < graphic_num; i++)
    {
        graphic_data[i].operate_tpye = OPERATE_NULL; //清空graphic_data[]的数据，便于下次再写入
    }
    for (int i = 0; i < graphic_num; i++)
    {
        UiFrame.UiFrameBuffer[16 + 15 * i] = OPERATE_NULL; //清空UiFrameBuffer[]的数据（如果下次紧跟着发送字符串的话，只会改写前45字节的内容，后面的图形会重复作用，所以也得清空）
    }

    return 1;
}
/**
 * @brief  判断当前哪个graphic为被使用
 * @note
 * @retval 当前第一个空的graphic
 */
uint8_t check_empty_graphic(void)
{
    for (int i = 0; i < GRAPHIC_NUM; i++)
    {
        if (graphic_data[i].operate_tpye == OPERATE_NULL)
        {
            return i;
        }
    }
    return GRAPHIC_NUM;
}
/**
 * @brief  画直线
 * @note   调用该函数画直线(我现在没检查坐标，因为我不知道多少是越界，用的时候自己注意一下)
 * @param  operate: 需要进行的操作类型，1-增加，2-修改
 * @param  start_x: 直线起始x坐标
 * @param  start_y: 直线起始y坐标
 * @param  end_x: 直线结束x坐标
 * @param  end_y: 直线结束y坐标
 * @param  name:  图形名字(必须3个uint8_t，不够记得补零，实在懒得写判断了....)
 * @param  width: 线宽
 * @param  layer: 图层
 * @param  color: 颜色
 * @retval 1:配置正确  0:所有graphic已经堆满  2:坐标错误 3:图层错误 4:颜色错误
 */
uint8_t draw_line(uint8_t operate, uint32_t start_x, uint32_t start_y, uint32_t end_x, uint32_t end_y, uint8_t name[],
                  uint32_t width, uint8_t layer, uint8_t color)
{
    uint8_t index;
    index = check_empty_graphic();

    if (index >= GRAPHIC_NUM)
    {
        return 0;
    }

    graphic_data[index].graphic_name[0] = name[0];
    graphic_data[index].graphic_name[1] = name[1];
    graphic_data[index].graphic_name[2] = name[2];

    switch (operate)
    {
    case 1:
    {
        graphic_data[index].operate_tpye = OPERATE_ADD;
        break;
    }
    case 2:
    {
        graphic_data[index].operate_tpye = OPERATE_CHANGE;
        break;
    }
    }

    graphic_data[index].width = width;
    graphic_data[index].graphic_tpye = GRAPHIC_LINE;

    graphic_data[index].start_x = start_x;
    graphic_data[index].start_y = start_y;
    graphic_data[index].end_x = end_x;
    graphic_data[index].end_y = end_y;

    //下面三个用不到的变量为空
    graphic_data[index].start_angle = 0;
    graphic_data[index].end_angle = 0;
    graphic_data[index].radius = 0;

    if (layer > LAYER_NUM)
    {
        return 3;
    }
    else
    {
        graphic_data[index].layer = layer;
    }

    if (color >= COLOR_NUM)
    {
        return 4;
    }
    else
    {
        graphic_data[index].color = color;
    }
    return 1;
}

/**
 * @brief  画矩形
 * @note   调用该函数画矩形(没检查坐标)
 * @param  operate: 需要进行的操作类型，1-增加，2-修改
 * @param  start_x: 起始x坐标
 * @param  start_y: 起始y坐标
 * @param  end_x: 结束x坐标
 * @param  end_y: 结束y坐标
 * @param  name:  图形名字(必须3个uint8_t，不够记得补零，实在懒得写判断了....)
 * @param  width: 线宽
 * @param  layer: 图层
 * @param  color: 颜色
 * @retval 1:配置正确  0:所有graphic已经堆满  2:坐标错误 3:图层错误 4:颜色错误
 */
uint8_t draw_rect(uint8_t operate, uint32_t start_x, uint32_t start_y, uint32_t end_x, uint32_t end_y, uint8_t name[],
                  uint32_t width, uint8_t layer, uint8_t color)
{
    uint8_t index;
    index = check_empty_graphic();

    if (index >= GRAPHIC_NUM)
    {
        return 0;
    }

    graphic_data[index].graphic_name[0] = name[0];
    graphic_data[index].graphic_name[1] = name[1];
    graphic_data[index].graphic_name[2] = name[2];

    switch (operate)
    {
    case 1:
    {
        graphic_data[index].operate_tpye = OPERATE_ADD;
        break;
    }
    case 2:
    {
        graphic_data[index].operate_tpye = OPERATE_CHANGE;
        break;
    }
    }

    graphic_data[index].width = width;
    graphic_data[index].graphic_tpye = GRAPHIC_RECT;

    graphic_data[index].start_x = start_x;
    graphic_data[index].start_y = start_y;
    graphic_data[index].end_x = end_x;
    graphic_data[index].end_y = end_y;

    //下面三个用不到的变量为空
    graphic_data[index].start_angle = 0;
    graphic_data[index].end_angle = 0;
    graphic_data[index].radius = 0;

    if (layer > LAYER_NUM)
    {
        return 3;
    }
    else
    {
        graphic_data[index].layer = layer;
    }

    if (color >= COLOR_NUM)
    {
        return 4;
    }
    else
    {
        graphic_data[index].color = color;
    }

    return 1;
}

/**
 * @brief  画圆
 * @note   调用该函数画圆(没检查坐标)
 * @param  operate: 需要进行的操作类型，1-增加，2-修改
 * @param  start_x: 圆心x坐标
 * @param  start_y: 圆心y坐标
 * @param  radius: 半径
 * @param  name:  图形名字(必须3个uint8_t，不够记得补零)
 * @param  width: 线宽
 * @param  layer: 图层
 * @param  color: 颜色
 * @retval 1:配置正确  0:所有graphic已经堆满  2:坐标错误 3:图层错误 4:颜色错误
 */
uint8_t draw_circle(uint8_t operate, uint32_t start_x, uint32_t start_y, uint32_t radius, uint8_t name[], uint32_t width, uint8_t layer, uint8_t color)
{
    uint8_t index;
    index = check_empty_graphic();

    if (index >= GRAPHIC_NUM)
    {
        return 0;
    }

    switch (operate)
    {
    case 1:
    {
        graphic_data[index].operate_tpye = OPERATE_ADD;
        break;
    }
    case 2:
    {
        graphic_data[index].operate_tpye = OPERATE_CHANGE;
        break;
    }
    }

    graphic_data[index].graphic_name[0] = name[0];
    graphic_data[index].graphic_name[1] = name[1];
    graphic_data[index].graphic_name[2] = name[2];

    graphic_data[index].width = width;
    graphic_data[index].graphic_tpye = GRAPHIC_CIRCLE;

    graphic_data[index].start_x = start_x;
    graphic_data[index].start_y = start_y;
    graphic_data[index].radius = radius;

    if (layer > LAYER_NUM)
    {
        return 3;
    }
    else
    {
        graphic_data[index].layer = layer;
    }

    if (color >= COLOR_NUM)
    {
        return 4;
    }
    else
    {
        graphic_data[index].color = color;
    }
    return 1;
}

/**
 * @brief  画椭圆
 * @note   调用该函数画椭圆(没检查坐标)
 * @param  operate: 需要进行的操作类型，1-增加，2-修改
 * @param  start_x: 圆心x坐标
 * @param  start_y: 圆心y坐标
 * @param  end_x: x半轴长
 * @param  end_y: y半轴长
 * @param  name:  图形名字(必须3个uint8_t，不够记得补零，实在懒得写判断了....)
 * @param  width: 线宽
 * @param  layer: 图层
 * @param  color: 颜色
 * @retval 1:配置正确  0:所有graphic已经堆满  2:坐标错误 3:图层错误 4:颜色错误
 */
uint8_t draw_ellipse(uint8_t operate, uint32_t start_x, uint32_t start_y, uint32_t end_x, uint32_t end_y, uint8_t name[], uint32_t width, uint8_t layer, uint8_t color)
{
    uint8_t index;
    index = check_empty_graphic();

    if (index >= GRAPHIC_NUM)
    {
        return 0;
    }

    switch (operate)
    {
    case 1:
    {
        graphic_data[index].operate_tpye = OPERATE_ADD;
        break;
    }
    case 2:
    {
        graphic_data[index].operate_tpye = OPERATE_CHANGE;
        break;
    }
    }

    graphic_data[index].graphic_name[0] = name[0];
    graphic_data[index].graphic_name[1] = name[1];
    graphic_data[index].graphic_name[2] = name[2];

    graphic_data[index].width = width;
    graphic_data[index].graphic_tpye = GRAPHIC_ELLIPSE;

    graphic_data[index].start_x = start_x;
    graphic_data[index].start_y = start_y;
    graphic_data[index].end_x = end_x;
    graphic_data[index].end_y = end_y;

    if (layer > LAYER_NUM)
    {
        return 3;
    }
    else
    {
        graphic_data[index].layer = layer;
    }

    if (color >= COLOR_NUM)
    {
        return 4;
    }
    else
    {
        graphic_data[index].color = color;
    }
    return 1;
}

/**
 * @brief  画弧
 * @note   调用该函数画弧(没检查坐标)
 * @param  operate: 需要进行的操作类型，1-增加，2-修改
 * @param  start_x: 圆心x坐标
 * @param  start_y: 圆心y坐标
 * @param  end_x: x半轴长
 * @param  end_y: y半轴长
 * @param  start_angle: 起始角度
 * @param  end_angle: 停止角度
 * @param  name:  图形名字(必须3个uint8_t，不够记得补零，实在懒得写判断了....)
 * @param  width: 线宽
 * @param  layer: 图层
 * @param  color: 颜色
 * @retval 1:配置正确  0:所有graphic已经堆满  2:坐标错误 3:图层错误 4:颜色错误
 */
uint8_t draw_arc(uint8_t operate, uint32_t start_x, uint32_t start_y, uint32_t end_x, uint32_t end_y, uint32_t start_angle, uint32_t end_angle, uint8_t name[], uint32_t width, uint8_t layer, uint8_t color)
{
    uint8_t index;
    index = check_empty_graphic();

    if (index >= GRAPHIC_NUM)
    {
        return 0;
    }

    switch (operate)
    {
    case 1:
    {
        graphic_data[index].operate_tpye = OPERATE_ADD;
        break;
    }
    case 2:
    {
        graphic_data[index].operate_tpye = OPERATE_CHANGE;
        break;
    }
    }

    graphic_data[index].graphic_name[0] = name[0];
    graphic_data[index].graphic_name[1] = name[1];
    graphic_data[index].graphic_name[2] = name[2];

    graphic_data[index].width = width;
    graphic_data[index].graphic_tpye = GRAPHIC_ARC;

    graphic_data[index].start_x = start_x;
    graphic_data[index].start_y = start_y;
    graphic_data[index].end_x = end_x;
    graphic_data[index].end_y = end_y;
    graphic_data[index].start_angle = start_angle;
    graphic_data[index].end_angle = end_angle;

    if (layer > LAYER_NUM)
    {
        return 3;
    }
    else
    {
        graphic_data[index].layer = layer;
    }

    if (color >= COLOR_NUM)
    {
        return 4;
    }
    else
    {
        graphic_data[index].color = color;
    }
    return 1;
}
/**
 * @brief  写整型数
 * @note   调用该函数写整型数(没检查坐标)
 * @param  start_x: 字符起始x坐标
 * @param  start_y: 字符起始y坐标
 * @param  name:  图形名字(必须3个uint8_t，不够记得补零，实在懒得写判断了....)
 * @param  width: 线宽
 * @param  size: 字体大小
 * @param  layer: 图层
 * @param  color: 颜色
 * @param  contents: 内容
 * @retval 0:所有graphic已经堆满 1:正常发送 2:坐标错误 3:图层错误 4:颜色错误
 */
uint8_t write_int(uint8_t operate, uint32_t start_x, uint32_t start_y, uint8_t name[], uint32_t width, uint32_t size, uint8_t layer,
                  uint8_t color, uint32_t contents)
{
    uint8_t index;
    index = check_empty_graphic();

    if (index >= GRAPHIC_NUM)
    {
        return 0;
    }

    graphic_data[index].graphic_name[0] = name[0];
    graphic_data[index].graphic_name[1] = name[1];
    graphic_data[index].graphic_name[2] = name[2];

    switch (operate)
    {
    case 1:
    {
        graphic_data[index].operate_tpye = OPERATE_ADD;
        break;
    }
    case 2:
    {
        graphic_data[index].operate_tpye = OPERATE_CHANGE;
        break;
    }
    }

    graphic_data[index].graphic_tpye = GRAPHIC_INT;
    graphic_data[index].width = width;
    graphic_data[index].start_angle = size;

    graphic_data[index].start_x = start_x;
    graphic_data[index].start_y = start_y;

    graphic_data[index].radius = contents;
    graphic_data[index].end_x = (contents >> 10);
    graphic_data[index].end_y = (contents >> 21);

    //用不到的变量为空
    graphic_data[index].end_angle = 0;

    if (layer > LAYER_NUM)
    {
        return 3;
    }
    else
    {
        graphic_data[index].layer = layer;
    }

    if (color >= COLOR_NUM)
    {
        return 4;
    }
    else
    {
        graphic_data[index].color = color;
    }

    return 1;
}

/**
 * @brief  写浮点数
 * @note   调用该函数写浮点数(没检查坐标)
 * @param  operate: 需要进行的操作类型，1-增加，2-修改
 * @param  start_x: 字符起始x坐标
 * @param  start_y: 字符起始y坐标
 * @param  name:  图形名字(必须3个uint8_t，不够记得补零，实在懒得写判断了....)
 * @param  width: 线宽
 * @param  size: 字体大小
 * @param  decimal: 小数有效位数
 * @param  layer: 图层
 * @param  color: 颜色
 * @param  contents: 内容
 * @retval 0:所有graphic已经堆满 1:正常发送 2:坐标错误 3:图层错误 4:颜色错误
 */
uint8_t write_float(uint8_t operate, uint32_t start_x, uint32_t start_y, uint8_t name[], uint32_t width, uint32_t size, uint32_t decimal,
                    uint8_t layer, uint8_t color, float contents)
{
    uint8_t index;
    index = check_empty_graphic();
    uint32_t integer;
    memcpy(&integer,&contents,4);

    if (index >= GRAPHIC_NUM)
    {
        return 0;
    }

    graphic_data[index].graphic_name[0] = name[0];
    graphic_data[index].graphic_name[1] = name[1];
    graphic_data[index].graphic_name[2] = name[2];

    switch (operate)
    {
    case 1:
    {
        graphic_data[index].operate_tpye = OPERATE_ADD;
        break;
    }
    case 2:
    {
        graphic_data[index].operate_tpye = OPERATE_CHANGE;
        break;
    }
    }

    graphic_data[index].graphic_tpye = GRAPHIC_FLOAT;
    graphic_data[index].width = width;
    graphic_data[index].start_angle = size;
    graphic_data[index].end_angle = decimal;

    graphic_data[index].start_x = start_x;
    graphic_data[index].start_y = start_y;

    graphic_data[index].radius = integer;
    graphic_data[index].end_x = (integer >> 10);
    graphic_data[index].end_y = (integer >> 21);

    if (layer > LAYER_NUM)
    {
        return 3;
    }
    else
    {
        graphic_data[index].layer = layer;
    }

    if (color >= COLOR_NUM)
    {
        return 4;
    }
    else
    {
        graphic_data[index].color = color;
    }

    return 1;
}

/**
 * @brief  写字符串
 * @note   调用该函数写字(没检查坐标)
           个人感觉字符串发送帧的内容空间配置和其他图形的发送不同，
           无法涵盖在send_graphic()函数里发送，所以里面单独进行发送，不需再调用send_graphic() by陈宇涵
 * @param  operate: 需要进行的操作类型，1-增加，2-修改
 * @param  start_x: 字符起始x坐标
 * @param  start_y: 字符起始y坐标
 * @param  name:  图形名字(必须3个uint8_t，不够记得补零，实在懒得写判断了....)
 * @param  width: 线宽
 * @param  size:  字体大小
 * @param  layer: 图层
 * @param  color: 颜色
 * @param  contents: 内容
 * @param  length: 长度
 * @retval 1:正常发送 2:坐标错误 3:图层错误 4:颜色错误
 */
uint8_t write_chars(uint8_t operate, uint32_t start_x, uint32_t start_y, uint8_t name[], uint32_t width, uint32_t size, uint8_t layer, uint8_t color, uint8_t contents[], uint32_t length)
{
    custom_character.graphic_data_struct.graphic_name[0] = name[0];
    custom_character.graphic_data_struct.graphic_name[1] = name[1];
    custom_character.graphic_data_struct.graphic_name[2] = name[2];

    switch (operate)
    {
    case 1:
    {
        custom_character.graphic_data_struct.operate_tpye = OPERATE_ADD;
        break;
    }
    case 2:
    {
        custom_character.graphic_data_struct.operate_tpye = OPERATE_CHANGE;
        break;
    }
    }

    custom_character.graphic_data_struct.graphic_tpye = GRAPHIC_CHAR;
    custom_character.graphic_data_struct.width = width;
    custom_character.graphic_data_struct.start_angle = size;
    custom_character.graphic_data_struct.end_angle = length;

    custom_character.graphic_data_struct.start_x = start_x;
    custom_character.graphic_data_struct.start_y = start_y;

    //下面三个用不到的变量为空
    custom_character.graphic_data_struct.radius = 0;
    custom_character.graphic_data_struct.end_x = 0;
    custom_character.graphic_data_struct.end_y = 0;

    if (layer > LAYER_NUM)
    {
        return 3;
    }
    else
    {
        custom_character.graphic_data_struct.layer = layer;
    }

    if (color >= COLOR_NUM)
    {
        return 4;
    }
    else
    {
        custom_character.graphic_data_struct.color = color;
    }

    header_data.data_cmd_id = 0x0110; //写字符的内容ID

    load_chars(contents, length);
    UiFrame.Frame.frame_header.Frame.data_length.Frame = 51; //ext_student_interactive_header_data + grapic_data_struct + data[30] = 6 + 15 + 30 = 51

    send_seq++;
    UiFrame.UiFrameBuffer[3] = send_seq;

    //进行数据发送，所以外部不需要调用send_graphic()
    memcpy(UiFrame.UiFrameBuffer + JUDGE_DATA_OFFSET, &header_data, sizeof(ext_student_interactive_header_data_t));
    memcpy(UiFrame.UiFrameBuffer + JUDGE_DATA_OFFSET + sizeof(ext_student_interactive_header_data_t), &custom_character, 45); //grapic_data_struct + data[30] = 15 + 30 = 45
    Append_CRC_Check_Sum(UiFrame.UiFrameBuffer, UiFrame.UiFrameBuffer[1]);
    HAL_UART_Transmit(&REFEREE_HUART, UiFrame.UiFrameBuffer, (UiFrame.UiFrameBuffer[1] + 9), 0xffff); //头帧加校验共9字节

    //以下操作相当于清空数据（操作类型写成NULL就相当于不产生作用了）
    for (int i = 0; i < 3; i++)
    {
        UiFrame.UiFrameBuffer[16 + 15 * i] = OPERATE_NULL; //清空UiFrameBuffer[]的数据（如果下次紧跟着发送少于三个的其他图形的话，不能将45字节的内容全部改写，多余的数据可能会导致奇怪的结果，所以也得清空）
    }
    return 1;
}
/**
 * @brief  向custom_character中装入字符串，需要给定字符串长度（不需要计算'\0'）
 * @note
 * @retval 正常录入 1
 */
uint8_t load_chars(uint8_t chars_to_send[], uint8_t length)
{
    uint8_t count = 0;

    for (count = 0; count < length; count++)
    {
        custom_character.data[count] = chars_to_send[count];
    }
    return 1;
}
