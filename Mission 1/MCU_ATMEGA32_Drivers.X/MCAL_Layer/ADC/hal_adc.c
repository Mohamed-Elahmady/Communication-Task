/* 
 * File:   hal_adc.c
 * Author: mohammedel-ahmady
 *
 * Created on August 13, 2025, 3:29 AM
 */

/******************* Section 0 : Includes *******************/

#include "hal_adc.h"

/******************* Section 1 :  Variables Definitions *******************/



/******************* Section 2 :  Helper Functions Declarations *******************/

#if ((Interrupt_Feature_Enable) == (ADC_Interrupt_Feature))
static ADC_Handler ADC_Interrupt_Handler = NULL;
#endif

static Std_ReturnType ADC_Set_Disable(const ADC_Config *adc);
static Std_ReturnType ADC_Set_Conversion_Clock(const ADC_Config *adc);
static Std_ReturnType ADC_Set_Voltage_Reference(const ADC_Config *adc);
static Std_ReturnType ADC_Set_Channel_Configuration(ADC_Channel_Select channel);
static Std_ReturnType ADC_Set_Result_Format(const ADC_Config *adc);
#if ((Interrupt_Feature_Enable) == (ADC_Interrupt_Feature))
static Std_ReturnType ADC_Configure_Interrupt(const ADC_Config *adc);
#endif
static Std_ReturnType ADC_Set_Enable(const ADC_Config *adc);

/******************* Section 3 : Software Interfaces Definitions (APIs) *******************/

#if ((ADC_Feature_Enable) == (ADC_Feature_CFG))

/**
 * @brief           : This function initializes the ADC module with the given configuration.
 * @param adc       : Pointer to ADC configuration structure.
 * @ref             : ADC_Config
 * @return          : Status of the function  
 *      (E_OK)      : The function done successfully  
 *      (E_NOT_OK)  : The function has an issue to perform the action  
 */

Std_ReturnType ADC_INIT(const ADC_Config *adc){
    Std_ReturnType Retval = E_OK;
    if(NULL == adc){
        Retval = E_NOT_OK;
    }
    else{
        // 1. Disable ADC
        Retval = ADC_Set_Disable(adc);
        // 2. Set conversion clock
        Retval = ADC_Set_Conversion_Clock(adc);
        // 3. Set voltage reference
        Retval = ADC_Set_Voltage_Reference(adc);
        // 4. select channel
        Retval = ADC_Set_Channel_Configuration(adc->channel);
        // 5. set result format
        Retval = ADC_Set_Result_Format(adc);
        // 6. configure Interrupt 
#if ((Interrupt_Feature_Enable) == (ADC_Interrupt_Feature))
        ADC_Configure_Interrupt(adc);
#endif
        // 6. Enable ADC
        Retval = ADC_Set_Enable(adc);
    }
    return Retval;
}

/**
 * @brief           : This function de-initializes the ADC module and disables interrupts if enabled.
 * @param adc       : Pointer to ADC configuration structure.
 * @ref             : ADC_Config
 * @return          : Status of the function  
 *      (E_OK)      : The function done successfully  
 *      (E_NOT_OK)  : The function has an issue to perform the action  
 */

Std_ReturnType ADC_DEINIT(const ADC_Config *adc){
    Std_ReturnType Retval = E_OK;
    if(NULL == adc){
        Retval = E_NOT_OK;
    }
    else{
        // 1. Disable ADC
        Retval = ADC_Set_Disable(adc);
#if ((Interrupt_Feature_Enable) == (ADC_Interrupt_Feature))        
        // 2. Disable ADC Interrupt Enable
        Internal_Interrupt_ADC_Disable();
        // 3. Disable ADC Interrupt Flag
        Internal_Interrupt_ADC_Clear_Flag();
#endif
    }
    return Retval;
}

/**
 * @brief           : This function sets the ADC channel to be used for conversion.
 * @param adc       : Pointer to ADC configuration structure.
 * @param channel   : ADC channel selection.
 * @ref             : ADC_Config, ADC_Channel_Select
 * @return          : Status of the function  
 *      (E_OK)      : The function done successfully  
 *      (E_NOT_OK)  : The function has an issue to perform the action  
 */

Std_ReturnType ADC_Set_Channel(const ADC_Config *adc, ADC_Channel_Select channel){
    Std_ReturnType Retval = E_OK;
    if(NULL == adc){
        Retval = E_NOT_OK;
    }
    else{
        Retval = ADC_Set_Channel_Configuration(channel);
    }
    return Retval;
}

