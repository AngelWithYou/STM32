#include "stm32f4xx.h"                  // Device header
#include "led.h"
#include "beep.h"
#include "key_exit.h"
#include "delay.h"
#include "time.h"
#include "usart.h"
#include "stdio.h"
#include "string.h"
#include "esp8266.h"
#include "string.h"
#include "dht11.h"
#include "stdlib.h"
#include "light.h"
#include "OLED_I2C.h"

#include "hc_sr04.h"
#include "hc-sr501.h"
#include "led.h"
#include "key_exit.h"

int temp;
int sh;
u8 Switch =0;		//总开关
int Switch1 = 0;	//滑动开关
u8 Switch2 =0;		//自动开关
int i = 0;
int compare_cnt;		//pwm 重载值
u16 val; 	//光敏电阻初始量
int data;		//距离	

int flag = 1;	//模式标准位

//重写printf
int fputc(int c,FILE *stream)
{
	USART_SendData(USART2,c & 0xFF);
	while(USART_GetFlagStatus(USART2,USART_FLAG_TXE) == RESET);
	
	return 0;
}


int main(void)
{
	
	NVIC_PriorityGroupConfig (NVIC_PriorityGroup_2);
	int32_t re;
	u8 buf[5]={0};
	delay_init(168);
	Led_init();
	key_exti_init();
	Usrat1_init(115200);
	Hc_sr04_Init();	//超声波初始化
	Hc_sr501_Init();	//人体红外初始化
	time9_init_led();	//初始化台灯，默认为0
	Light_Init();	//初始化 光敏电阻
	Usrat2_init(115200);	//初始化串口2，用于wife与云端通信
	dht11_init();		//初始化温度
	
	I2C_Configuration();	//oled IIC通信配置
	OLED_Init();			//oled 初始化
	OLED_CLS();				//清屏
	OLED_ShowCN(0, 0, 0);
	OLED_ShowCN(16, 0, 1);
	OLED_ShowCN(32, 0, 2);
	OLED_ShowCN(48, 0, 3);
	OLED_ShowCN(64, 0, 4);
	
	
	OLED_ShowCN(0, 2, 9);
	OLED_ShowCN(16, 2, 10);		//温度
	OLED_ShowCN(0, 4, 11);
	OLED_ShowCN(16, 4, 12);		//湿度
	
	OLED_ShowCN(0, 6, 15);
	OLED_ShowCN(16, 6, 16);		//状态
	OLED_ShowCN(32, 6, 4);
	
	OLED_ShowCN(64, 2, 21);
	OLED_ShowCN(80, 2, 22);

	Esp8266_Init();		//初始化 8266 用于发送AT指令
	beep_init();		//初始化 蜂鸣器
	

	ADC_SoftwareStartConv(ADC3);	//打开ADC
	

	while(1)
	{
		data = Hc_sro4_Start();	//读取超声波距离
		TIM_SetCompare1(TIM9,100-compare_cnt);
		re=dht11_read_data(buf);	//读取温度值与湿度值
		int sta = Hc_sr501_Start(); 	//人体感应
		val = Read_Light();		//读取光敏电阻
		
		
		//判断状态
		if(sta == 0)
		{
			OLED_ShowCN(48, 6, 19);
			OLED_ShowCN(64, 6, 20);
			Switch =0;		//总开关
			Switch1 = 0;	//滑动开关
			Switch2 =0;		//自动开关
			flag = 1;
			led_ctrl(D1,led_off);
		}
		else
		{
			OLED_ShowCN(48, 6, 17);
			OLED_ShowCN(64, 6, 18);
		}
		
		
		
		//判断距离
		if(data < 15)
		{
			OLED_ShowCN(96, 2, 23);
			OLED_ShowCN(112, 2, 24);
			beep_ctrl(Beep_on);		
		}
		else
		{
			OLED_ShowCN(96, 2, 25);
			OLED_ShowCN(112, 2, 26);
			beep_ctrl(Beep_off);
		}
		
		
		
		
		//判断温度检测是否正常
		if(re==0)
			{
				temp = buf[2];	//温度整数部分
				sh = buf[0];	//温度小数部分
				Esp_Send();		//将温度值上传到云端
			}
		else
			{
				printf("AT+MQTTPUB=0,\"/k0e5qQifHja/ESP8266/user/update\",\"dht11 error\",1,0\r\n");
			}
			
			
			
		//判断是否进入自动模式	
		if(Switch2 == 1)
			{
				flag = 3;
			}
			
			
		//选择
		switch(flag)
		{
				//云端按键控制
			case 1:
				Esp_Recv();		//接收云端信息
			
				OLED_ShowCN(80, 0, 5);
				OLED_ShowCN(96, 0, 6);
				if(Switch == 0)		//开关 关
				{
					compare_cnt = 0;
				}
				if(Switch == 1)	//打开开关，进入亮度可调节
				{
					flag = 2;
				}
			break;
				
				//滑动调节亮度
			case 2:
				
				Esp_Recv();		//接收云端信息
				if(Switch1 > 10)
				{
					compare_cnt = Switch1;
				}
				
				if(Switch == 0)		//关闭灯，灯灭
				{
					compare_cnt = 0;
					Switch1 = 0;
				}
				
				break;
				
				
				
				
			//自动模式
			case 3:
				
				OLED_ShowCN(80, 0, 7);
				OLED_ShowCN(96, 0, 8);
				compare_cnt = val/40;	//光PWM 重载值 
			
				Esp_Recv();		//接收云端信息
			
				if(Switch2 == 0)
				{
					flag = 1;
				}
			break;
				
				
		
		}
		char data_wd[20];
		char data_sd[20];
		char data_light[20];
		
		
		sprintf(data_wd,":%d",buf[2]);
		OLED_ShowStr(33,2,(unsigned char*)data_wd,2);
		sprintf(data_sd,":%d",buf[0]);
		OLED_ShowStr(33,4,(unsigned char*)data_sd,2);
		
		
		sprintf(data_light,"light:%d",compare_cnt);
		OLED_ShowStr(70,5,(unsigned char*)data_light,1);
		
		
	}
}
