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
/*!        \file  e2e_props_config.h
 *        \brief  Configuration for the E2EProps.
 *        \unit   SomeIpBinding::SomeIpBinding::Configuration::JsonParser::JsonConfigTypes
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_BINDING_INCLUDE_AMSR_SOMEIP_BINDING_INTERNAL_CONFIGURATION_E2E_PROPS_CONFIG_H_
#define LIB_SOMEIP_BINDING_INCLUDE_AMSR_SOMEIP_BINDING_INTERNAL_CONFIGURATION_E2E_PROPS_CONFIG_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstdint>
#include "amsr/core/array.h"
#include "amsr/e2e/profiles/data_id_mode_enum.h"
#include "amsr/e2e/profiles/end2end_event_protection_props.h"
#include "amsr/someip_binding/internal/configuration/config_types.h"
#include "amsr/someip_binding/internal/configuration/event_config.h"
#include "someip-protocol/internal/someip_posix_types.h"

namespace amsr {
namespace someip_binding {
namespace internal {
namespace configuration {

// VECTOR Next Construct Metric-OO.WMC.One: MD_SOMEIPBINDING_Metric-OO.WMC.One_methods_per_class
/*!
 * \brief E2EProps configuration.
 * \details Configuration class storing the configuration parameters for the E2EProps.
 *          Stored configuration parameters:
 *          - EventId
 *          - DataId
 *          - DataLength
 *          - MinDataLength
 *          - MaxDataLength
 *          - CounterOffset
 *          - CrcOffset
 *          - HeaderOffset
 *          - DataIdNibbleOffset
 *          - DataIdMode
 *          - MaxNoNewOrRepeatedData
 *          - SyncCounterInit
 *          - DataIdList
 *          - E2EProfileShortname
 */
class E2EPropsConfig final {
 public:
  /*! Type-alias for the E2E event ID. */
  using EventId = ::amsr::someip_protocol::internal::EventId;

  /*! Type-alias for the E2E data ID. */
  using DataId = std::uint32_t;

  /*! Type-alias for the E2E data length. */
  using DataLength = std::uint16_t;

  /*! Type-alias for the E2E min data length. */
  using MinDataLength = std::uint32_t;

  /*! Type-alias for the E2E max data length. */
  using MaxDataLength = std::uint32_t;

  /*! Type-alias for the E2E counter offset. */
  using CounterOffset = std::uint16_t;

  /*! Type-alias for the E2E CRC offset. */
  using CrcOffset = std::uint16_t;

  /*! Type-alias for the E2E header offset. */
  using HeaderOffset = std::uint16_t;

  /*! Type-alias for the E2E data ID nibble offset. */
  using DataIdNibbleOffset = std::uint16_t;

  /*! Type-alias for the E2E data ID mode. */
  using DataIdMode = ::amsr::e2e::profiles::DataIdModeEnum;

  /*! Type-alias for the E2E max no new or repeated data. */
  using MaxNoNewOrRepeatedData = std::uint8_t;

  /*! Type-alias for the E2E sync counter init. */
  using SyncCounterInit = std::uint8_t;

  /*! Type-alias for the E2E data id list. */
  using DataIdList = amsr::core::Array<std::uint8_t, 16>;

  /*! Type-alias for the E2E profile shortname. */
  using E2EProfileShortname = ::amsr::someip_binding::internal::configuration::Shortname;

  /*!
   * \brief Default value for the E2E data ID mode.
   */
  static constexpr ::amsr::e2e::profiles::DataIdModeEnum kDataIdModeDefaultValue{
      ::amsr::e2e::profiles::DataIdModeEnum::kBoth};

  /*!
   * \brief Default constructor.
   * \spec
   *   requires true;
   * \endspec
   */
  E2EPropsConfig() noexcept = default;

  /*!
   * \brief Default destructor.
   * \spec
   *   requires true;
   * \endspec
   */
  // VCA_SOMEIPBINDING_ACCESSING_OBJECT_POSSIBLY_INVALID
  ~E2EPropsConfig() noexcept = default;

  /*!
   * \brief Default copy constructor.
   * \spec
   *   requires true;
   * \endspec
   */
  // VCA_SOMEIPBINDING_ACCESSING_OBJECT_POSSIBLY_INVALID
  E2EPropsConfig(E2EPropsConfig const&) noexcept = default;

