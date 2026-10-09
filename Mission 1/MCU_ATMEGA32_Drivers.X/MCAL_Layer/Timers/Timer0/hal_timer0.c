/* 
 * File:   hal_timer0.c
 * Author: mohammedel-ahmady
 *
 * Created on August 20, 2025, 6:52 AM
 */

/******************* Section 0 : Includes *******************/

#include "hal_timer0.h"

/******************* Section 1 :  Variables Definitions *******************/

static uint8 preloaded_value = 0;

/******************* Section 2 :  Helper Functions Declarations *******************/

#if ((Interrupt_Feature_Enable) == (Timer0_Interrupt_Feature))

static Timer0_Handler Timer0_Handler_Function = NULL;

#endif

static Std_ReturnType Timer0_Set_Mode(const Timer0_CFG *timer0);
static Std_ReturnType Timer0_Set_Prescaler(const Timer0_CFG *timer0);
static void Timer0_Set_Force_Output(void);

#if ((Interrupt_Feature_Enable) == (Timer0_Interrupt_Feature))

static Std_ReturnType Timer0_Configure_Interrupt(const Timer0_CFG *timer0);

#endif

/******************* Section 3 : Software Interfaces Definitions (APIs) *******************/

/**
 * @brief           : Initialize Timer0 peripheral with the given configuration
 * @param timer0    : Pointer to Timer0 configuration structure
 * @ref             : Timer0_CFG
 * @return          : Status of the function
 *      (E_OK)      : The function done Successfully
 *      (E_NOT_OK)  : The function has issue (NULL pointer or invalid config)
 */

Std_ReturnType Timer0_INIT(const Timer0_CFG *timer0) {
    Std_ReturnType Retval = E_OK;
    if (NULL == timer0) {
        Retval = E_NOT_OK;
    } else {
        // 1. Set Timer 0 Mode
        Retval = Timer0_Set_Mode(timer0);
        // 2. Initalize Timer0 Register Data
        TCNT0 = timer0->timer0_preloaded_value;
        preloaded_value = timer0->timer0_preloaded_value;
        // 3. Configure Timer0 Interrupt if needed
#if ((Interrupt_Feature_Enable) == (Timer0_Interrupt_Feature))
        Retval = Timer0_Configure_Interrupt(timer0);
#endif
        // 4. Set Timer 0 Prescalar
        Retval = Timer0_Set_Prescaler(timer0);
    }
    return Retval;
}

/**
 * @brief           : De-initialize Timer0 (disable clock and interrupts)
 * @param timer0    : Pointer to Timer0 configuration structure
 * @ref             : Timer0_CFG
 * @return          : Status of the function
 *      (E_OK)      : The function done Successfully
 *      (E_NOT_OK)  : The function has issue (NULL pointer or invalid config)
 */

Std_ReturnType Timer0_DEINIT(const Timer0_CFG *timer0) {
    Std_ReturnType Retval = E_OK;
    if (NULL == timer0) {
        Retval = E_NOT_OK;
    } else {
        // 1. Disable Timer 0 Set CS<00:02> = 000 => no clock source
        TCCR0 &= ~((1 << CS00) | (1 << CS01) | (1 << CS02));

#if ((Interrupt_Feature_Enable) == (Timer0_Interrupt_Feature))
        switch (timer0->mode) {
            case Timer0_Normal_Mode:
                // 2. Disable Timer0 Normal Interrupt
                Internal_Interrupt_Timer0_Normal_Disable();
                // 3. Clear Timer0 Normal Flag
                Internal_Interrupt_Timer0_Normal_Clear_Flag();
                break;
            case Timer0_CTC_Mode:
                // 2. Disable Timer0 CTC Interrupt
                Internal_Interrupt_Timer0_CTC_Disable();
                // 3. Clear Timer0 CTC Flag
                Internal_Interrupt_Timer0_CTC_Clear_Flag();
                break;
            default:
                Retval = E_NOT_OK;
                break;
        }
#endif
    }
    return Retval;
}

/**
 * @brief           : Write a preload value into Timer0 counter register (TCNT0)
 * @param timer0    : Pointer to Timer0 configuration structure
 * @param Data      : 8-bit value to be written in TCNT0
 * @ref             : Timer0_CFG
 * @return          : Status of the function
 *      (E_OK)      : The function done Successfully
 *      (E_NOT_OK)  : The function has issue (NULL pointer)
 */

Std_ReturnType Timer0_Write_Data(const Timer0_CFG *timer0, uint8 Data) {
    Std_ReturnType Retval = E_OK;
    if (NULL == timer0) {
        Retval = E_NOT_OK;
    } else {
        TCNT0 = Data;
    }
    return Retval;
}

/**
 * @brief           : Read the current value of Timer0 counter register (TCNT0)
 * @param timer0    : Pointer to Timer0 configuration structure
 * @param Data      : Pointer to variable where the counter value will be stored
 * @ref             : Timer0_CFG
 * @return          : Status of the function
 *      (E_OK)      : The function done Successfully
 *      (E_NOT_OK)  : The function has issue (NULL pointer)
 */

