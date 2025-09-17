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
/**     \file
 *      \brief      Contains the definition of the log stream that provides the << operator.
 *      \details
 *
 *********************************************************************************************************************/
#ifndef LIB_ARA_LOGGING_INCLUDE_ARA_LOG_LOGSTREAM_H_
// VECTOR NC AutosarC++17_10-M2.10.1_ambigous_identifiers
#define LIB_ARA_LOGGING_INCLUDE_ARA_LOG_LOGSTREAM_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/log/log_types.h"
#include "amsr/log/logstream.h"  // IWYU pragma: export

namespace ara {
namespace log {
/*!
 * \brief Wrapper type for hexadecimal representation
 *
 * \details As defined in SWS_LOG_00108
 *
 * \vpublic
 */
using LogHex8 = amsr::log::internal::LogHex8;

/*!
 * \brief Wrapper type for hexadecimal representation
 *
 * \details As defined in SWS_LOG_00109
 *
 * \vpublic
 */
using LogHex16 = amsr::log::internal::LogHex16;

/*!
 * \brief Wrapper type for hexadecimal representation
 *
 * \details As defined in SWS_LOG_00110
 *
 * \vpublic
 */
using LogHex32 = amsr::log::internal::LogHex32;

/*!
 * \brief Wrapper type for hexadecimal representation
 *
 * \details As defined in SWS_LOG_00111
 *
 * \vpublic
 */
using LogHex64 = amsr::log::internal::LogHex64;

/*!
 * \brief Wrapper type for binary representation
 *
 * \details As defined in SWS_LOG_00112
 *
 * \vpublic
 */
using LogBin8 = amsr::log::internal::LogBin8;

/*!
 * \brief Wrapper type for binary representation
 *
 * \details As defined in SWS_LOG_00113
 *
 * \vpublic
 */
using LogBin16 = amsr::log::internal::LogBin16;

/*!
 * \brief Wrapper type for binary representation
 *
 * \details As defined in SWS_LOG_00114
 *
 * \vpublic
 */
using LogBin32 = amsr::log::internal::LogBin32;

/*!
 * \brief Wrapper type for binary representation
 *
 * \details As defined in SWS_LOG_00115
 *
 * \vpublic
 */
using LogBin64 = amsr::log::internal::LogBin64;

/*!
 * \copydoc amsr::log::LogRawBuffer
 */
// VECTOR NC AutosarC++17_10-M3.4.1: MD_LOG_AutosarC++17_10-M3.4.1_inline_class_definition
using LogRawBuffer = amsr::log::LogRawBuffer;

/*!
 * \copydoc amsr::log::LogStream
 */
// VECTOR NC AutosarC++17_10-M3.4.1: MD_LOG_AutosarC++17_10-M3.4.1_inline_class_definition
using LogStream = amsr::log::LogStream;

}  // namespace log
}  // namespace ara

#endif  // LIB_ARA_LOGGING_INCLUDE_ARA_LOG_LOGSTREAM_H_
