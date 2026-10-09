/* 
 * File:   hal_interrupt_cfg.h
 * Author: mohammedel-ahmady
 *
 * Created on August 3, 2025, 6:18 PM
 */

/******************* Section 0 : File Guard *******************/

#ifndef HAL_INTERRUPT_CFG_H
#define	HAL_INTERRUPT_CFG_H

/******************* Section 1 : Includes *******************/

#include <avr/interrupt.h>
#include "hal_interrupt_gen_cfg.h"
#include "../GPIO/hal_gpio.h"
#include "../mcal_std_types.h"
#include "../device_cfg.h"

/******************* Section 2 : Macros Declarations *******************/

#define Sreg_Interrupt_Enable_Bit_Pos            (uint8)0x07

// Flag Bit
#define Interrupt_Not_Occur                      (uint8)0x00
#define Interrupt_Occur                          (uint8)0x01

// Enable Bit

#define Interrupt_Disable                        (uint8)0x00
#define Interrupt_Enable                         (uint8)0x01

/******************* Section 3 : Macros Functions Declarations *******************/

// Global Interrupt_Enable_Bit
#define GLobal_Interrupt_Disable_SREG()          Clear_Bit(SREG, Sreg_Interrupt_Enable_Bit_Pos)
#define GLobal_Interrupt_Enable_SREG()           Set_Bit(SREG, Sreg_Interrupt_Enable_Bit_Pos)

/******************* Section 4 : User Defined Data Types & Variables Declarations *******************/



/******************* Section 5 : Software Interfaces Declarations (APIs) *******************/


#endif	/* HAL_INTERRUPT_CFG_H */

