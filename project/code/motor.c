/*
 * motor.c
 *
 *  Created on: 2023��4��8��
 *      Author: ����
 */


#include "motor.h"

//��ʼ�����PWM���źͷ�������
#define MOTOR1_A   D12                      // ����1�������ת����
#define MOTOR1_B   TIM4_PWM_MAP1_CH2_D13         // ����1���PWM����

#define MOTOR2_A   D15                      // ����2�������ת����
#define MOTOR2_B   TIM4_PWM_MAP1_CH3_D14         // ����2���PWM����

void motor_init(void)
{
    //����������Ƶ��ѡ��13K-17K
    //���ռ�ձ�ֵPWM_DUTY_MAX ������zf_driver_pwm.h�ļ����޸� Ĭ��Ϊ10000

    gpio_init(MOTOR1_A, GPO, 0, GPIO_PIN_CONFIG);
    pwm_init(MOTOR1_B, 17000, 0);
    gpio_init(MOTOR2_A, GPO, 0, GPIO_PIN_CONFIG);
    pwm_init(MOTOR2_B, 17000, 0);
}

void motor_pid(int16 encoder, int16 expect_speed)
{

}


void motor_control(int32 duty_1, int32 duty_2)
{
    //��ռ�ձ��޷�
    if(duty_1>PWM_DUTY_MAX)         duty_1 = PWM_DUTY_MAX;
    else if(duty_1<-PWM_DUTY_MAX)   duty_1 = -PWM_DUTY_MAX;

    if(duty_2>PWM_DUTY_MAX)         duty_2 = PWM_DUTY_MAX;
    else if(duty_2<-PWM_DUTY_MAX)   duty_2 = -PWM_DUTY_MAX;


    if(0<=duty_1)
    {
        gpio_set_level(MOTOR1_A, 0);
        pwm_set_duty(MOTOR1_B, duty_1);
    }
    else
    {
        gpio_set_level(MOTOR1_A, 1);
        pwm_set_duty(MOTOR1_B, -duty_1);
    }

    if(0<=duty_2)
    {
        gpio_set_level(MOTOR2_A, 0);
        pwm_set_duty(MOTOR2_B, duty_2);
    }
    else
    {
        gpio_set_level(MOTOR2_A, 1);
        pwm_set_duty(MOTOR2_B, -duty_2);
    }


}
