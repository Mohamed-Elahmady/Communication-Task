
#include <avr/io.h>
#include "Platform_types.h"
#include "DIO.h"
#include "SPI.h"
#include "util/delay.h"

int main(void)
{
    uint8 counter = 0;
    uint8 ones;
    uint8 tens;
    uint8 displayData;
    
    DIO_initPort(PORT_A, PORT_OUTPUT);

    /* Initialize SPI */
    SPI_Init();

    /* Button on PC0 */
    DIO_initPin(PIN_0, PORT_C, INPUT);

    /* SS as Output */
    DIO_initPin(SS, SPI_PORT, OUTPUT);

    /* Initially Slave is not selected */
    DIO_WritePin(SS, SPI_PORT, HIGH);

    while (1)
    {
        /* Button pressed */
        if (DIO_ReadPin(PIN_0, PORT_C) == HIGH)
        {
            counter++;

            /* 00 -> 99 */
            if (counter > 99)
            {
                counter = 0;
            }

            /* Separate tens and ones */
            tens = counter / 10;
            ones = counter % 10;

            /*
             * PA0 - PA3 = ones
             * PA4 - PA7 = tens
             */
            displayData = (tens << 4) | ones;

            /* Display on Master's two BCD displays */
            DIO_WritePort(PORT_A, displayData);

            /* Select Slave */
            DIO_WritePin(SS, SPI_PORT, LOW);

            /* Send BCD value to Slave */
            SPI_SlendRecieveData(displayData);

            /* Deselect Slave */
            DIO_WritePin(SS, SPI_PORT, HIGH);

            /* Debouncing */
            _delay_ms(300);
        }
    }
}