# 基于STM32系列的智能台灯系统

> 制作人：梁振							时间： 2023年11月5日

### 1.概述

> 本设计是基于一款STM32的物联网智能台灯设计，使用STM32F407最小系统作为主控，主要功能如下：
>
> * 使用ESP8266_WIFI模块与云平台通信进行PC端的远程控制，利用AT指令控制esp8266进行网络通信，使用mqtt协议与与平台交互
> * 用光敏电阻检测环境关照强度，使用模数转换（ADC)  ,通过PWM调节占空比自动调节台灯的亮度
> * 使用US-100超声波传感器检测人与台灯的距离判断坐姿是否异常。异常蜂鸣器会提醒
> * 利用DHT11温湿度检测模块，测量环境的温度与湿度，并且实时上传到云端，通过表盘和折线图显示
> * 利用人体红外传感器，检测到人体离开一段时间后自动关灯
> * 利用人体0.96寸的OLED显示屏来实时显示温度，湿度，坐姿是否异常和是否有人等信息，使用按键来控制灯的开关，亮度的调节。

### 2.开发环境及使用的模块

> **keil5,source insight,STM32F407ZET6,FlyMCU**
>
> * source insight:负责代码的编写
> * keil5:负责编译
> * FlyMCU：实现烧录
> * STM32F407ZET6: 实现功能

### 3.具体的开发流程

#### 3.1 ESP8266-wife及 云端控制界面 的初始化

>  **云端控制**
>
>  > 注册一个阿里云服务器账号，使用其物联网平台，添加一个属于自己的产品和设备，记录其各个部分参数，以便后续的操作，使用物联网应用开发平台，设计一个控制界面，跟产品的参数绑定，以达到远程参数的上传和控制。
>
>  **esp8266模块的初始化 **
>
>  > 使用的官网下载的MQTT协议固件库，自己将固件烧录完成后，便可以同过AT指令连接WIFI接入阿里云，AT指令的发送使用开发的的串口2.

#### 3.2 光敏电阻传感器的初始化

> 光敏电阻传感器需要使用开发板的ADC(模数准换通道) ，光线越大，电阻越小，电压的值会越大，通过测出来的值，经过运算后直接通过全局变量将数值传给 PWM 重载值，改变占空比，实现小灯亮度控制。

#### 3.3 US-超声波模块的初始化

> 超声波模块原理为一个孔发出声波，另外一个孔接收到声波便会发出高电平，将GPIO口设置成输入，读取到到高电平变开始计时，设声速为 340m/s 乘以高电平的时间，就可以计算出距离了。

#### 3.4 DHT11温湿度传感器的初始化

> DHT11 使用的数据传输协议为严格的IO时序，除开VCC 和 GND 以外，只有一个数据传输线，先将主机的GPIO口设置为输出模式，向传感器发送开始检测信号，当传感器接收到开始信号后，会发检测的数据，通过微妙级的延时，判断bit位，实现数据的传输，这时我们因该把GPIO口设置为输入，去读取它的数据，最后打印出来。
>
> 具体的时序图如下：
>
> ![1699185131135](C:\Users\86183\AppData\Roaming\Typora\typora-user-images\1699185131135.png)
>
> ![1699185173008](C:\Users\86183\AppData\Roaming\Typora\typora-user-images\1699185173008.png)

#### 3.5 红外人体检测模块初始化

> 人体红外传感器时用探头检测到人体发出的热红外线来判断是否有人，只要有人，便会向引脚上输出一个高电平，反之则是低电平，每个一段时间去读取电平，变可以判断是否有人。

### 4.主要的功能函数及代码的整合

> esp8266实现的数据接收函数的处理，由于使用的串口通信，只能一个数据一个数据的发送，所以要把一段数据接收，需要将一段数据整合到一个数组中，具体实现代码如下：
>
> ```c
> //使用的串口2，中断服务函数中接收信号
> //由于数据以/r/n结尾，所以可以作为判断条件
> void USART2_IRQHandler(void)
> {
> 
> 	if(USART_GetITStatus(USART2,USART_IT_RXNE) == SET)
> 	{
> 		Recs[i++] = USART_ReceiveData(USART2);
> 		if((Recs[i-2] == '\r')|(Recs[i-1] == '\n')) 
> 		{
> 			Recs[i-2] = '\0';
> 			i = 0;
> 		}
> 	}
> 	USART_ClearITPendingBit(USART2,USART_IT_RXNE);
> }
> 
> ```
>
> 云端数据的发送，发送的信号会经过esp8266通过串口反馈给单片机，但是使用的反馈的信息会加上数据的来源编号和订阅号，也需要我们自己去接收解析，具体代码如下：
>
> ```c
> //每个接收信号前会加上+MQTTSUBRECV:这串字符，可以作为接收信号的判断条件
> //每个信号都有属于自己的独特的标记，使用strncmp便可以轻松定位到响应的标记，最后读取：后的值，便是数据
> char *fun1 = "temperature";		//温度标记
> char *fun2 = "Humidity";		//湿度标记
> char *fun3 = "LowPowerSwitch";	//开关标记
> char *fun4 = "NightLightSwitch";
> char *fun5 = "PowerSwitch";
> void Esp_Recv(void)
> {
> 	if(strncmp(Recs,"+MQTTSUBRECV:",13) == 0)
> 	{
> 	u8 j = 0;
> 	while(Recs[j++] != '\0')
> 	{
> 		if(strncmp((Recs+j),fun3,14) == 0)
> 		{
> 			while(Recs[j++] != ':');
> 			Switch = Recs[j] - 48;
> 		}
> 		if(strncmp((Recs+j),fun4,16) == 0)
> 		{
> 			while(Recs[j++] != ':');
> 			int a = Recs[j] - 48;
> 			int b = Recs[j+1] - 48;
> 			Switch1 = a*10+b;
> 		}
> 		
> 		if(strncmp((Recs+j),fun5,11) == 0)
> 		{
> 			while(Recs[j++] != ':');
> 			Switch2 = Recs[j] - 48;
> 		}
> 		
> 	}
> }
> 	memset(Recs,0,sizeof(Recs));
> }
> ```
>
> 主函数的代码：
>
> ```c
> //主要采用的标志位方法，实现各个模式之间的转换过程
> 
> //选择
> 		switch(flag)
> 		{
> 				//云端按键控制
> 			case 1:
> 				Esp_Recv();		//接收云端信息
> 			
> 				OLED_ShowCN(80, 0, 5);
> 				OLED_ShowCN(96, 0, 6);
> 				if(Switch == 0)		//开关 关
> 				{
> 					compare_cnt = 0;
> 				}
> 				if(Switch == 1)	//打开开关，进入亮度可调节
> 				{
> 					flag = 2;
> 				}
> 			break;
> 				
> 				//滑动调节亮度
> 			case 2:
> 				
> 				Esp_Recv();		//接收云端信息
> 				if(Switch1 > 10)
> 				{
> 					compare_cnt = Switch1;
> 				}
> 				
> 				if(Switch == 0)		//关闭灯，灯灭
> 				{
> 					compare_cnt = 0;
> 					Switch1 = 0;
> 				}
> 				break;	
> 			//自动模式
> 			case 3:
> 				
> 				OLED_ShowCN(80, 0, 7);
> 				OLED_ShowCN(96, 0, 8);
> 				compare_cnt = val/40;	//光PWM 重载值 
> 			
> 				Esp_Recv();		//接收云端信息
> 			
> 				if(Switch2 == 0)
> 				{
> 					flag = 1;
> 				}
> 			break;
> 		}
> ```

### 
