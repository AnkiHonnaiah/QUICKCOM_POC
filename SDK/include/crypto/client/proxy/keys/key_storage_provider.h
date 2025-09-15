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
/*!        \file  proxy/keys/key_storage_provider.h
 *        \brief  Proxy implementation for ara::crypto::keys::KeyStorageProvider.
 *
 *      \details  Client side implementation of the ara::crypto::keys::KeyStorageProvider interface that
 *                transparently communicates with a server side implementation that performs the real cryptography.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBCLIENT_INCLUDE_CRYPTO_CLIENT_PROXY_KEYS_KEY_STORAGE_PROVIDER_H_
#define LIB_LIBCLIENT_INCLUDE_CRYPTO_CLIENT_PROXY_KEYS_KEY_STORAGE_PROVIDER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include <utility>
#include <vector>

#include "ara/crypto/keys/key_storage_provider.h"
#include "crypto/client/base_proxy.h"

namespace crypto {
namespace client {
namespace proxy {
namespace keys {

// VECTOR NC Metric-OO.WMC.One: MD_CRYPTO_Metric-OO.WMC.One
// VECTOR NC AutosarC++17_10-A10.1.1: MD_CRYPTO_AutosarC++17_10-A10.1.1_multiple_inheritance_for_proxies
/*!
 * \brief Proxy implementation for ara::crypto::keys::KeyStorageProvider
 * \vprivate Component Private
 * \trace DSGN-Crypto-ProcessIsolation, DSGN-Crypto-Skeleton_Proxy
 */
class KeyStorageProvider final : public BaseProxy, public ara::crypto::keys::KeyStorageProvider {
 public:
  /*!
   * \brief Constructor
   * \param[in] transporter Transporter used for server/client communication
   * \vprivate Component Private
   */
  explicit KeyStorageProvider(amsr::sec::ipc::Transporter& transporter)
      : BaseProxy(transporter, message_builder_, message_serializer_, message_deserializer_),
        ara::crypto::keys::KeyStorageProvider(),
        message_builder_{},
        message_serializer_{},
        message_deserializer_{},
        logger_{common::util::kClientLoggingContext, ""_sv} {}

  /*!
   * \brief Destructor
   * \vprivate Component Private
   */
  ~KeyStorageProvider() noexcept final = default;

  /*!
   * \brief Deleted copy constructor
   * \vprivate Component Private
   */
  KeyStorageProvider(KeyStorageProvider const& /*other*/) = delete;

  /*!
   * \brief Deleted move constructor
   * \vprivate Component Private
   */
  KeyStorageProvider(KeyStorageProvider&& /*other*/) noexcept = delete;

  // VECTOR NC AutosarC++17_10-A10.2.1: MD_CRYPTO_AutosarC++17_10-A10.2.1_hiding_functions
  /*!
   * \brief Deleted copy assignment operator
   * \return -
   * \vprivate Component Private
   */
  KeyStorageProvider& operator=(KeyStorageProvider const& /*other*/) = delete;

  // VECTOR NC AutosarC++17_10-A10.2.1: MD_CRYPTO_AutosarC++17_10-A10.2.1_hiding_functions
  /*!
   * \brief Deleted move assignment operator
   * \return -
   * \vprivate Component Private
   */
  KeyStorageProvider& operator=(KeyStorageProvider&& /*other*/) = delete;

