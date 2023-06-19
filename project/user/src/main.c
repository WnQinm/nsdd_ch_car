#include "zf_common_headfile.h"
#include "servo.h"
#include "motor.h"
#include "lai.h"
#include "qu.h"
#include "tof.h"
#include "hall_stopline.h"
#include "Read_ADC.h"
#include "judgement.h"

#define IMG_PIT_CH                  (TIM6_PIT )                                      // 使用的周期中断编号 如果修改 需要同步对应修改周期中断编号与 isr.c 中的调用
#define IMG_PIT_PRIORITY            (TIM6_IRQn)

int main (void)
{
    clock_init(SYSTEM_CLOCK_144M);                                              // 初始化芯片时钟 工作频率为 144MHz
    debug_init();                                                               // 初始化默认 Debug UART
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
        // 此处编写需要循环执行的代码

        // 此处编写需要循环执行的代码
    }

}

void handler()
{
    Read_ADC();
    judgement();
    if(!left_circle_flag && !right_circle_flag)
        servo_control(Status_Common);
    else
    {
        if(left_circle_flag)
        {
            static int cnt=0;
            switch (circle_status)
            {
                case 1:// step1 避开第一个断口(正常巡线应该就行)
                    servo_control(Status_Common);
                    if(++cnt>100 && adc_LL>circle_threshold)    // 直走半秒
                    {
                        circle_status++;
                        cnt%=100;
                    }
                    break;
                case 2:// step2 第二个断口入环，根据竖电感数值强行扭头入环
                    servo_control(Status_Circle);
                    if(++cnt>100)   // 暂定扭半秒头
                    {
                        circle_status++;
                        cnt%=100;
                    }
                    break;
                case 3:// step3 正常巡线
                    servo_control(Status_Common);
                    if(adc_RR>circle_threshold)
                    {
                        circle_status++;
                    }
                    break;
                case 4:// step4 出环（正常巡线应该可以）
                    servo_control(Status_Common);
                    if(adc_LL>circle_threshold)
                    {
                        circle_status++;
                    }
                    break;
                case 5:// step5 出环后第一个断口摄像头循右线
                    servo_control(Status_Common);
                    if(adc_LL<circle_threshold)
                    {
                        circle_status = 0;
                        left_circle_flag = false;
                    }
                    break;
            }
        }
        else
        {
            static int cnt=0;
            switch (circle_status)
            {
                case 1:// step1 避开第一个断口(正常巡线应该就行)
                    servo_control(Status_Common);
                    if(++cnt>100 && adc_RR>circle_threshold)    // 直走半秒
                    {
                        circle_status++;
                        cnt%=100;
                    }
                    break;
                case 2:// step2 第二个断口入环，根据竖电感数值强行扭头入环
                    servo_control(Status_Circle);
                    if(++cnt>100)   // 暂定扭半秒头
                    {
                        circle_status++;
                        cnt%=100;
                    }
                    break;
                case 3:// step3 正常巡线
                    servo_control(Status_Common);
                    if(adc_LL>circle_threshold)
                    {
                        circle_status++;
                    }
                    break;
                case 4:// step4 出环（正常巡线应该可以）
                    servo_control(Status_Common);
                    if(adc_RR>circle_threshold)
                    {
                        circle_status++;
                    }
                    break;
                case 5:// step5 出环后第一个断口摄像头循右线
                    servo_control(Status_Common);
                    if(adc_RR<circle_threshold)
                    {
                        circle_status = 0;
                        right_circle_flag = false;
                    }
                    break;
            }
        }
    }
}
