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
/*!        \file  object_pool.h
 *        \brief  ObjectPool class that can dynamically allocate a set of objects and then never deallocates them but
 *                recycles the memory for new objects.
 *         \unit  VaCommonLib::MemoryManagement::ObjectPool
 *
 *      \details  Currently, this class supports only one initial allocation of memory. All memory is allocated
 *                immediately, even if no objects are created.
 *
 *********************************************************************************************************************/

#ifndef LIB_VAC_INCLUDE_VAC_MEMORY_OBJECT_POOL_H_
#define LIB_VAC_INCLUDE_VAC_MEMORY_OBJECT_POOL_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include <mutex>
#include <new>
#include <utility>

#include "ara/core/abort.h"
#include "ara/core/memory_resource.h"
#include "vac/container/static_vector.h"
#include "vac/testing/test_adapter.h"

namespace vac {
namespace memory {

// VECTOR Next Construct AutosarC++17_10-A11.3.1: MD_VAC_A11.3.1_doNotUseFriend
/*!
 * \brief   Class to manage pools of objects of a specific type.
 * \details Note: The lifetime of the ObjectPool must exceed the lifetime of the allocated objects. Otherwise, all
 *          pointers obtained from the pool will be dangling. Note: Currently, there is only the singleton instance
 *          supported, i.e., the object pool does not allow on-stack allocations.
 *          Note: ObjectPoolImpl is an internal helper class. Only use ObjectPool or SmartObjectPool in your code. The
 *          current implementation of this class heavily relies on exceptions. This is purely for development purposes:
 *          The occurrence of any of these exceptions indicates either a serious programming error
 *          (returning memory to the wrong pool) or a misconfiguration (an insufficient amount of resources was
 *          configured). For both cases, it must be ensured prior to production that the error conditions do not occur.
 * \tparam T The type of the managed objects. Object pool cannot be used with const objects.
 * \tparam Moveable The bool type to enable/disable move constructor.
 * \tparam Alloc is an allocator which fulfill C++ Named Requirements: Allocator.
 *         The default type of Alloc is ara::core::PolymorphicAllocator<T>.
 * \trace  CREQ-VaCommonLib-ObjectPool
 */
template <typename T, bool Moveable, typename Alloc = ara::core::PolymorphicAllocator<T>>
class ObjectPoolImpl {
  /*! \brief Friendly class declaration.
   */
  friend class UT__ObjectPools;

  /*! \brief Friendly class declaration.
   */
  friend class UT__ObjectPools__SmartBaseTypeObjectPool;

  /*! Friend declaration for testing purposes */
  FRIEND_TEST(UT__ObjectPools, ReservedSizeAndCapacity);

  static_assert(!(std::is_const<T>::value), "Object pool cannot be used with const Objects.");

 public:
  /*!
   * \brief Typedef for raw pointers.
   */
  using pointer = T*;

  /*!
   * \brief Typedef for raw pointers.
   */
  using const_pointer = T const*;

  /*!
   * \brief Typedef for the size type used in this implementation.
   */
  using size_type = std::size_t;

  /*!
   * \brief Constructor to create an empty ObjectPool.
   * \pre   -
   * \spec
   *  requires true;
   * \endspec
   */
  ObjectPoolImpl() noexcept {}

  /*!
   * \brief Deleted copy constructor.
   */
  ObjectPoolImpl(ObjectPoolImpl const&) = delete;

  /*!
   * \brief Deleted copy assignment.
   * \return
   */
  ObjectPoolImpl& operator=(ObjectPoolImpl const&) = delete;

  // VECTOR Next Construct AutosarC++17_10-A15.5.1: MD_VAC_A15.5.1_explicitNoexceptIfAppropriate
  /*!
   * \brief Default move constructor.
   * \spec
   *  requires true;
   * \endspec
   */
  constexpr ObjectPoolImpl(ObjectPoolImpl&&) noexcept = default;

