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
/*!        \file  polymorphic_value.h
 *        \brief  Contains the PolymorphicValue class implementation.
 *         \unit  VaCommonLib::ContainerLibrary::UnionContainers::PolymorphicValue
 *
 *********************************************************************************************************************/

#ifndef LIB_VAC_INCLUDE_VAC_CONTAINER_POLYMORPHIC_VALUE_H_
#define LIB_VAC_INCLUDE_VAC_CONTAINER_POLYMORPHIC_VALUE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include <type_traits>
#include <utility>
#include "ara/core/abort.h"
#include "ara/core/variant.h"
#include "vac/language/cpp17_backport.h"
#include "vac/testing/test_adapter.h"

namespace vac {
namespace container {

/*!
 * \brief   A polymorphic object with value semantics.
 * \details A polymorphic object that supports copying without slicing.
 *          The tparam TBase and ... TDerived will be stored as ara::core::Variant
 *          container alternatives and must fulfill ara::core::Variant alternatives type requirements
 * \tparam  TBase Type of the base class from which TDerived must derive from.
 * \tparam  TDerived Types of the derived classes. Must derive from TBase.
 * \trace   CREQ-VaCommonLib-PolymorphicValue
 */
template <typename TBase, typename... TDerived>
class PolymorphicValue final {
  // Assertions.
  static_assert(language::compile_time::all<std::is_base_of<TBase, TDerived>::value...>::value,
                "PolymorphicValues must derive from a common base");

  /*! \brief Base type. */
  using base_type = TBase;
  /*! \brief Base type pointer. */
  using base_pointer_type = typename std::add_pointer<base_type>::type;
  /*! \brief Base type pointer. */
  using const_base_pointer_type = typename std::add_pointer<base_type const>::type;
  /*! \brief Base type reference. */
  using base_reference_type = typename std::add_lvalue_reference<base_type>::type;
  /*! \brief Base type reference. */
  using const_base_reference_type = typename std::add_lvalue_reference<base_type const>::type;
  /*! \brief Type used to store the object. base_pointer_type is stored as variant to store nullptr. */
  using storage_type = ara::core::Variant<base_pointer_type, TDerived...>;

 public:
  // VECTOR Disable AutosarC++17_10-A12.1.5: MD_VAC_A12.1.5_useDelegatingConstructor
  /*!
   * \brief   Default construction.
   * \details The default constructor exploits that default construction of a variant is supported (only) for its first
   *          specialization, in our case this is always the base _pointer_ type - which is initialized to nullptr.
   * \pre     -
   * \spec
   *   requires true;
   * \endspec
   */
  PolymorphicValue() noexcept : base_ptr_(nullptr), storage_(nullptr) {}

  /*!
   * \brief     Construction from value.
   * \tparam    T Type of the value from which to construct.
   * \param[in] value The value from which to construct.
   * \pre    -
   * \spec
   *   requires true;
   * \endspec
   */
  template <typename T, typename = typename std::enable_if<
                            !std::is_same<typename std::decay<T>::type, PolymorphicValue>::value>::type>
  explicit PolymorphicValue(T&& value) noexcept : base_ptr_(nullptr), storage_(std::forward<T>(value)) {
    base_ptr_ = storage_.template get_if<T>();
  }

  /*!
   * \brief Copy constructor.
   * \param other The variant from which to construct.
   * \pre   -
   * \spec
   *   requires true;
   * \endspec
   */
  PolymorphicValue(PolymorphicValue const& other) noexcept : base_ptr_(nullptr), storage_(other.storage_) {
    if (!other.empty()) {
      fix_base_ptr();
    }
  }

  // VECTOR Next Construct AutosarC++17_10-A12.8.1: MD_VAC_A12.8.1_moveConstructorHasSideEffect
  /*!
   * \brief     Move constructor.
   * \param[in] other The variant from which to construct.
   * \pre       -
   * \spec
   *   requires true;
   * \endspec
   */
  PolymorphicValue(PolymorphicValue&& other) noexcept : base_ptr_(nullptr), storage_(std::move(other.storage_)) {
    if (!other.empty()) {
      other.base_ptr_ = nullptr;  // assure other is completely moved.
      fix_base_ptr();
    }
  }

  /*!
   * \brief   Destructor.
   * \pre     -
   * \spec
   *   requires true;
   * \endspec
   * \details Will call destructor of contained value.
   */
  ~PolymorphicValue() noexcept { base_ptr_ = nullptr; }

