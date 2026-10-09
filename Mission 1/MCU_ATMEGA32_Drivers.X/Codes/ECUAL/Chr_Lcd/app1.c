///*
// * File:   main.c
// * Author: mohammedel-ahmady
// *
// * Created on June 26, 2025, 7:24 PM
// */
//
//#include "main.h"
//
//const uint8 Battery[5][8] = {{0x0E, 0x1F, 0x11, 0x11, 0x11, 0x11, 0x1F, 0x00},
//                             {0x0E, 0x1F, 0x11, 0x11, 0x11, 0x1F, 0x1F, 0x00},
//                             {0x0E, 0x1F, 0x11, 0x11, 0x1F, 0x1F, 0x1F, 0x00},
//                             {0x0E, 0x1F, 0x11, 0x1F, 0x1F, 0x1F, 0x1F, 0x00},
//                             {0x0E, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x00}};
//
//uint8 str1[],str2[];
//
//int main(void) {
//    Std_ReturnType ret = E_NOT_OK;
//    Application_Init();
////    ret = GPIO_Chr_Lcd_4Bit_Send_Character(&lcd1, 'C');
////    ret = GPIO_Chr_Lcd_8Bit_Send_Character(&lcd2, 'C');
////    
////    ret = GPIO_Chr_Lcd_4Bit_Send_Character_Pos(&lcd1, 'M', 2, 8);
////    ret = GPIO_Chr_Lcd_8Bit_Send_Character_Pos(&lcd2, 'M', 2, 8);
////    
////    _delay_ms(1000);
////    ret = GPIO_Chr_Lcd_4Bit_Send_String_Pos(&lcd1, "Mohamed", 1, 1);
////    ret = GPIO_Chr_Lcd_8Bit_Send_String_Pos(&lcd2, "Mohamed", 1, 1);
////    
////    ret = GPIO_Chr_Lcd_4Bit_Send_String_Pos(&lcd1, "Ghareeb ", 2, 8);
////    ret = GPIO_Chr_Lcd_8Bit_Send_String_Pos(&lcd2, "Ghareeb ", 2, 8);
////
////    ret = GPIO_Chr_Lcd_4Bit_Send_String(&lcd1, "Mohamed");
////    ret = GPIO_Chr_Lcd_8Bit_Send_String(&lcd2, "Mohamed");
////
////    _delay_ms(1000);
////    ret = GPIO_Chr_Lcd_4Bit_Send_Command(&lcd1, 0x01);
////    ret = GPIO_Chr_Lcd_8Bit_Send_Command(&lcd2, 0x01);
//    while (1) {
//        
//        for(uint8 i = 0; i < 5 ;i++){
//            GPIO_Chr_Lcd_4Bit_Send_Custom_Character(&lcd1, Battery[i], 1, 20, 0);
//            GPIO_Chr_Lcd_8Bit_Send_Custom_Character(&lcd2, Battery[i], 1, 20, 0);
//            _delay_ms(250);
//        }
//        ret = GPIO_Chr_Lcd_4Bit_Send_Command(&lcd1, 0x01);
//        ret = GPIO_Chr_Lcd_8Bit_Send_Command(&lcd2, 0x01);
//        _delay_ms(1000);
//        Convert_Byte_To_String(33,str1);
//        ret = GPIO_Chr_Lcd_4Bit_Send_String_Pos(&lcd1, str1, 2, 8);
//        Convert_Short_To_String(550,str2);
//        ret = GPIO_Chr_Lcd_8Bit_Send_String_Pos(&lcd2, str2, 2, 8);
//        _delay_ms(1000);
//    }
//
//    return EXIT_SUCCESS;
//}
//
//void Application_Init(void){
//    Ecual_Init();
//}





