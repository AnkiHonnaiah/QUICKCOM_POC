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
 *        \brief  Defines the single-producer-single-consumer queue.
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_MEMORY_CONTAINER_INTERNAL_SPSC_QUEUE_H_
#define LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_MEMORY_CONTAINER_INTERNAL_SPSC_QUEUE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <algorithm>
#include <cstdint>

#include "amsr/core/abort.h"
#include "amsr/core/result.h"
#include "amsr/core/span.h"
#include "amsr/core/variant.h"
#include "amsr/memory/container/types.h"
#include "amsr/memory/memory_types.h"
#include "amsr/memory/read_writable_typed_memory_view.h"
#include "amsr/memory/readable_typed_memory_view.h"
#include "osabstraction/osab_error_domain.h"

namespace amsr {
namespace memory {
namespace container {
namespace internal {

/*!
 * \brief           Producer internal data structure.
 * \tparam      T   Data type of every queue element.
 */
template <typename T>
class ProducerInternalData final {
 public:
  /*!
   * \brief           Creates a producer specific queue data structures.
   *
   * \param[in]       producer_ptr_to_head      pointer to head index underlying memory address
   * \param[in]       producer_ptr_to_tail      pointer to tail index underlying memory address
   * \param[in]       producer_data_buffer_view pointer to buffer view
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  explicit ProducerInternalData(QueueIndex* producer_ptr_to_head, QueueIndex const* producer_ptr_to_tail,
                                ::amsr::core::Span<T> producer_data_buffer_view)
      : producer_head_index_ptr_{producer_ptr_to_head},
        producer_tail_index_ptr_{producer_ptr_to_tail},
        producer_data_buffer_view_{producer_data_buffer_view} {}

  /*!
   * \brief           Defaulted destructor.
   *
   * \spec
   *   requires true;
   * \endspec
   */
  ~ProducerInternalData() noexcept = default;

  /*!
   * \brief           Deleted copy constructor.
   */
  ProducerInternalData(ProducerInternalData const&) noexcept = delete;

  /*!
   * \brief           Deleted assignment operator.
   */
  ProducerInternalData& operator=(ProducerInternalData const&) & noexcept = delete;

  /*!
   * \brief           Defaulted move constructor.
   */
  ProducerInternalData(ProducerInternalData&&) noexcept = default;

  /*!
   * \brief           Defaulted move assignment operator.
   */
  ProducerInternalData& operator=(ProducerInternalData&&) & noexcept = default;

  /*!
   * \brief    Producer pointer to the atomic head index of the queue in the underlying memory.
   * \details  Head index is mapped to the same physical address for producer and consumer, but with different access
   *           rights.
   */
  QueueIndex* const producer_head_index_ptr_;

  /*!
   * \brief    Consumer pointer to the atomic tail index of the queue in the underlying memory.
   * \details  Tail index is mapped to the same physical address for producer and consumer, but with different access
   *           rights.
   */
  QueueIndex const* const producer_tail_index_ptr_;

  /*!
   * \brief     Producer view to managed buffer.
   *
   * \details   The managed buffer shall be mapped to the same physical memory for the producer and the consumer.
   *            They only have different access rights to the mapped memory.
   */
  ::amsr::core::Span<T> const producer_data_buffer_view_;
};

/*!
 * \brief           Producer internal data structure.
 * \tparam      T   Data type of every queue element.
 */
template <typename T>
class ConsumerInternalData final {
 public:
  /*!
   * \brief           Creates a producer specific queue data structures.
   *
   * \param[in]       consumer_ptr_to_head      pointer to head index underlying memory address
   * \param[in]       consumer_ptr_to_tail      pointer to tail index underlying memory address
   * \param[in]       consumer_data_buffer_view pointer to buffer view
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  ConsumerInternalData(QueueIndex const* consumer_ptr_to_head, QueueIndex* consumer_ptr_to_tail,
                       ::amsr::core::Span<T const> consumer_data_buffer_view)
      : consumer_head_index_ptr_{consumer_ptr_to_head},
        consumer_tail_index_ptr_{consumer_ptr_to_tail},
        consumer_data_buffer_view_{consumer_data_buffer_view} {}

  /*!
   * \brief           Defaulted destructor.
   *
   * \spec
   *   requires true;
   * \endspec
   */
  ~ConsumerInternalData() noexcept = default;

  /*!
   * \brief           Deleted copy constructor.
   */
  ConsumerInternalData(ConsumerInternalData const&) noexcept = delete;

