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
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file  lib/libcommon/include/crypto/common/util/observable.h
 *        \brief  Class providing the ability to listen to events.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBCOMMON_INCLUDE_CRYPTO_COMMON_UTIL_OBSERVABLE_H_
#define LIB_LIBCOMMON_INCLUDE_CRYPTO_COMMON_UTIL_OBSERVABLE_H_

/*********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <functional>
#include <map>
#include <utility>

#include "ara/log/logging.h"

namespace crypto {
namespace common {
namespace util {

/*!
 * \brief Makes it possible for the inheriting class to emit events which can be listened to by extern classes.
 * \vprivate Component Private
 */
class Observable {
 public:
  /*!
   * \brief Enumeration of events. May be extended.
   * \vprivate Component Private
   */
  enum class Event : std::uint32_t {
    kUpdate, /*!< The object is updated. */
    kDelete  /*!< The object is deleted. */
  };

  /*!
   * \brief Callback lambdas.
   * \vprivate Component Private
   */
  using Callback = std::function<void(void)>;

 private:
  std::multimap<Event, Callback> events_; /*!< Registration of callbacks for events. */

 public:
  /*!
   * \brief Creates the observable object.
   * \vprivate Component Private
   */
  Observable() noexcept = default;

  /*!
   * \brief Deletes the observable object.
   * \vprivate Component Private
   */
  virtual ~Observable() noexcept = default;

  /*!
   * \brief Registers an event listener.
   * \param[in] event The event to listen to.
   * \param[in] callback Callback to be executed when the event occurs.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  void On(Event event, Callback callback) noexcept;

  /*!
   * \brief Emits an event (executes all callbacks registered for that event).
   * \param[in] event Event to be emitted.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  void Emit(Event event) const noexcept;

 protected:
  /*!
   * \brief Copy constructor.
   * \vpublic
   */
  Observable(Observable const& /*other*/) noexcept = default;

  /*!
   * \brief Copy assignment operator.
   * \return A reference to the object
   * \vpublic
   */
  auto operator=(Observable const& /*other*/) & noexcept -> Observable& = default;

  /*!
   * \brief Move constructor.
   * \vpublic
   */
  Observable(Observable&& /*other*/) noexcept = default;

  /*!
   * \brief Move assignment.
   * \return A reference to the object
   * \vpublic
   */
  auto operator=(Observable&& /*other*/) & noexcept -> Observable& = default;
};

}  // namespace util
}  // namespace common
}  // namespace crypto

#endif  // LIB_LIBCOMMON_INCLUDE_CRYPTO_COMMON_UTIL_OBSERVABLE_H_
