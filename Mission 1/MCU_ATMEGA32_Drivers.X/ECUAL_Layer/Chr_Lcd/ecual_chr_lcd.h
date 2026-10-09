/* 
 * File:   ecual_chr_lcd.h
 * Author: mohammedel-ahmady
 *
 * Created on July 22, 2025, 3:23 PM
 */

/******************* Section 0 : File Guard *******************/

#ifndef ECUAL_CHR_LCD_H
#define	ECUAL_CHR_LCD_H

/******************* Section 1 : Includes *******************/

#include "../../MCAL_Layer/GPIO/hal_gpio.h"
#include "ecual_chr_lcd_cfg.h"

/******************* Section 2 : Macros Declarations *******************/

// Configuration Commands

// Clear Display
#define Lcd_Clear_Display                               (uint8)0x01
// Cursor Home
#define Lcd_Cursor_Home                                 (uint8)0x02
// Entry Set Mode
#define Lcd_Esm_Dec_Sft_Off                             (uint8)0x04
#define Lcd_Esm_Dec_Sft_On                              (uint8)0x05
#define Lcd_Esm_Inc_Sft_Off                             (uint8)0x06
#define Lcd_Esm_Inc_Sft_On                              (uint8)0x07
// Display On/Off
#define Lcd_Display_Off                                 (uint8)0x08
#define Lcd_Display_On_Ulc_Off_Blc_Off                  (uint8)0x0C
#define Lcd_Display_On_Ulc_Off_Blc_On                   (uint8)0x0D
#define Lcd_Display_On_Ulc_On_Blc_Off                   (uint8)0x0E
#define Lcd_Display_On_Ulc_On_Blc_On                    (uint8)0x0F
// Cursor Or Display Shift
#define Lcd_Mvc_Left                                    (uint8)0x10
#define Lcd_Mvc_Right                                   (uint8)0x14
#define Lcd_Sfd_Left                                    (uint8)0x18
#define Lcd_Sfd_Right                                   (uint8)0x1C
// Function Set
#define Lcd_4Bit_Onl_F58                                (uint8)0x20
#define Lcd_4Bit_Onl_F511                               (uint8)0x24
#define Lcd_4Bit_Twl_F58                                (uint8)0x28
#define Lcd_4Bit_Twl_F511                               (uint8)0x2C

#define Lcd_8Bit_Onl_F58                                (uint8)0x30
#define Lcd_8Bit_Onl_F511                               (uint8)0x34
#define Lcd_8Bit_Twl_F58                                (uint8)0x38
#define Lcd_8Bit_Twl_F511                               (uint8)0x3C
// Set CGRAM Start Address
#define Lcd_CGRAM_Address                               (uint8)0x40
// Set DDRAM Start Address
#define Lcd_DDRAM_Address                               (uint8)0x80

// Driver Macros

#define Lcd_4Bit_Mode                                   (uint8)0x04
#define Lcd_8Bit_Mode                                   (uint8)0x08

#define Lcd_Row1                                        (uint8)0x01
#define Lcd_Row2                                        (uint8)0x02
#define Lcd_Row3                                        (uint8)0x03
#define Lcd_Row4                                        (uint8)0x04

#define Row1_Add                                        (uint8)0x80
#define Row2_Add                                        (uint8)0xC0
#define Row3_Add                                        (uint8)0x94
#define Row4_Add                                        (uint8)0xD4

#define Custom_Char_No                                  (uint8)0x08

/******************* Section 3 : Macros Functions Declarations *******************/



/******************* Section 4 : User Defined Data Types & Variables Declarations *******************/

typedef struct{
    GPIO_Pin_CFG Rs_Pin;
    GPIO_Pin_CFG Rw_Pin;
    GPIO_Pin_CFG En_Pin;
    GPIO_Pin_CFG Data_Pins[Lcd_4Bit_Mode];
}Chr_Lcd_4Bit;

typedef struct{
    GPIO_Pin_CFG Rs_Pin;
    GPIO_Pin_CFG Rw_Pin;
    GPIO_Pin_CFG En_Pin;
    GPIO_Pin_CFG Data_Pins[Lcd_8Bit_Mode];
}Chr_Lcd_8Bit;

/******************* Section 5 : Software Interfaces Declarations (APIs) *******************/

Std_ReturnType GPIO_Chr_Lcd_4Bit_INIT(const Chr_Lcd_4Bit *lcd);
Std_ReturnType GPIO_Chr_Lcd_4Bit_Send_Command(const Chr_Lcd_4Bit *lcd, uint8 command);
Std_ReturnType GPIO_Chr_Lcd_4Bit_Send_Character(const Chr_Lcd_4Bit *lcd, uint8 character);
Std_ReturnType GPIO_Chr_Lcd_4Bit_Send_Character_Pos(const Chr_Lcd_4Bit *lcd, uint8 character, uint8 row, uint8 col);
Std_ReturnType GPIO_Chr_Lcd_4Bit_Send_String(const Chr_Lcd_4Bit *lcd, uint8 *str);
Std_ReturnType GPIO_Chr_Lcd_4Bit_Send_String_Pos(const Chr_Lcd_4Bit *lcd, uint8 *str, uint8 row, uint8 col);
Std_ReturnType GPIO_Chr_Lcd_4Bit_Send_Custom_Character(const Chr_Lcd_4Bit *lcd, uint8 symbol[], uint8 row, uint8 col, uint8 ram_pos);

Std_ReturnType GPIO_Chr_Lcd_8Bit_INIT(const Chr_Lcd_8Bit *lcd);
Std_ReturnType GPIO_Chr_Lcd_8Bit_Send_Command(const Chr_Lcd_8Bit *lcd, uint8 command);
Std_ReturnType GPIO_Chr_Lcd_8Bit_Send_Character(const Chr_Lcd_8Bit *lcd, uint8 character);
Std_ReturnType GPIO_Chr_Lcd_8Bit_Send_Character_Pos(const Chr_Lcd_8Bit *lcd, uint8 character, uint8 row, uint8 col);
Std_ReturnType GPIO_Chr_Lcd_8Bit_Send_String(const Chr_Lcd_8Bit *lcd, uint8 *str);
Std_ReturnType GPIO_Chr_Lcd_8Bit_Send_String_Pos(const Chr_Lcd_8Bit *lcd, uint8 *str, uint8 row, uint8 col);
Std_ReturnType GPIO_Chr_Lcd_8Bit_Send_Custom_Character(const Chr_Lcd_8Bit *lcd, uint8 symbol[], uint8 row, uint8 col, uint8 ram_pos);

Std_ReturnType Convert_uint8_To_String(uint8 data, uint8 *str);
Std_ReturnType Convert_uint16_To_String(uint16 data, uint8 *str);
Std_ReturnType Convert_uint32_To_String(uint32 data, uint8 *str);

#endif	/* ECUAL_CHR_LCD_H */

