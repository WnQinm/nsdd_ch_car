/*
 * Readadc.h
 *
 *  Created on: 2023��4��18��
 *      Author: Lenovo
 */

#ifndef READ_ADC_H_
#define READ_ADC_H_

#include "zf_common_headfile.h"
// �������·��У��Ų�ӦΪ������������

void ADC_init();
void Read_ADC();

extern float ADC_MAX,ADC_MIN;
extern uint8 adc_LL,adc_L,adc_R,adc_RR;//��вɼ�ֵ

#endif /* READ_ADC_H_ */
