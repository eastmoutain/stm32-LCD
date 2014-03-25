#include "lcd.h"
#include "font.h"
#include <stdio.h>
#include "systick.h"
#include "stm32f10x_fsmc.h"

/* Private variable ---------------------------------------------------------*/
uint16_t DeviceCode;

/* Private typedef -----------------------------------------------------------*/
typedef struct
{
  vu16 LCD_REG;
  vu16 LCD_RAM;
} LCD_TypeDef;

/* LCD is connected to the FSMC_Bank1_NOR/SRAM4 and NE4 is used as ship select signal */
#define LCD_BASE    ((u32)(0x60000000 | 0x0C000000))
#define LCD         ((LCD_TypeDef *) LCD_BASE)

/*-------------------private function */

void LCD_WR_REG(uint16_t LCD_Reg)
{
	/* Write 16-bit Index, then Write Reg */
	LCD->LCD_REG = LCD_Reg;	  
}

void LCD_WR_DATA(uint16_t LCD_Data)	
{
	/* Write 16-bit Reg */
	LCD->LCD_RAM = LCD_Data;
} 


uint16_t LCD_ReadReg(uint16_t LCD_Reg)
{
	u16 temp;
	/* Write 16-bit Index (then Read Reg) */
	LCD->LCD_REG = LCD_Reg;
	temp = LCD->LCD_RAM;
	temp = LCD->LCD_RAM;
	/* Read 16-bit Reg */
	return temp;  	
}

void LCD_WriteReg(uint16_t LCD_Reg ,uint16_t LCD_RegValue)
{
	/* Write 16-bit Index, then Write Reg */
	LCD->LCD_REG = LCD_Reg;
	/* Write 16-bit Reg */
	LCD->LCD_RAM = LCD_RegValue;	
}

/**
	*���ƣ�
	*������
	*���أ�
	*���ܣ�
	*��ע����ʼдGRAM
**/
void LCD_WriteRAM_Prepare(void)
{
	LCD_WR_REG(0x22);
}

/**
	*���ƣ�
	*������
	*���أ�
	*���ܣ�LCDдGRAM
	*��ע��
**/
void LCD_WriteRAM(u16 RGB_Code)
{							    
	LCD_WR_DATA(RGB_Code);    /*дʮ��λGRAM*/
}


/**
	*���ƣ�void LCD_SetCursor(u16 Xpos, u16 Ypos)
	*������Xpos ������
		   Ypos ������
	*���أ���
	*���ܣ����ù��λ��
**/
void LCD_SetCursor(u16 Xpos, u16 Ypos)
{
	LCD_WriteReg(0x004E, Xpos);
	LCD_WriteReg(0X004F, Ypos);
}

/**
	*���ƣ�void_LCD_Clear(uint16_t Colour)
	*������Colour 16λ��ɫ����
	*���أ���
	*���ܣ���ָ����ɫˢ��
**/
void LCD_Clear(uint16_t Colour)
{
	uint32_t index=0;      
	LCD_SetCursor(0x00,0x0000);      /*���ù��λ��  */
	LCD_WriteRAM_Prepare();          /*��ʼд��GRAM	 */	  
	for(index=0;index<76800;index++)
	{
		LCD->LCD_RAM=Colour;    
	}
}

/****************************************************************************
* ��    �ƣ�u16 LCD_GetPoint(u16 x,u16 y)
* ��    �ܣ���ȡָ���������ɫֵ
* ��ڲ�����x      ������
*           y      ������
* ���ڲ�������ǰ������ɫֵ
* ˵    ����
* ���÷�����CurrentColor = LCD_GetPoint(10,10);
* ��ע���˺����д���֤���˴�����
****************************************************************************/
//u16 LCD_GetPoint(u16 x,u16 y)
//{
//  LCD_SetCursor(x,y);
//  if(DeviceCode=0X8999)
//    return (LCD_ReadRAM());
//  else
//    return (LCD_BGRtoRGB(LCD_ReadRAM()));
//}

