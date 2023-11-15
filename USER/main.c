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
u8 Switch =0;		//�ܿ���
int Switch1 = 0;	//��������
u8 Switch2 =0;		//�Զ�����
int i = 0;
int compare_cnt;		//pwm ����ֵ
u16 val; 	//���������ʼ��
int data;		//����	

int flag = 1;	//ģʽ��׼λ

//��дprintf
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
	Hc_sr04_Init();	//��������ʼ��
	Hc_sr501_Init();	//��������ʼ��
	time9_init_led();	//��ʼ��̨�ƣ�Ĭ��Ϊ0
	Light_Init();	//��ʼ�� ��������
	Usrat2_init(115200);	//��ʼ������2������wife���ƶ�ͨ��
	dht11_init();		//��ʼ���¶�
	
	I2C_Configuration();	//oled IICͨ������
	OLED_Init();			//oled ��ʼ��
	OLED_CLS();				//����
	OLED_ShowCN(0, 0, 0);
	OLED_ShowCN(16, 0, 1);
	OLED_ShowCN(32, 0, 2);
	OLED_ShowCN(48, 0, 3);
	OLED_ShowCN(64, 0, 4);
	
	
	OLED_ShowCN(0, 2, 9);
	OLED_ShowCN(16, 2, 10);		//�¶�
	OLED_ShowCN(0, 4, 11);
	OLED_ShowCN(16, 4, 12);		//ʪ��
	
	OLED_ShowCN(0, 6, 15);
	OLED_ShowCN(16, 6, 16);		//״̬
	OLED_ShowCN(32, 6, 4);
	
	OLED_ShowCN(64, 2, 21);
	OLED_ShowCN(80, 2, 22);

	Esp8266_Init();		//��ʼ�� 8266 ���ڷ���ATָ��
	beep_init();		//��ʼ�� ������
	

	ADC_SoftwareStartConv(ADC3);	//��ADC
	

	while(1)
	{
		data = Hc_sro4_Start();	//��ȡ����������
		TIM_SetCompare1(TIM9,100-compare_cnt);
		re=dht11_read_data(buf);	//��ȡ�¶�ֵ��ʪ��ֵ
		int sta = Hc_sr501_Start(); 	//�����Ӧ
		val = Read_Light();		//��ȡ��������
		
		
		//�ж�״̬
		if(sta == 0)
		{
			OLED_ShowCN(48, 6, 19);
			OLED_ShowCN(64, 6, 20);
			Switch =0;		//�ܿ���
			Switch1 = 0;	//��������
			Switch2 =0;		//�Զ�����
			flag = 1;
			led_ctrl(D1,led_off);
		}
		else
		{
			OLED_ShowCN(48, 6, 17);
			OLED_ShowCN(64, 6, 18);
		}
		
		
		
		//�жϾ���
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
		
		
		
		
		//�ж��¶ȼ���Ƿ�����
		if(re==0)
			{
				temp = buf[2];	//�¶���������
				sh = buf[0];	//�¶�С������
				Esp_Send();		//���¶�ֵ�ϴ����ƶ�
			}
		else
			{
				printf("AT+MQTTPUB=0,\"/k0e5qQifHja/ESP8266/user/update\",\"dht11 error\",1,0\r\n");
			}
			
			
			
		//�ж��Ƿ�����Զ�ģʽ	
		if(Switch2 == 1)
			{
				flag = 3;
			}
			
			
		//ѡ��
		switch(flag)
		{
				//�ƶ˰�������
			case 1:
				Esp_Recv();		//�����ƶ���Ϣ
			
				OLED_ShowCN(80, 0, 5);
				OLED_ShowCN(96, 0, 6);
				if(Switch == 0)		//���� ��
				{
					compare_cnt = 0;
				}
				if(Switch == 1)	//�򿪿��أ��������ȿɵ���
				{
					flag = 2;
				}
			break;
				
				//������������
			case 2:
				
				Esp_Recv();		//�����ƶ���Ϣ
				if(Switch1 > 10)
				{
					compare_cnt = Switch1;
				}
				
				if(Switch == 0)		//�رյƣ�����
				{
					compare_cnt = 0;
					Switch1 = 0;
				}
				
				break;
				
				
				
				
			//�Զ�ģʽ
			case 3:
				
				OLED_ShowCN(80, 0, 7);
				OLED_ShowCN(96, 0, 8);
				compare_cnt = val/40;	//��PWM ����ֵ 
			
				Esp_Recv();		//�����ƶ���Ϣ
			
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
