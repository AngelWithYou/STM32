#include "stm32f4xx.h"   
#include "key_exit.h"
#include "led.h"
#include "beep.h"
#include "delay.h"
#include "usart.h"
#include "esp8266.h"



//{
//	int i;
//	while(m--)
//	{
//		for(i = 0 ; i < 0x2000 ; i++);
//	}
//}


//初始化按键
void key_exti_init(void)
{
	//1.配置GPIO控制器为输入模式
	GPIO_InitTypeDef GPIO_Init_key;
	EXTI_InitTypeDef EXTI_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE | RCC_AHB1Periph_GPIOA,ENABLE);
	GPIO_Init_key.GPIO_Mode = GPIO_Mode_IN;
	GPIO_Init_key.GPIO_OType = GPIO_OType_PP;
	GPIO_Init_key.GPIO_Pin = GPIO_Pin_0;
	GPIO_Init_key.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init_key.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA,&GPIO_Init_key);
	
	GPIO_Init_key.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4;
	GPIO_Init(GPIOE,&GPIO_Init_key);

	
	
	//2.配置SYSCFG选择器
	//开启时钟总线AHB2
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	//初始化选择器具体的输入来源
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource2);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource3);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource4);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0);
	
	
	

	//3.初始化exti外部中断控制器（是外设，也需要使能时钟但是默认打开）
	//定义结构体
	
	//要初始化的外部中断线
	EXTI_InitStruct.EXTI_Line = EXTI_Line0|EXTI_Line2|EXTI_Line3|EXTI_Line4;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStruct);

	//4.NVIC 控制器配置
	
	//指定中断通道
	NVIC_InitStruct.NVIC_IRQChannel = EXTI0_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);
	NVIC_InitStruct.NVIC_IRQChannel = EXTI2_IRQn;
	NVIC_Init(&NVIC_InitStruct);
	NVIC_InitStruct.NVIC_IRQChannel = EXTI3_IRQn;
	NVIC_Init(&NVIC_InitStruct);
	NVIC_InitStruct.NVIC_IRQChannel = EXTI4_IRQn;
	NVIC_Init(&NVIC_InitStruct);
	
}




void EXTI0_IRQHandler(void)
{

	if(EXTI_GetITStatus(EXTI_Line0) == SET)	//进入中断函数执行
		{
			while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0) == 0);
			//按键1按下 判断值
			if(Switch == 0)		//开灯
			{
				Switch = 1;
				flag = 2;		//进入滑动调节模式
				Switch1 = 99;
				Switch2 = 0;
				
			}
			else if(Switch == 1)		//关灯
			{
				Switch = 0;
				Switch1 = 0;
				Switch2 = 0;
				flag = 1; 
			}
			
			
			delay_ms(10);
			
		}
	EXTI_ClearITPendingBit(EXTI_Line0);
}

	//中断执行函数
void EXTI2_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line2) == SET)	//进入中断函数执行
		{
			while(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_2) == 0);
			//控制灯亮度
			flag = 2;
			Switch1 += 10;
			if(Switch1 > 95)
			{
				Switch1 = 11;
			}
			
			delay_ms(10);
		}
	EXTI_ClearITPendingBit(EXTI_Line2);
}


void EXTI3_IRQHandler(void)
{

	if(EXTI_GetITStatus(EXTI_Line3) == SET)	//进入中断函数执行
		{
			while( GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_3) == 0);
			//控制自动还是手动
			if(Switch2 == 0)
			{
				Switch2 = 1;
			}
			delay_ms(10);
			
		}
	EXTI_ClearITPendingBit(EXTI_Line3);
}



void EXTI4_IRQHandler(void)
{

	if(EXTI_GetITStatus(EXTI_Line4) == SET)	//进入中断函数执行
		{
	
			while( GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4) == 0);
			
			//反转led1的电平
			GPIO_ToggleBits(GPIOF,GPIO_Pin_9);
			GPIO_ToggleBits(GPIOF,GPIO_Pin_10);
			GPIO_ToggleBits(GPIOE,GPIO_Pin_13);
			GPIO_ToggleBits(GPIOE,GPIO_Pin_14);
			
		}
	EXTI_ClearITPendingBit(EXTI_Line4);
}





