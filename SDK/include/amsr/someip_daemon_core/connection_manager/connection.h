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
 *        \brief  Implementation of connection class
 *
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONNECTION_MANAGER_CONNECTION_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONNECTION_MANAGER_CONNECTION_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <algorithm>
#include <cassert>
#include "amsr/net/ip/address.h"
#include "amsr/someip_daemon_core/configuration/configuration_types.h"
#include "ara/core/vector.h"

namespace amsr {
namespace someip_daemon_core {
namespace connection_manager {

/*!
 * \brief           Connection base class
 * \tparam          SenderType
 *                  Type of the corresponding sender.
 */
template <class SenderType>
class Connection {
 public:
  // Delete default constructor.
  Connection() = delete;

  /*!
   * \brief           Destructs instance of connection
   * \context         ANY
   * \pre             -
   * \threadsafe      FALSE
   * \reentrant       FALSE
   * \steady          TRUE
   */
  virtual ~Connection() noexcept = default;

  Connection(Connection const &) = delete;
  Connection(Connection &&) = delete;
  Connection &operator=(Connection const &) = delete;
  Connection &operator=(Connection &&) = delete;

  /*!
   * \brief           A sender pointer type
   */
  using SenderPtr = typename std::add_pointer<SenderType>::type;

  /*!
   * \brief           Constructs instance of Connection
   * \param[in]       address
   *                  The address of the requesting connection.
   * \param[in]       port
   *                  The port of the requesting connection.
   * \context         Init
   * \pre             -
   * \threadsafe      TRUE
   * \reentrant       FALSE
   * \steady          TRUE
   */
  Connection(someip_daemon_core::IpAddress const &address, amsr::net::ip::Port const port) noexcept
      : address_{address}, port_{port}, senders_{}, removed_senders_{} {}

  /*!
   * \brief           Indicates whether this connection is connected
   * \return          true if connection is connected, otherwise false.
   * \context         ANY
   * \pre             -
   * \threadsafe      TRUE
   * \reentrant       FALSE
   * \steady          TRUE
   */
  virtual bool IsConnected() const noexcept = 0;

  /*!
   * \brief           Getter function to return the remote IP address
   * \return          Returns the remote IP address.
   * \context         ANY
   * \pre             -
   * \threadsafe      TRUE
   * \reentrant       FALSE
   * \steady          TRUE
   */
  someip_daemon_core::IpAddress const &GetAddress() const noexcept { return address_; }

  /*!
   * \brief           Getter function to return the remote port number
   * \return          A remote port number.
   * \context         ANY
   * \pre             -
   * \threadsafe      TRUE
   * \reentrant       FALSE
   * \steady          TRUE
   */
  amsr::net::ip::Port GetPort() const noexcept { return port_; }

  /*!
   * \brief           Registers a sender
   * \param[in]       sender
   *                  A sender.
   * \context         ANY
   * \pre             -
   * \threadsafe      TRUE
   * \reentrant       FALSE
   * \steady          TRUE
   */
  void RegisterSender(SenderPtr sender) noexcept { senders_.push_back(sender); }

  /*!
   * \brief           Unregisters a sender
   * \param[in]       sender
   *                  A sender.
   * \context         ANY
   * \pre             -
   * \threadsafe      FALSE
   * \reentrant       FALSE
   * \steady          TRUE
   */
  void UnregisterSender(SenderPtr sender) noexcept {
    if (inside_notify_) {
      RemoveSenderInsideNotify(sender);
    } else {
      RemoveSender(sender);
    }
  }

 private:
  /*!
   * \brief           A sender pointer type
   */
  using SenderContainer = ara::core::Vector<SenderPtr>;

  /*!
   * \brief           Alias for the sender container const iterator
   */
  using SenderContainerConstIterator = typename SenderContainer::const_iterator;

 protected:
  /*!
   * \brief           Get the size of the container of the removed senders
   * \return          The size of the container of the removed senders
   * \context         ANY
   * \pre             -
   * \threadsafe      FALSE
   * \steady          TRUE
   */
  std::size_t GetRemovedSendersSize() const noexcept { return removed_senders_.size(); }

