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
/*!        \file  someip_daemon_client/internal/types.h
 *        \brief  SOME/IP Daemon Client Service Instance Types used for proxy and server.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CLIENT_INCLUDE_SOMEIP_DAEMON_CLIENT_INTERNAL_TYPES_H_
#define LIB_SOMEIP_DAEMON_CLIENT_INCLUDE_SOMEIP_DAEMON_CLIENT_INTERNAL_TYPES_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <algorithm>
#include <utility>
#include "amsr/someip_daemon_client/external/libvac/flexible_unique_memory_buffer_allocator.h"
#include "ara/core/abort.h"
#include "ara/core/vector.h"
#include "osabstraction/io/io_buffer.h"
#include "someipd_app_protocol/internal/message.h"
#include "vac/memory/allocator/memory_buffer_ptr.h"
#include "vac/memory/non_zeroing_allocator.h"

namespace amsr {
namespace someip_daemon_client {
namespace internal {

/*!
 * \brief Type-alias for the const I/O memory buffer.
 */
using ConstIOBuffer = osabstraction::io::ConstIOBuffer;

/*!
 * \brief Type-alias for a container of const I/O memory buffers.
 */
using ConstIOBufferContainer = ara::core::Vector<ConstIOBuffer>;
/*!
 * \brief Type-alias for ProxySDRequiredServiceInstanceType.
 */
using ProxySDRequiredServiceInstanceType = ::amsr::someipd_app_protocol::internal::ServiceInstanceIdentificationData;
/*!
 * \brief Type-alias for ProxyCreationRequiredServiceInstanceType.
 */
using ProxyCreationRequiredServiceInstanceType =
    ::amsr::someipd_app_protocol::internal::ServiceInstanceIdentificationData;
/*!
 * \brief Type-alias for ProvidedServiceInstanceType.
 */
using ProvidedServiceInstanceType = ::amsr::someipd_app_protocol::internal::ServiceInstanceIdentificationData;

/*!
 * \brief RequiredEventType Structure used for events.
 */
struct RequiredEventType {
  /*!
   * \brief Exact SOME/IP service id of the service.
   */
  ::amsr::someip_protocol::internal::ServiceId service_id;
  /*!
   * \brief Exact SOME/IP instance id of the service.
   */
  ::amsr::someip_protocol::internal::InstanceId instance_id;
  /*!
   * \brief Exact event id of the service.
   */
  ::amsr::someip_protocol::internal::EventId event_id;
  /*!
   * \brief Exact major version of the service.
   */
  ::amsr::someip_protocol::internal::MajorVersion major_version;
  /*!
   * \brief Exact minor version of the service.
   */
  ::amsr::someip_protocol::internal::MinorVersion minor_version;
  /*!
   * \brief Subscriber client id.
   */
  ::amsr::someip_protocol::internal::ClientId client_id;
};

/*!
 * \brief RequestedServiceType Structure.
 */
struct RequestedServiceType {
  /*!
   * \brief Exact SOME/IP service id of the service.
   */
  ::amsr::someip_protocol::internal::ServiceId service_id;

  /*!
   * \brief Exact SOME/IP instance id of the service.
   */
  ::amsr::someip_protocol::internal::InstanceId instance_id;

  /*!
   * \brief Exact major version of the service.
   */
  ::amsr::someip_protocol::internal::MajorVersion major_version;

  /*!
   * \brief Exact minor version of the service.
   */

  ::amsr::someip_protocol::internal::MinorVersion minor_version;
  /*!
   * \brief Exact client ID which was requested previously and shall be released.
   */
  ::amsr::someip_protocol::internal::ClientId client_id;
};

/*!
 * \brief The type of an entry in the transmit queue.
 */
class TransmitQueueEntry final {
 public:
  /*!
   * \brief Destructs the transmit queue entry.
   * \steady FALSE
   */
  ~TransmitQueueEntry() noexcept = default;

  TransmitQueueEntry(TransmitQueueEntry const&) = delete;
  auto operator=(TransmitQueueEntry const&) & -> TransmitQueueEntry& = delete;