Std_ReturnType Timer0_Read_Data(const Timer0_CFG *timer0, uint8 *Data) {
    Std_ReturnType Retval = E_OK;
    if (NULL == timer0 || NULL == Data) {
        Retval = E_NOT_OK;
    } else {
        *Data = TCNT0;
    }
    return Retval;
}

/**
 * @brief           : Write the maximum compare value into CTC register (OCR0)
 * @param timer0    : Pointer to Timer0 configuration structure
 * @param ctc_max   : 8-bit compare match value
 * @ref             : Timer0_CFG
 * @return          : Status of the function
 *      (E_OK)      : The function done Successfully
 *      (E_NOT_OK)  : The function has issue (NULL pointer)
 */

Std_ReturnType Timer0_Write_CTC_Max(const Timer0_CFG *timer0, uint8 ctc_max) {
    Std_ReturnType Retval = E_OK;
    if (NULL == timer0) {
        Retval = E_NOT_OK;
    } else {
        OCR0 = ctc_max;
    }
    return Retval;
}

/**
 * @brief           : Read the maximum compare value from CTC register (OCR0)
 * @param timer0    : Pointer to Timer0 configuration structure
 * @param ctc_max   : Pointer to variable where OCR0 value will be stored
 * @ref             : Timer0_CFG
 * @return          : Status of the function
 *      (E_OK)      : The function done Successfully
 *      (E_NOT_OK)  : The function has issue (NULL pointer)
 */

Std_ReturnType Timer0_Read_CTC_Max(const Timer0_CFG *timer0, uint8 *ctc_max) {
    Std_ReturnType Retval = E_OK;
    if (NULL == timer0 || NULL == ctc_max) {
        Retval = E_NOT_OK;
    } else {
        *ctc_max = OCR0;
    }
    return Retval;
}

#if ((Interrupt_Feature_Enable) == (Timer0_Interrupt_Feature))

/**
 * @brief           : Timer0 Normal Mode ISR (Overflow Interrupt Service Routine)
 * @note            : Clears interrupt flag, reloads preload value, and calls user callback
 * @param void      : None
 * @return          : void
 */

void TMR0_ISR(void) {
    // 1. Clear Timer0 flag
    Internal_Interrupt_Timer0_Normal_Clear_Flag();
    // 2. Code
    TCNT0 = preloaded_value;
    // 3. callback function
    if (Timer0_Handler_Function) {
        Timer0_Handler_Function();
    }
}

/**
 * @brief           : Timer0 CTC Mode ISR (Compare Match Interrupt Service Routine)
 * @note            : Clears interrupt flag and calls user callback
 * @param void      : None
 * @return          : void
 */

void CTC0_ISR(void) {
    // 1. Clear Timer0 flag
    Internal_Interrupt_Timer0_CTC_Clear_Flag();
    // 2. Code

    // 3. callback function
    if (Timer0_Handler_Function) {
        Timer0_Handler_Function();
    }
}

#endif

/******************* Section 4:  Helper Functions Definitions *******************/

/**
 * @brief           : Configure Timer0 working mode (Normal/CTC) and compare output behavior
 * @param timer0    : Pointer to Timer0 configuration structure
 * @ref             : Timer0_CFG
 * @return          : Status of the function
 *      (E_OK)      : The function done Successfully
 *      (E_NOT_OK)  : The function has issue (NULL pointer or invalid mode)
 */

