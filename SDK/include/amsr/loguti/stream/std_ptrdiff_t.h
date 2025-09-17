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
/*!        \file
 *        \brief
 *      \details
 *
 *********************************************************************************************************************/
#ifndef LIB_LOG_UTILITY_INCLUDE_AMSR_LOGUTI_STREAM_STD_PTRDIFF_T_H_
#define LIB_LOG_UTILITY_INCLUDE_AMSR_LOGUTI_STREAM_STD_PTRDIFF_T_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstddef>

#include "amsr/loguti/stream/ns.h"
#include "ara/log/logstream.h"

namespace AMSR_LOGUTI_LOG_NS {
namespace log {
/*!
 * \brief   Appends a std::ptrdiff_t to the log stream.
 *
 * \details This overload is used when std::ptrdiff_t does not have an exact match to any type with an operator<<
 *          overload for ara::log::LogStream as defined by the AUTOSAR standard (e.g., when std::ptrdiff_t is a
 *          signed long long and std::int64_t is a signed long).
 *
 * \tparam  T       Must be of type ptrdiff_t.
 *                  Shall not be of type std::int64_t nor std::int32_t.
 *                  The size of T shall not be larger than the size of an std::int64_t.
 *
 * \param   stream  The log stream to append to.
 * \param   value   The value to be logged.
 *
 * \return  The passed log stream.
 *
 * \context ANY
 * \pre -
 * \spec
 *   requires true;
 * \endspec
 * \threadsafe FALSE
 * \reentrant FALSE
 *
 * \vpublic
 * \unit Stream
 */
template <typename T,
          // VECTOR NL AutosarC++17_10-A3.9.1: MD_LOGU_AutosarC++17_10-A3.9.1_basic_type
          std::enable_if_t<std::is_same<T, ptrdiff_t>::value && (not std::is_same<T, std::int64_t>::value) &&
                           (not std::is_same<T, std::int32_t>::value)>* = nullptr>
auto operator<<(::ara::log::LogStream& stream, T const& value) noexcept -> ::ara::log::LogStream& {
  static_assert(sizeof(T) <= sizeof(std::int64_t), "std::ptrdiff_t is larger than std::int64_t!");
  stream << static_cast<std::int64_t>(value);
  return stream;
}
}  // namespace log
}  // namespace AMSR_LOGUTI_LOG_NS
#endif  // LIB_LOG_UTILITY_INCLUDE_AMSR_LOGUTI_STREAM_STD_PTRDIFF_T_H_