///* 
// * File:   ECUAL_INIT.c
// * Author: mohammedel-ahmady
// *
// * Created on July 14, 2025, 12:30 PM
// */
//
///******************* Section 0 : Includes *******************/
//
//#include "ECUAL_INIT.h"
//
///******************* Section 1 :  Variables Definitions *******************/
//
//
///******************* Section 2 :  Helper Functions Declarations *******************/
//
//Chr_Lcd_4Bit lcd1 = {
//    .Rs_Pin.Port = GPIO_PortB,
//    .Rs_Pin.Pin = GPIO_Pin0,
//    .Rs_Pin.Direction = GPIO_Output,
//    .Rs_Pin.Logic = GPIO_Low,
//
//    .Rw_Pin.Port = GPIO_PortB,
//    .Rw_Pin.Pin = GPIO_Pin1,
//    .Rw_Pin.Direction = GPIO_Output,
//    .Rw_Pin.Logic = GPIO_Low,
//
//    .En_Pin.Port = GPIO_PortB,
//    .En_Pin.Pin = GPIO_Pin2,
//    .En_Pin.Direction = GPIO_Output,
//    .En_Pin.Logic = GPIO_Low,
//
//    .Data_Pins[0].Port = GPIO_PortC,
//    .Data_Pins[0].Pin = GPIO_Pin0,
//    .Data_Pins[0].Direction = GPIO_Output,
//    .Data_Pins[0].Logic = GPIO_Low,
//
//    .Data_Pins[1].Port = GPIO_PortC,
//    .Data_Pins[1].Pin = GPIO_Pin1,
//    .Data_Pins[1].Direction = GPIO_Output,
//    .Data_Pins[1].Logic = GPIO_Low,
//
//    .Data_Pins[2].Port = GPIO_PortC,
//    .Data_Pins[2].Pin = GPIO_Pin2,
//    .Data_Pins[2].Direction = GPIO_Output,
//    .Data_Pins[2].Logic = GPIO_Low,
//
//    .Data_Pins[3].Port = GPIO_PortC,
//    .Data_Pins[3].Pin = GPIO_Pin3,
//    .Data_Pins[3].Direction = GPIO_Output,
//    .Data_Pins[3].Logic = GPIO_Low
//};
//
//Chr_Lcd_8Bit lcd2 = {
//    .Rs_Pin.Port = GPIO_PortB,
//    .Rs_Pin.Pin = GPIO_Pin3,
//    .Rs_Pin.Direction = GPIO_Output,
//    .Rs_Pin.Logic = GPIO_Low,
//
//    .Rw_Pin.Port = GPIO_PortB,
//    .Rw_Pin.Pin = GPIO_Pin4,
//    .Rw_Pin.Direction = GPIO_Output,
//    .Rw_Pin.Logic = GPIO_Low,
//
//    .En_Pin.Port = GPIO_PortB,
//    .En_Pin.Pin = GPIO_Pin5,
//    .En_Pin.Direction = GPIO_Output,
//    .En_Pin.Logic = GPIO_Low,
//
//    .Data_Pins[0].Port = GPIO_PortD,
//    .Data_Pins[0].Pin = GPIO_Pin0,
//    .Data_Pins[0].Direction = GPIO_Output,
//    .Data_Pins[0].Logic = GPIO_Low,
//
//    .Data_Pins[1].Port = GPIO_PortD,
//    .Data_Pins[1].Pin = GPIO_Pin1,
//    .Data_Pins[1].Direction = GPIO_Output,
//    .Data_Pins[1].Logic = GPIO_Low,
//
//    .Data_Pins[2].Port = GPIO_PortD,
//    .Data_Pins[2].Pin = GPIO_Pin2,
//    .Data_Pins[2].Direction = GPIO_Output,
//    .Data_Pins[2].Logic = GPIO_Low,
//
//    .Data_Pins[3].Port = GPIO_PortD,
//    .Data_Pins[3].Pin = GPIO_Pin3,
//    .Data_Pins[3].Direction = GPIO_Output,
//    .Data_Pins[3].Logic = GPIO_Low,
//    
//    .Data_Pins[4].Port = GPIO_PortD,
//    .Data_Pins[4].Pin = GPIO_Pin4,
//    .Data_Pins[4].Direction = GPIO_Output,
//    .Data_Pins[4].Logic = GPIO_Low,
//
//    .Data_Pins[5].Port = GPIO_PortD,
//    .Data_Pins[5].Pin = GPIO_Pin5,
//    .Data_Pins[5].Direction = GPIO_Output,
//    .Data_Pins[5].Logic = GPIO_Low,
//
//    .Data_Pins[6].Port = GPIO_PortD,
//    .Data_Pins[6].Pin = GPIO_Pin6,
//    .Data_Pins[6].Direction = GPIO_Output,
//    .Data_Pins[6].Logic = GPIO_Low,
//
//    .Data_Pins[7].Port = GPIO_PortD,
//    .Data_Pins[7].Pin = GPIO_Pin7,
//    .Data_Pins[7].Direction = GPIO_Output,
//    .Data_Pins[7].Logic = GPIO_Low
//};
//
///******************* Section 3 : Software Interfaces Definitions (APIs) *******************/
//
//void Ecual_Init(void){
//    Std_ReturnType ret = E_NOT_OK;
//    ret = GPIO_Chr_Lcd_4Bit_INIT(&lcd1);
//    ret = GPIO_Chr_Lcd_8Bit_INIT(&lcd2);
//}
//
///******************* Section 4:  Helper Functions Definitions *******************/
