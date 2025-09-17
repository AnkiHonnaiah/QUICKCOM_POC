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
/**        \file  inplace_function.h
 *        \brief  A non-allocating equivalent of std::function without heap allocation.
 *         \unit  VaCommonLib::LanguageSupport::InplaceFunction
 *
 *      \details  vac::language::InplaceFunction is an owning function wrapper that will not do any heap allocation.
 *                This implementation always allocates stack memory to store the callable and the object itself will
 *                hold all the data needed for a given callable.
 *                InplaceFunction doesn't provide support of allocators, instead user specifies the max size of a
 *                callable that InplaceFunction can hold in a template parameter, and any callable that exceeds
 *                that size will fail the overload resolution for all constructors and assignment operators.
 *                InplaceFunction is copyable and movable, and only handling trivially copyable callables.
 *********************************************************************************************************************/

#ifndef LIB_VAC_INCLUDE_VAC_LANGUAGE_INPLACE_FUNCTION_H_
#define LIB_VAC_INCLUDE_VAC_LANGUAGE_INPLACE_FUNCTION_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <algorithm>
#include <cassert>
#include <memory>
#include <type_traits>
#include <utility>

#include "amsr/core/utility.h"
#include "ara/core/abort.h"
#include "vac/language/cpp20_backport.h"
#include "vac/language/detail/function_signature.h"

