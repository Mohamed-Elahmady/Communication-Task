/* 
 * File:   hal_ext_interrupt.c
 * Author: mohammedel-ahmady
 *
 * Created on August 3, 2025, 6:15 PM
 */

/******************* Section 0 : Includes *******************/

#include "hal_ext_interrupt.h"

/******************* Section 1 :  Variables Definitions *******************/



/******************* Section 2 :  Helper Functions Declarations *******************/

static Interrupt_Handler INT0_Handler = NULL;
static Interrupt_Handler INT1_Handler = NULL;
static Interrupt_Handler INT2_Handler = NULL;

static Std_ReturnType External_Interrupt_INTX_Clear_Flag(const External_Interrupt_Intx *intx);
static Std_ReturnType External_Interrupt_INTX_Disable(const External_Interrupt_Intx *intx);
static Std_ReturnType External_Interrupt_INTX_Enable(const External_Interrupt_Intx *intx);
static Std_ReturnType External_Interrupt_INTX_Set_Edge(const External_Interrupt_Intx *intx);
static Std_ReturnType External_Interrupt_INTX_Pin_Init(const External_Interrupt_Intx *intx);

static Std_ReturnType External_Interrupt_INTX_Set_Handler(const External_Interrupt_Intx *intx);
static Std_ReturnType External_Interrupt_INT0_Set_Handler(const Interrupt_Handler int0_handle);
static Std_ReturnType External_Interrupt_INT1_Set_Handler(const Interrupt_Handler int1_handle);
static Std_ReturnType External_Interrupt_INT2_Set_Handler(const Interrupt_Handler int2_handle);

/******************* Section 3 : Software Interfaces Definitions (APIs) *******************/

Std_ReturnType External_Interrupt_INTx_INIT(const External_Interrupt_Intx *intx){
    Std_ReturnType Retval = E_OK;
    if(NULL == intx){
        Retval = E_NOT_OK;
    }
    else{
        // 1. Disable INTX
        Retval = External_Interrupt_INTX_Disable(intx);
        // 2. Clear INTX Flag
        Retval = External_Interrupt_INTX_Clear_Flag(intx);
        // 3. Set INTX Edge
        Retval = External_Interrupt_INTX_Set_Edge(intx);
        // 4. INTX Pin INIT
        Retval = External_Interrupt_INTX_Pin_Init(intx);
        // 5. Assign Call Back Function
        Retval = External_Interrupt_INTX_Set_Handler(intx);
        // 6. Enable INTX
        Retval = External_Interrupt_INTX_Enable(intx);
    }
    return Retval;
}

Std_ReturnType External_Interrupt_INTx_DEINIT(const External_Interrupt_Intx *intx){
    Std_ReturnType Retval = E_OK;
    if(NULL == intx){
        Retval = E_NOT_OK;
    }
    else{
        // 1. Disable INTx
        Retval = External_Interrupt_INTX_Disable(intx);
        // 2. Clear Flag
        Retval = External_Interrupt_INTX_Clear_Flag(intx);
    }
    return Retval;
}


void INT0_ISR(void){
    // 1. Clear Flag
    External_Interrupt_Int0_Clear_Flag();
    // 2. Code
    
    // 3. Call back Function
    if(INT0_Handler){
        INT0_Handler();
    }
    else{/* Nothing */}
}

void INT1_ISR(void){
    // 1. Clear Flag
    External_Interrupt_Int1_Clear_Flag();
    // 2. Code
    
    // 3. Call back Function
    if(INT1_Handler){
        INT1_Handler();
    }
    else{/* Nothing */}
}

void INT2_ISR(void){
    // 1. Clear Flag
    External_Interrupt_Int2_Clear_Flag();
    // 2. Code
    
    // 3. Call back Function
    if(INT2_Handler){
        INT2_Handler();
    }
    else{/* Nothing */}
}

/******************* Section 4:  Helper Functions Definitions *******************/

static Std_ReturnType External_Interrupt_INTX_Clear_Flag(const External_Interrupt_Intx *intx){
    Std_ReturnType Retval = E_OK;
    if(NULL == intx){
        Retval = E_NOT_OK;
    }
    else{
        switch(intx->source){
            case Interrupt_Int0:
                External_Interrupt_Int0_Clear_Flag();
                break;
            case Interrupt_Int1:
                External_Interrupt_Int1_Clear_Flag();
                break;
            case Interrupt_Int2:
                External_Interrupt_Int2_Clear_Flag();
                break;
            default :
                Retval = E_NOT_OK;
                break;
        }
    }
    return Retval;
}

static Std_ReturnType External_Interrupt_INTX_Disable(const External_Interrupt_Intx *intx){
    Std_ReturnType Retval = E_OK;
    if(NULL == intx){
        Retval = E_NOT_OK;
    }
    else{
        switch(intx->source){
            case Interrupt_Int0:
                External_Interrupt_Int0_Disable();
                break;
            case Interrupt_Int1:
                External_Interrupt_Int1_Disable();
                break;
            case Interrupt_Int2:
                External_Interrupt_Int2_Disable();
                break;
            default :
                Retval = E_NOT_OK;
                break;
        }
    }
    return Retval;
}

