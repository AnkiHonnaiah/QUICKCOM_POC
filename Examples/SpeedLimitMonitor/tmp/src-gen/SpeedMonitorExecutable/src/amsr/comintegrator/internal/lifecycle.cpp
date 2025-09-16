/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2025 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/**        \file  SpeedMonitorExecutable/src/amsr/comintegrator/internal/lifecycle.cpp
 *        \brief  Manages the life cycle of ara::com components.
 *
 *      \details
 *
 *********************************************************************************************************************/
/**********************************************************************************************************************
 *  GENERATOR INFORMATION
 *  -------------------------------------------------------------------------------------------------------------------
 *    Generator Name: amsr_comintegrator
 *         Commit ID: 7e30554bf0628e882432510b5d30f2c6e50f0e2b
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <atomic>

#include "amsr/comintegrator/internal/lifecycle.h"
#include "amsr/socal/internal/lifecycle.h"
#include "amsr/comtrace/com_trace_error_domain.h"
#include "amsr/comtrace/internal/life_cycle.h"
#include "amsr/core/error_code.h"
#include "amsr/core/error_domain.h"
#include "ara/com/com_error_domain.h"
#include "ipc_binding/internal/life_cycle.h"
#include "ipc_binding_core/internal/error_domain.h"

namespace amsr {
namespace comintegrator {
namespace internal {

/*!
 * \brief The internal initialization state of ara::com.
 */
static std::atomic<bool> is_initialized{false};

::amsr::core::Result<void> InitializeComponent() noexcept {
  ::amsr::core::Result<void> result{
      ::ara::com::MakeErrorCode(::ara::com::ComErrc::kWrongInitSequence, 0, "ara::com is already initialized!")};

  if (!IsComponentInitialized()) {
    // VCA_COMINTEGRATOR_EXTERNAL_FUNCTION_CONTRACT
    result = ::amsr::comtrace::internal::InitializeComponent()  // Initialize ComTrace.
                 .MapError([](::amsr::core::ErrorCode const& err_code) -> ::amsr::core::ErrorCode {
                   ::amsr::core::ErrorCode mapped_error_code{
                       ::ara::com::MakeErrorCode(::ara::com::ComErrc::kErrorNotOk, 0, err_code.Message().data())};
                   using CodeType = typename ::amsr::core::ErrorDomain::CodeType;
                   // VECTOR NC AutosarC++17_10-A6.4.1: MD_COMINTEGRATOR_AutosarC++17_10-A6.4.1_SwitchWithLessCodePaths
                   switch (err_code.Value()) {
                     case static_cast<CodeType>(::amsr::comtrace::ComTraceErrc::json_parsing_failure):
                       mapped_error_code = ::ara::com::MakeErrorCode(::ara::com::ComErrc::kJsonParsingFailed, 0,
                                                                     err_code.Message().data());
                       break;
                     default:
                       // Do nothing.
                       break;
                   }
                   return mapped_error_code;
                 })
                 .AndThen([]() {
                   // Initialize Socal.
                   // VCA_COMINTEGRATOR_EXTERNAL_FUNCTION_CONTRACT
                   return ::amsr::socal::internal::InitializeComponent();
                 })
                 .AndThen([]() {
                   // Initialize IPC binding.
                   // VCA_COMINTEGRATOR_EXTERNAL_FUNCTION_CONTRACT
                   return ::amsr::ipc_binding::internal::InitializeComponent().MapError(
                       [](::amsr::core::ErrorCode const& err_code) -> ::amsr::core::ErrorCode {
                         ::amsr::core::ErrorCode mapped_error_code{
                             ::ara::com::MakeErrorCode(::ara::com::ComErrc::kErrorNotOk, 0, err_code.Message().data())};
                         using CodeType = typename ::amsr::core::ErrorDomain::CodeType;
                         switch (err_code.Value()) {
                           case static_cast<CodeType>(
                               ::amsr::ipc_binding_core::internal::IpcBindingErrc::wrong_init_sequence):
                             mapped_error_code = ::ara::com::MakeErrorCode(::ara::com::ComErrc::kWrongInitSequence, 0,
                                                                           err_code.Message().data());
                             break;
                           case static_cast<CodeType>(
                               ::amsr::ipc_binding_core::internal::IpcBindingErrc::json_parsing_failure):
                             mapped_error_code = ::ara::com::MakeErrorCode(::ara::com::ComErrc::kJsonParsingFailed, 0,
                                                                           err_code.Message().data());
                             break;
                           default:
                             // Do nothing.
                             break;
                         }
                         return mapped_error_code;
                       });
                 })
                 .Inspect([]() {
                   // Start Socal.
                   ::amsr::socal::internal::Start();  // VCA_COMINTEGRATOR_EXTERNAL_FUNCTION_CONTRACT
                 })
                 .Inspect([]() {
                   is_initialized.store(true);
                   ::amsr::socal::internal::SetAraComInitStatus(true); // VCA_COMINTEGRATOR_EXTERNAL_FUNCTION_CONTRACT
                 });
  }

  return result;
}

::amsr::core::Result<void> DeinitializeComponent() noexcept {
  ::amsr::core::Result<void> result{};
  if (IsComponentInitialized()) {
    // Stop Socal.
    result = ::amsr::socal::internal::Stop()  // VCA_COMINTEGRATOR_EXTERNAL_FUNCTION_CONTRACT
                 .AndThen([]() {
                   // De-initialize IPC binding.
                   // VCA_COMINTEGRATOR_EXTERNAL_FUNCTION_CONTRACT
                   return ::amsr::ipc_binding::internal::DeinitializeComponent();
                 })
                 .AndThen([]() {
                   // De-initialize Socal.
                   // VCA_COMINTEGRATOR_EXTERNAL_FUNCTION_CONTRACT
                   return ::amsr::socal::internal::DeinitializeComponent();
                 })
                 .AndThen([]() {
                   // De-initialize ComTrace.
                   // VCA_COMINTEGRATOR_EXTERNAL_FUNCTION_CONTRACT
                   return ::amsr::comtrace::internal::DeinitializeComponent();
                 })
                 .Inspect([]() {
                   is_initialized.store(false);
                   ::amsr::socal::internal::SetAraComInitStatus(false); // VCA_COMINTEGRATOR_EXTERNAL_FUNCTION_CONTRACT
                 })
                 .MapError([](::amsr::core::ErrorCode const& err_code) -> ::amsr::core::ErrorCode {
                   return ::ara::com::MakeErrorCode(::ara::com::ComErrc::kErrorNotOk, 0, err_code.Message().data());
                 });
  }

  return result;
}

bool IsComponentInitialized() noexcept { return is_initialized.load(); }

}  // namespace internal
}  // namespace comintegrator
}  // namespace amsr

