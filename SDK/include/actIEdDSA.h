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
/*!        \file  actIEdDSA.h
 *        \brief  Ed25519 and Ed448 internal programming interface
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

#ifndef ACTIEDDSA_H
# define ACTIEDDSA_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

# include "actITypes.h"
# include "actISHA2_64.h"

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
 *  actEdDSAInit()
 *********************************************************************************************************************/
/*! \brief       Init Ed25519 or Ed448.
 *  \details     This function initializes the Ed25519 or Ed448 algorithm respectively
 *  \param[in,out]  ws  Pointer to the work-space structure.
 *  \param[in]   instance  EdDSA instance to use; for Ed25519 use:
 *                            - ESL_INSTANCE_Ed25519
 *                            - ESL_INSTANCE_Ed25519CTX
 *                            - ESL_INSTANCE_Ed25519PH
 *                                                 for Ed448 use:
 *                            - ESL_INSTANCE_Ed448
 *                            - ESL_INSTANCE_Ed448PH
 *  \param[in]   context   Pointer to the context data
 *  \param[in]   ctxLen    Length of the context data
 *  \param[in]   curve  underlying curve; supports
 *                      - actEDWARDSCURVE25519
 *                      - actEDWARDSCURVE448
 *  \return      actOK  -  actOK is always returned.
 *  \pre         ws must be a valid pointer.
 *  \context     TASK
 *  \reentrant   TRUE, for different workspaces
 *  \synchronous TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(actRETURNCODE) actEdDSAInit(
  VSECPRIM_P2VAR_PARA(actEdDSASTRUCT) ws,
  const actU8 instance,
  VSECPRIM_P2CONST_PARA(actU8) context,
  const actLengthType ctxLen,
  const actU16 curve);

/**********************************************************************************************************************
 *  actEdDSAUpdate()
 *********************************************************************************************************************/
/*! \brief       Update Ed25519 or Ed448.
 *  \details     This function updates the internal hash (SHA512 for Ed25519 or SHAKE256 for Ed448), if pre-hash is used.
 *  \param[in,out]  ws     Pointer to the work-space structure.
 *  \param[in]   message   Pointer to the input data.
 *  \param[in]   msgLen    Length of the input data.
 *  \param[in]   watchdog  Pointer to watchdog trigger function.
 *  \return      actOK  -  Update was successful.
 *               actEXCEPTION_ALGOID  -  Invalid instance.
 *               actEXCEPTION_LENGTH  -  Length exception.
 *               actEXCEPTION_MODE - when actSHAKEUpdate() is called AFTER getDigest
 *  \pre         pointers must be valid
 *               ws must be initialized
 *  \context     TASK
 *  \reentrant   TRUE, for different workspaces
 *  \synchronous TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(actRETURNCODE) actEdDSAUpdate(
  VSECPRIM_P2VAR_PARA(actEdDSASTRUCT) ws,
  VSECPRIM_P2CONST_PARA(actU8) message,
  const actLengthType msgLen,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

/**********************************************************************************************************************
 *  actEdDSASign()
 *********************************************************************************************************************/
