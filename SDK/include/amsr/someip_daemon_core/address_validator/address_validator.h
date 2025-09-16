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
/*!        \file  address_validator.h
 *        \brief  Class for IP address validation of a message.
 *      \details  Class to validate IP address of a message against the network(s) of port mapping(s).
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_ADDRESS_VALIDATOR_ADDRESS_VALIDATOR_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_ADDRESS_VALIDATOR_ADDRESS_VALIDATOR_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/net/ip/address.h"
#include "amsr/net/ip/network.h"

namespace amsr {
namespace someip_daemon_core {
namespace address_validator {

/*!
 * \brief Validate the IP address of a message.
 *
 * \details Validate IP address of a message against the network(s) of port mapping(s).
 */
class AddressValidator final {
 public:
  /*!
   * \brief   The destructor.
   * \steady  FALSE
   */
  ~AddressValidator() noexcept = default;

  /*!
   * \brief Constructor for network.
   *
   * \param[in] network Network.
   *
   * \steady  FALSE
   */
  explicit AddressValidator(amsr::net::ip::Network const& network) noexcept;

  AddressValidator(AddressValidator const&) = delete;
  AddressValidator(AddressValidator&&) = delete;
  AddressValidator& operator=(AddressValidator const&) = delete;
  AddressValidator& operator=(AddressValidator&&) = delete;

  /*!
   * \brief Validate the passed-in IP address against the network.
   *
   * \details Validate the passed-in IP address against the internal network object.
   *
   * \param[in] ip_address IP address to validate.
   *
   * \context   Network
   * \reentrant FALSE
   * \steady    TRUE
   *
   * \return "true" if the IP address belongs to the network and "false" otherwise.
   */
  bool Validate(amsr::net::ip::Address const& ip_address) const noexcept;

 private:
  /*!
   * \brief Network to compare the IP addresses against.
   */
  amsr::net::ip::Network const network_;
};

}  // namespace address_validator
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_ADDRESS_VALIDATOR_ADDRESS_VALIDATOR_H_
