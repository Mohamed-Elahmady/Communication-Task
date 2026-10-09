/* 
 * File:   hal_pwm.c
 * Author: mohammedel-ahmady
 *
 * Created on September 13, 2025, 2:31 PM
 */

/******************* Section 0 : Includes *******************/

#include "hal_pwm.h"

/******************* Section 1 :  Variables Definitions *******************/



/******************* Section 2 :  Helper Functions Declarations *******************/

Std_ReturnType PWM_Set_Disable(const PWM_CFG *pwm);
Std_ReturnType PWM_Set_Mode(const PWM_CFG *pwm);
Std_ReturnType PWM_Set_Action(const PWM_CFG *pwm);
Std_ReturnType PWM_Pin_INIT(const PWM_CFG *pwm);
Std_ReturnType PWM_Set_Prescaler(const PWM_CFG *pwm);

/******************* Section 3 : Software Interfaces Definitions (APIs) *******************/

Std_ReturnType PWM_INIT(const PWM_CFG *pwm){
    Std_ReturnType Retval = E_OK;
    if(NULL == pwm){
        Retval = E_NOT_OK;
    }
    else{
        // 1. Disable PWM module
        Retval = PWM_Set_Disable(pwm);
        // 2. Set PWM Mode & Resolution for timer 1
        Retval = PWM_Set_Mode(pwm);
        // 3. Set PWM Action
        Retval = PWM_Set_Action(pwm);
        // 4. PWM Pin INIT
        Retval = PWM_Pin_INIT(pwm);
        // 5. Set Prescaler
        Retval = PWM_Set_Prescaler(pwm);
    }
    return Retval;
}

Std_ReturnType PWM_DEINIT(const PWM_CFG *pwm){
    Std_ReturnType Retval = E_OK;
    if(NULL == pwm){
        Retval = E_NOT_OK;
    }
    else{
        // 1. Disable PWM module
        Retval = PWM_Set_Disable(pwm);
    }
    return Retval;
}

Std_ReturnType PWM_Start(const PWM_CFG *pwm){
    Std_ReturnType Retval = E_OK;
    if(NULL == pwm){
        Retval = E_NOT_OK;
    }
    else{
        switch(pwm->pwm_src){
            case PWM_OC0:
                TCNT0 = 0x00;
                break;
            case PWM_OCA1:case PWM_OCB1:
                TCNT1 = 0x0000;
                break;
            case PWM_OC2:
                TCNT2 = 0x00;
                break;
            default :
                Retval = E_NOT_OK;
                break;
        }
    }
    return Retval;
}

Std_ReturnType PWM_Stop(const PWM_CFG *pwm){
    Std_ReturnType Retval = E_OK;
    if(NULL == pwm){
        Retval = E_NOT_OK;
    }
    else{
        // 1. Disable PWM module
        Retval = PWM_Set_Disable(pwm);
    }
    return Retval;
}

Std_ReturnType PWM_Set_Duty_Cycle(const PWM_CFG *pwm, uint16 duty){
    Std_ReturnType Retval = E_OK;
    if(NULL == pwm){
        Retval = E_NOT_OK;
    }
    else{
        switch(pwm->pwm_src){
            case PWM_OC0:
                if(duty > 255){
                    Retval = E_NOT_OK;
                }
                else{
                    OCR0 = duty;
                }
                break;
            case PWM_OCA1:
                if(pwm->pwm_res == PWM_8Bit){
                    if(duty > 255){
                        Retval = E_NOT_OK;
                    }
                    else{
                        OCR1A = duty;
                    }
                }
                else if(pwm->pwm_res == PWM_9Bit){
                    if(duty > 511){
                        Retval = E_NOT_OK;
                    }
                    else{
                        OCR1A = duty;
                    }
                }
                else if(pwm->pwm_res == PWM_10Bit){
                    if(duty > 1023){
                        Retval = E_NOT_OK;
                    }
                    else{
                        OCR1A = duty;
                    }
                }
                else{
                    Retval = E_NOT_OK;
                }
                break;
            case PWM_OCB1:
                if(pwm->pwm_res == PWM_8Bit){
                    if(duty > 255){
                        Retval = E_NOT_OK;
                    }
                    else{
                        OCR1B = duty;
                    }
                }
                else if(pwm->pwm_res == PWM_9Bit){
                    if(duty > 511){
                        Retval = E_NOT_OK;
                    }
                    else{
                        OCR1B = duty;
                    }
                }
                else if(pwm->pwm_res == PWM_10Bit){
                    if(duty > 1023){
                        Retval = E_NOT_OK;
                    }
                    else{
                        OCR1B = duty;
                    }
                }
                else{
                    Retval = E_NOT_OK;
                }
                break;
            case PWM_OC2:
                if(duty > 255){
                    Retval = E_NOT_OK;
                }
                else{
                    OCR2 = duty;
                }
                break;
            default :
                Retval = E_NOT_OK;
                break;
        }
    }
    return Retval;
}

/******************* Section 4:  Helper Functions Definitions *******************/

