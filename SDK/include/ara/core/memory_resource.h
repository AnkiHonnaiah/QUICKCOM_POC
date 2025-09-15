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
/*!      \file  ara/core/memory_resource.h
 *      \brief  Memory resources and PolymorphicAllocator.
 *       \unit  VaCommonLib::MemoryManagement::MemoryResource
 *********************************************************************************************************************/

#ifndef LIB_VAC_INCLUDE_ARA_CORE_MEMORY_RESOURCE_H_
#define LIB_VAC_INCLUDE_ARA_CORE_MEMORY_RESOURCE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <atomic>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <memory>
#include <utility>
#include "amsr/core/abort.h"

namespace ara {
namespace core {

/*!
 * \brief   Abstract interface to a set of classes encapsulating memory resources.
 * \trace   CREQ-VaCommonLib-MemoryResource
 * \vpublic
 */
// VECTOR NC AutosarC++17_10-A12.0.1: MD_VAC_A12-0-1_onlyDtorImplemented
// VECTOR NC VectorC++-V12.0.1: MD_VAC_V12-0-1_onlyDtorImplemented
class MemoryResource {
 public:
  /*!
   * \brief Destructs the instance of MemoryResource.
   */
  virtual ~MemoryResource() = default;

  /*!
   * \brief     Allocates storage with a size of at least \p bytes bytes.
   * \param[in] bytes      The number of bytes to allocate.
   * \param[in] alignment  The alignment of the allocated storage.
   * \return    Pointer to the allocated storage.
   * \steady    TRUE
   */
  auto allocate(std::size_t bytes, std::size_t alignment = alignof(std::max_align_t)) noexcept -> void*;

  /*!
   * \brief     Deallocates storage.
   * \param[in] p          Pointer to the storage to deallocate.
   * \param[in] bytes      The number of bytes to deallocate.
   * \param[in] alignment  The alignment of the storage to deallocate.
   * \steady    TRUE
   */
  void deallocate(void* p, std::size_t bytes, std::size_t alignment) noexcept;

  /*!
   * \brief     Checks whether \p other equals \p this.
   * \param[in] other  The memory resource to compare.
   * \return    true, if the memory resources are equal; false, otherwise.
   * \steady    TRUE
   */
  auto is_equal(MemoryResource const& other) const noexcept -> bool;

 protected:
  /*!
   * \brief     Allocates storage with a size of at least \p bytes bytes.
   * \param[in] bytes      The number of bytes to allocate.
   * \param[in] alignment  The alignment of the allocated storage.
   * \return    Pointer to the allocated storage.
   */
  virtual auto DoAllocate(std::size_t bytes, std::size_t alignment) noexcept -> void* = 0;

  /*!
   * \brief     Deallocates storage.
   * \param[in] p          Pointer to the storage to deallocate.
   * \param[in] bytes      The number of bytes to deallocate.
   * \param[in] alignment  The alignment of the storage to deallocate.
   */
  // VECTOR NC AutosarC++17_10-M9.3.3: MD_VAC_M9.3.3_methodCanBeDeclaredConstLogicalNonConst
  virtual void DoDeallocate(void* p, std::size_t bytes, std::size_t alignment) noexcept = 0;

