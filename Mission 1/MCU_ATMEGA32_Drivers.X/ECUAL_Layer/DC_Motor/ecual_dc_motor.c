/* 
 * File:   ecual_dc_motor.c
 * Author: mohammedel-ahmady
 *
 * Created on July 9, 2025, 10:19 PM
 */

/******************* Section 0 : Includes *******************/

#include "ecual_dc_motor.h"

/******************* Section 1 :  Variables Definitions *******************/



/******************* Section 2 :  Helper Functions Declarations *******************/



/******************* Section 3 : Software Interfaces Definitions (APIs) *******************/

#if ((GPIO_DC_Motor_CFG_Enable) == (GPIO_DC_Motor_CFG))

/**
 * @brief           : This function initializes the DC motor control pins (IN1, IN2)  
 * @param motor     : Pointer to the DC_Motor configuration structure  
 * @ref             : DC_Motor  
 * @return          : Status of the function  
 *      (E_OK)      : The function executed successfully  
 *      (E_NOT_OK)  : The function failed due to invalid pointer  
 */

Std_ReturnType GPIO_DC_Motor_INIT(const DC_Motor *motor){
    Std_ReturnType Retval = E_OK;
    if(NULL == motor){
        Retval = E_NOT_OK;
    }
    else{
        for(uint8 i = 0; i < DC_Motor_PINS; i++){
            Retval = GPIO_Pin_INIT(&(motor->INs[i]));
        }
    }
    return Retval;
}

/**
 * @brief           : This function reads the current rotation state of the motor  
 * @param motor     : Pointer to the DC_Motor configuration structure  
 * @param state     : Pointer to variable to store the current motor state  
 * @ref             : Motor_State  
 * @return          : Status of the function  
 *      (E_OK)      : The function executed successfully  
 *      (E_NOT_OK)  : The function failed due to invalid pointer or read error  
 */

Std_ReturnType GPIO_DC_Motor_Read_Rotation_State(const DC_Motor *motor, Motor_State *state){
    Std_ReturnType Retval = E_OK;
    if(NULL == motor || NULL == state){
        Retval = E_NOT_OK;
    }
    else{
        GPIO_Logic logic[DC_Motor_PINS];
        for(uint8 i = 0; i < DC_Motor_PINS; i++){
            Retval = GPIO_Pin_Read_Logic(&(motor->INs[i]), &logic[i]);
        }
        if(logic[0] == GPIO_High && logic[1] == GPIO_Low){
            *state = Motor_Rotate_ClockWise;
        }
        else if(logic[0] == GPIO_Low && logic[1] == GPIO_High){
            *state = Motor_Rotate_Counter_ClockWise;
        }
        else{
            *state = Motor_Brake;
        }
    }
    return Retval;
}

/**
 * @brief           : This function stops the motor by applying brake (IN1 = 0, IN2 = 0)  
 * @param motor     : Pointer to the DC_Motor configuration structure  
 * @ref             : DC_Motor  
 * @return          : Status of the function  
 *      (E_OK)      : The function executed successfully  
 *      (E_NOT_OK)  : The function failed due to invalid pointer  
 */

Std_ReturnType GPIO_DC_Motor_Brake(const DC_Motor *motor){
    Std_ReturnType Retval = E_OK;
    if(NULL == motor){
        Retval = E_NOT_OK;
    }
    else{
        GPIO_Logic logic[DC_Motor_PINS] = {GPIO_Low, GPIO_Low};
        for(uint8 i = 0; i < DC_Motor_PINS; i++){
            Retval = GPIO_Pin_Write_Logic(&(motor->INs[i]), logic[i]);
        }
    }
    return Retval;
}

/**
 * @brief           : This function rotates the motor in the clockwise direction (IN1 = 1, IN2 = 0)  
 * @param motor     : Pointer to the DC_Motor configuration structure  
 * @ref             : DC_Motor  
 * @return          : Status of the function  
 *      (E_OK)      : The function executed successfully  
 *      (E_NOT_OK)  : The function failed due to invalid pointer  
 */

Std_ReturnType GPIO_DC_Motor_Rotate_ClockWise(const DC_Motor *motor){
    Std_ReturnType Retval = E_OK;
    if(NULL == motor){
        Retval = E_NOT_OK;
    }
    else{
        GPIO_Logic logic[DC_Motor_PINS] = {GPIO_High, GPIO_Low};
        for(uint8 i = 0; i < DC_Motor_PINS; i++){
            Retval = GPIO_Pin_Write_Logic(&(motor->INs[i]), logic[i]);
        }
    }
    return Retval;
}

/**
 * @brief           : This function rotates the motor in the counter-clockwise direction (IN1 = 0, IN2 = 1)  
 * @param motor     : Pointer to the DC_Motor configuration structure  
 * @ref             : DC_Motor  
 * @return          : Status of the function  
 *      (E_OK)      : The function executed successfully  
 *      (E_NOT_OK)  : The function failed due to invalid pointer  
 */

Std_ReturnType GPIO_DC_Motor_Rotate_Counter_ClockWise(const DC_Motor *motor){
    Std_ReturnType Retval = E_OK;
    if(NULL == motor){
        Retval = E_NOT_OK;
    }
    else{
        GPIO_Logic logic[DC_Motor_PINS] = {GPIO_Low, GPIO_High};
        for(uint8 i = 0; i < DC_Motor_PINS; i++){
            Retval = GPIO_Pin_Write_Logic(&(motor->INs[i]), logic[i]);
        }
    }
    return Retval;
}

/**
 * @brief           : This function toggles the motor rotation direction (CW ? CCW)  
 * @param motor     : Pointer to the DC_Motor configuration structure  
 * @ref             : DC_Motor  
 * @return          : Status of the function  
 *      (E_OK)      : The function executed successfully  
 *      (E_NOT_OK)  : The function failed due to invalid pointer or read error  
 */

Std_ReturnType GPIO_DC_Motor_Toggle_Rotation_Direction(const DC_Motor *motor){
    Std_ReturnType Retval = E_OK;
    if(NULL == motor){
        Retval = E_NOT_OK;
    }
    else{
        Motor_State state;
        Retval = GPIO_DC_Motor_Read_Rotation_State(motor, &state);
        if(Motor_Rotate_ClockWise == state){
            Retval = GPIO_DC_Motor_Rotate_Counter_ClockWise(motor);
        }
        else{
            Retval = GPIO_DC_Motor_Rotate_ClockWise(motor);
        }
    }
    return Retval;
}


#endif  

/******************* Section 2 :  Helper Functions Definitions *******************/


