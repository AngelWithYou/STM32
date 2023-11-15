#ifndef _ESP8266_H__
#define _ESP8266_H__
#include "stdio.h"
void Usrat2_init(uint32_t baud);
//自定义发送函数
void USART_SendDatas1(USART_TypeDef* USARTx, uint8_t *Data,uint8_t len);

void Esp8266_Init(void);

extern int temp;
extern int sh;
extern u8 Switch;
extern char Recs[200];
extern int i;
extern int Switch1;
extern u8 Switch2;

extern int flag;

extern char *pub;
extern char *sub ;
extern char *fun1;
extern char *fun2 ;
extern char *fun3 ;
	
u8 Esp_Send(void);
void Esp_Recv(void);
#endif
