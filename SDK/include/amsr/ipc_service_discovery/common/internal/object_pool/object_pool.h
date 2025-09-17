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
/*! \file
 *  \brief Implementation of a smart object pool.
 *  \unit IpcServiceDiscovery::Common::ObjectPool
 *
 *********************************************************************************************************************/
#ifndef LIB_IPC_SERVICE_DISCOVERY_COMMON_INCLUDE_AMSR_IPC_SERVICE_DISCOVERY_COMMON_INTERNAL_OBJECT_POOL_OBJECT_POOL_H_
#define LIB_IPC_SERVICE_DISCOVERY_COMMON_INCLUDE_AMSR_IPC_SERVICE_DISCOVERY_COMMON_INTERNAL_OBJECT_POOL_OBJECT_POOL_H_

#include <cassert>
#include <memory>
#include <stack>
#include "amsr/ipc_service_discovery/common/internal/abort_wrapper/abort.h"

namespace amsr {
namespace ipc_service_discovery {
namespace common {
namespace internal {

/*!
 * \brief Smart object pool following the RAII principle to return objects to the pool.
 * \details Implementation of a generic object pool which makes use of the RAII principle to acquire and release
 *          the objects from and to the pool by using smart pointers. This means, as soon as an acquired object is
 *          destroyed, it automatically returns to the pool it was acquired from. Furthermore, it is ensured that
 *          the memory of the objects is released properly, even if the pool has already been destroyed and not all
 *          objects have been returned to the pool yet.
 * \tparam T Type of the objects stored in the object pool stack.
 * \vprivate Component Private
 */
template <typename T>
// VCA_IPCSERVICEDISCOVERY_SPC_15_LNG_03_COMPILER_GENERATED_CODE
class ObjectPool final {
 private:
  /*!
   * \brief Pointer to the ObjectPool.
   */
  using ObjectPoolPtr = ObjectPool<T>*;

  /*!
   * \brief Deleter for the template type.
   */
  using Deleter = std::default_delete<T>;

 public:
  /*!
   * \brief To pool deleter functor.
   * \details Custom ToPoolDeleter for the to pool unique pointer. The released object is either returned
   *          to the pool (only if the pool still exits) or deleted via the default deleter.
   * \trace DSGN-IpcServiceDiscovery-Common-ObjectPoolRelease
   */
  // VCA_IPCSERVICEDISCOVERY_SPC_15_LNG_03_COMPILER_GENERATED_CODE
  class ToPoolDeleter {
   public:
    /*!
     * \brief Create a ToPoolDeleter.
     * \param[in] object_pool_ptr_weak_ptr Weak pointer to an object pool pointer.
     * \context ANY
     * \pre -
     * \threadsafe FALSE
     * \reentrant FALSE
     * \synchronous TRUE
     * \vprivate Component Private
     */
    explicit ToPoolDeleter(std::weak_ptr<ObjectPoolPtr> object_pool_ptr_weak_ptr) noexcept
        : object_pool_ptr_weak_ptr_(object_pool_ptr_weak_ptr) {}

    /*!
     * \brief Delete or return the given object to the pool and perform precondition check.
     * \param[in] object_ptr Raw object pointer to be deleted or returned to pool. Must not be a nullptr.
     * \context ANY
     * \pre -
     * \threadsafe FALSE
     * \reentrant FALSE
     * \synchronous TRUE
     * \vprivate Component Private
     */
    void operator()(T* object_ptr) const noexcept {
      if (object_ptr == nullptr) {
        Abort("ToPoolDeleter() must not be called with a nullptr.");  // COV_IpcSD_utility
      }
      ReturnOrDeleteObject(object_ptr);
    }

