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
/*!        \file  ara/crypto/keys/key_storage_provider.h
 *        \brief  Key Storage Provider interface.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBARA_INCLUDE_ARA_CRYPTO_KEYS_KEY_STORAGE_PROVIDER_H_
#define LIB_LIBARA_INCLUDE_ARA_CRYPTO_KEYS_KEY_STORAGE_PROVIDER_H_

/**********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include <vector>

#include "ara/core/instance_specifier.h"
#include "ara/crypto/common/provider_info.h"
#include "ara/crypto/common/std_api.h"
#include "ara/crypto/common/trusted_container.h"
#include "ara/crypto/keys/elementary_types.h"
#include "ara/crypto/keys/key_slot_content_props.h"
#include "ara/crypto/keys/key_slot_prototype_props.h"
#include "ara/crypto/keys/updates_observer.h"
#include "ara/crypto/keys/user_permissions.h"

namespace ara {
namespace crypto {
namespace keys {

// VECTOR NC AutosarC++17_10-A10.1.1: MD_CRYPTO_AutosarC++17_10-A10.1.1_multiple_inheritance_false_positive
// VECTOR NC Metric-OO.WMC.One: MD_CRYPTO_Metric-OO.WMC.One
/*!
 * \brief Key Storage Provider interface.
 * \details Any object is uniquely identified by the combination of its GUID and type.
 * \details Only a single instance of any object is allowed in scope of each Crypto Provider in the persistent storage!
 * \details HSMs/TPMs implementing non-extractable keys should use own copies of externally supplied crypto objects.
 * \details A few software Crypto Providers can share single key slot if they support same format.
 * \vpublic
 */
class KeyStorageProvider : public std::enable_shared_from_this<KeyStorageProvider>, public ProviderInfo {
 protected:
  /*!
   * \brief Default move constructor
   * \vprivate Component Private
   */
  KeyStorageProvider(KeyStorageProvider&& /*other*/) noexcept = default;

 public:
  /*!
   * \brief Default constructor
   * \vprivate Component Private
   */
  KeyStorageProvider() = default;

  // VECTOR NC AutosarC++17_10-A10.3.2: MD_CRYPTO_AutosarC++17_10-A10.3.2_virtual_keyword_needed
  /*!
   * \brief Destructor.
   * \vprivate Component Private
   */
  virtual ~KeyStorageProvider() noexcept = default;  // Attention! Deviation: Added virtual keyword.

  /*!
   * \brief Deleted copy constructor
   * \vprivate Component Private
   */
  KeyStorageProvider(KeyStorageProvider const& /*other*/) = delete;

  // VECTOR NC AutosarC++17_10-A10.2.1: MD_CRYPTO_AutosarC++17_10-A10.2.1_hiding_functions
  /*!
   * \brief Deleted move assignment constructor
   * \vprivate Component Private
   */
  KeyStorageProvider& operator=(KeyStorageProvider&& /*other*/) = delete;

  // VECTOR NC AutosarC++17_10-A10.2.1: MD_CRYPTO_AutosarC++17_10-A10.2.1_hiding_functions
  /*!
   * \brief Deleted copy assignment constructor
   * \vprivate Component Private
   */
  KeyStorageProvider& operator=(KeyStorageProvider const& /*other*/) = delete;

  /*!
   * \brief Shared smart pointer of the interface.
   * \vpublic
   */
  using Sptr = std::shared_ptr<KeyStorageProvider>;

  /*!
   * \brief Definition of an object UID type.
   * \vpublic
   */
  using ObjectUid = CryptoObjectUid;

  /*!
   * \brief Definition of an object UID type.
   * \vpublic
   */
  using SlotUid = LogicalSlotUid;

