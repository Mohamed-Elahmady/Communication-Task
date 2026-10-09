/* 
 * File:   hal_eeprom_cfg.h
 * Author: mohammedel-ahmady
 *
 * Created on August 5, 2025, 6:50 PM
 */

/******************* Section 0 : File Guard *******************/

#ifndef HAL_EEPROM_CFG_H
#define	HAL_EEPROM_CFG_H

/******************* Section 1 : Includes *******************/



/******************* Section 2 : Macros Declarations *******************/

#define EEPROM_Write_CFG_Disable                        0x00
#define EEPROM_Write_CFG_Enable                         0x01
#define EEPROM_Read_CFG_Disable                         0x00
#define EEPROM_Read_CFG_Enable                          0x01

#define EEPROM_Write_CFG                                (EEPROM_Write_CFG_Enable)
#define EEPROM_Read_CFG                                 (EEPROM_Read_CFG_Enable)

/******************* Section 3 : Macros Functions Declarations *******************/



/******************* Section 4 : User Defined Data Types & Variables Declarations *******************/



/******************* Section 5 : Software Interfaces Declarations (APIs) *******************/



#endif	/* HAL_EEPROM_CFG_H */

