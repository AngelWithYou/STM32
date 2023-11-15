//���������ģ��

#include "stm32f4xx.h"                  // Device header
#include "delay.h"
#include "hc_sr04.h"


//��ʼ�� PB6Ϊ���ģʽ PE6Ϊ����ģʽ
void  Hc_sr04_Init(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOE,ENABLE);
	
	GPIO_InitTypeDef GPIO_Initstruct;
	
	//PB6���
	GPIO_Initstruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_Initstruct.GPIO_Pin = GPIO_Pin_6;
	GPIO_Initstruct.GPIO_OType = GPIO_OType_PP;
	GPIO_Initstruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Initstruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_Initstruct);
	GPIO_Initstruct.GPIO_Mode = GPIO_Mode_IN;
	GPIO_Init(GPIOE,&GPIO_Initstruct);
	
	//��ʼ��PB6����͵�ƽ
	GPIO_ResetBits(GPIOB,GPIO_Pin_6);
	
}


//��������
int Hc_sro4_Start(void)
{
	u32 t = 0;
	u32 d = 0;
	//��ʼ�ź�
	GPIO_SetBits(GPIOB,GPIO_Pin_6);	//����PB6,������ʼ�źţ�����10us����
	delay_us(20);
	GPIO_ResetBits(GPIOB,GPIO_Pin_6);
	
	//�ȴ�PE6����ߵ�ƽ
	while(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_6) == 0)
	{
		t++;
		delay_us(1);
		if(t > 10000)
		{
			//����
			return 1;
		}
	}
	
	//�ȴ�PE6�ߵ�ƽ����ʱ��
	t = 0 ;
	while(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_6))
	{
		t++;
		delay_us(27); 	//27us == 1cm;
		if(t > 10000)
		{
			//����
			return 2;
		}
	}
	
	t = t/2;	//����ʱ��
	d = t*1;	//���� cm
	
	return d;
	
}
