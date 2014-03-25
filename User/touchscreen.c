#include "stm32f10x.h"
#include "stm32f10x_conf.h"
#include "systick.h"
#include "usart.h"
#include "touchscreen.h"
#include "lcd.h"

void TS_GPIO_Init(void)
{
	GPIO_InitTypeDef	GPIO_InitStructure;
    // configure sending data port
	RCC_APB2PeriphClockCmd(RCC_APB_TS_Data_In, ENABLE);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  /* out pp mode */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = TS_Data_In_Pin;
	GPIO_Init(TS_Data_In_GPIO, &GPIO_InitStructure);
	// configure receiving data port
	RCC_APB2PeriphClockCmd(RCC_APB_TS_Data_Out, ENABLE);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;  /* float in mode */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = TS_Data_Out_Pin;
	GPIO_Init(TS_Data_Out_GPIO, &GPIO_InitStructure);
	// configure touch screen CS
	RCC_APB2PeriphClockCmd(RCC_APB_TS_CS, ENABLE);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  /* out pp mode */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = TS_CS_Pin;
	GPIO_Init(TS_CS_GPIO, &GPIO_InitStructure);
	// configure touch screen clock
	RCC_APB2PeriphClockCmd(RCC_APB_TS_Clk, ENABLE);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  /* out pp mode */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = TS_Clk_Pin;
	GPIO_Init(TS_Clk_GPIO, &GPIO_InitStructure);

	RCC_APB2PeriphClockCmd(RCC_APB_Pen_Clk, ENABLE);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;  /* out pp mode */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = TS_Pen_Pin;
	GPIO_Init(TS_Pen_GPIO, &GPIO_InitStructure);
}

void TS_Write_Cmd(char cmd)
{
	int i = 0;
//	USART_SendStr(USART1,"enter TS_Write \r\n");
	TS_CS_L();
	TS_Clk_L();
	for(i = 0; i < 8 ; i++)
	{
		if(cmd & 0x80)
		{
			GPIO_SetBits(TS_Data_In_GPIO, TS_Data_In_Pin);
		}
		else
		{
			GPIO_ResetBits(TS_Data_In_GPIO, TS_Data_In_Pin);	
		}

		Delay_Nms(1);
		TS_Clk_H();
		Delay_Nms(1);
		TS_Clk_L();

		cmd <<= 1;
	}
	TS_CS_H();
//	USART_SendStr(USART1,"leave TS_Write \r\n");
}

unsigned short TS_Read_Data(void)
{
	int i = 0;
	unsigned short	rd_data = 0;
//	USART_SendStr(USART1,"enter TS_Read \r\n");

	TS_CS_L();
	Delay_Nus(1);
	TS_Clk_L();
	Delay_Nus(1);
   	TS_Clk_H();
	Delay_Nus(1);
	

	for(i = 0; i < 12; i++)
	{
		rd_data <<= 1;
		TS_Clk_L();
		Delay_Nus(1);
		if(GPIO_ReadInputDataBit(TS_Data_Out_GPIO, TS_Data_Out_Pin))
		{
			rd_data |= 0x0001;
		}
		else
		{
			rd_data &= ~(0x0001);
		}
		TS_Clk_H();
		Delay_Nus(1);
	
	}
	TS_Clk_L();
	TS_CS_H();
//	USART_SendStr(USART1,"leave TS_Read \r\n");
//	Delay_Nus(1000);
	return rd_data;
}

void Get_Coordinate(Coordinate *cod)
{
	
	char data[5];
	unsigned short temp_x, temp_y;
//	Pen_Int_Reset();
	TS_Write_Cmd(Y_POSITION);
	coord.y_pos_val = TS_Read_Data();
	TS_Write_Cmd(X_POSITION);
	coord.x_pos_val = TS_Read_Data();
//	Delay_Nus(1000);
//	Pen_Int_Set();

	temp_y = coord.y_pos_val & 0x0fff;
	temp_x = coord.x_pos_val &= 0x0fff;

	if(temp_x < 240 || temp_x > 3900 || temp_y < 240 || temp_y > 3900) return;
	
	temp_x = (int)((temp_x - 240)* 240/3660);
	cod->x_pos_val = temp_x;
	data[0] = temp_x / 1000 + '0';
	temp_x = temp_x % 1000;
	data[1] = temp_x / 100 + '0';
	temp_x = temp_x % 100;
	data[2] = temp_x / 10 + '0';
	data[3] = temp_x % 10 + '0';
	data[4] = '\0';
	USART_SendStr(USART1,"x_val is  ");
	USART_SendStr(USART1,data);
	USART_SendStr(USART1,"\r\n");
	
	temp_y = 320 - (int)((temp_y - 240)* 320/3660);
	cod->y_pos_val = temp_y;
	data[0] = temp_y / 1000 + '0';
	temp_y = temp_y % 1000;
	data[1] = temp_y / 100 + '0';
	temp_y = temp_y % 100;
	data[2] = temp_y / 10 + '0';
	data[3] = temp_y % 10 + '0';
	data[4] = '\0';
	USART_SendStr(USART1,"y_val is  ");
	USART_SendStr(USART1,data);
	USART_SendStr(USART1,"\r\n"); 

}

void TS_IRQ(void)
{
	Coordinate cod;
	int i=0;
	int j=0;
	for(i=0;i<1000;i++)
	for(j=0;j<500;j++);
//	Delay_Nms(10);
	if(GPIO_ReadInputDataBit(TS_Pen_GPIO, TS_Pen_Pin) == 0)
	{ 
		Get_Coordinate(&cod);
		LCD_DrawPoint(cod.x_pos_val+1,cod.y_pos_val+1);
		LCD_DrawPoint(cod.x_pos_val+1,cod.y_pos_val-1);
		LCD_DrawPoint(cod.x_pos_val,cod.y_pos_val);  //.
		LCD_DrawPoint(cod.x_pos_val,cod.y_pos_val+1);
		LCD_DrawPoint(cod.x_pos_val,cod.y_pos_val-1);
		LCD_DrawPoint(cod.x_pos_val+1,cod.y_pos_val);
		LCD_DrawPoint(cod.x_pos_val-1,cod.y_pos_val);
		LCD_DrawPoint(cod.x_pos_val-1,cod.y_pos_val-1);
		LCD_DrawPoint(cod.x_pos_val-1,cod.y_pos_val+1);
	}
	

}
void Pen_Int_Set(void)
{
	EXTI->IMR|=EXTI_Line7;   //开启line7上的中断	  		   
}

void Pen_Int_Reset(void)
{	  	
	EXTI->IMR&=~(EXTI_Line7); //关闭line7上的中断	   
}


