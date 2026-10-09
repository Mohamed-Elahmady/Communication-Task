/* 
 * File:   mcal_std_types.h
 * Author: mohammedel-ahmady
 *
 * Created on June 26, 2025, 8:06 PM
 */

/******************* Section 0 : File Guard *******************/

#ifndef MCAL_STD_TYPES_H
#define	MCAL_STD_TYPES_H

/******************* Section 1 : Includes *******************/

#include "mcal_std_libraries.h"
#include "compiler.h"

/******************* Section 2 : Macros Declarations *******************/

#define STD_LOW                 0x00
#define STD_HIGH                0x01

#define STD_OFF                 0x00
#define STD_ON                  0x01

#define STD_IDLE                0x00
#define STD_ACTIVE              0x01

#define CPU_TYPE_08             0x08
#define CPU_TYPE_16             0x10
#define CPU_TYPE_32             0x20
#define CPU_TYPE_64             0x40

#define Bit_Mask                            (uint8)0x01
#define Port_Mask                           (uint8)0xFF

#define CPU_TYPE                (CPU_TYPE_08)

/******************* Section 3 : Macros Functions Declarations *******************/



/******************* Section 4 : User Defined Data Types & Variables Declarations *******************/

/*************** CPU_TYPE_64 *******************/
#if (CPU_TYPE == CPU_TYPE_64)

typedef bool                    boolean;
typedef unsigned char           uint8;
typedef unsigned short          uint16;
typedef unsigned long           uint32;
typedef unsigned long long      uint64;

typedef signed char             sint8;
typedef signed short            sint16;
typedef signed long             sint32;
typedef signed long long        sint64;

typedef float                   float32;
typedef double                  float64;
typedef long double             float128;

#define HWREG64(__X)             (*((volatile uint64 *)(__X)))

#define Set_Bit(REG, BIT_POS)               (REG |= (uint64)(Bit_Mask << BIT_POS))
#define Clear_Bit(REG, BIT_POS)             (REG &= ~(uint64)(Bit_Mask << BIT_POS))
#define Toggle_Bit(REG, BIT_POS)            (REG ^= (uint64)(Bit_Mask << BIT_POS))
#define Read_Bit(REG, BIT_POS)              ((REG >> BIT_POS) & (uint64)(Bit_Mask))

/*************** CPU_TYPE_32 *******************/
#elif (CPU_TYPE == CPU_TYPE_32)

typedef bool                    boolean;
typedef unsigned char           uint8;
typedef unsigned short          uint16;
typedef unsigned long           uint32;
typedef unsigned long long      uint64;

typedef signed char             sint8;
typedef signed short            sint16;
typedef signed long             sint32;
typedef signed long long        sint64;

typedef float                   float32;
typedef double                  float64;
typedef long double             float128;

#define HWREG32(__X)             (*((volatile uint32 *)(__X)))

#define Set_Bit(REG, BIT_POS)               (REG |= (uint32)(Bit_Mask << BIT_POS))
#define Clear_Bit(REG, BIT_POS)             (REG &= ~(uint32)(Bit_Mask << BIT_POS))
#define Toggle_Bit(REG, BIT_POS)            (REG ^= (uint32)(Bit_Mask << BIT_POS))
#define Read_Bit(REG, BIT_POS)              ((REG >> BIT_POS) & (uint32)(Bit_Mask))

/*************** CPU_TYPE_16 *******************/
#elif (CPU_TYPE == CPU_TYPE_16)

typedef bool                    boolean;
typedef unsigned char           uint8;
typedef unsigned short          uint16;
typedef unsigned long           uint32;
typedef unsigned long long      uint64;

typedef signed char             sint8;
typedef signed short            sint16;
typedef signed long             sint32;
typedef signed long long        sint64;

typedef float                   float32;
typedef double                  float64;
typedef long double             float128;

#define HWREG16(__X)             (*((volatile uint16 *)(__X)))

#define Set_Bit(REG, BIT_POS)               (REG |= (uint16)(Bit_Mask << BIT_POS))
#define Clear_Bit(REG, BIT_POS)             (REG &= ~(uint16)(Bit_Mask << BIT_POS))
#define Toggle_Bit(REG, BIT_POS)            (REG ^= (uint16)(Bit_Mask << BIT_POS))
#define Read_Bit(REG, BIT_POS)              ((REG >> BIT_POS) & (uint16)(Bit_Mask))

/*************** CPU_TYPE_8 *******************/
#elif (CPU_TYPE == CPU_TYPE_08)

typedef bool                    boolean;
typedef unsigned char           uint8;
typedef unsigned short          uint16;
typedef unsigned long           uint32;
typedef unsigned long long      uint64;

typedef signed char             sint8;
typedef signed short            sint16;
typedef signed long             sint32;
typedef signed long long        sint64;

typedef float                   float32;
typedef double                  float64;
typedef long double             float128;

#define HWREG8(__X)             (*((volatile uint8 *)(__X)))

#define Set_Bit(REG, BIT_POS)               (REG |= (uint8)(Bit_Mask << BIT_POS))
#define Clear_Bit(REG, BIT_POS)             (REG &= ~(uint8)(Bit_Mask << BIT_POS))
#define Toggle_Bit(REG, BIT_POS)            (REG ^= (uint8)(Bit_Mask << BIT_POS))
#define Read_Bit(REG, BIT_POS)              ((REG >> BIT_POS) & (uint8)(Bit_Mask))

#else
#error
#endif

typedef enum{
    E_NOT_OK =                  (uint8)0x00,
    E_OK     =                  (uint8)0x01
}Std_ReturnType;

/******************* Section 5 : Software Interfaces Declarations (APIs) *******************/


#endif	/* MCAL_STD_TYPES_H */