  /*!
   * \brief  Default move assignment.
   * \return A reference to the assigned-to object.
   * \spec
   *  requires true;
   * \endspec
   */
  ObjectPoolImpl& operator=(ObjectPoolImpl&&) & noexcept = default;

  /*!
   * \brief Default destructor which does not call destructor of objects in owned memory.
   * \spec
   *  requires true;
   * \endspec
   */
  virtual ~ObjectPoolImpl() noexcept = default;  // VCA_VAC_SAFE_DEALLOCATION

  // VECTOR Next Construct AutosarC++17_10-A15.5.1: MD_VAC_A15.5.1_explicitNoexceptIfAppropriate
  /*!
   * \brief     Move constructor.
   * \details   This constructor is only available if the "Moveable" template parameter is set to true.
   * \tparam    U The type from T.
   * \pre       -
   * \param[in] other The ObjectPoolImpl to move from.
   * \spec
   *  requires true;
   * \endspec
   */
  template <typename U = T, typename = typename std::enable_if<Moveable, U>::type>
  ObjectPoolImpl(ObjectPoolImpl&& other) noexcept {
    std::lock(other.free_list_mutex_, free_list_mutex_);  // VCA_VAC_STD_FUNC_FULFILL_CONTRACT
    std::lock_guard<std::mutex> const other_lock{{other.free_list_mutex_}, {std::adopt_lock}};
    std::lock_guard<std::mutex> const this_lock{{free_list_mutex_}, {std::adopt_lock}};

    storage_ = std::move(other.storage_);
    std::swap(free_list_, other.free_list_);
    std::swap(allocation_count_, other.allocation_count_);
  }

  // VECTOR Next Construct AutosarC++17_10-A15.5.1: MD_VAC_A15.5.1_explicitNoexceptIfAppropriate
  /*!
   * \brief     Move assignment.
   * \details   This constructor is only available if the "Moveable" template parameter is set to true.
   * \tparam    U The type from T.
   * \param[in] other The ObjectPoolImpl to move from.
   * \pre       -
   * \return    A reference to the moved-to ObjectPoolImpl.
   * \spec
   *  requires true;
   * \endspec
   */
  template <typename U = T, typename = typename std::enable_if<Moveable, U>::type>
  ObjectPoolImpl& operator=(ObjectPoolImpl&& other) & noexcept {
    std::lock(other.free_list_mutex_, free_list_mutex_);  // VCA_VAC_STD_FUNC_FULFILL_CONTRACT
    std::lock_guard<std::mutex> const other_lock{{other.free_list_mutex_}, {std::adopt_lock}};
    std::lock_guard<std::mutex> const this_lock{{free_list_mutex_}, {std::adopt_lock}};

    storage_.swap(other.storage_);
    std::swap(free_list_, other.free_list_);
    std::swap(allocation_count_, other.allocation_count_);

    return *this;
  }

  /*!
   * \brief     Update the memory allocation.
   * \details   The current implementation only allows a single allocation. All further reservations will only limit the
   *            visible memory. All subsequent calls to reserve() where new_capacity is greater than the initial
   *            new_capacity will be ignored.
   * \param[in] new_capacity The number of T's to reserve space for.
   * \pre       -
   * \spec
   *  requires true;
   * \endspec
   */
  void reserve(size_type new_capacity) noexcept {
    std::lock_guard<std::mutex> const lock{free_list_mutex_};
    if (new_capacity > storage_.size()) {
      // Resize default-constructs all StoredType unions. Their default constructor activates the StoredType* member.
      storage_.resize(new_capacity);
      // Create the free list inside of storage
      free_list_ = nullptr;
      for (StoredType& elem : storage_) {
        elem.free = free_list_;  // VCA_VAC_VALID_PTR_ASSIGNING_NOT_DEREFERENCING
        free_list_ = &elem;
      }
    }
  }

