/*
 * laiqu.c
 *
 *  Created on: 2023��6��19��
 *      Author: ����
 */

#include "laiqu.h"

void laiInit()
{
    while(1){
        if(dm1xa_init()){
            printf("\r\n DM1RA init error. \r\n");                                     // DL1A ��ʼ��ʧ��
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
            printf("\r\nDM1TA init error.\r\n");                                     // DL1A ��ʼ��ʧ��
        }
        else{
            break;
        }
    }
}

// �����ж�����Ϊ10-20m���ж�����Խ�������ԽԶ
uint16 getCarDist()
{
//    printf("\r\n DM1RA distance data: %5d \r\n", dm1xa_receiver_ranging());
    return dm1xa_receiver_ranging();
}

void sendDistData()
{
    dm1xa_transmitter_ranging();
}
