#include <stdio.h>
#include "stm32f10x.h"
#include "stm32f10x_conf.h"
#include "hw_config.h"
#include "systick.h"
#include "usart.h"
#include "SystemClock.h"
#include "touchscreen.h"
#include "lcd.h"
#include "qie.h"

//#define WHITE         	 0xFFFF
//#define BLACK         	 0x0000	  
//#define BLUE         	 0x001F  
//#define RED           	 0xF800
//#define MAGENTA       	 0xF81F
//#define GREEN         	 0x07E0
//#define CYAN          	 0x7FFF
//#define YELLOW        	 0xFFE0
//#define BROWN 			 0XBC40 //棕色
//#define BRRED 			 0XFC07 //棕红色
//#define GRAY  			 0X8430 //灰色
//#define LGRAY 			 0XC618 //浅灰色
int clor[11]={WHITE,BLUE,RED,MAGENTA,GREEN,CYAN,YELLOW,BROWN,BRRED,GRAY,LGRAY};

Coordinate coord;
char pen_flag ;
const unsigned char gImage_qie[];

int main(void)
{
	int x_s = 0;
	int y_s = 0;
	int x_e = 40;
	int y_e = 60;
	int i = 0;
	int j = 0;
	int x_flag = 1;
	int y_flag = 1;
	 	

	RCC_Configuration();
	Init_USART();
	TS_GPIO_Init();
	LED_Configuration();
	BUTTON_Configuration();	 
	NVIC_Configuration();
	EXTI_Configuration();
	LCD_Init();			/* LCD初始化 */
	pen_flag = 0;
	LCD_Clear(BLACK);		  //清屏
//	LCD_DrawPicture(100,100,160,200,gImage_qie);
	
	while (1)
	{

		Draw_Circle(20, 280, 20);
		if (i == 11) i = 0;
		LCD_ShowNum(210,30,i/2,1, RED, BLUE);
		LCD_Fill(x_s, y_s ,x_e,y_e, clor[i]);
		if(x_flag )
		{
			if((x_s < 230) && (x_e < 230)&&(x_s < x_e)&&(y_s < 330) && (y_e < 330)&&(y_s < y_e))  
			{x_s +=1; x_e +=2;
				y_s +=1; y_e +=2;
			}
			else x_flag = 0;
		}
		else
		{
			if((y_s > 10) && ( y_e > 10) && (y_s < y_e)&&(x_s > 10) && ( x_e > 10) && (x_s < x_e))  
			{x_s -=1; x_e -=2;
				y_s -=1; y_e -=2;
			}
			else x_flag = 1;
		}
		Delay_Nms(1000);
		LCD_Clear(BLACK);
		j++;
		if(j == 10)  {j=0;i++;}

	   USART_SendStr(USART1,"OK!\r\n") ;
	}
}

