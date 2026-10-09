///*
// * File:   main.c
// * Author: mohammedel-ahmady
// *
// * Created on June 26, 2025, 7:24 PM
// */
//
//#include "main.h"
//
//GPIO_Relay relay1 = {
//    .pin.Port = GPIO_PortC,
//    .pin.Pin = GPIO_Pin0,
//    .pin.Direction = GPIO_Output,
//    .pin.Logic = GPIO_Low
//};
//
//GPIO_Relay relay2 = {
//    .pin.Port = GPIO_PortC,
//    .pin.Pin = GPIO_Pin1,
//    .pin.Direction = GPIO_Output,
//    .pin.Logic = GPIO_High
//};
//
//Relay_State st1;
//
//int main(void) {
//    Std_ReturnType ret = E_NOT_OK;
//    
//    ret = GPIO_Relay_INIT(&relay1);
//    ret = GPIO_Relay_INIT(&relay2);
//    
//    while (1) {
//        ret = GPIO_Relay_Turn_On(&relay1);
//        ret = GPIO_Relay_Read_State(&relay1, &st1);
//        _delay_ms(2000);
//        ret = GPIO_Relay_Turn_Off(&relay1);
//        ret = GPIO_Relay_Read_State(&relay1, &st1);
//        _delay_ms(2000);
//        ret = GPIO_Relay_Turn_Toggle(&relay1);
//        ret = GPIO_Relay_Read_State(&relay1, &st1);
//        _delay_ms(2000);
//        
//        if(st1 == Relay_On){
//            ret = GPIO_Relay_Turn_Toggle(&relay2);
//        }
//    }
//
//    return EXIT_SUCCESS;
//}