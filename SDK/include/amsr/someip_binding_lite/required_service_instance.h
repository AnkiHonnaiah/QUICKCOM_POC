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
/*!        \file   someip_binding_lite/required_service_instance.h
 *         \brief  Required service instance (delegator).
 *         \unit   SomeIpBinding::SomeIpBindingLite::Client::SomeIpBindingLiteRequiredServiceInstance
 *
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_BINDING_LITE_INCLUDE_AMSR_SOMEIP_BINDING_LITE_REQUIRED_SERVICE_INSTANCE_H_
#define LIB_SOMEIP_BINDING_LITE_INCLUDE_AMSR_SOMEIP_BINDING_LITE_REQUIRED_SERVICE_INSTANCE_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include <utility>

#include "amsr/someip_binding_lite/internal/required_service_instance_impl.h"
#include "ara/core/result.h"

namespace amsr {
namespace someip_binding_lite {

/*!
 * \brief Required service instance.
 * \details Receives notifications when service instances are found.
 *          Allows creation of clients based on discovered service instance IDs.
 * \vpublic
 */
class RequiredServiceInstance final {
 public:
  /*!
   * \brief Constructor.
   * \param[in] implementation The required service instance implementation
   * \vprivate Vector product internal API.
   *
   * \steady      FALSE
   */
  explicit RequiredServiceInstance(std::shared_ptr<internal::RequiredServiceInstanceImpl> implementation) noexcept;

  /*!
   * \brief Destructor.
   * \steady FALSE
   */
  // VCA_SPC_15_SOMEIPBINDING_STL_TYPE_DESTRUCTOR
  ~RequiredServiceInstance() noexcept;

  /*!
   * \brief Move constructor.
   * \details     This will invalidate the owned resource. Any call to an instance of a
   *              moved-from object of this type will trigger an abort.
   * \steady   FALSE
   * \vprivate Vector product internal API.
   */
  RequiredServiceInstance(RequiredServiceInstance&&) noexcept = default;

  RequiredServiceInstance(RequiredServiceInstance const&) = delete;
  RequiredServiceInstance& operator=(RequiredServiceInstance const&) = delete;
  RequiredServiceInstance& operator=(RequiredServiceInstance&&) = delete;

  /*!
   * \brief Get the list of found services.
   * \vpublic
   * \return A list with provided instance IDs of found services.
   * \pre This function shall not be invoked on a moved-from instance.
   * \context     ANY
   * \reentrant   FALSE
   * \threadsafe  FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  ara::core::Vector<ProvidedInstanceId> GetFoundServices() const noexcept;

  /*!
   * \brief Create a client based on a provided instance ID.
   * \vpublic
   * \param[in] instance_id Provided instance ID.
   *
   * \return \p ara::core::Result with an instance of "SomeIpClient" or an error.
   * \error SomeIpBindingLiteErrc::kSystemPrivilegesNoAccess If system privileges do not provide resource access.
   * \error SomeIpBindingLiteErrc::kRuntimeBufferDeserializationError If buffer deserialization fails.
   * \error SomeIpBindingLiteErrc::kRuntimeConnectionLost If connection is lost.
   * \error SomeIpBindingLiteErrc::kRuntimeBufferInvalid If an invalid buffer is encountered.
   * \error SomeIpBindingLiteErrc::kUnknown If an unknown error occurs.
   *
   * \pre \p instance_id shall be the same as the one used to create this required service instance
   *      except when the ID represents all IDs.
   * \pre This function shall not be invoked on a moved-from instance.
   * \context     ANY
   * \reentrant   FALSE
   * \threadsafe  FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  ara::core::Result<SomeIpClient> CreateClient(ProvidedInstanceId const instance_id) const noexcept;

 private:
  /*!
   * \brief Pointer to the implementation's instance.
   */
  std::shared_ptr<internal::RequiredServiceInstanceImpl> implementation_;
};

}  // namespace someip_binding_lite
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_LITE_INCLUDE_AMSR_SOMEIP_BINDING_LITE_REQUIRED_SERVICE_INSTANCE_H_
