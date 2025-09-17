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
/**        \file  WarningDisplayExecutable/src/amsr/socal/internal/runtime_gen.cpp
 *        \brief  Runtime initialization.
 *
 *      \details
 *
 *********************************************************************************************************************/
/**********************************************************************************************************************
 *  GENERATOR INFORMATION
 *  -------------------------------------------------------------------------------------------------------------------
 *    Generator Name: amsr_socal
 *         Commit ID: ad694a8ff64fb52ed9a0b9551cf05c10d499927a
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/socal/internal/lifecycle_manager.h"
#include "amsr/socal/internal/runtime.h"
#include "ara/core/abort.h"
#include "vac/container/string_literals.h"

namespace amsr {
namespace socal {
namespace internal {
configuration::RuntimeProcessingMode const Runtime::runtime_processing_mode_{::amsr::socal::internal::configuration::RuntimeProcessingMode::kThreadDriven};
static_assert(std::is_trivially_destructible<configuration::RuntimeProcessingMode>::value,
              "::amsr::socal::internal::configuration::RuntimeProcessingMode not trivially destructible!");

// VECTOR NC AutosarC++17_10-M9.3.3: MD_SOCAL_AutosarC++17_10-M9.3.3_Method_can_be_declared_static
// VCA_SOCAL_EXTERNAL_FUNCTION_CONTRACT
void Runtime::InitializeLifecycleManager() noexcept { ::amsr::socal::internal::LifecycleManager::Initialize(); }

// VECTOR NC AutosarC++17_10-M9.3.3: MD_SOCAL_AutosarC++17_10-M9.3.3_Method_can_be_declared_static
// VCA_SOCAL_EXTERNAL_FUNCTION_CONTRACT
void Runtime::DeinitializeLifecycleManager() noexcept { ::amsr::socal::internal::LifecycleManager::Deinitialize(); }

}  // namespace internal
}  // namespace socal
}  // namespace amsr