  /*!
   * \brief     Checks whether \p other equals \p this.
   * \param[in] other  The memory resource to compare.
   * \return    true, if the memory resources are equal; false, otherwise.
   */
  virtual auto DoIsEqual(MemoryResource const& other) const noexcept -> bool = 0;
};

/*!
 * \brief     Compares two memory resources for equality.
 * \param[in] a  The first memory resource.
 * \param[in] b  The second memory resource.
 * \return    true, if the memory resource are equal; false, otherwise.
 * \pre       -
 * \vpublic
 * \steady    TRUE
 */
auto operator==(MemoryResource const& a, MemoryResource const& b) noexcept -> bool;

/*!
 * \brief     Compares two memory resources for inequality.
 * \param[in] a  The first memory resource.
 * \param[in] b  The second memory resource.
 * \return    true, if the memory resource are not equal; false, otherwise.
 * \pre       -
 * \vpublic
 * \steady    TRUE
 */
auto operator!=(MemoryResource const& a, MemoryResource const& b) noexcept -> bool;

/*!
 * \brief   Returns the default memory resource.
 * \details NewDeleteResource is the default memory resource.
 * \return  The default memory resource.
 * \pre     -
 * \context ANY
 * \vpublic
 * \steady  TRUE
 */
auto GetDefaultResource() noexcept -> MemoryResource*;

/*!
 * \brief     Set the default memory resource.
 * \param[in] resource  The new default memory resource.
 * \return    The previous default memory resource.
 * \pre       -
 * \context   ANY
 * \vpublic
 * \steady    TRUE
 */
auto SetDefaultResource(MemoryResource* resource) noexcept -> MemoryResource*;

/*!
 * \brief   Allocator with allocation behavior depending on the provided memory resource.
 * \details The lifetime of the provided memory resource must be at least as long as the lifetime of the
 *          PolymorphicAllocator.
 * \tparam  T  The element type of container types which use this allocator.
 * \trace   CREQ-VaCommonLib-PolymorphicAllocator
 * \vpublic
 */
// VECTOR NC AutosarC++17_10-A12.0.1: MD_VAC_A12.0.1_missingMoveConstructorAssignmentOperator
// VECTOR NC VectorC++-V12.0.1: MD_VAC_V12.0.1_missingMoveConstructorAssignmentOperator
template <typename T>
class PolymorphicAllocator {
 public:
  /*!
   * \brief The element type of container types which use this allocator.
   */
  using value_type = T;

  /*!
   * \brief   Constructs a PolymorphicAllocator with the default memory resource.
   * \pre     -
   * \context ANY
   * \steady  TRUE
   */
  PolymorphicAllocator() noexcept = default;

  /*!
   * \brief     Constructs a PolymorphicAllocator with a given MemoryResource as underlying memory resource.
   * \param[in] r  A pointer to the MemoryResource to use.
   * \steady    TRUE
   */
  explicit PolymorphicAllocator(MemoryResource* r) noexcept : memory_resource_{r} {}

  /*!
   * \brief  Copy constructor.
   * \steady TRUE
   */
  // VECTOR NL AutosarC++17_10-A12.8.6: MD_VAC_A12.8.6_copyMoveConstructorAssignmentShallBeProtectedOrDeleted
  PolymorphicAllocator(PolymorphicAllocator const&) noexcept = default;  // VCA_VAC_VALID_ASSIGNING_NOT_ACCESSING

  /*!
   * \brief   Move constructor.
   * \details The moved from PolymorphicAllocator is unchanged after this call.
   * \steady  TRUE
   */
  // VECTOR NC AutosarC++17_10-A12.8.6: MD_VAC_A12.8.6_copyMoveConstructorAssignmentShallBeProtectedOrDeleted
  // VCA_VAC_VALID_ASSIGNING_NOT_ACCESSING, VCA_VAC_VALID_PTR_ASSIGNING_NOT_DEREFERENCING, VCA_VAC_VALID_ARGUMENT
  PolymorphicAllocator(PolymorphicAllocator&&) noexcept = default;

  /*!
   * \brief     Constructs a PolymorphicAllocator from the memory resource of a different PolymorphicAllocator.
   * \tparam    U  The element type of the other PolymorphicAllocator.
   * \param[in] other  The other PolymorphicAllocator.
   * \steady    TRUE
   */
  // VECTOR NC AutosarC++17_10-A12.8.6: MD_VAC_A12.8.6_copyMoveConstructorAssignmentShallBeProtectedOrDeleted
  // VECTOR NC VectorC++-V12.1.4: MD_VAC_V12.1.4_ExplicitConstructor
  template <typename U>
  PolymorphicAllocator(PolymorphicAllocator<U> const& other) noexcept : PolymorphicAllocator{other.resource()} {}

  /*!
   * \brief   Destructor.
   * \details Does not destroy the underlying memory resource.
   * \steady  TRUE
   */
  // VCA_VAC_FREEING_STATIC_THIS_POINTER
  virtual ~PolymorphicAllocator() noexcept = default;

  // Deleted copy-assignment operator.
  auto operator=(PolymorphicAllocator const&) noexcept -> PolymorphicAllocator& = delete;

  /*!
   * \brief   Move-assignment operator.
   * \details The moved from PolymorphicAllocator is unchanged after moving it to *this.
   * \return  *this containing the contents of the moved from PolymorphicAllocator.
   * \steady  TRUE
   */
  // VECTOR NC AutosarC++17_10-A12.8.6: MD_VAC_A12.8.6_copyMoveConstructorAssignmentShallBeProtectedOrDeleted
  auto operator=(PolymorphicAllocator&&) & noexcept -> PolymorphicAllocator& = default;

