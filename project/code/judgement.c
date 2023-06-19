/*
 * judgement.c
 *
 *  Created on: 2023年6月18日
 *      Author: 清梦
 */
#include "judgement.h"

uint8 left_circle_flag=false, right_circle_flag=false, cross_flag=false;
float circle_threshold, cross_threshold;//todo 设置值
uint8 circle_status=0;//环岛处理标志

void judgement()
{
    // 环岛结束标志位应该在环岛部分主动清除
    if(adc_LL>circle_threshold && adc_RR<circle_threshold)
    {
        left_circle_flag = true;
        circle_status = 1;
    }
    else if(adc_LL<circle_threshold && adc_RR>circle_threshold)
    {
        right_circle_flag = true;
        circle_status = 1;
    }
//    else if(adc_LL>circle_threshold && adc_M<cross_threshold)
//        cross_flag = true;
//    else
//    {
//        left_circle_flag = false;
//        right_circle_flag = false;
//        cross_flag = false;
//    }
}
