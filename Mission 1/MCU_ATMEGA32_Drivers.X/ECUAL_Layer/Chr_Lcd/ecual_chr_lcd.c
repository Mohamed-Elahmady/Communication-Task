/* 
 * File:   ecual_chr_lcd.c
 * Author: mohammedel-ahmady
 *
 * Created on July 22, 2025, 3:23 PM
 */

/******************* Section 0 : Includes *******************/

#include "ecual_chr_lcd.h"

/******************* Section 1 :  Variables Definitions *******************/



/******************* Section 2 :  Helper Functions Declarations *******************/

static Std_ReturnType Chr_Lcd_4Bit_Send_4Bit(const Chr_Lcd_4Bit *lcd, uint8 data);
static Std_ReturnType Chr_Lcd_4Bit_Set_Cursor(const Chr_Lcd_4Bit *lcd, uint8 row, uint8 col);
static Std_ReturnType Chr_Lcd_4Bit_Send_Enable_Signal(const Chr_Lcd_4Bit *lcd);

static Std_ReturnType Chr_Lcd_8Bit_Set_Cursor(const Chr_Lcd_8Bit *lcd, uint8 row, uint8 col);
static Std_ReturnType Chr_Lcd_8Bit_Send_Enable_Signal(const Chr_Lcd_8Bit *lcd);

/******************* Section 3 : Software Interfaces Definitions (APIs) *******************/

#if((GPIO_Lcd_4Bit_CFG_Enable) == (GPIO_Lcd_4Bit_CFG))

Std_ReturnType GPIO_Chr_Lcd_4Bit_INIT(const Chr_Lcd_4Bit *lcd){
    Std_ReturnType Retval = E_OK;
    if(NULL == lcd){
        Retval = E_NOT_OK;
    }
    else{
        // Control Pins Init
        Retval = GPIO_Pin_INIT(&(lcd->Rs_Pin));
        Retval = GPIO_Pin_INIT(&(lcd->Rw_Pin));
        Retval = GPIO_Pin_INIT(&(lcd->En_Pin));
        // Data Pins Init
        for(uint8 i = 0; i < Lcd_4Bit_Mode; i++){
            Retval = GPIO_Pin_INIT(&(lcd->Data_Pins[i]));
        }

        _delay_ms(20);
        Retval = GPIO_Chr_Lcd_4Bit_Send_Command(lcd, Lcd_8Bit_Twl_F58);
        _delay_ms(5);
        Retval = GPIO_Chr_Lcd_4Bit_Send_Command(lcd, Lcd_8Bit_Twl_F58);
        _delay_us(150);
        Retval = GPIO_Chr_Lcd_4Bit_Send_Command(lcd, Lcd_8Bit_Twl_F58);
        
        Retval = GPIO_Chr_Lcd_4Bit_Send_Command(lcd, Lcd_Clear_Display);
        Retval = GPIO_Chr_Lcd_4Bit_Send_Command(lcd, Lcd_Cursor_Home);
        Retval = GPIO_Chr_Lcd_4Bit_Send_Command(lcd, Lcd_Esm_Inc_Sft_Off);
        Retval = GPIO_Chr_Lcd_4Bit_Send_Command(lcd, Lcd_Display_On_Ulc_Off_Blc_Off);
        Retval = GPIO_Chr_Lcd_4Bit_Send_Command(lcd, Lcd_4Bit_Twl_F58);
        Retval = GPIO_Chr_Lcd_4Bit_Send_Command(lcd, Lcd_DDRAM_Address);
    }
    return Retval;
}

