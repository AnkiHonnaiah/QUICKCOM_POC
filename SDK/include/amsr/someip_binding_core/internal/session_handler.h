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
/**        \file  amsr/someip_binding_core/internal/session_handler.h
 *        \brief  Session Id handling (incrementing) for method request, events and fields.
 *        \unit   SomeIpBinding::SomeIpBindingCore::Server::SessionHandler
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_SESSION_HANDLER_H_
#define LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_SESSION_HANDLER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <atomic>
#include "someip-protocol/internal/someip_posix_types.h"

namespace amsr {
namespace someip_binding_core {
namespace internal {

/*!
 * \brief Session Id handling (incrementing) for method request, events and fields.
 * \details Wrapper prevents from incorrect usage of the session identifier. For instance it prevents from
 * decrementing (--session) or using a signed SessionType.
 * \trace SPEC-10144395, SPEC-10144397
 */

class SessionHandler final {
 public:
  /*!
   *  \brief For SomeIP session handling, use SessionId type declared in someip_protocol.h
   */
  using SessionId = ::amsr::someip_protocol::internal::SessionId;

  /*!
   * \brief Return the current session Id value by assignment.
   *
   * \return Current session Id value as Uint16_t
   * \pre          -
   * \context      App
   * \threadsafe   FALSE
   * \reentrant    FALSE
   * \synchronous  TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  explicit operator SessionId() const noexcept;

  /*!
   * \brief Default constructor.
   * \spec
   *   requires true;
   * \endspec
   */
  SessionHandler() noexcept = default;
  /*!
   * \brief Default destructor.
   * \spec
   *   requires true;
   * \endspec
   */
  ~SessionHandler() noexcept = default;

  SessionHandler(SessionHandler const&) = delete;
  SessionHandler(SessionHandler&&) = delete;
  SessionHandler& operator=(SessionHandler const&) & = delete;
  SessionHandler& operator=(SessionHandler&&) & = delete;

  /*!
   * \brief Handles the increment and wrap-around logic.
   * The user is not in the need to check for the 0-value. Done within
   * this operator defined.
   * \return Reference to the updated SessionHandler
   * \pre          -
   * \context      App
   * \threadsafe   FALSE
   * \reentrant    FALSE
   * \synchronous  TRUE
   * \spec
   *   requires true;
   * \endspec
   *
   * \trace SPEC-10144396
   */
  SessionHandler& operator++() noexcept;

 private:
  /*!
   * \brief Skipping the zero-value on a wrap-around as it is specified.
   * \pre          -
   * \context      App
   * \threadsafe   FALSE
   * \reentrant    FALSE
   * \synchronous  TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  void SkipZero() noexcept;
  /*!
   * \brief Initial value is set to 1. A session Id of value 0 would mean: session handling disabled.
   */
  std::atomic<SessionId> value_{1U};
};

}  // namespace internal
}  // namespace someip_binding_core
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_SESSION_HANDLER_H_
