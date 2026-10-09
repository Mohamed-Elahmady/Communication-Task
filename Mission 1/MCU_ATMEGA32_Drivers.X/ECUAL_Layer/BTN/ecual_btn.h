/* 
 * File:   ecual_btn.h
 * Author: mohammedel-ahmady
 *
 * Created on July 5, 2025, 1:07 PM
 */

/******************* Section 0 : File Guard *******************/

#ifndef ECUAL_BTN_H
#define	ECUAL_BTN_H

/******************* Section 1 : Includes *******************/

#include "../../MCAL_Layer/GPIO/hal_gpio.h"
#include "ecual_btn_cfg.h"

/******************* Section 2 : Macros Declarations *******************/



/******************* Section 3 : Macros Functions Declarations *******************/



/******************* Section 4 : User Defined Data Types & Variables Declarations *******************/

typedef enum{
    Btn_Released    = (uint8)0x00,
    Btn_Pressed     = (uint8)0x01
}Btn_State;

typedef enum{
    Btn_Active_Low  = (uint8)0x00,
    Btn_Active_High = (uint8)0x01
}Btn_Con;

typedef struct{
    GPIO_Pin_CFG   pin;
    Btn_Con connection;
}GPIO_Btn;

/******************* Section 5 : Software Interfaces Declarations (APIs) *******************/

Std_ReturnType GPIO_Btn_INIT(const GPIO_Btn *btn);
Std_ReturnType GPIO_Btn_Read_State(const GPIO_Btn *btn, Btn_State *state);

#endif	/* ECUAL_BTN_H */

