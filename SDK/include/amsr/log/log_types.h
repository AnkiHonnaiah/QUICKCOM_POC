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
/*!        \file  log_types.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef LIB_AMSR_LOG_INCLUDE_AMSR_LOG_LOG_TYPES_H_
#define LIB_AMSR_LOG_INCLUDE_AMSR_LOG_LOG_TYPES_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/log/log_level.h"
#include "vac/container/string_literals.h"
#include "vac/language/compile_time.h"

namespace amsr {
namespace log {

namespace internal {
/*!
 * \brief Enum of base system
 *
 * \details The value represent the number of bits that can fit into one character
 *
 * \vprivate Component private.
 */
enum class NumericBase : std::uint8_t { kBinBase = 1u, kHexBase = 4u };

/*!
 * \brief Checks if the log level left hand side can be activated
 *
 * \tparam lhs The log level to be checked
 * \tparam rhs The log level that contains the compile time limit
 *
 * \details Is not implemented as constexpr function to force compile time calculation
 *
 * \vprivate Component private.
 */
template <LogLevel lhs, LogLevel rhs>
struct CanLoggerBeEnabled {
  /*! true if the left is a subset of the right */
  constexpr static bool value{lhs <= rhs};  // NOLINT(readability-identifier-naming)
};

/*!
 * \brief Executes the function only if the log level left hand side can be activated
 *
 * \tparam lhs The log level to be checked
 * \tparam rhs The log level that contains the compile time limit
 * \tparam Fn  The type of the function to execute.
 *             Must be a function that can take zero arguments.
 * \param fn   The function to execute in case it can be activated.
 *
 * \vprivate Component private.
 * \spec
 *   requires true;
 * \endspec
 */
template <LogLevel lhs, LogLevel rhs, typename Fn>
constexpr void WithFilteredSeverity(Fn&& fn) noexcept {
  // VECTOR NC AutosarC++17_10-M0.1.2: MD_LOG_AutosarC++17_10-M0.1.2_false_positive_dead_code
  if (CanLoggerBeEnabled<lhs, rhs>::value) {
    // VCA_LOGAPI_CHECKED_CONTRACT
    fn();
  }
}
}  // namespace internal

/*!
 * \brief   Wrapper type for hexadecimal or binary representation
 *
 * \tparam  T       The underlying integer type.
 *                  Must be of an unsigned integer type.
 * \tparam  kBase   The base of the integer.
 *                  Enumeration of NumericBase.
 *
 * \vprivate
 */
template <typename T, internal::NumericBase kBase>
struct IntLogWrapper {
  /*!
   * \brief value
   */
  T value;
};

namespace internal {
/*!
 * \brief Wrapper type for hexadecimal representation
 * \vprivate
 */
using LogHex8 = IntLogWrapper<std::uint8_t, NumericBase::kHexBase>;

/*!
 * \brief Wrapper type for hexadecimal representation
 * \vprivate
 */
using LogHex16 = IntLogWrapper<std::uint16_t, NumericBase::kHexBase>;

/*!
 * \brief Wrapper type for hexadecimal representation
 * \vprivate
 */
using LogHex32 = IntLogWrapper<std::uint32_t, NumericBase::kHexBase>;

/*!
 * \brief Wrapper type for hexadecimal representation
 * \vprivate
 */
using LogHex64 = IntLogWrapper<std::uint64_t, NumericBase::kHexBase>;

/*!
 * \brief Wrapper type for binary representation
 *
 * \vprivate
 */
using LogBin8 = IntLogWrapper<std::uint8_t, NumericBase::kBinBase>;

/*!
 * \brief Wrapper type for binary representation
 * \vprivate
 */
using LogBin16 = IntLogWrapper<std::uint16_t, NumericBase::kBinBase>;

/*!
 * \brief Wrapper type for binary representation
 * \vprivate
 */
using LogBin32 = IntLogWrapper<std::uint32_t, NumericBase::kBinBase>;

/*!
 * \brief Wrapper type for binary representation
 * \vprivate
 */
using LogBin64 = IntLogWrapper<std::uint64_t, NumericBase::kBinBase>;

/*!
 * \brief Checks if the passed type is in fact a IntLogWrapper.
 *
 * \tparam T The type to check.
 */
template <typename T>
struct IsIntLogWrapper
    : std::integral_constant<bool, vac::language::compile_time::any_in<T, LogHex8, LogHex16, LogHex32, LogHex64,
                                                                       LogBin8, LogBin16, LogBin32, LogBin64>()> {};
}  // namespace internal

/*!
 * \brief Wrapper type for raw buffer representation
 *
 * \details As defined in SWS_LOG_00116
 *
 * \vpublic
 */
struct LogRawBuffer {
  /*! \brief buffer pointer. */
  char const* buffer;
  /*! \brief buffer size. */
  std::size_t size;
};

/*!
 * \brief Type of unique application and context IDs
 *
 * \vprivate Component private.
 */
using IdType = std::uint32_t;

}  // namespace log
}  // namespace amsr

#endif  // LIB_AMSR_LOG_INCLUDE_AMSR_LOG_LOG_TYPES_H_
