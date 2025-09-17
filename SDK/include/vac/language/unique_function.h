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
/**        \file  unique_function.h
 *        \brief  A move-only equivalent of std::function.
 *         \unit  VaCommonLib::LanguageSupport::UniqueFunction
 *
 *      \details  vac::language::UniqueFunction provides a move-only equivalent of std::function.
 *                This implementation always allocates dynamic memory to store the callable.
 *                Uses virtual functions to allow UniqueFunction to call any callable type and correctly
 *                deallocate memory and destroy stored callables and allocators.
 *********************************************************************************************************************/

#ifndef LIB_VAC_INCLUDE_VAC_LANGUAGE_UNIQUE_FUNCTION_H_
#define LIB_VAC_INCLUDE_VAC_LANGUAGE_UNIQUE_FUNCTION_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <algorithm>
#include <cassert>
#include <memory>
#include <type_traits>
#include <utility>

#include "amsr/core/abort.h"
#include "amsr/core/utility.h"
#include "vac/language/allocator_arg.h"
#include "vac/language/cpp17_backport.h"
#include "vac/language/cpp20_backport.h"
#include "vac/language/detail/function_signature.h"
#include "vac/language/rebind_allocator.h"

namespace vac {
namespace language {
namespace detail {

/*!
 * \brief   The maximum size of a functor stored on the stack.
 * \details Same size requirements as std::function on stack optimization: UniqueFunction should store function
 *          pointers and reference_wrappers in the UniqueFunction memory. sizeof(type) + alignof(type) is used since
 *          this is the space required to store type in FunctionManagerNoAlloc.
 */
constexpr static std::size_t const kLocalBufferSize{
    std::max(sizeof(void (*)()) + std::alignment_of<void (*)()>::value,
             sizeof(std::reference_wrapper<char>) + std::alignment_of<std::reference_wrapper<char>>::value)};

/*!
 * \brief Static aligned storage type used as local buffer.
 */
using BufferType = typename std::aligned_storage<detail::kLocalBufferSize>::type;

namespace uniquefunction {

// VECTOR Next Construct AutosarC++17_10-A12.4.1: MD_VAC_A12.4.1_noVirtualDtor
/*!
 * \brief  Holds a pointer to dynamically, or statically, allocated storage for a callable and if necessary an
 *         allocator.
 * \tparam Signature  The function type of the callable. May be const qualified, use lvalue/rvalue reference qualifier
 *                    and/or noexcept qualifier.
 */
template <typename Signature>
class CallableStorage;

// VECTOR Next Construct AutosarC++17_10-A12.4.1: MD_VAC_A12.4.1_noVirtualDtor
/*!
 * \brief  Call operator implementation for signatures with const, noexept and ref qualifiers. Inherits from
 *         CallableStorage. Provides the call operator and callable storage for UniqueFunction.
 * \tparam Sig1      Signature of the callable type.
 * \tparam Sig2      Signature of the callable type. Is duplicate of Sig1.
 * \tparam Noexcept  Bool to determine if the signature has noexcept specifier.
 */
template <typename Sig1, typename Sig2, bool Noexcept = signature::HasNoexcept<Sig2>::value>
class StorageAndOperatorImpl;  // VCA_VAC_SAFE_DEALLOCATION

/*!
 * \brief  Specialization for one of the twelve supported combinations of const, noexcept and ref qualifiers.
 * \tparam Signature  The function type of the callable. For this specialization it shall be non qualified.
 * \tparam R          Return type of Signature.
 * \tparam Args       The types in Signature's parameter-list.
 */
template <typename Signature, typename R, typename... Args>
class StorageAndOperatorImpl<Signature, R(Args...), false> : public CallableStorage<Signature> {
 public:
  // VECTOR NC AutosarC++17_10-M9.3.3: MD_VAC_M9.3.3_methodCanBeDeclaredConstOperator
  /*!
   * \brief      Call operator.
   * \param[in]  args  Input of callable.
   * \return     Result after invoking the callable.
   * \pre        -
   * \spec       requires true; \endspec
   */
  R operator()(Args... args) noexcept {  // COV_LIBVAC_NO_EXCEPT_SPECIFIER
    this->AssertCallable();
    // VECTOR NC AutosarC++17_10-A18.9.2: MD_VAC_A18.9.2_useStdMoveIfTheValueIsARvalueReference
    // VECTOR NC AutosarC++17_10-M0.3.1: MD_VAC_M0.3.1_dynamicMemPossiblyUsedAfterReleased
    // VCA_VAC_VALID_FUNC_CALL_COMPILE_TIME_CHECK
    return static_cast<R>(this->function_manager_ptr_->Invoke(std::forward<Args>(args)...));
  }
};
/*!
 * \brief  Specialization for one of the twelve supported combinations of const, noexcept and ref qualifiers.
 * \tparam Signature  The function type of the callable. For this specialization it shall be const qualified.
 * \tparam R          Return type of Signature.
 * \tparam Args       The types in Signature's parameter-list.
 */
template <typename Signature, typename R, typename... Args>
class StorageAndOperatorImpl<Signature, R(Args...) const, false> : public CallableStorage<Signature> {
 public:
  // VECTOR Next Construct AutosarC++17_10-A18.9.2: MD_VAC_A18.9.2_useStdMoveIfTheValueIsARvalueReference
  /*!
   * \brief      Call operator.
   * \param[in]  args  Input of callable.
   * \return     Result after invoking the callable.
   * \pre        -
   * \spec       requires true; \endspec
   */
  R operator()(Args... args) const noexcept {  // COV_LIBVAC_NO_EXCEPT_SPECIFIER
    this->AssertCallable();
    // VECTOR Next Construct AutosarC++17_10-M0.3.1: MD_VAC_M0.3.1_dynamicMemPossiblyUsedAfterReleased
    // VCA_VAC_VALID_FUNC_CALL_COMPILE_TIME_CHECK
    return static_cast<R>(this->function_manager_ptr_->Invoke(std::forward<Args>(args)...));
  }
};

/*!
 * \brief  Specialization for one of the twelve supported combinations of const, noexcept and ref qualifiers.
 * \tparam Signature  The function type of the callable. For this specialization it shall be lvalue-ref qualified.
 * \tparam R          Return type of Signature.
 * \tparam Args       The types in Signature's parameter-list.
 */
template <typename Signature, typename R, typename... Args>
class StorageAndOperatorImpl<Signature, R(Args...)&, false> : public CallableStorage<Signature> {
 public:
  // VECTOR NC AutosarC++17_10-M9.3.3: MD_VAC_M9.3.3_methodCanBeDeclaredConstOperator
  // VECTOR Next Construct AutosarC++17_10-A18.9.2: MD_VAC_A18.9.2_useStdMoveIfTheValueIsARvalueReference
  /*!
   * \brief      Call operator.
   * \param[in]  args  Input of callable.
   * \return     Result after invoking the callable.
   * \pre        -
   * \spec       requires true; \endspec
   */
  R operator()(Args... args) & noexcept {  // COV_LIBVAC_NO_EXCEPT_SPECIFIER
    this->AssertCallable();
    return static_cast<R>(this->function_manager_ptr_->Invoke(std::forward<Args>(args)...));
  }
};

/*!
 * \brief  Specialization for one of the twelve supported combinations of const, noexcept and ref qualifiers.
 * \tparam Signature  The function type of the callable. For this specialization it shall be const-lvalue-ref
 *                    qualified.
 * \tparam R          Signature of the callable type.
 * \tparam Args       The types of signature parameters.
 */
template <typename Signature, typename R, typename... Args>
class StorageAndOperatorImpl<Signature, R(Args...) const&, false> : public CallableStorage<Signature> {
 public:
  // VECTOR NC AutosarC++17_10-M9.3.3: MD_VAC_M9.3.3_methodCanBeDeclaredConstOperator
  /*!
   * \brief      Call operator.
   * \param[in]  args  Input of callable.
   * \return     Result after invoking the callable.
   * \pre        -
   * \spec       requires true; \endspec
   */
  R operator()(Args... args) const& noexcept {  // COV_LIBVAC_NO_EXCEPT_SPECIFIER
    this->AssertCallable();
    // VECTOR Next Construct AutosarC++17_10-A18.9.2: MD_VAC_A18.9.2_useStdMoveIfTheValueIsARvalueReference
    // VCA_VAC_VALID_FUNC_CALL_COMPILE_TIME_CHECK
    return static_cast<R>(this->function_manager_ptr_->Invoke(std::forward<Args>(args)...));
  }
};

/*!
 * \brief  Specialization for one of the twelve supported combinations of const, noexcept and ref qualifiers.
 * \tparam Signature  The function type of the callable. For this specialization it shall be rvalue-ref qualified.
 * \tparam R          Signature of the callable type.
 * \tparam Args       The types of signature parameters.
 */
template <typename Signature, typename R, typename... Args>
class StorageAndOperatorImpl<Signature, R(Args...)&&, false> : public CallableStorage<Signature> {
 public:
  // VECTOR NC AutosarC++17_10-M9.3.3: MD_VAC_M9.3.3_methodCanBeDeclaredConstOperator
  /*!
   * \brief      Call operator.
   * \param[in]  args  Input of callable.
   * \return     Result after invoking the callable.
   * \pre        -
   * \spec       requires true; \endspec
   */
  R operator()(Args... args) && noexcept {  // COV_LIBVAC_NO_EXCEPT_SPECIFIER
    this->AssertCallable();
    // VECTOR NC AutosarC++17_10-M9.3.3: MD_VAC_M9.3.3_methodCanBeDeclaredConstOperator
    // VECTOR Next Construct AutosarC++17_10-A18.9.2: MD_VAC_A18.9.2_useStdMoveIfTheValueIsARvalueReference
    // VCA_VAC_VALID_FUNC_CALL_COMPILE_TIME_CHECK
    return static_cast<R>(this->function_manager_ptr_->Invoke(std::forward<Args>(args)...));
  }
};
/*!
 * \brief  Specialization for one of the twelve supported combinations of const, noexcept and ref qualifiers.
 * \tparam Signature  The function type of the callable. For this specialization it shall be const-rvalue-ref
 *                    qualified.
 * \tparam R          Signature of the callable type.
 * \tparam Args       The types of signature parameters.
 */
template <typename Signature, typename R, typename... Args>
class StorageAndOperatorImpl<Signature, R(Args...) const&&, false> : public CallableStorage<Signature> {
 public:
  // VECTOR NC AutosarC++17_10-M9.3.3: MD_VAC_M9.3.3_methodCanBeDeclaredConstOperator
  /*!
   * \brief      Call operator.
   * \param[in]  args  Input of callable.
   * \return     Result after invoking the callable.
   * \pre        -
   * \spec       requires true; \endspec
   */
  R operator()(Args... args) const&& noexcept {  // COV_LIBVAC_NO_EXCEPT_SPECIFIER
    this->AssertCallable();
    // VECTOR Next Construct AutosarC++17_10-A18.9.2: MD_VAC_A18.9.2_useStdMoveIfTheValueIsARvalueReference
    // VCA_VAC_VALID_FUNC_CALL_COMPILE_TIME_CHECK
    return static_cast<R>(this->function_manager_ptr_->Invoke(std::forward<Args>(args)...));
  }
};
// VECTOR NC Compiler-#552: MD_VAC_Compiler-#552_noexceptNotAllowed
/*!
 * \brief  Specialization for one of the twelve supported combinations of const, noexcept and ref qualifiers.
 * \tparam Signature  The function type of the callable. For this specialization it shall be noexcept qualified.
 * \tparam R          Signature of the callable type.
 * \tparam Args       The types of signature parameters.
 */
template <typename Signature, typename R, typename... Args>
class StorageAndOperatorImpl<Signature, R(Args...) noexcept, true> : public CallableStorage<Signature> {
 public:
  /*!
   * \brief      Call operator.
   * \param[in]  args  Input of callable.
   * \return     Result after invoking the callable.
   * \pre        -
   * \spec       requires true; \endspec
   */
  R operator()(Args... args) noexcept {
    this->AssertCallable();
    // VECTOR NC AutosarC++17_10-M0.3.1: MD_VAC_M0.3.1_dynamicMemPossiblyUsedAfterReleased
    // VCA_VAC_VALID_FUNC_CALL_COMPILE_TIME_CHECK
    return static_cast<R>(this->function_manager_ptr_->Invoke(std::forward<Args>(args)...));
  }
};
// VECTOR NC Compiler-#552: MD_VAC_Compiler-#552_noexceptNotAllowed
/*!
 * \brief  Specialization for one of the twelve supported combinations of const, noexcept and ref qualifiers.
 * \tparam Signature  The function type of the callable. For this specialization it shall be const-noexcept qualified.
 * \tparam R          Signature of the callable type.
 * \tparam Args       The types of signature parameters.
 */
template <typename Signature, typename R, typename... Args>
class StorageAndOperatorImpl<Signature, R(Args...) const noexcept, true> : public CallableStorage<Signature> {
 public:
  /*!
   * \brief      Call operator.
   * \param[in]  args  Input of callable.
   * \return     Result after invoking the callable.
   * \pre        -
   * \spec       requires true; \endspec
   */
  R operator()(Args... args) const noexcept {
    this->AssertCallable();
    // VCA_VAC_VALID_FUNC_CALL_COMPILE_TIME_CHECK
    return static_cast<R>(this->function_manager_ptr_->Invoke(std::forward<Args>(args)...));
  }
};
// VECTOR NC Compiler-#552: MD_VAC_Compiler-#552_noexceptNotAllowed
/*!
 * \brief  Specialization for one of the twelve supported combinations of const, noexcept and ref qualifiers.
 * \tparam Signature  The function type of the callable. For this specialization it shall be lvalue-ref noexcept
 *                    qualified.
 * \tparam R          Signature of the callable type.
 * \tparam Args       The types of signature parameters.
 */
template <typename Signature, typename R, typename... Args>
class StorageAndOperatorImpl<Signature, R(Args...) & noexcept, true> : public CallableStorage<Signature> {
 public:
  /*!
   * \brief      Call operator.
   * \param[in]  args  Input of callable.
   * \return     Result after invoking the callable.
   * \pre        -
   * \spec       requires true; \endspec
   */
  R operator()(Args... args) & noexcept {
    this->AssertCallable();
    // VCA_VAC_VALID_FUNC_CALL_COMPILE_TIME_CHECK
    return static_cast<R>(this->function_manager_ptr_->Invoke(std::forward<Args>(args)...));
  }
};
// VECTOR NC Compiler-#552: MD_VAC_Compiler-#552_noexceptNotAllowed
/*!
 * \brief  Specialization for one of the twelve supported combinations of const, noexcept and ref qualifiers.
 * \tparam Signature  The function type of the callable. For this specialization it shall be const-lvalue-ref noexcept
 *                    qualified.
 * \tparam R          Return type of Signature.
 * \tparam Args       The types in Signature's parameter-list.
 */
template <typename Signature, typename R, typename... Args>
class StorageAndOperatorImpl<Signature, R(Args...) const & noexcept, true> : public CallableStorage<Signature> {
 public:
  /*!
   * \brief      Call operator.
   * \param[in]  args  Input of callable.
   * \return     Result after invoking the callable.
   * \pre        -
   * \spec       requires true; \endspec
   */
  R operator()(Args... args) const& noexcept {
    this->AssertCallable();
    // VCA_VAC_VALID_FUNC_CALL_COMPILE_TIME_CHECK
    return static_cast<R>(this->function_manager_ptr_->Invoke(std::forward<Args>(args)...));
  }
};
// VECTOR NC Compiler-#552: MD_VAC_Compiler-#552_noexceptNotAllowed
/*!
 * \brief  Specialization for one of the twelve supported combinations of const, noexcept and ref qualifiers.
 * \tparam Signature  The function type of the callable. For this specialization it shall be rvalue-ref noexcept
 *                    qualified.
 * \tparam R          Return type of Signature.
 * \tparam Args       The types in Signature's parameter-list.
 */
template <typename Signature, typename R, typename... Args>
class StorageAndOperatorImpl<Signature, R(Args...) && noexcept, true> : public CallableStorage<Signature> {
 public:
  /*!
   * \brief      Call operator.
   * \param[in]  args  Input of callable.
   * \return     Result after invoking the callable.
   * \pre        -
   * \spec       requires true; \endspec
   */
  R operator()(Args... args) && noexcept {
    this->AssertCallable();
    // VCA_VAC_VALID_FUNC_CALL_COMPILE_TIME_CHECK
    return static_cast<R>(this->function_manager_ptr_->Invoke(std::forward<Args>(args)...));
  }
};
// VECTOR NC Compiler-#552: MD_VAC_Compiler-#552_noexceptNotAllowed
/*!
 * \brief  Specialization for one of the twelve supported combinations of const, noexcept and ref qualifiers.
 * \tparam Signature  The function type of the callable. For this specialization it shall be const-rvalue-ref noexcept
 *                    qualified.
 * \tparam R          Return type of Signature.
 * \tparam Args       The types in Signature's parameter-list.
 */
template <typename Signature, typename R, typename... Args>
class StorageAndOperatorImpl<Signature, R(Args...) const && noexcept, true> : public CallableStorage<Signature> {
 public:
  /*!
   * \brief      Call operator.
   * \param[in]  args  Input of callable.
   * \return     Result after invoking the callable.
   * \pre        -
   * \spec       requires true; \endspec
   */
  R operator()(Args... args) const&& noexcept {
    this->AssertCallable();
    // VCA_VAC_VALID_FUNC_CALL_COMPILE_TIME_CHECK
    return static_cast<R>(this->function_manager_ptr_->Invoke(std::forward<Args>(args)...));
  }
};

/*!
 * \brief  An alias which forwards Signature twice instead of having UniqueFunction do this.
 * \tparam Signature  The function type of the callable. May be const qualified, use lvalue/rvalue reference qualifier
 *                    and/or noexcept qualifier.
 */
template <typename Signature>
using StorageAndCallOperator = StorageAndOperatorImpl<Signature, Signature>;

/*!
 * \brief  Interface for a stored callable object. Enables virtual calls, deallocation, and destruction. Used from
 *         StorageAndCallOperator::operator() inherited by UniqueFunction.
 * \tparam R     Return type of Signature.
 * \tparam Args  The types in Signature's parameter-list.
 */
template <typename R, typename... Args>
class IFunctionManager {
 public:
  // VECTOR Next Construct AutosarC++17_10-A15.5.1: MD_VAC_A15.5.1_explicitNoexceptIfAppropriate
  /*!
   * \brief Default destructor.
   * \spec  requires true; \endspec
   */
  virtual ~IFunctionManager() noexcept = default;

