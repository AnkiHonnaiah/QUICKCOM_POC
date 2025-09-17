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
 *        \brief  Memory resource that can be used to allocate memory for std::shared_ptr.
 *         \unit  VaCommonLib::MemoryManagement::SharedPtrResource
 *
 *********************************************************************************************************************/

#ifndef LIB_VAC_INCLUDE_AMSR_SHARED_PTR_RESOURCE_H_
#define LIB_VAC_INCLUDE_AMSR_SHARED_PTR_RESOURCE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cassert>
#include <memory>
#include <mutex>
#include <new>
#include "amsr/core/abort.h"
#include "amsr/core/array.h"
#include "amsr/shared_ptr.h"
#include "ara/core/memory_resource.h"

namespace amsr {
/*!
 * \brief           A memory resource for allocating memory for the object and control block of std::shared_ptr
 * \details         It pre-allocates memory for N shared pointers during construction and aborts if allocation fails.
 * \vprivate        Product private internal API
 * \tparam          T
 *                  The object type of the shared pointer.
 * \tparam          N
 *                  The number of shared pointers to allocate memory for.
 * \tparam          StdSp
 *                  If the resource is meant for a std::shared_ptr. If false, then amsr::SharedPtr is assumed.
 * \trace           CREQ-VaCommonLib-SharedPtrResource
 */
template <typename T, std::size_t N = 1, bool StdSp = true>
class SharedPtrResource final : public ara::core::MemoryResource {
  /*!
   * \brief  Helper function to find the std::shared_ptr type specified by the StdSp template parameter.
   * \tparam B Boolean to determine if the resource is meant for std::shared_ptr (true) or amsr::SharedPtr (false)
   * \return No return value, only the type std::shared_ptr<T>
   */
  template <bool B = StdSp, std::enable_if_t<B, bool> = true>
  static auto GetUsedPtrType() noexcept -> std::shared_ptr<T>;

  /*!
   * \brief  Helper function to find the amsr::SharedPtr type specified by the StdSp template parameter.
   * \tparam B Boolean to determine if the resource is meant for std::shared_ptr (true) or amsr::SharedPtr (false)
   * \return No return value, only the type amsr::SharedPtr<T>
   */
  template <bool B = StdSp, std::enable_if_t<!B, bool> = true>
  static auto GetUsedPtrType() noexcept -> amsr::SharedPtr<T>;

 public:
  /*!
   * \brief           The shared pointer type that the resource is reserving memory for
   * \details         If StdSp is true then this is a std::shared_ptr, and if false amsr::SharedPtr
   * \vprivate        product
   */
  using SharedPtrType = decltype(GetUsedPtrType<StdSp>());

  /*!
   * \brief           Constructs a SharedPtrResource
   * \details         Arguments are only needed, if T has no default constructor. Only participates in in resolution
   *                  overload if the element type T can be constructed from \p args.
   * \tparam          Args
   *                  Types of the constructor arguments of T.
   * \param[in]       args
   *                  Dummy constructor arguments for T. The instances of T are only used to determine the necessary
   *                  memory and they are destructed at the end of this constructor.
   * \context         ANY
   * \pre             -
   * \steady          FALSE
   */
  template <typename... Args, std::enable_if_t<std::is_constructible<T, Args...>::value, bool> = true>
  explicit SharedPtrResource(Args&&... args) noexcept
      : SharedPtrResource{ara::core::GetDefaultResource(), std::forward<Args>(args)...} {}

  /*!
   * \brief           Constructs a SharedPtrResource
   * \details         Arguments are only needed, if T has no default constructor. Only participates in in resolution
   *                  overload if the element type T can be constructed from \p args and if StdSp is true.
   * \vprivate        product
   * \tparam          Args
   *                  Types of the constructor arguments of T.
   * \param[in]       memory_resource
   *                  The memory resource to use as upstream resource.
   * \param[in]       args
   *                  Dummy constructor arguments for T. The instances of T are only used to determine the necessary
   *                  memory and they are destructed at the end of this constructor.
   * \context         ANY
   * \pre             -
   * \steady          FALSE
   */
  template <typename... Args, std::enable_if_t<std::is_constructible<T, Args...>::value && StdSp, bool> = true>
  explicit SharedPtrResource(ara::core::MemoryResource* memory_resource, Args&&... args) noexcept
      : MemoryResource{}, upstream_resource_(memory_resource), resource_ptr_info_{} {
    amsr::core::Array<std::shared_ptr<T>, N> a{};
    ara::core::PolymorphicAllocator<T> const alloc{this};
    for (std::size_t i{0}; i < N; ++i) {
      a[i] = std::allocate_shared<T>(alloc, args...);  // VCA_VAC_STD_FUNC_FULFILL_CONTRACT
    }
    max_allocations_ = resources_in_use_;
  }

