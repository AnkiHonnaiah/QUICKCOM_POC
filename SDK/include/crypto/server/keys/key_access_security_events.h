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
/*!        \file  key_access_security_events.h
 *        \brief  Holds all crypto server key access security events and its reporting functions.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBSERVER_INCLUDE_CRYPTO_SERVER_KEYS_KEY_ACCESS_SECURITY_EVENTS_H_
#define LIB_LIBSERVER_INCLUDE_CRYPTO_SERVER_KEYS_KEY_ACCESS_SECURITY_EVENTS_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/core/array.h"
#include "amsr/core/instance_specifier.h"
#include "amsr/core/string.h"
#include "amsr/ipc/credentials.h"
#include "ara/crypto/common/base_id_types.h"
#include "ara/crypto/keys/elementary_types.h"
#include "ara/idsm/common.h"
#include "crypto/server/event_reporter_interface.h"
#include "crypto/server/security_events_reporter_interface.h"
#include "vac/container/string_literals.h"

namespace crypto {
namespace server {
namespace keys {

// VECTOR NC AutosarC++17_10-M7.3.6: MD_CRYPTO_AutosarC++17_10-M7.3.6_sv
/*! Introduce string literal for StringView creation */
using vac::container::literals::operator"" _sv;

// VECTOR NC AutosarC++17_10-A3.3.2: MD_CRYPTO_AutosarC++17_10-A3.3.2_not_POD_obj_static
/*!
 * \brief Default key access denied security event port path
 */
static constexpr amsr::core::StringView const kKeyAccessDeniedPath{
    "amsr_crypto_daemon/CryptoDaemonRootSwc/security_event_key_access_denied"_sv};

/*!
 * \brief Reporter for the crypto server key security events
 * \vprivate Component Private
 */
class KeyAccessSecurityEvents : public SecurityEventsReporterInterface {
 public:
  /*!
   * \brief Size of the serialized slot uuid.
   * \vprivate Component Private
   */
  static constexpr size_t const kSerializedSlotUuidSize{ara::crypto::LogicalSlotUid::kUuidByteLength};

  /*!
   * \brief Size of the serialized user id.
   * \vprivate Component Private
   */
  static constexpr size_t const kSerializedUserIdSize{sizeof(amsr::ipc::Credentials)};

  /*!
   * \brief Size of the serialized context data.
   * \vprivate Component Private
   */
  static constexpr size_t const kSerializedContextDataSize{kSerializedSlotUuidSize + kSerializedUserIdSize};

  /*!
   * \brief Data type representing the serialized user id.
   * \vprivate Component Private
   */
  using SerializedUserId = amsr::core::Array<std::uint8_t, kSerializedUserIdSize>;

  /*!
   * \brief Data type representing the serialized context data.
   * \vprivate Component Private
   */
  using SerializedContextData = amsr::core::Array<std::uint8_t, kSerializedContextDataSize>;

  /*!
   * \brief Deleted default constructor
   * \vprivate Component Private
   */
  KeyAccessSecurityEvents() = delete;

  // KeyAccessSecurityEvents is not copy assignable
  auto operator=(KeyAccessSecurityEvents const& /*other*/) & -> KeyAccessSecurityEvents& = delete;

  // KeyAccessSecurityEvents is not move assignable
  auto operator=(KeyAccessSecurityEvents&& /*other*/) & noexcept -> KeyAccessSecurityEvents& = delete;

  // KeyAccessSecurityEvents is not copy constructable
  KeyAccessSecurityEvents(KeyAccessSecurityEvents const& /*other*/) = delete;

  // KeyAccessSecurityEvents is not move constructable
  KeyAccessSecurityEvents(KeyAccessSecurityEvents&& /*other*/) noexcept = delete;

  /*!
   * \brief Destroy this instance.
   * \vprivate Component Private
   */
  ~KeyAccessSecurityEvents() noexcept override = default;

  /*!
   * \brief Constructor a new KeyAccessSecurityEvents instance.
   * \param[in] key_access_denied_instance_specifier The instance specifier for the security event keyAccessDenied.
   * \vprivate Component Private
   */
  explicit KeyAccessSecurityEvents(amsr::core::InstanceSpecifier const& key_access_denied_instance_specifier) noexcept;

  /*!
   * \brief Constructor a new KeyAccessSecurityEvents instance.
   * \param[in] key_access_denied_reporter The event reporter for the security event keyAccessDenied.
   * \pre key_access_denied_reporter != nullptr
   * \vprivate Component Private
   */
  explicit KeyAccessSecurityEvents(std::unique_ptr<EventReporterInterface> key_access_denied_reporter) noexcept;

  /*!
   * \brief Report the keyAccessDenied security event to the IDSM.
   * \param[in] context_data The data to report as context.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error ara::crypto::SecurityErrc::kRuntimeFault Unable to report event.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto ReportKeyAccessDenied(ContextData const& context_data) noexcept -> void override;

  /*!
   * \brief Get the counter of how often the security event keyAccessDenied was reported.
   * \return The report counter of the keyAccessDenied security event.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto GetKeyAccessDeniedEventCount() const noexcept -> ara::idsm::CountType { return key_access_denied_count_; };

  /*!
   * \brief Serialize context data used in reporting the events.
   * \param[in] context_data The data to serialize and to be used to report as context.
   * \return The serialized context data.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  static auto SerializeContextData(ContextData const& context_data) noexcept -> SerializedContextData;

 protected:
  /*!
   * \brief Directly report the key access allowed event to IDSM.
   * \return -
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  virtual auto ReportKeyAccessDeniedPlain() noexcept -> void;

 private:
  /*!
   * \brief Serialize a user id using big endian byte order.
   * \param[in] user_id The user id to serialize using big endian byte order.
   * \return The serialized version of the passed process user id
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  static auto SerializeUserIdBigEndian(amsr::ipc::Credentials const& user_id) noexcept -> SerializedUserId;

  /*!
   * \brief Get a timestamp of the current time (microseconds since epoch).
   * \return A timestamp of the current time.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  static auto CurrentTime() noexcept -> std::uint64_t;

  std::unique_ptr<EventReporterInterface>
      key_access_denied_reporter_;                 /*!< Reporter for the security event keyAccessDenied. */
  ara::idsm::CountType key_access_denied_count_{}; /*!< Counter for the security event keyAccessDenied. */

  SerializedContextData serialized_context_data_; /*!< Serialized context data containing a process user id in big
                                                       endian byte order followed by a slot uuid. */
};
}  // namespace keys
}  // namespace server
}  // namespace crypto

#endif  // LIB_LIBSERVER_INCLUDE_CRYPTO_SERVER_KEYS_KEY_ACCESS_SECURITY_EVENTS_H_
