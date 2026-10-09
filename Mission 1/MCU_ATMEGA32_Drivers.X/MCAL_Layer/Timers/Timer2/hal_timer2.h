/* 
 * File:   hal_timer2.h
 * Author: mohammedel-ahmady
 *
 * Created on August 28, 2025, 12:50 PM
 */

/******************* Section 0 : File Guard *******************/

#ifndef HAL_TIMER2_H
#define	HAL_TIMER2_H

/******************* Section 1 : Includes *******************/

#include "../../mcal_std_types.h"
#include "../../GPIO/hal_gpio.h"
#include "../../Interrupt/hal_int_interrupt.h"
#include "hal_timer2_cfg.h"
#include "../../device_cfg.h"

/******************* Section 2 : Macros Declarations *******************/



/******************* Section 3 : Macros Functions Declarations *******************/

#define Timer2_Waveform_Mode20_Normal()                     Clear_Bit(TCCR2, WGM20)
#define Timer2_Waveform_Mode21_Normal()                     Clear_Bit(TCCR2, WGM21)

#define Timer2_Waveform_Mode20_CTC()                        Clear_Bit(TCCR2, WGM20)
#define Timer2_Waveform_Mode21_CTC()                        Set_Bit(TCCR2, WGM21)

#define Timer2_COM20_OC2_Disable()                          Clear_Bit(TCCR2, COM20)
#define Timer2_COM21_OC2_Disable()                          Clear_Bit(TCCR2, COM21)

#define Timer2_COM20_OC2_Toggle()                           Set_Bit(TCCR2, COM20)
#define Timer2_COM21_OC2_Toggle()                           Clear_Bit(TCCR2, COM21)

#define Timer2_COM20_OC2_Clear()                            Clear_Bit(TCCR2, COM20)
#define Timer2_COM21_OC2_Clear()                            Set_Bit(TCCR2, COM21)

#define Timer2_COM20_OC2_Set()                              Set_Bit(TCCR2, COM20)
#define Timer2_COM21_OC2_Set()                              Set_Bit(TCCR2, COM21)


#define Timer2_Force_Compare_Output_Enable()                Set_Bit(TCCR2, FOC2)

#define Timer2_Reset_Prescaler_Cirrcuit()                   Set_Bit(SFIOR, PSR2)

/******************* Section 4 : User Defined Data Types & Variables Declarations *******************/

typedef void (* Timer2_Handler)(void);

typedef enum{
    Timer2_Moudle_Disable = (uint8)0x00,
    Timer2_Prescaler_DIV_1,
    Timer2_Prescaler_DIV_8,
    Timer2_Prescaler_DIV_32,
    Timer2_Prescaler_DIV_64,
    Timer2_Prescaler_DIV_128,
    Timer2_Prescaler_DIV_256,
    Timer2_Prescaler_DIV_1024
}Timer2_Prescaler_Select;

typedef enum{
    Timer2_Normal_Mode = (uint8)0x00,
    Timer2_CTC_Mode
}Timer2_Mode;

typedef enum{
    Timer2_COM_Mode_OC2_Disable = (uint8)0x00,
    Timer2_COM_Mode_OC2_Toggle,
    Timer2_COM_Mode_OC2_Clear,
    Timer2_COM_Mode_OC2_Set
}Timer2_COM_Mode;

typedef struct{
#if ((Interrupt_Feature_Enable) == (Timer2_Interrupt_Feature))
    Timer2_Handler       timer2_interrupt;
#endif
    uint8                 preloaded_value;
    uint8                         ctc_max;
    Timer2_Mode                      mode;
    Timer2_Prescaler_Select     prescaler;
    Timer2_COM_Mode              com_mode;
}Timer2_CFG;

/******************* Section 5 : Software Interfaces Declarations (APIs) *******************/

Std_ReturnType Timer2_INIT(const Timer2_CFG *timer2);
Std_ReturnType Timer2_DEINIT(const Timer2_CFG *timer2);
Std_ReturnType Timer2_Write_Data(const Timer2_CFG *timer2, uint8 Data);
Std_ReturnType Timer2_Read_Data(const Timer2_CFG *timer2, uint8 *Data);
Std_ReturnType Timer2_Write_CTC_Max(const Timer2_CFG *timer2, uint8 ctc_max);
Std_ReturnType Timer2_Read_CTC_Max(const Timer2_CFG *timer2, uint8 *ctc_max);

#endif	/* HAL_TIMER2_H */

