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
/*!        \file  object_registration.h
 *        \brief  Object registration holding all registered objects as Identifiables.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBSERVER_INCLUDE_CRYPTO_SERVER_OBJECT_REGISTRATION_H_
#define LIB_LIBSERVER_INCLUDE_CRYPTO_SERVER_OBJECT_REGISTRATION_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <map>
#include <memory>
#include <utility>

#include "amsr/sec/ipc/protocol.h"
#include "crypto/common/identifiable.h"
#include "crypto/common/util/logger.h"
#include "crypto/server/base_skeleton.h"

namespace crypto {
namespace server {

// VECTOR NC AutosarC++17_10-M7.3.6: MD_CRYPTO_AutosarC++17_10-M7.3.6_sv
/*! Introduce string literal for StringView creation */
using vac::container::literals::operator"" _sv;

/*!
 * \brief Interface for Registration of different object types and skeletons
 * \vprivate Component Private
 */
class ObjectRegistration {
 public:
  /*!
   * \brief Default constructor
   * \vprivate Component Private
   */
  ObjectRegistration() = default;

  /*!
   * \brief Deleted copy assignment operator
   * \return -
   * \vprivate Component Private
   */
  ObjectRegistration& operator=(ObjectRegistration const& /*other*/) & = delete;

  /*!
   * \brief Deleted move assignment operator
   * \return -
   * \vprivate Component Private
   */
  ObjectRegistration& operator=(ObjectRegistration&& /*other*/) & noexcept = delete;

  /*!
   * \brief Destructor
   * \vprivate Component Private
   */
  virtual ~ObjectRegistration() noexcept = default;

  /*!
   * \brief Lookup method to retrieve single object, managed by this pool
   * \param[in] proxy_id ProxyId of the requested object
   * \return shared_ptr of the requested object
   * \context ANY
   * \pre -
   * \error ara::crypto::SecurityErrc::kRpcUnknownObjectIdentifier If no object for given proxy_id was found
   * \threadsafe FALSE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  virtual auto GetIdentifiable(amsr::sec::ipc::ProxyId proxy_id) noexcept
      -> amsr::core::Result<std::shared_ptr<common::Identifiable>>;

  /*!
   * \brief Lookup method to retrieve single object, managed by this pool
   * \param[in] proxy_id ProxyId of the requested object
   * \return Reference to the requested object.
   * \context ANY
   * \pre -
   * \error ara::crypto::SecurityErrc::kRpcUnknownObjectIdentifier If no object for given proxy_id was found
   * \threadsafe FALSE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  virtual auto GetIdentifiableReference(amsr::sec::ipc::ProxyId proxy_id) noexcept
      -> amsr::core::Result<common::Identifiable&>;

  /*!
   * \brief Registration function for object to be registered
   * \param[in] proxy_id   Id under which the new object will be registered
   * \param[in] registrant  Object to be registered
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  virtual void RegisterInstance(amsr::sec::ipc::ProxyId proxy_id,
                                std::shared_ptr<common::Identifiable const> const& registrant) noexcept;

  /*!
   * \brief Registration function for object reference to be registered
   * \details Registering an object by reference does not affect the life time of the object, but it can be retrieved
   * using GetObjectReference().
   * \param[in] proxy_id   Id under which the new object will be registered
   * \param[in] registrant  Object to be registered
   * \vprivate Component Private
   */
  virtual void RegisterInstance(amsr::sec::ipc::ProxyId proxy_id, common::Identifiable const& registrant) noexcept;

  /*!
   * \brief Unregistration of object, this might cause deletion of the object depending on its reference counter
   * \param[in] registrant Object to be unregistered
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  virtual void UnregisterInstance(common::Identifiable const& registrant) noexcept;

 protected:
  /*!
   * \brief Default copy constructor
   * \vprivate Component Private
   */
  ObjectRegistration(ObjectRegistration const& /*other*/) = default;

  /*!
   * \brief Default move constructor
   * \vprivate Component Private
   */
  ObjectRegistration(ObjectRegistration&& /*other*/) noexcept = default;

 private:
  using ObjectMapType =
      std::map<amsr::sec::ipc::ProxyId, std::shared_ptr<common::Identifiable>>; /*!< Type for object maps. */
  using ObjectReferenceMapType =
      std::map<amsr::sec::ipc::ProxyId,
               std::reference_wrapper<common::Identifiable>>; /*!< Type for object reference maps. */

  common::util::Logger logger_{common::util::kServerLoggingContext, ""_sv};            /*!< Logging instance */
  std::map<amsr::sec::ipc::ProxyId, std::shared_ptr<common::Identifiable>> objects_{}; /*!< Instance pool */
  std::map<amsr::sec::ipc::ProxyId, std::reference_wrapper<common::Identifiable>>
      objects_by_reference_{}; /*!< Instance pool */
};

}  // namespace server
}  // namespace crypto

#endif  // LIB_LIBSERVER_INCLUDE_CRYPTO_SERVER_OBJECT_REGISTRATION_H_
