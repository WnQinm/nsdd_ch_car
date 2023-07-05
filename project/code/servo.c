/*
 * servo.c
 *
 *  Created on: 2023��4��18��
 *      Author: Lenovo
 */

#include "servo.h"

float Angle=90;

// ��Ų���
float elec_Kp = 15;
float elec_Kd = 0.9;
float current_err_common, current_err_circle;
float last_err_elec=0;

// ����ͷ����
#define  camera_Kp 30//30
#define  camera_Kd 0
int midline_f, midline_ff, midline_fff;
//ͨ��aimline��������3�м���ƫ��
uint8 aimLine=default_aimline;
float current_err_Lcamera, current_err_Rcamera, last_err_camera;

void servo_init()
{
    pwm_init(SERVO_PIN, SERVO_MOTOR_FREQ, SERVO_CENTER);
}

void servo_control(RaceStatus status)
{
    // ��ͨ���Ѳ��
    current_err_common = ((int16)(adc_L-adc_R)<<7)/(adc_L+adc_R+1);
    // ��ֱ��е��Ѳ��
    current_err_circle = ((int16)(adc_LL-adc_RR)<<7)/(adc_LL+adc_RR+1);
    // ����ͷѭ�����
    current_err_Lcamera = ((  5 * (leftline[aimLine]-Road_Width_Min/2) +
                             3 * (leftline[aimLine+1]-Road_Width_Min/2) +
                             2 * (leftline[aimLine+2]-Road_Width_Min/2)) / (10.000f)) - COL/2;
    current_err_Lcamera = current_err_Lcamera * 0.80f - regression(aimLine, aimLine+5) * 0.20f;
    // ����ͷѭ�ұ���
    current_err_Rcamera = ((  5 * (rightline[aimLine]-Road_Width_Min/2) +
                             3 * (rightline[aimLine+1]-Road_Width_Min/2) +
                             2 * (rightline[aimLine+2]-Road_Width_Min/2)) / (10.000f)) - COL/2;
    current_err_Rcamera = current_err_Rcamera * 0.80f - regression(aimLine, aimLine+5) * 0.20f;
#if SERVO_DEBUG_STATUS
#define ENLARGE_TIME 1      // �Ŵ��������ڴ��ڴ�������ֻ�ܴ������������Կ�����Ҫ�Ŵ����
    virtual_oscilloscope_data_conversion((int16)(current_err_common*ENLARGE_TIME),
                                         (int16)(current_err_circle*ENLARGE_TIME),
                                         (int16)(current_err_Lcamera*ENLARGE_TIME),
                                         (int16)(current_err_Rcamera*ENLARGE_TIME));
//        uart_write_buffer(UART_3, virtual_oscilloscope_data, 10);
//    bluetooth_ch9141_send_buff(virtual_oscilloscope_data, 10);
#endif
    switch (status)
    {
        case Status_Stop:return;
        case Status_Common:elec_pid(current_err_common);break;
        case Status_Circle:elec_pid(current_err_circle);break;
        case Status_LCamera:camera_pid(current_err_Lcamera);break;
        case Status_RCamera:camera_pid(current_err_Rcamera);break;
    }

    // �Ƕ��޷�
    if(Angle<75) Angle=75;
    if(Angle>105) Angle=105;
#if ANGLE_90_MODE
    pwm_set_duty(SERVO_PIN, SERVO_MOTOR_DUTY(90));
#else
    pwm_set_duty(SERVO_PIN, SERVO_MOTOR_DUTY(Angle));
#endif
}


void elec_pid(float current_err)
{
    float output = elec_Kp * (current_err + elec_Kd * (current_err - last_err_elec))/100;
    last_err_elec = current_err;
    Angle=90+output;// ��Ӧ����Angle+=output���Ǽܲ�ס����Ч����
}

void camera_pid(float current_err)
{
    float output = camera_Kp * (current_err + camera_Kd * (current_err - last_err_camera))/100;
    last_err_camera = current_err;
    Angle = 90 - output;
}
