#include "zf_common_headfile.h"
#include "servo.h"
#include "motor.h"
#include "laiqu.h"
#include "tof.h"
#include "hall_stopline.h"
#include "Read_ADC.h"
#include "judgement.h"

#define IMG_PIT_CH                  (TIM7_PIT )                                      // 使用的周期中断编号 如果修改 需要同步对应修改周期中断编号与 isr.c 中的调用
#define IMG_PIT_PRIORITY            (TIM7_IRQn)

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

//    motor_control(600,600);


    while(1)
    {
        // 此处编写需要循环执行的代码

        // 此处编写需要循环执行的代码
    }

}

void handler()
{
    Read_ADC();
    ips200_show_string(0, 120, "Status");
    ips200_show_int(100, 120, circle_status, 5);
    if(!left_circle_flag && !right_circle_flag)
    {
        judgement();
        servo_control(Status_Common);
    }
    else
    {
        static int cnt=0;
        if(left_circle_flag)
        {
            switch (circle_status)
            {
                case 1:// step1 避开第一个断口(正常巡线应该就行)
                    servo_control(Status_Common);
                    if(++cnt>100 && adc_LL>circle_threshold)// && current_err_circle>current_err_common
                    {
                        circle_status++;
                        cnt%=100;
                    }
                    break;
                case 2:// step2 第二个断口入环，根据竖电感数值强行扭头入环
//                    servo_control(Status_Circle);
                    pwm_set_duty(SERVO_PIN, SERVO_MOTOR_DUTY(98));
                    if(++cnt>200)
                    {
                        circle_status++;
                        cnt%=200;
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
                    pwm_set_duty(SERVO_PIN, SERVO_MOTOR_DUTY(98));
                    if(++cnt>200)// && adc_LL<circle_threshold
                    {
                        circle_status++;
                        cnt%=200;
                    }
                    break;
                case 5:// step5 出环后
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
            switch (circle_status)
            {
                case 1:// step1 避开第一个断口(正常巡线应该就行)
                    servo_control(Status_Common);
                    if(++cnt>100 && adc_RR>circle_threshold)// && current_err_circle>current_err_common
                    {
                        circle_status++;
                        cnt%=100;
                    }
                    break;
                case 2:// step2 第二个断口入环，根据竖电感数值强行扭头入环
//                    servo_control(Status_Circle);
                    pwm_set_duty(SERVO_PIN, SERVO_MOTOR_DUTY(82));
                    if(++cnt>200)
                    {
                        circle_status++;
                        cnt%=200;
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
                    pwm_set_duty(SERVO_PIN, SERVO_MOTOR_DUTY(82));
                    if(++cnt>200)// && adc_RR<circle_threshold
                    {
                        circle_status++;
                        cnt%=200;
                    }
                    break;
                case 5:// step5 出环后
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
