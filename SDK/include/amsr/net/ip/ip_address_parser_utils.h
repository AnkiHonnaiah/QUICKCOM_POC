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
 *        \brief  Utilities to work with strings that hold IP addresses.
 *        \unit   osabstraction::net
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_NET_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_NET_IP_IP_ADDRESS_PARSER_UTILS_H_
#define LIB_NET_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_NET_IP_IP_ADDRESS_PARSER_UTILS_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/core/result.h"
#include "amsr/core/string_view.h"

#include "osabstraction/osab_error_domain.h"

namespace amsr {
namespace net {
namespace ip {

/*!
 * \brief           Utilities to extract information or partial strings of a string that holds an IP address.
 */
class IpAddressParserUtils {
 public:
  /*!
   * \brief           Outputs whether the IP address string contains a network interface identification.
   *
   * \details         The string is considered to hold a network interface identification if it contains the separation
   *                  character ('%') followed by at least one other character.
   *
   * \param[in]       address_string                            A string that holds an IP address.
   *
   * \return          True if the address string contains a network interface identification, else false.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      TRUE if the input is thread-safe
   * \steady          TRUE
   * \vprivate        Vector product internal API
   */
  static bool HasInterface(::amsr::core::StringView address_string);

  /*!
   * \brief           Outputs the part of the string that holds the IP address.
   *
   * \details         The string being output is not guaranteed to be a valid representation of an IP address.
   *
   * \param[in]       address_string                            A string that holds the IP address.
   *
   * \return          The part of the input string that holds the IP address. This has not been copied to a new string
   *                  but is a view of the same underlying data structure the input is a view of.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      TRUE if the input is thread-safe
   * \steady          TRUE
   *
   * \vprivate        Vector product internal API
   */
  static ::amsr::core::StringView GetAddress(::amsr::core::StringView address_string);

  /*!
   * \brief           Outputs the part of the string that holds the network interface identification.
   *
   * \details         The string being output is not guaranteed to be a valid identification of local network interface.
   *
   * \param[in]       address_string                            A string that holds an IP address.
   *
   * \return          The part of the input string that holds the network interface identification. This has not been
   *                  copied to a new string but is a view of the same underlying data structure the input is a view of.
   *
   * \error           OsabErrc::kApiError                       The string does not hold a network interface
   *                                                            identification.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      TRUE if the input is thread-safe
   * \steady          TRUE
   *
   * \vprivate        Vector product internal API
   */
  static ::amsr::core::Result<::amsr::core::StringView> GetInterface(::amsr::core::StringView address_string);
};

}  // namespace ip
}  // namespace net
}  // namespace amsr

#endif  // LIB_NET_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_NET_IP_IP_ADDRESS_PARSER_UTILS_H_
