/*
 * lai.c
 *
 *  Created on: 2023年5月22日
 *      Author: Lenovo
 */


#include <zf_common_headfile.h>
#include <zf_device_dm1xa.h>


#define LED1                        (E2 )
#define LED2                        (E9 )

uint16 pit_ms_count=0;

void laiinit()
{
    gpio_init(LED1, GPO, GPIO_HIGH, GPO_PUSH_PULL);                             // 初始化 LED1 输出 默认高电平 推挽输出模式
        gpio_init(LED2, GPO, GPIO_HIGH, GPO_PUSH_PULL);                             // 初始化 LED2 输出 默认高电平 推挽输出模式
        while(1)
        {
            if(dm1xa_init())
            {
                printf("\r\n DM1RA init error. \r\n");                                     // DL1A 初始化失败
            }
            else
            {
                break;
            }
            gpio_toggle_level(LED1);                                                // 翻转 LED 引脚输出电平 控制 LED 亮灭 初始化出错这个灯会闪的很慢

}
}

uint16 laishuchu()
{

    uint32 output;

while(1)
      {
          // 此处编写需要循环执行的代码
          system_delay_ms(500);
          printf("\r\n DM1RA distance data: %5d \r\n", dm1xa_receiver_ranging());
          gpio_toggle_level(LED2);                                                // 翻转 LED 引脚输出电平 控制 LED 亮灭
          // 此处编写需要循环执行的代码
      }
output=dm1xa_receiver_ranging();

return output;
}

