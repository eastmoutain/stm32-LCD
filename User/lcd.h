#ifndef __LCD_H_
#define __LCD_H_
#include "stm32f10x_conf.h"

/*Private define-------------------------*/
#define POINT_COLOR RED
#define BACK_COLOR	0xFFFF
/*--------16λ��ɫֵ---------------*/
#define WHITE         	 0xFFFF
#define BLACK         	 0x0000	  
#define BLUE         	 0x001F  
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 			 0XBC40 //��ɫ
#define BRRED 			 0XFC07 //�غ�ɫ
#define GRAY  			 0X8430 //��ɫ
#define LGRAY 			 0XC618 //ǳ��ɫ

/*----------��������------------*/

void LCD_Fill(uint8_t xsta, uint16_t ysta, uint8_t xend, uint16_t yend, uint16_t colour);
void LCD_DrawLine(uint16_t xsta, uint16_t ysta, uint16_t xend, uint16_t yend);
void Draw_Circle(uint16_t x0, uint16_t y0, uint8_t r);
void LCD_DrawRectangle(uint16_t xsta, uint16_t ysta, uint16_t xend, uint16_t yend);
void LCD_DrawPoint(uint16_t xsta, uint16_t ysta);
void LCD_Clear(uint16_t Colour);

void LCD_WindowMax (unsigned int x,unsigned int y,unsigned int x_end,unsigned int y_end);
/*��ʾ�ַ� ������Ӣ��*/
void LCD_ShowString(u16 x0, u16 y0, u8 *pcStr, u16 PenColor, u16 BackColor);
/*��ʾͼƬ gImageȡģ bmp��ʽ*/
void LCD_DrawPicture(u16 StartX,u16 StartY,u16 Xend,u16 Yend,const unsigned char *pic);


void LCD_Configuration(void);
void LCD_FSMCConfiguration(void);
void LCD_Init(void);


#endif
