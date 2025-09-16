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
/*!        \file  non_zeroing_allocator.h
 *        \brief  Non-Zeroing Allocation
 *         \unit  VaCommonLib::MemoryManagement::NonZeroingAllocator
 *
 *********************************************************************************************************************/

#ifndef LIB_VAC_INCLUDE_VAC_NON_ZEROING_ALLOCATOR_H_
#define LIB_VAC_INCLUDE_VAC_NON_ZEROING_ALLOCATOR_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>

namespace vac {
namespace memory {

// VECTOR Next Construct AutosarC++17_10-A12.4.1: MD_VAC_A12.4.1_noVirtualDtor
/*!
 * \brief    An allocator that does not perform zero initialization during construction.
 * \details  NonZeroingAllocator is an allocator that overrides Alloc::construct() and Alloc::destroy() so it
 *           creates new objects with non zero-initialization when possible. If T is both trivially constructible and
 *           trivially destructible then the T constructor and destructor will no be called otherwise they
 *           will be called.
 * \tparam   T The type of the object to allocate memory for. Must be cv-unqualified object type.
 * \tparam   Alloc The allocator type which fulfill C++ Named Requirements: Allocator.
 *           The default of Alloc is std::allocator<T>.
 *
 * \trace    CREQ-VaCommonLib-NonZeroingAllocator
 * \vprivate Product Private
 */
template <typename T, typename Alloc = std::allocator<T>>
class NonZeroingAllocator : public Alloc {
  /*!
   * \brief  An alias for std::allocator_traits<Alloc>.
   */
  using alloc_traits = std::allocator_traits<Alloc>;

 public:
  /*!
   * \brief  Struct provides a way to obtain an allocator for a different type.
   * \tparam U The type of the object to be constructed in the allocated memory. Must be cv-unqualified object type.
   */
  template <typename U>
  struct rebind {
    /*!
     * \brief  Alias for a NonZeroingAllocator allocator with a different type.
     */
    using other = NonZeroingAllocator<U, typename alloc_traits::template rebind_alloc<U>>;
  };

  using Alloc::Alloc;

  // VECTOR Next Construct AutosarC++17_10-A10.2.1: MD_VAC_A10.2.1_redefinitionOfNonVirtualFunction
  // VECTOR Next Construct AutosarC++17_10-M0.1.8: MD_VAC_M0.1.8_voidFunctionHasNoExternalSideEffect
  /*!
   * \brief   Constructs an object of type U in allocated storage.
   * \tparam  U The type of the object to be constructed in the allocated memory.
   *          - Must be cv-unqualified object type.
   *          - U can not be trivially constructible nor trivially destructible.
   * \param   ptr pointer to allocated uninitialized storage.
   * \pre     ptr must not be a nullptr and must be valid.
   * \spec
   *   requires ptr != nullptr;
   * \endspec
   */
  template <typename U, std::enable_if_t<!(std::is_trivially_default_constructible<U>::value &&
                                           std::is_trivially_destructible<U>::value),
                                         std::int8_t> = 0>
  static void construct(U* ptr) noexcept {
    ::new (static_cast<void*>(ptr)) U;  // VCA_VAC_VALID_CONSTRUCTOR_CALLED
  }

  // VECTOR Next Construct AutosarC++17_10-M0.1.8: MD_VAC_M0.1.8_emptyVoidFunctionHasNoExternalSideEffect
  /*!
   * \brief   This overload will not call the constructor.
   * \tparam  U The type of the object to not be constructed in the allocated memory.
   *          - Must be cv-unqualified object type.
   *          - U must be both trivially constructible and trivially destructible.
   */
  template <typename U, std::enable_if_t<(std::is_trivially_default_constructible<U>::value &&
                                          std::is_trivially_destructible<U>::value),
                                         std::int8_t> = 0>
  static void construct(U*) noexcept {
    //   The constructor is not called.
  }

  // VECTOR Next Construct AutosarC++17_10-A10.2.1: MD_VAC_A10.2.1_redefinitionOfNonVirtualFunction
  /*!
   * \brief  Constructs an object of type U in the allocated storage.
   * \tparam U The type of the object to be constructed in the allocated memory. Must be cv-unqualified object type.
   * \tparam Args The types needed to construct one U.
   * \param  ptr pointer to allocated uninitialized storage.
   * \param  args the constructor arguments to use.
   * \pre    ptr must not be a nullptr and must be valid.
   * \spec
   *   requires ptr != nullptr;
   * \endspec
   */
  template <typename U, typename... Args>
  void construct(U* ptr, Args&&... args) noexcept {
    alloc_traits::construct(static_cast<Alloc&>(*this), ptr, std::forward<Args>(args)...);  // VCA_VAC_VALID_ARGUMENT
  }

  /*!
   * \brief   Calls the destructor of the object pointed to by p.
   * \tparam  U The type of the object that the memory was allocated for.
   *          - Must be cv-unqualified object type.
   *          - U not must be both trivially constructible and trivially destructible.
   * \param   ptr pointer to the object that is going to be destroyed.
   * \pre     ptr must not be a nullptr and must be valid.
   * \spec
   *   requires ptr != nullptr;
   * \endspec
   */
  template <typename U, std::enable_if_t<!(std::is_trivially_default_constructible<U>::value &&
                                           std::is_trivially_destructible<U>::value),
                                         std::int8_t> = 0>
  static void destroy(U* ptr) noexcept {
    ptr->~U();  // VCA_VAC_VALID_PRECONDITIONED_NONSTATIC_METHOD
  }

  // VECTOR Next Construct AutosarC++17_10-M0.1.8: MD_VAC_M0.1.8_emptyVoidFunctionHasNoExternalSideEffect
  /*!
   * \brief   This overload will do nothing since nothing was constructed, therefore nothing is destroyed.
   * \tparam  U The type of the object that the memory was allocated for.
   *          - Must be cv-unqualified object type.
   *          - U must not be both trivially constructible and trivially destructible.
   */
  template <typename U, std::enable_if_t<(std::is_trivially_default_constructible<U>::value &&
                                          std::is_trivially_destructible<U>::value),
                                         std::int8_t> = 0>
  static void destroy(U*) noexcept {
    //   construct(...) didn't call the constructor, therefore destroy(...) will not call the destructor.
  }

  /*!
   * \brief  Returns a default-constructed NonZeroingAllocator object.
   * \return A default-constructed NonZeroingAllocator object.
   * \note   This method is used through std::allocator_traits.
   * \steady TRUE
   */
  // VECTOR NL AutosarC++17_10-M9.3.3: MD_VAC_M9.3.3_methodCanBeDeclaredStatic
  auto select_on_container_copy_construction() const noexcept -> NonZeroingAllocator<T, Alloc> {
    return NonZeroingAllocator();
  }
};

}  // namespace memory
}  // namespace vac

#endif  // LIB_VAC_INCLUDE_VAC_NON_ZEROING_ALLOCATOR_H_
