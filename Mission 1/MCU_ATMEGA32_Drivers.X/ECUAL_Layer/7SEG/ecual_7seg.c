/* 
 * File:   ecual_7seg.h
 * Author: mohammedel-ahmady
 *
 * Created on July 10, 2025, 8:34 PM
 */


/******************* Section 0 : Includes *******************/

#include "ecual_7seg.h"

/******************* Section 1 :  Variables Definitions *******************/



/******************* Section 2 :  Helper Functions Declarations *******************/



/******************* Section 3 : Software Interfaces Definitions (APIs) *******************/

#if ((GPIO_Segment_CFG_Enable) == (GPIO_Segment_CFG))

Std_ReturnType GPIO_Segment_INIT(const GPIO_Segment *seg){
    Std_ReturnType Retval = E_OK;
    if(NULL == seg){
        Retval = E_NOT_OK;
    }
    else{
        for(uint8 i = 0; i < Segment_Pins; i++){
            Retval = GPIO_Pin_INIT(&(seg->pins[i]));
        }
    }
    return Retval;
}

Std_ReturnType GPIO_Segment_Read_Number(const GPIO_Segment *seg, uint8 *number){
    Std_ReturnType Retval = E_OK;
    if(NULL == seg || NULL == number){
        Retval = E_NOT_OK;
    }
    else{
        GPIO_Logic logic;
        for(uint8 i = 0; i < Segment_Pins; i++){
            Retval = GPIO_Pin_Read_Logic(&(seg->pins[i]), &logic);
            *number |= ((uint8)logic << i);
        }
        if(*number >= Max_Segment_Number){
            Retval = E_NOT_OK;
        }
    }
    return Retval;
}

Std_ReturnType GPIO_Segment_Write_Number(const GPIO_Segment *seg, uint8 number){
    Std_ReturnType Retval = E_OK;
    if(NULL == seg || number >= Max_Segment_Number){
        Retval = E_NOT_OK;
    }
    else{
        for(uint8 i = 0; i < Segment_Pins; i++){
            Retval = GPIO_Pin_Write_Logic(&(seg->pins[i]), (GPIO_Logic)Read_Bit(number, i));
        }
    }
    return Retval;
}

#endif

/******************* Section 2 :  Helper Functions Definitions *******************/


