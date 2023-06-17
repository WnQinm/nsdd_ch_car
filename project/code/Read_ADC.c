/*
 * Readadc.c
 *
 *  Created on: 2023年4月17日
 *      Author: Lenovo
 */
#include <Read_ADC.h>
#include "zf_common_headfile.h"
#include "zf_driver_adc.h"

//float ad_v0,ad_v1,ad_v2,ad_v3,ad_v4,ad_v5;
float l,m,r,sum;
float adc_L,adc_LM,adc_RM,adc_R;//电感采集值



void Read_ADC()
{


    adc_init(ADC1_IN14_C4,ADC_8BIT);//c0 l2
    adc_init(ADC1_IN13_C3,ADC_8BIT);//c1 r2

    adc_RM=adc_mean_filter_convert(ADC1_IN14_C4, 10);
    adc_LM=adc_mean_filter_convert(ADC1_IN13_C3, 10);


}

