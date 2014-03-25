#include "stm32f10x.h"

void Init_USART(void)
{  /*串口初始化程序*/
	
	GPIO_InitTypeDef GPIO_InitStruct;
	USART_InitTypeDef USART_InitStruct;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD
							| RCC_APB2Periph_USART1 | RCC_APB2Periph_AFIO,ENABLE);

//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2 | RCC_APB1Periph_USART3, ENABLE);

//	GPIO_PinRemapConfig(GPIO_FullRemap_USART3, ENABLE);
	//串口1的IO初始化
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;						
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;	
	GPIO_Init(GPIOA,&GPIO_InitStruct); //TX	
	//串口2的IO初始化
//	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2;
//	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;	
//	GPIO_Init(GPIOA,&GPIO_InitStruct); //TX
	//串口3的IO初始化
/*	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;	
	GPIO_Init(GPIOD,&GPIO_InitStruct); //TX
*/
   //串口1的IO初始化
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;	
	GPIO_Init(GPIOA,&GPIO_InitStruct); //RX	
	//串口2的IO初始化
//	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3;
//	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;	
//	GPIO_Init(GPIOA,&GPIO_InitStruct); //RX
	 //串口3的IO初始化
/*	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;	
	GPIO_Init(GPIOD,&GPIO_InitStruct); //RX
 */
	//串口1 @ 3初始化
	USART_InitStruct.USART_BaudRate  = 115200;
	USART_InitStruct.USART_WordLength  = USART_WordLength_8b;
	USART_InitStruct.USART_StopBits  = USART_StopBits_1;
	USART_InitStruct.USART_Parity  = USART_Parity_No;
	USART_InitStruct.USART_Mode  = USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStruct.USART_HardwareFlowControl  = USART_HardwareFlowControl_None;

	USART_Init(USART1, &USART_InitStruct);
//	USART_Init(USART3, &USART_InitStruct);

	//串口2初始化
//	USART_InitStruct.USART_BaudRate  = 9600;
//	USART_InitStruct.USART_WordLength  = USART_WordLength_8b;  
//	USART_InitStruct.USART_StopBits  = USART_StopBits_1;
//	USART_InitStruct.USART_Parity  = USART_Parity_No;
//	USART_InitStruct.USART_Mode  = USART_Mode_Rx | USART_Mode_Tx;
//	USART_InitStruct.USART_HardwareFlowControl  = USART_HardwareFlowControl_None;
//
//	USART_Init(USART1, &USART_InitStruct);

	USART_Cmd(USART1, ENABLE);
//	USART_Cmd(USART2, ENABLE);
//	USART_Cmd(USART3, ENABLE);

	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
	USART_ITConfig(USART1,USART_IT_TXE,DISABLE);



}

//USART发送字符
void USART_SendChar(USART_TypeDef* USARTx, char dat)
{
	while(!USART_GetFlagStatus(USARTx, USART_FLAG_TXE));	
	USART_SendData(USARTx, dat);

}
//USART接收数据
char USART_GetChar(USART_TypeDef* USARTx)
{
	char dat;
//	while (!USART_GetFlagStatus(USARTx,USART_FLAG_RXNE));
	dat = (char)(USART_ReceiveData(USARTx) & 0xff);
	return dat;		
}

void USART_SendStr(USART_TypeDef* USARTx, char *s)
{
	while (*s != '\0')
	USART_SendChar(USARTx, *s++);
}
//USART2 中断
void USART1_IRQ(void)
{
//	unsigned char rx_dat;
	
	if(USART_GetFlagStatus(USART1,USART_FLAG_RXNE))
		USART_ClearFlag(USART1,USART_FLAG_RXNE); 
//	rx_dat = Usart_GetChar(USART1);	

}
