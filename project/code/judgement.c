/*
 * judgement.c
 *
 *  Created on: 2023��6��18��
 *      Author: ����
 */
#include "judgement.h"

RaceStatus CURRENT_STATUS = Status_Common;
RaceStatus CURRENT_MOTOR_STATUS = Status_Common;

uint8 out_garage_flag = false, in_garage_flag = false;
uint8 left_circle_flag=false, right_circle_flag=false, cross_flag=false, obstacle_flag=false, front_diuxian_flag=false, slope_flag=false;
#if CAR_TYPE
uint8 circle_threshold=60, cross_threshold=60;
#else
uint8 circle_threshold=70, cross_threshold=70;
#endif
uint8 circle_status=0;//���������־
bool out_flag = false;

uint16 cross_cnt = 0;
uint32 circle_cnt = 5000;

void judgement()
{

    if(!cross_flag && cross_cnt>=CROSS_DELAY_TIME && (adc_LL>cross_threshold && adc_RR>cross_threshold))
    {// ��ʮ���ж�
        cross_flag = true;
        cross_cnt%=CROSS_DELAY_TIME;
    }
    else if(cross_flag && cross_cnt>=CROSS_DELAY_TIME && (adc_LL>cross_threshold && adc_RR>cross_threshold))
    {// ��ʮ���ж�
        cross_flag = false;
        cross_cnt%=CROSS_DELAY_TIME;
    }
#if ENABLE_LOOP
    else if(!cross_flag && cross_cnt>=CROSS_DELAY_TIME && circle_cnt>=Circle_Delay_time && Angle<91 && adc_LL>circle_threshold && adc_RR<circle_threshold)
    {// �󻷵��ж�
        left_circle_flag = true;
        circle_status = 1;
        cross_cnt = 0;
    }
    else if(!cross_flag && cross_cnt>=CROSS_DELAY_TIME && circle_cnt>=Circle_Delay_time && Angle>89 && adc_LL<circle_threshold && adc_RR>circle_threshold)
    {// �һ����ж�
        right_circle_flag = true;
        circle_status = 1;
        cross_cnt = 0;
    }
#endif

    if(cross_cnt<=CROSS_DELAY_TIME)
        cross_cnt++;

    if(circle_cnt<=Circle_Delay_time)
        circle_cnt++;
}



















