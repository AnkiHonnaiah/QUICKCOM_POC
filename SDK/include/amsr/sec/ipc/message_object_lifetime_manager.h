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
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file  message_object_lifetime_manager.h
 *        \brief  Manager for heap objects created during message deserialization.
 *
 *      \details  As messages contains references and pointer values that are created during deserialization, this
 *                object tracks the objects and is responsible for release of these objects on message destruction.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBCOMMON_INCLUDE_AMSR_SEC_IPC_MESSAGE_OBJECT_LIFETIME_MANAGER_H_
#define LIB_LIBCOMMON_INCLUDE_AMSR_SEC_IPC_MESSAGE_OBJECT_LIFETIME_MANAGER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include <vector>

#include "amsr/core/string.h"
#include "ara/crypto/common/crypto_object_uid.h"
#include "ara/crypto/common/guid.h"
#include "ara/crypto/common/mem_region.h"
#include "ara/crypto/cryp/crypto_provider.h"
#include "ara/crypto/keys/key_slot_content_props.h"
#include "ara/crypto/keys/key_slot_prototype_props.h"
#include "ara/crypto/keys/user_permissions.h"
#include "ara/crypto/x509/certificate.h"
#include "ara/crypto/x509/x509_provider.h"

namespace amsr {
namespace sec {
namespace ipc {

// VECTOR NC Metric-OO.RFC: MD_CRYPTO_Metric-OO.RFC
/*!
 * \brief Manager for heap objects created during message deserialization. Will be flushed when the message is deleted
 *        and delete all created objects
 * \details  As the message contains references and pointer values, that are created during deserialization,
 * this object tracks the objects and is responsible for releasing these objects on message destruction.
 * \vprivate Component Private
 */
class MessageObjectLifetimeManager {
 public:
  /*!
   * \brief Default constructor
   * \vprivate Component Private
   */
  MessageObjectLifetimeManager() = default;

  // VECTOR NC AutosarC++17_10-A12.8.6: MD_CRYPTO_AutosarC++17_10-A12.8.6_only_virtual_for_mock
  /*!
   * \brief Default copy constructor
   * \vprivate Component Private
   */
  MessageObjectLifetimeManager(MessageObjectLifetimeManager const& /*other*/) = default;

  // VECTOR NC AutosarC++17_10-A12.8.6: MD_CRYPTO_AutosarC++17_10-A12.8.6_only_virtual_for_mock
  /*!
   * \brief Default move constructor
   * \vprivate Component Private
   */
  MessageObjectLifetimeManager(MessageObjectLifetimeManager&& /*other*/) noexcept = default;

  // VECTOR NC AutosarC++17_10-A12.8.6: MD_CRYPTO_AutosarC++17_10-A12.8.6_only_virtual_for_mock
  /*!
   * \brief Default copy assignment operator
   * \return reference to this
   * \vprivate Component Private
   */
  MessageObjectLifetimeManager& operator=(MessageObjectLifetimeManager const& /*other*/) & = default;

  // VECTOR NC AutosarC++17_10-A12.8.6: MD_CRYPTO_AutosarC++17_10-A12.8.6_only_virtual_for_mock
  /*!
   * \brief Default move assignment operator
   * \return reference to this
   * \vprivate Component Private
   */
  MessageObjectLifetimeManager& operator=(MessageObjectLifetimeManager&& /*other*/) & noexcept = default;

  /*!
   * \brief Custom destructor to prevent destruction of unique pointer objects
   * \vprivate Component Private
   */
  virtual ~MessageObjectLifetimeManager() noexcept;

