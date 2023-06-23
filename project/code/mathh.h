/*
 * mathh.h
 *
 *  Created on: 2023年4月28日
 *      Author: 清梦
 */

#ifndef MATHH_H_
#define MATHH_H_

#include "imgproc.h"

//用作除法分母,以防除0
#define eps 0.0000001
#define gamma 1e2

uint8 limit(int input, uint8 max, uint8 min);
uint8 my_abs(uint8 x);
uint8 max(uint8 a, uint8 b);
uint8 min(uint8 a, uint8 b);
float slope(uint8 upx, uint8 upy, uint8 downx, uint8 downy);
void CommonRectificate(unsigned char data[],unsigned char begin,unsigned char end);
float Slope_Calculate(uint8 begin,uint8 end,uint8 *p);
int regression(int startline,int endline);

#endif /* MATHH_H_ */