  /*!
   * \brief Move constructor.
   * \steady TRUE
   */
  // VCA_SDCL_COMPILER_GENERATED_CONSTRUCTOR_IS_SAFE
  TransmitQueueEntry(TransmitQueueEntry&&) noexcept = default;

  /*!
   * \brief  Move assignment constructor.
   * \return Reference to a TransmitQueueEntry object.
   * \steady TRUE
   */
  // VCA_SDCL_FUNCTION_NO_PRECONDITION
  auto operator=(TransmitQueueEntry&&) & noexcept -> TransmitQueueEntry& = default;

  /*!
   * \brief     Constructor of TransmitQueueEntry.
   * \details   This constructor copies the contents of the ipc headers into a new buffer. It also takes ownership of
   *            the unique pointer which holds the message payload to be sent.
   * \param[in] ipc_header A reference to a memory buffer containing the ipc headers of the message.
   * \param[in] packet     A memory buffer containing the payload of the message.
   * \pre       Both ipc_header and packet points to valid memory buffers.
   * \context   App
   * \reentrant FALSE
   * \steady TRUE
   */
  explicit TransmitQueueEntry(vac::memory::allocator::MemoryBuffer const& ipc_header,
                              vac::memory::allocator::MemoryBufferPtr packet) noexcept
      // VCA_SDCL_CALLING_CONSTRUCTOR_METHOD
      : packet_{std::move(packet)} {
    amsr::someip_daemon_client::external::libvac::FlexibleUniqueMemoryBufferAllocator<
        vac::memory::NonZeroingAllocator<std::uint8_t>>
        allocator{};
    amsr::core::Result<vac::memory::allocator::MemoryBufferPtr> allocated_packet_result{
        // VCA_SDCL_SLC10_SLC22_MEMBER_OBJECT_REF_FOR_MEMBER_FUNCTION_CALL_HAS_LONGER_LIFETIME
        allocator.Allocate(ipc_header.GetView(0U).size())};
    if (!allocated_packet_result.HasValue()) {
      amsr::core::Abort("Failed to allocate memory buffer.");
    }

    // VCA_SDCL_SLC10_SLC22_MEMBER_OBJECT_REF_FOR_MEMBER_FUNCTION_CALL_HAS_LONGER_LIFETIME
    ipc_header_ = std::move(allocated_packet_result.Value());
    // VECTOR NC AutosarC++17_10-M0.3.1: MD_SOMEIPDAEMONCLIENT_M0.3.1_Previously_checked_pointer
    static_cast<void>(
        // VCA_SDCL_SLC10_SLC22_OBJECT_REF_FOR_MEMBER_FUNCTION_CALL_IS_ALREADY_NULL_CHECKED
        std::copy(ipc_header.GetView(0U).begin(), ipc_header.GetView(0).end(), ipc_header_->GetView(0U).begin()));
  }

  /*!
   * \brief     Get a reference to the buffer which contains the ipc headers for the packet to be sent.
   * \return    A reference to the buffer containing the ipc headers.
   * \pre       -
   * \context   Reactor
   * \reentrant FALSE
   * ?steady TRUE
   */
  auto GetIpcHeader() const noexcept -> vac::memory::allocator::MemoryBuffer const& { return *ipc_header_; }

  /*!
   * \brief     Get a reference to the buffer which contains the data payload to be sent.
   * \return    A reference to the buffer containing the packet.
   * \pre       -
   * \context   Reactor
   * \reentrant FALSE
   * ?steady TRUE
   */
  auto GetPacket() const noexcept -> vac::memory::allocator::MemoryBuffer const& { return *packet_; }

 private:
  /*!
   * \brief A pointer to a buffer that contains the ipc headers of the message.
   */
  // VCA_SDCL_DESTRUCTOR_OF_UNIQUE_PTR, VCA_SDCL_DESTRUCTOR_NO_PRECONDITION
  vac::memory::allocator::MemoryBufferPtr ipc_header_{};

  /*!
   * \brief A pointer to a buffer that contains the payload of the message.
   */
  vac::memory::allocator::MemoryBufferPtr packet_{};
};

}  // namespace internal
}  // namespace someip_daemon_client
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CLIENT_INCLUDE_SOMEIP_DAEMON_CLIENT_INTERNAL_TYPES_H_
