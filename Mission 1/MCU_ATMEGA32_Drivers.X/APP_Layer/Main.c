/*
 * File:   main.c
 * Author: mohammedel-ahmady
 *
 * Created on September 13, 2025, 4:55 PM
 */

#include "Main.h"

void Application_Init(void);

#define SLAVE_ADD   0x30

TWI_CFG twi1 = {
    .TWI_Interrupt = NULL,
    .mode = TWI_Master,
    .mst_clk = 100000,
    .mst_prescaler = TWI_Prescaler_FOSC_DIV_1
};

int main(void) {
    Application_Init();
    while (1) {
        TWI_Send_Frame(&twi1, SLAVE_ADD, 'M');
        _delay_ms(300);
    }
    return EXIT_SUCCESS;
}

void Application_Init(void) {
    Ecual_Init();
    TWI_INIT(&twi1);
}
