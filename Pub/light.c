#include "stm32f4xx.h"
#include "light.h"
#include "usart.h"
#include "delay.h"

//初始化ADC3 PF7
void Light_Init(void)
{
	
	GPIO_InitTypeDef  GPIO_InitStruct;	//初始化GPIOF
	ADC_CommonInitTypeDef ADC_CommonInitStruct;	//ADC常规初始化
	ADC_InitTypeDef ADC_InitStruct;
		

	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3, ENABLE);

	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AN;	//模拟模式
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOF, &GPIO_InitStruct);


	//常规初始化
	ADC_CommonInitStruct.ADC_Mode = ADC_Mode_Independent;	//独立工作模式，只使用一个ADC
	ADC_CommonInitStruct.ADC_Prescaler = ADC_Prescaler_Div2;	//ADC的硬件时钟 PB2 84MHz 二分频
	ADC_CommonInitStruct.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;	//不需要映射数据，直接读取寄存器中的值
	ADC_CommonInitStruct.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;	//5*Tadc
	ADC_CommonInit(&ADC_CommonInitStruct);

	ADC_InitStruct.ADC_Resolution = ADC_Resolution_12b;	//十二位精度
	ADC_InitStruct.ADC_ScanConvMode = DISABLE;	//单个通道
	ADC_InitStruct.ADC_ContinuousConvMode = ENABLE;	//不断进行转换
	ADC_InitStruct.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None; 	//不需要内部脉冲触发adc工作
	ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
	ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;	//右对齐存储
	ADC_InitStruct.ADC_NbrOfConversion = 1;		//只有单个通道
	ADC_Init(ADC3, &ADC_InitStruct);

	//将通道5和ADC关联起来，优先级为 1 1~16 采样时间点为 5*tadc
	ADC_RegularChannelConfig( ADC3, ADC_Channel_5, 1, ADC_SampleTime_3Cycles);

	//使能
	ADC_Cmd(ADC3,ENABLE);
}



uint16_t Read_Light(void)
{

	uint16_t adc_val;
	//等待ADC3转换完成
	while(ADC_GetFlagStatus(ADC3, ADC_FLAG_EOC) == RESET);
	ADC_ClearFlag(ADC3, ADC_FLAG_EOC);

	//得到结果值
	adc_val = ADC_GetConversionValue(ADC3);
	return adc_val;
}
