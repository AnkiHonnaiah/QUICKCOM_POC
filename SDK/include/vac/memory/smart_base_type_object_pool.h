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
/*!        \file  object_pool.h
 *        \brief  ObjectPool class that can dynamically allocate a set of objects and then never deallocates them but
 *                recycles the memory for new objects.
 *         \unit  VaCommonLib::MemoryManagement::ObjectPool
 *
 *      \details  Currently, this class supports only one initial allocation of memory. All memory is allocated
 *                immediately, even if no objects are created.
 *
 *********************************************************************************************************************/

#ifndef LIB_VAC_INCLUDE_VAC_MEMORY_SMART_BASE_TYPE_OBJECT_POOL_H_
#define LIB_VAC_INCLUDE_VAC_MEMORY_SMART_BASE_TYPE_OBJECT_POOL_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cassert>
#include <memory>
#include <utility>

#include "vac/memory/object_pool.h"

namespace vac {
namespace memory {

// Forward declaration of DeleteableSmartPoolObject.
class DeleteableSmartPoolObject;

/*!
 * \brief   Interface for the actual Deleter for a smart pool object.
 * \details This class not copyable nor moveable.
 */
class SmartObjectPoolDeleterContext {
 public:
  /*!
   * \brief Default constructor.
   * \spec
   *  requires true;
   * \endspec
   */
  SmartObjectPoolDeleterContext() noexcept = default;

  /*!
   * \brief Default virtual destructor.
   * \spec
   *  requires true;
   * \endspec
   */
  virtual ~SmartObjectPoolDeleterContext() noexcept = default;

  /*!
   * \brief     Actual Deleter function.
   * \param[in] ptr Pointer to smart pool object.
   * \pre       -
   * \spec
   *  requires true;
   * \endspec
   */
  virtual void destroy(DeleteableSmartPoolObject* ptr) = 0;

  /*!
   * \brief Actual Deleter function.
   * \param ptr Const Pointer to smart pool object.
   * \pre   -
   * \spec
   *  requires true;
   * \endspec
   */
  virtual void destroy(DeleteableSmartPoolObject const* ptr) = 0;

 protected:
  /*!
   * \brief Deleted copy constructor.
   * \spec
   *  requires true;
   * \endspec
   */
  SmartObjectPoolDeleterContext(SmartObjectPoolDeleterContext const&) noexcept = delete;

  /*!
   * \brief Deleted move constructor.
   * \spec
   *  requires true;
   * \endspec
   */
  SmartObjectPoolDeleterContext(SmartObjectPoolDeleterContext&&) noexcept = delete;

  /*!
   * \brief  Deleted copy assignment operator.
   * \return A reference to the assigned-to object.
   * \spec
   *  requires true;
   * \endspec
   */
  SmartObjectPoolDeleterContext& operator=(SmartObjectPoolDeleterContext const&) & noexcept = delete;

  /*!
   * \brief  Deleted move assignment operator.
   * \return A reference to the assigned-to object.
   * \spec
   *  requires true;
   * \endspec
   */
  SmartObjectPoolDeleterContext& operator=(SmartObjectPoolDeleterContext&&) & noexcept = delete;
};

/*!
 * \brief Proxy on the object side for deleting SmartPoolObjects.
 */
class DeleteableSmartPoolObject {
 public:
  /*!
   * \brief     Constructor for SmartPoolObjects.
   * \param[in] deleter_context DeleterContext for the current object.
   * \pre       -
   * \spec
   *  requires true;
   * \endspec
   */
  explicit DeleteableSmartPoolObject(SmartObjectPoolDeleterContext* deleter_context) noexcept
      : deleter_context_(deleter_context) {}

  /*!
   * \brief  Returns the DeleterContext for deleting the current object.
   * \pre    -
   * \return DeleterContext.
   * \spec
   *  requires true;
   * \endspec
   */
  SmartObjectPoolDeleterContext* GetDeleterContext() const noexcept { return deleter_context_; }

  /*!
   * \brief Destructor.
   * \spec
   *  requires true;
   * \endspec
   */
  virtual ~DeleteableSmartPoolObject() noexcept = default;  // VCA_VAC_SAFE_DEALLOCATION

 protected:
  /*!
   * \brief Default copy constructor.
   * \spec
   *  requires true;
   * \endspec
   */
  DeleteableSmartPoolObject(DeleteableSmartPoolObject const&) noexcept = default;

  /*!
   * \brief Default move constructor.
   * \spec
   *  requires true;
   * \endspec
   */
  DeleteableSmartPoolObject(DeleteableSmartPoolObject&&) noexcept = default;

  /*!
   * \brief Default copy assignment operator.
   * \return A reference to the assigned-to object.
   * \spec
   *  requires true;
   * \endspec
   */
  DeleteableSmartPoolObject& operator=(DeleteableSmartPoolObject const&) & noexcept = default;