/**
	*���ƣ�void LCD_DrawPoint(void)
	*������xsta X��ʼ���� 0~239
		   ysta	Y��ʼ���� 0~319
	*���أ���
	*���ܣ�POINT_COLORָ�������ɫ

**/
void LCD_DrawPoint(uint16_t xsta, uint16_t ysta)
{
	LCD_SetCursor(xsta,ysta);  /*���ù��λ��  */
	LCD_WR_REG(0x22);           /*��ʼд��GRAM */
	LCD_WR_DATA(POINT_COLOR); 
}

/**					
	*���ƣ�void LCD_WindowMax()
	*������
	*���أ���
	*���ܣ����ô���
	*��ע��
**/
void LCD_WindowMax (unsigned int x,unsigned int y,unsigned int x_end,unsigned int y_end) 
{
	LCD_WriteReg(0x44,x|((x_end-1)<<8));
	LCD_WriteReg(0x45,y);
	LCD_WriteReg(0x46,y_end-1);
}

/**
	*���ƣ�void LCD_Fill(uint8_t xsta, uint16_t ysta, uint8_t xend, uint16_t yend, uint16_t colour)
	*������xsta ��ʼX����
		   ysta ��ʼY����
		   xend ����X����
		   yend ����Y����
		   color �������ɫ
	*���أ���
	*���ܣ���ָ�������������ָ����ɫ�������С(xend-xsta)*(yend-ysta)
	*��ע������������һ�����ص�	
**/
void LCD_Fill(uint8_t xsta, uint16_t ysta, uint8_t xend, uint16_t yend, uint16_t colour)
{                    
    u32 n;

	/*���ô���	*/	
	LCD_WindowMax (xsta, ysta, xend, yend); 
	LCD_SetCursor(xsta,ysta);        /*���ù��λ�� */ 
	LCD_WriteRAM_Prepare();         /*��ʼд��GRAM*/	 	   	   
	n=(u32)(yend-ysta+1)*(xend-xsta+1);    
	while(n--){LCD_WR_DATA(colour);} /*��ʾ��������ɫ*/
	 
	/*�ָ�����*/
	LCD_WindowMax (0, 0, 240, 320); 
}  

/**
	*���ƣ�void LCD_DrawLine(uint16_t xsta, uint16_t ysta, uint16_t xend, uint16_t yend)
	*������xsta X��ʼ����
	   	   ysta Y��ʼ����
		   xend X�յ�����
		   yend Y�յ�����	
	*���أ���
	*���ܣ�ָ������(����)������
	*��ע����Ҫ������ɫ����
**/ 
void LCD_DrawLine(uint16_t xsta, uint16_t ysta, uint16_t xend, uint16_t yend)
{
    u16 x, y, t;
	if((xsta==xend)&&(ysta==yend))LCD_DrawPoint(xsta, ysta);
	else if(abs(yend-ysta)>abs(xend-xsta))/*б�ʴ���1 */ 
	{
		if(ysta>yend) 
		{
			t=ysta;
			ysta=yend;
			yend=t; 
			t=xsta;
			xsta=xend;
			xend=t; 
		}
		for(y=ysta;y<yend;y++)            /*��y��Ϊ��׼*/ 
		{
			x=(u32)(y-ysta)*(xend-xsta)/(yend-ysta)+xsta;
			LCD_DrawPoint(x, y);  
		}
	}
	else     /*б��С�ڵ���1 */
	{
		if(xsta>xend)
		{
			t=ysta;
			ysta=yend;
			yend=t;
			t=xsta;
			xsta=xend;
			xend=t;
		}   
		for(x=xsta;x<=xend;x++)  /*��x��Ϊ��׼*/ 
		{
			y =(u32)(x-xsta)*(yend-ysta)/(xend-xsta)+ysta;
			LCD_DrawPoint(x,y); 
		}
	} 
} 

