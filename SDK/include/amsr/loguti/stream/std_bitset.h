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
#ifndef LIB_LOG_UTILITY_INCLUDE_AMSR_LOGUTI_STREAM_STD_BITSET_H_
#define LIB_LOG_UTILITY_INCLUDE_AMSR_LOGUTI_STREAM_STD_BITSET_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <bitset>

#include "amsr/core/string_view.h"
#include "amsr/loguti/stream/ns.h"
#include "ara/log/logstream.h"

namespace AMSR_LOGUTI_LOG_NS {
namespace log {
/*!
 * \brief   Appends a std::bitset to the log stream.
 *
 * \tparam  N       Number of bits.
 *                  Must be of type std::size_t.
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
 *
 * \internal
 * - Log the given bitset as literal ones and zeros where the first character corresponds to the last (N-1th) bit and
 *   the last character corresponds to the first bit, to achieve the same behaviour as std::bitset::to_string.
 * \endinternal
 */
template <std::size_t N>
auto operator<<(ara::log::LogStream& stream, std::bitset<N> const& value) noexcept -> ara::log::LogStream& {
  std::array<char, N> buffer{};
  for (std::size_t i{N}; i > 0; --i) {
    buffer[N - i] = value[i - 1] ? '1' : '0';
  }
  amsr::core::StringView const sv{buffer.data(), buffer.size()};
  stream << sv;
  return stream;
}
}  // namespace log
}  // namespace AMSR_LOGUTI_LOG_NS
#endif  // LIB_LOG_UTILITY_INCLUDE_AMSR_LOGUTI_STREAM_STD_BITSET_H_