  /*!
   * \brief Find a slot number by the Logic (persistent) Slot UID.
   * \param[in] slot_uid  Logic Slot UID for search
   * \param[out] provider_uid  an optional pointer to UID of default Crypto Provider assigned for servicing this slot
   * \return Number of found slot.
   * \pre -
   * \threadsafe TRUE
   * \error ara::crypto::keys::kInvalidSlot No slot found with the given UID.
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ara::crypto::keys::SlotNumber FindSlot(SlotUid const& slot_uid,
                                         ara::crypto::CryptoProviderUid* provider_uid) const noexcept final;

  /*!
   * \brief Find a slot number by the Crypto Object's UID and type.
   * \param[in] object_uid  target object UID
   * \param[in] object_type  type of the target object
   * \param[in,out] provider_uid  the UID of Crypto Provider responsible for servicing of the slot (a non-zero input
   *                value restricts a search scope to specific Crypto Provider)
   * \param[in] previous_found  the number of previous found key slot (the search will start from next slot number)
   * \return number of a slot containing the found object or kInvalidSlot if the object was not found
   * \pre -
   * \threadsafe TRUE
   * \error ara::crypto::keys::kInvalidSlot No slot found with the given UID.
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ara::crypto::keys::SlotNumber FindObject(ObjectUid const& object_uid, ContentType object_type,
                                           ara::crypto::CryptoProviderUid& provider_uid,
                                           ara::crypto::keys::SlotNumber previous_found) const noexcept final;

  /*!
   * \brief Check the slot for emptiness.
   * \param[in] slot_num  the target slot number
   * \return true if the slot is empty or false otherwise
   * \pre -
   * \threadsafe TRUE
   * \error ara::crypto::SecurityErrc::kInvalidUsageOrder Trying to use key storage provider before connecting it to a
   * database.
   * \error ara::crypto::SecurityErrc::kUnreservedResource Trying to check emptiness of non-existing slot.
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ara::core::Result<bool> IsEmpty(ara::crypto::keys::SlotNumber slot_num) const noexcept final;

  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
  /*!
   * \brief Open a slot containing an existing object with "User" permissions and associate a trusted container
   *           to it (suitable for reading only).
   * \param[in] slot_num  target slot number
   * \param[in] subscribe_for_updates  if this flag is true then the UpdatesObserver instance supplied to the
   *            Key Storage factory function will be subscribed for updates of the slot_num key slot
   * \return an unique smart pointer to allocated trusted container associated with the slot content
   * \details Only a non-empty slot may be opened by this method!
   * \pre -
   * \threadsafe TRUE
   * \error ara::crypto::SecurityErrc::kInvalidUsageOrder Trying to use key storage provider before connecting it to a
   * database.
   * \error ara::crypto::SecurityErrc::kUnreservedResource Trying to open non-existing slot as user.
   * \error ara::crypto::SecurityErrc::kEmptyContainer Trying to open empty slot as user.
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ara::core::Result<ara::crypto::TrustedContainer::Uptrc> OpenAsUser(ara::crypto::keys::SlotNumber slot_num,
                                                                     bool subscribe_for_updates = false) noexcept final;

  /*!
   * \brief Open a key slot with "Owner" permissions and associate a trusted container with it for exclusive access.
   * \param[in] slot_num  the target slot number
   * \return an unique smart pointer to allocated trusted container associated to the slot's space
   * \details Only single instance of the "Owner" TrustedContainer may exist for a key slot simultaneously!
   * \pre -
   * \threadsafe TRUE
   * \error ara::crypto::SecurityErrc::kInvalidUsageOrder Trying to use key storage provider before connecting it to a
   * database.
   * \error ara::crypto::SecurityErrc::kUnreservedResource Trying to open non-existing slot as user.
   * \error ara::crypto::SecurityErrc::kBusyResource Trying to claim a slot which is already owned.
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ara::core::Result<ara::crypto::TrustedContainer::Uptr> OpenAsOwner(
      ara::crypto::keys::SlotNumber slot_num) noexcept final;

  /*!
   * \brief Save a content of provided source trusted container to a persistent slot by their "Owner".
   * \param[in] slot_num  the target slot number
   * \param[in] container  the source volatile container
   * \details The source container may represent as a temporary (volatile) container, so a persistent slot in the
   *       Key Storage, but in the second case the calling application must be "Owner" of the source container too!
   * \pre -
   * \threadsafe TRUE
   * \error ara::crypto::SecurityErrc::kInvalidUsageOrder Trying to use key storage provider before connecting it to a
   * database.
   * \error ara::crypto::SecurityErrc::kIncompatibleObject Trying to persist a session object.
   * \error ara::crypto::SecurityErrc::kUnreservedResource Trying to save to non-existing slot.
   * \error ara::crypto::SecurityErrc::kEmptyContainer Trying to save empty trusted container.
   * \error ara::crypto::SecurityErrc::kIncompatibleObject Unable to retrieve the trusted container's meta information
   * or Unable to retrieve the trusted container's payload.
   * \error ara::crypto::SecurityErrc::kContentDuplication Trying to persist object which already seems to exist in
   * the database.
   * \error ara::crypto::SecurityErrc::kContentRestrictions Trying to save incompatible data set.
   * \error ara::crypto::SecurityErrc::kRuntimeFault Unable to update slot.
   * \reentrant FALSE
   * \synchronous TRUE
   * \trace DSGN-Crypto-Adapter-Pattern
   * \vprivate Component Private
   */
  ara::core::Result<void> SaveCopy(ara::crypto::keys::SlotNumber slot_num,
                                   ara::crypto::TrustedContainer const& container) noexcept final;

