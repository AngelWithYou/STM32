#include "stm32f4xx.h"                  // Device header
#include "hc-sr501.h"
#include "led.h"
#include "esp8266.h"
#include "delay.h"

//初始化人体感应
void Hc_sr501_Init(void)
{
	//开启GPIO PC8；
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);
	
	GPIO_InitTypeDef GPIO_Initstruct;
	
	//PC8 输入
	GPIO_Initstruct.GPIO_Mode = GPIO_Mode_IN;
	GPIO_Initstruct.GPIO_Pin = GPIO_Pin_8;
	GPIO_Initstruct.GPIO_OType = GPIO_OType_PP;
	GPIO_Initstruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Initstruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&GPIO_Initstruct);
	
}


int Hc_sr501_Start(void)
{
	
	if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_8) == 1)
	{
		led_ctrl(D1,led_on);
		
	}
	else
	{
		if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_8) == 0)
		{
			return 0;
		
		}
	}
	return 1;
}
