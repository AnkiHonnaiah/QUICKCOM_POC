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
/*!        \file   someip_binding_lite/internal/someip_binding_lite_factory.h
 *         \brief
 *         \unit   SomeIpBinding::SomeIpBindingLite::SomeIpBindingLiteFactory
 *
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_BINDING_LITE_INCLUDE_AMSR_SOMEIP_BINDING_LITE_INTERNAL_SOMEIP_BINDING_LITE_FACTORY_H_
#define LIB_SOMEIP_BINDING_LITE_INCLUDE_AMSR_SOMEIP_BINDING_LITE_INTERNAL_SOMEIP_BINDING_LITE_FACTORY_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include <utility>

#include "amsr/core/result.h"
#include "amsr/someip_binding_core/internal/dummy_polling_wrapper.h"
#include "amsr/someip_binding_core/internal/logging/ara_com_logger_singleton.h"
#include "amsr/someip_binding_core/internal/someip_daemon_client_wrapper.h"
#include "amsr/someip_binding_lite/basic_ipc_address.h"
#include "amsr/someip_binding_lite/data_types.h"
#include "amsr/someip_binding_lite/internal/constants.h"
#include "amsr/someip_binding_lite/internal/someip_binding_lite_impl.h"
#include "amsr/someip_binding_lite/someip_binding_lite.h"

namespace amsr {
namespace someip_binding_lite {
namespace internal {

/*!
 * \brief SomeIpBindingLite factory.
 *
 * \tparam ReactorType Type of the reactor.
 * \tparam SomeIpBindingLite Direct access provider to SOME/IP bindings core.
 */
template <typename ReactorType, typename SomeIpBindingLite>
class SomeIpBindingLiteFactory final {
 public:
  /*!
   * \brief Type-alias for the DummyPollingWrapper.
   */
  using DummyPollingWrapper = ::amsr::someip_binding_core::internal::DummyPollingWrapper;
  /*!
   * \brief Destructor.
   * \steady FALSE
   */
  // VCA_SPC_15_SOMEIPBINDING_STL_TYPE_DESTRUCTOR
  ~SomeIpBindingLiteFactory() noexcept = default;

  SomeIpBindingLiteFactory() noexcept = delete;
  SomeIpBindingLiteFactory(SomeIpBindingLiteFactory&&) noexcept = delete;
  SomeIpBindingLiteFactory(SomeIpBindingLiteFactory const&) = delete;
  SomeIpBindingLiteFactory& operator=(SomeIpBindingLiteFactory const&) = delete;
  SomeIpBindingLiteFactory& operator=(SomeIpBindingLiteFactory&&) = delete;