  /*!
   * \brief     Allocates memory for \p n elements using the underlying memory resource.
   * \details   Aborts, if the number of bytes to allocate is too large for std::size_t.
   * \param[in] n  The number of elements to allocate memory for.
   * \return    nullptr if the underlying memory resource fails to allocate memory; a pointer to the allocated memory
   *            otherwise.
   * \pre       The underlying memory resource must be a valid object.
   * \steady    TRUE
   */
  auto allocate(std::size_t n) noexcept -> T* {
    T* result{nullptr};
    if (n <= (std::numeric_limits<std::size_t>::max() / sizeof(T))) {
      // VECTOR NC AutosarC++17_10-M5.2.8: MD_VAC_M5.2.8_voidPointerToTypedPointer
      // VCA_VAC_VALID_PRECONDITIONED_NONSTATIC_METHOD
      result = static_cast<T*>(memory_resource_->allocate(n * sizeof(T), alignof(T)));
    } else {
      amsr::core::Abort(
          "PolymorphicAllocator: Number of bytes to allocate is larger than std::numeric_limits<std::size_t>::max()!");
    }
    return result;
  }

  /*!
   * \brief     Deallocates the memory for \p n elements at location \p p.
   * \param[in] p  The memory to deallocate.
   * \param[in] n  The number of elements for which the memory was allocated.
   * \pre       The memory at \p p has been allocated using \a allocate(n).
   * \pre       The underlying memory resource must be a valid object.
   * \steady    TRUE
   */
  void deallocate(T* p, std::size_t n) noexcept {
    // VCA_VAC_VALID_PRECONDITIONED_NONSTATIC_METHOD
    memory_resource_->deallocate(p, n * sizeof(T), alignof(T));
  }

  /*!
   * \brief     Constructs an object in previously allocated memory.
   * \tparam    U     The type of the object to construct.
   * \tparam    Args  The types of U's constructor arguments.
   * \param[in] p     The memory to construct the object at. Must be allocated but uninitialized.
   * \param[in] args  The arguments of U's constructor.
   * \steady    TRUE
   */
  template <typename U, typename... Args>
  static void construct(U* p, Args&&... args) noexcept {
    // VCA_VAC_FUNCTION_PRECONDITIONED
    new (static_cast<void*>(p)) U(std::forward<Args>(args)...);
  }

  // VECTOR NC AutosarC++17_10-M9.3.1: MD_VAC_M9.3.1_ConstMemberFunctionsShallNotReturnNonConstPointers
  /*!
   * \brief     Destroys an object.
   * \tparam    U     The type of the object to destroy.
   * \param[in] p     The memory to construct the object at. Must point to a valid object.
   * \pre       -
   * \steady    TRUE
   */
  template <typename U>
  static void destroy(U* p) noexcept {
    constexpr bool kMustCallDestructor{!std::is_trivially_destructible<U>::value};
    if (kMustCallDestructor) {
      p->~U();  // VCA_VAC_VALID_NONSTATIC_METHOD, VCA_VAC_VALID_FUNC_CALL_COMPILE_TIME_CHECK
    }
  }

  /*!
   * \brief  Returns the underlying memory resource.
   * \return A pointer to the underlying memory resource.
   * \steady TRUE
   */
  // VECTOR NL AutosarC++17_10-M9.3.1: MD_VAC_M9.3.1_ConstMemberFunctionsShallNotReturnNonConstPointers
  auto resource() const noexcept -> MemoryResource* { return memory_resource_; }

  /*!
   * \brief  Returns a default-constructed PolymorphicAllocator object.
   * \return A default-constructed PolymorphicAllocator object.
   * \note   This method is used through std::allocator_traits.
   * \steady TRUE
   */
  // VECTOR NL AutosarC++17_10-M9.3.3: MD_VAC_M9.3.3_methodCanBeDeclaredStatic
  auto select_on_container_copy_construction() const noexcept -> PolymorphicAllocator<T> {
    return PolymorphicAllocator();
  }

