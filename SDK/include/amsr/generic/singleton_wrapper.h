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
/**     \file       singleton_wrapper.h
 *      \brief      Implementations of reference counter, singleton access and singleton.
 *       \unit      VaCommonLib::MemoryManagement::SingletonWrapper
 *
 *********************************************************************************************************************/
#ifndef LIB_VAC_INCLUDE_AMSR_GENERIC_SINGLETON_WRAPPER_H_
#define LIB_VAC_INCLUDE_AMSR_GENERIC_SINGLETON_WRAPPER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <atomic>
#include <cassert>
#include <functional>
#include <limits>
#include <utility>

#include "amsr/core/abort.h"
#include "amsr/core/optional.h"
#include "vac/testing/test_adapter.h"

namespace amsr {
namespace generic {
namespace internal {

/*!
 * \brief   A reference counter class to increase/decrease reference counter.
 * \details Reference counter can make sure the current state is access state instead of initialization or
 *          deinitization. To prevent the accessed data from undefined behavior.
 * \vprivate
 */
class RefCounter final {
 public:
  /*! \brief Alias type for std::size_t.*/
  using CountType = std::size_t;

  /*! \brief Minimum value of reference counter.*/
  static constexpr RefCounter::CountType kMinRef{std::numeric_limits<RefCounter::CountType>::min() + 1};
  /*! \brief Maximum value of reference counter.*/
  static constexpr RefCounter::CountType kMaxRef{std::numeric_limits<RefCounter::CountType>::max() - 1};

  /*!
   * \brief Constructor of RefCounter.
   * \param count A reference counter contains minimum value of CountType.
   * \spec
   *   requires true;
   * \endspec
   */
  constexpr explicit RefCounter(CountType count = kMinRef) noexcept : ref_count_{count} {
    assert((count >= kMinRef) && (count <= kMaxRef));
  }

  // VECTOR Disable AutosarC++17_10-M3.2.4: MD_VAC_M3.2.4_functionWithoutDefinition
  /*!
   * \brief      Get the value of reference counter.
   * \threadsafe TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  CountType GetRefCount() const& noexcept;

  /*!
   * \brief      Set the value for reference counter.
   * \param      count The count to set reference counter.
   * \threadsafe TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  void SetRefCount(CountType count) noexcept;

  /*!
   * \brief      Check if is still referenced.
   * \threadsafe TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  bool HasReferences() const& noexcept;

  /*!
   * \brief      Increase the reference counter, abort when counter is overflowed.
   * \threadsafe TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  void AddRef() & noexcept;

  /*!
   * \brief      Decrease the reference counter.
   * \threadsafe TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  void RemoveRef() & noexcept;
  // VECTOR Enable AutosarC++17_10-M3.2.4

 private:
  /*! \brief Reference counter for static instance */
  std::atomic<CountType> ref_count_;
};

}  // namespace internal

// VECTOR Next Construct AutosarC++17_10-A11.3.1: MD_VAC_A11.3.1_doNotUseFriend
/*!
 * \brief   Access the singleton object and reference counter.
 * \tparam  T The type of the singleton object. T has the same requirements as T in class Singleton.
 * \vprivate Product private
 */
template <typename T>
class SingletonAccess final {
  /*! \brief Friend declaration for test requiring access to RefCounter. */
  template <typename>
  FRIEND_TEST(UT__SingletonWrapper__MultiTypes, AddReference);
  /*! \brief Friend declaration for test requiring access to RefCounter. */
  FRIEND_TEST(UT__SingletonWrapper, ConversionOperator);

 public:
  /*! \brief Default constructor */
  SingletonAccess() noexcept = delete;

  /*!
   * \brief Parameterized constructor.
   * \param obj An object for static instance.
   * \param ref_counter Reference counter for static instance.
   * \vprivate Product private
   * \spec
   *   requires true;
   * \endspec
   */
  SingletonAccess(T& obj, internal::RefCounter& ref_counter) noexcept : obj_{obj}, ref_counter_{ref_counter} {}

