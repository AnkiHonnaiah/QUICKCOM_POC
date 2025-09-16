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
/*!        \file  actX448core.h
 *        \brief  Interface to Core Curve448 implementation..
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

#ifndef ACTX448CORE_H
# define ACTX448CORE_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "act448util.h"

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
 *  actX448_initCore()
 *********************************************************************************************************************/
/*! \brief       Initialize the core.
 *  \details     This function initializes the internal data structures.
 *  \param[in,out]  ws  Pointer to the work-space structure.
 *  \pre         ws must be a valid pointer
 *  \context     TASK
 *  \reentrant   TRUE, for different workspaces
 *  \synchronous TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(void) actX448_initCore(VSECPRIM_P2VAR_PARA(actEdDHSTRUCT) ws);

/**********************************************************************************************************************
 *  actX448_eval()
 *********************************************************************************************************************/
/*! \brief       Evaluate Curve448
 *  \details     This function evaluates the raw Curve448 function. The private key and u coordinate(s) are passed
 *               in the work-space.
 *  \param[in,out]  ws  Pointer to the work-space structure.
 *  \param[in]      watchdog     pointer to the watchdog function
 *  \return      actOK                - Scalar multiplication successful.
 *               actEXCEPTION_UNKNOWN - Invert 0.
 *  \pre         ws must be a valid pointer
 *               ws must be initialized
 *  \context     TASK
 *  \reentrant   TRUE, for different workspaces
 *  \synchronous TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(actRETURNCODE) actX448_eval(
  VSECPRIM_P2VAR_PARA(actEdDHSTRUCT) ws,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

# ifdef __cplusplus                                       /* COV_VSECPRIM_CPLUSPLUS XF */
}
# endif

# define VSECPRIM_STOP_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* ACTX448CORE_H */

/**********************************************************************************************************************
 *  END OF FILE: actX448core.h
 *********************************************************************************************************************/
