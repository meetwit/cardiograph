#ifndef __USART3_H
#define __USART3_H	 

#include "stm32f10x.h"

void usart3_init(u32 baud);	//≥ı ºªØ		
void PcTx_Byte3(u8 Tx_data);
void PcTx_String3(u8 *str);

#endif

















