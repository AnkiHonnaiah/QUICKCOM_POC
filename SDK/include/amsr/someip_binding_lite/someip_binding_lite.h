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
/*!        \file   someip_binding_lite/someip_binding_lite.h
 *         \brief  Direct access provider to SOME/IP bindings core (delegator).
 *         \unit   SomeIpBinding::SomeIpBindingLite::SomeIpBindingLite
 *
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_BINDING_LITE_INCLUDE_AMSR_SOMEIP_BINDING_LITE_SOMEIP_BINDING_LITE_H_
#define LIB_SOMEIP_BINDING_LITE_INCLUDE_AMSR_SOMEIP_BINDING_LITE_SOMEIP_BINDING_LITE_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include <utility>

#include "amsr/someip_binding_lite/basic_ipc_address.h"
#include "amsr/someip_binding_lite/data_types.h"
#include "amsr/someip_binding_lite/internal/someip_binding_lite_factory.h"
#include "amsr/someip_binding_lite/internal/someip_binding_lite_impl.h"
#include "ara/core/result.h"

namespace amsr {
namespace someip_binding_lite {

/*!
 * \brief Direct access provider to SOME/IP bindings core.
 * \vpublic
 */
class SomeIpBindingLite final {
 public:
  /*!
   * \brief Type alias for reactor.
   */
  using ReactorType = osabstraction::io::reactor1::Reactor1;

  /*!
   * \brief Create class instance.
   * \vpublic
   *
   * \return \p ara::core::Result with an instance of "SomeIpBindingLite" or an error.
   * \error SomeIpBindingLiteErrc::kRuntimeResourceOutOf If out of system resources.
   * \error SomeIpBindingLiteErrc::kUnknown If an unknown error occurs.
   *
   * \pre -
   * \context     ANY
   * \reentrant   FALSE
   * \threadsafe  FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  static ara::core::Result<SomeIpBindingLite> Create() noexcept;

  /*!
   * \brief Create class instance.
   * \vpublic
   * \param[in] someip_daemon_address Address to use for IPC connection with the daemon.
   *
   * \return \p ara::core::Result with an instance of "SomeIpBindingLite" or an error.
   * \error SomeIpBindingLiteErrc::kRuntimeResourceOutOf If out of system resources.
   * \error SomeIpBindingLiteErrc::kUnknown If an unknown error occurs.
   *
   * \pre -
   * \context     ANY
   * \reentrant   FALSE
   * \threadsafe  FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  static ara::core::Result<SomeIpBindingLite> Create(BasicIpcAddress const someip_daemon_address) noexcept;

  /*!
   * \brief Create class instance.
   * \vpublic
   * \param[in] someip_daemon_address Address to use for IPC connection with the daemon.
   * \param[in] ipc_max_queue_size Maximum queue size to use for IPC connection.
   *
   * \return \p ara::core::Result with an instance of "SomeIpBindingLite" or an error.
   * \error SomeIpBindingLiteErrc::kRuntimeResourceOutOf If out of system resources.
   * \error SomeIpBindingLiteErrc::kUnknown If an unknown error occurs.
   *
   * \pre -
   * \context     ANY
   * \reentrant   FALSE
   * \threadsafe  FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  static ara::core::Result<SomeIpBindingLite> Create(BasicIpcAddress const someip_daemon_address,
                                                     IpcMaxQueueSize const ipc_max_queue_size) noexcept;

  /*!
   * \brief Constructor.
   * \param[in] implementation The SomeIpBindingLite implementation.
   * \steady   FALSE
   * \vprivate Vector product internal API.
   */
  explicit SomeIpBindingLite(std::unique_ptr<internal::SomeIpBindingLiteImpl<>>&& implementation) noexcept;

  /*!
   * \brief Destructor.
   * \steady FALSE
   */
  // VCA_SPC_15_SOMEIPBINDING_STL_TYPE_DESTRUCTOR
  ~SomeIpBindingLite() noexcept = default;

  /*!
   * \brief Move constructor.
   * \details     This will invalidate the owned resource. Any call to an instance of a
   *              moved-from object of this type will trigger an abort.
   * \steady FALSE
   * \vprivate Vector product internal API.
   */
  SomeIpBindingLite(SomeIpBindingLite&&) noexcept = default;

  SomeIpBindingLite(SomeIpBindingLite const&) = delete;
  SomeIpBindingLite& operator=(SomeIpBindingLite const&) = delete;
  SomeIpBindingLite& operator=(SomeIpBindingLite&&) = delete;

  /*!
   * \brief Add service instance to the list of required instances.
   * \vpublic
   * \param[in] service_id Service ID.
   * \param[in] required_instance_id Required instance ID.
   * \param[in] major_interface_version Major interface version.
   * \param[in] minor_interface_version Minor interface version.
   *
   * \return \p ara::core::Result with an instance of "RequiredServiceInstance" or an error.
   * \error No error is returned.
   *
   * \pre This function shall not be invoked on a moved-from instance.
   * \context     ANY
   * \reentrant   FALSE
   * \threadsafe  FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  ara::core::Result<RequiredServiceInstance> CreateRequiredServiceInstance(
      ServiceId const service_id, RequiredInstanceId const& required_instance_id,
      MajorInterfaceVersion const major_interface_version,
      MinorInterfaceVersion const minor_interface_version) const noexcept;

  /*!
   * \brief Start the communication.
   * \vpublic
   *
   * \return \p ara::core::Result with no value or an error.
   * \error SomeIpBindingLiteErrc::kRuntimeThreadCreationFailed If thread creation fails.
   * \error SomeIpBindingLiteErrc::kRuntimeResourceOutOf If out of system resources.
   * \error SomeIpBindingLiteErrc::kSystemConfigurationInvalid If system configuration is invalid.
   * \error SomeIpBindingLiteErrc::kSystemPrivilegesNoAccess If system privileges do not provide resource access.
   * \error SomeIpBindingLiteErrc::kUnknown If an unknown error occurs.
   *
   * \pre SOME/IP daemon shall be running and shall accept connection requests.
   * \pre This function shall not be invoked on a moved-from instance.
   * \context     ANY
   * \reentrant   FALSE
   * \threadsafe  FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  ara::core::Result<void> Start() const noexcept;

  /*!
   * \brief Stop the communication.
   * \vpublic
   *
   * \return \p ara::core::Result with no value or an error.
   * \error SomeIpBindingLiteErrc::kRuntimeThreadJoiningFailed If thread joining fails.
   * \error SomeIpBindingLiteErrc::kUnknown If an unknown error occurs.
   * \pre Start function shall execute without returning an error.
   * \pre This function shall not be invoked on a moved-from instance.
   * \context     ANY
   * \reentrant   FALSE
   * \threadsafe  FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  ara::core::Result<void> Stop() const noexcept;

 private:
  /*!
   * \brief Pointer to the implementation's instance.
   */
  std::unique_ptr<internal::SomeIpBindingLiteImpl<>> implementation_;
};

}  // namespace someip_binding_lite
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_LITE_INCLUDE_AMSR_SOMEIP_BINDING_LITE_SOMEIP_BINDING_LITE_H_
