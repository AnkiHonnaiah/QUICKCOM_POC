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
/*!        \file  actIECCheckKey.h
 *        \brief  Headerfile for actIECCheckKey.c
 *
 *      \details Currently the actClib version is used.
 *               This file is part of the embedded systems library cvActLib/ES
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

#ifndef ACTIECCHECKKEY_H
# define ACTIECCHECKKEY_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "actITypes.h"

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
# define VSECPRIM_START_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

# ifdef __cplusplus                                       /* COV_VSECPRIM_CPLUSPLUS XF */
extern "C"
{
# endif

/**********************************************************************************************************************
 *  actECinitCheckKeyValidity()
 *********************************************************************************************************************/
/*! \brief          Pass input parameters to actECinitCheckKey.
 *  \details        -
 *  \param[in]      domain       domain parameter
 *  \param[in]      domain_ext   domain parameter extensions (Montgomery constants etc.)
 *  \param[in,out]  wksp         (in) workspace
 *                               (out) initialized workspace
 *  \param[in]      wksp_len     length of workspace in bytes
 *  \return         actEXCEPTION_DOMAIN       domain decoding error
 *                  actEXCEPTION_DOMAIN_EXT   domain_ext decoding error
 *                  actEXCEPTION_SPEEDUP_EXT  speedup_ext decoding error
 *                  actEXCEPTION_MEMORY       wksp_len to small
 *                  actOK                     else
 *  \pre            domain, domain_ext and wksp must be valid pointers
 *                  domain, domain_ext and speedup_ext need to be valid ASN1 structures for EC domain parameters and
 *                  precomputed points.
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
 VSECPRIM_FUNC(actRETURNCODE) actECinitCheckKeyValidity(
   VSECPRIM_P2ROMCONST_PARA(actU8) domain,
   VSECPRIM_P2ROMCONST_PARA(actU8) domain_ext,
   VSECPRIM_P2VAR_PARA(actECPBasicStruct) wksp,
   actLengthType wksp_len);

/**********************************************************************************************************************
 *  actECCheckValidityOfPublicKey()
 *********************************************************************************************************************/
/*! \brief       Check the validity of the public key
 *  \details     Checks if the public key is within range or if it's zero
*                and checks if the public key is on the given curve
 *  \param[in,out]  workspace           pointer to workspace
 *  \param[in]      publicKey_x         pointer to the public key's x-coordinate
 *  \param[in]      publicKeyLength_x   length of the public key's x-coordinate
 *  \param[in]      publicKey_y         pointer to the public key's y-coordinate
 *  \param[in]      publicKeyLength_y   length of the public key's y-coordinate
 *  \param[in]      watchdog            Pointer to watchdog trigger function
 *  \return      actEXCEPTION_PUBKEY    public key is not within the range
 *               actOK                  else
 *  \pre         Pointers must be valid.
 *  \context     TASK
 *  \reentrant   TRUE, for different workspaces
 *  \synchronous TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC(actRETURNCODE) actECCheckValidityOfPublicKey(VSECPRIM_P2CONST_PARA(act_WorkSpaceEcPKeyCheck) workspace, 
   VSECPRIM_P2CONST_PARA(actU8) publicKey_x, const actLengthType publicKeyLength_x,
   VSECPRIM_P2CONST_PARA(actU8) publicKey_y, const actLengthType publicKeyLength_y, 
   VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

/**********************************************************************************************************************
 *  actECCheckValidityOfPrivateKey()
 *********************************************************************************************************************/
/*! \brief       Pass input parameters to actECinitCheckKey.
 *  \details     This function takes the private key and checks if it is in range from (0, n) or if it's zero
 *  \param[in]           curveInformation   pointer to curve information
 *  \param[in]           privateKeyInBytes       pointer to the given private key
 *  \param[in]           givenPrivateKeyLength   length of the given private key
 *  \return      actEXCEPTION_PRIVKEY       private key is not within the range
 *               actOK                      else
 *  \pre         all provided pointers must be valid
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC(actRETURNCODE) actECCheckValidityOfPrivateKey(
  VSECPRIM_P2CONST_PARA(actECCURVE) curveInformation,
  VSECPRIM_P2CONST_PARA(actU8) privateKeyInBytes,
  const actLengthType givenPrivateKeyLength);

# ifdef __cplusplus                                       /* COV_VSECPRIM_CPLUSPLUS XF */
}                               /* extern "C" */
# endif

# define VSECPRIM_STOP_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* ACTIECCHECKKEY_H */

/**********************************************************************************************************************
 *  END OF FILE: actIECKeyCheck.h
 *********************************************************************************************************************/
