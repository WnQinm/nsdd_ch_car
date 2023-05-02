/*
 * mathh.h
 *
 *  Created on: 2023��4��28��
 *      Author: ����
 */

#ifndef MATHH_H_
#define MATHH_H_

#include "imgproc.h"

int16 Limit_Protect(int16 num, int32 min, int32 max);
int16 Fit_Point(uint8 i, float Ka, float Kb);
void Curve1_Fitting(float *Ka, float *Kb, int16 *Start, int16 *Line_Add, int16 Mode);
void Curve3_Fitting(float *Ka, float *Kb, uint8 Start,uint8 End, int16 *Line, int16 Mode);//�������ר�ã������������
int regression(int startline,int endline);

#endif /* MATHH_H_ */
