/*
 * motor.h
 *
 *  Created on: 2023��4��18��
 *      Author: Lenovo
 */

#ifndef MOTOR_H_
#define MOTOR_H_

#include "zf_common_headfile.h"
#include "pit.h"
#include "config.h"
#include "mathh.h"

//��ʼ�����PWM���źͷ�������
#define MOTOR1_A   D12                      // ����1�������ת����
#define MOTOR1_B   TIM4_PWM_MAP1_CH2_D13         // ����1���PWM����

#define MOTOR2_A   D15                      // ����2�������ת����
#define MOTOR2_B   TIM4_PWM_MAP1_CH3_D14         // ����2���PWM����

#define NORMAL_DUTY 800

/*pid*/
typedef struct
{
    float target_val;     //Ŀ��ֵ
    float actual_val;     //ʵ��ֵ
    float err;            //���嵱ǰƫ��ֵ
    float err_next;       //������һ��ƫ��ֵ
    float err_last;       //�������һ��ƫ��ֵ
    float Kp, Ki, Kd;     //������������֡�΢��ϵ��
}_pid;

void motor_init(void);
void motor_control(int32 duty_1, int32 duty_2);
void getPulseCount();

extern _pid Lmotor_pid, Rmotor_pid;
extern int16 pulseCount_1, pulseCount_2;
extern int16 motorPWML, motorPWMR;
void PID_param_init();
void set_pid_target(float temp_val);
float get_pid_target(void);
void set_p_i_d(float p, float i, float d);
float PID_realize(uint8 LR, float temp_val);

#endif /* MOTOR_H_ */
