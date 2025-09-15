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
/*!       \file   someip_daemon_client/internal/queue_handler.h
 *        \brief  The class managing the IPC queue.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CLIENT_INCLUDE_SOMEIP_DAEMON_CLIENT_INTERNAL_QUEUE_HANDLER_H_
#define LIB_SOMEIP_DAEMON_CLIENT_INCLUDE_SOMEIP_DAEMON_CLIENT_INTERNAL_QUEUE_HANDLER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <utility>
#include "amsr/pool_resource.h"
#include "amsr/unique_ptr.h"
#include "ara/core/memory_resource.h"
#include "ara/log/logstream.h"
#include "someip_daemon_client/internal/config_model.h"
#include "someip_daemon_client/internal/logging/ara_com_logger.h"
#include "someip_daemon_client/internal/someip_daemon_client_error_code.h"
#include "vac/memory/allocator/memory_buffer_ptr.h"

namespace amsr {
namespace someip_daemon_client {
namespace internal {

/*!
 * \brief The type of an entry in the queue.
 */
// VCA_SDCL_DESTRUCTOR_OF_UNIQUE_PTR, VCA_SDCL_DESTRUCTOR_NO_PRECONDITION
struct QueueEntry {
  /*!
   * \brief A pointer to a buffer that contains the IPC header of the message.
   */
  vac::memory::allocator::MemoryBufferPtr ipc_header{};

  /*!
   * \brief A pointer to a buffer that contains the payload of the message.
   */
  vac::memory::allocator::MemoryBufferPtr payload{};

  /*!
   * \brief A pointer to the next message in the queue.
   */
  amsr::UniquePtr<QueueEntry> next{nullptr};
};

/*!
 * \brief The class handling the queue.
 */
class QueueHandler final {
 public:
  /*!
   * \brief Remove default constructors.
   */
  QueueHandler(QueueHandler const&) = delete;
  auto operator=(QueueHandler const&) & -> QueueHandler& = delete;
  QueueHandler(QueueHandler&&) noexcept = delete;
  auto operator=(QueueHandler&&) & noexcept -> QueueHandler& = delete;

  /*!
   * \brief Default constructor of the QueueHandler class.
   * \param[in]   config The SomeIpDaemonClient configuration struct.
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady FALSE
   */
  // VECTOR NC AutosarC++17_10-A12.6.1: MD_SOMEIPDAEMONCLIENT_AutosarC++17_10-A12.6.1_Member_not_explicitly_initialized
  // VECTOR NC VectorC++-V12.1.1: MD_SOMEIPDAEMONCLIENT_VectorC++-V12.1.1_Member_not_explicitly_initialized
  explicit QueueHandler(SomeIpDaemonClientConfigModel const& config) noexcept { Init(config); }

  /*!
   * \brief Constructor of the QueueHandler class.
   * \details Constructor where the user can supply the memory resources to be used. Can be used for testing purposes to
   *          set the upstream memory resources.
   * \param[in]   config            The SomeIpDaemonClient configuration struct.
   * \param[in]   data_resource     The underlying memory resource used for allocating queue entries of type data.
   * \param[in]   command_resource  The underlying memory resource used for allocating queue entries of type command.
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady FALSE
   *
   */
  // VECTOR NC AutosarC++17_10-A12.6.1: MD_SOMEIPDAEMONCLIENT_AutosarC++17_10-A12.6.1_Member_not_explicitly_initialized
  // VECTOR NC VectorC++-V12.1.1: MD_SOMEIPDAEMONCLIENT_VectorC++-V12.1.1_Member_not_explicitly_initialized
  explicit QueueHandler(SomeIpDaemonClientConfigModel const& config, amsr::PoolResource& data_resource,
                        amsr::PoolResource& command_resource) noexcept
      // VCA_SDCL_CALLING_UNDEFINED_FUNCTION
      : data_pool_resource_{&data_resource}, command_pool_resource_{&command_resource} {
    Init(config);
  }

  /*!
   * \brief Destructor of the QueueHandler.
   * \steady FALSE
   */
  ~QueueHandler() noexcept {
    Clear();
    data_pool_resource_.SetSteadyMode(false);  // VCA_SDCL_CALLING_UNDEFINED_FUNCTION
  }                                            // VCA_SDCL_CALLING_UNDEFINED_FUNCTION