  /*!
   * \brief Default move assignment operator.
   * \return A reference to the assigned-to object.
   * \spec
   *  requires true;
   * \endspec
   */
  DeleteableSmartPoolObject& operator=(DeleteableSmartPoolObject&&) & noexcept = default;

 private:
  /*!
   * \brief Deleter context for a smart pool object.
   */
  SmartObjectPoolDeleterContext* deleter_context_;
};

namespace detail {
/*!
 * \brief Master Deleter class using the DeleterContext from the object to delete.
 */
class SmartBaseTypeObjectPoolDeleter final {
 public:
  /*!
   * \brief     The actual deleter function for const SmartPoolObject.
   * \param[in] ptr Pointer to object.
   * \pre       -
   * \spec
   *  requires true;
   * \endspec
   */
  void operator()(DeleteableSmartPoolObject* ptr) noexcept {
    if (ptr != nullptr) {
      SmartObjectPoolDeleterContext* const context{ptr->GetDeleterContext()};
      // VECTOR NL AutosarC++17_10-M0.1.2: MD_VAC_M0.1.2_infeasiblePaths
      if (context != nullptr) {
        context->destroy(ptr);  // VCA_VAC_VALID_FUNC_CALL_RUN_TIME_CHECK
      } else {
        // VECTOR Next Line AutosarC++17_10-A18.5.2: MD_VAC_A18.5.2_libraryFunction
        delete ptr;  // VCA_VAC_VALID_FUNC_CALL_RUN_TIME_CHECK
      }
    }
  }

  /*!
   * \brief The actual deleter function for const SmartPoolObject.
   * \param ptr Pointer to object.
   * \pre   -
   * \spec
   *  requires true;
   * \endspec
   */
  void operator()(DeleteableSmartPoolObject const* ptr) noexcept {
    if (ptr != nullptr) {
      SmartObjectPoolDeleterContext* const context{ptr->GetDeleterContext()};
      // VECTOR NL AutosarC++17_10-M0.1.2: MD_VAC_M0.1.2_infeasiblePaths
      if (context != nullptr) {
        context->destroy(ptr);  // VCA_VAC_VALID_FUNC_CALL_RUN_TIME_CHECK
      } else {
        // VECTOR Next Line AutosarC++17_10-A18.5.2: MD_VAC_A18.5.2_libraryFunction
        delete ptr;  // VCA_VAC_VALID_FUNC_CALL_RUN_TIME_CHECK
      }
    }
  }
};

// VECTOR Next Construct AutosarC++17_10-M5.2.3: MD_VAC_M5.2.3_castFromPolymorphicBaseClassToDerivedClass
// VECTOR Next Construct VectorC++-V5.2.3: MD_VAC_V5-2-3_castFromPolymorphicBaseClassToDerivedClass
/*!
 * \brief   Deleter for unique_ptrs to ObjectPool-managed objects
 * \details This class not copyable nor moveable.
 *          This class is only used internally in SmartBaseTypeObjectPool<T> as a deleter for std::unique_ptr<T>, using
 *          SmartBaseTypeObjectPoolDeleter as a delegate. In this context, it is safe to use static_cast<T*>.
 * \tparam  T The type of the allocated object.
 *          must implement DeleteableSmartPoolObject.
 * \tparam  alloc is an allocator which fulfill C++ Named Requirements: Allocator.
 *          The default type of alloc is std::allocator<T>.
 */
template <typename T, typename alloc>
class SmartObjectPoolDeleterContextImpl : public SmartObjectPoolDeleterContext {
  static_assert(std::is_base_of<DeleteableSmartPoolObject, T>::value, "T must implement DeleteableSmartPoolObject");

 public:
  /*!
   * \brief Deleted copy constructor.
   * \spec
   *  requires true;
   * \endspec
   */
  SmartObjectPoolDeleterContextImpl(SmartObjectPoolDeleterContextImpl const&) noexcept = delete;

  /*!
   * \brief Deleted move constructor.
   * \spec
   *  requires true;
   * \endspec
   */
  SmartObjectPoolDeleterContextImpl(SmartObjectPoolDeleterContextImpl&&) noexcept = delete;

  // VECTOR Next Construct AutosarC++17_10-A10.2.1: MD_VAC_A10.2.1_redefinitionOfNonVirtualFunction
  /*!
   * \brief  Deleted copy assignment operator.
   * \return A reference to the assigned-to object.
   * \spec
   *  requires true;
   * \endspec
   */
  SmartObjectPoolDeleterContextImpl& operator=(SmartObjectPoolDeleterContextImpl const&) & noexcept = delete;

