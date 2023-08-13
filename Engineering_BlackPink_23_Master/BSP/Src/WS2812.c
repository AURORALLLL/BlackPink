#include "WS2812.h"
 
 
uint16_t  RGB_buffur[RESET_PULSE + WS2812_DATA_LEN] = { 0 };
uint8_t LED_State = 0;
uint16_t cnttt = 0;
RGB_UI_t RGB_Value[7];


void Following_Lamp(uint8_t R, uint8_t G, uint8_t B)
{
	static uint8_t FL_State = 0;
	static uint16_t FLCnt = 0;
	
	if(FL_State == 0)
	{
		
		for(int i = 0;i<=6;i++)
		{
			if(i>=FL_State&&i<=FL_State+2)
			{
				RGB_Value[i].Green = G;
		    RGB_Value[i].Red = R;
		    RGB_Value[i].Blue = B;
			}
			else
			{
				RGB_Value[i].Green = 0;
		    RGB_Value[i].Red = 0;
		    RGB_Value[i].Blue = 0;
			}
				
		}
		FLCnt++;
		if(FLCnt>50)
		{
			FL_State = 1;
			FLCnt = 0;
		}
	}
	if(FL_State == 1)
	{
		
		for(int i = 0;i<=6;i++)
		{
			if(i>=FL_State&&i<=FL_State+2)
			{
				RGB_Value[i].Green = G;
		    RGB_Value[i].Red = R;
		    RGB_Value[i].Blue = B;
			}
			else
			{
				RGB_Value[i].Green = 0;
		    RGB_Value[i].Red = 0;
		    RGB_Value[i].Blue = 0;
			}
				
		}
		FLCnt++;
		if(FLCnt>50)
		{
			FL_State = 2;
			FLCnt = 0;
		}
	}
	if(FL_State == 2)
	{
		
		for(int i = 0;i<=6;i++)
		{
			if(i>=FL_State&&i<=FL_State+2)
			{
				RGB_Value[i].Green = G;
		    RGB_Value[i].Red = R;
		    RGB_Value[i].Blue = B;
			}
			else
			{
				RGB_Value[i].Green = 0;
		    RGB_Value[i].Red = 0;
		    RGB_Value[i].Blue = 0;
			}
				
		}
		FLCnt++;
		if(FLCnt>50)
		{
			FL_State = 3;
			FLCnt = 0;
		}
	}
	if(FL_State == 3)
	{
		
		for(int i = 0;i<=6;i++)
		{
			if(i>=FL_State&&i<=FL_State+2)
			{
				RGB_Value[i].Green = G;
		    RGB_Value[i].Red = R;
		    RGB_Value[i].Blue = B;
			}
			else
			{
				RGB_Value[i].Green = 0;
		    RGB_Value[i].Red = 0;
		    RGB_Value[i].Blue = 0;
			}
				
		}
		FLCnt++;
		if(FLCnt>50)
		{
			FL_State = 4;
			FLCnt = 0;
		}
	}
	if(FL_State == 4)
	{
		
		for(int i = 0;i<=6;i++)
		{
			if(i>=FL_State&&i<=FL_State+2)
			{
				RGB_Value[i].Green = G;
		    RGB_Value[i].Red = R;
		    RGB_Value[i].Blue = B;
			}
			else
			{
				RGB_Value[i].Green = 0;
		    RGB_Value[i].Red = 0;
		    RGB_Value[i].Blue = 0;
			}
				
		}
		FLCnt++;
		if(FLCnt>50)
		{
			FL_State = 5;
			FLCnt = 0;
		}
	}
	if(FL_State == 5)
	{
		
		for(int i = 0;i<=6;i++)
		{
			if((i>=FL_State&&i<=FL_State+1)||i == 0)
			{
				RGB_Value[i].Green = G;
		    RGB_Value[i].Red = R;
		    RGB_Value[i].Blue = B;
			}
			else
			{
				RGB_Value[i].Green = 0;
		    RGB_Value[i].Red = 0;
		    RGB_Value[i].Blue = 0;
			}
				
		}
		FLCnt++;
		if(FLCnt>50)
		{
			FL_State = 6;
			FLCnt = 0;
		}
	}
		if(FL_State == 6)
	{
		
		for(int i = 0;i<=6;i++)
		{
			if((i>=FL_State&&i<=FL_State+1)||i == 0||i==1)
			{
				RGB_Value[i].Green = G;
		    RGB_Value[i].Red = R;
		    RGB_Value[i].Blue = B;
			}
			else
			{
				RGB_Value[i].Green = 0;
		    RGB_Value[i].Red = 0;
		    RGB_Value[i].Blue = 0;
			}
				
		}
		FLCnt++;
		if(FLCnt>50)
		{
			FL_State = 0;
			FLCnt = 0;
		}
	}
	
}

 
 


