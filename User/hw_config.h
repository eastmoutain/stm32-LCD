#ifndef __HW_CONFIG_H_
#define __HW_CONFIG_H_
#include "stm32f10x_conf.h"
/**
   *下面这部分是端口位带操作,参考Cortex-M3权威指南
   */ 
#define GPIOA_ODR_Addr    (GPIOA_BASE+12) //0x4001080C 
#define GPIOB_ODR_Addr    (GPIOB_BASE+12) //0x40010C0C 
#define GPIOC_ODR_Addr    (GPIOC_BASE+12) //0x4001100C 
#define GPIOD_ODR_Addr    (GPIOD_BASE+12) //0x4001140C 
#define GPIOE_ODR_Addr    (GPIOE_BASE+12) //0x4001180C 
#define GPIOF_ODR_Addr    (GPIOF_BASE+12) //0x40011A0C    
#define GPIOG_ODR_Addr    (GPIOG_BASE+12) //0x40011E0C    

#define GPIOA_IDR_Addr    (GPIOA_BASE+8) //0x40010808 
#define GPIOB_IDR_Addr    (GPIOB_BASE+8) //0x40010C08 
#define GPIOC_IDR_Addr    (GPIOC_BASE+8) //0x40011008 
#define GPIOD_IDR_Addr    (GPIOD_BASE+8) //0x40011408 
#define GPIOE_IDR_Addr    (GPIOE_BASE+8) //0x40011808 
#define GPIOF_IDR_Addr    (GPIOF_BASE+8) //0x40011A08 
#define GPIOG_IDR_Addr    (GPIOG_BASE+8) //0x40011E08 
#define PFout(n)	*((volatile unsigned long *)(0x42000000+((GPIOF_ODR_Addr-0x40000000)<<5)+(n<<2)))

/*------------------------LED---------------------------*/
#define RCC_APB2Periph_LED	RCC_APB2Periph_GPIOF
#define GPIO_LedPort	GPIOF
#define LED1	GPIO_Pin_6
#define LED2	GPIO_Pin_7
#define LED3	GPIO_Pin_8
#define LED4	GPIO_Pin_9
#define LED1_OFF()	GPIO_WriteBit(GPIO_LedPort, LED1, Bit_SET)
#define LED1_ON()  	GPIO_WriteBit(GPIO_LedPort, LED1, Bit_RESET)

#define LED2_OFF()	GPIO_WriteBit(GPIO_LedPort, LED2, Bit_SET)
#define LED2_ON()	GPIO_WriteBit(GPIO_LedPort, LED2, Bit_RESET)

#define LED3_OFF()	GPIO_WriteBit(GPIO_LedPort, LED3, Bit_SET)
#define LED3_ON()	GPIO_WriteBit(GPIO_LedPort, LED3, Bit_RESET)

#define LED4_OFF()	GPIO_WriteBit(GPIO_LedPort, LED4, Bit_SET)
#define LED4_ON()	GPIO_WriteBit(GPIO_LedPort, LED4, Bit_RESET)

/*------------------------BUTTON-----------------------*/
#define RCC_APB2Periph_BUTTON	RCC_APB2Periph_GPIOD
#define GPIO_ButtonPort	GPIOD
#define BUTTON1	GPIO_Pin_8

/*----------------------------USART----------------------------*/
/*使用串口之前先打开相应时钟
 * USART		TX		RX
   USART1       PA9     PA10
   USART2		PA2		PA3*/
#define GPIO_UsartPort	GPIOA
#define UsartPort   USART1
#define UsartTX	GPIO_Pin_9
#define UsartRX	GPIO_Pin_10


/* 函数声明 */
void NVIC_Configuration(void);

void LED_Configuration(void);
void BUTTON_Configuration(void);
void EXTI_Configuration(void);
#endif
