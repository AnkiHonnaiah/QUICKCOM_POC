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
/*!        \file  buffer_provider.h
 *        \brief  BufferProvider class that return a pointer to a free buffer of type T.
 *         \unit  VaCommonLib::MemoryManagement::BufferProvider
 *
 *      \details  Currently, this class supports only one initial allocation of memory.
 *                All memory is allocated immediately.
 *
 *********************************************************************************************************************/

#ifndef LIB_VAC_INCLUDE_VAC_MEMORY_BUFFER_PROVIDER_H_
#define LIB_VAC_INCLUDE_VAC_MEMORY_BUFFER_PROVIDER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <algorithm>
#include <functional>
#include <map>
#include <memory>
#include <mutex>
#include <utility>
#include "ara/core/abort.h"
#include "vac/container/static_map.h"
#include "vac/testing/test_adapter.h"

namespace vac {
namespace memory {
/*!
 * \brief  BufferProvider Class to manage buffers of objects of a specific type.
 * \tparam T The type of the callable for raw pointer.
 * \trace  DSGN-VaCommonLib-BufferProvider
 */
template <class T>
class BufferProvider final {
  /*! Friend declaration for testing purposes */
  FRIEND_TEST(UT__BufferProvider, DeallocateTwice);
  /*! Friend declaration for testing purposes */
  FRIEND_TEST(UT__BufferProvider__CreateBufferProvider, ReserveOnlyOnce);

 public:
  /*!
   * \brief Typedef for raw pointers.
   */
  using pointer = T*;

  /*!
   * \brief Typedef for the size type used in this implementation.
   */
  using size_type = std::size_t;

  /*!
   * \brief Constructor.
   * \spec
   *  requires true;
   * \endspec
   */
  BufferProvider() noexcept {}

  /*!
   * \brief Default copy constructor deleted.
   */
  BufferProvider(BufferProvider const&) = delete;
  /*!
   * \brief  Default copy assignment operator deleted.
   * \return
   */
  BufferProvider& operator=(BufferProvider const&) = delete;
  /*!
   * \brief Default move constructor deleted.
   */
  BufferProvider(BufferProvider&&) = delete;
  /*!
   * \brief  Default copy assignment operator deleted.
   * \return
   */
  BufferProvider operator=(BufferProvider&&) = delete;

  /*!
   * \brief Default destructor.
   * \spec
   *  requires true;
   * \endspec
   */
  ~BufferProvider() noexcept = default;  // VCA_VAC_STD_FUNC_FULFILL_CONTRACT

  /*!
   * \brief     Update the memory allocation.
   * \details   The current implementation only allows a single allocation.
   *            Subsequent calls to reserve() does nothing if less memory is reserved than already allocated. If more
   *            memory is reserved than allocated, the method will terminate.
   *            Total memory necessary is equal to number_buffer * number_elements. Which means that a different
   *            dimension can require the same amount of memory. A call to reserve() with this result would fail
   *            silently.
   *            This method terminates if there is insufficient memory.
   * \param     number_buffer Number of buffers.
   * \param     number_elements Number of elements inside each buffer.
   * \pre       Will trigger ara::core::Abort if reserve() is called after a successful allocation and
   *            the reserve() call requires more memory than already allocated.
   * \trace     DSGN-VaCommonLib-BufferProvider
   */
  void reserve(size_type number_buffer, size_type number_elements) noexcept {
    std::lock_guard<std::mutex> const lock{buffer_mutex_};
    if ((number_buffer * number_elements) > (reserved_number_buffer_ * reserved_number_elements_)) {
      // We need to allocate additional memory. Current implementation can only allocate once initially.
      if (buffer_storage_) {
        ara::core::Abort(
            "vac::memory::BufferProvider::reserve(size_type, size_type): Allocation is already initialized!");
      } else {
        // VECTOR Next Construct AutosarC++17_10-A18.1.1: MD_VAC_A18.1.1_cStyleArraysShouldNotBeUsed
        // VECTOR Next Construct VectorC++-V3.0.1: MD_VAC_V3-0-1_cStyleArraysShouldNotBeUsed
        buffer_storage_ =                                            // VCA_VAC_STD_FUNC_FULFILL_CONTRACT
            std::make_unique<T[]>(number_buffer * number_elements);  // VCA_VAC_STD_FUNC_FULFILL_CONTRACT
        reserved_number_elements_ = number_elements;
        reserved_number_buffer_ = number_buffer;
        free_buffer_map_.reserve(number_buffer);
        // Associate each available buffer with a boolean.
        for (size_type i{0}; i < reserved_number_buffer_; ++i) {
          // VECTOR NC AutosarC++17_10-M5.0.16: MD_VAC_M5.0.16_pointerArithmetic
          // VECTOR NL AutosarC++17_10-M5.0.15: MD_VAC_M5.0.15_pointerArithmeticAllocatorIterator
          pointer next{buffer_storage_.get() + (i * reserved_number_elements_)};
          static_cast<void>(free_buffer_map_.emplace(next, true));
        }
      }
    }
  }

