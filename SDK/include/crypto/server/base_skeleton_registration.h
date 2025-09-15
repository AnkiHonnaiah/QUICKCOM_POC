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
/*!        \file  base_skeleton_registration.h
 *        \brief  Registration for BaseSkeletons.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBSERVER_INCLUDE_CRYPTO_SERVER_BASE_SKELETON_REGISTRATION_H_
#define LIB_LIBSERVER_INCLUDE_CRYPTO_SERVER_BASE_SKELETON_REGISTRATION_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <map>
#include <memory>
#include "amsr/sec/ipc/protocol.h"
#include "base_skeleton.h"
#include "crypto/common/util/logger.h"

namespace crypto {
namespace server {

// VECTOR NC AutosarC++17_10-M7.3.6: MD_CRYPTO_AutosarC++17_10-M7.3.6_sv
/*! Introduce string literal for StringView creation */
using vac::container::literals::operator"" _sv;

/*!
 * \brief Interface for BaseSkeleton handling
 * \vprivate Component Private
 * \trace DSGN-Crypto-ProcessIsolation, DSGN-Crypto-Skeleton_Proxy
 */
class BaseSkeletonRegistration {
 public:
  /*!
   * \brief Default constructor
   * \vprivate Component Private
   */
  BaseSkeletonRegistration() = default;

  /*!
   * \brief Default copy constructor
   * \vprivate Component Private
   */
  BaseSkeletonRegistration(BaseSkeletonRegistration const& /*other*/) = default;

  /*!
   * \brief Default move constructor
   * \vprivate Component Private
   */
  BaseSkeletonRegistration(BaseSkeletonRegistration&& /*other*/) noexcept = default;

  /*!
   * \brief Deleted copy assignment operator
   * \return -
   * \vprivate Component Private
   */
  BaseSkeletonRegistration& operator=(BaseSkeletonRegistration const& /*other*/) & = delete;

  /*!
   * \brief Deleted move assignment operator
   * \return -
   * \vprivate Component Private
   */
  BaseSkeletonRegistration& operator=(BaseSkeletonRegistration&& /*other*/) & noexcept = delete;

  /*!
   * \brief Destructor
   * \vprivate Component Private
   */
  virtual ~BaseSkeletonRegistration() noexcept = default;

  /*!
   * \brief Returns a reference to a base skeleton for its object ID.
   *
   * \param[in] proxy_id The object ID used for registration.
   * \return Reference to the underlying base skeleton.
   * \context ANY
   * \pre -
   * \error ara::crypto::SecurityErrc::kRpcUnknownObjectIdentifier If no corresponding base skeleton was found.
   * \threadsafe FALSE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto GetBaseSkeleton(amsr::sec::ipc::ProxyId proxy_id) noexcept -> amsr::core::Result<std::shared_ptr<BaseSkeleton>> {
    // VECTOR NC AutosarC++17_10-A8.5.2: MD_CRYPTO_AutosarC++17_10-A8.5.2_auto_from_ret_type
    auto result = amsr::core::Result<std::shared_ptr<BaseSkeleton>>::FromError(
        ara::crypto::SecurityErrc::kRpcUnknownObjectIdentifier);
    ObjectMapType::iterator const object_it{objects_.find(proxy_id)};
    if (object_it != objects_.end()) {
      result.EmplaceValue(object_it->second);
    } else {
      logger_.LogError(AMSR_FILE, __LINE__) << "No instance for given ID " << proxy_id;
    }
    return result;
  }

  /*!
   * \brief Registers a base skeleton under given object ID.
   *
   * \param[in] proxy_id The object ID used for registration.
   * \param[in] registrant The base skeleton to register.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  void RegisterBaseSkeleton(amsr::sec::ipc::ProxyId proxy_id,
                            std::shared_ptr<BaseSkeleton> const& registrant) noexcept {
    logger_.LogVerbose(AMSR_FILE, __LINE__) << "Registered skeleton with ID " << proxy_id;
    std::shared_ptr<BaseSkeleton> const ptr{std::const_pointer_cast<BaseSkeleton>(registrant)};
    objects_[proxy_id] = ptr;
  }

  /*!
   * \brief Unregisters a base skeleton with given object ID.
   *
   * \param[in] proxy_id The object UD used for registration.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  void UnregisterBaseSkeleton(amsr::sec::ipc::ProxyId proxy_id) noexcept {
    ObjectMapType::iterator const object_it{objects_.find(proxy_id)};
    if (object_it != objects_.end()) {
      static_cast<void>(objects_.erase(object_it));
      logger_.LogVerbose(AMSR_FILE, __LINE__) << "Unregistered skeleton with ID " << proxy_id;
    } else {
      logger_.LogWarn(AMSR_FILE, __LINE__)
          << "Unregistration of skeleton with ID " << proxy_id << " not possible: ID not found.";
    }
  }

 private:
  using ObjectMapType = std::map<amsr::sec::ipc::ProxyId, std::shared_ptr<BaseSkeleton>>; /*!< Type for object maps. */
  crypto::common::util::Logger logger_{common::util::kServerLoggingContext,
                                       "BaseSkeletonRegistration"_sv};         /*!< Logging instance */
  std::map<amsr::sec::ipc::ProxyId, std::shared_ptr<BaseSkeleton>> objects_{}; /*!< Instance pool */
};

}  // namespace server
}  // namespace crypto

#endif  // LIB_LIBSERVER_INCLUDE_CRYPTO_SERVER_BASE_SKELETON_REGISTRATION_H_
