///*
// * File:   main.c
// * Author: mohammedel-ahmady
// *
// * Created on June 26, 2025, 7:24 PM
// */
//
//#include "main.h"
//
//GPIO_Led led1 = {
//    .pin.Port = GPIO_PortC,
//    .pin.Pin = GPIO_Pin2,
//    .pin.Direction = GPIO_Output,
//    .connection = Led_Source,
//    .state = Led_Off
//};
//
//GPIO_Led led2 = {
//    .pin.Port = GPIO_PortC,
//    .pin.Pin = GPIO_Pin4,
//    .pin.Direction = GPIO_Output,
//    .connection = Led_Sink,
//    .state = Led_Off
//};
//
//
//int main(void) {
//    Std_ReturnType ret = E_NOT_OK;
//    Led_State led_status;
//
//    ret = GPIO_Led_INIT(&led2);
//    while (1) {
////        GPIO_Led_Turn_On(&led2);
////        GPIO_Led_Read_Logic(&led2, &led_status);
////        
////        _delay_ms(1000);
////        
////        GPIO_Led_Turn_Off(&led2);
////        GPIO_Led_Read_Logic(&led2, &led_status);
////        _delay_ms(1000);
//
//        GPIO_Led_Turn_Toggle(&led2);
//        _delay_ms(200);
//    }
//
//    return EXIT_SUCCESS;
//}