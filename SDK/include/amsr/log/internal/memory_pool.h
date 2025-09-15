/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2023 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/**     \file       memory_pool.h
 *      \brief
 *      \details
 *
 *********************************************************************************************************************/
#ifndef LIB_AMSR_LOG_INCLUDE_AMSR_LOG_INTERNAL_MEMORY_POOL_H_
#define LIB_AMSR_LOG_INCLUDE_AMSR_LOG_INTERNAL_MEMORY_POOL_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <deque>

#include "amsr/core/vector.h"
#include "amsr/log/internal/abort.h"
#include "amsr/log/internal/algorithm.h"
#include "amsr/log/internal/ref.h"
#include "amsr/log/internal/sync/primitive.h"
#include "amsr/log/internal/utils.h"
#include "ara/core/vector.h"
#include "vac/container/string_literals.h"
#include "vac/language/unique_function.h"

namespace amsr {
namespace log {
namespace internal {

/*! \brief Forward declaration for a memory pool. */
template <typename T, sync::Strategy kStrategy = sync::Strategy::kDefault>
class MemoryPool;

/*!
 * \brief   A deleter for a memory shard.
 *
 * \tparam  T           The pool element type.
 *                      Must comply to the requirements of T for amsr::log::internal::MemoryPool<T, kStrategy>.
 * \tparam  kStrategy   Switches between synchronization strategies.
 *
 * \unit    LogAndTrace::Logging::PoolShardDeleter
 */
template <typename T, sync::Strategy kStrategy = sync::Strategy::kDefault>
class PoolShardDeleter final {
  /*! \brief The pool to return it to. */
  MemoryPool<T, kStrategy>* pool_{nullptr};
  /*! \brief The index where the buffer is stored. */
  std::size_t index_{0};

 public:
  /*!
   * \brief Default constructor to enable default for the unique pointer.
   * \spec
   *   requires true;
   * \endspec
   */
  PoolShardDeleter() noexcept = default;

  /*!
   * \brief Populate the deleter
   *
   * \param pool The pool reference to delete
   * \param idx  The index of the element in the pool
   * \spec
   *   requires true;
   * \endspec
   */
  PoolShardDeleter(MemoryPool<T, kStrategy>& pool, std::size_t idx) noexcept : pool_{&pool}, index_{idx} {}

  /*!
   * \brief Returns the used buffer to the pool.
   * \spec
   *   requires true;
   * \endspec
   */
  void operator()(T const*) noexcept;
};

/*! \brief The boxed buffer with its deleter. */
template <typename T, sync::Strategy kStrategy = sync::Strategy::kDefault>
using PoolShard = Box<T, PoolShardDeleter<T, kStrategy>>;

/*!
 * \brief   Handles multiple buffers per logger element.
 *
 * \tparam  T           The type for the buffer.
 *                      Must implement the EmplaceConstructible named requirement.
 * \tparam  kStrategy   Switches between synchronization strategies.
 *
 * \unit    LogAndTrace::Logging::MemoryPool
 */
template <typename T, sync::Strategy kStrategy>
class MemoryPool final {
  /*! \brief The guard used to synchronize the pool access. */
  using Guard = sync::Primitive<kStrategy>;

 public:
  /*! \brief The type for buffer containers */
  using Buffers = std::deque<T>;
  /*! \brief The type for the usage buffer */
  using VectorBool = char;
  using Usages = ara::core::Vector<VectorBool>;

 private:
  /*! \brief The buffers. */
  Buffers buffers_;
  /*! \brief The marker vector. */
  Usages usages_;
  /*! \brief Mutex guarding the get/return operation. */
  Guard mutex_{};
  /*! \brief Function to initialize or reset the contained buffers. */
  vac::language::UniqueFunction<void(T&) const> init_;

 public:
  /*!
   * \brief Construct the handler with 'num_preallocated' elements inside.
   *
   * \param num_preallocated The number of elements that are preallocated.
   * \param init             The init function that is called before an element is handed out.
   * \spec
   *   requires true;
   * \endspec
   */
  MemoryPool(std::size_t num_preallocated, vac::language::UniqueFunction<void(T&) const>&& init) noexcept
      //  VCA_LOGAPI_CHECKED_CONTRACT
      : buffers_(num_preallocated), usages_(num_preallocated, false), init_{std::move(init)} {}

