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
/*!        \file  assembler_mapping.h
 *        \brief  Mapping for SOME/IP-TP assemblers.
 *
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_TP_INCLUDE_SOMEIP_TP_ASSEMBLY_ASSEMBLER_MAPPING_H_
#define LIB_SOMEIP_TP_INCLUDE_SOMEIP_TP_ASSEMBLY_ASSEMBLER_MAPPING_H_

#include "amsr/net/ip/address.h"
#include "amsr/net/ip/address_v4.h"
#include "amsr/net/ip/address_v6.h"
#include "ara/core/map.h"
#include "ara/core/memory_resource.h"
#include "someip-protocol/internal/message.h"
#include "someip_tp/assembly/assembler.h"
#include "someip_tp/someip_tp_types.h"

namespace amsr {
namespace someip_tp {

/*!
 * \brief The AssemblerMapping controls access to Assembler instances based on its configuration.
 */
class AssemblerMapping final {
 public:
  /*!
   * \brief The configuration for an AssemblerMapping instance.
   */
  struct Config {
    /*!
     * \brief Key to access the configuration values for one assembler.
     */
    using Key = std::tuple<someip_protocol::internal::ServiceId, someip_protocol::internal::MajorVersion,
                           someip_protocol::internal::MethodId, someip_protocol::internal::SomeIpMessageType>;
    /*!
     * \brief The maximum size of the assemblers assembly buffer.
     */
    using MaxRxMessageSize = std::size_t;
    /*!
     * \brief The flag to indicate if the assembler shall use the pre-allocated memory allocator.
     */
    using Deterministic = bool;
    /*!
     * \brief The value type of the key-value pair for an assemblers config.
     */
    using Value = std::pair<MaxRxMessageSize, Deterministic>;
    /*!
     * \brief The container type used to store the configurations look up table.
     */
    using LookUpTable = ara::core::Vector<std::pair<Key, Value>>;
    /*!
     * \brief The look-up table is comprised of configurations for assemblers.
     */
    LookUpTable lookup_table{};
    /*!
     * \brief The total size-in-bytes of the monotonic memory resource of the deterministic allocator.
     */
    std::size_t pre_allocation_size{};
  };
  /*!
   * \brief Shorter typedef for Assembler.
   */
  using Assembler = amsr::someip_tp::Assembler;

 private:
  using Port = std::uint16_t;
  /*!
   * \brief The Key type used to index assemblers.
   */
  using Key = std::tuple<someip_protocol::internal::InstanceId, someip_protocol::internal::ServiceId,
                         someip_protocol::internal::MethodId, someip_protocol::internal::ClientId,
                         someip_protocol::internal::InterfaceVersion, someip_protocol::internal::SomeIpMessageType,
                         amsr::net::ip::Address, Port>;
  /*!
   * \brief The container type used to hold assemblers.
   */
  using AssemblerContainer = ara::core::Vector<std::pair<Key, Assembler>>;

  /*!
   * \brief Assemblers map.
   */
  AssemblerContainer assemblers_;
  /*!
   * \brief The configuration of this assembler mapping.
   */
  Config const config_;
  /*!
   * \brief An allocator for dynamic assemblers.
   */
  ara::core::PolymorphicAllocator<std::uint8_t> flexible_allocator_;
  /*!
   * \brief The monotonic memory buffer for the deterministic allocator.
   */
  ara::core::MonotonicBufferResource monotonic_buffer_;
  /*!
   * \brief An allocator for deterministic assemblers.
   */
  ara::core::PolymorphicAllocator<std::uint8_t> deterministic_allocator_;
  /*!
   * \brief Logger.
   */
  logging::AraComLogger logger_{"someip_tp::AssemblerMapping"};

