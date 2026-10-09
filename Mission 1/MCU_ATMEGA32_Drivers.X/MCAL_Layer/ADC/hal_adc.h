/* 
 * File:   hal_adc.h
 * Author: mohammedel-ahmady
 *
 * Created on August 13, 2025, 3:29 AM
 */

/******************* Section 0 : File Guard *******************/

#ifndef HAL_ADC_H
#define	HAL_ADC_H

/******************* Section 1 : Includes *******************/

#include "hal_adc_cfg.h"
#include "../Interrupt/hal_int_interrupt.h"
#include "../GPIO/hal_gpio.h"
#include "../mcal_std_types.h"

/******************* Section 2 : Macros Declarations *******************/



/******************* Section 3 : Macros Functions Declarations *******************/

// For ADMUX Register

#define ADC_Result_Format_Right_B0_B9()                  Clear_Bit(ADMUX, ADLAR)
#define ADC_Result_Format_Left_B6_B15()                  Set_Bit(ADMUX, ADLAR)

// for Voltage Reference

#define ADC_AREF_Voltage_Reference_REFS0()               Clear_Bit(ADMUX, REFS0)
#define ADC_AREF_Voltage_Reference_REFS1()               Clear_Bit(ADMUX, REFS1)

#define ADC_AVCC_Voltage_Reference_REFS0()               Set_Bit(ADMUX, REFS0)
#define ADC_AVCC_Voltage_Reference_REFS1()               Clear_Bit(ADMUX, REFS1)

#define ADC_Internal_Voltage_Reference_REFS0()           Set_Bit(ADMUX, REFS0)
#define ADC_Internal_Voltage_Reference_REFS1()           Set_Bit(ADMUX, REFS1)

// For ADCSRA Register

// this bit automatically cleared by microcontroller after conversion
#define ADC_Start_Convert()                              Set_Bit(ADCSRA, ADSC)

#define ADC_Module_Disable()                             Clear_Bit(ADCSRA, ADEN)
#define ADC_Module_Enable()                              Set_Bit(ADCSRA, ADEN)

/******************* Section 4 : User Defined Data Types & Variables Declarations *******************/

typedef void (* ADC_Handler)(void);

typedef enum{
    ADC_Channel_ADC0 = (uint8)0x00,
    ADC_Channel_ADC1,
    ADC_Channel_ADC2,
    ADC_Channel_ADC3,
    ADC_Channel_ADC4,
    ADC_Channel_ADC5,
    ADC_Channel_ADC6,
    ADC_Channel_ADC7
}ADC_Channel_Select;
    
typedef enum{
    ADC_Result_Format_Right = (uint8)0x00,
    ADC_Result_Format_Left
}ADC_Result_Format;

typedef enum{
    ADC_AREF_Voltage_Reference = (uint8)0x00,
    ADC_AVCC_Voltage_Reference = (uint8)0x01,
    ADC_Internal_Voltage_Reference = (uint8)0x03
}ADC_Voltage_Reference;

typedef enum{
    ADC_Conversion_Clock_FOSC_DIV_2 = (uint8)0x01,
    ADC_Conversion_Clock_FOSC_DIV_4,
    ADC_Conversion_Clock_FOSC_DIV_8,
    ADC_Conversion_Clock_FOSC_DIV_16,
    ADC_Conversion_Clock_FOSC_DIV_32,
    ADC_Conversion_Clock_FOSC_DIV_64,
    ADC_Conversion_Clock_FOSC_DIV_128
}ADC_Conversion_Clock;

typedef enum{
    ADC_Conversion_Completed = (uint8)0x00,
    ADC_Conversion_In_Progress
}ADC_Conversion_Status;

typedef struct{
#if ((Interrupt_Feature_Enable) == (ADC_Interrupt_Feature))
    ADC_Handler  ADC_Interrupt;
#endif
    ADC_Channel_Select channel;
    ADC_Conversion_Clock clock;
    ADC_Voltage_Reference  ref;
    ADC_Result_Format   format;
}ADC_Config;

/******************* Section 5 : Software Interfaces Declarations (APIs) *******************/

Std_ReturnType ADC_INIT(const ADC_Config *adc);
Std_ReturnType ADC_DEINIT(const ADC_Config *adc);
Std_ReturnType ADC_Set_Channel(const ADC_Config *adc, ADC_Channel_Select channel);
Std_ReturnType ADC_Start_Conversion(const ADC_Config *adc);
Std_ReturnType ADC_Is_Conversion_Done(const ADC_Config *adc, ADC_Conversion_Status *state);
Std_ReturnType ADC_Get_Conversion_Result(const ADC_Config *adc, uint16 *result);
Std_ReturnType ADC_Get_Conversion_Blocking(const ADC_Config *adc, ADC_Channel_Select channel, uint16 *result);
Std_ReturnType ADC_Start_Conversion_Interrupt(const ADC_Config *adc, ADC_Channel_Select channel);

#endif	/* HAL_ADC_H */