static Std_ReturnType External_Interrupt_INTX_Enable(const External_Interrupt_Intx *intx){
    Std_ReturnType Retval = E_OK;
    if(NULL == intx){
        Retval = E_NOT_OK;
    }
    else{
        switch(intx->source){
            case Interrupt_Int0:
                GLobal_Interrupt_Enable_SREG();
                External_Interrupt_Int0_Enable();
                break;
            case Interrupt_Int1:
                GLobal_Interrupt_Enable_SREG();
                External_Interrupt_Int1_Enable();
                break;
            case Interrupt_Int2:
                GLobal_Interrupt_Enable_SREG();
                External_Interrupt_Int2_Enable();
                break;
            default :
                Retval = E_NOT_OK;
                break;
        }
    }
    return Retval;
}

static Std_ReturnType External_Interrupt_INTX_Set_Edge(const External_Interrupt_Intx *intx){
    Std_ReturnType Retval = E_OK;
    if(NULL == intx){
        Retval = E_NOT_OK;
    }
    else{
        switch(intx->source){
            case Interrupt_Int0:
                if(Interrupt_Low_Level == intx->edge){
                    External_Interrupt_Int0_Low_Level_ISC00();
                    External_Interrupt_Int0_Low_Level_ISC01();
                }
                else if(Interrupt_On_Change == intx->edge){
                    External_Interrupt_Int0_On_Change_ISC00();
                    External_Interrupt_Int0_On_Change_ISC01();
                }
                else if(Interrupt_Falling_Edge == intx->edge){
                    External_Interrupt_Int0_Falling_Edge_ISC00();
                    External_Interrupt_Int0_Falling_Edge_ISC01();
                }
                else if(Interrupt_Rising_Edge == intx->edge){
                    External_Interrupt_Int0_Rising_Edge_ISC00();
                    External_Interrupt_Int0_Rising_Edge_ISC01();
                }
                else{
                    Retval = E_NOT_OK;
                }
                break;
            case Interrupt_Int1:
                if(Interrupt_Low_Level == intx->edge){
                    External_Interrupt_Int1_Low_Level_ISC10();
                    External_Interrupt_Int1_Low_Level_ISC11();
                }
                else if(Interrupt_On_Change == intx->edge){
                    External_Interrupt_Int1_On_Change_ISC10();
                    External_Interrupt_Int1_On_Change_ISC11();
                }
                else if(Interrupt_Falling_Edge == intx->edge){
                    External_Interrupt_Int1_Falling_Edge_ISC10();
                    External_Interrupt_Int1_Falling_Edge_ISC11();
                }
                else if(Interrupt_Rising_Edge == intx->edge){
                    External_Interrupt_Int1_Rising_Edge_ISC10();
                    External_Interrupt_Int1_Rising_Edge_ISC11();
                }
                else{
                    Retval = E_NOT_OK;
                }
                break;
            case Interrupt_Int2:
                if(Interrupt_Falling_Edge == intx->edge){
                    External_Interrupt_Int2_Falling_Edge_ISC2();
                }
                else if(Interrupt_Rising_Edge == intx->edge){
                    External_Interrupt_Int2_Rising_Edge_ISC2();
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

static Std_ReturnType External_Interrupt_INTX_Pin_Init(const External_Interrupt_Intx *intx){
    Std_ReturnType Retval = E_OK;
    if(NULL == intx){
        Retval = E_NOT_OK;
    }
    else{
        Retval = GPIO_Pin_Direction_INIT(&(intx->pin));
    }
    return Retval;
}

static Std_ReturnType External_Interrupt_INTX_Set_Handler(const External_Interrupt_Intx *intx){
    Std_ReturnType Retval = E_OK;
    if(NULL == intx){
        Retval = E_NOT_OK;
    }
    else{
        switch(intx->source){
            case Interrupt_Int0:
                Retval = External_Interrupt_INT0_Set_Handler(intx->Intx_ISR);
                break;
            case Interrupt_Int1:
                Retval = External_Interrupt_INT1_Set_Handler(intx->Intx_ISR);
                break;
            case Interrupt_Int2:
                Retval = External_Interrupt_INT2_Set_Handler(intx->Intx_ISR);
                break;
            default :
                Retval = E_NOT_OK;
                break;
        }
    }
    return Retval;
}

static Std_ReturnType External_Interrupt_INT0_Set_Handler(const Interrupt_Handler int0_handle){
    Std_ReturnType Retval = E_OK;
    if(NULL == int0_handle){
        Retval = E_NOT_OK;
    }
    else{
        INT0_Handler = int0_handle;
    }
    return Retval;
}

static Std_ReturnType External_Interrupt_INT1_Set_Handler(const Interrupt_Handler int1_handle){
    Std_ReturnType Retval = E_OK;
    if(NULL == int1_handle){
        Retval = E_NOT_OK;
    }
    else{
        INT1_Handler = int1_handle;
    }
    return Retval;
}

static Std_ReturnType External_Interrupt_INT2_Set_Handler(const Interrupt_Handler int2_handle){
    Std_ReturnType Retval = E_OK;
    if(NULL == int2_handle){
        Retval = E_NOT_OK;
    }
    else{
        INT2_Handler = int2_handle;
    }
    return Retval;
}
