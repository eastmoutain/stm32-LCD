#include "stm32f10x.h"
#include "systick.h"


void Delay_Nms(int ms)
{ 
	int i=ms;
	for(i;i>0;i--)
	Delay_Nus(1000);  
}   
//ÑÓÊ±us           
void Delay_Nus(int us)
{ 	int k=us;
	int i=10000;
	for(k;k>0;k--)
	for(i;i>0;i--);
}  
