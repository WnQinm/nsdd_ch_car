/*
 * servo.c
 *
 *  Created on: 2023年4月18日
 *      Author: Lenovo
 */

#include "servo.h"

float Angle=90;

// 电磁参数
#define elec_Kp 15
#define elec_Kd 0.9
float current_err_common, current_err_circle;
float last_err_elec=0;

// 摄像头参数
#define  camera_Kp 30//30
#define  camera_Kd 0
int midline_f, midline_ff, midline_fff;
//通过aimline及其下面3行计算偏差
uint8 aimLine=default_aimline;
float current_err_camera, last_err_camera;

void servo_init()
{
    pwm_init(SERVO_PIN, SERVO_MOTOR_FREQ, SERVO_CENTER);
}

void servo_control(RaceStatus status)
{
    // todo 引入边界保护后可以去除分母的1
    current_err_common = ((int16)(adc_L-adc_R)<<7)/(adc_L+adc_R+1);
    current_err_circle = ((int16)(adc_LL-adc_RR)<<7)/(adc_LL+adc_RR+1);

    current_err_camera = ((  5 * centerline[aimLine] +
                             3 * centerline[aimLine + 1] +
                             2 * centerline[aimLine + 2]) / (10.000f)) - COL/2;
    current_err_camera = current_err_camera * 0.80f - regression(aimLine, aimLine+5) * 0.20f;

    switch (status)
    {
        case Status_Stop:return;
        case Status_Common:elec_pid(current_err_common);break;
        case Status_Circle:elec_pid(current_err_circle);break;
        case Status_Camera:camera_pid(current_err_camera);break;
    }

    // 角度限幅
    if(Angle<75) Angle=75;
    if(Angle>105) Angle=105;
    pwm_set_duty(SERVO_PIN, SERVO_MOTOR_DUTY(Angle));
}

void elec_pid(float current_err)
{
    float output = elec_Kp * (current_err + elec_Kd * (current_err - last_err_elec))/100;
    last_err_elec = current_err;
    Angle=90+output;// 这应该是Angle+=output但是架不住那样效果好
}

void camera_pid(float current_err)
{
    float output = camera_Kp * (current_err + camera_Kd * (current_err - last_err_camera))/100;
    last_err_camera = current_err;
    Angle = 90 - output;
}
