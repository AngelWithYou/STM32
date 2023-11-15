#include "dht11.h"
#include "stm32f4xx.h" 
#include "delay.h" 
void dht11_init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_9;
	GPIO_InitStruct.GPIO_PuPd=GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOG, &GPIO_InitStruct);

	GPIO_SetBits(GPIOG, GPIO_Pin_9);
}


int32_t dht11_read_data(u8 *pdata)
{
	int t=0;
	int i=0,j=0;
	u8 d=0;
	u32 check_sum=0;
	GPIO_InitTypeDef GPIO_InitStruct;
	//PG9����Ϊ���ģʽ
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_9;
	GPIO_InitStruct.GPIO_PuPd=GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOG, &GPIO_InitStruct);

	GPIO_ResetBits(GPIOG, GPIO_Pin_9);
	delay_ms(20);
	GPIO_SetBits(GPIOG, GPIO_Pin_9);
	delay_us(30);
	
	
	//PG9����Ϊ����ģʽ
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IN;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_9;
	GPIO_InitStruct.GPIO_PuPd=GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOG, &GPIO_InitStruct);
	//�ȴ��͵�ƽ����
	t=0;
	while(GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_9))
		{
			t++;
			delay_us(1);
			if(t>4000)
				{
					return -1;
				}
		}
	//�����͵�ƽ�ĺϷ���
	t=0;
	while(!GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_9))
		{
			t++;//����ʱ����
			delay_us(1);
			if(t>100)
				{
					return -2;
				}
		}
	//�����ߵ�ƽ�ĺϷ���
	t=0;
	while(GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_9))
		{
			t++;//����ʱ����
			delay_us(1);
			if(t>100)
				{
					return -3;
				}
		}

	//�ź�ƥ�䣬���Խ�������
	for(j=0;j<5;j++)
		{
			d=0;
			for(i=7;i>=0;i--)
			{
				t=0;
				while(!GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_9))
					{
						t++;//����ʱ����
						delay_us(1);
						if(t>100)
							{
								return -4;
							}
					}
				delay_us(40);
				if(GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_9))
					{
						d|=1<<i;
						t=0;
						//�ȴ��ߵ�ƽ�������
						while(GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_9))
							{
								t++;//����ʱ����
								delay_us(1);
								if(t>1000)
									{
										return -5;
									}
							}
					}
			}
		pdata[j]=d;
		}

	//����У���
	check_sum=(pdata[0]+pdata[1]+pdata[2]+pdata[3])&0xFF;
	if(pdata[4]!=check_sum)
		{
			return -6;
		}
	return 0;
}
