/*
 * judgement.h
 *
 *  Created on: 2023年6月18日
 *      Author: 清梦
 */

#ifndef JUDGEMENT_H_
#define JUDGEMENT_H_

// 道路状态枚举
typedef enum
{
    Status_Common = 0,
    Status_Circle = 1,
    Status_Camera = 2
}RaceStatus;

#include "zf_common_headfile.h"
#include "Read_ADC.h"
#include "imgproc.h"
#include "servo.h"

void judgement();

extern uint8 left_circle_flag, right_circle_flag, cross_flag;
extern uint8 circle_threshold, cross_threshold;
extern uint8 circle_status;
extern bool out_flag;

#endif /* JUDGEMENT_H_ */