/**
	*���ƣ�void Draw_Circle(uint16_t x0, uint16_t y0, uint8_t r)
	*������x0 ���ĵ������
	       y0 ���ĵ�������
		   r  �뾶
	*���أ���
	*���ܣ���ָ��λ�û�һ��ָ����С��Բ
	*��ע��������ɫ�������Ƿ�����
**/
void Draw_Circle(uint16_t x0, uint16_t y0, uint8_t r)
{
	int a,b;
	int di;
	a=0;b=r;	  
	di=3-(r<<1);             /*�ж��¸���λ�õı�־*/
	while(a<=b)
	{
		LCD_DrawPoint(x0-b,y0-a);             //3           
		LCD_DrawPoint(x0+b,y0-a);             //0           
		LCD_DrawPoint(x0-a,y0+b);             //1       
		LCD_DrawPoint(x0-b,y0-a);             //7           
		LCD_DrawPoint(x0-a,y0-b);             //2             
		LCD_DrawPoint(x0+b,y0+a);             //4               
		LCD_DrawPoint(x0+a,y0-b);             //5
		LCD_DrawPoint(x0+a,y0+b);             //6 
		LCD_DrawPoint(x0-b,y0+a);             
		a++;

		/*ʹ��Bresenham�㷨��Բ*/     
		if(di<0)di +=4*a+6;	  
		else
		{
			di+=10+4*(a-b);   
			b--;
		} 
		LCD_DrawPoint(x0+a,y0+b);
	}
} 

/**
	*���ƣ�void LCD_DrawRectangle(uint16_t xsta, uint16_t ysta, uint16_t xend, uint16_t yend)
	*������xsta X��ʼ����
	       ysta Y��ʼ����
		   xend X��������
		   yend Y��������
	*���أ���
	*���ܣ���ָ�����򻭾���
	*��ע��

**/
void LCD_DrawRectangle(uint16_t xsta, uint16_t ysta, uint16_t xend, uint16_t yend)
{
	LCD_DrawLine(xsta,ysta,xend,ysta);
	LCD_DrawLine(xsta,ysta,xsta,yend);
	LCD_DrawLine(xsta,yend,xend,yend);
	LCD_DrawLine(xend,ysta,xend,yend);
} 

/**
	*���ƣ�void LCD_ShowChar(u8 x, u16 y, u8 num, u8 size, u16 PenColor, u16 BackColor)
	*������x��y      ��ʼ���꣨x:0~234 y:0~308��
		   num       �ַ�ASCII��ֵ
		   size      �ַ���С��ʹ��Ĭ��8*16
		   PenColor  ������ɫ
		   BackColor ���屳����ɫ
	*���ܣ�
	*��ע��ע����Ļ��С
**/
void LCD_ShowChar(u8 x, u16 y, u8 num, u8 size, u16 PenColor, u16 BackColor)
{       
#define MAX_CHAR_POSX 232
#define MAX_CHAR_POSY 304 
    u8 temp;
    u8 pos,t;
    if(x>MAX_CHAR_POSX||y>MAX_CHAR_POSY)return;		    
	LCD_WindowMax(x,y,x+size/2,y+size);	   /*���ô���	*/										
	LCD_SetCursor(x, y);                  /*���ù��λ�� */
  
	LCD_WriteRAM_Prepare();               /*��ʼд��GRAM  */   
	num=num-' ';                         /*�õ�ƫ�ƺ��ֵ */
	for(pos=0;pos<size;pos++)
	{
		if(size==12)
			temp=asc2_1206[num][pos];/*����1206����*/
		else 
			temp=asc2_1608[num][pos];		 /*����1608����	*/
		for(t=0;t<size/2;t++)
	    {                 
	        if(temp&0x01)			   /*�ӵ�λ��ʼ*/
			{
				LCD_WR_DATA(PenColor);  /*��������ɫ һ����*/
			}
			else 
				LCD_WR_DATA(BackColor);	   /*��������ɫ һ����*/     
	        temp>>=1; 
	    }
	}			
	LCD_WindowMax(0x0000,0x0000,240,320);	/*�ָ������С*/	 
} 

