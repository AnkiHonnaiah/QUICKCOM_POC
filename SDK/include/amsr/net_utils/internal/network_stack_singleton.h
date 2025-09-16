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
 *        \brief  Singleton to control which network stack instance to use.
 *        \unit   osabstraction::net_utils
 *
 *********************************************************************************************************************/

#ifndef LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_NET_UTILS_INTERNAL_NETWORK_STACK_SINGLETON_H_
#define LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_NET_UTILS_INTERNAL_NETWORK_STACK_SINGLETON_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstdint>

#include "amsr/core/array.h"
#include "amsr/core/optional.h"
#include "amsr/core/string_view.h"

#include "amsr/core/span.h"
#include "amsr/net_utils/internal/thread_aware_lock.h"
#include "amsr/net_utils/network_stack_id.h"

namespace amsr {
namespace net_utils {
namespace internal {

/*!
 * \brief           Manages access to the network stack selection.
 *
 * \attention       Changes to the SOCK environment variable must only and exclusively happen through this class.
 *                  This limitation applies to all code linked into the final executable binary.
 *
 * \vprivate        Component private
 */
class NetworkStackSingleton final {
 public:
  /*!
   * \brief           Outputs the singleton instance.
   *
   * \return          The singleton instance. Only valid as long as below preconditions of this function all hold true.
   *
   * \context         ANY|!NetworkStackSingleton constructor
   *
   * \pre             OsAbstraction component must be initialized.
   * \pre             The main thread of this process has not returned from main().
   * \pre             exit() has not been called by any thread of this process.
   * \pre             terminate() has not been called by any thread of this process.
   * \pre             If the SOCK environment variable is set at startup of the calling process, its value must consist
   *                  of at most NetworkStackId::kMaxNetworkStackIdSize characters.
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      TRUE
   * \steady          FALSE
   *
   * \trace           DSGN-Osab-BasicIP_MultipleNetworkStacks
   *
   * \vprivate        Vector component internal API
   */
  static NetworkStackSingleton& GetInstance() noexcept;

  /*!
   * \brief           Gets the ID of the default network stack.
   *
   * \return          The default network stack ID. Only valid as long as below preconditions of this function all hold
   *                  true.
   *
   * \context         ANY|!NetworkStackSingleton constructor
   *
   * \pre             OsAbstraction component must be initialized
   * \pre             The main thread of this process has not returned from main().
   * \pre             exit() has not been called by any thread of this process.
   * \pre             terminate() has not been called by any thread of this process.
   * \pre             If the SOCK environment variable is set at startup of the calling process, its value must consist
   *                  of at most NetworkStackId::kMaxNetworkStackIdSize characters.
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
   */
  NetworkStackId const& GetDefaultValue() noexcept { return default_id_.value(); }

  /*!
   * \brief           Acquires the lock protecting the currently set network stack instance.
   *
   * \details         If the lock is held by some other thread, the calling thread is blocked until the lock can be
   *                  acquired.
   *                  On QNX, this lock must be held when calling SetValue(), RestoreValue() or any
   *                  system call that establishes a connection to io-pkt.
   *
   * \context         ANY
   *
   * \pre             The calling thread does not hold the lock.
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      TRUE
   * \steady          TRUE
   *
   * \trace           DSGN-Osab-BasicIP_MultipleNetworkStacks
   *
   * \vprivate        Vector component internal API
   */
  void Lock() noexcept;

  /*!
   * \brief           Releases the lock acquired by Lock().
   *
   * \context         ANY
   *
   * \pre             The calling thread must hold the lock.
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      TRUE
   * \steady          TRUE
   *
   * \trace           DSGN-Osab-BasicIP_MultipleNetworkStacks
   *
   * \vprivate        Vector component internal API
   */
  void Unlock() noexcept;

  /*!
   * \brief           Sets the network stack instance to work with.
   *
   * \param[in]       id                        ID of the network stack to set.
   *
   * \context         ANY
   *
   * \pre             The calling thread must hold the lock.
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      TRUE if lock is held.
   * \steady          TRUE
   *
   * \trace           DSGN-Osab-BasicIP_MultipleNetworkStacks
   *
   * \vprivate        Vector component internal API
   */
  void SetValue(NetworkStackId const& id) noexcept;

  /*!
   * \brief           Sets the network stack instance to work with to the instance it was before the last call to
   *                  SetValue().
   *
   * \details         If SetValue() was not called yet, sets the default network stack instance.
   *
   * \context         ANY
   *
   * \pre             The calling thread must hold the lock.
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      TRUE if lock is held.
   * \steady          TRUE
   *
   * \trace           DSGN-Osab-BasicIP_MultipleNetworkStacks
   *
   * \vprivate        Vector component internal API
   */
  void RestoreValue() noexcept;

