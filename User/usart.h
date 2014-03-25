#ifndef __USART_H_
#define __USART_H_

#include "stm32f10x.h"


//��������
void Init_USART(void);
//USART�����ַ�
void USART_SendChar(USART_TypeDef* USARTx,char dat);
//USART��������
char USART_GetChar(USART_TypeDef* USARTx);
//USART�����ַ���
void USART_SendStr(USART_TypeDef* USARTx, char* s);
//USART�����ַ���,�ַ����ĳ��ȵ��ڵ�һ���ַ��Ĵ�С
void USART_SendDataStr(USART_TypeDef* USARTx, char *s);
//USART1���жϴ������
void USART1_IRQ(void);

#endif

