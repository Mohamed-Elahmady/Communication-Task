/* 
 * File:   hal_gpio.c
 * Author: mohammedel-ahmady
 *
 * Created on June 26, 2025, 8:03 PM
 */

/******************* Section 0 : Includes *******************/

#include "hal_gpio.h"

/******************* Section 1 :  Variables Definitions *******************/

volatile uint8 *DDR_REGs[REG_NO] = {&DDRA, &DDRB, &DDRC, &DDRD};
volatile uint8 *PIN_REGs[REG_NO] = {&PINA, &PINB, &PINC, &PIND};
volatile uint8 *PORT_REGs[REG_NO] = {&PORTA, &PORTB, &PORTC, &PORTD};

/******************* Section 2 :  Helper Functions Declarations *******************/



/******************* Section 3 : Software Interfaces Definitions (APIs) *******************/

#if ((GPIO_Pin_CFG_Enable) == (GPIO_Pin_CFG_State))

/**
 * @brief           : This function to assign a direction for the pin "INPUT/OUTPUT"  
 * @ref             : GPIO_Direction
 * @param pin       : Pointer to pin configurations  
 * @ref             : GPIO_Pin_CFG
 * @return          : Status of the function  
 *      (E_OK)      : The function done Successfully  
 *      (E_NOT_OK)  : The function has issue to perform the action  
 */

Std_ReturnType GPIO_Pin_Direction_INIT(const GPIO_Pin_CFG *pin){
    Std_ReturnType Retval = E_OK;
    if(NULL == pin || pin->Pin >= Pin_NO){
        Retval = E_NOT_OK;
    }
    else{
        switch(pin->Direction){
            case GPIO_Input:
                Clear_Bit(*DDR_REGs[pin->Port], pin->Pin);
                break;
            case GPIO_Output:
                Set_Bit(*DDR_REGs[pin->Port], pin->Pin);
                break;
            default :
                Retval = E_NOT_OK;
                break;
        }
    }
    return (Retval);
}

/**
 * @brief           : This function gets the current direction of a pin  
 * @ref             : GPIO_Direction
 * @param pin       : Pointer to pin configurations  
 * @param direction : Pointer to variable to store the pin direction  
 * @return          : Status of the function  
 *      (E_OK)      : The function done Successfully  
 *      (E_NOT_OK)  : The function has issue to perform the action  
 */

Std_ReturnType GPIO_Pin_Get_Direction_Status(const GPIO_Pin_CFG *pin, GPIO_Direction *direction){
    Std_ReturnType Retval = E_OK;
    if(NULL == pin || NULL == direction || pin->Pin >= Pin_NO){
        Retval = E_NOT_OK;
    }
    else{
        *direction = Read_Bit(*DDR_REGs[pin->Port], pin->Pin);
    }
    return (Retval);
}

/**
 * @brief           : This function writes logic level to a pin (HIGH or LOW)  
 * @ref             : GPIO_Logic
 * @param pin       : Pointer to pin configurations  
 * @param logic     : Logic level to write  
 * @return          : Status of the function  
 *      (E_OK)      : The function done Successfully  
 *      (E_NOT_OK)  : The function has issue to perform the action  
 */

Std_ReturnType GPIO_Pin_Write_Logic(const GPIO_Pin_CFG *pin, GPIO_Logic logic){
    Std_ReturnType Retval = E_OK;
    if(NULL == pin || pin->Pin >= Pin_NO){
        Retval = E_NOT_OK;
    }
    else{
        switch(logic){
            case GPIO_Low:
                Clear_Bit(*PORT_REGs[pin->Port], pin->Pin);
                break;
            case GPIO_High:
                Set_Bit(*PORT_REGs[pin->Port], pin->Pin);
                break;
            default :
                Retval = E_NOT_OK;
                break;
        }
    }
    return (Retval);
}

/**
 * @brief           : This function reads the current logic level of the pin  
 * @ref             : GPIO_Logic
 * @param pin       : Pointer to pin configurations  
 * @param logic     : Pointer to variable to store the read logic level  
 * @return          : Status of the function  
 *      (E_OK)      : The function done Successfully  
 *      (E_NOT_OK)  : The function has issue to perform the action  
 */

Std_ReturnType GPIO_Pin_Read_Logic(const GPIO_Pin_CFG *pin, GPIO_Logic *logic){
    Std_ReturnType Retval = E_OK;
    if(NULL == pin || NULL == logic || pin->Pin >= Pin_NO){
        Retval = E_NOT_OK;
    }
    else{
        *logic = Read_Bit(*PIN_REGs[pin->Port], pin->Pin);
    }
    return (Retval);
}

/**
 * @brief           : This function toggles the current logic level of the pin  
 * @param pin       : Pointer to pin configurations  
 * @return          : Status of the function  
 *      (E_OK)      : The function done Successfully  
 *      (E_NOT_OK)  : The function has issue to perform the action  
 */

