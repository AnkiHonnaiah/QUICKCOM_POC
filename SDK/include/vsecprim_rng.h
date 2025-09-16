/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2023 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file  vsecprim_rng.h
 *        \brief  Callback registration function and implementation for usage in adaptive CryptoProvider
 *
 *********************************************************************************************************************/
#ifndef AMSR_VECTOR_FS_SEC_CRYPTOSTACK_DRIVER_LIB_ES_VSECPRIM_RNG_H
#define AMSR_VECTOR_FS_SEC_CRYPTOSTACK_DRIVER_LIB_ES_VSECPRIM_RNG_H

#include "ESLib_ERC.h"
#include "ESLib_t.h"

#ifdef __cplusplus /* COV_VSECPRIM_CPLUSPLUS XF */
extern "C" {
#endif

/*!
 * \brief Callback registration function to be used in adaptive crypto provider
 * \param callback Callback function pointer to be registered
 * \return Errorcode for successful registration
 */
eslt_ErrorCode esl_RegisterRngCallback(eslt_ErrorCode (*callback)(const eslt_Length targetLength, eslt_Byte* target));

#ifdef __cplusplus /* COV_VSECPRIM_CPLUSPLUS XF */
} /* extern "C" */
#endif

#endif  // AMSR_VECTOR_FS_SEC_CRYPTOSTACK_DRIVER_LIB_ES_VSECPRIM_RNG_H
