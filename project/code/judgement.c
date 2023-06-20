/*
 * judgement.c
 *
 *  Created on: 2023年6月18日
 *      Author: 清梦
 */
#include "judgement.h"

uint8 left_circle_flag=false, right_circle_flag=false, cross_flag=false;
uint8 circle_threshold=210, cross_threshold;//todo 设置值
uint8 circle_status=0;//环岛处理标志
bool out_flag = false;

void judgement()
{
    // todo 环岛阈值设置为拐弯时的最大值+10之类的

    // 很可能弯道误判,阈值需要合理设置
    if(Angle<91 && adc_LL>circle_threshold && adc_RR<circle_threshold)
    {
        left_circle_flag = true;
        circle_status = 1;
    }
    else if(Angle>89 && adc_LL<circle_threshold && adc_RR>circle_threshold)
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
