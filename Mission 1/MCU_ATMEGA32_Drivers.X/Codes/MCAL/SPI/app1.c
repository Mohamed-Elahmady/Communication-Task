///*
// * File:   main.c
// * Author: mohammedel-ahmady
// *
// * Created on September 13, 2025, 4:55 PM
// */
//
//#include "Main.h"
//
//void Application_Init(void);
//
//GPIO_Led led1 = {
//    .pin.Port = GPIO_PortC,
//    .pin.Pin = GPIO_Pin0,
//    .pin.Direction = GPIO_Output,
//    .connection = Led_Source,
//    .state = Led_Off
//};
//
//SPI_CFG spi1 = {
//    .spi_interrupt = NULL,
//
//    .mode = SPI_Master_Mode,
//    .master_freq = SPI_Master_FOSC_Div_16,
//
//    .clk_polarity = SPI_Idle_Low,
//    .clk_phase = SPI_Receive_Rising_Transmit_Falling,
//
//    .start_bit = SPI_Most_Significant_Bit_First,
//
//    .ss_pin.Port = GPIO_PortD,
//    .ss_pin.Pin = GPIO_Pin0,
//    .ss_pin.Direction = GPIO_Output,
//    .ss_pin.Logic = GPIO_Low,   // Idle high
//};
//
//uint8 data = 0x00;
//
//int main(void) {
//    Application_Init();
//
//    while (1) {
//            SPI_Read_Byte_Blocking(&spi1, &data);
//            _delay_ms(200);
//            SPI_Write_Byte_Blocking(&spi1, data);
//            if (data == 'c') {
//                GPIO_Led_Turn_On(&led1);   
//            }
//    }
//
//    return EXIT_SUCCESS;
//}
//
//void Application_Init(void) {
//    Ecual_Init();
//    SPI_INIT(&spi1);
//    GPIO_Led_INIT(&led1);
//}
//
///*
// * File:   main.c
// * Author: mohammedel-ahmady
// *
// * Created on September 13, 2025, 4:55 PM
// */
//
////#include "Main.h"
////
////void Application_Init(void);
////
////SPI_CFG spi2 = {
////    .spi_interrupt = NULL,
////
////    .mode = SPI_Slave_Mode,
////
////    .clk_polarity = SPI_Idle_Low,
////    .clk_phase = SPI_Receive_Rising_Transmit_Falling,
////
////    .start_bit = SPI_Most_Significant_Bit_First,
////};
////
////
////int main(void) {
////    Application_Init();
////
////    while (1) {
////        SPI_Write_Byte_Blocking(&spi2, 'c');
////    }
////
////    return EXIT_SUCCESS;
////}
////
////void Application_Init(void) {
////    Ecual_Init();
////    SPI_INIT(&spi2);
////}