namespace vac {
namespace language {
namespace detail {

/*!
 * \brief  Static aligned storage type used as local buffer.
 * \tparam Capacity    The maximum size for static aligned storage.
 * \tparam Alignment   The alignment for static aligned storage
 */
template <std::size_t Capacity, std::size_t Alignment>
using BufferTypeAligned = std::aligned_storage_t<Capacity, Alignment>;

namespace inplacefunction {
/*!
 * \brief    Holds statically allocated storage for an InplaceFunction callable.
 * \tparam   Signature   The function type of the callable. May be const qualified, may have lvalue/rvalue
 *                       reference qualifier and/or noexcept qualifier.
 * \tparam   Capacity    The maximum size for the contained callable.
 * \tparam   Alignment   The alignment for contained callable.
 * \vprivate Component private
 */
template <typename Signature, std::size_t Capacity, std::size_t Alignment>
class CallableStorage;

// VECTOR Next Construct AutosarC++17_10-A12.4.1: MD_VAC_A12.4.1_noVirtualDtor
/*!
 * \brief    Call operator implementation for signatures with const, noexcept and ref qualifiers. Inherits from
 *           CallableStorage. Provides the call operator and callable storage for InplaceFunction.
 * \tparam   Capacity    The maximum size for the contained callable.
 * \tparam   Alignment   The alignment for contained callable.
 * \tparam   Sig1       Signature of the callable type.
 * \tparam   Sig2       Signature of the callable type.
 * \tparam   Noexcept   if noexcept is part of Signature of the callable type.
 * \vprivate Component private
 */
template <std::size_t Capacity, std::size_t Alignment, typename Sig1, typename Sig2,
          bool Noexcept = signature::HasNoexcept<Sig2>::value>
class StorageAndOperatorImpl;  // VCA_VAC_SAFE_DEALLOCATION

/*!
 * \brief    Specialization for one of the twelve supported combinations of const, noexcept and ref qualifiers.
 * \tparam   Signature   The function type of the callable. For this specialization it shall be non qualified.
 * \tparam   Capacity    The maximum size for the contained callable.
 * \tparam   Alignment   The alignment for contained callable.
 * \tparam   R           Signature's return type.
 * \tparam   Args        The types of signature parameters.
 * \vprivate Component private
 */
template <std::size_t Capacity, std::size_t Alignment, typename Signature, typename R, typename... Args>
class StorageAndOperatorImpl<Capacity, Alignment, Signature, R(Args...), false>
    : public CallableStorage<Signature, Capacity, Alignment> {
 public:
  // VECTOR NC AutosarC++17_10-M9.3.3: MD_VAC_M9.3.3_methodCanBeDeclaredConstOperator
  /*!
   * \brief     Call operator.
   * \param[in] args   Input of callable.
   * \return    Result after invoking the callable.
   * \vprivate  Component private
   * \spec      requires true; \endspec
   */
  auto operator()(Args... args) noexcept -> R {
    assert(this->function_manager_ptr_ != nullptr);
    // VCA_VAC_VALID_FUNC_CALL_COMPILE_TIME_CHECK
    return static_cast<R>(this->function_manager_ptr_->Invoke(std::forward<Args>(args)...));
  }
};

/*!
 * \brief    Specialization for one of the twelve supported combinations of const, noexcept and ref qualifiers.
 * \tparam   Signature   The function type of the callable. For this specialization it shall be noexcept qualified.
 * \tparam   Capacity    The maximum size for the contained callable.
 * \tparam   Alignment   The alignment for contained callable.
 * \tparam   R           Signature's return type.
 * \tparam   Args        The types of signature parameters.
 * \vprivate Component private
 */
// VECTOR NC Compiler-#552: MD_VAC_Compiler-#552_noexceptNotAllowed
template <std::size_t Capacity, std::size_t Alignment, typename Signature, typename R, typename... Args>
class StorageAndOperatorImpl<Capacity, Alignment, Signature, R(Args...) noexcept, true>
    : public CallableStorage<Signature, Capacity, Alignment> {
 public:
  // VECTOR NC AutosarC++17_10-M9.3.3: MD_VAC_M9.3.3_methodCanBeDeclaredConstOperator
  /*!
   * \brief     Call operator.
   * \param[in] args   Input of callable.
   * \return    Result after invoking the callable.
   * \vprivate  Component private
   * \spec      requires true; \endspec
   */
  auto operator()(Args... args) noexcept -> R {
    assert(this->function_manager_ptr_ != nullptr);
    // VCA_VAC_VALID_FUNC_CALL_COMPILE_TIME_CHECK
    return static_cast<R>(this->function_manager_ptr_->Invoke(std::forward<Args>(args)...));
  }
};

/*!
 * \brief    Specialization for one of the twelve supported combinations of const, noexcept and ref qualifiers.
 * \tparam   Signature   The function type of the callable. For this specialization it shall be const qualified
 * \tparam   Capacity    The maximum size for the contained callable.
 * \tparam   Alignment   The alignment for contained callable.
 * \tparam   R           Signature's return type.
 * \tparam   Args        The types of signature parameters.
 * \vprivate Component private
 */
template <std::size_t Capacity, std::size_t Alignment, typename Signature, typename R, typename... Args>
class StorageAndOperatorImpl<Capacity, Alignment, Signature, R(Args...) const, false>
    : public CallableStorage<Signature, Capacity, Alignment> {
 public:
  // VECTOR Next Construct AutosarC++17_10-A18.9.2: MD_VAC_A18.9.2_useStdMoveIfTheValueIsARvalueReference
  /*!
   * \brief     Call operator.
   * \param[in] args   Input of callable.
   * \return    Result after invoking the callable.
   * \vprivate  Component private
   * \spec      requires true; \endspec
   */
  auto operator()(Args... args) const noexcept -> R {
    assert(this->function_manager_ptr_ != nullptr);
    // VCA_VAC_VALID_FUNC_CALL_COMPILE_TIME_CHECK
    return static_cast<R>(this->function_manager_ptr_->Invoke(std::forward<Args>(args)...));
  }
};

/*!
 * \brief    Specialization for one of the twelve supported combinations of const, noexcept and ref qualifiers.
 * \tparam   Signature   The function type of the callable. For this specialization it shall be const
 *                       and noexcept qualified.
 * \tparam   Capacity    The maximum size for the contained callable.
 * \tparam   Alignment   The alignment for contained callable.
 * \tparam   R           Signature's return type.
 * \tparam   Args        The types of signature parameters.
 * \vprivate Component private
 */
// VECTOR NC Compiler-#552: MD_VAC_Compiler-#552_noexceptNotAllowed
template <std::size_t Capacity, std::size_t Alignment, typename Signature, typename R, typename... Args>
class StorageAndOperatorImpl<Capacity, Alignment, Signature, R(Args...) const noexcept, true>
    : public CallableStorage<Signature, Capacity, Alignment> {
 public:
  // VECTOR Next Construct AutosarC++17_10-A18.9.2: MD_VAC_A18.9.2_useStdMoveIfTheValueIsARvalueReference
  /*!
   * \brief     Call operator.
   * \param[in] args   Input of callable.
   * \return    Result after invoking the callable.
   * \vprivate  Component private
   * \spec      requires true; \endspec
   */
  auto operator()(Args... args) const noexcept -> R {
    assert(this->function_manager_ptr_ != nullptr);
    // VCA_VAC_VALID_FUNC_CALL_COMPILE_TIME_CHECK
    return static_cast<R>(this->function_manager_ptr_->Invoke(std::forward<Args>(args)...));
  }
};

/*!
 * \brief    Specialization for one of the twelve supported combinations of const, noexcept and ref qualifiers.
 * \tparam   Signature   The function type of the callable. For this specialization it shall be lvalue-ref qualified.
 * \tparam   Capacity    The maximum size for the contained callable.
 * \tparam   Alignment   The alignment for contained callable.
 * \tparam   R           Signature's return type.
 * \tparam   Args        The types of signature parameters.
 * \vprivate Component private
 */
template <std::size_t Capacity, std::size_t Alignment, typename Signature, typename R, typename... Args>
class StorageAndOperatorImpl<Capacity, Alignment, Signature, R(Args...)&, false>
    : public CallableStorage<Signature, Capacity, Alignment> {
 public:
  // VECTOR NC AutosarC++17_10-M9.3.3: MD_VAC_M9.3.3_methodCanBeDeclaredConstOperator
  // VECTOR Next Construct AutosarC++17_10-A18.9.2: MD_VAC_A18.9.2_useStdMoveIfTheValueIsARvalueReference
  /*!
   * \brief     Call operator.
   * \param[in] args   Input of callable.
   * \return    Result after invoking the callable.
   * \vprivate  Component private
   * \spec      requires true; \endspec
   */
  auto operator()(Args... args) & noexcept -> R {
    assert(this->function_manager_ptr_ != nullptr);
    // VCA_VAC_VALID_FUNC_CALL_COMPILE_TIME_CHECK
    return static_cast<R>(this->function_manager_ptr_->Invoke(std::forward<Args>(args)...));
  }
};

/*!
 * \brief    Specialization for one of the twelve supported combinations of const, noexcept and ref qualifiers.
 * \tparam   Signature   The function type of the callable. For this specialization it shall be lvalue-ref
 *                       and noexcept qualified.
 * \tparam   Capacity    The maximum size for the contained callable.
 * \tparam   Alignment   The alignment for contained callable.
 * \tparam   R           Signature's return type.
 * \tparam   Args        The types of signature parameters.
 * \vprivate Component private
 */
// VECTOR NC Compiler-#552: MD_VAC_Compiler-#552_noexceptNotAllowed
template <std::size_t Capacity, std::size_t Alignment, typename Signature, typename R, typename... Args>
class StorageAndOperatorImpl<Capacity, Alignment, Signature, R(Args...) & noexcept, true>
    : public CallableStorage<Signature, Capacity, Alignment> {
 public:
  // VECTOR NC AutosarC++17_10-M9.3.3: MD_VAC_M9.3.3_methodCanBeDeclaredConstOperator
  // VECTOR Next Construct AutosarC++17_10-A18.9.2: MD_VAC_A18.9.2_useStdMoveIfTheValueIsARvalueReference
  /*!
   * \brief     Call operator.
   * \param[in] args   Input of callable.
   * \return    Result after invoking the callable.
   * \vprivate  Component private
   * \spec      requires true; \endspec
   */
  auto operator()(Args... args) & noexcept -> R {
    assert(this->function_manager_ptr_ != nullptr);
    // VCA_VAC_VALID_FUNC_CALL_COMPILE_TIME_CHECK
    return static_cast<R>(this->function_manager_ptr_->Invoke(std::forward<Args>(args)...));
  }
};

/*!
 * \brief    Specialization for one of the twelve supported combinations of const, noexcept and ref qualifiers.
 * \tparam   Signature   The function type of the callable. For this specialization it shall be const-lvalue-ref
 *                        qualified.
 * \tparam   Capacity    The maximum size for the contained callable.
 * \tparam   Alignment   The alignment for contained callable.
 * \tparam   R           Signature's return type.
 * \tparam   Args        The types of signature parameters.
 * \vprivate Component private
 */
template <std::size_t Capacity, std::size_t Alignment, typename Signature, typename R, typename... Args>
class StorageAndOperatorImpl<Capacity, Alignment, Signature, R(Args...) const&, false>
    : public CallableStorage<Signature, Capacity, Alignment> {
 public:
  // VECTOR Next Construct AutosarC++17_10-A18.9.2: MD_VAC_A18.9.2_useStdMoveIfTheValueIsARvalueReference
  /*!
   * \brief     Call operator.
   * \param[in] args   Input of callable.
   * \return    Result after invoking the callable.
   * \vprivate  Component private
   * \spec      requires true; \endspec
   */
  auto operator()(Args... args) const& noexcept -> R {
    assert(this->function_manager_ptr_ != nullptr);
    // VCA_VAC_VALID_FUNC_CALL_COMPILE_TIME_CHECK
    return static_cast<R>(this->function_manager_ptr_->Invoke(std::forward<Args>(args)...));
  }
};

/*!
 * \brief    Specialization for one of the twelve supported combinations of const, noexcept and ref qualifiers.
 * \tparam   Signature   The function type of the callable. For this specialization it shall be const-lvalue-ref
 *                        and noexcept qualified.
 * \tparam   Capacity    The maximum size for the contained callable.
 * \tparam   Alignment   The alignment for contained callable.
 * \tparam   R           Signature's return type.
 * \tparam   Args        The types of signature parameters.
 * \vprivate Component private
 */
// VECTOR NC Compiler-#552: MD_VAC_Compiler-#552_noexceptNotAllowed
template <std::size_t Capacity, std::size_t Alignment, typename Signature, typename R, typename... Args>
class StorageAndOperatorImpl<Capacity, Alignment, Signature, R(Args...) const & noexcept, true>
    : public CallableStorage<Signature, Capacity, Alignment> {
 public:
  // VECTOR Next Construct AutosarC++17_10-A18.9.2: MD_VAC_A18.9.2_useStdMoveIfTheValueIsARvalueReference
  /*!
   * \brief     Call operator.
   * \param[in] args   Input of callable.
   * \return    Result after invoking the callable.
   * \vprivate  Component private
   * \spec      requires true; \endspec
   */
  auto operator()(Args... args) const& noexcept -> R {
    assert(this->function_manager_ptr_ != nullptr);
    // VCA_VAC_VALID_FUNC_CALL_COMPILE_TIME_CHECK
    return static_cast<R>(this->function_manager_ptr_->Invoke(std::forward<Args>(args)...));
  }
};

/*!
 * \brief    Specialization for one of the twelve supported combinations of const, noexcept and ref qualifiers.
 * \tparam   Signature   The function type of the callable. For this specialization it shall be rvalue-ref qualified.
 * \tparam   Capacity    The maximum size for the contained callable.
 * \tparam   Alignment   The alignment for contained callable.
 * \tparam   R           Signature's return type.
 * \tparam   Args        The types of signature parameters.
 * \vprivate Component private
 */
template <std::size_t Capacity, std::size_t Alignment, typename Signature, typename R, typename... Args>
class StorageAndOperatorImpl<Capacity, Alignment, Signature, R(Args...)&&, false>
    : public CallableStorage<Signature, Capacity, Alignment> {
 public:
  // VECTOR NC AutosarC++17_10-M9.3.3: MD_VAC_M9.3.3_methodCanBeDeclaredConstOperator
  // VECTOR Next Construct AutosarC++17_10-A18.9.2: MD_VAC_A18.9.2_useStdMoveIfTheValueIsARvalueReference
  /*!
   * \brief     Call operator.
   * \param[in] args   Input of callable.
   * \return    Result after invoking the callable.
   * \vprivate  Component private
   * \spec      requires true; \endspec
   */
  auto operator()(Args... args) && noexcept -> R {
    assert(this->function_manager_ptr_ != nullptr);
    // VCA_VAC_VALID_FUNC_CALL_COMPILE_TIME_CHECK
    return static_cast<R>(this->function_manager_ptr_->Invoke(std::forward<Args>(args)...));
  }
};

/*!
 * \brief    Specialization for one of the twelve supported combinations of const, noexcept and ref qualifiers.
 * \tparam   Signature   The function type of the callable. For this specialization it shall be rvalue-ref
 *                       and noexcept qualified.
 * \tparam   Capacity    The maximum size for the contained callable.
 * \tparam   Alignment   The alignment for contained callable.
 * \tparam   R           Signature's return type.
 * \tparam   Args        The types of signature parameters.
 * \vprivate Component private
 */
// VECTOR NC Compiler-#552: MD_VAC_Compiler-#552_noexceptNotAllowed
template <std::size_t Capacity, std::size_t Alignment, typename Signature, typename R, typename... Args>
class StorageAndOperatorImpl<Capacity, Alignment, Signature, R(Args...) && noexcept, true>
    : public CallableStorage<Signature, Capacity, Alignment> {
 public:
  // VECTOR NC AutosarC++17_10-M9.3.3: MD_VAC_M9.3.3_methodCanBeDeclaredConstOperator
  // VECTOR Next Construct AutosarC++17_10-A18.9.2: MD_VAC_A18.9.2_useStdMoveIfTheValueIsARvalueReference
  /*!
   * \brief     Call operator.
   * \param[in] args   Input of callable.
   * \return    Result after invoking the callable.
   * \vprivate  Component private
   * \spec      requires true; \endspec
   */
  auto operator()(Args... args) && noexcept -> R {
    assert(this->function_manager_ptr_ != nullptr);
    // VCA_VAC_VALID_FUNC_CALL_COMPILE_TIME_CHECK
    return static_cast<R>(this->function_manager_ptr_->Invoke(std::forward<Args>(args)...));
  }
};

/*!
 * \brief   Specialization for one of the twelve supported combinations of const, noexcept and ref qualifiers.
 * \tparam   Signature   The function type of the callable. For this specialization it shall be
 *                       const rvalue-ref qualified.
 * \tparam   Capacity    The maximum size for the contained callable.
 * \tparam   Alignment   The alignment for contained callable.
 * \tparam   R           Signature's return type.
 * \tparam   Args        The types of signature parameters.
 * \vprivate Component private
 */
template <std::size_t Capacity, std::size_t Alignment, typename Signature, typename R, typename... Args>
class StorageAndOperatorImpl<Capacity, Alignment, Signature, R(Args...) const&&, false>
    : public CallableStorage<Signature, Capacity, Alignment> {
 public:
  // VECTOR Next Construct AutosarC++17_10-A18.9.2: MD_VAC_A18.9.2_useStdMoveIfTheValueIsARvalueReference
  /*!
   * \brief     Call operator.
   * \param[in] args   Input of callable.
   * \return    Result after invoking the callable.
   * \vprivate  Component private
   * \spec      requires true; \endspec
   */
  auto operator()(Args... args) const&& noexcept -> R {
    assert(this->function_manager_ptr_ != nullptr);
    // VCA_VAC_VALID_FUNC_CALL_COMPILE_TIME_CHECK
    return static_cast<R>(this->function_manager_ptr_->Invoke(std::forward<Args>(args)...));
  }
};

/*!
 * \brief    Specialization for one of the twelve supported combinations of const, noexcept and ref qualifiers.
 * \tparam   Signature   The function type of the callable. May be const qualified, may have lvalue/rvalue
 *                       reference qualifier and/or noexcept qualifier.
 * \tparam   Capacity    The maximum size for the contained callable.
 * \tparam   Alignment   The alignment for contained callable.
 * \tparam   R           Signature's return type.
 * \tparam   Args        The types of signature parameters.
 * \vprivate Component private
 */
// VECTOR NC Compiler-#552: MD_VAC_Compiler-#552_noexceptNotAllowed
template <std::size_t Capacity, std::size_t Alignment, typename Signature, typename R, typename... Args>
class StorageAndOperatorImpl<Capacity, Alignment, Signature, R(Args...) const && noexcept, true>
    : public CallableStorage<Signature, Capacity, Alignment> {
 public:
  // VECTOR Next Construct AutosarC++17_10-A18.9.2: MD_VAC_A18.9.2_useStdMoveIfTheValueIsARvalueReference
  /*!
   * \brief     Call operator.
   * \param[in] args   Input of callable.
   * \return    Result after invoking the callable.
   * \vprivate  Component private
   * \spec      requires true; \endspec
   */
  auto operator()(Args... args) const&& noexcept -> R {
    assert(this->function_manager_ptr_ != nullptr);
    // VCA_VAC_VALID_FUNC_CALL_COMPILE_TIME_CHECK
    return static_cast<R>(this->function_manager_ptr_->Invoke(std::forward<Args>(args)...));
  }
};

/*!
 * \brief    An alias which forwards Signature twice instead of having InplaceFunction do this.
 * \tparam   Signature   The function type of the callable. May be const qualified, may have lvalue/rvalue
 *                       reference qualifier and/or noexcept qualifier.
 * \tparam   Capacity    The maximum size for the contained callable.
 * \tparam   Alignment   The alignment for contained callable.
 * \vprivate Component private
 */
template <typename Signature, std::size_t Capacity, std::size_t Alignment>
using StorageAndCallOperator = StorageAndOperatorImpl<Capacity, Alignment, Signature, Signature>;

/*!
 * \brief    Interface for a stored callable object. Enables virtual calls, deallocation, and destruction. Used from
 *           StorageAndCallOperator::operator() inherited by InplaceFunction.
 * \tparam   R      Return type of Signature.
 * \tparam   Args   The types in Signature's parameter-list.
 * \vprivate Component private
 */
template <typename R, typename... Args>
class IFunctionManager {
  /*!
   * \brief Type for FunctionManager Interface.
   */
  using IFuncMan = IFunctionManager<R, Args...>;

