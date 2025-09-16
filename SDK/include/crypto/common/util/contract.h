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
/*!        \file  contract.h
 *        \brief  Function contract handling utilities.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBCOMMON_INCLUDE_CRYPTO_COMMON_UTIL_CONTRACT_H_
#define LIB_LIBCOMMON_INCLUDE_CRYPTO_COMMON_UTIL_CONTRACT_H_

/*********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <algorithm>

#include "amsr/core/result.h"
#include "amsr/core/string.h"
#include "amsr/core/string_view.h"
#include "amsr/iostream/stateful_output_stream.h"
#include "amsr/iostream/stringstream/output_string_stream.h"
#include "ara/crypto/common/mem_region.h"
#include "crypto/common/util/logger.h"
#include "vac/container/string_literals.h"

namespace crypto {
namespace common {
namespace util {

// VECTOR NC AutosarC++17_10-M7.3.6: MD_CRYPTO_AutosarC++17_10-M7.3.6_sv
/*! Introduce string literal for StringView creation */
using vac::container::literals::operator"" _sv;

/*!
 * \brief Utility functions for contract checking.
 * \vprivate Product Private
 */
class Contract final {
 public:
  /*! \brief A null terminated character sequence */
  using CString = Logger::CString;
  /*! \brief The type for line numbers */
  using LineNumber = Logger::LineNumber;

  // VECTOR NC VectorC++-V3.9.2: MD_CRYPTO_VectorC++-V3.9.2_built_in_types_as_function_parameters
  /*!
   * \brief Checks a precondition and terminates the program if it is not met.
   * \param[in] file_name File where the precondition is specified.
   * \param[in] line_num Line number where the precondition is specified.
   * \param[in] condition The precondition.
   * \param[in] msg Message to display when terminating.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  static void Precondition(CString file_name, LineNumber line_num, bool condition,
                           amsr::core::StringView const msg) noexcept {
    Condition("Precondition"_sv, file_name, line_num, condition, msg);
  }

  // VECTOR NC VectorC++-V3.9.2: MD_CRYPTO_VectorC++-V3.9.2_built_in_types_as_function_parameters
  /*!
   * \brief Checks a precondition specifying the range of a numerical value and terminates the program if it is not met.
   * \tparam NumType Numeric type of the value.
   * \param[in] file_name File where the precondition is specified.
   * \param[in] line_num Line number where the precondition is specified.
   * \param[in] val The value.
   * \param[in] min Minimum allowed value.
   * \param[in] max Maximum allowed value.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  template <typename NumType, typename = typename std::enable_if<std::is_arithmetic<NumType>::value>::type>
  static void PreconditionInRange(CString file_name, LineNumber line_num, NumType val, NumType min,
                                  NumType max) noexcept {
    ConditionInRange<NumType>("Precondition"_sv, file_name, line_num, val, min, max);
  }

  // VECTOR NC VectorC++-V3.9.2: MD_CRYPTO_VectorC++-V3.9.2_built_in_types_as_function_parameters
  /*!
   * \brief Checks a postcondition and terminates the program if it is not met.
   * \param[in] file_name File where the precondition is specified.
   * \param[in] line_num Line number where the postcondition is specified.
   * \param[in] condition The postcondition.
   * \param[in] msg Message to display when terminating.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  static void Postcondition(CString file_name, LineNumber line_num, bool condition,
                            amsr::core::StringView const msg) noexcept {
    Condition("Precondition"_sv, file_name, line_num, condition, msg);
  }

  // VECTOR NC VectorC++-V3.9.2: MD_CRYPTO_VectorC++-V3.9.2_built_in_types_as_function_parameters
  /*!
   * \brief Checks a postcondition specifying the range of a numerical value and terminates the program if it is not
   * met.
   * \tparam NumType Numeric type of the value.
   * \param[in] file_name File where the precondition is specified.
   * \param[in] line_num Line number where the postcondition is specified.
   * \param[in] val The value.
   * \param[in] min Minimum allowed value.
   * \param[in] max Maximum allowed value.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  template <typename NumType, typename = typename std::enable_if<std::is_arithmetic<NumType>::value>::type>
  static void PostconditionInRange(CString file_name, LineNumber line_num, NumType val, NumType min,
                                   NumType max) noexcept {
    ConditionInRange<NumType>("Precondition"_sv, file_name, line_num, val, min, max);
  }

  // VECTOR NC VectorC++-V3.9.2: MD_CRYPTO_VectorC++-V3.9.2_built_in_types_as_function_parameters
 private:
  /*!
   * \brief Checks a condition and terminates the program if it is not met.
   * \param[in] cond_name Name of the condition type.
   * \param[in] file_name File where the precondition is specified.
   * \param[in] line_num Line number where the condition is specified.
   * \param[in] condition The condition.
   * \param[in] msg Message to display when terminating.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  static void Condition(amsr::core::StringView const cond_name, CString file_name, LineNumber line_num, bool condition,
                        amsr::core::StringView const msg) noexcept {
    if (!condition) {
      crypto::common::util::Logger::LogFatal(kCommonLoggingContext, file_name, line_num)
          << cond_name << " not satisfied. " << msg << " Terminating application.";

      amsr::core::Abort(msg.data());
    }
  }

  // VECTOR NC VectorC++-V3.9.2: MD_CRYPTO_VectorC++-V3.9.2_built_in_types_as_function_parameters
  /*!
   * \brief Checks a condition specifying the range of a numerical value and terminates the program if it is not met.
   * \tparam NumType Numeric type of the value.
   * \param[in] cond_name Name of the condition type.
   * \param[in] file_name File where the precondition is specified.
   * \param[in] line_num Line number where the condition is specified.
   * \param[in] val The value.
   * \param[in] min Minimum allowed value.
   * \param[in] max Maximum allowed value.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  template <typename NumType, typename = typename std::enable_if<std::is_arithmetic<NumType>::value>::type>
  static void ConditionInRange(amsr::core::StringView const cond_name, CString file_name, LineNumber line_num,
                               NumType val, NumType min, NumType max) noexcept {
    if ((val < min) || (val > max)) {
      amsr::core::StringView const msg{"Value should be between "};
      std::size_t const buffer_size{200};
      amsr::core::Array<char, buffer_size> buffer{};
      amsr::core::Span<char> const span{buffer.begin(), buffer.end()};
      amsr::stream::stringstream::OutputStringStream oss{span};
      amsr::stream::StatefulOutputStream soss{oss};
      soss.Write(msg);
      soss.Write(min);
      soss.Write(" and "_sv);
      soss.Write(max);
      soss.Write(" but is "_sv);
      soss.Write(val);
      soss.Write('.');
      if (soss.GetState().HasValue()) {
        amsr::core::String const message{oss.AsStringView().ToString()};
        Condition(cond_name, file_name, line_num, false, message);
      }
    }
  }
};

}  // namespace util
}  // namespace common
}  // namespace crypto

#endif  // LIB_LIBCOMMON_INCLUDE_CRYPTO_COMMON_UTIL_CONTRACT_H_
