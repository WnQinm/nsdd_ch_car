 /*
 * hall_stopline.c
 *
 *  Created on: 2023��5��26��
 *      Author: Lenovo
 */

#include "hall_stopline.h"

//uint16 stopline_delay[2]={800,800};
//uint16 outgarage_delay[4]={1000,875,300,1500};

void hallInit()
{
    gpio_init(HALL_PIN, GPI, GPIO_HIGH, GPI_PULL_UP);
}

bool judgeStopline()
{
    return gpio_get_level(HALL_PIN);
}

void wait_for_charge(){
    //ͨ��ײ���ķ�ʽ���������ź�
    pwm_set_duty(SERVO_PIN, SERVO_MOTOR_DUTY(90));
    ips200_show_string(0, 0, "wait for charge to start...");
//    uint8 cnt=0;
    bool key=false;
    while(voltage_now<=12 && !key){
        float voltage=Get_Battery_Voltage();
        ips200_show_string(0,20,"Voltage");
        ips200_show_float(70,20,voltage,3,3);
//        if(cnt>=200){
//            break;
//        }else{
//            cnt++;
//            system_delay_ms(10);
//        }
        key_scanner();
        switch(key_get_state(KEY_1))
        {
            case KEY_SHORT_PRESS:
            case KEY_LONG_PRESS:
                key=true;
                break;
            default:
                break;
        }
    }

    pwm_set_duty(SERVO_PIN, SERVO_MOTOR_DUTY(90));
    ips200_show_string(0, 20, "running out...");
    //ײ��
    motor_control(1500,1500);
    system_delay_ms(3000);
    motor_control(0,0);
    system_delay_ms(5000);
    out_garage_flag=true;
    ips200_clear();
}

void wait_for_launch(){
    //ͨ�����ײ���ķ�ʽ���������ź�
    getPulseCount();
    do {
        getPulseCount();
//        printf("%d\n",previous_pulseCount_1);
        system_delay_ms(50);
    }while(previous_pulseCount_1<=10);
}

void out_garage()
{
    ips200_show_string(0, 0, "wait for key1 to start...");
    while(!out_garage_flag)
    {
        pwm_set_duty(SERVO_PIN, SERVO_MOTOR_DUTY(90));
        key_scanner();
        switch(key_get_state(KEY_1))
        {
            case KEY_SHORT_PRESS:
            case KEY_LONG_PRESS:
                ips200_show_string(0, 20, "running out...");
#if !LAUNCH_FORWARD
                //ֱ��
                for(uint16 i=0;i<outgarage_pulse[0];i+=previous_pulseCount_1){
                    getPulseCount();
                    pwm_set_duty(SERVO_PIN, SERVO_MOTOR_DUTY(90));
                    motor_control(1200,1200);
                    system_delay_ms(MAIN_PIT_ms_INTERVAL);
                }
                //��ת
                for(uint16 i=0;i<outgarage_pulse[1];i+=previous_pulseCount_1){
                    getPulseCount();
                    pwm_set_duty(SERVO_PIN, SERVO_MOTOR_DUTY(75));
                    motor_control(1200,1200);
                    system_delay_ms(MAIN_PIT_ms_INTERVAL);
                }
                //ֹͣ
                pwm_set_duty(SERVO_PIN, SERVO_MOTOR_DUTY(90));
                motor_control(0,0);
                system_delay_ms(300);
                getPulseCount();
                //���ˣ��ӽ�С��
                for(uint16 i=0;i<outgarage_pulse[2];i+=previous_pulseCount_1){
                    getPulseCount();
                    pwm_set_duty(SERVO_PIN, SERVO_MOTOR_DUTY(90));
                    motor_control(-1000,-1000);
                    system_delay_ms(MAIN_PIT_ms_INTERVAL);
                }
#else
                //ֱ��
                for(uint16 i=0;i<outgarage_pulse[0];i+=previous_pulseCount_1){
                    getPulseCount();
                    pwm_set_duty(SERVO_PIN, SERVO_MOTOR_DUTY(90));
                    motor_control(1200,1200);
                    system_delay_ms(MAIN_PIT_ms_INTERVAL);
                }
                //��ת
                for(uint16 i=0;i<outgarage_pulse[1];i+=previous_pulseCount_1){
                    getPulseCount();
                    pwm_set_duty(SERVO_PIN, SERVO_MOTOR_DUTY(105));
                    motor_control(1200,1200);
                    system_delay_ms(MAIN_PIT_ms_INTERVAL);
                }
                //ֹͣ
                pwm_set_duty(SERVO_PIN, SERVO_MOTOR_DUTY(90));
                motor_control(0,0);
                system_delay_ms(300);
                getPulseCount();
                //���ˣ��ӽ�С��
                for(uint16 i=0;i<outgarage_pulse[2];i+=previous_pulseCount_1){
                    getPulseCount();
                    pwm_set_duty(SERVO_PIN, SERVO_MOTOR_DUTY(90));
                    motor_control(-1000,-1000);
                    system_delay_ms(MAIN_PIT_ms_INTERVAL);
                }
#endif
                //ֹͣ���ȴ����
                pwm_set_duty(SERVO_PIN, SERVO_MOTOR_DUTY(90));
                motor_control(0,0);
                out_garage_flag = true;
                break;
            default:
                break;
        }
    }
    getPulseCount();
    ips200_clear();
}