  /*!
   * \brief           Deleted assignment operator.
   */
  ConsumerInternalData& operator=(ConsumerInternalData const&) & noexcept = delete;

  /*!
   * \brief           Defaulted move constructor.
   */
  ConsumerInternalData(ConsumerInternalData&&) noexcept = default;

  /*!
   * \brief           Defaulted move assignment operator.
   */
  ConsumerInternalData& operator=(ConsumerInternalData&&) & noexcept = default;

  /*!
   * \brief    Consumer pointer to the atomic head index of the queue in the underlying memory.
   * \details  Head index is mapped to the same physical address for producer and consumer, but with different access
   *           rights.
   */
  QueueIndex const* const consumer_head_index_ptr_;

  /*!
   * \brief    Consumer pointer to the atomic tail index of the queue in the underlying memory.
   * \details  Tail index is mapped to the same physical address for producer and consumer, but with different access
   *           rights.
   */
  QueueIndex* const consumer_tail_index_ptr_;

  /*!
   * \brief     Consumer view to managed buffer.
   *
   * \details   The managed buffer shall be mapped to the same physical memory for the producer and the consumer.
   *            They only have different access rights to the mapped memory.
   */
  ::amsr::core::Span<T const> const consumer_data_buffer_view_;
};

/*!
 * \brief       Defines a single-producer-single-consumer queue.
 *
 * \details     The single-producer-single-consumer queue expects memory mapped buffer array, head index, tail index.
 *              Depending on the access rights of the passed memory views, only the relevant members/APIs for the
 *              specific use case (producer or consumer) are initialized/provided.
 * \tparam      T   Data type of every queue element.
 */
template <typename T>
class SPSCQueue final {
 public:
  // Assert that the atomics that are used in the ring buffer are always lock-free, otherwise they cannot be used to
  // synchronize access to the ring buffer in the underlying memory. It is assumed that lock-free atomics are also
  // address- free on all architectures supported by AMSR. This is also a precondition for the lock-free ring buffer
  // algorithm to work in underlying memory but this cannot be checked by the compiler.
  static_assert(
      ATOMIC_INT_LOCK_FREE == 2,
      "std::atomic<std::uint32_t> is not always lock-free and can therefore not be used in underlying memory.");

  /*!
   * \brief       Consumer side single-producer-single-consumer queue constructor.
   *
   * \param[in]   consumer_data_buffer_view  view to the buffer of read-only T elements.
   * \param[in]   consumer_ptr_to_head       pointer to the read-only head index.
   * \param[in]   consumer_ptr_to_tail       pointer to read-writable tail index.
   *
   * \context     ANY
   *
   * \pre         -
   *
   * \reentrant   FALSE
   * \synchronous TRUE
   * \threadsafe  FALSE
   */
  /*!
   * \internal
   *  - Initializes consumer relevant members.
   * \endinternal
   */
  explicit SPSCQueue(::amsr::core::Span<T const> consumer_data_buffer_view, QueueIndex const* consumer_ptr_to_head,
                     QueueIndex* consumer_ptr_to_tail) noexcept {
    endpoint_specific_data_.template emplace<ConsumerInternalData<T>>(consumer_ptr_to_head, consumer_ptr_to_tail,
                                                                      consumer_data_buffer_view);
  }

  /*!
   * \brief       Producer side single-producer-single-consumer queue constructor.
   *
   * \param[in]   producer_data_buffer_view  view to the buffer of read-writable T elements.
   * \param[in]   producer_ptr_to_head       pointer to the read-writable head index.
   * \param[in]   producer_ptr_to_tail       pointer to read-only tail index.
   *
   * \context     ANY
   *
   * \pre         producer_data_buffer_view size lower or equal than UINT32_MAX/2.
   *
   * \reentrant   FALSE
   * \synchronous TRUE
   * \threadsafe  FALSE
   */
  /*!
   * \internal
   *  - Initializes producer relevant members.
   * \endinternal
   */
  explicit SPSCQueue(::amsr::core::Span<T> producer_data_buffer_view, QueueIndex* producer_ptr_to_head,
                     QueueIndex const* producer_ptr_to_tail) noexcept {
    endpoint_specific_data_.template emplace<ProducerInternalData<T>>(producer_ptr_to_head, producer_ptr_to_tail,
                                                                      producer_data_buffer_view);
  }

