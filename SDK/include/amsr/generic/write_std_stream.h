/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2020 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file  write_std_stream.h
 *        \brief  Writes directly to stdout or stderr.
 *         \unit  VaCommonLib::LanguageSupport::WriteStdStream
 *
 *********************************************************************************************************************/
#ifndef LIB_VAC_INCLUDE_AMSR_GENERIC_WRITE_STD_STREAM_H_
#define LIB_VAC_INCLUDE_AMSR_GENERIC_WRITE_STD_STREAM_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/core/result.h"
#include "amsr/core/string_view.h"

namespace amsr {
namespace generic {

// VECTOR Disable AutosarC++17_10-M3.2.4: MD_VAC_M3.2.4_functionWithoutDefinition
/*!
 * \brief  Writes the given string to stderr.
 * \param  msg The raw string to write.
 * \return Result with the amount of bytes written or an error code.
 * \error  ara::core::PosixErrc::*, when bytes cannot be written.
 * \trace  CREQ-VaCommonLib-WriteStdStream
 * \pre    msg must be an initialized null-terminated string.
 * \vprivate
 */
ara::core::Result<std::size_t> WriteToStderr(char const* msg) noexcept;

/*!
 * \brief   Writes the given string to stderr.
 * \details Will print the same amount of bytes as msg.Length().
 * \param   msg The StringView to write.
 * \return  Result with the amount of bytes written or an error code.
 * \error   ara::core::PosixErrc::*, when bytes cannot be written.
 * \trace   CREQ-VaCommonLib-WriteStdStream
 * \vprivate
 * \spec
 *   requires true;
 * \endspec
 */
ara::core::Result<std::size_t> WriteToStderr(ara::core::StringView msg) noexcept;

/*!
 * \brief  Writes the given string to stdout.
 * \param  msg The raw string to write.
 * \return Result with the amount of bytes written or an error code.
 * \error  ara::core::PosixErrc::*, when bytes cannot be written.
 * \trace  CREQ-VaCommonLib-WriteStdStream
 * \pre    msg must be an initialized null-terminated string.
 * \vprivate
 */
ara::core::Result<std::size_t> WriteToStdout(char const* msg) noexcept;

/*!
 * \brief   Writes the given string to stdout.
 * \details Will print the same amount of bytes as msg.Length().
 * \param   msg The StringView to write.
 * \return  Result with the amount of bytes written or an error code.
 * \error   ara::core::PosixErrc::*, when bytes cannot be written.
 * \trace   CREQ-VaCommonLib-WriteStdStream
 * \vprivate
 * \spec
 *   requires true;
 * \endspec
 */
ara::core::Result<std::size_t> WriteToStdout(ara::core::StringView msg) noexcept;
// VECTOR Enable AutosarC++17_10-M3.2.4

}  // namespace generic
}  // namespace amsr

#endif  // LIB_VAC_INCLUDE_AMSR_GENERIC_WRITE_STD_STREAM_H_
