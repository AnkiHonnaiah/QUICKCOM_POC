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
 *        \brief  Implements a queue of file descriptors.
 *
 *      \details  -
 *
 *********************************************************************************************************************/
#ifndef LIB_COMMON_LIBOSABSTRACTION_LINUX_INCLUDE_AMSR_IPC_INTERNAL_FILE_DESCRIPTOR_QUEUE_H_
#define LIB_COMMON_LIBOSABSTRACTION_LINUX_INCLUDE_AMSR_IPC_INTERNAL_FILE_DESCRIPTOR_QUEUE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstddef>
#include <cstdint>
#include <utility>
#include "amsr/core/optional.h"
#include "ara/core/vector.h"
#include "osabstraction/io/file_descriptor.h"

namespace amsr {
namespace ipc {
namespace internal {

/*!
 * \brief   Queue of file descriptors.
 */
class FileDescriptorQueue {
 public:
  /*!
   * \brief           Add a valid file descriptor to the back of the queue.
   *
   * \param[in]       fd   File descriptor to add.
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
   *  - If not all added file descriptors should be discarded immediately
   *    - If the number of added file descriptors that should be discarded immediately is 0
   *      - Add the file descriptor to the queue.
   *    - Otherwise decrement the number of added file descriptors that should be discarded immediately.
   * \endinternal
   */
  void PushFd(osabstraction::io::FileDescriptor fd) noexcept {
    if (!discard_all_fds_) {
      if (num_fds_to_discard_ == 0) {
        fd_queue_.push_back(std::move(fd));
      } else {
        --num_fds_to_discard_;
      }
    }
  }

  /*!
   * \brief           Add an invalid file descriptor to the back of the queue.
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
   *  - Create an invalid file descriptor and pish it.
   * \endinternal
   */
  void PushInvalidFd() noexcept { PushFd(osabstraction::io::FileDescriptor{}); }

  /*!
   * \brief           Removes the file descriptor at the front of the queue.
   *
   * \return          File descriptor at the front of the queue or an empty optional if the queue is empty.
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
   *  - If the file descriptor queue is empty
   *    - Return an empty optional.
   *  - Otherwise
   *    - Remove the next file descriptor from the queue and return it.
   * \endinternal
   */
  auto PopFd() noexcept -> ::amsr::core::Optional<osabstraction::io::FileDescriptor> {
    ::amsr::core::Optional<osabstraction::io::FileDescriptor> result{};

    if (!fd_queue_.empty()) {
      ara::core::Vector<osabstraction::io::FileDescriptor>::iterator const it{fd_queue_.begin()};
      result.emplace(std::move(*it));
      static_cast<void>(fd_queue_.erase(it));
    }

    return result;
  }

  /*!
   * \brief           Discards the next file descriptor in the queue.
   *
   * \details         The file descriptor at the front of the queue is discarded if the queue is not empty, otherwise
   *                  the next added file descriptor will be discarded immediately.
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
   *  - If the file descriptor queue is empty
   *    - Increment the number of added file descriptors that should be discarded immediately.
   *  - Otherwise
   *    - Remove the next file descriptor from the queue.
   * \endinternal
   */
  void DiscardNextFd() noexcept {
    if (!fd_queue_.empty()) {
      ara::core::Vector<osabstraction::io::FileDescriptor>::iterator const it{fd_queue_.begin()};
      static_cast<void>(fd_queue_.erase(it));
    } else {
      ++num_fds_to_discard_;
    }
  }

  /*!
   * \brief           Discards all file descriptors that are currently in the queue and that will be added in the
   *                  future.
   *
   * \details         File descriptors will not be discarded anymore after a call to Reset().
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
   *  - Clear the file descriptor queue and set the flag that all added file descriptors should be discarded
   *    immediately.
   * \endinternal
   */
  void DiscardAllFds() noexcept {
    fd_queue_.clear();
    discard_all_fds_ = true;
  }

  /*!
   * \brief           Clear the file descriptor queue.
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
   *  - Clear the file descriptor queue.
   *  - Set the number of file descriptors that should be discarded when added to 0.
   *  - Clear the flag that all added file descriptors should be discarded immediately.
   * \endinternal
   */
  void Reset() noexcept {
    fd_queue_.clear();
    num_fds_to_discard_ = 0;
    discard_all_fds_ = false;
  }

 private:
  /*!
   * \brief  Internal queue of file descriptors.
   */
  ara::core::Vector<osabstraction::io::FileDescriptor> fd_queue_{};

  /*!
   * \brief  Number of file descriptors that shall be discarded when they are added with PushFd().
   */
  std::size_t num_fds_to_discard_{0};

  /*!
   * \brief  true if all file descriptors that are added should be discarded immediately, false otherwise.
   */
  bool discard_all_fds_{false};
};

}  // namespace internal
}  // namespace ipc
}  // namespace amsr

#endif  // LIB_COMMON_LIBOSABSTRACTION_LINUX_INCLUDE_AMSR_IPC_INTERNAL_FILE_DESCRIPTOR_QUEUE_H_
