#include "zf_common_headfile.h"
#include "servo.h"
#include "motor.h"
#include "lai.h"
#include "qu.h"
#include "tof.h"
#include "hall_stopline.h"
#include "Read_ADC.h"
#include "judgement.h"

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
//    tofinit();
//    hallinit();
    ips200_init(IPS200_TYPE_PARALLEL8);
    ADC_init();

    pit_ms_init(IMG_PIT_CH, 5);
    interrupt_set_priority(IMG_PIT_PRIORITY, 5);

    motor_control(800,800);

    while(1)
    {
        // �˴���д��Ҫѭ��ִ�еĴ���

        // �˴���д��Ҫѭ��ִ�еĴ���
    }

}

void handler()
{
    Read_ADC();
//    judgement();
    if(!left_circle_flag && !right_circle_flag)
        servo_control(Status_Common);
    else
    {
        if(left_circle_flag)
        {
            // step1 �ܿ���һ���Ͽ�
            // step2 �ڶ����Ͽ��뻷�������������ֵ�н�
            // step3 ����Ѳ��
            // step4 ����������Ѳ��Ӧ�ÿ��ԣ�
            // step5 �������һ���Ͽ�����ͷѭ���߳�����adc_M���͵�һ����ֵ
        }
    }
}
