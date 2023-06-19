/*
 * laiqu.h
 *
 *  Created on: 2023��6��19��
 *      Author: ����
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
