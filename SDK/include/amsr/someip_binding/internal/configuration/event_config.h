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
/*!        \file  event_config.h
 *        \brief  Event configuration class.
 *        \unit   SomeIpBinding::SomeIpBinding::Configuration::JsonParser::JsonConfigTypes
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_BINDING_INCLUDE_AMSR_SOMEIP_BINDING_INTERNAL_CONFIGURATION_EVENT_CONFIG_H_
#define LIB_SOMEIP_BINDING_INCLUDE_AMSR_SOMEIP_BINDING_INTERNAL_CONFIGURATION_EVENT_CONFIG_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/core/optional.h"
#include "amsr/someip_binding/internal/configuration/config_types.h"
#include "someip-protocol/internal/someip_posix_types.h"

namespace amsr {
namespace someip_binding {
namespace internal {
namespace configuration {

/*!
 * \brief Event configuration.
 * \details Configuration class storing the configuration parameters for an event.
 *          Stored configuration parameters:
 *          - EventId
 *          - Shortname
 *          - Serialization [optional]
 *          - DisableE2eCheck [optional]
 *          - DisableSessionHandling [optional]
 *          - PduHeaderExtensionTx [optional]
 *          - PduHeaderExtensionRx [optional]
 *          - E2EUpdateBitPosition [optional]
 *          - E2EProtectedOffset [optional]
 *          - HasSignalBasedE2eRange [optional]
 *
 */
class EventConfig final {
 public:
  /*!
   * \brief Enumeration defining the serialization mode.
   */
  enum class Serialization : std::uint8_t {
    signal_based, /*!< Use signal based serialization. */
    someip        /*!< Use someip serialization. */
  };

  /*! Name alias for event identifier type. */
  using EventId = ::amsr::someip_protocol::internal::EventId;

  /*! Type-alias for the disable E2E check type. */
  using DisableE2eCheck = bool;

  /*! Type-alias for the disable session handling type. */
  using DisableSessionHandling = bool;

  /*!
   * \brief Length of pdu header extension in bytes for signal based messages.
   */
  static constexpr std::size_t kPduHeaderExtnLength{8U};

  /*! Type-alias for the pdu header extension tx array. */
  using PduHeaderExtensionTx = amsr::core::Array<std::uint8_t, kPduHeaderExtnLength>;

  /*! Type-alias for the pdu header extension rx flag. */
  using PduHeaderExtensionRx = bool;

  /*! Type-alias for update bit position*/
  using E2EUpdateBitPosition = std::size_t;

  /*! Type-alias for the e2e protected offset*/
  using E2EProtectedOffset = std::size_t;

  /*! Type-alias for the has signal based e2e range*/
  using HasSignalBasedE2eRange = bool;

  /*!
   * \brief Set the id of the event.
   * \details The id defines the id of the event.
   * \param[in] id The event id.
   * \pre -
   * \context ANY
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE.
   * \spec
   *   requires true;
   * \endspec
   */
  void SetEventId(EventId const id) noexcept;

  /*!
   * \brief Get the id of the event.
   * \return The id of the event.
   * \pre -
   * \context ANY
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  EventId GetEventId() const noexcept;

  /*!
   * \brief Sets the shortname.
   * \param shortname The shortname.
   *
   * \pre -
   * \context ANY
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  void SetShortname(Shortname const& shortname) noexcept;

  /*!
   * \brief  Gets the shortname.
   * \return The shortname.
   *
   * \pre -
   * \context ANY
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  Shortname const& GetShortname() const noexcept;

  /*!
   * \brief Set the serialization.
   * \details The serialization defines the serialization of the event.
   * \param[in] serialization The serialization.
   * \pre -
   * \context ANY
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE.
   * \spec
   *   requires true;
   * \endspec
   */
  void SetSerialization(Serialization const serialization) noexcept;

  /*!
   * \brief Get the serialization.
   * \return The serialization.
   * \pre -
   * \context ANY
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  Serialization GetSerialization() const noexcept;

  /*!
   * \brief Set the disable E2E check value.
   * \param[in] disable_e2e_check The disable E2E check value.
   * \pre -
   * \context ANY
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE.
   * \spec
   *   requires true;
   * \endspec
   */
  void SetDisableE2eCheck(DisableE2eCheck const disable_e2e_check) noexcept;

  /*!
   * \brief Get the disable E2E check value.
   * \return The disable E2E check value.
   * \pre -
   * \context ANY
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  amsr::core::Optional<DisableE2eCheck> GetDisableE2eCheck() const noexcept;

  /*!
   * \brief Set the disable session handling value.
   * \param[in] disable_session_handling The disable session handling value.
   * \pre -
   * \context ANY
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE.
   * \spec
   *   requires true;
   * \endspec
   */
  void SetDisableSessionHandling(DisableSessionHandling const disable_session_handling) noexcept;

