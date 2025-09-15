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
/*!        \file  socal_runtime_wrapper.h
 *        \brief  Implementation of RuntimeInterface using amsr::socal::internal::Runtime
 *        \unit   SomeIpBinding::SomeIpBinding::SocalRuntimeWrapper
 *
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_BINDING_INCLUDE_AMSR_SOMEIP_BINDING_INTERNAL_SOCAL_RUNTIME_WRAPPER_H_
#define LIB_SOMEIP_BINDING_INCLUDE_AMSR_SOMEIP_BINDING_INTERNAL_SOCAL_RUNTIME_WRAPPER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include "amsr/core/abort.h"
#include "amsr/core/instance_specifier.h"
#include "amsr/core/string_view.h"
#include "amsr/generic/singleton_wrapper.h"
#include "amsr/loguti/stream.h"
#include "amsr/socal/internal/configuration/configuration.h"
#include "amsr/socal/internal/internal_instance_identifier.h"
#include "amsr/socal/internal/runtime.h"
#include "amsr/someip_binding/internal/runtime_interface.h"
#include "amsr/someip_binding_core/internal/logging/ara_com_logger_singleton.h"
#include "amsr/steady_timer/timer_manager_interface.h"
#include "ara/com/instance_identifier.h"
#include "osabstraction/io/reactor1/reactor1.h"

namespace amsr {
namespace someip_binding {
namespace internal {

/*!
 * \brief Use string literal for creation of StringViews.
 */
// VECTOR NC AutosarC++17_10-M7.3.6, VectorC++-V5.0.1: MD_SOMEIPBINDING_AutosarC++17_10-M7.3.6_using_directive
// VECTOR NC AutosarC++17_10-M7.3.4, VectorC++-V5.0.1: MD_SOMEIPBINDING_AutosarC++17_10-M7.3.4_using_directive
using vac::container::literals::operator""_sv;

/*!
 * \brief  RuntimeInterface implementation that delegates all calls to amsr::socal::internal::Runtime instance.
 * \tparam RuntimeType Used runtime class to perform the requested operations.
 */
template <typename RuntimeType = amsr::socal::internal::Runtime>
class SocalRuntimeWrapper : public RuntimeInterface {
 public:
  /*!
   * \brief   Default constructor.
   * \context Init
   * \spec
   *   requires true;
   * \endspec
   */
  SocalRuntimeWrapper() = default;

  /*!
   * \brief Use default destructor.
   * \spec
   *   requires true;
   * \endspec
   */
  // VCA_SLC_23_SOMEIPBINDING_DEFAULT_CTOR_DTOR_MOLE_1298
  ~SocalRuntimeWrapper() noexcept override = default;

  SocalRuntimeWrapper(SocalRuntimeWrapper const&) = delete;
  SocalRuntimeWrapper(SocalRuntimeWrapper&&) = delete;
  SocalRuntimeWrapper& operator=(SocalRuntimeWrapper const&) & = delete;
  SocalRuntimeWrapper& operator=(SocalRuntimeWrapper&&) & = delete;

  /*!
   * \brief       Get the used Reactor.
   * \return      Reactor.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   *
   * \internal
   * - Delegate the call to the Socal runtime.
   * \endinternal
   */
  // VECTOR NC AutosarC++17_10-A15.4.2: MD_SOMEIPBINDING_AutosarC++17_10-A15.4.2_A15.5.1_A15.5.2_A15.5.3_GoogleTest
  // VECTOR NC AutosarC++17_10-A15.5.3: MD_SOMEIPBINDING_AutosarC++17_10-A15.4.2_A15.5.1_A15.5.2_A15.5.3_GoogleTest
  osabstraction::io::reactor1::Reactor1& GetReactor() noexcept override {
    // VCA_SOMEIPBINDING_TRIVIAL_FUNCTION_CONTRACT
    return socal_runtime_->GetReactor();
  }

  /*!
   * \brief Returns reference to the steady TimerManager object.
   * \return reference to the timer manager
   *
   * \pre -
   * \context Init
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  ::amsr::steady_timer::TimerManagerInterface& GetSteadyTimerManager() noexcept override {
    // VCA_SOMEIPBINDING_TRIVIAL_FUNCTION_CONTRACT
    return socal_runtime_->GetSteadyTimerManager();
  }

  /*!
   * \brief       Get the function that triggers the polling API in Socal.
   * \return      A function that triggers Socal's polling API.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   *
   * \internal
   * - Create a lambda function that calls processes polling API on the Socal runtime.
   *  - Return the lambda function.
   * \endinternal
   */
  // VECTOR NC AutosarC++17_10-A15.4.2: MD_SOMEIPBINDING_AutosarC++17_10-A15.4.2_A15.5.1_A15.5.2_A15.5.3_GoogleTest
  // VECTOR NC AutosarC++17_10-A15.5.3: MD_SOMEIPBINDING_AutosarC++17_10-A15.4.2_A15.5.1_A15.5.2_A15.5.3_GoogleTest
  vac::language::UniqueFunction<void(), ara::core::PolymorphicAllocator<std::uint8_t>>
  GetProcessPollingFunction() noexcept override {
    ::amsr::generic::SingletonAccess<RuntimeType> const socal_runtime{RuntimeType::getSingletonInstance().GetAccess()};
    // VCA_SOMEIPBINDING_TRIVIAL_FUNCTION_CONTRACT
    return {[runtime_access = socal_runtime]() mutable { static_cast<void>(runtime_access->ProcessPolling()); }};
  }

