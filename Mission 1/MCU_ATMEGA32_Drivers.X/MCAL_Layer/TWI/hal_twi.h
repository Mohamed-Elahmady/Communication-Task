/* 
 * File:   hal_twi.h
 * Author: mohammedel-ahmady
 *
 * Created on October 9, 2025, 1:56 PM
 */

/******************* Section 0 : File Guard *******************/

#ifndef HAL_TWI_H
#define	HAL_TWI_H

/******************* Section 1 : Includes *******************/

#include "hal_twi_cfg.h"
#include "../mcal_std_types.h"
#include "../device_cfg.h"
#include "../GPIO/hal_gpio.h"
#include "../Interrupt/hal_int_interrupt.h"

/******************* Section 2 : Macros Declarations *******************/

#define MTR_S_Condition                                   0x08
#define MTR_Sr_Condition                                  0x10

#define MT_SLA_W_ACK                                      0x18
#define MT_SLA_W_NACK                                     0x20
#define MT_DT_ACK                                         0x28
#define MT_DT_NACK                                        0x30

#define MR_SLA_W_ACK                                      0x40
#define MR_SLA_W_NACK                                     0x48
#define MR_DT_ACK                                         0x50
#define MR_DT_NACK                                        0x58

#define ST_DT_ACK                                         0xB8
#define ST_DT_NACK                                        0xC0

#define SR_DT_ACK                                         0x80
#define SR_DT_NACK                                        0x88

/******************* Section 3 : Macros Functions Declarations *******************/

#define TWI_Read_Collision_State()                          Read_Bit(TWCR, TWWC)
#define TWI_Read_Transmision_Receiving_State()              Read_Bit(TWCR, TWINT)

#define TWI_Module_Disable()                                Clear_Bit(TWCR, TWEN)
#define TWI_Module_Enable()                                 Set_Bit(TWCR, TWEN)

#define TWI_Send_NACK()                                     Clear_Bit(TWCR, TWEA)
#define TWI_Send_ACK()                                      Set_Bit(TWCR, TWEA)

#define TWI_Master_Send_Start()                             Set_Bit(TWCR, TWSTA)
#define TWI_Master_Send_Stop()                              Set_Bit(TWCR, TWSTO)

#define TWI_Slave_Receive_General_Call_Disable()            Clear_Bit(TWAR, TWGCE)
#define TWI_Slave_Receive_General_Call_Enable()             Set_Bit(TWAR, TWGCE)

#define TWI_Master_Set_Prescaler(__Prescaler)               (TWSR = (__Prescaler & 0x03))
 

/******************* Section 4 : User Defined Data Types & Variables Declarations *******************/

typedef void(* TWI_Handler)(void);

typedef enum{
    TWI_Slave = (uint8)0x00,
    TWI_Master
}TWI_Mode;

typedef enum{
    TWI_Prescaler_FOSC_DIV_1 = (uint8)0x00,
    TWI_Prescaler_FOSC_DIV_4,
    TWI_Prescaler_FOSC_DIV_16,
    TWI_Prescaler_FOSC_DIV_64
}TWI_Prescaler;

typedef enum{
    TWI_General_Call_Disable = (uint8)0x00,
    TWI_General_Call_Enable
}TWI_General_Call;

typedef struct{
#if((Interrupt_Feature_Enable) == (TWI_Interrupt_Feature))
    TWI_Handler       TWI_Interrupt;
#endif
    TWI_Mode                   mode;
    uint32                  mst_clk;
    TWI_Prescaler     mst_prescaler;
    uint8               slv_address;
    TWI_General_Call         slv_gc;
}TWI_CFG;

/******************* Section 5 : Software Interfaces Declarations (APIs) *******************/

Std_ReturnType TWI_INIT(const TWI_CFG *twi);
Std_ReturnType TWI_DEINIT(const TWI_CFG *twi);

Std_ReturnType TWI_Master_Send_Start_Condition(const TWI_CFG *twi);
Std_ReturnType TWI_Master_Send_Stop_Condition(const TWI_CFG *twi);
Std_ReturnType TWI_Master_Send_Repeated_Start_Condition(const TWI_CFG *twi);


Std_ReturnType TWI_Write_Byte_Blocking(const TWI_CFG *twi, uint8 data);
Std_ReturnType TWI_Write_String_Blocking(const TWI_CFG *twi, uint8 *str);
Std_ReturnType TWI_Read_Byte_Blocking(const TWI_CFG *twi, uint8 *data);
Std_ReturnType TWI_Send_Frame(const TWI_CFG *twi, uint8 add, uint8 data);


#endif	/* HAL_TWI_H */