  /*!
   * \brief       Deleted copy constructor.
   */
  SPSCQueue(SPSCQueue const& other) noexcept = delete;

  /*!
   * \brief       Move constructs a SPSCQueue.
   */
  SPSCQueue(SPSCQueue&& other) noexcept = default;

  /*!
   * \brief       Deleted assignment operator.
   */
  SPSCQueue& operator=(SPSCQueue const& other) & noexcept = delete;

  /*!
   * \brief       Move assigns a SPSCQueue.
   */
  SPSCQueue& operator=(SPSCQueue&& other) & noexcept = default;

  /*!
   * \brief       Destroys the Consumer.
   *
   * \context     ANY
   *
   * \pre         -
   *
   * \reentrant   FALSE
   * \synchronous TRUE
   * \threadsafe  FALSE
   *
   * \spec
   *   requires true;
   * \endspec
   */
  ~SPSCQueue() noexcept = default;

  /*!
   * \brief       Returns the capacity of the managed queue.
   *
   * \return      Maximum number of elements the queue can contain.
   *
   * \context     ANY
   *
   * \pre         -
   *
   * \reentrant   FALSE
   * \synchronous TRUE
   * \threadsafe  FALSE
   * \trace       CREQ-Osab-MemorySpScQueueGetCapacity
   *
   * \spec
   *   requires true;
   * \endspec
   */
  /*!
   * \internal
   *  - Returns the size of the element view passed during object construction.
   * \endinternal
   */
  auto GetCapacity() const noexcept -> QueueSlots {
    return (::amsr::core::holds_alternative<ConsumerInternalData<T>>(endpoint_specific_data_))
               ? QueueSlots{::amsr::core::get<ConsumerInternalData<T>>(endpoint_specific_data_)
                                .consumer_data_buffer_view_.size()}
               : QueueSlots{::amsr::core::get<ProducerInternalData<T>>(endpoint_specific_data_)
                                .producer_data_buffer_view_.size()};
  }

  /*!
   * \brief       Returns the number of occupied slots in the managed queue and updates the max_fill_level_ accordingly.
   *
   * \return      Number of elements which are produced but not yet consumed.
   *
   * \error       osabstraction::OsabErrc::kProtocolError      Memory corruption: invalid index values read
   *                                                           from underlying memory.
   *
   * \context     ANY
   *
   * \pre         -
   *
   * \reentrant   FALSE
   * \synchronous TRUE
   * \threadsafe  FALSE
   * \trace       CREQ-Osab-MemorySpScQueueGetFillLevel
   */
  /*!
   * \internal
   *  - Updates the cached indexes with the values in the underlying memory.
   *  - If the indexes are valid:
   *    - Compute and return the number of occupied slots and update max_fill_level_.
   *  - Otherwise return kProtocolError.
   * \endinternal
   */
  auto GetFillLevel() noexcept -> ::amsr::core::Result<QueueSlots> {
    ::amsr::core::Result<QueueSlots> result{osabstraction::OsabErrc::kProtocolError};
    UpdateCachedIndexes();
    if (ValidateCacheIndexes()) {
      QueueSlots const fill_level{GetOccupiedSlots()};
      UpdateMaxFillLevel(fill_level);
      result.EmplaceValue(fill_level);
    }
    return result;
  }

  /*!
   * \brief       Returns the estimated maximum number of occupied slots in the managed queue, since its creation.
   *
   * \details     The returned value is only a pessimistic estimation because it is not possible for one endpoint
   *              to track every operation on the underlying memory. This is acceptable because this is a debug feature
   *              used to estimate valid values for the queue capacity.
   *              Snapshots of the physical buffer used for this estimation are taken at every public API except
   *              from GetCapacity().
   *
   * \return      Maximum estimated number of elements which were produced but not yet consumed since the queue was
   *              created.
   *
   * \error       osabstraction::OsabErrc::kProtocolError      Memory corruption: invalid index values read
   *                                                           from underlying memory.
   *
   * \context     ANY
   *
   * \pre         -
   *
   * \reentrant   FALSE
   * \synchronous TRUE
   * \threadsafe  FALSE
   * \trace       CREQ-Osab-MemorySpScQueueGetMaxFillLevel
   */
  /*!
   * \internal
   *  - Updates the cached indexes with the values in the underlying memory.
   *  - If the indexes are valid:
   *    - Compute and return the number of occupied slots and update max_fill_level_.
   *    - Returns max_fill_level_.
   *  - Otherwise return kProtocolError.
   * \endinternal
   */
  auto GetMaxFillLevel() noexcept -> ::amsr::core::Result<QueueSlots> {
    ::amsr::core::Result<QueueSlots> result{GetFillLevel()};
    if (result) {
      result.EmplaceValue(max_fill_level_);
    }
    return result;
  };

