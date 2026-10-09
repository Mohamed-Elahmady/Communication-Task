///* 
// * File:   main.c
// * Author: mohammedel-ahmady
// *
// * Created on June 26, 2025, 7:24 PM
// */
//
//#include "main.h"
//
////GPIO_Pin_CFG led = {
////    .Port = GPIO_PortC,
////    .Pin = GPIO_Pin0,
////    .Direction = GPIO_Output,
////    .Logic = GPIO_Low
////};
////
////GPIO_Pin_CFG btn = {
////    .Port = GPIO_PortD,
////    .Pin = GPIO_Pin0,
////    .Direction = GPIO_Input,
////    .Logic = GPIO_Low
////};
////
////Std_ReturnType ret = E_OK;
////GPIO_Logic logic = GPIO_Low;
////GPIO_Direction dir;
////uint8 temp;
//
//int main(void) {
////    ret = GPIO_Pin_INIT(&led);
////    ret = GPIO_Pin_Direction_INIT(&btn);
////    ret = GPIO_Pin_Get_Direction_Status(&led, &dir);
//    while(1){
////        ret = GPIO_Pin_Read_Logic(&btn, &logic);
////        if(GPIO_High == logic){
////            ret = GPIO_Pin_Toggle_Logic(&led, GPIO_High);
////            _delay_ms(100);
////        }
//        
////        ret = GPIO_Port_INIT(GPIO_PortC, 0xFF, 0x55);
////        _delay_ms(1000);
//////        ret = GPIO_Port_Toggle_Logic(GPIO_PortC);
//////        _delay_ms(1000);
////        ret = GPIO_Port_Get_Direction_Status(GPIO_PortC, &temp);
////        if(temp == 0xff){
////            ret = GPIO_Port_Write_Logic(GPIO_PortC, 0x00);
////            _delay_ms(2000);
////        }
////        ret = GPIO_Port_Read_Logic(GPIO_PortC, &temp);
////        if(temp == 0x00){
////            ret = GPIO_Port_Write_Logic(GPIO_PortC, 0xAA);
////            _delay_ms(2000);
////        }
//    }
//
//    return EXIT_SUCCESS;
//}