  // VECTOR Next Construct AutosarC++17_10-A13.3.1: MD_VAC_A13.3.1_forwardingFunctionsShallNotBeOverloaded
  /*!
   * \brief  Copy assignment.
   * \param  other The value that is to be copied.
   * \return A reference on the copied-to variant.
   * \pre    -
   * \spec
   *   requires true;
   * \endspec
   */
  PolymorphicValue& operator=(PolymorphicValue const& other) & noexcept {
    base_ptr_ = nullptr;  // In case of exception keep empty.
    this->storage_ = other.storage_;
    if (!other.empty()) {
      fix_base_ptr();
    }
    return *this;
  }

  // VECTOR Next Construct AutosarC++17_10-A13.3.1: MD_VAC_A13.3.1_forwardingFunctionsShallNotBeOverloaded
  /*!
   * \brief  Store an object into the PolymorphicValue by copy.
   * \tparam T Type of the value that is to be set.
   * \param  value The value that is to be set.
   * \return A reference on the copied-to PolymorphicValue.
   * \pre    -
   * \spec
   *   requires true;
   * \endspec
   */
  template <typename T, typename = std::enable_if_t<!std::is_same<std::decay_t<T>, PolymorphicValue>::value>>
  PolymorphicValue& operator=(T const& value) & noexcept {
    base_ptr_ = nullptr;  // In case of exception keep empty.
    storage_.template emplace<T>(value);
    base_ptr_ = storage_.template get_if<T>();
    return *this;
  }

  // VECTOR Next Construct AutosarC++17_10-A13.3.1: MD_VAC_A13.3.1_forwardingFunctionsShallNotBeOverloaded
  /*!
   * \brief     Move assignment.
   * \param[in] other The value that is to be moved.
   * \return    A reference on the moved-to variant.
   * \pre       -
   * \spec
   *   requires true;
   * \endspec
   */
  PolymorphicValue& operator=(PolymorphicValue&& other) & noexcept {
    base_ptr_ = nullptr;  // In case of exception keep empty.
    std::swap(storage_, other.storage_);
    std::swap(base_ptr_, other.base_ptr_);
    if (!empty()) {
      fix_base_ptr();
    }
    return *this;
  }

  // VECTOR Next Construct AutosarC++17_10-A13.3.1: MD_VAC_A13.3.1_forwardingFunctionsShallNotBeOverloaded
  /*!
   * \brief     Move an object into the PolymorphicValue.
   * \tparam    T Type of the value that is to be set.
   * \param[in] value The value that is to be set.
   * \return    A reference on the copied-to PolymorphicValue.
   * \pre       -
   * \spec
   *   requires true;
   * \endspec
   */
  template <typename T, typename = std::enable_if_t<!std::is_same<std::decay_t<T>, PolymorphicValue>::value>>
  PolymorphicValue& operator=(T&& value) & noexcept {
    using reference_removed_type = std::remove_reference_t<T>;
    base_ptr_ = nullptr;  // In case of exception keep empty.
    storage_.template emplace<reference_removed_type>(std::forward<T>(value));
    base_ptr_ = storage_.template get_if<reference_removed_type>();
    return *this;
  }

  /*!
   * \brief     Emplacement by forwarding.
   * \tparam    T Type of the constructed element.
   * \tparam    TArgs The input parameter types for the constructed element.
   * \param[in] args The arguments to pass to a constructor.
   * \return    Reference to the constructed element.
   * \pre       -
   * \spec
   *   requires true;
   * \endspec
   */
  template <typename T, typename... TArgs>
  T& emplace(TArgs&&... args) noexcept {
    base_ptr_ = nullptr;  // In case of exception keep empty.
    storage_.template emplace<T>(std::forward<TArgs>(args)...);
    base_ptr_ = storage_.template get_if<T>();
    return *storage_.template get_if<T>();  // VCA_VAC_VALID_RETURN_VALUE
  }

  /*!
   * \brief  Operator* const.
   * \return Base class reference to the stored object.
   * \spec
   *   requires true;
   * \endspec
   * \trace  CREQ-VaCommonLib-PolymorphicValue
   */
  const_base_reference_type operator*() const noexcept {
    if (empty()) {
      // Abort to be consistent with get<T> while T is not currently stored.
      ara::core::Abort("Bad Variant access.");
    }
    return *base_ptr_;
  }

