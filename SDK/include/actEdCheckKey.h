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
/*!        \file  actEdCheckKey.h
 *        \brief  Headerfile for actEdCheckKey.c
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

#ifndef ACTEDCHECK_H
# define ACTEDCHECK_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "actITypes.h"


/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

#if (VSECPRIM_ACTEDCHECKKEY_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
# define VSECPRIM_START_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  actEdKeyCheck_initCore()
 *********************************************************************************************************************/
/*! \brief       Initialize Edwardscurve Key Check
 *  \details     This function initializes the Ed25519 or the Ed448 algorithm for key validation 
 *               depending on given curve.
 *  \param[in,out]       workspace   pointer to the workspace
 *  \pre         all provided pointers must be valid
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC(void) actEdKeyCheck_initCore(VSECPRIM_P2VAR_PARA(actEdwardsKeyCheck) workspace);

/**********************************************************************************************************************
 *  actEdCheckPublicKey()
 *********************************************************************************************************************/
/*! \brief       Passes parameters to specific public key check function. 
 *  \details     Differentiates between the two curves and calls the corresponding key check function
 *  \param[in,out]  workspace  pointer to the workspace
 *  \param[in]      publicKey  Pointer to the public key
 *  \param[in]      watchdog   Pointer to watchdog trigger function
 *  \return      actOK - success
 *               actEXCEPTION_UNKNOWN - invalid key
 *               actEXCEPTION_PUBKEY  - key is not on curve 
 *  \pre         all provided pointers must be valid
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
 VSECPRIM_FUNC(actRETURNCODE) actEdCheckPublicKey(
    VSECPRIM_P2VAR_PARA(actEdwardsKeyCheck) workspace,
    VSECPRIM_P2CONST_PARA(actU8) publicKey,
    VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));


/**********************************************************************************************************************
 *  actEdCheckPrivateKey()
 *********************************************************************************************************************/
/*! \brief          The function checks if a private key is valid.
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
 VSECPRIM_FUNC(actRETURNCODE) actEdCheckPrivateKey(
   VSECPRIM_P2CONST_PARA(actEdwardsKeyCheck) workspace,
   VSECPRIM_P2CONST_PARA(actU8) privateKey,
   const actLengthType privateKeyLength);


# define VSECPRIM_STOP_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */
#endif /* (VSECPRIM_ACTEDCHECKKEY_ENABLED == STD_ON) */
#endif /* ACTEDCHECK_H */

/**********************************************************************************************************************
 *  END OF FILE: actEdCheckKey.h
 *********************************************************************************************************************/
