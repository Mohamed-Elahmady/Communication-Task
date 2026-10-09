/* 
 * File:   hal_timer2.c
 * Author: mohammedel-ahmady
 *
 * Created on August 28, 2025, 12:50 PM
 */

/******************* Section 0 : Includes *******************/

#include "hal_timer2.h"

/******************* Section 1 :  Variables Definitions *******************/

static uint8 timer2_preloaded_value = 0;

/******************* Section 2 :  Helper Functions Declarations *******************/

#if ((Interrupt_Feature_Enable) == (Timer2_Interrupt_Feature))

static Timer2_Handler Timer2_Handler_Function = NULL;

#endif

static Std_ReturnType Timer2_Set_Mode(const Timer2_CFG *timer2);
static Std_ReturnType Timer2_Set_Prescaler(const Timer2_CFG *timer2);
static void Timer2_Set_Force_Output(void);

#if ((Interrupt_Feature_Enable) == (Timer2_Interrupt_Feature))

static Std_ReturnType Timer2_Configure_Interrupt(const Timer2_CFG *timer2);

#endif

/******************* Section 3 : Software Interfaces Definitions (APIs) *******************/

/**
 * @brief           : Initialize Timer2 peripheral with specific configuration
 * @param timer2    : Pointer to timer2 configuration structure
 * @ref             : Timer2_CFG
 * @return          : Status of the function  
 *      (E_OK)      : Initialization done successfully  
 *      (E_NOT_OK)  : Invalid pointer or configuration error  
 */

Std_ReturnType Timer2_INIT(const Timer2_CFG *timer2){
    Std_ReturnType Retval = E_OK;
    if(NULL == timer2){
        Retval = E_NOT_OK;
    }
    else{
        // 1. Set Timer 2 Mode
        Retval = Timer2_Set_Mode(timer2);
        // 2. Initialize Timer 2 Value
        TCNT2 = timer2->preloaded_value;
        timer2_preloaded_value = timer2->preloaded_value;
        // 3. Configure Timer 2 Interrupt
        Retval = Timer2_Configure_Interrupt(timer2);
        // 4. Set Timer 2 Prescaler
        Retval = Timer2_Set_Prescaler(timer2);
    }
    return Retval;
}

/**
 * @brief           : De-initialize Timer2 peripheral (disable module & interrupts)
 * @param timer2    : Pointer to timer2 configuration structure
 * @ref             : Timer2_CFG
 * @return          : Status of the function  
 *      (E_OK)      : De-initialization done successfully  
 *      (E_NOT_OK)  : Invalid pointer or configuration error  
 */

Std_ReturnType Timer2_DEINIT(const Timer2_CFG *timer2){
    Std_ReturnType Retval = E_OK;
    if(NULL == timer2){
        Retval = E_NOT_OK;
    }
    else{
        // 1. Disable Timer 2 Module
        TCCR2 &= ~((1 << CS20) | (1 << CS21) | (1 << CS22));
        // 2. Disable Timer 2 Interrupt and Clear Flag
        switch(timer2->mode){
            case Timer2_Normal_Mode:
                Internal_Interrupt_Timer2_Normal_Disable();
                Internal_Interrupt_Timer2_Normal_Clear_Flag();
                break;
            case Timer2_CTC_Mode:
                Internal_Interrupt_Timer2_CTC_Disable();
                Internal_Interrupt_Timer2_CTC_Clear_Flag();                
                break;
            default :
                Retval = E_NOT_OK;
                break;
        }
    }
    return Retval;
}

/**
 * @brief           : Write a value directly into Timer2 counter register (TCNT2)
 * @param timer2    : Pointer to timer2 configuration structure
 * @param data      : The value to load into Timer2 counter
 * @return          : Status of the function  
 *      (E_OK)      : Data written successfully  
 *      (E_NOT_OK)  : Invalid pointer  
 */

Std_ReturnType Timer2_Write_Data(const Timer2_CFG *timer2, uint8 data){
    Std_ReturnType Retval = E_OK;
    if(NULL == timer2){
        Retval = E_NOT_OK;
    }
    else{
        TCNT2 = data;
    }
    return Retval;
}

