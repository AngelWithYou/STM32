#include "stm32f4xx.h"
#include "esp8266.h"
#include "stdio.h"
#include "usart.h"
#include "delay.h"
#include "string.h"
#include "stdlib.h"

char Recs[200];
char *pub = "/sys/k0e5qQifHja/ESP8266/thing/event/property/post";			//发布
char *sub = "/sys/k0e5qQifHja/ESP8266/thing/event/property/post_reply";		//订阅
char *fun1 = "temperature";		//温度标记
char *fun2 = "Humidity";		//湿度标记
char *fun3 = "LowPowerSwitch";	//开关标记
char *fun4 = "NightLightSwitch";
char *fun5 = "PowerSwitch";



void Usrat2_init(uint32_t baud)
{
	
	GPIO_InitTypeDef GPIO_Initstruct;
	USART_InitTypeDef USART_Initstruct;
	NVIC_InitTypeDef NVIC_Initstruct;
	
	
	//1.打开PA硬件时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	
	//2.打开串口1硬件时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	
	//3.配置pa9和pa10为复用功能模式
	GPIO_Initstruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_Initstruct.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_Initstruct.GPIO_OType = GPIO_OType_PP;
	GPIO_Initstruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Initstruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_Initstruct);
	
	//将串口与pa9 pa10相连接
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_USART2);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_USART2);
	
	//4.配置串口1的 波特率 无校验位 8位数据位 1个停止位
	USART_Initstruct.USART_BaudRate = baud;					//波特率
	USART_Initstruct.USART_WordLength = USART_WordLength_8b;	//8个数据位
	USART_Initstruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//没有硬件流控制
	USART_Initstruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//可以收发数据
	USART_Initstruct.USART_StopBits = USART_StopBits_1;				//一位停止位
	USART_Initstruct.USART_Parity = USART_Parity_No;				//没有奇偶校验位
	USART_Init(USART2,&USART_Initstruct);
	
	
	
	//5.配置串口1的中断触发方式：接收一个字节触发中断
	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);
	
	//6.配置串口1的中断优先级
	NVIC_Initstruct.NVIC_IRQChannel = USART2_IRQn;
	NVIC_Initstruct.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_Initstruct.NVIC_IRQChannelSubPriority = 2;
	NVIC_Initstruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_Initstruct);
	
	//7.使能串口工作
	USART_Cmd(USART2,ENABLE);
	
	
}

void Esp8266_Init(void)
{
	printf("AT+RST\r\n");
	delay_ms(3000);

	printf("AT+RESTORE\r\n");
	delay_ms(3000);
	
	printf("AT+CWMODE=1\r\n");
	delay_ms(3000);
	
	printf("AT+CIPSNTPCFG=1,8,\"ntp1.aliyun.com\"\r\n");
	delay_ms(3000);
	
	printf("AT+CWJAP=\"ESP8266\",\"123456789\"\r\n");
	delay_ms(7000);
	
	printf("AT+MQTTUSERCFG=0,1,\"NULL\",\"ESP8266&k0e5qQifHja\",\"c045703158dbf6b2f22731f18ce32fc1fd5b2ff1e944bbc75f6084b97599f232\",0,0,\"\"\r\n");
	delay_ms(3000);
	
	printf("AT+MQTTCLIENTID=0,\"k0e5qQifHja.ESP8266|securemode=2\\,signmethod=hmacsha256\\,timestamp=1698628776978|\"\r\n");
	delay_ms(3000);
	
	printf("AT+MQTTCONN=0,\"iot-06z00es4p0tuith.mqtt.iothub.aliyuncs.com\",1883,1\r\n");
	delay_ms(3000);
	
	
	printf("AT+MQTTSUB=0,\"/k0e5qQifHja/ESP8266/user/get\",1\r\n");
	delay_ms(3000);
}


u8 Esp_Send(void)
{
	memset(Recs,0,sizeof(Recs));
	printf("AT+MQTTPUB=0,\"%s\",\"{\\\"method\\\":\\\"thing.event.property.post\\\"\\,\\\"params\\\":{\\\"%s\\\":%d\\,\\\"%s\\\":%d\\,\\\"%s\\\":%d\\,\\\"%s\\\":%d\\,\\\"%s\\\":%d}}\",0,0\r\n",pub,fun1,temp,fun2,sh,fun4,Switch1,fun3,Switch,fun5,Switch2);
	delay_ms(3000);
	return 1;
}

void Esp_Recv(void)
{
	if(strncmp(Recs,"+MQTTSUBRECV:",13) == 0)
	{
	u8 j = 0;
	while(Recs[j++] != '\0')
	{
		if(strncmp((Recs+j),fun3,14) == 0)
		{
			while(Recs[j++] != ':');
			Switch = Recs[j] - 48;
		}
		if(strncmp((Recs+j),fun4,16) == 0)
		{
			while(Recs[j++] != ':');
			int a = Recs[j] - 48;
			int b = Recs[j+1] - 48;
			Switch1 = a*10+b;
		}
		
		if(strncmp((Recs+j),fun5,11) == 0)
		{
			while(Recs[j++] != ':');
			Switch2 = Recs[j] - 48;
		}
		
	}
}
	memset(Recs,0,sizeof(Recs));
}

void USART2_IRQHandler(void)
{
//	u8 c;
//	if(USART_GetITStatus(USART2,USART_IT_RXNE) == SET)
//	{
//		c = USART_ReceiveData(USART2);
//		USART_SendData(USART1,c);
//		while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
//		USART_ClearFlag(USART1, USART_FLAG_TXE);
//	}
//	USART_ClearITPendingBit(USART2,USART_IT_RXNE);
	
	if(USART_GetITStatus(USART2,USART_IT_RXNE) == SET)
	{
		Recs[i++] = USART_ReceiveData(USART2);
		if((Recs[i-2] == '\r')|(Recs[i-1] == '\n')) 
		{
			Recs[i-2] = '\0';
			i = 0;
		}
	}
	USART_ClearITPendingBit(USART2,USART_IT_RXNE);
}




//自定义发送函数
void USART_SendDatas1(USART_TypeDef* USARTx, uint8_t *Data,uint8_t len)
	{
		int i;
		for(i=0;i<len;i++)
		{
			while(USART_GetFlagStatus(USARTx,USART_FLAG_TXE) == RESET);
			USART_SendData(USARTx,Data[i]);
		}
	}

	