  /*!
   * \brief      Invocation interface.
   * \param[in]  ...  Input to be invoked.
   * \return
   * \pre        -
   * \spec       requires true; \endspec
   */
  // VECTOR NL AutosarC++17_10-M9.3.3: MD_VAC_M9.3.3_methodCanBeDeclaredConstVirtual
  virtual R Invoke(Args...) noexcept = 0;

  /*!
   * \brief  Move this to the provided buffer of type BufferType if it is stored statically or return the pointer
   *         'this' if *this is stored dynamically.
   * \return Pointer to IFunctionManager.
   * \pre    -
   * \spec   requires true; \endspec
   */
  virtual IFunctionManager* Move(detail::BufferType*) noexcept = 0;

  /*!
   * \brief OnDelete allows the allocator to be type erased.
   * \pre   -
   * \spec  requires true; \endspec
   */
  virtual void OnDelete() noexcept = 0;

  /*!
   * \brief Default constructor.
   * \spec  requires true; \endspec
   */
  constexpr IFunctionManager() noexcept = default;

  /*!
   * \brief Deleted copy constructor.
   */
  IFunctionManager(IFunctionManager const&) = delete;

  /*!
   * \brief Deleted move constructor.
   */
  IFunctionManager(IFunctionManager&&) = delete;