 public:
  /*!
   * \brief    Default Constructor.
   * \vprivate Component private
   * \spec     requires true; \endspec
   */
  IFunctionManager() noexcept = default;
  /*!
   * \brief Deleted copy constructor.
   */
  IFunctionManager(IFunctionManager const&) = delete;
  /*!
   * \brief Deleted move constructor.
   */
  IFunctionManager(IFunctionManager&&) = delete;
  /*!
   * \brief Deleted copy assignment.
   */
  IFunctionManager& operator=(IFunctionManager const&) = delete;
  /*!
   * \brief Deleted move assignment.
   */
  IFunctionManager& operator=(IFunctionManager&&) = delete;

  /*!
   * \brief    Default destructor.
   * \vprivate Component private
   * \spec     requires true; \endspec
   */
  // VECTOR NL AutosarC++17_10-A15.5.1: MD_VAC_A15.5.1_explicitNoexceptIfAppropriate
  virtual ~IFunctionManager() noexcept = default;

  /*!
   * \brief    Invocation interface
   * \tparam   Input   to be invoked.
   * \return   Invocation result.
   * \vprivate Component private
   * \spec     requires true; \endspec
   */
  // VECTOR NL AutosarC++17_10-M9.3.3: MD_VAC_M9.3.3_methodCanBeDeclaredConstVirtual
  virtual R Invoke(Args...) noexcept = 0;

  /*!
   * \brief    OnDelete allows the allocator to be type erased.
   * \vprivate Component private
   * \spec     requires true; \endspec
   */
  virtual void OnDelete() noexcept = 0;

