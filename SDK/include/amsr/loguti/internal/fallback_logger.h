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
/*!     \file
 *      \brief      Header file for logging without ara::log
 *      \details    -
 *
 *********************************************************************************************************************/
#ifndef LIB_LOG_UTILITY_INCLUDE_AMSR_LOGUTI_INTERNAL_FALLBACK_LOGGER_H_
#define LIB_LOG_UTILITY_INCLUDE_AMSR_LOGUTI_INTERNAL_FALLBACK_LOGGER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/core/error_code.h"
#include "amsr/core/optional.h"
#include "amsr/core/string_view.h"
#include "amsr/generic/write_std_stream.h"
#include "amsr/loguti/internal/helper.h"
#include "vac/container/c_string_view.h"
#include "vac/container/string_literals.h"

namespace amsr {
namespace loguti {
namespace internal {
/*!
 * \brief The fallback logger used
 *
 * \trace DSGN-LogUtility-SafeLogger
 * \vprivate
 */
class FallbackLogger final {
  /*! \brief The prefix that is to be logged before the first message. */
  vac::container::CStringView prefix_;
  /*! \brief Marks if flushing is necessary */
  bool dirty_{false};

 public:
  /*!
   * \brief Initialize the fallback logger with an optional prefix.
   * \param prefix The prefix to use before the log message.
   * \spec
   *   requires true;
   * \endspec
   */
  explicit FallbackLogger(vac::container::CStringView prefix = kEmptyString) noexcept;

  /*!
   * \brief Move constructor that will disable logging of the moved logger.
   * \param other The other logger that is moved into this one.
   * \spec
   *   requires true;
   * \endspec
   */
  FallbackLogger(FallbackLogger&& other) noexcept;

  /*! \brief Delete Move assignment as the ara::log::LogStream does not support it either. */
  auto operator=(FallbackLogger&& other) & -> FallbackLogger& = delete;

  /*! \brief Delete the copy constructor. */
  FallbackLogger(FallbackLogger const&) = delete;

  /*! \brief Delete the copy assignment. */
  auto operator=(FallbackLogger const&) & -> FallbackLogger& = delete;

  /*!
   *  \brief Flush on destruction.
   * \spec
   *   requires true;
   * \endspec
   *  */
  ~FallbackLogger() noexcept;

  /*!
   * \brief Appends string views to the terminal.
   * \param sv value to be logged
   * \return A reference to the log stream.
   * \spec
   *   requires true;
   * \endspec
   */
  auto operator<<(amsr::core::StringView sv) & noexcept -> FallbackLogger&;

  /*!
   * \brief Appends c string views to the terminal.
   * \param csv value to be logged
   * \return A reference to the log stream.
   * \spec
   *   requires true;
   * \endspec
   */
  auto operator<<(vac::container::CStringView csv) & noexcept -> FallbackLogger&;

  /*!
   * \brief Appends boolean parameter to the terminal.
   * \param value value to be logged
   * \return A reference to the log stream.
   * \spec
   *   requires true;
   * \endspec
   */
  auto operator<<(bool value) & noexcept -> FallbackLogger&;

  /*!
   * \brief Appends unsigned int 8 bit parameter to the terminal.
   * \param value value to be logged
   * \return A reference to the log stream.
   * \spec
   *   requires true;
   * \endspec
   */
  auto operator<<(char value) & noexcept -> FallbackLogger&;

  /*!
   * \brief Appends an error code to the terminal.
   * \param value The value to be logged.
   * \return The passed log stream.
   * \spec
   *   requires true;
   * \endspec
   */
  auto operator<<(amsr::core::ErrorCode value) noexcept -> FallbackLogger&;

  /*!
   * \brief Finishes & flushes the message.
   * \spec
   *   requires true;
   * \endspec
   */
  void Flush() noexcept;

 private:
  /*!
   *  \brief Write a prefix if it is configured to.
   * \spec
   *   requires true;
   * \endspec
   */
  void WritePrefix() noexcept;
};
}  // namespace internal
}  // namespace loguti
}  // namespace amsr

#endif  // LIB_LOG_UTILITY_INCLUDE_AMSR_LOGUTI_INTERNAL_FALLBACK_LOGGER_H_
