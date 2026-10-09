/* 
 * File:   ecual_keypad.c
 * Author: mohammedel-ahmady
 *
 * Created on July 14, 2025, 12:46 PM
 */

/******************* Section 0 : Includes *******************/

#include "ecual_keypad.h"

/******************* Section 1 :  Variables Definitions *******************/

#if ((Keypad_Phone) == (Keypad_Type))

const uint8 Keypad_Btns[Keypad_Row][Keypad_Col]  = {{'1','2','3'},
                                                    {'4','5','6'},
                                                    {'7','8','9'},
                                                    {'*','0','#'}};

#elif ((Keypad_Small_Calc) == (Keypad_Type))

const uint8 Keypad_Btns[Keypad_Row][Keypad_Col] = {{'7','8','9','/'},
                                                   {'4','5','6','*'},
                                                   {'1','2','3','-'},
                                                   {'C','0','=','+'}};

#elif ((Keypad_Calculator) == (Keypad_Type))

const uint8 Keypad_Btns[Keypad_Row][Keypad_Col]  = {{'C','7','8','9','*','/'},
                                                    {'±','4','5','6','-','R'},
                                                    {'%','1','2','3','+','N'},
                                                    {'?','0','.','=','+','P'}};

#endif

/******************* Section 2 :  Helper Functions Declarations *******************/



/******************* Section 3 : Software Interfaces Definitions (APIs) *******************/

#if ((GPIO_Keypad_CFG_Enable) == (GPIO_Keypad_CFG))

Std_ReturnType GPIO_Keypad_INIT(const GPIO_Keypad *keypad){
    Std_ReturnType Retval = E_OK;
    if(NULL == keypad){
        Retval = E_NOT_OK;
    }
    else{
        for(uint8 i = Zero_Init; i < Keypad_Row; i++){
            Retval = GPIO_Pin_INIT(&(keypad->Row_Pins[i]));
        }
        
        for(uint8 j = Zero_Init; j < Keypad_Col; j++){
            Retval = GPIO_Pin_Direction_INIT(&(keypad->Col_Pins[j]));
        }
    }
    return Retval;
}

Std_ReturnType GPIO_Keypad_Read_Data(const GPIO_Keypad *keypad, uint8 *data){
    Std_ReturnType Retval = E_OK;
    if(NULL == keypad || NULL == data){
        Retval = E_NOT_OK;
    }
    else{
        GPIO_Logic Deactivation_Pin = GPIO_Low;
        GPIO_Logic Activation_Pin  = GPIO_High;
        GPIO_Logic Read_Pin;
        for(uint8 i = Zero_Init; i < Keypad_Row; i++){
            for(uint8 j = Zero_Init; j < Keypad_Row; j++){
                Retval = GPIO_Pin_Write_Logic(&(keypad->Row_Pins[j]), Deactivation_Pin);
            }
            
            Retval = GPIO_Pin_Write_Logic(&(keypad->Row_Pins[i]), Activation_Pin);
            _delay_ms(10);
            
            for(uint8 k = Zero_Init; k < Keypad_Col; k++){
                Retval = GPIO_Pin_Read_Logic(&(keypad->Col_Pins[k]), &Read_Pin);
                
                if(GPIO_High == Read_Pin){
                    *data = Keypad_Btns[i][k];
                    break;
                }
            }
        }
        
    }
    return Retval;
}

#endif

/******************* Section 4:  Helper Functions Definitions *******************/