  // Singleton must never be copied or moved.
  NetworkStackSingleton(NetworkStackSingleton const&) noexcept = delete;
  NetworkStackSingleton(NetworkStackSingleton&&) noexcept = delete;
  NetworkStackSingleton& operator=(NetworkStackSingleton const&) noexcept = delete;
  NetworkStackSingleton& operator=(NetworkStackSingleton&&) noexcept = delete;

 private:
  /*!
   * \brief           Sets up the buffer for the value of the SOCK environment variable and stores the current value
   *                  as default value.
   *
   * \context         ANY
   *
   * \pre             The current value of the SOCK environment variable consists of at most
   *                  NetworkStackId::kMaxNetworkStackIdSize characters.
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      TRUE
   * \steady          FALSE
   *
   * \trace           DSGN-Osab-BasicIP_MultipleNetworkStacks
   */
  NetworkStackSingleton() noexcept;

  /*!
   * \brief           Resets the SOCK environment variable to the default value.
   *
   * \context         ANY
   *
   * \pre             The lock is not held by any thread.
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   *
   * \trace           DSGN-Osab-BasicIP_MultipleNetworkStacks
   */
  ~NetworkStackSingleton() noexcept;

  /*!
   * \brief           Offset into the buffer to the value of the SOCK environment variable.
   *
   * \details         Length of the string"SOCK=".
   */
  static constexpr std::size_t kEnvvarValueOffset{5};

  /*!
   * \brief           Size of the buffer set by putenv for the SOCK environment variable.
   *
   * \details         Size is made up of components
   *                  - length to store the string "SOCK="
   *                  - maximum size of the network stack ID (value to assign to SOCK)
   *                  - 1 for the terminating \0 character
   */
  static constexpr std::size_t kPutenvBufferSize{kEnvvarValueOffset + NetworkStackId::kMaxNetworkStackIdSize + 1};

  /*!
   * \brief   Lock that protects the SOCK environment variable.
   */
  ThreadAwareLock lock_{};

  /*!
   * \brief   Buffer set up by putenv to hold the environment variable.
   *
   * \details Content of that buffer is "SOCK=<envvar value>\0"
   */
  ::amsr::core::Array<char, kPutenvBufferSize> putenv_buffer_{};

  /*!
   * \brief   View on the part of the buffer that holds the environment variable value and the \0 character(s).
   */
  ::amsr::core::Span<char> const envvar_value_part_{
      ::amsr::core::Span<char>{putenv_buffer_}.subspan(kEnvvarValueOffset)};

  /*!
   * \brief   The default network stack ID (value of SOCK at process startup).
   *
   * \details Optional to allow default construction of NetworkStackSingleton without invoking the default constructor
   *          of NetworkStackId as that default constructor tries to use the singleton.
   *          Always has a value after NetworkStackSingleton construction is finished.
   */
  ::amsr::core::Optional<NetworkStackId> default_id_{};

  /*!
   * \brief   The value of SOCK before the most recent change to it.
   *
   * \details Optional to allow default construction of NetworkStackSingleton without invoking the default constructor
   *          of NetworkStackId as that default constructor tries to use the singleton.
   *          Always has a value after NetworkStackSingleton construction is finished.
   */
  ::amsr::core::Optional<NetworkStackId> previous_id_{};
};

/*!
 * \exclusivearea ::amsr::net_utils::internal::NetworkStackSingleton::lock_::mutex_
 * Ensures freedom from race conditions when changing the SOCK environment variable and doing a system call afterwards.
 * \protects ::amsr::net_utils::internal::NetworkStackSingleton::putenv_buffer_,
 *           ::amsr::net_utils::internal::NetworkStackSingleton::envvar_value_part_,
 *           ::amsr::net_utils::internal::NetworkStackSingleton::previous_id_,
 *           SOCK environment variable
 * \usedin   ::amsr::net_utils::internal::NetworkStackSingleton::Lock,
 *           ::amsr::net_utils::internal::NetworkStackSingleton::Unlock,
 *           ::amsr::net_utils::internal::NetworkStackSingleton::SetValue,
 *           ::amsr::net_utils::internal::NetworkStackSingleton::GetValue,
 *           ::amsr::net_utils::internal::NetworkStackSingleton::RestoreValue,
 * \exclude  All other of these methods of the same object, other areas enclosed by NetworkStackSingleton::Lock() and
 *           NetworkStackSingleton::Unlock() calls.
 * \length   LONG Calling system calls.
 *
 * \endexclusivearea
 */

}  // namespace internal
}  // namespace net_utils
}  // namespace amsr

#endif  // LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_NET_UTILS_INTERNAL_NETWORK_STACK_SINGLETON_H_
