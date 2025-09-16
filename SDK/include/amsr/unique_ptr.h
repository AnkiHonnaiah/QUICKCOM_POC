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
 *        \brief  A helper constructor for a unique pointer with custom allocator.
 *         \unit  VaCommonLib::MemoryManagement::UniquePtr
 *
 *********************************************************************************************************************/

#ifndef LIB_VAC_INCLUDE_AMSR_UNIQUE_PTR_H_
#define LIB_VAC_INCLUDE_AMSR_UNIQUE_PTR_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include "amsr/generic/generic_error_domain.h"
#include "ara/core/memory_resource.h"
#include "ara/core/result.h"

namespace amsr {

/*!
 * \brief           Deleter class that ensures objects are deallocated correctly
 * \vpublic
 * \tparam          T
 *                  The type of the object to delete.
 * \tparam          Alloc
 *                  is an allocator which fulfills C++ Named Requirements: Allocator. The default type of Alloc is
 *                  ara::core::PolymorphicAllocator<T>.
 */
template <typename T, typename Alloc = ara::core::PolymorphicAllocator<T>>
class Deleter final {
 public:
  /*!
   * \brief           Default constructs a Deleter
   * \vprivate        Component Private
   * \context         ANY
   * \pre             -
   */
  // VECTOR NL AutosarC++17_10-A12.1.5: MD_VAC_A12.1.5_useDelegatingConstructor
  Deleter() noexcept : n_{1} {}

  /*!
   * \brief           Constructs a Deleter
   * \vprivate        Component Private
   * \param[in]       allocator
   *                  The allocator to use for destruction and deallocation.
   * \param[in]       n
   *                  The amsr::UniquePtr array size.
   * \context         ANY
   * \pre             -
   */
  // VECTOR NC AutosarC++17_10-A12.1.5: MD_VAC_A12.1.5_useDelegatingConstructor
  // VCA_VAC_VALID_ARGUMENT
  explicit Deleter(Alloc allocator, std::size_t n = 1) noexcept : allocator_{allocator}, n_{n} {}

  /*!
   * \brief           Constructs a Deleter from a deleter with a different object and allocator type
   * \vprivate        Component Private
   * \tparam          U
   *                  Type of the others' object to delete.
   * \tparam          A
   *                  Type of the others' allocator.
   * \param[in]       other
   *                  The other deleter.
   * \context         ANY
   * \pre             -
   */
  // VECTOR NC VectorC++-V12.1.4: MD_VAC_V12.1.4_ExplicitConstructor
  template <typename U, typename A>
  Deleter(Deleter<U, A> other) noexcept : allocator_{other.allocator_}, n_{other.n_} {}  // VCA_VAC_VALID_ARGUMENT

  /*!
   * \brief           Copy constructs a Deleter
   * \vprivate        Component Private
   * \param[in]       other
   *                  The deleter to copy.
   * \context         ANY
   * \pre             -
   */
  Deleter(Deleter const& other) noexcept = default;

  /*!
   * \brief           Move constructs a Deleter
   * \vprivate        Component Private
   * \param[in]       other
   *                  The deleter to move.
   * \context         ANY
   * \pre             -
   */
  Deleter(Deleter&& other) noexcept = default;

  /*!
   * \brief           Destructor
   * \vprivate        Component Private
   * \context         ANY
   * \pre             -
   */
  ~Deleter() noexcept = default;

  /*!
   * \brief           Copy-assignment operator
   * \vprivate        Component Private
   * \return          A reference to the object.
   * \context         ANY
   * \pre             -
   */
  auto operator=(Deleter const&) & noexcept -> Deleter& = default;

  /*!
   * \brief           Move-assignment operator
   * \vprivate        Component Private
   * \return          A reference to the object.
   * \context         ANY
   * \pre             -
   */
  auto operator=(Deleter&&) & noexcept -> Deleter& = default;

