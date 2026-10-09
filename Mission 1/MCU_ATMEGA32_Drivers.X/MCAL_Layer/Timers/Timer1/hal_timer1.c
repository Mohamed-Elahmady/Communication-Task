/* 
 * File:   hal_timer1.c
 * Author: mohammedel-ahmady
 *
 * Created on August 25, 2025, 10:43 AM
 */

/******************* Section 0 : Includes *******************/

#include "hal_timer1.h"

/******************* Section 1 :  Variables Definitions *******************/

static uint16 timer1_preloaded_value = 0;

/******************* Section 2 :  Helper Functions Declarations *******************/

#if ((Interrupt_Feature_Enable) == (Timer1_Interrupt_Feature))
static Timer1_Handler Timer1_Handler_Function = NULL;
#endif

static Std_ReturnType Timer1_Select_COM_Output_Channel(const Timer1_CFG *timer1);
static Std_ReturnType Timer1_Set_Mode(const Timer1_CFG *timer1);
static Std_ReturnType Timer1_Set_Prescaler(const Timer1_CFG *timer1);
static Std_ReturnType Timer1_Set_Capture_Edge(const Timer1_CFG *timer1);

#if ((Interrupt_Feature_Enable) == (Timer1_Interrupt_Feature))

static Std_ReturnType Timer1_Configure_Interrupt(const Timer1_CFG *timer1);

#endif

/******************* Section 3 : Software Interfaces Definitions (APIs) *******************/

/**
 * @brief           : Initialize Timer1 module according to user configuration
 * @ref             : Timer1_CFG
 * @param timer1    : Pointer to Timer1 configuration structure
 * @return          : Status of the function
 *      (E_OK)      : The function executed successfully
 *      (E_NOT_OK)  : The function failed due to invalid input
 */

Std_ReturnType Timer1_INIT(const Timer1_CFG *timer1){
    Std_ReturnType Retval = E_OK;
    if(NULL == timer1){
        Retval = E_NOT_OK;
    }
    else{
        // 1. Timer 1 Set Mode
        Retval = Timer1_Set_Mode(timer1);
        // 2. Set Prescaler
        Retval = Timer1_Set_Prescaler(timer1);
        // 3. initialize timer register
        TCNT1 = timer1->preloaded_value;
        timer1_preloaded_value = timer1->preloaded_value;
        // 4. Select Capture Edge
        Retval = Timer1_Set_Capture_Edge(timer1);
        // 5. Configure interrupt if needed
#if ((Interrupt_Feature_Enable) == (Timer1_Interrupt_Feature))
        Retval = Timer1_Configure_Interrupt(timer1);
#endif
    }
    return Retval;
}

/**
 * @brief           : Deinitialize Timer1 module and disable its interrupts
 * @ref             : Timer1_CFG
 * @param timer1    : Pointer to Timer1 configuration structure
 * @return          : Status of the function
 *      (E_OK)      : Timer1 deinitialized successfully
 *      (E_NOT_OK)  : Invalid pointer or error occurred
 */

Std_ReturnType Timer1_DEINIT(const Timer1_CFG *timer1){
    Std_ReturnType Retval = E_OK;
    if(NULL == timer1){
        Retval = E_NOT_OK;
    }
    else{
        // 1. timer 1 disable interrupt & clear flag
        TCCR1B &= ~((1 << CS10) | (1 << CS11) | (1 << CS12));
#if ((Interrupt_Feature_Enable) == (Timer1_Interrupt_Feature))
        if(timer1->mode == Timer1_Normal_Mode){
            Internal_Interrupt_Timer1_Normal_Disable();
            Internal_Interrupt_Timer1_Normal_Clear_Flag();
        }
        else if(timer1->mode == Timer1_Counter_CTC_OCR_Mode && timer1->channel == Timer1_Counter_Mode_Channel_OC1A){
            Internal_Interrupt_Timer1_CTC_OC1A_Disable();
            Internal_Interrupt_Timer1_CTC_OC1A_Clear_Flag();
        }
        else if(timer1->mode == Timer1_Counter_CTC_OCR_Mode && timer1->channel == Timer1_Counter_Mode_Channel_OC1B){
            Internal_Interrupt_Timer1_CTC_OC1B_Disable();
            Internal_Interrupt_Timer1_CTC_OC1B_Clear_Flag();
        }
        else if(timer1->mode == Timer1_Counter_CTC_ICR_Mode){
            Internal_Interrupt_Timer1_CTC_ICR_Disable();
            Internal_Interrupt_Timer1_CTC_ICR_Clear_Flag();
        }
#endif
    }
    return Retval;
}

