/* 
 * File:   hal_interrupt_mng.h
 * Author: mohammedel-ahmady
 *
 * Created on August 3, 2025, 6:17 PM
 */

/******************* Section 0 : File Guard *******************/

#ifndef HAL_INTERRUPT_MNG_H
#define	HAL_INTERRUPT_MNG_H

/******************* Section 1 : Includes *******************/

#include "hal_interrupt_cfg.h"

/******************* Section 2 : Macros Declarations *******************/



/******************* Section 3 : Macros Functions Declarations *******************/



/******************* Section 4 : User Defined Data Types & Variables Declarations *******************/



/******************* Section 5 : Software Interfaces Declarations (APIs) *******************/

// For external interrupt
void INT0_ISR(void);
void INT1_ISR(void);
void INT2_ISR(void);

// For internal interrupt
void ADC_ISR(void);
void TMR0_ISR(void);
void CTC0_ISR(void);
void TMR1_ISR(void);
void CTC_OCRA_ISR(void);
void CTC_OCRB_ISR(void);
void CTC_ICR_ISR(void);
void TMR2_ISR(void);
void CTC2_ISR(void);
void USART_TX_ISR(void);
void USART_TSR_ISR(void);
void USART_RX_ISR(void);
void SPI_ISR(void);

#endif	/* HAL_INTERRUPT_MNG_H */

