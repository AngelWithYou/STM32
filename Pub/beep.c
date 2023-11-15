#include "stm32f4xx.h"                  // Device header
#include "beep.h"


//蜂鸣器初始话
void beep_init(void)
{
	GPIO_InitTypeDef GPIO_Init_beep;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF,ENABLE);
	GPIO_Init_beep.GPIO_Pin = GPIO_Pin_8;
	GPIO_Init_beep.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_Init_beep.GPIO_OType = GPIO_OType_PP;
	GPIO_Init_beep.GPIO_Speed = GPIO_Speed_25MHz;
	GPIO_Init_beep.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOF,&GPIO_Init_beep);
	
	//初始化蜂鸣器为 0
	GPIO_ResetBits(GPIOF,GPIO_Pin_8);
	
}

//蜂鸣器控制
void beep_ctrl(int status)
{
	status ? GPIO_ResetBits(GPIOF,GPIO_Pin_8) : GPIO_SetBits(GPIOF,GPIO_Pin_8);
}
