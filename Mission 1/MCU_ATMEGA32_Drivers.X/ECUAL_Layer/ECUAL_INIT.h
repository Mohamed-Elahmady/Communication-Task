/* 
 * File:   ECUAL_INIT.h
 * Author: mohammedel-ahmady
 *
 * Created on July 14, 2025, 12:30 PM
 */

/******************* Section 0 : File Guard *******************/

#ifndef ECUAL_INIT_H
#define	ECUAL_INIT_H

/******************* Section 1 : Includes *******************/

#include "LED/ecual_led.h"
#include "BTN/ecual_btn.h"
#include "Relay/ecual_relay.h"
#include "DC_Motor/ecual_dc_motor.h"
#include "7SEG/ecual_7seg.h"
#include "Keypad/ecual_keypad.h"
#include "Chr_Lcd/ecual_chr_lcd.h"

#include "../MCAL_Layer/Interrupt/hal_int_interrupt.h"
#include "../MCAL_Layer/Interrupt/hal_ext_interrupt.h"

#include "../MCAL_Layer/EEPROM/hal_eeprom.h"

#include "../MCAL_Layer/ADC/hal_adc.h"

#include "../MCAL_Layer/Timers/Timer0/hal_timer0.h"
#include "../MCAL_Layer/Timers/Timer1/hal_timer1.h"
#include "../MCAL_Layer/Timers/Timer2/hal_timer2.h"

#include "../MCAL_Layer/PWM/hal_pwm.h"

#include "../MCAL_Layer/USART/hal_usart.h"
#include "../MCAL_Layer/SPI/hal_spi.h"
#include "../MCAL_Layer/TWI/hal_twi.h"

/******************* Section 2 : Macros Declarations *******************/



/******************* Section 3 : Macros Functions Declarations *******************/



/******************* Section 4 : User Defined Data Types & Variables Declarations *******************/



/******************* Section 5 : Software Interfaces Declarations (APIs) *******************/

void Ecual_Init(void);

#endif	/* ECUAL_INIT_H */

