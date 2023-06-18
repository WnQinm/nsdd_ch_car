/*
 * lai.c
 *
 *  Created on: 2023年5月22日
 *      Author: Lenovo
 */
#include "lai.h"

void laiInit(){
    while(1){
        if(dm1xa_init()){
            printf("\r\n DM1RA init error. \r\n");                                     // DL1A 初始化失败
        }
        else{
            break;
        }
    }
}

uint16 laiQuDistanceOutput(){
    uint32 output;
//    printf("\r\n DM1RA distance data: %5d \r\n", dm1xa_receiver_ranging());

    output=dm1xa_receiver_ranging();
    return output;
}

