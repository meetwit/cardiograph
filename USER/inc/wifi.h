#ifndef _WIFI_MT_
#define _WIFI_MT_

//---重定义关键词---//
#ifndef uchar
#define uchar unsigned char
#endif

#ifndef uint 
#define uint unsigned int
#endif

void wifi_init(void);
void updat2(uchar R,uchar T);
void Uart1Sends(uchar *str);
void Uart1Sendsx(uchar *str,uchar len);


#endif