  // VECTOR NL VectorC++-V2.7.6: MD_VAC_V2-7-6_cCommentShouldBeDoxygen
  /*VCAST_DONT_INSTRUMENT_START*/
  // VECTOR NL VectorC++-V2.7.6: MD_VAC_V2-7-6_cCommentShouldBeDoxygen
  /*VCAST_INSTRUMENTATION_ISSUE_REFERENCE: TAR-24033*/
  // VECTOR Next Construct VectorC++-V3.9.2: MD_VAC_V3-9-2_parametersBasedOnBuiltInTypes
  /*!
   * \brief     Allocate and initialize an object of the pool.
   * \details   Calls the constructor with the given set of arguments.
   *            For all other properties, see allocate(). Terminates if the constructor of the object throws.
   * \tparam    Args The types used to construct T.
   * \param[in] args Forwarded arguments that fulfill the requirements to construct T.
   * \pre       -
   * \return    A pointer to an initialized T.
   * \spec
   *  requires true;
   * \endspec
   */
  template <typename... Args>
  auto create(Args&&... args) noexcept -> pointer {
    pointer const ptr{this->allocate()};
    new (ptr) T(std::forward<Args>(args)...);  // VCA_VAC_TEMPLATE_REQ_MET
    return ptr;
  }
  // VECTOR NL VectorC++-V2.7.6: MD_VAC_V2-7-6_cCommentShouldBeDoxygen
  /*VCAST_DONT_INSTRUMENT_END*/

  /*!
   * \brief     Destroy an object and return it to the pool.
   * \details   Calls the destructor. For all other properties, see deallocate().
   *            Terminates if the destructor of the object throws.
   * \param[in] ptr A pointer to an initialized T.
   * \pre       The pointer should be managed, otherwise ara::core::Abort will be triggered.
   */
  void destroy(pointer ptr) noexcept {
    if (ptr != nullptr) {
      if (IsManaged(ptr)) {
        ptr->~T();  // VCA_VAC_VALID_NONSTATIC_METHOD
        deallocate(ptr);
      } else {
        ara::core::Abort("vac::memory::ObjectPoolImpl::destroy(pointer): Pointer is not managed!");
      }
    }
  }

  /*!
   * \brief   Destroy an object and return it to the pool.
   * \details Calls the destructor. For all other properties, see deallocate().
   *          Terminates if the destructor of the object throws.
   * \param   ptr A pointer to an initialized T.
   * \pre     The pointer should be managed, otherwise ara::core::Abort will be triggered.
   */
  void destroy(const_pointer ptr) noexcept {
    // VECTOR NL AutosarC++17_10-A5.2.3: MD_VAC_A5.2.3_constCastReducesCodeDuplication
    destroy(const_cast<pointer>(ptr));
  }

  /*!
   * \brief  Determine whether the allocated memory is full.
   * \pre    -
   * \return False if there is space left in this object pool, i.e., the next call to create() or allocate() will
   *         succeed. True otherwise.
   * \spec
   *  requires true;
   * \endspec
   */
  bool full() const noexcept {  // Comment to force line-break. See TAR-22835.
    return free_list_ == nullptr;
  }

  /*!
   * \brief  Determine whether there are not objects currently allocated.
   * \pre    -
   * \return True if no element is allocated inside this object pool. False otherwise.
   * \spec
   *  requires true;
   * \endspec
   */
  bool empty() const noexcept {  // Comment to force line-break. See TAR-22835.
    return allocation_count_ == 0;
  }

  /*!
   * \brief  The total number of objects that can be allocated in this pool.
   * \pre    -
   * \return The total number of objects that can be allocated in this pool.
   * \spec
   *  requires true;
   * \endspec
   */
  size_type capacity() const noexcept { return storage_.size(); }

  /*!
   * \brief  The number of objects currently allocated in this pool.
   * \pre    -
   * \return The number of objects currently allocated in this pool.
   * \spec
   *  requires true;
   * \endspec
   */
  size_type size() const noexcept { return allocation_count_; }