void RGB_Bling(uint8_t Num, uint8_t red, uint8_t green, uint8_t blue)
{
   if(Num<=6&&Num>=0)
	 {
		 static uint8_t Cnt = 0;
		 static uint8_t BlingState = 0;
		 if(BlingState ==0)
		 {
		 RGB_Value[Num].Red = red;
		 RGB_Value[Num].Blue = blue;
		 RGB_Value[Num].Green = green;
			 Cnt++;
			 if(Cnt >100)
			 {
				 Cnt = 0;
				 BlingState = 1;
			 }
		 }
		 
		  else if(BlingState ==1)
		 {
		 RGB_Value[Num].Red = 0;
		 RGB_Value[Num].Blue = 0;
		 RGB_Value[Num].Green = 0;
			 Cnt++;
			 if(Cnt >100)
			 {
				 Cnt = 0;
				 BlingState = 0;
			 }
		 }
		 
		 
	 }
}


void ws2812_set_RGB(uint8_t R, uint8_t G, uint8_t B, uint16_t num)
{
    //指针偏移:需要跳过复位信号的N个0
    uint16_t* p = (RGB_buffur + RESET_PULSE) + (num * LED_DATA_LEN);
    
    for (uint16_t i = 0;i < 8;i++)
    {
        //填充数组
        p[i]      = (G << i) & (0x80)?ONE_PULSE:ZERO_PULSE;
        p[i + 8]  = (R << i) & (0x80)?ONE_PULSE:ZERO_PULSE;
        p[i + 16] = (B << i) & (0x80)?ONE_PULSE:ZERO_PULSE;
    }
 
}
 
void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim)
{
//    HAL_TIM_PWM_Stop_DMA(&htim4,TIM_CHANNEL_4);
//    HAL_TIM_PWM_Stop_DMA(&htim1,TIM_CHANNEL_1);
	HAL_TIM_PWM_Stop_DMA(&htim8,TIM_CHANNEL_1);//PA8
}
 


void ws2812_Set(void)
{
    //#1.填充数组
    ws2812_set_RGB(RGB_Value[0].Red, RGB_Value[0].Green, RGB_Value[0].Blue, 0);
	  ws2812_set_RGB(RGB_Value[1].Red, RGB_Value[1].Green, RGB_Value[1].Blue, 1);
	  ws2812_set_RGB(RGB_Value[2].Red, RGB_Value[2].Green, RGB_Value[2].Blue, 2);
	  ws2812_set_RGB(RGB_Value[3].Red, RGB_Value[3].Green, RGB_Value[3].Blue, 3);
	  ws2812_set_RGB(RGB_Value[4].Red, RGB_Value[4].Green, RGB_Value[4].Blue, 4);
	  ws2812_set_RGB(RGB_Value[5].Red, RGB_Value[5].Green, RGB_Value[5].Blue, 5);
	  ws2812_set_RGB(RGB_Value[6].Red, RGB_Value[6].Green, RGB_Value[6].Blue, 6);
	    
	  HAL_TIM_PWM_Start_DMA(&htim8,TIM_CHANNEL_1,(uint32_t *)RGB_buffur,DMA_LEN);
    
	}
