/*
 * lai.c
 *
 *  Created on: 2023��5��22��
 *      Author: Lenovo
 */


#include <zf_common_headfile.h>
#include <zf_device_dm1xa.h>


#define LED1                        (E2 )
#define LED2                        (E9 )

uint16 pit_ms_count=0;

void laiinit()
{
    gpio_init(LED1, GPO, GPIO_HIGH, GPO_PUSH_PULL);                             // ��ʼ�� LED1 ��� Ĭ�ϸߵ�ƽ �������ģʽ
        gpio_init(LED2, GPO, GPIO_HIGH, GPO_PUSH_PULL);                             // ��ʼ�� LED2 ��� Ĭ�ϸߵ�ƽ �������ģʽ
        while(1)
        {
            if(dm1xa_init())
            {
                printf("\r\n DM1RA init error. \r\n");                                     // DL1A ��ʼ��ʧ��
            }
            else
            {
                break;
            }
            gpio_toggle_level(LED1);                                                // ��ת LED ���������ƽ ���� LED ���� ��ʼ����������ƻ����ĺ���

}
}

uint16 laishuchu()
{

    uint32 output;

while(1)
      {
          // �˴���д��Ҫѭ��ִ�еĴ���
          system_delay_ms(500);
          printf("\r\n DM1RA distance data: %5d \r\n", dm1xa_receiver_ranging());
          gpio_toggle_level(LED2);                                                // ��ת LED ���������ƽ ���� LED ����
          // �˴���д��Ҫѭ��ִ�еĴ���
      }
output=dm1xa_receiver_ranging();

return output;
}