 private:
  // VECTOR Next Construct AutosarC++17_10-A9.5.1: MD_VAC_A9.5.1_unionsShallNotBeUsed
  /*!
   * \brief   Union type for the storage.
   * \details Allows the storage to be created with the size of max(sizeof(T), sizeof(FreeListNodeType));.
   *          Note: Unions are not styleguide conformant, this is a deliberate styleguide violation. Justification:
   *          The Idea is that the same block of memory can either be interpreted as a node of the free list (state "not
   *          allocated") or as an actual block of data (state "allocated"). Changing the meaning of the
   *          block upon allocation/deallocation is a fairly common practice.
   *          Using the union here is dangerous in the way that the underlying memory may be accessed in the way in
   *          which it is not initialized. This can happen for two reasons:
   *            (1) Programming Error inside ObjectPool. ObjectPool is the class that makes the switch between
   *                both types. This must be handled using code inspection and testing.
   *            (2) Accessing through a dangling pointer to the data member. This is just as bad as any dangling pointer
   *                access and is independent of the use of a union.
   *          There are multiple other solutions to this problem, but none of them are any better:
   *            (1) Drop the union and use a reinterpret_cast to convert the pointer. This means you have to
   *                cast *a lot* in this class. It also means that we need to juggle min/max(sizeof's), which we get for
   *                free using the union.
   *            (2) Make the Free List intrusive, e.g., Create a "Poolable" Interface that classes must implement.
   *                However, it is not clear how you can destroy an object and then still use its free list
   *                interpretation.
   *            (3) Make a dedicated, non-intrusive free list (class StoredType { T data; StoredType* free; }
   *                rather than union StoredType). However, this requires management of an additional "non-free-list" to
   *                store allocated blocks, requires a search in this this list to find the correct block upon
   *                deallocation, and also uses more memory.
   *          Therefore, using the union in this particular case is no worse than any of the other solutions while
   *          requiring the least amount of code.
   */
  union StoredType {
    // VECTOR Disable VectorC++-V11.0.2: MD_VAC_V11-0-2_mutableMemberShallBePrivate
    /*!
     * \brief Member for the free list.
     */
    StoredType* free;
    /*!
     * \brief Member for the payload data.
     */
    T data;

    /*!
     * \brief Constructor.
     * \pre   -
     * \spec
     *  requires true;
     * \endspec
     */
    StoredType() noexcept
        : free{nullptr}  // VCA_VAC_VALID_MEMBER_VARIABLE_ACCESS, VCA_VAC_VALID_ASSIGNING_NOT_ACCESSING
    {}                   // VCA_VAC_VALID_CONSTRUCTOR_CALLED

    // VECTOR Next Construct AutosarC++17_10-A15.5.1: MD_VAC_A15.5.1_explicitNoexceptIfAppropriate
    /*!
     * \brief Destructor.
     * \pre   -
     * \spec
     *  requires true;
     * \endspec
     */
    ~StoredType() noexcept {  // VCA_VAC_VALID_NONSTATIC_METHOD
      free = nullptr;         // VCA_VAC_VALID_MEMBER_VARIABLE_ACCESS, VCA_VAC_VALID_ASSIGNING_NOT_ACCESSING
    }

    /*!
     * \brief Default copy constructor.
     * \param other The storage to copy construct from.
     * \spec
     *  requires true;
     * \endspec
     */
    StoredType(StoredType const& other) noexcept = default;

    // VECTOR Next Construct AutosarC++17_10-A15.5.1: MD_VAC_A15.5.1_explicitNoexceptIfAppropriate
    /*!
     * \brief Default move constructor.
     * \param other The storage to move construct from.
     * \spec
     *  requires true;
     * \endspec
     */
    StoredType(StoredType&& other) noexcept = default;

    /*!
     * \brief Default copy operator.
     * \param other The storage to copy assign from.
     * \return lvalue.
     * \spec
     *  requires true;
     * \endspec
     */
    StoredType& operator=(StoredType const& other) & noexcept = default;