   private:
    /*!
     * \brief Delete or return the given object to the pool.
     * \param[in] object_ptr Raw object pointer to be deleted or returned to pool.
     *                       Must not be a nullptr. This is not checked.
     * \context ANY
     * \pre Object pool pointer inside shared pointer must not be nullptr.
     * \threadsafe FALSE
     * \reentrant FALSE
     * \synchronous TRUE
     * \vprivate Component Private
     *
     * \internal
     * - Try to convert weak pointer to shared pointer.
     *   - Extract object pool pointer from shared pointer.
     *   - Abort if object pool pointer is nullptr.
     *   - Return given object to object pool.
     * - Otherwise delete object via default deleter.
     * \endinternal
     */
    void ReturnOrDeleteObject(T* object_ptr) const noexcept {
      /*! Only if the weak pointer can be converted to a shared pointer, the object pool still exists and the
       *  object can be returned to object pool. Otherwise, the object will be deleted via the default deleter. */
      // VECTOR NL AutosarC++17_10-A18.5.8: MD_IPCSERVICEDISCOVERY_A18.5.8_localObjectsShallBeAllocatedOnTheStack
      if (std::shared_ptr<ObjectPoolPtr> const object_pool_ptr_shared_ptr{object_pool_ptr_weak_ptr_.lock()}) {
        ObjectPoolPtr const object_pool_ptr{*object_pool_ptr_shared_ptr};
        assert(object_pool_ptr != nullptr);
        object_pool_ptr->Add(object_ptr);
      } else {
        // VCA_IPCSERVICEDISCOVERY_SPC_15_STDLIB_FULFILLS_FUNCTION_CONTRACT
        Deleter{}(object_ptr);
      }
    }

    /*! Weak pointer to object pool pointer. */
    std::weak_ptr<ObjectPoolPtr> object_pool_ptr_weak_ptr_;
  };

  ObjectPool<T>() = default;
  // VCA_IPCSERVICEDISCOVERY_SPC_15_STDLIB_FULFILLS_FUNCTION_CONTRACT
  ~ObjectPool<T>() = default;
  ObjectPool(ObjectPool const&) = delete;
  ObjectPool(ObjectPool&&) = delete;
  auto operator=(ObjectPool const&) -> ObjectPool& = delete;
  auto operator=(ObjectPool&&) -> ObjectPool& = delete;

  /*!
   * \brief Alias for a unique pointer with ToPoolDeleter.
   */
  using ObjectUniquePtr = std::unique_ptr<T, ToPoolDeleter>;

  /*!
   * \brief Acquire an object from the pool.
   * \details An object from the pool is acquired. If the pool is empty, a new object will be
   *          created and returned. The acquired object is automatically returned to the pool
   *          if it is deleted and the object pool still exits.
   * \return Object unique pointer with ToPoolDeleter holding a valid object.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   * \trace DSGN-IpcServiceDiscovery-Common-ObjectPoolAcquire
   *
   * \internal
   * - If pool is empty create new object.
   * - Otherwise return initialized object from pool.
   * \endinternal
   */
  template <typename... Args>
  auto Acquire(Args&&... args) noexcept -> ObjectUniquePtr {
    ObjectUniquePtr object_unique_ptr{CreateEmptyObjectUniquePtr()};

    if (IsEmpty()) {
      object_unique_ptr = CreateObject(std::forward<Args>(args)...);
    } else {
      object_unique_ptr = GetInitializedObject(std::forward<Args>(args)...);
    }
    return object_unique_ptr;
  }

  /*!
   * \brief Create and return empty object unique pointer with ToPoolDeleter.
   * \details An emtpy unique pointer is returned holding no valid object.
   *          The returned object unique pointer is set to nullptr.
   * \return Empty object unique pointer with ToPoolDeleter holding no object.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto CreateEmptyObjectUniquePtr() const noexcept -> ObjectUniquePtr {
    return ObjectUniquePtr(nullptr, CreateToPoolDeleter());
  }

  /*!
   * \brief Check if the object pool is empty.
   * \details Check if the object pool holds no objects inside the object stack.
   * \return true if empty, false otherwise.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto IsEmpty() const noexcept -> bool { return pool_.empty(); }

  /*!
   * \brief Get the number of objects available to be acquired.
   * \return Number of objects inside the pool stack.
   * \context ANY
   * \pre Unused function. Do not use this function unless covered by component tests!
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  // clang-format off
  auto GetSize() const noexcept -> size_t /* COV_IpcSD_unused */ {
    // clang-format on
    return pool_.size();
  }

