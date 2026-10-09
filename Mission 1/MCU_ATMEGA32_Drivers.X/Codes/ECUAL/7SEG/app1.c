///*
// * File:   main.c
// * Author: mohammedel-ahmady
// *
// * Created on June 26, 2025, 7:24 PM
// */
//
//#include "main.h"
//
//GPIO_Segment seg1 = {
//    .pins[Segment_Pin0].Port = GPIO_PortC,
//    .pins[Segment_Pin0].Pin = GPIO_Pin0,
//    .pins[Segment_Pin0].Direction = GPIO_Output,
//    .pins[Segment_Pin0].Logic = GPIO_Low,
//    
//    .pins[Segment_Pin1].Port = GPIO_PortC,
//    .pins[Segment_Pin1].Pin = GPIO_Pin1,
//    .pins[Segment_Pin1].Direction = GPIO_Output,
//    .pins[Segment_Pin1].Logic = GPIO_Low,
//    
//    .pins[Segment_Pin2].Port = GPIO_PortC,
//    .pins[Segment_Pin2].Pin = GPIO_Pin2,
//    .pins[Segment_Pin2].Direction = GPIO_Output,
//    .pins[Segment_Pin2].Logic = GPIO_Low,
//    
//    .pins[Segment_Pin3].Port = GPIO_PortC,
//    .pins[Segment_Pin3].Pin = GPIO_Pin3,
//    .pins[Segment_Pin3].Direction = GPIO_Output,
//    .pins[Segment_Pin3].Logic = GPIO_Low,
//    
//    .connection = Segment_Common_Anode
//};
//
//GPIO_Pin_CFG pin1 = {
//    .Port = GPIO_PortD,
//    .Pin = GPIO_Pin0,
//    .Direction = GPIO_Output,
//    .Logic = GPIO_Low
//};
//
//GPIO_Pin_CFG pin2 = {
//    .Port = GPIO_PortD,
//    .Pin = GPIO_Pin1,
//    .Direction = GPIO_Input,
//    .Logic = GPIO_Low
//};
//
//uint8 number = 25;
//uint8 var = 0;
//
//int main(void) {
//    Std_ReturnType ret = E_NOT_OK;
//    ret = GPIO_Segment_INIT(&seg1);
//    ret = GPIO_Pin_INIT(&pin1);
//    ret = GPIO_Pin_INIT(&pin2);
//    while (1) {
////        for(uint8 i = 0; i < 50; i++){
////            ret = GPIO_Pin_Write_Logic(&pin2, GPIO_High);
////            ret = GPIO_Segment_Write_Number(&seg1, number%10);
////            _delay_ms(10);
////            ret = GPIO_Pin_Write_Logic(&pin2, GPIO_Low);
////            ret = GPIO_Pin_Write_Logic(&pin1, GPIO_High);
////            ret = GPIO_Segment_Write_Number(&seg1, number/10);
////            _delay_ms(10);
////            ret = GPIO_Pin_Write_Logic(&pin1, GPIO_Low);
////        }
////        number++;
////        if(number >= 100)
////            number = 0;
//        
//        ret = GPIO_Pin_Write_Logic(&pin2, GPIO_High);
//        ret = GPIO_Segment_Write_Number(&seg1, number%10);
//        ret = GPIO_Segment_Read_Number(&seg1, &var);
//        _delay_ms(1000);
//        if(var = 5){
//            ret = GPIO_Pin_Write_Logic(&pin2, GPIO_Low);
//            ret = GPIO_Pin_Write_Logic(&pin1, GPIO_High);
//            ret = GPIO_Segment_Write_Number(&seg1, number/10);
//        }
//        _delay_ms(2000);
//    }
//
//    return EXIT_SUCCESS;
//}