    // VECTOR Next Construct AutosarC++17_10-A15.5.1: MD_VAC_A15.5.1_explicitNoexceptIfAppropriate
    /*!
     * \brief Default move operator.
     * \param other The storage to move assign from.
     * \return rvalue.
     * \spec
     *  requires true;
     * \endspec
     */
    StoredType& operator=(StoredType&& other) & noexcept = default;
  };
  // VECTOR Enable VectorC++-V11.0.2

  /*!
   * \brief  Determine whether a pointed-to location is managed by this ObjectPool.
   * \param  ptr The pointer to check.
   * \pre    -
   * \return True if the location is managed, false otherwise. Also returns false on nullptr.
   * \spec
   *  requires true;
   * \endspec
   */
  bool IsManaged(T const* const ptr) const noexcept {
    bool ret_value{false};
    if ((storage_.data() != nullptr) && (ptr != nullptr)) {
      // VECTOR Disable AutosarC++17_10-M5.0.18: MD_VAC_M5.0.18_comparingUnrelatedPointers
      ret_value = (static_cast<void const*>(storage_.data()) <= static_cast<void const*>(ptr)) &&
                  (static_cast<void const*>(ptr) <= static_cast<void const*>(&storage_.back()));
      // VECTOR Enable AutosarC++17_10-M5.0.18
    }
    return ret_value;
  }

  /*!
   * \brief   Return a pointer to a chunk of memory large enough for storing a T.
   * \details When the pool is exhausted, ara::core::Abort() is called.
   * \pre     The head pointer to free_list can not be nullptr, otherwise ara::core::Abort will be triggered.
   * \return  Returns a pointer to a chunk of memory large enough for storing a T. The memory is not initialized, no T
   *          is constructed in this place.
   */
  T* allocate() noexcept {
    std::unique_lock<std::mutex> lock{free_list_mutex_};
    if (free_list_ == nullptr) {
      // Out of memory.
      ara::core::Abort("vac::memory::ObjectPoolImpl::allocate(): The pool is out of memory!");
    }
    StoredType* const element{free_list_};
    free_list_ = element->free;
    ++allocation_count_;
    // Deactivate free list member.
    element->free = nullptr;
    lock.unlock();
    return &element->data;
  }

  // VECTOR NC AutosarC++17_10-A8.4.4: MD_VAC_A8.4.4_useReturnValueInsteadOfOutputParameter
  /*!
   * \brief          Return a chunk of memory to the pool. The passed object is not destructed.
   * \param[in, out] ptr Pointer to the chunk of memory to be returned to the pool.
   * \pre            The pointer should be managed, otherwise ara::core::Abort will be triggered.
   */
  void deallocate(T* ptr) noexcept {
    if (ptr != nullptr) {
      std::lock_guard<std::mutex> const lock{free_list_mutex_};
      if (this->IsManaged(ptr)) {
        // VECTOR NL AutosarC++17_10-A5.2.4: MD_VAC_A5.2.4_reinterpretCast
        StoredType* const element{reinterpret_cast<StoredType*>(ptr)};  // Convert from data member back to union.
        // Activate free list member.
        element->free = free_list_;  // VCA_VAC_VALID_FUNC_CALL_RUN_TIME_CHECK
        free_list_ = element;
        --allocation_count_;
      } else {
        ara::core::Abort("vac::memory::ObjectPoolImpl::deallocate(T*): Pointer is not managed!");
      }
    }
  }

  /*!
   * \brief The actual memory location managed by the pool.
   */
  vac::container::StaticVector<StoredType, Alloc> storage_{};

  /*!
   * \brief Head pointer to the free list.
   */
  StoredType* free_list_{nullptr};

  /*!
   * \brief Mutex to synchronize access to the free_list_.
   */
  std::mutex free_list_mutex_{};

