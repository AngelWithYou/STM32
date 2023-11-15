#include "stm32f4xx.h"                  // Device header
#include "usart.h"
#include "delay.h"
#include "stdio.h"
#include "string.h"

//usart1 pa9（u1-->tx）pa10(u1 --> rx)
void Usrat1_init(uint32_t baud)
{
	
	GPIO_InitTypeDef GPIO_Initstruct;
	USART_InitTypeDef USART_Initstruct;
	NVIC_InitTypeDef NVIC_Initstruct;
	
	
	//1.打开PA硬件时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	
	//2.打开串口1硬件时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	
	//3.配置pa9和pa10为复用功能模式
	GPIO_Initstruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_Initstruct.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
	GPIO_Initstruct.GPIO_OType = GPIO_OType_PP;
	GPIO_Initstruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Initstruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_Initstruct);
	
	//将串口与pa9 pa10相连接
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1);
	
	//4.配置串口1的 波特率 无校验位 8位数据位 1个停止位
	USART_Initstruct.USART_BaudRate = baud;					//波特率
	USART_Initstruct.USART_WordLength = USART_WordLength_8b;	//8个数据位
	USART_Initstruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//没有硬件流控制
	USART_Initstruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//可以收发数据
	USART_Initstruct.USART_StopBits = USART_StopBits_1;				//一位停止位
	USART_Initstruct.USART_Parity = USART_Parity_No;				//没有奇偶校验位
	USART_Init(USART1,&USART_Initstruct);
	
	
	
	//5.配置串口1的中断触发方式：接收一个字节触发中断
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
	
	//6.配置串口1的中断优先级
	NVIC_Initstruct.NVIC_IRQChannel = USART1_IRQn;
	NVIC_Initstruct.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_Initstruct.NVIC_IRQChannelSubPriority = 2;
	NVIC_Initstruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_Initstruct);
	
	//7.使能串口工作
	USART_Cmd(USART1,ENABLE);
	
}



void USART1_IRQHandler(void)
{
//	u8 Res;
//	while(1)
//	{	
		if(USART_GetITStatus(USART1,USART_IT_RXNE) == SET)
//			{
//				Res = USART_ReceiveData(USART1);
//				if(Res == '\n')
//				{
//					USART_RX_BUF[data_len] = '\0';
//					if(strstr((const char*)USART_RX_BUF,"on"))
//					{
//						GPIO_ResetBits(GPIOF,GPIO_Pin_9);
//					}
//					if(strstr((const char*)USART_RX_BUF,"off"))
//					{
//						GPIO_SetBits(GPIOF,GPIO_Pin_9);
//					}
//					data_len = 0;
		
//				}
//				else
//				{
//					USART_RX_BUF[data_len++] = Res;
//					if(data_len > USART_REC_LEN)
//					{
//						data_len = 0;
//					}
//				}
//			}
			USART_ClearITPendingBit(USART1,USART_IT_RXNE);
//	}	
	
}




//自定义发送函数
void USART_SendDatas(USART_TypeDef* USARTx, uint8_t *Data,uint8_t len)
	{
		int i;
		for(i=0;i<len;i++)
		{
			USART_SendData(USARTx,Data[i]);
			while(USART_GetFlagStatus(USARTx,USART_FLAG_TXE) == RESET);
			USART_ClearFlag(USART1, USART_FLAG_TXE);
			
		}
	}

	
////重写printf
//int fputc(int c,FILE *stream)
//{
//	USART_SendData(USART1,c & 0xFF);
//	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE) == RESET);
//	
//	return 0;
//}
