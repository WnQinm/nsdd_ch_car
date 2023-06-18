/*
 * judgement.c
 *
 *  Created on: 2023��6��18��
 *      Author: ����
 */
#include "judgement.h"

uint8 left_circle_flag=false, right_circle_flag=false, cross_flag=false;
float circle_threshold, cross_threshold;//todo ����ֵ

void judgement()
{
    // todo �����·һʱ�����������Ҫ�������ͷ�ж�
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