void In_Garage()
{
    //ֱ�ߣ��ӽ�����
    for(uint16 i=0;i<stopline_pulse[0];i+=previous_pulseCount_1){
        getPulseCount();
        pwm_set_duty(SERVO_PIN, SERVO_MOTOR_DUTY(90));
        motor_control(1200,1200);
        system_delay_ms(1);
    }
    //��ת
    for(uint16 i=0;i<stopline_pulse[1];i+=previous_pulseCount_1){
        getPulseCount();
        pwm_set_duty(SERVO_PIN, SERVO_MOTOR_DUTY(75));
        motor_control(1200,1200);
        system_delay_ms(1);
    }
    //ֱ�����
    for(uint16 i=0;i<stopline_pulse[2];i+=previous_pulseCount_1){
        getPulseCount();
        pwm_set_duty(SERVO_PIN, SERVO_MOTOR_DUTY(90));
        motor_control(800,800);
        system_delay_ms(1);
    }
    //ֹͣ��������
    pwm_set_duty(SERVO_PIN, SERVO_MOTOR_DUTY(90));
    motor_control(0,0);
    ips200_clear();
    ips200_show_string(0,0,"Finished!");
    while(1){}
}

void Stop_At_Stopline(){
    //��⵽ֹͣ�ߣ���Ҫ��ǰ������һ��
    for(uint16 i=0;i<stopAtStopline_pulse;i+=previous_pulseCount_1){
        getPulseCount();
        pwm_set_duty(SERVO_PIN, SERVO_MOTOR_DUTY(90));
        motor_control(1200,1200);
        system_delay_ms(1);
    }
    //ֹͣ��ͣ�����
    pwm_set_duty(SERVO_PIN, SERVO_MOTOR_DUTY(90));
    motor_control(0,0);
    ips200_clear();
    ips200_show_string(0,0,"Finished!");
    while(1){}
}

void In_Garage_with_Hall()
{
    ips200_clear();
    ips200_show_string(0,0,"Garage!");
#if !LAUNCH_FORWARD
    //ֱ��
    for(uint16 i=0;i<In_Garage_with_Hall_pulse[0];i+=previous_pulseCount_1){
        getPulseCount();
        pwm_set_duty(SERVO_PIN, SERVO_MOTOR_DUTY(90));
        motor_control(1200,1200);
        system_delay_ms(1);
    }
    ips200_show_string(0,20,"P1 Finished!");
    //ͣ��
    pwm_set_duty(SERVO_PIN, SERVO_MOTOR_DUTY(90));
    motor_control(0,0);
    system_delay_ms(In_Garage_with_Hall_pulse[1]);
    ips200_show_string(0,40,"P2 Finished!");
    //�������
    for(uint16 i=0;i<In_Garage_with_Hall_pulse[2];i+=previous_pulseCount_1){
        getPulseCount();
        pwm_set_duty(SERVO_PIN, SERVO_MOTOR_DUTY(75));
        motor_control(-1200,-1200);
        system_delay_ms(1);
    }
    ips200_show_string(0,60,"P3 Finished!");
    //ֱ�����
    for(uint16 i=0;i<In_Garage_with_Hall_pulse[3];i+=previous_pulseCount_1){
        getPulseCount();
        pwm_set_duty(SERVO_PIN, SERVO_MOTOR_DUTY(90));
        motor_control(-1200,-1200);
        system_delay_ms(1);
    }
    ips200_show_string(0,80,"P4 Finished!");
    //ֹͣ��������
    pwm_set_duty(SERVO_PIN, SERVO_MOTOR_DUTY(90));
    motor_control(0,0);
#else
    //ֱ��
    for(uint16 i=0;i<In_Garage_with_Hall_pulse[0];i+=previous_pulseCount_1){
        getPulseCount();
        pwm_set_duty(SERVO_PIN, SERVO_MOTOR_DUTY(90));
        motor_control(1200,1200);
        system_delay_ms(1);
    }
    ips200_show_string(0,20,"P1 Finished!");
    //ͣ��
    pwm_set_duty(SERVO_PIN, SERVO_MOTOR_DUTY(90));
    motor_control(0,0);
    system_delay_ms(In_Garage_with_Hall_pulse[1]);
    ips200_show_string(0,40,"P2 Finished!");
    //�������
    for(uint16 i=0;i<In_Garage_with_Hall_pulse[2];i+=previous_pulseCount_1){
        getPulseCount();
        pwm_set_duty(SERVO_PIN, SERVO_MOTOR_DUTY(105));
        motor_control(-1200,-1200);
        system_delay_ms(1);
    }
    ips200_show_string(0,60,"P3 Finished!");
    //ֱ�����
    for(uint16 i=0;i<In_Garage_with_Hall_pulse[3];i+=previous_pulseCount_1){
        getPulseCount();
        pwm_set_duty(SERVO_PIN, SERVO_MOTOR_DUTY(90));
        motor_control(-1200,-1200);
        system_delay_ms(1);
    }
    ips200_show_string(0,80,"P4 Finished!");
    //ֹͣ��������
    pwm_set_duty(SERVO_PIN, SERVO_MOTOR_DUTY(90));
    motor_control(0,0);
#endif
    ips200_clear();
    ips200_show_string(0,100,"Finished!");
    while(1){}
}