  /*!
   * \brief Create class instance.
   *
   * \param[in] someip_daemon_address Address to use for IPC connection with the daemon.
   * \param[in] ipc_max_queue_size Maximum queue size to use for IPC connection.
   *
   * \return \p amsr::core::Result with an instance of "SomeIpBindingLite" or an error.
   * \error SomeIpBindingLiteErrc::kRuntimeResourceOutOf If out of system resources.
   * \error SomeIpBindingLiteErrc::kUnknown If an unknown error occurs.
   *
   * \pre -
   * \context     ANY
   * \reentrant   FALSE
   * \threadsafe  FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \internal
   * - If reactor's construction passes,
   *   - Create instances of needed classes (dependencies).
   *   - Create instance of "SomeIpBindingLiteImpl" and inject the dependencies.
   * - Else
   *   - Return the error.
   * \endinternal
   */
  template <typename SomeIpDaemonClient = ::amsr::someip_daemon_client::internal::SomeIpDaemonClient<
                ::amsr::someip_daemon_client::internal::SomeIpDaemonClientDefaultTemplateConfiguration>>
  static auto Create(BasicIpcAddress const someip_daemon_address, IpcMaxQueueSize const ipc_max_queue_size) noexcept
      -> amsr::core::Result<SomeIpBindingLite> {
    using ReactorConstructionTokenType = osabstraction::io::reactor1::Reactor1::ConstructionToken;
    using SomeIpDaemonClientConfigModelType = amsr::someip_daemon_client::internal::SomeIpDaemonClientConfigModel;

    // Initialize logging singleton.
    if (!::amsr::someip_binding_core::internal::logging::AraComLoggerSingleton::GetInstance().IsInitialized()) {
      ::amsr::someip_binding_core::internal::logging::AraComLoggerSingleton::GetInstance().Create();
    }

    static constexpr std::uint16_t const kDefaultReactorCallbacks{1024u};

    amsr::core::Result<SomeIpBindingLite> result{SomeIpBindingLiteErrc::kUnknown};

    amsr::core::Result<ReactorConstructionTokenType> result_reactor_token{
        ReactorType::Preconstruct(kDefaultReactorCallbacks)};

    if (result_reactor_token.HasValue()) {
      // VECTOR NC AutosarC++17_10-A18.5.8: MD_SOMEIPBINDING_AutosarC++17_10-A18.5.8_Local_object_in_heap
      // VCA_SPC_15_SOMEIPBINDING_STL_TYPE_FUNCTION
      std::shared_ptr<DummyPollingWrapper> poll_wrapper{std::make_shared<DummyPollingWrapper>()};
      // VECTOR NC AutosarC++17_10-A18.5.8: MD_SOMEIPBINDING_AutosarC++17_10-A18.5.8_Local_object_in_heap
      std::unique_ptr<internal::ReactorInterfaceType> uptr_reactor{
          // VCA_SPC_15_SOMEIPBINDING_STL_TYPE_FUNCTION
          std::make_unique<internal::ReactorType>(std::move(result_reactor_token.Value()))};

      // VECTOR NC AutosarC++17_10-A18.5.8: MD_SOMEIPBINDING_AutosarC++17_10-A18.5.8_Local_object_in_heap
      // VCA_SPC_15_SOMEIPBINDING_OTHER_COMPONENT_TYPE_FUNCTION
      std::unique_ptr<SomeIpDaemonClient> someip_daemon_client{std::make_unique<SomeIpDaemonClient>(
          SomeIpDaemonClientConfigModelType{someip_daemon_address.domain, someip_daemon_address.port,
                                            ipc_max_queue_size},
          // VECTOR NC VectorC++-V5.2.3: MD_SOMEIPBINDING_VectorC++-V5.2.3_cast_from_base_to_derived
          // VECTOR NC AutosarC++17_10-M5.2.3: MD_SOMEIPBINDING_AutosarC++17_10-M5.2.3_cast_from_base_to_derived
          static_cast<internal::ReactorType&>(*uptr_reactor))};

      // VECTOR NC AutosarC++17_10-A18.5.8: MD_SOMEIPBINDING_AutosarC++17_10-A18.5.8_Local_object_in_heap
      std::unique_ptr<amsr::someip_binding_core::internal::SomeIpDaemonClientWrapper<SomeIpDaemonClient>>
          someip_daemon_client_wrapper{
              std::make_unique<amsr::someip_binding_core::internal::SomeIpDaemonClientWrapper<SomeIpDaemonClient>>(
                  std::move(someip_daemon_client))};

      // VECTOR NC AutosarC++17_10-A18.5.8: MD_SOMEIPBINDING_AutosarC++17_10-A18.5.8_Local_object_in_heap
      std::shared_ptr<SomeIpBindingCoreType> someip_binding_core{
          // VCA_SOMEIPBINDING_TRIVIAL_FUNCTION_CONTRACT
          std::make_shared<SomeIpBindingCoreType>(std::move(someip_daemon_client_wrapper),
                                                  // VCA_SOMEIPBINDING_RUNTIME_OBJECT_LIFECYCLE
                                                  poll_wrapper, false)};

      // Create an implementation instance and pass the ownership of the created instances to the implementation.
      // VECTOR NC AutosarC++17_10-A18.5.8: MD_SOMEIPBINDING_AutosarC++17_10-A18.5.8_Local_object_in_heap
      std::unique_ptr<internal::SomeIpBindingLiteImpl<>> uptr_implementation{
          // VCA_SPC_15_SOMEIPBINDING_STL_TYPE_FUNCTION
          std::make_unique<internal::SomeIpBindingLiteImpl<>>(std::move(uptr_reactor), std::move(someip_binding_core),
                                                              someip_daemon_address)};

      // Create a wrapper class instance and move the implementation instance into it.
      result.EmplaceValue(std::move(uptr_implementation));
      // VCA_SPC_15_SOMEIPBINDING_STL_TYPE_DESTRUCTOR
    } else {
      result.EmplaceError(TranslateErrorCodeOsabErrc(result_reactor_token.Error()));
    }

    return result;
  }
};

}  // namespace internal
}  // namespace someip_binding_lite
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_LITE_INCLUDE_AMSR_SOMEIP_BINDING_LITE_INTERNAL_SOMEIP_BINDING_LITE_FACTORY_H_