Std_ReturnType PWM_Set_Disable(const PWM_CFG *pwm){
    Std_ReturnType Retval = E_OK;
    if(NULL == pwm){
        Retval = E_NOT_OK;
    }
    else{
        switch(pwm->pwm_src){
            case PWM_OC0:
                PWM_TIMER0_SET_PRESCALER(PWM_Module_Disable);
                break;
            case PWM_OCA1:case PWM_OCB1:
                PWM_TIMER1_SET_PRESCALER(PWM_Module_Disable);
                break;
            case PWM_OC2:
                PWM_TIMER2_SET_PRESCALER(PWM_Module_Disable);
                break;
            default :
                Retval = E_NOT_OK;
                break;
        }
    }
    return Retval;
}

Std_ReturnType PWM_Set_Mode(const PWM_CFG *pwm){
    Std_ReturnType Retval = E_OK;
    if(NULL == pwm){
        Retval = E_NOT_OK;
    }
    else{
        switch(pwm->pwm_mode){
            case PWM_Phase_Mode:
                if(pwm->pwm_src == PWM_OC0){
                    PWM_Phase_Timer0_Waveform_Generate();
                }
                else if(pwm->pwm_src == PWM_OCA1 || pwm->pwm_src == PWM_OCB1){
                    PWM_Phase_Timer1_Waveform_Generate();
                    if(pwm->pwm_res == PWM_8Bit){
                        PWM_Timer1_Waveform_Generate_8Bit();
                    }
                    else if(pwm->pwm_res == PWM_9Bit){
                        PWM_Timer1_Waveform_Generate_9Bit();
                    }
                    else if(pwm->pwm_res == PWM_10Bit){
                        PWM_Timer1_Waveform_Generate_10Bit();
                    }
                    else{
                        Retval = E_NOT_OK;
                    }
                }
                else if(pwm->pwm_src == PWM_OC2){
                    PWM_Phase_Timer2_Waveform_Generate();
                }
                else{
                    Retval = E_NOT_OK;
                }
                break;
            case PWM_Fast_Mode:
                if(pwm->pwm_src == PWM_OC0){
                    PWM_Fast_Timer0_Waveform_Generate();
                }
                else if(pwm->pwm_src == PWM_OCA1 || pwm->pwm_src == PWM_OCB1){
                    PWM_Fast_Timer1_Waveform_Generate();
                    if(pwm->pwm_res == PWM_8Bit){
                        PWM_Timer1_Waveform_Generate_8Bit();
                    }
                    else if(pwm->pwm_res == PWM_9Bit){
                        PWM_Timer1_Waveform_Generate_9Bit();
                    }
                    else if(pwm->pwm_res == PWM_10Bit){
                        PWM_Timer1_Waveform_Generate_10Bit();
                    }
                    else{
                        Retval = E_NOT_OK;
                    }
                }
                else if(pwm->pwm_src == PWM_OC2){
                    PWM_Fast_Timer2_Waveform_Generate();
                }
                else{
                    Retval = E_NOT_OK;
                }
                break;
            default :
                Retval = E_NOT_OK;
                break;
        }
    }
    return Retval;
}

Std_ReturnType PWM_Set_Action(const PWM_CFG *pwm){
    Std_ReturnType Retval = E_OK;
    if(NULL == pwm){
        Retval = E_NOT_OK;
    }
    else{
        switch(pwm->pwm_src){
            case PWM_OC0:
                if(pwm->pwm_act == PWM_OCXX_Disconnected_Pin){
                    PWM_Timer0_OC0_Disconnected();
                }
                else if(pwm->pwm_act == PWM_Non_Inverting_OCXX_Clear_Pin){
                    PWM_Timer0_OC0_Clear_Logic_Non_Inverting();
                }
                else if(pwm->pwm_act == PWM_Inverting_OCXX_Set_Pin){
                    PWM_Timer0_OC0_Set_Logic_Inverting();
                }
                else{
                    Retval = E_NOT_OK;
                }
                break;
            case PWM_OCA1:
                if(pwm->pwm_act == PWM_OCXX_Disconnected_Pin){
                    PWM_Timer1_OCA1_Disconnected();
                }
                else if(pwm->pwm_act == PWM_OCXX_Toggle_Pin){
                    PWM_Timer1_OCA1_Toggle_Logic();
                }
                else if(pwm->pwm_act == PWM_Non_Inverting_OCXX_Clear_Pin){
                    PWM_Timer1_OCA1_Clear_Logic_Non_Inverting();
                }
                else if(pwm->pwm_act == PWM_Inverting_OCXX_Set_Pin){
                    PWM_Timer1_OCA1_Set_Logic_Inverting();
                }
                else{
                    Retval = E_NOT_OK;
                }
                break;
            case PWM_OCB1:
                if(pwm->pwm_act == PWM_OCXX_Disconnected_Pin){
                    PWM_Timer1_OCB1_Disconnected();
                }
                else if(pwm->pwm_act == PWM_OCXX_Toggle_Pin){
                    PWM_Timer1_OCB1_Toggle_Logic();
                }
                else if(pwm->pwm_act == PWM_Non_Inverting_OCXX_Clear_Pin){
                    PWM_Timer1_OCB1_Clear_Logic_Non_Inverting();
                }
                else if(pwm->pwm_act == PWM_Inverting_OCXX_Set_Pin){
                    PWM_Timer1_OCB1_Set_Logic_Inverting();
                }
                else{
                    Retval = E_NOT_OK;
                }
                break;
            case PWM_OC2:
                if(pwm->pwm_act == PWM_OCXX_Disconnected_Pin){
                    PWM_Timer2_OC2_Disconnected();
                }
                else if(pwm->pwm_act == PWM_Non_Inverting_OCXX_Clear_Pin){
                    PWM_Timer2_OC2_Clear_Logic_Non_Inverting();
                }
                else if(pwm->pwm_act == PWM_Inverting_OCXX_Set_Pin){
                    PWM_Timer2_OC2_Set_Logic_Inverting();
                }
                else{
                    Retval = E_NOT_OK;
                }
                break;
            default :
                Retval = E_NOT_OK;
                break;
        }
    }
    return Retval;
}

