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
/*!        \file  composite_key_storage_provider.h
 *        \brief  KeyStorageProvider which combines all KSPs of the available CryptoProviders.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBSERVER_INCLUDE_CRYPTO_SERVER_KEYS_COMPOSITE_KEY_STORAGE_PROVIDER_H_
#define LIB_LIBSERVER_INCLUDE_CRYPTO_SERVER_KEYS_COMPOSITE_KEY_STORAGE_PROVIDER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include <utility>
#include <vector>

#include "amsr/core/instance_specifier.h"
#include "amsr/core/map.h"
#include "amsr/core/string.h"
#include "amsr/core/string_view.h"
#include "amsr/sec/ipc/protocol.h"
#include "crypto/common/keys/key_storage_provider.h"
#include "crypto/common/trusted_container_impl.h"
#include "crypto/server/call_context_registry.h"
#include "crypto/server/config/access_control_config.h"
#include "crypto/server/crypto_provider_factory.h"
#include "crypto/server/keys/json_key_database.h"
#include "crypto/server/security_events_reporter_interface.h"

namespace crypto {
namespace server {
namespace keys {

// VECTOR NC Metric-OO.RFC: MD_CRYPTO_Metric-OO.RFC
// VECTOR NC Metric-OO.WMC.One: MD_CRYPTO_Metric-OO.WMC.One
/*!
 * \brief KeyStorageProvider which combines all KSPs of the available CryptoProviders
 * \trace DSGN-Crypto-KeyStorage
 * \trace DSGN-CryptoAccessRestriction
 * \vprivate Component Private
 */
class CompositeKeyStorageProvider : public common::keys::KeyStorageProvider {
 public:
  /*!
   * \brief Unique pointer to the CKSP.
   * \vprivate Component Private
   */
  using Uptr = std::unique_ptr<CompositeKeyStorageProvider>;

  /*!
   * \brief Factory method for generating CKSPs.
   * \param[in] config_file Path to the key configuration file.
   * \param[in] call_context_registry Registry providing information about the current caller.
   * \return The created instance.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error ara::crypto::SecurityErrc::kInvalidUsageOrder If the database is already attached to a resource.
   * \error ara::crypto::SecurityErrc::kResourceFault If the database config file cannot be opened, the slot folder
   * cannot be created or one of the slot files cannot be accessed.
   * \error ara::crypto::SecurityErrc::kUnsupportedFormat If the configuration JSON is malformed.
   * \error ara::crypto::SecurityErrc::kInvalidArgument If a UUID or a path in the configuration is malformed.
   * \error ara::crypto::SecurityErrc::kInsufficientCapacity If the database buffer is too small to contain all the
   * slots.
   * \error ara::crypto::SecurityErrc::kBusyResource If some slot numbers are used twice.
   * \error ara::crypto::SecurityErrc::kRuntimeFault Unable to connect to database.
   * \reentrant TRUE
   * \synchronous TRUE
   */
  static auto Create(amsr::core::StringView config_file,
                     crypto::server::CallContextRegistry const& call_context_registry) noexcept
      -> amsr::core::Result<Uptr>;

  /*!
   * \brief Set the access control configuration.
   * \param[in] access_control_config Access control configuration file.
   * \return void.
   * \context ANY
   * \pre -
   * \error ara::crypto::SecurityErrc::kInvalidArgument If key_db == nullptr
   * \error ara::crypto::SecurityErrc::kRuntimeFault Failed to connect
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \trace CREQ-Crypto-RestrictAccessToCryptographicMaterial
   * \vprivate Component Private
   */
  auto SetAccessControlConfig(
      std::unique_ptr<crypto::server::config::AccessControlConfig> access_control_config) noexcept -> void;

  /*!
   * \brief Set the security events reporter.
   * \param[in] security_events_reporter Security events reporter object.
   * \return void.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \trace CREQ-Crypto-ReportKeyAccessSecurityEvents
   * \vprivate Component Private
   */
  auto SetSecurityEventsReporter(std::unique_ptr<SecurityEventsReporterInterface> security_events_reporter) noexcept
      -> void;

  /*!
   * \brief Connects the key storage provider to a given database.
   * \param[in,out] key_db Database to connect to.
   * \context ANY
   * \pre -
   * \error ara::crypto::SecurityErrc::kInvalidArgument If key_db == nullptr
   * \error ara::crypto::SecurityErrc::kRuntimeFault Failed to connect
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto Connect(std::shared_ptr<JsonKeyDatabase> key_db) noexcept -> amsr::core::Result<void>;

  /*!
   * \brief Constructor
   * \param[in] call_context_registry Registry providing information about the current caller.
   * \vprivate Component Private
   */
  explicit CompositeKeyStorageProvider(crypto::server::CallContextRegistry const& call_context_registry) noexcept;

  /*!
   * \brief Default copy constructor.
   * \vprivate Component Private
   */
  CompositeKeyStorageProvider(CompositeKeyStorageProvider const& /*other*/) = delete;

  /*!
   * \brief Default move constructor.
   * \vprivate Component Private
   */
  CompositeKeyStorageProvider(CompositeKeyStorageProvider&& /*other*/) noexcept = delete;

  /*!
   * \brief Destructor.
   * \vprivate Component Private
   */
  ~CompositeKeyStorageProvider() noexcept override = default;

  // VECTOR NC AutosarC++17_10-A10.2.1: MD_CRYPTO_AutosarC++17_10-A10.2.1_hiding_functions
  /*!
   * \brief Deleted copy assignment operator.
   * \return -
   * \vprivate Component Private
   */
  CompositeKeyStorageProvider& operator=(CompositeKeyStorageProvider const& /*other*/) & = delete;

