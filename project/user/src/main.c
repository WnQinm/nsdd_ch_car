#include "zf_common_headfile.h"
#include "servo.h"
#include "motor.h"
#include "laiqu.h"
#include "tof.h"
#include "hall_stopline.h"
#include "Read_ADC.h"
#include "judgement.h"
#include "pit.h"

void img_handler();

int main (void)
{

    clock_init(SYSTEM_CLOCK_144M);                                              // ��ʼ��оƬʱ�� ����Ƶ��Ϊ 144MHz
    debug_init();                                                               // ��ʼ��Ĭ�� Debug UART
    servo_init();
    motor_init();
    mt9v03x_init();
//    quinit();
//    laiinit();
//    tofinit();
//    hallinit();
    ips200_init(IPS200_TYPE_PARALLEL8);
    ADC_init();

    timer_init(TIM_5, TIMER_MS);//��ʱ�����鿴��������ʱ��

    encoder_quad_init(ENCODER_1, ENCODER_1_A, ENCODER_1_B);                     // ��ʼ��������ģ�������� �������������ģʽ
    encoder_quad_init(ENCODER_2, ENCODER_2_A, ENCODER_2_B);                     // ��ʼ��������ģ�������� �������������ģʽ

    ImagePerspective_Init();


    ALL_pit_init();

//    motor_control(800,800);

    while(1)
    {
        // �˴���д��Ҫѭ��ִ�еĴ���
//        if(!out_flag)
//            motor_control(600, 600);
//        else
//            motor_control(0, 0);

        if(mt9v03x_finish_flag)
        {
            img_handler();
            mt9v03x_finish_flag = 0;
        }
        // �˴���д��Ҫѭ��ִ�еĴ���
    }

}

void ips200_show()
{
    ips200_show_gray_image(0,0,_img[0],RESULT_COL,RESULT_ROW,RESULT_COL,RESULT_ROW,0);
//    camera_send_image(DEBUG_UART_INDEX, (const uint8 *)bin_image, MT9V03X_IMAGE_SIZE);
//    bluetooth_ch9141_send_image((const uint8 *)bin_image, MT9V03X_IMAGE_SIZE);
    Draw_Side();

    ips200_draw_line(0, aimLine, RESULT_COL-1, aimLine, RGB565_GREEN);
    ips200_show_int(0, 80, road_width[aimLine], 5);
    ips200_show_int(0, 100, leftline[aimLine], 5);
    ips200_show_int(50, 100, rightline[aimLine], 5);

    ips200_show_string(0, 130, "Angle:");
    ips200_show_float(50, 130, Angle, 5, 2);

    ips200_show_string(0, 150, "elec ADC value:");
    ips200_show_int(0, 170, adc_LL, 5);
    ips200_show_int(40, 170, adc_L, 5);
    ips200_show_int(80, 170, adc_R, 5);
    ips200_show_int(120, 170, adc_RR, 5);

    // motor pid
//    ips200_show_float(0, 190, v1, 5, 5);
//    ips200_show_float(100, 190, v2, 5, 5);
//    ips200_show_int(0, 210, pulseCount_1, 5);
//    ips200_show_int(100, 210, pulseCount_2, 5);
//    ips200_show_int(0, 230, MotorPI(pulseCount_1, -55), 5);
//    ips200_show_int(100, 230, MotorPI(pulseCount_2, -55), 5);

    // camera data
    ips200_show_string(0, 190, "corner point:");
    ips200_show_int(0, 210, LeftBreakpoint.end_y, 4);
    ips200_show_int(40, 210, RightBreakpoint.end_y, 4);
    ips200_show_int(0, 230, LeftBreakpoint.start_y, 4);
    ips200_show_int(40, 230, RightBreakpoint.start_y, 4);

    ips200_show_string(0, 250, "Status");
    ips200_show_int(100, 250, circle_status, 5);
    if(cross_flag)
        ips200_show_string(0, 270, "cross");
    else if(!cross_flag)
        ips200_show_string(0, 270, "     ");
    ips200_show_int(100, 270, cross_cnt, 6);
}

