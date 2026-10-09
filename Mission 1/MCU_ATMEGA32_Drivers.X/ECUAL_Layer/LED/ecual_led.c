/* 
 * File:   ecual_led.c
 * Author: mohammedel-ahmady
 *
 * Created on June 26, 2025, 8:01 PM
 */

/******************* Section 0 : Includes *******************/

#include "ecual_led.h"

/******************* Section 1 :  Variables Definitions *******************/



/******************* Section 2 :  Helper Functions Declarations *******************/



/******************* Section 3 : Software Interfaces Definitions (APIs) *******************/

#if ((GPIO_Led_CFG_Enable) == (GPIO_Led_CFG))

/**
 * @brief           : Initializes the LED pin with the specified configuration.
 * @param led       : Pointer to the LED configuration structure.
 * @ref             : GPIO_Led
 * @return          : Status of the function
 *      (E_OK)      : Initialization completed successfully
 *      (E_NOT_OK)  : Null pointer or failure during pin initialization
 */

Std_ReturnType GPIO_Led_INIT(const GPIO_Led *led){
    Std_ReturnType Retval = E_OK;
    if(NULL == led){
        Retval = E_NOT_OK;
    }
    else{
        Retval = GPIO_Pin_Direction_INIT(&(led->pin));
        GPIO_Logic logic;
        switch(led->connection){
            case Led_Sink:
                if(led->state == Led_On){
                    logic = GPIO_Low;
                }
                else if(led->state == Led_Off){
                    logic = GPIO_High;
                }
                else{
                    Retval = E_NOT_OK;
                }
                break;
            case Led_Source:
                if(led->state == Led_On){
                    logic = GPIO_High;
                }
                else if(led->state == Led_Off){
                    logic = GPIO_Low;
                }
                else{
                    Retval = E_NOT_OK;
                }
                break;
            default :
                Retval = E_NOT_OK;
                break;
        }
        Retval = GPIO_Pin_Write_Logic(&(led->pin), logic);
    }
    return Retval;
}

/**
 * @brief           : Reads the current state (ON/OFF) of the LED based on its connection type.
 * @param led       : Pointer to the LED configuration structure.
 * @param state     : Pointer to variable where the LED state will be stored.
 * @ref             : Led_State
 * @return          : Status of the function
 *      (E_OK)      : Reading completed successfully
 *      (E_NOT_OK)  : Null pointer or invalid logic read from pin
 */

Std_ReturnType GPIO_Led_Read_Logic(const GPIO_Led *led, Led_State *state){
    Std_ReturnType Retval = E_OK;
    if(NULL == led || NULL == state){
        Retval = E_NOT_OK;
    }
    else{
        GPIO_Logic logic;
        Retval = GPIO_Pin_Read_Logic(&(led->pin), &logic);
        switch(led->connection){
            case Led_Sink:
                if(GPIO_High == logic){
                    *state = Led_Off;
                }
                else if(GPIO_Low == logic){
                    *state = Led_On;
                }
                else{
                    Retval = E_NOT_OK;
                }
                break;
            case Led_Source:
                if(GPIO_High == logic){
                    *state = Led_On;
                }
                else if(GPIO_Low == logic){
                    *state = Led_Off;
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

/**
 * @brief           : Turns the LED OFF depending on its connection type (Sink/Source).
 * @param led       : Pointer to the LED configuration structure.
 * @return          : Status of the function
 *      (E_OK)      : LED turned off successfully
 *      (E_NOT_OK)  : Null pointer or invalid connection type
 */

Std_ReturnType GPIO_Led_Turn_Off(const GPIO_Led *led){
    Std_ReturnType Retval = E_OK;
    if(NULL == led){
        Retval = E_NOT_OK;
    }
    else{
        GPIO_Logic logic;
        switch(led->connection){
            case Led_Sink:
                logic = GPIO_High;
                Retval = GPIO_Pin_Write_Logic(&(led->pin), logic);
                break;
            case Led_Source:
                logic = GPIO_Low;
                Retval = GPIO_Pin_Write_Logic(&(led->pin), logic);
                break;
            default :
                Retval = E_NOT_OK;
                break;
        }
    }
    return Retval;
}

/**
 * @brief           : Turns the LED ON depending on its connection type (Sink/Source).
 * @param led       : Pointer to the LED configuration structure.
 * @return          : Status of the function
 *      (E_OK)      : LED turned on successfully
 *      (E_NOT_OK)  : Null pointer or invalid connection type
 */

Std_ReturnType GPIO_Led_Turn_On(const GPIO_Led *led){
    Std_ReturnType Retval = E_OK;
    if(NULL == led){
        Retval = E_NOT_OK;
    }
    else{
        GPIO_Logic logic;
        switch(led->connection){
            case Led_Sink:
                logic = GPIO_Low;
                Retval = GPIO_Pin_Write_Logic(&(led->pin), logic);
                break;
            case Led_Source:
                logic = GPIO_High;
                Retval = GPIO_Pin_Write_Logic(&(led->pin), logic);
                break;
            default :
                Retval = E_NOT_OK;
                break;
        }
    }
    return Retval;
}

/**
 * @brief           : Toggles the current state of the LED pin.
 * @param led       : Pointer to the LED configuration structure.
 * @return          : Status of the function
 *      (E_OK)      : LED toggled successfully
 *      (E_NOT_OK)  : Null pointer
 */

Std_ReturnType GPIO_Led_Turn_Toggle(const GPIO_Led *led){
    Std_ReturnType Retval = E_OK;
    if(NULL == led){
        Retval = E_NOT_OK;
    }
    else{
        Retval = GPIO_Pin_Toggle_Logic(&(led->pin));
    }
    return Retval;
}

#endif
/******************* Section 2 :  Helper Functions Definitions *******************/
