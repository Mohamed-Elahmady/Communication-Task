/* 
 * File:   hal_usart.c
 * Author: mohammedel-ahmady
 *
 * Created on September 22, 2025, 8:17 PM
 */

/******************* Section 0 : Includes *******************/

#include "hal_usart.h"

/******************* Section 1 :  Variables Definitions *******************/



/******************* Section 2 :  Helper Functions Declarations *******************/

#if (((Interrupt_Feature_Enable)) == (USART_Interrupt_Feature))

static USART_Handler Tx_Handler_Function = NULL;
static USART_Handler TSR_Handler_Function = NULL;
static USART_Handler Rx_Handler_Function = NULL;
static USART_Handler Framming_Handler_Function = NULL;
static USART_Handler Overrun_Handler_Function = NULL;

#endif

static Std_ReturnType USART_Set_Mode_Configurations(const USART_CFG *usart);
static Std_ReturnType USART_Set_Baud_Rate_Configurations(const USART_CFG *usart);
static Std_ReturnType USART_Set_TX_Configurations(const USART_CFG *usart);
static Std_ReturnType USART_Set_RX_Configurations(const USART_CFG *usart);

/******************* Section 3 : Software Interfaces Definitions (APIs) *******************/

Std_ReturnType USART_INIT(const USART_CFG *usart){
    Std_ReturnType Retval = E_OK;
    if(NULL == usart){
        Retval = E_NOT_OK;
    }
    else{
        // 1. Set USART_Mode & Frame Configuration
        Retval = USART_Set_Mode_Configurations(usart);
        // 2. Set Baud Rate Configuration
        Retval = USART_Set_Baud_Rate_Configurations(usart);
        // 3. Set Tx Configurations & Interrupts
        Retval = USART_Set_TX_Configurations(usart);
        // 4. Set Rx Configurations & Interrupts
        Retval = USART_Set_RX_Configurations(usart);
    }
    return Retval;
}

Std_ReturnType USART_DEINIT(const USART_CFG *usart){
    Std_ReturnType Retval = E_OK;
    if(NULL == usart){
        Retval = E_NOT_OK;
    }
    else{
        // 1. Disable Transmission Functionality
        USART_Transmission_Disable();
        // 1. Disable Receiving Functionality
        USART_Receiving_Disable();
        // 3. Disable Interrupt if used
#if (((Interrupt_Feature_Enable)) == (USART_Interrupt_Feature))
        Internal_Interrupt_TX_Clear_Flag();
        Internal_Interrupt_TX_Disable();
        Internal_Interrupt_TSR_Disable();
        Internal_Interrupt_RX_Disable();
#endif
    }
    return Retval;
}

Std_ReturnType USART_Tx_Write_Byte_Non_Blocking(const USART_CFG *usart, uint16 data){
    Std_ReturnType Retval = E_OK;
    if(NULL == usart){
        Retval = E_NOT_OK;
    }
    else{
        switch(usart->data_size){
            case USART_Data_5Bit:
                if(USART_Read_Transmission_Shift_Register_Status()){
                    UDR = (uint8)(data & 0x1F);
                }
                else{/* Nothing */}
                break;
            case USART_Data_6Bit:
                if(USART_Read_Transmission_Shift_Register_Status()){
                    UDR = (uint8)(data & 0x3F);
                }
                else{/* Nothing */}
                break;
            case USART_Data_7Bit:
                if(USART_Read_Transmission_Shift_Register_Status()){
                    UDR = (uint8)(data & 0x7F);
                }
                else{/* Nothing */}
                break;
            case USART_Data_8Bit:
                if(USART_Read_Transmission_Shift_Register_Status()){
                    UDR = (uint8)(data & 0xFF);
                }
                else{/* Nothing */}
                break;
            case USART_Data_9Bit:
                if(USART_Read_Transmission_Shift_Register_Status()){
                    USART_Transmit_Ninth_Bit_Data(data);
                    UDR = (uint8)(data & 0xFF);
                }
                else{/* Nothing */}
                break;
            default :
                Retval = E_NOT_OK;
                break;
        }
    }
    return Retval;
}

