/* 
 * File:   ecual_dc_motor.h
 * Author: mohammedel-ahmady
 *
 * Created on July 9, 2025, 10:19 PM
 */

/******************* Section 0 : File Guard *******************/

#ifndef ECUAL_DC_MOTOR_H
#define	ECUAL_DC_MOTOR_H

/******************* Section 1 : Includes *******************/

#include "../../MCAL_Layer/GPIO/hal_gpio.h"
#include "ecual_dc_motor_cfg.h"

/******************* Section 2 : Macros Declarations *******************/

#define DC_Motor_PINS       (uint8)0x02

/******************* Section 3 : Macros Functions Declarations *******************/



/******************* Section 4 : User Defined Data Types & Variables Declarations *******************/

typedef enum{
    Motor_Brake = (uint8)0x00,
    Motor_Rotate_ClockWise = (uint8)0x01,
    Motor_Rotate_Counter_ClockWise = (uint8)0x02
}Motor_State;

typedef struct{
    GPIO_Pin_CFG INs[DC_Motor_PINS];
}DC_Motor;

/******************* Section 5 : Software Interfaces Declarations (APIs) *******************/

Std_ReturnType GPIO_DC_Motor_INIT(const DC_Motor *motor);
Std_ReturnType GPIO_DC_Motor_Read_Rotation_State(const DC_Motor *motor, Motor_State *state);
Std_ReturnType GPIO_DC_Motor_Brake(const DC_Motor *motor);
Std_ReturnType GPIO_DC_Motor_Rotate_ClockWise(const DC_Motor *motor);
Std_ReturnType GPIO_DC_Motor_Rotate_Counter_ClockWise(const DC_Motor *motor);
Std_ReturnType GPIO_DC_Motor_Toggle_Rotation_Direction(const DC_Motor *motor);

#endif	/* ECUAL_DC_MOTOR_H */

