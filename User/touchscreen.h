#ifndef _TOUCHSCREEN_H_
#define _TOUCHSCREEN_H_

// configure XPT2046 
#define		RCC_APB_TS_Data_Out			RCC_APB2Periph_GPIOB
#define 	TS_Data_Out_GPIO			GPIOB
#define		TS_Data_Out_Pin				GPIO_Pin_14

#define		RCC_APB_TS_Data_In			RCC_APB2Periph_GPIOB
#define 	TS_Data_In_GPIO				GPIOB
#define		TS_Data_In_Pin				GPIO_Pin_15

#define		RCC_APB_TS_CS				RCC_APB2Periph_GPIOG
#define 	TS_CS_GPIO					GPIOG
#define		TS_CS_Pin					GPIO_Pin_11

#define		RCC_APB_TS_Clk				RCC_APB2Periph_GPIOB
#define 	TS_Clk_GPIO					GPIOB
#define		TS_Clk_Pin					GPIO_Pin_13

#define		RCC_APB_Pen_Clk				RCC_APB2Periph_GPIOG
#define 	TS_Pen_GPIO					GPIOG
#define		TS_Pen_Pin					GPIO_Pin_7
																	

#define		TS_Clk_H()		(GPIO_SetBits(TS_Clk_GPIO, TS_Clk_Pin))
#define		TS_Clk_L()		(GPIO_ResetBits(TS_Clk_GPIO, TS_Clk_Pin))

#define		TS_CS_H()		(GPIO_SetBits(TS_CS_GPIO, TS_CS_Pin))
#define		TS_CS_L()		(GPIO_ResetBits(TS_CS_GPIO, TS_CS_Pin))

#define		Y_POSITION		0x90	//  S A2 A1 A0 MODE SER/DFR PD1 PD0           A2 A1 A0 = 001
#define		X_POSITION		0xd0	//  1 X  X  X   0     0      0   0            A2 A1 A0 = 101

typedef struct{
				unsigned short x_pos_val;
				unsigned short y_pos_val;
			} Coordinate;

extern Coordinate coord;
extern char pen_flag;

void	TS_GPIO_Init(void);
void	TS_Write_Cmd(char);
unsigned short TS_Read_Data(void);
void Get_Coordinate(Coordinate *cod);
void	TS_IRQ(void);
void TS_Interrupt_DISABLE(void);
void TS_Interrupt_ENABLE(void);
void Comupute_Pixel(Coordinate * cod);
void Pen_Int_Set(void);
void Pen_Int_Reset(void);

#endif
