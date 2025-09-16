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
/**        \file  SpeedMonitorExecutable/src/services/speedlimitservice/speedlimitservice_skeleton.cpp
 *        \brief  Skeleton for service 'SpeedLimitService'.
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
#include "services/speedlimitservice/speedlimitservice_skeleton.h"
#include "amsr/core/abort.h"
#include "amsr/core/error_code.h"
#include "amsr/core/string.h"
#include "amsr/socal/internal/instance_specifier_lookup_table.h"
#include "amsr/socal/internal/logging/ara_com_logger.h"

/*!
 * \trace SPEC-4980240, SPEC-4980241
 */
namespace services {
namespace speedlimitservice {



namespace skeleton {

SpeedLimitServiceSkeleton::ConstructionResult SpeedLimitServiceSkeleton::Preconstruct(
    ::ara::com::InstanceIdentifier instance_id, ::ara::com::MethodCallProcessingMode const mode,
    std::size_t const max_requests) noexcept {
  return Base::Preconstruct(instance_id, mode, max_requests);
}

SpeedLimitServiceSkeleton::ConstructionResult SpeedLimitServiceSkeleton::Preconstruct(
    ::ara::core::InstanceSpecifier instance, ::ara::com::MethodCallProcessingMode const mode,
    std::size_t const max_requests) noexcept {
  return Base::Preconstruct(instance, mode, max_requests);
}

SpeedLimitServiceSkeleton::ConstructionResult SpeedLimitServiceSkeleton::Preconstruct(
    ::ara::com::InstanceIdentifierContainer instance_identifiers, ::ara::com::MethodCallProcessingMode const mode,
    std::size_t const max_requests) noexcept {
  return Base::Preconstruct(instance_identifiers, mode, max_requests); // VCA_SOCAL_CALLING_DESTRUCTOR_OF_STACK_OBJECTS
}

SpeedLimitServiceSkeleton::SpeedLimitServiceSkeleton(ConstructionToken&& token) noexcept
: Base{std::move(token)}
, SpeedOverLimitEvent(this)
{
}

SpeedLimitServiceSkeleton::SpeedLimitServiceSkeleton(::ara::com::InstanceIdentifier instance,
                                               ::ara::com::MethodCallProcessingMode mode) noexcept
     : SpeedLimitServiceSkeleton{Preconstruct(instance, mode)  // VCA_SOCAL_VALID_COPY_OR_MOVE_CTOR_ARGUMENT
                               .InspectError([](::amsr::core::ErrorCode const& error_code) {
                                 ::amsr::core::String error_msg{  // VCA_SOCAL_EXTERNAL_FUNCTION_CONTRACT
                                     "Failed to create SpeedLimitServiceSkeleton::ConstructionToken with error: "};
                                 error_msg.append(error_code.Message());  // VCA_SOCAL_EXTERNAL_FUNCTION_CONTRACT
                                 ::amsr::core::Abort(error_msg.c_str());
                               })
                               .Value()} {}

SpeedLimitServiceSkeleton::SpeedLimitServiceSkeleton(::ara::core::InstanceSpecifier instance,
                                               ::ara::com::MethodCallProcessingMode mode) noexcept
     : SpeedLimitServiceSkeleton{Preconstruct(instance, mode)
                               .InspectError([](::amsr::core::ErrorCode const& error_code) {
                                 ::amsr::core::String error_msg{  // VCA_SOCAL_EXTERNAL_FUNCTION_CONTRACT
                                     "Failed to create SpeedLimitServiceSkeleton::ConstructionToken with error: "};
                                 error_msg.append(error_code.Message());  // VCA_SOCAL_EXTERNAL_FUNCTION_CONTRACT
                                 ::amsr::core::Abort(error_msg.c_str());
                               })
                               .Value()} {}

SpeedLimitServiceSkeleton::SpeedLimitServiceSkeleton(::ara::com::InstanceIdentifierContainer instance_identifiers,
                                               ::ara::com::MethodCallProcessingMode mode) noexcept
     // VCA_SOCAL_CALLING_STL_APIS, VCA_SOCAL_VALID_COPY_OR_MOVE_CTOR_ARGUMENT
     : SpeedLimitServiceSkeleton{Preconstruct(instance_identifiers, mode)
                               .InspectError([](::amsr::core::ErrorCode const& error_code) {
                                 ::amsr::core::String error_msg{  // VCA_SOCAL_EXTERNAL_FUNCTION_CONTRACT
                                     "Failed to create SpeedLimitServiceSkeleton::ConstructionToken with error: "};
                                 error_msg.append(error_code.Message());  // VCA_SOCAL_EXTERNAL_FUNCTION_CONTRACT
                                 ::amsr::core::Abort(error_msg.c_str());
                               })
                               .Value()} {}

// VCA_SOCAL_FREEING_STATICALLY_ALLOCATED_MEMORY
SpeedLimitServiceSkeleton::~SpeedLimitServiceSkeleton() noexcept {
  if (IsServiceOffered()) {
    logger_.LogFatalAndAbort([](::amsr::socal::internal::logging::AraComLogger::StringStream& s) { 
      // VCA_SOCAL_CALLING_STL_APIS
      s << "Skeleton must stop offering the service before destruction."; },
                    static_cast<char const*>(__func__), __LINE__);
  }
} // VCA_SOCAL_CALLING_STL_APIS

// VECTOR NC AutosarC++17_10-M0.1.8: MD_SOCAL_M0.1.8_VoidFunctionHasNoExternalSideEffect
// VECTOR NC Metric-HIS.PATH, Metric-HIS.VG: MD_SOCAL_Metric-HIS.PATH_FieldInitChecks
void SpeedLimitServiceSkeleton::DoFieldInitializationChecks() noexcept {
}

}  // namespace skeleton
}  // namespace speedlimitservice
}  // namespace services

