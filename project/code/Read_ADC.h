/*
 * Readadc.h
 *
 *  Created on: 2023年4月18日
 *      Author: Lenovo
 */

#ifndef READ_ADC_H_
#define READ_ADC_H_

#include "zf_common_headfile.h"
// 如果是五路电感，排布应为：竖横竖横竖

void ADC_init();
void Read_ADC();

extern float ADC_MAX,ADC_MIN;
extern uint8 adc_LL,adc_L,adc_R,adc_RR;//电感采集值

#endif /* READ_ADC_H_ */