/**
 * @brief           : Read the current value from Timer2 counter register (TCNT2)
 * @param timer2    : Pointer to timer2 configuration structure
 * @param data      : Pointer to store the current counter value
 * @return          : Status of the function  
 *      (E_OK)      : Data read successfully  
 *      (E_NOT_OK)  : Invalid pointer  
 */

Std_ReturnType Timer2_Read_Data(const Timer2_CFG *timer2, uint8 *data){
    Std_ReturnType Retval = E_OK;
    if(NULL == timer2 || NULL == data){
        Retval = E_NOT_OK;
    }
    else{
        *data = TCNT2;
    }
    return Retval;
}

/**
 * @brief           : Write the compare match value (OCR2) for Timer2 CTC mode
 * @param timer2    : Pointer to timer2 configuration structure
 * @param ctc_max   : The maximum compare value to load into OCR2
 * @return          : Status of the function  
 *      (E_OK)      : Value written successfully  
 *      (E_NOT_OK)  : Invalid pointer  
 */

Std_ReturnType Timer2_Write_CTC_Max(const Timer2_CFG *timer2, uint8 ctc_max){
    Std_ReturnType Retval = E_OK;
    if(NULL == timer2){
        Retval = E_NOT_OK;
    }
    else{
        OCR2 = ctc_max;
    }
    return Retval;
}


/**
 * @brief           : Read the compare match value (OCR2) of Timer2
 * @param timer2    : Pointer to timer2 configuration structure
 * @param ctc_max   : Pointer to store the compare match value
 * @return          : Status of the function  
 *      (E_OK)      : Value read successfully  
 *      (E_NOT_OK)  : Invalid pointer  
 */

Std_ReturnType Timer2_Read_CTC_Max(const Timer2_CFG *timer2, uint8 *ctc_max){
    Std_ReturnType Retval = E_OK;
    if(NULL == timer2 || NULL == ctc_max){
        Retval = E_NOT_OK;
    }
    else{
        *ctc_max = OCR2;
    }
    return Retval;
}

#if ((Interrupt_Feature_Enable) == (Timer2_Interrupt_Feature))

/**
 * @brief           : Timer2 Normal Mode ISR  
 * @note            : Clears flag, reloads pre-loaded value, and executes callback
 */

void TMR2_ISR(void){
    // 1. Clear Timer 2 Flag
    Internal_Interrupt_Timer2_Normal_Clear_Flag();
    // 2. Code
    TCNT2 = timer2_preloaded_value;
    // 3. Callback Function
    if(Timer2_Handler_Function){
        Timer2_Handler_Function();
    }
}

/**
 * @brief           : Timer2 CTC Mode ISR  
 * @note            : Clears flag and executes callback
 */

void CTC2_ISR(void){
    // 1. Clear Timer 2 Flag
    Internal_Interrupt_Timer2_CTC_Clear_Flag();
    // 2. Code
    // 3. Callback Function
    if(Timer2_Handler_Function){
        Timer2_Handler_Function();
    }
}

#endif

/******************* Section 4:  Helper Functions Definitions *******************/

/**
 * @brief           : This function sets the operating mode for Timer2 (Normal / CTC).
 * @param timer2    : Pointer to timer2 configuration structure.
 * @ref             : Timer2_CFG
 * @return          : Status of the function
 *      (E_OK)      : The function executed successfully
 *      (E_NOT_OK)  : The function failed due to NULL pointer or invalid mode
 */