/**
 * @brief           : Write a value to Timer1 counter register (TCNT1)
 * @ref             : Timer1_CFG
 * @param timer1    : Pointer to Timer1 configuration structure
 * @param Data      : 16-bit value to load into TCNT1
 * @return          : Status of the function
 *      (E_OK)      : Value written successfully
 *      (E_NOT_OK)  : Invalid pointer provided
 */

Std_ReturnType Timer1_Write_Data(const Timer1_CFG *timer1, uint16 Data){
    Std_ReturnType Retval = E_OK;
    if(NULL == timer1){
        Retval = E_NOT_OK;
    }
    else{
        TCNT1 = Data;
    }
    return Retval;
}

/**
 * @brief           : Read the current value of Timer1 counter register (TCNT1)
 * @ref             : Timer1_CFG
 * @param timer1    : Pointer to Timer1 configuration structure
 * @param Data      : Pointer to store the read value
 * @return          : Status of the function
 *      (E_OK)      : Value read successfully
 *      (E_NOT_OK)  : Invalid pointer provided
 */

Std_ReturnType Timer1_Read_Data(const Timer1_CFG *timer1, uint16 *Data){
    Std_ReturnType Retval = E_OK;
    if(NULL == timer1 || NULL == Data){
        Retval = E_NOT_OK;
    }
    else{
        *Data = TCNT1;
    }
    return Retval;
}

/**
 * @brief           : Set the CTC max value for Timer1 (OCR1A or ICR1)
 * @ref             : Timer1_CFG
 * @param timer1    : Pointer to Timer1 configuration structure
 * @param ctc_max   : 16-bit CTC limit value
 * @return          : Status of the function
 *      (E_OK)      : CTC max value set successfully
 *      (E_NOT_OK)  : Invalid mode or pointer
 */

Std_ReturnType Timer1_Write_CTC_Max(const Timer1_CFG *timer1, uint16 ctc_max){
    Std_ReturnType Retval = E_OK;
    if(NULL == timer1){
        Retval = E_NOT_OK;
    }
    else{
        if(timer1->mode == Timer1_Counter_CTC_OCR_Mode){
            OCR1A = ctc_max;
        }
        else if(timer1->mode == Timer1_Counter_CTC_ICR_Mode){
            ICR1 = ctc_max;
        }
        else{
            Retval = E_NOT_OK;
        }
    }
    return Retval;
}

/**
 * @brief           : Read the CTC max value for Timer1 (OCR1A or ICR1)
 * @ref             : Timer1_CFG
 * @param timer1    : Pointer to Timer1 configuration structure
 * @param ctc_max   : Pointer to store the CTC max value
 * @return          : Status of the function
 *      (E_OK)      : Value read successfully
 *      (E_NOT_OK)  : Invalid mode or pointer
 */

Std_ReturnType Timer1_Read_CTC_Max(const Timer1_CFG *timer1, uint16 *ctc_max){
    Std_ReturnType Retval = E_OK;
    if(NULL == timer1 || NULL == ctc_max){
        Retval = E_NOT_OK;
    }
    else{
        if(timer1->mode == Timer1_Counter_CTC_OCR_Mode){
            *ctc_max = OCR1A;
        }
        else if(timer1->mode == Timer1_Counter_CTC_ICR_Mode){
            *ctc_max = ICR1;
        }
        else{
            Retval = E_NOT_OK;
        }
    }
    return Retval;
}

/**
 * @brief           : Capture the input value of Timer1 (ICR1)
 * @ref             : Timer1_CFG
 * @param timer1    : Pointer to Timer1 configuration structure
 * @param capture   : Pointer to store captured value
 * @return          : Status of the function
 *      (E_OK)      : Capture successful
 *      (E_NOT_OK)  : Invalid pointer or unsupported mode
 */

Std_ReturnType Timer1_Take_Capture(const Timer1_CFG *timer1, uint16 *capture){
    Std_ReturnType Retval = E_OK;
    if(NULL == timer1 || NULL == capture){
        Retval = E_NOT_OK;
    }
    else{
        if(timer1->mode == Timer1_Normal_Mode || timer1->mode == Timer1_Counter_CTC_OCR_Mode){
            *capture = ICR1;
        }
        else if(timer1->mode == Timer1_Counter_CTC_ICR_Mode){/* Nothing */}
        else{
            Retval = E_NOT_OK;
        }
    }
    return Retval;
}