  /*!
   * \brief Definition of a slot content type.
   * \vpublic
   */
  using ContentType = CryptoObjectType;

  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
  // VECTOR NC AutosarC++17_10-A0.1.5: MD_CRYPTO_AutosarC++17_10-A0.1.5_pure_virtual
  /*!
   * \brief Find a slot number by the Logic (persistent) Slot UID.
   * \param[in] slot_uid  Logic Slot UID for search
   * \param[out] provider_uid  an optional pointer to UID of default Crypto Provider assigned for servicing this slot
   * \return number of found slot or kInvalidSlot if a slot with such Logic UID was not found
   * \details If the returned provider_uid has nil value then the slot content can be loaded to any Crypto Provider!
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual SlotNumber FindSlot(SlotUid const& slot_uid, CryptoProviderUid* provider_uid = nullptr) const noexcept = 0;

  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
  /*!
   * \brief Find a slot number by the Crypto Object's UID and type.
   * \param[in] object_uid  target object UID
   * \param[in] object_type  type of the target object
   * \param[in,out] provider_uid  the UID of Crypto Provider responsible for servicing of the slot (a non-zero input
   *                value restricts a search scope to specific Crypto Provider)
   * \param[in] previous_found  the number of previous found key slot (the search will start from next slot number)
   * \return number of a slot containing the found object or kInvalidSlot if the object was not found
   * \details Use previous_found = kInvalidSlot to start the search from the begin.
   * \details If the provider UID has the zero value then the search is executed through whole Key Storage (without
   *       limitation to any specific Crypto Provider).
   * \details If an application needs to find all instances of a crypto objects (through all Crypto Providers) then
   * this method should be called multiple times until it will return kInvalidSlot.
   * \details If the returned provider_uid has nil value then the slot content can be loaded to any Crypto Provider!
   * \details Also see cryp::CryptoObject::Save() for restrictions of content saving to a key slot.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual SlotNumber FindObject(ObjectUid const& object_uid, ContentType object_type, CryptoProviderUid& provider_uid,
                                SlotNumber previous_found = kInvalidSlot) const noexcept = 0;

  /*!
   * \brief Check the slot for emptiness.
   * \param[in] slot_num  the target slot number
   * \return true if the slot is empty or false otherwise
   * \details If the specified slot is involved to a proceeding transaction then the status of the "User" visible
   *       part should be returned!
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error SecurityErrorDomain::kUnreservedResource  if the slot number is incorrect (the slot is not
   *                allocated)
   * \error SecurityErrorDomain::kAccessViolation  if this method is called by an Actor, which has no any
   *                ("Owner" or "User") access rights to the key slot
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual ara::core::Result<bool> IsEmpty(SlotNumber slot_num) const noexcept = 0;

  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
  // VECTOR NC AutosarC++17_10-M9.3.3: MD_CRYPTO_AutosarC++17_10-M9.3.3_AUTOSAR_API
  /*!
   * \brief Open a slot containing an existing object with "User" permissions and associate a trusted container
   *           to it (suitable for reading only).
   * \param[in] slot_num  target slot number
   * \param[in] subscribe_for_updates  if this flag is true then the UpdatesObserver instance supplied to the
   *            Key Storage factory function will be subscribed for updates of the slot_num key slot
   * \return an unique smart pointer to allocated trusted container associated with the slot content
   * \details Only a non-empty slot may be opened by this method!
   * \details If the UpdatesObserver interface was provided to the call of RegisterObserver() then
   *       the UpdatesObserver::OnUpdate() method should be called by Key Storage engine (in a dedicated
   *       thread) every time when this slot is updated (and become visible for "Users").
   * \details Monitoring of the opened key slot will be continued even after destruction of the returned
   *       TrustedContainer, because content of the slot may be loaded to volatile memory (as a CryptoObject
   *       or to a CryptoContext of a crypto primitive), but the TrustedContainer may be destroyed after this.
   *       Therefore if you need to terminate monitoring of the key slot then you should directly call method
   *       UnsubscribeObserver(SlotNumber).
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error SecurityErrorDomain::kUnreservedResource  if the slot number is incorrect (i.e. the slot is not
   *                allocated)
   * \error SecurityErrorDomain::kEmptyContainer  if the slot is empty
   * \error SecurityErrorDomain::kAccessViolation  if this method is called by an Actor, which has no any
   *                "User" access rights to the key slot
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual ara::core::Result<TrustedContainer::Uptrc> OpenAsUser(SlotNumber slot_num,
                                                                bool subscribe_for_updates = false) noexcept = 0;

  // VECTOR NC AutosarC++17_10-M9.3.3: MD_CRYPTO_AutosarC++17_10-M9.3.3_AUTOSAR_API
  /*!
   * \brief Open a key slot with "Owner" permissions and associate a trusted container with it for exclusive access.
   * \param[in] slot_num  the target slot number
   * \return an unique smart pointer to allocated trusted container associated to the slot's space
   * \details Only single instance of the "Owner" TrustedContainer may exist for a key slot simultaneously!
   * \details Slots opened by this method are not monitored by the UpdateObserver notification mechanism!
   * \error SecurityErrorDomain::kUnreservedResource  if the slot number is incorrect (the slot is not
   *                allocated)
   * \error SecurityErrorDomain::kBusyResource  if the specified slot is busy, i.e. another instance of the
   *                "Owner" TrustedContainer already exists
   * \error SecurityErrorDomain::kAccessViolation  if this method is called by an Actor, which is not
   *                "Owner" of the key slot
   * \vpublic
   */
  virtual ara::core::Result<TrustedContainer::Uptr> OpenAsOwner(SlotNumber slot_num) noexcept = 0;

