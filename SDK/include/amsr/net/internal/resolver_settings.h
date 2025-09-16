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
/*!        \file
 *        \brief  Implements a settings data type used in BasicResolver class.
 *        \unit   osabstraction::net
 *
 *      \details
 *
 *********************************************************************************************************************/

#ifndef LIB_NET_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_NET_INTERNAL_RESOLVER_SETTINGS_H_
#define LIB_NET_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_NET_INTERNAL_RESOLVER_SETTINGS_H_

#include <cstring>
#include "amsr/net/internal/types.h"

namespace amsr {
namespace net {
namespace internal {

/*!
 * \brief Template class for ResolverSettings.
 *
 * \details
 * This class represents abstraction for Os datatypes that the user enters to the resolver logic.
 *
 * \tparam  Protocol  The protocol that shall be used.
 */
template <class Protocol>
class ResolverSettings final {
 public:
  /*!
   * \brief Alias to the template parameter Protocol. Check class description for details.
   *
   * \details
   * The Protocol Type shall satisfy the following requirements:
   * - static constexpr auto GetType() noexcept -> amsr::net::internal::SocketType
   *
   */
  using ProtocolType = Protocol;

  /*!
   * \brief           Construct a default settings entry.
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
  ResolverSettings() noexcept : hints_{} {
    std::memset(&hints_, 0, sizeof(struct addrinfo));
    hints_.ai_flags = 0;
    hints_.ai_socktype = ProtocolType::GetType().value;
    hints_.ai_family = AF_UNSPEC;  // To return both IPv4 & IPv6 addresses
  }

  /*!
   * \brief           Destroys the settings struct
   *
   * \context         -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   *
   * \vprivate        Vector component internal API
   */
  ~ResolverSettings() noexcept = default;

  /*!
   * \brief           Deleted copy constructor.
   */
  ResolverSettings(ResolverSettings const&) noexcept = delete;

  /*!
   * \brief           Deleted move constructor
   */
  ResolverSettings(ResolverSettings&&) noexcept = delete;

  /*!
   * \brief           Deleted copy assignment operator.
   */
  auto operator=(ResolverSettings const&) noexcept -> ResolverSettings& = delete;

  /*!
   * \brief           Deleted move assignment operator.
   */
  auto operator=(ResolverSettings&&) & noexcept -> ResolverSettings& = delete;

  /*!
   * \brief           Sets protocol family.
   *
   * \param[in]       protocol_family  AF_INET6 or AF_INET.
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
  void SetProtocolFamily(SocketFamily const protocol_family) noexcept { hints_.ai_family = protocol_family.value; }

  /*!
   * \brief           Get the hints structure associated with the settings.
   *
   * \return          The hints structure to resolve domain name.
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
   *
   * \spec
   *   requires true;
   * \endspec
   */
  AddrInfoView GetHints() noexcept { return AddrInfoView{&hints_}; }

 private:
  /*!
   * \brief Stores hints structure passed to resolver.
   */
  addrinfo hints_{};
};

}  // namespace internal
}  // namespace net
}  // namespace amsr

#endif  // LIB_NET_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_NET_INTERNAL_RESOLVER_SETTINGS_H_