#if ((Interrupt_Feature_Enable) == (Timer1_Interrupt_Feature))

/**
 * @brief           : Timer1 Overflow Interrupt Service Routine (Normal Mode)
 * @ref             : Timer1_CFG
 * @param void
 * @return          : void
 * @note            : Clears the overflow flag, reloads preloaded value, 
 *                    and calls the user-defined callback function if set.
 */

void TMR1_ISR(void){
    // 1. Clear Timer1 flag
    Internal_Interrupt_Timer1_Normal_Clear_Flag();
    // 2. code
    TCNT1 = timer1_preloaded_value;
    // 3. call back function
    if(Timer1_Handler_Function){
        Timer1_Handler_Function();
    }
}

/**
 * @brief           : Timer1 Compare Match A Interrupt Service Routine (CTC OCR Mode, OC1A)
 * @ref             : Timer1_CFG
 * @param void
 * @return          : void
 * @note            : Clears the OCA flag and calls the user-defined callback function if set.
 */

void CTC_OCRA_ISR(void){
    // 1. Clear OCA flag
    Internal_Interrupt_Timer1_CTC_OC1A_Clear_Flag();
    // 2. code

    // 3. call back function
    if(Timer1_Handler_Function){
        Timer1_Handler_Function();
    }
}

/**
 * @brief           : Timer1 Compare Match B Interrupt Service Routine (CTC OCR Mode, OC1B)
 * @ref             : Timer1_CFG
 * @param void
 * @return          : void
 * @note            : Clears the OCB flag and calls the user-defined callback function if set.
 */

void CTC_OCRB_ISR(void){
    // 1. Clear OCB flag
    Internal_Interrupt_Timer1_CTC_OC1B_Clear_Flag();
    // 2. code

    // 3. call back function
    if(Timer1_Handler_Function){
        Timer1_Handler_Function();
    }
}

/**
 * @brief           : Timer1 Input Capture Interrupt Service Routine (CTC ICR Mode)
 * @ref             : Timer1_CFG
 * @param void
 * @return          : void
 * @note            : Clears the ICR flag and calls the user-defined callback function if set.
 */

void CTC_ICR_ISR(void){
    // 1. Clear ICR flag
    Internal_Interrupt_Timer1_CTC_ICR_Clear_Flag();
    // 2. code

    // 3. call back function
    if(Timer1_Handler_Function){
        Timer1_Handler_Function();
    }
}

#endif

/******************* Section 4:  Helper Functions Definitions *******************/

/**
 * @brief           : Configure Timer1 Compare Output Mode (OC1A/OC1B) and initialize GPIO pin
 * @ref             : Timer1_CFG
 * @param timer1    : Pointer to Timer1 configuration structure
 * @return          : Status of the function
 *      (E_OK)      : COM output configured successfully
 *      (E_NOT_OK)  : Invalid pointer or invalid channel selection
 */

static Std_ReturnType Timer1_Select_COM_Output_Channel(const Timer1_CFG *timer1){
    Std_ReturnType Retval = E_OK;
    if(NULL == timer1){
        Retval = E_NOT_OK;
    }
    else{
        switch(timer1->channel){
            case Timer1_Counter_Mode_Channel_OC1A:{
                GPIO_Pin_CFG OC1A_Pin = {.Port = GPIO_PortD, .Pin = GPIO_Pin4, .Direction = GPIO_Output, .Logic = GPIO_Low};
                Retval = GPIO_Pin_INIT(&OC1A_Pin);
                if(timer1->com_mode == Timer1_COM_Mode_Toggle_OC1X){
                    Timer1_COM1A0_OC1A_Toggle();
                    Timer1_COM1A1_OC1A_Toggle();
                }
                else if(timer1->com_mode == Timer1_COM_Mode_Clear_OC1X){
                    Timer1_COM1A0_OC1A_Clear();
                    Timer1_COM1A1_OC1A_Clear();
                }
                else if(timer1->com_mode == Timer1_COM_Mode_Set_OC1X){
                    Timer1_COM1A0_OC1A_Set();
                    Timer1_COM1A1_OC1A_Set();
                }
                else{
                Timer1_COM1A0_OC1A_Disable();
                Timer1_COM1A1_OC1A_Disable();
                }
                break;
            }
            case Timer1_Counter_Mode_Channel_OC1B:{
                GPIO_Pin_CFG OC1B_Pin = {.Port = GPIO_PortD, .Pin = GPIO_Pin5, .Direction = GPIO_Output, .Logic = GPIO_Low};
                Retval = GPIO_Pin_INIT(&OC1B_Pin);
                if(timer1->com_mode == Timer1_COM_Mode_Toggle_OC1X){
                    Timer1_COM1B0_OC1B_Toggle();
                    Timer1_COM1B1_OC1B_Toggle();
                }
                else if(timer1->com_mode == Timer1_COM_Mode_Clear_OC1X){
                    Timer1_COM1B0_OC1B_Clear();
                    Timer1_COM1B1_OC1B_Clear();
                }
                else if(timer1->com_mode == Timer1_COM_Mode_Set_OC1X){
                    Timer1_COM1B0_OC1B_Set();
                    Timer1_COM1B1_OC1B_Set();
                }
                else{
                Timer1_COM1B0_OC1B_Disable();
                Timer1_COM1B1_OC1B_Disable();
                }
                break;
            }
            default :
                Retval = E_NOT_OK;
                break;
        }
    }
    return Retval;
}