  /*!
   * \brief       Puts a data message at the end of the queue.
   * \param[in]   ipc_header The IPC header of the message.
   * \param[in]   payload    The payload of the message.
   * \return      Empty result in case of success, else an error code.
   * \error       SomeIpDaemonClientErrc::kRuntimeResourceOutOf The error is returned if the underlying memory resource
   *              is full.
   * \pre         ipc_header and payload are valid pointers to memory buffers.
   * \context     Any
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady TRUE
   */
  auto PushBackData(vac::memory::allocator::MemoryBufferPtr ipc_header,
                    vac::memory::allocator::MemoryBufferPtr payload) noexcept
      -> amsr::core::Result<void> {  // COV_SOMEIPDAEMON_CLIENT_QUEUE_HANDLER
    // VCA_SDCL_VALID_ARGUMENTS_IS_A_FUNCTION_PRECONDITION
    return PushBack(data_allocator_, std::move(ipc_header), std::move(payload));
  }

  /*!
   * \brief       Puts a command message at the end of the queue.
   * \param[in]   ipc_header The IPC header of the message.
   * \param[in]   payload    The payload of the message.
   * \return      Empty result in case of success, else an error code.
   * \error       SomeIpDaemonClientErrc::kRuntimeResourceOutOf The error is returned if the underlying memory resource
   *              is full.
   * \pre         ipc_header and payload are valid pointers to memory buffers.
   * \context     Any
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady FALSE
   */
  auto PushBackCommand(vac::memory::allocator::MemoryBufferPtr ipc_header,
                       vac::memory::allocator::MemoryBufferPtr payload) noexcept
      -> amsr::core::Result<void> {  // COV_SOMEIPDAEMON_CLIENT_QUEUE_HANDLER
    // VCA_SDCL_VALID_ARGUMENTS_IS_A_FUNCTION_PRECONDITION
    return PushBack(command_allocator_, std::move(ipc_header), std::move(payload));
  }

  /*!
   * \brief       Check if the queue is empty or not.
   * \return      Bool indicating if the queue is empty or not.
   * \pre         -
   * \context     Any
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady TRUE
   */
  auto IsEmpty() const noexcept -> bool {  // COV_SOMEIPDAEMON_CLIENT_QUEUE_HANDLER
    bool result{true};
    if (head_.get() != nullptr) {
      result = false;
    }
    return result;
  }

  /*!
   * \brief       Gets a read only reference to the first entry in the queue.
   * \return      Const reference to the first entry in the queue.
   * \pre         Calling this method when the queue is empty causes undefined behaviour.
   * \context     Any
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady TRUE
   */
  auto Front() const noexcept -> QueueEntry const& { return *head_; }  // COV_SOMEIPDAEMON_CLIENT_QUEUE_HANDLER

  /*!
   * \brief       Deletes the first entry in the queue. If the queue is empty, nothing happens.
   * \pre         -
   * \context     Any
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady TRUE
   */
  auto PopFront() noexcept -> void {  // COV_SOMEIPDAEMON_CLIENT_QUEUE_HANDLER
    if (head_ != nullptr) {
      head_ = std::move(head_->next);  // VCA_SDCL_POSSIBLY_INVALID_ARGUMENT
    }
  }

 private:
  /*!
   * \brief Delegating constructor of the QueueHandler class.
   * \param[in]   config The SomeIpDaemonClient configuration struct.
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady FALSE
   *
   * \internal
   * - If a max IPC queue size has been configured:
   *   - Try to reserve memory for the data_pool_resource.
   *   - If successful:
   *     - Set SteadyMode true on the data_pool_resource.
   *   - Else:
   *     - Log error and abort.
   * - Else:
   *   - No max queue has been configured, set SteadyMode to false for the data_pool_resource.
   * \endinternal
   */
  auto Init(SomeIpDaemonClientConfigModel const& config) noexcept -> void {
    if (config.ipc_max_queue_size != kDefaultSomeipdIpcMaxQueueSize) {
      amsr::core::Result<void> const result{
          // VCA_SDCL_CALLING_UNDEFINED_FUNCTION
          data_pool_resource_.Reserve(config.ipc_max_queue_size, sizeof(QueueEntry), alignof(QueueEntry))};
      if (result.HasValue()) {
        data_pool_resource_.SetSteadyMode(true);  // VCA_SDCL_CALLING_UNDEFINED_FUNCTION
      } else {
        logger_.LogFatalAndAbort(
            [](logging::AraComLogger::StringStream& s) {
              s << "Unable to reserve memory in the pool resource.";  // VCA_SDCL_STRING_STREAM_USAGE
                                                                      // VCA_SDCL_STRING_STREAM_USAGE
            },
            static_cast<char const*>(__func__), __LINE__);
      }
    } else {
      data_pool_resource_.SetSteadyMode(false);  // VCA_SDCL_CALLING_UNDEFINED_FUNCTION
    }
    data_allocator_ = ara::core::PolymorphicAllocator<QueueEntry>{&data_pool_resource_};
    command_allocator_ = ara::core::PolymorphicAllocator<QueueEntry>{&command_pool_resource_};
  }

