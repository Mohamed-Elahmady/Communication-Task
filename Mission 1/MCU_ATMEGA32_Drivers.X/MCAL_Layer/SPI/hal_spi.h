/* 
 * File:   hal_spi.h
 * Author: mohammedel-ahmady
 *
 * Created on September 27, 2025, 7:25 PM
 */

/******************* Section 0 : File Guard *******************/

#ifndef HAL_SPI_H
#define	HAL_SPI_H

/******************* Section 1 : Includes *******************/

#include "hal_spi_cfg.h"
#include "../GPIO/hal_gpio.h"
#include "../Interrupt/hal_int_interrupt.h"
#include "../mcal_std_types.h"
#include "../device_cfg.h"

/******************* Section 2 : Macros Declarations *******************/



/******************* Section 3 : Macros Functions Declarations *******************/

#define SPI_Read_Transmit_Receive_Status()                      Read_Bit(SPSR, SPIF)
#define SPI_Read_Transmit_Collision_Status()                    Read_Bit(SPSR, WCOL)

#define SPI_Module_Disable()                                    Clear_Bit(SPCR, SPE)
#define SPI_Module_Enable()                                     Set_Bit(SPCR, SPE)

#define SPI_Transmit_Receive_MSB_First()                        Clear_Bit(SPCR, DORD)
#define SPI_Transmit_Receive_LSB_First()                        Set_Bit(SPCR, DORD)

#define SPI_Set_Slave_Mode()                                    Clear_Bit(SPCR, MSTR)
#define SPI_Set_Master_Mode()                                   Set_Bit(SPCR, MSTR)

#define SPI_Clock_Polarity_Idle_Low()                           Clear_Bit(SPCR, CPOL)
#define SPI_Clock_Polarity_Idle_High()                          Set_Bit(SPCR, CPOL)

#define SPI_Clock_Phase_Receive_Rising_Transmit_Falling()       Clear_Bit(SPCR, CPHA)
#define SPI_Clock_Phase_Transmit_Rising_Receive_Falling()       Set_Bit(SPCR, CPHA)

/******************* Section 4 : User Defined Data Types & Variables Declarations *******************/

typedef void (* SPI_Handler)(void);

typedef enum{
    SPI_Slave_Mode = (uint8)0x00,
    SPI_Master_Mode
}SPI_Mode;

typedef enum{
    
    SPI_Most_Significant_Bit_First = (uint8)0x00,
    SPI_Least_Significant_Bit_First
}SPI_TR_First;

typedef enum{
    SPI_Idle_Low = (uint8)0x00,
    SPI_Idle_High
}SPI_Clock_Polarity;

typedef enum{
    SPI_Receive_Rising_Transmit_Falling = (uint8)0x00,
    SPI_Transmit_Rising_Receive_Falling
}SPI_Clock_Phase;

typedef enum{
    SPI_Master_FOSC_Div_4 = (uint8)0x00,
    SPI_Master_FOSC_Div_16,
    SPI_Master_FOSC_Div_64,
    SPI_Master_FOSC_Div_128,
    SPI_Master_FOSC_Div_2,
    SPI_Master_FOSC_Div_8,
    SPI_Master_FOSC_Div_32
}SPI_Master_Clock_Frequency;

typedef struct{
#if((Interrupt_Feature_Enable) == (SPI_Interrupt_Feature))
    SPI_Handler                spi_interrupt;
#endif
    SPI_Mode                            mode;
    SPI_Master_Clock_Frequency   master_freq;
    
    SPI_Clock_Polarity          clk_polarity;
    SPI_Clock_Phase                clk_phase;
    
    SPI_TR_First                   start_bit;
    
    GPIO_Pin_CFG                      ss_pin;
}SPI_CFG;

/******************* Section 5 : Software Interfaces Declarations (APIs) *******************/

Std_ReturnType SPI_INIT(const SPI_CFG *spi);
Std_ReturnType SPI_DEINIT(const SPI_CFG *spi);

Std_ReturnType SPI_Write_Byte_Non_Blocking(const SPI_CFG *spi, uint8 data);
Std_ReturnType SPI_Write_String_Non_Blocking(const SPI_CFG *spi, uint8 *str);
Std_ReturnType SPI_Write_Byte_Blocking(const SPI_CFG *spi, uint8 data);
Std_ReturnType SPI_Write_String_Blocking(const SPI_CFG *spi, uint8 *str);

Std_ReturnType SPI_Read_Byte_Non_Blocking(const SPI_CFG *spi, uint8 *data);
Std_ReturnType SPI_Read_Byte_Blocking(const SPI_CFG *spi, uint8 *data);

#endif	/* HAL_SPI_H */

