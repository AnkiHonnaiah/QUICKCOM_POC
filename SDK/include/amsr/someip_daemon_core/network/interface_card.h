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
/**        \file  interface_card.h
 *        \brief  A Interface Card class to wrap the interface functionality from LibOsAbstraction.
 *
 *      \details  -
 *
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_NETWORK_INTERFACE_CARD_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_NETWORK_INTERFACE_CARD_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/net/interface.h"
#include "amsr/net/ip/address.h"
#include "amsr/someip_daemon_core/network/interface_card_interface.h"

namespace amsr {
namespace someip_daemon_core {
namespace network {

/*!
 * \brief A Interface card class to wrap the interface functionality from LibOsAbstraction.
 */
class InterfaceCard final : public InterfaceCardInterface {
 public:
  /*!
   * \brief constructor
   *
   * \param[in] interface The Interface.
   *
   * \context           Init
   * \pre               -
   * \reentrant         FALSE
   * \steady            FALSE
   */
  explicit InterfaceCard(amsr::net::Interface const& interface) noexcept;

  /*!
   * \brief Default destructor.
   *
   * \steady          FALSE
   */
  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  ~InterfaceCard() noexcept override = default;

  InterfaceCard() = delete;
  InterfaceCard(InterfaceCard const&) = delete;
  InterfaceCard(InterfaceCard&&) = delete;
  InterfaceCard& operator=(InterfaceCard const&) & = delete;
  InterfaceCard& operator=(InterfaceCard&&) & = delete;

  /*!
   * \brief           Extracts the status of this interface
   *
   * \details         Checks if the administrative and operational state of the interface is set.
                      Each interface has its own independent socket.
                      The socket is only opened if SetStatus() or ExtractSTatus() are used. To reduce the
                      number of open sockets call SetStatus() or ExtractStatus() on the same Interface object.
   *
   * \return          The status of this interface.
   *
   * \error           osabstraction::OsabErrc::kUnexpected              Unexpected error that is mapped to no category.

   * \error           osabstraction::OsabErrc::kInsufficientPrivileges  The process has insufficient privileges.

   * \error           osabstraction::OsabErrc::kResource                Not enough system resources.

   * \error           osabstraction::OsabErrc::kSystemEnvironmentError  Interface isn't available anymore.
   *
   * \context         ANY
   *
   * \pre             The interface name should not be longer than kMaxNetworkInterfaceName.
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          FALSE
   */
  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  ara::core::Result<amsr::net::Interface::InterfaceStatus> ExtractStatus() noexcept override;

 private:
  /*!
   * \brief The Network interface.
   */
  amsr::net::Interface interface_;
};

}  // namespace network
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_NETWORK_INTERFACE_CARD_H_
