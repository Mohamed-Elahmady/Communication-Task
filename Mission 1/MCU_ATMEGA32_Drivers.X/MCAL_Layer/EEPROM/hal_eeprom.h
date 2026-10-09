/* 
 * File:   hal_eeprom.h
 * Author: mohammedel-ahmady
 *
 * Created on August 5, 2025, 6:50 PM
 */

/******************* Section 0 : File Guard *******************/

#ifndef HAL_EEPROM_H
#define	HAL_EEPROM_H

/******************* Section 1 : Includes *******************/

#include "hal_eeprom_cfg.h"
#include "../Interrupt/hal_int_interrupt.h"
#include "../mcal_std_types.h"
#include "../device_cfg.h"

/******************* Section 2 : Macros Declarations *******************/



/******************* Section 3 : Macros Functions Declarations *******************/

#define EEPROM_Master_Write_Enable()                            Set_Bit(EECR, EEMWE)
#define EEPROM_Write_Enable()                                   Set_Bit(EECR, EEWE)
#define EEPROM_Write_Enable_Read_Status()                       Read_Bit(EECR, EEWE)
#define EEPROM_Read_Enable()                                    Set_Bit(EECR, EERE)

/******************* Section 4 : User Defined Data Types & Variables Declarations *******************/



/******************* Section 5 : Software Interfaces Declarations (APIs) *******************/

Std_ReturnType EEPROM_Write_One_Byte(uint16 ee_add, uint8 ee_data);
Std_ReturnType EEPROM_Read_One_Byte(uint16 ee_add, uint8 *ee_data);


#endif	/* HAL_EEPROM_H */

