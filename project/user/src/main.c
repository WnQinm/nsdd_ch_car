/*********************************************************************************************************************
* CH32V307VCT6 Opensourec Library ����CH32V307VCT6 ��Դ�⣩��һ�����ڹٷ� SDK �ӿڵĵ�������Դ��
* Copyright (c) 2022 SEEKFREE ��ɿƼ�
*
* ���ļ���CH32V307VCT6 ��Դ���һ����
*
* CH32V307VCT6 ��Դ�� ��������
* �����Ը��������������ᷢ���� GPL��GNU General Public License���� GNUͨ�ù������֤��������
* �� GPL �ĵ�3�棨�� GPL3.0������ѡ��ģ��κκ����İ汾�����·�����/���޸���
*
* ����Դ��ķ�����ϣ�����ܷ������ã�����δ�������κεı�֤
* ����û�������������Ի��ʺ��ض���;�ı�֤
* ����ϸ����μ� GPL
*
* ��Ӧ�����յ�����Դ���ͬʱ�յ�һ�� GPL �ĸ���
* ���û�У������<https://www.gnu.org/licenses/>
*
* ����ע����
* ����Դ��ʹ�� GPL3.0 ��Դ���֤Э�� �����������Ϊ���İ汾
* �������Ӣ�İ��� libraries/doc �ļ����µ� GPL3_permission_statement.txt �ļ���
* ���֤������ libraries �ļ����� �����ļ����µ� LICENSE �ļ�
* ��ӭ��λʹ�ò����������� ���޸�����ʱ���뱣����ɿƼ��İ�Ȩ����������������
*
* �ļ�����          main
* ��˾����          �ɶ���ɿƼ����޹�˾
* �汾��Ϣ          �鿴 libraries/doc �ļ����� version �ļ� �汾˵��
* ��������          MounRiver Studio V1.8.1
* ����ƽ̨          CH32V307VCT6
* ��������          https://seekfree.taobao.com/
*
* �޸ļ�¼
* ����                                      ����                             ��ע
* 2022-09-15        ��W            first version
********************************************************************************************************************/
#include <Read_ADC.h>
#include "zf_common_headfile.h"
#include "servo.h"
#include "motor.h"
#include "lai.h"
#include "qu.h"
#include "tof.h"
#include "hall_stopline.h"

#define IMG_PIT_CH                  (TIM6_PIT )                                      // ʹ�õ������жϱ�� ����޸� ��Ҫͬ����Ӧ�޸������жϱ���� isr.c �еĵ���
#define IMG_PIT_PRIORITY            (TIM6_IRQn)




int main (void)
{
    clock_init(SYSTEM_CLOCK_144M);                                              // ��ʼ��оƬʱ�� ����Ƶ��Ϊ 144MHz
    debug_init();                                                               // ��ʼ��Ĭ�� Debug UART
    servo_init();
    motor_init();
//    quinit();
//    laiinit();
    tofinit();
    hallinit();

   ips200_init(IPS200_TYPE_PARALLEL8);
//       adc_init(ADC1_IN10_C0,ADC_8BIT);//c0 l2
      adc_init(ADC1_IN14_C4,ADC_8BIT);//c1 r2
      adc_init(ADC1_IN13_C3,ADC_8BIT);//a7 l1
//      adc_init(ADC1_IN9_B1,ADC_8BIT);//b1 r1
        pit_ms_init(IMG_PIT_CH, 5);
        interrupt_set_priority(IMG_PIT_PRIORITY, 5);




    // �˴���д�û����� ���������ʼ�������
//shuchu();
        // �˴���д�û����� ���������ʼ�������

        while(1)
        {
            // �˴���д��Ҫѭ��ִ�еĴ���
//            stopline();

            // �˴���д��Ҫѭ��ִ�еĴ���
        }

}


