/*
 * servo.h
 *
 *  Created on: 2023��4��18��
 *      Author: Lenovo
 */

#ifndef SERVO_H_
#define SERVO_H_

#include <Read_ADC.h>
#include "zf_common_headfile.h"

#define SERVO_MOTOR_FREQ 50

// ����Ƕ�->ռ�ձ�
#define SERVO_MOTOR_DUTY(x)    ((float)PWM_DUTY_MAX / (1000.0 / (float)SERVO_MOTOR_FREQ) * (0.5 + (float)(x) / 90.0))

// ���峵ģѰ���������
#define SERVO_PIN           TIM2_PWM_MAP3_CH1_A15

// �����ֵ����Ҫ����ʵ�ʵ�SERVO_MOTOR_DUTY�еĲ���
#define SERVO_CENTER        90



#define MAX_SERVO_DUTY 170          // �������Ҫʵ�ʲ���
#define MIN_SERVO_DUTY 10
#define MIDDLE_SERVO_DUTY 90// ��ֵ


void servo_init(void);
void servo_con();
float servo_chabiji(float ad_lm,float ad_rm);

#endif /* SERVO_H_ */
