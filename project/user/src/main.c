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

// 避障相关
#define OBSTACLE_DIS 600
uint16 distance=0;
long obstacle_pulse_cnt=0;
uint8 obstacle_phase=0;
//uint16 obstacle_delay[7]={180,600,200,300,200,600,180};
#if !CAR_TYPE&&!OBSTACLE_LEFTorRIGHT
long obstacle_pulse[7]={1300,1700,800,500,800,1700,1300};
#elif !CAR_TYPE&&OBSTACLE_LEFTorRIGHT
long obstacle_pulse[7]={800,1700,1300,500,1300,1700,800};
#elif CAR_TYPE && !OBSTACLE_LEFTorRIGHT
long obstacle_pulse[7]={1500,1000,900,500,1200,1700,1500};
#elif CAR_TYPE && OBSTACLE_LEFTorRIGHT
long obstacle_pulse[7]={900,1000,1500,500,1500,1700,1200};
#endif

// 过坡道相关
uint16 slope_cnt=0;

// 调试相关
#if MOTOR_DEBUG_STATUS
#if CAR_TYPE
    float target_pulse[3] = {15, 23, 30};
#else
    float target_pulse[3] = {10, 20, 30};
#endif
#endif

int main (void)
{

    clock_init(SYSTEM_CLOCK_144M);                                              // 初始化芯片时钟 工作频率为 144MHz
    debug_init();                                                               // 初始化默认 Debug UART
    servo_init();
    motor_init();
    mt9v03x_init();
    ips200_init(IPS200_TYPE_PARALLEL8);
    ADC_init();
    tofInit();
    hallInit();

    timer_init(TIM_5, TIMER_MS);//计时器，查看程序运行时间

    encoder_quad_init(ENCODER_1, ENCODER_1_A, ENCODER_1_B);                     // 初始化编码器模块与引脚 正交解码编码器模式
    encoder_quad_init(ENCODER_2, ENCODER_2_A, ENCODER_2_B);                     // 初始化编码器模块与引脚 正交解码编码器模式

    ImagePerspective_Init();

// 如果没装蓝牙但是开启了发送可能由于高频IO而卡顿
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

#if CAR_TYPE && !MOTOR_DEBUG_STATUS && !SERVO_DEBUG_STATUS
    out_garage();
    system_delay_ms(3000);
    wait_for_launch();
#elif !CAR_TYPE && !MOTOR_DEBUG_STATUS && !SERVO_DEBUG_STATUS
    wait_for_charge();
#endif
//
//    motorPWML = 1200;
//    motorPWMR = 1200;
//    set_pid_target(15);

    Main_pit_init();


    while(1)
    {
        printf("Alive!\n");
        // 此处编写需要循环执行的代码
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
        // 此处编写需要循环执行的代码
    }

}

