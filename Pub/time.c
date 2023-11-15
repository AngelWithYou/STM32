#include "stm32f4xx.h"                  // Device header
#include "time.h"
#include "delay.h"
#include "led.h"




////初始化定时器十三
//void time13_init(void)
//{
//	u8 mode = 1;
//	u32 compare_cnt = 0;
//	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
//	//NVIC_InitTypeDef NVIC_InitStructure;
//	TIM_OCInitTypeDef TIM_OCInitStructure;
//	GPIO_InitTypeDef GPIO_Init_led;
//	//定义引脚结构体
//	
//	//int32_t p = 0;
//	//1.开启时钟
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF,ENABLE);
//	GPIO_Init_led.GPIO_Pin = GPIO_Pin_8;
//	GPIO_Init_led.GPIO_Mode = GPIO_Mode_AF;
//	GPIO_Init_led.GPIO_OType = GPIO_OType_PP;
//	GPIO_Init_led.GPIO_Speed = GPIO_Fast_Speed;
//	GPIO_Init_led.GPIO_PuPd = GPIO_PuPd_NOPULL;
//	GPIO_Init(GPIOF,&GPIO_Init_led);
//	
//	
//	//将PF9引脚连接到定时器14
//	GPIO_PinAFConfig(GPIOF,GPIO_PinSource8,GPIO_AF_TIM13);
//	
//	
////	void TIM_TimeBaseInit(TIM_TypeDef* TIMx, TIM_TimeBaseInitTypeDef* TIM_TimeBaseInitStruct)

//	//使能定时器三的时钟
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM13,ENABLE);
//	//配置定时器3的分频值，计数值
//	TIM_TimeBaseInitStruct.TIM_Period = 100-1;	//计数值，  决定输出频率为 1s 决定输出频率为 100 Hz
//	TIM_TimeBaseInitStruct.TIM_Prescaler = 8400 - 1;	//预分频值 （默认加1） 进行一个8400的预分频，进行第一次分屏
//	TIM_TimeBaseInitStruct.TIM_ClockDivision = 0;	//不支持
//	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
//	TIM_TimeBaseInit(TIM13,&TIM_TimeBaseInitStruct);
//	
////	//配置定时器3中断的触发方式
////	TIM_ITConfig(TIM13,TIM_IT_Update,ENABLE);
////	
////	//配置定时器3的中断优先级
////	NVIC_InitStructure.NVIC_IRQChannel = TIM8_UP_TIM13_IRQn;
////	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
////	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
////	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
////	NVIC_Init(&NVIC_InitStructure);

//	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;	//PWM1 计数值小于比较值 通道1有效
//	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
//	TIM_OCInitStructure.TIM_Pulse = 96;
//	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
//	
//	TIM_OC1Init(TIM13,&TIM_OCInitStructure);
//	
//	//使能
//	TIM_Cmd(TIM13,ENABLE);
//	
////	while(1)
////	{
////		for(p = 0; p <=100; p+=5)
////		{
////			TIM_SetCompare1(TIM13,p);
////			my_delayms(4);
////		}
////		for(p = 100; p >=0; p-=5)
////		{
////			TIM_SetCompare1(TIM13,p);
////			my_delayms(4);
////		}
////	}

//	while(1)
//	{
//		if(mode == 1)
//		{
//			compare_cnt+=10;
//			if(compare_cnt == 100)
//			{
//				mode = 0;
//			}
//		}
//		
//		if(mode == 0)
//		{
//			compare_cnt-=10;
//			if(compare_cnt == 0)
//			{
//				mode = 1;
//			}
//		}
//		
//		TIM_SetCompare1(TIM13,compare_cnt);
//		
//	}
//	
//	
//}

//////中断服务函数
//// void TIM8_UP_TIM13_IRQHandler(void)
//// {
////	 //判断标准位
////	 if(TIM_GetITStatus(TIM13,TIM_IT_Update) == SET)
////	 {
////		 GPIO_ToggleBits(GPIOF,GPIO_Pin_9);
////		 GPIO_ToggleBits(GPIOF,GPIO_Pin_10);
////	 }
////	 
////	 //清空标志位
////	 TIM_ClearITPendingBit(TIM13,TIM_IT_Update);
//// }
// 
// 

//void Tim1_Init(void)      //用定时器一控制PE13,PE14的pwm方波输出
//{

//	//初始化GPIO口
//	//开始GPIO时钟
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
//	GPIO_InitTypeDef GPIO_InitStruct;
//	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
//	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14;
//	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
//	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
//	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(GPIOE, &GPIO_InitStruct);

//	//将GPIO口复用为定时器1
//	GPIO_PinAFConfig(GPIOE, GPIO_PinSource13, GPIO_AF_TIM1);
//	GPIO_PinAFConfig(GPIOE, GPIO_PinSource14, GPIO_AF_TIM1);

