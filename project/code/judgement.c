/*
 * judgement.c
 *
 *  Created on: 2023��6��18��
 *      Author: ����
 */
#include "judgement.h"

RaceStatus CURRENT_STATUS = Status_Common;

uint8 left_circle_flag=false, right_circle_flag=false, cross_flag=false;
uint8 circle_threshold=64, cross_threshold=64;//todo ����ֵ
uint8 circle_status=0;//���������־
bool out_flag = false;

uint8 cross_cnt = 0;

void judgement()
{

    if(!cross_flag && cross_cnt>=200 && adc_LL>cross_threshold && adc_RR>cross_threshold)
    {// ��ʮ���ж�
        cross_flag = true;
        cross_cnt%=200;
    }
    else if(cross_flag && cross_cnt>=200 && adc_LL>cross_threshold && adc_RR>cross_threshold)
    {// ��ʮ���ж�
        cross_flag = false;
        cross_cnt%=200;
    }
    else if(!cross_flag && Angle<91 && adc_LL>circle_threshold && adc_RR<circle_threshold*2/3)
    {// �󻷵��ж�
        left_circle_flag = true;
        circle_status = 1;
    }
    else if(!cross_flag && Angle>89 && adc_LL<circle_threshold*2/3 && adc_RR>circle_threshold)
    {// �һ����ж�
        right_circle_flag = true;
        circle_status = 1;
    }

    if(cross_cnt<=200)
        cross_cnt++;

}



