  // VECTOR NC AutosarC++17_10-A10.2.1: MD_CRYPTO_AutosarC++17_10-A10.2.1_hiding_functions
  /*!
   * \brief Deleted move assignment operator.
   * \return -
   * \vprivate Component Private
   */
  CompositeKeyStorageProvider& operator=(CompositeKeyStorageProvider&& /*other*/) & noexcept = delete;

  /*!
   * \brief Returns an encoded version of the Provider.
   * \details The QWORD value returned by this method includes two DWORD-length fields:
   *          - "Version structure" (32 bits) - most significant DWORD
   *          - "Build date & time" (32 bits) - least significant DWORD (LSDW)
   *          The "Version structure" includes four fields (each of them corresponds to one byte):
   *          - Major version (8 bits) - most significant byte (MSB) of the Version DWORD
   *          - Minor version (8 bits)
   *          - Patch version (8 bits)
   *          - Revision version (8 bits) - least significant byte (LSB) of the Version DWORD
   *          The "Build date & time" is a number of minutes since 1st of January 1970 00:00.
   *          So generally the Version QWORD has following structure:
   *
   *          |  MSB  | 8 bits | 8 bits |  LSB  |      LSDW (32 bits)     |
   *          | :---: | :----: | :----: | :---: | :---------------------: |
   *          | Major | Minor  | Patch  | Revis |    Build date & time    |
   * \return Encoded version that has following structure:
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  std::uint64_t GetProviderVersion() const noexcept override;

  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
  /*!
   * \brief Returns a human readable name of the Provider.
   * \details Life-time of the returned string is not less than the Provider instance life-time.
   * \return A pointer to null-terminated string with Provider Name.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  char const* GetProviderName() const noexcept override;

  /*!
   * \brief Get type of the Provider.
   * \param[out] provider_uid  The Globally Unique Identifier data structure - parameter unused.
   * \return Type of the Provider.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  void GetProviderUid(ara::crypto::Guid& provider_uid) const noexcept override;

  /*!
   * \brief Get type of the Provider.
   * \return Type of the Provider.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ara::crypto::ProviderType GetProviderType() const noexcept override;

  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
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
   * \vprivate Component Private
   */
  ara::crypto::keys::SlotNumber FindSlot(ara::crypto::keys::KeyStorageProvider::SlotUid const& slot_uid,
                                         ara::crypto::CryptoProviderUid* provider_uid) const noexcept override;

  /*!
   * \brief Find a slot number by the Crypto Object's UID and type.
   * \param[in] object_uid  target object UID
   * \param[in] object_type  type of the target object
   * \param[in,out] provider_uid  the UID of Crypto Provider responsible for servicing of the slot (a non-zero input
   *                value restricts a search scope to specific Crypto Provider)
   * \param[in] previous_found  the number of previous found key slot (the search will start from next slot number)
   * \return number of a slot containing the found object or kInvalidSlot if the object was not found
   * \details Use previous_found = kInvalidSlot to start the search from the begin.
   * \details If the provider UID has the zero value then the search is executed through whole %Key Storage (without
   *       limitation to any specific Crypto Provider).
   * \details If an application needs to find all instances of a crypto objects (through all Crypto Providers) then
   * this method should be called multiple times until it will return kInvalidSlot.
   * \details If the returned
   * provider_uid has nil value then the slot content can be loaded to any Crypto Provider!
   * \details Also see cryp::CryptoObject::Save() for restrictions of content saving to a key slot.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ara::crypto::keys::SlotNumber FindObject(ara::crypto::keys::KeyStorageProvider::ObjectUid const& object_uid,
                                           ara::crypto::keys::KeyStorageProvider::ContentType object_type,
                                           ara::crypto::CryptoProviderUid& provider_uid,
                                           ara::crypto::keys::SlotNumber previous_found) const noexcept override;

  /*!
   * \brief Check the slot for emptiness.
   * \param[in] slot_num  the target slot number
   * \return true if the slot is empty or false otherwise
   * \details If the specified slot is involved to a proceeding transaction then the status of the "User" visible
   *       part should be returned!
   * \context ANY
   * \pre The key storage provider is connected to a database.
   * \threadsafe FALSE
   * \error ara::crypto::SecurityErrc::kInvalidUsageOrder Trying to use key storage provider before connecting it to a
   * database.
   * \error ara::crypto::SecurityErrc::kUnreservedResource Trying to check emptiness of non-existing slot.
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  amsr::core::Result<bool> IsEmpty(ara::crypto::keys::SlotNumber slot_num) const noexcept override;

  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
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
   * \pre The key storage provider is connected to a database.
   * \threadsafe FALSE
   * \error ara::crypto::SecurityErrc::kInvalidUsageOrder Trying to use key storage provider before connecting it to a
   * database.
   * \error ara::crypto::SecurityErrc::kUnreservedResource Trying to open non-existing slot as user.
   * \error ara::crypto::SecurityErrc::kEmptyContainer Trying to open empty slot as user.
   * \reentrant TRUE
   * \synchronous TRUE
   * \trace CREQ-Crypto-LoadCryptographicMaterial
   * \vprivate Component Private
   */
  amsr::core::Result<ara::crypto::TrustedContainer::Uptrc> OpenAsUser(
      ara::crypto::keys::SlotNumber slot_num, bool subscribe_for_updates = false) noexcept override;

