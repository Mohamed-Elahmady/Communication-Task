// ///*
//// * File:   main.c
//// * Author: mohammedel-ahmady
//// *
//// * Created on June 26, 2025, 7:24 PM
//// */
////
//
//#include "Main.h"
//
//void tmr0_app_isr(void);
//
//GPIO_Led led1 = {.pin.Port = GPIO_PortC, .pin.Pin = GPIO_Pin0, .pin.Direction = GPIO_Output, .connection = Led_Source, .state = Led_Off};
//
//Timer0_CFG t0_timer = {
//    .timer0_interrupt = tmr0_app_isr,
//    .mode = Timer0_Normal_Mode,
//    .operation = Timer0_Timer_Operation,
//    .compare_mode = Timer0_COM_Mode_Disable_OC0,
//    .timer0_ctc_max = 0x00,
//    .prescaler = Timer0_Prescaler_DIV_1024,
//    .timer0_preloaded_value = 0x0C
//};
//
//int main(void) {
//    Std_ReturnType ret = E_NOT_OK;
//    Application_Init();
//    
//    while (1) {
//        
//    }
//    return EXIT_SUCCESS;
//}
//
//void Application_Init(void) {
//    Ecual_Init();
//    Timer0_INIT(&t0_timer);
//    GPIO_Led_INIT(&led1);
//}
//
//void tmr0_app_isr(void){
//    GPIO_Led_Turn_Toggle(&led1);
//}