Std_ReturnType GPIO_Chr_Lcd_4Bit_Send_Command(const Chr_Lcd_4Bit *lcd, uint8 command){
    Std_ReturnType Retval = E_OK;
    if(NULL == lcd){
        Retval = E_NOT_OK;
    }
    else{
        Retval = GPIO_Pin_Write_Logic(&(lcd->Rs_Pin), GPIO_Low);
        Retval = GPIO_Pin_Write_Logic(&(lcd->Rw_Pin), GPIO_Low);
        
        Retval = Chr_Lcd_4Bit_Send_4Bit(lcd, (command >> 4));
        Retval = Chr_Lcd_4Bit_Send_Enable_Signal(lcd);
        Retval = Chr_Lcd_4Bit_Send_4Bit(lcd, command);
        Retval = Chr_Lcd_4Bit_Send_Enable_Signal(lcd);
    }
    return Retval;
}

Std_ReturnType GPIO_Chr_Lcd_4Bit_Send_Character(const Chr_Lcd_4Bit *lcd, uint8 character){
    Std_ReturnType Retval = E_OK;
    if(NULL == lcd){
        Retval = E_NOT_OK;
    }
    else{
        Retval = GPIO_Pin_Write_Logic(&(lcd->Rs_Pin), GPIO_High);
        Retval = GPIO_Pin_Write_Logic(&(lcd->Rw_Pin), GPIO_Low);
        
        Retval = Chr_Lcd_4Bit_Send_4Bit(lcd, (character >> 4));
        Retval = Chr_Lcd_4Bit_Send_Enable_Signal(lcd);
        Retval = Chr_Lcd_4Bit_Send_4Bit(lcd, character);
        Retval = Chr_Lcd_4Bit_Send_Enable_Signal(lcd);
    }
    return Retval;
}

Std_ReturnType GPIO_Chr_Lcd_4Bit_Send_Character_Pos(const Chr_Lcd_4Bit *lcd, uint8 character, uint8 row, uint8 col){
    Std_ReturnType Retval = E_OK;
    if(NULL == lcd){
        Retval = E_NOT_OK;
    }
    else{
        Retval = Chr_Lcd_4Bit_Set_Cursor(lcd, row, col);
        Retval = GPIO_Chr_Lcd_4Bit_Send_Character(lcd, character);
    }
    return Retval;
}

Std_ReturnType GPIO_Chr_Lcd_4Bit_Send_String(const Chr_Lcd_4Bit *lcd, uint8 *str){
    Std_ReturnType Retval = E_OK;
    if(NULL == lcd || NULL == str){
        Retval = E_NOT_OK;
    }
    else{
        while(*str){
            Retval = GPIO_Chr_Lcd_4Bit_Send_Character(lcd, *str);
            str++;
        }
    }
    return Retval;
}

Std_ReturnType GPIO_Chr_Lcd_4Bit_Send_String_Pos(const Chr_Lcd_4Bit *lcd, uint8 *str, uint8 row, uint8 col){
    Std_ReturnType Retval = E_OK;
    if(NULL == lcd || NULL == str){
        Retval = E_NOT_OK;
    }
    else{
        Retval = Chr_Lcd_4Bit_Set_Cursor(lcd, row, col);
        Retval = GPIO_Chr_Lcd_4Bit_Send_String(lcd, str);
    }
    return Retval;
}

Std_ReturnType GPIO_Chr_Lcd_4Bit_Send_Custom_Character(const Chr_Lcd_4Bit *lcd, uint8 symbol[], uint8 row, uint8 col, uint8 ram_pos){
    Std_ReturnType Retval = E_OK;
    if(NULL == lcd || NULL == symbol || ram_pos >= Custom_Char_No){
        Retval = E_NOT_OK;
    }
    else{
        Retval = GPIO_Chr_Lcd_4Bit_Send_Command(lcd, (Lcd_CGRAM_Address + (ram_pos * 8)));
        
        for(uint8 i = 0; i < Custom_Char_No; i++){
            Retval = GPIO_Chr_Lcd_4Bit_Send_Character(lcd, symbol[i]);
        }
        
        Retval = GPIO_Chr_Lcd_4Bit_Send_Character_Pos(lcd, ram_pos, row, col);
    }
    return Retval;
}

#endif

#if((GPIO_Lcd_8Bit_CFG_Enable) == (GPIO_Lcd_8Bit_CFG))