  /*!
   * \brief     Move constructor.
   * \details   While it may look like the content of 'other' is moved, it is actually copied to this SingletonAccess
   *            (moving a std::reference_wrapper has the same effect as copying it). The 'std::move()' are only there
   *            to satisfy Bauhaus.
   *            Since a SingletonAccess does not own the referenced data, there is no harm in copying.
   * \param[in] other The SingletonAccess to move from.
   * \vprivate  Product private
   * \spec
   *   requires true;
   * \endspec
   */
  SingletonAccess(SingletonAccess&& other) noexcept
      : obj_{std::move(other.obj_)}, ref_counter_{std::move(other.ref_counter_)} {
    ref_counter_.get().AddRef();  // VCA_VAC_VALID_NONSTATIC_METHOD
  }

  /*!
   * \brief Copy constructor.
   * \param other The SingletonAccess to copy from.
   * \vprivate Product private
   * \spec
   *   requires true;
   * \endspec
   */
  SingletonAccess(SingletonAccess const& other) noexcept : obj_{other.obj_}, ref_counter_{other.ref_counter_} {
    ref_counter_.get().AddRef();  // VCA_VAC_VALID_NONSTATIC_METHOD
  }

  /*!
   * \brief  Move Assignment.
   * \return A reference to the object.
   * \spec
   *   requires true;
   * \endspec
   */
  SingletonAccess& operator=(SingletonAccess&&) & noexcept { return *this; }

  /*!
   * \brief Copy Assignment.
   * \return A reference to the object.
   * \spec
   *   requires true;
   * \endspec
   */
  SingletonAccess& operator=(SingletonAccess const&) & noexcept { return *this; }

  // VECTOR NC AutosarC++17_10-A13.5.2: MD_VAC_A13.5.2_explicitUserDefinedConversionOperatorFalsePositive
  /*!
   * \brief  Allow explicit conversion from T to a base of T.
   * \tparam U A base of T.
   * \return A SingletonAccess<U> instance created from this.
   * \spec
   *    requires true;
   * \endspec
   */
  template <typename U, typename = typename std::enable_if_t<std::is_base_of<U, T>::value>>
  explicit operator SingletonAccess<U>() const noexcept {
    ref_counter_.get().AddRef();                   // VCA_VAC_VALID_NONSTATIC_METHOD
    SingletonAccess<U> other{obj_, ref_counter_};  // VCA_VAC_VALID_ARGUMENT
    return other;
  }

  // VECTOR NC AutosarC++17_10-M9.3.3: MD_VAC_M9.3.3_methodCanBeDeclaredConstOperator
  /*!
   * \brief  Access the contained value.
   * \return Pointer to the contained object.
   * \spec
   *   requires true;
   * \endspec
   */
  T* operator->() noexcept { return &obj_.get(); }

  // VECTOR NC AutosarC++17_10-M9.3.3: MD_VAC_M9.3.3_methodCanBeDeclaredConstOperator
  /*!
   * \brief  Dereference to access contained value.
   * \return Reference to the contained object.
   * \spec
   *   requires true;
   * \endspec
   */
  T& operator*() noexcept { return obj_.get(); }

  /*!
   * \brief Destructor.
   * \spec
   *   requires true;
   * \endspec
   */
  ~SingletonAccess() noexcept { ref_counter_.get().RemoveRef(); }  // VCA_VAC_VALID_NONSTATIC_METHOD

  /*!
   * \brief      Get reference counter.
   * \details    Used for tests in libvac. Other usage should be avoided.
   * \return     Reference counter.
   * \threadsafe TRUE
   * \vprivate   Component private
   * \spec
   *   requires true;
   * \endspec
   */
  std::size_t SingletonAccessGetRefCount() const noexcept {
    return ref_counter_.get().GetRefCount();  // VCA_VAC_VALID_NONSTATIC_METHOD
  }

 private:
  /*!
   * \brief   The object wrapped by the Singleton from which this SingletonAccess is derived.
   * \details Realized as reference_wrapper, because the referenced object is owned by the Singleton object.
   */
  std::reference_wrapper<T> obj_;

