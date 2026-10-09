/* 
 * File:   hal_usart.h
 * Author: mohammedel-ahmady
 *
 * Created on September 22, 2025, 8:17 PM
 */

/******************* Section 0 : File Guard *******************/

#ifndef HAL_USART_H
#define	HAL_USART_H

/******************* Section 1 : Includes *******************/

#include "hal_usart_cfg.h"
#include "../GPIO/hal_gpio.h"
#include "../Interrupt/hal_int_interrupt.h"
#include "../mcal_std_types.h"
#include "../device_cfg.h"


/******************* Section 2 : Macros Declarations *******************/

#define TXB8_Shift_To_Position                      0x08
#define TXB8_Mask                                   0x01

/******************* Section 3 : Macros Functions Declarations *******************/

// USART Transmission Function Macros

#define USART_Read_Transmission_Shift_Register_Status()                   Read_Bit(UCSRA, UDRE)

#define USART_Transmission_Disable()                                      Clear_Bit(UCSRB, TXEN)
#define USART_Transmission_Enable()                                       Set_Bit(UCSRB, TXEN)

#define USART_Transmit_Ninth_Bit_Data(__Data)                             (UCSRB = (UCSRB & ~(1<<TXB8)) | (((__Data >> TXB8_Shift_To_Position) & TXB8_Mask) << TXB8))


// USART Receiving Function Macros

#define USART_Read_Receiving_Shift_Register_Status()                      Read_Bit(UCSRA, RXC)
#define USART_Read_Ninth_Bit_Data()                                       Read_Bit(UCSRB, RXB8)

#define USART_Receiving_Disable()                                         Clear_Bit(UCSRB, RXEN)
#define USART_Receiving_Enable()                                          Set_Bit(UCSRB, RXEN)

// USART Error Handling Function Macros

#define USART_Async_Read_Parity_Error_Status()                            Read_Bit(UCSRA, PE)
#define USART_Async_Framing_Error_Status()                                Read_Bit(UCSRA, FE)
#define USART_Read_Data_OverrRun_Error_Status()                           Read_Bit(UCSRA, DOR)

// USART Function Macros

#define USART_Asynchronous_Mode()                                        do{    \
                                                                              Set_Bit(UCSRC, URSEL);       \
                                                                              Clear_Bit(UCSRC, UMSEL);     \
                                                                            }while(0)
#define USART_Synchronous_Mode()                                         do{    \
                                                                              Set_Bit(UCSRC, URSEL);       \
                                                                              Set_Bit(UCSRC, UMSEL);       \
                                                                            }while(0)

#define USART_Async_Baud_Rate_X1_Doubling()                               Clear_Bit(UCSRA, U2X)
#define USART_Async_Baud_Rate_X2_Doubling()                               Set_Bit(UCSRA, U2X)

#define USART_Async_Send_Data_Address_Frame()                             Clear_Bit(UCSRA, MPCM)
#define USART_Async_Send_Address_Frame_Only()                             Set_Bit(UCSRA, MPCM)

#define USART_Select_UBRRH_Register()                                     Clear_Bit(UCSRC, URSEL)
#define USART_Select_UCSRC_Register()                                     Set_Bit(UCSRC, URSEL)

#define USART_Async_Disable_Parity_Mode()                                 do{    \
                                                                              Set_Bit(UCSRC, URSEL);     \
                                                                              Clear_Bit(UCSRC, UPM0);    \
                                                                              Clear_Bit(UCSRC, UPM1);    \
                                                                            }while(0)
#define USART_Async_Even_Parity_Mode()                                   do{    \
                                                                              Set_Bit(UCSRC, URSEL);     \
                                                                              Clear_Bit(UCSRC, UPM0);    \
                                                                              Set_Bit(UCSRC, UPM1);      \
                                                                            }while(0)
#define USART_Async_Odd_Parity_Mode()                                    do{    \
                                                                              Set_Bit(UCSRC, URSEL);     \
                                                                              Set_Bit(UCSRC, UPM0);      \
                                                                              Set_Bit(UCSRC, UPM1);      \
                                                                            }while(0)

#define USART_Async_Set_1Stop_Bit()                                      do{    \
                                                                              Set_Bit(UCSRC, URSEL);      \
                                                                              Clear_Bit(UCSRC, USBS);     \
                                                                            }while(0)
#define USART_Async_Set_2Stop_Bit()                                      do{    \
                                                                              Set_Bit(UCSRC, URSEL);      \
                                                                              Set_Bit(UCSRC, USBS);       \
                                                                            }while(0)

#define USART_Sync_Sample_On_Rising()                                    do{    \
                                                                              Set_Bit(UCSRC, URSEL);       \
                                                                              Clear_Bit(UCSRC, UCPOL);     \
                                                                            }while(0)
#define USART_Sync_Sample_On_Falling()                                   do{    \
                                                                              Set_Bit(UCSRC, URSEL);      \
                                                                              Set_Bit(UCSRC, UCPOL);      \
                                                                            }while(0)

/******************* Section 4 : User Defined Data Types & Variables Declarations *******************/

typedef void (* USART_Handler)(void);

// USART Transmission enum & data types

typedef enum{
    USART_Tx_Interrupt_Disable = (uint8)0x00,
    USART_Tx_Interrupt_Enable
}USART_Tx_Interrupt_State;

