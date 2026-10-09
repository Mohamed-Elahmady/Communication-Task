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
//void tmr1_app_isr(void);
//
//GPIO_Led led1 = {.pin.Port = GPIO_PortC, .pin.Pin = GPIO_Pin0, .pin.Direction = GPIO_Output, .connection = Led_Source, .state = Led_Off};
//
////Timer1_CFG t1_timer ={
////    .timer1_interrupt = tmr1_app_isr,
////    .mode = Timer1_Normal_Mode,
////    .operation = Timer1_Timer,
////    .prescaler = Timer1_Prescaler_DIV_1024,
////    .com_mode = Timer1_COM_Mode_Disable_OC1X,
////    .preloaded_value = 0xC327
////};
////
////Timer1_CFG t1_counter ={
////    .timer1_interrupt = tmr1_app_isr,
////    .mode = Timer1_Normal_Mode,
////    .operation = Timer1_Counter,
////    .prescaler = Timer1_Counter_Rising_Edge,
////    .com_mode = Timer1_COM_Mode_Disable_OC1X,
////    .preloaded_value = 0x0000
////};
//
////Timer1_CFG t1_timer_ctc_ocr = {
////    .timer1_interrupt = tmr1_app_isr,
////    .mode = Timer1_Counter_CTC_OCR_Mode,
////    .operation = Timer1_Timer,
////    .prescaler = Timer1_Prescaler_DIV_1024,
////    .channel = Timer1_Counter_Mode_Channel_OC1A,
////    .com_mode = Timer1_COM_Mode_Disable_OC1X,
////    .preloaded_value = 0x0000,  // TCNT1 initial value
////    .ctc_max = 100              // OCR1A limit
////};
//
////Timer1_CFG t1_ctc_counter_ocr = {
////    .timer1_interrupt = tmr1_app_isr,
////    .mode = Timer1_Counter_CTC_OCR_Mode,
////    .operation = Timer1_Counter,
////    .prescaler = Timer1_Counter_Rising_Edge,
////    .channel = Timer1_Counter_Mode_Channel_OC1A,
////    .com_mode = Timer1_COM_Mode_Toggle_OC1X,
////    .ctc_max = 25,
////    .preloaded_value = 0x0000
////};
//
////Timer1_CFG t1_counter_ctc_icr = {
////    .timer1_interrupt = tmr1_app_isr,
////    .mode = Timer1_Counter_CTC_ICR_Mode,
////    .operation = Timer1_Counter,
////    .prescaler = Timer1_Counter_Rising_Edge,
////    .channel = Timer1_Counter_Mode_Channel_OC1A,
////    .com_mode = Timer1_COM_Mode_Disable_OC1X,
////    .preloaded_value = 0x0000,  // TCNT1 initial value
////    .ctc_max = 22              // ICR1 limit
////};
//
//uint16 flag = 0;
//
//int main(void) {
//    Std_ReturnType ret = E_NOT_OK;
//    
//    Application_Init();
//    
//    while (1) {
////        Timer1_Read_Data(&t1_counter_ctc_icr, &flag);
//    }
//    return EXIT_SUCCESS;
//}
//
//void Application_Init(void) {
//    Ecual_Init();
//    GPIO_Led_INIT(&led1);
////    Timer1_INIT(&t1_counter_ctc_icr);
//}
//
//void tmr1_app_isr(void){
//    GPIO_Led_Turn_Toggle(&led1);
//}