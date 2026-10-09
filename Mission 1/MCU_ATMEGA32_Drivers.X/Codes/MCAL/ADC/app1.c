/////*
//// * File:   main.c
//// * Author: mohammedel-ahmady
//// *
//// * Created on June 26, 2025, 7:24 PM
//// */
////
//#include "main.h"
//
//ADC_Config adc1 = {
//    
//    .channel = ADC_Channel_ADC0,
//    .clock = ADC_Conversion_Clock_FOSC_DIV_32,
//    .ref = ADC_Internal_Voltage_Reference,
//    .format = ADC_Result_Format_Right
//};
//
//uint16 res1 = 0,res2= 0,res3= 0,res4= 0;
//uint8 txt1[6],txt2[6],txt3[6],txt4[6];
//
//int main(void) {
//    Std_ReturnType ret = E_NOT_OK;
//    Application_Init();
//    GPIO_Chr_Lcd_4Bit_Send_String_Pos(&lcd1, "ADC Test", 1, 7);
//    _delay_ms(500);
//    GPIO_Chr_Lcd_4Bit_Send_Command(&lcd1, 0x01);
//    GPIO_Chr_Lcd_4Bit_Send_String_Pos(&lcd1, "POT 1 :", 1, 1);
//    GPIO_Chr_Lcd_4Bit_Send_String_Pos(&lcd1, "POT 2 :", 2, 1);
//    GPIO_Chr_Lcd_4Bit_Send_String_Pos(&lcd1, "POT 3 :", 3, 1);
//    GPIO_Chr_Lcd_4Bit_Send_String_Pos(&lcd1, "POT 4 :", 4, 1);
//    while (1) {
//        ADC_Get_Conversion_Blocking(&adc1, ADC_Channel_ADC0, &res1);
//        ADC_Get_Conversion_Blocking(&adc1, ADC_Channel_ADC1, &res2);
//        ADC_Get_Conversion_Blocking(&adc1, ADC_Channel_ADC2, &res3);
//        ADC_Get_Conversion_Blocking(&adc1, ADC_Channel_ADC3, &res4);
//        
//        Convert_uint16_To_String(res1, txt1);
//        Convert_uint16_To_String(res2, txt2);
//        Convert_uint16_To_String(res3, txt3);
//        Convert_uint16_To_String(res4, txt4);
//        
//        GPIO_Chr_Lcd_4Bit_Send_String_Pos(&lcd1, txt1, 1, 9);
//        GPIO_Chr_Lcd_4Bit_Send_String_Pos(&lcd1, txt2, 2, 9);
//        GPIO_Chr_Lcd_4Bit_Send_String_Pos(&lcd1, txt3, 3, 9);
//        GPIO_Chr_Lcd_4Bit_Send_String_Pos(&lcd1, txt4, 4, 9);
//        
//    }
//    return EXIT_SUCCESS;
//}
//
//void Application_Init(void){
//    Ecual_Init();
//    ADC_INIT(&adc1);
//}