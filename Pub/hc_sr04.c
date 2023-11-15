//超声波测距模块

#include "stm32f4xx.h"                  // Device header
#include "delay.h"
#include "hc_sr04.h"


//初始化 PB6为输出模式 PE6为输入模式
void  Hc_sr04_Init(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOE,ENABLE);
	
	GPIO_InitTypeDef GPIO_Initstruct;
	
	//PB6输出
	GPIO_Initstruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_Initstruct.GPIO_Pin = GPIO_Pin_6;
	GPIO_Initstruct.GPIO_OType = GPIO_OType_PP;
	GPIO_Initstruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Initstruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_Initstruct);
	GPIO_Initstruct.GPIO_Mode = GPIO_Mode_IN;
	GPIO_Init(GPIOE,&GPIO_Initstruct);
	
	//初始化PB6输出低电平
	GPIO_ResetBits(GPIOB,GPIO_Pin_6);
	
}


//测量函数
int Hc_sro4_Start(void)
{
	u32 t = 0;
	u32 d = 0;
	//起始信号
	GPIO_SetBits(GPIOB,GPIO_Pin_6);	//拉高PB6,发出起始信号，持续10us以上
	delay_us(20);
	GPIO_ResetBits(GPIOB,GPIO_Pin_6);
	
	//等待PE6输出高电平
	while(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_6) == 0)
	{
		t++;
		delay_us(1);
		if(t > 10000)
		{
			//出错
			return 1;
		}
	}
	
	//等待PE6高电平产生时间
	t = 0 ;
	while(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_6))
	{
		t++;
		delay_us(27); 	//27us == 1cm;
		if(t > 10000)
		{
			//出错
			return 2;
		}
	}
	
	t = t/2;	//两倍时间
	d = t*1;	//距离 cm
	
	return d;
	
}
