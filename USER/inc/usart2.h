#ifndef __USART2_H
#define __USART2_H	 

#include "stm32f10x.h"

void usart2_init(u32 baud);	//≥ı ºªØ		
void PcTx_Byte2(u8 Tx_data);
void PcTx_String2(u8 *str);

void hmi_send_label(u8 type,u8 num,u8 data,u8 pos);
void hmi_send_table(u8 id,u8 data);
void hmi_cle_table(u8 id);
#endif

















