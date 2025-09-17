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
/*!        \file  act448util.h
 *        \brief  Curve448 utilities
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

#ifndef ACT448UTIL_H
# define ACT448UTIL_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

# include "actBigNum.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

# ifdef __cplusplus                                       /* COV_VSECPRIM_CPLUSPLUS XF */
extern "C"
{
# endif

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

# define VSECPRIM_START_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  act448_initCore()
 *********************************************************************************************************************/
/*! \brief       Init core x448.
 *  \details     This function initializes the core x448 data structure.
 *  \param[in,out]  ws  Pointer to workspace.
 *  \pre         ws must be a valid pointer.
 *  \context     TASK
 *  \reentrant   TRUE, for different workspaces
 *  \synchronous TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(void) act448_initCore(VSECPRIM_P2VAR_PARA(actEdwardsCurveWs) ws);

/**********************************************************************************************************************
 *  act448_reduceP()
 *********************************************************************************************************************/
/*! \brief       Reduce a mod p
 *  \details     This function reduces a given value a modulo p, where p is field prime (see below).
 *  \param[in,out]  ws  Pointer to workspace.
 *  \param[in,out]  r   Pointer to result.
 *  \param[in]      a   Pointer to dividend.
 *  \pre         Pointers must be valid.
 *               The buffer referenced by r must provide at least BNDIGITS_456 many actBNDIGITs.
 *               The buffer referenced by a must provide exactly BNDIGITS_912 many actBNDIGITs.
 *               a < p^2, where p is the curve prime p = 2^448 - 2^224 - 1.
 *  \context     TASK
 *  \reentrant   TRUE, for different workspaces
 *  \synchronous TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(void) act448_reduceP(
  VSECPRIM_P2VAR_PARA(actEdwardsCurveWs) ws,
  VSECPRIM_P2VAR_PARA(actBNDIGIT) r,
  VSECPRIM_P2CONST_PARA(actBNDIGIT) a);

/**********************************************************************************************************************
 *  act448_reduceQ()
 *********************************************************************************************************************/
/*! \brief       Reduce a mod q
 *  \details     This function reduces a given value a modulo q, where q is the group order defined as L
 *               in www.rfc-editor.org/rfc/rfc8032#section-5.2
 *  \param[in,out]  ws  Pointer to workspace.
 *  \param[in,out]  r   Pointer to result.
 *  \param[in]      a   Pointer to dividend.
 *  \pre         Pointers must be valid.
 *               The buffer referenced by r must provide at least BNDIGITS_456 many actBNDIGITs.
 *               The buffer referenced by a must provide exactly BNDIGITS_912 many actBNDIGITs.
 *  \context     TASK
 *  \reentrant   TRUE, for different workspaces
 *  \synchronous TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(void) act448_reduceQ(
  VSECPRIM_P2VAR_PARA(actEdwardsCurveWs) ws,
  VSECPRIM_P2VAR_PARA(actBNDIGIT) r,
  VSECPRIM_P2CONST_PARA(actBNDIGIT) a);

/**********************************************************************************************************************
 *  act448_addModP()
 *********************************************************************************************************************/
/*! \brief       r = (a + b) mod p
 *  \details     This function performs addition in a finite field.
 *  \param[in,out]  ws  Pointer to workspace.
 *  \param[in,out]  r   Pointer to result.
 *  \param[in]      a   Pointer to a.
 *  \param[in]      b   Pointer to b.
 *  \pre         Pointers must be valid.
 *               The buffer referenced by r must provide at least BNDIGITS_456 many actBNDIGITs.
 *               The buffer referenced by a must provide exactly BNDIGITS_456 many actBNDIGITs.
 *               The buffer referenced by b must provide exactly BNDIGITS_456 many actBNDIGITs.
 *  \context     TASK
 *  \reentrant   TRUE, for different workspaces
 *  \synchronous TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(void) act448_addModP(
  VSECPRIM_P2VAR_PARA(actEdwardsCurveWs) ws,
  VSECPRIM_P2VAR_PARA(actBNDIGIT) r,
  VSECPRIM_P2CONST_PARA(actBNDIGIT) a,
  VSECPRIM_P2CONST_PARA(actBNDIGIT) b);

/**********************************************************************************************************************
 *  act448_subModP()
 *********************************************************************************************************************/
/*! \brief       r = (a - b) mod p
 *  \details     This function performs subtraction in a finite field.
 *  \param[in,out]  ws  Pointer to workspace.
 *  \param[in,out]  r   Pointer to result.
 *  \param[in]      a   Pointer to a.
 *  \param[in]      b   Pointer to b.
 *  \pre         Pointers must be valid.
 *               The buffer referenced by r must provide at least BNDIGITS_456 many actBNDIGITs.
 *               The buffer referenced by a must provide exactly BNDIGITS_456 many actBNDIGITs.
 *               The buffer referenced by b must provide exactly BNDIGITS_456 many actBNDIGITs.
 *  \context     TASK
 *  \reentrant   TRUE, for different workspaces
 *  \synchronous TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(void) act448_subModP(
  VSECPRIM_P2VAR_PARA(actEdwardsCurveWs) ws,
  VSECPRIM_P2VAR_PARA(actBNDIGIT) r,
  VSECPRIM_P2CONST_PARA(actBNDIGIT) a,
  VSECPRIM_P2CONST_PARA(actBNDIGIT) b);

/**********************************************************************************************************************
 *  act448_mulModP()
 *********************************************************************************************************************/
/*! \brief       r = (a * b) mod p
 *  \details     This function performs multiplication in a finite field.
 *  \param[in,out]  ws  Pointer to workspace.
 *  \param[in,out]  r   Pointer to result.
 *  \param[in]      a   Pointer to a.
 *  \param[in]      b   Pointer to b.
 *  \pre         Pointers must be valid.
 *               The buffer referenced by r must provide at least BNDIGITS_456 many actBNDIGITs.
 *               The buffer referenced by a must provide exactly BNDIGITS_456 many actBNDIGITs.
 *               The buffer referenced by b must provide exactly BNDIGITS_456 many actBNDIGITs.
 *  \context     TASK
 *  \reentrant   TRUE, for different workspaces
 *  \synchronous TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(void) act448_mulModP(
  VSECPRIM_P2VAR_PARA(actEdwardsCurveWs) ws,
  VSECPRIM_P2VAR_PARA(actBNDIGIT) r,
  VSECPRIM_P2CONST_PARA(actBNDIGIT) a,
  VSECPRIM_P2CONST_PARA(actBNDIGIT) b);

/**********************************************************************************************************************
 *  act448_powModP()
 *********************************************************************************************************************/
/*! \brief       r = (a ^ b) mod p
 *  \details     This function proceeds the exponentiation of a base a with exponent b in a finite field.
 *  \param[in,out]  ws        Pointer to workspace.
 *  \param[in,out]  r         Pointer to result.
 *  \param[in]      a         Pointer to a.
 *  \param[in]      b         Pointer to b.
 *  \param[in]      watchdog  Pointer to watchdog trigger function.
 *  \pre         Pointers must be valid.
 *               The buffer referenced by r must provide at least BNDIGITS_456 many actBNDIGITs.
 *               The buffer referenced by a must provide exactly BNDIGITS_456 many actBNDIGITs.
 *               The buffer referenced by b must provide exactly BNDIGITS_456 many actBNDIGITs.
 *  \context     TASK
 *  \reentrant   TRUE, for different workspaces
 *  \synchronous TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(void) act448_powModP(
  VSECPRIM_P2VAR_PARA(actEdwardsCurveWs) ws,
  VSECPRIM_P2VAR_PARA(actBNDIGIT) r,
  VSECPRIM_P2VAR_PARA(actBNDIGIT) a,
  VSECPRIM_P2CONST_PARA(actBNDIGIT) b,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

/**********************************************************************************************************************
 *  act448_invModP()
 *********************************************************************************************************************/
/*! \brief       r = (1 / a) mod p
 *  \details     This function performs inversion in a finite field according to
 *               https:/ /tools.ietf.org/html/draft-irtf-cfrg-eddsa-08#section-5.2.1
 *  \param[in,out]  ws        Pointer to workspace.
 *  \param[in,out]  r         Pointer to result.
 *  \param[in]      a         Pointer to a.
 *  \param[in]      watchdog  Pointer to watchdog trigger function.
 *  \return      actOK - success
 *               actEXCEPTION_UNKNOWN - value "zero" not accepted
 *  \pre         Pointers must be valid.
 *               The buffer referenced by r must provide at least BNDIGITS_456 many actBNDIGITs.
 *               The buffer referenced by a must provide exactly BNDIGITS_456 many actBNDIGITs.
 *  \context     TASK
 *  \reentrant   TRUE, for different workspaces
 *  \synchronous TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(actRETURNCODE) act448_invModP(
  VSECPRIM_P2VAR_PARA(actEdwardsCurveWs) ws,
  VSECPRIM_P2VAR_PARA(actBNDIGIT) r,
  VSECPRIM_P2CONST_PARA(actBNDIGIT) a,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

/**********************************************************************************************************************
 *  act448_sqrtModP()
 *********************************************************************************************************************/
/*! \brief       r = a ^ ((p + 3) / 8) mod p
 *  \details     This function performs squaring in a finite field according to
 *               https:/ /tools.ietf.org/html/draft-irtf-cfrg-eddsa-08#section-5.2.3
 *  \param[in,out]  ws        Pointer to workspace.
 *  \param[in,out]  r         Pointer to result.
 *  \param[in]      a         Pointer to a.
 *  \param[in]      watchdog  Pointer to watchdog trigger function.
 *  \pre         Pointers must be valid.
 *               The buffer referenced by r must provide at least BNDIGITS_456 many actBNDIGITs.
 *               The buffer referenced by a must provide exactly BNDIGITS_456 many actBNDIGITs.
 *  \return      TRUE  - Found
 *               FALSE - Not Found
 *  \context     TASK
 *  \reentrant   TRUE, for different workspaces
 *  \synchronous TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(actU8) act448_sqrtModP(
  VSECPRIM_P2VAR_PARA(actEdwardsCurveWs) ws,
  VSECPRIM_P2VAR_PARA(actBNDIGIT) r,
  VSECPRIM_P2CONST_PARA(actBNDIGIT) a,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

/**********************************************************************************************************************
 *  act448_addModQ()
 *********************************************************************************************************************/
/*! \brief       r = (a + b) mod q
 *  \details     This function performs addition modulo the group order q defined in
 *               www.rfc-editor.org/rfc/rfc8032#section-5.2 under L.
 *  \param[in,out]  ws  Pointer to workspace.
 *  \param[in,out]  r   Pointer to result.
 *  \param[in]      a   Pointer to a.
 *  \param[in]      b   Pointer to b.
 *  \pre         Pointers must be valid.
 *               The buffer referenced by r must provide at least BNDIGITS_456 many actBNDIGITs.
 *               The buffer referenced by a must provide exactly BNDIGITS_456 many actBNDIGITs.
 *               The buffer referenced by b must provide exactly BNDIGITS_456 many actBNDIGITs.
 *  \context     TASK
 *  \reentrant   TRUE, for different workspaces
 *  \synchronous TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(void) act448_addModQ(
  VSECPRIM_P2VAR_PARA(actEdwardsCurveWs) ws,
  VSECPRIM_P2VAR_PARA(actBNDIGIT) r,
  VSECPRIM_P2CONST_PARA(actBNDIGIT) a,
  VSECPRIM_P2CONST_PARA(actBNDIGIT) b);

/**********************************************************************************************************************
 *  act448_mulModQ()
 *********************************************************************************************************************/
/*! \brief       r = (a * b) mod q
 *  \details     This function performs multiplication modulo the group order q defined in
 *               www.rfc-editor.org/rfc/rfc8032#section-5.2 under L.
 *  \param[in,out]  ws  Pointer to workspace.
 *  \param[in,out]  r   Pointer to result.
 *  \param[in]      a   Pointer to a.
 *  \param[in]      b   Pointer to b.
 *  \pre         Pointers must be valid.
 *               The buffer referenced by r must provide at least BNDIGITS_456 many actBNDIGITs.
 *               The buffer referenced by a must provide exactly BNDIGITS_456 many actBNDIGITs.
 *               The buffer referenced by b must provide exactly BNDIGITS_456 many actBNDIGITs.
 *  \context     TASK
 *  \reentrant   TRUE, for different workspaces
 *  \synchronous TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(void) act448_mulModQ(
  VSECPRIM_P2VAR_PARA(actEdwardsCurveWs) ws,
  VSECPRIM_P2VAR_PARA(actBNDIGIT) r,
  VSECPRIM_P2CONST_PARA(actBNDIGIT) a,
  VSECPRIM_P2CONST_PARA(actBNDIGIT) b);

/**********************************************************************************************************************
 *  act448_encodePoint()
 *********************************************************************************************************************/
/*! \brief       Encode point.
 *  \details     This function encodes a given point according to
 *               https:/ /tools.ietf.org/html/draft-irtf-cfrg-eddsa-08#section-5.2.2
 *  \param[in,out]  ws         Pointer to workspace.
 *  \param[in,out]  buffer     Pointer to buffer where the encoded point shall be stored.
 *  \param[in]      point      Pointer to the point on the curve.
 *  \param[in]      watchdog   Pointer to watchdog trigger function.
 *  \return      actOK                - Encoding successful
 *               actEXCEPTION_UNKNOWN - Invert 0
 *  \pre         Pointers must be valid.
 *               The buffer referenced by buffer must provide at least BNBYTES_456 many bytes.
 *  \context     TASK
 *  \reentrant   TRUE, for different workspaces
 *  \synchronous TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(actRETURNCODE) act448_encodePoint(
  VSECPRIM_P2VAR_PARA(actEdwardsCurveWs) ws,
  VSECPRIM_P2VAR_PARA(actU8) buffer,
  VSECPRIM_P2CONST_PARA(actHomogeneousPointEd448) point,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

/**********************************************************************************************************************
 *  act448_decodePoint()
 *********************************************************************************************************************/
/*! \brief       Decode point.
 *  \details     This function decodes a given point according to
 *               https:/ /tools.ietf.org/html/draft-irtf-cfrg-eddsa-08#section-5.2.3
 *  \param[in,out]  ws         Pointer to workspace.
 *  \param[in,out]  point      Pointer where the curve point shall be stored.
 *  \param[in]      buffer     Pointer to the encrypted point.
 *  \param[in]      watchdog   Pointer to watchdog trigger function.
 *  \return      actOK                - Decoding successful
 *               actEXCEPTION_UNKNOWN - inversion fails.
 *  \pre         Pointers must be valid.
 *               The buffer referenced by buffer must provide exactly BNBYTES_456 many bytes.
 *  \context     TASK
 *  \reentrant   TRUE, for different workspaces
 *  \synchronous TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(actRETURNCODE) act448_decodePoint(
  VSECPRIM_P2VAR_PARA(actEdwardsCurveWs) ws,
  VSECPRIM_P2VAR_PARA(actHomogeneousPointEd448) point,
  VSECPRIM_P2CONST_PARA(actU8) buffer,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

/**********************************************************************************************************************
 *  act448_addPoint()
 *********************************************************************************************************************/
/*! \brief       actHomogeneousPointEd448 addition.
 *  \details     This function performs point addition. According to:
 *               https:/ /tools.ietf.org/html/draft-irtf-cfrg-eddsa-08#section-5.2.4
 *  \param[in,out]  ws        Pointer to workspace.
 *  \param[in,out]  r         Pointer to the first point.
 *  \param[in]      p         Pointer to point which shall be added.
 *  \param[in]      watchdog  Pointer to watchdog trigger function.
 *  \pre         Pointers must be valid.
 *  \context     TASK
 *  \reentrant   TRUE, for different workspaces
 *  \synchronous TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(void) act448_addPoint(
  VSECPRIM_P2VAR_PARA(actEdwardsCurveWs) ws,
  VSECPRIM_P2VAR_PARA(actHomogeneousPointEd448) r,
  VSECPRIM_P2CONST_PARA(actHomogeneousPointEd448) p,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

/**********************************************************************************************************************
 *  act448_doublePoint()
 *********************************************************************************************************************/
 /*! \brief       actHomogeneousPointEd448 doubling.
  *  \details     This function performs addition of curve point with itself. According to:
  *               https:/ /tools.ietf.org/html/draft-irtf-cfrg-eddsa-08#section-5.2.4
  *  \param[in,out]   ws        Pointer to workspace.
  *  \param[in,out]   r         Pointer to the first point.
  *  \param[in]       watchdog  Pointer to watchdog trigger function.
  *  \pre         Pointers must be valid.
  *  \context     TASK
  *  \reentrant   TRUE, for different workspaces
  *  \synchronous TRUE
  *********************************************************************************************************************/
extern VSECPRIM_FUNC(void) act448_doublePoint(
  VSECPRIM_P2VAR_PARA(actEdwardsCurveWs) ws,
  VSECPRIM_P2VAR_PARA(actHomogeneousPointEd448) r, /* PRQA S 3673 */ /* MD_VSECPRIM_16.7 */
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

/**********************************************************************************************************************
 *  act448_mulPoint()
 *********************************************************************************************************************/
/*! \brief       actHomogeneousPointEd448 multiplication.
 *  \details     This function performs point multiplication. According to:
 *               https:/ /tools.ietf.org/html/draft-irtf-cfrg-eddsa-08#section-5.2.4
 *  \param[in,out]  ws        Pointer to workspace.
 *  \param[in,out]  r         Pointer to the result.
 *  \param[in]      p         Pointer to point which shall be multiplied.
 *  \param[in]      s         Pointer to scalar which shall be used for multiplication.
 *  \param[in]      watchdog  Pointer to watchdog trigger function.
 *  \pre         Pointers must be valid.
 *               The buffer referenced by s must provide exactly BNDIGITS_456 many actBNDIGITs.
 *  \context     TASK
 *  \reentrant   TRUE, for different workspaces
 *  \synchronous TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(void) act448_mulPoint(
  VSECPRIM_P2VAR_PARA(actEdwardsCurveWs) ws,
  VSECPRIM_P2VAR_PARA(actHomogeneousPointEd448) r,
  VSECPRIM_P2VAR_PARA(actHomogeneousPointEd448) p,
  VSECPRIM_P2CONST_PARA(actBNDIGIT) s,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

/**********************************************************************************************************************
 *  act448_mulAddPoint()
 *********************************************************************************************************************/
/*! \brief       actHomogeneousPointEd448 multiplication by point addition.
 *  \details     This function performs point multiplication by point addition. According to:
 *               https:/ /tools.ietf.org/html/draft-irtf-cfrg-eddsa-08#section-5.2.4
 *  \param[in,out]  ws        Pointer to workspace.
 *  \param[in,out]  r         Pointer to the result.
 *  \param[in]      p         Pointer to point which shall be multiplied.
 *  \param[in]      s         Pointer to scalar which shall be used for multiplication.
 *  \param[in]      watchdog  Pointer to watchdog trigger function.
 *  \pre         Pointers must be valid.
 *               The buffer referenced by s must provide exactly BNDIGITS_456 many actBNDIGITs.
 *  \context     TASK
 *  \reentrant   TRUE, for different workspaces
 *  \synchronous TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(void) act448_mulAddPoint(
  VSECPRIM_P2VAR_PARA(actEdwardsCurveWs) ws,
  VSECPRIM_P2VAR_PARA(actHomogeneousPointEd448) r,
  VSECPRIM_P2VAR_PARA(actHomogeneousPointEd448) p,
  VSECPRIM_P2CONST_PARA(actBNDIGIT) s,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

/**********************************************************************************************************************
 *  act448_mulBasePoint()
 *********************************************************************************************************************/
/*! \brief       Multiply base point.
 *  \details     This function performs multiplication of base point with given scalar.
 *  \param[in,out]  ws        Pointer to workspace.
 *  \param[in,out]  r         Pointer to the result.
 *  \param[in]      s         Pointer to scalar which shall be used for multiplication.
 *  \param[in]      watchdog  Pointer to watchdog trigger function.
 *  \pre         Pointers must be valid.
 *               The buffer referenced by s must provide exactly BNDIGITS_456 many actBNDIGITs.
 *  \context     TASK
 *  \reentrant   TRUE, for different workspaces
 *  \synchronous TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(void) act448_mulBasePoint(
  VSECPRIM_P2VAR_PARA(actEdwardsCurveWs) ws,
  VSECPRIM_P2VAR_PARA(actHomogeneousPointEd448) r,
  VSECPRIM_P2CONST_PARA(actBNDIGIT) s,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

/**********************************************************************************************************************
 *  act448_comparePoints()
 *********************************************************************************************************************/
/*! \brief       Compare given points.
 *  \details     This function compares the given points.
 *  \param[in,out]  ws        Pointer to workspace.
 *  \param[in,out]  p         Pointer to the result.
 *  \param[in]      q         Pointer to scalar which shall be used for multiplication.
 *  \param[in]      watchdog  Pointer to watchdog trigger function.
 *  \return      actOK                 Points are identical.
 *               actEXCEPTION_UNKNOWN  Points are not identical.
 *  \pre         Pointers must be valid.
 *  \context     TASK
 *  \reentrant   TRUE, for different workspaces
 *  \synchronous TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(actRETURNCODE) act448_comparePoints(
  VSECPRIM_P2VAR_PARA(actEdwardsCurveWs) ws,
  VSECPRIM_P2CONST_PARA(actHomogeneousPointEd448) p,
  VSECPRIM_P2CONST_PARA(actHomogeneousPointEd448) q,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

/**********************************************************************************************************************
 *  act448_BNStoreLE()
 *********************************************************************************************************************/
/*! \brief       Copy BigNum into a byte array.
 *  \details     This function copies BigNum arrays of length BNDIGITS_456 into a byte array of length BNBYTES_456.
 *               Copy no more than length of byte array - Needed for Encoding
 *                - Case actBN_BYTES_PER_DIGIT == 4:
 *                  - BNDIGITS_456 = 15
 *                  - BNBYTES_456  = 57
 *               Stop after 14 loops (56 bytes stored)
 *               Copy the last byte independently
 *
 *                - Case actBN_BYTES_PER_DIGIT == 2:
 *                  - BNDIGITS_456 = 29
 *                  - BNBYTES_456  = 57
 *               Stop after 28 loops (56 bytes stored)
 *               Copy the last byte independently
 *
 *                - Case actBN_BYTES_PER_DIGIT == 1:
 *                  - BNDIGITS_456 = 57
 *                  - BNBYTES_456  = 57
 *               57 loops
 *  \param[in,out]  bytes    pointer to result.
 *  \param[in]      bigNum   pointer to BigNum for copy.
 *  \pre         Pointers must be valid.
                 Parameter bytes must have length BNBYTES_456.
                 Parameter bigNum must have length BNDIGITS_456.
 *  \context     TASK
 *  \reentrant   TRUE, for different workspaces
 *  \synchronous TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(void) act448_BNStoreLE(
  VSECPRIM_P2VAR_PARA(actU8) bytes,
  VSECPRIM_P2CONST_PARA(actBNDIGIT) bigNum);

/**********************************************************************************************************************
 *  act448_BNLoadLE()
 *********************************************************************************************************************/
/*! \brief       Copy a byte array into BigNum.
 *  \details     This function copies byte arrays of length BNBYTES_456 bytes
 *               into BigNum arrays of length BNDIGITS_456 digits.
 *                Length of BigNum is NOT a divisor of length of byte array.
 *                We first handle the last bytes and load them in the last digit before we start with a loop.
 *                #10 Load the last bytes in the last digit
 *                  Remark: One digit is in big endian representation, we revert the order of the bytes.
 *                #20 Case actBN_BYTES_PER_DIGIT == 4: Create 1 digit with 4 bytes
 *                #30 Case actBN_BYTES_PER_DIGIT == 2: Create 1 digit with 2 bytes
 *                #40 Case actBN_BYTES_PER_DIGIT == 1: Create 1 digit with 1 bytes
 *                #50 Load computed digit in BigNum
 *  \param[in,out]  bigNum        pointer to result.
 *  \param[in]      bytes         pointer to byte array for copy.
 *  \param[in]      byteLength    length of parameter bytes
 *  \pre         Pointers must be valid.
 *               Buffer "bigNum" must have at least the memory space of buffer "bytes".
 *  \context     TASK
 *  \reentrant   TRUE, for different workspaces
 *  \synchronous TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(void) act448_BNLoadLE(
  VSECPRIM_P2VAR_PARA(actBNDIGIT) bigNum,
  VSECPRIM_P2CONST_PARA(actU8) bytes,
  const actU32 byteLength);

# ifdef __cplusplus                                       /* COV_VSECPRIM_CPLUSPLUS XF */
}
# endif

# define VSECPRIM_STOP_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* ACT448UTIL_H */

/**********************************************************************************************************************
 *  END OF FILE: act448util.h
 *********************************************************************************************************************/