  /*!
   * \brief Add a new mapping entry between a ProvidedInstanceSpecifier and a InstanceIdentifier.
   * \details This API must be called multiple times to register all known mappings.
   *
   * \param[in] instance_specifier The instance specifier which will be the key for the new entry.
   * \param[in] instance_identifier The instance identifier which will be the value for the new entry.
   *                                Expected format: "<binding_name>:<binding specific instance ID>".
   * \pre The same InstanceIdentifier and InstanceSpecifier combination must have not been mapped earlier.
   * \context Init
   * \threadsafe FALSE
   * \reentrant FALSE
   * \vprivate Vector component internal API
   * \synchronous TRUE
   *
   * \internal
   * - Delegate call to the productive runtime instance.
   *
   * Calls "amsr::core::Abort()" if:
   * - The given instance specifier is not present in the provided instance specifiers lookup table, or
   * - The given instance identifier is already mapped to the given instance specifier.
   * \endinternal
   */
  void MapProvidedInstanceSpecifierToInstanceId(
      amsr::core::InstanceSpecifier const& instance_specifier,
      ara::com::InstanceIdentifier const& instance_identifier) noexcept override {
    // VCA_SOMEIPBINDING_TEMPLATED_INSTANCE_IDENTIFIER_CONSTRUCTOR
    amsr::socal::internal::ProvidedInstanceIdentifier const provided_id{instance_identifier};
    // VCA_SOMEIPBINDING_UNDEFINED_FUNCTION
    bool const result{socal_runtime_->MapProvidedInstanceSpecifierToInstanceId(instance_specifier, provided_id)};
    if (!result) {
      ::amsr::someip_binding_core::internal::logging::AraComLoggerSingleton::GetInstance()
          .GetAccess()
          ->LogFatalAndAbort(
              ::amsr::core::StringView{"SocalRuntimeWrapper"},
              [&instance_specifier,
               &instance_identifier](::amsr::someip_binding_core::internal::logging::StringStream& s) {
                s << "Unknown InstanceSpecifier('";
                s << instance_specifier.ToString();
                s << "') or the InstanceIdentifier('";
                s << instance_identifier.ToString();
                s << "') is already present in the lookup table.";
              },
              static_cast<char const*>(__func__), __LINE__);
    }
  }

  /*!
   * \brief Remove mapping entry between a ProvidedInstanceSpecifier and a InstanceIdentifier.
   * \details This API must be called multiple times to remove all known mappings.
   *
   * \param[in] instance_specifier The instance specifier which will be the key for the new entry.
   * \param[in] instance_identifier The instance identifier which will be the value for the new entry. Expected
   * format:
   * "<Binding type/prefix>:<binding specific instance ID>"
   * \pre The InstanceIdentifier and InstanceSpecifier combination must have been mapped earlier.
   * \context Shutdown
   * \threadsafe FALSE
   * \reentrant FALSE
   * \vprivate Vector component internal API
   * \synchronous TRUE
   *
   * \internal
   * - Delegate call to the productive runtime instance.
   *
   * Calls "amsr::core::Abort()" if:
   * - The given instance specifier is not present in the provided instance specifiers lookup table, or
   * - The given instance identifier is not already mapped to the given instance specifier.
   * \endinternal
   */
  void RemoveProvidedInstanceSpecifierEntry(amsr::core::InstanceSpecifier const& instance_specifier,
                                            ara::com::InstanceIdentifier const& instance_identifier) noexcept override {
    // VCA_SOMEIPBINDING_TRIVIAL_FUNCTION_CONTRACT
    bool const result{socal_runtime_->RemoveProvidedInstanceSpecifierEntry(instance_specifier, instance_identifier)};
    if (!result) {
      ::amsr::someip_binding_core::internal::logging::AraComLoggerSingleton::GetInstance()
          .GetAccess()
          ->LogFatalAndAbort(
              ::amsr::core::StringView{"SocalRuntimeWrapper"},
              [&instance_specifier,
               &instance_identifier](::amsr::someip_binding_core::internal::logging::StringStream& s) {
                s << "Unknown InstanceSpecifier('";
                s << instance_specifier.ToString();
                s << "') and InstanceIdentifier('";
                s << instance_identifier.ToString();
                s << "') mapping.";
              },
              static_cast<char const*>(__func__), __LINE__);
    }
  }

