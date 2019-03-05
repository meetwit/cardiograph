#include "stdio.h"
#include "stm32f10x.h"
#include "string.h"

#include "usart1.h"
#include "usart2.h"
#include "usart3.h"
#include "timer.h"

#include "myiic.h"
#include "MAX30100.h"

#define no_use_num	60						//������ָ�սӴ���ģ���ܲ��ȶ��������ӵ�ǰ��60������
int main(){
	double p,temp;
	u16 up_down1,up_down,heart_rate_num,heart_beat;
	
	i2cInit();
	usart_init(9600);
	usart2_init(9600);
	TIM3_Int_Init(10-1,7199);				//10Khz�ļ���Ƶ�ʣ�����Ϊ1ms  
	
	MAX30100_Begin();
	
	hmi_cle_table(1);								//hmi���������
	hmi_send_label('j',0,0,20);			//hmi����������
	hmi_send_label('n',0,0,10);			//hmi���ʲɼ�����
	while(1){
		init_flag=MAX30100_Read(0x00);
				 if(init_flag==0x20)
        {
            MAX30100_Update();
					if(rawIRValue>8000){
						datasave[data_index++]=rawIRValue;
						p=data_index*100.0/max_data;
						if(p>=100){
							p=100.0;
						}
//						printf("��������=====%5.2f\r\n",p);
						hmi_send_label('j',0,p,20);										//��ʾhmi ������
						hmi_send_table(1,(rawIRValue-8000)/4);				//��ʾhmi ���ʲɼ�
						
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
							
							temp=heart_rate_num*30/temp;				//һ����60S������30������Ϊ��¼�������أ��½��ӣ�������Ҫ����2
							heart_beat = (temp+0.5)/1;
							hmi_send_label('n',0,heart_beat,10);
							data_index=0;
							ht=0;
							heart_rate_num=0;
						}
						
					}else if(rawIRValue>5000){
							data_index=0;
							ht=0;
							heart_rate_num=0;
						hmi_send_label('j',0,0,20);
//						printf("�����ָ�Ӵ���������\r\n");
					}else{
							data_index=0;
							ht=0;
							heart_rate_num=0;
						hmi_send_label('j',0,0,20);
//						printf("�����ָ�ŵ�����������\r\n");
					}
				}
	}
	
}