//	//开启定时器时钟
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);	//168MHz
//	//配置基本定时器
//	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
//	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;	//时钟分频因子，用于将定时器时钟分频
//	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;	//计数方式为向上计数
//	TIM_TimeBaseInitStruct.TIM_Prescaler = 16800 - 1;	//设置预分频值
//	TIM_TimeBaseInitStruct.TIM_Period = (10000) - 1;//设置计数值  （1s钟）
//	TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;	//没有重复计数值
//	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseInitStruct);
//	
//	//配置定时器1的中断方式
//	TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);

//	NVIC_InitTypeDef  NVIC_InitStruct;
//	NVIC_InitStruct.NVIC_IRQChannel = TIM1_UP_TIM10_IRQn;
//	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 2;	//抢占优先级
//	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 2 ; 	//响应优先级
//	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE ;	//使能
//	NVIC_Init(&NVIC_InitStruct);


//	//配置通道三和通道四为pwm
//	TIM_OCInitTypeDef TIM_OCInitStruct;
//	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1; 	//pwm1小与比较值为有效电平；
//	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_Low; //有效电平为低电平
//	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;	//输出通道使能
//	TIM_OCInitStruct.TIM_Pulse = 9998;	//设置比较值；
//	TIM_OC3Init(TIM1,&TIM_OCInitStruct);
//	TIM_OC4Init(TIM1,&TIM_OCInitStruct);

//	//使能定时器1
//	TIM_Cmd(TIM1, ENABLE);
//}

////tim1中断函数
//void TIM1_UP_TIM10_IRQHandler(void)
//{

//	if(TIM_GetITStatus(TIM1,TIM_IT_Update) == SET)

//		{

//			GPIO_ToggleBits(GPIOF, GPIO_Pin_9);
//		}
//	TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
//}

//初始化定时器十三
void time9_init_led(void)
{
//	u8 mode = 1;
//	u32 compare_cnt = 0;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	//NVIC_InitTypeDef NVIC_InitStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	GPIO_InitTypeDef GPIO_Init_led;
	//定义引脚结构体
	
	//int32_t p = 0;
	//1.开启时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);
	GPIO_Init_led.GPIO_Pin = GPIO_Pin_5;
	GPIO_Init_led.GPIO_Mode = GPIO_Mode_AF;
	GPIO_Init_led.GPIO_OType = GPIO_OType_PP;
	GPIO_Init_led.GPIO_Speed = GPIO_Fast_Speed;
	GPIO_Init_led.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOE,&GPIO_Init_led);
	
	
	//将PF9引脚连接到定时器14
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource5,GPIO_AF_TIM9);
	
	
//	void TIM_TimeBaseInit(TIM_TypeDef* TIMx, TIM_TimeBaseInitTypeDef* TIM_TimeBaseInitStruct)

	//使能定时器三的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM9,ENABLE);
	//配置定时器3的分频值，计数值
	TIM_TimeBaseInitStruct.TIM_Period = 100-1;	//计数值，  决定输出频率为 1s 决定输出频率为 100 Hz
	TIM_TimeBaseInitStruct.TIM_Prescaler = 16800 - 1;	//预分频值 （默认加1） 进行一个8400的预分频，进行第一次分屏
	TIM_TimeBaseInitStruct.TIM_ClockDivision = 0;	//不支持
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM9,&TIM_TimeBaseInitStruct);
	
//	//配置定时器3中断的触发方式
//	TIM_ITConfig(TIM13,TIM_IT_Update,ENABLE);
//	
//	//配置定时器3的中断优先级
//	NVIC_InitStructure.NVIC_IRQChannel = TIM8_UP_TIM13_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_Init(&NVIC_InitStructure);

	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;	//PWM1 计数值小于比较值 通道1有效
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 100;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	
	TIM_OC1Init(TIM9,&TIM_OCInitStructure);
	
	//使能
	TIM_Cmd(TIM9,ENABLE);
	
//	while(1)
//	{
//		for(p = 0; p <=100; p+=5)
//		{
//			TIM_SetCompare1(TIM13,p);
//			my_delayms(4);
//		}
//		for(p = 100; p >=0; p-=5)
//		{
//			TIM_SetCompare1(TIM13,p);
//			my_delayms(4);
//		}
//	}

//	while(1)
//	{
//		if(mode == 1)
//		{
//			compare_cnt+=10;
//			if(compare_cnt == 100)
//			{
//				mode = 0;
//			}
//		}
//		
//		if(mode == 0)
//		{
//			compare_cnt-=10;
//			if(compare_cnt == 0)
//			{
//				mode = 1;
//			}
//		}
//		
//		TIM_SetCompare1(TIM9,compare_cnt);
//		delay_ms(100);
//		
//	}
	
	
}