  /*!
   * \brief    Copy construct the contents of *this to the supplied storage.
   * \vprivate Component private
   * \spec     requires true; \endspec
   */
  virtual void CopyConstruct(IFuncMan*) const noexcept = 0;
  /*!
   * \brief    Move construct the contents of *this to the supplied storage.
   * \vprivate Component private
   * \spec     requires true; \endspec
   */
  virtual void MoveConstruct(IFuncMan*) noexcept = 0;
  /*!
   * \brief    To check if FunctionManager has callable or not.
   * \return   Returns true FunctionManager has callable, false otherwise.
   * \vprivate Component private
   * \spec     requires true; \endspec
   */
  virtual auto HasCallable() const noexcept -> bool = 0;
};

// VECTOR Next Construct VectorC++-V12.0.1: MD_VAC_V12-0-1_onlyDtorImplemented
// VECTOR Next Construct AutosarC++17_10-A12.0.1: MD_VAC_A12-0-1_onlyDtorImplemented
/*!
 * \brief    Stores a callable object. This is the storage implementation that CallableStorage actually points to.
 * \tparam   FuncT           The type of the stored function object. Must fulfill C++ named requirements:
 *                           FunctionObject.
 * \tparam   InvokeAsRRef    The bool type to check if FunctionManger is invoked as R value reference.
 * \tparam   InvokeAsConst   The bool type to check if FunctionManger is invoked as const.
 * \tparam   R               Return type of Signature.
 * \tparam   Args            The types in Signature's parameter-list.
 * \vprivate Component private
 */
template <typename FuncT, bool InvokeAsRRef, bool InvokeAsConst, typename R, typename... Args>
class FunctionManager final : public IFunctionManager<R, Args...> {
  /*!
   * \brief Type for FunctionManager Interface.
   */
  using IFuncMan = IFunctionManager<R, Args...>;

  /*!
   * \brief FunctionManager for F and signature.
   */
  using ExactFuncMan = FunctionManager<FuncT, InvokeAsRRef, InvokeAsConst, R, Args...>;

 public:
  /*!
   * \brief Default destructor.
   * \spec  requires true; \endspec
   */
  ~FunctionManager() noexcept final = default;  // VCA_VAC_SAFE_DEALLOCATION

  // VECTOR NC AutosarC++17_10-A12.1.5: MD_VAC_A12.1.5_useDelegatingConstructor
  /*!
   * \brief     In place constructor.
   * \tparam    CArgs   The types used to construct FuncT.
   * \param[in] args   Input to construct the FunctionManager.
   * \vprivate  Component private
   * \spec      requires true; \endspec
   */
  template <typename... CArgs>
  explicit FunctionManager(amsr::core::in_place_t, CArgs&&... args) noexcept
      : IFuncMan{}, func_{std::forward<CArgs>(args)...} {}

  /*!
   * \brief         Move constructor for callables.
   * \param[in,out] callable   The function object used to construct the FunctionManager.
   * \vprivate      Component private
   * \spec          requires true; \endspec
   */
  explicit FunctionManager(FuncT&& callable) noexcept
      : IFuncMan{}, func_{std::move(callable)} {}  // VCA_VAC_VALID_FUNC_CALL

  /*!
   * \brief     Copy constructor for callables.
   * \param[in] callable   The function object used to construct the FunctionManager.
   * \vprivate  Component private
   * \spec      requires true; \endspec
   */
  explicit FunctionManager(FuncT const& callable) noexcept : IFuncMan{}, func_{callable} {}

  /*!
   * \brief         Copy construct the contents of *this to the supplied storage.
   * \param[in,out] function_manager_ptr   Pointer to storage to construct FunctionManager in.
   * \vprivate      Component private
   * \spec          requires true; \endspec
   */
  void CopyConstruct(IFuncMan* function_manager_ptr) const noexcept final {
    assert(function_manager_ptr != nullptr);
    new (function_manager_ptr) ExactFuncMan{func_};  // VCA_VAC_CHECKED_ACCESS_TO_VALID_POINTER
  }

  /*!
   * \brief         Move construct the contents of *this to the supplied storage.
   * \param[in,out] function_manager_ptr   Pointer to storage to construct FunctionManager in.
   * \vprivate      Component private
   * \spec          requires true; \endspec
   */
  void MoveConstruct(IFuncMan* function_manager_ptr) noexcept final {
    assert(function_manager_ptr != nullptr);
    new (function_manager_ptr) ExactFuncMan{std::move(func_)};  // VCA_VAC_CHECKED_ACCESS_TO_VALID_POINTER
  }

  /*!
   * \brief     Invokes the callable with InvQuals.
   * \param[in] args   Invocation argument types.
   * \return    Invocation result.
   * \vprivate  Component private
   * \spec      requires true; \endspec
   */
  R Invoke(Args... args) noexcept final {
    using CallableCVRef = typename AddConditionalCLRRef<FuncT, InvokeAsConst, !InvokeAsRRef, InvokeAsRRef>::type;
    // VCA_VAC_VALID_FUNC_CALL_COMPILE_TIME_CHECK, VCA_VAC_VALID_FUNC_CALL
    return static_cast<R>(static_cast<CallableCVRef>(func_)(std::forward<Args>(args)...));
  }

  // VECTOR Next Construct AutosarC++17_10-M0.1.8: MD_VAC_M0.1.8_functionHasNoExternalSideEffect
  /*!
   * \brief    OnDelete override. Call destructor. No dynamic memory to release.
   * \vprivate Component private
   */
  void OnDelete() noexcept final { this->~FunctionManager(); }

  /*!
   * \brief    Returns true if FunctionManager contains a callable, false otherwise.
   * \return   True
   * \vprivate Component private
   */
  auto HasCallable() const noexcept -> bool final { return true; }

 private:
  /*!
   * \brief Callable.
   */
  FuncT func_;
};

/*!
 * \brief    CallableStorage implementation when the InplaceFunction is empty.
 * \tparam   InvokeAsRRef    The bool type to check if FunctionManger is invoked as R reference.
 * \tparam   InvokeAsConst   The bool type to check if FunctionManger is invoked as const.
 * \tparam   R               Return type of the function object.
 * \tparam   Args            The types of signature parameters.
 * \vprivate Component private
 */
template <bool InvokeAsRRef, bool InvokeAsConst, typename R, typename... Args>
class EmptyFunctionManager final : public IFunctionManager<R, Args...> {
  /*!
   * \brief Type for FunctionManager Interface.
   */
  using IFuncMan = IFunctionManager<R, Args...>;

 public:
  /*!
   * \brief    Default constructor.
   * \vprivate Component private
   * \spec     requires true; \endspec
   */
  EmptyFunctionManager() noexcept = default;
  /*!
   * \brief Deleted copy constructor.
   */
  EmptyFunctionManager(EmptyFunctionManager const&) = delete;
  /*!
   * \brief Deleted move constructor.
   */
  EmptyFunctionManager(EmptyFunctionManager&&) = delete;
  /*!
   * \brief Deleted copy assignment.
   */
  EmptyFunctionManager& operator=(EmptyFunctionManager const&) = delete;
  /*!
   * \brief Deleted move assignment.
   */
  EmptyFunctionManager& operator=(EmptyFunctionManager&&) = delete;
  /*!
   * \brief    Default destructor.
   * \vprivate Component private
   * \spec     requires true; \endspec
   */
  ~EmptyFunctionManager() noexcept final = default;  // VCA_VAC_SAFE_DEALLOCATION

  /*!
   * \brief    Invokes the callable. Since it's an EmptyFunction manager, the execution will be aborted.
   * \vprivate Component private
   */
  auto Invoke(Args...) noexcept -> R final {
    ara::core::Abort("vac::language::InplaceFunction: InplaceFunction called without callable");
  }

  // VECTOR Next Construct AutosarC++17_10-M0.1.8: MD_VAC_M0.1.8_functionHasNoExternalSideEffect
  /*!
   * \brief    OnDelete override. Calls destructor.
   * \vprivate Component private
   */
  void OnDelete() noexcept final { this->~EmptyFunctionManager(); }

  /*!
   * \brief    Returns true if FunctionManager contains a callable, false otherwise.
   * \return   True
   * \vprivate Component private
   */
  auto HasCallable() const noexcept -> bool final { return false; }

  // VECTOR NC AutosarC++17_10-M0.1.8: MD_VAC_M0.1.8_emptyVoidFunctionWithoutExternalSideEffect
  /*!
   * \brief    Copy from given FunctionManager.
   * \vprivate Component private
   * \spec     requires true; \endspec
   */
  void CopyConstruct(IFuncMan*) const noexcept final {}

