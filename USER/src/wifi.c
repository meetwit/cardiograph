#include "stdio.h"
#include "stm32f10x.h"
#include "string.h"
#include "wifi.h"
#include "delay.h"
#include "usart3.h"

uchar AT1[]={"AT+CIPSTART=\"TCP\",\"183.230.40.39\",876\r\n"};
uchar AT2[]={"AT+CIPMODE=1\r\n"};
uchar AT3[]={"AT+CIPSEND\r\n"};

void SendOneByte(u8 d){
	PcTx_Byte3(d);
}

void delay1s(void){
	delay_ms(1000);
}

uchar AT4[]={	 
				0X10,0X31,0X00,0X03,0X45,0X44,0X50,0X01,0X40,0X01,0X2C,0X00,
				0X08,0X33,0X35,0X31,0X36,0X36,0X33,0X37,0X38,0X00,0X1C,0X78,
				0X61,0X4F,0X36,0X4B,0X43,0X78,0X34,0X70,0X6E,0X75,0X6A,0X4F,
				0X42,0X35,0X4A,0X65,0X67,0X55,0X31,0X78,0X58,0X75,0X43,0X4D,
				0X56,0X6B,0X3D
				};


				
uchar AT51[]={
				0X80,0X61,0X00,0X01,0X00,0X5D,0X7B,0X0A,0X09,0X22,0X64,0X61,
				0X74,0X61,0X73,0X74,0X72,0X65,0X61,0X6D,0X73,0X22,0X3A,0X09,
				0X5B,0X7B,0X0A,0X09,0X09,0X09,0X22,0X69,0X64,0X22,0X3A,0X09,
				0X22
				};

				//0X77,	 w	//0x73,	s

uchar AT52[]={				
				0X64,0X22,0X2C,0X0A,0X09,0X09,0X09,0X22,0X64,0X61,0X74,0X61,
				0X70,0X6F,0X69,0X6E,0X74,0X73,0X22,0X3A,0X09,0X5B,0X7B,0X0A,
				0X09,0X09,0X09,0X09,0X09,0X22,0X76,0X61,0X6C,0X75,0X65,0X22,
				0X3A,0X09
				};

				//0X31,0X32,0X2E,0X33,0X34,	  12.34

uchar AT53[]={
				0X30,0X30,0X30,0X30,0X0A,0X09,0X09,0X09,0X09,0X7D,0X5D,0X0A,
				0X09,0X09,0X7D,0X5D,0X0A,0X7D
				};


void wifi_init(){
	Uart1Sends(AT1);
	delay1s();
	Uart1Sends(AT2);
	delay1s();
	Uart1Sends(AT3);
	delay1s();
	delay1s();		  //2
	Uart1Sendsx(AT4,51);
	delay1s();
}

void updat2(uchar R,uchar T){
	 		 Uart1Sendsx(AT51,37);
			 SendOneByte(0x73);		//shidu
			 Uart1Sendsx(AT52,38);
			 SendOneByte(R%100/10+0x30);
			 SendOneByte(R%10+0x30);
			 SendOneByte(0x2e);
			 SendOneByte(0x30);
			 SendOneByte(0x30);
			 Uart1Sendsx(AT53,18);

			 delay1s();

			 Uart1Sendsx(AT51,37);
			 SendOneByte(0x77);	   //wendu
			 Uart1Sendsx(AT52,38);
			 SendOneByte(T%100/10+0x30);
			 SendOneByte(T%10+0x30);
			 SendOneByte(0x2e);
			 SendOneByte(0x30);
			 SendOneByte(0x30);
			 Uart1Sendsx(AT53,18);

}


//串行口连续发送char型数组，遇到终止号/0将停止
void Uart1Sends(uchar *str)
{
	while(*str!='\0')
	{
		SendOneByte(*str++);
	}
}

void Uart1Sendsx(uchar *str,uchar len)
{
	while(len--)
	{
		SendOneByte(*str++);
	}
}

