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
 *
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file  object_provider.h
 *        \brief  Interface for object management instance which tracks object identifiers and objects
 *                and provides the corresponding object for a given identifier.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBSERVER_INCLUDE_CRYPTO_SERVER_OBJECT_PROVIDER_H_
#define LIB_LIBSERVER_INCLUDE_CRYPTO_SERVER_OBJECT_PROVIDER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstdint>
#include <memory>
#include "amsr/sec/ipc/protocol.h"
#include "crypto/server/base_skeleton.h"
#include "object_registration.h"

namespace crypto {
namespace server {

/*!
 * \brief Interface for object management instance which tracks object identifiers and objects and provides the
 * corresponding object for a given identifier
 * \vprivate Component Private
 */
class ObjectProvider {
 public:
  /*!
   * \brief Constructor
   * \param[in] object_registration registration holding all objects as Identifiable
   * \vprivate Component Private
   */
  explicit ObjectProvider(crypto::server::ObjectRegistration& object_registration)
      : registration_(object_registration) {}

  /*!
   * \brief Default constructor
   * \vprivate Component Private
   */
  ObjectProvider() = delete;

  /*!
   * \brief Deleted copy assignment operator
   * \return -
   * \vprivate Component Private
   */
  ObjectProvider& operator=(ObjectProvider const& /*other*/) & = delete;

  /*!
   * \brief Deleted move assignment operator
   * \return -
   * \vprivate Component Private
   */
  ObjectProvider& operator=(ObjectProvider&& /*other*/) & noexcept = delete;

  /*!
   * \brief Deconstructor
   * \vprivate Component Private
   */
  virtual ~ObjectProvider() noexcept = default;

  /*!
   * \brief Get a reference to a certain object for given object ID.
   *
   * \tparam T The type of the object to retrieve.
   * \param[in] proxy_id The object ID used for registration.
   * \param[out] out_value The retrieved object.
   * \context ANY
   * \pre -
   * \error ara::crypto::SecurityErrc::kRpcRuntimeFault If the given object does not match the requested type.
   * \threadsafe FALSE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  template <typename T>
  auto GetObject(amsr::sec::ipc::ProxyId proxy_id, std::shared_ptr<T>& out_value) noexcept -> amsr::core::Result<void> {
    // VECTOR NL AutosarC++17_10-M8.5.2: MD_CRYPTO_AutosarC++17_10-M8.5.2_template_type
    return registration_.GetIdentifiable(proxy_id).AndThen([&out_value](
                                                               std::shared_ptr<crypto::common::Identifiable> obj) {
      // Justification: This dynamic cast is required to convert the stored Identifiable pointers to their correct
      // type. As the requested type can be any derived or parent type of the initially stored one, it is not enough
      // to store the object in a map per type. This dynamic_cast ensures type safety for all requested types and
      // allows handling errors in a defined way.
      out_value = std::dynamic_pointer_cast<T>(obj);
      return (out_value) ? amsr::core::Result<void>::FromValue()
                         : amsr::core::Result<void>::FromError(ara::crypto::SecurityErrc::kRpcRuntimeFault,
                                                               "Requested object does not match the requested type");
    });
  }

  /*!
   * \brief Get a reference to a certain object for given object ID.
   *
   * \tparam T The type of the object to retrieve.
   * \param[in] proxy_id The object ID used for registration.
   * \return The retrieved object
   * \error ara::crypto::SecurityErrc::kRpcUnknownObjectIdentifier if no Reference for this proxy_id has been
   * registered.
   * \threadsafe FALSE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  template <typename T>
  auto GetObjectReference(amsr::sec::ipc::ProxyId proxy_id) noexcept -> amsr::core::Result<T&> {
    amsr::core::Result<T&> result{ara::crypto::SecurityErrc::kRpcUnknownObjectIdentifier};
    // VECTOR NL AutosarC++17_10-M8.5.2: MD_CRYPTO_AutosarC++17_10-M8.5.2_template_type
    amsr::core::Result<crypto::common::Identifiable&> identifiable_result{
        registration_.GetIdentifiableReference(proxy_id)};
    if (identifiable_result.HasValue()) {
      // VECTOR NC AutosarC++17_10-A5.2.1: MD_CRYPTO_AutosarC++17_10-A5.2.1_dynamic_cast
      // VECTOR NC AutosarC++17_10-M5.2.3: MD_CRYPTO_AutosarC++17_10-M5.2.3_down_cast
      // VECTOR NC VectorC++-V5.2.1: MD_CRYPTO_VectorC++-V5.2.1_dynamic_cast
      // VECTOR NC VectorC++-V5.2.3: MD_CRYPTO_VectorC++-V5.2.3_dynamic_downcast
      // Justification: This dynamic cast is required to convert the stored Identifiable pointers to their correct type.
      // As the requested type can be any derived or parent type of the initially stored one, it is not enough to store
      // the object in a map per type. This dynamic_cast ensures type safety for all requested types and allows to
      // handle errors in a defined way by catching the exception.
      result.EmplaceValue(dynamic_cast<T&>(identifiable_result.Value()));
    }
    return result;
  }

 protected:
  /*!
   * \brief Default copy constructor
   * \vprivate Component Private
   */
  ObjectProvider(ObjectProvider const&) = default;

  /*!
   * \brief Default move constructor
   * \vprivate Component Private
   */
  ObjectProvider(ObjectProvider&&) noexcept = default;

 private:
  crypto::server::ObjectRegistration& registration_; /*!< Registration holding the Identifiable objects */
};

}  // namespace server
}  // namespace crypto

#endif  // LIB_LIBSERVER_INCLUDE_CRYPTO_SERVER_OBJECT_PROVIDER_H_
