/*
 * gyro.c
 *
 *  Created on: 2023��4��9��
 *      Author: ����
 */

#include "gyro.h"

int16 gyro[3];

// �����ǣ��о�������
void gyro_data()
{
    imu660ra_get_gyro();
    gyro[0] = imu660ra_gyro_x;
    gyro[1] = imu660ra_gyro_y;
    gyro[2] = imu660ra_gyro_z;

    ips200_show_string(0, 240, "gyro:");
    ips200_show_int(40, 240, gyro[0], 4);
    ips200_show_int(80, 240, gyro[1], 4);
    ips200_show_int(120, 240, gyro[2], 4);
}
