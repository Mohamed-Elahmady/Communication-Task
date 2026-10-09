/* 
 * File:   ecual_led.h
 * Author: mohammedel-ahmady
 *
 * Created on June 26, 2025, 8:01 PM
 */

/******************* Section 0 : File Guard *******************/

#ifndef ECUAL_LED_H
#define	ECUAL_LED_H

/******************* Section 1 : Includes *******************/

#include "../../MCAL_Layer/GPIO/hal_gpio.h"
#include "ecual_led_cfg.h"

/******************* Section 2 : Macros Declarations *******************/



/******************* Section 3 : Macros Functions Declarations *******************/



/******************* Section 4 : User Defined Data Types & Variables Declarations *******************/

typedef enum{
    Led_Off =   (uint8)0x00,
    Led_On  =   (uint8)0x01
}Led_State;

typedef enum{
    Led_Sink    =   (uint8)0x00,
    Led_Source  =   (uint8)0x01
}Led_Con;

typedef struct{
    GPIO_Pin_CFG   pin;
    Led_State    state;
    Led_Con connection;
}GPIO_Led;

/******************* Section 5 : Software Interfaces Declarations (APIs) *******************/

Std_ReturnType GPIO_Led_INIT(const GPIO_Led *led);
Std_ReturnType GPIO_Led_Read_Logic(const GPIO_Led *led, Led_State *state);
Std_ReturnType GPIO_Led_Turn_Off(const GPIO_Led *led);
Std_ReturnType GPIO_Led_Turn_On(const GPIO_Led *led);
Std_ReturnType GPIO_Led_Turn_Toggle(const GPIO_Led *led);

#endif	/* ECUAL_LED_H */