  /*!
   * \brief Method to register an object in this lifetime manager.
   * \details The object will be held in an internal map and will be destroyed when this manager is destroyed.
   * \param[in] object Object to be registered.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  virtual void RegisterObjectForRelease(std::unique_ptr<ara::crypto::Uuid> object);
  /*!
   * \copydoc MessageObjectLifetimeManager::RegisterObjectForRelease(std::unique_ptr<ara::crypto::Uuid>)
   * \vprivate Component Private
   */
  virtual void RegisterObjectForRelease(std::unique_ptr<ara::crypto::CryptoObjectUid> object);
  /*!
   * \copydoc MessageObjectLifetimeManager::RegisterObjectForRelease(std::unique_ptr<ara::crypto::Uuid>)
   * \vprivate Component Private
   */
  virtual void RegisterObjectForRelease(std::unique_ptr<ara::crypto::keys::KeySlotPrototypeProps> object);
  /*!
   * \copydoc MessageObjectLifetimeManager::RegisterObjectForRelease(std::unique_ptr<ara::crypto::Uuid>)
   * \vprivate Component Private
   */
  virtual void RegisterObjectForRelease(std::unique_ptr<ara::crypto::keys::KeySlotContentProps> object);
  /*!
   * \copydoc MessageObjectLifetimeManager::RegisterObjectForRelease(std::unique_ptr<ara::crypto::Uuid>)
   * \vprivate Component Private
   */
  virtual void RegisterObjectForRelease(std::unique_ptr<ara::core::Vector<ara::crypto::keys::UserPermissions>> object);
  /*!
   * \copydoc MessageObjectLifetimeManager::RegisterObjectForRelease(std::unique_ptr<ara::crypto::Uuid>)
   * \vprivate Component Private
   */
  virtual void RegisterObjectForRelease(std::unique_ptr<ara::crypto::ReadOnlyMemRegion> object);
  /*!
   * \copydoc MessageObjectLifetimeManager::RegisterObjectForRelease(std::unique_ptr<ara::crypto::Uuid>)
   * \vprivate Component Private
   */
  virtual void RegisterObjectForRelease(std::unique_ptr<ara::crypto::WritableMemRegion> object);
  /*!
   * \copydoc MessageObjectLifetimeManager::RegisterObjectForRelease(std::unique_ptr<ara::crypto::Uuid>)
   * \vprivate Component Private
   */
  virtual void RegisterObjectForRelease(std::unique_ptr<amsr::core::String> object);
  /*!
   * \copydoc MessageObjectLifetimeManager::RegisterObjectForRelease(std::unique_ptr<ara::crypto::Uuid>)
   * \vprivate Component Private
   */
  virtual void RegisterObjectForRelease(
      std::unique_ptr<ara::crypto::cryp::CryptoProvider::ContainedObjectsList> object);

  /*!
   * \copydoc MessageObjectLifetimeManager::RegisterObjectForRelease(std::unique_ptr<ara::crypto::Uuid>)
   * \vprivate Component Private
   */
  virtual void RegisterObjectForRelease(
      std::shared_ptr<ara::core::Vector<std::unique_ptr<ara::crypto::x509::Certificate, ara::crypto::CustomDeleter>>>
          object);

  /*!
   * \copydoc MessageObjectLifetimeManager::RegisterObjectForRelease(std::unique_ptr<ara::crypto::Uuid>)
   * \vprivate Component Private
   */
  virtual void RegisterObjectForRelease(std::shared_ptr<ara::core::Vector<ara::crypto::ReadOnlyMemRegion>> object);

  /*!
   * \copydoc MessageObjectLifetimeManager::RegisterObjectForRelease(std::unique_ptr<ara::crypto::Uuid>)
   * \vprivate Component Private
   */
  virtual void RegisterObjectForRelease(std::shared_ptr<ara::crypto::x509::X509Provider::StorageIndex> object);

 private:
  ara::core::Vector<std::shared_ptr<ara::crypto::Uuid>> uuids_; /*!< List of elements to be cleared */
  ara::core::Vector<std::shared_ptr<ara::crypto::ReadOnlyMemRegion>>
      read_mem_regions_; /*!< List of elements to be cleared */
  ara::core::Vector<std::shared_ptr<ara::crypto::WritableMemRegion>>
      write_mem_regions_;                                                   /*!< List of elements to be cleared */
  ara::core::Vector<std::shared_ptr<amsr::core::String>> strings_;          /*!< List of elements to be cleared */
  ara::core::Vector<std::shared_ptr<ara::crypto::CryptoObjectUid>> couids_; /*!< List of elements to be cleared */
  ara::core::Vector<std::shared_ptr<ara::crypto::keys::KeySlotPrototypeProps>>
      key_slot_proto_props_; /*!< List of elements to be cleared */
  ara::core::Vector<std::shared_ptr<ara::crypto::keys::KeySlotContentProps>>
      key_slot_content_props_; /*!< List of elements to be cleared */
  ara::core::Vector<std::shared_ptr<ara::core::Vector<ara::crypto::keys::UserPermissions>>>
      vector_user_permissions_; /*!< List of elements to be cleared */
  ara::core::Vector<std::shared_ptr<ara::crypto::cryp::CryptoProvider::ContainedObjectsList>>
      object_lists_; /*!< List of elements to be cleared */
  ara::core::Vector<
      std::shared_ptr<ara::core::Vector<std::unique_ptr<ara::crypto::x509::Certificate, ara::crypto::CustomDeleter>>>>
      vector_certs_; /*!< List of elements to be cleared */
  ara::core::Vector<std::shared_ptr<ara::core::Vector<ara::crypto::ReadOnlyMemRegion>>>
      vector_mem_regions_; /*!< List of elements to be cleared */
  ara::core::Vector<std::shared_ptr<ara::crypto::x509::X509Provider::StorageIndex>>
      storage_indices_; /*!< List of elements to be cleared */
};

}  // namespace ipc
}  // namespace sec
}  // namespace amsr

#endif  // LIB_LIBCOMMON_INCLUDE_AMSR_SEC_IPC_MESSAGE_OBJECT_LIFETIME_MANAGER_H_