//m^n����
u32 mypow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}			 
//��ʾ2������
//x,y :�������	 
//len :���ֵ�λ��
//size:�����С
//color:��ɫ
//num:��ֵ(0~4294967295);	 
void LCD_ShowNum(u8 x,u8 y,u32 num,u8 len, u16 PenColor, u16 BackColor)
{    
	u8 size = 16; /*  ����ʹ��Ĭ�ϵ�16*8  */     	
	u8 t,temp;
	u8 enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/mypow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				LCD_ShowChar(x+(size/2)*t,y,' ',size, PenColor, BackColor);
				continue;
			}else enshow=1; 
		 	 
		}
	 	LCD_ShowChar(x+(size/2)*t,y,temp+'0',size, PenColor, BackColor); 
	}
} 
  


/**
	���ƣ�void LCD_ShowCharString(uint16_t x, uint16_t y, const uint8_t *p, uint16_t PenColor, uint16_t BackColor)
	������x��y      ��ʼ����
	      p         ָ���ַ�����ʼ��ַ
		  PenColor  �ַ���ɫ
		  BackColor ������ɫ
	���ܣ�
	��ע����16���壬���Ե��� �˺������ܵ�������
**/
void LCD_ShowCharString(uint16_t x, uint16_t y, const uint8_t *p, uint16_t PenColor, uint16_t BackColor)
{   
	uint8_t size = 16;     /*---�ַ���СĬ��16*8---*/
	 
    if(x>MAX_CHAR_POSX){x=0;y+=size;}			         /*����X��������С��λ������*/
    if(y>MAX_CHAR_POSY){y=x=0;LCD_Clear(WHITE);}	 /*����Y��������С��λ���ص�ԭ�㣬��������*/
    LCD_ShowChar(x, y, *p, size, PenColor, BackColor);			   /*0��ʾ�ǵ��ӷ�ʽ*/
}

/**
	*����: u16 findHzIndex(u8 *hz)
	*������hz
	*���ܣ��������ִ洢���ڴ��ַ
	*��ע��
**/
u16 findHzIndex(u8 *hz)                            /* ���Զ��庺�ֿ��ڲ�����Ҫ��ʾ */
                                                      /* �ĺ��ֵ�λ�� */
{
	u16 i=0;
	FNT_GB16 *ptGb16 = (FNT_GB16 *)GBHZ_16;		  /*ptGb16ָ��GBHZ_16*/
	while(ptGb16[i].Index[0] > 0x80)
	{
	    if ((*hz == ptGb16[i].Index[0]) && (*(hz+1) == ptGb16[i].Index[1])) /*��������λ����ʾ��ַ��*/
		{
	        return i;
	    }
	    i++;
	    if(i > (sizeof((FNT_GB16 *)GBHZ_16) / sizeof(FNT_GB16) - 1))  /* �����±�Լ�� */
	    {
		    break;
	    }
	}
	return 0;
}

/**
	*���ƣ�void WriteOneHz(uint16_t x0, uint16_t y0, uint8_t *pucMsk, uint16_t PenColor, uint16_t BackColor)
	*������x0,y0     ��ʼ����
		   *pucMsk   ָ��
		   PenColor	 �ַ���ɫ
		   BackColor ������ɫ
	*���ܣ�
	*��ע���˺������ܵ�����Ϊ�����ַ���ʾ											  
**/					
void WriteOneHz(u16 x0, u16 y0, u8 *pucMsk, u16 PenColor, u16 BackColor)
{
    u16 i,j;
    u16 mod[16];                                      /* ��ǰ��ģ 16*16 */
    u16 *pusMsk;                                      /* ��ǰ�ֿ��ַ  */
    u16 y;

	u16 size = 16;       /*����Ĭ�ϴ�С16*16*/

    pusMsk = (u16 *)pucMsk;


    for(i=0; i<16; i++)                                    /* ���浱ǰ���ֵ���ʽ��ģ       */
    {
        mod[i] = *pusMsk;                                /* ȡ�õ�ǰ��ģ�����ֶ������   */
        mod[i] = ((mod[i] & 0xff00) >> 8) | ((mod[i] & 0x00ff) << 8);/* ��ģ�����ߵ��ֽ�*/
		pusMsk = pusMsk+1;
    }
    y = y0;
	LCD_WindowMax(x0,y0,x0+size,y0+size);	 	/*���ô���*/
	LCD_SetCursor(x0,y0);                       /*���ù��λ�� */ 
	LCD_WriteRAM_Prepare();                     /*��ʼд��GRAM*/  
    for(i=0; i<16; i++)                                    /* 16��   */
    {                                              
        for(j=0; j<16; j++)                                /* 16��   */
        {
		    if((mod[i] << j) & 0x8000)       /* ��ʾ��i�� ��16���� */
            { 
			    LCD_WriteRAM(PenColor);
            } 
			else 
			{
                LCD_WriteRAM(BackColor);      /* �ö���ʽ����д�հ׵������*/
			}
        }
        y++;
    }
	LCD_WindowMax(0x0000,0x0000,240,320);  	/*�ָ������С*/
}

