/* 
 * File:   ecual_relay.c
 * Author: mohammedel-ahmady
 *
 * Created on July 7, 2025, 6:06 PM
 */

/******************* Section 0 : Includes *******************/

#include "ecual_relay.h"

/******************* Section 1 :  Variables Definitions *******************/



/******************* Section 2 :  Helper Functions Declarations *******************/



/******************* Section 3 : Software Interfaces Definitions (APIs) *******************/

#if ((GPIO_Relay_CFG_Enable) == (GPIO_Relay_CFG))

/**
 * @brief           : This function initializes the relay pin by setting its direction and configuration
 * @param relay     : Pointer to relay configuration structure
 * @ref             : GPIO_Relay
 * @return          : Status of the function  
 *      (E_OK)      : The function completed successfully  
 *      (E_NOT_OK)  : The function encountered an error (e.g., NULL pointer)  
 */

Std_ReturnType GPIO_Relay_INIT(const GPIO_Relay *relay){
    Std_ReturnType Retval = E_OK;
    if(NULL == relay){
        Retval = E_NOT_OK;
    }
    else{
        Retval = GPIO_Pin_INIT(&(relay->pin));
    }
    return Retval;
}

/**
 * @brief           : This function reads the current logic state of the relay and determines whether it is ON or OFF
 * @param relay     : Pointer to relay configuration structure  
 * @ref             : GPIO_Relay  
 * @param state     : Pointer to variable to store the current relay state (ON/OFF)  
 * @ref             : Relay_State  
 * @return          : Status of the function  
 *      (E_OK)      : The function completed successfully  
 *      (E_NOT_OK)  : The function encountered an error (e.g., NULL pointer)  
 */

Std_ReturnType GPIO_Relay_Read_State(const GPIO_Relay *relay, Relay_State *state){
    Std_ReturnType Retval = E_OK;
    if(NULL == relay || NULL == state){
        Retval = E_NOT_OK;
    }
    else{
        GPIO_Logic logic;
        Retval = GPIO_Pin_Read_Logic(&(relay->pin), &logic);
        if(GPIO_High == logic){
            *state = Relay_On;
        }
        else if(GPIO_High == logic){
            *state = Relay_Off;
        }
        else{
            Retval = E_NOT_OK;
        }
    }
    return Retval;
}

/**
 * @brief           : This function turns the relay ON by writing HIGH logic to its control pin
 * @param relay     : Pointer to relay configuration structure  
 * @ref             : GPIO_Relay  
 * @return          : Status of the function  
 *      (E_OK)      : The function completed successfully  
 *      (E_NOT_OK)  : The function encountered an error (e.g., NULL pointer)  
 */

Std_ReturnType GPIO_Relay_Turn_On(const GPIO_Relay *relay){
    Std_ReturnType Retval = E_OK;
    if(NULL == relay){
        Retval = E_NOT_OK;
    }
    else{
        GPIO_Logic logic = GPIO_High;
        Retval = GPIO_Pin_Write_Logic(&(relay->pin), logic);
    }
    return Retval;
}

/**
 * @brief           : This function turns the relay OFF by writing LOW logic to its control pin
 * @param relay     : Pointer to relay configuration structure  
 * @ref             : GPIO_Relay  
 * @return          : Status of the function  
 *      (E_OK)      : The function completed successfully  
 *      (E_NOT_OK)  : The function encountered an error (e.g., NULL pointer)  
 */

Std_ReturnType GPIO_Relay_Turn_Off(const GPIO_Relay *relay){
    Std_ReturnType Retval = E_OK;
    if(NULL == relay){
        Retval = E_NOT_OK;
    }
    else{
        GPIO_Logic logic = GPIO_Low;
        Retval = GPIO_Pin_Write_Logic(&(relay->pin), logic);
    }
    return Retval;
}

/**
 * @brief           : This function toggles the current logic state of the relay pin (ON to OFF or OFF to ON)
 * @param relay     : Pointer to relay configuration structure  
 * @ref             : GPIO_Relay  
 * @return          : Status of the function  
 *      (E_OK)      : The function completed successfully  
 *      (E_NOT_OK)  : The function encountered an error (e.g., NULL pointer)  
 */

Std_ReturnType GPIO_Relay_Turn_Toggle(const GPIO_Relay *relay){
    Std_ReturnType Retval = E_OK;
    if(NULL == relay){
        Retval = E_NOT_OK;
    }
    else{
        Retval = GPIO_Pin_Toggle_Logic(&(relay->pin));
    }
    return Retval;
}

#endif
/******************* Section 2 :  Helper Functions Definitions *******************/
