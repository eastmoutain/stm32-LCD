#ifndef __USART_H_
#define __USART_H_

#include "stm32f10x.h"


//函数申明
void Init_USART(void);
//USART发送字符
void USART_SendChar(USART_TypeDef* USARTx,char dat);
//USART接收数据
char USART_GetChar(USART_TypeDef* USARTx);
//USART发送字符串
void USART_SendStr(USART_TypeDef* USARTx, char* s);
//USART发送字符串,字符串的长度等于第一个字符的大小
void USART_SendDataStr(USART_TypeDef* USARTx, char *s);
//USART1的中断处理程序
void USART1_IRQ(void);

#endif