 private:
  /*!
   * \brief The underlying memory resource.
   */
  MemoryResource* memory_resource_{GetDefaultResource()};
};

/*!
 * \brief     Checks if two PolymorphicAllocator are equal.
 * \details   They are equal if their underlying memory resources compare equal.
 * \tparam    T1  Element type of the first PolymorphicAllocator.
 * \tparam    T2  Element type of the second PolymorphicAllocator.
 * \param[in] lhs  The first PolymorphicAllocator.
 * \param[in] rhs  The second PolymorphicAllocator.
 * \return    True if the underlying memory resources of the two allocators are equal, false otherwise.
 * \steady    TRUE
 */
template <typename T1, typename T2>
auto operator==(PolymorphicAllocator<T1> const& lhs, PolymorphicAllocator<T2> const& rhs) noexcept -> bool {
  bool result{false};
  if ((lhs.resource() != nullptr) && (rhs.resource() != nullptr)) {
    result = lhs.resource()->is_equal(*rhs.resource());  // VCA_VAC_VALID_PTR_ARG_IN_UNDEFINED_FUN
  } else {
    result = lhs.resource() == rhs.resource();
  }
  return result;
}

/*!
 * \brief     Checks if two PolymorphicAllocator are not equal.
 * \details   They are equal if their underlying memory resources compare equal.
 * \tparam    T1  Element type of the first PolymorphicAllocator.
 * \tparam    T2  Element type of the second PolymorphicAllocator.
 * \param[in] lhs  The first PolymorphicAllocator.
 * \param[in] rhs  The second PolymorphicAllocator.
 * \return    True if the underlying memory resources of the two allocators are not equal, false otherwise.
 * \steady    TRUE
 */
template <typename T1, typename T2>
auto operator!=(PolymorphicAllocator<T1> const& lhs, PolymorphicAllocator<T2> const& rhs) noexcept -> bool {
  return !(lhs == rhs);
}

namespace internal {

/*!
 * \brief     Adjusts the size of a memory block such that consecutive blocks are aligned with \p alignment.
 * \details   The new size is calculated as (size + alignment - 1) & ~(alignment - 1). As alignment is a power of two
 *            (2^n), there is only the (n + 1)th bit set, e.g. the 4th bit for alignment of 8 = 2^3. (alignment - 1) has
 *            the first n bits set, e.g. the 1st till 3rd bit are set for alignment of 8. Then, ~(alignment - 1)
 *            inverses the bits, e.g. all bits starting from the (n + 1)th bit are set. Thus, combining this with & with
 *            some size, the first n bits of size are set to zero. To avoid that the size is reduced, (alignment - 1)
 *            has to be added to size before removing the first n bits.
 * \param[in] size       The size of the memory block in bytes.
 * \param[in] alignment  The alignment. Must be a power of two.
 * \return    The adjusted size in bytes.
 * \vprivate  Component Private
 * \steady    TRUE
 */
static inline auto AlignSize(std::size_t size, std::size_t alignment) noexcept -> std::size_t {
  return (size + (alignment - 1)) & ~(alignment - 1);
}

/*!
 * \brief    A memory resource that doesn't perform any allocation
 * \trace    CREQ-VaCommonLib-NullMemoryResource
 * \vprivate Component Private
 */
// VCA_VAC_POSSIBLY_FREEING_STATIC_MEMORY
class NullMemoryResource : public MemoryResource {
 protected:
  /*!
   * \brief  Doesn't perform any allocation and always returns nullptr .
   * \return Returns nullptr.
   * \steady TRUE
   */
  auto DoAllocate(std::size_t, std::size_t) noexcept -> void* final;

  /*!
   * \brief  Has no effect as no memory exists to deallocate.
   * \steady TRUE
   */
  void DoDeallocate(void*, std::size_t, std::size_t) noexcept final;

  /*!
   * \brief     Checks if \p other equals \p this.
   * \param[in] other  The other memory resource.
   * \return    True if \p other equals this object, false otherwise.
   * \steady    TRUE
   */
  auto DoIsEqual(MemoryResource const& other) const noexcept -> bool final;
};

/*!
 * \brief   A memory resource which allocates on the heap.
 * \trace   CREQ-VaCommonLib-NewDeleteResource
 * \vpublic
 */
// VCA_VAC_POSSIBLY_FREEING_STATIC_MEMORY
class NewDeleteResource : public MemoryResource {
 protected:
  /*!
   * \brief     Allocates the requested number of bytes on the heap using the platform's default alignment.
   * \param[in] bytes      The number of bytes to allocate.
   * \param[in] alignment  This parameter is not used.
   * \return    A pointer to the allocated memory or nullptr if no memory could be allocated.
   * \steady    FALSE
   */
  auto DoAllocate(std::size_t bytes, std::size_t alignment) noexcept -> void* override;

