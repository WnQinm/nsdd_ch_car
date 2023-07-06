/*
 * motor.c
 *
 *  Created on: 2023��4��18��
 *      Author: Lenovo
 */
#include "motor.h"

int16 pulseCount_1, pulseCount_2;
int16 motorPWML=0, motorPWMR=0;

void motor_init(void)
{
    //����������Ƶ��ѡ��13K-17K
    //���ռ�ձ�ֵPWM_DUTY_MAX ������zf_driver_pwm.h�ļ����޸� Ĭ��Ϊ10000

    gpio_init(MOTOR1_A, GPO, 0, GPIO_PIN_CONFIG);
    pwm_init(MOTOR1_B, 17000, 0);
    gpio_init(MOTOR2_A, GPO, 0, GPIO_PIN_CONFIG);
    pwm_init(MOTOR2_B, 17000, 0);
}

#define SPEED_RECORD_NUM 50
float speed_Record1[SPEED_RECORD_NUM]={0};
float speed_Record2[SPEED_RECORD_NUM]={0};
int16 Speed_Low_Filter(float new_Spe,float *speed_Record)
{
    float sum = 0.0f;
    for(uint8_t i=SPEED_RECORD_NUM-1;i>0;i--)//���������ݺ���һλ
    {
        speed_Record[i] = speed_Record[i-1];
        sum += speed_Record[i-1];
    }
    speed_Record[0] = new_Spe;//��һλ���µ�����
    sum += new_Spe;
    return (int16)sum/SPEED_RECORD_NUM;//���ؾ�ֵ
}

void getPulseCount()
{
    pulseCount_1 = abs(encoder_get_count(ENCODER_1)/5);// ��ȡ����������
    pulseCount_2 = abs(encoder_get_count(ENCODER_2)/5);// ��ȡ����������
    encoder_clear_count(ENCODER_1);// ��ձ���������
    encoder_clear_count(ENCODER_2);// ��ձ���������

    pulseCount_1 = Speed_Low_Filter(pulseCount_1, speed_Record1);
    pulseCount_2 = Speed_Low_Filter(pulseCount_2, speed_Record2);
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



_pid Lmotor_pid, Rmotor_pid;
uint8 Lindex=1, Rindex=1;

/**
  * @brief  PID������ʼ��
    *   @note   ��
  * @retval ��
  */
void PID_param_init()
{
  /* ��ʼ������ */
  Lmotor_pid.target_val=0;
  Lmotor_pid.actual_val=0.0;
  Lmotor_pid.err = 0.0;
  Lmotor_pid.err_last = 0.0;
  Lmotor_pid.err_next = 0.0;
  Lmotor_pid.Kp = 0.25;
  Lmotor_pid.Ki = 0.0001;
  Lmotor_pid.Kd = 0;

  Rmotor_pid.target_val=0;
  Rmotor_pid.actual_val=0.0;
  Rmotor_pid.err = 0.0;
  Rmotor_pid.err_last = 0.0;
  Rmotor_pid.err_next = 0.0;
  Rmotor_pid.Kp = 0.25;
  Rmotor_pid.Ki = 0.0001;
  Rmotor_pid.Kd = 0;
}

/**
  * @brief  ����Ŀ��ֵ
  * @param  val     Ŀ��ֵ
    *   @note   ��
  * @retv0al ��
  */
void set_pid_target(float temp_val)
{
    Lmotor_pid.target_val = temp_val;    // ���õ�ǰ��Ŀ��ֵ
    Rmotor_pid.target_val = temp_val;
}

/**
  * @brief  ��ȡĿ��ֵ
  * @param  ��
    *   @note   ��
  * @retval Ŀ��ֵ
  */
float get_pid_target(void)
{
  return Lmotor_pid.target_val;    // ���õ�ǰ��Ŀ��ֵ
}

/**
  * @brief  ���ñ��������֡�΢��ϵ��
  * @param  p������ϵ�� P
  * @param  i������ϵ�� i
  * @param  d��΢��ϵ�� d
    *   @note   ��
  * @retval ��
  */
void set_p_i_d(float p, float i, float d)
{
  Lmotor_pid.Kp = p;    // ���ñ���ϵ�� P
  Lmotor_pid.Ki = i;    // ���û���ϵ�� I
  Lmotor_pid.Kd = d;    // ����΢��ϵ�� D

  Rmotor_pid.Kp = p;    // ���ñ���ϵ�� P
  Rmotor_pid.Ki = i;    // ���û���ϵ�� I
  Rmotor_pid.Kd = d;    // ����΢��ϵ�� D
}

/**
  * @brief  PID�㷨ʵ��
  * @param  val     Ŀ��ֵ
    *   @note   ��
  * @retval ͨ��PID���������
  */
float PID_realize(uint8 LR, float temp_val)
{
    if(LR==0)
    {
        /*����Ŀ��ֵ��ʵ��ֵ�����*/
        Lmotor_pid.err = Lmotor_pid.target_val - temp_val;

//        // ���ַ���
//        if(my_abs(Lmotor_pid.err)<5)
//            Lindex=0;
//        else
//            Lindex=1;

        /*PID�㷨ʵ��*/
        Lmotor_pid.actual_val += Lmotor_pid.Kp * (Lmotor_pid.err - Lmotor_pid.err_next)
                     +  Lindex * Lmotor_pid.Ki *  Lmotor_pid.err
                     +  Lmotor_pid.Kd * (Lmotor_pid.err - 2 * Lmotor_pid.err_next + Lmotor_pid.err_last);
        /*�������*/
        Lmotor_pid.err_last = Lmotor_pid.err_next;
        Lmotor_pid.err_next = Lmotor_pid.err;

        /*���ص�ǰʵ��ֵ*/
        return Lmotor_pid.actual_val;
    }
    else
    {
        /*����Ŀ��ֵ��ʵ��ֵ�����*/
        Rmotor_pid.err = Rmotor_pid.target_val - temp_val;

//        if(my_abs(Rmotor_pid.err)<5)
//            Rindex=0;
//        else
//            Rindex=1;

        /*PID�㷨ʵ��*/
        Rmotor_pid.actual_val += Rmotor_pid.Kp * (Rmotor_pid.err - Rmotor_pid.err_next)
                     +  Rindex * Rmotor_pid.Ki *  Rmotor_pid.err
                     +  Rmotor_pid.Kd * (Rmotor_pid.err - 2 * Rmotor_pid.err_next + Rmotor_pid.err_last);
        /*�������*/
        Rmotor_pid.err_last = Rmotor_pid.err_next;
        Rmotor_pid.err_next = Rmotor_pid.err;

        /*���ص�ǰʵ��ֵ*/
        return Rmotor_pid.actual_val;
    }
}
