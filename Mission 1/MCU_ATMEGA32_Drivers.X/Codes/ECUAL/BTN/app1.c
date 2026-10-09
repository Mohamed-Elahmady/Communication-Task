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
//    .pin.Pin = GPIO_Pin0,
//    .pin.Direction = GPIO_Output,
//    .connection = Led_Source,
//    .state = Led_Off
//};
//
//GPIO_Led led2 = {
//    .pin.Port = GPIO_PortC,
//    .pin.Pin = GPIO_Pin1,
//    .pin.Direction = GPIO_Output,
//    .connection = Led_Sink,
//    .state = Led_Off
//};
//
//GPIO_Btn btn1 = {
//    .pin.Port = GPIO_PortC,
//    .pin.Pin  = GPIO_Pin2,
//    .pin.Direction = GPIO_Input,
//    .pin.Logic = GPIO_Low,
//    .connection = Btn_Active_High
//};
//
//GPIO_Btn btn2 = {
//    .pin.Port = GPIO_PortD,
//    .pin.Pin  = GPIO_Pin0,
//    .pin.Direction = GPIO_Input,
//    .pin.Logic = GPIO_High,
//    .connection = Btn_Active_Low
//};
//
//Btn_State st1,st2;
//
//int main(void) {
//    Std_ReturnType ret = E_NOT_OK;
//
//    ret = GPIO_Led_INIT(&led1);
//    ret = GPIO_Led_INIT(&led2);
//    ret = GPIO_Btn_INIT(&btn1);
//    ret = GPIO_Btn_INIT(&btn2);
//    
//    while (1) {
//        ret = GPIO_Btn_Read_State(&(btn2), &st2);
//        if(Btn_Pressed == st2){
//            ret = GPIO_Led_Turn_On(&led2);
//        }
//        else{
//            ret = GPIO_Led_Turn_Off(&led2);
//        }
//    }
//
//    return EXIT_SUCCESS;
//}