

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
/**        \file  SpeedMonitorExecutable/src/ara/core/initialization.cpp
 *        \brief  Provides ara::core::Initialize() and ara::core::Deinitialize().
 *
 *      \details  Provides the global initialization and shutdown functions that initialize resp. deinitialize data
*                 structures and threads of the AUTOSAR Runtime for Adaptive Applications (ARA).
 *
 *********************************************************************************************************************/
/**********************************************************************************************************************
 *  GENERATOR INFORMATION
 *  -------------------------------------------------------------------------------------------------------------------
 *    Generator Name: amsr_applicationbase
 *         Commit ID: d235206e9500accd0a88da4dc69c9cdbd297a572
 *********************************************************************************************************************/

#include <cstdlib>
#include <string>

#include "amsr/application_base/initialization_error_domain.h"
#include "amsr/application_base/internal/lifecycle.h"
#include "amsr/comintegrator/internal/lifecycle.h"
#include "amsr/generic/write_std_stream.h"
#include "amsr/log/internal/lifecycle.h"
#include "amsr/thread/internal/lifecycle.h"
#include "ara/core/initialization.h"
#include "ara/core/optional.h"
#include "ara/log/logger.h"
#include "ara/log/logging.h"
#include "osabstraction/internal/lifecycle.h"
#include "vac/container/string_literals.h"
#include "vac/internal/lifecycle.h"


namespace ara {
namespace core {
namespace details {

// VECTOR NC AutosarC++17_10-M3.4.1: MD_APP_BASE_AutosarC++17_10-M3.4.1_VariableCanBeDeclaredStaticInPrimaryFile
InitializationState g_initialization_state{InitializationState::kUninitialized};

}  // namespace details

/*!
 * \brief Returns a Result that contains an error code if the pre-conditions for initialization are not met.
 */
static ara::core::Result<void> CheckPreCondition() {
  ara::core::Result<void> res{amsr::application_base::InitializationErrc::kWrongSequence};

  if (ara::core::details::g_initialization_state == ara::core::details::InitializationState::kUninitialized) {
    res.EmplaceValue();
  }

  return res;
}

/*!
 * \brief Registers a de-initialization check that is to be made at normal program termination.
 */
static void RegisterAtExit() {
  static_cast<void>(std::atexit([] { // VCA_APPLICATIONBASE_UNDEFINED_FUNCTION, VCA_APPLICATIONBASE_FULFILLED_FUNCTION_CONTRACT
    if (ara::core::details::g_initialization_state == ara::core::details::InitializationState::kInitialized) {
      static_cast<void>(
          amsr::generic::WriteToStderr("Application terminated without proper call to ara::core::Deinitialize()!\n"));
    }
  }));
}

}  // namespace core
}  // namespace ara

ara::core::Result<void> ara::core::Initialize() noexcept {
  using vac::container::literals::operator""_sv;
  vac::container::CStringView current_initialize_component{"CheckPreCondition()"};
  return CheckPreCondition()
      .AndThen([&current_initialize_component]() { // COV_AppBase_UnreachableLambda
        current_initialize_component = "vac::internal::InitializeComponent()"_sv;
        return vac::internal::InitializeComponent(); // VCA_APPLICATIONBASE_VALID_FUNCTION_CALL, VCA_APPLICATIONBASE_FUNCTION_CALL_VERIFIED
      })
      .AndThen([&current_initialize_component]() {
        current_initialize_component = "amsr::application_base::internal::InitializeComponent()"_sv;
        return amsr::application_base::internal::InitializeComponent(); // VCA_APPLICATIONBASE_VALID_FUNCTION_CALL, VCA_APPLICATIONBASE_FUNCTION_CALL_VERIFIED
      })
      .AndThen([&current_initialize_component]() {
        current_initialize_component = "osabstraction::internal::InitializeComponent()"_sv;
        return osabstraction::internal::InitializeComponent(); // VCA_APPLICATIONBASE_VALID_FUNCTION_CALL, VCA_APPLICATIONBASE_FUNCTION_CALL_VERIFIED
      })
      .AndThen([&current_initialize_component]() {
        current_initialize_component = "amsr::thread::internal::InitializeComponent()"_sv;
        return amsr::thread::internal::InitializeComponent(); // VCA_APPLICATIONBASE_VALID_FUNCTION_CALL, VCA_APPLICATIONBASE_FUNCTION_CALL_VERIFIED
      })
      .AndThen([&current_initialize_component]() {
        current_initialize_component = "amsr::log::internal::InitializeComponent()"_sv;
        return amsr::log::internal::InitializeComponent(); // VCA_APPLICATIONBASE_VALID_FUNCTION_CALL, VCA_APPLICATIONBASE_FUNCTION_CALL_VERIFIED
      })
      .AndThen([&current_initialize_component]() {
        current_initialize_component = "amsr::comintegrator::internal::InitializeComponent()"_sv;
        return amsr::comintegrator::internal::InitializeComponent(); // VCA_APPLICATIONBASE_VALID_FUNCTION_CALL, VCA_APPLICATIONBASE_FUNCTION_CALL_VERIFIED
      })
      .Inspect([]() {
        ara::core::details::g_initialization_state = ara::core::details::InitializationState::kInitialized;
        RegisterAtExit();
      })
      .InspectError([&current_initialize_component](ara::core::ErrorCode const& error) {
        static_cast<void>(amsr::generic::WriteToStderr("ara::core::Initialize() failed! Error in "));
        static_cast<void>(amsr::generic::WriteToStderr(current_initialize_component));
        static_cast<void>(amsr::generic::WriteToStderr(". Result contains: "));
        static_cast<void>(amsr::generic::WriteToStderr(error.Message())); // VCA_APPLICATIONBASE_NON_VERIFIED_PARAMETERS
        static_cast<void>(amsr::generic::WriteToStderr(", "));
        static_cast<void>(amsr::generic::WriteToStderr(error.UserMessage())); // VCA_APPLICATIONBASE_NON_VERIFIED_PARAMETERS
        static_cast<void>(amsr::generic::WriteToStderr("\n"));
      });
}