  // VECTOR NC AutosarC++17_10-M0.1.8: MD_VAC_M0.1.8_emptyVoidFunctionWithoutExternalSideEffect
  /*!
   * \brief    Move from given FunctionManager.
   * \vprivate Component private
   * \spec     requires true; \endspec
   */
  void MoveConstruct(IFuncMan*) noexcept final {}
};

/*!
 * \brief    Expands the argument type pack and provides the fully specified IFunctionManager type.
 * \tparam   SigProps   Signature properties received from signature::GetSigProps.
 * \tparam   ArgsT      The types of signature input parameters.
 * \vprivate Component private
 */
template <typename SigProps, typename ArgsT>
struct IFunctionManagerSelectorArgExpander;

/*!
 * \brief    Expands the argument type pack and provides the fully specified IFunctionManager type.
 * \tparam   SigProps   Signature properties received from signature::GetSigProps.
 * \tparam   Args       The types in Signature's parameter-list.
 * \vprivate Component private
 */
template <typename SigProps, typename... Args>
struct IFunctionManagerSelectorArgExpander<SigProps, signature::SignatureArgs<Args...>>
    : vac::language::type_identity<IFunctionManager<typename SigProps::RetT, Args...>> {};

/*!
 * \brief    Provides the IFunctionManager type using some indirections.
 * \tparam   Signature  The function type of the callable. May be const qualified, may have lvalue/rvalue
 *                       reference qualifier and/or noexcept qualifier.
 * \vprivate Component private
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

/*!
 * \brief    For FuncT as non-void type, FunctionManager is used.
 * \tparam   FuncT           The type of the stored function object. Must fulfill C++ named requirements:
 *                           FunctionObject.
 * \tparam   InvokeAsRRef    The bool type to check if FunctionManger is invoked as R value reference.
 * \tparam   InvokeAsConst   The bool type to check if FunctionManger is invoked as const.
 * \tparam   RetT            Return type of Signature.
 * \tparam   Args            The types in Signature's parameter-list.
 * \vprivate Component private
 */
template <typename FuncT, bool InvokeAsRRef, bool InvokeAsConst, typename RetT, typename... Args>
class FunctionManagerStorageSelector {
 public:
  /*!
   * \brief Type of FunctionManager storing the function object.
   */
  using type = FunctionManager<FuncT, InvokeAsRRef, InvokeAsConst, RetT, Args...>;
};

/*!
 * \brief    For FuncT as void type, EmptyFunctionManager is used.
 * \tparam   InvokeAsRRef    The bool type to check if FunctionManger is invoked as R value reference.
 * \tparam   InvokeAsConst   The bool type to check if FunctionManger is invoked as const.
 * \tparam   RetT            Return type of Signature.
 * \tparam   Args            The types in Signature's parameter-list.
 * \vprivate Component private
 */
template <bool InvokeAsRRef, bool InvokeAsConst, typename RetT, typename... Args>
class FunctionManagerStorageSelector<void, InvokeAsRRef, InvokeAsConst, RetT, Args...> {
 public:
  /*!
   * \brief class FunctionManager used in static storage.
   */
  using type = EmptyFunctionManager<InvokeAsRRef, InvokeAsConst, RetT, Args...>;
};

/*!
 * \brief    Expands the argument type pack and provides the fully specified FunctionManager type.
 * \tparam   FuncT      The type of the stored function object. Must fulfill C++ named requirements: FunctionObject.
 * \tparam   SigProps   Signature properties received from signature::GetSigProps.
 * \tparam   Args       The types in Signature's parameter-list.
 * \vprivate Component private
 */
template <typename FuncT, typename SigProps, typename ArgsT>
class FunctionManagerSelectorArgExpander;

/*!
 * \brief    Expands the argument type pack and provides the fully specified FunctionManager type.
 * \tparam   FuncT      The type of the stored function object. Must fulfill C++ named requirements: FunctionObject.
 * \tparam   SigProps   Signature properties received from signature::GetSigProps.
 * \tparam   Args       The types in Signature's parameter-list.
 * \vprivate Component private
 */
template <typename FuncT, typename SigProps, typename... Args>
class FunctionManagerSelectorArgExpander<FuncT, SigProps, signature::SignatureArgs<Args...>> {
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
   * \brief Storage type for callable: FunctionManager/EmptyFunctionManager.
   */
  using type = typename FunctionManagerStorageSelector<FuncT, InvokeAsRRef, InvokeAsConst, typename SigProps::RetT,
                                                       Args...>::type;
};

/*!
 * \brief    Provides the FunctionManager type using some indirections.
 * \tparam   FuncT       The type of the stored function object. Must fulfill C++ named requirements: FunctionObject.
 * \tparam   Signature   The function type of the callable. May be const qualified, may have lvalue/rvalue
 *                       reference qualifier and/or noexcept qualifier.
 * \vprivate Component private
 */
template <typename FuncT, typename Signature>
class FunctionManagerSelector {
  /*!
   * \brief Signature properties.
   */
  using SigProps = signature::GetSigProps<Signature>;

 public:
  /*!
   * \brief Expand arguments to get correct function manager.
   */
  using type = typename FunctionManagerSelectorArgExpander<FuncT, SigProps, typename SigProps::ArgsT>::type;
};

// VECTOR NC VectorC++-V12.0.1: MD_VAC_V12-0-1_onlyDtorImplemented
// VECTOR NC AutosarC++17_10-A12.0.1: MD_VAC_A12-0-1_onlyDtorImplemented
/*!
 * \brief    Holds a statically allocated storage, used for storing a FunctionManager.
 * \details  Manages a pointer to an interface allowing calls and destruction of the function object.
 * \tparam   Signature   The function type of the callable. May be const qualified, may have lvalue/rvalue
 *                       reference qualifier and/or noexcept qualifier.
 * \tparam   Capacity    The maximum size for of the contained callable.
 * \tparam   Alignment   The alignment for contained callable.
 * \vprivate Component private
 */
template <typename Signature, std::size_t Capacity, std::size_t Alignment>
class CallableStorage {
  /*!
   * \brief  FunctionManager for F and signature.
   * \tparam F   The callable type to construct FunctionManager.
   */
  template <typename F>
  using FuncMan = typename FunctionManagerSelector<std::decay_t<F>, Signature>::type;

  /*!
   * \brief Type for FunctionManager Interface.
   */
  using IFuncMan = typename IFunctionManagerSelector<Signature>::type;

  /*!
   * \brief Aligned storage with given capacity and alignment.
   */
  using BufferT = detail::BufferTypeAligned<Capacity + sizeof(void*), Alignment>;

  /*!
   * \brief Buffer used for storing callables on the stack.
   */
  BufferT local_buffer_{};

 protected:
  // VECTOR Next Construct VectorC++-V11.0.2: MD_VAC_V11-0-2_mutableMemberShallBePrivate
  /*!
   * \brief Pointer to a function manager.
   */
  IFuncMan* function_manager_ptr_{new (Storage()) FuncMan<void>()};

 public:
  /*!
   * \brief    Custom destructor to handle raw pointer.
   * \vprivate Component private
   * \spec     requires true; \endspec
   */
  // VCA_VAC_SAFE_DEALLOCATION_RUN_TIME_CHECK
  virtual ~CallableStorage() noexcept {
    assert(function_manager_ptr_ != nullptr);
    // VCA_VAC_VALID_FUNC_CALL_RUN_TIME_CHECK
    function_manager_ptr_->OnDelete();
    function_manager_ptr_ = nullptr;
  }

  /*!
   * \brief     Construct using forwarded arguments in the local_buffer_.
   * \tparam    F      Function object type.
   * \tparam    Args   Construction argument types.
   * \param[in] args   Construction arguments.
   * \vprivate  Component private
   * \spec      requires true; \endspec
   */
  template <typename F, typename... Args>
  void ConstructInStorage(Args&&... args) noexcept {
    function_manager_ptr_ = new (Storage()) FuncMan<F>(std::forward<Args>(args)...);
  }

  /*!
   * \brief    Clear storage.
   * \vprivate Component private
   * \spec     requires true; \endspec
   */
  void Clear() noexcept {
    assert(function_manager_ptr_ != nullptr);
    // VCA_VAC_VALID_FUNC_CALL_RUN_TIME_CHECK
    function_manager_ptr_->OnDelete();
    function_manager_ptr_ = new (Storage()) FuncMan<void>();
  }