  // VECTOR Next Construct AutosarC++17_10-M0.3.1: MD_VAC_M0.3.1_castMayTruncateValue
  // VECTOR Next Construct AutosarC++17_10-A4.7.1: MD_VAC_A4.7.1_castMayTruncateValue
  /*!
   * \brief           Callable operator. Destroys the object when UniquePtr's scope ends
   * \vprivate        Component Private
   * \tparam          U
   *                  Type of the object to destroy.
   * \param[in]       u
   *                  The object to destroy.
   * \context         ANY
   * \pre             -
   */
  template <typename U>
  void operator()(U* u) noexcept {
    for (std::size_t i{0}; i < n_; i++) {
      std::allocator_traits<Alloc>::destroy(  // VCA_VAC_STD_FUNC_FULFILL_CONTRACT
          allocator_, std::next(u, static_cast<std::ptrdiff_t>(i)));
    }
    std::allocator_traits<Alloc>::deallocate(allocator_, u, n_);  // VCA_VAC_STD_FUNC_FULFILL_CONTRACT
  }

 private:
  /*!
   * \brief           Friend class declaration
   * \tparam          U
   *                  The type of the object to be deleted.
   * \tparam          A
   *                  An allocator which fulfills C++ Named Requirements: Allocator.
   */
  // VECTOR NL AutosarC++17_10-A11.3.1: MD_VAC_A11.3.1_doNotUseFriendConversion
  template <typename U, typename A>
  friend class Deleter;

  /*! \brief          Allocator instance */
  Alloc allocator_{};