Std_ReturnType GPIO_Chr_Lcd_8Bit_INIT(const Chr_Lcd_8Bit *lcd){
    Std_ReturnType Retval = E_OK;
    if(NULL == lcd){
        Retval = E_NOT_OK;
    }
    else{
        // Control Pins Init
        Retval = GPIO_Pin_INIT(&(lcd->Rs_Pin));
        Retval = GPIO_Pin_INIT(&(lcd->Rw_Pin));
        Retval = GPIO_Pin_INIT(&(lcd->En_Pin));
        // Data Pins Init
        for(uint8 i = 0; i < Lcd_8Bit_Mode; i++){
            Retval = GPIO_Pin_INIT(&(lcd->Data_Pins[i]));
        }

        _delay_ms(20);
        Retval = GPIO_Chr_Lcd_8Bit_Send_Command(lcd, Lcd_8Bit_Twl_F58);
        _delay_ms(5);
        Retval = GPIO_Chr_Lcd_8Bit_Send_Command(lcd, Lcd_8Bit_Twl_F58);
        _delay_us(150);
        Retval = GPIO_Chr_Lcd_8Bit_Send_Command(lcd, Lcd_8Bit_Twl_F58);
        
        Retval = GPIO_Chr_Lcd_8Bit_Send_Command(lcd, Lcd_Clear_Display);
        Retval = GPIO_Chr_Lcd_8Bit_Send_Command(lcd, Lcd_Cursor_Home);
        Retval = GPIO_Chr_Lcd_8Bit_Send_Command(lcd, Lcd_Esm_Inc_Sft_Off);
        Retval = GPIO_Chr_Lcd_8Bit_Send_Command(lcd, Lcd_Display_On_Ulc_Off_Blc_Off);
        Retval = GPIO_Chr_Lcd_8Bit_Send_Command(lcd, Lcd_8Bit_Twl_F58);
        Retval = GPIO_Chr_Lcd_8Bit_Send_Command(lcd, Lcd_DDRAM_Address);
    }
    return Retval;
}

Std_ReturnType GPIO_Chr_Lcd_8Bit_Send_Command(const Chr_Lcd_8Bit *lcd, uint8 command){
    Std_ReturnType Retval = E_OK;
    if(NULL == lcd){
        Retval = E_NOT_OK;
    }
    else{
        Retval = GPIO_Pin_Write_Logic(&(lcd->Rs_Pin), GPIO_Low);
        Retval = GPIO_Pin_Write_Logic(&(lcd->Rw_Pin), GPIO_Low);
        
        for(uint8 i = 0; i < Lcd_8Bit_Mode; i++){
            Retval = GPIO_Pin_Write_Logic(&(lcd->Data_Pins[i]), (GPIO_Logic)Read_Bit(command, i));
        }
        
        Retval = Chr_Lcd_8Bit_Send_Enable_Signal(lcd);
    }
    return Retval;
}

Std_ReturnType GPIO_Chr_Lcd_8Bit_Send_Character(const Chr_Lcd_8Bit *lcd, uint8 character){
    Std_ReturnType Retval = E_OK;
    if(NULL == lcd){
        Retval = E_NOT_OK;
    }
    else{
        Retval = GPIO_Pin_Write_Logic(&(lcd->Rs_Pin), GPIO_High);
        Retval = GPIO_Pin_Write_Logic(&(lcd->Rw_Pin), GPIO_Low);
        
        for(uint8 i = 0; i < Lcd_8Bit_Mode; i++){
            Retval = GPIO_Pin_Write_Logic(&(lcd->Data_Pins[i]), (GPIO_Logic)Read_Bit(character, i));
        }
        
        Retval = Chr_Lcd_8Bit_Send_Enable_Signal(lcd);
    }
    return Retval;
}