  /*!
   * \brief       Tries to insert a new element at the back of the queue.
   *
   * \details     The element is copied into the buffer only if there is at least one slot free.
   *              Cached indexes and max_fill_level_ are updated according to the index values in the underlying memory.
   *
   * \param[in]   element  Reference to the element which should be inserted.
   *
   * \return      true if the element was successfully inserted, false otherwise.
   *
   * \error       osabstraction::OsabErrc::kProtocolError      Memory corruption: invalid index values read
   *                                                           from underlying memory.
   *
   * \context     ANY
   *
   * \pre         -
   *
   * \reentrant   FALSE
   * \synchronous TRUE
   * \threadsafe  FALSE
   * \trace       CREQ-Osab-MemorySpScQueueProduce
   */
  /*!
   * \internal
   *  - Retrieves the fill level of the queue and if it succeeds:
   *    - If the queue is not full:
   *      - Copies the input parameter in the buffer space pointed by head.
   *      - Advance cached head index by one element and stores it in the underlying memory.
   *      - Update max_fill_level_.
   *      - Returns true in the result.
   *    - Otherwise return false in the result.
   *  -Otherwise return kProtocolError.
   * \endinternal
   */
  // VECTOR NC AutosarC++17_10-A8.4.4: MD_OSA_A8.4.4_OutputParameterInsteadOfReturn
  auto Produce(T const& element) noexcept -> ::amsr::core::Result<bool> {
    ::amsr::core::Result<bool> result{osabstraction::OsabErrc::kProtocolError};
    ::amsr::core::Result<QueueSlots> fill_level_result{GetFillLevel()};
    if (fill_level_result) {
      QueueSlots const capacity{
          ::amsr::core::get<ProducerInternalData<T>>(endpoint_specific_data_).producer_data_buffer_view_.size()};
      if (fill_level_result.Value().amount < capacity.amount) {
        // VECTOR NC AutosarC++17_10-M0.3.1: MD_OSA_M0.3.1_SPSCQueueCastMayTruncateValue
        // VECTOR NC AutosarC++17_10-A4.7.1: MD_OSA_A4.7.1_SPSCQueueCastMayTruncateValue
        std::uint32_t const current_head_index_inside_buffer{
            static_cast<std::uint32_t>(current_head_index_ % static_cast<std::uint32_t>(capacity.amount))};
        ::amsr::core::get<ProducerInternalData<T>>(endpoint_specific_data_)           // VCA_OSA_EXT_CALL_PASS_PTR_RW
            .producer_data_buffer_view_[current_head_index_inside_buffer] = element;  // VCA_OSA_EXT_CALL_OBJ_PASS_VALUE
        AdvanceIndexByOne(current_head_index_);
        StoreHeadIndex();
        // Pessimistic update of max_fill_level_, because a peer may have consumed an element before head index has been
        // written to the underlying memory.
        QueueSlots const estimated_fill_level{fill_level_result.Value().amount + 1};
        UpdateMaxFillLevel(estimated_fill_level);
        result.EmplaceValue(true);
      } else {
        result.EmplaceValue(false);
      }
    }
    return result;
  }

  /*!
   * \brief       Tries to remove an element at the front of the queue.
   *
   * \details     The element is copied into the parameter only if there is at least one element in the queue.
   *              Cached indexes and max_fill_level_ are updated according to the index values in the underlying memory.
   *
   * \param[out]  element  Location where the consumed element should be copied to.
   *
   * \return      true if the element was successfully removed, false otherwise.
   *
   * \error       osabstraction::OsabErrc::kProtocolError      Memory corruption: invalid index values read
   *                                                           from underlying memory.
   *
   * \context     ANY
   *
   * \pre         -
   *
   * \reentrant   FALSE
   * \synchronous TRUE
   * \threadsafe  FALSE
   * \trace       CREQ-Osab-MemorySpScQueueConsume
   */
  /*!
   * \internal
   *  - Peeks one element from the queue and if succeeds:
   *    - Copies it in the output parameter.
   *    - Discards it.
   *  - Otherwise if the updated indexes are valid return false in the result.
   *  - Otherwise return kProtocolError.
   * \endinternal
   */
  auto Consume(T& element) noexcept -> ::amsr::core::Result<bool> {
    return Peek(element).Inspect([this](bool peeked) {
      if (peeked) {
        DiscardInternal();
      }
    });
  }

