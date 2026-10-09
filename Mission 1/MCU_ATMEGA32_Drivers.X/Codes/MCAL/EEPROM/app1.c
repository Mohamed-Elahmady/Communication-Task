///*
// * File:   main.c
// * Author: mohammedel-ahmady
// *
// * Created on June 26, 2025, 7:24 PM
// */
//
//#include "main.h"
//
//uint8 counter = 0;
//uint8 rest = 0;
//
//GPIO_Led led1 = {.pin.Port = GPIO_PortC, .pin.Pin = GPIO_Pin0, .pin.Direction = GPIO_Output, .connection = Led_Source, .state = Led_Off};
//
//int main(void) {
//    Std_ReturnType ret = E_NOT_OK;
//    Application_Init();
//
//    while (1) {
//        ret = EEPROM_Write_One_Byte(0x3ff, counter++);
//        ret = EEPROM_Read_One_Byte(0x3ff, &rest);
//        if((rest&0x05) == 0x05){
//            GPIO_Led_Turn_Toggle(&led1);
//        }
//        _delay_ms(1000);       
//    }
//    return EXIT_SUCCESS;
//}
//
//void Application_Init(void){
//    Ecual_Init();
//    GPIO_Led_INIT(&led1);
//}