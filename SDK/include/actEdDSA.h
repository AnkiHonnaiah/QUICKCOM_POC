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
/*!        \file  actEdDSA.h
 *        \brief  Ed25519 and Ed448 helper functions
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

#ifndef ACTEDDSA_H
# define ACTEDDSA_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

# include "actITypes.h"
# include "actIEdDSA.h"
# include "actISHA2_64.h"
# include "actISHAKE.h"

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
 *  act25519_hashPM()
 *********************************************************************************************************************/
/*! \brief       Hash the private key and message
 *  \details     This function performs step 2 of the Ed25519 signature algorithm.
 *               https:/ /tools.ietf.org/html/draft-irtf-cfrg-eddsa-08#section-5.1.6
 *  \param[in,out]  ws  Pointer to the work-space structure.
 *  \param[out]  hash  Pointer to the output buffer for the hash value
 *  \param[in]   watchdog  Pointer to watchdog trigger function.
 *  \return      actOK  -  actOK is always returned.
 *  \pre         Hash must be at least size of info->result_length. Full hash is 64 byte.
 *               ws, hash must be valid pointers
 *               ws must be initialized
 *  \context     TASK
 *  \reentrant   TRUE, for different workspaces
 *  \synchronous TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC(actRETURNCODE) act25519_hashPM(
  VSECPRIM_P2VAR_PARA(actEdDSASTRUCT) ws,
  VSECPRIM_P2VAR_PARA(actU8) hash,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

/**********************************************************************************************************************
 *  act25519_hashRAM()
 *********************************************************************************************************************/
/*! \brief       Hash R, A and the Message
 *  \details     This function performs step 4 of the Ed25519 signature algorithm
 *               https:/ /tools.ietf.org/html/draft-irtf-cfrg-eddsa-08#section-5.1.6
 *               or step 2 of the Ed25519 verification algorithm
 *               https:/ /tools.ietf.org/html/draft-irtf-cfrg-eddsa-08#section-5.1.7
 *  \param[in,out]  ws  Pointer to the work-space structure.
 *  \param[in]   R  Pointer to the R value.
 *  \param[out]  hash  Pointer to the output buffer for the hash value.
 *  \param[in]   watchdog  Pointer to watchdog trigger function.
 *  \return      actOK  -  actOK is always returned.
 *  \pre         Hash must be at least size of info->result_length. Full hash is 64 byte.
 *               ws, hash must be valid pointers
 *               ws must be initialized
 *  \context     TASK
 *  \reentrant   TRUE, for different workspaces
 *  \synchronous TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC(actRETURNCODE) act25519_hashRAM(
  VSECPRIM_P2VAR_PARA(actEdDSASTRUCT) ws,
  VSECPRIM_P2CONST_PARA(actU8) R,
  VSECPRIM_P2VAR_PARA(actU8) hash,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

# ifdef __cplusplus                                       /* COV_VSECPRIM_CPLUSPLUS XF */
}                               /* extern "C" */
# endif

# define VSECPRIM_STOP_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /*  ACTEDDSA_H */

/**********************************************************************************************************************
 *  END OF FILE: actEdDSA.h
 *********************************************************************************************************************/
