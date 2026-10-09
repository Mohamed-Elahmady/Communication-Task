/* 
 * File:   hal_spi.h
 * Author: mohammedel-ahmady
 *
 * Created on September 27, 2025, 7:25 PM
 */

/******************* Section 0 : Includes *******************/

#include "hal_spi.h"

/******************* Section 1 :  Variables Definitions *******************/



/******************* Section 2 :  Helper Functions Declarations *******************/

#if((Interrupt_Feature_Enable) == (SPI_Interrupt_Feature))
static SPI_Handler SPI_Handler_Function = NULL;
#endif

static Std_ReturnType SPI_Set_Disable(const SPI_CFG *spi);
static Std_ReturnType SPI_Set_Enable(const SPI_CFG *spi);
static Std_ReturnType SPI_Set_Mode_Configurations(const SPI_CFG *spi);
static Std_ReturnType SPI_Pins_Initalization(const SPI_CFG *spi);
static Std_ReturnType SPI_Set_Clock_Configurations(const SPI_CFG *spi);
#if((Interrupt_Feature_Enable) == (SPI_Interrupt_Feature))
static Std_ReturnType SPI_Configure_Interrupt(const SPI_CFG *spi);
#endif
/******************* Section 3 : Software Interfaces Definitions (APIs) *******************/

Std_ReturnType SPI_INIT(const SPI_CFG *spi){
    Std_ReturnType Retval = E_OK;
    if(NULL == spi){
        Retval = E_NOT_OK;
    }
    else{
        // 1. Disable SPI Module
        Retval = SPI_Set_Disable(spi);
        // 2. Select Mode & Mode Configurations
        Retval = SPI_Set_Mode_Configurations(spi);
        // 3. SPI Pins Initialization
        Retval = SPI_Pins_Initalization(spi);
        // 4. Set Clock Polarity & Clock Phase
        Retval = SPI_Set_Clock_Configurations(spi);
        // 5. Configure SPI Interrupt if needed
#if((Interrupt_Feature_Enable) == (SPI_Interrupt_Feature))
        Retval = SPI_Configure_Interrupt(spi);
#endif
        // 6. Enable SPI Module
        Retval = SPI_Set_Enable(spi);
    }
    return Retval;
}

Std_ReturnType SPI_DEINIT(const SPI_CFG *spi){
    Std_ReturnType Retval = E_OK;
    if(NULL == spi){
        Retval = E_NOT_OK;
    }
    else{
        // 1. Disable SPI Module
        Retval = SPI_Set_Disable(spi);
#if((Interrupt_Feature_Enable) == (SPI_Interrupt_Feature))
        Internal_Interrupt_SPI_Disable();
        uint8 dummy = SPDR; // to clear flag if set
#endif
    }
    return Retval;
}

Std_ReturnType SPI_Write_Byte_Non_Blocking(const SPI_CFG *spi, uint8 data){
    Std_ReturnType Retval = E_OK;
    if(NULL == spi){
        Retval = E_NOT_OK;
    }
    else{
        // check transmiision state
        if(!SPI_Read_Transmit_Receive_Status()){
            // 2. Set Data From SPI Data Register
            SPDR = data;
            // 2. Check collision
            if(SPI_Read_Transmit_Collision_Status()){
                uint8 dummy = SPDR;
                Retval = E_NOT_OK;
            }
        }
        else{/* Nothing */}
    }
    return Retval;
}

Std_ReturnType SPI_Write_String_Non_Blocking(const SPI_CFG *spi, uint8 *str){
    Std_ReturnType Retval = E_OK;
    static uint8 *temp_str = NULL;

    if(NULL == spi || (str == NULL && temp_str == NULL)){
        Retval = E_NOT_OK;
    }
    else{
        if(temp_str == NULL && str != NULL){
            temp_str = str;
        }

        if(temp_str != NULL && *temp_str != '\0'){
            if(!SPI_Read_Transmit_Receive_Status()){
                SPDR = *temp_str;
                if(SPI_Read_Transmit_Collision_Status()){
                    uint8 dummy = SPDR;
                    Retval = E_NOT_OK;
                }
                else{
                    temp_str++;
                }
            }
            else{
                Retval = E_NOT_OK;
            }
        }
        else if(temp_str != NULL && *temp_str == '\0'){
            temp_str = NULL;
            Retval = E_OK;
        }
    }
    return Retval;
}

Std_ReturnType SPI_Write_Byte_Blocking(const SPI_CFG *spi, uint8 data){
    Std_ReturnType Retval = E_OK;
    if(NULL == spi){
        Retval = E_NOT_OK;
    }
    else{
        // 1. Set Data From SPI Data Register
        SPDR = data;
        // 2. Check collision
        if(SPI_Read_Transmit_Collision_Status()){
            uint8 dummy = SPDR;
            Retval = E_NOT_OK;
        }
        // 3. Wait for Transmission
        while(SPI_Read_Transmit_Receive_Status());
        uint8 dummy = SPDR;
    }
    return Retval;
}