  // VECTOR NC AutosarC++17_10-M9.3.3: MD_CRYPTO_AutosarC++17_10-M9.3.3_AUTOSAR_API
  /*!
   * \brief Save a content of provided source trusted container to a persistent slot by their "Owner".
   * \param[in] slot_num  the target slot number
   * \param[in] container  the source volatile container
   * \details The source container may represent as a temporary (volatile) container, so a persistent slot in the
   *       Key Storage, but in the second case the calling application must be "Owner" of the source container too!
   * \details This method may be used for atomic update of a key slot scoped to some transaction. In such case the
   *       "User" visible part of the slot will be updated only after correspondent call of CommitTransaction().
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error SecurityErrorDomain::kAccessViolation  if this method is called by an Actor, which is not
   *                "Owner" of the target key slot or source container (if it is a persistent container)
   * \error SecurityErrorDomain::kBusyResource  if the target slot is opened by its "Owner", i.e.
   *                a trusted container returned by a call OpenAsOwner(slot_num) is not destroyed yet
   * \error SecurityErrorDomain::kIncompatibleObject  if an object in the container is "session"
   * \error SecurityErrorDomain::kEmptyContainer  if the source trusted container is empty
   * \error SecurityErrorDomain::kContentRestrictions  if an object in the container doesn't satisfy
   *                the slot restrictions (including version control)
   * \error SecurityErrorDomain::kUnreservedResource  if the slot number is incorrect
   *                (the slot is not allocated)
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual ara::core::Result<void> SaveCopy(SlotNumber slot_num, TrustedContainer const& container) noexcept = 0;

  // VECTOR NC AutosarC++17_10-M9.3.3: MD_CRYPTO_AutosarC++17_10-M9.3.3_AUTOSAR_API
  /*!
   * \brief Clear the slot identified by its number.
   * \param[in] slot_num  the target slot number
   * \details This method must perform a secure cleanup without the ability to restore the object data!
   * \details If an object stored in the container references to another key slot, then "references counter"
   *       of the referenced key slot must be automatically decremented (after successful deleting of this object).
   * \details This method may be used for atomic update of a key slot scoped to some transaction. In such case the
   *       "User" visible part of the slot will be updated only after correspondent call of CommitTransaction().
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error SecurityErrorDomain::kUnreservedResource  if the slot number is incorrect (the slot is not
   *                allocated)
   * \error SecurityErrorDomain::kBusyResource  if the target slot is opened by its "Owner", i.e.
   *                a trusted container returned by a call OpenAsOwner(slot_num) is not destroyed yet
   * \error SecurityErrorDomain::kLockedByReference  if the internal "references counter" of this slot
   *                has non-zero value
   * \error SecurityErrorDomain::kBadObjectReference  if the internal "references counter" of a slot
   *                referenced from this one already has zero value
   * \error SecurityErrorDomain::kAccessViolation  if this method is called by an Actor, which is not
   *                "Owner" of the key slot
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual ara::core::Result<void> Clear(SlotNumber slot_num) noexcept = 0;

  /*!
   * \brief Get the prototyped properties of the key slot.
   * \param[in] slot_num  the target slot number
   * \param[out] props  the output buffer for storing the prototype properties of the key slot
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error SecurityErrorDomain::kUnreservedResource  if the slot number is incorrect (the slot is not
   *                allocated)
   * \error SecurityErrorDomain::kAccessViolation  if this method is called by an Actor, which has no any
   *                ("Owner" or "User") access rights to the key slot
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual ara::core::Result<void> GetPrototypedProps(SlotNumber slot_num,
                                                     KeySlotPrototypeProps& props) const noexcept = 0;

  /*!
   * \brief Get an actual properties of a content in the key slot.
   * \param[in] slot_num  the target slot number
   * \param[out] props  the output buffer for storing an actual properties of a content in the key slot
   * \details If this method called by a "User" Actor then always: props.exportability == false.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error SecurityErrorDomain::kUnreservedResource  if the slot number is incorrect (the slot is not
   *                allocated)
   * \error SecurityErrorDomain::kEmptyContainer  if the slot is empty
   * \error SecurityErrorDomain::kAccessViolation  if this method is called by an Actor, which has no any
   *                ("Owner" or "User") access rights to the key slot
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual ara::core::Result<void> GetContentProps(SlotNumber slot_num, KeySlotContentProps& props) const noexcept = 0;

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
   * \details If an object loaded to the slot can be loaded by any installed Crypto Provider then nil UID should be
   *       returned.
   * \details This method checks the slot prototype meta-information (@see KeySlotPrototypeProps).
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error SecurityErrorDomain::kUnreservedResource  if the slot number is incorrect (the slot is not
   *                allocated)
   * \error SecurityErrorDomain::kAccessViolation  if this method is called by an Actor, which has no any
   *                ("Owner" or "User") access rights to the key slot
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual ara::core::Result<void> GetDefaultCryptoProviderUid(SlotNumber slot_num,
                                                              CryptoProviderUid& provider_uid) const noexcept = 0;

  /*!
   * \brief Get UID of an Actor granted by the "Owner" rights for the key slot.
   * \param[in] slot_num  the target slot number
   * \param[out] owner_uid  the output buffer for storing the Owner UID of the key slot
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error SecurityErrorDomain::kUnreservedResource  if the slot number is incorrect (the slot is not
   *                allocated)
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual ara::core::Result<void> GetOwner(SlotNumber slot_num, ActorUid& owner_uid) const noexcept = 0;

  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
  /*!
   * \brief Get Users' Permissions list of all Actors granted by the User rights for the key slot.
   * \param[in] slot_num  the target slot number
   * \param[out] users  the optional pointer to a vector for storing output list
   * \return number of "Users" in the output list
   * \details If (users != nullptr) then capacity of the output vector should be enough for storing
   *       permissions of all "Users".
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error SecurityErrorDomain::kUnreservedResource  if the slot number is incorrect (the slot is not
   *                allocated)
   * \error SecurityErrorDomain::kInsufficientCapacity  if capacity of the output vector is not enough
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual ara::core::Result<std::size_t> GetUsers(
      SlotNumber slot_num, ara::core::Vector<UserPermissions>* users = nullptr) const noexcept = 0;

  // VECTOR NC AutosarC++17_10-A0.1.5: MD_CRYPTO_AutosarC++17_10-A0.1.5_pure_virtual
  // VECTOR NC AutosarC++17_10-M9.3.3: MD_CRYPTO_AutosarC++17_10-M9.3.3_AUTOSAR_API
  /*!
   * \brief Begin new transaction for key slots update.
   * \details A transaction is dedicated for updating related key slots simultaneously (in the atomic way).
   *          Each transaction must start from definition of its "scope" presented by a list of target slots
   *          that should be update during the transaction.
   * \param[in] target_slots  a list of slots that should be updated during this transaction
   * \return an ID assigned to this transaction and unique on this ECU
   * \details Whole code implementing single transaction must be located in a single thread!
   * \details Any changes of the slots covered by a transaction become visible for "User" applications only after
   *       commit, as opposed to a change of a slot executed out of a transaction scope that become visible for
   *       "User" applications at once!
   * \details Any Key Storage implementation should reserve double space for each key slot: 1st for "actual" (in-use)
   *       content and 2nd for "hidden" (new) content, updated during a transaction.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error SecurityErrorDomain::kAccessViolation  if target_slots list has slot numbers that are not
   *                owned by current application
   * \error SecurityErrorDomain::kBusyResource  if target_slots list has slot numbers that are already
   *                involved to another pending transaction
   * \error SecurityErrorDomain::kInvalidArgument  if target_slots list has repetitions of slot numbers
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual ara::core::Result<TransactionId> BeginTransaction(TransactionScope const& target_slots) noexcept = 0;

  // VECTOR NC AutosarC++17_10-A0.1.5: MD_CRYPTO_AutosarC++17_10-A0.1.5_pure_virtual
  // VECTOR NC AutosarC++17_10-M9.3.3: MD_CRYPTO_AutosarC++17_10-M9.3.3_AUTOSAR_API
  /*!
   * \brief Commit changes of the transaction to Key Storage.
   * \details The commit command permanently saves all changes made during the transaction in Key Storage and
   *          makes them visible to all "User" Actors/applications (according to their access rights).
   * \param[in] id  an ID of a transaction that should be commited
   * \details Any changes of key slots made during a transaction are invisible to all "Users" (including the "Owner"
   *       application) up to the commit execution!
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error SecurityErrorDomain::kInvalidArgument  if provided id is invalid, i.e. this ID is unknown
   *                or correspondent transaction already was finished (commited or rolled back)
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual ara::core::Result<void> CommitTransaction(TransactionId id) noexcept = 0;

  // VECTOR NC AutosarC++17_10-A0.1.5: MD_CRYPTO_AutosarC++17_10-A0.1.5_pure_virtual
  // VECTOR NC AutosarC++17_10-M9.3.3: MD_CRYPTO_AutosarC++17_10-M9.3.3_AUTOSAR_API
  /*!
   * \brief Rollback all changes executed during the transaction in Key Storage.
   * \details The rollback command permanently cancels all changes made during the transaction in Key Storage.
   *          A rolled back transaction is completely invisible for all "User" applications.
   * \param[in] id  an ID of a transaction that should be rolled back
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error SecurityErrorDomain::kInvalidArgument  if provided id is invalid, i.e. this ID is unknown
   *                or correspondent transaction already was finished (commited or rolled back)
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual ara::core::Result<void> RollbackTransaction(TransactionId id) noexcept = 0;

  // VECTOR NC AutosarC++17_10-A0.1.5: MD_CRYPTO_AutosarC++17_10-A0.1.5_pure_virtual
  // VECTOR NC AutosarC++17_10-M9.3.3: MD_CRYPTO_AutosarC++17_10-M9.3.3_AUTOSAR_API
  /*!
   * \brief Unsubscribe the Update Observer from changes monitoring of the specified slot.
   * \param[in] slot  number of a slot that should be unsubscribed from the updates observing
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error SecurityErrorDomain::kInvalidArgument  if the specified slot is not monitored now (i.e. if it
   *        was not successfully opened via OpenAsUser() or it was already unsubscribed by this method)
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual ara::core::Result<void> UnsubscribeObserver(SlotNumber slot) noexcept = 0;

  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
  // VECTOR NC AutosarC++17_10-A0.1.5: MD_CRYPTO_AutosarC++17_10-A0.1.5_pure_virtual
  /*!
   * \brief Find next slot that refers to the target one (due to the context dependency).
   * \param[in] target_slot  the number of the target physical slot
   * \param[in] previous_found  the number of previous found key slot (the search will start from next slot number)
   * \return a number of found slot or kInvalidSlot if a slot referring to the target one was not found
   * \details Use the previous_found = kInvalidSlot to start the search from the begin.
   * \details If an application needs to find all slots referring to the target one then
   *       this method should be called multiple times until it will return kInvalidSlot.
   * \details Also see cryp::CryptoObject::Save() for additional information about the references between slots.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual SlotNumber FindReferringSlot(SlotNumber target_slot,
                                       SlotNumber previous_found = kInvalidSlot) const noexcept = 0;

  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
  // VECTOR NC AutosarC++17_10-A0.1.5: MD_CRYPTO_AutosarC++17_10-A0.1.5_pure_virtual
  /*!
   * \brief Reset the reference from specified slot to another one (without the slot opening).
   * \param[in] referrer_slot  the number of the "referrer" (source) slot
   * \param[in] referenced_slot  the number of the "referenced" (target) slot
   * \details This operation can be executed only if the caller executable is "Owner" of the both slots.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error SecurityErrorDomain::kUnreservedResource  if the referrer_slot is incorrect (the slot is not
   *                allocated)
   * \error SecurityErrorDomain::kBadObjectReference  if an object in the the "referenced" slot has COUID
   *                different from the field KeySlotContentProps::mDependencyUid value of the "referrer" slot
   *                or if the objects in these slots has incompatible for referencing types
   * \error SecurityErrorDomain::kAccessViolation  if this method is called by an Actor, which is not
   *                "Owner" of the both key slots
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual ara::core::Result<void> ResetReference(SlotNumber referrer_slot,
                                                 SlotNumber referenced_slot = kInvalidSlot) const noexcept = 0;

  // VECTOR NC AutosarC++17_10-A0.1.5: MD_CRYPTO_AutosarC++17_10-A0.1.5_pure_virtual
  /*!
   * \brief Check the possibility to load an object from specified key slot to specified Crypto Provider.
   * \param[in] slot_num  a slot number for the check
   * \param[in] provider_uid  an UID of Crypto Provider for the check
   * \return true if the slot content can be loaded to specified provider and false otherwise
   * \details If the provider_uid has nil value then the method checks the possibility to load the slot content to
   *       any Crypto Provider installed in the system.
   * \details This method should check not only the Crypto Providers "trust relationship matrix", but also actual
   *       content of the slot (object type, algorithm identifier, reference to dependency object) and the technical
   *       capabilities of the target Crypto Provider.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error SecurityErrorDomain::kUnreservedResource  if the slot number is incorrect (the slot is not
   *                allocated)
   * \error SecurityErrorDomain::kEmptyContainer  if the slot is empty, but its prototype is not strict,
   *                i.e. if there is impossible to make the check due to lack of meta-information
   * \error SecurityErrorDomain::kAccessViolation  if this method is called by an Actor, which has no any
   *                ("Owner" or "User") access rights to the key slot
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual ara::core::Result<bool> CanLoadToCryptoProvider(SlotNumber slot_num,
                                                          CryptoProviderUid const& provider_uid) const noexcept = 0;

  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
  // VECTOR NC AutosarC++17_10-A0.1.5: MD_CRYPTO_AutosarC++17_10-A0.1.5_pure_virtual
  // VECTOR NC AutosarC++17_10-M9.3.3: MD_CRYPTO_AutosarC++17_10-M9.3.3_AUTOSAR_API
  /*!
   * \brief Register consumer Updates Observer.
   * \param[in] observer  optional pointer to a client-supplied UpdatesObserver instance that should be registered
   *            inside Key Storage implementation and called every time, when an opened for usage/loading key slot is
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
   * \vpublic
   */
  virtual UpdatesObserver::Sptr RegisterObserver(UpdatesObserver::Sptr observer = nullptr) noexcept = 0;