ara::core::Result<void> ara::core::Deinitialize() noexcept {
  using vac::container::literals::operator""_sv;
  ara::core::Result<void> res{amsr::application_base::InitializationErrc::kWrongSequence};

  if (ara::core::details::g_initialization_state == ara::core::details::InitializationState::kInitialized) {
    // when Initialize() was called before, deinitialize; otherwise return kWrongSequence
    res = amsr::comintegrator::internal::DeinitializeComponent() // VCA_APPLICATIONBASE_FUNCTION_CALL_VERIFIED
              .AndThen([]() {
                if (amsr::application_base::internal::IsComponentInitialized()) {
                  ara::core::Optional<std::size_t> const default_resource_size{amsr::application_base::internal::GetInstance()->GetDefaultResourceSize()};
                  if (default_resource_size) {
                    ara::log::Logger& logger{ara::log::CreateLogger("vDRS"_sv, "Vector logger for AMSR Default Resource."_sv)};
                    logger.LogInfo() << "DefaultResource: MonotonicBufferResource size: "_sv << default_resource_size.value();
                  }
                }
                return amsr::log::internal::DeinitializeComponent(); // VCA_APPLICATIONBASE_VALID_FUNCTION_CALL, VCA_APPLICATIONBASE_FUNCTION_CALL_VERIFIED
              })
              .AndThen([]() { return amsr::thread::internal::DeinitializeComponent(); }) // VCA_APPLICATIONBASE_VALID_FUNCTION_CALL, VCA_APPLICATIONBASE_FUNCTION_CALL_VERIFIED
              .AndThen([]() { return osabstraction::internal::DeinitializeComponent(); }) // VCA_APPLICATIONBASE_VALID_FUNCTION_CALL, VCA_APPLICATIONBASE_FUNCTION_CALL_VERIFIED
              .AndThen([]() { return amsr::application_base::internal::DeinitializeComponent(); }) // VCA_APPLICATIONBASE_VALID_FUNCTION_CALL, VCA_APPLICATIONBASE_FUNCTION_CALL_VERIFIED
              .AndThen([]() { return vac::internal::DeinitializeComponent(); }) // VCA_APPLICATIONBASE_VALID_FUNCTION_CALL, VCA_APPLICATIONBASE_FUNCTION_CALL_VERIFIED, COV_AppBase_UnreachableLambda
              .Inspect([]() {ara::core::details::g_initialization_state = ara::core::details::InitializationState::kDeinitialized;})
              .InspectError([](ara::core::ErrorCode const& error) {
                static_cast<void>(amsr::generic::WriteToStderr("ara::core::Deinitialize() failed! Result contains: "));
                static_cast<void>(amsr::generic::WriteToStderr(error.Message())); // VCA_APPLICATIONBASE_NON_VERIFIED_PARAMETERS
                static_cast<void>(amsr::generic::WriteToStderr(", "));
                static_cast<void>(amsr::generic::WriteToStderr(error.UserMessage())); // VCA_APPLICATIONBASE_NON_VERIFIED_PARAMETERS
                static_cast<void>(amsr::generic::WriteToStderr("\n"));
              });
  }

  return res;
}

// COV_JUSTIFICATION_BEGIN
//
// \ID COV_AppBase_UnreachableLambda
//   \ACCEPT xx
//   \REASON The InitializeComponent and DeinitializeComponent functions for VaCommonLib always produces a valid Result.
//           Consequently, the case where an Error is returned cannot be covered.
//
// COV_JUSTIFICATION_END