  /*!
   * \brief     Return a pointer to a free Buffer big enough for number_elements. Otherwise return a null pointer.
   * \param     number_elements Number of elements needed inside the buffer.
   * \pre       -
   * \return    Raw pointer to a free buffer.
   * \spec
   *  requires true;
   * \endspec
   */
  pointer allocate(size_type number_elements) noexcept {
    pointer ret_value{nullptr};
    std::lock_guard<std::mutex> const lock{buffer_mutex_};
    if (number_elements <= reserved_number_elements_) {
      // VCA_VAC_STD_FUNC_FULFILL_CONTRACT
      typename FreeBufferMap::iterator it{std::find_if(free_buffer_map_.begin(), free_buffer_map_.end(),
                                                       [](PairRawPtrBool const pair) { return pair.second; })};
      if (it != free_buffer_map_.end()) {
        it->second = false;  // VCA_VAC_VALID_ASSIGNING_NOT_ACCESSING, VCA_VAC_VALID_PTR_ASSIGNING_NOT_DEREFERENCING
        ret_value = it->first;
        ++allocation_counter_;
      }
    }
    return ret_value;
  }

  /*!
   * \brief     Release a buffer.
   * \param[in] ptr Pointer to the buffer to be released.
   * \pre       The pointer can not be nullptr when deallocating, otherwise ara::core::Abort will be triggered.
   * \pre       The pointer must exist in free_buffer_map, otherwise ara::core::Abort will be triggered.
   */
  void deallocate(pointer ptr) noexcept {
    std::lock_guard<std::mutex> const lock{buffer_mutex_};
    if (ptr == nullptr) {
      ara::core::Abort("vac::memory::BufferProvider::deallocate(pointer): Attempting to deallocate a nullptr!");
    } else {
      typename FreeBufferMap::iterator it{free_buffer_map_.find(ptr)};
      if (it == free_buffer_map_.end()) {
        ara::core::Abort("vac::memory::BufferProvider::deallocate(pointer): Pointer does not exist!");
      } else {
        if (!it->second) {
          --allocation_counter_;
        }
        it->second = true;  // VCA_VAC_VALID_PTR_ASSIGNING_NOT_DEREFERENCING,
      }
    }
  }

  /*!
   * \brief  Check for whether BufferProvider has any allocated memory.
   * \return bool true if BufferProvider has allocated memory, false otherwise.
   */
  bool HasAllocations() const noexcept {
    std::lock_guard<std::mutex> const lock{buffer_mutex_};
    return allocation_counter_ != 0;
  }

