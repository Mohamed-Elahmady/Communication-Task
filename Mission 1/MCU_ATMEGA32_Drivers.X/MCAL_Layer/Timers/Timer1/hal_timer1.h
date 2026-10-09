/* 
 * File:   hal_timer1.h
 * Author: mohammedel-ahmady
 *
 * Created on August 25, 2025, 10:43 AM
 */

/******************* Section 0 : File Guard *******************/

#ifndef HAL_TIMER1_H
#define	HAL_TIMER1_H

/******************* Section 1 : Includes *******************/

#include "hal_timer1_cfg.h"
#include "../../GPIO/hal_gpio.h"
#include "../../Interrupt/hal_int_interrupt.h"
#include "../../mcal_std_types.h"
#include "../../device_cfg.h"

/******************* Section 2 : Macros Declarations *******************/



/******************* Section 3 : Macros Functions Declarations *******************/

// Normal Mode
#define Timer1_Waveform_Generation10_Normal_Mode()                  Clear_Bit(TCCR1A, WGM10)
#define Timer1_Waveform_Generation11_Normal_Mode()                  Clear_Bit(TCCR1A, WGM11)
#define Timer1_Waveform_Generation12_Normal_Mode()                  Clear_Bit(TCCR1B, WGM12)
#define Timer1_Waveform_Generation13_Normal_Mode()                  Clear_Bit(TCCR1B, WGM13)

// CTC OCR Mode
#define Timer1_Waveform_Generation10_CTC_OCR_Mode()                  Clear_Bit(TCCR1A, WGM10)
#define Timer1_Waveform_Generation11_CTC_OCR_Mode()                  Clear_Bit(TCCR1A, WGM11)
#define Timer1_Waveform_Generation12_CTC_OCR_Mode()                  Set_Bit(TCCR1B, WGM12)
#define Timer1_Waveform_Generation13_CTC_OCR_Mode()                  Clear_Bit(TCCR1B, WGM13)

// CTC OCR Mode
#define Timer1_Waveform_Generation10_CTC_ICR_Mode()                  Clear_Bit(TCCR1A, WGM10)
#define Timer1_Waveform_Generation11_CTC_ICR_Mode()                  Clear_Bit(TCCR1A, WGM11)
#define Timer1_Waveform_Generation12_CTC_ICR_Mode()                  Set_Bit(TCCR1B, WGM12)
#define Timer1_Waveform_Generation13_CTC_ICR_Mode()                  Set_Bit(TCCR1B, WGM13)

// Force Compare Output for OC1A
#define Timer1_Force_Compare_Output_OC1A_Enable()                    Set_Bit(TCCR1A, FOC1A)
// Force Compare Output for OC1B
#define Timer1_Force_Compare_Output_OC1B_Enable()                    Set_Bit(TCCR1A, FOC1B)

// Compare Output Mode Operation for OC1A
#define Timer1_COM1A0_OC1A_Disable()                                 Clear_Bit(TCCR1A, COM1A0)
#define Timer1_COM1A1_OC1A_Disable()                                 Clear_Bit(TCCR1A, COM1A1)

#define Timer1_COM1A0_OC1A_Toggle()                                  Set_Bit(TCCR1A, COM1A0)
#define Timer1_COM1A1_OC1A_Toggle()                                  Clear_Bit(TCCR1A, COM1A1)

#define Timer1_COM1A0_OC1A_Clear()                                   Clear_Bit(TCCR1A, COM1A0)
#define Timer1_COM1A1_OC1A_Clear()                                   Set_Bit(TCCR1A, COM1A1)

#define Timer1_COM1A0_OC1A_Set()                                     Set_Bit(TCCR1A, COM1A0)
#define Timer1_COM1A1_OC1A_Set()                                     Set_Bit(TCCR1A, COM1A1)

// Compare Output Mode Operation for OC1B
#define Timer1_COM1B0_OC1B_Disable()                                 Clear_Bit(TCCR1A, COM1B0)
#define Timer1_COM1B1_OC1B_Disable()                                 Clear_Bit(TCCR1A, COM1B1)

#define Timer1_COM1B0_OC1B_Toggle()                                  Set_Bit(TCCR1A, COM1B0)
#define Timer1_COM1B1_OC1B_Toggle()                                  Clear_Bit(TCCR1A, COM1B1)

