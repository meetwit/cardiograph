#include "usart2.h"
#include "string.h"
#include "stdio.h"


/************************************************************
函数名：void usart2_init(u32 baud);
形  参：u32 baud:波特率
返回值：无
功  能：串口2初始化
作  者：meetwit
时  间：2017年11月19日11:26:29
*************************************************************/
void usart2_init(u32 baud){
	/*
	1.开GPIO时钟
	2.初始化PA2(发送	复用推挽)  PA3(接收	浮空输入)
	3.开串口2时钟
	4.字长：1个起始位，8个数据位，1个停止位
	5.发送使能
	6.波特率
	7.串口使能
	*/
	float div,div_frr;
	u16 div_man;
	
	RCC ->APB2ENR |= (0x1<<2);//GPIOA使能
	GPIOA ->CRL &=~(0xF<<8);
	GPIOA ->CRL |=(0xB<<8);//复用推挽
	//GPIOA ->CRL |=(0X8<<8);//浮空输入,临时避免陀螺仪不能配置
	
	GPIOA ->CRL &=~(0Xf<<12);
	GPIOA ->CRL |=(0X8<<12);//浮空输入
	
	RCC ->APB1ENR |=(0x1<<17);//串口2时钟使能
	USART2->CR1 &=~(0x1<<12);//一个起始位，8个数据位
	USART2->CR2 &=~(0X3<<12);//一个停止位
	USART2->CR1 |=(0x1<<3);//发送使能
	
	USART2->CR1|=(0x1<<2);    //接收使能
	USART2->CR1|=(1<<5);    //当USART_SR中的ORE或者RXNE为’1’时，产生USART中断
		
	div=(float) 36000000/(baud * 16);
	div_man =div;
	div_frr=div-div_man;
	div_man <<= 4;
	
	USART2->BRR = div_man + (div_frr*16);
	
	/*设置优先级*/
	NVIC_SetPriority(USART2_IRQn,NVIC_EncodePriority(7-2,2,2));
	/*使能中断*/
	NVIC_EnableIRQ(USART2_IRQn);//NVIC_EnableIRQ是用来使能外部中断的
	
	USART2->CR1 |=(0x1<<13);//串口使能	
}


/************************************************************
函数名：PcTx_Byte2(u8 Tx_data)
形  参：u8 Tx_data
返回值：无
功  能：串口2发送一个字节
作  者：meetwit
时  间：2017年11月19日11:26:55
*************************************************************/
void PcTx_Byte2(u8 Tx_data){
	while(!(USART2->SR &(0x1<<7)));
	USART2 ->DR = Tx_data;
	
}

/************************************************************
函数名：PcTx_String2(u8 *str)
形  参：u8 *str 字符串地址
返回值：无
功  能：串口2发送字符串
作  者：meetwit
时  间：2017年11月19日11:27:21
*************************************************************/
void PcTx_String2(u8 *str){
		while(*str !='\0'){
			PcTx_Byte2(*(str++));
		}
}



/************************************************************
函数名：USART2_IRQHandler()
形  参：无
返回值：无
功  能：串口2中断
作  者：meetwit
时  间：2017年11月19日11:27:43
*************************************************************/
	void USART2_IRQHandler(void)
{
	u8 res_t2;
	if(USART2->SR&(0x1<<5))	//接收到数据
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

//实例1:cle 1,0 (清除ID为1的曲线控件的0通道数据)
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