/**
 * @brief           : This function starts an ADC conversion process.
 * @param adc       : Pointer to ADC configuration structure.
 * @ref             : ADC_Config
 * @return          : Status of the function  
 *      (E_OK)      : The function done successfully  
 *      (E_NOT_OK)  : The function has an issue to perform the action  
 */

Std_ReturnType ADC_Start_Conversion(const ADC_Config *adc){
    Std_ReturnType Retval = E_OK;
    if(NULL == adc){
        Retval = E_NOT_OK;
    }
    else{
        ADC_Start_Convert();
    }
    return Retval;
}

/**
 * @brief           : This function checks if the ADC conversion is completed.
 * @param adc       : Pointer to ADC configuration structure.
 * @param state     : Pointer to store the conversion status.
 * @ref             : ADC_Config, ADC_Conversion_Status
 * @return          : Status of the function  
 *      (E_OK)      : The function done successfully  
 *      (E_NOT_OK)  : The function has an issue to perform the action  
 */

Std_ReturnType ADC_Is_Conversion_Done(const ADC_Config *adc, ADC_Conversion_Status *state){
    Std_ReturnType Retval = E_OK;
    if(NULL == adc){
        Retval = E_NOT_OK;
    }
    else{
        if(!Read_Bit(ADCSRA, ADSC)){
            *state = ADC_Conversion_Completed;
        }
        else{
            *state = ADC_Conversion_In_Progress;
        }
    }
    return Retval;
}

/**
 * @brief           : This function retrieves the ADC conversion result.
 * @param adc       : Pointer to ADC configuration structure.
 * @param result    : Pointer to store the conversion result.
 * @ref             : ADC_Config
 * @return          : Status of the function  
 *      (E_OK)      : The function done successfully  
 *      (E_NOT_OK)  : The function has an issue to perform the action  
 */

Std_ReturnType ADC_Get_Conversion_Result(const ADC_Config *adc, uint16 *result){
    Std_ReturnType Retval = E_OK;
    if(NULL == adc || NULL == result){
        Retval = E_NOT_OK;
    } else {
        uint8 low = ADCL;
        uint8 high = ADCH;
        switch(adc->format){
            case ADC_Result_Format_Right:
                *result = ((uint16)high << 8) | low;
                break;
            case ADC_Result_Format_Left:
                *result = ((uint16)high << 2) | (low >> 6);
                break;
            default:
                Retval = E_NOT_OK;
                break;
        }
    }
    return Retval;
}

/**
 * @brief           : This function performs a blocking ADC conversion on a specific channel.
 * @param adc       : Pointer to ADC configuration structure.
 * @param channel   : ADC channel to be read.
 * @param result    : Pointer to store the conversion result.
 * @ref             : ADC_Config, ADC_Channel_Select
 * @return          : Status of the function  
 *      (E_OK)      : The function done successfully  
 *      (E_NOT_OK)  : The function has an issue to perform the action  
 */

Std_ReturnType ADC_Get_Conversion_Blocking(const ADC_Config *adc, ADC_Channel_Select channel, uint16 *result){
    Std_ReturnType Retval = E_OK;
    if(NULL == adc){
        Retval = E_NOT_OK;
    }
    else{
        // 1. set channel
        Retval = ADC_Set_Channel(adc, channel);
        // 2. start conversion
        ADC_Start_Conversion(adc);
        // 3. wait until conversion end
        while(Read_Bit(ADCSRA, ADSC));
        // 4. return conversion result
        Retval = ADC_Get_Conversion_Result(adc, result);
    }
    return Retval;
}

/**
 * @brief           : This function starts an ADC conversion using interrupts.
 * @param adc       : Pointer to ADC configuration structure.
 * @param channel   : ADC channel to be read.
 * @ref             : ADC_Config, ADC_Channel_Select
 * @return          : Status of the function  
 *      (E_OK)      : The function done successfully  
 *      (E_NOT_OK)  : The function has an issue to perform the action  
 */

Std_ReturnType ADC_Start_Conversion_Interrupt(const ADC_Config *adc, ADC_Channel_Select channel){
    Std_ReturnType Retval = E_OK;
    if(NULL == adc){
        Retval = E_NOT_OK;
    }
    else{
        // 1. set channel
        Retval = ADC_Set_Channel(adc, channel);
        // 2. start conversion
        Retval = ADC_Start_Conversion(adc);
        // 3. when the conversion end the flag will be set and the ADC ISR will happen
    }
    return Retval;
}