  /*! \brief Delete copy constructor. */
  MemoryPool(MemoryPool const&) = delete;
  /*! \brief Delete copy assignment operator. */
  auto operator=(MemoryPool const&) & noexcept -> MemoryPool& = delete;
  /*! \brief Delete move constructor. */
  MemoryPool(MemoryPool&&) = delete;
  /*! \brief Delete move assignment operator. */
  auto operator=(MemoryPool&&) & noexcept -> MemoryPool& = delete;

  /*!
   * \brief Destroys the Memory Pool object.
   * \details Aborts in case any memory object is still in use.
   * \spec
   *   requires true;
   * \endspec
   */
  ~MemoryPool() noexcept {
    using vac::container::operator""_sv;
    std::lock_guard<Guard> const guard{this->mutex_};
    internal::Assert(this->IsUnused(), "Memory pool is still in use during destruction!"_sv);
    // VCA_LOGAPI_CHECKED_CONTRACT
  }

  /*!
   * \brief Gets an unused Buffer.
   *
   * \return a new unused buffer.
   * \spec
   *   requires true;
   * \endspec
   */
  auto Get() noexcept -> PoolShard<T, kStrategy> {
    using vac::container::literals::operator""_sv;

    std::lock_guard<Guard> const guard{this->mutex_};

    T* buf{nullptr};
    std::size_t index{0};

    // Find an unused buffer
    if (Entry<Usages> const entry{Find(this->usages_, false)}) {
      // It is unused, so we mark it as used
      // VCA_LOGAPI_CHECKED_CONTRACT
      *entry = true;
      std::ptrdiff_t const i{entry.Index()};

      index = static_cast<std::size_t>(i);
      // VCA_LOGAPI_CHECKED_CONTRACT
      buf = &this->buffers_[index];
    } else {
      // grow buffer
      // VCA_LOGAPI_CHECKED_CONTRACT
      this->usages_.push_back(true);
      // VCA_LOGAPI_CHECKED_CONTRACT
      this->buffers_.emplace_back();
      // VCA_LOGAPI_CHECKED_CONTRACT
      buf = &this->buffers_.back();
      index = this->usages_.size() - 1;
    }
    // Reset the buffer
    // VCA_LOGAPI_VALID_OBJECT
    this->init_(*buf);

    PoolShardDeleter<T, kStrategy> deleter{*this, index};
    return PoolShard<T, kStrategy>{buf, std::move(deleter)};
  }

  /*!
   * \brief Returns the used buffer to the pool
   *
   * \param index The index to return
   * \spec
   *   requires true;
   * \endspec
   */
  void Return(std::size_t index) noexcept {
    std::lock_guard<Guard> const guard{this->mutex_};
    // VCA_LOGAPI_CHECKED_CONTRACT
    this->usages_[index] = false;
  }

  /*!
   * \brief Shows the number of elements allocated in this buffer
   *
   * \return The number of elements in the buffer
   * \spec
   *   requires true;
   * \endspec
   */
  auto GetSize() const noexcept -> std::size_t { return this->usages_.size(); }

  /*!
   * \brief Checks if the memory pool is unused.
   *
   * \return True in case no memory shard has been given out.
   * \spec
   *   requires true;
   * \endspec
   */
  auto IsUnused() const noexcept -> bool {
    return std::none_of(this->usages_.begin(), this->usages_.end(), [](bool b) { return b; });
  }
};

template <typename T, sync::Strategy kStrategy>
void PoolShardDeleter<T, kStrategy>::operator()(T const*) noexcept {
  if (this->pool_ != nullptr) {
    this->pool_->Return(this->index_);
  }
}

}  // namespace internal
}  // namespace log
}  // namespace amsr

/*!
 * \exclusivearea   ::amsr::log::internal::MemoryPool::mutex_
 *                  Prevents from concurrent state updates or changes
 * \protects        ::amsr::log::internal::MemoryPool::usages_
 *                  ::amsr::log::internal::MemoryPool::buffers_
 * \usedin          ::amsr::log::internal::MemoryPool::~MemoryPool
 *                  ::amsr::log::internal::MemoryPool::Get
 *                  ::amsr::log::internal::MemoryPool::Return
 * \exclude         All other functions of MemoryPool
 * \length          MEDIUM
 *                  Lock active during all relevant updates to the contained pool elements.
 * \endexclusivearea
 */

#endif  // LIB_AMSR_LOG_INCLUDE_AMSR_LOG_INTERNAL_MEMORY_POOL_H_
