/*
 * judgement.h
 *
 *  Created on: 2023��6��18��
 *      Author: ����
 */

#ifndef JUDGEMENT_H_
#define JUDGEMENT_H_

#include "zf_common_headfile.h"
#include "Read_ADC.h"

// ��·״̬ö��
typedef enum
{
    Status_Common = 0,
    Status_Circle = 1
}RaceStatus;

void judgement();

extern uint8 left_circle_flag, right_circle_flag, cross_flag;
extern float circle_threshold, cross_threshold;
extern uint8 circle_status;

#endif /* JUDGEMENT_H_ */
