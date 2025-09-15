/**********************************************************************************************************************
 *  COPYRIGHT
 * -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2024 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 * -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 * -----------------------------------------------------------------------------------------------------------------*/
/**       \file     types.h
 *        \brief    Type definitions for vaJson.
 *
 *        \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_VAJSON_INCLUDE_AMSR_JSON_UTIL_TYPES_H_
#define LIB_VAJSON_INCLUDE_AMSR_JSON_UTIL_TYPES_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/core/optional.h"
#include "amsr/core/span.h"
#include "amsr/core/string.h"
#include "amsr/core/string_view.h"
#include "vac/container/c_string_view.h"
#include "vac/container/string_literals.h"

namespace amsr {
namespace json {
/*!
 * \brief Unqualified access to Optional.
 * \tparam T Type of value.
 *
 * \vpublic
 */
template <typename T>
using Optional = amsr::core::Optional<T>;

/*!
 * \brief Unqualified access to CStringView.
 *
 * \vpublic
 */
using CStringView = vac::container::CStringView;

/*!
 * \brief Unqualified access to StringView.
 *
 * \vpublic
 */
using StringView = amsr::core::StringView;

/*!
 * \brief Unqualified access to String.
 *
 * \vpublic
 */
using String = amsr::core::String;

/*!
 * \brief Unqualified access to binary data.
 *
 * \vpublic
 */
using Bytes = amsr::core::Span<char const>;

/*!
 * \brief Short form for creating a StringView.
 *
 * \vpublic
 */
using vac::container::literals::operator""_sv;  // VECTOR SL AutosarC++17_10-M7.3.6: MD_JSON_using_StringView

/*!
 * \brief The encoding of the document.
 *
 * \vpublic
 */
enum class EncodingType : std::uint8_t {
  kNone,
  kUtf8,
};
}  // namespace json
}  // namespace amsr

#endif  // LIB_VAJSON_INCLUDE_AMSR_JSON_UTIL_TYPES_H_