  // VECTOR Next Construct AutosarC++17_10-M7.1.2: MD_VAC_M7.1.2_parameterAsReferencePointerToConst
  /*!
   * \brief         Swaps two CallableStorage objects.
   * \param[in,out] other   The CallableStorage to be swapped.
   * \vprivate      Component private
   * \spec          requires true; \endspec
   */
  void Swap(CallableStorage& other) noexcept {
    assert((this->function_manager_ptr_ != nullptr) && (other.function_manager_ptr_ != nullptr));
    bool const this_has_callable{this->HasCallable()};
    bool const other_has_callable{other.HasCallable()};

    // When *this has no callable and other does have, then move contents of other to *this and Clear the other.
    if ((!this_has_callable) && other_has_callable) {
      this->MoveConstruct(other.function_manager_ptr_);
      other.Clear();
    }
    // When *this has callable and other doesn't, then move contents of *this to other and Clear *this.
    else if (this_has_callable && (!other_has_callable)) {
      other.MoveConstruct(this->function_manager_ptr_);
      this->Clear();
    }
    // When both *this and other has callable, then move contents of *this  and other using a temporary storage.
    else {
      CallableStorage tmp_storage{};
      assert(tmp_storage.function_manager_ptr_ != nullptr);
      tmp_storage.CopyConstruct(this->function_manager_ptr_);
      this->MoveConstruct(other.function_manager_ptr_);
      other.MoveConstruct(tmp_storage.function_manager_ptr_);
    }
  }

  /*!
   * \brief     Copy contents of \p other to *this.
   * \param[in] other   IFunctionManager pointer.
   * \vprivate  Component private
   * \spec      requires true; \endspec
   */
  void CopyConstruct(IFuncMan const* other) noexcept {
    assert(function_manager_ptr_ != nullptr);
    // VCA_VAC_VALID_FUNC_CALL_RUN_TIME_CHECK
    other->CopyConstruct(function_manager_ptr_);
  }

  /*!
   * \brief         Move contents of \p other to *this.
   * \param[in,out] other   IFunctionManager pointer.
   * \vprivate      Component private
   * \spec          requires true; \endspec
   */
  void MoveConstruct(IFuncMan* other) noexcept {
    assert(function_manager_ptr_ != nullptr);
    // VCA_VAC_VALID_FUNC_CALL_RUN_TIME_CHECK
    other->MoveConstruct(function_manager_ptr_);
  }

  /*!
   * \brief     Clear the current storage and then copy contents of \p other to *this.
   * \param[in] other   IFunctionManager pointer.
   * \vprivate  Component private
   * \spec      requires true; \endspec
   */
  void CopyAssign(IFuncMan const* other) noexcept {
    Clear();
    assert(function_manager_ptr_ != nullptr);
    // VCA_VAC_VALID_FUNC_CALL_RUN_TIME_CHECK
    other->CopyConstruct(function_manager_ptr_);
  }

  /*!
   * \brief         Clear the current storage and then move contents of \p other to *this.
   * \param[in,out] other   IFunctionManager pointer.
   * \spec          requires true; \endspec
   */
  void MoveAssign(IFuncMan* other) noexcept {
    Clear();
    assert(function_manager_ptr_ != nullptr);
    // VCA_VAC_VALID_FUNC_CALL_RUN_TIME_CHECK
    other->MoveConstruct(function_manager_ptr_);
  }

  /*!
   * \brief    Checks if CallableStorage has callable or not.
   * \return   True if CallableStorage has callable, False otherwise.
   * \vprivate Component private
   * \spec     requires true; \endspec
   */
  auto HasCallable() const noexcept -> bool {
    assert(function_manager_ptr_ != nullptr);
    // VCA_VAC_VALID_FUNC_CALL_RUN_TIME_CHECK
    return function_manager_ptr_->HasCallable();
  }

  /*!
   * \brief    Returns address of the local_buffer_.
   * \return   pointer to local_buffer_.
   * \vprivate Component private
   * \spec     requires true; \endspec
   */
  auto Storage() noexcept -> IFuncMan* {
    // VECTOR NL AutosarC++17_10-A5.2.4: MD_VAC_A5.2.4_reinterpretCast
    return reinterpret_cast<IFuncMan*>(std::addressof(local_buffer_));
  }

  /*!
   * \brief    Returns address of the local_buffer_.
   * \return   const pointer to local_buffer_.
   * \vprivate Component private
   * \spec     requires true; \endspec
   */
  auto StorageConst() const noexcept -> IFuncMan const* {
    // VECTOR NL AutosarC++17_10-A5.2.4: MD_VAC_A5.2.4_reinterpretCast
    return reinterpret_cast<IFuncMan const*>(std::addressof(local_buffer_));
  }
};

/*!
 * \brief   The max default size of a callable stored on the stack.
 * \details Same size requirements as std::function on stack optimization: InplaceFunction should store function
 *          pointers and reference_wrappers in the InplaceFunction memory. sizeof(type) + alignof(type) is used since
 *          this is the space required to store type in FunctionManager.
 */
constexpr static std::size_t const kLocalBufferSize{
    std::max(sizeof(void (*)()) + std::alignment_of<void (*)()>::value,
             sizeof(std::reference_wrapper<char>) + std::alignment_of<std::reference_wrapper<char>>::value)};

}  // namespace inplacefunction

/*!
 * \brief    Check if a callable is small enough to store in the local buffer.
 * \details  Checks if the size required to store callable in FunctionManager is lesser or equal than
 *           detail::BufferTypeAligned<Capacity, Alignment>.
 * \tparam   FuncT       Type of callable.
 * \tparam   Signature   The function type of the callable. May be const qualified, may have lvalue/rvalue
 *                       reference qualifier and/or noexcept qualifier.
 * \vprivate Component private
 */
template <typename FuncT, std::size_t Capacity, std::size_t Alignment>
class CallableFits {
  /*!
   * \brief An alias for BufferTypeAligned<Capacity, Alignment>.
   */
  using BufferT = BufferTypeAligned<Capacity, Alignment>;

 public:
  /*!
   * \brief bool which is True if the callable can be stored in the local buffer, false otherwise.
   */
  static constexpr bool const value{sizeof(FuncT) <= sizeof(BufferT)};
};

}  // namespace detail

/*!
 * \brief    InplaceFunction is non-allocating equivalent of std::function without heap allocation.
 * \details  The callable is stored in static memory allocated using std::aligned_storage with provided Capacity
 *           and alignment. An interface allows this class which is typed only with a signature to call any callable.
 * \tparam   Signature   The function type of the contained callable. May be const qualified, may have lvalue/rvalue
 *                       reference qualifier and/or noexcept qualifier.
 * \tparam   Capacity    the maximum size of aligned_storage for callable, must be greater than 0.
 * \tparam   Alignment   the alignment of aligned_storage for callable, must be the value of alignof(T)
 *                       for some type T.
 * \trace    CREQ-VaCommonLib-InplaceFunction
 * \vprivate Product private
 */
template <typename Signature, std::size_t Capacity = detail::inplacefunction::kLocalBufferSize,
          std::size_t Alignment = std::alignment_of<void*>::value>
class InplaceFunction final : private detail::inplacefunction::StorageAndCallOperator<Signature, Capacity, Alignment> {
  static_assert(Capacity > 0, "vac::language::InplaceFunction: Template parameter Capacity must be greater than 0.");

  // VECTOR NC AutosarC++17_10-A11.3.1: MD_VAC_A11.3.1_doNotUseFriend
  template <typename Sig, std::size_t Cap, std::size_t Align>
  friend class InplaceFunction;
  /*!
   * \brief  Invocability requirement.
   * \tparam F   The type of callable.
   */
  template <typename F>
  using IsInvocableReq = detail::signature::IsInvocableForSig<std::decay_t<F>, Signature>;

  /*!
   * \brief Typedef to privately inherited superclass with storage and operator().
   */
  using StorageT = typename detail::inplacefunction::StorageAndCallOperator<Signature, Capacity, Alignment>;

