/*
 * qu.c
 *
 *  Created on: 2023��5��20��
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

       gpio_init(LED1, GPO, GPIO_HIGH, GPO_PUSH_PULL);                             // ��ʼ�� LED1 ��� Ĭ�ϸߵ�ƽ �������ģʽ
         gpio_init(LED2, GPO, GPIO_HIGH, GPO_PUSH_PULL);                             // ��ʼ�� LED1 ��� Ĭ�ϸߵ�ƽ �������ģʽ

         while(1)
         {
             if(dm1xa_init())
             {
                 printf("\r\nDM1TA init error.\r\n");                                     // DL1A ��ʼ��ʧ��
             }
             else
             {
                 break;
             }
             gpio_toggle_level(LED1);                                                // ��ת LED ���������ƽ ���� LED ���� ��ʼ����������ƻ����ĺ���
         }


}

void go()
{
    if(0 == b % 20)                                                  // ÿ 20ms ����һ�β��
        {
            // ������Ƶ��Ӧ�� 50-100Hz ������ 10-20ms
            // ������Ƶ��Ӧ�� 50-100Hz ������ 10-20ms
            // ������Ƶ��Ӧ�� 50-100Hz ������ 10-20ms
            dm1xa_transmitter_ranging();
        }
        b = (b == 995) ? (0) : (b + 5);            // 1000ms ���ڼ���
}

void qujiance()
{
    while(1)
       {
           // �˴���д��Ҫѭ��ִ�еĴ���
           system_delay_ms(500);
           gpio_toggle_level(LED2);                                                // ��ת LED ���������ƽ ���� LED ����
           // �˴���д��Ҫѭ��ִ�еĴ���
       }
}