Std_ReturnType SPI_Write_String_Blocking(const SPI_CFG *spi, uint8 *str){
    Std_ReturnType Retval = E_OK;
    if(NULL == spi || NULL == str){
        Retval = E_NOT_OK;
    }
    else{
        while(*str){
            Retval = SPI_Write_Byte_Blocking(spi, *str);
            str++;
        }
    }
    return Retval;
}

Std_ReturnType SPI_Read_Byte_Non_Blocking(const SPI_CFG *spi, uint8 *data){
    Std_ReturnType Retval = E_OK;
    if(NULL == spi || NULL == data){
        Retval = E_NOT_OK;
    }
    else{
        // 1. check if receiving completed
        if(SPI_Read_Transmit_Receive_Status()){
            // 2. Set Data From SPI Data Register
            *data = SPDR;
        }
        else{ /* Nothing */}
    }
    return Retval;
}

Std_ReturnType SPI_Read_Byte_Blocking(const SPI_CFG *spi, uint8 *data){
    Std_ReturnType Retval = E_OK;
    if(NULL == spi || NULL == data){
        Retval = E_NOT_OK;
    }
    else{
        // 1. Wait for Receiving
        while(SPI_Read_Transmit_Receive_Status());
        // 2. Set Data From SPI Data Register
        *data = SPDR;
    }
    return Retval;
}

#if((Interrupt_Feature_Enable) == (SPI_Interrupt_Feature))

void SPI_ISR(void){
    // 1. Clear flag by receive dummy values
    uint8 dummy = SPDR;
    // 2. Code
    //3. Callback Function
    if(SPI_Handler_Function){
        SPI_Handler_Function();
    }
}

#endif

/******************* Section 4:  Helper Functions Definitions *******************/

static Std_ReturnType SPI_Set_Disable(const SPI_CFG *spi){
    Std_ReturnType Retval = E_OK;
    if(NULL == spi){
        Retval = E_NOT_OK;
    }
    else{
        SPI_Module_Disable();
    }
    return Retval;
}

static Std_ReturnType SPI_Set_Enable(const SPI_CFG *spi){
    Std_ReturnType Retval = E_OK;
    if(NULL == spi){
        Retval = E_NOT_OK;
    }
    else{
        SPI_Module_Enable();
    }
    return Retval;
}