  /*!
   * \brief Number of elements the ObjectPool has handed out and that have not been returned.
   */
  size_type allocation_count_{0};
};

/*!
 * \brief  Non-Moveable ObjectPool for public usage.
 * \tparam T The type of the object.
 * \tparam Alloc is an allocator which fulfill C++ Named Requirements: Allocator.
 *         The default type of Alloc is ara::core::PolymorphicAllocator<T>.
 * \trace  CREQ-VaCommonLib-ObjectPool
 */
template <typename T, typename Alloc = ara::core::PolymorphicAllocator<T>>
using ObjectPool = ObjectPoolImpl<T, false, Alloc>;

/*!
 * \brief  Moveable ObjectPool for public usage.
 * \tparam T The type of the object.
 * \tparam Alloc is an allocator which fulfill C++ Named Requirements: Allocator.
 *         The default type of Alloc is ara::core::PolymorphicAllocator<T>.
 */
template <typename T, typename Alloc = ara::core::PolymorphicAllocator<T>>
using MoveableObjectPool = ObjectPoolImpl<T, true, Alloc>;

/*!
 * \brief  Deleter for unique_ptrs to ObjectPool-managed objects.
 * \tparam T The type of the object.
 * \tparam Alloc is an allocator which fulfill C++ Named Requirements: Allocator.
 *         The default type of Alloc is ara::core::PolymorphicAllocator<T>.
 */
template <typename T, typename Alloc = ara::core::PolymorphicAllocator<T>>
class SmartObjectPoolDeleter final {
 public:
  /*!
   * \brief Noexcept constructor.
   * \pre   -
   * \spec
   *  requires true;
   * \endspec
   */
  SmartObjectPoolDeleter() noexcept : SmartObjectPoolDeleter(nullptr) {}

  /*!
   * \brief     Constructor.
   * \pre       -
   * \param[in] pool Object pool to return an object to on destruction.
   * \spec
   *  requires true;
   * \endspec
   */
  explicit SmartObjectPoolDeleter(ObjectPool<T, Alloc>* pool) noexcept : pool_{pool} {}

  /*!
   * \brief Copy constructor.
   * \spec
   *  requires true;
   * \endspec
   */
  SmartObjectPoolDeleter(SmartObjectPoolDeleter const&) noexcept = default;

  /*!
   * \brief  Copy assignment.
   * \return A reference to the assigned-to object.
   * \spec
   *  requires true;
   * \endspec
   */
  SmartObjectPoolDeleter& operator=(SmartObjectPoolDeleter const&) & noexcept = default;

  // VECTOR Next Construct AutosarC++17_10-A15.5.1: MD_VAC_A15.5.1_explicitNoexceptIfAppropriate
  /*!
   * \brief Move constructor.
   * \spec
   *  requires true;
   * \endspec
   */
  SmartObjectPoolDeleter(SmartObjectPoolDeleter&&) noexcept = default;

  // VECTOR Next Construct AutosarC++17_10-A15.5.1: MD_VAC_A15.5.1_explicitNoexceptIfAppropriate
  /*!
   * \brief  Move assignment.
   * \return A reference to the assigned-to object.
   * \spec
   *  requires true;
   * \endspec
   */
  SmartObjectPoolDeleter& operator=(SmartObjectPoolDeleter&&) & noexcept = default;

  // VECTOR Next Construct AutosarC++17_10-A15.5.1: MD_VAC_A15.5.1_explicitNoexceptIfAppropriate
  /*!
   * \brief Default destructor.
   * \spec
   *  requires true;
   * \endspec
   */
  ~SmartObjectPoolDeleter() noexcept = default;

  /*!
   * \brief     The actual deleter function.
   * \pre       -
   * \param[in] ptr A pointer to the ObjectPool.
   * \spec
   *  requires true;
   * \endspec
   */
  void operator()(typename ObjectPool<T, Alloc>::pointer ptr) noexcept {
    if (pool_ != nullptr) {
      pool_->destroy(ptr);
    } else if (ptr != nullptr) {
      ptr->~T();
    } else {
      // Both pool and ptr are nullptr, nothing to do.
    }
  }

