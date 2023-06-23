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

//��ʼ�����PWM���źͷ�������
#define MOTOR1_A   D12                      // ����1�������ת����
#define MOTOR1_B   TIM4_PWM_MAP1_CH2_D13         // ����1���PWM����

#define MOTOR2_A   D15                      // ����2�������ת����
#define MOTOR2_B   TIM4_PWM_MAP1_CH3_D14         // ����2���PWM����

void motor_init(void);
void motor_control(int32 duty_1, int32 duty_2);
void motor_pid(int16 encoder, int16 expect_speed);

extern float v1, v2;
extern float pulseCount_1, pulseCount_2;
void getVelocity(float interval);
int16 MotorPI (int16 Encoder,int16 Target);

#endif /* MOTOR_H_ */