Std_ReturnType PWM_Pin_INIT(const PWM_CFG *pwm){
    Std_ReturnType Retval = E_OK;
    if(NULL == pwm){
        Retval = E_NOT_OK;
    }
    else{
        switch(pwm->pwm_src){
            case PWM_OC0:{
                GPIO_Pin_CFG oc0_pin = {.Port = GPIO_PortB, .Pin = GPIO_Pin3, .Direction = GPIO_Output, .Logic = GPIO_Low};
                Retval = GPIO_Pin_INIT(&oc0_pin);
                break;
            }
            case PWM_OCA1:{
                GPIO_Pin_CFG oca1_pin = {.Port = GPIO_PortD, .Pin = GPIO_Pin5, .Direction = GPIO_Output, .Logic = GPIO_Low};
                Retval = GPIO_Pin_INIT(&oca1_pin);
                break;
            }
            case PWM_OCB1:{
                GPIO_Pin_CFG ocb1_pin = {.Port = GPIO_PortD, .Pin = GPIO_Pin4, .Direction = GPIO_Output, .Logic = GPIO_Low};
                Retval = GPIO_Pin_INIT(&ocb1_pin);
                break;
            }
            case PWM_OC2:{
                GPIO_Pin_CFG oc2_pin = {.Port = GPIO_PortD, .Pin = GPIO_Pin7, .Direction = GPIO_Output, .Logic = GPIO_Low};
                Retval = GPIO_Pin_INIT(&oc2_pin);
                break;
            }
            default :
                Retval = E_NOT_OK;
                break;
        }
    }
    return Retval;
}

Std_ReturnType PWM_Set_Prescaler(const PWM_CFG *pwm){
    Std_ReturnType Retval = E_OK;
    if(NULL == pwm){
        Retval = E_NOT_OK;
    }
    else{
        switch(pwm->pwm_src){
            case PWM_OC0:
                if(pwm->pwm_prescaler == PWM_Prescaler_DIV_1 || pwm->pwm_prescaler == PWM_Prescaler_DIV_8 || 
                   pwm->pwm_prescaler == PWM_Prescaler_DIV_64 || pwm->pwm_prescaler == PWM_Prescaler_DIV_256 || 
                   pwm->pwm_prescaler == PWM_Prescaler_DIV_1024){
                    PWM_TIMER0_SET_PRESCALER(pwm->pwm_prescaler);
                }
                else{
                    Retval = E_NOT_OK;
                }
                break;
            case PWM_OCA1:case PWM_OCB1:
                if(pwm->pwm_prescaler == PWM_Prescaler_DIV_1 || pwm->pwm_prescaler == PWM_Prescaler_DIV_8 || 
                   pwm->pwm_prescaler == PWM_Prescaler_DIV_64 || pwm->pwm_prescaler == PWM_Prescaler_DIV_256 || 
                   pwm->pwm_prescaler == PWM_Prescaler_DIV_1024){
                    PWM_TIMER1_SET_PRESCALER(pwm->pwm_prescaler);
                }
                else{
                    Retval = E_NOT_OK;
                }
                break;
            case PWM_OC2:
                if(pwm->pwm_prescaler == PWM_Prescaler_DIV_1 || pwm->pwm_prescaler == PWM_Prescaler_DIV_8 || 
                   pwm->pwm_prescaler == PWM_Prescaler_DIV_32 || pwm->pwm_prescaler == PWM_Prescaler_DIV_64 || 
                   pwm->pwm_prescaler == PWM_Prescaler_DIV_128 || pwm->pwm_prescaler == PWM_Prescaler_DIV_256 || 
                   pwm->pwm_prescaler == PWM_Prescaler_DIV_1024){
                    PWM_TIMER2_SET_PRESCALER(pwm->pwm_prescaler);
                }
                else{
                    Retval = E_NOT_OK;
                }
                break;
            default :
                Retval = E_NOT_OK;
                break;
        }
    }
    return Retval;
}
