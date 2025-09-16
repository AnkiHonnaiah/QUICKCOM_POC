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
/*!        \file  route_message_parser.h
 *         \brief
 *         \unit  osabstraction::net
 *
 *********************************************************************************************************************/
#ifndef LIB_NET_LIBOSABSTRACTION_LINUX_INCLUDE_AMSR_NET_INTERNAL_ROUTE_MESSAGE_PARSER_H_
#define LIB_NET_LIBOSABSTRACTION_LINUX_INCLUDE_AMSR_NET_INTERNAL_ROUTE_MESSAGE_PARSER_H_

#include <linux/if_addr.h>
#include "amsr/core/result.h"
#include "amsr/net/internal/specific_types.h"
#include "amsr/net/ip/address.h"

namespace amsr {
namespace net {
namespace internal {

/*!
 * \brief             Represents a route address message parser.
 */
class RouteAddressMessageParser final {
 public:
  /*!
   * \brief           Parse a route address message.
   *
   * \details         For Linux a route address message is defined as the RouteAddressMessageHdr (struct ifaddrmsg)
   *                  followed by none, one or multiple attributes.
   *
   * \param[in]       route_addr_message  Pointer to the route address message header.
   *                  The caller must ensure that the data remains valid throughout the lifetime of this object.
   *
   * \param[in]       payload_len  Length of the payload that follows the address message header.
   *
   * \pre             -
   *
   * \context         ANY
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   *
   * \vprivate        Vector component internal API
   */
  RouteAddressMessageParser(amsr::net::internal::RouteAddressMessageHdr const* route_addr_message,
                            std::size_t payload_len);

  // Disable the move and copy operations.
  RouteAddressMessageParser(RouteAddressMessageParser const&) = delete;
  RouteAddressMessageParser(RouteAddressMessageParser&&) = delete;
  auto operator=(RouteAddressMessageParser const&) -> RouteAddressMessageParser& = delete;
  auto operator=(RouteAddressMessageParser&&) -> RouteAddressMessageParser& = delete;

  /*!
   * \brief Default destructor.
   */
  ~RouteAddressMessageParser() noexcept = default;

  /*!
   * \brief           Returns the IP address of the interface.
   *
   * \return          The parsed IP address, or the occurred error.
   *
   * \error           osabstraction::OsabErrc::kAddressNotAvailable     The parsed message did not include an interface
   *                                                                    IP address.
   * \error           osabstraction::OsabErrc::kSystemEnvironmentError  The type of native address is neither IPv4 nor
   *                                                                    IPv6.
   * \error           osabstraction::OsabErrc::kUnexpected              Error mapped to no category.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   *
   * \vprivate        Vector component internal API
   */
  auto GetIpAddress() const noexcept -> ::amsr::core::Result<amsr::net::ip::Address>;

 private:
  /*!
   * \brief            Structure of the parsed arguments.
   */
  struct RoutingAttributesData {
    /*!
     * \brief         Void pointer to the interface address.
     * \details       Can be of the type struct in_addr or struct in6_addr.
     */
    void const* interface_ip_address{nullptr};
  };

  /*!
   * \brief           Stores the type of the address family, which is reported in the route address message.
   */
  std::uint8_t addr_family_;

  /*!
   * \brief            Structure to store the parsed data.
   */
  RoutingAttributesData parsed_data_{};
};

}  // namespace internal
}  // namespace net
}  // namespace amsr

#endif  // LIB_NET_LIBOSABSTRACTION_LINUX_INCLUDE_AMSR_NET_INTERNAL_ROUTE_MESSAGE_PARSER_H_
