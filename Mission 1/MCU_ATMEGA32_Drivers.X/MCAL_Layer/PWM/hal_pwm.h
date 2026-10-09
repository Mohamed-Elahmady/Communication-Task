/* 
 * File:   hal_pwm.h
 * Author: mohammedel-ahmady
 *
 * Created on September 13, 2025, 2:31 PM
 */

/******************* Section 0 : File Guard *******************/

#ifndef HAL_PWM_H
#define	HAL_PWM_H

/******************* Section 1 : Includes *******************/

#include "hal_pwm_cfg.h"
#include "../GPIO/hal_gpio.h"
#include "../Timers/Timer0/hal_timer0.h"
#include "../Timers/Timer1/hal_timer1.h"
#include "../Timers/Timer2/hal_timer2.h"
#include "../mcal_std_types.h"
#include "../device_cfg.h"

/******************* Section 2 : Macros Declarations *******************/



/******************* Section 3 : Macros Functions Declarations *******************/

// PWM with Timer 0

#define PWM_Phase_Timer0_Waveform_Generate()              (TCCR0 = (TCCR0 | (1 << WGM00)) & ~(1 << WGM01))
#define PWM_Fast_Timer0_Waveform_Generate()               (TCCR0 = (TCCR0 | (1 << WGM00)) | (1 << WGM01))

#define PWM_Timer0_OC0_Disconnected()                     (TCCR0 = (TCCR0 & ~(1 << COM00)) & ~(1 << COM01))
#define PWM_Timer0_OC0_Clear_Logic_Non_Inverting()        (TCCR0 = (TCCR0 & ~(1 << COM00)) | (1 << COM01))
#define PWM_Timer0_OC0_Set_Logic_Inverting()              (TCCR0 = (TCCR0 | (1 << COM00)) | (1 << COM01))

#define PWM_TIMER0_SET_PRESCALER(__PRESCALER)             (TCCR0 = ((TCCR0 & 0xF8) | (__PRESCALER & 0x07)))

// PWM with Timer 1

#define PWM_Phase_Timer1_Waveform_Generate()              (TCCR1B = (TCCR1B & ~(1 << WGM12)) & ~(1 << WGM13))
#define PWM_Fast_Timer1_Waveform_Generate()               (TCCR1B = (TCCR1B | (1 << WGM12)) & ~(1 << WGM13))

#define PWM_Timer1_Waveform_Generate_8Bit()               (TCCR1A = (TCCR1A | (1 << WGM10)) & ~(1 << WGM11))
#define PWM_Timer1_Waveform_Generate_9Bit()               (TCCR1A = (TCCR1A & ~(1 << WGM10)) | (1 << WGM11))
#define PWM_Timer1_Waveform_Generate_10Bit()              (TCCR1A = (TCCR1A | (1 << WGM10)) | (1 << WGM11))

#define PWM_Timer1_OCA1_Disconnected()                    (TCCR1A = (TCCR1A & ~(1 << COM1A0)) & ~(1 << COM1A1))
#define PWM_Timer1_OCA1_Toggle_Logic()                    (TCCR1A = (TCCR1A | (1 << COM1A0)) & ~(1 << COM1A1)) 
#define PWM_Timer1_OCA1_Clear_Logic_Non_Inverting()       (TCCR1A = (TCCR1A & ~(1 << COM1A0)) | (1 << COM1A1)) 
#define PWM_Timer1_OCA1_Set_Logic_Inverting()             (TCCR1A = (TCCR1A | (1 << COM1A0)) | (1 << COM1A1)) 

#define PWM_Timer1_OCB1_Disconnected()                    (TCCR1A = (TCCR1A & ~(1 << COM1B0)) & ~(1 << COM1B1))
#define PWM_Timer1_OCB1_Toggle_Logic()                    (TCCR1A = (TCCR1A | (1 << COM1B0)) & ~(1 << COM1B1))
#define PWM_Timer1_OCB1_Clear_Logic_Non_Inverting()       (TCCR1A = (TCCR1A & ~(1 << COM1B0)) | (1 << COM1B1)) 
#define PWM_Timer1_OCB1_Set_Logic_Inverting()             (TCCR1A = (TCCR1A | (1 << COM1B0)) | (1 << COM1B1)) 

#define PWM_TIMER1_SET_PRESCALER(__PRESCALER)             (TCCR1B = ((TCCR1B & 0xF8) | (__PRESCALER & 0x07)))

// PWM with Timer 2

#define PWM_Phase_Timer2_Waveform_Generate()              (TCCR2 = (TCCR2 | (1 << WGM20)) & ~(1 << WGM21))
#define PWM_Fast_Timer2_Waveform_Generate()               (TCCR2 = (TCCR2 | (1 << WGM20)) | (1 << WGM21))

#define PWM_Timer2_OC2_Disconnected()                     (TCCR2 = (TCCR2 & ~(1 << COM20)) & ~(1 << COM21))
#define PWM_Timer2_OC2_Clear_Logic_Non_Inverting()        (TCCR2 = (TCCR2 & ~(1 << COM20)) | (1 << COM21))
#define PWM_Timer2_OC2_Set_Logic_Inverting()              (TCCR2 = (TCCR2 | (1 << COM20)) | (1 << COM21))

#define PWM_TIMER2_SET_PRESCALER(__PRESCALER)             (TCCR2 = ((TCCR2 & 0xF8) | (__PRESCALER & 0x07)))

/******************* Section 4 : User Defined Data Types & Variables Declarations *******************/

typedef enum{
    PWM_OC0 = (uint8)0x00,
    PWM_OCA1,
    PWM_OCB1,
    PWM_OC2
}PWM_Source;

typedef enum{
    PWM_Phase_Mode = (uint8)0x00,
    PWM_Fast_Mode
}PWM_Mode;

typedef enum{
    PWM_8Bit = (uint8)0x00,
    PWM_9Bit,
    PWM_10Bit
}PWM_Resolution;

typedef enum{
    PWM_OCXX_Disconnected_Pin = (uint8)0x00,
    PWM_OCXX_Toggle_Pin,
    PWM_Non_Inverting_OCXX_Clear_Pin,        
    PWM_Inverting_OCXX_Set_Pin
}PWM_OCXX_PIN_ACTION;

typedef enum{
    PWM_Module_Disable = (uint8)0x00,
    PWM_Prescaler_DIV_1,
    PWM_Prescaler_DIV_8,
    PWM_Prescaler_DIV_32,        
    PWM_Prescaler_DIV_64,
    PWM_Prescaler_DIV_128,        
    PWM_Prescaler_DIV_256,
    PWM_Prescaler_DIV_1024
}PWM_Prescaler;

typedef struct{
    PWM_Source                  pwm_src;
    PWM_Mode                   pwm_mode;
    PWM_Resolution              pwm_res;
    PWM_OCXX_PIN_ACTION         pwm_act;
    PWM_Prescaler         pwm_prescaler;
}PWM_CFG;

/******************* Section 5 : Software Interfaces Declarations (APIs) *******************/

Std_ReturnType PWM_INIT(const PWM_CFG *pwm);
Std_ReturnType PWM_DEINIT(const PWM_CFG *pwm);
Std_ReturnType PWM_Start(const PWM_CFG *pwm);
Std_ReturnType PWM_Stop(const PWM_CFG *pwm);
Std_ReturnType PWM_Set_Duty_Cycle(const PWM_CFG *pwm, uint16 duty);

#endif	/* HAL_PWM_H */