Std_ReturnType USART_Tx_Write_String_Non_Blocking(const USART_CFG *usart, uint8 *str){
    Std_ReturnType Retval = E_OK;
    if(NULL == usart || NULL == str){
        Retval = E_NOT_OK;
    }
    else{
        while(*str){
            while(!USART_Read_Transmission_Shift_Register_Status());
            Retval = USART_Tx_Write_Byte_Non_Blocking(usart, *str);
            str++;
        }
    }
    return Retval;
}

Std_ReturnType USART_Tx_Write_Byte_Blocking(const USART_CFG *usart, uint16 data){
    Std_ReturnType Retval = E_OK;
    if(NULL == usart){
        Retval = E_NOT_OK;
    }
    else{
        switch(usart->data_size){
            case USART_Data_5Bit:
                while(!USART_Read_Transmission_Shift_Register_Status());
                UDR = (uint8)(data & 0x1F);
                break;
            case USART_Data_6Bit:
                while(!USART_Read_Transmission_Shift_Register_Status());
                UDR = (uint8)(data & 0x3F);
                break;
            case USART_Data_7Bit:
                while(!USART_Read_Transmission_Shift_Register_Status());
                UDR = (uint8)(data & 0x7F);
                break;
            case USART_Data_8Bit:
                while(!USART_Read_Transmission_Shift_Register_Status());
                UDR = (uint8)(data & 0xFF);
                break;
            case USART_Data_9Bit:
                while(!USART_Read_Transmission_Shift_Register_Status());
                USART_Transmit_Ninth_Bit_Data(data);
                UDR = (uint8)(data & 0xFF);
                break;
            default :
                Retval = E_NOT_OK;
                break;
        }
    }
    return Retval;
}

Std_ReturnType USART_Tx_Write_String_Blocking(const USART_CFG *usart, uint8 *str){
    Std_ReturnType Retval = E_OK;
    if(NULL == usart || NULL == str){
        Retval = E_NOT_OK;
    }
    else{
        while(*str){
            Retval = USART_Tx_Write_Byte_Blocking(usart, *str);
            str++;
        }
    }
    return Retval;
}

Std_ReturnType USART_Rx_Restart(const USART_CFG *usart){
    Std_ReturnType Retval = E_OK;
    if(NULL == usart){
        Retval = E_NOT_OK;
    }
    else{
        USART_Receiving_Disable();
        USART_Receiving_Enable();
    }
    return Retval;
}

Std_ReturnType USART_Rx_Read_Byte_Non_Blocking(const USART_CFG *usart, uint16 *data){
    Std_ReturnType Retval = E_OK;
    if(NULL == usart || NULL == data){
        Retval = E_NOT_OK;
    }
    else{
        switch(usart->data_size){
            case USART_Data_5Bit:
                if(USART_Read_Receiving_Shift_Register_Status()){
                    *data = (uint8)(UDR & 0x1F);
                }
                else{/* Nothing */}
                break;
            case USART_Data_6Bit:
                if(USART_Read_Receiving_Shift_Register_Status()){
                    *data = (uint8)(UDR & 0x3F);
                }
                else{/* Nothing */}
                break;
            case USART_Data_7Bit:
                if(USART_Read_Receiving_Shift_Register_Status()){
                    *data = (uint8)(UDR & 0x7F);
                }
                else{/* Nothing */}
                break;
            case USART_Data_8Bit:
                if(USART_Read_Receiving_Shift_Register_Status()){
                    *data = (uint8)(UDR & 0xFF);
                }
                else{/* Nothing */}
                break;
            case USART_Data_9Bit:
                if(USART_Read_Receiving_Shift_Register_Status()){
                    *data = (uint8)(USART_Read_Ninth_Bit_Data() << 8);
                    *data |= (uint8)(UDR & 0xFF);
                }
                else{/* Nothing */}
                break;
            default :
                Retval = E_NOT_OK;
                break;
        }
        
        if(USART_Async_Read_Parity_Error_Status() || USART_Async_Framing_Error_Status() ||
           USART_Read_Data_OverrRun_Error_Status()){
            Retval = USART_Rx_Restart(usart);
            Retval = E_NOT_OK;
        }
        else{/* Nothing */}
    }
    return Retval;
}

