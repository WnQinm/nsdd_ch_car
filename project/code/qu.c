/*
 * qu.c
 *
 *  Created on: 2023��5��20��
 *      Author: Lenovo
 */

#include "qu.h"

void quInit(){
    while(1){
        if(dm1xa_init()){
            printf("\r\nDM1TA init error.\r\n");                                     // DL1A ��ʼ��ʧ��
        }
        else{
            break;
        }
    }
}

void sendLaiQuDistance(){
    dm1xa_transmitter_ranging();
}
