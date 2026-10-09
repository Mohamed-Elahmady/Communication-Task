
#include <avr/io.h>
#include "Platform_types.h"
#include "DIO.h"
#include "SPI.h"
#include "util/delay.h"

int main(void)
{
    uint8 receivedData;
    DDRA=0xFF;

    /* PORT A connected to two BCD 7-Segment displays */
    DIO_initPort(PORT_A, PORT_OUTPUT);

    /* Initialize SPI as Slave */
    SPI_Init();

    while (1)
    {
        /* Receive data from Master */
        receivedData = SPI_SlendRecieveData(0x00);

        /* Display received data */
        DIO_WritePort(PORT_A, receivedData);
    }
}