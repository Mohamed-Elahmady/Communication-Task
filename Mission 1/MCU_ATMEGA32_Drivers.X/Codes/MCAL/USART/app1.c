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
//USART_CFG usart1 = {
//    .tx_transmitter = {
//        .tx_interrupt = NULL,
//        .tx_tsr_interrupt = NULL,
//        .tx_state = USART_Tx_Enable,
//        .tx_int_state = USART_Tx_Interrupt_Disable,
//        .tsr_int_state = USART_TSR_Interrupt_Disable,
//    },
//    .rx_receiver = {
//        .rx_interrupt = NULL,
//        .framing_interrupt = NULL,
//        .overrun_interrupt = NULL,
//        .rx_state = USART_Rx_Enable,
//        .rx_int_state = USART_Rx_Interrupt_Disable,
//    },
//    
//    .mode = USART_Asynchronous,
//.data_classification = USART_Transmit_Receive_Data_Address,
//    .data_size = USART_Data_8Bit,
//    .parity = USART_Disable_Parity,
//    .stop_bits = USART_1Stop_Bit,
//    
//    .br_speed = USART_Baud_Rate_X1Doubling,
//    .baud_rate = 9600,
//    
//    .error_states = 0x00,
//};
//
//uint16 data = 0x00;
//
//int main(void) {
//    Application_Init();
//    USART_Tx_Write_String_Blocking(&usart1, "Loading...\r");
//    _delay_ms(200);
//    USART_Tx_Write_String_Blocking(&usart1, "Welcome to USART Test\r");
//    while (1) {
//        USART_Rx_Read_Byte_Blocking(&usart1, &data);
//        USART_Tx_Write_Byte_Blocking(&usart1, data);
//        data = 0;
//
//    }
//    
//    return EXIT_SUCCESS;
//}
//
//void Application_Init(void) {
//    Ecual_Init();
//    USART_INIT(&usart1);
//}