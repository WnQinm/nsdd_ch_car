/*
 * Readadc.h
 *
 *  Created on: 2023��4��18��
 *      Author: Lenovo
 */

#ifndef READ_ADC_H_
#define READ_ADC_H_

#include "zf_common_headfile.h"
#include "judgement.h"
#include "zf_driver_adc.h"
#include "motor.h"
// �������·��У��Ų�ӦΪ������������

void ADC_init();
void Read_ADC();
float Get_Battery_Voltage();
void ADC_Battery_init();

typedef struct{
    float LastP;
    float Now_P;
    float out;
    float Kg;
    float Q;
    float R;
}kalman_param;

extern float voltage_now;//��ص�ѹ
extern float ADC_MAX,ADC_MIN;
extern uint16 adc_LL,adc_L,adc_M,adc_R,adc_RR;//��вɼ�ֵ
#endif /* READ_ADC_H_ */