  /*!
   * \brief  Deleted copy assignment.
   * \return
   */
  IFunctionManager& operator=(IFunctionManager const&) = delete;

  /*!
   * \brief  Deleted move assignment.
   * \return
   */
  IFunctionManager& operator=(IFunctionManager&&) = delete;
};

// VECTOR NL VectorC++-V2.7.6: MD_VAC_V2-7-6_cCommentShouldBeDoxygen
/*VCAST_DONT_INSTRUMENT_START*/
// VECTOR NL VectorC++-V2.7.6: MD_VAC_V2-7-6_cCommentShouldBeDoxygen
/*VCAST_INSTRUMENTATION_ISSUE_REFERENCE: TAR-24033*/

/*!
 * \brief  Stores a callable object together with an allocator. Handles deallocation of self with custom allocator.
 *         This is the storage implementation that CallableStorage actually points to.
 * \tparam Alloc          Allocator of the callable type. Must fulfill C++ named requirements: Allocator.
 * \tparam FuncT          The type of the stored function object. Must fulfill C++ named requirements: FunctionObject.
 * \tparam InvokeAsRRef   The bool type to check if FunctionManger is invoked as R value reference.
 * \tparam InvokeAsConst  The bool type to check if FunctionManger is invoked as const.
 * \tparam R              Return type of Signature.
 * \tparam Args           The types in Signature's parameter-list.
 */
template <typename Alloc, typename FuncT, bool InvokeAsRRef, bool InvokeAsConst, typename R, typename... Args>
class FunctionManager final : public IFunctionManager<R, Args...> {
 public:
  /*!
   * \brief Rebound allocator type for FuncT.
   */
  using ReboundAlloc = typename vac::language::rebindallocator::RebindAllocator<Alloc, FunctionManager>::type;

  /*! \brief Base class. */
  using IFuncMan = IFunctionManager<R, Args...>;

  // VECTOR Next Construct AutosarC++17_10-A12.1.5: MD_VAC_A12.1.5_useDelegatingConstructor
  /*!
   * \brief      Move constructor for callables.
   * \param[in]  alloc     The allocator to construct the FunctionManager.
   * \param[in]  callable  The callables to construct the FunctionManager.
   * \pre        -
   * \spec       requires true; \endspec
   */
  explicit FunctionManager(Alloc const& alloc, FuncT&& callable) noexcept
      : IFuncMan{}, func_{std::move(callable)}, alloc_{alloc} {}  // VCA_VAC_VALID_FUNC_CALL

  /*!
   * \brief      Copy constructor for callables.
   * \param[in]  alloc     The allocator to copy construct the FunctionManager.
   * \param[in]  callable  The callables to construct the FunctionManager.
   * \pre        -
   * \spec       requires true; \endspec
   */
  explicit FunctionManager(Alloc const& alloc, FuncT const& callable) noexcept
      : IFuncMan{}, func_{callable}, alloc_{alloc} {}

  /*!
   * \brief      In place constructor.
   * \tparam     CArgs  The types in Signature's parameter-list.
   * \param[in]  alloc  The allocator to construct the FunctionManager.
   * \param[in]  args   Input to construct the FunctionManager.
   * \pre        -
   * \spec       requires true; \endspec
   */
  template <typename... CArgs>
  explicit FunctionManager(Alloc const& alloc, amsr::core::in_place_t, CArgs&&... args) noexcept
      : IFuncMan{}, func_{std::forward<CArgs>(args)...}, alloc_{alloc} {}

  // VECTOR Next Construct AutosarC++17_10-A8.5.4: MD_VAC_A8.5.4_initializerlistShallBeDefinedInClasses
  // VECTOR Next Construct VectorC++-V8.5.4: MD_VAC_V8-5-4_ctorShallNotBeDefinedToAcceptStdInitializerList
  /*!
   * \brief      In place constructor for initializer list.
   * \tparam     I      The type for std::initializer_list.
   * \tparam     CArgs  The types in Signature's parameter-list.
   * \param[in]  alloc  The allocator to construct the FunctionManager.
   * \param[in]  il     Initializer list to construct the FunctionManager.
   * \param[in]  args   Input to construct the FunctionManager.
   * \pre        -
   * \spec       requires true; \endspec
   */
  template <typename I, typename... CArgs>
  explicit FunctionManager(Alloc const& alloc, amsr::core::in_place_t, std::initializer_list<I> il,
                           CArgs&&... args) noexcept
      : IFuncMan{}, func_{std::move(il), std::forward<CArgs>(args)...}, alloc_{alloc} {}

  // VECTOR Next Construct AutosarC++17_10-A15.5.1: MD_VAC_A15.5.1_explicitNoexceptIfAppropriate
  /*!
   * \brief Default destructor.
   * \spec  requires true; \endspec
   */
  ~FunctionManager() noexcept final = default;  // VCA_VAC_SAFE_DEALLOCATION

  /*!
   * \brief Deleted copy constructor.
   */
  FunctionManager(FunctionManager const&) = delete;

  /*!
   * \brief Deleted move constructor.
   */
  FunctionManager(FunctionManager&&) = delete;

  /*!
   * \brief  Deleted copy assignment.
   * \return
   */
  FunctionManager& operator=(FunctionManager const&) = delete;

  /*!
   * \brief  Deleted move assignment.
   * \return
   */
  FunctionManager& operator=(FunctionManager&&) = delete;

  /*!
   * \brief  Return a pointer to allocated memory containing *this.
   * \return Pointer to allocated memory containing *this.
   * \pre    -
   * \spec   requires true; \endspec
   */
  FunctionManager* Move(detail::BufferType*) noexcept final { return this; }

  // VECTOR Next Construct AutosarC++17_10-M0.1.8: MD_VAC_M0.1.8_functionHasNoExternalSideEffect
  // VECTOR Next Construct AutosarC++17_10-A18.9.2: MD_VAC_A18.9.2_useStdMoveIfTheValueIsARvalueReference
  /*!
   * \brief      Invokes the callable with InvQuals.
   * \param[in]  args  Invocation argument types.
   * \return     Invocation result.
   * \pre        -
   * \spec       requires true; \endspec
   */
  R Invoke(Args... args) noexcept final {
    /*!
     * \brief Callable type with conditional const and ref qualifiers.
     */
    using CallableCRef = typename AddConditionalCLRRef<FuncT, InvokeAsConst, !InvokeAsRRef, InvokeAsRRef>::type;
    // VCA_VAC_VALID_FUNC_CALL_COMPILE_TIME_CHECK, VCA_VAC_VALID_FUNC_CALL
    return static_cast<R>(static_cast<CallableCRef>(func_)(std::forward<Args>(args)...));
  }

