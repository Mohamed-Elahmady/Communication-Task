/* 
 * File:   ecual_keypad.h
 * Author: mohammedel-ahmady
 *
 * Created on July 14, 2025, 12:46 PM
 */

/******************* Section 0 : File Guard *******************/

#ifndef ECUAL_KEYPAD_H
#define	ECUAL_KEYPAD_H

/******************* Section 1 : Includes *******************/

#include "../../MCAL_Layer/GPIO/hal_gpio.h"
#include "ecual_keypad_cfg.h"

/******************* Section 2 : Macros Declarations *******************/

#define Zero_Init                   (uint8)0x00

#if ((Keypad_Phone) == (Keypad_Type))

#define Keypad_Row                  (uint8)0x04
#define Keypad_Col                  (uint8)0x03

extern const uint8 Keypad_Btns[Keypad_Row][Keypad_Col];

#elif ((Keypad_Small_Calc) == (Keypad_Type))

#define Keypad_Row                  (uint8)0x04
#define Keypad_Col                  (uint8)0x04

extern const uint8 Keypad_Btns[Keypad_Row][Keypad_Col];

#elif ((Keypad_Calculator) == (Keypad_Type))

#define Keypad_Row                  (uint8)0x04
#define Keypad_Col                  (uint8)0x06

extern const uint8 Keypad_Btns[Keypad_Row][Keypad_Col];

#endif

/******************* Section 3 : Macros Functions Declarations *******************/



/******************* Section 4 : User Defined Data Types & Variables Declarations *******************/

typedef struct{
    GPIO_Pin_CFG Row_Pins[Keypad_Row];
    GPIO_Pin_CFG Col_Pins[Keypad_Col];
}GPIO_Keypad;

/******************* Section 5 : Software Interfaces Declarations (APIs) *******************/

Std_ReturnType GPIO_Keypad_INIT(const GPIO_Keypad *keypad);
Std_ReturnType GPIO_Keypad_Read_Data(const GPIO_Keypad *keypad, uint8 *data);

#endif	/* ECUAL_KEYPAD_H */

