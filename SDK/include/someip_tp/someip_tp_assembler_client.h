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
/**        \file  someip_tp_assembler_client.h
 *        \brief  Implementation of SOMEIP-TP Assembler client
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_TP_INCLUDE_SOMEIP_TP_SOMEIP_TP_ASSEMBLER_CLIENT_H_
#define LIB_SOMEIP_TP_INCLUDE_SOMEIP_TP_SOMEIP_TP_ASSEMBLER_CLIENT_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/unique_ptr.h"
#include "ara/core/optional.h"
#include "someip-protocol/internal/marshalling.h"
#include "someip_tp.h"
#include "vac/memory/allocator/flexible/flexible_unique_memory_buffer_allocator.h"
#include "vac/memory/allocator/memory_buffer_ptr.h"
#include "vac/memory/memory_buffer_wrapper.h"
#include "vac/memory/non_zeroing_allocator.h"

namespace amsr {
namespace someip_tp {

/*!
 * \brief        Each assembler client instance holds a dedicated assembler for
 *               the de-fragmentation of incoming SOME/IP-TP segments.
 */
class TransportProtocolAssemblerClient final {
 public:
  /*!
   * \brief Type for MemoryBufferAllocator interface.
   */
  using Allocator = vac::memory::allocator::MemoryBufferAllocator;
  // VECTOR NC AutosarC++17_10-A15.4.2: MD_SomeIpDaemon_AutosarC++17_10_A15.4.2_noexcept_specifier
  /*!
   * \brief             Construct one client that holds the reference to one assembler.
   * \param[in]         client_id   Client from the SOME/IP header.
   * \param[in]         current_session Current session ID.
   * \param[in]         maximum_original_rx_length The maximum buffer size to provide based on the  configuration value.
   * \param[in]         num_rx_buffers The number of preallocated buffers based on the  configuration value.
   * \context           Init
   * \pre               -
   * \threadsafe        FALSE
   * \reentrant         FALSE
   * \steady            TRUE
   */
  TransportProtocolAssemblerClient(someip_protocol::internal::ClientId client_id,
                                   someip_protocol::internal::SessionId current_session,
                                   someip_protocol::internal::LengthField maximum_original_rx_length,
                                   someip_tp::SomeIpTpNumMessageBuffers num_rx_buffers) noexcept
      : client_id_{client_id},
        reallocator_{nullptr},
        assembler_{std::max(1u, num_rx_buffers), maximum_original_rx_length, current_session} {
    if (num_rx_buffers == 0u) {
      reallocator_ = MakeUniqueOrAbort<FlexibleAllocator>();
    }
  }

  /*!
   * \brief             Getter function to get the client ID assigned to this assembler.
   * \return            Returns the client ID assigned for this assembler.
   * \pre               -
   * \context           ANY
   * \threadsafe        TRUE
   * \reentrant         FALSE
   * \steady            TRUE
   */
  someip_protocol::internal::ClientId GetClientId() const noexcept { return client_id_; }
  /*!
   * \brief             Getter function to get the assembler assigned to a certain client.
   * \return            Returns a reference to the SOMEIP/TP message reception assembler.
   * \pre               -
   * \context           ANY
   * \threadsafe        TRUE
   * \reentrant         FALSE
   * \steady            TRUE
   */
  SomeIpTpMessageReceptionAssembler& GetAssembler() { return assembler_; }

  /*!
   * \brief             Function to create someipmessage using the clients configured allocator.
   * \return            Returns result from message creation.
   * \pre               allocator is not null
   * \context           ANY
   * \threadsafe        TRUE
   * \reentrant         FALSE
   * \steady            TRUE
   */
  ara::core::Result<someip_protocol::internal::SomeIpMessage> GetReassembledMessage() noexcept {
    // someip_protocol::internal::SomeIpMessage is non-copiable and we can't use early return types, hence the
    // "Optional" workaround
    using ResultType = ara::core::Result<someip_protocol::internal::SomeIpMessage>;
    ara::core::Optional<ResultType> ret{};
    vac::memory::allocator::MemoryBufferPtr msg{assembler_.ReleaseMessage()};

    if (msg && (!reallocator_)) {
      // directly forward the message from the deterministic allocator to the upper layers
      ret.emplace(someip_protocol::internal::SomeIpMessage::CreateSomeIpMessage(
          amsr::AllocateUnique<MemoryBufferWrapper>(ara::core::PolymorphicAllocator<MemoryBufferWrapper>{},
                                                    std::move(msg))
              .Value()));
    } else if (msg && reallocator_) {
      // reallocate and let "msg" go out of scope and back to the assembler's deterministic allocator immediately.
      vac::memory::allocator::MemoryBuffer::MemoryBufferView const view{msg->GetView(0)};
      ara::core::Result<MemoryBufferPtr> alloc_ret{reallocator_->Allocate(view.size())};
      if (alloc_ret.HasValue()) {
        // VECTOR NC AutosarC++17_10-A18.5.8: MD_SomeIpDaemon_AutosarC++17_10-A18.5.8_false_positive_local_heap
        amsr::UniquePtr<MemoryBufferWrapper> wrapper{
            amsr::AllocateUnique<MemoryBufferWrapper>(ara::core::PolymorphicAllocator<MemoryBufferWrapper>{},
                                                      std::move(alloc_ret.Value()))
                .Value()};
        static_cast<void>(wrapper->CopyIn(0U, view.size(), view.data()));
        ret.emplace(someip_protocol::internal::SomeIpMessage::CreateSomeIpMessage(std::move(wrapper)));
      } else {
        ret.emplace(alloc_ret.Error());
      }
    } else {
      ret.emplace(ResultType::FromError(someip_protocol::SomeIpProtocolErrc::error_not_ok,
                                        ara::core::StringView{"No reassembled message available"}));
    }
    return std::move(*ret);
  }

 private:
  /*!
   * \brief         Type-alias for the allocator of this controller.
   */
  using MemoryBufferPtr = vac::memory::allocator::MemoryBufferPtr;

  /*!
   * \brief         Type-alias for the allocator of this controller.
   */
  using MemoryBufferWrapper = vac::memory::MemoryBufferWrapper<osabstraction::io::MutableIOBuffer>;
  /*!
   * \brief Type for FlexibleMemoryBufferAllocator.
   */
  using FlexibleAllocator = vac::memory::allocator::flexible::FlexibleUniqueMemoryBufferAllocator<
      vac::memory::NonZeroingAllocator<std::uint8_t>>;
  /*!
   * \brief             The client ID for this assembler.
   */
  someip_protocol::internal::ClientId const client_id_;
  /*!
   * \brief             Allocator used in dynamic mode for reallocating and copying.
   */
  amsr::UniquePtr<FlexibleAllocator> reallocator_;
  /*!
   * \brief             One assembler for this client.
   */
  SomeIpTpMessageReceptionAssembler assembler_;
};

}  // namespace someip_tp
}  // namespace amsr

#endif  // LIB_SOMEIP_TP_INCLUDE_SOMEIP_TP_SOMEIP_TP_ASSEMBLER_CLIENT_H_