  // VECTOR Disable AutosarC++17_10-A5.2.3: MD_VAC_A5.2.3_constCastReducesCodeDuplication
  // VECTOR Next Construct AutosarC++17_10-M9.3.3: MD_VAC_M9.3.3_methodCanBeDeclaredConst
  /*!
   * \brief  Operator*.
   * \return Base class reference to the stored object.
   * \spec
   *   requires true;
   * \endspec
   */
  base_reference_type operator*() noexcept {
    return const_cast<base_reference_type>(static_cast<PolymorphicValue const*>(this)->operator*());
  }
  // VECTOR Enable AutosarC++17_10-A5.2.3

  // VECTOR Next Construct AutosarC++17_10-M9.3.3: MD_VAC_M9.3.3_methodCanBeDeclaredConst
  /*!
   * \brief  Operator ->.
   * \return Base class pointer to the stored object.
   * \spec
   *   requires true;
   * \endspec
   * \trace  CREQ-VaCommonLib-PolymorphicValue
   */
  base_pointer_type operator->() noexcept { return base_ptr_; }

  /*!
   * \brief  Operator -> const.
   * \return Base class pointer to the stored object.
   * \spec
   *   requires true;
   * \endspec
   */
  const_base_pointer_type operator->() const noexcept { return base_ptr_; }

  /*!
   * \brief  Get the base class reference.
   * \return Base class reference to the stored object.
   * \spec
   *   requires true;
   * \endspec
   */
  base_reference_type get() noexcept { return this->operator*(); }

  /*!
   * \brief  Get the base class reference, const version.
   * \return Base class reference to the stored object.
   * \spec
   *   requires true;
   * \endspec
   */
  const_base_reference_type get() const noexcept { return this->operator*(); }

  /*!
   * \brief  Get a derived class reference.
   * \tparam T Type of the derived class.
   * \return Reference to the stored object.
   * \spec
   *   requires true;
   * \endspec
   */
  template <typename T>
  auto get(std::enable_if_t<not std::is_same<std::decay_t<T>, TBase>::value> const* = nullptr) noexcept
      -> std::add_lvalue_reference_t<T> {
    static_assert(language::compile_time::any<std::is_same<T, TDerived>::value...>::value, "Must use a derived type");
    return ara::core::get<T>(storage_);  // VCA_VAC_VALID_RETURN_VALUE_RUN_TIME_CHECK
  }

  /*!
   * \brief  Get a derived class reference, const version.
   * \tparam T Type of the derived class.
   * \return Reference to the stored object.
   * \spec
   *   requires true;
   * \endspec
   */
  template <typename T>
  auto get(std::enable_if_t<not std::is_same<std::decay_t<T>, TBase>::value> const* = nullptr) const noexcept
      -> std::add_lvalue_reference_t<T const> {
    static_assert(language::compile_time::any<std::is_same<T, TDerived>::value...>::value, "Must use a derived type");
    return ara::core::get<T>(storage_);  // VCA_VAC_VALID_RETURN_VALUE_RUN_TIME_CHECK
  }

  /*!
   * \brief  Get the base class reference.
   * \tparam T Type of the base class.
   * \return Base class reference to the stored object.
   * \spec
   *   requires true;
   * \endspec
   */
  template <typename T>
  auto get(std::enable_if_t<std::is_same<std::decay_t<T>, TBase>::value> const* = nullptr) noexcept
      -> std::add_lvalue_reference_t<T> {
    return **this;
  }

  /*!
   * \brief  Get the base class reference, const version.
   * \tparam T Type of the base class.
   * \return Base class reference to the stored object.
   * \spec
   *   requires true;
   * \endspec
   */
  template <typename T>
  auto get(std::enable_if_t<std::is_same<std::decay_t<T>, TBase>::value> const* = nullptr) const noexcept
      -> std::add_lvalue_reference_t<T const> {
    return **this;
  }

  /*!
   * \brief  Get the base class reference.
   * \return Base class reference to the stored object or nullptr if no object is stored.
   * \spec
   *   requires true;
   * \endspec
   */
  base_pointer_type get_if() noexcept { return base_ptr_; }

  /*!
   * \brief  Get the base class reference, const version.
   * \return Base class reference to the stored object or nullptr if no object is stored.
   * \spec
   *   requires true;
   * \endspec
   */
  const_base_pointer_type get_if() const noexcept { return base_ptr_; }

  // VECTOR Next Construct AutosarC++17_10-M9.3.3: MD_VAC_M9.3.3_methodCanBeDeclaredConst
  /*!
   * \brief  Get a non-owning base class pointer.
   * \tparam T Type of the base class.
   * \return Base class reference to the stored object or nullptr if no object is stored.
   * \spec
   *   requires true;
   * \endspec
   */
  template <typename T>
  auto get_if(std::enable_if_t<std::is_same<std::decay_t<T>, TBase>::value> const* = nullptr) noexcept
      -> base_pointer_type {
    return base_ptr_;
  }

