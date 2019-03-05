#include "usart2.h"
#include "string.h"
#include "stdio.h"


/************************************************************
��������void usart2_init(u32 baud);
��  �Σ�u32 baud:������
����ֵ����
��  �ܣ�����2��ʼ��
��  �ߣ�meetwit
ʱ  �䣺2017��11��19��11:26:29
*************************************************************/
void usart2_init(u32 baud){
	/*
	1.��GPIOʱ��
	2.��ʼ��PA2(����	��������)  PA3(����	��������)
	3.������2ʱ��
	4.�ֳ���1����ʼλ��8������λ��1��ֹͣλ
	5.����ʹ��
	6.������
	7.����ʹ��
	*/
	float div,div_frr;
	u16 div_man;
	
	RCC ->APB2ENR |= (0x1<<2);//GPIOAʹ��
	GPIOA ->CRL &=~(0xF<<8);
	GPIOA ->CRL |=(0xB<<8);//��������
	//GPIOA ->CRL |=(0X8<<8);//��������,��ʱ���������ǲ�������
	
	GPIOA ->CRL &=~(0Xf<<12);
	GPIOA ->CRL |=(0X8<<12);//��������
	
	RCC ->APB1ENR |=(0x1<<17);//����2ʱ��ʹ��
	USART2->CR1 &=~(0x1<<12);//һ����ʼλ��8������λ
	USART2->CR2 &=~(0X3<<12);//һ��ֹͣλ
	USART2->CR1 |=(0x1<<3);//����ʹ��
	
	USART2->CR1|=(0x1<<2);    //����ʹ��
	USART2->CR1|=(1<<5);    //��USART_SR�е�ORE����RXNEΪ��1��ʱ������USART�ж�
		
	div=(float) 36000000/(baud * 16);
	div_man =div;
	div_frr=div-div_man;
	div_man <<= 4;
	
	USART2->BRR = div_man + (div_frr*16);
	
	/*�������ȼ�*/
	NVIC_SetPriority(USART2_IRQn,NVIC_EncodePriority(7-2,2,2));
	/*ʹ���ж�*/
	NVIC_EnableIRQ(USART2_IRQn);//NVIC_EnableIRQ������ʹ���ⲿ�жϵ�
	
	USART2->CR1 |=(0x1<<13);//����ʹ��	
}


/************************************************************
��������PcTx_Byte2(u8 Tx_data)
��  �Σ�u8 Tx_data
����ֵ����
��  �ܣ�����2����һ���ֽ�
��  �ߣ�meetwit
ʱ  �䣺2017��11��19��11:26:55
*************************************************************/
void PcTx_Byte2(u8 Tx_data){
	while(!(USART2->SR &(0x1<<7)));
	USART2 ->DR = Tx_data;
	
}

/************************************************************
��������PcTx_String2(u8 *str)
��  �Σ�u8 *str �ַ�����ַ
����ֵ����
��  �ܣ�����2�����ַ���
��  �ߣ�meetwit
ʱ  �䣺2017��11��19��11:27:21
*************************************************************/
void PcTx_String2(u8 *str){
		while(*str !='\0'){
			PcTx_Byte2(*(str++));
		}
}



/************************************************************
��������USART2_IRQHandler()
��  �Σ���
����ֵ����
��  �ܣ�����2�ж�
��  �ߣ�meetwit
ʱ  �䣺2017��11��19��11:27:43
*************************************************************/
	void USART2_IRQHandler(void)
{
	u8 res_t2;
	if(USART2->SR&(0x1<<5))	//���յ�����
	{ 
		res_t2=USART2->DR;
	}		
}

//wepo 80,10
//repo n0.val,10
void hmi_send_label(u8 type,u8 num,u8 data,u8 pos){
	PcTx_Byte2('w');
	PcTx_Byte2('e');
	PcTx_Byte2('p');
	PcTx_Byte2('o');
	PcTx_Byte2(' ');
	PcTx_Byte2(data/100+0x30);
	PcTx_Byte2(data%100/10+0x30);
	PcTx_Byte2(data%10+0x30);
	PcTx_Byte2(',');
	PcTx_Byte2(pos%100/10+0x30);
	PcTx_Byte2(pos%10+0x30);
	PcTx_Byte2(0xff);
	PcTx_Byte2(0xff);
	PcTx_Byte2(0xff);
	
	PcTx_Byte2('r');
	PcTx_Byte2('e');
	PcTx_Byte2('p');
	PcTx_Byte2('o');
	PcTx_Byte2(' ');
	PcTx_Byte2(type);
	PcTx_Byte2(num%10+0x30);
	PcTx_Byte2('.');
	PcTx_Byte2('v');
	PcTx_Byte2('a');
	PcTx_Byte2('l');
	PcTx_Byte2(',');
	PcTx_Byte2(pos%100/10+0x30);
	PcTx_Byte2(pos%10+0x30);
	PcTx_Byte2(0xff);
	PcTx_Byte2(0xff);
	PcTx_Byte2(0xff);
}

/*
printf("add 2,0,%d",(rawIRValue1-10000)/16);
*/
void hmi_send_table(u8 id,u8 data){
	PcTx_Byte2('a');
	PcTx_Byte2('d');
	PcTx_Byte2('d');
	PcTx_Byte2(' ');
	PcTx_Byte2(id%10+0x30);
	PcTx_Byte2(',');
	PcTx_Byte2(0x30);
	PcTx_Byte2(',');
	PcTx_Byte2(data/100+0x30);
	PcTx_Byte2(data%100/10+0x30);
	PcTx_Byte2(data%10+0x30);
	PcTx_Byte2(0xff);
	PcTx_Byte2(0xff);
	PcTx_Byte2(0xff);
}

//ʵ��1:cle 1,0 (���IDΪ1�����߿ؼ���0ͨ������)
void hmi_cle_table(u8 id){
	PcTx_Byte2('c');
	PcTx_Byte2('l');
	PcTx_Byte2('e');
	PcTx_Byte2(' ');
	PcTx_Byte2(id%10+0x30);
	PcTx_Byte2(',');
	PcTx_Byte2(0x30);
	PcTx_Byte2(0xff);
	PcTx_Byte2(0xff);
	PcTx_Byte2(0xff);
}