Std_ReturnType GPIO_Chr_Lcd_8Bit_Send_Character_Pos(const Chr_Lcd_8Bit *lcd, uint8 character, uint8 row, uint8 col){
    Std_ReturnType Retval = E_OK;
    if(NULL == lcd){
        Retval = E_NOT_OK;
    }
    else{
        Retval = Chr_Lcd_8Bit_Set_Cursor(lcd, row, col);
        Retval = GPIO_Chr_Lcd_8Bit_Send_Character(lcd, character);
    }
    return Retval;
}
Std_ReturnType GPIO_Chr_Lcd_8Bit_Send_String(const Chr_Lcd_8Bit *lcd, uint8 *str){
    Std_ReturnType Retval = E_OK;
    if(NULL == lcd || NULL == str){
        Retval = E_NOT_OK;
    }
    else{
        while(*str){
            Retval = GPIO_Chr_Lcd_8Bit_Send_Character(lcd, *str);
            str++;
        }
    }
    return Retval;
}
Std_ReturnType GPIO_Chr_Lcd_8Bit_Send_String_Pos(const Chr_Lcd_8Bit *lcd, uint8 *str, uint8 row, uint8 col){
    Std_ReturnType Retval = E_OK;
    if(NULL == lcd || NULL == str){
        Retval = E_NOT_OK;
    }
    else{
        Retval = Chr_Lcd_8Bit_Set_Cursor(lcd, row, col);
        Retval = GPIO_Chr_Lcd_8Bit_Send_String(lcd, str);
    }
    return Retval;
}
Std_ReturnType GPIO_Chr_Lcd_8Bit_Send_Custom_Character(const Chr_Lcd_8Bit *lcd, uint8 symbol[], uint8 row, uint8 col, uint8 ram_pos){
    Std_ReturnType Retval = E_OK;
    if(NULL == lcd || NULL == symbol || ram_pos >= Custom_Char_No){
        Retval = E_NOT_OK;
    }
    else{
        Retval = GPIO_Chr_Lcd_8Bit_Send_Command(lcd, (Lcd_CGRAM_Address + (ram_pos * 8)));
        
        for(uint8 i = 0; i < Custom_Char_No; i++){
            Retval = GPIO_Chr_Lcd_8Bit_Send_Character(lcd, symbol[i]);
        }
        
        Retval = GPIO_Chr_Lcd_8Bit_Send_Character_Pos(lcd, ram_pos, row, col);
    }
    return Retval;
}

#endif


Std_ReturnType Convert_uint8_To_String(uint8 data, uint8 *str){
    Std_ReturnType Retval = E_OK;
    if(NULL == str){
        Retval = E_NOT_OK;
    }
    else{
        uint8 Null_data[4] = {0};
        uint8 Data_Counter = 0;
        
        memset(str, ' ', 3);
        str[3] = '\0';
        sprintf(Null_data, "%i", data);
        
        while(Null_data[Data_Counter] != '\0'){
            str[Data_Counter] = Null_data[Data_Counter];
            Data_Counter++;
        }
    }
    return Retval;
}
Std_ReturnType Convert_uint16_To_String(uint16 data, uint8 *str){
    Std_ReturnType Retval = E_OK;
    if(NULL == str){
        Retval = E_NOT_OK;
    }
    else{
        uint8 Null_data[6] = {0};
        uint8 Data_Counter = 0;
        
        memset(str, ' ', 5);
        str[3] = '\0';
        sprintf(Null_data, "%i", data);
        
        while(Null_data[Data_Counter] != '\0'){
            str[Data_Counter] = Null_data[Data_Counter];
            Data_Counter++;
        }
    }
    return Retval;
}
Std_ReturnType Convert_uint32_To_String(uint32 data, uint8 *str){
    Std_ReturnType Retval = E_OK;
    if(NULL == str){
        Retval = E_NOT_OK;
    }
    else{
        uint8 Null_data[11] = {0};
        uint8 Data_Counter = 0;
        
        memset(str, ' ', 10);
        str[3] = '\0';
        sprintf(Null_data, "%i", data);
        
        while(Null_data[Data_Counter] != '\0'){
            str[Data_Counter] = Null_data[Data_Counter];
            Data_Counter++;
        }
    }
    return Retval;
}

