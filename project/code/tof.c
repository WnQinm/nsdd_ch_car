/*
 * tof.c
 *
 *  Created on: 2023年5月24日
 *      Author: Lenovo
 */

#include "tof.h"

void tofInit(){
    while(1){
        if(dl1a_init()){
           printf("\r\nDL1A init error.");                                     // DL1A 初始化失败
        }
        else{
           break;
        }
    }
}

uint16 Get_Distance(){
    dl1a_get_distance();                                                    // 测距调用频率不应高于 30Hz 周期不应低于 33.33ms
    uint16 output;

//    printf("\r\nDL1A distance data: %5d", dl1a_distance_mm);

    output=dl1a_distance_mm;
    return output;
}