  /*!
   * \brief Clear the slot identified by its number.
   * \param[in] slot_num  the target slot number
   * \details This method must perform a secure cleanup without the ability to restore the object data!
   * \details If an object stored in the container references to another key slot, then "references counter"
   *       of the referenced key slot must be automatically decremented (after successful deleting of this object).
   * \pre -
   * \threadsafe TRUE
   * \error ara::crypto::SecurityErrc::kInvalidUsageOrder Trying to use key storage provider before connecting it to a
   * database.
   * \error ara::crypto::SecurityErrc::kUnreservedResource Trying to operate on non-existing slot.
   * \error ara::crypto::SecurityErrc::kBusyResource Slot already opened for writing.
   * \error ara::crypto::SecurityErrc::kRuntimeFault Unable to update slot.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ara::core::Result<void> Clear(ara::crypto::keys::SlotNumber slot_num) noexcept final;

  /*!
   * \brief Get the prototyped properties of the key slot.
   * \param[in] slot_num  the target slot number
   * \param[out] props  the output buffer for storing the prototype properties of the key slot
   * \pre -
   * \threadsafe TRUE
   * \error ara::crypto::SecurityErrc::kInvalidUsageOrder Trying to use key storage provider before connecting it to a
   * database.
   * \error ara::crypto::SecurityErrc::kUnreservedResource Trying to operate on non-existing slot.
   * \error ara::crypto::SecurityErrc::kRuntimeFault Unable to derive type of dependency.
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ara::core::Result<void> GetPrototypedProps(ara::crypto::keys::SlotNumber slot_num,
                                             ara::crypto::keys::KeySlotPrototypeProps& props) const noexcept final;

  /*!
   * \brief Get an actual properties of a content in the key slot.
   * \param[in] slot_num  the target slot number
   * \param[out] props  the output buffer for storing an actual properties of a content in the key slot
   * \pre -
   * \threadsafe TRUE
   * \error ara::crypto::SecurityErrc::kInvalidUsageOrder Trying to use key storage provider before connecting it to a
   * database.
   * \error ara::crypto::SecurityErrc::kUnreservedResource Trying to operate on non-existing slot.
   * \error ara::crypto::SecurityErrc::kEmptyContainer Unable to retrieve properties for empty slot.
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ara::core::Result<void> GetContentProps(ara::crypto::keys::SlotNumber slot_num,
                                          ara::crypto::keys::KeySlotContentProps& props) const noexcept final;

  /*!
   * \brief Get UID of the default Crypto Provider assigned for servicing of the specified key slot.
   * \param[in] slot_num  the target slot number
   * \param[out] provider_uid  the UID of Crypto Provider responsible for servicing of the slot
   * \details Any key slot always has an associated default Crypto Provider that can serve this key slot.
   *       In the simplest case all key slots can be served by a single Crypto Provider installed on the Adaptive
   *       Platform. But in a more complicated case a few different Crypto Providers may coexist in the system,
   *       for example if ECU has one or a few HSMs and software cryptography implementation too, and each of them
   *       has own physical key storage. In such case different dedicated Crypto Providers may serve mentioned HSMs
   *       and the software implementation.
   * \pre -
   * \threadsafe TRUE
   * \error ara::crypto::SecurityErrc::kInvalidUsageOrder Trying to use key storage provider before connecting it to a
   * database.
   * \error ara::crypto::SecurityErrc::kUnreservedResource Trying to operate on non-existing slot.
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ara::core::Result<void> GetDefaultCryptoProviderUid(
      ara::crypto::keys::SlotNumber slot_num, ara::crypto::CryptoProviderUid& provider_uid) const noexcept final;

  /*!
   * \brief Get UID of an Actor granted by the "Owner" rights for the key slot.
   * \param[in] slot_num  the target slot number
   * \param[out] owner_uid  the output buffer for storing the Owner UID of the key slot
   * \pre -
   * \threadsafe TRUE
   * \error ara::crypto::SecurityErrc::kInvalidUsageOrder Trying to use key storage provider before connecting it to a
   * database.
   * \error ara::crypto::SecurityErrc::kUnreservedResource Trying to operate on non-existing slot.
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ara::core::Result<void> GetOwner(ara::crypto::keys::SlotNumber slot_num,
                                   ara::crypto::ActorUid& owner_uid) const noexcept final;

  /*!
   * \brief Get Users' Permissions list of all Actors granted by the User rights for the key slot.
   * \param[in] slot_num  the target slot number
   * \param[out] users  the optional pointer to a vector for storing output list
   * \return number of "Users" in the output list
   * \details If (users != nullptr) then capacity of the output vector should be enough for storing
   *       permissions of all "Users".
   * \pre -
   * \threadsafe TRUE
   * \error ara::crypto::SecurityErrc::kUnsupported Method not implemented.
   * \reentrant TRUE
   * \synchronous TRUE
   */
  ara::core::Result<std::size_t> GetUsers(
      ara::crypto::keys::SlotNumber slot_num,
      ara::core::Vector<ara::crypto::keys::UserPermissions>* users) const noexcept final;

