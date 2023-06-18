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

float Angle=90;
float last_err=0;

void servo_init()
{
    pwm_init(SERVO_PIN, SERVO_MOTOR_FREQ, SERVO_CENTER);
}

void servo_control(RaceStatus status)
{
    float current_err, output;
    if(status==Status_Common)
        current_err = (float)(adc_L-adc_R)/(float)(adc_L+adc_R);
    else if(status==Status_Circle)
        current_err = (float)(adc_LL-adc_M)/(float)(adc_LL+adc_M);
    output = Kp * (current_err + Kd * (current_err - last_err))/100;
    last_err = current_err;

    Angle=90+output;
    // 角度限幅
    if(Angle<75) Angle=75;
    if(Angle>105) Angle=105;
    pwm_set_duty(SERVO_PIN, SERVO_MOTOR_DUTY(Angle));
}