  /*!
   * \brief OnDelete creates correct allocator to deallocate memory.
   * \pre   -
   * \spec  requires true; \endspec
   */
  void OnDelete() noexcept final {
    // Create a rebound allocator on the stack to enable deallocation after destruction of *this.
    ReboundAlloc rebound_alloc{alloc_};
    std::allocator_traits<ReboundAlloc>::destroy(rebound_alloc, this);
    std::allocator_traits<ReboundAlloc>::deallocate(rebound_alloc, this, 1);  // VCA_VAC_STD_FUNC_FULFILL_CONTRACT
  }

 private:
  /*!
   * \brief Callable.
   */
  FuncT func_;

  /*!
   * \brief Allocator.
   */
  Alloc alloc_;
};

/*!
 * \brief  Stores a callable object. This is the storage implementation that CallableStorage actually points to.
 * \tparam FuncT          The type of the stored function object. Must fulfill C++ named requirements: FunctionObject.
 * \tparam InvokeAsRRef   The bool type to check if FunctionManger is invoked as R value reference.
 * \tparam InvokeAsConst  The bool type to check if FunctionManger is invoked as const.
 * \tparam R              Return type of Signature.
 * \tparam Args           The types in Signature's parameter-list.
 */
template <typename FuncT, bool InvokeAsRRef, bool InvokeAsConst, typename R, typename... Args>
class FunctionManagerNoAlloc final : public IFunctionManager<R, Args...> {
  /*! \brief Base class. */
  using IFuncMan = IFunctionManager<R, Args...>;

 public:
  /*!
   * \brief Deleted copy constructor.
   */
  FunctionManagerNoAlloc(FunctionManagerNoAlloc const&) = delete;

  /*!
   * \brief Deleted move constructor.
   */
  FunctionManagerNoAlloc(FunctionManagerNoAlloc&&) = delete;

  /*!
   * \brief  Deleted copy assignment.
   * \return
   */
  FunctionManagerNoAlloc& operator=(FunctionManagerNoAlloc const&) = delete;

  /*!
   * \brief  Deleted move assignment.
   * \return
   */
  FunctionManagerNoAlloc& operator=(FunctionManagerNoAlloc&&) = delete;

  // VECTOR Next Construct AutosarC++17_10-A12.1.5: MD_VAC_A12.1.5_useDelegatingConstructor
  /*!
   * \brief     In place constructor.
   * \tparam    CArgs  The types used to construct FuncT.
   * \param[in] args  Input to construct the FunctionManager.
   * \pre       -
   * \spec      requires true; \endspec
   */
  template <typename... CArgs>
  explicit FunctionManagerNoAlloc(amsr::core::in_place_t, CArgs&&... args) noexcept
      : IFuncMan{}, func_{std::forward<CArgs>(args)...} {}

  // VECTOR Next Construct AutosarC++17_10-A8.5.4: MD_VAC_A8.5.4_initializerlistShallBeDefinedInClasses
  // VECTOR Next Construct VectorC++-V8.5.4: MD_VAC_V8-5-4_ctorShallNotBeDefinedToAcceptStdInitializerList
  /*!
   * \brief      In place constructor for initializer list.
   * \tparam     I      The type for std::initializer_list.
   * \tparam     CArgs  The types used to construct FuncT.
   * \param[in]  il    Initializer list to construct FuncT.
   * \param[in]  args  Input to construct the function object: func_.
   * \pre        -
   * \spec       requires true; \endspec
   */
  template <typename I, typename... CArgs>
  explicit FunctionManagerNoAlloc(amsr::core::in_place_t, std::initializer_list<I> il, CArgs&&... args) noexcept
      : IFuncMan{}, func_{std::move(il), std::forward<CArgs>(args)...} {}

  /*!
   * \brief      Move constructor for callables.
   * \param[in]  callable  The callables to construct the FunctionManager.
   * \pre        -
   * \spec       requires true; \endspec
   */
  explicit FunctionManagerNoAlloc(FuncT&& callable) noexcept
      : IFuncMan{}, func_{std::move(callable)} {}  // VCA_VAC_VALID_FUNC_CALL

  /*!
   * \brief      Copy constructor for callables.
   * \param[in] callable  The callables to construct the FunctionManager.
   * \pre        -
   * \spec       requires true; \endspec
   */
  explicit FunctionManagerNoAlloc(FuncT const& callable) noexcept : IFuncMan{}, func_{callable} {}

  /*!
   * \brief      Creates the contents of *this to the supplied storage. Calls destructor of this.
   * \param[in]  storage  Local storage to construct FunctionManager in.
   * \return     pointer to the constructed clone object.
   * \pre        -
   * \spec       requires true; \endspec
   */
  FunctionManagerNoAlloc* Move(detail::BufferType* storage) noexcept final {
    using ExactFuncMan = FunctionManagerNoAlloc<FuncT, InvokeAsRRef, InvokeAsConst, R, Args...>;
    // VECTOR Next Line AutosarC++17_10-A18.5.8: MD_VAC_A18.5.8_localPlacementNew
    ExactFuncMan* const ptr{new (storage) ExactFuncMan{std::move(func_)}};  // VCA_VAC_CHECKED_ACCESS_TO_VALID_POINTER
    this->~FunctionManagerNoAlloc();
    return ptr;
  }

  /*!
   * \brief Destructor.
   * \pre   -
   * \spec  requires true; \endspec
   */
  ~FunctionManagerNoAlloc() noexcept final = default;  // VCA_VAC_SAFE_DEALLOCATION

  // VECTOR Next Construct AutosarC++17_10-A18.9.2: MD_VAC_A18.9.2_useStdMoveIfTheValueIsARvalueReference
  /*!
   * \brief      Invokes the callable with InvQuals.
   * \param[in]  args  Invocation argument types.
   * \return     Invocation result.
   * \pre        -
   * \spec       requires true; \endspec
   */
  R Invoke(Args... args) noexcept final {
    using CallableCRef = typename AddConditionalCLRRef<FuncT, InvokeAsConst, !InvokeAsRRef, InvokeAsRRef>::type;
    // VCA_VAC_VALID_FUNC_CALL_COMPILE_TIME_CHECK, VCA_VAC_VALID_FUNC_CALL
    return static_cast<R>(static_cast<CallableCRef>(func_)(std::forward<Args>(args)...));
  }

  // VECTOR Next Construct AutosarC++17_10-M0.1.8: MD_VAC_M0.1.8_functionHasNoExternalSideEffect
  /*!
   * \brief OnDelete override. Call destructor. No dynamic memory to release.
   */
  void OnDelete() noexcept final { this->~FunctionManagerNoAlloc(); }

 private:
  /*!
   * \brief Callable.
   */
  FuncT func_;
};

// VECTOR NL VectorC++-V2.7.6: MD_VAC_V2-7-6_cCommentShouldBeDoxygen
/*VCAST_DONT_INSTRUMENT_END*/

/*!
 * \brief  For Alloc with non-void type FunctionManager with allocator is used.
 * \tparam Alloc          Allocator type. Must fulfill C++ named requirements: Allocator.
 * \tparam FuncT          The type of the stored function object. Must fulfill C++ named requirements: FunctionObject.
 * \tparam InvokeAsRRef   The bool type to check if FunctionManger is invoked as R value reference.
 * \tparam InvokeAsConst  The bool type to check if FunctionManger is invoked as const.
 * \tparam RetT           Return type of Signature.
 * \tparam Args           The types in Signature's parameter-list.
 */
template <typename Alloc, typename FuncT, bool InvokeAsRRef, bool InvokeAsConst, typename RetT, typename... Args>
class FunctionManagerStorageSelector {
 public:
  /*!
   * \brief class FunctionManager used in dynamic storage.
   */
  using type = FunctionManager<Alloc, FuncT, InvokeAsRRef, InvokeAsConst, RetT, Args...>;
};

/*!
 * \brief  Specialization for Alloc as void type. FunctionManagerNoAlloc is used.
 * \tparam FuncT          The type of the stored function object. Must fulfill C++ named requirements: FunctionObject.
 * \tparam InvokeAsRRef   The bool type to check if FunctionManger is invoked as R value reference.
 * \tparam InvokeAsConst  The bool type to check if FunctionManger is invoked as const.
 * \tparam RetT           Return type of Signature.
 * \tparam Args           The types in Signature's parameter-list.
 */
template <typename FuncT, bool InvokeAsRRef, bool InvokeAsConst, typename RetT, typename... Args>
class FunctionManagerStorageSelector<void, FuncT, InvokeAsRRef, InvokeAsConst, RetT, Args...> {
 public:
  /*!
   * \brief class FunctionManager used in static storage.
   */
  using type = FunctionManagerNoAlloc<FuncT, InvokeAsRRef, InvokeAsConst, RetT, Args...>;
};

/*!
 * \brief  Expands the argument type pack and provides the fully specified FunctionManager type.
 * \tparam Alloc     Allocator used if callable is dynamically stored. Must fulfill C++ named requirements: Allocator.
 * \tparam FuncT     The type of the stored function object. Must fulfill C++ named requirements: FunctionObject.
 * \tparam SigProps  Signature properties received from signature::GetSigProps.
 * \tparam Args      The types in Signature's parameter-list.
 */
template <typename Alloc, typename FuncT, typename SigProps, typename ArgsT>
class FunctionManagerSelectorArgExpander;

/*!
 * \brief  Expands the argument type pack and provides the fully specified FunctionManager type.
 * \tparam Alloc     Allocator used if callable is dynamically stored. Must fulfill C++ named requirements: Allocator.
 * \tparam FuncT     The type of the stored function object. Must fulfill C++ named requirements: FunctionObject.
 * \tparam SigProps  Signature properties received from signature::GetSigProps.
 * \tparam Args      The types in Signature's parameter-list.
 */
template <typename Alloc, typename FuncT, typename SigProps, typename... Args>
class FunctionManagerSelectorArgExpander<Alloc, FuncT, SigProps, signature::SignatureArgs<Args...>> {
  /*!
   * \brief The bool to check if it is invoked as rvalue reference.
   */
  static constexpr bool InvokeAsRRef{SigProps::Qualifiers::kRef == signature::SignatureRefQual::RRef};
  /*!
   * \brief The bool to check if it is invoked as a const.
   */
  static constexpr bool InvokeAsConst{SigProps::Qualifiers::kIsConst};