/**
	*���ƣ�void LCD_ShowHzString(u16 x0, u16 y0, u8 *pcStr, u16 PenColor, u16 BackColor)
	*������x0��y0    ��ʼ����
		   pcStr     ָ��
		   PenColor  ������ɫ
		   BackColor ���屳��
	*���ܣ���ʾ�����ַ���
	*��ע������������ܵ�������	       
**/
void LCD_ShowHzString(u16 x0, u16 y0, u8 *pcStr, u16 PenColor, u16 BackColor)
{
#define MAX_HZ_POSX 224
#define MAX_HZ_POSY 304 
	u16 usIndex;
	u8 size = 16; 
	FNT_GB16 *ptGb16 = 0;    
    ptGb16 = (FNT_GB16 *)GBHZ_16; 

	if(x0>MAX_HZ_POSX){x0=0;y0+=size;}			         /*����X��������С��λ������*/
    if(y0>MAX_HZ_POSY){y0=x0=0;LCD_Clear(WHITE);}	   /*����Y��������С��λ���ص�ԭ�㣬��������*/

	usIndex = findHzIndex(pcStr);
	WriteOneHz(x0, y0, (u8 *)&(ptGb16[usIndex].Msk[0]),  PenColor, BackColor); /* ��ʾ�ַ� */
}

/**
	*���ƣ�void LCD_ShowString(u16 x0, u16 y0, u8 *pcstr, u16 PenColor, u16 BackColor)
	*������x0 y0     ��ʼ����
		   pcstr     �ַ���ָ��
		   PenColor  ������ɫ
		   BackColor ���屳��ɫ
	*���ܣ������ַ��ͺ�����ʾ������ʵ���ַ�����ʾ
	*��ע��	
**/
void LCD_ShowString(u16 x0, u16 y0, u8 *pcStr, u16 PenColor, u16 BackColor)
{
	while(*pcStr!='\0')
	{
	 	if(*pcStr>0x80) /*��ʾ����*/
		{
			LCD_ShowHzString(x0, y0, pcStr, PenColor, BackColor);
			pcStr += 2;
			x0 += 16;	
		}
		else           /*��ʾ�ַ�*/
		{
			LCD_ShowCharString(x0, y0, pcStr, PenColor, BackColor);	
			pcStr +=1;
			x0+= 8;
		}
	
	}	

}

/****************************************************************************
* ��    �ƣ�u16 ili9320_BGRtoRGB(u16 Color)
* ��    �ܣ�RRRRRGGGGGGBBBBB ��Ϊ BBBBBGGGGGGRRRRR ��ʽ
* ��ڲ�����Color      BRG ��ɫֵ
* ���ڲ�����RGB ��ɫֵ
* ˵    �����ڲ���������
* ���÷�����
****************************************************************************/
u16 LCD_RGBtoBGR(u16 Color)
{						   
  u16  r, g, b, bgr;

  b = (Color>>0)  & 0x1f;	/* ��ȡB    */
  g = (Color>>5)  & 0x3f;	/* �м���λ */
  r = (Color>>11) & 0x1f;	/* ��ȡR    */
  
  bgr =  (b<<11) + (g<<5) + (r<<0);

  return( bgr );
}

