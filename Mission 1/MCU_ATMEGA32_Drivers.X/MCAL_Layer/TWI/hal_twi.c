/* 
 * File:   hal_twi.c
 * Author: mohammedel-ahmady
 *
 * Created on October 9, 2025, 1:56 PM
 */

/******************* Section 0 : Includes *******************/

#include "hal_twi.h"

/******************* Section 1 :  Variables Definitions *******************/



/******************* Section 2 :  Helper Functions Declarations *******************/

#if((Interrupt_Feature_Enable) == (TWI_Interrupt_Feature))
TWI_Handler TWI_Handler_Function = NULL;
#endif

Std_ReturnType TWI_Set_Disable(const TWI_CFG *twi);
Std_ReturnType TWI_Set_Enable(const TWI_CFG *twi);
Std_ReturnType TWI_Set_Mode_Configuration(const TWI_CFG *twi);
Std_ReturnType TWI_Pins_Init(const TWI_CFG *twi);

#if((Interrupt_Feature_Enable) == (TWI_Interrupt_Feature))
Std_ReturnType TWI_Configure_Interrupt(const TWI_CFG *twi);
#endif
/******************* Section 3 : Software Interfaces Definitions (APIs) *******************/

Std_ReturnType TWI_INIT(const TWI_CFG *twi){
    Std_ReturnType Retval = E_OK;
    if(NULL == twi){
        Retval = E_NOT_OK;
    }
    else{
        // 1. Disable TWI Module
        Retval = TWI_Set_Disable(twi);
        // 2. Set TWI Mode Configurations
        Retval = TWI_Set_Mode_Configuration(twi);
        // 3. TWI Pins initialization
        Retval = TWI_Pins_Init(twi);
        // 4. TWI Configure Interrupt
        Retval = TWI_Configure_Interrupt(twi);
        // 2. Enable TWI Module
        Retval = TWI_Set_Enable(twi);
    }
    return Retval;
}

Std_ReturnType TWI_DEINIT(const TWI_CFG *twi){
    Std_ReturnType Retval = E_OK;
    if(NULL == twi){
        Retval = E_NOT_OK;
    }
    else{
        // 1. Disable TWI Module
        Retval = TWI_Set_Disable(&twi);
        // 2. Disable TWI Interrupt
        Internal_Interrupt_TWI_Disable();
        // 3. Clear TWI Interrupt Flag
        Internal_Interrupt_TWI_Clear_Flag();
    }
    return Retval;
}


Std_ReturnType TWI_Master_Send_Start_Condition(const TWI_CFG *twi){
    Std_ReturnType Retval = E_OK;
    if(NULL == twi){
        Retval = E_NOT_OK;
    }
    else{
        // 1. Clear Flag and Set Start Condition Bit
        Internal_Interrupt_TWI_Clear_Flag();
        TWI_Master_Send_Start();
//        TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
        // 2. Wait until start bit transmitted
        while(!TWI_Read_Transmision_Receiving_State());
        if((TWSR & 0xF8) != MTR_S_Condition){
            Retval = E_NOT_OK;
        }
        else{/* Nothing */}
    }
    return Retval;
}

Std_ReturnType TWI_Master_Send_Stop_Condition(const TWI_CFG *twi){
    Std_ReturnType Retval = E_OK;
    if(NULL == twi){
        Retval = E_NOT_OK;
    }
    else{
        // 2. Clear Flag and Set Start Condition Bit
        Internal_Interrupt_TWI_Clear_Flag();
        TWI_Master_Send_Stop();
//        TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);

    }
    return Retval;
}

Std_ReturnType TWI_Master_Send_Repeated_Start_Condition(const TWI_CFG *twi){
    Std_ReturnType Retval = E_OK;
    if(NULL == twi){
        Retval = E_NOT_OK;
    }
    else{
        // 1. Clear Flag and Set Start Condition Bit
        Internal_Interrupt_TWI_Clear_Flag();
        TWI_Master_Send_Start();
//        TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
        // 2. Wait until start bit transmitted
        while(!TWI_Read_Transmision_Receiving_State());
        if((TWSR & 0xF8) != MTR_Sr_Condition){
            Retval = E_NOT_OK;
        }
        else{/* Nothing */}
    }
    return Retval;
}


