///*
// * File:   main.c
// * Author: mohammedel-ahmady
// *
// * Created on June 26, 2025, 7:24 PM
// */
//
//#include "main.h"
//
//DC_Motor motor1 = {
//    .INs[0].Port = GPIO_PortC,
//    .INs[0].Pin = GPIO_Pin0,
//    .INs[0].Direction = GPIO_Output,
//    .INs[0].Logic = GPIO_Low,
//    
//    .INs[1].Port = GPIO_PortC,
//    .INs[1].Pin = GPIO_Pin1,
//    .INs[1].Direction = GPIO_Output,
//    .INs[1].Logic = GPIO_Low,
//    
//};
//
//DC_Motor motor2 = {
//    .INs[0].Port = GPIO_PortC,
//    .INs[0].Pin = GPIO_Pin2,
//    .INs[0].Direction = GPIO_Output,
//    .INs[0].Logic = GPIO_Low,
//    
//    .INs[1].Port = GPIO_PortC,
//    .INs[1].Pin = GPIO_Pin3,
//    .INs[1].Direction = GPIO_Output,
//    .INs[1].Logic = GPIO_Low,
//    
//};
//
//Motor_State st;
//
//int main(void) {
//    Std_ReturnType ret = E_NOT_OK;
//    ret = GPIO_DC_Motor_INIT(&motor1);
//    ret = GPIO_DC_Motor_INIT(&motor2);
//    while (1) {
//        ret = GPIO_DC_Motor_Rotate_ClockWise(&motor1);
//        _delay_ms(3000);
//        ret = GPIO_DC_Motor_Rotate_Counter_ClockWise(&motor1);
//        _delay_ms(3000);
//        ret = GPIO_DC_Motor_Toggle_Rotation_Direction(&motor1);
//        _delay_ms(3000);
//        ret = GPIO_DC_Motor_Brake(&motor1);
//        ret = GPIO_DC_Motor_Read_Rotation_State(&motor1, &st);
//        _delay_ms(3000);
//        
//        if(st == Motor_Brake){
//            ret = GPIO_DC_Motor_Rotate_ClockWise(&motor2);
//            _delay_ms(3000);
//            ret = GPIO_DC_Motor_Rotate_Counter_ClockWise(&motor2);
//        }
//        else{
//            ret = GPIO_DC_Motor_Rotate_Counter_ClockWise(&motor2);
//        }
//    }
//
//    return EXIT_SUCCESS;
//}