/*
 * pit.h
 *
 *  Created on: 2023��4��8��
 *      Author: ����
 */

#ifndef PIT_H_
#define PIT_H_

#include "zf_common_headfile.h"
#define IMG_PIT_CH                  (TIM6_PIT )                                      // ʹ�õ������жϱ�� ����޸� ��Ҫͬ����Ӧ�޸������жϱ���� isr.c �еĵ���
#define IMG_PIT_PRIORITY            (TIM6_IRQn)                                      // ��Ӧ�����жϵ��жϱ��

void IMG_pit_init();

#endif /* PIT_H_ */
