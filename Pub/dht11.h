#ifndef __DHT11_H__
#define __DHT11_H__
#include "stm32f4xx.h" 

void dht11_init(void);
int32_t dht11_read_data(u8 *pdata);


#endif
