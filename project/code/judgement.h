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
    Status_Stop   = 0,
    Status_Common = 1,
    Status_Circle = 2,
    Status_Camera = 3
}RaceStatus;

#include "zf_common_headfile.h"
#include "Read_ADC.h"
#include "imgproc.h"
#include "servo.h"

void judgement();

extern RaceStatus CURRENT_STATUS;

extern uint8 left_circle_flag, right_circle_flag, cross_flag;
extern uint8 circle_threshold, cross_threshold;
extern uint8 circle_status;
extern uint8 cross_cnt;
extern bool out_flag;

#endif /* JUDGEMENT_H_ */
