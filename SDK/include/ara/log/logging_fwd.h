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
/**     \file       logging_fwd.h
 *      \brief      Contains forward declarations for ara::log types
 *      \details
 *
 *********************************************************************************************************************/
#ifndef LIB_ARA_LOGGING_INCLUDE_ARA_LOG_LOGGING_FWD_H_
#define LIB_ARA_LOGGING_INCLUDE_ARA_LOG_LOGGING_FWD_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/log_fwd.h"
#include "ara/log/common.h"

namespace ara {
namespace log {
/*!
 * \copydoc amsr::log::Logger
 */
using Logger = amsr::log::Logger;

/*!
 * \copydoc amsr::log::LogStream
 */
using LogStream = amsr::log::LogStream;
}  // namespace log
}  // namespace ara

#endif  // LIB_ARA_LOGGING_INCLUDE_ARA_LOG_LOGGING_FWD_H_