static Std_ReturnType Timer0_Set_Mode(const Timer0_CFG *timer0) {
    Std_ReturnType Retval = E_OK;
    if (NULL == timer0) {
        Retval = E_NOT_OK;
    } 
    else {
        switch (timer0->mode) {
            case Timer0_Normal_Mode:
                Timer0_Waveform_Mode00_Normal();
                Timer0_Waveform_Mode01_Normal();

                if (timer0->operation == Timer0_Timer_Operation) {
                    /* Nothing */
                }
                else if (timer0->operation == Timer0_Counter_Operation) {
                    GPIO_Pin_CFG T0_Pin = {.Port = GPIO_PortB, .Pin = GPIO_Pin0, .Direction = GPIO_Input};
                    Retval = GPIO_Pin_Direction_INIT(&T0_Pin);
                }
                else {
                    Retval = E_NOT_OK;
                }

                break;
            case Timer0_CTC_Mode:
                Timer0_Waveform_Mode00_CTC();
                Timer0_Waveform_Mode01_CTC();

                OCR0 = timer0->timer0_ctc_max;

                if (timer0->operation == Timer0_Timer_Operation) {
                    /* Nothing */
                }
                else if (timer0->operation == Timer0_Counter_Operation) {
                    GPIO_Pin_CFG T0_Pin = {.Port = GPIO_PortB, .Pin = GPIO_Pin0, .Direction = GPIO_Input};
                    Retval = GPIO_Pin_Direction_INIT(&T0_Pin);
                }
                else {
                    Retval = E_NOT_OK;
                }
                break;
            default:
                Retval = E_NOT_OK;
                break;
        }
        
        if (timer0->compare_mode == Timer0_COM_Mode_Toggle_OC0) {
            Timer0_Set_Force_Output();

            GPIO_Pin_CFG OC0_Pin = {.Port = GPIO_PortB, .Pin = GPIO_Pin3, .Direction = GPIO_Output, .Logic = GPIO_Low};
            Retval = GPIO_Pin_INIT(&OC0_Pin);
            
            Timer0_COM00_OC0_Toggle();
            Timer0_COM01_OC0_Toggle();
        }
        else if (timer0->compare_mode == Timer0_COM_Mode_Clear_OC0) {
            Timer0_Set_Force_Output();
            
            GPIO_Pin_CFG OC0_Pin = {.Port = GPIO_PortB, .Pin = GPIO_Pin3, .Direction = GPIO_Output, .Logic = GPIO_Low};
            Retval = GPIO_Pin_INIT(&OC0_Pin);
            
            Timer0_COM00_OC0_Clear();
            Timer0_COM01_OC0_Clear();
        }
        else if (timer0->compare_mode == Timer0_COM_Mode_Set_OC0) {
            Timer0_Set_Force_Output();
            
            GPIO_Pin_CFG OC0_Pin = {.Port = GPIO_PortB, .Pin = GPIO_Pin3, .Direction = GPIO_Output, .Logic = GPIO_Low};
            Retval = GPIO_Pin_INIT(&OC0_Pin);
            
            Timer0_COM00_OC0_Set();
            Timer0_COM01_OC0_Set();
        }
        else {
            Timer0_COM00_OC0_Disable();
            Timer0_COM01_OC0_Disable();
        }
        
    }
    return Retval;
}

/**
 * @brief           : Configure Timer0 prescaler (clock source selection)
 * @param timer0    : Pointer to Timer0 configuration structure
 * @ref             : Timer0_CFG
 * @return          : Status of the function
 *      (E_OK)      : The function done Successfully
 *      (E_NOT_OK)  : The function has issue (NULL pointer or invalid prescaler)
 */

static Std_ReturnType Timer0_Set_Prescaler(const Timer0_CFG *timer0) {
    Std_ReturnType Retval = E_OK;
    if (NULL == timer0) {
        Retval = E_NOT_OK;
    } else {
        Timer0_Reset_Prescaler_Circuit();
        switch (timer0->operation) {
            case Timer0_Timer_Operation:
                if (timer0->prescaler >= Timer0_No_Prescaler && timer0->prescaler <= Timer0_Prescaler_DIV_1024) {
                    TCCR0 = (uint8) ((TCCR0 & 0xF8) | (timer0->prescaler & 0x07));
                } else {
                    /* Nothing */
                }
                break;
            case Timer0_Counter_Operation:
                if (timer0->prescaler == Timer0_Counter_Falling_Edge || timer0->prescaler == Timer0_Counter_Rising_Edge) {
                    TCCR0 = (uint8) ((TCCR0 & 0xF8) | (timer0->prescaler & 0x07));
                } else {
                    /* Nothing */
                }
                break;
            default:
                Retval = E_NOT_OK;
                break;
        }
    }
    return Retval;
}

/**
 * @brief           : Force an output compare action on OC0 pin (FOC0 bit)
 * @param void      : None
 * @return          : void
 */

static void Timer0_Set_Force_Output(void) {
    Timer0_Force_Output_Compare_Enable();
}

#if ((Interrupt_Feature_Enable) == (Timer0_Interrupt_Feature))

/**
 * @brief           : Configure Timer0 interrupt (Normal/CTC mode)
 * @param timer0    : Pointer to Timer0 configuration structure
 * @ref             : Timer0_CFG
 * @return          : Status of the function
 *      (E_OK)      : The function done Successfully
 *      (E_NOT_OK)  : The function has issue (NULL pointer or invalid mode)
 */

static Std_ReturnType Timer0_Configure_Interrupt(const Timer0_CFG *timer0) {
    Std_ReturnType Retval = E_OK;
    if (NULL == timer0) {
        Retval = E_NOT_OK;
    } else {
        switch (timer0->mode) {
            case Timer0_Normal_Mode:
                // 1. Clear Timer0 Normal Flag
                Internal_Interrupt_Timer0_Normal_Clear_Flag();
                // 2. Enable Timer0 Normal Interrupt
                Internal_Interrupt_Timer0_Normal_Enable();
                break;
            case Timer0_CTC_Mode:
                // 1. Clear Timer0 CTC Flag
                Internal_Interrupt_Timer0_CTC_Clear_Flag();
                // 2. Enable Timer0 CTC Interrupt
                Internal_Interrupt_Timer0_CTC_Enable();
                break;
            default:
                Retval = E_NOT_OK;
                break;
        }
        // 3. Global Interrupt Enable
        GLobal_Interrupt_Enable_SREG();
        // 4. Set call back function
        Timer0_Handler_Function = timer0->timer0_interrupt;
    }
    return Retval;
}

#endif