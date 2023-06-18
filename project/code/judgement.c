/*
 * judgement.c
 *
 *  Created on: 2023年6月18日
 *      Author: 清梦
 */
#include "judgement.h"

uint8 left_circle_flag=false, right_circle_flag=false, cross_flag=false;
float circle_threshold, cross_threshold;//todo 设置值

void judgement()
{
    // todo 如果四路一时间出不来就需要结合摄像头判定
    if(adc_M>ADC_MAX && adc_LL>circle_threshold && adc_M<cross_threshold)
        left_circle_flag = true;
    else if(adc_M>ADC_MAX && adc_M<cross_threshold && adc_LL<circle_threshold)
        right_circle_flag = true;
    else if(adc_LL>circle_threshold && adc_M<cross_threshold)
        cross_flag = true;
    else
    {
        left_circle_flag = false;
        right_circle_flag = false;
        cross_flag = false;
    }
}
