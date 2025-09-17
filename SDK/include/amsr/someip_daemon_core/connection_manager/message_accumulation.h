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
 *        \brief  Class to accumulate messages.
 *
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONNECTION_MANAGER_MESSAGE_ACCUMULATION_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONNECTION_MANAGER_MESSAGE_ACCUMULATION_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <chrono>
#include <memory>  // shared_ptr
#include "amsr/someip_daemon_core/logging/ara_com_logger.h"
#include "amsr/steady_timer/timer_interface.h"
#include "amsr/steady_timer/timer_manager_interface.h"
#include "ara/core/span.h"
#include "ara/core/string_view.h"
#include "ara/core/variant.h"
#include "ara/core/vector.h"
#include "someip-protocol/internal/pdu_message.h"
#include "someip-protocol/internal/someip_message.h"
#include "vac/language/unique_function.h"

namespace amsr {
namespace someip_daemon_core {
namespace connection_manager {

/*!
 * \brief           Class to accumulate raw messages for a period of time
 * \trace           SPEC-5951437
 */
class MessageAccumulation final {
 public:
  /*!
   * \brief           Type of stored packages. Can store a SOMEIP or PDU packet
   */
  using PacketType = ara::core::Variant<std::shared_ptr<someip_protocol::internal::SomeIpMessage>,
                                        std::shared_ptr<someip_protocol::internal::PduMessage>>;
  /*!
   * \brief           Type of the callback. It gets a sequence of sequences of io_vec and has to return if the send was
   *                  successful. The outer span level of the sequence represents fragments. The inner a scatter-gather
   *                  buffer
   */
  using CallbackType =
      vac::language::UniqueFunction<bool(ara::core::Span<ara::core::Span<osabstraction::io::ConstIOBuffer>>)>;
  /*!
   * \brief           constructor
   * \param[in]       timer_manager
   *                  Timer manager.
   * \param[in]       max_accumulated_bytes
   *                  Maximum total bytes that the queue can accumulate. Any bundle of messages sent will have this
   *                  total byte size or lower.
   * \param[in]       max_fragment_bytes
   *                  The accumulated data can be split into multiple fragments up to a give size. Such fragments will
   *                  be rounded down in size to only contain full messages. This argument controls the maximum size in
   *                  bytes of each fragment. Messages exceeding this size will still be sent as a single fragment. If
   *                  fragmentation is to be disabled use a value equal or bigger to "max_size", like
   *                  numeric_limits<std::size_t>::max().
   * \param[in]       max_fragment_n_iovec
   *                  The maximum number of io-vectors (ConstIOBuffer) that a fragment (aka 2nd level of span nesting)
   *                  can contain. This usually comes from an imposed OS limit.
   * \param[in]       wire_send
   *                  Function/callback that will be used to send the bytes.
   * \context         Init
   * \reentrant       FALSE
   * \steady          FALSE
   */
  MessageAccumulation(amsr::steady_timer::TimerManagerInterface* timer_manager, std::size_t const max_accumulated_bytes,
                      std::size_t const max_fragment_bytes, std::size_t const max_fragment_n_iovec,
                      CallbackType wire_send) noexcept;
  /*!
   * \brief           destructor
   * \context         ANY
   * \steady          FALSE
   */
  ~MessageAccumulation() noexcept;

  MessageAccumulation() = delete;
  MessageAccumulation(MessageAccumulation const&) = delete;
  MessageAccumulation(MessageAccumulation&&) = delete;
  MessageAccumulation& operator=(MessageAccumulation const&) = delete;
  MessageAccumulation& operator=(MessageAccumulation&&) = delete;

  /*!
   * \brief           Add packet for accumulation
   * \param[in]       packet
   *                  Data to send or accumulate.
   * \return          false if there was a transmission error, otherwise true.
   * \context         APP
   * \steady          FALSE
   */
  bool AddPacket(PacketType packet) noexcept;

 private:
  /*!
   * \brief Private function. Updates the active timer with the delay of a new scheduled message.
   * \context           APP
   */
  void UpdateTimer(std::chrono::nanoseconds const new_delay) noexcept;

  /*!
   * \brief Private function. Implementation detail. See implementation.
   * \return false if there was a transmission error, otherwise true.
   * \context           ANY
   * \steady FALSE
   */
  bool Flush() noexcept;

  /*!
   * \brief           Get a view of the data that is currently enqueued as a 2 level span
   *
   * \return          ara::core::Span<ara::core::Span<osabstraction::io::ConstIOBuffer>> view
   * \steady          FALSE
   */
  ara::core::Span<ara::core::Span<osabstraction::io::ConstIOBuffer>> GetQueueViewAsFragments() noexcept;

  /*!
   * \brief Returns if the timer has started.
   * \return true if the timer has started, false otherwise.
   * \context           ANY
   */
  bool TimerStarted() const noexcept;

  /*!
   * \brief           queue
   */
  ara::core::Vector<PacketType> queue_{};
  /*!
   * \brief           queue storing the amount of iovecs on each fragment. It's a member variable for memory reuse
   */
  ara::core::Vector<std::size_t> n_iovec_fragment_{};
  /*!
   * \brief           RAW storage of ConstIOBuffer for invoking callbacks. It's a member variable for memory reuse
   */
  ara::core::Vector<osabstraction::io::ConstIOBuffer> io_vecs_{};
  /*!
   * \brief           Queue storing the amount of messages on each fragment. It's a member variable for memory reuse
   */
  ara::core::Vector<ara::core::Span<osabstraction::io::ConstIOBuffer>> fragments_{};
  /*!
   * \brief           threshold in bytes for the full queue
   */
  std::size_t const q_threshold_bytes_;
  /*!
   * \brief           maximum size of a fragment in bytes. See MessageAccumulation::MessageAccumulation documentation
   */
  std::size_t const max_fragment_bytes_;
  /*!
   * \brief           maximum number of io-vectors inside a fragment. See MessageAccumulation::MessageAccumulation
   *                  documentation
   */
  std::size_t const max_fragment_n_iovec_;
  /*!
   * \brief           bytes enqueued
   */
  std::size_t q_bytes_{0U};
  /*!
   * \brief callback
   */
  CallbackType wire_send_;
  /*!
   * \brief The timer manager, used to access the current clock to compute a next expiry (now + timeout)
   */
  amsr::steady_timer::TimerManagerInterface* timer_manager_;
  /*!
   * \brief The timer to handle transmission timeouts
   */
  std::unique_ptr<amsr::steady_timer::TimerInterface> accumulation_timer_;
  /*!
   * \brief Our logger.
   */
  someip_daemon_core::logging::AraComLogger logger_{
      someip_daemon_core::logging::kServiceDiscoveryLoggerContextId,
      someip_daemon_core::logging::kServiceDiscoveryLoggerContextDescription,
      ara::core::StringView{"UDP message accumulation"}};
};

}  // namespace connection_manager
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONNECTION_MANAGER_MESSAGE_ACCUMULATION_H_
