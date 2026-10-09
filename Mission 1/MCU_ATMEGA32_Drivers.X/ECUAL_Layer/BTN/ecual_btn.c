/* 
 * File:   ecual_btn.c
 * Author: mohammedel-ahmady
 *
 * Created on July 5, 2025, 1:07 PM
 */

/******************* Section 0 : Includes *******************/

#include "ecual_btn.h"

/******************* Section 1 :  Variables Definitions *******************/



/******************* Section 2 :  Helper Functions Declarations *******************/



/******************* Section 3 : Software Interfaces Definitions (APIs) *******************/

#if ((GPIO_Btn_CFG_Enable) == (GPIO_Btn_CFG))

/**
 * @brief           : This function initializes the direction of the button pin  
 * @ref             : GPIO_Btn, GPIO_Pin_Direction_INIT
 * @param btn       : Pointer to button configuration structure  
 * @return          : Status of the function  
 *      (E_OK)      : The function done Successfully  
 *      (E_NOT_OK)  : The function has issue to perform the action (e.g., NULL pointer)  
 */

Std_ReturnType GPIO_Btn_INIT(const GPIO_Btn *btn){
    Std_ReturnType Retval = E_OK;
    if(NULL == btn){
        Retval = E_NOT_OK;
    }
    else{
        Retval = GPIO_Pin_Direction_INIT(&(btn->pin));
    }
    return Retval;
}

/**
 * @brief           : This function reads the current state of the button (Pressed or Released)  
 * @ref             : GPIO_Btn, Btn_State, GPIO_Pin_Read_Logic, GPIO_Logic  
 * @param btn       : Pointer to button configuration structure  
 * @param state     : Pointer to variable to store the current button state  
 * @return          : Status of the function  
 *      (E_OK)      : The function done Successfully  
 *      (E_NOT_OK)  : The function has issue to perform the action (e.g., NULL pointer or invalid logic state)  
 */

Std_ReturnType GPIO_Btn_Read_State(const GPIO_Btn *btn, Btn_State *state){
    Std_ReturnType Retval = E_OK;
    if(NULL == btn || NULL == state){
        Retval = E_NOT_OK;
    }
    else{
        GPIO_Logic logic = GPIO_Low;
        Retval = GPIO_Pin_Read_Logic(&(btn->pin), &logic);
        switch(btn->connection){
            case Btn_Active_Low:
                if(GPIO_High == logic){
                    *state = Btn_Released;
                }
                else if(GPIO_Low == logic){
                    *state = Btn_Pressed;
                }
                else{
                    Retval = E_NOT_OK;
                }
                break;
            case Btn_Active_High:
                if(GPIO_High == logic){
                    *state = Btn_Pressed;
                }
                else if(GPIO_Low == logic){
                    *state = Btn_Released;
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

#endif

/******************* Section 2 :  Helper Functions Definitions *******************/


