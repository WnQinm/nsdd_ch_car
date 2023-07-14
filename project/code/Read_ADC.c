/*
 * Readadc.c
 *
 *  Created on: 2023年4月17日
 *      Author: Lenovo
 */
#include "Read_ADC.h"

float ADC_MAX = 235,ADC_MIN = 0;
float voltage_now=0;
uint16 adc_LL,adc_L,adc_M,adc_R,adc_RR;//电感采集值     | - - |
kalman_param kfp0, kfp1, kfp2, kfp3, kfp4;

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
    kfp_init_son(&kfp4);
}

void ADC_init()
{
#if CAR_TYPE
    adc_init(ADC1_IN12_C2, ADC_8BIT);
    adc_init(ADC1_IN13_C3,ADC_8BIT);
    adc_init(ADC1_IN14_C4,ADC_8BIT);
    adc_init(ADC1_IN15_C5, ADC_8BIT);
    adc_init(ADC1_IN1_A1, ADC_8BIT);
#else
    adc_init(ADC1_IN12_C2, ADC_8BIT);
    adc_init(ADC1_IN13_C3,ADC_8BIT);
    adc_init(ADC1_IN14_C4,ADC_8BIT);
    adc_init(ADC1_IN15_C5, ADC_8BIT);
    adc_init(ADC1_IN1_A1, ADC_8BIT);
#endif
    kfp_init();


}

void ADC_Battery_init(){
    adc_init(ADC2_IN9_B1, ADC_12BIT);
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
#if CAR_TYPE
    adc_LL=kalman_filter(&kfp0, adc_mean_filter_convert(ADC1_IN12_C2, 10));
    adc_L=kalman_filter(&kfp1, adc_mean_filter_convert(ADC1_IN13_C3, 10));
    adc_M=kalman_filter(&kfp2, adc_mean_filter_convert(ADC1_IN14_C4, 10));
    adc_R=kalman_filter(&kfp3, adc_mean_filter_convert(ADC1_IN15_C5, 10));
    adc_RR=kalman_filter(&kfp4, adc_mean_filter_convert(ADC1_IN1_A1, 10));
#else
    adc_LL=kalman_filter(&kfp0, adc_mean_filter_convert(ADC1_IN12_C2, 10));//左竖
    adc_L=kalman_filter(&kfp1, adc_mean_filter_convert(ADC1_IN13_C3, 10));//右竖
    adc_M=kalman_filter(&kfp2, adc_mean_filter_convert(ADC1_IN14_C4, 10));//中竖
    adc_R=kalman_filter(&kfp3, adc_mean_filter_convert(ADC1_IN15_C5, 10));//左横
    adc_RR=kalman_filter(&kfp4, adc_mean_filter_convert(ADC1_IN1_A1, 10));//右横
#endif

    adc_LL = Normal(adc_LL);
    adc_L = Normal(adc_L);
    adc_M = Normal(adc_M);
    adc_R = Normal(adc_R);
    adc_RR = Normal(adc_RR);

    if (adc_LL + adc_RR + adc_L + adc_R<100)
        out_flag = true;
    else
        out_flag = false;
}

float Get_Battery_Voltage(){
    uint16 voltage_adc = adc_convert(ADC2_IN9_B1);
    voltage_now = 11.0f * 3.3f * ((float)voltage_adc / 4095);
//    printf("Voltage: %2.2f\n", voltage_now);
//    if(voltage_now<=11.0f){
//        ips200_clear();
//        motor_control(0,0);
//        ips200_show_string(1,1,"BATTERY VERY LOW! ");
//        ips200_show_string(1,21,"PLEASE CHARGE NOW! ");
//        while (1){}
//    }
    return voltage_now;
}