static Std_ReturnType SPI_Set_Mode_Configurations(const SPI_CFG *spi){
    Std_ReturnType Retval = E_OK;
    if(NULL == spi){
        Retval = E_NOT_OK;
    }
    else{
        switch(spi->mode){
            case SPI_Master_Mode:
                // 1. Set Mode Bit
                SPI_Set_Master_Mode();
                
                // 2. Set Master Clock Frequency speed
                if(spi->master_freq == SPI_Master_FOSC_Div_4){
                    Clear_Bit(SPCR, SPR0);
                    Clear_Bit(SPCR, SPR1);
                    Clear_Bit(SPSR, SPI2X);
                }
                else if(spi->master_freq == SPI_Master_FOSC_Div_16){
                    Set_Bit(SPCR, SPR0);
                    Clear_Bit(SPCR, SPR1);
                    Clear_Bit(SPSR, SPI2X);
                }
                else if(spi->master_freq == SPI_Master_FOSC_Div_64){
                    Clear_Bit(SPCR, SPR0);
                    Set_Bit(SPCR, SPR1);
                    Clear_Bit(SPSR, SPI2X);
                }
                else if(spi->master_freq == SPI_Master_FOSC_Div_128){
                    Set_Bit(SPCR, SPR0);
                    Set_Bit(SPCR, SPR1);
                    Clear_Bit(SPSR, SPI2X);
                }
                else if(spi->master_freq == SPI_Master_FOSC_Div_2){
                    Clear_Bit(SPCR, SPR0);
                    Clear_Bit(SPCR, SPR1);
                    Set_Bit(SPSR, SPI2X);
                }
                else if(spi->master_freq == SPI_Master_FOSC_Div_8){
                    Set_Bit(SPCR, SPR0);
                    Clear_Bit(SPCR, SPR1);
                    Set_Bit(SPSR, SPI2X);
                }
                else if(spi->master_freq == SPI_Master_FOSC_Div_32){
                    Clear_Bit(SPCR, SPR0);
                    Set_Bit(SPCR, SPR1);
                    Set_Bit(SPSR, SPI2X);
                }
                else{
                    Retval = E_NOT_OK;
                }
                
                // 3. Set transmission / receiver first bit
                if(spi->start_bit == SPI_Most_Significant_Bit_First){
                    SPI_Transmit_Receive_MSB_First();
                }
                else if(spi->start_bit == SPI_Least_Significant_Bit_First){
                    SPI_Transmit_Receive_LSB_First();
                }
                else{
                    Retval = E_NOT_OK;
                }
                
                break;
            case SPI_Slave_Mode:
                // 1. Set Mode Bit
                SPI_Set_Slave_Mode();
                
                // 2. Set transmission / receiver first bit
                if(spi->start_bit == SPI_Most_Significant_Bit_First){
                    SPI_Transmit_Receive_MSB_First();
                }
                else if(spi->start_bit == SPI_Least_Significant_Bit_First){
                    SPI_Transmit_Receive_LSB_First();
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

static Std_ReturnType SPI_Pins_Initalization(const SPI_CFG *spi){
    Std_ReturnType Retval = E_OK;
    if(NULL == spi){
        Retval = E_NOT_OK;
    }
    else{
        switch(spi->mode){
            case SPI_Master_Mode:{
                // 1. First Definition of Main Pins
                GPIO_Pin_CFG MOSI_Pin = {.Port = GPIO_PortB, .Pin = GPIO_Pin5, .Direction = GPIO_Output, .Logic = GPIO_Low};
                GPIO_Pin_CFG MISO_Pin = {.Port = GPIO_PortB, .Pin = GPIO_Pin6, .Direction = GPIO_Input};
                GPIO_Pin_CFG SCK_Pin = {.Port = GPIO_PortB, .Pin = GPIO_Pin7, .Direction = GPIO_Output, .Logic = GPIO_Low};
                
                // 2. Initalize Main Pins
                Retval = GPIO_Pin_INIT(&MOSI_Pin);
                Retval = GPIO_Pin_Direction_INIT(&MISO_Pin);
                Retval = GPIO_Pin_INIT(&SCK_Pin);
                
                // 3. Init GPIO -> Slave Slect Pin
                Retval = GPIO_Pin_INIT(&(spi->ss_pin));
                
                break;
            }
            case SPI_Slave_Mode:{
                // 1. First Definition of Main Pins
                GPIO_Pin_CFG SS_Pin = {.Port = GPIO_PortB, .Pin = GPIO_Pin4, .Direction = GPIO_Input};
                GPIO_Pin_CFG MOSI_Pin = {.Port = GPIO_PortB, .Pin = GPIO_Pin5, .Direction = GPIO_Input};
                GPIO_Pin_CFG MISO_Pin = {.Port = GPIO_PortB, .Pin = GPIO_Pin6, .Direction = GPIO_Output, .Logic = GPIO_Low};
                GPIO_Pin_CFG SCK_Pin = {.Port = GPIO_PortB, .Pin = GPIO_Pin7, .Direction = GPIO_Input};
                
                // 2. Initalize Main Pins
                Retval = GPIO_Pin_Direction_INIT(&MOSI_Pin);
                Retval = GPIO_Pin_Direction_INIT(&MOSI_Pin);
                Retval = GPIO_Pin_INIT(&MISO_Pin);
                Retval = GPIO_Pin_Direction_INIT(&SCK_Pin);
                
                break;
            }
            default :
                break;
        }
    }
    return Retval;
}

static Std_ReturnType SPI_Set_Clock_Configurations(const SPI_CFG *spi){
    Std_ReturnType Retval = E_OK;
    if(NULL == spi){
        Retval = E_NOT_OK;
    }
    else{
        // 1. Set Clock Polarity
        if(spi->clk_polarity == SPI_Idle_High){
            SPI_Clock_Polarity_Idle_High();
        }
        else if(spi->clk_polarity == SPI_Idle_Low){
            SPI_Clock_Polarity_Idle_Low();
        }
        else{
            Retval = E_NOT_OK;
        }
        
        // 2. Set Clock Phase & Clock Sample
        if(spi->clk_phase == SPI_Receive_Rising_Transmit_Falling){
            SPI_Clock_Phase_Receive_Rising_Transmit_Falling();
        }
        else if(spi->clk_phase == SPI_Transmit_Rising_Receive_Falling){
            SPI_Clock_Phase_Transmit_Rising_Receive_Falling();
        }
        else{
            Retval = E_NOT_OK;
        }
    }
    return Retval;
}

#if((Interrupt_Feature_Enable) == (SPI_Interrupt_Feature))

static Std_ReturnType SPI_Configure_Interrupt(const SPI_CFG *spi){
    Std_ReturnType Retval = E_OK;
    if(NULL == spi){
        Retval = E_NOT_OK;
    }
    else{
        // 1. Enable SPI Interrupt
        Internal_Interrupt_SPI_Enable();
        // 2. Enable Global Interrupt
        GLobal_Interrupt_Enable_SREG();
        // 3. Assign Callback function
        SPI_Handler_Function = spi->spi_interrupt;
    }
    return Retval;
}

#endif