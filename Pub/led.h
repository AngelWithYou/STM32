#ifndef __LED_H_
#define __LED_H_


//led的状态
enum Led_status
{
	led_on,
	led_off
};

//led的编号
enum Led_num
{
	D1,
	D2,
	D3,
	D4
};


void led_ctrl(int num,int status);
void Led_init(void);
#endif
