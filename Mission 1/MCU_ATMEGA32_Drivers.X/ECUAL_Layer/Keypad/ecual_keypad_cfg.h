/* 
 * File:   ecual_keypad_cfg.h
 * Author: mohammedel-ahmady
 *
 * Created on July 14, 2025, 12:47 PM
 */

/******************* Section 0 : File Guard *******************/

#ifndef ECUAL_KEYPAD_CFG_H
#define	ECUAL_KEYPAD_CFG_H

/******************* Section 1 : Includes *******************/



/******************* Section 2 : Macros Declarations *******************/

#define GPIO_Keypad_CFG_Disable                 0x00
#define GPIO_Keypad_CFG_Enable                  0x01

#define GPIO_Keypad_CFG                         (GPIO_Keypad_CFG_Enable)

#define Keypad_Phone                            0x00
#define Keypad_Small_Calc                       0x01
#define Keypad_Calculator                       0x02

#define Keypad_Type                             (Keypad_Small_Calc)

/******************* Section 3 : Macros Functions Declarations *******************/



/******************* Section 4 : User Defined Data Types & Variables Declarations *******************/



/******************* Section 5 : Software Interfaces Declarations (APIs) *******************/

#endif	/* ECUAL_KEYPAD_CFG_H */

