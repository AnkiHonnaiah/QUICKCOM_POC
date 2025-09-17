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
#ifndef LIB_LOG_UTILITY_INCLUDE_AMSR_LOGUTI_SAFE_LOGGER_H_
#define LIB_LOG_UTILITY_INCLUDE_AMSR_LOGUTI_SAFE_LOGGER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/core/optional.h"
#include "amsr/loguti/internal/fallback_logger.h"
#include "amsr/loguti/internal/helper.h"
#include "ara/log/logstream.h"
#include "vac/container/c_string_view.h"
#include "vac/container/string_literals.h"

namespace amsr {
namespace loguti {

/*!
 * \brief The safe logger used
 *
 * \details The safe logger is designed to be used with rvalue references only encouraging a use without storing it.
 *          The logger will check if ara::log can be used on construction.
 *          As amsr::core::Deinitialize is not thread-safe this should be enough to be safe.
 *
 * \vprivate Vector product internal API.
 * \unit amsr::loguti::SafeLogger
 */
class SafeLogger final {
  /*! \brief The optional ara::log LogStream that is used if logging is initialized. */
  amsr::core::Optional<ara::log::LogStream> asr_{};
  /*! \brief The fallback logger that is used if logging is not initialized. */
  internal::FallbackLogger fallback_{};

 public:
  /*!
   * \brief Move is allowed.
   * \spec
   *   requires true;
   * \endspec
   */
  SafeLogger(SafeLogger&&) noexcept = default;
  /*! \brief Move assignment is deleted because of ara::log::LogStream. */
  auto operator=(SafeLogger&&) & -> SafeLogger& = delete;
  /*! \brief Don't allow copying. */
  SafeLogger(SafeLogger const&) = delete;
  /*! \brief Don't allow copying. */
  auto operator=(SafeLogger const&) & -> SafeLogger& = delete;
  /*!
   * \brief Default destructor.
   * \spec
   *   requires true;
   * \endspec
   */
  ~SafeLogger() noexcept = default;

  /*!
   * \brief Create the SafeLogger for fatal messages.
   * \param ctx_id The context ID to use in case AUTOSAR logging is used.
   * \return The constructed safe logger.
   * \spec
   *   requires true;
   * \endspec
   */
  static auto Fatal(vac::container::CStringView ctx_id = internal::kVctrCtxId) noexcept -> SafeLogger;

  /*!
   * \brief Create the SafeLogger for error messages.
   * \param ctx_id The context ID to use in case AUTOSAR logging is used.
   * \return The constructed safe logger.
   * \spec
   *   requires true;
   * \endspec
   */
  static auto Error(vac::container::CStringView ctx_id = internal::kVctrCtxId) noexcept -> SafeLogger;

  /*!
   * \brief   Appends a value to the logger.
   *
   * \tparam  T       Must be a type that is supported according to CREQ-LogUtility-SafeLogger
   *
   * \param   value   value to be logged.
   *
   * \return  log stream.
   * \spec
   *   requires true;
   * \endspec
   */
  template <typename T>
  auto operator<<(T value) noexcept -> SafeLogger&;

 private:
  /*!
   * \brief Create the logger with a severity.
   * \param ctx_id The context ID to use in case AUTOSAR logging is used.
   * \param fatal Switch to use either fatal or error severity.
   * \spec
   *   requires true;
   * \endspec
   */
  SafeLogger(vac::container::CStringView ctx_id, bool fatal) noexcept;
};

/*!
 * \internal
 * - #10 If the ara::log is usable.
 *  - #40 Then write the log message fragment via the ara::log::LogStream.
 * - #50 Else write the log message fragment via the FallbackLogger.
 * \endinternal
 * \spec
 *   requires true;
 * \endspec
 */
template <typename T>
auto SafeLogger::operator<<(T value) noexcept -> SafeLogger& {
  using vac::container::literals::operator""_sv;

  if (!!this->asr_) {
    // VCA_LOGUTI_OBJECT_IS_VALID
    *this->asr_ << value;
  } else {
    this->fallback_ << value;
  }

  return *this;
}

}  // namespace loguti
}  // namespace amsr

#endif  // LIB_LOG_UTILITY_INCLUDE_AMSR_LOGUTI_SAFE_LOGGER_H_