  // VECTOR NC AutosarC++17_10-A15.4.2: MD_SomeIpDaemon_AutosarC++17_10-A15.4.2_A15.5.3_STL_exceptions
  // VECTOR NC AutosarC++17_10-A15.5.3: MD_SomeIpDaemon_AutosarC++17_10-A15.4.2_A15.5.3_STL_exceptions
  /*!
   * \brief           Notifies registered senders about a change in the connection state
   * \context         ANY
   * \pre             -
   * \threadsafe      FALSE
   * \reentrant       FALSE
   *
   * \internal
   * Set boolean flag indicating senders are being notified to true. Clear the removed senders list. For each sender: If
   * this sender is not among the removed senders notify the sender. (The removed senders list can be updated while this
   * function is executing, see for example function: RemoveSenderInsideNotify()) Set boolean flag indicating senders
   * are being notified to false.
   *
   * Erase from the senders list, all senders found in the removed senders list.
   * \endinternal
   * \steady          TRUE
   */
  void Notify() noexcept {
    inside_notify_ = true;
    removed_senders_.resize(0);

    for (SenderPtr const s : senders_) {
      SenderContainerConstIterator const it{std::find(removed_senders_.cbegin(), removed_senders_.cend(), s)};
      if (it == removed_senders_.cend()) {
        s->Notify();
      }
    }

    inside_notify_ = false;

    typename SenderContainer::iterator const iter_begin{senders_.begin()};
    typename SenderContainer::iterator const iter_end{senders_.end()};

    typename SenderContainer::iterator const new_begin_to_erase{
        std::remove_if(iter_begin, iter_end, [this](SenderPtr const &s) noexcept {
          return std::find(removed_senders_.cbegin(), removed_senders_.cend(), s) != removed_senders_.cend();
        })};

    static_cast<void>(senders_.erase(new_begin_to_erase, senders_.end()));
  }

  /*!
   * \brief           Removes a sender while there are no active notification in progress
   * \param[in]       sender
   *                  A registered sender to be removed from active senders.
   * \context         ANY
   * \pre             -
   * \threadsafe      FALSE
   * \reentrant       FALSE
   * \internal
   * Find the sender among the stored senders and remove it from the senders list.
   * \endinternal
   * \steady          TRUE
   */
  void RemoveSender(SenderPtr sender) noexcept {
    assert(!inside_notify_);
    SenderContainerConstIterator const it{std::find(senders_.cbegin(), senders_.cend(), sender)};
    assert(it != senders_.cend());
    static_cast<void>(senders_.erase(it));
  }

  /*!
   * \brief           Removes a sender while a notification is in progress
   * \param[in]       sender
   *                  A registered sender to be removed from active senders.
   * \context         ANY
   * \pre             -
   * \threadsafe      FALSE
   * \reentrant       FALSE
   * \internal
   * Find the sender among the senders and add it to the sender removal list.
   * \endinternal
   * \steady          TRUE
   */
  void RemoveSenderInsideNotify(SenderPtr sender) noexcept {
    assert(inside_notify_);
    SenderContainerConstIterator const it{std::find(senders_.cbegin(), senders_.cend(), sender)};
    assert(it != senders_.cend());
    static_cast<void>(it);  // To enable release builds (optimized) with -Werror.
    removed_senders_.push_back(sender);
  }

 private:
  /*!
   * \brief           A remote IP address
   */
  someip_daemon_core::IpAddress const address_;

  /*!
   * \brief           A remote port number
   */
  amsr::net::ip::Port const port_;

  /*!
   * \brief           A container of registered senders
   */
  SenderContainer senders_;

  /*!
   * \brief           A container of removed senders. (Used when senders are removed while senders are being notified.)
   */
  SenderContainer removed_senders_;

  /*!
   * \brief           Indicates whether a notification of registered senders is in progress or not
   */
  bool inside_notify_{false};
};

}  // namespace connection_manager
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONNECTION_MANAGER_CONNECTION_H_
