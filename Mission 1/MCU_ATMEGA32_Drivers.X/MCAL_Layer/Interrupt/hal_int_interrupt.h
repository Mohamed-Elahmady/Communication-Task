/* 
 * File:   hal_int_interrupt.h
 * Author: mohammedel-ahmady
 *
 * Created on August 3, 2025, 6:15 PM
 */

/******************* Section 0 : File Guard *******************/

#ifndef HAL_INT_INTERRUPT_H
#define	HAL_INT_INTERRUPT_H

/******************* Section 1 : Includes *******************/

#include "hal_interrupt_cfg.h"

/******************* Section 2 : Macros Declarations *******************/

// Internal Interrupts

// 1. For ADC Interrupt
#define Internal_Interrupt_ADC_Clear_Flag()                        Set_Bit(ADCSRA, ADIF)

#define Internal_Interrupt_ADC_Disable()                           Clear_Bit(ADCSRA, ADIE)
#define Internal_Interrupt_ADC_Enable()                            Set_Bit(ADCSRA, ADIE)

// 2. For Timer0 Interrupts
#define Internal_Interrupt_Timer0_Normal_Clear_Flag()              Set_Bit(TIFR, TOV0)

#define Internal_Interrupt_Timer0_Normal_Disable()                 Clear_Bit(TIMSK, TOIE0)
#define Internal_Interrupt_Timer0_Normal_Enable()                  Set_Bit(TIMSK, TOIE0)

#define Internal_Interrupt_Timer0_CTC_Clear_Flag()                 Set_Bit(TIFR, OCF0)

#define Internal_Interrupt_Timer0_CTC_Disable()                    Clear_Bit(TIMSK, OCIE0)
#define Internal_Interrupt_Timer0_CTC_Enable()                     Set_Bit(TIMSK, OCIE0)

// 3. For Timer1 Interrupts
#define Internal_Interrupt_Timer1_Normal_Clear_Flag()              Set_Bit(TIFR, TOV1)

#define Internal_Interrupt_Timer1_Normal_Disable()                 Clear_Bit(TIMSK, TOIE1)
#define Internal_Interrupt_Timer1_Normal_Enable()                  Set_Bit(TIMSK, TOIE1)

#define Internal_Interrupt_Timer1_CTC_OC1A_Clear_Flag()            Set_Bit(TIFR, OCF1A)

#define Internal_Interrupt_Timer1_CTC_OC1A_Disable()               Clear_Bit(TIMSK, OCIE1A)
#define Internal_Interrupt_Timer1_CTC_OC1A_Enable()                Set_Bit(TIMSK, OCIE1A)

#define Internal_Interrupt_Timer1_CTC_OC1B_Clear_Flag()            Set_Bit(TIFR, OCF1B)

#define Internal_Interrupt_Timer1_CTC_OC1B_Disable()               Clear_Bit(TIMSK, OCIE1B)
#define Internal_Interrupt_Timer1_CTC_OC1B_Enable()                Set_Bit(TIMSK, OCIE1B)

#define Internal_Interrupt_Timer1_CTC_ICR_Clear_Flag()             Set_Bit(TIFR, ICF1)

#define Internal_Interrupt_Timer1_CTC_ICR_Disable()                Clear_Bit(TIMSK, TICIE1)
#define Internal_Interrupt_Timer1_CTC_ICR_Enable()                 Set_Bit(TIMSK, TICIE1)

// 4. For Timer2 Interrupts
#define Internal_Interrupt_Timer2_Normal_Clear_Flag()              Set_Bit(TIFR, TOV2)

#define Internal_Interrupt_Timer2_Normal_Disable()                 Clear_Bit(TIMSK, TOIE2)
#define Internal_Interrupt_Timer2_Normal_Enable()                  Set_Bit(TIMSK, TOIE2)

#define Internal_Interrupt_Timer2_CTC_Clear_Flag()                 Set_Bit(TIFR, OCF2)

#define Internal_Interrupt_Timer2_CTC_Disable()                    Clear_Bit(TIMSK, OCIE2)
#define Internal_Interrupt_Timer2_CTC_Enable()                     Set_Bit(TIMSK, OCIE2)

// 5. For USART Interrupts

// this interrupt for each completed transmission

#define Internal_Interrupt_TX_Clear_Flag()                         Set_Bit(UCSRA, TXC)

#define Internal_Interrupt_TX_Disable()                            Clear_Bit(UCSRB, TXCIE)
#define Internal_Interrupt_TX_Enable()                             Set_Bit(UCSRB, TXCIE)

// this interrupt for happen when the transmission shift register is empty

#define Internal_Interrupt_TSR_Disable()                           Clear_Bit(UCSRB, UDRIE)
#define Internal_Interrupt_TSR_Enable()                            Set_Bit(UCSRB, UDRIE)

// this interrupt for each completed Receiving

#define Internal_Interrupt_RX_Disable()                            Clear_Bit(UCSRB, RXCIE)
#define Internal_Interrupt_RX_Enable()                             Set_Bit(UCSRB, RXCIE)

// 6. For SPI Interrupts

#define Internal_Interrupt_SPI_Disable()                           Clear_Bit(SPCR, SPIE)
#define Internal_Interrupt_SPI_Enable()                            Set_Bit(SPCR, SPIE)

// 7. For TWI Interrupt
#define Internal_Interrupt_TWI_Clear_Flag()                        Set_Bit(TWCR, TWINT)

#define Internal_Interrupt_TWI_Disable()                           Clear_Bit(TWCR, TWIE)
#define Internal_Interrupt_TWI_Enable()                            Set_Bit(TWCR, TWIE)

/******************* Section 3 : Macros Functions Declarations *******************/



/******************* Section 4 : User Defined Data Types & Variables Declarations *******************/



/******************* Section 5 : Software Interfaces Declarations (APIs) *******************/


#endif	/* HAL_INT_INTERRUPT_H */