  /*!
   * \brief Checks if provided callable can be fit in detail::BufferTypeAligned<Capacity, Alignment>.
   */
  template <typename F>
  using IsSmallCallable = detail::CallableFits<F, Capacity, Alignment>;

 public:
  /*!
   * \brief    Default constructor.
   * \vprivate Product private
   * \spec     requires true; \endspec
   */
  InplaceFunction() noexcept = default;

  // VECTOR NC AutosarC++17_10-A12.1.6: MD_VAC_A12.1.6_useInheritingConstructor
  /*!
   * \brief    Constructor for nullptr.
   * \vprivate Product private
   * \spec     requires true; \endspec
   */
  explicit InplaceFunction(std::nullptr_t) noexcept : StorageT{} {}

  // VECTOR Next Construct AutosarC++17_10-A12.8.4: MD_VAC_A12.8.4_moveConstructorShallNotUseCopySemantics
  /*!
   * \brief     Move constructor.
   * \param[in] other   The InplaceFunction object to take ownership of.
   * \vprivate  Product private
   * \spec      requires true; \endspec
   */
  InplaceFunction(InplaceFunction&& other) noexcept : StorageT{} { this->MoveConstruct(other.Storage()); }

  /*!
   * \brief     Move assignment.
   * \param[in] rhs   InplaceFunction is being move assigned from.
   * \return    This InplaceFunction object.
   * \vprivate  Product private
   * \spec      requires true; \endspec
   */
  auto operator=(InplaceFunction&& rhs) & noexcept -> InplaceFunction& {
    this->MoveAssign(rhs.Storage());
    return *this;
  }

  /*!
   * \brief    Assignment to nullptr.
   * \return   This InplaceFunction object.
   * \vprivate Product private
   * \spec     requires true; \endspec
   */
  auto operator=(std::nullptr_t) & noexcept -> InplaceFunction& {
    this->Clear();
    return *this;
  }

  /*!
   * \brief     Copy constructor.
   * \param[in] other   The InplaceFunction object to be copied from.
   * \vprivate  Product private
   * \spec      requires true; \endspec
   */
  InplaceFunction(InplaceFunction const& other) noexcept : StorageT{} { this->CopyConstruct(other.StorageConst()); }

  /*!
   * \brief     Copy assignment.
   * \param[in] rhs   InplaceFunction is being copy assigned from.
   * \return    This InplaceFunction object.
   * \vprivate  Product private
   * \spec      requires true; \endspec
   */
  auto operator=(InplaceFunction const& rhs) & noexcept -> InplaceFunction& {
    this->CopyAssign(rhs.StorageConst());
    return *this;
  }

  /*!
   * \brief    Default destructor.
   * \vprivate Product private
   * \spec     requires true; \endspec
   */
  ~InplaceFunction() noexcept final = default;  // VCA_VAC_SAFE_DEALLOCATION

  /*!
   * \brief     Construct from a callable.
   * \details   The callable must not be of the same type as *this and size of callable must be less than '*this'
   *            capacity. The return value of the callable must be convertible to R when the argument types Args are
   *            supplied.
   * \tparam    F   The type of the stored function object. Must fulfill C++ named requirements: FunctionObject.
   * \param[in] callable   Either a function object or a function pointer.
   * \vprivate  Product private
   * \spec      requires true; \endspec
   */
  template <typename F,
            typename = std::enable_if_t<(!std::is_same<vac::language::remove_cvref_t<F>, InplaceFunction>::value) &&
                                        std::is_trivially_copyable<std::decay_t<F>>::value &&
                                        std::is_constructible<std::decay_t<F>, F>::value && IsInvocableReq<F>::value>>
  explicit InplaceFunction(F&& callable) noexcept : StorageT{} {  // NOLINT(runtime/explicit)
    static_assert(IsSmallCallable<F>::value,
                  "vac::language::InplaceFunction: Size of callable must be less than InplaceFunction::Capacity.");
    ConstructInCallableStorage<F>(std::forward<F>(callable));
  }

  /*!
   * \brief     Move construct from a callable which is a InplaceFunction with same type but different capacity.
   * \details   The callable must not be of the same as *this and the capacity of callable must be less than or equal
   *            to '*this' capacity.
   * \param[in] callable   another InplaceFunction object.
   * \vprivate  Product private
   * \spec      requires true; \endspec
   */
  template <std::size_t Cap, std::size_t Align,
            typename = std::enable_if_t<!std::is_same<
                vac::language::remove_cvref_t<InplaceFunction<Signature, Cap, Align>>, InplaceFunction>::value>>
  explicit InplaceFunction(InplaceFunction<Signature, Cap, Align>&& callable) noexcept : StorageT{} {
    static_assert((Cap <= Capacity),
                  "vac::language::InplaceFunction: Size of callable must be less than InplaceFunction::Capacity.");
    if (callable) {
      this->MoveConstruct(callable.Storage());
    }
  }

  /*!
   * \brief     Copy construct from a callable which is a InplaceFunction with same type but different capacity.
   * \details   The callable must not be of the same as *this and the capacity of callable must be less than or equal
   *            to '*this' capacity.
   * \param[in] callable   another InplaceFunction object.
   * \vprivate  Product private
   * \spec      requires true; \endspec
   */
  template <typename std::size_t Cap, std::size_t Align,
            typename = std::enable_if_t<!std::is_same<
                vac::language::remove_cvref_t<InplaceFunction<Signature, Cap, Align>>, InplaceFunction>::value>>
  explicit InplaceFunction(InplaceFunction<Signature, Cap, Align> const& callable) noexcept : StorageT{} {
    static_assert((Cap <= Capacity),
                  "vac::language::InplaceFunction: Size of callable must be less than InplaceFunction::Capacity.");
    if (callable) {
      this->CopyConstruct(callable.StorageConst());
    }
  }

  /*!
   * \brief     In place construct a callable.
   * \details   The callable must not be of the same type as *this. The return value of the callable must be
   *            convertible to R when the argument types Args are supplied.
   * \tparam    F      The type of the callable.
   * \tparam    Args   The types of the callables.
   * \param[in] args   Arguments for in place construction.
   * \vprivate  Product private
   * \spec      requires true; \endspec
   */
  template <typename F, typename... Args,
            std::enable_if_t<std::is_constructible<std::decay_t<F>, Args...>::value &&
                                 std::is_trivially_copyable<std::decay_t<F>>::value && IsInvocableReq<F>::value,
                             std::int32_t> = 0>
  explicit InplaceFunction(amsr::core::in_place_type_t<F>, Args&&... args) noexcept : StorageT{} {
    static_assert(IsSmallCallable<F>::value,
                  "vac::language::InplaceFunction: Size of callable must be less than InplaceFunction::Capacity.");
    ConstructInCallableStorage<F>(amsr::core::in_place, std::forward<Args>(args)...);
  }

  // VECTOR Next Construct AutosarC++17_10-A13.3.1: MD_VAC_A13.3.1_forwardingFunctionsShallNotBeOverloaded
  /*!
   * \brief     Assign from a callable.
   * \details   The callable must not be of the same type as *this and size of callable must be less than '*this'
   *            capacity. The return value of the callable must be convertible to R when the argument types Args are
   *            supplied.
   * \tparam    F   The type of the callable.
   * \param[in] callable   Either a function object or a function pointer.
   * \return    A new InplaceFunction based on the provided callable.
   * \vprivate  Product private
   * \spec      requires true; \endspec
   */
  template <typename F,
            typename = std::enable_if_t<std::is_trivially_copyable<std::decay_t<F>>::value &&
                                        std::is_constructible<std::decay_t<F>, F>::value && IsInvocableReq<F>::value>>
  auto operator=(F&& callable) & noexcept -> InplaceFunction& {
    static_assert(IsSmallCallable<F>::value,
                  "vac::language::InplaceFunction: Size of callable must be less than InplaceFunction::Capacity.");
    this->MoveAssign(InplaceFunction{std::forward<F>(callable)}.Storage());
    return *this;
  }