  /*!
   * \brief The actual deleter function for const T.
   * \pre   -
   * \param ptr A pointer to the ObjectPool.
   * \spec
   *  requires true;
   * \endspec
   */
  void operator()(typename ObjectPool<T, Alloc>::const_pointer ptr) noexcept {
    if (pool_ != nullptr) {
      pool_->destroy(ptr);
    } else if (ptr != nullptr) {
      // VECTOR NL AutosarC++17_10-A5.2.3: MD_VAC_A5.2.3_impicitCastRemovesConstQualification
      ptr->~T();  // VCA_VAC_VALID_PRECONDITIONED_NONSTATIC_METHOD, VCA_VAC_TEMPLATE_REQ_MET
    } else {
      // Both pool and ptr are nullptr, nothing to do.
    }
  }

 private:
  /*!
   * \brief The ObjectPool to return an object to on destruction.
   */
  ObjectPool<T, Alloc>* pool_;
};

/*!
 * \brief  Smart Pointer type used by the ObjectPool.
 * \tparam T The type of the object.
 * \tparam Alloc is an allocator which fulfill C++ Named Requirements: Allocator.
 *         The default type of Alloc is ara::core::PolymorphicAllocator<T>.
 */
template <typename T, typename Alloc = ara::core::PolymorphicAllocator<T>>
using SmartObjectPoolUniquePtr = std::unique_ptr<T, SmartObjectPoolDeleter<T, Alloc>>;

/*!
 * \brief  Smart Pointer type used by the ObjectPool for const Objects.
 * \tparam T The type of the object.
 * \tparam Alloc is an allocator which fulfill C++ Named Requirements: Allocator.
 *         The default type of Alloc is ara::core::PolymorphicAllocator<T>.
 */
template <typename T, typename Alloc = ara::core::PolymorphicAllocator<T>>
using SmartObjectPoolUniquePtrToConst = std::unique_ptr<T const, SmartObjectPoolDeleter<T, Alloc>>;

// VECTOR Next Construct AutosarC++17_10-A11.3.1: MD_VAC_A11.3.1_doNotUseFriend
/*!
 * \brief  ObjectPool that deals in unique_ptrs instead of raw pointers.
 * \tparam T The type of the object.
 * \tparam Alloc is an allocator which fulfill C++ Named Requirements: Allocator.
 *         The default type of Alloc is ara::core::PolymorphicAllocator<T>.
 * \trace  CREQ-VaCommonLib-ObjectPool
 */
template <typename T, typename Alloc = ara::core::PolymorphicAllocator<T>>
class SmartObjectPool final {
  /*! \brief Friendly class declaration.
   */
  friend class ObjectPoolTest;
  /*! Friend declaration for testing purposes */
  FRIEND_TEST(ObjectPoolTest, ReservedSizeAndCapacity);

  /*! \brief Friendly class declaration.
   */
  friend class UT__ObjectPools;
  /*! Friend declaration for testing purposes */
  FRIEND_TEST(UT__ObjectPools, ReservedSizeAndCapacity);

 public:
  /*!
   * \brief Typedef for the raw-pointer ObjectPool used for storage.
   */
  using StoragePoolType = ObjectPool<T, Alloc>;

  /*!
   * \brief Typedef for the size type used in this ObjectPool.
   */
  using size_type = typename StoragePoolType::size_type;

  /*!
   * \brief Deleter for unique_ptrs to ObjectPool-managed objects.
   */
  using Deleter = SmartObjectPoolDeleter<T, Alloc>;

  /*!
   * \brief Smart Pointer type used by the ObjectPool.
   */
  using UniquePtr = SmartObjectPoolUniquePtr<T, Alloc>;

  /*!
   * \brief Smart Pointer type used by the ObjectPool.
   */
  using UniquePtrToConst = SmartObjectPoolUniquePtrToConst<T, Alloc>;