  /*!
   * \brief Find an existing assembler or create a new Assemler if not found.
   * \details A new Assembler is only created if the SOME/IP header is known in the configuration.
   * \param[in] instance_id Instance Id.
   * \param[in] someip_header SOME/IP header.
   * \param[in] ip_address The IP address is needed to form the key.
   * \param[in] ip_port The port of the IP address is needed to form the key.
   * \param[in] create Flag to indicate if a new Assembler should be created.
   * \steady TRUE
   * \return Assembler* Pointer to Assembler, or nullptr.
   */
  Assembler* FindOrCreateAssembler(someip_protocol::internal::InstanceId const instance_id,
                                   someip_protocol::internal::SomeIpMessageHeader const& someip_header,
                                   amsr::net::ip::Address const& ip_address, amsr::net::ip::Port const ip_port,
                                   bool create) noexcept {
    // [PRS_SOMEIP_00738] The receiver shall match segments for reassembly based
    // on the configured values of Message-ID, Protocol-Version, Interface-Version and
    // Message-Type (w/o TP Flag). Fulfilled.
    // [PRS_SOMEIP_00740] It shall be supported to reassemble multiple messages with
    // the same Message ID but sent from different clients (difference in Sender IP, Sender
    // Port, or Client ID) in parallel. Fulfilled - Instance ID is mapped by SenderIP:SenderPort
    // [PRS_SOMEIP_00751] Reordering of segments of completely different original messages (e.g. Message ID is
    // different) is not of concern since those segments go to different buffers. Fulfilled.
    Assembler* assembler{nullptr};
    // [PRS_SOMEIP_00738], [PRS_SOMEIP_00740]
    someip_protocol::internal::SomeIpMessageType const message_type{
        FromSomeIpTpMessageType(someip_header.message_type_)};
    Key const key{instance_id,
                  someip_header.service_id_,
                  someip_header.method_id_,
                  someip_header.client_id_,
                  someip_header.interface_version_,
                  message_type,
                  ip_address,
                  ip_port.port};
    AssemblerContainer::iterator it{
        std::find_if(assemblers_.begin(), assemblers_.end(),
                     [&key](typename AssemblerContainer::value_type const& e) noexcept { return key == e.first; })};
    if (it == assemblers_.end()) {
      // Look up config for the SOME/IP(-TP) message
      Config::Key const config_key{someip_header.service_id_, someip_header.interface_version_,
                                   someip_header.method_id_, message_type};
      Config::LookUpTable::const_iterator config_it{std::find_if(
          config_.lookup_table.begin(), config_.lookup_table.end(),
          [&config_key](typename Config::LookUpTable::value_type const& e) noexcept { return config_key == e.first; })};
      if (config_it == config_.lookup_table.end()) {
        logger_.LogVerbose(
            [this, &someip_header](ara::log::LogStream& s) noexcept {
              s << "The SOME/IP(-TP) message is not known by this assembler mapping.";
            },
            static_cast<char const*>(__func__), __LINE__);
      } else {  // Known message, we need a new assembler for it if needed.
        if (create) {
          ara::core::PolymorphicAllocator<std::uint8_t> allocator{config_it->second.second ? deterministic_allocator_
                                                                                           : flexible_allocator_};
          assemblers_.push_back(AssemblerContainer::value_type{
              key, Assembler{config_it->second.first, allocator}});  // [PRS_SOMEIP_00751]
          assembler = std::addressof(assemblers_.back().second);
        }
      }
    } else {
      assembler = std::addressof(it->second);
    }
    return assembler;
  }

 public:
  /*!
   * \brief Construct a new AssemblerMapping object
   *
   * \param[in, out] config A configuration to take ownership for.
   */
  explicit AssemblerMapping(Config config) noexcept
      : assemblers_{},
        config_{config},
        flexible_allocator_{ara::core::NewDeleteResource()},
        monotonic_buffer_{config_.pre_allocation_size, ara::core::NewDeleteResource()},
        deterministic_allocator_{std::addressof(monotonic_buffer_)} {}

  /*!
   * \brief Check if a SOME/IP message requires reassembly.
   * \details An existing Assembler is reset if the message type of a known TP-message changed to non-TP message type.
   * \param[in] instance_id The instance ID is needed to form the key.
   * \param[in] someip_header The header is needed to form the key and to look up if it's known from configuraiton.
   * \param[in] ip_address The IP address is needed to form the key.
   * \param[in] ip_port The port of the IP address is needed to form the key.
   * \steady TRUE
   * \return Pointer to Assembler or nullptr if not part of this configuration.
   */
  bool RequiresAssembly(someip_protocol::internal::InstanceId const instance_id,
                        someip_protocol::internal::SomeIpMessageHeader const& someip_header,
                        amsr::net::ip::Address const& ip_address, amsr::net::ip::Port const ip_port) noexcept {
    bool required{IsSomeIpTpMessage(someip_header.message_type_)};
    if (!required) {
      Assembler* assembler{FindOrCreateAssembler(instance_id, someip_header, ip_address, ip_port, false)};
      if (assembler != nullptr) {
        // If we have an assembler for this message and it's not a TP message type we cancel the ongoing
        // reassembly process.
        assembler->Cancel();
      }
    }
    return required;
  }

  /*!
   * \brief Get an Assembler
   * \details If the message header is new and known to the configuration a new Assembler is created only once.
   * \param[in] instance_id The instance ID is needed to form the key.
   * \param[in] someip_header The header used for the key and look-up in the configuration.
   * \param[in] ip_address The IP address is needed to form the key.
   * \param[in] ip_port The port of the IP address is needed to form the key.
   * \steady TRUE
   * \return Pointer to Assembler or nullptr if not part of this configuration.
   */
  Assembler* GetAssembler(someip_protocol::internal::InstanceId const instance_id,
                          someip_protocol::internal::SomeIpMessageHeader const& someip_header,
                          amsr::net::ip::Address const& ip_address, amsr::net::ip::Port const ip_port) noexcept {
    return FindOrCreateAssembler(instance_id, someip_header, ip_address, ip_port, true);
  }
};

}  // namespace someip_tp
}  // namespace amsr

#endif  // LIB_SOMEIP_TP_INCLUDE_SOMEIP_TP_ASSEMBLY_ASSEMBLER_MAPPING_H_