  /*!
   * \brief     Deallocates memory which was allocated by \a DoAllocate().
   * \param[in] p          A pointer to memory. Must have been allocated using \a DoAllocate().
   * \param[in] bytes      This parameter is not used.
   * \param[in] alignment  This parameter is not used.
   * \steady    FALSE
   */
  void DoDeallocate(void* p, std::size_t bytes, std::size_t alignment) noexcept override;

  /*!
   * \brief     Checks if \p other equals \p this.
   * \details   They are considered equal if they refer to the same object, that is, if the addresses of the objects are
   *            equal.
   * \param[in] other  The other memory resource.
   * \return    True if \p other equals this object, false otherwise.
   * \steady    TRUE
   */
  auto DoIsEqual(MemoryResource const& other) const noexcept -> bool override;
};

}  // namespace internal

/*!
 * \brief   Returns the NullMemoryResource instance.
 * \return  A pointer to the NullMemoryResource.
 * \vpublic
 * \steady  TRUE
 */
auto NullMemoryResource() noexcept -> MemoryResource*;

/*!
 * \brief   Returns the NewDeleteResource instance.
 * \return  A pointer to the NewDeleteResource.
 * \vpublic
 * \steady  TRUE
 */
auto NewDeleteResource() noexcept -> MemoryResource*;

/*!
 * \brief   Returns the NewDeleteResource instance wrapped in an AbortOnErrorResource.
 * \return  A pointer to the resource.
 * \vpublic
 * \steady  TRUE
 */
auto AbortingNewDeleteResource() noexcept -> MemoryResource*;

/*!
 * \brief   A memory resource which allocates memory using an upstream memory resource. Deallocation requests are only
 *          performed when this object is destroyed (or explicitly by calling release()).
 * \details A MonotonicBufferResource maintains the list of chunks allocated by the upstream memory resource. When
 *          DoAllocate() is called, it checks if the last allocated chunk has enough space for the requested memory.
 *          Otherwise, a new chunk is requested from the upstream memory resource.
 * \trace   CREQ-VaCommonLib-MonotonicBufferResource
 * \vpublic
 */
class MonotonicBufferResource : public MemoryResource {
 public:
  /*!
   * \brief   Default constructor. Uses the result of GetDefaultResource() as upstream memory resource.
   * \vpublic
   */
  MonotonicBufferResource() noexcept;

  /*!
   * \brief     Constructs a MonotonicBufferResource with the provided upstream memory resource.
   * \param[in] upstream_memory_resource  Pointer to a valid memory resource.
   * \vpublic
   */
  explicit MonotonicBufferResource(MemoryResource* upstream_memory_resource) noexcept;

  /*!
   * \brief     Constructs a MonotonicBufferResource with the provided initial buffer size and the result of
   *            GetDefaultResource() as upstream memory resource.
   * \param[in] initial_size  Size in bytes of the first memory chunk to be allocated. Must not be 0.
   * \vpublic
   */
  explicit MonotonicBufferResource(std::size_t initial_size) noexcept;

  /*!
   * \brief     Constructs a MonotonicBufferResource with the provided initial buffer size and the provided upstream
   *            memory resource.
   * \param[in] initial_size              Size in bytes of the first memory chunk to be allocated. Must not be 0.
   * \param[in] upstream_memory_resource  Pointer to a valid memory resource.
   * \vpublic
   */
  MonotonicBufferResource(std::size_t initial_size, MemoryResource* upstream_memory_resource) noexcept;

  /*!
   * \brief     Constructs a MonotonicBufferResource with the provided initial buffer and the result of
   *            GetDefaultResource() as upstream memory resource.
   * \param[in] buffer       Pointer to allocated memory to use as the first memory chunk.
   * \param[in] buffer_size  The size in bytes of \p buffer.
   * \vpublic
   */
  MonotonicBufferResource(void* buffer, std::size_t buffer_size) noexcept;

  /*!
   * \brief     Constructs a MonotonicBufferResource with the provided initial buffer and the provided upstream memory
   *            resource.
   * \param[in] buffer                    Pointer to allocated memory to use as the first memory chunk.
   * \param[in] buffer_size               The size in bytes of \p buffer.
   * \param[in] upstream_memory_resource  Pointer to a valid memory resource.
   * \vpublic
   */
  MonotonicBufferResource(void* buffer, std::size_t buffer_size, MemoryResource* upstream_memory_resource) noexcept;