  /*!
   * \brief           Constructs a SharedPtrResource
   * \details         Arguments are only needed, if T has no default constructor. Only participates in in resolution
   *                  overload if the element type T can be constructed from \p args and if StdSp is false.
   * \vprivate        product
   * \tparam          Args
   *                  Types of the constructor arguments of T.
   * \param[in]       memory_resource
   *                  The memory resource to use as upstream resource.
   * \param[in]       args
   *                  Dummy constructor arguments for T. The instances of T are only used to determine the necessary
   *                  memory and they are destructed at the end of this constructor.
   * \context         ANY
   * \pre             -
   * \steady          FALSE
   */
  template <typename... Args, std::enable_if_t<std::is_constructible<T, Args...>::value && !StdSp, bool> = true>
  explicit SharedPtrResource(ara::core::MemoryResource* memory_resource, Args&&... args) noexcept
      : MemoryResource{}, upstream_resource_(memory_resource), resource_ptr_info_{} {
    amsr::core::Array<amsr::SharedPtr<T>, N> a{};
    ara::core::PolymorphicAllocator<T> const alloc{this};
    for (std::size_t i{0}; i < N; ++i) {
      // If alloc fails to allocate, it will abort. No need to check for success.
      a[i] = amsr::AllocateShared<T>(alloc, args...).Value();
    }
    max_allocations_ = resources_in_use_;
  }

  // Deleted Copy Constructor.
  SharedPtrResource(SharedPtrResource const&) = delete;

  // Deleted Move Constructor.
  SharedPtrResource(SharedPtrResource&&) = delete;

  // Deleted Copy assignment operator.
  SharedPtrResource& operator=(SharedPtrResource const&) = delete;

  // Deleted Move assignment operator.
  SharedPtrResource& operator=(SharedPtrResource&&) = delete;

  /*!
   * \brief           Destructor: deleting the allocated memory
   * \steady          FALSE
   */
  ~SharedPtrResource() noexcept final {  // VCA_VAC_POSSIBLY_FREEING_STATIC_MEMORY
    assert(upstream_resource_ != nullptr);
    for (std::size_t index{0}; index < resource_ptr_info_.size(); ++index) {
      // VCA_VAC_VALID_PTR_ARG_IN_UNDEFINED_FUN
      upstream_resource_->deallocate(resource_ptr_info_[index].allocated_ptr, resource_ptr_info_[index].allocated_bytes,
                                     0);
    }
  }

  /*!
   * \brief           To check if this memory resource is being used or not
   * \return          True if memory resource is being used, false otherwise.
   * \steady          TRUE
   */
  auto IsInUse() const noexcept -> bool { return resources_in_use_ > 0U; }

 protected:
  /*!
   * \brief           Allocates the requested number of bytes on the heap using the platform's default alignment
   * \details         Also, sets the usage status for allocated memory and this resource. No new allocation is performed
   *                  if already allocated memory is not being used and abort if no memory could be allocated.
   * \param[in]       bytes
   *                  The number of bytes to allocate.
   * \return          A pointer to the allocated memory.
   * \steady          TRUE
   */
  auto DoAllocate(std::size_t bytes, std::size_t) noexcept -> void* final {
    assert(upstream_resource_ != nullptr);
    void* ret_ptr{nullptr};
    for (std::size_t index{0}; index < max_allocations_; ++index) {
      if (resource_ptr_info_[index].allocated_ptr == nullptr) {
        resource_ptr_info_[index].allocated_ptr = upstream_resource_->allocate(bytes);
        ret_ptr = resource_ptr_info_[index].allocated_ptr;
        resource_ptr_info_[index].memory_in_use = true;
        resource_ptr_info_[index].allocated_bytes = bytes;
        resources_in_use_++;
        break;
      } else if ((!resource_ptr_info_[index].memory_in_use) && (resource_ptr_info_[index].allocated_bytes == bytes)) {
        ret_ptr = resource_ptr_info_[index].allocated_ptr;
        resource_ptr_info_[index].memory_in_use = true;
        resources_in_use_++;
        break;
      } else {
        continue;
      }
    }
    if (ret_ptr == nullptr) {
      amsr::core::Abort("SharedPtrResource: Cannot allocate memory for SharedPtr!");
    }
    return ret_ptr;
  }

  /*!
   * \brief           Un-sets the usage status of memory pointed by given pointer
   * \param[in]       p
   *                  A pointer to memory. Must have been allocated using \a DoAllocate().
   * \steady          TRUE
   */
  void DoDeallocate(void* p, std::size_t, std::size_t) noexcept final {
    for (std::size_t index{0}; index < max_allocations_; ++index) {
      if (resource_ptr_info_[index].allocated_ptr == p) {
        resource_ptr_info_[index].memory_in_use = false;
        resources_in_use_--;
        break;
      }
    }
  }

