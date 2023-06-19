/*
 * Readadc.c
 *
 *  Created on: 2023年4月17日
 *      Author: Lenovo
 */
#include <Read_ADC.h>
#include "zf_common_headfile.h"
#include "zf_driver_adc.h"

float ADC_MAX,ADC_MIN;
uint8 adc_LL,adc_L,adc_R,adc_RR;//电感采集值     | - - |

void ADC_init()
{
    adc_init(ADC1_IN12_C2, ADC_8BIT);
    adc_init(ADC1_IN13_C3,ADC_8BIT);
    adc_init(ADC1_IN14_C4,ADC_8BIT);
    adc_init(ADC1_IN15_C5, ADC_8BIT);
}

void Read_ADC()
{
    // todo 限幅
    adc_LL=adc_mean_filter_convert(ADC1_IN12_C2, 10);
    adc_L=adc_mean_filter_convert(ADC1_IN13_C3, 10);
    adc_R=adc_mean_filter_convert(ADC1_IN14_C4, 10);
    adc_RR=adc_mean_filter_convert(ADC1_IN15_C5, 10);

}

