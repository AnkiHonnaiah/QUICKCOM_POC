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
 *        \brief  Defines a SafeIPC unicast address.
 *        \unit     osabstraction::ipc::IpcUtils_LinuxPikeOs
 *        \unit     osabstraction::ipc::IpcUtils_QNX
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_IPC_UNICAST_ADDRESS_H_
#define LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_IPC_UNICAST_ADDRESS_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstddef>
#include <cstdint>

namespace amsr {
namespace ipc {

/*!
 * \brief   Communication domain.
 */
using Domain = std::uint32_t;
/*!
 * \brief   Application port.
 */
using Port = std::uint32_t;

/*!
 * \brief   SafeIPC unicast address.
 *
 * \details This address is platform independent.
 */
class UnicastAddress final {
 public:
  UnicastAddress() = delete;

  /*!
   * \brief   Constructs an address object.
   *
   * \param[in]       domain Address domain.
   * \param[in]       port   Application port.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          TRUE
   *
   * \trace           DSGN-Osab-SafeIpcServer,
   *                  DSGN-Osab-SafeIpcClient
   *
   * \vprivate        Vector product internal API
   *
   * \spec
   *   requires true;
   * \endspec
   */
  constexpr UnicastAddress(Domain domain, Port port) noexcept : domain_{domain}, port_{port} {}

  /*!
   * \brief Returns the stored domain.
   *
   * \return Domain of unicast address.
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          TRUE
   *
   * \vprivate        Vector product internal API
   *
   * \spec
   *   requires true;
   * \endspec
   */
  Domain GetDomain() const noexcept { return domain_; }

  /*!
   * \brief Returns the stored port.
   *
   * \return Port of unicast address.
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          TRUE
   *
   * \vprivate        Vector product internal API
   *
   * \spec
   *   requires true;
   * \endspec
   */
  Port GetPort() const noexcept { return port_; }

  /*!
   * \brief Compares this address to another unicast address for equality.
   *
   * \param[in]   other  Unicast address to compare to.
   *
   * \return true if domain and port of this unicast address match port and domain of the other unicast address, false
   *         otherwise.
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          TRUE
   *
   * \vprivate        Vector product internal API
   *
   * \spec
   *   requires true;
   * \endspec
   */
  auto operator==(UnicastAddress const& other) const noexcept -> bool {
    return (other.domain_ == domain_) && (other.port_ == port_);
  }

  /*!
   * \brief Compares this address to another unicast address for inequality.
   *
   * \param[in]   other  Unicast address to compare to.
   *
   * \return true if domain or port of this unicast address are different than port or domain of the other unicast
   *         address, false otherwise.
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          TRUE
   *
   * \vprivate        Vector product internal API
   *
   * \spec
   *   requires true;
   * \endspec
   */
  auto operator!=(UnicastAddress const& other) const noexcept -> bool { return !(other == *this); }

 private:
  /*!
   * \brief   Application domain.
   */
  Domain domain_;

  /*!
   * \brief   Application port.
   */
  Port port_;
};

}  // namespace ipc
}  // namespace amsr

#endif  // LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_IPC_UNICAST_ADDRESS_H_
