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
/**        \file  libosabstraction-linux/include/osabstraction/io/reactor1/types.h
 *        \brief  Reactor1 types for Linux
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_COMMON_LIBOSABSTRACTION_LINUX_INCLUDE_OSABSTRACTION_IO_REACTOR1_TYPES_H_
#define LIB_COMMON_LIBOSABSTRACTION_LINUX_INCLUDE_OSABSTRACTION_IO_REACTOR1_TYPES_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <limits>
#include "osabstraction/io/native_types.h"
#include "vac/language/unique_function.h"

namespace osabstraction {
namespace io {
namespace reactor1 {

/*!
 * \brief    Reactor1 callback handle type.
 *
 * \vprivate pes internal API
 */
using CallbackHandle = std::uint64_t;

/*!
 * \brief    Invalid callback handle value.
 *
 * \vprivate Vector product internal API
 */
constexpr CallbackHandle kInvalidCallbackHandle{std::numeric_limits<CallbackHandle>::max()};

/*!
 * \brief    Reactor1 event types Linux implementation.
 * \unit     osabstraction::Reactor1_Linux
 * \vprivate pes internal API
 */
class EventTypes final {
 public:
  /*!
   * \brief    Check if read event is set.
   * \return   true if the read event is set, false otherwise.
   * \steady   TRUE
   *
   * \vprivate pes internal API
   */
  bool HasReadEvent() const noexcept { return read_event_; }

  /*!
   * \brief    Check if write event is set.
   * \return   true if the write event is set, false otherwise.
   *
   * \steady   TRUE
   *
   * \vprivate pes internal API
   */
  bool HasWriteEvent() const noexcept { return write_event_; }

  /*!
   * \brief    Check if error event is set.
   * \return   true if the error event is set, false otherwise.
   *
   * \steady   TRUE

   * \vprivate pes internal API
   */
  bool HasErrorEvent() const noexcept { return error_event_; }

  /*!
   * \brief    Check if software event is set.
   * \return   true if the software event is set, false otherwise.
   *
   * \steady   TRUE
   *
   * \vprivate pes internal API
   */
  bool HasSoftwareEvent() const noexcept { return software_event_; }

  /*!
   * \brief    Check if any IO event is set.
   * \return   true if the IO event is set, false otherwise.
   *
   * \steady   TRUE
   *
   * \vprivate pes internal API
   */
  bool HasAnyIoEvent() const noexcept { return read_event_ || write_event_ || error_event_; }

  /*!
   * \brief       Set read event.
   * \param[in]   enable If true, the read event is set; if false, it is cleared.
   * \return      Reference to this object.
   *
   * \steady      TRUE
   *
   * \vprivate    pes internal API
   */
  EventTypes& SetReadEvent(bool enable) noexcept {
    read_event_ = enable;
    return *this;
  }

  /*!
   * \brief       Set write event.
   * \param[in]   enable If true, the write event is set; if false, it is cleared.
   * \return      Reference to this object.
   * \steady      TRUE
   *
   * \vprivate    pes internal API
   */
  EventTypes& SetWriteEvent(bool enable) noexcept {
    write_event_ = enable;
    return *this;
  }

  /*!
   * \brief       Set error event.
   * \param[in]   enable If true, the error event is set; if false, it is cleared.
   * \return      Reference to this object.
   * \steady      TRUE
   *
   * \vprivate    pes internal API
   */
  EventTypes& SetErrorEvent(bool enable) noexcept {
    error_event_ = enable;
    return *this;
  }

  /*!
   * \brief       Set software event.
   * \param[in]   enable If true, the software event is set; if false, it is cleared.
   * \return      Reference to this object.
   * \steady      TRUE
   *
   * \vprivate    pes internal API
   */
  EventTypes& SetSoftwareEvent(bool enable) noexcept {
    software_event_ = enable;
    return *this;
  }

  /*!
   * \brief        Compare EventTypes for equality
   * \param[in]    other The EventTypes object to compare against.
   * \return       true if all events are equal, false otherwise.
   * \steady       TRUE
   *
   * \vprivate     pes internal API
   */
  bool operator==(EventTypes const& other) const noexcept {
    return (read_event_ == other.read_event_) && (write_event_ == other.write_event_) &&
           (error_event_ == other.error_event_) && (software_event_ == other.software_event_);
  }

  /*!
   * \brief       Compare EventTypes for inequality
   * \param[in]   other Another EventTypes object to compare against.
   * \return      true if the objects are not equal, false if all events are equal.
   * \steady      TRUE
   * \vprivate    pes internal API
   */
  bool operator!=(EventTypes const& other) const noexcept { return !(*this == other); }

 private:
  /*!
   * \brief Read event
   */
  bool read_event_{false};

  /*!
   * \brief Write event
   */
  bool write_event_{false};

  /*!
   * \brief Error event
   */
  bool error_event_{false};

  /*!
   * \brief   Software event
   * \details Only valid if the callback is registered for a software event.
   */
  bool software_event_{false};
};

/*!
 * \brief    Reactor1 callback type.
 *
 * \details  CallbackHandle is the handle of the callback that is being called.
 *           EventTypes contains the IO events that occurred.
 *           The callback shall not throw exceptions.
 *
 * \context  HandleEvents()
 *
 * \vprivate pes internal API
 */
using CallbackType = vac::language::UniqueFunction<void(CallbackHandle, EventTypes)>;

/*!
 * \brief    Reactor1 Unregister() file descriptor closing options.
 * \vprivate pes internal API
 */
enum class CloseOnUnregister : std::uint32_t {
  kDoNotCloseHandle, /*!< \brief File descriptor should not be closed by the reactor. */
  kCloseHandle       /*!< \brief File descriptor should be closed by the reactor when it is safe to do so. */
};

/*!
 * \brief    Reactor1 HandleEvents() return codes.
 * \vprivate pes internal API
 */
enum class UnblockReason : std::uint32_t {
  kEventsHandledOrUnblock, /*!< \brief Events were handled by the Reactor or the Reactor was unblocked. */
  kTimeout,                /*!< \brief Timeout occurred. */
  kSignal                  /*!< \brief A Signal unblocked the Reactor. */
};

/*!
 * \brief    Os specific resources for Reactor1 usage.
 * \details  Empty struct on Linux.
 * \vprivate pes internal API
 */
struct OsParameters {};

}  // namespace reactor1
}  // namespace io
}  // namespace osabstraction

#endif  // LIB_COMMON_LIBOSABSTRACTION_LINUX_INCLUDE_OSABSTRACTION_IO_REACTOR1_TYPES_H_