 private:
  // VECTOR Next Construct AutosarC++17_10-A18.1.1: MD_VAC_A18.1.1_cStyleArraysShouldNotBeUsed
  // VECTOR Next Construct VectorC++-V3.0.1: MD_VAC_V3-0-1_cStyleArraysShouldNotBeUsed
  /*!
   * \brief Typedef for storage unique pointers.
   */
  using StorageUniquePtr = std::unique_ptr<T[]>;

  /*!
   * \brief Type definition for a pair of raw_pointer and boolean.
   */
  using PairRawPtrBool = std::pair<pointer, bool>;

  /*!
   * \brief Type definition for map associating a buffer with a boolean to inform if a buffer is already used.
   */
  using FreeBufferMap =
      vac::container::StaticMap<typename PairRawPtrBool::first_type, typename PairRawPtrBool::second_type>;

  /*!
   * \brief The amount of elements reserved for one buffer.
   */
  size_type reserved_number_elements_{0};

  /*!
   * \brief Number of buffers currently reserved.
   */
  size_type reserved_number_buffer_{0};

  /*!
   * \brief Unique pointer to the allocated memory.
   */
  StorageUniquePtr buffer_storage_{nullptr};

  /*!
   * \brief FreeBufferMap.
   */
  FreeBufferMap free_buffer_map_{};

  /*!
   * \brief Mutex to synchronize access to the buffers.
   */
  mutable std::mutex buffer_mutex_{};

  /*! \brief Counter to track allocation of MemoryBuffers. */
  std::uint32_t allocation_counter_{0};
};

/*!
 * \brief  Implement smart buffer provider.
 * \tparam T The type of the callable.
 * \trace  DSGN-VaCommonLib-BufferProvider
 */
template <class T>
class SmartBufferProvider final {
 public:
  /*!
   * \brief Typedef for the size type used in this implementation.
   */
  using size_type = std::size_t;

  /*!
   * \brief Default constructor.
   * \spec
   *  requires true;
   * \endspec
   */
  SmartBufferProvider() noexcept = default;
  /*!
   * \brief Default copy constructor deleted.
   */
  SmartBufferProvider(SmartBufferProvider const&) = delete;
  /*!
   * \brief  Default copy assignment operator deleted.
   * \return
   */
  SmartBufferProvider& operator=(SmartBufferProvider const&) = delete;
  /*!
   * \brief Default move constructor deleted.
   */
  SmartBufferProvider(SmartBufferProvider&&) = delete;
  /*!
   * \brief  Default move assignment operator deleted.
   * \return
   */
  SmartBufferProvider& operator=(SmartBufferProvider&&) = delete;

  /*!
   * \brief Default destructor.
   * \spec
   *  requires true;
   * \endspec
   */
  ~SmartBufferProvider() noexcept = default;

  /*!
   * \brief Deleter for unique pointers to buffers.
   */
  class SmartBufferProviderDeleter final {
   public:
    /*!
     * \brief Constructor.
     * \spec
     *  requires true;
     * \endspec
     */
    SmartBufferProviderDeleter() noexcept : SmartBufferProviderDeleter(nullptr) {}

    /*!
     * \brief     Overloaded constructor.
     * \param[in] buffer_provider The buffer provider whose pointers may be.
     * \spec
     *  requires true;
     * \endspec
     */
    explicit SmartBufferProviderDeleter(BufferProvider<T>* buffer_provider) noexcept
        : buffer_provider_(buffer_provider) {}

    /*!
     * \brief Copy constructor.
     * \spec
     *  requires true;
     * \endspec
     */
    SmartBufferProviderDeleter(SmartBufferProviderDeleter const&) noexcept = default;

    /*!
     * \brief  Copy assignment.
     * \return A reference to the assigned-to object.
     * \spec
     *  requires true;
     * \endspec
     */
    SmartBufferProviderDeleter& operator=(SmartBufferProviderDeleter const&) & noexcept = default;

    /*!
     * \brief Move constructor.
     * \spec
     *  requires true;
     * \endspec
     */
    SmartBufferProviderDeleter(SmartBufferProviderDeleter&&) noexcept = default;

