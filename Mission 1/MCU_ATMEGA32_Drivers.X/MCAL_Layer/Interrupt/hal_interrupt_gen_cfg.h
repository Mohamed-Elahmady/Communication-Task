/* 
 * File:   hal_interrupt_gen_cfg.h
 * Author: mohammedel-ahmady
 *
 * Created on August 13, 2025, 1:43 PM
 */

/******************* Section 0 : File Guard *******************/

#ifndef HAL_INTERRUPT_GEN_CFG_H
#define	HAL_INTERRUPT_GEN_CFG_H

/******************* Section 1 : Includes *******************/



/******************* Section 2 : Macros Declarations *******************/

// Interrupt Features
#define Interrupt_Feature_Disable                0x00
#define Interrupt_Feature_Enable                 0x01

// for external interrupts
#define External_Interrupt_INTX                  (Interrupt_Feature_Enable)
// for internal interrupts
#define ADC_Interrupt_Feature                    (Interrupt_Feature_Enable)
#define Timer0_Interrupt_Feature                 (Interrupt_Feature_Enable)
#define Timer1_Interrupt_Feature                 (Interrupt_Feature_Enable)
#define Timer2_Interrupt_Feature                 (Interrupt_Feature_Enable)
#define USART_Interrupt_Feature                  (Interrupt_Feature_Enable)
#define SPI_Interrupt_Feature                    (Interrupt_Feature_Enable)
#define TWI_Interrupt_Feature                    (Interrupt_Feature_Enable)

/******************* Section 3 : Macros Functions Declarations *******************/



/******************* Section 4 : User Defined Data Types & Variables Declarations *******************/



/******************* Section 5 : Software Interfaces Declarations (APIs) *******************/


#endif	/* HAL_INTERRUPT_GEN_CFG_H */