/****************************************************************************
* ��    �ƣ�void LCD_DrawPicture(u16 StartX,u16 StartY,u16 EndX,u16 EndY,u16 *pic)
* ��    �ܣ���ָ�����귶Χ��ʾһ��ͼƬ
* ��ڲ�����StartX     ����ʼ����
*           StartY     ����ʼ����
*           EndX       �н�������
*           EndY       �н�������
            pic        ͼƬͷָ��
* ���ڲ�������
* ˵    ����ͼƬȡģ��ʽΪˮƽɨ�裬16λ��ɫģʽ
* ���÷�����LCD_DrawPicture(0,0,100,100,(u16*)demo);
****************************************************************************/
void LCD_DrawPicture(u16 StartX,u16 StartY,u16 Xend,u16 Yend,const unsigned char *pic)
{
	static	u16 i=0,j=0;
	u16 *bitmap = (u16 *)pic;
	/*����ͼƬ��ʾ���ڴ�С*/
	LCD_WindowMax(StartX, StartY, Xend, Yend);	
	LCD_SetCursor(StartX,StartY);
	LCD_WriteRAM_Prepare();
	for(j=0; j<Yend-StartY; j++)
	{
		for(i=0; i<Xend-StartX; i++) LCD_WriteRAM(*bitmap++); 	
	}
	/*�ָ�����*/
	LCD_WindowMax(0, 0, 240, 320);
}

void LCD_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	/* Enable FSMC, GPIOD, GPIOE, GPIOF, GPIOG and AFIO clocks */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE |
	                     RCC_APB2Periph_GPIOF | RCC_APB2Periph_GPIOG |
	                     RCC_APB2Periph_AFIO, ENABLE);
	
	/* Set  PD.00(D2), PD.01(D3), PD.04(NOE)--LCD_RD ������, PD.05(NWE)--LCD_WR��д��,
	      PD.08(D13),PD.09(D14),PD.10(D15),PD.14(D0),PD.15(D1) */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_8 | 
								GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_14 | GPIO_Pin_15  ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	     /*�����������*/
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	/* Set PE.07(D4), PE.08(D5), PE.09(D6), PE.10(D7), PE.11(D8), PE.12(D9), PE.13(D10),
	 PE.14(D11), PE.15(D12) as alternate function push pull */
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | 
	                            GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | 
	                            GPIO_Pin_15;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	
	/* Set PF.00(A0 (RS)) as alternate function push pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_Init(GPIOF, &GPIO_InitStructure);
	
	/* Set PG.12 PE2 (LCD_CS) as alternate function push pull - CE3(LCD /CS) */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_Init(GPIOG, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	     /*�����������*/
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOE, GPIO_Pin_2);
}

/**
	*���ƣ�void FSMC_Configuration(void)
	*��������
	*���أ���
	*���ܣ�FSMC��ʼ������TFTʹ��
**/
void LCD_FSMCConfiguration(void)
{
	FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;
	FSMC_NORSRAMTimingInitTypeDef  p;
	
	/*-- FSMC Configuration ------------------------------------------------------*/
	/*----------------------- SRAM Bank 4 ----------------------------------------*/
	/* FSMC_Bank1_NORSRAM4 configuration */
	p.FSMC_AddressSetupTime = 6;
	p.FSMC_AddressHoldTime = 8;
	p.FSMC_DataSetupTime = 3;
	p.FSMC_BusTurnAroundDuration = 0;
	p.FSMC_CLKDivision = 0;
	p.FSMC_DataLatency = 0;
	p.FSMC_AccessMode = FSMC_AccessMode_A;
	
	/* Color LCD configuration ------------------------------------
	LCD configured as follow:
	- Data/Address MUX = Disable
	- Memory Type = SRAM
	- Data Width = 16bit
	- Write Operation = Enable
	- Extended Mode = Enable
	- Asynchronous Wait = Disable */
	FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM4;
	FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable;
	FSMC_NORSRAMInitStructure.FSMC_MemoryType = FSMC_MemoryType_SRAM;
	FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;
	FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable;
	FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
	FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;
	FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;
	FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;
	FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;
	FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Disable;
	FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable;
	FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &p;
	FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &p;
	
	FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure);  
	
	/* BANK 4 (of NOR/SRAM Bank 1~4) is enabled */
	FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM4, ENABLE);
}