  /*!
   * \brief Add a new mapping entry between a RequiredInstanceSpecifier and a InstanceIdentifier.
   * \details This API must be called multiple times to register all known mappings.
   *
   * \param[in] instance_specifier The instance specifier which will be the key for the new entry.
   * \param[in] instance_identifier The instance identifier which will be the value for the new entry.
   *                                Expected format:"<binding_name>:<binding specific instance ID>".
   * \pre The same InstanceIdentifier and InstanceSpecifier combination must have not been mapped earlier.
   * \context Init
   * \threadsafe FALSE
   * \reentrant FALSE
   * \vprivate Vector component internal API
   * \synchronous TRUE
   *
   * \internal
   * - Delegate call to the productive runtime instance.
   *
   * Calls "amsr::core::Abort()" if:
   * - The given instance specifier is not present in the required instance specifiers lookup table, or
   * - The given instance identifier is already mapped to the given instance specifier.
   * \endinternal
   */
  void MapRequiredInstanceSpecifierToInstanceId(
      amsr::core::InstanceSpecifier const& instance_specifier,
      ara::com::InstanceIdentifier const& instance_identifier) noexcept override {
    // VCA_SOMEIPBINDING_TEMPLATED_INSTANCE_IDENTIFIER_CONSTRUCTOR
    amsr::socal::internal::RequiredInstanceIdentifier const required_id{instance_identifier};
    // VCA_SOMEIPBINDING_UNDEFINED_FUNCTION
    bool const result{socal_runtime_->MapRequiredInstanceSpecifierToInstanceId(instance_specifier, required_id)};
    if (!result) {
      ::amsr::someip_binding_core::internal::logging::AraComLoggerSingleton::GetInstance()
          .GetAccess()
          ->LogFatalAndAbort(
              ::amsr::core::StringView{"SocalRuntimeWrapper"},
              [&instance_specifier,
               &instance_identifier](::amsr::someip_binding_core::internal::logging::StringStream& s) {
                s << "Unknown InstanceSpecifier('";
                s << instance_specifier.ToString();
                s << "') or the InstanceIdentifier('";
                s << instance_identifier.ToString();
                s << "') is already present in the lookup table.";
              },
              static_cast<char const*>(__func__), __LINE__);
    }
  }

  /*!
   * \brief Remove mapping entry between a RequiredInstanceSpecifier and a InstanceIdentifier.
   * \details This API must be called multiple times to remove all known mappings.
   *
   * \param[in] instance_specifier The instance specifier which will be the key for the new entry.
   * \param[in] instance_identifier The instance identifier which will be the value for the new entry. Expected
   * format:
   * "<Binding type/prefix>:<binding specific instance ID>"
   * \pre The InstanceIdentifier and InstanceSpecifier combination must have been mapped earlier.
   * \context Shutdown
   * \threadsafe FALSE
   * \reentrant FALSE
   * \vprivate Vector component internal API
   * \synchronous TRUE
   *
   * \internal
   * - Delegate call to the productive runtime instance.
   *
   * Calls "amsr::core::Abort()" if:
   * - The given instance specifier is not present in the required instance specifiers lookup table, or
   * - The given instance identifier is not already mapped to the given instance specifier.
   * \endinternal
   */
  void RemoveRequiredInstanceSpecifierEntry(amsr::core::InstanceSpecifier const& instance_specifier,
                                            ara::com::InstanceIdentifier const& instance_identifier) noexcept override {
    // VCA_SOMEIPBINDING_TRIVIAL_FUNCTION_CONTRACT
    bool const result{socal_runtime_->RemoveRequiredInstanceSpecifierEntry(instance_specifier, instance_identifier)};
    if (!result) {
      ::amsr::someip_binding_core::internal::logging::AraComLoggerSingleton::GetInstance()
          .GetAccess()
          ->LogFatalAndAbort(
              ::amsr::core::StringView{"SocalRuntimeWrapper"},
              [&instance_specifier,
               &instance_identifier](::amsr::someip_binding_core::internal::logging::StringStream& s) {
                s << "Unknown InstanceSpecifier('";
                s << instance_specifier.ToString();
                s << "') and InstanceIdentifier('";
                s << instance_identifier.ToString();
                s << "') mapping.";
              },
              static_cast<char const*>(__func__), __LINE__);
    }
  }

 private:
  /*!
   * \brief   Socal runtime instance.
   * \details De-initializing Socal is not possible until this object is destroyed.
   *          So this member is added here to make sure SomeIpBindnig is deinitialized before Socal.
   */
  ::amsr::generic::SingletonAccess<RuntimeType> socal_runtime_{RuntimeType::getSingletonInstance().GetAccess()};
};

}  // namespace internal
}  // namespace someip_binding
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_INCLUDE_AMSR_SOMEIP_BINDING_INTERNAL_SOCAL_RUNTIME_WRAPPER_H_
