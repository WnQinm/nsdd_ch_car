 /*
 * hall_stopline.c
 *
 *  Created on: 2023��5��26��
 *      Author: Lenovo
 */

#include "hall_stopline.h"

//uint16 stopline_delay[2]={800,800};
uint16 stopline_pulse[3]={2700,4200,3000};  //����С����С���ѵ������
uint16 In_Garage_with_Hall_pulse[4]={1000,300,2500,1200};  //����С��
uint16 stopAtStopline_pulse=3000;  //���޴󳵣����ѵ������
//uint16 outgarage_delay[4]={1000,875,300,1500};
uint16 outgarage_pulse[3]={1300,2300,1500};  //���޴󳵣����ѵ������

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

    //todo ������⣬���������ײ��
    system_delay_ms(5000);

    pwm_set_duty(SERVO_PIN, SERVO_MOTOR_DUTY(90));
    ips200_show_string(0, 20, "running out...");
    //ײ��
    motor_control(1000,1000);
    system_delay_ms(1000);
    motor_control(0,0);
    system_delay_ms(10000);
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
        key_scanner();
        switch(key_get_state(KEY_1))
        {
            case KEY_SHORT_PRESS:
            case KEY_LONG_PRESS:
                ips200_show_string(0, 20, "running out...");
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
                //todo ����Ҫ�ĳ�ʹ��������ȥ����
                for(uint16 i=0;i<outgarage_pulse[2];i+=previous_pulseCount_1){
                    getPulseCount();
                    pwm_set_duty(SERVO_PIN, SERVO_MOTOR_DUTY(90));
                    motor_control(-1000,-1000);
                    system_delay_ms(MAIN_PIT_ms_INTERVAL);
                }
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
    printf("Garage!\n");
    //ֱ��
    for(uint16 i=0;i<In_Garage_with_Hall_pulse[0];i+=previous_pulseCount_1){
        getPulseCount();
        pwm_set_duty(SERVO_PIN, SERVO_MOTOR_DUTY(90));
        motor_control(1200,1200);
        system_delay_ms(1);
    }
    printf("P1 Finished!\n");
    //ͣ��
    pwm_set_duty(SERVO_PIN, SERVO_MOTOR_DUTY(90));
    motor_control(0,0);
    system_delay_ms(In_Garage_with_Hall_pulse[1]);
    printf("P2 Finished!\n");
    //�������
    for(uint16 i=0;i<In_Garage_with_Hall_pulse[2];i+=previous_pulseCount_1){
        getPulseCount();
        pwm_set_duty(SERVO_PIN, SERVO_MOTOR_DUTY(75));
        motor_control(-1200,-1200);
        system_delay_ms(1);
    }
    printf("P3 Finished!\n");
    //ֱ�����
    for(uint16 i=0;i<In_Garage_with_Hall_pulse[3];i+=previous_pulseCount_1){
        getPulseCount();
        pwm_set_duty(SERVO_PIN, SERVO_MOTOR_DUTY(90));
        motor_control(-1200,-1200);
        system_delay_ms(1);
    }
    printf("P4 Finished!\n");
    //ֹͣ��������
    pwm_set_duty(SERVO_PIN, SERVO_MOTOR_DUTY(90));
    motor_control(0,0);
    ips200_clear();
    ips200_show_string(0,0,"Finished!");
    while(1){}
}
