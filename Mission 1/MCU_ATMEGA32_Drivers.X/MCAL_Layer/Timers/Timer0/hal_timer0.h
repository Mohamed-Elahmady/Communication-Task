/* 
 * File:   hal_timer0.h
 * Author: mohammedel-ahmady
 *
 * Created on August 20, 2025, 6:52 AM
 */

/******************* Section 0 : File Guard *******************/

#ifndef HAL_TIMER0_H
#define	HAL_TIMER0_H

/******************* Section 1 : Includes *******************/

#include "../../mcal_std_types.h"
#include "../../Interrupt/hal_int_interrupt.h"
#include "../../GPIO/hal_gpio.h"
#include "hal_timer0_cfg.h"
#include "../../device_cfg.h"

/******************* Section 2 : Macros Declarations *******************/



/******************* Section 3 : Macros Functions Declarations *******************/

// Timer 0 Waveform Generation Mode
#define Timer0_Waveform_Mode00_Normal()                                Clear_Bit(TCCR0, WGM00)
#define Timer0_Waveform_Mode01_Normal()                                Clear_Bit(TCCR0, WGM01)

#define Timer0_Waveform_Mode00_CTC()                                   Clear_Bit(TCCR0, WGM00)
#define Timer0_Waveform_Mode01_CTC()                                   Set_Bit(TCCR0, WGM01)

// Timer 0 No PWM Compare Match Mode
#define Timer0_COM00_OC0_Disable()                                     Clear_Bit(TCCR0, COM00)
#define Timer0_COM01_OC0_Disable()                                     Clear_Bit(TCCR0, COM01)

#define Timer0_COM00_OC0_Toggle()                                      Set_Bit(TCCR0, COM00)
#define Timer0_COM01_OC0_Toggle()                                      Clear_Bit(TCCR0, COM01)

#define Timer0_COM00_OC0_Clear()                                       Clear_Bit(TCCR0, COM00)
#define Timer0_COM01_OC0_Clear()                                       Set_Bit(TCCR0, COM01)

#define Timer0_COM00_OC0_Set()                                         Set_Bit(TCCR0, COM00)
#define Timer0_COM01_OC0_Set()                                         Set_Bit(TCCR0, COM01)

// Timer0 Force Output Compare
#define Timer0_Force_Output_Compare_Enable()                           Set_Bit(TCCR0, FOC0)

// ?Reset timer Prescaller Before use
#define Timer0_Reset_Prescaler_Circuit()                               Set_Bit(SFIOR, PSR10)

/******************* Section 4 : User Defined Data Types & Variables Declarations *******************/

typedef void (* Timer0_Handler)(void);

typedef enum{
    Timer0_Module_Disable = (uint8)0x00,
    Timer0_No_Prescaler, // Prescaler = 1
    Timer0_Prescaler_DIV_8,
    Timer0_Prescaler_DIV_64,
    Timer0_Prescaler_DIV_256,
    Timer0_Prescaler_DIV_1024,
    Timer0_Counter_Falling_Edge,
    Timer0_Counter_Rising_Edge
}Timer0_Clock_Prescaler_Select;

typedef enum{
    Timer0_Normal_Mode = (uint8)0x00, // timer or counter has limits from 0 -> 255
    Timer0_CTC_Mode                   // timer or counter has limits from 0 -> OCR0
}Timer0_Mode;

typedef enum{
    Timer0_Timer_Operation = (uint8)0x00,
    Timer0_Counter_Operation
}Timer0_Operation;

typedef enum{
    Timer0_COM_Mode_Disable_OC0 = (uint8)0x00,
    Timer0_COM_Mode_Toggle_OC0,
    Timer0_COM_Mode_Clear_OC0,
    Timer0_COM_Mode_Set_OC0
}Timer0_COM_Mode;

typedef struct{
#if ((Interrupt_Feature_Enable) == (Timer0_Interrupt_Feature))
    Timer0_Handler          timer0_interrupt;
#endif
    uint8             timer0_preloaded_value;
    uint8                     timer0_ctc_max; // for only CTC Mode
    Timer0_Clock_Prescaler_Select  prescaler;
    Timer0_Mode                         mode;
    Timer0_Operation               operation;
    Timer0_COM_Mode             compare_mode;
}Timer0_CFG;

/******************* Section 5 : Software Interfaces Declarations (APIs) *******************/

Std_ReturnType Timer0_INIT(const Timer0_CFG *timer0);
Std_ReturnType Timer0_DEINIT(const Timer0_CFG *timer0);
Std_ReturnType Timer0_Write_Data(const Timer0_CFG *timer0, uint8 Data);
Std_ReturnType Timer0_Read_Data(const Timer0_CFG *timer0, uint8 *Data);
Std_ReturnType Timer0_Write_CTC_Max(const Timer0_CFG *timer0, uint8 ctc_max);
Std_ReturnType Timer0_Read_CTC_Max(const Timer0_CFG *timer0, uint8 *ctc_max);


#endif	/* HAL_TIMER0_H */