Std_ReturnType USART_Rx_Read_Byte_Blocking(const USART_CFG *usart, uint16 *data){
    Std_ReturnType Retval = E_OK;
    if(NULL == usart || NULL == data){
        Retval = E_NOT_OK;
    }
    else{
        switch(usart->data_size){
            case USART_Data_5Bit:
                while(!USART_Read_Receiving_Shift_Register_Status());
                *data = (uint8)(UDR & 0x1F);
                break;
            case USART_Data_6Bit:
                while(!USART_Read_Receiving_Shift_Register_Status());
                *data = (uint8)(UDR & 0x3F);
                break;
            case USART_Data_7Bit:
                while(!USART_Read_Receiving_Shift_Register_Status());
                *data = (uint8)(UDR & 0x7F);
                break;
            case USART_Data_8Bit:
                while(!USART_Read_Receiving_Shift_Register_Status());
                *data = (uint8)(UDR & 0xFF);
                break;
            case USART_Data_9Bit:
                while(!USART_Read_Receiving_Shift_Register_Status());
                *data = (uint8)(USART_Read_Ninth_Bit_Data() << 8);
                *data |= (uint8)(UDR & 0xFF);
                break;
            default :
                Retval = E_NOT_OK;
                break;
        }
        
        if(USART_Async_Read_Parity_Error_Status() || USART_Async_Framing_Error_Status() ||
           USART_Read_Data_OverrRun_Error_Status()){
            Retval = USART_Rx_Restart(usart);
            Retval = E_NOT_OK;
        }
        else{/* Nothing */}
    }
    return Retval;
}

#if ((Interrupt_Feature_Enable) == (USART_Interrupt_Feature))

void USART_TX_ISR(void){
    // 1. Clear Flag
    Internal_Interrupt_TX_Clear_Flag();
    // 2. Code
    // 3. TX Callback Function
    if(Tx_Handler_Function){
        Tx_Handler_Function();
    }
    else{/* Nothing */}
}

void USART_TSR_ISR(void){
    // 1. Code
    // 2. TX Callback Function
    if(TSR_Handler_Function){
        TSR_Handler_Function();
    }
    else{/* Nothing */}
}

void USART_RX_ISR(void){
    // 1. Code
    // 2. RX Callback Function
    if(Rx_Handler_Function){
        Rx_Handler_Function();
    }
    else{/* Nothing */}
    if(Framming_Handler_Function){
        Framming_Handler_Function();
    }
    else{/* Nothing */}
    if(Overrun_Handler_Function){
        Overrun_Handler_Function();
    }
    else{/* Nothing */}
}

#endif

/******************* Section 4:  Helper Functions Definitions *******************/

