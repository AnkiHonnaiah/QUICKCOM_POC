/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2023 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/**     \file       reactor_fwd.h
 *      \brief
 *      \details
 *
 *********************************************************************************************************************/
#ifndef LIB_COMMON_INCLUDE_AMSR_LOG_INTERNAL_REACTOR_FWD_H_
#define LIB_COMMON_INCLUDE_AMSR_LOG_INTERNAL_REACTOR_FWD_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
namespace amsr {
namespace log {
namespace internal {
/*! f\brief Forward declaration of a reactor that may run on a separate thread */
class Reactor;
/*! \brief Forward declaration of a reactor that runs on a separate thread */
class OwnedReactor;
/*! \brief Forward declaration of a borrowed reactor */
class BorrowedReactor;
}  // namespace internal
}  // namespace log
}  // namespace amsr

#endif  // LIB_COMMON_INCLUDE_AMSR_LOG_INTERNAL_REACTOR_FWD_H_
