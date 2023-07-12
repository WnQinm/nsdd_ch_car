#include "zf_common_headfile.h"
#include "servo.h"
#include "motor.h"
#include "laiqu.h"
#include "tof.h"
#include "hall_stopline.h"
#include "Read_ADC.h"
#include "judgement.h"
#include "pit.h"
#include "zf_device_virtual_oscilloscope.h"
#include "config.h"

void img_handler();

// �������
uint16 distance=0;
uint16 obstacle_cnt=0;
uint8 obstacle_phase=0;

// ������
float tmp_angle=90;

uint16 slope_cnt=0;

// �������
#if MOTOR_DEBUG_STATUS
#if CAR_TYPE
    float target_pulse[3] = {15, 23, 30};
#else
    float target_pulse[3] = {10, 20, 30};
#endif
#endif

int main (void)
{

    clock_init(SYSTEM_CLOCK_144M);                                              // ��ʼ��оƬʱ�� ����Ƶ��Ϊ 144MHz
    debug_init();                                                               // ��ʼ��Ĭ�� Debug UART
    servo_init();
    motor_init();
    mt9v03x_init();
    ips200_init(IPS200_TYPE_PARALLEL8);

    ADC_init();
//    tofInit();
//    hallInit();

    timer_init(TIM_5, TIMER_MS);//��ʱ�����鿴��������ʱ��

    encoder_quad_init(ENCODER_1, ENCODER_1_A, ENCODER_1_B);                     // ��ʼ��������ģ�������� �������������ģʽ
    encoder_quad_init(ENCODER_2, ENCODER_2_A, ENCODER_2_B);                     // ��ʼ��������ģ�������� �������������ģʽ

    ImagePerspective_Init();

// ���ûװ�������ǿ����˷��Ϳ������ڸ�ƵIO������
#if MOTOR_DEBUG_STATUS || SERVO_DEBUG_STATUS
//    uart_init(UART_3, 115200, UART3_MAP0_TX_B10, UART3_MAP0_RX_B11);
    while(bluetooth_ch9141_init())
    {
        ips200_show_string(0, 0, "bluetooth init failed");
    }
    ips200_clear();
#endif

    key_init(5);
    PID_param_init();
    // 1m/s 54pulse/5ms

#if !MOTOR_DEBUG_STATUS && !SERVO_DEBUG_STATUS
    set_pid_target(15);
//    out_garage();
#endif
//
//    motorPWML = 1200;
//    motorPWMR = 1200;

    Main_pit_init();


    while(1)
    {
        // �˴���д��Ҫѭ��ִ�еĴ���
#if MOTOR_DEBUG_STATUS
        key_scanner();
        switch(key_get_state(KEY_1))
        {
            case KEY_SHORT_PRESS:
            case KEY_LONG_PRESS:
                set_pid_target(target_pulse[0]);
                break;
            default:
                break;
        }
        switch(key_get_state(KEY_2))
        {
            case KEY_SHORT_PRESS:
            case KEY_LONG_PRESS:
                set_pid_target(target_pulse[1]);
                break;
            default:
                break;
        }
        switch(key_get_state(KEY_3))
        {
            case KEY_SHORT_PRESS:
            case KEY_LONG_PRESS:
                set_pid_target(target_pulse[2]);
                break;
            default:
                break;
        }
#endif

        if(mt9v03x_finish_flag)
        {
            img_handler();

//#if !MOTOR_DEBUG_STATUS && !SERVO_DEBUG_STATUS
//            if(isStopLine())
//            {
//                in_garage_flag=isStopLine();
//            }
//#endif

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

    ips200_show_string(110, 0, "Angle:");
    ips200_show_float(110, 20, Angle, 5, 2);

//    ips200_show_string(110,40,"Distance: ");
//    ips200_show_int(150,40,distance,5);
//    ips200_show_string(110,60,"Voltage: ");
//    ips200_show_float(150,60,voltage_now,2,3);

    // motor pid
#if MOTOR_DEBUG_STATUS
    ips200_show_string(0, 80, "pulse count:");
    ips200_show_int(100, 80, pulseCount_1, 5);
    ips200_show_int(150, 80, pulseCount_2, 5);
    ips200_show_string(0, 100, "motor:");
    ips200_show_int(70, 100, motorPWML, 5);
    ips200_show_int(120, 100, motorPWMR, 5);
    ips200_show_string(0, 120, "target pulse:");
    ips200_show_int(100, 120, get_pid_target(), 3);

    ips200_show_string(0, 140, "Kp");
    ips200_show_float(40, 140, Rmotor_pid.Kp, 2, 6);
    ips200_show_string(0, 160, "Ki");
    ips200_show_float(40, 160, Rmotor_pid.Ki, 2, 6);
    ips200_show_string(0, 180, "Kd");
    ips200_show_float(40, 180, Rmotor_pid.Kd, 2, 6);

#elif SERVO_DEBUG_STATUS
    ips200_show_string(0, 80, "servo pid:");
    ips200_show_string(0, 100, "Kp:");
    ips200_show_float(40, 100, elec_Kp, 5, 5);
    ips200_show_string(0, 120, "Kd:");
    ips200_show_float(40, 120, elec_Kd, 5, 5);
#else
    ips200_show_string(0, 70, "pulse count:");
    ips200_show_int(100, 70, pulseCount_1, 5);
    ips200_show_int(150, 70, pulseCount_2, 5);
    ips200_show_string(0, 90, "target pulse:");
    ips200_show_int(100, 90, get_pid_target(), 3);
    ips200_show_string(0, 150, "elec ADC value:");
    ips200_show_int(0, 170, adc_LL, 5);
    ips200_show_int(40, 170, adc_L, 5);
    ips200_show_int(80, 170, adc_M, 5);
    ips200_show_int(120, 170, adc_R, 5);
    ips200_show_int(160, 170, adc_RR, 5);

    // camera data
    ips200_show_string(0, 190, "corner point:");
    ips200_show_int(0, 210, LeftBreakpoint.end_y, 4);
    ips200_show_int(40, 210, RightBreakpoint.end_y, 4);
    ips200_show_int(0, 230, LeftBreakpoint.start_y, 4);
    ips200_show_int(40, 230, RightBreakpoint.start_y, 4);

//    ips200_draw_line(0, aimLine, RESULT_COL-1, aimLine, RGB565_GREEN);
//    ips200_show_int(0, 80, road_width[aimLine], 5);
//    ips200_show_int(0, 100, leftline[aimLine], 5);
//    ips200_show_int(50, 100, rightline[aimLine], 5);

    ips200_show_string(0, 250, "Status");
    ips200_show_int(100, 250, circle_status, 5);
    if(cross_flag)
        ips200_show_string(0, 270, "cross");
    else if(!cross_flag)
        ips200_show_string(0, 270, "     ");
    ips200_show_int(100, 270, cross_cnt, 6);

#endif
}

void elec_handler()
{
    // adc+encoder+scope...<200us

    Read_ADC();

    static uint32 elec_handler_cnt = 0;// ͨ��count����ʵ��delayЧ��
    if(elec_handler_cnt%Delay_cnt_calc(5)==0)
    {
        getPulseCount();

//#if MOTOR_DEBUG_STATUS
//        virtual_oscilloscope_data_conversion(pulseCount_1,pulseCount_2,motorPWML,motorPWMR);
////        uart_write_buffer(UART_3, virtual_oscilloscope_data, 10);
//        bluetooth_ch9141_send_buff(virtual_oscilloscope_data, 10);
//#endif

        motorPWML += PID_realize(0, pulseCount_1);
        motorPWMR += PID_realize(1, pulseCount_2);

        if(motorPWML>3000)
            motorPWML = 3000;
        if(motorPWMR>3000)
            motorPWMR=3000;

        motor_control(motorPWML, motorPWMR);
    }

    // �������
//    if (!obstacle_flag && !slope_flag && elec_handler_cnt % Delay_cnt_calc(100) == 0)
//    {
//        // ������Բ�ͬ����ɫ���ϰ������жȲ�ͬ,�Ժ�ɫ�ϰ������ֵƫ�󣬶���ɫ�ϰ������ֵƫС
//        distance = Get_Distance();
//        if (distance <= 600 && lostline_cnt>Road_Width_Min)
//        {
//            obstacle_flag=true;
//            obstacle_cnt=0;
//        }
//        else if(distance <= 600 && lostline_cnt<=Road_Width_Min)
//        {
//            slope_flag = true;
//        }
//    }

//#if MOTOR_DEBUG_STATUS
//    if(elec_handler_cnt%Delay_cnt_calc(500)==2){
//        static char buff[64];
//        static char *end;
//        uint16 data_len = bluetooth_ch9141_read_buff(buff, 64);                 // �鿴�Ƿ�����Ϣ Ĭ�ϻ������� BLUETOOTH_CH9141_BUFFER_SIZE �ܹ� 64 �ֽ�
//        if(data_len != 0)                                                       // �յ�����Ϣ ��ȡ�����᷵��ʵ�ʶ�ȡ�������ݸ���
//        {
//            //����ʾ����p0.01
//            switch (buff[0]) {
//                case 'p':
//                    Lmotor_pid.Kp=strtof(&buff[1],&end);
//                    Rmotor_pid.Kp=strtof(&buff[1],&end);
//                    break;
//                case 'i':
//                    Lmotor_pid.Ki=strtof(&buff[1],&end);
//                    Rmotor_pid.Ki=strtof(&buff[1],&end);
//                    break;
//                case 'd':
//                    Lmotor_pid.Kd=strtof(&buff[1],&end);
//                    Rmotor_pid.Kd=strtof(&buff[1],&end);
//                    break;
//                case 'v':
//                    set_pid_target(strtof(&buff[1],&end));
//                    break;
//            }
//
//            memset(buff, 0, 64);//��ջ�����
//        }
//    }
//#elif SERVO_DEBUG_STATUS
//    if(elec_handler_cnt%Delay_cnt_calc(500)==2){
//        static char buff[64];
//        static char *end;
//        uint16 data_len = bluetooth_ch9141_read_buff(buff, 64);                 // �鿴�Ƿ�����Ϣ Ĭ�ϻ������� BLUETOOTH_CH9141_BUFFER_SIZE �ܹ� 64 �ֽ�
//        if(data_len != 0)                                                       // �յ�����Ϣ ��ȡ�����᷵��ʵ�ʶ�ȡ�������ݸ���
//        {
//            bool isModified=true;
//            //����ʾ����p0.01
//            switch (buff[0]) {
//                case 'p':
//                    elec_Kp = strtof(&buff[1],&end);
//                    break;
//                case 'd':
//                    elec_Kd = strtof(&buff[1],&end);
//                    break;
//                default:
//                    isModified=false;
//            }
//            memset(buff, 0, 64);//��ջ�����
//        }
//    }
//#endif

    if(!left_circle_flag && !right_circle_flag && !obstacle_flag && !slope_flag && !in_garage_flag)
    {
        judgement();
        CURRENT_STATUS = Status_Common;
    }
    else if(left_circle_flag || right_circle_flag)
    {
        static uint16 cnt=0;
#if CAR_TYPE
        if(left_circle_flag)
        {
            switch (circle_status)
            {
                case 1:// step1 �ܿ���һ���Ͽ�(����Ѳ��Ӧ�þ���)
                    CURRENT_STATUS = Status_Common;

                    if(adc_LL>circle_threshold)
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
                case 2:
                    CURRENT_STATUS = Status_Common;
                    if(adc_LL<circle_threshold/2 && adc_RR<circle_threshold)
                    {
                        circle_status++;
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
                case 3:
                    CURRENT_STATUS = Status_Common;
                    if(adc_LL>circle_threshold-circle_threshold/10 && adc_RR<circle_threshold)
                    {
                        circle_status++;
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
                case 4:// step2 �ڶ����Ͽ��뻷��ǿ��Ťͷ�뻷
                    CURRENT_STATUS = Status_Stop;
                    pwm_set_duty(SERVO_PIN, SERVO_MOTOR_DUTY(100));
                    if(++cnt>Delay_cnt_calc(500))
                    {
                        circle_status++;
                        cnt=0;
                    }
                    break;
                case 5:// step3 ����Ѳ��
                    CURRENT_STATUS = Status_Common;
                    tmp_angle = (tmp_angle+Angle)/2;
                    if(adc_RR>circle_threshold-circle_threshold/10*2)
                    {
                        circle_status++;
                        tmp_angle = tmp_angle+2;
                    }
                    break;
                case 6:// step4 ����������Ѳ��Ӧ�ÿ��ԣ�
                    CURRENT_STATUS = Status_Stop;
                    pwm_set_duty(SERVO_PIN, SERVO_MOTOR_DUTY(tmp_angle));
                    if(++cnt>Delay_cnt_calc(500))
                    {
                        circle_status++;
                        cnt = 0;
                    }
                    break;
                case 7:// step5 ������
                    CURRENT_STATUS = Status_Common;
                    if(++cnt>Delay_cnt_calc(500))
                    {
                        cnt=0;
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

                    if(adc_RR>circle_threshold)
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
                case 2:
                    CURRENT_STATUS = Status_Common;
                    if(adc_RR<circle_threshold/2 && adc_LL<circle_threshold)
                    {
                        circle_status++;
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
                case 3:
                    CURRENT_STATUS = Status_Common;
                    if(adc_RR>circle_threshold-circle_threshold/10 && adc_LL<circle_threshold)
                    {
                        circle_status++;
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
                case 4:// step2 �ڶ����Ͽ��뻷��ǿ��Ťͷ�뻷
                    CURRENT_STATUS = Status_Stop;
                    pwm_set_duty(SERVO_PIN, SERVO_MOTOR_DUTY(82));
                    if(++cnt>Delay_cnt_calc(500))
                    {
                        circle_status++;
                        cnt = 0;
                    }
                    break;
                case 5:// step3 ����Ѳ��
                    CURRENT_STATUS = Status_Common;
                    tmp_angle = (tmp_angle+Angle)/2;
                    if(adc_LL>circle_threshold-circle_threshold/10*2)
                    {
                        circle_status++;
                        tmp_angle=tmp_angle-3;
                    }
                    break;
                case 6:// step4 ����������Ѳ��Ӧ�ÿ��ԣ�
                    CURRENT_STATUS = Status_Stop;
                    pwm_set_duty(SERVO_PIN, SERVO_MOTOR_DUTY(tmp_angle));
                    if(++cnt>Delay_cnt_calc(500))
                    {
                        circle_status++;
                        cnt = 0;
                    }
                    break;
                case 7:// step5 ������
                    CURRENT_STATUS = Status_Common;
                    if(++cnt>Delay_cnt_calc(500))
                    {
                        cnt = 0;
                        circle_status = 0;
                        right_circle_flag = false;
                    }
                    break;
            }
        }
#else
        if(left_circle_flag)
        {
            switch (circle_status)
            {
                case 1:// step1 �ܿ���һ���Ͽ�(����Ѳ��Ӧ�þ���)
                    CURRENT_STATUS = Status_Common;

                    if(adc_LL>circle_threshold)
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
                case 2:
                    CURRENT_STATUS = Status_Common;
                    if(adc_LL<circle_threshold/2 && adc_RR<circle_threshold)
                    {
                        circle_status++;
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
                case 3:
                    CURRENT_STATUS = Status_Common;
                    if(adc_LL>circle_threshold-circle_threshold/10 && adc_RR<circle_threshold)
                    {
                        circle_status++;
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
                case 4:// step2 �ڶ����Ͽ��뻷��ǿ��Ťͷ�뻷
                    CURRENT_STATUS = Status_Stop;
                    pwm_set_duty(SERVO_PIN, SERVO_MOTOR_DUTY(98));
                    if(++cnt>Delay_cnt_calc(500))
                    {
                        circle_status++;
                        cnt=0;
                    }
                    break;
                case 5:// step3 ����Ѳ��
                    CURRENT_STATUS = Status_Common;
                    if(adc_RR>circle_threshold-cross_threshold/10)
                    {
                        circle_status++;
                    }
                    break;
                case 6:// step4 ����
                    CURRENT_STATUS = Status_Stop;
                    pwm_set_duty(SERVO_PIN, SERVO_MOTOR_DUTY(95));
                    if(++cnt>Delay_cnt_calc(750))
                    {
                        circle_status++;
                        cnt=0;
                    }
                    break;
                case 7:// step5 ������
                    CURRENT_STATUS = Status_RCamera;
                    if(++cnt>Delay_cnt_calc(500))
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

                    if(adc_RR>circle_threshold)
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
                case 2:
                    CURRENT_STATUS = Status_Common;
                    if(adc_RR<circle_threshold/2 && adc_LL<circle_threshold)
                    {
                        circle_status++;
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
                case 3:
                    CURRENT_STATUS = Status_Common;
                    if(adc_RR>circle_threshold-circle_threshold/10 && adc_LL<circle_threshold)
                    {
                        circle_status++;
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
                case 4:// step2 �ڶ����Ͽ��뻷��ǿ��Ťͷ�뻷
                    CURRENT_STATUS = Status_Stop;
                    pwm_set_duty(SERVO_PIN, SERVO_MOTOR_DUTY(82));
                    if(++cnt>Delay_cnt_calc(500))
                    {
                        circle_status++;
                        cnt=0;
                    }
                    break;
                case 5:// step3 ����Ѳ��
                    CURRENT_STATUS = Status_Common;
                    if(adc_LL>circle_threshold-cross_threshold/10)
                    {
                        circle_status++;
                    }
                    break;
                case 6:// step4 ����������Ѳ��Ӧ�ÿ��ԣ�
                    CURRENT_STATUS = Status_Stop;
                    pwm_set_duty(SERVO_PIN, SERVO_MOTOR_DUTY(85));
                    if(++cnt>Delay_cnt_calc(750))
                    {
                        circle_status++;
                        cnt=0;
                    }
                    break;
                case 7:// step5 ������
                    CURRENT_STATUS = Status_LCamera;
                    if(++cnt>Delay_cnt_calc(500))
                    {
                        cnt = 0;
                        circle_status = 0;
                        right_circle_flag = false;
                    }
                    break;
            }
        }
#endif
    }
//    else if(obstacle_flag)
//    {
//        set_pid_target(15);
//        CURRENT_STATUS = Status_Stop;
//        switch (obstacle_phase)
//        {
//            case 0:
//                pwm_set_duty(SERVO_PIN, SERVO_MOTOR_DUTY(75));
//                if(obstacle_cnt>=Delay_cnt_calc(180)){
//                    obstacle_cnt=0;
//                    obstacle_phase=1;
//                }else{
//                    obstacle_cnt++;
//                }
//                break;
//            case 1:
//                pwm_set_duty(SERVO_PIN, SERVO_MOTOR_DUTY(90));
//                if(obstacle_cnt>=Delay_cnt_calc(600)){
//                    obstacle_cnt=0;
//                    obstacle_phase=2;
//                }else{
//                    obstacle_cnt++;
//                }
//                break;
//            case 2:
//                pwm_set_duty(SERVO_PIN, SERVO_MOTOR_DUTY(105));
//                if(obstacle_cnt>=Delay_cnt_calc(200)){
//                    obstacle_cnt=0;
//                    obstacle_phase=3;
//                }else{
//                    obstacle_cnt++;
//                }
//                break;
//            case 3:
//                pwm_set_duty(SERVO_PIN, SERVO_MOTOR_DUTY(90));
//                if(obstacle_cnt>=Delay_cnt_calc(300)){
//                    obstacle_cnt=0;
//                    obstacle_phase=4;
//                }else{
//                    obstacle_cnt++;
//                }
//                break;
//            case 4:
//                pwm_set_duty(SERVO_PIN, SERVO_MOTOR_DUTY(105));
//                if(obstacle_cnt>=Delay_cnt_calc(200)){
//                    obstacle_cnt=0;
//                    obstacle_phase=5;
//                }else{
//                    obstacle_cnt++;
//                }
//                break;
//            case 5:
//                pwm_set_duty(SERVO_PIN, SERVO_MOTOR_DUTY(90));
//                if(obstacle_cnt>=Delay_cnt_calc(600)){
//                    obstacle_cnt=0;
//                    obstacle_phase=6;
//                }else{
//                    obstacle_cnt++;
//                }
//                break;
//            case 6:
//                pwm_set_duty(SERVO_PIN, SERVO_MOTOR_DUTY(75));
//                if(obstacle_cnt>=Delay_cnt_calc(180)){
//                    obstacle_cnt=0;
//                    obstacle_phase=0;
//                    obstacle_flag=false;
//                }else{
//                    obstacle_cnt++;
//                }
//                break;
//        }
//    }
    else if(in_garage_flag)
    {
        In_Garage();
    }
    else if(slope_flag)
    {
        CURRENT_STATUS=Status_Common;
        if(slope_cnt<Delay_cnt_calc(500))
        {
            set_pid_target(50);
            slope_cnt++;
        }
        else
        {
           slope_cnt=0;
           slope_flag=false;
           set_pid_target(15);
        }
    }

    //��ص������
//    if(elec_handler_cnt%Delay_cnt_calc(5000)){
//        Get_Battery_Voltage();
//    }

    if (elec_handler_cnt == Delay_cnt_calc(5000)) {
        elec_handler_cnt = 0;
    }else{
        elec_handler_cnt++;
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