  /*!
   * \brief       Recursively deletes all elements in the queue.
   * \pre         -
   * \context     Shutdown
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady FALSE
   */
  auto Clear() noexcept -> void {
    while (head_ != nullptr) {
      head_ = std::move(head_->next);  // VCA_SDCL_POSSIBLY_INVALID_ARGUMENT
    }
  }

  /*!
   * \brief       Puts a message at the end of the queue.
   * \param[in]   allocator  The allocator used to create the new QueueEntry object from.
   * \param[in]   ipc_header The header of the message.
   * \param[in]   payload    The payload of the message.
   * \return      Empty result in case of success, else an error code.
   * \error       SomeIpDaemonClientErrc::kRuntimeResourceOutOf The error is returned if the underlying memory resource
   *              is full.
   * \pre -
   * \context     Any
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady TRUE
   *
   * \internal
   * - Prepare result.
   * - If allocation for the new QueueEntry object succeeded:
   *   - Move the buffer pointers to the QueueEntry object.
   *   - If the queue is currently empty:
   *     - Set the head_ and tail pointers to point to the new entry.
   *   - Else:
   *     - Put the new message at the end of the queue.
   * - Return the result.
   * \endinternal
   */
  auto PushBack(ara::core::PolymorphicAllocator<QueueEntry> const& allocator,
                vac::memory::allocator::MemoryBufferPtr ipc_header,
                vac::memory::allocator::MemoryBufferPtr payload) noexcept -> amsr::core::Result<void> {
    amsr::core::Result<void> result{amsr::core::Result<void>::FromError(SomeIpDaemonClientErrc::kRuntimeResourceOutOf)};
    amsr::core::Result<amsr::UniquePtr<QueueEntry>> alloc_res{AllocateUnique<QueueEntry>(allocator)};
    if (alloc_res.HasValue()) {
      result.EmplaceValue();
      if (head_ == nullptr) {
        head_ = std::move(alloc_res.Value());  // VCA_SDCL_VALID_QUEUE_ENTRY_OBJECT
        tail_ = head_.get();
      } else {
        tail_->next = std::move(alloc_res.Value());  // VCA_SDCL_VALID_QUEUE_ENTRY_OBJECT
        tail_ = tail_->next.get();
      }
      // VECTOR NL AutosarC++17_10-M0.3.1: MD_SOMEIPDAEMONCLIENT_M0.3.1_Previously_checked_pointer
      tail_->ipc_header = std::move(ipc_header);  // VCA_SDCL_VALID_MEMORY_BUFFER_PTR
      // VECTOR NL AutosarC++17_10-M0.3.1: MD_SOMEIPDAEMONCLIENT_M0.3.1_Previously_checked_pointer
      tail_->payload = std::move(payload);  // VCA_SDCL_VALID_MEMORY_BUFFER_PTR
    }
    return result;
  }

  /*!
   * \brief QueueHandler logger.
   */
  logging::AraComLogger logger_{logging::kSomeIpLoggerContextId, logging::kSomeIpLoggerContextDescription,
                                amsr::core::StringView{"QueueHandler"}};

  /*!
   * \brief Pointer to the first element of the queue.
   */
  amsr::UniquePtr<QueueEntry> head_{nullptr};

  /*!
   * \brief Raw pointer to the last element of the queue.
   */
  QueueEntry* tail_{nullptr};

  /*!
   * \brief The memory resource used for data messages.
   */
  amsr::PoolResource data_pool_resource_{};  // VCA_SDCL_CALLING_UNDEFINED_FUNCTION

  /*!
   * \brief The memory resource used for command messages.
   */
  amsr::PoolResource command_pool_resource_{};  // VCA_SDCL_CALLING_UNDEFINED_FUNCTION

  /*!
   * \brief Allocator for queue elements that holds messages of type data.
   */
  ara::core::PolymorphicAllocator<QueueEntry> data_allocator_;

  /*!
   * \brief Allocator for queue elements that holds messages of type command.
   */
  ara::core::PolymorphicAllocator<QueueEntry> command_allocator_;
};

}  // namespace internal
}  // namespace someip_daemon_client
}  // namespace amsr

// COV_JUSTIFICATION_BEGIN
//
// \ID COV_SOMEIPDAEMON_CLIENT_QUEUE_HANDLER
//   \ACCEPT XX
//   \REASON This class it yet not integrated and used within someipdaemonclient. Thus no calls to this class will
//           happen in component tests. This justification should be removed when the QueueHandler class has been
//           integrated.
//
// COV_JUSTIFICATION_END

#endif  // LIB_SOMEIP_DAEMON_CLIENT_INCLUDE_SOMEIP_DAEMON_CLIENT_INTERNAL_QUEUE_HANDLER_H_
