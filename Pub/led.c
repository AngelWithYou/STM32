#include "led.h"
#include "stm32f4xx.h"                  // Device header

void Led_init(void)
{
	//定义引脚结构体
	GPIO_InitTypeDef GPIO_Init_led;
	
	//1.开启时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF | RCC_AHB1Periph_GPIOE,ENABLE);

	//2.设置引脚
	GPIO_Init_led.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
	//3.设置输入模式
	GPIO_Init_led.GPIO_Mode = GPIO_Mode_OUT;
	//4.设置输入类型
	GPIO_Init_led.GPIO_OType = GPIO_OType_PP;
	//5.设置速率
	GPIO_Init_led.GPIO_Speed = GPIO_Fast_Speed;
	//6.设置上拉
	GPIO_Init_led.GPIO_PuPd = GPIO_PuPd_NOPULL;
	
	GPIO_Init(GPIOF,&GPIO_Init_led);
	
	GPIO_Init_led.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14;
	
	GPIO_Init(GPIOE,&GPIO_Init_led);
	
	//初始全灭
	GPIO_SetBits(GPIOE, GPIO_Pin_14);
	GPIO_SetBits(GPIOE, GPIO_Pin_13);
	GPIO_SetBits(GPIOF, GPIO_Pin_10);
	GPIO_SetBits(GPIOF, GPIO_Pin_9);
	
}


//led控制
void led_ctrl(int num,int status)
{
	switch(num)
	{
		case D1:status?GPIO_SetBits(GPIOF, GPIO_Pin_9):GPIO_ResetBits(GPIOF, GPIO_Pin_9);break;
		case D2:status?GPIO_SetBits(GPIOF, GPIO_Pin_10):GPIO_ResetBits(GPIOF, GPIO_Pin_10);break;
		case D3:status?GPIO_SetBits(GPIOE, GPIO_Pin_13):GPIO_ResetBits(GPIOE, GPIO_Pin_13);break;
		case D4:status?GPIO_SetBits(GPIOE, GPIO_Pin_14):GPIO_ResetBits(GPIOE, GPIO_Pin_14);break;	
	}
}
