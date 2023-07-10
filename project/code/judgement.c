/*
 * judgement.c
 *
 *  Created on: 2023年6月18日
 *      Author: 清梦
 */
#include "judgement.h"

RaceStatus CURRENT_STATUS = Status_Common;

uint8 out_garage_flag = false, in_garage_flag = false;
uint8 left_circle_flag=false, right_circle_flag=false, cross_flag=false, obstacle_flag=false, front_diuxian_flag=false, slope_flag=false;
uint8 circle_threshold=80, cross_threshold=80;
uint8 circle_status=0;//环岛处理标志
bool out_flag = false;

uint16 cross_cnt = 0;

void judgement()
{

    if(!cross_flag && cross_cnt>=CROSS_DELAY_TIME && (adc_LL>cross_threshold && adc_RR>cross_threshold))
    {// 入十字判断
        cross_flag = true;
        cross_cnt%=CROSS_DELAY_TIME;
    }
    else if(cross_flag && cross_cnt>=CROSS_DELAY_TIME && (adc_LL>cross_threshold && adc_RR>cross_threshold))
    {// 出十字判断
        cross_flag = false;
        cross_cnt%=CROSS_DELAY_TIME;
    }
    else if(!cross_flag && cross_cnt>=CROSS_DELAY_TIME && Angle<91 && adc_LL>circle_threshold && adc_RR<circle_threshold)
    {// 左环岛判断
        left_circle_flag = true;
        circle_status = 1;
        cross_cnt = 0;
    }
    else if(!cross_flag && cross_cnt>=CROSS_DELAY_TIME && Angle>89 && adc_LL<circle_threshold && adc_RR>circle_threshold)
    {// 右环岛判断
        right_circle_flag = true;
        circle_status = 1;
        cross_cnt = 0;
    }

    if(cross_cnt<=CROSS_DELAY_TIME)
        cross_cnt++;

}



