  // VECTOR Next Construct AutosarC++17_10-A10.2.1: MD_VAC_A10.2.1_redefinitionOfNonVirtualFunction
  /*!
   * \brief  Deleted move assignment operator.
   * \return A reference to the assigned-to object.
   * \spec
   *  requires true;
   * \endspec
   */
  SmartObjectPoolDeleterContextImpl& operator=(SmartObjectPoolDeleterContextImpl&&) & noexcept = delete;
  /*!
   * \brief Default overridden destructor.
   * \spec
   *  requires true;
   * \endspec
   */
  ~SmartObjectPoolDeleterContextImpl() noexcept override = default;  // VCA_VAC_SAFE_DEALLOCATION

  /*!
   * \brief     Constructor.
   * \param[in] pool Object pool to return an object to on destruction.
   * \pre       -
   * \spec
   *  requires true;
   * \endspec
   */
  explicit SmartObjectPoolDeleterContextImpl(ObjectPool<T, alloc>* pool) noexcept
      : SmartObjectPoolDeleterContext{}, pool_{pool} {}

  // VECTOR NC AutosarC++17_10-M0.1.2: MD_VAC_M0.1.2_infeasiblePaths
  /*!
   * \brief     Actual Deleter function.
   * \param[in] ptr Pointer to smart pool object.
   * \pre       -
   * \spec
   *  requires true;
   * \endspec
   */
  void destroy(DeleteableSmartPoolObject* ptr) noexcept override {
    if (ptr != nullptr) {
      if (pool_ != nullptr) {
        pool_->destroy(static_cast<T*>(ptr));
      } else {
        // VECTOR Next Line AutosarC++17_10-A18.5.2: MD_VAC_A18.5.2_libraryFunction
        delete ptr;  // VCA_VAC_VALID_FUNC_CALL_RUN_TIME_CHECK
      }
    }
  }

  // VECTOR NC AutosarC++17_10-M0.1.2: MD_VAC_M0.1.2_infeasiblePaths
  /*!
   * \brief     Actual Deleter function.
   * \param[in] ptr Pointer to smart pool object.
   * \pre       -
   * \spec
   *  requires true;
   * \endspec
   */
  void destroy(DeleteableSmartPoolObject const* ptr) noexcept override {
    if (ptr != nullptr) {
      if (pool_ != nullptr) {
        pool_->destroy(static_cast<T const*>(ptr));
      } else {
        // VECTOR Next Line AutosarC++17_10-A18.5.2: MD_VAC_A18.5.2_libraryFunction
        delete ptr;  // VCA_VAC_VALID_FUNC_CALL_RUN_TIME_CHECK
      }
    }
  }

 private:
  /*!
   * \brief The ObjectPool to return an object to on destruction.
   */
  ObjectPool<T, alloc>* pool_;
};
}  // namespace detail

/*!
 * \brief  Smart Pointer type used by the ObjectPool.
 * \tparam T The type to construct smart pointer type.
 */
template <typename T>
using SmartBaseTypeObjectPoolUniquePtr = std::unique_ptr<T, detail::SmartBaseTypeObjectPoolDeleter>;

/*!
 * \brief  Smart Pointer type used by the ObjectPool for const Objects.
 * \tparam T The type to construct smart pointer type for const object.
 */
template <typename T>
using SmartBaseTypeObjectPoolUniquePtrToConst = std::unique_ptr<T const, detail::SmartBaseTypeObjectPoolDeleter>;

// VECTOR Next Construct AutosarC++17_10-A11.3.1: MD_VAC_A11.3.1_doNotUseFriend
/*!
 * \brief   ObjectPool that deals in unique_ptrs instead of raw pointers, allowing for base type pointers.
 * \details This class is not copyable nor moveable.
 * \tparam  T The type of the object wich will be allocated.
 *          must implement DeleteableSmartPoolObject.
 * \tparam  alloc is an allocator which fulfill C++ Named Requirements: Allocator.
 *          The default type of alloc is std::allocator<T>.
 * \pre   -
 * \trace CREQ-VaCommonLib-ObjectPool
 */
template <typename T, typename alloc = std::allocator<T>>
class SmartBaseTypeObjectPool {
  /*! \brief Friendly class declaration.
   */
  friend class UT__ObjectPools__SmartBaseTypeObjectPool;
  static_assert(std::is_base_of<DeleteableSmartPoolObject, T>::value, "T must implement DeleteableSmartPoolObject");

 public:
  /*!
   * \brief Typedef for the raw-pointer ObjectPool used for storage.
   */
  using StoragePoolType = ObjectPool<T, alloc>;

  /*!
   * \brief Typedef for the size type used in this ObjectPool.
   */
  using size_type = typename StoragePoolType::size_type;

  /*!
   * \brief Deleter for unique_ptrs to ObjectPool-managed objects.
   */
  using Deleter = detail::SmartBaseTypeObjectPoolDeleter;

