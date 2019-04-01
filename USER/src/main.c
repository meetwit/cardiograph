#include "stdio.h"
#include "stm32f10x.h"
#include "string.h"

#include "usart1.h"
#include "usart2.h"
#include "usart3.h"
#include "timer.h"

#include "myiic.h"
#include "MAX30100.h"

#include "wifi.h"
#include "delay.h"

#define no_use_num	60						//由于手指刚接触，模块会很不稳定，所以扔掉前面60个数据
int main(){
	double p,temp;
	u16 up_down1,up_down,heart_rate_num,heart_beat,n=0;
	
	i2cInit();
	usart_init(9600);
	usart2_init(9600);
	usart3_init(115200);
	TIM3_Int_Init(10-1,7199);				//10Khz的计数频率，计数为1ms  
	
	delay_init(72);
	
	MAX30100_Begin();
	
	delayS(20);
	
		wifi_init();
	updat2(0,0);
	
	hmi_cle_table(1);								//hmi波形区清空
	hmi_send_label('j',0,0,20);			//hmi进度条清零
	hmi_send_label('n',0,0,10);			//hmi心率采集清零
	while(1){
		init_flag=MAX30100_Read(0x00);
				 if(init_flag==0x20)
        {
            MAX30100_Update();
					/*start*/
					/*
					输入rawIRValue(大小) -> 算法 -> heart_beat(心率)
					hmi_send_label('j',0,x,20);			//hmi进度条显示百分比为x（0-100）
					hmi_send_label('n',0,y,10);			//hmi心率数值显示为 y
					hmi_send_table(1,z);						//波形在通道1显示，大小为z
					updat2(a,b);										//通过WiFi上传数据 a和b
					*/
					if(rawIRValue>8000){
						datasave[data_index++]=rawIRValue;
						p=data_index*100.0/max_data;
						if(p>=100){
							p=100.0;
						}
//						printf("测量进度=====%5.2f\r\n",p);
						hmi_send_label('j',0,p,20);										//显示hmi 进度条
						hmi_send_table(1,(rawIRValue-8000)/4);				//显示hmi 心率采集
						
						if(data_index<no_use_num){
							ht=0;
							heart_rate_num=0;
						}
						else 
						if(data_index > max_data){
							temp = ht/1001.1;
							for(u8 i=no_use_num;i<max_data;i++){
								datasave[i]=(datasave[i]*5+datasave[i-1]*3+datasave[i-2])/9;
							}
							for(u8 i=no_use_num,j=4;i<max_data;i++){
								if(i%j==0){
										if(datasave[i-j]>datasave[i]){
											up_down=0;		//down
										}else{
											up_down=1;
										}
										if(up_down!=up_down1){
											heart_rate_num++;
										}
										up_down1=up_down;
								}
							}
							
							temp=heart_rate_num*30/temp;				//一分钟60S，乘以30，是因为记录了上升沿，下降延，所以需要除以2
							heart_beat = (temp+0.5)/1;
							hmi_send_label('n',0,heart_beat,10);//显示数值
							data_index=0;
							ht=0;
							heart_rate_num=0;
							
							updat2(heart_beat,n++);
						}
						
					}else if(rawIRValue>5000){
							data_index=0;
							ht=0;
							heart_rate_num=0;
						hmi_send_label('j',0,0,20);			//清空进度条
//						printf("请把手指接触到传感器\r\n");
					}else{
							data_index=0;
							ht=0;
							heart_rate_num=0;
						hmi_send_label('j',0,0,20);
//						printf("请把手指放到传感器上面\r\n");
					}
					/*end*/
				}
	}
	
}








