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
/*!        \file  actEd448core.h
 *        \brief  Core Ed448 implementation.
 *
 *      \details This file is part of the embedded systems library cvActLib
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

#ifndef ACTED448CORE_H
# define ACTED448CORE_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

# include "actITypes.h"

# define VSECPRIM_START_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

# ifdef __cplusplus                                       /* COV_VSECPRIM_CPLUSPLUS XF */
extern "C"
{
# endif

/**********************************************************************************************************************
 *  actEd448_initCore()
 *********************************************************************************************************************/
/*! \brief       Init core.
 *  \details     This function initializes the internal data structures for Ed448.
 *  \param[in,out]  ws  Pointer to the work-space structure.
 *  \pre         ws must be a valid workspace pointer
 *  \context     TASK
 *  \reentrant   TRUE, for different workspaces
 *  \synchronous TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(void) actEd448_initCore(VSECPRIM_P2VAR_PARA(actEdDSASTRUCT) ws);

/**********************************************************************************************************************
 *  actEd448_sign()
 *********************************************************************************************************************/
/*! \brief       Creates a signature over the data.
 *  \details     This function creates a signature over the given data for Ed448.
 *  \param[in,out]  ws  Pointer to the work-space structure.
 *  \param[out]  signature  Pointer to the signature buffer (114 bytes).
 *  \param[in]  watchdog  Pointer to watchdog trigger function.
 *  \return      actOK                - Signature generation successful.
 *               actEXCEPTION_UNKNOWN - invert 0.
 *  \pre         ws must be a valid workspace pointer
 *               signature must be a valid pointer
 *               ws must be initialized
 *  \context     TASK
 *  \reentrant   TRUE, for different workspaces
 *  \synchronous TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC(actRETURNCODE) actEd448_sign(
  VSECPRIM_P2VAR_PARA(actEdDSASTRUCT) ws,
  VSECPRIM_P2VAR_PARA(actU8) signature,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

/**********************************************************************************************************************
 *  actEd448_verify()
 *********************************************************************************************************************/
/*! \brief       Verifies signature over data.
 *  \details     This function verifies a signature over the data for Ed448.
 *  \param[in,out]  ws  Pointer to the work-space structure.
 *  \param[out]  signature  Pointer to the signature.
 *  \param[in]  watchdog  Pointer to watchdog trigger function.
 *  \return      actOK - Signatures verification successful.
 *               actEXCEPTION_UNKNOWN - inversion fails.
 *               actVERIFICATION_FAILED - otherwise.
 *  \pre         ws must be a valid workspace pointer
 *               signature must be a valid pointer
 *               ws must be initialized
 *  \context     TASK
 *  \reentrant   TRUE, for different workspaces
 *  \synchronous TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(actRETURNCODE) actEd448_verify(
  VSECPRIM_P2VAR_PARA(actEdDSASTRUCT) ws,
  VSECPRIM_P2CONST_PARA(actU8) signature,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

/**********************************************************************************************************************
 *  actEd448_importKeyPair()
 *********************************************************************************************************************/
/*! \brief       Import key pair.
 *  \details     This function imports the secret and / or public key for signature generation / 
 *               verification for Ed448.
 *               The private scalar is derived from the secret key, and when no public key is passed,
 *               the public key is calculated.
 *  \param[in,out]  ws  Pointer to the work-space structure.
 *  \param[in]   secretKey  Pointer to the secret key (57 bytes).
 *  \param[in]   publicKey  Pointer to the public key (57 bytes).
 *  \param[in]   watchdog  Pointer to watchdog trigger function.
 *  \return      actOK                - Import key pair successful.
 *               actEXCEPTION_UNKNOWN - invert 0.
 *  \pre         ws must be a valid workspace pointer
 *               secretKey, publicKey must be valid pointers
 *               ws must be initialized
 *  \context     TASK
 *  \reentrant   TRUE, for different workspaces
 *  \synchronous TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC(actRETURNCODE) actEd448_importKeyPair(
  VSECPRIM_P2VAR_PARA(actEdDSASTRUCT) ws,
  VSECPRIM_P2CONST_PARA(actU8) secretKey,
  VSECPRIM_P2CONST_PARA(actU8) publicKey,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

# ifdef __cplusplus                                       /* COV_VSECPRIM_CPLUSPLUS XF */
}
# endif

# define VSECPRIM_STOP_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* ACTED448CORE_H */

/**********************************************************************************************************************
 *  END OF FILE: actEd448core.h
 *********************************************************************************************************************/
