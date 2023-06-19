/*
 * laiqu.h
 *
 *  Created on: 2023年6月19日
 *      Author: 清梦
 */

#ifndef LAIQU_H_
#define LAIQU_H_

#include "zf_common_headfile.h"

typedef enum
{
    lai = 0,
    qu = 1
}laiqu;

void laiInit();
uint16 getCarDist();
void quInit();
void sendDistData();

#endif /* LAIQU_H_ */
