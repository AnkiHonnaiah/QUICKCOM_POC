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
/*!        \file  actECCheckKey.h
 *        \brief  Headerfile for actECCheckKey.c
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

#ifndef ACTECCHECK_H
# define ACTECCHECK_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "actITypes.h"
# include "actECPoint.h"


/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

#if (VSECPRIM_ACTECCHECKKEY_ENABLED == STD_ON)
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
 *  actECinitCheckKey()
 *********************************************************************************************************************/
/*! \brief          This function initializes the ECC workspace.
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
 VSECPRIM_FUNC(actRETURNCODE) actECinitCheckKey(
    VSECPRIM_P2ROMCONST_PARA(actU8) domain,
    VSECPRIM_P2ROMCONST_PARA(actU8) domain_ext,
    VSECPRIM_P2VAR_PARA(actECPBasicStruct) wksp,
    actLengthType wksp_len);

/**********************************************************************************************************************
 *  actECCheckPublicKey()
 *********************************************************************************************************************/
/*! \brief       This function checks if the given public key is valid
 *  \details     This function takes the public key, represented as a point, transforms the coordinates of that point
 *               into Montgomery Representations and checks if the resulting coordinates are on the given curve.
 *  \param[in]   point     Represents the public key, coordinates of the public key are saved in this parameter
 *  \param[in]   curve     Includes the curve parameters
 *  \param[in]   watchdog  Pointer to watchdog trigger function
 *  \return      actEXCEPTION_POINT        if the point is not on curve
 *               actOK                     else
 *  \pre         all provided pointers must be valid, the coordinates of the 
 *               public key are not in montgomery representation
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
 VSECPRIM_FUNC(actRETURNCODE) actECCheckPublicKey(
   VSECPRIM_P2VAR_PARA(actECPOINT) point,
   VSECPRIM_P2CONST_PARA(actECCURVE) curve,
   VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

/**********************************************************************************************************************
 *  actECCheckPrivateKey()
 *********************************************************************************************************************/
/*! \brief       This function checks if the given private key is valid
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
VSECPRIM_FUNC(actRETURNCODE) actECCheckPrivateKey(
  VSECPRIM_P2CONST_PARA(actECCURVE) curveInformation,
  VSECPRIM_P2CONST_PARA(actU8) privateKeyInBytes,
  const actLengthType givenPrivateKeyLength);

# ifdef __cplusplus                                       /* COV_VSECPRIM_CPLUSPLUS XF */
}                               /* extern "C" */
# endif

# define VSECPRIM_STOP_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */
#endif /* (VSECPRIM_ACTECCHECKKEY_ENABLED == STD_ON) */
#endif /* ACTECCHECK_H */

/**********************************************************************************************************************
 *  END OF FILE: actECCHeckKey.h
 *********************************************************************************************************************/
