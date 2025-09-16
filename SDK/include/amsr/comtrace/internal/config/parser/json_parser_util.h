/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2024 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file
 *        \brief  Utilities for JSON parsing.
 *      \details
 *********************************************************************************************************************/

#ifndef LIB_COM_TRACE_INCLUDE_AMSR_COMTRACE_INTERNAL_CONFIG_PARSER_JSON_PARSER_UTIL_H_
#define LIB_COM_TRACE_INCLUDE_AMSR_COMTRACE_INTERNAL_CONFIG_PARSER_JSON_PARSER_UTIL_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/core/error_code.h"
#include "amsr/json/reader.h"  // IWYU pragma: keep
#include "amsr/json/reader/json_data.h"
#include "amsr/json/reader/parser_state.h"
#include "vac/container/string_literals.h"

namespace amsr {
namespace comtrace {
namespace internal {
namespace config {
namespace parser {

// VECTOR NC AutosarC++17_10-M7.3.6: MD_ComTrace_M7.3.6_StringView_literal
/*!
 * \brief Use string literal for creation of StringViews.
 */
using vac::container::literals::operator""_sv;

/*!
 * \brief Type-alias for JsonData.
 */
using JsonData = amsr::json::JsonData;

/*!
 * \brief Type-alias for easy access to ParserResult.
 */
using ParserResult = amsr::json::ParserResult;

/*!
 * \brief Map error code to binding specific JSON parsing failure error code.
 *
 * \param[in] error_code  ErrorCode to be mapped.
 *
 * \return Mapped ErrorCode.
 *
 * \steady FALSE
 *
 * \spec requires true; \endspec
 *
 * \unit ComTrace::TraceConfig
 */
auto MapToComponentError(amsr::core::ErrorCode const& error_code) noexcept -> amsr::core::ErrorCode;

}  // namespace parser
}  // namespace config
}  // namespace internal
}  // namespace comtrace
}  // namespace amsr

#endif  // LIB_COM_TRACE_INCLUDE_AMSR_COMTRACE_INTERNAL_CONFIG_PARSER_JSON_PARSER_UTIL_H_
