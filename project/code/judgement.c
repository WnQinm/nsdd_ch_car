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

uint16 cross_cnt = 0;

void judgement()
{

    if(!cross_flag && cross_cnt>=2000 && adc_LL>cross_threshold && adc_RR>cross_threshold)
    {// ��ʮ���ж�
        cross_flag = true;
        cross_cnt%=2000;
    }
    else if(cross_flag && cross_cnt>=2000 && adc_LL>cross_threshold && adc_RR>cross_threshold)
    {// ��ʮ���ж�
        cross_flag = false;
        cross_cnt%=2000;
    }
    else if(!cross_flag && cross_cnt>=2000 && Angle<91 && adc_LL>circle_threshold && adc_RR<circle_threshold*2/3)
    {// �󻷵��ж�
        left_circle_flag = true;
        circle_status = 1;
        cross_cnt = 0;
    }
    else if(!cross_flag && cross_cnt>=2000 && Angle>89 && adc_LL<circle_threshold*2/3 && adc_RR>circle_threshold)
    {// �һ����ж�
        right_circle_flag = true;
        circle_status = 1;
        cross_cnt = 0;
    }

    if(cross_cnt<=2000)
        cross_cnt++;

}



















