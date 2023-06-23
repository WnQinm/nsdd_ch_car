/*
 * motor.c
 *
 *  Created on: 2023��4��18��
 *      Author: Lenovo
 */
#include "motor.h"

float v1=0; //�����ٶ�
float v2=0; //�����ٶ�
float pulseCount_1, pulseCount_2;

void motor_init(void)
{
    //����������Ƶ��ѡ��13K-17K
    //���ռ�ձ�ֵPWM_DUTY_MAX ������zf_driver_pwm.h�ļ����޸� Ĭ��Ϊ10000

    gpio_init(MOTOR1_A, GPO, 0, GPIO_PIN_CONFIG);
    pwm_init(MOTOR1_B, 17000, 0);
    gpio_init(MOTOR2_A, GPO, 0, GPIO_PIN_CONFIG);
    pwm_init(MOTOR2_B, 17000, 0);
}

void getVelocity(float interval){
    pulseCount_1 = encoder_get_count(ENCODER_1)/5;//(float)abs();// ��ȡ����������
    pulseCount_2 = -encoder_get_count(ENCODER_2)/5;//(float)abs();// ��ȡ����������

    encoder_clear_count(ENCODER_1);// ��ձ���������
    encoder_clear_count(ENCODER_2);// ��ձ���������

    v1=(pulseCount_1*1.0f/1024 * 9.42)/interval *10;
    v2=(pulseCount_2*1.0f/1024 * 9.42)/interval *10;
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

int16 Motor_Bias, Motor_Last_Bias, IntegrationM; // ������ò���
int16 M_P=  -1;//-50
int16 M_D=0 ;
int16 M_I= 0;//-6 -1
int16 velocity= -71;//���pid����  -10 - -40
int16 velocity1= -55;//���pid����  -10 - -40
int16 velocity2= -67;//���pid����  -10 - -40

int16 MotorPI (int16 Encoder,int16 Target)
{
    static int16 PwmMotor=0;

    Motor_Bias = Encoder + Target;                   // ����ƫ��
    IntegrationM+=Motor_Bias;
    if(IntegrationM<-300)      IntegrationM=-300;   //�޷�
    else if(IntegrationM>300)  IntegrationM= 300;   //�޷�
    PwmMotor = M_D * (Motor_Bias - Motor_Last_Bias) + M_P * Motor_Bias +M_I*IntegrationM; // ����ʽPI������

    if(PwmMotor > 2000) PwmMotor = 2000;               // �޷�,�ȴ���ϸ�ĵ���
    else if(PwmMotor < -2000)PwmMotor = -2000;         // �޷�

    Motor_Last_Bias = Motor_Bias;              // ������һ��ƫ��

    return PwmMotor; // �������
}
