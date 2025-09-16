/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2022 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/**        \file
 *        \brief  A queue holding packets.
 *
 *      \details  Packets can be added until the buffer is full, then the oldest packets are discarded.
 *
 *         \unit  amsr::ipcmulticast::ClientDaemonImpl::PacketQueue
 *
 *********************************************************************************************************************/

#ifndef LIB_IPCMULTICAST_DAEMON_IMPL_INCLUDE_AMSR_IPCMULTICAST_DAEMON_IMPL_INTERNAL_PACKET_QUEUE_H_
#define LIB_IPCMULTICAST_DAEMON_IMPL_INCLUDE_AMSR_IPCMULTICAST_DAEMON_IMPL_INTERNAL_PACKET_QUEUE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <queue>

#include "amsr/ipcmulticast/types.h"

namespace amsr {
namespace ipcmulticast {
namespace daemon_impl {
namespace internal {

/*!
 * \brief       PacketQueue configuration.
 *
 * \details     The total size is given by \c maximum_buffer_length. The whole queue, i.e., all packets, should send at
 *              ones using a outgoing communication buffer. Therefore, an additional header is used. The \c
 *              header_overhead_size must be at least the size of this additional header to ensure the bulk sending.
 */
struct PacketQueueConfig {
  /*! PacketQueue maximum buffer length */
  std::size_t maximum_buffer_length;

  /*! Type of the PacketQueue header overhead size */
  std::size_t header_overhead_size;
};

/*!
 * \brief       A queue holding packets.
 *
 * \details     The queue stores the packages. The total size is given by \c maximum_buffer_length. The whole queue,
 *              i.e., all packets, should send at ones using a outgoing communication buffer. Therefore, an additional
 *              header is used. The \c header_overhead_size must be at least the size of this additional header to
 *              ensure the bulk sending.
 */
class PacketQueue final {
 public:
  /*!
   * \brief       The constructor.
   *
   * \param[in]   config   The configuration holding the maximum size of the total queue that must not be lager than the
   *                       outgoing communication buffer. Also holding the reserved header size per packet that must be
   *                       at least the size of the additional header.
   *
   * \context     ANY
   *
   * \pre         -
   *
   * \reentrant   FALSE
   * \synchronous TRUE
   * \threadsafe  FALSE
   */
  explicit PacketQueue(PacketQueueConfig const& config) noexcept;

  /*! Destructor. */
  ~PacketQueue() noexcept = default;  // VCA_IPCMULTICAST_SPC_15_DEFAULT_CTOR_DTOR_MOLE_1298

  PacketQueue(PacketQueue const&) = delete;
  PacketQueue(PacketQueue&&) = delete;
  auto operator=(PacketQueue const&) & -> PacketQueue& = delete;
  auto operator=(PacketQueue&&) & -> PacketQueue& = delete;

  /*!
   * \brief       Enqueues a packet to the queue.
   *
   * \details     The queue holds a copy of the packet. In case the packet does not fit into the queue, old packets will
   * be dropped in order to make space.
   *
   * \param[in]   packet_view   A view to a packet. The packet view must not be modified during function call.
   *
   * \return      The number of packets that have been dropped. (0 in case that no packets have been dropped)
   *
   * \context     ANY
   *
   * \pre         -
   *
   * \reentrant   FALSE
   * \synchronous TRUE
   * \threadsafe  FALSE
   */
  auto Emplace(PacketView const& packet_view) noexcept -> std::size_t;

  /*!
   * \brief       Pops the first packet from queue.
   *
   * \context     ANY
   *
   * \pre         -
   *
   * \reentrant   FALSE
   * \synchronous TRUE
   * \threadsafe  FALSE
   * \spec
   *   requires true;
   * \endspec
   */
  void Pop() noexcept;

  /*!
   * \brief       Gives a view to the first element.
   *
   * \return      View to the first element or an empty view when the queue is empty.
   *
   * \context     ANY
   *
   * \pre         -
   *
   * \reentrant   FALSE
   * \synchronous TRUE
   * \threadsafe  FALSE
   */
  auto Front() const noexcept -> PacketView;

  /*!
   * \brief       Checks if the queue has no packets.
   *
   * \return      true if the queue is empty, false otherwise.
   *
   * \context     ANY
   *
   * \pre         -
   *
   * \reentrant   FALSE
   * \synchronous TRUE
   * \threadsafe  FALSE
   */
  auto Empty() const noexcept -> bool;

  /*!
   * \brief       Returns the number of packets.
   *
   * \return      The number of packets in the queue.
   *
   * \context     ANY
   *
   * \pre         -
   *
   * \reentrant   FALSE
   * \synchronous TRUE
   * \threadsafe  FALSE
   */
  auto Size() const noexcept -> std::size_t;

  /*!
   * \brief       Clears the queue.
   *
   * \context     ANY
   *
   * \pre         -
   *
   * \reentrant   FALSE
   * \synchronous TRUE
   * \threadsafe  FALSE
   */
  void Clear() noexcept;

 private:
  /*! The queue of packets. */
  std::queue<PacketBuffer> queue_{};

  /*! The length of all buffered packets in the queue. */
  std::size_t used_buffer_length_{0};

  /*! The maximum length of all buffered packets in the queue. */
  std::size_t maximum_buffer_length_;

  /*! The reserved header size per packet. */
  std::size_t header_overhead_size_;
};

}  // namespace internal
}  // namespace daemon_impl
}  // namespace ipcmulticast
}  // namespace amsr

#endif  // LIB_IPCMULTICAST_DAEMON_IMPL_INCLUDE_AMSR_IPCMULTICAST_DAEMON_IMPL_INTERNAL_PACKET_QUEUE_H_
