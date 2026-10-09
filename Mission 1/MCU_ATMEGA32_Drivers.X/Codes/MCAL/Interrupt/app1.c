///*
// * File:   main.c
// * Author: mohammedel-ahmady
// *
// * Created on June 26, 2025, 7:24 PM
// */
//
//#include "main.h"
//
//void int0_isr_app(void);
//void int1_isr_app(void);
//void int2_isr_app(void);
//
//GPIO_Led led1 = {.pin.Port = GPIO_PortC, .pin.Pin = GPIO_Pin0, .pin.Direction = GPIO_Output, .connection = Led_Source, .state = Led_Off};
//
//GPIO_Led led2 = {.pin.Port = GPIO_PortC, .pin.Pin = GPIO_Pin1, .pin.Direction = GPIO_Output, .connection = Led_Source, .state = Led_Off};
//
//GPIO_Led led3 = {.pin.Port = GPIO_PortC, .pin.Pin = GPIO_Pin2, .pin.Direction = GPIO_Output, .connection = Led_Source, .state = Led_Off};
//
//External_Interrupt_Intx int00 = {.pin.Port = GPIO_PortD, .pin.Pin = GPIO_Pin2, .pin.Direction = GPIO_Input,
//                                 .Intx_ISR = int0_isr_app, .source = Interrupt_Int0, .edge = Interrupt_Low_Level};
//
//External_Interrupt_Intx int01 = {.pin.Port = GPIO_PortD, .pin.Pin = GPIO_Pin3, .pin.Direction = GPIO_Input,
//                                 .Intx_ISR = int1_isr_app, .source = Interrupt_Int1, .edge = Interrupt_Low_Level};
//
//External_Interrupt_Intx int02 = {.pin.Port = GPIO_PortB, .pin.Pin = GPIO_Pin2, .pin.Direction = GPIO_Input,
//                                 .Intx_ISR = int2_isr_app, .source = Interrupt_Int2, .edge = Interrupt_Falling_Edge};
//
//int main(void) {
//    Std_ReturnType ret = E_NOT_OK;
//    Application_Init();
//    ret = GPIO_Led_INIT(&led1);
//    ret = GPIO_Led_INIT(&led2);
//    ret = GPIO_Led_INIT(&led3);
//    ret = External_Interrupt_INTx_INIT(&int00);
//    ret = External_Interrupt_INTx_INIT(&int01);
//    ret = External_Interrupt_INTx_INIT(&int02);
//    while (1) {
//      
//    }
//    return EXIT_SUCCESS;
//}
//
//void Application_Init(void){
//    Ecual_Init();
//}
//
//void int0_isr_app(void){
//    GPIO_Led_Turn_Toggle(&led1);
//}
//
//void int1_isr_app(void){
//    GPIO_Led_Turn_Toggle(&led2);
//}
//
//void int2_isr_app(void){
//    GPIO_Led_Turn_Toggle(&led3);
//}