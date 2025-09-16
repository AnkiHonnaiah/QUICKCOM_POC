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
 *        \brief  Identification to select between multiple network stack instances.
 *        \unit   osabstraction::net_utils
 *
 *   \complexity  Only minor violation, no action required.
 *
 *********************************************************************************************************************/

#ifndef LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_NET_UTILS_NETWORK_STACK_ID_H_
#define LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_NET_UTILS_NETWORK_STACK_ID_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstdint>

#include "amsr/core/array.h"
#include "amsr/core/result.h"
#include "amsr/core/string_view.h"

namespace amsr {
namespace net_utils {

/*!
 * \brief           Identifies an instance of a network interface.
 *
 * \vprivate        Product private
 */
class NetworkStackId final {
 public:
  /*!
   * \brief           Maximum number of characters a network stack ID consists of.
   */
  static constexpr std::size_t kMaxNetworkStackIdSize{16};

  /*!
   * \brief           Creates a network stack ID that represents the default network stack.
   *
   * \details         The default network stack is the network stack selected by the value of the SOCK environment
   *                  variable at process startup.
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
   * \trace           DSGN-Osab-BasicIP_MultipleNetworkStacks
   *
   * \vprivate        Vector product internal API
   */
  NetworkStackId() noexcept;

  /*!
   * \brief           Creates a network stack ID.
   *
   * \param[in]       id                        Network stack ID string. Must at most be kMaxNetworkStackIdSize
   *                                            characters long.
   *
   * \return          The created network stack ID.
   *
   * \error           OsabErrc::kApiError                   Input network stack ID is too long.
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
   * \trace           DSGN-Osab-BasicIP_MultipleNetworkStacks
   *
   * \vprivate        Vector product internal API
   *
   * \spec
   *   requires true;
   * \endspec
   */
  static ::amsr::core::Result<NetworkStackId> MakeNetworkStackId(::amsr::core::StringView id) noexcept;

  /*!
   * \brief           Creates an empty network stack ID.
   *
   * \details         The empty network stack ID identifies the network stack that would be used if SOCK was not set.
   *
   * \return          The created network stack ID.
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
   * \trace           DSGN-Osab-BasicIP_MultipleNetworkStacks
   *
   * \vprivate        Vector product internal API
   *
   * \spec
   *   requires true;
   * \endspec
   */
  static NetworkStackId MakeEmptyId() noexcept;

  /*!
   * \brief           Destroys the instance.
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
   * \trace           DSGN-Osab-BasicIP_MultipleNetworkStacks
   *
   * \vprivate        Vector product internal API
   *
   * \spec
   *   requires true;
   * \endspec
   */
  ~NetworkStackId() noexcept = default;

  /*!
   * \brief           Creates a copy of an existing instance.
   *
   * \param[in]       other                     Network stack ID to copy.
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
   * \trace           DSGN-Osab-BasicIP_MultipleNetworkStacks
   *
   * \vprivate        Vector product internal API
   *
   * \spec
   *   requires true;
   * \endspec
   */
  NetworkStackId(NetworkStackId const& other) noexcept;

  /*!
   * \brief           Copy-assigns to this instance.
   *
   * \param[in]       other                     Network stack ID to copy.
   *
   * \return          This object.
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
   * \trace           DSGN-Osab-BasicIP_MultipleNetworkStacks
   *
   * \vprivate        Vector product internal API
   *
   * \spec
   *   requires true;
   * \endspec
   */
  NetworkStackId& operator=(NetworkStackId const& other) & noexcept;

  /*!
   * \brief           Creates an instance by moving from another one.
   *
   * \details         Falls back to a copy. This is required to allow using this type with amsr::core::Result.
   *
   * \param[in]       other                     Network stack ID to move from.
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
   * \trace           DSGN-Osab-BasicIP_MultipleNetworkStacks
   *
   * \vprivate        Vector product internal API
   *
   * \spec
   *   requires true;
   * \endspec
   */
  NetworkStackId(NetworkStackId&& other) noexcept;

  /*!
   * \brief           Move-assigns to this instance.
   *
   * \details         Falls back to a copy. This is required to allow using this type with amsr::core::Result.
   *
   * \param[in]       other                     Network stack ID to move.
   *
   * \return          This object.
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
   * \trace           DSGN-Osab-BasicIP_MultipleNetworkStacks
   *
   * \vprivate        Vector product internal API
   *
   * \spec
   *   requires true;
   * \endspec
   */
  NetworkStackId& operator=(NetworkStackId&& other) & noexcept;

  /*!
   * \brief           Returns the network ID string.
   *
   * \return          View to the network ID string (not null-terminated). Only valid as long this object exists.
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
   * \trace           DSGN-Osab-BasicIP_MultipleNetworkStacks
   *
   * \vprivate        Vector component internal API
   *
   * \spec
   *   requires true;
   * \endspec
   */
  ::amsr::core::StringView GetId() const noexcept;

  /*!
   * \brief           Compares two network stack IDs for equality.
   *
   * \param[in]       other                     Network stack ID to compare with.
   *
   * \return          true if both IDs refer to the same network stack, else false.
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
   * \trace           DSGN-Osab-BasicIP_MultipleNetworkStacks
   *
   * \vprivate        Vector product internal API
   *
   * \spec
   *   requires true;
   * \endspec
   */
  bool operator==(NetworkStackId const& other) const noexcept;

  /*!
   * \brief           Compares two network stack IDs for inequality.
   *
   * \param[in]       other                     Network stack ID to compare with.
   *
   * \return          true if both IDs refer to the different network stacks, else false.
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
   * \trace           DSGN-Osab-BasicIP_MultipleNetworkStacks
   *
   * \vprivate        Vector product internal API
   *
   * \spec
   *   requires true;
   * \endspec
   */
  bool operator!=(NetworkStackId const& other) const noexcept;

  /*!
   * \brief           Compares two network stack IDs for less-than order.
   *
   * \param[in]       other                     Network stack ID to compare with.
   *
   * \return          true if this ID < the other network stack, else false.
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
   * \trace           DSGN-Osab-BasicIP_MultipleNetworkStacks
   *
   * \vprivate        Vector product internal API
   *
   * \spec
   *   requires true;
   * \endspec
   */
  bool operator<(NetworkStackId const& other) const noexcept;

  /*!
   * \brief           Compares two network stack IDs for greater-than order.
   *
   * \param[in]       other                     Network stack ID to compare with.
   *
   * \return          true if this ID > the other network stack, else false.
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
   * \trace           DSGN-Osab-BasicIP_MultipleNetworkStacks
   *
   * \vprivate        Vector product internal API
   *
   * \spec
   *   requires true;
   * \endspec
   */
  bool operator>(NetworkStackId const& other) const noexcept;

 private:
  /*!
   * \brief           Creates a network stack ID from an ID string.
   *
   * \param[in]       id                        Network stack ID string. Must at most be kMaxNetworkStackIdSize
   *                                            characters long.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   *
   * \trace           DSGN-Osab-BasicIP_MultipleNetworkStacks
   *
   * \vprivate        Vector component internal API
   */
  explicit NetworkStackId(::amsr::core::StringView id) noexcept;

  /*!
   * \brief           Buffer that holds the network stack ID string.
   */
  ::amsr::core::Array<char, kMaxNetworkStackIdSize> string_buffer_;

  /*!
   * \brief           The string with the network stack ID.
   */
  ::amsr::core::StringView string_;
};

}  // namespace net_utils
}  // namespace amsr

#endif  // LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_NET_UTILS_NETWORK_STACK_ID_H_
