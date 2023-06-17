/*
 * tof.c
 *
 *  Created on: 2023��5��24��
 *      Author: Lenovo
 */


#include <zf_common_headfile.h>

#define LED1                        (E2 )
#define LED2                        (E9 )

uint16 a=0;

void tofinit()
{

    gpio_init(LED1, GPO, GPIO_HIGH, GPO_PUSH_PULL);                             // ��ʼ�� LED1 ��� Ĭ�ϸߵ�ƽ �������ģʽ
            gpio_init(LED2, GPO, GPIO_HIGH, GPO_PUSH_PULL);                             // ��ʼ�� LED1 ��� Ĭ�ϸߵ�ƽ �������ģʽ
    while(1)
               {
                   if(dl1a_init())
                   {
                       printf("\r\nDL1A init error.");                                     // DL1A ��ʼ��ʧ��
                   }
                   else
                   {
                       break;
                   }
                   gpio_toggle_level(LED1);                                                // ��ת LED ���������ƽ ���� LED ���� ��ʼ����������ƻ����ĺ���
               }
}

void  ceju()
{

    if(0 == a % 40)                                                  // ÿ 40ms ��ȡһ�β����Ϣ ���� 40 ms Ƶ�� 25Hz
       {
           dl1a_get_distance();                                                    // ������Ƶ�ʲ�Ӧ���� 30Hz ���ڲ�Ӧ���� 33.33ms
       }
       a = (a == 995) ? (0) : (a + 5);            // 1000ms ���ڼ���



}

uint16 shuchu()
{
    uint32 output;
    ceju();
    while(1)
    {
//            �˴���д��Ҫѭ��ִ�еĴ���
        system_delay_ms(500);
                printf("\r\nDL1A distance data: %5d", dl1a_distance_mm);
                gpio_toggle_level(LED2);

//            �˴���д��Ҫѭ��ִ�еĴ���
    }

    output=dl1a_distance_mm;
    return output;
}