  /*!
   * \brief  Get a non-owning base class pointer, const version.
   * \tparam T Type of the base class.
   * \return Base class reference to the stored object, or nullptr if no object is stored.
   * \spec
   *   requires true;
   * \endspec
   */
  template <typename T>
  auto get_if(std::enable_if_t<std::is_same<std::decay_t<T>, TBase>::value> const* = nullptr) const noexcept
      -> const_base_pointer_type {
    return base_ptr_;
  }

  /*!
   * \brief  Get a non-owning pointer to the requested derived class.
   * \tparam T Type of the derived class.
   * \return Pointer to the stored class or nullptr if no or a different derived class is stored.
   * \spec
   *   requires true;
   * \endspec
   */
  template <typename T>
  auto get_if(std::enable_if_t<not std::is_same<std::decay_t<T>, TBase>::value> const* = nullptr) noexcept
      -> std::add_pointer_t<T> {
    static_assert(language::compile_time::any<std::is_same<T, TDerived>::value...>::value, "Must use a derived type");
    return storage_.template get_if<T>();
  }

  /*!
   * \brief  Get a non-owning pointer to the requested derived class, const version.
   * \tparam T Type of the derived class.
   * \return Pointer to the stored class or nullptr if no or a different derived class is stored.
   * \spec
   *   requires true;
   * \endspec
   */
  template <typename T>
  auto get_if(std::enable_if_t<not std::is_same<std::decay_t<T>, TBase>::value> const* = nullptr) const noexcept
      -> std::add_pointer_t<T const> {
    static_assert(language::compile_time::any<std::is_same<T, TDerived>::value...>::value, "Must use a derived type");
    return storage_.template get_if<T>();
  }

  /*!
   * \brief  Test whether a concrete derived object is stored.
   * \pre    -
   * \tparam T Type of the derived object.
   * \return True if the stored object is of the given type, otherwise false.
   * \spec
   *   requires true;
   * \endspec
   */
  template <typename T>
  bool holds_alternative() const noexcept {
    static_assert(language::compile_time::any<std::is_same<T, TDerived>::value...>::value, "Must use a derived type");
    return ara::core::holds_alternative<T>(storage_);
  }

  /*!
   * \brief  Test if any object is stored.
   * \pre    -
   * \spec
   *   requires true;
   * \endspec
   * \return True there is no stored object, otherwise false.
   */
  bool empty() const noexcept { return base_ptr_ == nullptr; }

  /*!
   * \brief Clear the polyobject.
   * \pre   -
   * \spec
   *   requires true;
   * \endspec
   */
  void reset() noexcept {
    base_ptr_ = nullptr;
    storage_.template emplace<base_pointer_type>(nullptr);
  }

 private:
  /*!
   * \brief Set up base type pointer.
   */
  class get_base_ptr_op final {
   public:
    /*!
     * \brief     Returns pointer to a derived class.
     * \param[in] variant The variant to store the object that base pointer point to.
     * \tparam    Type The type of the derived class.
     * \return    Return the reference of variant.
     * \spec
     *   requires true;
     * \endspec
     */
    template <typename Type>
    auto operator()(Type& variant) noexcept -> base_pointer_type {
      return &variant;
    }
    /*! \brief     Returns base pointer.
     *  \param[in] variant The variant to store the object that base pointer point to.
     *  \return    Return variant.
     *  \spec
     *    requires true;
     *  \endspec
     */
    base_pointer_type operator()(base_pointer_type variant) noexcept { return variant; }
  };

  /*!
   * \brief Set up base type pointer.
   * \pre   -
   * \spec
   *   requires true;
   * \endspec
   */
  void fix_base_ptr() noexcept { base_ptr_ = ara::core::visit(get_base_ptr_op{}, storage_); }

 private:
  /*!
   * \brief Pointer to an object, or null.
   */
  base_pointer_type base_ptr_;
  /*!
   * \brief Storage for a polymorphic value.
   */
  storage_type storage_;

  /*! Friend declaration for testing purposes */
  FRIEND_TEST(UT__PolymorphicValue, GetBasePtrOp);
};

}  // namespace container
}  // namespace vac

#endif  // LIB_VAC_INCLUDE_VAC_CONTAINER_POLYMORPHIC_VALUE_H_
