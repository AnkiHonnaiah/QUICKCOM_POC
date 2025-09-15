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
/**        \file  someip_tp_assembler_slot.h
 *        \brief  Implementation of SOMEIP-TP Assembler slot
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_TP_INCLUDE_SOMEIP_TP_SOMEIP_TP_ASSEMBLER_SLOT_H_
#define LIB_SOMEIP_TP_INCLUDE_SOMEIP_TP_SOMEIP_TP_ASSEMBLER_SLOT_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <tuple>
#include "ara/core/vector.h"
#include "someip_tp_assembler_client.h"

namespace amsr {
namespace someip_tp {

/*!
 * \brief               For each method or event of a SOME/IP service configured for SOME/IP-TP
 *                      there is one assembler slot available.
 */
class TransportProtocolAssemblerSlot final {
 public:
  /*!
   * \brief             Adds an assembler-slot to this controller based on the configuration.
   * \param[in]         service_id SOME/IP service identifier from the configuration.
   * \param[in]         method_id SOME/IP method/event identifier from the configuration.
   * \param[in]         maximum_original_rx_length The maximum buffer size to provide based on the configuration value.
   * \param[in]         num_rx_buffers The number of buffers to pre-allocate based on the configuration value.
   * \context           Init
   * \pre               -
   * \threadsafe        TRUE
   * \reentrant         FALSE
   * \steady            TRUE
   */
  TransportProtocolAssemblerSlot(someip_protocol::internal::ServiceId service_id,
                                 someip_protocol::internal::MethodId method_id,
                                 someip_protocol::internal::LengthField maximum_original_rx_length,
                                 someip_protocol::internal::LengthField num_rx_buffers) noexcept
      : identity_{std::make_tuple(service_id, method_id)},
        clients_{},
        original_rx_length_{maximum_original_rx_length},
        num_rx_buffers_{num_rx_buffers} {}
  /*!
   * \brief             Based on the configuration one slot is placed in one controller.
   * \param[in]         service_id SOME/IP service identifier from the configuration.
   * \param[in]         method_id SOME/IP method/event identifier from the configuration.
   * \pre               -
   * \context           Init
   * \threadsafe        FALSE
   * \reentrant         FALSE
   * \steady            TRUE
   */
  TransportProtocolAssemblerSlot(someip_protocol::internal::ServiceId service_id,
                                 someip_protocol::internal::MethodId method_id)
      : TransportProtocolAssemblerSlot(service_id, method_id, 0U, 0U) {}
  /*!
   * \brief             Client / Session container of assemblers.
   */
  using AssemblerClients = ara::core::Vector<TransportProtocolAssemblerClient>;
  /*!
   * \brief             Add a client to this assembler slot or get an existing one from the collection.
   * \param[in]         client_id Client identifier
   * \param[in]         current_session Current session identifier
   * \return            Returns reference to either found or newly created assembler client
   * \pre               -
   * \context           ANY
   * \threadsafe        FALSE
   * \reentrant         FALSE
   * \steady            TRUE
   *
   * \internal
   * - Search for the requested SOME/IP Client ID inside the clients collection.
   * - If a client is found and available, return an reference to it.
   * - Otherwise,
   *   - Construct one client that holds the reference to one assembler.
   *   - Append the client to the collection of clients and return the reference to the assembler client.
   * \endinternal
   */
  TransportProtocolAssemblerClient& GetTransportProtocolAssemblerClient(
      someip_protocol::internal::ClientId client_id, someip_protocol::internal::SessionId current_session) {
    AssemblerClients::iterator assembler_client{};

    // Check for an available client.
    assembler_client =
        std::find_if(clients_.begin(), clients_.end(), [&client_id](TransportProtocolAssemblerClient const& client_av) {
          return (client_av.GetClientId() == client_id);
        });

    if (assembler_client == clients_.end()) {
      // Create and push a new client to AssemblerClients
      clients_.emplace_back(client_id, current_session, original_rx_length_, num_rx_buffers_);
      assembler_client = std::prev(clients_.end());
    }

    // Returns an reference to the found/created assembler
    return *assembler_client;
  }
  /*!
   * \brief             Compares two assembler slots.
   * \param[in]         other The slot to compare to.
   * \return            true if the identity of service ID and method ID matches with the given as the parameter.
   * \pre               -
   * \context           ANY
   * \threadsafe        FALSE
   * \reentrant         FALSE
   * \steady            TRUE
   */
  inline bool operator==(TransportProtocolAssemblerSlot const& other) const { return identity_ == other.identity_; }

 private:
  /*!
   * \brief             Based on this identity we know if SOME/IP-TP segmentation is used
   *                    for this certain method ID / event ID.
   */
  using StaticIdentity = std::tuple<someip_protocol::internal::ServiceId, someip_protocol::internal::MethodId>;
  /*!
   * \brief             Identity of this assembler slot based on service ID, method/event ID
   *                    from the configuration.
   */
  StaticIdentity identity_;
  /*!
   * \brief             A collection of clients that may send segmented SOME/IP-TP messages
   *                    for this pair of service ID & method ID.
   */
  AssemblerClients clients_;
  /*!
   * \brief             Length for the buffer provided.
   */
  someip_protocol::internal::LengthField original_rx_length_;
  /*!
   * \brief             Number of the buffers provided.
   */
  someip_protocol::internal::LengthField num_rx_buffers_;
};

}  // namespace someip_tp
}  // namespace amsr

#endif  // LIB_SOMEIP_TP_INCLUDE_SOMEIP_TP_SOMEIP_TP_ASSEMBLER_SLOT_H_
