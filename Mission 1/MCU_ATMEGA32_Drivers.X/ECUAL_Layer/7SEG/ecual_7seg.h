/* 
 * File:   ecual_7seg.h
 * Author: mohammedel-ahmady
 *
 * Created on July 10, 2025, 8:34 PM
 */

/******************* Section 0 : File Guard *******************/

#ifndef ECUAL_7SEG_H
#define	ECUAL_7SEG_H

/******************* Section 1 : Includes *******************/

#include "../../MCAL_Layer/GPIO/hal_gpio.h"
#include "ecual_7seg_cfg.h"

/******************* Section 2 : Macros Declarations *******************/

#define Segment_Pin0            (uint8)0x00
#define Segment_Pin1            (uint8)0x01
#define Segment_Pin2            (uint8)0x02
#define Segment_Pin3            (uint8)0x03

#define Segment_Pins            (uint8)0x04

#define Max_Segment_Number      (uint8)0x0A

/******************* Section 3 : Macros Functions Declarations *******************/



/******************* Section 4 : User Defined Data Types & Variables Declarations *******************/

typedef enum{
    Segment_Common_Cathode = (uint8)0x00,
    Segment_Common_Anode   = (uint8)0x01
}Segment_Con;

typedef struct{
    GPIO_Pin_CFG pins[Segment_Pins];
    Segment_Con          connection;
}GPIO_Segment;

/******************* Section 5 : Software Interfaces Declarations (APIs) *******************/

Std_ReturnType GPIO_Segment_INIT(const GPIO_Segment *seg);
Std_ReturnType GPIO_Segment_Read_Number(const GPIO_Segment *seg, uint8 *number);
Std_ReturnType GPIO_Segment_Write_Number(const GPIO_Segment *seg, uint8 number);

#endif	/* ECUAL_7SEG_H */