/******************* Section 4:  Helper Functions Definitions *******************/

static Std_ReturnType Chr_Lcd_4Bit_Send_4Bit(const Chr_Lcd_4Bit *lcd, uint8 data){
    Std_ReturnType Retval = E_OK;
    if(NULL == lcd){
        Retval = E_NOT_OK;
    }
    else{
        for(uint8 i = 0; i < Lcd_4Bit_Mode; i++){
            Retval = GPIO_Pin_Write_Logic(&(lcd->Data_Pins[i]), (GPIO_Logic)Read_Bit(data, i));
        }
    }
    return Retval;
}
static Std_ReturnType Chr_Lcd_4Bit_Set_Cursor(const Chr_Lcd_4Bit *lcd, uint8 row, uint8 col){
    Std_ReturnType Retval = E_OK;
    if(NULL == lcd){
        Retval = E_NOT_OK;
    }
    else{
        col--;
        switch(row){
            case Lcd_Row1:
                Retval = GPIO_Chr_Lcd_4Bit_Send_Command(lcd, (Row1_Add + col));
                break;
            case Lcd_Row2:
                Retval = GPIO_Chr_Lcd_4Bit_Send_Command(lcd, (Row2_Add + col));
                break;
            case Lcd_Row3:
                Retval = GPIO_Chr_Lcd_4Bit_Send_Command(lcd, (Row3_Add + col));
                break;
            case Lcd_Row4:
                Retval = GPIO_Chr_Lcd_4Bit_Send_Command(lcd, (Row4_Add + col));
                break;
            default :
                Retval = E_NOT_OK;
                break;
        }
    }
    return Retval;
}
static Std_ReturnType Chr_Lcd_4Bit_Send_Enable_Signal(const Chr_Lcd_4Bit *lcd){
    Std_ReturnType Retval = E_OK;
    if(NULL == lcd){
        Retval = E_NOT_OK;
    }
    else{
        Retval = GPIO_Pin_Write_Logic(&(lcd->En_Pin), GPIO_High);
        _delay_us(205);
        Retval = GPIO_Pin_Write_Logic(&(lcd->En_Pin), GPIO_Low);
    }
    return Retval;
}

static Std_ReturnType Chr_Lcd_8Bit_Set_Cursor(const Chr_Lcd_8Bit *lcd, uint8 row, uint8 col){
    Std_ReturnType Retval = E_OK;
    if(NULL == lcd){
        Retval = E_NOT_OK;
    }
    else{
        col--;
        switch(row){
            case Lcd_Row1:
                Retval = GPIO_Chr_Lcd_8Bit_Send_Command(lcd, (Row1_Add + col));
                break;
            case Lcd_Row2:
                Retval = GPIO_Chr_Lcd_8Bit_Send_Command(lcd, (Row2_Add + col));
                break;
            case Lcd_Row3:
                Retval = GPIO_Chr_Lcd_8Bit_Send_Command(lcd, (Row3_Add + col));
                break;
            case Lcd_Row4:
                Retval = GPIO_Chr_Lcd_8Bit_Send_Command(lcd, (Row4_Add + col));
                break;
            default :
                Retval = E_NOT_OK;
                break;
        }
    }
    return Retval;
}
static Std_ReturnType Chr_Lcd_8Bit_Send_Enable_Signal(const Chr_Lcd_8Bit *lcd){
    Std_ReturnType Retval = E_OK;
    if(NULL == lcd){
        Retval = E_NOT_OK;
    }
    else{
        Retval = GPIO_Pin_Write_Logic(&(lcd->En_Pin), GPIO_High);
        _delay_us(205);
        Retval = GPIO_Pin_Write_Logic(&(lcd->En_Pin), GPIO_Low);
    }
    return Retval;
}