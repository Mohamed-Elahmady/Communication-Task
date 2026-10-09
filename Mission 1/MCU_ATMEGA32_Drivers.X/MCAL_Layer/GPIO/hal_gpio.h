/* 
 * File:   hal_gpio.h
 * Author: mohammedel-ahmady
 *
 * Created on June 26, 2025, 8:03 PM
 */

/******************* Section 0 : File Guard *******************/

#ifndef HAL_GPIO_H
#define	HAL_GPIO_H

/******************* Section 1 : Includes *******************/

#include "../mcal_std_types.h"
#include "hal_gpio_cfg.h"
#include "../device_cfg.h"

/******************* Section 2 : Macros Declarations *******************/

#define Pin_NO                              (uint8)0x08
#define Port_NO                             (uint8)0x04
#define REG_NO                              (uint8)0x04

/******************* Section 3 : Macros Functions Declarations *******************/



/******************* Section 4 : User Defined Data Types & Variables Declarations *******************/

extern volatile uint8 *DDR_REGs[REG_NO];
extern volatile uint8 *PIN_REGs[REG_NO];
extern volatile uint8 *PORT_REGs[REG_NO];

typedef enum{
    GPIO_PortA  =     (uint8)0x00,
    GPIO_PortB  =     (uint8)0x01,
    GPIO_PortC  =     (uint8)0x02,
    GPIO_PortD  =     (uint8)0x03
}GPIO_Port;

typedef enum{
    GPIO_Pin0   =     (uint8)0x00,
    GPIO_Pin1   =     (uint8)0x01,
    GPIO_Pin2   =     (uint8)0x02,
    GPIO_Pin3   =     (uint8)0x03,
    GPIO_Pin4   =     (uint8)0x04,
    GPIO_Pin5   =     (uint8)0x05,
    GPIO_Pin6   =     (uint8)0x06,
    GPIO_Pin7   =     (uint8)0x07
}GPIO_Pin;

typedef enum{
    GPIO_Low   =      (uint8)0x00,
    GPIO_High   =     (uint8)0x01
}GPIO_Logic;

typedef enum{
    GPIO_Input   =    (uint8)0x00,
    GPIO_Output   =   (uint8)0x01
}GPIO_Direction;

typedef struct{
    uint8 Port             :3;
    uint8 Pin              :3;
    uint8 Direction        :1;
    uint8 Logic            :1;
}GPIO_Pin_CFG;

/******************* Section 5 : Software Interfaces Declarations (APIs) *******************/

Std_ReturnType GPIO_Pin_Direction_INIT(const GPIO_Pin_CFG *pin);
Std_ReturnType GPIO_Pin_Get_Direction_Status(const GPIO_Pin_CFG *pin, GPIO_Direction *direction);
Std_ReturnType GPIO_Pin_Write_Logic(const GPIO_Pin_CFG *pin, GPIO_Logic logic);
Std_ReturnType GPIO_Pin_Read_Logic(const GPIO_Pin_CFG *pin, GPIO_Logic *logic);
Std_ReturnType GPIO_Pin_Toggle_Logic(const GPIO_Pin_CFG *pin);
Std_ReturnType GPIO_Pin_INIT(const GPIO_Pin_CFG *pin);

Std_ReturnType GPIO_Port_Direction_INIT(GPIO_Port port, uint8 direction);
Std_ReturnType GPIO_Port_Get_Direction_Status(GPIO_Port port, uint8 *direction);
Std_ReturnType GPIO_Port_Write_Logic(GPIO_Port port, uint8 logic);
Std_ReturnType GPIO_Port_Read_Logic(GPIO_Port port, uint8 *logic);
Std_ReturnType GPIO_Port_Toggle_Logic(GPIO_Port port);
Std_ReturnType GPIO_Port_INIT(GPIO_Port port, uint8 direction, uint8 logic);

#endif	/* HAL_GPIO_H */