  MonotonicBufferResource(MonotonicBufferResource const&) noexcept = delete;

  /*!
   * \brief     Move constructor.
   * \details   The upstream resource of \p other will be invalidated after the move.
   * \param[in] other  The value to be moved.
   * \pre       -
   */
  MonotonicBufferResource(MonotonicBufferResource&& other) noexcept;

  /*!
   * \brief   Destructor. Deallocates all allocated memory chunks owned by this MonotonicBufferResource.
   * \details If this MonotonicBufferResource was constructed with an initial buffer, this buffer is not deallocated.
   * \vpublic
   */
  ~MonotonicBufferResource() noexcept override;

  auto operator=(MonotonicBufferResource const&) noexcept -> MonotonicBufferResource& = delete;

  /*!
   * \brief     Move-assignment operator.
   * \details   The upstream resource of \p other will be invalidated after the move.
   * \param[in] other  The value to be move assigned.
   * \return    *this, containing the contents of other.
   * \pre       -
   */
  auto operator=(MonotonicBufferResource&& other) & noexcept -> MonotonicBufferResource&;

  /*!
   * \brief   Deallocates all allocated memory chunks owned by this MonotonicBufferResource.
   * \details If this MonotonicBufferResource was constructed with an initial buffer, this buffer is not deallocated.
   * \vpublic
   * \steady  TRUE
   */
  void release() noexcept;

  /*!
   * \brief   Returns the upstream memory resource.
   * \return  A pointer to the upstream memory resource.
   * \vpublic
   * \steady  TRUE
   */
  auto upstream_resource() const noexcept -> MemoryResource*;

  /*!
   * \brief   Returns the amount of allocated memory.
   * \return  The capacity in bytes.
   * \vpublic
   * \steady  TRUE
   */
  auto Capacity() const noexcept -> std::size_t;

  /*!
   * \brief   Returns the amount of used memory.
   * \return  The size in bytes.
   * \vpublic
   * \steady  TRUE
   */
  auto Size() const noexcept -> std::size_t;

 protected:
  /*!
   * \brief     Allocates the requested number of bytes.
   * \details   A MonotonicBufferResource maintains a list of allocated memory chunks. If the last one has enough space
   *            to fulfill this allocation request, it is used. That is, a pointer into this memory chunk is returned.
   *            If it does not have enough space, a new large enough chunk of memory is requested from the upstream
   *            memory resource and returned.
   * \param[in] bytes      The number of bytes to allocate.
   * \param[in] alignment  The alignment of the memory to allocate.
   * \return    A pointer to the allocated memory.
   * \steady    TRUE
   */
  auto DoAllocate(std::size_t bytes, std::size_t alignment) noexcept -> void* override;

  /*!
   * \brief   This method does nothing.
   * \details The memory allocated through a MonotonicBufferResource can only be deallocated all at once by calling
   *          release() or the destructor.
   * \vpublic
   * \steady  TRUE
   */
  void DoDeallocate(void*, std::size_t, std::size_t) noexcept override;

  /*!
   * \brief     Checks if \p other equals \p this.
   * \details   They are considered equal if they refer to the same object, that is, if the addresses of the objects are
   *            equal.
   * \param[in] other  The other memory resource.
   * \return    True if \p other equals this object, false otherwise.
   * \vpublic
   * \steady    TRUE
   */
  auto DoIsEqual(MemoryResource const& other) const noexcept -> bool override;

 private:
  /*!
   * \brief Represents a chunk of memory which was either allocated through the upstream memory resource or provided by
   *        the constructor of MonotonicBufferResource.
   * \note  One such chunk does not necessarily correspond to a buffer returned by DoAllocate(). In fact, a chunk can
   *        hold multiple of these buffers.
   */
  class Chunk {
   public:
    /*!
     * \brief         Allocate memory from the upstream memory resource.
     * \param[in]     upstream_resource  The upstream memory resource.
     * \param[in]     bytes              The number of bytes to allocate. Must be smaller than the maximum value of
     *                                   std::size_t minus aligned sizeof(Chunk).
     * \param[in]     alignment          The alignment of the memory to allocate.
     * \param[in,out] head               Pointer to the chunk added last. Is set to the newly allocated chunk.
     * \return        Pointer to the allocated memory and size of the allocated memory in bytes.
     * \pre           -
     * \context       ANY
     * \steady        TRUE
     */
    static auto Allocate(MemoryResource* upstream_resource, std::size_t bytes, std::size_t alignment,
                         Chunk*& head) noexcept -> std::pair<void*, std::size_t>;

