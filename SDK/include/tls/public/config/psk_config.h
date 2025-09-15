/*!********************************************************************************************************************
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
/*!        \file  psk_config.h
 *        \brief  This file contains the definition for PskConfig.
 *
 *********************************************************************************************************************/
#ifndef LIB_LIBTLS_INCLUDE_TLS_PUBLIC_CONFIG_PSK_CONFIG_H_
#define LIB_LIBTLS_INCLUDE_TLS_PUBLIC_CONFIG_PSK_CONFIG_H_

#include <functional>  // less
#include <memory>

#include "amsr/core/map.h"
#include "amsr/core/string.h"
#include "ara/crypto/common/guid.h"

namespace amsr {
namespace tls {
namespace config {

/*!
 * \brief The PskConfig class comprises a set of configurable parameters for providers using a PSK cipher suite.
 * \vprivate Vector product private API.
 * \trace DSGN-SecCom-PSK
 */
class PskConfig {
 public:
  /*! \brief Maximum size of the server hint. */
  static constexpr std::size_t kPskHintMaxSize{256U};

  /*! \brief Type alias for a pre-shared key hint. */
  using PskHint = amsr::core::String;

  /*! \brief Type alias for a pre-shared key identity. */
  using PskId = amsr::core::String;

  /*! \brief Type alias for a unique pointer to the pre-shared key configuration. */
  using Uptr = std::unique_ptr<PskConfig>;

  /*! \brief Default destructor. */
  virtual ~PskConfig() noexcept = default;

  /*! \brief Default copy constructor. */
  // VECTOR NL AutosarC++17_10-A12.8.6: MD_libseccom_AutosarC++17_10-A12.8.6_Shall_be_declared_protected
  PskConfig(PskConfig const &) = default;

  /*!
   * \brief Default constructor.
   * \vprivate Vector product private API.
   */
  PskConfig() noexcept = default;

  PskConfig(PskConfig &&) = delete;
  PskConfig &operator=(PskConfig const &) & = delete;
  PskConfig &operator=(PskConfig &&) & = delete;

  /*!
   * \brief Constructor.
   * \param[in] hint A string containing the PSK identity hint.
   * \param[in] id A string containing the PSK identity.
   * \param[in] uuid A string containing the UUID.
   * \vprivate This function is component private.
   */
  PskConfig(PskHint hint, PskId id, amsr::core::StringView const uuid) noexcept;

  /*!
   * \brief Constructor.
   * \param[in] hint A string containing the PSK identity hint.
   * \param[in] id A string containing the PSK identity.
   * \param[in] uuid An object containing the UUID.
   * \vprivate Vector product private API.
   */
  PskConfig(PskHint hint, PskId id, ara::crypto::Uuid uuid) noexcept;

  /*!
   * \brief Set the server PSK identity hint.
   * \param[in] hint A string containing the PSK identity hint.
   * \context Init
   * \pre -
   * \threadsafe False
   * \reentrant False
   * \synchronous True
   * \vprivate Vector product private API.
   */
  void SetHint(PskHint hint) noexcept;

  /*!
   * \brief Add an entry in the map from ID hint to ID.
   * \param[in] hint A string containing the PSK identity hint.
   * \param[in] id A string containing the PSK identity.
   * \context Init
   * \pre -
   * \threadsafe False
   * \reentrant False
   * \synchronous True
   * \vprivate Vector product private API.
   */
  void AssociateHintWithId(PskHint hint, PskId id) noexcept;

  /*!
   * \brief Add an entry in the map from ID to UUID.
   * \param[in] id A string containing the PSK identity.
   * \param[in] uuid An object containing the UUID.
   * \context Init
   * \pre -
   * \threadsafe False
   * \reentrant False
   * \synchronous True
   * \vprivate Vector product private API.
   */
  void AssociateIdWithUuid(PskId id, ara::crypto::Uuid uuid) noexcept;

  /*!
   * \brief Add an entry in the map from ID to UUID.
   * \param[in] id A string containing the PSK identity.
   * \param[in] uuid A string representing the UUID.
   * \context Init
   * \pre -
   * \threadsafe False
   * \reentrant False
   * \synchronous True
   * \vprivate Vector product private API.
   */
  void AssociateIdWithUuid(PskId id, amsr::core::StringView const uuid) noexcept;

  /*!
   * \brief Get the PSK UUID corresponding to the given PSK identity and hint.
   * \param[in] id The PSK identity the UUID shall be fetched for.
   * \param[in] hint The PSK identity hint the UUID shall be fetched for.
   * \param[in] is_server True if this communication provider is a server.
   * \return The PSK UUID corresponding to the PSK identity and hint. The zero UUID if no UUID can be found.
   * \context Any
   * \pre The PSK configuration should be initialized.
   * \threadsafe True
   * \reentrant False
   * \synchronous True
   * \vprivate This function is component private.
   */
  virtual ara::crypto::Uuid GetPskUuid(amsr::core::StringView id, amsr::core::StringView hint,
                                       bool is_server) const noexcept;

  /*!
   * \brief Get the PSK identity corresponding to the given PSK identity hint.
   * \param[in] hint The PSK identity hint the PSK identity shall be fetched for.
   * \return The PSK identity corresponding to the given PSK identity hint.
   * \context Any
   * \pre The PSK configuration should be initialized.
   * \threadsafe True
   * \reentrant False
   * \synchronous True
   * \vprivate This function is component private.
   */
  virtual amsr::core::StringView GetPskIdentity(amsr::core::StringView hint) const noexcept;

  /*!
   * \brief Get the server identity hint.
   * \return A string containing the server identity hint.
   * \context Any
   * \pre The PSK configuration should be initialized.
   * \threadsafe True
   * \reentrant True
   * \synchronous True
   * \vprivate This function is component private.
   */
  amsr::core::StringView GetServerHint() const noexcept;

  /*!
   * \brief Whether the PSK configuration has been set or not.
   * \return True if the PSK configuration was set, false otherwise.
   * \context Any
   * \threadsafe True
   * \reentrant True
   * \synchronous True
   * \vprivate Vector product private API.
   */
  bool IsEmpty() const noexcept;

 private:
  /*!
   * \brief The PSK identity hint which a server provider sends to a client.
   */
  PskHint server_hint_{};

  /*!
   * \brief The server hint to PSK identity mapping. A client uses this to find the matching identity.
   * \details It uses a "transparent" comparator to allow for lookup with StringView.
   */
  amsr::core::Map<PskHint, PskId, std::less<>> hint_to_id_map_{};

  /*!
   * \brief The PSK identity to PSK UUID mapping for a pre shared session key. Used by both client and server.
   * \details It uses a "transparent" comparator to allow for lookup with StringView.
   */
  amsr::core::Map<PskId, ara::crypto::Uuid, std::less<>> id_to_uuid_map_{};
};

}  // namespace config
}  // namespace tls
}  // namespace amsr

#endif  // LIB_LIBTLS_INCLUDE_TLS_PUBLIC_CONFIG_PSK_CONFIG_H_
