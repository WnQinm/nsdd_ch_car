/*
 * qu.c
 *
 *  Created on: 2023年5月20日
 *      Author: Lenovo
 */

#include "qu.h"

void quInit(){
    while(1){
        if(dm1xa_init()){
            printf("\r\nDM1TA init error.\r\n");                                     // DL1A 初始化失败
        }
        else{
            break;
        }
    }
}

void sendLaiQuDistance(){
    dm1xa_transmitter_ranging();
}
