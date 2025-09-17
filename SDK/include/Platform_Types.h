/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2032 by Vector Informatik GmbH.                                              All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *         File:  Platform_Types.h
 *    Component:  -
 *       Module:  -
 *    Generator:  -
 *
 *  Description:  Provision of Platform Types for CANoeEmu
 *
 *  -------------------------------------------------------------------------------------------------------------------
 *  MISRA VIOLATIONS
 *  -------------------------------------------------------------------------------------------------------------------
 *
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  AUTHOR IDENTITY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Name                          Initials      Company
 *  -------------------------------------------------------------------------------------------------------------------
 *  Joachim Kalmbach              Jk            Vector Informatik
 *  Holger Birke                  Bir           Vector Informatik
 *  Timo Vanoni                   Tvi           Vector Informatik
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  01.00.00  2007-08-01  Jk                    Initial creation
 *  03.00.00  2007-10-09  Jk                    Addaption for AUTOSAR 3.0
 *  03.00.01  2007-11-20  Jk                    Defines for CFG management added
 *  03.01.00  2008-03-18  Jk                    Version defines changed to specification version
 *  04.00.00  2011-03-15  Bir/Tvi               support ASR 4.0R1
 *  04.01.00  2012-06-29  Bir                   support ASR 4.0R3
 *  04.01.01  2012-08-23  visht/vsibir          version define corrected - replace PATCH by REVISION - (PLATFORM_AR_RELEASE_REVISION_VERSION)
 *            2012-11-05  visseu/visbir         PLATFORM_MODULE_ID added as specified in "AUTOSAR_TR_BSWModuleList.pdf" (R4.0 Rev 3), component version corrected
 *  04.02.00  2014-09-25  visbir                AR4-667: Support sint64 and uint64
 *                                              ESCAN00072531  Add suffix "u"
 *  04.03.00  2015-07-10  visbir                ESCAN00083945 *lease types to long
*********************************************************************************************************************/

#ifndef PLATFORM_TYPES_H
#define PLATFORM_TYPES_H

#include <inttypes.h> // Attention! Deviation: Added include for linux system
#include <stddef.h> // Attention! Deviation: Added include for linux system

/**********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

/* ##V_CFG_MANAGEMENT ##CQProject : CommonAsr_Canoeemu CQComponent : Impl_PlatformTypes */
#define COMMONASR_CANOEEMU_IMPL_PLATFORMTYPES_VERSION 0x0403
#define COMMONASR_CANOEEMU_IMPL_PLATFORMTYPES_RELEASE_VERSION 0x00

#define PLATFORM_VENDOR_ID    30u    /* SREQ00015439, SREQ00015413 */
#define PLATFORM_MODULE_ID    199u   /* SREQ00015439, SREQ00015413 */


/* AUTOSAR Software Specification Document Version Information */

/* AUTOSAR release 4.0 R3 */
#   define PLATFORM_AR_RELEASE_MAJOR_VERSION      (4u)
#   define PLATFORM_AR_RELEASE_MINOR_VERSION      (0u)
#   define PLATFORM_AR_RELEASE_REVISION_VERSION   (3u)

/* Component Version Information */
#define PLATFORM_SW_MAJOR_VERSION       (4u)
#define PLATFORM_SW_MINOR_VERSION       (3u)
#define PLATFORM_SW_PATCH_VERSION       (0u)

#define CPU_TYPE_8       8u
#define CPU_TYPE_16      16u
#define CPU_TYPE_32      32u
#define CPU_TYPE_64      64u

#define MSB_FIRST        0u    /* big endian bit ordering */
#define LSB_FIRST        1u    /* little endian bit ordering */

#define HIGH_BYTE_FIRST  0u    /* big endian byte ordering */
#define LOW_BYTE_FIRST   1u    /* little endian byte ordering */

#ifndef TRUE
#define TRUE   1u
#endif

#ifndef FALSE
#define FALSE  0u
#endif

#if defined(__x86_64__) || defined(__x86_64) || defined(__amd64__) \
  || defined(__aarch64__) || (__mips == 64)
#define CPU_TYPE         CPU_TYPE_64
#elif defined(__i386__) || defined(__i386) || defined(__arm__) || (__mips == 32)
#define CPU_TYPE         CPU_TYPE_32
#else
#error "CPU type is not supported"
#endif

#if defined(__BYTE_ORDER__)
# if (__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__)
#  define CPU_BIT_ORDER    LSB_FIRST
#  define CPU_BYTE_ORDER   LOW_BYTE_FIRST
# elif(__BYTE_ORDER__ == __ORDER_BIG_ENDIAN__)
#  define CPU_BIT_ORDER    MSB_FIRST
#  define CPU_BYTE_ORDER   HIGH_BYTE_FIRST
# else
#  error "Mixed Endianess not supported!"
# endif
#else
# error "__BYTE__ORDER__ not defined by compiler!"
#endif

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/



/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

typedef uint8_t boolean;       /*        TRUE .. FALSE           */

typedef int8_t sint8;         /*        -127 .. +127            */
typedef uint8_t uint8;         /*           0 .. 255             */
typedef int16_t sint16;        /*      -32767 .. +32767          */
typedef uint16_t uint16;        /*           0 .. 65535           */
typedef int32_t sint32;        /* -2147483647 .. +2147483647     */
typedef uint32_t uint32;        /*           0 .. 4294967295      */

typedef int_least8_t sint8_least;   /* At least 7 bit + 1 bit sign    */
typedef uint_least8_t uint8_least;   /* At least 8 bit                 */
typedef int_least16_t sint16_least;  /* At least 15 bit + 1 bit sign   */
typedef uint_least16_t uint16_least;  /* At least 16 bit                */
typedef int_least32_t sint32_least;  /* At least 31 bit + 1 bit sign   */
typedef uint_fast32_t uint32_least;  /* At least 32 bit                */
#define PLATFORM_SUPPORT_SINT64_UINT64
typedef int64_t sint64;         /* -9223372036854775808 .. 9223372036854775807      */
typedef uint64_t uint64;         /*                    0 .. 18446744073709551615     */

typedef float float32;
typedef double float64;

/**********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/


#endif  /* PLATFORM_TYPES_H */

/**********************************************************************************************************************
 *  END OF FILE: Platform_Types.h
 *********************************************************************************************************************/