  /*!
   * \brief       Tries to peek an element at the front of the queue.
   *
   * \details     The element is copied into the parameter only if there is at least one element in the queue.
   *              The element is not removed from the queue.
   *              Cached indexes and max_fill_level_ are updated according to the index values in the underlying memory.
   *
   * \param[out]  element  Location where the peeked element should be copied to.
   *
   * \return      true if the element was successfully peeked, false otherwise.
   *
   * \error       osabstraction::OsabErrc::kProtocolError      Memory corruption: invalid index values read
   *                                                           from underlying memory.
   *
   * \context     ANY
   *
   * \pre         -
   *
   * \reentrant   FALSE
   * \synchronous TRUE
   * \threadsafe  FALSE
   * \trace       CREQ-Osab-MemorySpScQueuePeek
   */
  /*!
   * \internal
   *  - Retrieves the fill level of the queue and if it succeeds:
   *    - Update max_fill_level_.
   *    - If there is at least one element in the queue:
   *      - Copies it to the output parameter.
   *      - Returns true in the result.
   *    - Otherwise return false in the result.
   *  -Otherwise return kProtocolError.
   * \endinternal
   */
  // VECTOR NC AutosarC++17_10-A8.4.4: MD_OSA_A8.4.4_OutputParameterInsteadOfReturn
  auto Peek(T& element) noexcept -> ::amsr::core::Result<bool> {
    ::amsr::core::Result<bool> result{osabstraction::OsabErrc::kProtocolError};
    ::amsr::core::Result<QueueSlots> const fill_level_result{GetFillLevel()};
    if (fill_level_result) {
      if (fill_level_result.Value().amount > 0U) {
        // The size of the buffer view cannot be zero because otherwise the call to GetFillIndex above would have failed
        // when validating the indexes as there are no valid indexes for a zero-sized buffer.
        // VECTOR Next Construct AutosarC++17_10-A5.5.1: MD_OSA_A5.5.1_DivisorNotZero
        // VECTOR Next Construct AutosarC++17_10-M0.3.1: MD_OSA_M0.3.1_DivisorNotZero
        std::uint32_t const current_tail_index_inside_buffer{
            current_tail_index_ %
            static_cast<std::uint32_t>(
                ::amsr::core::get<ConsumerInternalData<T>>(endpoint_specific_data_).consumer_data_buffer_view_.size())};
        element = ::amsr::core::get<ConsumerInternalData<T>>(endpoint_specific_data_)  // VCA_OSA_EXT_CALL_PASS_PTR_RW
                      .consumer_data_buffer_view_[current_tail_index_inside_buffer];  // VCA_OSA_EXT_CALL_OBJ_PASS_VALUE
        result.EmplaceValue(true);
      } else {
        result.EmplaceValue(false);
      }
    }
    return result;
  }

  /*!
   * \brief       Tries to discard an element at the front of the queue.
   *
   * \details     The element is removed from the queue only if there is at least one element in the queue.
   *              Cached indexes and max_fill_level_ are updated according to the index values in the underlying memory.
   *
   * \return      true if the element was successfully removed, false otherwise.
   *
   * \error       osabstraction::OsabErrc::kProtocolError      Memory corruption: invalid index values read
   *                                                           from underlying memory.
   *
   * \context     ANY
   *
   * \pre         -
   *
   * \reentrant   FALSE
   * \synchronous TRUE
   * \threadsafe  FALSE
   * \trace       CREQ-Osab-MemorySpScQueueDiscard
   */
  /*!
   * \internal
   *  - Retrieves the fill level of the queue and if it succeeds:
   *    - Updates max_fill_level_.
   *    - If there is at least one element in the queue:
   *      - Discards it.
   *      - Returns true in the result.
   *    - Otherwise return false in the result.
   *  -Otherwise return kProtocolError.
   * \endinternal
   */
  auto Discard() noexcept -> ::amsr::core::Result<bool> {
    ::amsr::core::Result<bool> result{osabstraction::OsabErrc::kProtocolError};
    ::amsr::core::Result<QueueSlots> const fill_level_result{GetFillLevel()};
    if (fill_level_result) {
      UpdateMaxFillLevel(fill_level_result.Value());
      if (fill_level_result.Value().amount > 0U) {
        DiscardInternal();
        result.EmplaceValue(true);
      } else {
        result.EmplaceValue(false);
      }
    }
    return result;
  }

