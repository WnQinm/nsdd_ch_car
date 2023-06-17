/*
 * tof.c
 *
 *  Created on: 2023年5月24日
 *      Author: Lenovo
 */


#include <zf_common_headfile.h>

#define LED1                        (E2 )
#define LED2                        (E9 )

uint16 a=0;

void tofinit()
{

    gpio_init(LED1, GPO, GPIO_HIGH, GPO_PUSH_PULL);                             // 初始化 LED1 输出 默认高电平 推挽输出模式
            gpio_init(LED2, GPO, GPIO_HIGH, GPO_PUSH_PULL);                             // 初始化 LED1 输出 默认高电平 推挽输出模式
    while(1)
               {
                   if(dl1a_init())
                   {
                       printf("\r\nDL1A init error.");                                     // DL1A 初始化失败
                   }
                   else
                   {
                       break;
                   }
                   gpio_toggle_level(LED1);                                                // 翻转 LED 引脚输出电平 控制 LED 亮灭 初始化出错这个灯会闪的很慢
               }
}

void  ceju()
{

    if(0 == a % 40)                                                  // 每 40ms 获取一次测距信息 周期 40 ms 频率 25Hz
       {
           dl1a_get_distance();                                                    // 测距调用频率不应高于 30Hz 周期不应低于 33.33ms
       }
       a = (a == 995) ? (0) : (a + 5);            // 1000ms 周期计数



}

uint16 shuchu()
{
    uint32 output;
    ceju();
    while(1)
    {
//            此处编写需要循环执行的代码
        system_delay_ms(500);
                printf("\r\nDL1A distance data: %5d", dl1a_distance_mm);
                gpio_toggle_level(LED2);

//            此处编写需要循环执行的代码
    }

    output=dl1a_distance_mm;
    return output;
}