  /*!
   * \brief           Checks if \p other equals \p this
   * \details         They are considered equal if they refer to the same object, that is, if the addresses of the
   *                  objects are equal.
   * \param[in]       other
   *                  The other memory resource.
   * \return          True if \p other equals this object, false otherwise.
   * \steady          TRUE
   */
  auto DoIsEqual(ara::core::MemoryResource const& other) const noexcept -> bool final { return this == &other; }

 private:
  /*! \brief          Struct to keep the track of memory allocated for SharedPtrResource. */
  struct SharedPtrInfo {
    /*! \brief          Pointer to allocated memory. */
    void* allocated_ptr{nullptr};
    /*! \brief          Size of allocated memory. */
    size_t allocated_bytes{0};
    /*! \brief          Indicates whether the pointer is in use. */
    bool memory_in_use{false};
  };

  /*! \brief          Pointer to the upstream memory resource */
  ara::core::MemoryResource* upstream_resource_;

  /*!
   * \brief           Maximum number of allocations needed to create N shared pointers
   * \details         It is determined in constructor by creating N shared pointers.
   */
  std::size_t max_allocations_{2 * N};

  /*!
   * \brief           Container to hold allocated memory info
   * \details         The container size is 2 * N as object and control block might be allocated separately when using
   *                  std::allocate_shared<T>().
   */
  amsr::core::Array<SharedPtrInfo, 2 * N> resource_ptr_info_;

  /*! \brief          To set/unset usage of this resource. */
  std::size_t resources_in_use_{0U};
};

/*!
 * \brief           A thread safe memory resource for allocating memory for the object and control block of
 *                  std::shared_ptr
 * \details         It pre-allocates memory for N shared pointers during construction and aborts if allocation fails.
 * \vprivate        Product private internal API
 * \tparam          T
 *                  The object type of the shared pointer.
 * \tparam          N
 *                  The number of shared pointers to allocate memory for.
 * \tparam          StdSp
 *                  If the resource is meant for a std::shared_ptr. If false, then amsr::SharedPtr is assumed.
 * \trace           CREQ-VaCommonLib-SharedPtrResource
 */
template <typename T, std::size_t N = 1, bool StdSp = true>
class SynchronizedSharedPtrResource final : public ara::core::MemoryResource {
  /*!
   * \brief Helper function to find the std::shared_ptr type specified by the StdSp template parameter.
   * \tparam B Boolean to determine if the resource is meant for std::shared_ptr (true) or amsr::SharedPtr (false)
   * \return No return value, only the type std::shared_ptr<T>
   */
  template <bool B = StdSp, std::enable_if_t<B, bool> = true>
  static auto GetUsedPtrType() noexcept -> std::shared_ptr<T>;

  /*!
   * \brief Helper function to find the amsr::SharedPtr type specified by the StdSp template parameter.
   * \tparam B Boolean to determine if the resource is meant for std::shared_ptr (true) or amsr::SharedPtr (false)
   * \return No return value, only the type amsr::SharedPtr<T>
   */
  template <bool B = StdSp, std::enable_if_t<!B, bool> = true>
  static auto GetUsedPtrType() noexcept -> amsr::SharedPtr<T>;

 public:
  /*!
   * \brief           The shared pointer type that the resource is reserving memory for
   * \details         If StdSp is true then this is a std::shared_ptr, and if false amsr::SharedPtr
   * \vprivate        product
   */
  using SharedPtrType = decltype(GetUsedPtrType<StdSp>());

  /*!
   * \brief           Constructs a SynchronizedSharedPtrResource
   * \details         Arguments are only needed, if T has no default constructor. Only participates in in resolution
   *                  overload if the element type T can be constructed from \p args.
   * \tparam          Args
   *                  Types of the constructor arguments of T.
   * \param[in]       args
   *                  Dummy constructor arguments for T. The instances of T are only used to determine the necessary
   *                  memory and they are destructed at the end of this constructor.
   * \context         ANY
   * \pre             -
   * \steady          FALSE
   */
  template <typename... Args, std::enable_if_t<std::is_constructible<T, Args...>::value, bool> = true>
  explicit SynchronizedSharedPtrResource(Args&&... args) noexcept
      : SynchronizedSharedPtrResource(ara::core::GetDefaultResource(), std::forward<Args>(args)...) {}

