/*
 * laiqu.c
 *
 *  Created on: 2023年6月19日
 *      Author: 清梦
 */

#include "laiqu.h"

void laiInit()
{
    while(1){
        if(dm1xa_init()){
            printf("\r\n DM1RA init error. \r\n");                                     // DL1A 初始化失败
        }
        else{
            break;
        }
    }
}

void quInit()
{
    while(1){
        if(dm1xa_init()){
            printf("\r\nDM1TA init error.\r\n");                                     // DL1A 初始化失败
        }
        else{
            break;
        }
    }
}

// 调用中断周期为10-20m，中断周期越长，测距越远
uint16 getCarDist()
{
//    printf("\r\n DM1RA distance data: %5d \r\n", dm1xa_receiver_ranging());
    return dm1xa_receiver_ranging();
}

void sendDistData()
{
    dm1xa_transmitter_ranging();
}