  /*! \brief          Array size */
  std::size_t n_;
};

/*!
 * \brief           Alias for std::unique_ptr allowing custom deleter and allocator
 * \vpublic
 * \tparam          T
 *                  Type of the managed object.
 * \tparam          D
 *                  Custom deleter. Defaults to amsr::Deleter<T, ara::core::PolymorphicAllocator<T>.
 * \trace           CREQ-VaCommonLib-UniquePtr
 */
template <typename T, typename D = Deleter<T, ara::core::PolymorphicAllocator<T>>>
using UniquePtr = std::unique_ptr<T, D>;

/*!
 * \brief           Helper constructor for std::unique_ptr
 * \vpublic
 * \tparam          T
 *                  Type of the managed object.
 * \tparam          Alloc
 *                  An allocator which fulfills C++ Named Requirements: Allocator. The default type of Alloc is
 *                  ara::core::PolymorphicAllocator<T>.
 * \tparam          Args
 *                  The types needed to construct one T.
 * \param[in]       alloc
 *                  The allocator.
 * \param[in]       args
 *                  The arguments needed to construct an object of type T.
 * \return          A result containing either the unique pointer or an error code, if allocation of memory for the
 *                  managed object fails.
 * \error           amsr::generic::GenErrc::kRuntimeResourceOutOf
 *                  if allocating memory for the object fails.
 * \context         ANY
 * \pre             -
 * \trace           CREQ-VaCommonLib-UniquePtr
 * \spec            requires true; \endspec
 */
template <typename T, typename Alloc = ara::core::PolymorphicAllocator<T>, typename... Args>
auto AllocateUnique(Alloc alloc, Args&&... args) noexcept -> ara::core::Result<UniquePtr<T, Deleter<T, Alloc>>> {
  ara::core::Result<UniquePtr<T, Deleter<T, Alloc>>> result{amsr::generic::GenErrc::kRuntimeResourceOutOf};
  T* const p{std::allocator_traits<Alloc>::allocate(alloc, 1)};  // VCA_VAC_VALID_ARGUMENT
  if (p != nullptr) {
    std::allocator_traits<Alloc>::construct(alloc, p, std::forward<Args>(args)...);
    result.EmplaceValue(UniquePtr<T, Deleter<T, Alloc>>{p, Deleter<T, Alloc>{alloc}});
  }
  return result;
}

// VECTOR Next Construct VectorC++-V3.0.1: MD_VAC_V3-0-1_cStyleArraysShouldNotBeUsed
// VECTOR Next Construct AutosarC++17_10-A18.1.1: MD_VAC_A18.1.1_cStyleArraysShouldNotBeUsed
/*!
 * \brief           Helper constructor assists in enabling std::unique_ptr for array types
 * \vpublic
 * \tparam          T
 *                  Type of the managed object.
 * \tparam          Alloc
 *                  An allocator which fulfills C++ Named Requirements: Allocator. The default type of Alloc is
 *                  amsr::PolymorphicAllocator<T>.
 * \param[in]       alloc
 *                  The allocator.
 * \param[in]       n
 *                  The size of array.
 * \return          A result containing either the unique pointer or an error code, if allocation of memory for the
 *                  managed object fails.
 * \error           amsr::generic::GenErrc::kRuntimeResourceOutOf
 *                  if allocating memory for the object fails.
 * \context         ANY
 * \pre             -
 * \trace           CREQ-VaCommonLib-UniquePtr
 * \spec            requires true; \endspec
 */
template <typename T, typename Alloc = ara::core::PolymorphicAllocator<std::remove_cv_t<std::remove_all_extents_t<T>>>>
auto AllocateUniqueArray(Alloc alloc, std::size_t n) noexcept -> ara::core::Result<UniquePtr<T[], Deleter<T, Alloc>>> {
  ara::core::Result<UniquePtr<T[], Deleter<T, Alloc>>> result{amsr::generic::GenErrc::kRuntimeResourceOutOf};
  T* const p{std::allocator_traits<Alloc>::allocate(alloc, n)};  // VCA_VAC_VALID_ARGUMENT
  if (p != nullptr) {
    for (std::size_t i{0}; i < n; i++) {
      std::allocator_traits<Alloc>::construct(  // VCA_VAC_STD_FUNC_FULFILL_CONTRACT
          alloc, std::next(p, static_cast<std::ptrdiff_t>(i)));
    }
    result.EmplaceValue(UniquePtr<T[], Deleter<T, Alloc>>{p, Deleter<T, Alloc>{alloc, n}});
  }
  return result;
}

/*!
 * \brief           Helper constructor for std::unique_ptr
 * \vpublic
 * \tparam          T
 *                  Type of the managed object.
 * \param[in]       args
 *                  The arguments needed to construct an object of type T.
 * \return          A result containing either the unique pointer or an error code, if allocation of memory for the
 *                  managed object fails.
 * \error           amsr::generic::GenErrc::kRuntimeResourceOutOf
 *                  if allocating memory for the object fails.
 * \context         ANY
 * \pre             -
 * \trace           CREQ-VaCommonLib-UniquePtr
 * \spec            requires true; \endspec
 */
template <typename T, typename... Args>
auto MakeUnique(Args&&... args) noexcept -> ara::core::Result<UniquePtr<T, Deleter<T>>> {
  using Alloc = typename ara::core::PolymorphicAllocator<T>;
  // VCA_VAC_VALID_ARGUMENT
  return AllocateUnique<T, Alloc>(Alloc{}, std::forward<Args>(args)...);
}

/*!
 * \brief           Helper constructor assists in enabling std::unique_ptr for array types
 * \vpublic
 * \tparam          T
 *                  Type of the managed object.
 * \param[in]       n
 *                  The size of array.
 * \return          A result containing either the unique pointer or an error code, if allocation of memory for the
 *                  managed object fails.
 * \error           amsr::generic::GenErrc::kRuntimeResourceOutOf
 *                  if allocating memory for the object fails.
 * \context         ANY
 * \pre             -
 * \trace           CREQ-VaCommonLib-UniquePtr
 * \spec            requires true; \endspec
 */
template <typename T>
auto MakeUniqueArray(std::size_t n) noexcept -> ara::core::Result<UniquePtr<T[], Deleter<T>>> {
  using Alloc = typename ara::core::PolymorphicAllocator<std::remove_cv_t<std::remove_all_extents_t<T>>>;
  // VCA_VAC_VALID_ARGUMENT
  return AllocateUniqueArray<T>(Alloc{}, n);
}

}  // namespace amsr

#endif  // LIB_VAC_INCLUDE_AMSR_UNIQUE_PTR_H_