 private:
  /*!
   * \brief Add object from unique pointer with default deleter to the object pool.
   * \param[in] object_unique_ptr Object unique pointer to be added to the object pool.
   *                              Must not be a nullptr. This is not checked.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  // VCA Disable [SPC-15] : VCA_IPCSERVICEDISCOVERY_SPC_15_STDLIB_FULFILLS_FUNCTION_CONTRACT
  void Add(std::unique_ptr<T> object_unique_ptr) noexcept { pool_.push(std::move(object_unique_ptr)); }
  // VCA Enable : VCA_IPCSERVICEDISCOVERY_SPC_15_STDLIB_FULFILLS_FUNCTION_CONTRACT

  /*!
   * \brief Add object from raw object pointer to the object pool.
   * \param[in] object_unique_ptr The object unique pointer which will be added to the pool.
   *                              Must not be a nullptr. This is not checked.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  void Add(T* object_ptr) noexcept { Add(std::unique_ptr<T>{object_ptr}); }

  /*!
   * \brief Create object and return a unique pointer (with ToPoolDeleter) holding the object.
   * \tparam Args The types of signature parameters.
   * \param[in] args Arguments forwarded to the constructor.
   * \return Unique pointer (with ToPoolDeleter) holding an object.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  template <typename... Args>
  auto CreateObject(Args&&... args) const noexcept -> ObjectUniquePtr {
    return ConvertObjectUniquePtr(std::make_unique<T>(std::forward<Args>(args)...));
  }

  /*!
   * \brief Create instance of ToPoolDeleter.
   * \return Initialized instance of a ToPoolDeleter.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto CreateToPoolDeleter() const noexcept -> ToPoolDeleter {
    // VCA_IPCSERVICEDISCOVERY_SPC_15_STDLIB_FULFILLS_FUNCTION_CONTRACT
    return ToPoolDeleter{std::weak_ptr<ObjectPoolPtr>{object_pool_ptr_shared_ptr_}};
  }

  /*!
   * \brief Convert object unique pointer with default deleter to object unique pointer with ToPoolDeleter.
   * \param[in] object_unique_ptr Object unique pointer with default deleter to be converted.
   * \return Object unique pointer with ToPoolDeleter.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto ConvertObjectUniquePtr(std::unique_ptr<T> object_unique_ptr) const noexcept -> ObjectUniquePtr {
    T* const object_ptr{object_unique_ptr.release()};
    return ObjectUniquePtr(object_ptr, CreateToPoolDeleter());
  }

  /*!
   * \brief Retrieve an initialized object from the pool.
   * \tparam Args The types of signature parameters.
   * \param[in] args Arguments forwarded to the constructor.
   * \return Object unique pointer with ToPoolDeleter holding initialized object.
   * \context ANY
   * \pre The pool must contain an object. This is not checked.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   *
   * \internal
   * - Get object from pool.
   * - Initialize object with placement new.
   * - Return initialized object.
   * \endinternal
   */
  template <typename... Args>
  auto GetInitializedObject(Args&&... args) noexcept -> ObjectUniquePtr {
    assert(!IsEmpty());
    // VCA Disable [SPC-15] : VCA_IPCSERVICEDISCOVERY_SPC_15_STDLIB_FULFILLS_FUNCTION_CONTRACT
    T* const object_ptr{pool_.top().release()};
    pool_.pop();

    // VCA_IPCSERVICEDISCOVERY_SLC_10_SLC_11_LNG_03_PLACEMENT_NEW
    new (object_ptr) T{std::forward<Args>(args)...};
    return ObjectUniquePtr(object_ptr, CreateToPoolDeleter());
    // VCA Enable : VCA_IPCSERVICEDISCOVERY_SPC_15_STDLIB_FULFILLS_FUNCTION_CONTRACT
  }

  /*! Shared pointer to object pool pointer. */
  std::shared_ptr<ObjectPoolPtr> object_pool_ptr_shared_ptr_{std::make_shared<ObjectPoolPtr>(this)};

  /*! Pool holding the objects which can be acquired. */
  std::stack<std::unique_ptr<T>> pool_{};
};

}  // namespace internal
}  // namespace common
}  // namespace ipc_service_discovery
}  // namespace amsr

#endif  // LIB_IPC_SERVICE_DISCOVERY_COMMON_INCLUDE_AMSR_IPC_SERVICE_DISCOVERY_COMMON_INTERNAL_OBJECT_POOL_OBJECT_POOL_H_

// COV_JUSTIFICATION_BEGIN
// \ID COV_IpcSD_unused
//   \ACCEPT  XX
//   \REASON  FunctionCoverage gap is reported for the API.
//            Since the API is part of the public unit interface and supposed to be helpful for future extensions of
//            the component and tested by unit tests, no component test function coverage needs to be achieved for this
//            function. Usage of the function is strictly forbidden by precondition and compliance with this
//            precondition is checked by review.
// COV_JUSTIFICATION_END
