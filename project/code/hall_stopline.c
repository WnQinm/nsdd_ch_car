 /*
 * hall_stopline.c
 *
 *  Created on: 2023年5月26日
 *      Author: Lenovo
 */

#include "hall_stopline.h"

//uint16 stopline_delay[2]={800,800};
uint16 stopline_pulse[3]={2700,4200,3000};  //仅限小车，小车已调整完成
uint16 In_Garage_with_Hall_pulse[4]={1000,300,2500,1200};  //仅限小车
uint16 stopAtStopline_pulse=3000;  //仅限大车，大车已调整完成
//uint16 outgarage_delay[4]={1000,875,300,1500};
uint16 outgarage_pulse[3]={1300,2300,1500};  //仅限大车，大车已调整完成

void hallInit()
{
    gpio_init(HALL_PIN, GPI, GPIO_HIGH, GPI_PULL_UP);
}

bool judgeStopline()
{
    return gpio_get_level(HALL_PIN);
}

void wait_for_charge(){
    //通过撞击的方式发送启动信号
    pwm_set_duty(SERVO_PIN, SERVO_MOTOR_DUTY(90));
    ips200_show_string(0, 0, "wait for charge to start...");

    //todo 电量检测，充满电后再撞击
    system_delay_ms(5000);

    pwm_set_duty(SERVO_PIN, SERVO_MOTOR_DUTY(90));
    ips200_show_string(0, 20, "running out...");
    //撞击
    motor_control(1000,1000);
    system_delay_ms(1000);
    motor_control(0,0);
    system_delay_ms(10000);
    out_garage_flag=true;
    ips200_clear();
}

void wait_for_launch(){
    //通过检测撞击的方式接收启动信号
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
                //直走
                for(uint16 i=0;i<outgarage_pulse[0];i+=previous_pulseCount_1){
                    getPulseCount();
                    pwm_set_duty(SERVO_PIN, SERVO_MOTOR_DUTY(90));
                    motor_control(1200,1200);
                    system_delay_ms(MAIN_PIT_ms_INTERVAL);
                }
                //右转
                for(uint16 i=0;i<outgarage_pulse[1];i+=previous_pulseCount_1){
                    getPulseCount();
                    pwm_set_duty(SERVO_PIN, SERVO_MOTOR_DUTY(75));
                    motor_control(1200,1200);
                    system_delay_ms(MAIN_PIT_ms_INTERVAL);
                }
                //停止
                pwm_set_duty(SERVO_PIN, SERVO_MOTOR_DUTY(90));
                motor_control(0,0);
                system_delay_ms(300);
                getPulseCount();
                //后退，接近小车
                //todo 后续要改成使用有来有去倒车
                for(uint16 i=0;i<outgarage_pulse[2];i+=previous_pulseCount_1){
                    getPulseCount();
                    pwm_set_duty(SERVO_PIN, SERVO_MOTOR_DUTY(90));
                    motor_control(-1000,-1000);
                    system_delay_ms(MAIN_PIT_ms_INTERVAL);
                }
                //停止，等待充电
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
    //直走，接近车库
    for(uint16 i=0;i<stopline_pulse[0];i+=previous_pulseCount_1){
        getPulseCount();
        pwm_set_duty(SERVO_PIN, SERVO_MOTOR_DUTY(90));
        motor_control(1200,1200);
        system_delay_ms(1);
    }
    //右转
    for(uint16 i=0;i<stopline_pulse[1];i+=previous_pulseCount_1){
        getPulseCount();
        pwm_set_duty(SERVO_PIN, SERVO_MOTOR_DUTY(75));
        motor_control(1200,1200);
        system_delay_ms(1);
    }
    //直走入库
    for(uint16 i=0;i<stopline_pulse[2];i+=previous_pulseCount_1){
        getPulseCount();
        pwm_set_duty(SERVO_PIN, SERVO_MOTOR_DUTY(90));
        motor_control(800,800);
        system_delay_ms(1);
    }
    //停止，入库完成
    pwm_set_duty(SERVO_PIN, SERVO_MOTOR_DUTY(90));
    motor_control(0,0);
    ips200_clear();
    ips200_show_string(0,0,"Finished!");
    while(1){}
}

void Stop_At_Stopline(){
    //检测到停止线，需要向前继续走一段
    for(uint16 i=0;i<stopAtStopline_pulse;i+=previous_pulseCount_1){
        getPulseCount();
        pwm_set_duty(SERVO_PIN, SERVO_MOTOR_DUTY(90));
        motor_control(1200,1200);
        system_delay_ms(1);
    }
    //停止，停车完成
    pwm_set_duty(SERVO_PIN, SERVO_MOTOR_DUTY(90));
    motor_control(0,0);
    ips200_clear();
    ips200_show_string(0,0,"Finished!");
    while(1){}
}

void In_Garage_with_Hall()
{
    printf("Garage!\n");
    //直走
    for(uint16 i=0;i<In_Garage_with_Hall_pulse[0];i+=previous_pulseCount_1){
        getPulseCount();
        pwm_set_duty(SERVO_PIN, SERVO_MOTOR_DUTY(90));
        motor_control(1200,1200);
        system_delay_ms(1);
    }
    printf("P1 Finished!\n");
    //停稳
    pwm_set_duty(SERVO_PIN, SERVO_MOTOR_DUTY(90));
    motor_control(0,0);
    system_delay_ms(In_Garage_with_Hall_pulse[1]);
    printf("P2 Finished!\n");
    //倒车入库
    for(uint16 i=0;i<In_Garage_with_Hall_pulse[2];i+=previous_pulseCount_1){
        getPulseCount();
        pwm_set_duty(SERVO_PIN, SERVO_MOTOR_DUTY(75));
        motor_control(-1200,-1200);
        system_delay_ms(1);
    }
    printf("P3 Finished!\n");
    //直线入库
    for(uint16 i=0;i<In_Garage_with_Hall_pulse[3];i+=previous_pulseCount_1){
        getPulseCount();
        pwm_set_duty(SERVO_PIN, SERVO_MOTOR_DUTY(90));
        motor_control(-1200,-1200);
        system_delay_ms(1);
    }
    printf("P4 Finished!\n");
    //停止，入库完成
    pwm_set_duty(SERVO_PIN, SERVO_MOTOR_DUTY(90));
    motor_control(0,0);
    ips200_clear();
    ips200_show_string(0,0,"Finished!");
    while(1){}
}