Std_ReturnType GPIO_Pin_Toggle_Logic(const GPIO_Pin_CFG *pin){
    Std_ReturnType Retval = E_OK;
    if(NULL == pin || pin->Pin >= Pin_NO){
        Retval = E_NOT_OK;
    }
    else{
        Toggle_Bit(*PORT_REGs[pin->Port], pin->Pin);
    }
    return (Retval);
}

/**
 * @brief           : This function initializes the pin direction and default logic level  
 * @param pin       : Pointer to pin configurations  
 * @return          : Status of the function  
 *      (E_OK)      : The function done Successfully  
 *      (E_NOT_OK)  : The function has issue to perform the action  
 */

Std_ReturnType GPIO_Pin_INIT(const GPIO_Pin_CFG *pin){
    Std_ReturnType Retval = E_OK;
    if(NULL == pin || pin->Pin >= Pin_NO){
        Retval = E_NOT_OK;
    }
    else{
        Retval = GPIO_Pin_Direction_INIT(pin);
        Retval = GPIO_Pin_Write_Logic(pin, pin->Logic);
    }
    return (Retval);
}

#endif

#if ((GPIO_Port_CFG_Enable) == (GPIO_Port_CFG_State))

/**
 * @brief           : This function sets the direction for all pins in a port  
 * @param port      : Port index @ref GPIO_Port  
 * @param direction : 8-bit direction value for all pins  
 * @return          : Status of the function  
 *      (E_OK)      : The function done Successfully  
 *      (E_NOT_OK)  : The function has issue to perform the action  
 */

Std_ReturnType GPIO_Port_Direction_INIT(GPIO_Port port, uint8 direction){
    Std_ReturnType Retval = E_OK;
    if(port >= Port_NO){
        Retval = E_NOT_OK;
    }
    else{
        *DDR_REGs[port] = direction;
    }
    return (Retval);
}

/**
 * @brief           : This function gets the direction value of the port  
 * @param port      : Port index @ref GPIO_Port  
 * @param direction : Pointer to store the direction value  
 * @return          : Status of the function  
 *      (E_OK)      : The function done Successfully  
 *      (E_NOT_OK)  : The function has issue to perform the action  
 */

Std_ReturnType GPIO_Port_Get_Direction_Status(GPIO_Port port, uint8 *direction){
    Std_ReturnType Retval = E_OK;
    if(NULL == direction || port >= Port_NO){
        Retval = E_NOT_OK;
    }
    else{
        *direction = *DDR_REGs[port];
    }
    return (Retval);
}

/**
 * @brief           : This function writes logic levels to the port  
 * @param port      : Port index @ref GPIO_Port  
 * @param logic     : 8-bit logic level to write  
 * @return          : Status of the function  
 *      (E_OK)      : The function done Successfully  
 *      (E_NOT_OK)  : The function has issue to perform the action  
 */

Std_ReturnType GPIO_Port_Write_Logic(GPIO_Port port, uint8 logic){
    Std_ReturnType Retval = E_OK;
    if(port >= Port_NO){
        Retval = E_NOT_OK;
    }
    else{
        *PORT_REGs[port] = logic;
    }
    return (Retval);
}

/**
 * @brief           : This function reads logic levels from the port  
 * @param port      : Port index @ref GPIO_Port  
 * @param logic     : Pointer to store the read value  
 * @return          : Status of the function  
 *      (E_OK)      : The function done Successfully  
 *      (E_NOT_OK)  : The function has issue to perform the action  
 */

Std_ReturnType GPIO_Port_Read_Logic(GPIO_Port port, uint8 *logic){
    Std_ReturnType Retval = E_OK;
    if(NULL == logic || port >= Port_NO){
        Retval = E_NOT_OK;
    }
    else{
        *logic = *PIN_REGs[port];
    }
    return (Retval);
}

/**
 * @brief           : This function toggles all pins in the port  
 * @param port      : Port index @ref GPIO_Port  
 * @return          : Status of the function  
 *      (E_OK)      : The function done Successfully  
 *      (E_NOT_OK)  : The function has issue to perform the action  
 */

Std_ReturnType GPIO_Port_Toggle_Logic(GPIO_Port port){
    Std_ReturnType Retval = E_OK;
    if(port >= Port_NO){
        Retval = E_NOT_OK;
    }
    else{
        *PORT_REGs[port] ^= Port_Mask;
    }
    return (Retval);
}

/**
 * @brief           : This function initializes direction and logic level of a port  
 * @param port      : Port index @ref GPIO_Port  
 * @param direction : 8-bit direction value for the port  
 * @param logic     : 8-bit logic value for the port  
 * @return          : Status of the function  
 *      (E_OK)      : The function done Successfully  
 *      (E_NOT_OK)  : The function has issue to perform the action  
 */

Std_ReturnType GPIO_Port_INIT(GPIO_Port port, uint8 direction, uint8 logic){
    Std_ReturnType Retval = E_OK;
    if(port >= Port_NO){
        Retval = E_NOT_OK;
    }
    else{
        Retval = GPIO_Port_Direction_INIT(port, direction);
        Retval = GPIO_Port_Write_Logic(port, logic);
    }
    return (Retval);
}

#endif

/******************* Section 2 :  Helper Functions Definitions *******************/