  /*!
   * \brief   The reference counter of the Singleton from which this SingletonAccess is derived.
   * \details Realized as reference_wrapper, because the ref counter is owned by the Singleton object.
   */
  std::reference_wrapper<internal::RefCounter> ref_counter_;
};

/*!
 * \brief   Const type of singleton access.
 * \tparam  T The type of the singleton object.
 * \vprivate Product private
 */
template <typename T>
using SingletonConstAccess = SingletonAccess<typename std::add_const<T>::type>;

/*!
 * \brief   Singleton wrapper makes initialization and deinitialization in correct order.
 * \details Uninitialized object can emplace a value by Create() in initialization state.
 *          Singleton object can be accessed after it is initialized in access state.
 *          Initialized object can be destroyed by Destroy() in deinitialization state.
 * \tparam  T The type of the singleton object. T must be constructible.
 * \trace   DSGN-VaCommonLib-SingletonWrapper
 * \vprivate Product private
 */
template <typename T>
class Singleton {
 public:
  /*!
   * \brief      Check if the state is initialized.
   * \return     True if state is equal to initialized, otherwise false.
   * \threadsafe TRUE
   * \vprivate Product private
   * \spec
   *   requires true;
   * \endspec
   */
  bool IsInitialized() const {  // Comment to force line break. See TAR-22835.
    return data_.init_state_ == InitState::initialized;
  }

  /*!
   * \brief      Increase reference counter and check if the state is initialized, otherwise abort.
   * \return     Contained value and incremented reference counter.
   * \pre        State is InitState::initialized.
   * \threadsafe TRUE
   * \vprivate Product private
   */
  SingletonAccess<T> GetAccess() {
    data_.ref_counter_.AddRef();
    if (!IsInitialized()) {
      ara::core::Abort("amsr::generic::Singleton::GetAccess(): Invalid access!");
    }
    return SingletonAccess<T>{data_.opt_.value(), data_.ref_counter_};
  }

  /*!
   * \brief      Increase reference counter and check if the state is initialized, otherwise abort.
   * \return     Contained value and incremented reference counter.
   * \pre        State is InitState::initialized.
   * \threadsafe TRUE
   * \vprivate Product private
   */
  SingletonConstAccess<T> GetConstAccess() {
    data_.ref_counter_.AddRef();
    if (!IsInitialized()) {
      ara::core::Abort("amsr::generic::Singleton::GetConstAccess(): Invalid access!");
    }
    return SingletonConstAccess<T>{data_.opt_.value(), data_.ref_counter_};
  }

  /*!
   * \brief      Create a static instance and set current state to initialized.
   * \details    Emplace a value if the state is unintialized, otherwise abort.
   * \tparam     Args The types of arguments given to this function.
   * \param      args Arguments to be emplaced into the data.
   * \pre        State is InitState::uninitialized.
   * \threadsafe TRUE
   * \vprivate Product private
   */
  template <typename... Args>
  void Create(Args&&... args) noexcept {
    InitState const status{data_.init_state_.exchange(InitState::changing)};
    if (status != InitState::uninitialized) {
      ara::core::Abort("amsr::generic::Singleton::Create(): Concurrent init state change!");
    }
    data_.opt_.emplace(std::forward<Args>(args)...);
    data_.init_state_ = InitState::initialized;
  }

  /*!
   * \brief      Destroy a static instance and reset current state to uninitialized.
   * \details    Abort when the state is uninitialized or when reference counter is still referenced.
   * \pre        State is InitState::initialized and not used anymore (SingletonAccessGetRefCount() == 0).
   * \threadsafe TRUE
   * \vprivate Product private
   */
  void Destroy() {
    InitState const status{data_.init_state_.exchange(InitState::changing)};
    if (status != InitState::initialized) {
      ara::core::Abort("amsr::generic::Singleton::Destroy(): Concurrent init state change!");
    }
    if (data_.ref_counter_.HasReferences()) {
      ara::core::Abort("amsr::generic::Singleton::Destroy(): Still referenced!");
    }
    data_.opt_.reset();
    data_.init_state_ = InitState::uninitialized;
  }

 private:
  /*!
   * \brief The list of current state of static instance.
   */
  enum class InitState : std::uint8_t { uninitialized, changing, initialized };

  /*!
   * \brief   A structure to define current state, optional and reference counter.
   * \details Static optional is used to prevent dependency to other static storage variables,
   */
  struct Data {
    /*! \brief The initial state of static instance is assigned as uninitialized */
    std::atomic<InitState> init_state_{InitState::uninitialized};

    /*! \brief The container to store the data of ref_counter_ */
    ara::core::Optional<T> opt_;

    /*! \brief Reference counter for static instance */
    internal::RefCounter ref_counter_;
  };

  /*!
   * \brief The contained data for init_state_, opt_ and ref_counter_.
   */
  Data data_;
};

}  // namespace generic
}  // namespace amsr

#endif  // LIB_VAC_INCLUDE_AMSR_GENERIC_SINGLETON_WRAPPER_H_
