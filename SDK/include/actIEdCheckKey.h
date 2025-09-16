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
/*!        \file  actIEdCheckKey.h
 *        \brief  Headerfile for actIEdCheckKey.c
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

#ifndef ACTIEDCHECK_H
# define ACTIEDCHECK_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "actITypes.h"


/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

#if (VSECPRIM_ACTIEDCHECKKEY_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
# define VSECPRIM_START_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  actEdinitCheckKey()
 *********************************************************************************************************************/
/*! \brief          Initialize Edwardscurve Key Check
 *  \details        -
 *  \param[in,out]  workspace    (in)  workspace
 *                               (out) initialized workspace
 *  \pre            workspace must be a valid pointer
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
 VSECPRIM_FUNC(void) actEdinitCheckKey(
   VSECPRIM_P2VAR_PARA(actEdwardsKeyCheck) workspace);

/**********************************************************************************************************************
 *  actEdCheckValidityOfPublicKey()
 *********************************************************************************************************************/
/*! \brief          The function checks if a public key is valid.
 *  \details        The function checks if a public key is valid by checking that:
 *                           1. the key length is as expected 
 *                           2. the key's coordinates are within range [0,p)
 *                           3. the key is on the curve 
 *  \param[in,out]  workspace          pointer to workspace
 *  \param[in]      publicKey          pointer to public key
 *  \param[in]      publicKeyLength    length of public key
 *  \param[in]      watchdog           pointer to watchdog trigger function
 *  \return         actEXCEPTION_INPUT_LENGTH  public key length is not as expected
 *                  actEXCEPTION_PUBKEY        the public key is invalid, meaning the public key is either zero or 
 *                                             not on the curve
 *                  actEXCEPTION_UNKNOWN       error during decoding the public key
 *                  actOK                      else
 *  \pre            pointers must be valid
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
 VSECPRIM_FUNC(actRETURNCODE) actEdCheckValidityOfPublicKey(
    VSECPRIM_P2VAR_PARA(actEdwardsKeyCheck) workspace,
    VSECPRIM_P2CONST_PARA(actU8) publicKey,
    const actLengthType publicKeyLength,
    VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));
    
/**********************************************************************************************************************
 *  actEdCheckValidityOfPrivateKey()
 *********************************************************************************************************************/
/*! \brief          Passes parameters to private key check function. 
 *  \details        The function checks if a private key is valid by checking that:
 *                           1. the key length is as expected
 *                           2. the key is not zero
 *  \param[in]      workspace           pointer to workspace
 *  \param[in]      privateKey          pointer to private key
 *  \param[in]      privateKeyLength    length of private key
 *  \return         actEXCEPTION_INPUT_LENGTH  private key length is not as expected
 *                  actEXCEPTION_PRIVKEY       private key is invalid
 *                  actEXCEPTION_UNKNOWN       other error
 *                  actOK                      else
 *  \pre            pointers must be valid
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
  VSECPRIM_FUNC(actRETURNCODE) actEdCheckValidityOfPrivateKey(
    VSECPRIM_P2CONST_PARA(actEdwardsKeyCheck) workspace,
    VSECPRIM_P2CONST_PARA(actU8) privateKey,
    const actLengthType privateKeyLength);


# define VSECPRIM_STOP_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */
#endif /* (VSECPRIM_ACTIEDCHECKKEY_ENABLED == STD_ON) */
#endif /* ACTIEDCHECK_H */

/**********************************************************************************************************************
 *  END OF FILE: actIEdCheckKey.h
 *********************************************************************************************************************/