  // VECTOR Next Construct AutosarC++17_10-A13.3.1: MD_VAC_A13.3.1_forwardingFunctionsShallNotBeOverloaded
  /*!
   * \brief     Move Assign from a callable which is a InplaceFunction with same type but different capacity..
   * \details   The callable must not be of the same as *this and the capacity of callable must be less than or equal
   *            to '*this' capacity.
   * \param[in] callable   another InplaceFunction object.
   * \return    A new InplaceFunction based on the provided callable.
   * \vprivate  Product private
   * \spec      requires true; \endspec
   */
  template <typename std::size_t Cap, std::size_t Align,
            typename = std::enable_if_t<!std::is_same<
                vac::language::remove_cvref_t<InplaceFunction<Signature, Cap, Align>>, InplaceFunction>::value>>
  auto operator=(InplaceFunction<Signature, Cap, Align>&& callable) & noexcept -> InplaceFunction& {
    static_assert((Cap <= Capacity),
                  "vac::language::InplaceFunction: Size of callable must be less than InplaceFunction::Capacity.");
    this->MoveAssign(callable.Storage());
    return *this;
  }

  // VECTOR Next Construct AutosarC++17_10-A13.3.1: MD_VAC_A13.3.1_forwardingFunctionsShallNotBeOverloaded
  /*!
   * \brief     Copy Assign from a callable which is a InplaceFunction with same type but different capacity.
   * \details   The callable must not be of the same as *this and the capacity of callable must be less than or equal
   *            to '*this' capacity.
   * \param[in] callable   Either a function object or a function pointer.
   * \return    A new InplaceFunction based on the provided callable.
   * \vprivate  Product private
   * \spec      requires true; \endspec
   */
  template <std::size_t Cap, std::size_t Align,
            typename = std::enable_if_t<!std::is_same<
                vac::language::remove_cvref_t<InplaceFunction<Signature, Cap, Align>>, InplaceFunction>::value>>
  auto operator=(InplaceFunction<Signature, Cap, Align> const& callable) & noexcept -> InplaceFunction& {
    static_assert((Cap <= Capacity),
                  "vac::language::InplaceFunction: Size of callable must be less than InplaceFunction::Capacity.");
    this->CopyAssign(callable.StorageConst());
    return *this;
  }

  /*!
   * \brief     Swap the content of *this with the content of \p other.
   * \param[in] other   The other InplaceFunction to swap with.
   * \vprivate  Product private
   * \spec      requires true; \endspec
   */
  void swap(InplaceFunction& other) noexcept { StorageT::Swap(other); }

  /*!
   * \brief    Checks if *this contains a callable or not.
   * \return   True if *this contains a callable, False otherwise.
   * \vprivate Product private
   * \spec     requires true; \endspec
   */
  explicit operator bool() const noexcept {  // Comment to force line break. See TAR-22835.
    return StorageT::HasCallable();
  }

  /*!
   * \brief Call operator with the same const, noexcept and ref qualifiers as Signature for this InplaceFunction.
   */
  using StorageT::operator();

 private:
  /*!
   * \brief     Checks if the callable is a nullptr or not.
   * \tparam    F          The type of the callable.
   * \param[in] callable   The callable to check.
   * \return    True if the callable is a nullptr, False otherwise.
   * \vprivate  Component private
   * \spec      requires true; \endspec
   */
  template <typename F>
  static auto CallableIsNullptr(F&& callable) noexcept -> bool {
    return callable == nullptr;
  }

  /*!
   * \brief     Construct callable in CallableStorage via forwarded arguments.
   * \tparam    F      The type of the callable.
   * \tparam    Args   Construction argument types.
   * \param[in] args   Construction arguments.
   * \vprivate  Component private
   * \spec      requires true; \endspec
   */
  template <typename F, typename... Args,
            std::enable_if_t<(sizeof...(Args) != 1) || (!detail::IsPossiblyNullptr<F>::value), std::int32_t> = 0>
  void ConstructInCallableStorage(Args&&... args) noexcept {
    StorageT::template ConstructInStorage<std::decay_t<F>>(std::forward<Args>(args)...);
  }

  /*!
   * \brief     Construct callable in CallableStorage via forwarded arguments.
   * \tparam    F      The type of the callable.
   * \tparam    Args   Construction argument types.
   * \param[in] args   Construction arguments
   * \vprivate  Component private
   * \spec      requires true; \endspec
   */
  template <typename F, typename... Args,
            std::enable_if_t<(sizeof...(Args) == 1) && detail::IsPossiblyNullptr<F>::value, std::int32_t> = 0>
  void ConstructInCallableStorage(Args&&... args) noexcept {
    if (!CallableIsNullptr(std::forward<Args>(args)...)) {
      StorageT::template ConstructInStorage<std::decay_t<F>>(std::forward<Args>(args)...);
    }
  }
};

/*!
 * \brief         Swaps the content of two InplaceFunction objects.
 * \tparam        Signature   The signature of the contained callables.
 * \param[in,out] lhs   First object to swap with.
 * \param[in,out] rhs   Second object to swap with.
 * \vprivate      Product private
 * \spec          requires true; \endspec
 */
// VECTOR NC AutosarC++17_10-A2.11.4: MD_VAC_A2.11.4_nameOfObjectOrFunctionReused
template <typename Signature, std::size_t Capacity, std::size_t Alignment>
void swap(InplaceFunction<Signature, Capacity, Alignment>& lhs,
          InplaceFunction<Signature, Capacity, Alignment>& rhs) noexcept {
  lhs.swap(rhs);
}

/*!
 * \brief     Compares a InplaceFunction with a NullPtr.
 * \tparam    Signature   The signature of the contained callable.
 * \tparam    Capacity    The capacity of the contained callable.
 * \tparam    Alignment   The alignment of the contained callable.
 * \param[in] lhs   An InplaceFunction object to check.
 * \return    True if the InplaceFunction is a nullptr, False otherwise.
 * \vprivate  Product private
 * \spec      requires true; \endspec
 */
template <typename Signature, std::size_t Capacity, std::size_t Alignment>
auto operator==(InplaceFunction<Signature, Capacity, Alignment> const& lhs, std::nullptr_t) noexcept -> bool {
  return !lhs;
}

/*!
 * \brief     Compares NullPtr with a InplaceFunction.
 * \tparam    Signature   The signature of the contained callable.
 * \tparam    Capacity    The capacity of the contained callable.
 * \tparam    Alignment   The alignment of the contained callable.
 * \param[in] rhs   An InplaceFunction object to check.
 * \return    True if the InplaceFunction is a nullptr, False otherwise.
 * \vprivate  Product private
 * \spec      requires true; \endspec
 */
template <typename Signature, std::size_t Capacity, std::size_t Alignment>
auto operator==(std::nullptr_t, InplaceFunction<Signature, Capacity, Alignment> const& rhs) noexcept -> bool {
  return !rhs;
}

/*!
 * \brief     Compares InplaceFunction with a NullPtr.
 * \tparam    Signature   The signature of the contained callable.
 * \tparam    Capacity    The capacity of the contained callable.
 * \tparam    Alignment   The alignment of the contained callable.
 * \param[in] lhs   An InplaceFunction object to check.
 * \return    True if the InplaceFunction is not a nullptr, False otherwise.
 * \vprivate  Product private
 * \spec      requires true; \endspec
 */
template <typename Signature, std::size_t Capacity, std::size_t Alignment>
auto operator!=(InplaceFunction<Signature, Capacity, Alignment> const& lhs, std::nullptr_t) noexcept -> bool {
  return static_cast<bool>(lhs);
}

/*!
 * \brief     Compares NullPtr with a InplaceFunction.
 * \tparam    Signature   The signature of the contained callable.
 * \tparam    Capacity    The capacity of the contained callable.
 * \tparam    Alignment   The alignment of the contained callable.
 * \param[in] rhs   An InplaceFunction object to check.
 * \return    True if the InplaceFunction is not a nullptr, False otherwise.
 * \vprivate  Product private
 * \spec      requires true; \endspec
 */
template <typename Signature, std::size_t Capacity, std::size_t Alignment>
auto operator!=(std::nullptr_t, InplaceFunction<Signature, Capacity, Alignment> const& rhs) noexcept -> bool {
  return static_cast<bool>(rhs);
}

}  // namespace language
}  // namespace vac

#endif  // LIB_VAC_INCLUDE_VAC_LANGUAGE_INPLACE_FUNCTION_H_