static Std_ReturnType Timer2_Set_Mode(const Timer2_CFG *timer2){
    Std_ReturnType Retval = E_OK;
    if(NULL == timer2){
        Retval = E_NOT_OK;
    }
    else{
        switch(timer2->mode){
            case Timer2_Normal_Mode:
                Timer2_Waveform_Mode20_Normal();
                Timer2_Waveform_Mode21_Normal();
                break;
            case Timer2_CTC_Mode:
                Timer2_Waveform_Mode20_CTC();
                Timer2_Waveform_Mode21_CTC();
                
                OCR2 = timer2->ctc_max;
                
                break;
            default :
                Retval = E_NOT_OK;
                break;
        }
        if(timer2->com_mode == Timer2_COM_Mode_OC2_Toggle){
            Timer2_Set_Force_Output();
            
            GPIO_Pin_CFG OC2_pin = {.Port = GPIO_PortD, .Pin = GPIO_Pin7, .Direction = GPIO_Output, .Logic = GPIO_Low};
            Retval = GPIO_Pin_INIT(&OC2_pin);
            
            Timer2_COM20_OC2_Toggle();
            Timer2_COM21_OC2_Toggle();
        }
        else if(timer2->com_mode == Timer2_COM_Mode_OC2_Clear){
            Timer2_Set_Force_Output();
            
            GPIO_Pin_CFG OC2_pin = {.Port = GPIO_PortD, .Pin = GPIO_Pin7, .Direction = GPIO_Output, .Logic = GPIO_Low};
            Retval = GPIO_Pin_INIT(&OC2_pin);
            
            Timer2_COM20_OC2_Clear();
            Timer2_COM21_OC2_Clear();
        }
        else if(timer2->com_mode == Timer2_COM_Mode_OC2_Set){
            Timer2_Set_Force_Output();
            
            GPIO_Pin_CFG OC2_pin = {.Port = GPIO_PortD, .Pin = GPIO_Pin7, .Direction = GPIO_Output, .Logic = GPIO_Low};
            Retval = GPIO_Pin_INIT(&OC2_pin);
            
            Timer2_COM20_OC2_Set();
            Timer2_COM21_OC2_Set();
        }
        else{
            Timer2_COM20_OC2_Disable();
            Timer2_COM21_OC2_Disable();
        }
    }
    return Retval;
}

/**
 * @brief           : This function sets the prescaler value for Timer2.
 * @param timer2    : Pointer to timer2 configuration structure.
 * @ref             : Timer2_CFG
 * @return          : Status of the function
 *      (E_OK)      : Prescaler configured successfully
 *      (E_NOT_OK)  : Function failed due to NULL pointer
 */

static Std_ReturnType Timer2_Set_Prescaler(const Timer2_CFG *timer2){
    Std_ReturnType Retval = E_OK;
    if(NULL == timer2){
        Retval = E_NOT_OK;
    }
    else{
        TCCR2 = (uint8)((TCCR2 & 0xF8) | ((timer2->prescaler) & 0x07));
    }
    return Retval;
}

/**
 * @brief           : This function enables the Force Output Compare for Timer2.
 * @note            : It is required before configuring OC2 pin compare match modes.
 * @return          : None
 */

static void Timer2_Set_Force_Output(void){
    Timer2_Force_Compare_Output_Enable();
}

#if ((Interrupt_Feature_Enable) == (Timer2_Interrupt_Feature))

/**
 * @brief           : This function configures the interrupt settings for Timer2.
 * @param timer2    : Pointer to timer2 configuration structure.
 * @ref             : Timer2_CFG
 * @return          : Status of the function
 *      (E_OK)      : Interrupt configured successfully
 *      (E_NOT_OK)  : Function failed due to NULL pointer or invalid mode
 */

static Std_ReturnType Timer2_Configure_Interrupt(const Timer2_CFG *timer2){
    Std_ReturnType Retval = E_OK;
    if(NULL == timer2){
        Retval = E_NOT_OK;
    }
    else{
        switch(timer2->mode){
            case Timer2_Normal_Mode:
                Internal_Interrupt_Timer2_Normal_Enable();
                Internal_Interrupt_Timer2_Normal_Clear_Flag();
                break;
            case Timer2_CTC_Mode:
                Internal_Interrupt_Timer2_CTC_Enable();
                Internal_Interrupt_Timer2_CTC_Clear_Flag();                
                break;
            default :
                Retval = E_NOT_OK;
                break;
        }
        GLobal_Interrupt_Enable_SREG();
        Timer2_Handler_Function = timer2->timer2_interrupt;
    }
    return Retval;
}

#endif