void ips200_show()
{
#if PERSPECTIVE_DEBUG_STATUS
    camera_send_image(DEBUG_UART_INDEX, (const uint8 *)bin_image, MT9V03X_IMAGE_SIZE);
//    bluetooth_ch9141_send_image((const uint8 *)bin_image, MT9V03X_IMAGE_SIZE);
    ips200_show_gray_image(0,0,_img[0],RESULT_COL,RESULT_ROW,RESULT_COL,RESULT_ROW,0);
#else
    ips200_show_gray_image(0,0,_img[0],RESULT_COL,RESULT_ROW,RESULT_COL,RESULT_ROW,0);
#endif

//    Draw_Side();

    ips200_show_string(110, 0, "Angle:");
    ips200_show_float(110, 20, Angle, 5, 2);

//    ips200_show_string(110,40,"Distance: ");
//    ips200_show_int(150,40,distance,5);
    ips200_show_string(110,60,"Voltage: ");
    ips200_show_float(150,60,voltage_now,2,3);

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
    ips200_show_string(0, 60, "target pulse:");
    ips200_show_int(100, 60, Lmotor_pid.target_val, 5);
    ips200_show_int(150, 60, Rmotor_pid.target_val, 5);
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
    ips200_show_int(80, 170, adc_R, 5);
    ips200_show_int(120, 170, adc_RR, 5);//L

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

    static uint32 elec_handler_cnt = 0;// 通过count计数实现delay效果
    if(elec_handler_cnt%Delay_cnt_calc(5)==0)
    {
        getPulseCount();

#if MOTOR_DEBUG_STATUS
        virtual_oscilloscope_data_conversion(pulseCount_1,pulseCount_2,motorPWML,motorPWMR);
//        uart_write_buffer(UART_3, virtual_oscilloscope_data, 10);
        bluetooth_ch9141_send_buff(virtual_oscilloscope_data, 10);
#endif

        motorPWML += PID_realize(0, pulseCount_1);
        motorPWMR += PID_realize(1, pulseCount_2);

        if(motorPWML>2000)
            motorPWML=2000;
        if(motorPWMR>2000)
            motorPWMR=2000;
        if(motorPWML<-2000)
            motorPWML=-2000;
        if(motorPWMR<-2000)
            motorPWMR=-2000;

//        if(0<=motorPWML&&motorPWML<600)
//            motorPWML=600;
//        if(0<=motorPWMR&&motorPWMR<600)
//            motorPWMR=600;
//        if(-600<motorPWML&&motorPWML<=0)
//            motorPWML=-600;
//        if(-600<motorPWMR&&motorPWMR<=0)
//            motorPWMR=-600;


        motor_control(motorPWML, motorPWMR);
    }

//    // 红外避障
//    if (!obstacle_flag && elec_handler_cnt % Delay_cnt_calc(100) == 0)
//    {
//        // 红外测距对不同的颜色的障碍物敏感度不同,对红色障碍物测量值偏大，对蓝色障碍物测量值偏小
//        distance = Get_Distance();
////        printf("Dis: %d",distance);
//        if (distance <= OBSTACLE_DIS)
//        {
////            printf("Obstacle!");
//            obstacle_flag=true;
//            obstacle_pulse_cnt=0;
//        }
//    }
    if(elec_handler_cnt % Delay_cnt_calc(10) == 0){
//        in_garage_flag=isStopLine();
        if(!judgeStopline()){   //用霍尔传感器检测停止线
#if CAR_TYPE
            Stop_At_Stopline();
#else
            In_Garage_with_Hall();
#endif
        }
    }
//    //检测丢线
//    if(!front_diuxian_flag &&(elec_handler_cnt % Delay_cnt_calc(100)) == 0){
//        //todo 需要确定一个划分使正常使用时不误判
//        if(lostline_cnt>=100){
//            front_diuxian_flag=true;
//        }
//    }
//    //电池电量检测
//    if(elec_handler_cnt%Delay_cnt_calc(5000)){
//        Get_Battery_Voltage();
//    }

#if MOTOR_DEBUG_STATUS
    if(elec_handler_cnt%Delay_cnt_calc(500)==2){
        static char buff[64];
        static char *end;
        uint16 data_len = bluetooth_ch9141_read_buff(buff, 64);                 // 查看是否有消息 默认缓冲区是 BLUETOOTH_CH9141_BUFFER_SIZE 总共 64 字节
        if(data_len != 0)                                                       // 收到了消息 读取函数会返回实际读取到的数据个数
        {
            //输入示例：p0.01
            switch (buff[0]) {
                case 'p':
                    Lmotor_pid.Kp=strtof(&buff[1],&end);
                    Rmotor_pid.Kp=strtof(&buff[1],&end);
                    break;
                case 'i':
                    Lmotor_pid.Ki=strtof(&buff[1],&end);
                    Rmotor_pid.Ki=strtof(&buff[1],&end);
                    break;
                case 'd':
                    Lmotor_pid.Kd=strtof(&buff[1],&end);
                    Rmotor_pid.Kd=strtof(&buff[1],&end);
                    break;
                case 'v':
                    set_pid_target(strtof(&buff[1],&end));
                    break;
            }

            memset(buff, 0, 64);//清空缓冲区
        }
    }
#elif SERVO_DEBUG_STATUS
    if(elec_handler_cnt%Delay_cnt_calc(500)==2){
        static char buff[64];
        static char *end;
        uint16 data_len = bluetooth_ch9141_read_buff(buff, 64);                 // 查看是否有消息 默认缓冲区是 BLUETOOTH_CH9141_BUFFER_SIZE 总共 64 字节
        if(data_len != 0)                                                       // 收到了消息 读取函数会返回实际读取到的数据个数
        {
            bluetooth_ch9141_send_buff(buff,data_len);
            bool isModified=true;
            //输入示例：p0.01
            switch (buff[0]) {
                case 'p':
                    elec_Kp = strtof(&buff[1],&end);
                    break;
                case 'd':
                    elec_Kd = strtof(&buff[1],&end);
                    break;
                case 'v':
                    set_pid_target(strtof(&buff[1],&end));
                    break;
                default:
                    isModified=false;
            }
            memset(buff, 0, 64);//清空缓冲区
        }
    }
#endif

    if(!left_circle_flag && !right_circle_flag && !obstacle_flag && !in_garage_flag)
    {
        judgement();
        CURRENT_STATUS = Status_Common;
#if !MOTOR_DEBUG_STATUS&&!SERVO_DEBUG_STATUS
    set_pid_target(NORMAL_PULSE);
#endif
    }
    else if(left_circle_flag || right_circle_flag)
    {
        static uint16 cnt=0;
#if CAR_TYPE
    if(left_circle_flag)
    {
        switch (circle_status)
        {
            case 1:// step1 避开第一个断口(正常巡线应该就行)
                CURRENT_STATUS = Status_Common;

                if(adc_LL>circle_threshold)
                {
                    circle_status++;
                    cnt = 0;
                    break;
                }

                // 如果一边大了之后，两边同时都大了，说明误判了，当前应该是十字
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
                // 如果一边大了之后，两边同时都大了，说明误判了，当前应该是十字
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
                // 如果一边大了之后，两边同时都大了，说明误判了，当前应该是十字
                if(adc_LL>circle_threshold && adc_RR>circle_threshold)
                {
                    circle_status = 0;
                    cnt = 0;
                    left_circle_flag = false;
                    cross_cnt = 0;
                    cross_flag = true;
                }
                break;
            case 4:// step2 第二个断口入环，强行扭头入环
                CURRENT_STATUS = Status_Stop;
                pwm_set_duty(SERVO_PIN, SERVO_MOTOR_DUTY(100));
                if(++cnt>Delay_cnt_calc(500))
                {
                    circle_status++;
                    cnt = 0;
                }
                break;
            case 5:// step3 正常巡线
                CURRENT_STATUS = Status_Common;
                if(adc_RR>circle_threshold)
                {
                    circle_status++;
                }
                break;
            case 6:// step4 出环（正常巡线应该可以）
                CURRENT_STATUS = Status_Stop;
                pwm_set_duty(SERVO_PIN, SERVO_MOTOR_DUTY(98));
                if(++cnt>Delay_cnt_calc(750))// && adc_LL<circle_threshold
                {
                    circle_status++;
                    cnt = 0;
                }
                break;
            case 7:// step5 出环后
                CURRENT_STATUS = Status_Common;
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
            case 1:// step1 避开第一个断口(正常巡线应该就行)
                CURRENT_STATUS = Status_Common;

                if(adc_RR>circle_threshold)
                {
                    circle_status++;
                    cnt = 0;
                    break;
                }

                // 如果一边大了之后，两边同时都大了，说明误判了，当前应该是十字
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
                // 如果一边大了之后，两边同时都大了，说明误判了，当前应该是十字
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
                // 如果一边大了之后，两边同时都大了，说明误判了，当前应该是十字
                if(adc_LL>circle_threshold && adc_RR>circle_threshold)
                {
                    circle_status = 0;
                    cnt = 0;
                    right_circle_flag = false;
                    cross_cnt = 0;
                    cross_flag = true;
                }
                break;
            case 4:// step2 第二个断口入环，强行扭头入环
                CURRENT_STATUS = Status_Stop;
                pwm_set_duty(SERVO_PIN, SERVO_MOTOR_DUTY(82));
                if(++cnt>Delay_cnt_calc(750))
                {
                    circle_status++;
                    cnt = 0;
                }
                break;
            case 5:// step3 正常巡线
                CURRENT_STATUS = Status_Common;
                if(adc_LL>circle_threshold)
                {
                    circle_status++;
                }
                break;
            case 6:// step4 出环（正常巡线应该可以）
                CURRENT_STATUS = Status_Stop;
                pwm_set_duty(SERVO_PIN, SERVO_MOTOR_DUTY(82));
                if(++cnt>Delay_cnt_calc(1000))
                {
                    circle_status++;
                    cnt = 0;
                }
                break;
            case 7:// step5 出环后
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
                case 1:// step1 避开第一个断口(正常巡线应该就行)
                    CURRENT_STATUS = Status_Common;

                    if(adc_LL>circle_threshold)
                    {
                        circle_status++;
                        cnt = 0;
                        break;
                    }

                    // 如果一边大了之后，两边同时都大了，说明误判了，当前应该是十字
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
                    // 如果一边大了之后，两边同时都大了，说明误判了，当前应该是十字
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
                    // 如果一边大了之后，两边同时都大了，说明误判了，当前应该是十字
                    if(adc_LL>circle_threshold && adc_RR>circle_threshold)
                    {
                        circle_status = 0;
                        cnt = 0;
                        left_circle_flag = false;
                        cross_cnt = 0;
                        cross_flag = true;
                    }
                    break;
                case 4:// step2 第二个断口入环，强行扭头入环
                    CURRENT_STATUS = Status_Stop;
                    pwm_set_duty(SERVO_PIN, SERVO_MOTOR_DUTY(98));
                    if(++cnt>Delay_cnt_calc(500))
                    {
                        circle_status++;
                        cnt = 0;
                    }
                    break;
                case 5:// step3 正常巡线
                    CURRENT_STATUS = Status_Common;
                    if(adc_RR>circle_threshold)
                    {
                        circle_status++;
                    }
                    break;
                case 6:// step4 出环（正常巡线应该可以）
                    CURRENT_STATUS = Status_Stop;
                    pwm_set_duty(SERVO_PIN, SERVO_MOTOR_DUTY(95));
                    if(++cnt>Delay_cnt_calc(750))// && adc_LL<circle_threshold
                    {
                        circle_status++;
                        cnt = 0;
                    }
                    break;
                case 7:// step5 出环后
                    CURRENT_STATUS = Status_Common;
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
                case 1:// step1 避开第一个断口(正常巡线应该就行)
                    CURRENT_STATUS = Status_Common;

                    if(adc_RR>circle_threshold)
                    {
                        circle_status++;
                        cnt = 0;
                        break;
                    }

                    // 如果一边大了之后，两边同时都大了，说明误判了，当前应该是十字
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
                    // 如果一边大了之后，两边同时都大了，说明误判了，当前应该是十字
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
                    // 如果一边大了之后，两边同时都大了，说明误判了，当前应该是十字
                    if(adc_LL>circle_threshold && adc_RR>circle_threshold)
                    {
                        circle_status = 0;
                        cnt = 0;
                        right_circle_flag = false;
                        cross_cnt = 0;
                        cross_flag = true;
                    }
                    break;
                case 4:// step2 第二个断口入环，强行扭头入环
                    CURRENT_STATUS = Status_Stop;
                    pwm_set_duty(SERVO_PIN, SERVO_MOTOR_DUTY(85));
                    if(++cnt>Delay_cnt_calc(750))
                    {
                        circle_status++;
                        cnt = 0;
                    }
                    break;
                case 5:// step3 正常巡线
                    CURRENT_STATUS = Status_Common;
                    if(adc_LL>circle_threshold)
                    {
                        circle_status++;
                    }
                    break;
                case 6:// step4 出环（正常巡线应该可以）
                    CURRENT_STATUS = Status_Stop;
                    pwm_set_duty(SERVO_PIN, SERVO_MOTOR_DUTY(85));
                    if(++cnt>Delay_cnt_calc(1000))
                    {
                        circle_status++;
                        cnt = 0;
                    }
                    break;
                case 7:// step5 出环后
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
#endif
    }
    else if(elec_handler_cnt%Delay_cnt_calc(5)==0 && obstacle_flag)
    {
//        printf("%d\n",obstacle_phase);
        CURRENT_STATUS = Status_Stop;
#if OBSTACLE_LEFTorRIGHT
        switch (obstacle_phase)
        {
            case 0:
                pwm_set_duty(SERVO_PIN, SERVO_MOTOR_DUTY(75));
                if(obstacle_pulse_cnt >= obstacle_pulse[0]){
                    obstacle_pulse_cnt=0;
                    obstacle_phase=1;
                }else{
                    obstacle_pulse_cnt+=previous_pulseCount_1;
                }
                break;
            case 1:
                pwm_set_duty(SERVO_PIN, SERVO_MOTOR_DUTY(90));
                if(obstacle_pulse_cnt >= obstacle_pulse[1]){
                    obstacle_pulse_cnt=0;
                    obstacle_phase=2;
                }else{
                    obstacle_pulse_cnt+=previous_pulseCount_1;
                }
                break;
            case 2:
                pwm_set_duty(SERVO_PIN, SERVO_MOTOR_DUTY(105));
                if(obstacle_pulse_cnt >= obstacle_pulse[2]){
                    obstacle_pulse_cnt=0;
                    obstacle_phase=3;
                }else{
                    obstacle_pulse_cnt+=previous_pulseCount_1;
                }
                break;
            case 3:
                pwm_set_duty(SERVO_PIN, SERVO_MOTOR_DUTY(90));
                if(obstacle_pulse_cnt >= obstacle_pulse[3]){
                    obstacle_pulse_cnt=0;
                    obstacle_phase=4;
                }else{
                    obstacle_pulse_cnt+=previous_pulseCount_1;
                }
                break;
            case 4:
                pwm_set_duty(SERVO_PIN, SERVO_MOTOR_DUTY(105));
                if(obstacle_pulse_cnt >= obstacle_pulse[4]){
                    obstacle_pulse_cnt=0;
                    obstacle_phase=5;
                }else{
                    obstacle_pulse_cnt+=previous_pulseCount_1;
                }
                break;
            case 5:
                pwm_set_duty(SERVO_PIN, SERVO_MOTOR_DUTY(90));
                if(obstacle_pulse_cnt >= obstacle_pulse[5]){
                    obstacle_pulse_cnt=0;
                    obstacle_phase=6;
                }else{
                    obstacle_pulse_cnt+=previous_pulseCount_1;
                }
                break;
            case 6:
                pwm_set_duty(SERVO_PIN, SERVO_MOTOR_DUTY(75));
                if(obstacle_pulse_cnt >= obstacle_pulse[6]){
                    obstacle_pulse_cnt=0;
                    obstacle_phase=0;
                    CURRENT_STATUS = Status_Common;
                    obstacle_flag=false;
                    front_diuxian_flag=false;
                }else{
                    obstacle_pulse_cnt+=previous_pulseCount_1;
                }
                break;
            //用于中途跳出避障状态
            case 7:
                obstacle_pulse_cnt=0;
                obstacle_phase=0;
                CURRENT_STATUS = Status_Common;
                obstacle_flag=false;
                front_diuxian_flag=false;
                break;

        }
#else
        switch (obstacle_phase)
        {
            case 0:
                pwm_set_duty(SERVO_PIN, SERVO_MOTOR_DUTY(105));
                if(obstacle_pulse_cnt >= obstacle_pulse[0]){
                    obstacle_pulse_cnt=0;
                    obstacle_phase=1;
                }else{
                    obstacle_pulse_cnt+=previous_pulseCount_1;
                }
                break;
            case 1:
                pwm_set_duty(SERVO_PIN, SERVO_MOTOR_DUTY(90));
                if(obstacle_pulse_cnt >= obstacle_pulse[1]){
                    obstacle_pulse_cnt=0;
                    obstacle_phase=2;
                }else{
                    obstacle_pulse_cnt+=previous_pulseCount_1;
                }
                break;
            case 2:
                pwm_set_duty(SERVO_PIN, SERVO_MOTOR_DUTY(75));
                if(obstacle_pulse_cnt >= obstacle_pulse[2]){
                    obstacle_pulse_cnt=0;
                    obstacle_phase=3;
                }else{
                    obstacle_pulse_cnt+=previous_pulseCount_1;
                }
                break;
            case 3:
                pwm_set_duty(SERVO_PIN, SERVO_MOTOR_DUTY(90));
                if(obstacle_pulse_cnt >= obstacle_pulse[3]){
                    obstacle_pulse_cnt=0;
                    obstacle_phase=4;
                }else{
                    obstacle_pulse_cnt+=previous_pulseCount_1;
                }
                break;
            case 4:
                pwm_set_duty(SERVO_PIN, SERVO_MOTOR_DUTY(75));
                if(obstacle_pulse_cnt >= obstacle_pulse[4]){
                    obstacle_pulse_cnt=0;
                    obstacle_phase=5;
                }else{
                    obstacle_pulse_cnt+=previous_pulseCount_1;
                }
                break;
            case 5:
                pwm_set_duty(SERVO_PIN, SERVO_MOTOR_DUTY(90));
                if(obstacle_pulse_cnt >= obstacle_pulse[5]){
                    obstacle_pulse_cnt=0;
                    obstacle_phase=6;
                }else{
                    obstacle_pulse_cnt+=previous_pulseCount_1;
                }
                break;
            case 6:
                pwm_set_duty(SERVO_PIN, SERVO_MOTOR_DUTY(105));
                if(obstacle_pulse_cnt >= obstacle_pulse[6]){
                    obstacle_pulse_cnt=0;
                    obstacle_phase=0;
                    CURRENT_STATUS = Status_Common;
                    obstacle_flag=false;
                    front_diuxian_flag=false;
                }else{
                    obstacle_pulse_cnt+=previous_pulseCount_1;
                }
                break;
                //用于中途跳出避障状态
            case 7:
                obstacle_pulse_cnt=0;
                obstacle_phase=0;
                CURRENT_STATUS = Status_Common;
                obstacle_flag=false;
                front_diuxian_flag=false;
                break;

        }
#endif
    }
//    if(slope_flag) {
////        if(slope_cnt==0) {
////            motor_control(2500,2500);
////        }
////        if(slope_cnt<Delay_cnt_calc(1000)) {
////            motor_control(2500,2500);
////            slope_cnt++;
////        }else {
////            slope_cnt=0;
////            motor_control(1000,1000);
////            set_pid_target(NORMAL_PULSE);
////            slope_flag=false;
////        }
//
//        for(uint16 i=0;i<1000;i++){
//            getPulseCount();
//            pwm_set_duty(SERVO_PIN, SERVO_MOTOR_DUTY(90));
//            motor_control(2500,2500);
//            system_delay_ms(1);
//        }
//        slope_cnt=0;
//        set_pid_target(NORMAL_PULSE);
//        slope_flag=false;
//        slope_state_CD=5000;
//    }
#if !MOTOR_DEBUG_STATUS || !SERVO_DEBUG_STATUS
    else if(in_garage_flag)
    {
#if !CAR_TYPE && !MOTOR_DEBUG_STATUS && !SERVO_DEBUG_STATUS
//        In_Garage();
#elif CAR_TYPE && !MOTOR_DEBUG_STATUS && !SERVO_DEBUG_STATUS
        Stop_At_Stopline();
#endif
    }
#endif



    if (elec_handler_cnt == Delay_cnt_calc(5000)) {
        elec_handler_cnt = 0;
    }else{
        elec_handler_cnt++;
    }



}

void img_handler()
{
    // 开始计时
    timer_start(TIM_5);

    ImageProcess();
    // 结束计时
    timer_stop(TIM_5);
    ips200_show_string(0, 290, "imgproc time:");
    ips200_show_int(100, 290, (int16)timer_get(TIM_5), 8);
    timer_clear(TIM_5);

    ips200_show();
}
