/*
 * Readadc.h
 *
 *  Created on: 2023年4月18日
 *      Author: Lenovo
 */

#ifndef READ_ADC_H_
#define READ_ADC_H_

// 如果是五路电感，排布应为：竖横竖横竖

void ADC_init();
void Read_ADC();

extern float ADC_MAX,ADC_MIN;
extern float adc_LL,adc_L,adc_M,adc_R;//电感采集值

#endif /* READ_ADC_H_ */