 public:
  /*!
   * \brief Storage type for callable: FunctionManagerNoAlloc/FunctionManager.
   */
  using type = typename FunctionManagerStorageSelector<Alloc, FuncT, InvokeAsRRef, InvokeAsConst,
                                                       typename SigProps::RetT, Args...>::type;
};

/*!
 * \brief  Provides the FunctionManager type using some indirections.
 * \tparam Alloc      Allocator used if callable is dynamically stored. Must fulfill C++ named requirements: Allocator.
 * \tparam FuncT      The type of the stored function object. Must fulfill C++ named requirements: FunctionObject.
 * \tparam Signature  The function type of the callable. May be const qualified, use lvalue/rvalue reference qualifier
 *                    and/or noexcept qualifier. For signature::GetSigProps.
 */
template <typename Alloc, typename FuncT, typename Signature>
class FunctionManagerSelector {
  /*!
   * \brief Signature properties.
   */
  using SigProps = signature::GetSigProps<Signature>;

 public:
  /*!
   * \brief Expand arguments to get correct function manager.
   */
  using type = typename FunctionManagerSelectorArgExpander<Alloc, FuncT, SigProps, typename SigProps::ArgsT>::type;
};

/*!
 * \brief  Expands the argument type pack and provides the fully specified IFunctionManager type.
 * \tparam SigProps  Signature properties received from signature::GetSigProps.
 * \tparam ArgsT     The types of signature input parameters.
 */
template <typename SigProps, typename ArgsT>
struct IFunctionManagerSelectorArgExpander;

/*!
 * \brief  Expands the argument type pack and provides the fully specified IFunctionManager type.
 * \tparam SigProps  Signature properties received from signature::GetSigProps.
 * \tparam Args      The types in Signature's parameter-list.
 */
template <typename SigProps, typename... Args>
struct IFunctionManagerSelectorArgExpander<SigProps, signature::SignatureArgs<Args...>>
    : vac::language::type_identity<IFunctionManager<typename SigProps::RetT, Args...>> {};

/*!
 * \brief  Provides the IFunctionManager type using some indirections.
 * \tparam Signature  The function type of the callable. May be const qualified, use lvalue/rvalue reference qualifier
 *                    and/or noexcept qualifier. For signature::GetSigProps.
 */
template <typename Signature>
class IFunctionManagerSelector {
  /*!
   * \brief Signature properties.
   */
  using SigProps = signature::GetSigProps<Signature>;

 public:
  /*!
   * \brief Expand arguments to get correct function manager interface.
   */
  using type = typename IFunctionManagerSelectorArgExpander<SigProps, typename SigProps::ArgsT>::type;
};

// VECTOR Next Construct VectorC++-V12.0.1: MD_VAC_V12-0-1_onlyDtorImplemented
// VECTOR Next Construct AutosarC++17_10-A12.0.1: MD_VAC_A12-0-1_onlyDtorImplemented
/*!
 * \brief  Stores a dynamically allocated callable together with an allocator in FunctionManager or a statically
 *         allocated callable without an allocator in FunctionManagerNoAlloc. Manages a pointer to an interface
 *         allowing calls, deallocation, and destruction.
 * \tparam Signature  The function type of the callable. May be const qualified, use lvalue/rvalue reference qualifier
 *                    and/or noexcept qualifier. For signature::GetSigProps.
 */
template <typename Signature>
class CallableStorage {
  /*!
   * \brief  FunctionManager for F and signature.
   * \tparam Allocator  Allocator used if callable is dynamically stored. Must fulfill C++ named requirements:
   *                    Allocator.
   * \tparam F          The type of the stored function object. Must fulfill C++ named requirements: FunctionObject.
   */
  template <typename Allocator, typename F>
  using FuncMan = typename FunctionManagerSelector<std::decay_t<Allocator>, std::decay_t<F>, Signature>::type;

  /*!
   * \brief Type for FunctionManager Interface.
   */
  using IFuncMan = typename IFunctionManagerSelector<Signature>::type;

 public:
  // VECTOR Next Construct VectorC++-V11.0.2: MD_VAC_V11-0-2_mutableMemberShallBePrivate
  /*!
   * \brief Pointer to a function manager.
   */
  IFuncMan* function_manager_ptr_{static_cast<IFuncMan*>(nullptr)};

  /*!
   * \brief Custom destructor to handle raw pointer.
   * \spec  requires true; \endspec
   */
  virtual ~CallableStorage() noexcept {  // VCA_VAC_SAFE_DEALLOCATION
    this->Clear();
  }

  /*!
   * \brief     Construct using forwarded arguments in the local_buffer.
   * \tparam    F     The type of the stored function object. Must fulfill C++ named requirements: FunctionObject.
   * \tparam    Args  Types used to construct F.
   * \param[in] args  Construction arguments.
   * \pre       -
   * \spec      requires true; \endspec
   */
  template <typename F, typename... Args>
  void ConstructNoAlloc(Args&&... args) noexcept {
    /*!
     * \brief Exact type of FuncMan.
     */
    using ExactFuncMan = FuncMan<void, F>;

    function_manager_ptr_ = new (Storage()) ExactFuncMan(std::forward<Args>(args)...);
  }

  /*!
   * \brief      Construct using allocator and forwarded arguments.
   * \tparam     F      The type of the stored function object. Must fulfill C++ named requirements: FunctionObject.
   * \tparam     Alloc  Allocator type. Must fulfill C++ named requirements: Allocator.
   * \tparam     Args   Types used to construct F.
   * \param[in]  alloc  Allocator to construct a new function manager.
   * \param[in]  args   Construction arguments.
   * \pre        -
   * \spec       requires true; \endspec
   */
  template <typename Alloc, typename F, typename... Args>
  void Construct(Alloc const& alloc, Args&&... args) noexcept {
    /*!
     * \brief Exact type of FuncMan.
     */
    using ExactFuncMan = FuncMan<Alloc, F>;
    /*!
     * \brief Allocator type for ExactFuncMan.
     */
    using ReboundAlloc = typename ExactFuncMan::ReboundAlloc;

    ReboundAlloc rebound_alloc{alloc};
    ExactFuncMan* const new_storage{
        std::allocator_traits<ReboundAlloc>::allocate(rebound_alloc, 1)};  // VCA_VAC_STD_FUNC_FULFILL_CONTRACT

    if (new_storage == nullptr) {
      amsr::core::Abort("vac::language::detail::uniquefunction::CallableStorage::Construct: Allocation failed!");
    }

    // VCA_VAC_STD_FUNC_FULFILL_CONTRACT
    std::allocator_traits<ReboundAlloc>::construct(rebound_alloc, new_storage, alloc, std::forward<Args>(args)...);

    function_manager_ptr_ = new_storage;
  }

  /*!
   * \brief Clear storage.
   * \pre   -
   * \spec  requires true; \endspec
   */
  void Clear() noexcept {
    if (function_manager_ptr_ != nullptr) {
      // VCA_VAC_VALID_FUNC_CALL_RUN_TIME_CHECK
      function_manager_ptr_->OnDelete();
      function_manager_ptr_ = nullptr;
    }
  }

