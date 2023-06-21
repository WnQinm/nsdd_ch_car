/*
 * judgement.c
 *
 *  Created on: 2023��6��18��
 *      Author: ����
 */
#include "judgement.h"

uint8 left_circle_flag=false, right_circle_flag=false, cross_flag=false;
uint8 circle_threshold=150, cross_threshold=150;//todo ����ֵ
uint8 circle_status=0;//���������־
bool out_flag = false;

void judgement()
{
    // todo ������ֵ����Ϊ����ʱ�����ֵ+10֮���

    // �ܿ����������,��ֵ��Ҫ��������
    if(!cross_flag && Angle<91 && adc_LL>circle_threshold && adc_RR<circle_threshold*2/3)
    {
        left_circle_flag = true;
        circle_status = 1;
    }
    else if(!cross_flag && Angle>89 && adc_LL<circle_threshold*2/3 && adc_RR>circle_threshold)
    {
        right_circle_flag = true;
        circle_status = 1;
    }
    else if(!cross_flag && adc_LL>cross_threshold && adc_RR>cross_threshold)
    {
        ips200_show_string(0, 200, "cross");
        cross_flag = true;
    }
    else if(cross_flag && adc_LL>cross_threshold && adc_RR>cross_threshold)
    {
        ips200_show_string(0, 200, "     ");
        cross_flag = false;
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