    /*!
     * \brief  Move assignment.
     * \return A reference to the assigned-to object.
     * \spec
     *  requires true;
     * \endspec
     */
    SmartBufferProviderDeleter& operator=(SmartBufferProviderDeleter&&) & noexcept = default;

    /*!
     * \brief Default destructor.
     * \spec
     *  requires true;
     * \endspec
     */
    ~SmartBufferProviderDeleter() noexcept = default;

    /*!
     * \brief     The actual deleter function.
     * \pre       -
     * \param[in] ptr The pointer whose memory shall be deallocated.
     * \spec
     *  requires true;
     * \endspec
     */
    void operator()(typename BufferProvider<T>::pointer ptr) noexcept {
      if (buffer_provider_ != nullptr) {
        buffer_provider_->deallocate(ptr);
      }
    }

   private:
    /*!
     * \brief Pointer to the buffer provider.
     */
    BufferProvider<T>* buffer_provider_;
  };

  /*!
   * \brief Type definition for a unique pointer.
   */
  using UniqueBufferPtr = std::unique_ptr<T, SmartBufferProviderDeleter>;

  /*!
   * \brief     Update the memory allocation.
   * \details   The current implementation only allows a single allocation. All further reservations will only limit the
   *            visible memory. All subsequent calls to reserve() where new_capacity is greater than the initial
   *            new_capacity will be rejected with a bad_alloc.
   *            This method terminates if there is insufficient memory.
   * \param     number_buffer Number of buffers.
   * \param     size_buffer Size of each buffers.
   * \pre       Allocation can not be initialized in advance, otherwise ara::core::Abort will be triggered.
   */
  void reserve(size_type number_buffer, size_type size_buffer) noexcept {
    buffer_provider_.reserve(number_buffer, size_buffer);
  }

  // VECTOR Next Construct AutosarC++17_10-A18.5.8: MD_VAC_A18.5.8_localObjectsShallBeAllocatedOnStack
  /*!
   * \brief     Return a pointer to a free Buffer big enough for number_elements. Otherwise return a null pointer.
   * \param     number_elements Number of elements needed inside the buffer.
   * \pre       -
   * \return    Raw pointer to a free buffer.
   * \spec
   *  requires true;
   * \endspec
   */
  UniqueBufferPtr allocate(size_type number_elements) noexcept {
    typename BufferProvider<T>::pointer const buffer{buffer_provider_.allocate(number_elements)};
    UniqueBufferPtr unique_ptr{buffer, SmartBufferProviderDeleter(&buffer_provider_)};
    return unique_ptr;
  }

  /*!
   * \brief  Return the buffer provider.
   * \pre    -
   * \return Reference to the buffer provider.
   * \spec
   *  requires true;
   * \endspec
   */
  BufferProvider<T>& GetBufferProvider() noexcept { return buffer_provider_; }

 private:
  /*!
   * \brief Buffer Provider.
   */
  BufferProvider<T> buffer_provider_;
};

}  // namespace memory
}  // namespace vac

/*!
 * \exclusivearea   vac::memory::BufferProvider::buffer_mutex_
 *                  Make sure the buffer is not exhausted.
 * \protects        vac::memory::BufferProvider::reserved_number_elements_
                    vac::memory::BufferProvider::reserved_number_buffer_
                    vac::memory::BufferProvider::buffer_storage_
                    vac::memory::BufferProvider::free_buffer_map_
 * \usedin          vac::memory::BufferProvider::allocate
 *                  vac::memory::BufferProvider::deallocate
 *                  vac::memory::BufferProvider::reserve
 * \exclude         When the ptr is a nullptr during calling deallocate(), ara::core::Abort will be triggered.
 * \length          SHORT
 *                  This exclusive area happens when access buffers through buffer provider.
 * \endexclusivearea
 */

#endif  // LIB_VAC_INCLUDE_VAC_MEMORY_BUFFER_PROVIDER_H_