/**
 * @brief           : Set the waveform generation mode of Timer1 (Normal / CTC OCR / CTC ICR)
 * @ref             : Timer1_CFG
 * @param timer1    : Pointer to Timer1 configuration structure
 * @return          : Status of the function
 *      (E_OK)      : Mode set successfully
 *      (E_NOT_OK)  : Invalid pointer or invalid mode
 */

static Std_ReturnType Timer1_Set_Mode(const Timer1_CFG *timer1){
    Std_ReturnType Retval = E_OK;
    if(NULL == timer1){
        Retval = E_NOT_OK;
    }
    else{
        switch(timer1->mode){
            case Timer1_Normal_Mode:
                Timer1_Waveform_Generation10_Normal_Mode();
                Timer1_Waveform_Generation11_Normal_Mode();
                Timer1_Waveform_Generation12_Normal_Mode();
                Timer1_Waveform_Generation13_Normal_Mode();
                
                if(timer1->operation == Timer1_Timer){/* Nothing */}
                else if(timer1->operation == Timer1_Counter){
                    GPIO_Pin_CFG T1_pin = {.Port = GPIO_PortB, .Pin = GPIO_Pin1, .Direction = GPIO_Input};
                    Retval = GPIO_Pin_Direction_INIT(&T1_pin);
                }
                break;
            case Timer1_Counter_CTC_OCR_Mode:
                Retval = Timer1_Select_COM_Output_Channel(timer1);
                OCR1A = timer1->ctc_max;
                Timer1_Waveform_Generation10_CTC_OCR_Mode();
                Timer1_Waveform_Generation11_CTC_OCR_Mode();
                Timer1_Waveform_Generation12_CTC_OCR_Mode();
                Timer1_Waveform_Generation13_CTC_OCR_Mode();
                
                if(timer1->operation == Timer1_Timer){/* Nothing */}
                else if(timer1->operation == Timer1_Counter){
                    GPIO_Pin_CFG T1_pin = {.Port = GPIO_PortB, .Pin = GPIO_Pin1, .Direction = GPIO_Input};
                    Retval = GPIO_Pin_Direction_INIT(&T1_pin);
                }
                break;
            case Timer1_Counter_CTC_ICR_Mode:
                Retval = Timer1_Select_COM_Output_Channel(timer1);
                ICR1 = timer1->ctc_max;
                Timer1_Waveform_Generation10_CTC_ICR_Mode();
                Timer1_Waveform_Generation11_CTC_ICR_Mode();
                Timer1_Waveform_Generation12_CTC_ICR_Mode();
                Timer1_Waveform_Generation13_CTC_ICR_Mode();
                
                if(timer1->operation == Timer1_Timer){/* Nothing */}
                else if(timer1->operation == Timer1_Counter){
                    GPIO_Pin_CFG ICP1_pin = {.Port = GPIO_PortD, .Pin = GPIO_Pin6, .Direction = GPIO_Input};
                    Retval = GPIO_Pin_Direction_INIT(&ICP1_pin);
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
 * @brief           : Set the prescaler or external counter edge for Timer1
 * @ref             : Timer1_CFG
 * @param timer1    : Pointer to Timer1 configuration structure
 * @return          : Status of the function
 *      (E_OK)      : Prescaler configured successfully
 *      (E_NOT_OK)  : Invalid pointer or invalid prescaler selection
 */

static Std_ReturnType Timer1_Set_Prescaler(const Timer1_CFG *timer1){
    Std_ReturnType Retval = E_OK;
    if(NULL == timer1){
        Retval = E_NOT_OK;
    }
    else{
        switch (timer1->operation){
            case Timer1_Timer:
                if(timer1->prescaler >= Timer1_Module_Disable && timer1->prescaler <= Timer1_Prescaler_DIV_1024){
                    TCCR1B = (uint8)((TCCR1B & 0xF8) | (timer1->prescaler & 0x07));
                }
                else{/* Nothing */}
                break;
            case Timer1_Counter:
                if(timer1->prescaler >= Timer1_Counter_Falling_Edge && timer1->prescaler <= Timer1_Counter_Rising_Edge){
                    TCCR1B = (uint8)((TCCR1B & 0xF8) | (timer1->prescaler & 0x07));
                }
                else{/* Nothing */}
                break;
            default :
                Retval = E_NOT_OK;
                break;
        }
    }
    return Retval;
}

/**
 * @brief           : Configure input capture edge and noise cancellation for Timer1
 * @ref             : Timer1_CFG
 * @param timer1    : Pointer to Timer1 configuration structure
 * @return          : Status of the function
 *      (E_OK)      : Capture edge and noise settings applied successfully
 *      (E_NOT_OK)  : Invalid pointer or invalid configuration
 */

static Std_ReturnType Timer1_Set_Capture_Edge(const Timer1_CFG *timer1){
    Std_ReturnType Retval = E_OK;
    if(NULL == timer1){
        Retval = E_NOT_OK;
    }
    else{
        // 1. Configure Noise Cancelation
        if(timer1->noise == Timer1_Noise_Cancellation_Enable){
            Timer1_Noise_Cancelation_Enable();
        }
        else if(timer1->noise == Timer1_Noise_Cancellation_Disable){
            Timer1_Noise_Cancelation_Disable();
        }
        else{
            Retval = E_NOT_OK;
        }
        // 1. Configure Capture Edge
        if(timer1->capture_edge == Timer1_Capture_Rising_Edge){
            Timer1_Input_Capture_Rising_Edge();
        }
        else if(timer1->capture_edge == Timer1_Capture_Falling_Edge){
            Timer1_Input_Capture_Falling_Edge();
        }
        else{
            Retval = E_NOT_OK;
        }
    }
    return Retval;
}

#if ((Interrupt_Feature_Enable) == (Timer1_Interrupt_Feature))

/**
 * @brief           : Configure Timer1 interrupts based on mode and channel selection
 * @ref             : Timer1_CFG
 * @param timer1    : Pointer to Timer1 configuration structure
 * @return          : Status of the function
 *      (E_OK)      : Interrupt configured successfully
 *      (E_NOT_OK)  : Invalid pointer or invalid mode/channel
 */

static Std_ReturnType Timer1_Configure_Interrupt(const Timer1_CFG *timer1){
    Std_ReturnType Retval = E_OK;
    if(NULL == timer1){
        Retval = E_NOT_OK;
    }
    else{
        // 1. enable timer 1 
        if(timer1->mode == Timer1_Normal_Mode){
            Internal_Interrupt_Timer1_Normal_Enable();
            Internal_Interrupt_Timer1_Normal_Clear_Flag();
        }
        else if(timer1->mode == Timer1_Counter_CTC_OCR_Mode && timer1->channel == Timer1_Counter_Mode_Channel_OC1A){
            Internal_Interrupt_Timer1_CTC_OC1A_Enable();
            Internal_Interrupt_Timer1_CTC_OC1A_Clear_Flag();
        }
        else if(timer1->mode == Timer1_Counter_CTC_OCR_Mode && timer1->channel == Timer1_Counter_Mode_Channel_OC1B){
            Internal_Interrupt_Timer1_CTC_OC1B_Enable();
            Internal_Interrupt_Timer1_CTC_OC1B_Clear_Flag();
        }
        else if(timer1->mode == Timer1_Counter_CTC_ICR_Mode){
            Internal_Interrupt_Timer1_CTC_ICR_Enable();
            Internal_Interrupt_Timer1_CTC_ICR_Clear_Flag();
        }
        // 2. Enable Global Interrupt
        GLobal_Interrupt_Enable_SREG();
        // 3. Callback Function
        Timer1_Handler_Function = timer1->timer1_interrupt;
    }
    return Retval;
}

#endif