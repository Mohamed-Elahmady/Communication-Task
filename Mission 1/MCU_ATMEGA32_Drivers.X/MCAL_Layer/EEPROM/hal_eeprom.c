/* 
 * File:   hal_eeprom.c
 * Author: mohammedel-ahmady
 *
 * Created on August 5, 2025, 6:50 PM
 */

/******************* Section 0 : Includes *******************/

#include "hal_eeprom.h"

/******************* Section 1 :  Variables Definitions *******************/



/******************* Section 2 :  Helper Functions Declarations *******************/

/******************* Section 3 : Software Interfaces Definitions (APIs) *******************/

#if ((EEPROM_Write_CFG_Enable) == (EEPROM_Write_CFG))

/**
 * @brief           Write one byte of data to the internal EEPROM of ATmega32.
 * 
 * @param ee_add    The EEPROM address (0x000 to 0x3FF) where data will be written.
 * @param ee_data   The data byte to write (0x00 to 0xFF).
 * 
 * @return Std_ReturnType
 *      - E_OK       : Write operation completed successfully.
 *      - E_NOT_OK   : Invalid address or data value.
 */

Std_ReturnType EEPROM_Write_One_Byte(uint16 ee_add, uint8 ee_data) {
    Std_ReturnType Retval = E_OK;
    if (0x3FF < ee_add || 0xFF < ee_data) {
        Retval = E_NOT_OK;
    } else {
        // 0. Save the Global interrupt state
        uint8 Global_Interrupt_State = (uint8) ((SREG >> Sreg_Interrupt_Enable_Bit_Pos) & 0x01);
        // 1. check for any write operation in EEPROM
        while (EEPROM_Write_Enable_Read_Status());
        // 2. address the data location
        EEARH = (uint8) ((ee_add >> 8) & 0x03);
        EEARL = (uint8) (ee_add & 0xFF);
        // 3. assign data in EEPROM data register
        EEDR = ee_data;
        // 4. Set EEPROM Master Write Enable
        EEPROM_Master_Write_Enable();
        // 5. Disable Global Interrupts
        GLobal_Interrupt_Disable_SREG();
        // 6. Set EEPROM Write Enable
        EEPROM_Write_Enable();
        // 7. Set the state of global interrupt again
        if (Global_Interrupt_State) {
            GLobal_Interrupt_Enable_SREG();
        }
    }


    return Retval;
}

#endif 

#if ((EEPROM_Read_CFG_Enable) == (EEPROM_Read_CFG))

/**
 * @brief           Read one byte of data from the internal EEPROM of ATmega32.
 * 
 * @param ee_add    The EEPROM address (0x000 to 0x3FF) to read from.
 * @param ee_data   Pointer to store the read data byte.
 * 
 * @return Std_ReturnType
 *      - E_OK       : Read operation completed successfully.
 *      - E_NOT_OK   : NULL pointer or invalid address.
 */

Std_ReturnType EEPROM_Read_One_Byte(uint16 ee_add, uint8 *ee_data) {
    Std_ReturnType Retval = E_OK;
    if (NULL == ee_data || 0x3FF < ee_add || 0xFF < ee_data) {
        Retval = E_NOT_OK;
    } else {
        // 1. check for any write operation in EEPROM
        while (EEPROM_Write_Enable_Read_Status());
        // 2. address the data location
        EEARH = (uint8) ((ee_add >> 8) & 0x03);
        EEARL = (uint8) (ee_add & 0xFF);
        // 3. Set EEPROM Read Enable
        EEPROM_Read_Enable();
        // 4. Return data from EEPROM
        *ee_data = EEDR;
    }
    return Retval;
}

#endif

/******************* Section 4:  Helper Functions Definitions *******************/