  // VECTOR Next Construct AutosarC++17_10-M0.1.8: MD_VAC_M0.1.8_voidFunctionHasNoExternalSideEffect
  /*!
   * \brief Aborts if called without callable. Used from StorageAndOperatorImpl::operator().
   * \pre   -
   * \spec  requires true; \endspec
   */
  void AssertCallable() const noexcept {
    if (function_manager_ptr_ == nullptr) {
      amsr::core::Abort("UniqueFunction called without callable");
    }
  }

  /*!
   * \brief         Swaps two CallableStorage.
   * \param[in,out] other  The CallableStorage to be swapped.
   * \pre           -
   * \spec          requires true; \endspec
   */
  void Swap(CallableStorage& other) noexcept {
    // If only either function_manager_ptr_ is nullptr, clone into that function_manager_ptr_
    // and set the cloned to nullptr.
    if ((other.function_manager_ptr_ == nullptr) && (this->function_manager_ptr_ != nullptr)) {
      other.function_manager_ptr_ = this->function_manager_ptr_->Move(other.Storage());  // VCA_VAC_VALID_FUNC_CALL
      this->function_manager_ptr_ = nullptr;

    } else if ((this->function_manager_ptr_ == nullptr) && (other.function_manager_ptr_ != nullptr)) {
      this->function_manager_ptr_ = other.function_manager_ptr_->Move(this->Storage());  // VCA_VAC_VALID_FUNC_CALL
      other.function_manager_ptr_ = nullptr;

    } else if (this->function_manager_ptr_ != nullptr) {
      // If neither function_manager_ptr_ is nullptr:
      // FunctionManagers: this, other, and clone.
      // other -> clone.
      // this -> other.
      // clone -> this.
      IFuncMan* clone{nullptr};
      detail::BufferType temp_buffer{};
      assert(other.function_manager_ptr_ != nullptr);                                    // COV_MSR_INV_STATE_ASSERT
      clone = other.function_manager_ptr_->Move(std::addressof(temp_buffer));            // VCA_VAC_VALID_FUNC_CALL
      assert(this->function_manager_ptr_ != nullptr);                                    // COV_MSR_INV_STATE_ASSERT
      other.function_manager_ptr_ = this->function_manager_ptr_->Move(other.Storage());  // VCA_VAC_VALID_FUNC_CALL
      assert(clone != nullptr);                                                          // COV_MSR_INV_STATE_ASSERT
      this->function_manager_ptr_ = clone->Move(this->Storage());                        // VCA_VAC_VALID_FUNC_CALL
    } else {
      // Do nothing because both are nullptr; neither holds a callable.
    }
  }

  /*!
   * \brief  Checks if empty.
   * \return True if CallableStorage is empty, otherwise false.
   * \pre    -
   * \spec   requires true; \endspec
   */
  bool IsEmpty() const noexcept {  // Comment to force line break. See TAR-22835.
    return function_manager_ptr_ == nullptr;
  }

  /*!
   * \brief  Returns address of the local_buffer.
   * \return pointer to local_buffer.
   * \pre    -
   * \spec   requires true; \endspec
   */
  BufferType* Storage() noexcept { return std::addressof(local_buffer); }

 private:
  /*!
   * \brief Buffer used for storing callables on the stack.
   */
  detail::BufferType local_buffer{};
};
}  // namespace uniquefunction

/*!
 * \brief   Check if a callable is small enough to store on the stack.
 * \details Checks if the size required to store callable in FunctionManagerNoAlloc is lesser or equal than
 *          detail::kLocalBufferSize.
 * \tparam  F          The type of the function object. Must fulfill C++ named requirements: FunctionObject.
 * \tparam  Signature  The function type of the callable. May be const qualified, use lvalue/rvalue reference qualifier
 *                     and/or noexcept qualifier.
 */
template <typename F, typename Signature>
class IsSmallCallable {
 private:
  /*!
   * \brief Alias for the selected FunctionManager.
   */
  using FuncMan = typename uniquefunction::FunctionManagerSelector<void, std::decay_t<F>, Signature>::type;

 public:
  /*!
   * \brief bool which is True if the callable can be stored in the local buffer, false otherwise.
   */
  static constexpr bool const value{sizeof(FuncMan) <= kLocalBufferSize};
};
}  // namespace detail

/*!
 * \brief   UniqueFunction is a move-only version of std::function.
 * \details The callable is stored in dynamically allocated memory if it requires more than detail::kLocalBufferSize to
 *          store, otherwise it uses static memory. An interface allows this class which is typed only with a signature
 *          to call any callable and correctly handle allocation and destruction.
 * \tparam  Signature     The function type of the callable. May be const qualified, use lvalue/rvalue reference
 *                        qualifier and/or noexcept qualifier.
 * \tparam  DefAllocator  The allocator used if necessary. Defaulted to std::allocator<char>. DefAllocator is rebound
 *                        to FunctionManager before usage, initial type does not matter. Must fulfill C++ Named
 *                        Requirements: Allocator.
 * \trace   CREQ-VaCommonLib-UniqueFunction
 */
template <typename Signature, typename DefAllocator = std::allocator<char>>
class UniqueFunction final : private detail::uniquefunction::StorageAndCallOperator<Signature> {
  /*!
   * \brief Default allocator if no construction argument is given.
   */
  using DefAlloc = DefAllocator;

  /*!
   * \brief  Invocability requirement.
   * \tparam F  The type of the function object. Must fulfill C++ named requirements: FunctionObject.
   */
  template <typename F>
  using IsInvocableReq = detail::signature::IsInvocableForSig<std::decay_t<F>, Signature>;

  /*!
   * \brief Typedef to privately inherited superclass with storage and operator().
   */
  using StorageT = typename detail::uniquefunction::StorageAndCallOperator<Signature>;

 public:
  /*!
   * \brief Default constructor.
   * \spec  requires true; \endspec
   */
  UniqueFunction() noexcept = default;

  // VECTOR Next Construct AutosarC++17_10-A12.1.6: MD_VAC_A12.1.6_useInheritingConstructor
  /*!
   * \brief Constructor for nullptr.
   * \spec  requires true; \endspec
   */
  explicit UniqueFunction(std::nullptr_t) noexcept : detail::uniquefunction::StorageAndCallOperator<Signature>{} {}

  // VECTOR Next Construct AutosarC++17_10-A15.5.1: MD_VAC_A15.5.1_explicitNoexceptIfAppropriate
  // VECTOR Next Construct AutosarC++17_10-A12.8.4: MD_VAC_A12.8.4_moveConstructorShallNotUseCopySemantics
  /*!
   * \brief      Move constructor.
   * \param[in]  other  The UniqueFunction object to take ownership of.
   * \spec       requires true; \endspec
   */
  UniqueFunction(UniqueFunction&& other) noexcept : detail::uniquefunction::StorageAndCallOperator<Signature>{} {
    this->swap(other);
  }

  // VECTOR Next Construct AutosarC++17_10-A15.5.1: MD_VAC_A15.5.1_explicitNoexceptIfAppropriate
  // VECTOR Next Construct AutosarC++17_10-A13.3.1: MD_VAC_A13.3.1_forwardingFunctionsShallNotBeOverloaded
  // VECTOR Next Construct AutosarC++17_10-A12.8.4: MD_VAC_A12.8.4_moveConstructorShallNotUseCopySemantics
  /*!
   * \brief     Move assignment.
   * \param[in] rhs  UniqueFunction is being move assigned from.
   * \return    This UniqueFunction object.
   * \pre       -
   * \spec      requires true; \endspec
   */
  UniqueFunction& operator=(UniqueFunction&& rhs) & noexcept {
    UniqueFunction{std::move(rhs)}.swap(*this);
    return *this;
  }

  // VECTOR Next Construct AutosarC++17_10-A13.3.1: MD_VAC_A13.3.1_forwardingFunctionsShallNotBeOverloaded
  /*!
   * \brief  Assignment to nullptr.
   * \return This UniqueFunction object.
   * \pre    -
   * \spec   requires true; \endspec
   */
  UniqueFunction& operator=(std::nullptr_t) & noexcept {
    StorageT::Clear();
    return *this;
  }

  /*!
   * \brief Copy constructor deleted.
   */
  UniqueFunction(UniqueFunction const&) = delete;

  /*!
   * \brief  Copy assignment deleted.
   * \return
   */
  UniqueFunction& operator=(UniqueFunction const&) & noexcept = delete;

  // VECTOR Next Construct AutosarC++17_10-A15.5.1: MD_VAC_A15.5.1_explicitNoexceptIfAppropriate
  /*!
   * \brief Default destructor.
   * \spec  requires true; \endspec
   */
  ~UniqueFunction() noexcept final = default;  // VCA_VAC_SAFE_DEALLOCATION