  /*!
   * \brief Get the provider version.
   * \return The version of the provider.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  std::uint64_t GetProviderVersion() const noexcept final;

  /*!
   * \brief Get type of the Provider.
   * \return Type of the Provider.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ara::crypto::ProviderType GetProviderType() const noexcept final;

  /*!
   * \brief Returns a human readable name of the Provider.
   * \details Life-time of the returned string is not less than the Provider instance life-time.
   * \return A pointer to null-terminated string with Provider Name.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  char const* GetProviderName() const noexcept final;

  /*!
   * \brief Returns the Provider's Globally Unique Identifier (GUID).
   * \param[out] provider_uid An output buffer for the Provider's GUID.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  void GetProviderUid(ara::crypto::Guid& provider_uid) const noexcept final;

  /*!
   * \brief Begin new transaction for key slots update.
   * \details A transaction is dedicated for updating related key slots simultaneously (in the atomic way).
   *          Each transaction must start from definition of its "scope" presented by a list of target slots
   *          that should be update during the transaction.
   * \param[in] target_slots  a list of slots that should be updated during this transaction
   * \return an ID assigned to this transaction and unique on this ECU
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error ara::crypto::SecurityErrc::kUnsupported Method not supported.
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ara::core::Result<ara::crypto::keys::TransactionId> BeginTransaction(
      ara::crypto::keys::TransactionScope const& target_slots) noexcept final;

  /*!
   * \brief Commit changes of the transaction to Key Storage.
   * \details The commit command permanently saves all changes made during the transaction in Key Storage and
   *          makes them visible to all "User" Actors/applications (according to their access rights).
   * \param[in] id  an ID of a transaction that should be commited
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error ara::crypto::SecurityErrc::kUnsupported Method not supported.
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ara::core::Result<void> CommitTransaction(ara::crypto::keys::TransactionId id) noexcept final;

  /*!
   * \brief Rollback all changes executed during the transaction in Key Storage.
   * \param[in] id  an ID of a transaction that should be rolled back
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error ara::crypto::SecurityErrc::kUnsupported Method not supported.
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ara::core::Result<void> RollbackTransaction(ara::crypto::keys::TransactionId id) noexcept final;

  /*!
   * \brief Unsubscribe the Update Observer from changes monitoring of the specified slot.
   * \param[in] slot  number of a slot that should be unsubscribed from the updates observing
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error ara::crypto::SecurityErrc::kUnsupported Method not supported.
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ara::core::Result<void> UnsubscribeObserver(ara::crypto::keys::SlotNumber slot) noexcept final;

  /*!
   * \brief Find next slot that refers to the target one (due to the context dependency).
   * \param[in] target_slot  the number of the target physical slot
   * \param[in] previous_found  the number of previous found key slot (the search will start from next slot number)
   * \return a number of found slot or kInvalidSlot if a slot referring to the target one was not found
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error ara::crypto::keys::kInvalidSlot Invalid slot.
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ara::crypto::keys::SlotNumber FindReferringSlot(ara::crypto::keys::SlotNumber target_slot,
                                                  ara::crypto::keys::SlotNumber previous_found) const noexcept final;

  /*!
   * \brief Reset the reference from specified slot to another one (without the slot opening).
   * \param[in] referrer_slot  the number of the "referrer" (source) slot
   * \param[in] referenced_slot  the number of the "referenced" (target) slot
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error ara::crypto::SecurityErrc::kUnsupported Method not supported.
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ara::core::Result<void> ResetReference(ara::crypto::keys::SlotNumber referrer_slot,
                                         ara::crypto::keys::SlotNumber referenced_slot) const noexcept final;

  /*!
   * \brief Check the possibility to load an object from specified key slot to specified Crypto Provider.
   * \param[in] slot_num  a slot number for the check
   * \param[in] provider_uid  an UID of Crypto Provider for the check
   * \return true if the slot content can be loaded to specified provider and false otherwise
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error ara::crypto::SecurityErrc::kUnsupported Method not supported.
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ara::core::Result<bool> CanLoadToCryptoProvider(
      ara::crypto::keys::SlotNumber slot_num, ara::crypto::CryptoProviderUid const& provider_uid) const noexcept final;

  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
  /*!
   * \brief Register consumer Updates Observer.
   * \param[in] observer  optional pointer to a client-supplied UpdatesObserver instance that should be registered
   *            inside %Key Storage implementation and called every time, when an opened for usage/loading key slot is
   *            updated externally (by its "Owner" application)
   * \return shared pointer to previously registered Updates Observer interface (the pointer ownership is "moved out"
   *          to the caller code)
   * \details Only one instance of the UpdatesObserver may be registered by an application process, therefore this
   *       method always unregister previous observer and return its shared pointer.
   * \details If (nullptr == observer) then the method only unregister the previous observer!
   * \details The method returns nullptr if no observers have been registered yet!
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ara::crypto::keys::UpdatesObserver::Sptr RegisterObserver(
      ara::crypto::keys::UpdatesObserver::Sptr observer) noexcept final;

  /*!
   * \brief Get pointer of registered Updates Observer.
   * \return shared pointer to the registered Updates Observer interface (copy of an internal shared pointer
   *          is returned, i.e. the %Key Storage provider continues to keep the ownership)
   * \details The method returns nullptr if no observers have been registered yet!
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ara::crypto::keys::UpdatesObserver::Sptr GetRegisteredObserver() const noexcept final;

  // VECTOR NC AutosarC++17_10-M7.1.2: MD_CRYPTO_AutosarC++17_10-M7.1.2_parameters_should_stay_as_they_are
  /*!
   * \brief Find a slot number by the instance specifier of the target logical slot.
   * \details Method not supported.
   * \param[in] slot_specifier  instance specifier of the target logical slot for search
   * \param[out] provider_uid  the UID of Crypto Provider responsible for servicing of the slot
   * \return number of found slot or kInvalidSlot if a slot with such instance specifier was not found
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error SecurityErrorDomain::kInvalidArgument  if the slot_specifier has incorrect value
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ara::core::Result<ara::crypto::keys::SlotNumber> FindSlot(
      ara::core::InstanceSpecifier const& slot_specifier,
      ara::crypto::CryptoProviderUid& provider_uid) const noexcept final;

 private:
  amsr::sec::ipc::MessageBuilder message_builder_;           /*!< Message builder for communication */
  amsr::sec::ipc::MessageSerializer message_serializer_;     /*!< Message serializer for communication */
  amsr::sec::ipc::MessageDeserializer message_deserializer_; /*!< Message deserializer for communication */
  common::util::Logger logger_;                              /*!< Logging instance */
};

}  // namespace keys
}  // namespace proxy
}  // namespace client
}  // namespace crypto

#endif  // LIB_LIBCLIENT_INCLUDE_CRYPTO_CLIENT_PROXY_KEYS_KEY_STORAGE_PROVIDER_H_