 private:
  /*!
   * \brief       Updates the cached indexes with the values in the underlying memory.
   *
   * \context     ANY
   *
   * \pre         -
   *
   * \reentrant   FALSE
   * \synchronous TRUE
   * \threadsafe  FALSE
   */
  /*!
   * \internal
   *  - Loads and acquire atomic variables in the underlying memory.
   *  - Updates cached indexes with loaded values.
   * \endinternal
   */
  void UpdateCachedIndexes() noexcept {
    if (::amsr::core::holds_alternative<ConsumerInternalData<T>>(endpoint_specific_data_)) {
      current_head_index_ = ::amsr::core::get<ConsumerInternalData<T>>(endpoint_specific_data_)
                                .consumer_head_index_ptr_->value.load(std::memory_order_acquire);
    } else {
      current_tail_index_ = ::amsr::core::get<ProducerInternalData<T>>(endpoint_specific_data_)
                                .producer_tail_index_ptr_->value.load(std::memory_order_acquire);
    }
  }

  /*!
   * \brief           Stores the locally cached head index to the atomic head index of the ring buffer in the shared
   *                  memory.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  /*!
   * \internal
   *  - Store the locally cached head index to the atomic head index of the ring buffer in the underlying memory and
   *    perform release operation on it.
   * \endinternal
   */
  void StoreHeadIndex() noexcept {
    ::amsr::core::get<ProducerInternalData<T>>(endpoint_specific_data_)  // VCA_OSA_EXT_CALL_PASS_PTR_RW
        // VCA_OSA_STL_CALL_OBJ_PASS_VALUE
        .producer_head_index_ptr_->value.store(current_head_index_, std::memory_order_release);
  }

  /*!
   * \brief           Stores the locally cached tail index to the atomic tail index of the ring buffer in the shared
   *                  memory.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  /*!
   * \internal
   *  - Store the locally cached tail index to the atomic tail index of the ring buffer in the underlying memory and
   *    perform release operation on it.
   * \endinternal
   */
  void StoreTailIndex() noexcept {
    ::amsr::core::get<ConsumerInternalData<T>>(endpoint_specific_data_)  // VCA_OSA_EXT_CALL_PASS_PTR_RW
        // VCA_OSA_STL_CALL_OBJ_PASS_VALUE
        .consumer_tail_index_ptr_->value.store(current_tail_index_, std::memory_order_release);
  }

  /*!
   * \brief       Checks if the cached indexes are inside the allowed range according to the algorithm.
   *
   * \details     Algorithm behavior:
   *              - The cached indexes are only advanced modulo 2 times the buffer size.
   *              - The cached indexes are only updated if there is no buffer overflow/underflow.
   *
   *\error        kProtocolError invalid indexes detected.
   *
   * \context     ANY
   *
   * \pre         -
   *
   * \reentrant   FALSE
   * \synchronous TRUE
   * \threadsafe  FALSE
   *
   * \spec
   *   requires true;
   * \endspec
   */
  /*!
   * \internal
   *  - If the cached head index is greater or equal than two times the buffer capacity returns kProtocolError
   *  - If the cached tail index is greater or equal than two times the buffer capacity returns kProtocolError
   *  - If the cached tail and head index lead to a number of occupied slots greater than the buffer capacity
   *    returns kProtocolError.
   *  - Return empty result.
   * \endinternal
   */
  auto ValidateCacheIndexes() const noexcept -> ::amsr::core::Result<void> {
    ::amsr::core::Result<void> result{osabstraction::OsabErrc::kProtocolError};
    size_t const capacity{GetCapacity().amount};
    // data_buffer_.size() limited to 2147483647 during construction
    if ((current_head_index_ < static_cast<std::uint32_t>(2U * capacity)) &&
        (current_tail_index_ < static_cast<std::uint32_t>(2U * capacity))) {
      if (current_tail_index_ > current_head_index_) {
        if (static_cast<std::uint32_t>(current_tail_index_ - current_head_index_) >= capacity) {
          result.EmplaceValue();
        }
      } else if (static_cast<std::uint32_t>(current_head_index_ - current_tail_index_) <= capacity) {
        result.EmplaceValue();
      } else {
        // Intentionally left empty.
      }
    }
    return result;
  }

