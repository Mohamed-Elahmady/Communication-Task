/*
 * SPI.c
 *
 *  Created on: 22 Feb 2023
 *      Author: Alaa Wahba
 */

#include "spi.h"

void SPI_Init()
{

#ifdef MASTER_MODE

	/* GPIO pins Configuration */

	/* MOSI -> Output */
	DIO_initPin(MOSI, SPI_PORT, OUTPUT);

	/* MISO -> Input */
	DIO_initPin(MISO, SPI_PORT, INPUT);

	/* SCLK -> Output */
	DIO_initPin(SCLK, SPI_PORT, OUTPUT);

	/* SS -> Output */
	DIO_initPin(SS, SPI_PORT, OUTPUT);

	/* SS initially HIGH */
	DIO_WritePin(SS, SPI_PORT, HIGH);

	/*
	 * SPI Configuration
	 *
	 * SPE  -> SPI Enable
	 * MSTR -> Master Mode
	 * SPR0 -> F_CPU / 16
	 */
	SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0);

#endif


#ifdef SLAVE_MODE

	/* GPIO pins Configuration */

	/* MOSI -> Input */
	DIO_initPin(MOSI, SPI_PORT, INPUT);

	/* MISO -> Output */
	DIO_initPin(MISO, SPI_PORT, OUTPUT);

	/* SCLK -> Input */
	DIO_initPin(SCLK, SPI_PORT, INPUT);

	/* SS -> Input */
	DIO_initPin(SS, SPI_PORT, INPUT);

	/*
	 * SPI Configuration
	 *
	 * SPE -> SPI Enable
	 * MSTR = 0 -> Slave Mode
	 */
	SPCR = (1 << SPE);

#endif

}


uint8 SPI_SlendRecieveData(uint8 Data)
{
	/* Put data into SPI Data Register */
	SPDR = Data;

	/* Wait until transmission is complete */
	while (!(SPSR & (1 << SPIF)))
	{
		/* Wait */
	}

	/* Return received data */
	return SPDR;
}