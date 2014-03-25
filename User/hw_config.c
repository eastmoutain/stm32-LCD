/***********************************************

	*配置文件
************************************************/

#include "hw_config.h"
/**
	*函数名称：void NVIC_Configuration(void)
	*
	*入口参数：无
	*
	*出口参数：无
	*
	*功能说明：NVIC初始化配置
	*/
void NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

	/*外部中断线*/
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn ;	  
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE ;
	NVIC_Init(&NVIC_InitStructure);

}

/**
	*函数名称：void LED_Configuration(void)
	*
	*入口参数：无
	*
	*出口参数：无
	*
	*功能说明：LED初始化配置
	*/

void LED_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_LED, ENABLE);
	GPIO_InitStructure.GPIO_Pin = LED1 | LED2 | LED3 | LED4 ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	
	GPIO_Init(GPIO_LedPort,&GPIO_InitStructure);

	/*--------------初始状态四个灯off----------------*/
	LED1_OFF();
	LED2_OFF();
	LED3_OFF();
	LED4_OFF();
		
}

/**
	*函数名称：void BUTTON_Configuration(void)
	*
	*入口参数：无
	*
	*出口参数：无
	*
	*功能说明：BUTTON初始化配置
    */
void BUTTON_Configuration(void)
{
	GPIO_InitTypeDef	GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG,ENABLE);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;  /* 上拉输入模式 */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_Init(GPIOG,&GPIO_InitStructure);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_AFIO,ENABLE);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;  /* 上拉输入模式 */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_Init(GPIOD,&GPIO_InitStructure);
}

/**
	*函数名称：void EXTI_Configuration(void)
	*
	*入口参数：无
	*
	*出口参数：无
	*
	*功能说明：EXTI初始化配置
    */
void EXTI_Configuration(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;
	/*PD3外部中断输入*/
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOD , GPIO_PinSource3);
	EXTI_InitStructure.EXTI_Line = EXTI_Line3; 
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd	= ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
	/*PG7外部中断输入*/
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOG , GPIO_PinSource7);
	EXTI_InitStructure.EXTI_Line = EXTI_Line7;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd	= ENABLE; 
	EXTI_Init(&EXTI_InitStructure);
}