  /*!
   * \brief Default constructor.
   * \spec
   *  requires true;
   * \endspec
   */
  SmartObjectPool() noexcept = default;

  /*!
   * \brief Deleted copy constructor.
   */
  SmartObjectPool(SmartObjectPool const&) = delete;

  /*!
   * \brief Deleted copy assignment.
   * \return
   */
  SmartObjectPool& operator=(SmartObjectPool const&) = delete;

  /*!
   * \brief Deleted move constructor.
   */
  SmartObjectPool(SmartObjectPool&&) = delete;

  /*!
   * \brief Deleted move assignment.
   * \return
   */
  SmartObjectPool& operator=(SmartObjectPool&&) = delete;

  // VECTOR Next Construct AutosarC++17_10-A15.5.1: MD_VAC_A15.5.1_explicitNoexceptIfAppropriate
  /*!
   * \brief Default destructor.
   * \spec
   *  requires true;
   * \endspec
   */
  ~SmartObjectPool() noexcept = default;

  /*!
   * \brief Update the memory allocation.
   *        The current implementation only allows a single allocation. All further reservations will only limit the
   *        visible memory. All subsequent calls to reserve() where new_capacity is greater than the initial
   *        new_capacity will be rejected with a bad_alloc. \param new_capacity The number of T's to reserve space for.
   * \pre   -
   * \spec
   *  requires true;
   * \endspec
   */
  void reserve(size_type new_capacity) noexcept { pool_.reserve(new_capacity); }

  // VECTOR Next Construct AutosarC++17_10-A18.5.8: MD_VAC_A18.5.8_localObjectsShallBeAllocatedOnStack
  /*!
   * \brief     Allocate and initialize an object of the pool.
   * \details   Calls the constructor with the given set of arguments. For all other properties, see allocate().
   *            Terminates if the constructor of the object throws.
   * \tparam    Args The types used to construct T.
   * \param[in] args Forwarded arguments that fulfill the requirements to construct T.
   * \pre       -
   * \return    A unique pointer to an initialized T.
   * \spec
   *  requires true;
   * \endspec
   */
  template <typename... Args>
  auto create(Args&&... args) noexcept -> UniquePtr {
    // VECTOR NL AutosarC++17_10-M0.3.1: MD_VAC_M0.3.1_possibleLeakObjectPoolCreate
    typename StoragePoolType::pointer const ptr{pool_.create(std::forward<Args>(args)...)};
    UniquePtr smart_ptr{ptr, Deleter(&pool_)};
    return smart_ptr;
  }

  /*!
   * \brief  Determine whether the allocated memory is full.
   * \pre    -
   * \return False if there is space left in this object pool, i.e., the next call to create() will
   *         succeed.
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
   * \brief ObjectPool used for storage.
   */
  StoragePoolType pool_;
};

}  // namespace memory
}  // namespace vac

/*!
 * \exclusivearea   vac::memory::ObjectPoolImpl::free_list_mutex_
 *                  Make sure the memory pool is not exhausted.
 * \protects        vac::memory::ObjectPoolImpl::free_list_
 * \usedin          vac::memory::ObjectPoolImpl::allocate
 *                  vac::memory::ObjectPoolImpl::deallocate
 *                  vac::memory::ObjectPoolImpl::reserve
 *                  vac::memory::ObjectPoolImpl(&&)
 *                  vac::memory::ObjectPoolImpl::operator=(&&)
 * \exclude         When free_list is exhausted during calling allocate(), ara::core::Abort will be triggered.
 *                  When the ptr is not managed by the current object pool during calling deallocate(), ara::core::Abort
 *                  will be triggered.
 * \length          SHORT This exclusive area happens when access memory through object pool.
 * \endexclusivearea
 */

#endif  // LIB_VAC_INCLUDE_VAC_MEMORY_OBJECT_POOL_H_