/*! \brief       Sign provided message for curve Ed25519 or Ed448.
 *  \details     This function creates a signature over the data. When pre-hash is used, the data are hashed first.
 *               The final hash value is then signed.
 *  \param[in,out]  ws  Pointer to the work-space structure.
 *  \param[in]   message    Pointer to the input data.
 *  \param[in]   msgLen     Length of the input data
 *  \param[in]   secretKey  Pointer to the secret key used to sign data (32 bytes/57 bytes respectively)
 *  \param[in]   publicKey  Pointer to the public key used to sign data (32 bytes/57 bytes respectively - optional)
 *  \param[out]  signature  Pointer to the signature buffer (64 bytes/114 bytes respectively)
 *  \param[in]   watchdog   Pointer to watchdog trigger function.
 *  \return      actOK                    Signing successful
 *               actEXCEPTION_LENGTH      Hashing failure
 *               actEXCEPTION_UNKNOWN     Invert 0
 *  \pre         pointers must be valid
 *               ws must be initialized
 *               signature must be at least 512 bytes
 *  \context     TASK
 *  \reentrant   TRUE, for different workspaces
 *  \synchronous TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(actRETURNCODE) actEdDSASign(
  VSECPRIM_P2VAR_PARA(actEdDSASTRUCT) ws,
  VSECPRIM_P2CONST_PARA(actU8) message,
  const actLengthType msgLen,
  VSECPRIM_P2CONST_PARA(actU8) secretKey,
  VSECPRIM_P2CONST_PARA(actU8) publicKey,
  VSECPRIM_P2VAR_PARA(actU8) signature,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

/**********************************************************************************************************************
 *  actEdDSAVerify()
 *********************************************************************************************************************/
/*! \brief       Verifies signature over given data for curve Ed25519 or Ed448.
 *  \details     This function verifies the signature over the data using Ed25519 or Ed448. When pre-hash is used, the data is hashed first.
 *               The signature is then verified against the final hash value.
 *  \param[in,out]  ws  Pointer to the work-space structure.
 *  \param[in]   message    Pointer to the input data.
 *  \param[in]   msgLen     Length of the input data
 *  \param[in]   publicKey  Pointer to the key used to verify signature (32 bytes)/ (57 bytes)
 *  \param[in]   signature  Pointer to the signature buffer (64 bytes) / (114 bytes)
 *  \param[in]   watchdog   Pointer to watchdog trigger function.
 *  \return      actOK                     Signature is valid
 *               actVERIFICATION_FAILED    Signature is invalid
 *               actEXCEPTION_LENGTH       Hashing failure
 *               actEXCEPTION_UNKNOWN      Invert 0
 *  \pre         pointers must be valid
 *               ws must be initialized
 *  \context     TASK
 *  \reentrant   TRUE, for different workspaces
 *  \synchronous TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(actRETURNCODE) actEdDSAVerify(
  VSECPRIM_P2VAR_PARA(actEdDSASTRUCT) ws,
  VSECPRIM_P2CONST_PARA(actU8) message,
  const actLengthType msgLen,
  VSECPRIM_P2CONST_PARA(actU8) publicKey,
  VSECPRIM_P2CONST_PARA(actU8) signature,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

/**********************************************************************************************************************
 *  actEd25519GenKeyPair()
 *********************************************************************************************************************/
/*! \brief       Generate Key Pair.
 *  \details     This function generates a key pair.
 *  \param[in,out]  ws  Pointer to the work-space structure.
 *  \param[out]  secretKey  Pointer to the secret key buffer (32 bytes).
 *  \param[out]  publicKey  Pointer to the public key buffer (32 bytes).
 *  \param[in]   watchdog   Pointer to watchdog trigger function.
 *  \return      actOK  -  actOK is always returned.
 *               actEXCEPTION_PRIVKEY  -  Failed to generate private key
 *               actEXCEPTION_UNKNOWN  -  Invert 0
 *  \pre         pointers must be valid
 *               ws must be initialized
 *               secretKey, publicKey must be at least 32 bytes
 *  \context     TASK
 *  \reentrant   TRUE, for different workspaces
 *  \synchronous TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(actRETURNCODE) actEd25519GenKeyPair(
  VSECPRIM_P2VAR_PARA(actEdDSASTRUCT) ws,
  VSECPRIM_P2VAR_PARA(actU8) secretKey,
  VSECPRIM_P2VAR_PARA(actU8) publicKey,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

# ifdef __cplusplus                                       /* COV_VSECPRIM_CPLUSPLUS XF */
} /* extern "C" */
# endif

# define VSECPRIM_STOP_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* ACTIEDDSA_H */

/**********************************************************************************************************************
 *  END OF FILE: actIEdDSA.h
 *********************************************************************************************************************/
