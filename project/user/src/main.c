#include "zf_common_headfile.h"
#include "servo.h"
#include "motor.h"
#include "laiqu.h"
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

    motor_control(600,600);

    while(1)
    {
        // �˴���д��Ҫѭ��ִ�еĴ���

        // �˴���д��Ҫѭ��ִ�еĴ���
    }

}

void handler()
{
    Read_ADC();
    if(!left_circle_flag && !right_circle_flag)
    {
        judgement();
        servo_control(Status_Common);
    }
    else
    {
        if(left_circle_flag)
        {
            ips200_show_string(0, 120, "Status");
            ips200_show_int(100, 120, circle_status, 5);
            static int cnt=0;
            switch (circle_status)
            {
                case 1:// step1 �ܿ���һ���Ͽ�(����Ѳ��Ӧ�þ���)
                    servo_control(Status_Common);
                    if(++cnt>150 && adc_LL>circle_threshold && current_err_circle>current_err_common)    // ֱ��һ��
                    {
                        circle_status++;
                        cnt%=150;
                    }
                    break;
                case 2:// step2 �ڶ����Ͽ��뻷�������������ֵǿ��Ťͷ�뻷
                    servo_control(Status_Circle);
                    if(++cnt>400)   // �ݶ�Ť����ͷ
                    {
                        circle_status++;
                        cnt%=400;
                    }
                    break;
                case 3:// step3 ����Ѳ��
                    servo_control(Status_Common);
                    // todo �жϷ�ʽ��Ϊ����ͷ��⵽ĳЩ��������������������߽ǵ�֮��ģ���������ǰհ�ĸ���
                    if(adc_RR>circle_threshold)
                    {
                        circle_status++;
                    }
                    break;
                case 4:// step4 ����������Ѳ��Ӧ�ÿ��ԣ�
//                    servo_control(Status_Common);
                    // todo ������Դ�һ���ܶ�ʱ��Ľǣ�Ȼ������ͷѭ����
                    Angle = 98;
                    if(++cnt>200 && adc_LL<circle_threshold)
                    {
                        circle_status++;
                        cnt%=200;
                    }
                    break;
                case 5:// step5 ������
                    // todo �������һ���Ͽ�(�жϷ�ʽ�����ǿ�ȱ������������)����ͷ����ѭ����
                    servo_control(Status_Common);
                    if(++cnt>200 && adc_LL<circle_threshold)
                    {
                        cnt%=200;
                        circle_status = 0;
                        left_circle_flag = false;
                    }
                    break;
            }
        }
        else
        {
            ips200_show_string(0, 120, "Status");
            ips200_show_int(100, 120, circle_status, 5);
            static int cnt=0;
            switch (circle_status)
            {
                case 1:// step1 �ܿ���һ���Ͽ�(����Ѳ��Ӧ�þ���)
                    servo_control(Status_Common);
                    if(++cnt>150 && adc_RR>circle_threshold)    // ֱ��һ��
                    {
                        circle_status++;
                        cnt%=150;
                    }
                    break;
                case 2:// step2 �ڶ����Ͽ��뻷�������������ֵǿ��Ťͷ�뻷
                    servo_control(Status_Circle);
                    if(++cnt>400)   // �ݶ�Ť����ͷ
                    {
                        circle_status++;
                        cnt%=400;
                    }
                    break;
                case 3:// step3 ����Ѳ��
                    servo_control(Status_Common);
                    // todo �жϷ�ʽ��Ϊ����ͷ��⵽ĳЩ��������������������߽ǵ�֮��ģ���������ǰհ�ĸ���
                    if(adc_LL>circle_threshold)
                    {
                        circle_status++;
                    }
                    break;
                case 4:// step4 ����������Ѳ��Ӧ�ÿ��ԣ�
//                    servo_control(Status_Common);
                    // todo ������Դ�һ���ܶ�ʱ��Ľǣ�Ȼ������ͷѭ����
                    Angle = 82;
                    if(++cnt>200 && adc_RR<circle_threshold)
                    {
                        circle_status++;
                        cnt%=200;
                    }
                    break;
                case 5:// step5 ������
                    // todo �������һ���Ͽ�(�жϷ�ʽ�����ǿ�ȱ������������)����ͷ����ѭ����
                    servo_control(Status_Common);
                    if(++cnt>200 && adc_RR<circle_threshold)
                    {
                        cnt%=200;
                        circle_status = 0;
                        right_circle_flag = false;
                    }
                    break;
            }
        }
    }
}