  /*!
   * \brief Smart Pointer type used by the ObjectPool.
   */
  using UniquePtr = SmartBaseTypeObjectPoolUniquePtr<T>;

  /*!
   * \brief Smart Pointer type used by the ObjectPool.
   */
  using UniquePtrToConst = SmartBaseTypeObjectPoolUniquePtrToConst<T>;

  /*!
   * \brief Constructor.
   * \spec
   *  requires true;
   * \endspec
   */
  SmartBaseTypeObjectPool() noexcept {}

  /*!
   * \brief Default copy constructor.
   * \spec
   *  requires true;
   * \endspec
   */
  SmartBaseTypeObjectPool(SmartBaseTypeObjectPool const&) noexcept = delete;

  /*!
   * \brief Default move constructor.
   * \spec
   *  requires true;
   * \endspec
   */
  SmartBaseTypeObjectPool(SmartBaseTypeObjectPool&&) noexcept = delete;

  /*!
   * \brief  Default copy assignment operator.
   * \return A reference to the assigned-to object.
   * \spec
   *  requires true;
   * \endspec
   */
  SmartBaseTypeObjectPool& operator=(SmartBaseTypeObjectPool const&) & noexcept = delete;

  /*!
   * \brief  Default move assignment operator.
   * \return A reference to the assigned-to object.
   * \spec
   *  requires true;
   * \endspec
   */
  SmartBaseTypeObjectPool& operator=(SmartBaseTypeObjectPool&&) & noexcept = delete;

  /*!
   * \brief Default virtual destructor.
   * \spec
   *  requires true;
   * \endspec
   */
  virtual ~SmartBaseTypeObjectPool() noexcept = default;  // VCA_VAC_SAFE_DEALLOCATION

  /*!
   * \brief     Update the memory allocation.
   *            The current implementation only allows a single allocation. All further reservations will only limit the
   *            visible memory. All subsequent calls to reserve() where new_capacity is greater than the initial
   *            new_capacity will be rejected with a bad_alloc.
   * \pre       -
   * \param     new_capacity The number of T's to reserve space for.
   * \spec
   *  requires true;
   * \endspec
   */
  void reserve(size_type new_capacity) noexcept { pool_.reserve(new_capacity); }

  // VECTOR Next Construct AutosarC++17_10-A18.5.8: MD_VAC_A18.5.8_localObjectsShallBeAllocatedOnStack
  /*!
   * \brief     Allocate and initialize an object of the pool.
   * \details   Calls the constructor with the given set of arguments. For all other properties, see allocate().
   *            Terminates if the constructor of the new object throws.
   * \pre       -
   * \tparam    Args The types of the callables.
   * \param[in] args Arguments forwarded to the constructor.
   * \return    A pointer to an initialized T.
   * \spec
   *  requires true;
   * \endspec
   */
  template <typename... Args>
  auto create(Args&&... args) noexcept -> UniquePtr {
    // VECTOR NL AutosarC++17_10-M0.3.1: MD_VAC_M0.3.1_possibleLeakObjectPoolCreate
    typename StoragePoolType::pointer const ptr{pool_.create(std::forward<Args>(args)..., &deleter_context_)};
    UniquePtr smart_ptr{ptr, Deleter()};
    return smart_ptr;
  }

  /*!
   * \brief  Determine whether the allocated memory is full.
   * \pre    -
   * \return False if there is space left in this object pool, i.e., the next call to create() will
   *         succeed. Otherwise true.
   * \spec
   *  requires true;
   * \endspec
   */
  bool full() const noexcept { return pool_.full(); }

  /*!
   * \brief  Determine whether there are not objects currently allocated.
   * \pre    -
   * \return True if no element is allocated inside this object pool. false otherwise.
   * \spec
   *  requires true;
   * \endspec
   */
  bool empty() const noexcept { return pool_.empty(); }

  /*!
   * \brief  The total number of objects that can be allocated in this pool.
   * \pre    -
   * \return The total number of objects that can be allocated in this pool.
   * \spec
   *  requires true;
   * \endspec
   */
  size_type capacity() const noexcept { return pool_.capacity(); }

  /*!
   * \brief  The number of objects currently allocated in this pool.
   * \pre    -
   * \return The number of objects currently allocated in this pool.
   * \spec
   *  requires true;
   * \endspec
   */
  size_type size() const noexcept { return pool_.size(); }

 private:
  /*!
   * \brief pool used for storage.
   */
  StoragePoolType pool_{};

  /*!
   * \brief deleter context for ObjectPool-managed objects.
   */
  detail::SmartObjectPoolDeleterContextImpl<T, alloc> deleter_context_{&pool_};
};

}  // namespace memory
}  // namespace vac

#endif  // LIB_VAC_INCLUDE_VAC_MEMORY_SMART_BASE_TYPE_OBJECT_POOL_H_