typedef enum{
    USART_TSR_Interrupt_Disable = (uint8)0x00,
    USART_TSR_Interrupt_Enable
}USART_TSR_Interrupt_State;

typedef enum{
    USART_Tx_Disable = (uint8)0x00,
    USART_Tx_Enable
}USART_Tx_State;

typedef struct{
#if (((Interrupt_Feature_Enable)) == (USART_Interrupt_Feature))
    USART_Handler                        tx_interrupt;
    USART_Handler                    tx_tsr_interrupt;
    USART_Tx_Interrupt_State             tx_int_state;
    USART_TSR_Interrupt_State           tsr_int_state;
#endif
    USART_Tx_State                           tx_state;
}USART_Tx_CFG;

// USART Receiving enum & data types

typedef enum{
    USART_Rx_Interrupt_Disable = (uint8)0x00,
    USART_Rx_Interrupt_Enable
}USART_Rx_Interrupt_State;

typedef enum{
    USART_Rx_Disable = (uint8)0x00,
    USART_Rx_Enable
}USART_Rx_State;

typedef struct{
    USART_Handler                    rx_interrupt;
    USART_Handler               framing_interrupt;
    USART_Handler               overrun_interrupt;
    USART_Rx_Interrupt_State         rx_int_state;
    
    USART_Rx_State                      rx_state;
}USART_Rx_CFG;

// USART Error Handling enum & data types

typedef enum{
    USART_Async_Framing_Cleared = (uint8)0x00,
    USART_Async_Framing_Detected
}USART_Async_Framing_Status;

typedef enum{
    USART_Overrun_Cleared = (uint8)0x00,
    USART_Overrun_Detected
}USART_Overrun_Status;

typedef enum{
    USART_Async_Parity_Cleared = (uint8)0x00,
    USART_Async_Parity_Detected
}USART_Async_Parity_Status;

typedef union{
    struct{
        uint8 Framing_Error  : 1;
        uint8 Overrun_Error  : 1;
        uint8 Parity_Error   : 1;
        uint8 Reserved_Error : 5;
    };
    uint8 error_states;
}USART_Error_Status;

// USART enum & data types

typedef enum{
    USART_Asynchronous = (uint8)0x00,
    USART_Synchronous
}USART_Mode;

typedef enum{
    USART_Transmit_Receive_Data_Address = (uint8)0x00,
    USART_Transmit_Receive_Address
}USART_Async_Data_Classification;

typedef enum{
    USART_Data_5Bit = (uint8)0x00,
    USART_Data_6Bit,
    USART_Data_7Bit,
    USART_Data_8Bit,
    USART_Data_9Bit = (uint8)0x07
}USART_Data_Size;

typedef enum{
    USART_Disable_Parity = (uint8)0x00,
    USART_Even_Parity,
    USART_Odd_Parity
}USART_Async_Parity_Mode;

typedef enum{
    USART_1Stop_Bit = (uint8)0x00,
    USART_2Stop_Bit
}USART_Async_Stop_Size;

typedef enum{
    USART_Baud_Rate_X1Doubling = (uint8)0x00,
    USART_Baud_Rate_X2Doubling
}USART_Baud_Rate_Speed;

typedef enum{
    USART_Sync_Slave_Communication = (uint8)0x00,
    USART_Sync_Master_Communication
}USART_Sync_communication_type;

typedef enum{
    USART_Sync_Rising_Tx_Falling_Rx = (uint8)0x00,
    USART_Sync_Rising_Rx_Falling_Tx
}USART_Sync_Clock_Polarity;

typedef struct{
    USART_Tx_CFG                              tx_transmitter;
    USART_Rx_CFG                                 rx_receiver;
    
    USART_Mode                                          mode;
    
    uint32                                         baud_rate;
    USART_Baud_Rate_Speed                           br_speed;
    
    USART_Data_Size                                data_size;
    USART_Async_Data_Classification      data_classification;
    USART_Async_Parity_Mode                           parity;
    USART_Async_Stop_Size                          stop_bits;
    
    USART_Sync_communication_type                   com_mode;
    USART_Sync_Clock_Polarity                       clk_edge;
    
    USART_Error_Status                          error_states;
}USART_CFG;

/******************* Section 5 : Software Interfaces Declarations (APIs) *******************/

Std_ReturnType USART_INIT(const USART_CFG *usart);
Std_ReturnType USART_DEINIT(const USART_CFG *usart);

Std_ReturnType USART_Tx_Write_Byte_Non_Blocking(const USART_CFG *usart, uint16 data);
Std_ReturnType USART_Tx_Write_String_Non_Blocking(const USART_CFG *usart, uint8 *str);
Std_ReturnType USART_Tx_Write_Byte_Blocking(const USART_CFG *usart, uint16 data);
Std_ReturnType USART_Tx_Write_String_Blocking(const USART_CFG *usart, uint8 *str);

Std_ReturnType USART_Rx_Restart(const USART_CFG *usart);
Std_ReturnType USART_Rx_Read_Byte_Non_Blocking(const USART_CFG *usart, uint16 *data);
Std_ReturnType USART_Rx_Read_Byte_Blocking(const USART_CFG *usart, uint16 *data);

#endif	/* HAL_USART_H */