#if ((Interrupt_Feature_Enable) == (ADC_Interrupt_Feature))

/**
 * @brief           : ADC interrupt service routine. Clears the flag and calls the user callback function.
 * @param void      : None
 * @return          : None
 */

void ADC_ISR(void){
    // 1. Clear ADC Interrupt flag
    Internal_Interrupt_ADC_Clear_Flag();
    // 2. Code
    
    // 3. Call back function
    if(ADC_Interrupt_Handler){
        ADC_Interrupt_Handler();
    }
}

#endif

#endif

/******************* Section 4:  Helper Functions Definitions *******************/

/**
 * @brief           : This function disables the ADC module.
 * @param adc       : Pointer to ADC configuration structure.
 * @ref             : ADC_Config
 * @return          : Status of the function  
 *      (E_OK)      : The function done successfully  
 *      (E_NOT_OK)  : The function has an issue to perform the action  
 */

static Std_ReturnType ADC_Set_Disable(const ADC_Config *adc){
    Std_ReturnType Retval = E_OK;
    if(NULL == adc){
        Retval = E_NOT_OK;
    }
    else{
        ADC_Module_Disable();
    }
    return Retval;
}

/**
 * @brief           : This function sets the ADC conversion clock source.
 * @param adc       : Pointer to ADC configuration structure.
 * @ref             : ADC_Config
 * @return          : Status of the function  
 *      (E_OK)      : The function done successfully  
 *      (E_NOT_OK)  : The function has an issue to perform the action  
 */

static Std_ReturnType ADC_Set_Conversion_Clock(const ADC_Config *adc){
    Std_ReturnType Retval = E_OK;
    if(NULL == adc){
        Retval = E_NOT_OK;
    }
    else{
        ADCSRA = (ADCSRA & 0xF8) | (adc->clock & 0x07);
    }
    return Retval;
}

/**
 * @brief           : This function configures the ADC voltage reference.
 * @param adc       : Pointer to ADC configuration structure.
 * @ref             : ADC_Config, ADC_Voltage_Reference
 * @return          : Status of the function  
 *      (E_OK)      : The function done successfully  
 *      (E_NOT_OK)  : The function has an issue to perform the action  
 */

static Std_ReturnType ADC_Set_Voltage_Reference(const ADC_Config *adc){
    Std_ReturnType Retval = E_OK;
    if(NULL == adc){
        Retval = E_NOT_OK;
    }
    else{
        switch(adc->ref){
            case ADC_AREF_Voltage_Reference:
                ADC_AREF_Voltage_Reference_REFS0();
                ADC_AREF_Voltage_Reference_REFS1();
                break;
            case ADC_AVCC_Voltage_Reference:
                ADC_AVCC_Voltage_Reference_REFS0();
                ADC_AVCC_Voltage_Reference_REFS1();
                break;
            case ADC_Internal_Voltage_Reference:
                ADC_Internal_Voltage_Reference_REFS0();
                ADC_Internal_Voltage_Reference_REFS1();
                break;
            default :
                Retval = E_NOT_OK;
                break;
        }
    }
    return Retval;
}

/**
 * @brief           : This function selects and configures the ADC channel pin.
 * @param channel   : ADC channel selection.
 * @ref             : ADC_Channel_Select
 * @return          : Status of the function  
 *      (E_OK)      : The function done successfully  
 *      (E_NOT_OK)  : The function has an issue to perform the action  
 */

