#ifndef _USART_H__
#define _USART_H__
#include "stdio.h"
void Usrat1_init(uint32_t baud);
//自定义发送函数
void USART_SendDatas(USART_TypeDef* USARTx, uint8_t *Data,uint8_t len);


#endif
