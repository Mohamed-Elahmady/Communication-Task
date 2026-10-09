/* 
 * File:   hal_ext_interrupt.h
 * Author: mohammedel-ahmady
 *
 * Created on August 3, 2025, 6:15 PM
 */

/******************* Section 0 : File Guard *******************/

#ifndef HAL_EXT_INTERRUPT_H
#define	HAL_EXT_INTERRUPT_H

/******************* Section 1 : Includes *******************/

#include "hal_interrupt_cfg.h"

/******************* Section 2 : Macros Declarations *******************/



/******************* Section 3 : Macros Functions Declarations *******************/

// FOR INT0 EXTERNAL INTERRUPT

#define External_Interrupt_Int0_Clear_Flag()                     Set_Bit(GIFR, INTF0)

#define External_Interrupt_Int0_Disable()                        Clear_Bit(GICR, INT0)
#define External_Interrupt_Int0_Enable()                         Set_Bit(GICR, INT0)

#define External_Interrupt_Int0_Low_Level_ISC00()                Clear_Bit(MCUCR, ISC00)
#define External_Interrupt_Int0_Low_Level_ISC01()                Clear_Bit(MCUCR, ISC01)

#define External_Interrupt_Int0_On_Change_ISC00()                Set_Bit(MCUCR, ISC00)
#define External_Interrupt_Int0_On_Change_ISC01()                Clear_Bit(MCUCR, ISC01)

#define External_Interrupt_Int0_Falling_Edge_ISC00()             Clear_Bit(MCUCR, ISC00)
#define External_Interrupt_Int0_Falling_Edge_ISC01()             Set_Bit(MCUCR, ISC01)

#define External_Interrupt_Int0_Rising_Edge_ISC00()              Set_Bit(MCUCR, ISC00)
#define External_Interrupt_Int0_Rising_Edge_ISC01()              Set_Bit(MCUCR, ISC01)

// FOR INT1 EXTERNAL INTERRUPT

#define External_Interrupt_Int1_Clear_Flag()                     Set_Bit(GIFR, INTF1)

#define External_Interrupt_Int1_Disable()                        Clear_Bit(GICR, INT1)
#define External_Interrupt_Int1_Enable()                         Set_Bit(GICR, INT1)

#define External_Interrupt_Int1_Low_Level_ISC10()                Clear_Bit(MCUCR, ISC10)
#define External_Interrupt_Int1_Low_Level_ISC11()                Clear_Bit(MCUCR, ISC11)

#define External_Interrupt_Int1_On_Change_ISC10()                Set_Bit(MCUCR, ISC10)
#define External_Interrupt_Int1_On_Change_ISC11()                Clear_Bit(MCUCR, ISC11)

#define External_Interrupt_Int1_Falling_Edge_ISC10()             Clear_Bit(MCUCR, ISC10)
#define External_Interrupt_Int1_Falling_Edge_ISC11()             Set_Bit(MCUCR, ISC11)

#define External_Interrupt_Int1_Rising_Edge_ISC10()              Set_Bit(MCUCR, ISC10)
#define External_Interrupt_Int1_Rising_Edge_ISC11()              Set_Bit(MCUCR, ISC11)

// FOR INT2 EXTERNAL INTERRUPT

#define External_Interrupt_Int2_Clear_Flag()                     Set_Bit(GIFR, INTF2)

#define External_Interrupt_Int2_Disable()                        Clear_Bit(GICR, INT2)
#define External_Interrupt_Int2_Enable()                         Set_Bit(GICR, INT2)

#define External_Interrupt_Int2_Falling_Edge_ISC2()              Clear_Bit(MCUCSR, ISC2)
#define External_Interrupt_Int2_Rising_Edge_ISC2()               Set_Bit(MCUCSR, ISC2)



/******************* Section 4 : User Defined Data Types & Variables Declarations *******************/

typedef void (* Interrupt_Handler)(void);

typedef enum{
    Interrupt_Int0      = (uint8)0x00,
    Interrupt_Int1      = (uint8)0x01,
    Interrupt_Int2      = (uint8)0x02
}Interrupt_Source;

typedef enum{
    Interrupt_Low_Level     = (uint8)0x00,
    Interrupt_On_Change     = (uint8)0x01,
    Interrupt_Falling_Edge  = (uint8)0x02,
    Interrupt_Rising_Edge   = (uint8)0x03
}Interrupt_Edge;

typedef struct{
    Interrupt_Handler Intx_ISR;
    GPIO_Pin_CFG           pin;
    Interrupt_Source    source;
    Interrupt_Edge        edge;
}External_Interrupt_Intx;

/******************* Section 5 : Software Interfaces Declarations (APIs) *******************/

Std_ReturnType External_Interrupt_INTx_INIT(const External_Interrupt_Intx *intx);
Std_ReturnType External_Interrupt_INTx_DEINIT(const External_Interrupt_Intx *intx);

#endif	/* HAL_EXT_INTERRUPT_H */