    /*!
     * \brief         Releases all memory chunks to the upstream resource.
     * \param[in]     upstream_resource  The upstream memory resource.
     * \param[in,out] head               Pointer to the chunk added last. Is reset to \c nullptr after this call.
     * \pre           -
     * \context       ANY
     * \steady        TRUE
     */
    static void Release(MemoryResource* upstream_resource, Chunk*& head) noexcept;

    /*!
     * \brief     Summarizes the capacity of all memory chunks.
     * \param[in] head  Pointer to the chunk added last.
     * \return    The summarized capacity of all memory chunks in bytes.
     * \pre       -
     * \context   ANY
     * \steady    TRUE
     */
    static auto TotalCapacity(Chunk const* head) noexcept -> std::size_t;

   private:
    /*!
     * \brief     Constructs a new Chunk.
     * \param[in] size       Size in bytes.
     * \param[in] alignment  The alignment of the memory.
     * \param[in] next       Pointer to the next chunk.
     * \pre       -
     * \context   ANY
     */
    Chunk(std::size_t size, std::size_t alignment, Chunk* next) noexcept
        : size_{size},            // VCA_VAC_VALID_ACCESS_RUN_TIME_CHECK
          alignment_{alignment},  // VCA_VAC_VALID_ACCESS_RUN_TIME_CHECK
          next_{next} {}          // VCA_VAC_VALID_ACCESS_RUN_TIME_CHECK, VCA_VAC_VALID_CONSTRUCTOR_CALLED

    /*!
     * \brief Size in bytes.
     */
    std::size_t size_;

    /*!
     * \brief Alignment of the memory.
     */
    std::size_t alignment_;

    /*!
     * \brief   Start address of the next memory chunk.
     * \details If there is no next chunk, this is \c nullptr.
     */
    Chunk* next_;
  };

  /*! \brief The default size of the first memory chunk. */
  static constexpr std::size_t kInitSize{128U * sizeof(void*)};

  /*!
   * \brief The factor by which subsequently allocated chunks grow at least.
   */
  static constexpr std::uint8_t kGrowthFactor{2U};

  /*!
   * \brief The memory resource which will be used for memory allocations.
   */
  MemoryResource* upstream_memory_resource_;

  /*!
   * \brief The minimum size of the next chunk to get from the upstream memory resource.
   */
  std::size_t next_chunk_size_;

  /*!
   * \brief Pointer to the first chunk of allocated memory. If a memory buffer was provided by the constructor, it is
   *        the first one in this list.
   */
  Chunk* chunks_head_{nullptr};

  /*!
   * \brief Amount of free space in the current buffer, which can be used for the next allocation request.
   */
  std::size_t free_bytes_in_current_buffer_;

  /*!
   * \brief Buffer provided by the constructor.
   */
  void* original_buffer_{nullptr};

  /*!
   * \brief The size of the buffer provided to the constructor. 0 if no buffer was provided.
   */
  std::size_t original_buffer_size_;

  /*!
   * \brief Current buffer.
   */
  void* current_buffer_{nullptr};

  /*!
   * \brief The size of memory currently in use.
   */
  std::size_t used_size_;

  /*!
   * \brief     Computes the (increased) size of the chunk to allocate next.
   * \param[in] current_chunk_size  Size of the current chunk.
   * \return    The current chunk size multiplied with the growth factor, but at least kInitSize.
   * \steady    TRUE
   */
  static auto ComputeNextChunkSize(std::size_t const current_chunk_size) noexcept -> std::size_t;

  /*!
   * \brief     Tries to allocate a new chunk using the upstream memory resource and adds it to the chunk list.
   * \param[in] bytes      The number of bytes.
   * \param[in] alignment  The requested alignment.
   * \return    True if allocation was successful, false otherwise.
   * \steady    TRUE
   */
  auto TryAllocateNewChunk(std::size_t const bytes, std::size_t const alignment) noexcept -> bool;
};

}  // namespace core
}  // namespace ara

#endif  // LIB_VAC_INCLUDE_ARA_CORE_MEMORY_RESOURCE_H_