static Std_ReturnType ADC_Set_Channel_Configuration(ADC_Channel_Select channel){
    Std_ReturnType Retval = E_OK;
    if(channel > ADC_Channel_ADC7){
        Retval = E_NOT_OK;
    }
    else{
        ADMUX  = (ADMUX & 0xE0) | (channel & 0x1F);
        switch (channel){
            case ADC_Channel_ADC0:{
                GPIO_Pin_CFG pinA0 = {.Port = GPIO_PortA, .Pin = GPIO_Pin0, .Direction = GPIO_Input, .Logic = GPIO_Low};
                Retval = GPIO_Pin_INIT(&pinA0);
                break;
            }
            case ADC_Channel_ADC1:{
                GPIO_Pin_CFG pinA1 = {.Port = GPIO_PortA, .Pin = GPIO_Pin1, .Direction = GPIO_Input, .Logic = GPIO_Low};
                Retval = GPIO_Pin_INIT(&pinA1);
                break;
            }
            case ADC_Channel_ADC2:{
                GPIO_Pin_CFG pinA2 = {.Port = GPIO_PortA, .Pin = GPIO_Pin2, .Direction = GPIO_Input, .Logic = GPIO_Low};
                Retval = GPIO_Pin_INIT(&pinA2);
                break;
            }
            case ADC_Channel_ADC3:{
                GPIO_Pin_CFG pinA3 = {.Port = GPIO_PortA, .Pin = GPIO_Pin3, .Direction = GPIO_Input, .Logic = GPIO_Low};
                Retval = GPIO_Pin_INIT(&pinA3);
                break;
            }
            case ADC_Channel_ADC4:{
                GPIO_Pin_CFG pinA4 = {.Port = GPIO_PortA, .Pin = GPIO_Pin4, .Direction = GPIO_Input, .Logic = GPIO_Low};
                Retval = GPIO_Pin_INIT(&pinA4);
                break;
            }
            case ADC_Channel_ADC5:{
                GPIO_Pin_CFG pinA5 = {.Port = GPIO_PortA, .Pin = GPIO_Pin5, .Direction = GPIO_Input, .Logic = GPIO_Low};
                Retval = GPIO_Pin_INIT(&pinA5);
                break;
            }
            case ADC_Channel_ADC6:{
                GPIO_Pin_CFG pinA6 = {.Port = GPIO_PortA, .Pin = GPIO_Pin6, .Direction = GPIO_Input, .Logic = GPIO_Low};
                Retval = GPIO_Pin_INIT(&pinA6);
                break;
            }
            case ADC_Channel_ADC7:{
                GPIO_Pin_CFG pinA7 = {.Port = GPIO_PortA, .Pin = GPIO_Pin7, .Direction = GPIO_Input, .Logic = GPIO_Low};
                Retval = GPIO_Pin_INIT(&pinA7);
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
 * @brief           : This function sets the ADC result format (left or right alignment).
 * @param adc       : Pointer to ADC configuration structure.
 * @ref             : ADC_Config, ADC_Result_Format
 * @return          : Status of the function  
 *      (E_OK)      : The function done successfully  
 *      (E_NOT_OK)  : The function has an issue to perform the action  
 */

static Std_ReturnType ADC_Set_Result_Format(const ADC_Config *adc){
    Std_ReturnType Retval = E_OK;
    if(NULL == adc){
        Retval = E_NOT_OK;
    }
    else{
        switch(adc->format){
            case ADC_Result_Format_Right:
                ADC_Result_Format_Right_B0_B9();
                break;
            case ADC_Result_Format_Left:
                ADC_Result_Format_Left_B6_B15();
                break;
            default :
                Retval = E_NOT_OK;
                break;
        }
    }
    return Retval;
}

#if ((Interrupt_Feature_Enable) == (ADC_Interrupt_Feature))

/**
 * @brief           : This function configures ADC interrupts and assigns the callback.
 * @param adc       : Pointer to ADC configuration structure.
 * @ref             : ADC_Config
 * @return          : Status of the function  
 *      (E_OK)      : The function done successfully  
 *      (E_NOT_OK)  : The function has an issue to perform the action  
 */

static Std_ReturnType ADC_Configure_Interrupt(const ADC_Config *adc){
    Std_ReturnType Retval = E_OK;
    if(NULL == adc){
        Retval = E_NOT_OK;
    }
    else{
        // 1. clear ADC flag
        Internal_Interrupt_ADC_Clear_Flag();
        // 2. set ADC enable
        Internal_Interrupt_ADC_Enable();
        // 3. set global interrupt enable
        GLobal_Interrupt_Enable_SREG();
        // 4. assign call back function
        ADC_Interrupt_Handler = adc->ADC_Interrupt;
    }
    return Retval;
}

#endif

/**
 * @brief           : This function enables the ADC module.
 * @param adc       : Pointer to ADC configuration structure.
 * @ref             : ADC_Config
 * @return          : Status of the function  
 *      (E_OK)      : The function done successfully  
 *      (E_NOT_OK)  : The function has an issue to perform the action  
 */

static Std_ReturnType ADC_Set_Enable(const ADC_Config *adc){
    Std_ReturnType Retval = E_OK;
    if(NULL == adc){
        Retval = E_NOT_OK;
    }
    else{
        ADC_Module_Enable();
    }
    return Retval;
}
