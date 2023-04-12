/*
 * servo.h
 *
 *  Created on: 2023��4��8��
 *      Author: ����
 */

#ifndef SERVO_H_
#define SERVO_H_

#include "zf_common_headfile.h"
#include "imgproc.h"
#include "gyro.h"

#define SERVO_MOTOR_FREQ 50

// ����Ƕ�->ռ�ձ�
#define SERVO_MOTOR_DUTY(x)    (int)((float)PWM_DUTY_MAX / (1000.0 / (float)SERVO_MOTOR_FREQ) * (0.5 + (float)(x) / 90.0))

// ���峵ģѰ���������
#define SERVO_PIN           TIM2_PWM_MAP3_CH1_A15

// �����ֵ����Ҫ����ʵ�ʵ�SERVO_MOTOR_DUTY�еĲ���
#define SERVO_CENTER        90

extern float Angle;

void servo_init(void);
void servo_control();
void AngleControl(void);
void Angle_Get(void);
void Angle_out(void);

#endif /* SERVO_H_ */