  /*!
   * \brief Get the disable session handling value.
   * \return The disable session handling value.
   * \pre -
   * \context ANY
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  amsr::core::Optional<DisableSessionHandling> GetDisableSessionHandling() const noexcept;

  /*!
   * \brief Set the Pdu header extension for Tx.
   * \param[in] pdu_hdr_extn_tx The field in the pdu header extension.
   * \pre -
   * \context ANY
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE.
   * \spec
   *   requires true;
   * \endspec
   */
  void SetPduHeaderExtensionTxFields(PduHeaderExtensionTx const& pdu_hdr_extn_tx) noexcept;

  /*!
   * \brief Get the Pdu header extension for Tx.
   * \return The Pdu header extension for Tx.
   * \pre -
   * \context ANY
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  amsr::core::Optional<PduHeaderExtensionTx> GetPduHeaderExtensionTxFields() const noexcept;

  /*!
   * \brief Set the Pdu header extension flag for Rx.
   * \param[in] pdu_hdr_extn_rx The field in the pdu header extension.
   * \pre -
   * \context ANY
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE.
   * \spec
   *   requires true;
   * \endspec
   */
  void SetPduHeaderExtensionRxFlag(PduHeaderExtensionRx const& pdu_hdr_extn_rx) noexcept;

  /*!
   * \brief Get the E2E update bit position
   * \return The update bit position
   * \pre -
   * \context ANY
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  ara::core::Optional<E2EUpdateBitPosition> GetE2EUpdateBitPosition() const noexcept;

  /*!
   * \brief Set the E2E update bit position
   * \param[in] e2e_update_bit_position The update bit position
   * \pre -
   * \context Init
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  void SetE2EUpdateBitPosition(E2EUpdateBitPosition const e2e_update_bit_position) noexcept;

  /*!
   * \brief Get the Pdu header extension flag for Rx.
   * \return The Pdu header extension flag for Rx.
   * \pre -
   * \context ANY
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  amsr::core::Optional<PduHeaderExtensionRx> GetPduHeaderExtensionRxFlag() const noexcept;

  /*!
   * \brief Set the E2E protected offset
   * \param[in] e2e_protected_offset The e2e protected offset
   * \pre -
   * \context Init
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  void SetE2EProtectedOffset(E2EProtectedOffset const e2e_protected_offset) noexcept;

  /*!
   * \brief Get the e2e protected offset.
   * \return The e2e protected offset.
   * \pre -
   * \context ANY
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  amsr::core::Optional<E2EProtectedOffset> GetE2EProtectedOffset() const noexcept;

  /*!
   * \brief Set the has signal based e2e range value.
   * \param[in] has_signal_based_e2e_range The has signal based e2e range value.
   * \pre -
   * \context Init
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  void SetHasSignalBasedE2eRange(HasSignalBasedE2eRange const has_signal_based_e2e_range) noexcept;

  /*!
   * \brief Get the has signal based e2e range value.
   * \return The has signal based e2e range value.
   * \pre -
   * \context ANY
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  amsr::core::Optional<HasSignalBasedE2eRange> GetHasSignalBasedE2eRange() const noexcept;

 private:
  EventId event_id_{};                                                        /*!< The event identifier. */
  Shortname shortname_{};                                                     /*!< The Short name  */
  Serialization serialization_{Serialization::someip};                        /*!< The serialization. */
  amsr::core::Optional<DisableE2eCheck> disable_e2e_check_{};                 /*!< The disable E2E check value */
  amsr::core::Optional<DisableSessionHandling> disable_session_handling_{};   /*!< The disable session handling value */
  amsr::core::Optional<PduHeaderExtensionTx> pdu_hdr_extn_tx_{};              /*!< The Pdu header extension tx array. */
  amsr::core::Optional<PduHeaderExtensionRx> pdu_hdr_extn_rx_{};              /*!< The Pdu header extension rx flag. */
  amsr::core::Optional<E2EUpdateBitPosition> e2e_update_bit_position_{};      /*!< The E2E update bit*/
  amsr::core::Optional<E2EProtectedOffset> e2e_protected_offset_{};           /*!< The E2e protected offset*/
  amsr::core::Optional<HasSignalBasedE2eRange> has_signal_based_e2e_range_{}; /*!< The E2e protected offset*/
};

}  // namespace configuration
}  // namespace internal
}  // namespace someip_binding
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_INCLUDE_AMSR_SOMEIP_BINDING_INTERNAL_CONFIGURATION_EVENT_CONFIG_H_
