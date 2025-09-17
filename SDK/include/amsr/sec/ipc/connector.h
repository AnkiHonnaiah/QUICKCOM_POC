/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2023 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file  connector.h
 *        \brief  Provides an abstraction for IPC connection and handles connection establishment and signal handling.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBIPC_INCLUDE_AMSR_SEC_IPC_CONNECTOR_H_
#define LIB_LIBIPC_INCLUDE_AMSR_SEC_IPC_CONNECTOR_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <chrono>
#include <future>
#include <memory>

#include "amsr/ipc/connection.h"
#include "amsr/ipc/unicast_address.h"
#include "amsr/sec/ipc/ipc_transporter.h"
#include "osabstraction/io/reactor1/reactor1.h"

namespace amsr {
namespace sec {
namespace ipc {

static constexpr std::chrono::milliseconds const kConnectTimeout{
    std::chrono::seconds(10)}; /*!< Default connect timeout */

/*!
 * \brief Connector provides an abstraction for IPC connection and handles connection establishment and signal handling
 * \vprivate Component Private
 */
class Connector {
 public:
  /*!
   * \brief Constructs the object.
   * \param[in,out] reactor Reactor to handle events.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  explicit Connector(osabstraction::io::reactor1::Reactor1Interface& reactor);

  /*!
   * \brief Destructor-
   * \vprivate Component Private
   */
  virtual ~Connector() noexcept = default;
  /*!
   * \brief Deleted copy constructor
   * \vprivate Component Private
   */
  Connector(Connector const& /*other*/) = delete;

  /*!
   * \brief Deleted move constructor
   * \vprivate Component Private
   */
  Connector(Connector&& /*other*/) noexcept = delete;

  /*!
   * \brief Deleted copy assignment operator
   * \return -
   * \vprivate Component Private
   */
  Connector& operator=(Connector const& /*other*/) & = delete;

  /*!
   * \brief Deleted move assignment operator
   * \return -
   * \vprivate Component Private
   */
  Connector& operator=(Connector&& /*other*/) & noexcept = delete;

  /*!
   * \brief Connects to remote address.
   * \param[in] address Address to use for establishing the IPC connection.
   * \return A connected event driven transporter.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error osabstraction::OsabErrc::kUnexpected                 Unexpected error that is mapped to no category.
   * \error osabstraction::OsabErrc::kAlreadyConnected           Try to connect already connected connection.
   * \error osabstraction::OsabErrc::kResource                   Not enough system resources to create shared memory or
   *                                                             to perform connect or
   *                                                             to register to reactor.
   * \error osabstraction::OsabErrc::kDoesNotExist               Server configuration file does not exist.
   * \error osabstraction::OsabErrc::kInsufficientPrivileges     Not enough privileges to create shared memory file or
   *                                                             to call chmod on it or
   *                                                             to mmap shared memory file or
   *                                                             to perform connect.
   * \error osabstraction::OsabErrc::kBusy                       The shared memory file exists and is currently in use.
   * \error osabstraction::OsabErrc::kSystemEnvironmentError     Accessing the file system failed.
   * \error osabstraction::OsabErrc::kAddressNotAvailable        The client or server address is not available or
   *                                                             it is currently in use.
   * \error osabstraction::OsabErrc::kDisconnected               The connection has been refused by peer or
   *                                                             the connection request is timed out.
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto Connect(amsr::ipc::UnicastAddress address) const noexcept -> amsr::core::Result<std::unique_ptr<Transporter>>;

 private:
  osabstraction::io::reactor1::Reactor1Interface& reactor_; /*!< Reactor for handling */
};

}  // namespace ipc
}  // namespace sec
}  // namespace amsr

#endif  // LIB_LIBIPC_INCLUDE_AMSR_SEC_IPC_CONNECTOR_H_
