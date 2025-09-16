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
 *        \brief  Defines the producer of a single-producer-single-consumer queue.
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_MEMORY_CONTAINER_SPSC_QUEUE_PRODUCER_H_
#define LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_MEMORY_CONTAINER_SPSC_QUEUE_PRODUCER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "amsr/core/abort.h"
#include "amsr/core/result.h"
#include "amsr/memory/container/internal/spsc_queue.h"
#include "amsr/memory/container/types.h"
#include "amsr/memory/memory_types.h"
#include "amsr/memory/read_writable_typed_memory_view.h"
#include "amsr/memory/readable_typed_memory_view.h"
#include "osabstraction/osab_error_domain.h"

namespace amsr {
namespace memory {
namespace container {

/*!
 * \brief       Producer of a single-producer-single-consumer queue.
 *
 * \details     Creates and handles the single-producer-single-consumer queue using the provided memory views to the
 *              queue data structures (buffer, head, tail).
 *
 * \tparam      T   Data type of every queue element.
 *
 * \vprivate    Vector product internal API
 */
template <typename T>
class SPSCQueueProducer final {
 public:
  /*!
   * \brief       Creates a Producer of a single-producer-single-consumer queue.
   *
   * \param[in]   view_to_buffer  view to the readable, writable mapped memory for the queue elements buffer.
   *                              Size of the view has to be lower or equal than UINT32_MAX/2.
   * \param[in]   view_to_head    view to the readable, writable mapped memory for the queue head index.
   *                              Referenced index address has to be 8-byte aligned.
   *                              Size of the span has to be at least 1.
   * \param[in]   view_to_tail    view to the readable mapped memory for the queue tail index.
   *                              Referenced index address has to be 8-byte aligned.
   *                              Size of the span has to be at least 1.
   *
   * \context     ANY
   *
   * \pre         -
   *
   * \reentrant   FALSE
   * \synchronous TRUE
   * \threadsafe  FALSE
   * \steady      TRUE
   */
  /*!
   * \internal
   *  - Initializes internal member.
   *  - Performs checks on input parameters and aborts execution if parameters violate contract.
   *  - Initializes the head index in the referenced memory.
   * \endinternal
   */
  explicit SPSCQueueProducer(ReadWritableTypedMemoryView<T> view_to_buffer,
                             ReadWritableTypedMemoryView<QueueIndex> view_to_head,
                             ReadableTypedMemoryView<QueueIndex> view_to_tail) noexcept
      // VCA_OSA_CALL_OBJ_PASS_VOID
      : queue_{view_to_buffer.GetSpan(), &view_to_head.GetFirstElement(), &view_to_tail.GetFirstElement()} {
    if (view_to_buffer.GetSpan().size() > static_cast<std::size_t>(UINT32_MAX / 2U)) {
      ::amsr::core::Abort("SPSCQueueProducer created with buffer size greater than UINT32_MAX/2.");
    }

    if (view_to_head.GetAlignment().value < Alignment{8}.value) {
      ::amsr::core::Abort("SPSCQueueProducer created with not expected head index Span alignment.");
    }
    if (view_to_tail.GetAlignment().value < Alignment{8}.value) {
      ::amsr::core::Abort("SPSCQueueProducer created with not expected tail index Span alignment.");
    }
    view_to_head.GetFirstElement().value.store(0, std::memory_order_release);
  }

  /*!
   * \brief       Deleted copy constructor.
   */
  SPSCQueueProducer(SPSCQueueProducer const& other) noexcept = delete;

  /*!
   * \brief       Move constructs a SPSCQueueProducer.
   * \param[in]   other   SPSCQueueProducer to move construct from.
   *
   * \steady      TRUE
   */
  SPSCQueueProducer(SPSCQueueProducer&& other) noexcept = default;

  /*!
   * \brief       Deleted assignment operator.
   */
  SPSCQueueProducer& operator=(SPSCQueueProducer const& other) & noexcept = delete;

  /*!
   * \brief       Move assigns a SPSCQueueProducer.
   * \param[in]   other   SPSCQueueProducer to move assign.
   * \return      Reference to this object.
   *
   * \steady      TRUE
   */
  SPSCQueueProducer& operator=(SPSCQueueProducer&& other) & noexcept = default;

  /*!
   * \brief       Destroys the Producer.
   *
   * \context     ANY
   *
   * \pre         -
   *
   * \reentrant   FALSE
   * \synchronous TRUE
   * \threadsafe  FALSE
   * \steady      TRUE
   *
   * \spec
   *   requires true;
   * \endspec
   */
  ~SPSCQueueProducer() noexcept = default;

  /*!
   * \copydoc ::amsr::memory::container::internal::SPSCQueue::GetCapacity()
   *
   * \steady      TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  auto GetCapacity() const noexcept -> QueueSlots { return queue_.GetCapacity(); }

  /*!
   * \copydoc ::amsr::memory::container::internal::SPSCQueue::GetFillLevel()
   *
   * \steady      TRUE
   */
  auto GetFillLevel() noexcept -> ::amsr::core::Result<QueueSlots> { return queue_.GetFillLevel(); }

  /*!
   * \copydoc ::amsr::memory::container::internal::SPSCQueue::GetMaxFillLevel()
   *
   * \steady      TRUE
   */
  auto GetMaxFillLevel() noexcept -> ::amsr::core::Result<QueueSlots> { return queue_.GetMaxFillLevel(); }

  /*!
   * \copydoc ::amsr::memory::container::internal::SPSCQueue::Produce()
   *
   * \steady      TRUE
   */
  auto Produce(T const& element) noexcept -> ::amsr::core::Result<bool> { return queue_.Produce(element); }

 private:
  /*!
   * \brief       Managed queue object.
   */
  internal::SPSCQueue<T> queue_;
};

}  // namespace container
}  // namespace memory
}  // namespace amsr

#endif  // LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_MEMORY_CONTAINER_SPSC_QUEUE_PRODUCER_H_
