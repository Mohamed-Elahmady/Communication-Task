///*
// * File:   main.c
// * Author: mohammedel-ahmady
// *
// * Created on September 13, 2025, 4:55 PM
// */
//
//#include "Main.h"
//#include <util/delay.h>
//#include "../MCAL_Layer/PWM/hal_pwm.h"
//
///* PWM Configurations */
//PWM_CFG pwm0 = {
//    .pwm_src = PWM_OC0,
//    .pwm_mode = PWM_Fast_Mode,
//    .pwm_res = PWM_8Bit,
//    .pwm_act = PWM_Non_Inverting_OCXX_Clear_Pin,
//    .pwm_prescaler = PWM_Prescaler_DIV_1
//};
//
//PWM_CFG pwm1A = {
//    .pwm_src = PWM_OCA1,
//    .pwm_mode = PWM_Fast_Mode,
//    .pwm_res = PWM_8Bit,
//    .pwm_act = PWM_Non_Inverting_OCXX_Clear_Pin,
//    .pwm_prescaler = PWM_Prescaler_DIV_1
//};
//
//PWM_CFG pwm1B = {
//    .pwm_src = PWM_OCB1,
//    .pwm_mode = PWM_Fast_Mode,
//    .pwm_res = PWM_8Bit,
//    .pwm_act = PWM_Non_Inverting_OCXX_Clear_Pin,
//    .pwm_prescaler = PWM_Prescaler_DIV_1
//};
//
//PWM_CFG pwm2 = {
//    .pwm_src = PWM_OC2,
//    .pwm_mode = PWM_Fast_Mode,
//    .pwm_res = PWM_8Bit,
//    .pwm_act = PWM_Non_Inverting_OCXX_Clear_Pin,
//    .pwm_prescaler = PWM_Prescaler_DIV_1
//};
//
//void Application_Init(void);
//
//int main(void) {
//    Application_Init();
//
//    while (1) {
//        /* ===== Run all PWM channels together ===== */
//        PWM_Start(&pwm0);
//        PWM_Start(&pwm1A);
//        PWM_Start(&pwm1B);
//        PWM_Start(&pwm2);
//
//        for (uint8 duty = 0; duty <= 255; duty += 5) {
//            PWM_Set_Duty_Cycle(&pwm0, duty);
//            PWM_Set_Duty_Cycle(&pwm1A, duty);
//            PWM_Set_Duty_Cycle(&pwm1B, duty);
//            PWM_Set_Duty_Cycle(&pwm2, duty);
//            _delay_ms(1);
//        }
//
//        _delay_ms(1000);
//
//        for (uint16 duty = 255; duty >= 0; duty -= 5) {
//            PWM_Set_Duty_Cycle(&pwm0, duty);
//            PWM_Set_Duty_Cycle(&pwm1A, duty);
//            PWM_Set_Duty_Cycle(&pwm1B, duty);
//            PWM_Set_Duty_Cycle(&pwm2, duty);
//            _delay_ms(1);
//        }
//
//        _delay_ms(1000);
//    }
//
//    return EXIT_SUCCESS;
//}
//
//void Application_Init(void) {
//    Ecual_Init();
//    PWM_INIT(&pwm0);
//    PWM_INIT(&pwm1A);
//    PWM_INIT(&pwm1B);
//    PWM_INIT(&pwm2);
//}