  // VECTOR Next Construct VectorC++-V12.1.4: MD_VAC_V12-1-4_constructorShallBeDeclaredExplicit
  /*!
   * \brief     Construct from a callable.
   * \details   The callable must not be of the same type as *this. The return value of the callable must be
   *            convertible to R when the argument types Args are supplied.
   * \tparam    F  The type of the stored function object. Must fulfill C++ named requirements: FunctionObject.
   * \param[in] callable  Either a function object or a function pointer.
   * \pre       -
   * \spec      requires true; \endspec
   */
  template <typename F,
            typename = std::enable_if_t<(!std::is_same<vac::language::remove_cvref_t<F>, UniqueFunction>::value) &&
                                        std::is_constructible<std::decay_t<F>, F>::value && IsInvocableReq<F>::value>>
  UniqueFunction(F&& callable) noexcept : detail::uniquefunction::StorageAndCallOperator<Signature>{} {
    if (detail::IsSmallCallable<F, Signature>::value) {
      ConstructWithoutAlloc<F>(std::forward<F>(callable));
    } else {
      ConstructWithAlloc<F>(DefAlloc{}, std::forward<F>(callable));
    }
  }

  // VECTOR Next Construct VectorC++-V12.1.4: MD_VAC_V12-1-4_constructorShallBeDeclaredExplicit
  /*!
   * \brief     Construct from a callable which is a UniqueFunction.
   * \details   The callable must not be of the same type as *this. The return value of the callable must be
   *            convertible to R when the argument types Args are supplied and didn't construct anything if callable is
   *            an empty UniqueFunction.
   * \tparam    F  The UniqueFunction's signature.
   * \param[in] callable  A UniqueFunction object which can be empty.
   * \pre       -
   * \spec      requires true; \endspec
   */
  template <typename F, typename = std::enable_if_t<(!std::is_same<F, Signature>::value) &&
                                                    (IsInvocableReq<UniqueFunction<F>>::value)>>
  UniqueFunction(UniqueFunction<F>&& callable) noexcept
      : detail::uniquefunction::StorageAndCallOperator<Signature>{} {  // NOLINT(runtime/explicit)
    if (callable) {
      ConstructWithAlloc<UniqueFunction<F>>(DefAlloc{}, std::move(callable));
    }
  }

  /*!
   * \brief     In place construct a callable.
   * \details   The callable must not be of the same type as *this. The return value of the callable must be
   *            convertible to R when the argument types Args are supplied.
   * \tparam    F     The type of the stored function object. Must fulfill C++ named requirements: FunctionObject.
   * \tparam    Args  Types necessary to construct F.
   * \param[in] args  Arguments for in place construction.
   * \pre       -
   * \spec      requires true; \endspec
   */
  template <typename F, typename... Args,
            std::enable_if_t<std::is_constructible<std::decay_t<F>, Args...>::value && IsInvocableReq<F>::value,
                             std::int32_t> = 0>
  explicit UniqueFunction(amsr::core::in_place_type_t<F>, Args&&... args) noexcept
      : detail::uniquefunction::StorageAndCallOperator<Signature>{} {
    if (detail::IsSmallCallable<F, Signature>::value) {
      ConstructWithoutAlloc<F>(amsr::core::in_place, std::forward<Args>(args)...);
    } else {
      ConstructWithAlloc<F>(DefAlloc{}, amsr::core::in_place, std::forward<Args>(args)...);
    }
  }

  // VECTOR Next Construct AutosarC++17_10-A8.5.4: MD_VAC_A8.5.4_initializerlistShallBeDefinedInClasses
  // VECTOR Next Construct VectorC++-V8.5.4: MD_VAC_V8-5-4_ctorShallNotBeDefinedToAcceptStdInitializerList
  /*!
   * \brief      In place construct a callable with an initializer list.
   * \details    The callable must not be of the same type as *this. The return value of the callable must be
   *             convertible to R when the argument types Args are supplied.
   * \tparam     F     The type of the stored function object. Must fulfill C++ named requirements: FunctionObject.
   * \tparam     I     The value type of the std::initializer_list.
   * \tparam     Args  Types necessary to construct F.
   * \param[in]  il    Initializer list for in place construction.
   * \param[in]  args  Arguments for in place construction.
   * \pre        -
   * \spec       requires true; \endspec
   */
  template <
      typename F, typename I, typename... Args,
      typename = std::enable_if_t<std::is_constructible<std::decay_t<F>, std::initializer_list<I>, Args...>::value &&
                                  IsInvocableReq<F>::value>>
  UniqueFunction(amsr::core::in_place_type_t<F>, std::initializer_list<I> il, Args&&... args) noexcept
      : detail::uniquefunction::StorageAndCallOperator<Signature>{} {
    if (detail::IsSmallCallable<F, Signature>::value) {
      ConstructWithoutAlloc<F>(amsr::core::in_place, std::move(il), std::forward<Args>(args)...);
    } else {
      ConstructWithAlloc<F>(DefAlloc{}, amsr::core::in_place, std::move(il), std::forward<Args>(args)...);
    }
  }

  /*!
   * \brief      Construct from a callable and an allocator.
   * \details    The callable must not be of the same type as *this. The return value of the callable must be
   *             convertible to R when the argument types Args are supplied.
   * \tparam     Alloc  Allocator type used when storing callable dynamically. Must fulfill C++ named requirements:
   *                    Allocator.
   * \tparam     F      The type of the stored function object. Must fulfill C++ named requirements: FunctionObject.
   * \param[in]  alloc     An allocator that shall be used to allocate dynamic memory.
   * \param[in]  callable  Either a function object or a function pointer.
   * \pre        -
   * \spec       requires true; \endspec
   */
  template <typename Alloc, typename F,
            typename = std::enable_if_t<(!std::is_same<vac::language::remove_cvref_t<F>, UniqueFunction>::value) &&
                                        std::is_constructible<std::decay_t<F>, F>::value && IsInvocableReq<F>::value>>
  UniqueFunction(vac::language::allocator_arg_t, Alloc const& alloc, F&& callable) noexcept
      : detail::uniquefunction::StorageAndCallOperator<Signature>{} {
    if (detail::IsSmallCallable<F, Signature>::value) {
      ConstructWithoutAlloc<F>(std::forward<F>(callable));
    } else {
      ConstructWithAlloc<F>(alloc, std::forward<F>(callable));
    }
  }

  /*!
   * \brief      In place construct a callable and an allocator.
   * \details    The return value of the callable must be convertible to R when the argument types Args are supplied.
   * \tparam     Alloc  Allocator type used when storing the callable dynamically. Must fulfill C++ named requirements:
   *                    Allocator.
   * \tparam     F      The type of the stored function object. Must fulfill C++ named requirements: FunctionObject.
   *                    Must not be the same type as *this.
   * \tparam     Args   The types of the callables.
   * \param[in]  alloc  An allocator that shall be used to allocate dynamic memory.
   * \param[in]  args   Arguments for in place construction.
   * \pre        -
   * \spec       requires true; \endspec
   */
  template <
      typename Alloc, typename F, typename... Args,
      typename = std::enable_if_t<std::is_constructible<std::decay_t<F>, Args...>::value && IsInvocableReq<F>::value>>
  UniqueFunction(vac::language::allocator_arg_t, Alloc const& alloc, amsr::core::in_place_type_t<F>,
                 Args&&... args) noexcept
      : detail::uniquefunction::StorageAndCallOperator<Signature>{} {
    if (detail::IsSmallCallable<F, Signature>::value) {
      ConstructWithoutAlloc<F>(amsr::core::in_place, std::forward<Args>(args)...);
    } else {
      ConstructWithAlloc<F>(alloc, amsr::core::in_place, std::forward<Args>(args)...);
    }
  }

  // VECTOR Next Construct AutosarC++17_10-A8.5.4: MD_VAC_A8.5.4_initializerlistShallBeDefinedInClasses
  // VECTOR Next Construct VectorC++-V8.5.4: MD_VAC_V8-5-4_ctorShallNotBeDefinedToAcceptStdInitializerList
  /*!
   * \brief      In place construct a callable with an initializer list and an allocator.
   * \details    The callable must not be of the same type as *this. The return value of the callable must be
   *             convertible to R when the argument types Args are supplied.
   * \tparam     Alloc  Allocator type used when storing the callable dynamically. Must fulfill C++ named requirements:
   *                    Allocator.
   * \tparam     F      The type of the function object. Must fulfill C++ named requirements: FunctionObject.
   * \tparam     I      The value type of the std::initializer_list.
   * \tparam     Args   The types of the callables.
   * \param[in]  alloc  An allocator that shall be used to allocate dynamic memory.
   * \param[in]  il     Initializer list for in place construction.
   * \param[in]  args   Arguments for in place construction.
   * \pre        -
   * \spec       requires true; \endspec
   */
  template <
      typename Alloc, typename F, typename I, typename... Args,
      typename = std::enable_if_t<std::is_constructible<std::decay_t<F>, std::initializer_list<I>, Args...>::value &&
                                  IsInvocableReq<F>::value>>
  UniqueFunction(vac::language::allocator_arg_t, Alloc const& alloc, amsr::core::in_place_type_t<F>,
                 std::initializer_list<I> il, Args&&... args) noexcept
      : detail::uniquefunction::StorageAndCallOperator<Signature>{} {
    if (detail::IsSmallCallable<F, Signature>::value) {
      ConstructWithoutAlloc<F>(amsr::core::in_place, std::move(il), std::forward<Args>(args)...);
    } else {
      ConstructWithAlloc<F>(alloc, amsr::core::in_place, std::move(il), std::forward<Args>(args)...);
    }
  }

