/*
 * qu.c
 *
 *  Created on: 2023年5月20日
 *      Author: Lenovo
 */

#include <zf_common_headfile.h>
#include <zf_device_dm1xa.h>
#include <lai.h>

#define LED1                        (E2 )
#define LED2                        (E9 )

uint16 b=0;

void quinit()
{

       gpio_init(LED1, GPO, GPIO_HIGH, GPO_PUSH_PULL);                             // 初始化 LED1 输出 默认高电平 推挽输出模式
         gpio_init(LED2, GPO, GPIO_HIGH, GPO_PUSH_PULL);                             // 初始化 LED1 输出 默认高电平 推挽输出模式

         while(1)
         {
             if(dm1xa_init())
             {
                 printf("\r\nDM1TA init error.\r\n");                                     // DL1A 初始化失败
             }
             else
             {
                 break;
             }
             gpio_toggle_level(LED1);                                                // 翻转 LED 引脚输出电平 控制 LED 亮灭 初始化出错这个灯会闪的很慢
         }


}

void go()
{
    if(0 == b % 20)                                                  // 每 20ms 发起一次测距
        {
            // 测距调用频率应在 50-100Hz 周期在 10-20ms
            // 测距调用频率应在 50-100Hz 周期在 10-20ms
            // 测距调用频率应在 50-100Hz 周期在 10-20ms
            dm1xa_transmitter_ranging();
        }
        b = (b == 995) ? (0) : (b + 5);            // 1000ms 周期计数
}

void qujiance()
{
    while(1)
       {
           // 此处编写需要循环执行的代码
           system_delay_ms(500);
           gpio_toggle_level(LED2);                                                // 翻转 LED 引脚输出电平 控制 LED 亮灭
           // 此处编写需要循环执行的代码
       }
}