  /*!
   * \brief Open a key slot with "Owner" permissions and associate a trusted container with it for exclusive access.
   * \param[in] slot_num  the target slot number
   * \return an unique smart pointer to allocated trusted container associated to the slot's space
   * \details Only single instance of the "Owner" TrustedContainer may exist for a key slot simultaneously!
   * \details Slots opened by this method are not monitored by the UpdateObserver notification mechanism!
   * \context ANY
   * \pre The key storage provider is connected to a database.
   * \threadsafe FALSE
   * \error ara::crypto::SecurityErrc::kInvalidUsageOrder Trying to use key storage provider before connecting it to a
   * database.
   * \error ara::crypto::SecurityErrc::kUnreservedResource Trying to open non-existing slot as user.
   * \error ara::crypto::SecurityErrc::kBusyResource Trying to claim a slot which is already owned.
   * \error ara::crypto::SecurityErrc::kRuntimeFault Creation of trusted container failed.
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  amsr::core::Result<ara::crypto::TrustedContainer::Uptr> OpenAsOwner(
      ara::crypto::keys::SlotNumber slot_num) noexcept override;

  /*!
   * \brief Save a content of provided source trusted container to a persistent slot by their "Owner".
   * \param[in] slot_num  the target slot number
   * \param[in] container  the source volatile container
   * \details The source container may represent as a temporary (volatile) container, so a persistent slot in the
   *        Key Storage, but in the second case the calling application must be "Owner" of the source container too!
   * \details This method may be used for atomic update of a key slot scoped to some transaction. In such case the
   *       "User" visible part of the slot will be updated only after correspondent call of CommitTransaction().
   * \context ANY
   * \pre The key storage provider is connected to a database.
   * \threadsafe TRUE
   * \error ara::crypto::SecurityErrc::kInvalidUsageOrder Trying to use key storage provider before connecting it to a
   * database.
   * \error ara::crypto::SecurityErrc::kIncompatibleObject Trying to persist a session object or unable to
   * retrieve the trusted container's meta information or unable to retrieve the trusted container's payload.
   * \error ara::crypto::SecurityErrc::kUnreservedResource Trying to save to non-existing slot.
   * \error ara::crypto::SecurityErrc::kEmptyContainer Trying to save empty trusted container.
   * \error ara::crypto::SecurityErrc::kContentDuplication Trying to persist object which already seems to exist
   * in the database.
   * \error ara::crypto::SecurityErrc::kContentRestrictions Trying to save incompatible data set.
   * \error ara::crypto::SecurityErrc::kRuntimeFault Unable to update slot.
   * \reentrant FALSE
   * \synchronous TRUE
   * \trace CREQ-Crypto-PersistCryptographicMaterial
   * \vprivate Component Private
   */
  amsr::core::Result<void> SaveCopy(ara::crypto::keys::SlotNumber slot_num,
                                    ara::crypto::TrustedContainer const& container) noexcept override;