  // VECTOR Next Construct AutosarC++17_10-A13.3.1: MD_VAC_A13.3.1_forwardingFunctionsShallNotBeOverloaded
  /*!
   * \brief     Assign from a callable.
   * \details   The callable must not be of the same type as *this. The return value of the callable must be
   *            convertible to R when the argument types Args are supplied.
   * \tparam    F  The type of the function object. Must fulfill C++ named requirements: FunctionObject.
   * \param[in] callable  Either a function object or a function pointer.
   * \return    A new UniqueFunction based on the provided callable.
   * \pre       -
   * \spec      requires true; \endspec
   */
  template <typename F,
            typename = std::enable_if_t<(!std::is_same<vac::language::remove_cvref_t<F>, UniqueFunction>::value) &&
                                        std::is_move_constructible<std::decay_t<F>>::value && IsInvocableReq<F>::value>>
  UniqueFunction& operator=(F&& callable) & noexcept {
    UniqueFunction{std::forward<F>(callable)}.swap(*this);
    return *this;
  }

  // VECTOR Next Construct AutosarC++17_10-A15.5.1: MD_VAC_A15.5.1_explicitNoexceptIfAppropriate
  /*!
   * \brief     Swap the content of *this with the content of other.
   * \param[in] other  The other UniqueFunction to swap with.
   * \pre       -
   * \spec      requires true; \endspec
   */
  void swap(UniqueFunction& other) noexcept { StorageT::Swap(other); }

  /*!
   * \brief  Returns if *this contains a callable.
   * \return bool True if *this contains a callable, False otherwise.
   * \pre    -
   * \spec   requires true; \endspec
   */
  explicit operator bool() const noexcept {  // Comment to force line break. See TAR-22835.
    return !StorageT::IsEmpty();
  }

  /*!
   * \brief Call operator with the same const, noexcept and ref qualifiers as Signature for this UniqueFunction.
   */
  using detail::uniquefunction::StorageAndCallOperator<Signature>::operator();

 private:
  /*!
   * \brief      Construct using allocator and forwarded arguments.
   * \tparam     F      The type of the function object. Must fulfill C++ named requirements: FunctionObject.
   * \tparam     Alloc  Allocator type used when storing the callable dynamically. Must fulfill C++ named requirements:
   *                    Allocator.
   * \tparam     Args   The types of the callables.
   * \param[in]  alloc  Allocator of the callable to construct a new UniqueFunction.
   * \param[in]  args   Arguments for the construction.
   * \pre        -
   * \spec       requires true; \endspec
   */
  template <typename F, typename Alloc, typename... Args>
  void ConstructWithAlloc(Alloc const& alloc, Args&&... args) noexcept {
    StorageT::template Construct<Alloc, std::decay_t<F>>(alloc, std::forward<Args>(args)...);
  }

  /*!
   * \brief     Construct using forwarded arguments without an allocator.
   * \tparam    F     The type of the function object. Must fulfill C++ named requirements: FunctionObject.
   * \tparam    Args  The types of the callables.
   * \param[in] args  Arguments for the construction.
   * \pre       -
   * \spec      requires true; \endspec
   */
  template <typename F, typename... Args,
            std::enable_if_t<(sizeof...(Args) != 1) || (!detail::IsPossiblyNullptr<F>::value), std::int32_t> = 0>
  void ConstructWithoutAlloc(Args&&... args) noexcept {
    StorageT::template ConstructNoAlloc<std::decay_t<F>>(std::forward<Args>(args)...);
  }

  /*!
   * \brief     Checks if the callable is equal to nullptr.
   * \tparam    F  The type of the callable.
   * \param[in] f  The callable.
   * \return    Return true if the callable is a nullptr, otherwise return false.
   * \pre       -
   * \spec      requires true; \endspec
   */
  template <typename F>
  static bool CallableIsEqualToNullptr(F* f) noexcept {
    return f == nullptr;
  }

  /*!
   * \brief     Construct forwarded arguments without an allocator.
   * \tparam    F     The type of the function object. Must fulfill C++ named requirements: FunctionObject.
   * \tparam    Args  The types of the callables.
   * \param[in] args  Arguments for the construction.
   * \pre       -
   * \spec      requires true; \endspec
   */
  template <typename F, typename... Args,
            std::enable_if_t<(sizeof...(Args) == 1) && detail::IsPossiblyNullptr<F>::value, std::int32_t> = 0>
  void ConstructWithoutAlloc(Args&&... args) noexcept {
    if (!CallableIsEqualToNullptr(std::forward<Args>(args)...)) {
      // It has been certified that the args ... in this case is always a pointer.
      // A pointer will retain its value also after std::forward.
      StorageT::template ConstructNoAlloc<std::decay_t<F>>(std::forward<Args>(args)...);
    }
  }
};

// VECTOR Next Construct AutosarC++17_10-A15.5.1: MD_VAC_A15.5.1_explicitNoexceptIfAppropriate
/*!
 * \brief         Swaps the content of lhs and rhs.
 * \tparam        Signature  The function type of the callable. May be const qualified, use lvalue/rvalue reference
 *                           qualifier and/or noexcept qualifier.
 * \param[in,out] lhs  First object to swap with.
 * \param[in,out] rhs  Second object to swap with.
 * \pre           -
 * \spec          requires true; \endspec
 */
template <typename Signature>
void swap(UniqueFunction<Signature>& lhs, UniqueFunction<Signature>& rhs) noexcept {
  lhs.swap(rhs);
}

/*!
 * \brief      Checks a UniqueFunction with a nullptr_t.
 * \tparam     Signature  The function type of the callable. May be const qualified, use lvalue/rvalue reference
 *                        qualifier and/or noexcept qualifier.
 * \param[in]  lhs  A UniqueFunction object to check.
 * \return     True if the UniqueFunction does not contain a callable.
 * \pre        -
 * \spec       requires true; \endspec
 */
template <typename Signature>
bool operator==(UniqueFunction<Signature> const& lhs, std::nullptr_t) noexcept {
  return !lhs;
}

/*!
 * \brief      Compares a UniqueFunction with a nullptr_t.
 * \tparam     Signature  The function type of the callable. May be const qualified, use lvalue/rvalue reference
 *                        qualifier and/or noexcept qualifier.
 * \param[in]  rhs  A UniqueFunction object to check.
 * \return     True if the UniqueFunction does not contains a callable.
 * \pre        -
 * \spec       requires true; \endspec
 */
template <typename Signature>
bool operator==(std::nullptr_t, UniqueFunction<Signature> const& rhs) noexcept {
  return !rhs;
}

/*!
 * \brief      Compares a UniqueFunction with a nullptr_t.
 * \tparam     Signature  The function type of the callable. May be const qualified, use lvalue/rvalue reference
 *                        qualifier and/or noexcept qualifier.
 * \param[in]  lhs  A UniqueFunction object to check.
 * \return     True if the UniqueFunction contains a callable.
 * \pre        -
 * \spec       requires true; \endspec
 */
template <typename Signature>
bool operator!=(UniqueFunction<Signature> const& lhs, std::nullptr_t) noexcept {
  return static_cast<bool>(lhs);
}

/*!
 * \brief      Compares a UniqueFunction with a nullptr_t.
 * \tparam     Signature  The function type of the callable. May be const qualified, use lvalue/rvalue reference
 *                        qualifier and/or noexcept qualifier.
 * \param[in]  rhs  A UniqueFunction object to check.
 * \return     True if the UniqueFunction contains a callable.
 * \pre        -
 * \spec       requires true; \endspec
 */
template <typename Signature>
bool operator!=(std::nullptr_t, UniqueFunction<Signature> const& rhs) noexcept {
  return static_cast<bool>(rhs);
}

}  // namespace language
}  // namespace vac

//  COV_JUSTIFICATION_BEGIN
//    \ID COV_LIBVAC_NO_EXCEPT_SPECIFIER
//      \ACCEPT XX
//      \REASON For UniqueFunction to accept a signature including the noexcept specifier for C++17 and later,
//      specializations with this specifier are required. This however, introduces coverage gaps for C++14 since the
//      noexcept specifier is not part of the signature in C++14 and is therefore ignored causing signatures without the
//      noexcept specifier to use that specialization. Since signature::HasNoexcept will always be true for C++14, the
//      coverage gaps will be on the non-noexcept specializations.
//  COV_JUSTIFICATION_END

#endif  // LIB_VAC_INCLUDE_VAC_LANGUAGE_UNIQUE_FUNCTION_H_
