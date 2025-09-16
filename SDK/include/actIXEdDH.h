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
/*!        \file  actIXEdDH.h
 *        \brief  X25519 and X448 internal programming interface
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

#ifndef ACTIXEDDH_H
# define ACTIXEDDH_H

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
 *  actEdDHInit()
 *********************************************************************************************************************/
/*! \brief       Init X25519 or X448.
 *  \details     This function initializes the X25519 or the X448 algorithm.
 *  \param[in,out]  ws  Pointer to workspace.
 *  \return      actOK  -  actOK is always returned.
 *  \pre         ws must be a valid pointer
 *  \context     TASK
 *  \reentrant   TRUE, for different workspaces
 *  \synchronous TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(actRETURNCODE) actEdDHInit(VSECPRIM_P2VAR_PARA(actEdDHSTRUCT) ws);

/**********************************************************************************************************************
 *  actEdDHGenKeyPair()
 *********************************************************************************************************************/
/*! \brief       Generate key pair.
 *  \details     This function generates an ephemeral key-pair for EC-D/H.
 *  \param[in,out]  ws  Pointer to the work-space structure.
 *  \param[out]  publicKey  Pointer to memory where the public key shall be stored.
 *  \param[in]  watchdog  Pointer to watchdog trigger function.
 *  \return      actOK                - Generation of ephemeral key-pair successful.
 *               actEXCEPTION_UNKNOWN - Invert 0.
 *  \pre         ws, publicKey must be valid pointers
 *               ws must be initialized
 *               publicKey must be at least 32 bytes
 *  \context     TASK
 *  \reentrant   TRUE, for different workspaces
 *  \synchronous TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(actRETURNCODE) actEdDHGenKeyPair(
  VSECPRIM_P2VAR_PARA(actEdDHSTRUCT) ws,
  VSECPRIM_P2VAR_PARA(actU8) publicKey,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

/**********************************************************************************************************************
 *  actEdDHImportPrivateKey()
 *********************************************************************************************************************/
/*! \brief       Import private key.
 *  \details     This function imports the private key for EC-D/H.
 *  \param[in,out]  ws  Pointer to the work-space structure.
 *  \param[in]  privateKey  Pointer to the private key buffer.
 *  \param[in]  watchdog  Pointer to watchdog trigger function.
 *  \pre         ws, privateKey must be valid pointers
 *               ws must be initialized
 *               privateKey must be at least 32 bytes
 *  \context     TASK
 *  \reentrant   TRUE, for different workspaces
 *  \synchronous TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(void) actEdDHImportPrivateKey(
  VSECPRIM_P2VAR_PARA(actEdDHSTRUCT) ws,
  VSECPRIM_P2CONST_PARA(actU8) privateKey,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

/**********************************************************************************************************************
 *  actEdDHGenSecret()
 *********************************************************************************************************************/
/*! \brief       Generate shared secret.
 *  \details     This function generates the shared secret for EC-D/H.
 *  \param[in,out]  ws  Pointer to the work-space structure.
 *  \param[in]  publicKey  Pointer to the public key buffer.
 *  \param[out]  sharedSecret  Pointer to the buffer where the shared secret shall be stored.
 *  \param[in]  watchdog  Pointer to watchdog trigger function.
 *  \return      actOK                - Secret generation successful.
 *               actEXCEPTION_UNKNOWN - Invert 0.
 *  \pre         ws, publicKey, sharedSecret must be valid pointers
 *               ws must be initialized
 *               publicKey, sharedSecret must be at least 32 bytes
 *  \context     TASK
 *  \reentrant   TRUE, for different workspaces
 *  \synchronous TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(actRETURNCODE) actEdDHGenSecret(
  VSECPRIM_P2VAR_PARA(actEdDHSTRUCT) ws,
  VSECPRIM_P2CONST_PARA(actU8) publicKey,
  VSECPRIM_P2VAR_PARA(actU8) sharedSecret,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

# ifdef __cplusplus                                       /* COV_VSECPRIM_CPLUSPLUS XF */
} /* extern "C" */
# endif

# define VSECPRIM_STOP_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* ACTIXEDDH_H */

/**********************************************************************************************************************
 *  END OF FILE: actIXEdDH.h
 *********************************************************************************************************************/