void LCD_Init(void)
{
	LCD_Configuration();
	LCD_FSMCConfiguration();

	Delay_Nms(50); /* delay 50 ms */ 
	Delay_Nms(50); /* delay 50 ms */
	DeviceCode = LCD_ReadReg(0x0000);
	Delay_Nms(50); /* delay 50 ms */
//	if(DeviceCode==0x8999)	   /*��Ӧ������ICΪSSD1298*/
	{
		/*-----   Start Initial Sequence ------*/
		LCD_WriteReg(0x00, 0x0001); /*Start internal OSC */
		LCD_WriteReg(0x01, 0x3B3F); /*Driver output control */
		LCD_WriteReg(0x02, 0x0600); /* set 1 line inversion	*/
		/*-------- Power control setup --------*/
		LCD_WriteReg(0x0C, 0x0007); /* Adjust VCIX2 output voltage */
		LCD_WriteReg(0x0D, 0x0006); /* Set amplitude magnification of VLCD63 */
		LCD_WriteReg(0x0E, 0x3200); /* Set alternating amplitude of VCOM */
		LCD_WriteReg(0x1E, 0x00BB); /* Set VcomH voltage */
		LCD_WriteReg(0x03, 0x6A64); /* Step-up factor/cycle setting  */
		/*-------- RAM position control --------*/
		LCD_WriteReg(0x0F, 0x0000); /* Gate scan position start at G0 */
		LCD_WriteReg(0x44, 0xEF00); /* Horizontal RAM address position */
		LCD_WriteReg(0x45, 0x0000); /* Vertical RAM address start position*/
		LCD_WriteReg(0x46, 0x013F); /* Vertical RAM address end position */
		/* ------ Adjust the Gamma Curve -------*/
		LCD_WriteReg(0x30, 0x0000);
		LCD_WriteReg(0x31, 0x0706);
		LCD_WriteReg(0x32, 0x0206);
		LCD_WriteReg(0x33, 0x0300);
		LCD_WriteReg(0x34, 0x0002);
		LCD_WriteReg(0x35, 0x0000);
		LCD_WriteReg(0x36, 0x0707);
		LCD_WriteReg(0x37, 0x0200);
		LCD_WriteReg(0x3A, 0x0908);
		LCD_WriteReg(0x3B, 0x0F0D);
		/*--------- Special command -----------*/
		LCD_WriteReg(0x28, 0x0006); /* Enable test command	*/
		LCD_WriteReg(0x2F, 0x12EB); /* RAM speed tuning	 */
		LCD_WriteReg(0x26, 0x7000); /* Internal Bandgap strength */
		LCD_WriteReg(0x20, 0xB0E3); /* Internal Vcom strength */
		LCD_WriteReg(0x27, 0x0044); /* Internal Vcomh/VcomL timing */
		LCD_WriteReg(0x2E, 0x7E45); /* VCOM charge sharing time */ 
		/*--------- Turn On display ------------*/
		LCD_WriteReg(0x10, 0x0000); /* Sleep mode off */
		Delay_Nms(30);              /* Wait 30mS  */
		LCD_WriteReg(0x11, 0x6870); /* Entry mode setup. 262K type B, take care on the data bus with 16it only */
		LCD_WriteReg(0x07, 0x0033); /* Display ON	*/
	}						  
	Delay_Nms(50);	     /*��ʱ50ms*/
	LCD_Clear(BLACK);
}