  /*!
   * \brief           Advances an index (head or tail index) by a certain number of elements modulo two times the buffer
                      size.
   *
   * \details         This function works with the cached index values.
   *
   * \param[in]       index_to_advance  Reference to the index that shall be advanced.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   *
   * \spec
   *   requires true;
   * \endspec
   */
  /*!
   * \internal
   *  - Advance the passed index by one element.
   *  - If the passed index now exceeds two times the buffer size
   *    - A wrap-around of the index occurred. Subtract two times the data buffer size from the index.
   * \endinternal
   */
  void AdvanceIndexByOne(std::uint32_t& index_to_advance) const noexcept {
    size_t const capacity{GetCapacity().amount};
    index_to_advance += 1U;
    if (index_to_advance >= (2U * capacity)) {
      // Constructor guarantees that capacity is less or equal then UINT32_MAX/2.
      index_to_advance -= static_cast<std::uint32_t>(2U * capacity);
    }
  }

  /*!
   * \brief       Discards an element at the front of the queue.
   *
   * \details     Expects valid cached tail index and at least one element in the queue to be discarded.
   *
   * \context     ANY
   *
   * \pre         -
   *
   * \reentrant   FALSE
   * \synchronous TRUE
   * \threadsafe  FALSE
   */
  /*!
   * \internal
   *  - Advance the passed index by 1 element.
   *  - Store the updated tail index.
   * \endinternal
   */
  void DiscardInternal() noexcept {
    AdvanceIndexByOne(current_tail_index_);
    StoreTailIndex();
  };

  /*!
   * \brief       Returns the number of occupied slots in the managed queue.
   *
   * \details     Works on cached indexes, which are expected to be valid.
   *
   * \return      Number of elements which are produced but not yet consumed.
   *
   * \context     ANY
   *
   * \pre         -
   *
   * \reentrant   FALSE
   * \synchronous TRUE
   * \threadsafe  FALSE
   *
   * \spec
   *   requires true;
   * \endspec
   */
  /*!
   * \internal
   *  - If head is greater or equal tail return the difference between head and tail.
   *  - Otherwise return the 2 times the buffer size plus head minus tail, because head overflowed and tail not.
   * \endinternal
   */
  auto GetOccupiedSlots() const noexcept -> QueueSlots {
    QueueSlots occupied_slots{0};
    size_t const capacity{GetCapacity().amount};
    if (current_head_index_ >= current_tail_index_) {
      occupied_slots.amount = static_cast<std::uint32_t>(current_head_index_ - current_tail_index_);
    } else {
      occupied_slots.amount = static_cast<std::uint32_t>(((2U * capacity) - current_tail_index_) + current_head_index_);
    }
    return occupied_slots;
  }

  /*!
   * \brief       Updates max_fill_level given the snapshot of the occupied slots.
   *
   * \param[in]   occupied_slots   amount of occupied slots.
   *
   * \context     ANY
   *
   * \pre         -
   *
   * \reentrant   FALSE
   * \synchronous TRUE
   * \threadsafe  FALSE
   *
   * \spec
   *   requires true;
   * \endspec
   */
  /*!
   * \internal
   *  - Updates the max_fill_level_ if needed.
   * \endinternal
   */
  void UpdateMaxFillLevel(QueueSlots const occupied_slots) noexcept {
    max_fill_level_.amount = std::max(max_fill_level_.amount, occupied_slots.amount);
  }

  /*!
   * \brief   Estimated maximum number of produced, not yet consumed T elements in the queue, since queue creation.
   * \details This value is only a pessimistic estimation, that does not reflect every operation on the physical buffer.
   */
  QueueSlots max_fill_level_{QueueSlots{0}};

  /*!
   * \brief   Locally cached value of the queue head index.
   */
  std::uint32_t current_head_index_{0};

  /*!
   * \brief   Locally cached value of the ring buffer tail index.
   */
  std::uint32_t current_tail_index_{0};

  /*!
   * \brief   Storage for all endpoint specific data structures.
   */
  ::amsr::core::Variant<::amsr::core::monostate, ProducerInternalData<T>, ConsumerInternalData<T>>
      endpoint_specific_data_{};
};

}  // namespace internal
}  // namespace container
}  // namespace memory
}  // namespace amsr

#endif  // LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_MEMORY_CONTAINER_INTERNAL_SPSC_QUEUE_H_
