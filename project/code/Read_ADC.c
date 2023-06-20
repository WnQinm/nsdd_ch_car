/*
 * Readadc.c
 *
 *  Created on: 2023��4��17��
 *      Author: Lenovo
 */
#include <Read_ADC.h>
#include "zf_common_headfile.h"
#include "zf_driver_adc.h"

float ADC_MAX,ADC_MIN;
uint8 adc_LL,adc_L,adc_R,adc_RR;//��вɼ�ֵ     | - - |

void ADC_init()
{
//    adc_init(ADC1_IN12_C2, ADC_8BIT);
//    adc_init(ADC1_IN13_C3,ADC_8BIT);
//    adc_init(ADC1_IN14_C4,ADC_8BIT);
//    adc_init(ADC1_IN15_C5, ADC_8BIT);
    adc_init(ADC1_IN7_A7, ADC_8BIT);
    adc_init(ADC1_IN9_B1,ADC_8BIT);
    adc_init(ADC1_IN10_C0,ADC_8BIT);
    adc_init(ADC1_IN11_C1, ADC_8BIT);
}

void Read_ADC()
{
    // todo �޷�
//    adc_LL=adc_mean_filter_convert(ADC1_IN12_C2, 10);//����
//    adc_RR=adc_mean_filter_convert(ADC1_IN13_C3, 10);//����
//    adc_L=adc_mean_filter_convert(ADC1_IN14_C4, 10);//���
//    adc_R=adc_mean_filter_convert(ADC1_IN15_C5, 10);//�Һ�
    adc_LL=adc_mean_filter_convert(ADC1_IN7_A7, 10);//����
    adc_RR=adc_mean_filter_convert(ADC1_IN9_B1, 10);//����
    adc_L=adc_mean_filter_convert(ADC1_IN10_C0, 10);//���
    adc_R=adc_mean_filter_convert(ADC1_IN11_C1, 10);//�Һ�

    ips200_show_int(0, 20, adc_LL, 5);
    ips200_show_int(0, 50, adc_L, 5);
    ips200_show_int(50, 50, adc_R, 5);
    ips200_show_int(50, 20, adc_RR, 5);
}