static Std_ReturnType USART_Set_Mode_Configurations(const USART_CFG *usart){
    Std_ReturnType Retval = E_OK;
    if(NULL == usart){
        Retval = E_NOT_OK;
    }
    else{
        switch(usart->mode){
            case USART_Asynchronous:
                // 1. Set Mode
                USART_Asynchronous_Mode();
                
                // 1.1. Set Data type "Data & address or address only"
                if(usart->data_classification == USART_Transmit_Receive_Data_Address){
                    USART_Async_Send_Data_Address_Frame();
                }
                else if(usart->data_classification == USART_Transmit_Receive_Address){
                    USART_Async_Send_Address_Frame_Only();
                }
                else{
                    Retval = E_NOT_OK;
                }
                
                // 1.2. Set Data Bits Size
                // 1.2.1. Set UCSRC Register to write
                USART_Select_UCSRC_Register();
                // 1.2.2. Set Data Bits Size
                if(usart->data_size == USART_Data_5Bit){    
                    Clear_Bit(UCSRC, UCSZ0);
                    Clear_Bit(UCSRC, UCSZ1);
                    Clear_Bit(UCSRB, UCSZ2);
                }
                else if(usart->data_size == USART_Data_6Bit){
                    Set_Bit(UCSRC, UCSZ0);
                    Clear_Bit(UCSRC, UCSZ1);
                    Clear_Bit(UCSRB, UCSZ2);
                }
                else if(usart->data_size == USART_Data_7Bit){
                    Clear_Bit(UCSRC, UCSZ0);
                    Set_Bit(UCSRC, UCSZ1);
                    Clear_Bit(UCSRB, UCSZ2);
                }
                else if(usart->data_size == USART_Data_8Bit){
                    Set_Bit(UCSRC, UCSZ0);
                    Set_Bit(UCSRC, UCSZ1);
                    Clear_Bit(UCSRB, UCSZ2);
                }
                else if(usart->data_size == USART_Data_9Bit){
                    Set_Bit(UCSRC, UCSZ0);
                    Set_Bit(UCSRC, UCSZ1);
                    Set_Bit(UCSRB, UCSZ2);
                }
                else{
                    Retval = E_NOT_OK;
                }
                
                // 1.3. Set Parity Mode
                if(usart->parity == USART_Even_Parity){
                    USART_Async_Even_Parity_Mode();
                }
                else if(usart->parity == USART_Odd_Parity){
                    USART_Async_Odd_Parity_Mode();
                }
                else if(usart->parity == USART_Disable_Parity){
                    USART_Async_Disable_Parity_Mode();
                }
                else{
                    Retval = E_NOT_OK;
                }
                
                // 1.4. Set Stop Bits
                if(usart->stop_bits == USART_1Stop_Bit){
                    USART_Async_Set_1Stop_Bit();
                }
                else if(usart->stop_bits == USART_2Stop_Bit){
                    USART_Async_Set_2Stop_Bit();
                }
                else{
                    Retval = E_NOT_OK;
                }
                
                break;
            case USART_Synchronous:
                // 2. Set Mode
                USART_Synchronous_Mode();
                
                // 2.1 Set Clock edges
                if(usart->clk_edge == USART_Sync_Rising_Tx_Falling_Rx){
                    USART_Sync_Sample_On_Rising();
                }
                else if(usart->clk_edge == USART_Sync_Rising_Rx_Falling_Tx){
                    USART_Sync_Sample_On_Falling();
                }
                else{
                    Retval = E_NOT_OK;
                }
                
                // 2.2 Set Sync communication type
                if(usart->com_mode == USART_Sync_Master_Communication){
                    GPIO_Pin_CFG XCK_Pin = {.Port = GPIO_PortB, .Pin = GPIO_Pin0, .Direction = GPIO_Output, .Logic = GPIO_Low};
                    Retval = GPIO_Pin_INIT(&XCK_Pin);
                }
                else if(usart->com_mode == USART_Sync_Slave_Communication){
                    GPIO_Pin_CFG XCK_Pin = {.Port = GPIO_PortB, .Pin = GPIO_Pin0, .Direction = GPIO_Input};
                    Retval = GPIO_Pin_INIT(&XCK_Pin);
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

static Std_ReturnType USART_Set_Baud_Rate_Configurations(const USART_CFG *usart){
    Std_ReturnType Retval = E_OK;
    if(NULL == usart){
        Retval = E_NOT_OK;
    }
    else{
        float32 Baud_Rate_Temp = 0;
        switch(usart->mode){
            // 1. Set Mode
            case USART_Asynchronous:
                // 1.1. Set Sampling Rate
                if(usart->br_speed == USART_Baud_Rate_X1Doubling){
                    USART_Async_Baud_Rate_X1_Doubling();
                    Baud_Rate_Temp = (float32)(((F_CPU)/(16.0 * (usart->baud_rate))) - 1);
                }
                else if(usart->br_speed == USART_Baud_Rate_X2Doubling){
                    USART_Async_Baud_Rate_X2_Doubling();
                    Baud_Rate_Temp = (float32)(((F_CPU)/(8.0 * (usart->baud_rate))) - 1);
                }
                else{
                    Retval = E_NOT_OK;
                }
                
                // 1.2. Set UBRRH Register Address
                USART_Select_UBRRH_Register();
                
                // 1.3. Set Baud Rate in UBRR Registers
                UBRRL = (uint8)((uint32)Baud_Rate_Temp);
                UBRRH = (uint8)(((uint32)Baud_Rate_Temp >> 8));
                
                break;
            case USART_Synchronous:
                // . Set Sampling Rate
                Baud_Rate_Temp = (float32)(((F_CPU)/(2.0 * (usart->baud_rate))) - 1);
                
                // 1.2. Set UBRRH Register Address
                USART_Select_UBRRH_Register();
                
                // 1.3. Set Baud Rate in UBRR Registers
                UBRRL = (uint8)((uint32)Baud_Rate_Temp);
                UBRRH = (uint8)(((uint32)Baud_Rate_Temp >> 8));
                
                break;
            default :
                Retval = E_NOT_OK;
                break;
        }
    }
    return Retval;
}

static Std_ReturnType USART_Set_TX_Configurations(const USART_CFG *usart){
    Std_ReturnType Retval = E_OK;
    if(NULL == usart){
        Retval = E_NOT_OK;
    }
    else{
        // 1. Enable / Disable TX Pin
        if(usart->tx_transmitter.tx_state == USART_Tx_Enable){
            USART_Transmission_Enable();
            // 2. configure USART Interrupts if needed
#if (((Interrupt_Feature_Enable)) == (USART_Interrupt_Feature))
            // 2.1 Configure TX interrupt
            if(usart->tx_transmitter.tx_int_state == USART_Tx_Interrupt_Enable){
                // 2.1.1 Clear TX Interrupt flag
                Internal_Interrupt_TX_Clear_Flag();
                // 2.1.2 Enable TX Interrupt
                Internal_Interrupt_TX_Enable();
                // 2.1.3 Enable Gloabl Interrupt Enable
                GLobal_Interrupt_Enable_SREG();
                // 2.1.4 Assign Tx callback function
                Tx_Handler_Function = usart->tx_transmitter.tx_interrupt;
            }
            else if(usart->tx_transmitter.tx_int_state == USART_Tx_Interrupt_Disable){
                // 2.1.1 Disable TX Interrupt
                Internal_Interrupt_TX_Disable();
            }
            else {
                Retval = E_NOT_OK;
            }
            // 2.2 Configure TSR interrupt
            if(usart->tx_transmitter.tsr_int_state == USART_TSR_Interrupt_Enable){
                // 2.2.1 Enable TSR Interrupt
                Internal_Interrupt_TSR_Enable();
                // 2.2.2 Enable Gloabl Interrupt Enable
                GLobal_Interrupt_Enable_SREG();
                // 2.2.3 Assign TSR callback function
                TSR_Handler_Function = usart->tx_transmitter.tx_tsr_interrupt;
            }
            else if(usart->tx_transmitter.tsr_int_state == USART_TSR_Interrupt_Disable){
                Internal_Interrupt_TSR_Disable();
            }
            else {
                Retval = E_NOT_OK;
            }
#endif
            // 3. Tx Pin initialize
            GPIO_Pin_CFG tx_pin = {.Port = GPIO_PortD, .Pin = GPIO_Pin1, .Direction = GPIO_Output, .Logic = GPIO_Low};
            GPIO_Pin_INIT(&tx_pin);
        }
        else if(usart->tx_transmitter.tx_state == USART_Tx_Disable){
            USART_Transmission_Disable();
        }
        else{
            Retval = E_NOT_OK;
        }
    }
    return Retval;
}

static Std_ReturnType USART_Set_RX_Configurations(const USART_CFG *usart){
    Std_ReturnType Retval = E_OK;
    if(NULL == usart){
        Retval = E_NOT_OK;
    }
    else{
        // 1. Enable / Disable RX Pin
        if(usart->rx_receiver.rx_state == USART_Rx_Enable){
            USART_Receiving_Enable();
            // 2. configure USART Interrupts if needed
#if (((Interrupt_Feature_Enable)) == (USART_Interrupt_Feature))
            // 2.1 Configure RX interrupt
            if(usart->rx_receiver.rx_int_state == USART_Rx_Interrupt_Enable){
                // 2.1.2 Enable RX Interrupt
                Internal_Interrupt_RX_Enable();
                // 2.1.3 Enable Gloabl Interrupt Enable
                GLobal_Interrupt_Enable_SREG();
                // 2.1.4 Assign Rx callback functions
                Rx_Handler_Function = usart->rx_receiver.rx_interrupt;
                Framming_Handler_Function = usart->rx_receiver.framing_interrupt;
                Overrun_Handler_Function = usart->rx_receiver.overrun_interrupt;
            }
            else if(usart->rx_receiver.rx_int_state == USART_Rx_Interrupt_Disable){
                // 2.1.1 Disable RX Interrupt
                Internal_Interrupt_RX_Disable();
            }
            else {
                Retval = E_NOT_OK;
            }
#endif
            // 3. Rx Pin initialize
            GPIO_Pin_CFG rx_pin = {.Port = GPIO_PortD, .Pin = GPIO_Pin0, .Direction = GPIO_Input};
            GPIO_Pin_Direction_INIT(&rx_pin);
        }
        else if(usart->rx_receiver.rx_state == USART_Rx_Disable){
            USART_Receiving_Disable();
        }
        else{
            Retval = E_NOT_OK;
        }
    }
    return Retval;
}