/*
 * servo.c
 *
 *  Created on: 2023年4月18日
 *      Author: Lenovo
 */
#include "Read_ADC.h"
#include "servo.h"

#define Kp 15
#define Kd 0.9
#define gamma 1e2

float Angle=90;
float current_err, output;
float last_err=0;

void servo_init()
{
    pwm_init(SERVO_PIN, SERVO_MOTOR_FREQ, SERVO_CENTER);
}

void servo_control(RaceStatus status)
{
    // todo 引入边界保护后可以去除分母的1
    switch (status)
    {
        case Status_Common:current_err = ((int16)(adc_L-adc_R)<<7)/(adc_L+adc_R+1);break;
        case Status_Circle:current_err = ((int16)(adc_LL-adc_RR)<<7)/(adc_LL+adc_RR+1);break;
    }

    output = Kp * (current_err + Kd * (current_err - last_err))/100;
    last_err = current_err;

    Angle=90+output;// 这应该是Angle+=output但是架不住那样效果好
    ips200_show_float(0, 80, Angle, 5, 5);
    // 角度限幅
    if(Angle<75) Angle=75;
    if(Angle>105) Angle=105;
    pwm_set_duty(SERVO_PIN, SERVO_MOTOR_DUTY(Angle));
}