Std_ReturnType TWI_Write_Byte_Blocking(const TWI_CFG *twi, uint8 data){
    Std_ReturnType Retval = E_OK;
    if(NULL == twi){
        Retval = E_NOT_OK;
    }
    else{
        // 1. Write Data in TWI Data Register
        TWDR = data;
        // 2. Wait until Write Completed
        while(!TWI_Read_Transmision_Receiving_State());
        // 3. Clear Flag
        Internal_Interrupt_TWI_Clear_Flag();
        switch(twi->mode){
            case TWI_Master:
                if(((TWSR & 0xF8) == MT_SLA_W_ACK) || ((TWSR & 0xF8) == MT_DT_ACK)){/* Nothing */}
                else{
                    Retval = E_NOT_OK;
                }
                break;
            case TWI_Slave:
                if((TWSR & 0xF8) == ST_DT_ACK){/* Nothing */}
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

Std_ReturnType TWI_Write_String_Blocking(const TWI_CFG *twi, uint8 *str){
    Std_ReturnType Retval = E_OK;
    if(NULL == twi || NULL == str){
        Retval = E_NOT_OK;
    }
    else{
        while(*str){
            Retval = TWI_Write_Byte_Blocking(twi, *str);
            str++;
        }
    }
    return Retval;
}

Std_ReturnType TWI_Read_Byte_Blocking(const TWI_CFG *twi, uint8 *data){
    Std_ReturnType Retval = E_OK;
    if(NULL == twi || NULL == data){
        Retval = E_NOT_OK;
    }
    else{
        // 1. Wait until Last Receiving Completed
        while(!TWI_Read_Transmision_Receiving_State());
        // 2. Receive Data in TWI Data Register
        *data = TWDR;
        // 3. Clear Flag
        Internal_Interrupt_TWI_Clear_Flag();
        switch(twi->mode){
            case TWI_Master:
                if(((TWSR & 0xF8) == MR_SLA_W_ACK) || ((TWSR & 0xF8) == MR_DT_ACK)){/* Nothing */}
                else{
                    Retval = E_NOT_OK;
                }
                break;
            case TWI_Slave:
                if((TWSR & 0xF8) == SR_DT_ACK){/* Nothing */}
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

Std_ReturnType TWI_Send_Frame(const TWI_CFG *twi, uint8 add, uint8 data){
    Std_ReturnType Retval = E_OK;
    if(NULL == twi){
        Retval = E_NOT_OK;
    }
    else{
        Retval = TWI_Master_Send_Start_Condition(twi);
        Retval = TWI_Write_Byte_Blocking(twi, add);
        Retval = TWI_Write_Byte_Blocking(twi, data);
        Retval = TWI_Master_Send_Stop_Condition(twi);
    }
    return Retval;
}

/******************* Section 4:  Helper Functions Definitions *******************/

Std_ReturnType TWI_Set_Disable(const TWI_CFG *twi){
    Std_ReturnType Retval = E_OK;
    if(NULL == twi){
        Retval = E_NOT_OK;
    }
    else{
        TWI_Module_Disable();
    }
    return Retval;
}

Std_ReturnType TWI_Set_Enable(const TWI_CFG *twi){
    Std_ReturnType Retval = E_OK;
    if(NULL == twi){
        Retval = E_NOT_OK;
    }
    else{
        TWI_Module_Enable();
    }
    return Retval;
}

Std_ReturnType TWI_Set_Mode_Configuration(const TWI_CFG *twi){
    Std_ReturnType Retval = E_OK;
    if(NULL == twi){
        Retval = E_NOT_OK;
    }
    else{
        switch(twi->mode){
            case TWI_Master:
                // 1. Set Prescaler
                TWI_Master_Set_Prescaler(twi->mst_prescaler);
                // 2. Set Clock
                TWBR = (((F_CPU / (twi->mst_clk)) - 16.0) / (2.0 * pow(4, (TWSR & 0x03))));
                break;
            case TWI_Slave:
                // 1. Set Slave Address
                TWAR = ((twi->slv_address) & 0xFE);
                // 2. Disable General Call Initially
                TWI_Slave_Receive_General_Call_Disable();
                break;
            default :
                Retval = E_NOT_OK;
                break;
        }
    }
    return Retval;
}

Std_ReturnType TWI_Pins_Init(const TWI_CFG *twi){
    Std_ReturnType Retval = E_OK;
    if(NULL == twi){
        Retval = E_NOT_OK;
    }
    else{
        GPIO_Pin_CFG SCL_pin = {.Port = GPIO_PortC, .Pin = GPIO_Pin0, .Direction = GPIO_Input};
        GPIO_Pin_CFG SDA_pin = {.Port = GPIO_PortC, .Pin = GPIO_Pin1, .Direction = GPIO_Input};
        
        GPIO_Pin_Direction_INIT(&SCL_pin);
        GPIO_Pin_Direction_INIT(&SDA_pin);
    }
    return Retval;
}

#if((Interrupt_Feature_Enable) == (TWI_Interrupt_Feature))

Std_ReturnType TWI_Configure_Interrupt(const TWI_CFG *twi){
    Std_ReturnType Retval = E_OK;
    if(NULL == twi){
        Retval = E_NOT_OK;
    }
    else{
        Internal_Interrupt_TWI_Enable();
        Internal_Interrupt_TWI_Clear_Flag();
        GLobal_Interrupt_Enable_SREG();
        TWI_Handler_Function = twi->TWI_Interrupt;
    }
    return Retval;
}

#endif