  /*!
   * \brief Clear the slot identified by its number.
   * \param[in] slot_num  the target slot number
   * \details This method must perform a secure cleanup without the ability to restore the object data!
   * \details If an object stored in the container references to another key slot, then "references counter"
   *       of the referenced key slot must be automatically decremented (after successful deleting of this object).
   * \details This method may be used for atomic update of a key slot scoped to some transaction. In such case the
   *       "User" visible part of the slot will be updated only after correspondent call of CommitTransaction().
   * \context ANY
   * \pre The key storage provider is connected to a database.
   * \threadsafe FALSE
   * \error ara::crypto::SecurityErrc::kInvalidUsageOrder Trying to use key storage provider before connecting it to a
   * database.
   * \error ara::crypto::SecurityErrc::kUnreservedResource Trying to operate on non-existing slot.
   * \error ara::crypto::SecurityErrc::kBusyResource Slot already opened for writing.
   * \error ara::crypto::SecurityErrc::kRuntimeFault Unable to update slot.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  amsr::core::Result<void> Clear(ara::crypto::keys::SlotNumber slot_num) noexcept override;

  /*!
   * \brief Get the prototyped properties of the key slot.
   * \param[in] slot_num  the target slot number
   * \param[out] props  the output buffer for storing the prototype properties of the key slot
   * \context ANY
   * \pre The key storage provider is connected to a database.
   * \threadsafe FALSE
   * \error ara::crypto::SecurityErrc::kInvalidUsageOrder Trying to use key storage provider before connecting it to a
   * database.
   * \error ara::crypto::SecurityErrc::kUnreservedResource Trying to operate on non-existing slot.
   * \error ara::crypto::SecurityErrc::kRuntimeFault Unable to derive type of dependency.
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  amsr::core::Result<void> GetPrototypedProps(ara::crypto::keys::SlotNumber slot_num,
                                              ara::crypto::keys::KeySlotPrototypeProps& props) const noexcept override;

  /*!
   * \brief Get an actual properties of a content in the key slot.
   * \param[in] slot_num  the target slot number
   * \param[out] props  the output buffer for storing an actual properties of a content in the key slot
   * \details If this method called by a "User" Actor then always: props.exportability == false.
   * \context ANY
   * \pre The key storage provider is connected to a database.
   * \threadsafe FALSE
   * \error ara::crypto::SecurityErrc::kInvalidUsageOrder Trying to use key storage provider before connecting it to a
   * database.
   * \error ara::crypto::SecurityErrc::kUnreservedResource Trying to operate on non-existing slot.
   * \error ara::crypto::SecurityErrc::kEmptyContainer Unable to retrieve properties for empty slot.
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  amsr::core::Result<void> GetContentProps(ara::crypto::keys::SlotNumber slot_num,
                                           ara::crypto::keys::KeySlotContentProps& props) const noexcept override;

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
   * \context ANY
   * \pre The key storage provider is connected to a database.
   * \threadsafe FALSE
   * \error ara::crypto::SecurityErrc::kInvalidUsageOrder Trying to use key storage provider before connecting it to a
   * database.
   * \error ara::crypto::SecurityErrc::kUnreservedResource Trying to operate on non-existing slot.
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  amsr::core::Result<void> GetDefaultCryptoProviderUid(
      ara::crypto::keys::SlotNumber slot_num, ara::crypto::CryptoProviderUid& provider_uid) const noexcept override;

  /*!
   * \brief Get UID of an Actor granted by the "Owner" rights for the key slot.
   * \param[in] slot_num  the target slot number
   * \param[out] owner_uid  the output buffer for storing the Owner UID of the key slot
   * \context ANY
   * \pre The key storage provider is connected to a database.
   * \threadsafe FALSE
   * \error ara::crypto::SecurityErrc::kInvalidUsageOrder Trying to use key storage provider before connecting it to a
   * database.
   * \error ara::crypto::SecurityErrc::kUnreservedResource Trying to operate on non-existing slot.
   * \reentrant
   * TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  amsr::core::Result<void> GetOwner(ara::crypto::keys::SlotNumber slot_num,
                                    ara::crypto::ActorUid& owner_uid) const noexcept override;

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
   * \vprivate Component Private
   */
  amsr::core::Result<std::size_t> GetUsers(
      ara::crypto::keys::SlotNumber slot_num,
      ara::core::Vector<ara::crypto::keys::UserPermissions>* users) const noexcept override;

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
   * \vprivate Component Private
   */
  amsr::core::Result<ara::crypto::keys::TransactionId> BeginTransaction(
      ara::crypto::keys::TransactionScope const& target_slots) noexcept override;

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
   * \vprivate Component Private
   */
  amsr::core::Result<void> CommitTransaction(ara::crypto::keys::TransactionId id) noexcept override;

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
   * \vprivate Component Private
   */
  amsr::core::Result<void> RollbackTransaction(ara::crypto::keys::TransactionId id) noexcept override;

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
   * \vprivate Component Private
   */
  amsr::core::Result<void> UnsubscribeObserver(ara::crypto::keys::SlotNumber slot) noexcept override;

  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
  /*!
   * \brief Find next slot that refers to the target one (due to the context dependency).
   * \param[in] target_slot  the number of the target physical slot
   * \param[in] previous_found  the number of previous found key slot (the search will start from next slot number)
   * \return a number of found slot or kInvalidSlot if a slot referring to the target one was not found
   * \details Use the previous_found = kInvalidSlot to start the search from the beginning.
   * \details If an application needs to find all slots referring to the target one then
   *       this method should be called multiple times until it will return kInvalidSlot.
   * \details Also see cryp::CryptoObject::Save() for additional information about the references between slots.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ara::crypto::keys::SlotNumber FindReferringSlot(ara::crypto::keys::SlotNumber target_slot,
                                                  ara::crypto::keys::SlotNumber previous_found) const noexcept override;

  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
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
   * \vprivate Component Private
   */
  amsr::core::Result<void> ResetReference(ara::crypto::keys::SlotNumber referrer_slot,
                                          ara::crypto::keys::SlotNumber referenced_slot) const noexcept override;

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
   * \vprivate Component Private
   */
  amsr::core::Result<bool> CanLoadToCryptoProvider(
      ara::crypto::keys::SlotNumber slot_num,
      ara::crypto::CryptoProviderUid const& provider_uid) const noexcept override;

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
      ara::crypto::keys::UpdatesObserver::Sptr observer) noexcept override;

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
  ara::crypto::keys::UpdatesObserver::Sptr GetRegisteredObserver() const noexcept override;

  // VECTOR NC AutosarC++17_10-M7.1.2: MD_CRYPTO_AutosarC++17_10-M7.1.2_parameters_should_stay_as_they_are
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
   * \vprivate Component Private
   */
  amsr::core::Result<ara::crypto::keys::SlotNumber> FindSlot(
      amsr::core::InstanceSpecifier const& slot_specifier,
      ara::crypto::CryptoProviderUid& provider_uid) const noexcept override;

  /*!
   * \copydoc crypto::common::keys::KeyStorageProvider::UpdateKeySlot()
   */
  auto UpdateKeySlot(ara::crypto::keys::SlotNumber slot_number, ara::crypto::keys::KeySlotContentProps content_props,
                     ara::crypto::ReadOnlyMemRegion content_payload) noexcept -> amsr::core::Result<void> override;

  /*!
   * \brief Returns all key slot numbers for a crypto provider.
   * \param[in] crypto_provider The CryptoProvider for which to return all key slot numbers.
   * \return All key slot numbers belonging to the crypto provider \c crypto_provider with their respective UUIDs.
   *         The returned collection has no particular order.
   * \pre This key storage provider is connected.
   * \threadsafe FALSE
   * \error ara::crypto::SecurityErrc::kUnknownIdentifier if \c crypto_provider is unknown to this key storage provider.
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto GetAllKeySlotNumbers(ara::crypto::cryp::CryptoProvider const& crypto_provider) const noexcept
      -> amsr::core::Result<ara::core::Vector<std::pair<ara::crypto::keys::SlotNumber, ara::crypto::Uuid>>> override;

  /*!
   * \brief Register an OnClearCallback function to be notified
   * \param[in] crypto_provider The CryptoProvider for which to register the callback
   * \param[in] callback_function Callback function to register
   * \return -
   * \context CryptoProvider
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto RegisterOnClearCallback(ara::crypto::cryp::CryptoProvider const& crypto_provider,
                               OnClearCallback&& callback_function) noexcept -> void override;

  /*!
   * \brief Sets the crypto provider factory used by this KSP
   * \param[in] crypto_provider_factory Factory
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  void SetCryptoProviderFactory(std::weak_ptr<CryptoProviderFactory> crypto_provider_factory);

 protected:
  /*!
   * \brief Entry of the UUID index which allows for mapping from a slot UUID to a slot.
   * \vprivate Component Private
   */
  struct UuidIndexEntry {
    /*!
     * \brief UUID related to the slot.
     */
    ara::crypto::Uuid uuid{};

    /*!
     * \brief Number of the slot.
     */
    ara::crypto::keys::SlotNumber slot_number{};

    /*!
     * \brief Buffer index of the slot.
     */
    std::size_t index{};
  };

  /*!
   * \brief Entry of the COUID index which allows for mapping from an object COUID to a slot.
   * \vprivate Component Private
   */
  struct CouidIndexEntry {
    /*!
     * \brief COUID of the slot payload.
     */
    ara::crypto::CryptoObjectUid couid{};

    /*!
     * \brief Type of the slot payload.
     */
    ara::crypto::CryptoObjectType type{};

    /*!
     * \brief Number of the slot.
     */
    ara::crypto::keys::SlotNumber slot_number{};

    /*!
     * \brief Number of the slot.
     */
    std::size_t index{};
  };

 private:
  // VECTOR NC VectorC++-V11.0.3: MD_CRYPTO_VectorC++-V11-0-3_public_data_members
  /*!
   * \brief Internal slot representation.
   */
  struct Slot final {
    // VECTOR NC AutosarC++17_10-M8.5.1: MD_CRYPTO_AutosarC++17_10-M8.5.1_false_positive
    /*!
     * Default constructor.
     */
    Slot() = default;
    /*!
     * Copy constructor.
     * \param[in] other Other slot
     */
    Slot(Slot const& other)
        : handle(other.handle), content(other.content), owner_tc_exists(other.owner_tc_exists.load()) {}
    /*!
     * Move consructor.
     * \param[in] other Other slot
     */
    Slot(Slot&& other) noexcept
        : handle(std::move(other.handle)),
          content(std::move(other.content)),
          // VECTOR NL AutosarC++17_10-A12.8.4: MD_CRYPTO_AutosarC++17_10-A12.8.4_copy_of_atomic
          owner_tc_exists(other.owner_tc_exists.load()) {}

    /*!
     * Copy assignment.
     * \param[in] other Other slot
     * \return copied slot
     */
    Slot& operator=(Slot const& other) & {
      if (this != &other) {
        handle = other.handle;
        content = other.content;
        owner_tc_exists = other.owner_tc_exists.load();
      }
      return *this;
    }
    /*!
     * Move assignment.
     * \param[in] other Other slot
     * \return moved slot
     */
    Slot& operator=(Slot&& other) & noexcept {
      if (this != &other) {
        handle = std::move(other.handle);
        content = std::move(other.content);
        owner_tc_exists = other.owner_tc_exists.load();
      }
      return *this;
    }

    /*!
     * Default destructor
     */
    ~Slot() = default;
    KeySlotHandle handle{};              /*!< Handle to the slot. */
    KeySlotContent content{};            /*!< Slot content */
    std::atomic<bool> owner_tc_exists{}; /*!< Indicates if an owner TC currently exist. */
  };

  /*!
   * \brief Iterator for the slot number index map.
   * \vprivate Component Private
   */
  using SlotNumIdxIter = vac::container::StaticMap<JsonKeyDatabase::SlotNumber, std::size_t>::const_iterator;

  /*!
   * \brief Finds the UUID index entry pointing to the next greater slot number.
   * \param[in] begin Startpoint of the search.
   * \param[in] end Endpoint of the search.
   * \param[in] slot_number Reference slot number.
   * \return A pointer to the found UUID index entry.
   * \context ANY
   * \pre begin <= end
   * \threadsafe FALSE
   * \reentrant TRUE
   * \synchronous TRUE
   */
  static UuidIndexEntry* FindGreaterSlotNumber(UuidIndexEntry* begin, UuidIndexEntry* end,
                                               ara::crypto::keys::SlotNumber slot_number) noexcept;

  /*!
   * \brief Finds the UUID index entries pointing to a certain UUID.
   * \param[in] begin Startpoint of the search.
   * \param[in] end Endpoint of the search.
   * \param[in] uuid Reference UUID.
   * \return The range of found UUID index entries.
   * \context ANY
   * \pre begin <= end
   * \threadsafe FALSE
   * \reentrant TRUE
   * \synchronous TRUE
   */
  std::pair<UuidIndexEntry*, UuidIndexEntry*> FindUuid(UuidIndexEntry* begin, UuidIndexEntry* end,
                                                       ara::crypto::Uuid uuid) const noexcept;

  /*!
   * \brief Adds the given slot data to the UUID index.
   * \param[in] uuid UUID of the slot.
   * \param[in] slot_number Slot number of the slot.
   * \param[in] index Position in the buffer of the slot.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error ara::crypto::SecurityErrc::kInsufficientResource If the index overflows.
   * \error ara::crypto::SecurityErrc::kContentDuplication If the index already contains the UUID.
   * \reentrant FALSE
   * \synchronous TRUE
   */
  amsr::core::Result<void> AddToUuidIndex(ara::crypto::Uuid uuid, ara::crypto::keys::SlotNumber slot_number,
                                          std::size_t index) noexcept;

  /*!
   * \brief Adds the given slot data to the crypto provider index.
   * \param[in] uuid UUID of the slot's crypto provider.
   * \param[in] slot_number Slot number of the slot.
   * \param[in] index Position in the buffer of the slot.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error ara::crypto::SecurityErrc::kInsufficientResource If the index overflows.
   * \reentrant FALSE
   * \synchronous TRUE
   */
  amsr::core::Result<void> AddToProviderIndex(ara::crypto::Uuid uuid, ara::crypto::keys::SlotNumber slot_number,
                                              std::size_t index) noexcept;

  /*!
   * \brief Adds the given slot data to the COUID index.
   * \param[in] couid COUID of the slot payload.
   * \param[in] type Type of the slot payload.
   * \param[in] slot_number Slot number of the slot.
   * \param[in] index Position in the buffer of the slot.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error ara::crypto::SecurityErrc::kInsufficientResource If the index overflows.
   * \reentrant FALSE
   * \synchronous TRUE
   */
  amsr::core::Result<void> AddToCouidIndex(ara::crypto::CryptoObjectUid couid, ara::crypto::CryptoObjectType type,
                                           ara::crypto::keys::SlotNumber slot_number, std::size_t index) noexcept;

  /*!
   * \brief Finds the COUID index entry pointing to the next greater slot number.
   * \param[in] begin Startpoint of the search.
   * \param[in] end Endpoint of the search.
   * \param[in] slot_number Reference slot number.
   * \return A pointer to the found COUID index entry.
   * \context ANY
   * \pre begin <= end
   * \threadsafe FALSE
   * \reentrant TRUE
   * \synchronous TRUE
   */
  static CouidIndexEntry* FindGreaterSlotNumber(CouidIndexEntry* begin, CouidIndexEntry* end,
                                                ara::crypto::keys::SlotNumber slot_number) noexcept;

  /*!
   * \brief Finds the COUID index entries pointing to a certain COUID.
   * \param[in] begin Startpoint of the search.
   * \param[in] end Endpoint of the search.
   * \param[in] couid Reference COUID.
   * \param[in] type Type of the slot payload.
   * \return The range of found COUID index entries.
   * \context ANY
   * \pre begin <= end
   * \threadsafe FALSE
   * \reentrant TRUE
   * \synchronous TRUE
   */
  std::pair<CouidIndexEntry*, CouidIndexEntry*> FindCouid(CouidIndexEntry* begin, CouidIndexEntry* end,
                                                          ara::crypto::CryptoObjectUid couid,
                                                          ara::crypto::CryptoObjectType type) const noexcept;

  /*!
   * \brief Removes the first COUID in \c remove_range from the index if it is associated with \c slot_num
   * \param[in] remove_range Range of CouidIndexEntries to remove
   * \param[in] slot_num SlotNumber of the slot to COUID index entry to remove.
   * \return true iff an entry was removed.
   * \context ANY
   * \pre remove_range.first <= remove_range.second
   * \threadsafe FALSE
   * \reentrant TRUE
   * \synchronous TRUE
   */
  bool RemoveCouidFromIndex(std::pair<CouidIndexEntry*, CouidIndexEntry const*> remove_range,
                            ara::crypto::keys::SlotNumber slot_num) noexcept;

  /*!
   * \brief Looks up the type of the object currently stored in the slot identified by the given dependency UUID.
   * \param[in] dep_uuid UUID of the dependency slot.
   * \param[in] dep_provider_uuid Crypto provider UUID of the dependency slot.
   * \return The object type.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error ara::crypto::SecurityErrc::kRuntimeFault Unable to derive type of dependency.
   * \reentrant TRUE
   * \synchronous TRUE
   */
  amsr::core::Result<ara::crypto::CryptoObjectType> GetDependencyType(
      ara::crypto::Uuid dep_uuid, ara::crypto::Uuid dep_provider_uuid) const noexcept;

  /*!
   * \brief Read a slot from the database
   * \param[in] handle slot handle.
   * \param[in] idx buffer index.
   * \context ANY
   * \pre -
   * \error ara::crypto::SecurityErrc::kLogicFault If the reading the slot fails
   * \error ara::crypto::SecurityErrc::kResourceFault If the index overflows
   * \error ara::crypto::SecurityErrc::kRuntimeFault Unexpected error
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   */
  amsr::core::Result<void> ReadSlotFromDataBase(KeySlotHandle const& handle, std::size_t idx) noexcept;

  /*!
   * \brief Adds the slot's data to the indexes.
   * \param[in] slot_content slot metadata.
   * \param[in] slot_number  slot number.
   * \param[in] index        buffer index.
   * \context ANY
   * \pre -
   * \error ara::crypto::SecurityErrc::kRuntimeFault If Adding the slot's UID to the UUID index fails.
   * \error ara::crypto::SecurityErrc::kResourceFault If the index overflows
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   */
  amsr::core::Result<void> AddIndexes(KeySlotContent const& slot_content, ara::crypto::keys::SlotNumber slot_number,
                                      std::size_t index) noexcept;

  /*!
   * \brief Maps a slot number to a buffer index
   * \param[in] slot_number slot number to be mapped.
   * \param[in] index buffer index.
   * \context ANY
   * \pre -
   * \error ara::crypto::SecurityErrc::kRuntimeFault If the mapping fails.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   */
  amsr::core::Result<void> MapSlotNumberToBufferIndex(ara::crypto::keys::SlotNumber slot_number,
                                                      std::size_t index) noexcept;

  /*!
   * \brief Iterate through the provider index until matching the given entry index.
   * \param[in] provider_index Provider entry index.
   * \param[in] to_be_matched_index Entry index to be matched.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   */
  static void MatchProviderIndexEntry(std::pair<UuidIndexEntry*, UuidIndexEntry*>& provider_index,
                                      std::pair<CouidIndexEntry*, CouidIndexEntry*>& to_be_matched_index) noexcept;

  /*!
   * \brief Check whether a crypto object exists for a crypto provider.
   * \param[in] couid Uid of the crypto object to find.
   * \param[in] cotype Type of the crypto object to find.
   * \param[in] provider_uuid Uid of the crypto provider to check the association of the crypto object to find with.
   * \return True if the object exists for the crypto provider.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   */
  bool CryptoObjectExists(ara::crypto::CryptoObjectUid const couid, ara::crypto::CryptoObjectType const cotype,
                          ara::crypto::Uuid const provider_uuid) noexcept;

  /*!
   * \brief Update the crypto object version stamp.
   * \details Updates the version stamp using the current time or increments it if the version is newer.
   * \param[out] couid The couid containing the version stamp to update.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   */
  static void UpdateCryptoObjectVersionStamp(ara::crypto::CryptoObjectUid& couid) noexcept;

  /*!
   * \brief Clear the slot contents.
   * \details Clearing of the key slot can be prohibited by the a registered OnClearCallback function for the owning
   *          crypto provider.
   * \param[in] slot Target slot to be cleared
   * \param[in] slot_num Slot number of the target slot
   * \pre The key storage provider is connected to a database.
   * \threadsafe FALSE
   * \error ara::crypto::SecurityErrc::kRuntimeFault Unable to update slot.
   * \reentrant FALSE
   * \synchronous TRUE
   */
  auto ClearSlot(Slot& slot, ara::crypto::keys::SlotNumber slot_num) noexcept -> amsr::core::Result<void>;

  /*!
   * \brief Report a key access security event to IDSM if security events are enabled.
   * \param[in] slot The slot for which to report the security event.
   * \return -
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   */
  auto ReportKeyAccessSecurityEventIfEnabled(Slot const& slot) const noexcept -> void;

  /*!
   * \brief Get an interator of the slot number to slot buffer index map.
   * \param[in] slot_num Number of the slot to retrieve the iterator for.
   * \return Result containing the iterator if the slot is retrievable, otherwise error.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error ara::crypto::SecurityErrc::kInvalidUsageOrder Trying to use key storage provider before connecting it to a
   * database.
   * \error ara::crypto::SecurityErrc::kUnreservedResource Trying to operate on non-existing slot.
   * \reentrant FALSE
   * \synchronous TRUE
   */
  auto GetSlotNumberIdxIterator(ara::crypto::keys::SlotNumber slot_num) const noexcept
      -> amsr::core::Result<SlotNumIdxIter>;

  /*!
   * \brief Retrieve a slot.
   * \param[in] slot_num Number of the slot to retrieve.
   * \return The retrieved slot.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error ara::crypto::SecurityErrc::kInvalidUsageOrder Trying to use key storage provider before connecting it to a
   * database.
   * \error ara::crypto::SecurityErrc::kUnreservedResource Trying to operate on non-existing slot.
   * \reentrant FALSE
   * \synchronous TRUE
   */
  auto GetSlot(ara::crypto::keys::SlotNumber slot_num) const noexcept -> amsr::core::Result<Slot const&>;

  /*!
   * \brief Retrieve a slot.
   * \param[in] slot_num Number of the slot to retrieve.
   * \return The retrieved slot.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error ara::crypto::SecurityErrc::kInvalidUsageOrder Trying to use key storage provider before connecting it to a
   * database.
   * \error ara::crypto::SecurityErrc::kUnreservedResource Trying to operate on non-existing slot.
   * \reentrant FALSE
   * \synchronous TRUE
   */
  auto GetSlot(ara::crypto::keys::SlotNumber slot_num) noexcept -> amsr::core::Result<Slot&>;

  /*!
   * \brief Create a slot-owning trusted container.
   * \param[in] slot The slot from which to create the trusted container from.
   * \return The slot-owning trusted container.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error ara::crypto::SecurityErrc::kAccessViolation Trying to call restricted method.
   * \error ara::crypto::SecurityErrc::kBusyResource Slot already opened for writing.
   * \error ara::crypto::SecurityErrc::kRuntimeFault Failed to serialize slot payload.
   * \reentrant FALSE
   * \synchronous TRUE
   */
  auto CreateOwnerTrustedContainer(Slot& slot) const noexcept
      -> amsr::core::Result<ara::crypto::TrustedContainer::Uptr>;

  /*!
   * \brief Check whether the current client process can perform a certain operation on a slot.
   * \param[in] desired_operation The desired operation to check for.
   * \param[in] slot The slot on which to check the desired operation.
   * \return Whether the operation can be performed on the slot.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   */
  auto CurrentUserCan(crypto::server::config::Operation desired_operation, Slot const& slot) const noexcept -> bool;

  crypto::common::util::Logger logger_; /*!< Logging instance */

  crypto::server::CallContextRegistry const&
      call_context_registry_; /*!< Registry providing information about the current caller */

  std::unique_ptr<crypto::server::config::AccessControlConfig>
      access_control_config_; /*!< The access control configuration */

  // VECTOR NC AutosarC++17_10-A12.1.3: MD_CRYPTO_AutosarC++17_10-A12.1.3
  bool access_control_is_enforced_; /*!< Indicates if access control is enforced. */

  std::unique_ptr<SecurityEventsReporterInterface> security_events_reporter_; /*!< The security events reporter */

  // VECTOR NC AutosarC++17_10-A12.1.3: MD_CRYPTO_AutosarC++17_10-A12.1.3
  bool security_events_enabled_; /*!< Indicates if security events are reported. */

  std::shared_ptr<JsonKeyDatabase> key_db_; /*!< Connected key database. */

  // VECTOR NC AutosarC++17_10-A12.1.3: MD_CRYPTO_AutosarC++17_10-A12.1.3
  std::size_t slot_buffer_size_; /*!< Size of the buffer holding the slot data. */

  ara::core::Vector<Slot> slot_buffer_; /*!< Buffer holding the slot data. */

  vac::container::StaticMap<JsonKeyDatabase::SlotNumber, std::size_t>
      slot_number_idx_; /*!< Index mapping from slot number to buffer index. */

  // VECTOR NC AutosarC++17_10-A12.1.3: MD_CRYPTO_AutosarC++17_10-A12.1.3
  std::size_t uuid_idx_length_; /*!< Number of entries in the slot UUID index. */

  std::unique_ptr<ara::core::Vector<UuidIndexEntry>> uuid_idx_; /*!< Slot UUID index. */

  // VECTOR NC AutosarC++17_10-A12.1.3: MD_CRYPTO_AutosarC++17_10-A12.1.3
  std::size_t provider_idx_length_; /*!< Number of entries in the crypto provider UUID index. */

  std::unique_ptr<ara::core::Vector<UuidIndexEntry>> provider_idx_; /*!< Crypto provider UUID index. */

  // VECTOR NC AutosarC++17_10-A12.1.3: MD_CRYPTO_AutosarC++17_10-A12.1.3
  std::size_t couid_idx_length_; /*!< Number of entries in the object COUID index. */

  std::unique_ptr<ara::core::Vector<CouidIndexEntry>> couid_idx_; /*!< Object COUID index. */

  // VECTOR NC AutosarC++17_10-A12.1.3: MD_CRYPTO_AutosarC++17_10-A12.1.3
  bool connected_; /*!< Indicates if the key storage provider is connected to a database. */

  std::weak_ptr<CryptoProviderFactory>
      crypto_provider_factory_; /*!< ProviderFactory used to get access to concrete crypto
                               providers that are also used by the server runtime */

  amsr::core::Map<ara::crypto::Guid, OnClearCallback>
      on_clear_callback_functions_; /*!< Registered callback functions to notify when a key slot was cleared */

  FRIEND_TEST(UT__server__keys__CompositeKeyStorageProvider,
              AddIndexes__InsufficientResource); /*!< Friend declaration for testing. */
  FRIEND_TEST(UT__server__keys__CompositeKeyStorageProvider,
              MatchProviderIndexEntry); /*!< Friend declaration for testing. */
  FRIEND_TEST(UT__server__keys__CompositeKeyStorageProvider,
              SaveCopy__Reinsert); /*!< Friend declaration for testing. */
  FRIEND_TEST(UT__server__keys__CompositeKeyStorageProvider,
              Store__Load__Clear); /*!< Friend declaration for testing. */
  FRIEND_TEST(UT__server__keys__CompositeKeyStorageProvider,
              SaveCopy__Duplicate); /*!< Friend declaration for testing. */
  FRIEND_TEST(UT__server__keys__CompositeKeyStorageProvider,
              SaveCopy__RepeatedlyStore); /*!< Friend declaration for testing. */
  FRIEND_TEST(UT__server__keys__CompositeKeyStorageProvider,
              SaveCopy__Overwrite__FindObject); /*!< Friend declaration for testing. */
  FRIEND_TEST(UT__server__keys__CompositeKeyStorageProvider,
              FindObjectReinsert); /*!< Friend declaration for testing. */
  FRIEND_TEST(UT__softwareprovider__GeneratePrivateKeyTest,
              persistence__related); /*!< Friend declaration for testing. */
  FRIEND_TEST(UT__server__keys__CompositeKeyStorageProvider,
              UpdateContent_key_db_update_failed_slot_buffer_overflow);       /*!< Friend declaration for testing. */
  FRIEND_TEST(CT__CompositeKeyStorageProvider, OpenAsUser__Security_Events);  /*!< Friend declaration for testing. */
  FRIEND_TEST(CT__CompositeKeyStorageProvider, OpenAsOwner__Security_Events); /*!< Friend declaration for testing. */
  FRIEND_TEST(CT__CompositeKeyStorageProvider, SaveCopy__Security_Events);    /*!< Friend declaration for testing. */
  FRIEND_TEST(CT__CompositeKeyStorageProvider, Clear__Security_Events);       /*!< Friend declaration for testing. */
  FRIEND_TEST(UT__server__keys__CompositeKeyStorageProvider, KeySlots);       /*!< Friend declaration for testing. */
};

}  // namespace keys
}  // namespace server
}  // namespace crypto

#endif  // LIB_LIBSERVER_INCLUDE_CRYPTO_SERVER_KEYS_COMPOSITE_KEY_STORAGE_PROVIDER_H_
