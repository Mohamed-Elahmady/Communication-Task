/* 
 * File:   ecual_relay.h
 * Author: mohammedel-ahmady
 *
 * Created on July 7, 2025, 6:06 PM
 */

/******************* Section 0 : File Guard *******************/

#ifndef ECUAL_RELAY_H
#define	ECUAL_RELAY_H

/******************* Section 1 : Includes *******************/

#include "../../MCAL_Layer/GPIO/hal_gpio.h"
#include "ecual_relay_cfg.h"

/******************* Section 2 : Macros Declarations *******************/



/******************* Section 3 : Macros Functions Declarations *******************/



/******************* Section 4 : User Defined Data Types & Variables Declarations *******************/

typedef enum{
    Relay_Off   = (uint8)0x00,
    Relay_On    = (uint8)0x01
}Relay_State;

typedef struct{
    GPIO_Pin_CFG pin;
}GPIO_Relay;

/******************* Section 5 : Software Interfaces Declarations (APIs) *******************/

Std_ReturnType GPIO_Relay_INIT(const GPIO_Relay *relay);
Std_ReturnType GPIO_Relay_Read_State(const GPIO_Relay *relay, Relay_State *state);
Std_ReturnType GPIO_Relay_Turn_On(const GPIO_Relay *relay);
Std_ReturnType GPIO_Relay_Turn_Off(const GPIO_Relay *relay);
Std_ReturnType GPIO_Relay_Turn_Toggle(const GPIO_Relay *relay);

#endif	/* ECUAL_RELAY_H */

