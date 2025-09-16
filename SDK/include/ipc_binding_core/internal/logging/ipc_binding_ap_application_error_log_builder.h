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
 *        \brief  Provides helper functions to build the log streams for ApApplicationErrors values.
 *
 *********************************************************************************************************************/
#ifndef LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_LOGGING_IPC_BINDING_AP_APPLICATION_ERROR_LOG_BUILDER_H_
#define LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_LOGGING_IPC_BINDING_AP_APPLICATION_ERROR_LOG_BUILDER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/log/logging.h"
#include "someip-protocol/internal/ap_application_error.h"

namespace amsr {
namespace ipc_binding_core {
namespace internal {
namespace logging {

/*!
 * \brief Helper class for Ipc-Binding ApApplicationError values logging.
 *
 * \unit IpcBinding::IpcBindingCore::IpcBindingLogging
 */
class IpcBindingApApplicationErrorLogBuilder final {
 public:
  // ---- Construction -------------------------------------------------------------------------------------------------

  IpcBindingApApplicationErrorLogBuilder() noexcept = delete;
  IpcBindingApApplicationErrorLogBuilder(IpcBindingApApplicationErrorLogBuilder const&) noexcept = delete;
  auto operator=(IpcBindingApApplicationErrorLogBuilder const&) noexcept
      -> IpcBindingApApplicationErrorLogBuilder& = delete;
  IpcBindingApApplicationErrorLogBuilder(IpcBindingApApplicationErrorLogBuilder&&) noexcept = delete;
  auto operator=(IpcBindingApApplicationErrorLogBuilder&&) noexcept -> IpcBindingApApplicationErrorLogBuilder& = delete;

  /*!
   * \brief   Destroy the log builder.
   * \details Destructor for this utility class (static APIs only) provided to avoid potential undefined behavior in
   *          case of future extension (e.g. non-static members).
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  ~IpcBindingApApplicationErrorLogBuilder() noexcept = default;

  // ---- APIs ---------------------------------------------------------------------------------------------------------

  /*!
   * \brief Builds a log stream for the given ApApplicationError user message.
   *
   * \param[in,out] s             The log stream to write to.
   * \param[in]     user_message  The user message.
   *
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   * \steady    FALSE
   *
   * \spec requires true; \endspec
   */
  static void LogApAppErrorUserMessage(::ara::log::LogStream& s,
                                       ::ara::core::ErrorDomain::StringType const& user_message) noexcept;

  /*!
   * \brief Builds a log stream for the given ApApplicationError support data.
   *
   * \param[in,out] s             The log stream to write to.
   * \param[in]     support_data  The support data.
   *
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   * \steady    FALSE
   *
   * \spec requires true; \endspec
   */
  static void LogApAppErrorSupportData(::ara::log::LogStream& s,
                                       ::ara::core::ErrorDomain::SupportDataType const& support_data) noexcept;

  /*!
   * \brief Builds a log stream for the given ApApplicationError domain id.
   *
   * \param[in,out] s                The log stream to write to.
   * \param[in]     error_domain_id  The error domain id.
   *
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   * \steady    FALSE
   *
   * \spec requires true; \endspec
   */
  static void LogApAppErrorDomainId(::ara::log::LogStream& s,
                                    ::ara::core::ErrorDomain::IdType const error_domain_id) noexcept;

  /*!
   * \brief Builds a log stream for the given ApApplicationError value.
   *
   * \param[in,out] s                 The log stream to write to.
   * \param[in]     error_code_value  The error code value.
   *
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   * \steady    FALSE
   *
   * \spec requires true; \endspec
   */
  static void LogApAppErrorValue(::ara::log::LogStream& s,
                                 ::ara::core::ErrorDomain::CodeType const error_code_value) noexcept;

  /*!
   * \brief Builds a log stream for the given ApApplicationError.
   *
   * \param[in,out] s             The log stream to write to.
   * \param[in]     error_struct  The ApApplicationError structure.
   *
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   * \steady    FALSE
   *
   * \spec requires true; \endspec
   */
  static void LogCompleteApApplicationError(
      ::ara::log::LogStream& s,
      ::amsr::someip_protocol::internal::serialization::ApApplicationErrorStruct const& error_struct) noexcept;
};

}  // namespace logging
}  // namespace internal
}  // namespace ipc_binding_core
}  // namespace amsr

#endif  // LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_LOGGING_IPC_BINDING_AP_APPLICATION_ERROR_LOG_BUILDER_H_
