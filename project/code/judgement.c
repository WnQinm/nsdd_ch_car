/*
 * judgement.c
 *
 *  Created on: 2023��6��18��
 *      Author: ����
 */
#include "judgement.h"

uint8 left_circle_flag=false, right_circle_flag=false, cross_flag=false;
float circle_threshold, cross_threshold;//todo ����ֵ
uint8 circle_status=0;//���������־

void judgement()
{
    // ����������־λӦ���ڻ��������������
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
