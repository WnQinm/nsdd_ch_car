/*
 * gyro.c
 *
 *  Created on: 2023Äê5ÔÂ26ÈÕ
 *      Author: Lenovo
 */


#include "gyro.h"
int16 gyro_x, gyro_y, gyro_z;

void gyro_init()
{
    while(imu660ra_init())
    {
        ips200_show_string(0, 0, "gyro init failed");
    }
    ips200_clear();
}

void gyro_data()
{
    imu660ra_get_gyro();
    gyro_x = imu660ra_gyro_transition(imu660ra_gyro_x);
    gyro_y = imu660ra_gyro_transition(imu660ra_gyro_y);
    gyro_z = imu660ra_gyro_transition(imu660ra_gyro_z);
//    gyro_x = Gyro_Low_Filter(imu660ra_gyro_transition(imu660ra_gyro_x), gyro_Record1);
//    gyro_y = Gyro_Low_Filter(imu660ra_gyro_transition(imu660ra_gyro_y), gyro_Record2);
//    gyro_z = Gyro_Low_Filter(imu660ra_gyro_transition(imu660ra_gyro_z), gyro_Record3);

//    virtual_oscilloscope_data_conversion(gyro_x, gyro_y, gyro_z, 0);
//    bluetooth_ch9141_send_buff(virtual_oscilloscope_data, 10);

}