  /*!
   * \brief Default move constructor.
   * \spec
   *   requires true;
   * \endspec
   */
  // VCA_SOMEIPBINDING_ACCESSING_OBJECT_POSSIBLY_INVALID
  E2EPropsConfig(E2EPropsConfig&&) noexcept = default;

  E2EPropsConfig& operator=(E2EPropsConfig const&) & = delete;

  /*!
   * \brief Default move assignment.
   * \return Reference to the move-assigned E2EPropsConfig.
   * \spec
   *   requires true;
   * \endspec
   */
  E2EPropsConfig& operator=(E2EPropsConfig&&) & noexcept = default;

  /*!
   * \brief Set the id of the event.
   * \details The id defines the id of the event.
   * \param[in] id The event id.
   * \pre -
   * \context ANY
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
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
   * \brief Set the id of the data.
   * \details The id defines the id of the data.
   * \param[in] id The data id.
   * \pre -
   * \context ANY
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  void SetDataId(DataId const id) noexcept;

  /*!
   * \brief Get the id of the data.
   * \return The id of the data.
   * \pre -
   * \context ANY
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  DataId GetDataId() const noexcept;

  /*!
   * \brief Set the length of the data.
   * \details The length defines the length of the data.
   * \param[in] length The data length.
   * \pre -
   * \context ANY
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  void SetDataLength(DataLength const length) noexcept;

  /*!
   * \brief Get the length of the data.
   * \return The length of the data.
   * \pre -
   * \context ANY
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  DataLength GetDataLength() const noexcept;

  /*!
   * \brief Set the minimum length of the data.
   * \details The length defines the minimum length of the data.
   * \param[in] length The minimum data length.
   * \pre -
   * \context ANY
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  void SetMinDataLength(MinDataLength const length) noexcept;

  /*!
   * \brief Get the minimum length of the data.
   * \return The minimum length of the data.
   * \pre -
   * \context ANY
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  MinDataLength GetMinDataLength() const noexcept;

  /*!
   * \brief Set the maximum length of the data.
   * \details The length defines the maximum length of the data.
   * \param[in] length The maximum data length.
   * \pre -
   * \context ANY
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  void SetMaxDataLength(MaxDataLength const length) noexcept;

  /*!
   * \brief Get the maximum length of the data.
   * \return The maximum length of the data.
   * \pre -
   * \context ANY
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  MaxDataLength GetMaxDataLength() const noexcept;

  /*!
   * \brief Set the counter offset.
   * \details The offset defines the position of the counter.
   * \param[in] offset The counter offset.
   * \pre -
   * \context ANY
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  void SetCounterOffset(CounterOffset const offset) noexcept;

  /*!
   * \brief Get the counter offset.
   * \return The counter offset.
   * \pre -
   * \context ANY
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  CounterOffset GetCounterOffset() const noexcept;

  /*!
   * \brief Set the CRC offset.
   * \details The offset defines the position of the checksum.
   * \param[in] offset The CRC offset.
   * \pre -
   * \context ANY
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  void SetCrcOffset(CrcOffset const offset) noexcept;

  /*!
   * \brief Get the CRC offset.
   * \return The CRC offset.
   * \pre -
   * \context ANY
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  CrcOffset GetCrcOffset() const noexcept;

  /*!
   * \brief Set the Header offset.
   * \details The Header offset defines the position of the e2e header.
   * \param[in] offset The Header offset.
   * \pre -
   * \context ANY
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  void SetHeaderOffset(HeaderOffset const offset) noexcept;

  /*!
   * \brief Get the offset.
   * \return The offset.
   * \pre -
   * \context ANY
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  HeaderOffset GetHeaderOffset() const noexcept;

  /*!
   * \brief Set the data ID nibble offset.
   * \details The offset defines the position of the data ID nibble.
   * \param[in] offset The data ID nibble offset.
   * \pre -
   * \context ANY
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  void SetDataIdNibbleOffset(DataIdNibbleOffset const offset) noexcept;

  /*!
   * \brief Get the data ID nibble offset.
   * \return The data ID nibble offset.
   * \pre -
   * \context ANY
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  DataIdNibbleOffset GetDataIdNibbleOffset() const noexcept;

  /*!
   * \brief Set the data ID mode.
   * \details This attribute describes the inclusion mode that is used to
   * include the implicit two-byte Data ID in the one-byte CRC.
   * \param[in] mode The data ID mode.
   * \pre -
   * \context ANY
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  void SetDataIdMode(::amsr::e2e::profiles::DataIdModeEnum const mode) noexcept;

  /*!
   * \brief Get the data ID mode.
   * \return The data ID mode.
   * \pre -
   * \context ANY
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  ::amsr::e2e::profiles::DataIdModeEnum GetDataIdMode() const noexcept;

  /*!
   * \brief Set the maximum allowed failed counter checks.
   * \details The maximum allowed amount of consecutive failed counter checks.
   * \param[in] value The maximum allowed failed counter checks.
   * \pre -
   * \context ANY
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  void SetMaxNoNewOrRepeatedData(MaxNoNewOrRepeatedData const value) noexcept;

  /*!
   * \brief Get maximum allowed failed counter checks.
   * \return The maximum allowed failed counter checks.
   * \pre -
   * \context ANY
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  MaxNoNewOrRepeatedData GetMaxNoNewOrRepeatedData() const noexcept;

  /*!
   * \brief Set the number of checks.
   * \details Number of checks required for validating the consistency of the counter.
   * \param[in] value The number of checks.
   * \pre -
   * \context ANY
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  void SetSyncCounterInit(SyncCounterInit const value) noexcept;

  /*!
   * \brief Get the number of checks.
   * \return The number of checks.
   * \pre -
   * \context ANY
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  SyncCounterInit GetSyncCounterInit() const noexcept;

  /*!
   * \brief Set the data id list.
   * \details 16 8 bits values, linked to Counter value.
   * \param[in] value The number of checks.
   * \pre -
   * \context ANY
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  void SetDataIdList(DataIdList const& value) noexcept;

  /*!
   * \brief Get the data id list.
   * \return The data id list.
   * \pre -
   * \context ANY
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  DataIdList const& GetDataIdList() const noexcept;

  /*!
   * \brief Set the e2e profile shortname.
   * \param[in] e2e_profile_shortname the shortname.
   * \pre -
   * \context ANY
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  void SetE2EProfileShortname(E2EProfileShortname const& e2e_profile_shortname);

  /*!
   * \brief Get the e2e profile shortname.
   * \return The shortname.
   * \pre -
   * \context ANY
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  E2EProfileShortname const& GetE2EProfileShortname() const noexcept;

  /*!
   * \brief Get the End2End event protection props.
   * \param[in] serialization The serialization.
   * \return The End2End event protection props.
   * \pre -
   * \context ANY
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  ::amsr::e2e::profiles::End2EndEventProtectionProps GetEnd2EndEventProtectionProps(
      ::amsr::someip_binding::internal::configuration::EventConfig::Serialization serialization) const noexcept;

 private:
  EventId e2e_event_id_{};                                   /*!< The event id. */
  DataId e2e_data_id_{};                                     /*!< The data id. */
  DataLength e2e_data_length_{};                             /*!< The data length. */
  MinDataLength e2e_min_data_length_{};                      /*!< The min data length. */
  MaxDataLength e2e_max_data_length_{};                      /*!< The max data length. */
  CounterOffset e2e_counter_offset_{};                       /*!< The counter offset. */
  CrcOffset e2e_crc_offset_{};                               /*!< The CRC offset. */
  HeaderOffset e2e_header_offset_{};                         /*!< The header offset. */
  DataIdNibbleOffset e2e_data_id_nibble_offset_{};           /*!< The data ID nibble offset. */
  DataIdMode e2e_data_id_mode_{kDataIdModeDefaultValue};     /*!< The data ID mode. */
  MaxNoNewOrRepeatedData e2e_max_no_new_or_repeated_data_{}; /*!< The max no new or repeated data. */
  SyncCounterInit e2e_sync_counter_{};                       /*!< The sync counter init. */
  DataIdList e2e_data_id_list_{};                            /*!< The data id list. */
  E2EProfileShortname e2e_profile_shortname_{};              /*!< The e2e profile shortname. */
};

}  // namespace configuration
}  // namespace internal
}  // namespace someip_binding
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_INCLUDE_AMSR_SOMEIP_BINDING_INTERNAL_CONFIGURATION_E2E_PROPS_CONFIG_H_
