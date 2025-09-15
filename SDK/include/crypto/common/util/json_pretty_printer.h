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
/*!        \file  json_pretty_printer.h
 *        \brief  Makes JSON pretty.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBCOMMON_INCLUDE_CRYPTO_COMMON_UTIL_JSON_PRETTY_PRINTER_H_
#define LIB_LIBCOMMON_INCLUDE_CRYPTO_COMMON_UTIL_JSON_PRETTY_PRINTER_H_

/*********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/core/string.h"
#include "amsr/core/string_view.h"
#include "crypto/common/util/logger.h"

namespace crypto {
namespace common {
namespace util {

/*!
 * \brief Prettifies JSON.
 * \vprivate Product Private
 */
class JsonPrettyPrinter {
 public:
  /*!
   * \brief State of the JSON parser.
   * \vprivate Product Private
   */
  enum class State : std::uint8_t {
    Uninteresting,
    OpenObject,
    StartString,
    String,
    Escape,
    EndString,
    KeyValueSeparator,
    OpenArray,
    ArraySeparator,
    CloseArray,
    CloseObject
  };

 private:
  State state_;                         /*!< State. */
  crypto::common::util::Logger logger_; /*!< Logging instance */

  /*!
   * \brief Returns true if the given character is the beginning of a JSON value.
   * \param[in] a Character.
   * \return True if the given character is the beginning of a JSON value.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  inline bool ExpectValue(char a) noexcept;

 public:
  /*!
   * \brief Initializes the object.
   * \vprivate Product Private
   */
  JsonPrettyPrinter() noexcept;

  /*!
   * \brief Updates the inner state according to a given character.
   * \param[in] a Character.
   * \return New state.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  State Update(char a) noexcept;

  /*!
   * \brief generates an indentation for a JSON line.
   * \param[in] indent Size of the indentation.
   * \return Indentation.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  static inline amsr::core::String Indent(std::size_t indent) noexcept;

  /*!
   * \brief Prettifies a JSON string at once.
   * \param[in] json String.
   * \return The prettified JSON.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \exceptionsafety STRONG
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  amsr::core::String Process(amsr::core::StringView json) noexcept;
};

}  // namespace util
}  // namespace common
}  // namespace crypto

#endif  // LIB_LIBCOMMON_INCLUDE_CRYPTO_COMMON_UTIL_JSON_PRETTY_PRINTER_H_