#define Timer1_COM1B0_OC1B_Clear()                                   Clear_Bit(TCCR1A, COM1B0)
#define Timer1_COM1B1_OC1B_Clear()                                   Set_Bit(TCCR1A, COM1B1)

#define Timer1_COM1B0_OC1B_Set()                                     Set_Bit(TCCR1A, COM1B0)
#define Timer1_COM1B1_OC1B_Set()                                     Set_Bit(TCCR1A, COM1B1)

// Input Capture Noise Cancelation
#define Timer1_Noise_Cancelation_Disable()                           Clear_Bit(TCCR1B, ICNC1)
#define Timer1_Noise_Cancelation_Enable()                            Set_Bit(TCCR1B, ICNC1)
// Input Capture Select Edge
#define Timer1_Input_Capture_Falling_Edge()                          Clear_Bit(TCCR1B, ICES1)
#define Timer1_Input_Capture_Rising_Edge()                          Set_Bit(TCCR1B, ICES1)

/******************* Section 4 : User Defined Data Types & Variables Declarations *******************/

typedef void (* Timer1_Handler)(void);

typedef enum{
    Timer1_Module_Disable = (uint8)0x00,
    Timer1_Prescaler_DIV_1,
    Timer1_Prescaler_DIV_8,
    Timer1_Prescaler_DIV_64,
    Timer1_Prescaler_DIV_256,
    Timer1_Prescaler_DIV_1024,
    Timer1_Counter_Falling_Edge,
    Timer1_Counter_Rising_Edge
}Timer1_Prescaler_Select;

typedef enum{
    Timer1_Normal_Mode = (uint8)0x00,
    Timer1_Counter_CTC_OCR_Mode,
    Timer1_Counter_CTC_ICR_Mode
}Timer1_Mode;

typedef enum{
    Timer1_Timer = (uint8)0x00,
    Timer1_Counter
}Timer1_Operation;

typedef enum{
    Timer1_Counter_Mode_Channel_OC1A = (uint8)0x00,
    Timer1_Counter_Mode_Channel_OC1B
}Timer1_Channel_Select;

typedef enum{
    Timer1_COM_Mode_Disable_OC1X = (uint8)0x00,
    Timer1_COM_Mode_Toggle_OC1X,
    Timer1_COM_Mode_Clear_OC1X,
    Timer1_COM_Mode_Set_OC1X
}Timer1_COM_Mode;

typedef enum{
    Timer1_Capture_Falling_Edge = (uint8)0x00,
    Timer1_Capture_Rising_Edge
}Timer1_Capture_Edge;

typedef enum{
    Timer1_Noise_Cancellation_Disable = (uint8)0x00,
    Timer1_Noise_Cancellation_Enable
}Timer1_Noise_Cancellation;

typedef struct{
#if ((Interrupt_Feature_Enable) == (Timer1_Interrupt_Feature))
    Timer1_Handler               timer1_interrupt;
#endif
    uint16                        preloaded_value;
    uint16                                ctc_max;
    Timer1_Prescaler_Select             prescaler;
    Timer1_Mode                              mode;
    Timer1_Operation                    operation;
    Timer1_Channel_Select                 channel;
    Timer1_COM_Mode                      com_mode;
    Timer1_Capture_Edge              capture_edge;
    Timer1_Noise_Cancellation               noise;
}Timer1_CFG;

/******************* Section 5 : Software Interfaces Declarations (APIs) *******************/

Std_ReturnType Timer1_INIT(const Timer1_CFG *timer1);
Std_ReturnType Timer1_DEINIT(const Timer1_CFG *timer1);
Std_ReturnType Timer1_Write_Data(const Timer1_CFG *timer1, uint16 Data);
Std_ReturnType Timer1_Read_Data(const Timer1_CFG *timer1, uint16 *Data);
Std_ReturnType Timer1_Write_CTC_Max(const Timer1_CFG *timer1, uint16 ctc_max);
Std_ReturnType Timer1_Read_CTC_Max(const Timer1_CFG *timer1, uint16 *ctc_max);
Std_ReturnType Timer1_Take_Capture(const Timer1_CFG *timer1, uint16 *capture);



#endif	/* HAL_TIMER1_H */