  /*!
   * \brief           Constructs a SharedPtrResource
   * \details         Arguments are only needed, if T has no default constructor. Only participates in in resolution
   *                  overload if the element type T can be constructed from \p args.
   * \tparam          Args
   *                  Types of the constructor arguments of T.
   * \param[in]       memory_resource
   *                  The memory resource to use as upstream resource.
   * \param[in]       args
   *                  Dummy constructor arguments for T. The instances of T are only used to determine the necessary
   *                  memory and they are destructed at the end of this constructor.
   * \context         ANY
   * \pre             -
   * \steady          FALSE
   */
  template <typename... Args, std::enable_if_t<std::is_constructible<T, Args...>::value, bool> = true>
  explicit SynchronizedSharedPtrResource(ara::core::MemoryResource* memory_resource, Args&&... args) noexcept
      : MemoryResource{}, mutex_{}, delegate_(memory_resource, std::forward<Args>(args)...) {}

  // Deleted Copy Constructor.
  SynchronizedSharedPtrResource(SynchronizedSharedPtrResource const&) = delete;

  // Deleted Move Constructor.
  SynchronizedSharedPtrResource(SynchronizedSharedPtrResource&&) = delete;

  // Deleted Copy assignment operator.
  SynchronizedSharedPtrResource& operator=(SynchronizedSharedPtrResource const&) = delete;

  // Deleted Move assignment operator.
  SynchronizedSharedPtrResource& operator=(SynchronizedSharedPtrResource&&) = delete;

  /*!
   * \brief           Destructor: deleting the allocated memory
   * \details         The mutex_ is declared first in the members which means it will be destroyed last. Hence the
   *                  SharedPtrResource this class delegates to will be destroyed when the lock is still locked, and
   *                  then the destructor of the mutex will be called.
   * \steady          FALSE
   */
  ~SynchronizedSharedPtrResource() noexcept final { mutex_.lock(); };  // VCA_VAC_POSSIBLY_FREEING_STATIC_MEMORY

  /*!
   * \brief           To check if this memory resource is being used or not
   * \return          True if memory resource is being used, false otherwise.
   * \steady          TRUE
   */
  auto IsInUse() noexcept -> bool {
    std::lock_guard<std::mutex> const lock{mutex_};
    return delegate_.IsInUse();
  }

 protected:
  /*!
   * \brief           Allocates the requested number of bytes on the heap using the platform's default alignment
   * \details         Also, sets the usage status for allocated memory and this resource. No new allocation is performed
   *                  if already allocated memory is not being used and abort if no memory could be allocated.
   * \param[in]       bytes
   *                  The number of bytes to allocate.
   * \return          A pointer to the allocated memory.
   * \steady          TRUE
   */
  auto DoAllocate(std::size_t bytes, std::size_t) noexcept -> void* final {
    std::lock_guard<std::mutex> const lock{mutex_};
    return delegate_.allocate(bytes, 0);
  }

  /*!
   * \brief           Un-sets the usage status of memory pointed by given pointer
   * \param[in]       p
   *                  A pointer to memory. Must have been allocated using \a DoAllocate().
   * \steady          TRUE
   */
  void DoDeallocate(void* p, std::size_t, std::size_t) noexcept final {
    std::lock_guard<std::mutex> const lock{mutex_};
    delegate_.deallocate(p, 0, 0);  // VCA_VAC_CALLING_UNDEFINED_FUN
  }

  /*!
   * \brief           Checks if \p other equals \p this
   * \details         They are considered equal if they refer to the same object, that is, if the addresses of the
   *                  objects are equal.
   * \param[in]       other
   *                  The other memory resource.
   * \return          True if \p other equals this object, false otherwise. \steady TRUE
   */
  auto DoIsEqual(ara::core::MemoryResource const& other) const noexcept -> bool final { return this == &other; }

 private:
  /*! \brief          Type of the object to delegate to */
  using Delegate = SharedPtrResource<T, N, StdSp>;

  /*! \brief          A mutex to lock the shared_ptr_resource */
  std::mutex mutex_;

  /*! \brief          The shared_ptr_resource to delegate to */
  Delegate delegate_;
};

}  // namespace amsr

/*!
 * \exclusivearea   amsr::SynchronizedSharedPtrResource::mutex_
 *                  Protect against multiple threads using the same SharedPtrResource at the same time.
 * \protects        amsr::SynchronizedSharedPtrResource::delegate_
 * \usedin          amsr::SynchronizedSharedPtrResource::IsInUse
 *                  amsr::SynchronizedSharedPtrResource::DoAllocate
 *                  amsr::SynchronizedSharedPtrResource::DoDeallocate
 * \exclude         amsr::SynchronizedSharedPtrResource::DoIsEqual
 * \length          SHORT
 * \endexclusivearea
 */

#endif  // LIB_VAC_INCLUDE_AMSR_SHARED_PTR_RESOURCE_H_
