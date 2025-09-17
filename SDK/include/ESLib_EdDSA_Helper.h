/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2023 by Vector Informatik GmbH.                                              All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file  ESLib_EdDSA_Helper.h
 *        \brief  ES Library internal header file
 *
 *      \details  Contains type definitions and helper function declarations, which are internally used
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *
 *  FILE VERSION
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to module's header file.
 *********************************************************************************************************************/

#ifndef ESLIB_EDDSA_HELPER_H
# define ESLIB_EDDSA_HELPER_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

# include "ESLib.h"
# include "ESLib_types.h"

# include "actUtilities.h"

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/
# define Esl_MemSet(dest, fill, count)                                (actMemset(dest, fill, count))
# define Esl_MemClear(dest, count)                                    (actMemClear(dest, count))
# define Esl_MemCpyByteArray(dest, src, count)                        (actMemCpyByteArray(dest, src, count))
# define Esl_MemCpyWordArray(dest, src, count)                        (actMemCpyWordArray(dest, src, count))

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

# define VSECPRIM_START_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  esl_checkKeyLength()
 **********************************************************************************************************************/
/*! \brief         Validate key length.
 *  \details       This function validates the key length.
 *  \param[in]     keyLength  key length.
 *                            For Ed25519:
 *                              EdDSA:
 *                               publicKeyLength = 32
 *                               privateKeyLength = 32
 *                              EdDH:
 *                               publicKeyLength = 32
 *                               privateKeyLength = 32
 *                            For Ed448:
 *                              EdDSA:
 *                               publicKeyLength = 57
 *                               privateKeyLength = 57
 *                              EdDH:
 *                               publicKeyLength = 56
 *                               privateKeyLength = 56
 *  \param[in]     expectedLength  expected key length.
 *  \return        ESL_ERC_NO_ERROR  Operation successful
 *                 ESL_ERC_INVALID_LENGTH  keyLength is smaller than the expected reference value.
 *  \pre           Workspace needs to be initialized.
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC(eslt_ErrorCode) esl_checkKeyLength(
  const eslt_Length keyLength,
  const eslt_Length expectedLength);

/**********************************************************************************************************************
 *  esl_validateCommonSecretLength()
 **********************************************************************************************************************/
/*! \brief         Validate shared secret length pointer.
 *  \details       This function validates the common shared secret length pointer.
 *  \param[in]     curve               ID of the reference curve for which the shared secret length shall be checked.
 *  \param[in]     sharedSecretLength  Reference to the shared secret length.
 *  \return        ESL_ERC_NO_ERROR  Operation successful
 *                 ESL_ERC_PARAMETER_INVALID sharedSecretLength is not a valid pointer
 *                 ESL_ERC_BUFFER_TOO_SMALL *sharedSecretLength is smaller than the expected reference value.
 *  \pre           Workspace needs to be initialized.
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC(eslt_ErrorCode) esl_validateCommonSecretLength(
  eslt_Size16 curve,
  VSECPRIM_P2CONST_PARA(eslt_Length) sharedSecretLength);

/**********************************************************************************************************************
 *  esl_checkInstanceContext()
 **********************************************************************************************************************/
/*! \brief         Check instance context.
 *  \details       This function checks the curves instance contexts.
 *  \param[in]     instance  Instance identifier
 *  \param[in]     context  Context buffer pointer
 *  \param[in]     contextLength Context buffer length
 *  \param[in]     curve identifier -  underlying curve; supports
 *                      - ESL_CURVE25519
 *                      - ESL_CURVE448
 *  \return        ESL_ERC_NO_ERROR  Operation successful
 *                 ESL_ERC_INSTANCE_NOT_SUPPORTED  Instance is not supported
 *                 ESL_ERC_PARAMETER_INVALID  Invalid parameter provided
 *                 ESL_ERC_CURVE_NOT_SUPPORTED  neither Ed25519 nor Ed448 was chosen as underlying curve
 *  \pre           Workspace needs to be initialized.
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC(eslt_ErrorCode) esl_checkInstanceContext(
  const eslt_Byte instance,
  VSECPRIM_P2CONST_PARA(eslt_Byte) context,
  const eslt_Length contextLength,
  const eslt_Size16 curve);

# define VSECPRIM_STOP_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* ESLIB_EdDSA_HELPER_H */

/**********************************************************************************************************************
 *  END OF FILE: ESLib_EdDSA_Helper.h
 *********************************************************************************************************************/