void elec_handler()
{
    Read_ADC();

    if(!left_circle_flag && !right_circle_flag)
    {
        judgement();
        CURRENT_STATUS = Status_Common;
    }
    else
    {
        static uint16 cnt=0;
        if(left_circle_flag)
        {
            switch (circle_status)
            {
                case 1:// step1 �ܿ���һ���Ͽ�(����Ѳ��Ӧ�þ���)
                    CURRENT_STATUS = Status_Common;

                    if(++cnt>2000 && adc_LL>circle_threshold)
                    {
                        circle_status++;
                        cnt = 0;
                        break;
                    }

                    // ���һ�ߴ���֮������ͬʱ�����ˣ�˵�������ˣ���ǰӦ����ʮ��
                    if(adc_LL>circle_threshold && adc_RR>circle_threshold)
                    {
                        circle_status = 0;
                        cnt = 0;
                        left_circle_flag = false;
                        cross_cnt = 0;
                        cross_flag = true;
                    }
                    break;
                case 2:// step2 �ڶ����Ͽ��뻷��ǿ��Ťͷ�뻷
                    CURRENT_STATUS = Status_Stop;
                    pwm_set_duty(SERVO_PIN, SERVO_MOTOR_DUTY(100));
                    if(++cnt>1000)
                    {
                        circle_status++;
                        cnt = 0;
                    }
                    break;
                case 3:// step3 ����Ѳ��
                    CURRENT_STATUS = Status_Common;
                    if(adc_RR>circle_threshold)
                    {
                        circle_status++;
                    }
                    break;
                case 4:// step4 ����������Ѳ��Ӧ�ÿ��ԣ�
                    CURRENT_STATUS = Status_Stop;
                    pwm_set_duty(SERVO_PIN, SERVO_MOTOR_DUTY(98));
                    if(++cnt>1500)// && adc_LL<circle_threshold
                    {
                        circle_status++;
                        cnt = 0;
                    }
                    break;
                case 5:// step5 ������
                    CURRENT_STATUS = Status_RCamera;
                    if(++cnt>1000)
                    {
                        cnt = 0;
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
                case 1:// step1 �ܿ���һ���Ͽ�(����Ѳ��Ӧ�þ���)
                    CURRENT_STATUS = Status_Common;

                    if(++cnt>2000 && adc_RR>circle_threshold)// && current_err_circle>current_err_common
                    {
                        circle_status++;
                        cnt = 0;
                        break;
                    }

                    // ���һ�ߴ���֮������ͬʱ�����ˣ�˵�������ˣ���ǰӦ����ʮ��
                    if(adc_LL>circle_threshold && adc_RR>circle_threshold)
                    {
                        circle_status = 0;
                        cnt = 0;
                        right_circle_flag = false;
                        cross_cnt = 0;
                        cross_flag = true;
                    }
                    break;
                case 2:// step2 �ڶ����Ͽ��뻷��ǿ��Ťͷ�뻷
                    CURRENT_STATUS = Status_Stop;
                    pwm_set_duty(SERVO_PIN, SERVO_MOTOR_DUTY(82));
                    if(++cnt>1500)
                    {
                        circle_status++;
                        cnt = 0;
                    }
                    break;
                case 3:// step3 ����Ѳ��
                    CURRENT_STATUS = Status_Common;
                    if(adc_LL>circle_threshold)
                    {
                        circle_status++;
                    }
                    break;
                case 4:// step4 ����������Ѳ��Ӧ�ÿ��ԣ�
                    CURRENT_STATUS = Status_Stop;
                    pwm_set_duty(SERVO_PIN, SERVO_MOTOR_DUTY(82));
                    if(++cnt>2000)
                    {
                        circle_status++;
                        cnt = 0;
                    }
                    break;
                case 5:// step5 ������
                    CURRENT_STATUS = Status_Common;
                    if(++cnt>1000)// todo �һ����ǹյĲ������󻷻���
                    {
                        cnt = 0;
                        circle_status = 0;
                        right_circle_flag = false;
                    }
                    break;
            }
        }
    }

}

void img_handler()
{
    // ��ʼ��ʱ
    timer_start(TIM_5);

    ImageProcess();

    // ������ʱ
    timer_stop(TIM_5);
    ips200_show_string(0, 290, "imgproc time:");
    ips200_show_int(100, 290, (int16)timer_get(TIM_5), 8);
    timer_clear(TIM_5);

    ips200_show();
}
