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
/**     \file       fallback_logger.h
 *      \brief      Contains the declaration for the fallback logger.
 *      \details
 *
 *      \unit       LogAndTrace::Common::FallbackLogger
 *
 *********************************************************************************************************************/
#ifndef LIB_COMMON_INCLUDE_AMSR_LOG_INTERNAL_FALLBACK_LOGGER_H_
#define LIB_COMMON_INCLUDE_AMSR_LOG_INTERNAL_FALLBACK_LOGGER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/core/string.h"
#include "amsr/log/internal/log_error_domain.h"
#include "amsr/log/internal/utils.h"

namespace amsr {
namespace log {
namespace internal {
/*!
 * \brief The fallback logger used
 *
 * \vprivate Component internal.
 */
class FallbackLogger final {
  /*! \brief A buffer for a single message. */
  core::String buffer_{};

 public:
  /*!
   * \brief Creates the logger.
   * \spec
   *   requires true;
   * \endspec
   */
  FallbackLogger() noexcept = default;
  /*!
   * \brief Allow move construction.
   * \spec
   *   requires true;
   * \endspec
   */
  FallbackLogger(FallbackLogger&&) noexcept = default;
  /*!
   * \brief Allow move assignment.
   * \spec
   *   requires true;
   * \endspec
   */
  auto operator=(FallbackLogger&&) & noexcept -> FallbackLogger& = default;
  /*! \brief Prohibit copy construction. */
  FallbackLogger(FallbackLogger const&) noexcept = delete;
  /*! \brief Prohibit copy assignment. */
  auto operator=(FallbackLogger const&) & noexcept -> FallbackLogger& = delete;
  /*!
   * \brief Destroys the instance.
   * \spec
   *   requires true;
   * \endspec
   */
  ~FallbackLogger() noexcept;
  /*!
   * \brief Flushes the written message.
   * \spec
   *   requires true;
   * \endspec
   */
  void Flush() noexcept;
  /*!
   * \brief Adds a string view to the fallback logger.
   *
   * \param sv The string to add.
   * \return   A reference to the fallback logger.
   * \spec
   *   requires true;
   * \endspec
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   *
   * \vprivate Component internal.
   */
  auto operator<<(core::StringView sv) noexcept -> FallbackLogger&;

  // VECTOR NC AutosarC++17_10-A3.9.1: MD_LOG_AutosarC++17_10-A3.9.1_basic_type
  /*!
   * \brief Adds an integer to the fallback logger.
   *
   * \param value The integer to add.
   * \return      A reference to the fallback logger.
   * \spec
   *   requires true;
   * \endspec
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   *
   * \vprivate Component internal.
   */
  auto operator<<(unsigned long long int value) noexcept -> FallbackLogger&;  // NOLINT(runtime/int)
};

}  // namespace internal
}  // namespace log
}  // namespace amsr

#endif  // LIB_COMMON_INCLUDE_AMSR_LOG_INTERNAL_FALLBACK_LOGGER_H_
