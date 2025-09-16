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
/*!     \file
 *      \brief      Header file for logging without ara::log
 *      \details    -
 *
 *********************************************************************************************************************/
#ifndef LIB_LOG_UTILITY_INCLUDE_AMSR_LOGUTI_INTERNAL_ASR_LOGGER_H_
#define LIB_LOG_UTILITY_INCLUDE_AMSR_LOGUTI_INTERNAL_ASR_LOGGER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/loguti/internal/helper.h"
#include "ara/log/logger.h"

namespace amsr {
namespace loguti {
namespace internal {
/*!
 * \brief Gets the one logger that will be allocated for logutility
 * \param ctx_id The Context ID to use.
 * \return The reference to the logger.
 * \spec
 *   requires true;
 * \endspec
 */
auto GetAsrLoggerUnsafe(vac::container::CStringView ctx_id) noexcept -> ara::log::Logger&;

/*!
 * \brief Gets the one logger that will be allocated for logutility or nothing
 * \param ctx_id The Context ID to use.
 * \return The pointer to the acquired logger or a nullptr.
 * \spec
 *   requires true;
 * \endspec
 */
auto GetAsrLogger(vac::container::CStringView ctx_id = kVctrCtxId) noexcept -> ara::log::Logger*;
}  // namespace internal
}  // namespace loguti
}  // namespace amsr

#endif  // LIB_LOG_UTILITY_INCLUDE_AMSR_LOGUTI_INTERNAL_ASR_LOGGER_H_
