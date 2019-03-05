#ifndef __TIMER_H
#define __TIMER_H
#include "sys.h"

#define max_data 160

extern u32 ht,temp_time;
extern u8 init_flag;
extern u16 datasave[max_data],data_index;

void TIM3_Int_Init(u16 arr,u16 psc);
#endif























