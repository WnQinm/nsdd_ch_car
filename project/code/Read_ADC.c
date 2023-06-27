/*
 * Readadc.c
 *
 *  Created on: 2023年4月17日
 *      Author: Lenovo
 */
#include "Read_ADC.h"

float ADC_MAX = 235,ADC_MIN = 0;
uint16 adc_LL,adc_L,adc_R,adc_RR;//电感采集值     | - - |
kalman_param kfp0, kfp1, kfp2, kfp3;

void kfp_init_son(kalman_param *kfp)
{

    kfp->LastP = 0.02;
    kfp->Now_P = 0;
    kfp->out   = 0;
    kfp->Kg    = 0;
    kfp->Q     = 0.001;
    kfp->R     = 0.543;

}

void kfp_init(void)
{
    kfp_init_son(&kfp0);
    kfp_init_son(&kfp1);
    kfp_init_son(&kfp2);
    kfp_init_son(&kfp3);
}

void ADC_init()
{
//    adc_init(ADC1_IN12_C2, ADC_8BIT);
//    adc_init(ADC1_IN13_C3,ADC_8BIT);
//    adc_init(ADC1_IN14_C4,ADC_8BIT);
//    adc_init(ADC1_IN15_C5, ADC_8BIT);
    adc_init(ADC1_IN7_A7, ADC_8BIT);
    adc_init(ADC1_IN9_B1,ADC_8BIT);
    adc_init(ADC1_IN10_C0,ADC_8BIT);
    adc_init(ADC1_IN11_C1, ADC_8BIT);
    kfp_init();
}

// 卡尔曼滤波
int kalman_filter(kalman_param *kfp, uint16 input)
{

     kfp->Now_P = kfp->LastP + kfp->Q;
     //卡尔曼增益方程差
     kfp->Kg = kfp->Now_P / (kfp->Now_P + kfp->R);
     //更新最优值方程
     kfp->out = kfp->out + kfp->Kg * (input-kfp->out);
     //更新协方差方程
     kfp->LastP = (1-kfp->Kg) * kfp->Now_P;

     return kfp->out;
}

uint16 Normal(uint16 adc_val)
{
    return (adc_val*1.0/ADC_MAX)*100;
}

void Read_ADC()
{

//    adc_LL=adc_mean_filter_convert(ADC1_IN12_C2, 10);//左竖
//    adc_RR=adc_mean_filter_convert(ADC1_IN13_C3, 10);//右竖
//    adc_L=adc_mean_filter_convert(ADC1_IN14_C4, 10);//左横
//    adc_R=adc_mean_filter_convert(ADC1_IN15_C5, 10);//右横

//    do
//    {
    adc_LL=kalman_filter(&kfp0, adc_mean_filter_convert(ADC1_IN7_A7, 10));//左竖
    adc_RR=kalman_filter(&kfp1, adc_mean_filter_convert(ADC1_IN9_B1, 10));//右竖
    adc_L=kalman_filter(&kfp2, adc_mean_filter_convert(ADC1_IN10_C0, 10));//左横
    adc_R=kalman_filter(&kfp3, adc_mean_filter_convert(ADC1_IN11_C1, 10));//右横
//        motor_control(0,0);
//    }
//    while(adc_LL+adc_RR+adc_L+adc_R<100);

    adc_LL = Normal(adc_LL);
    adc_L = Normal(adc_L);
    adc_R = Normal(adc_R);
    adc_RR = Normal(adc_RR);

    if (adc_LL + adc_RR + adc_L + adc_R<100)
        out_flag = true;
    else
        out_flag = false;

}