  /*!
   * \brief Get pointer of registered Updates Observer.
   * \return shared pointer to the registered Updates Observer interface (copy of an internal shared pointer
   *          is returned, i.e. the Key Storage provider continues to keep the ownership)
   * \details The method returns nullptr if no observers have been registered yet!
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual UpdatesObserver::Sptr GetRegisteredObserver() const noexcept = 0;

  // VECTOR NC AutosarC++17_10-A0.1.5: MD_CRYPTO_AutosarC++17_10-A0.1.5_pure_virtual
  // VECTOR NC AutosarC++17_10-M7.1.2: MD_CRYPTO_AutosarC++17_10-M7.1.2_autosar_api
  /*!
   * \brief Find a slot number by the instance specifier of the target logical slot.
   * \param[in] slot_specifier  instance specifier of the target logical slot for search
   * \param[out] provider_uid  the UID of Crypto Provider responsible for servicing of the slot
   * \return number of found slot or kInvalidSlot if a slot with such instance specifier was not found
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error SecurityErrorDomain::kInvalidArgument  if the slot_specifier has incorrect value
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual ara::core::Result<SlotNumber> FindSlot(ara::core::InstanceSpecifier const& slot_specifier,
                                                 CryptoProviderUid& provider_uid) const noexcept = 0;
};

}  // namespace keys
}  // namespace crypto
}  // namespace ara

#endif  // LIB_LIBARA_INCLUDE_ARA_CRYPTO_KEYS_KEY_STORAGE_PROVIDER_H_
