/*
 * tof.c
 *
 *  Created on: 2023��5��24��
 *      Author: Lenovo
 */

#include "tof.h"

void tofInit(){
    while(1){
        if(dl1a_init()){
           printf("\r\nDL1A init error.");                                     // DL1A ��ʼ��ʧ��
        }
        else{
           break;
        }
    }
}

uint16 Get_Distance(){
    dl1a_get_distance();                                                    // ������Ƶ�ʲ�Ӧ���� 30Hz ���ڲ�Ӧ���� 33.33ms
    uint16 output;

//    printf("\r\nDL1A distance data: %5d", dl1a_distance_mm);

    output=dl1a_distance_mm;
    return output;
}
