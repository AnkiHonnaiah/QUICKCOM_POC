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
 *        \brief  Class to represent Network interface address abstraction.
 *        \unit   osabstraction::net
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_NET_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_NET_INTERNAL_INTERFACE_ADDRESS_VIEW_H_
#define LIB_NET_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_NET_INTERNAL_INTERFACE_ADDRESS_VIEW_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/core/result.h"
#include "amsr/net/ip/address.h"

namespace amsr {
namespace net {
namespace internal {

/*!
 * \brief           Represents the view to a list of network interface addresses returned by getifaddrs().
 */
class InterfaceAddressView final {
 public:
  /*!
   * \brief         Deleted copy constructor.
   */
  InterfaceAddressView(InterfaceAddressView const&) = delete;

  /*!
   * \brief         Deleted assignment operator.
   */
  InterfaceAddressView& operator=(InterfaceAddressView const&) & noexcept = delete;

  /*!
   * \brief         Deleted default constructor.
   */
  InterfaceAddressView() noexcept = delete;

  /*!
   * \brief         Move constructs an InterfaceAddressView object.
   *
   * \param[in]     other   InterfaceAddressView to move copy.
   *
   * \context       ANY
   *
   * \pre           -
   *
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \threadsafe    FALSE
   *
   * \vprivate      Vector component internal API
   */
  InterfaceAddressView(InterfaceAddressView&& other) noexcept : ifaddrs_(other.ifaddrs_) { other.ifaddrs_ = nullptr; }

  /*!
   * \brief         Moves other's InterfaceAddressView object.
   *
   * \param[in]     other   InterfaceAddressView to move assign.
   *
   * \return        This object.
   *
   * \context       ANY
   *
   * \pre           -
   *
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \threadsafe    FALSE
   *
   * \vprivate      Vector component internal API
   */
  InterfaceAddressView& operator=(InterfaceAddressView&& other) & noexcept {
    if (this != &other) {
      std::swap(ifaddrs_, other.ifaddrs_);
    }
    return *this;
  }

  /*!
   * \brief         Creates an instance containing ifaddrs.
   *
   * \param[in]       addr                   The pointer to the OS interface list.
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
  explicit InterfaceAddressView(ifaddrs* addr) noexcept : ifaddrs_(addr){};

  /*!
   * \brief  destructor.
   */
  ~InterfaceAddressView();

  /*!
   * \brief           Returns the pointer to the linked list of interface address structures.
   *
   * \return          The pointer to the linked list of interface address structures.
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
  ifaddrs* GetAddressPointer() const;

  /*!
   * \brief           Searches the name of an interface whose assigned IP address is known.
   *
   * \param[in]       address                   The IP address to be searched.
   * \param[out]      name                      The name of the interface, this buffer must be able to hold at least
   *                                            kMaxNetworkInterfaceName characters. Should the name be shorter than
   *                                            the buffer size, the remaining space is filled with \0.
   *
   * \return          StringView to the output name. Only valid until the underlying memory of the name parameter is
   *                  accessible.
   *
   * \error           OsabErrc::kAddressError                     The address is assigned to multiple interfaces.
   *
   * \error           OsabErrc::kSystemEnvironmentError           No interface with the supplied address assigned to
   *                                                              it has been found.
   *
   * \error           OsabErrc::kUnexpected                       Data output by the operating system is invalid.
   *
   * \context         ANY
   *
   * \pre             The provided buffer must be large enough to hold at least kMaxNetworkInterfaceName + 1 characters.
   *                  This precondition is already checked by the calling function FindInterfaceNameByAddress(), so
   *                  there is no need to repeat the check here.
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   *
   * \trace           DSGN-Osab-InterfaceTranslation
   *
   * \vprivate        Vector component internal API
   */
  ::amsr::core::Result<std::size_t> SearchNameFromAddress(ip::Address address,
                                                          ::amsr::core::Span<char> name) const noexcept;

 private:
  /*!
   * \brief Pointer to the linked list of interface address structures.
   */
  ifaddrs* ifaddrs_;
};

}  // namespace internal
}  // namespace net
}  // namespace amsr

#endif  // LIB_NET_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_NET_INTERNAL_INTERFACE_ADDRESS_VIEW_H_
