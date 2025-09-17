// VECTOR SL AutosarC++17_10-A2.9.1: MD_CRYPTO_AutosarC++17_10-A2.9.1
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
/*!        \file  common/type_traits.h
 *        \brief  Collection of type traits used in cryptostack client library.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBCOMMON_INCLUDE_CRYPTO_COMMON_TYPE_TRAITS_H_
#define LIB_LIBCOMMON_INCLUDE_CRYPTO_COMMON_TYPE_TRAITS_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include <vector>

#include "amsr/core/optional.h"
#include "amsr/core/result.h"
#include "amsr/core/string_view.h"
#include "ara/crypto/common/custom_disposable.h"
#include "vac/language/remove_shared_ptr.h"

namespace crypto {
namespace common {

/*!
 * \brief Checks if the given Type is a shared_ptr
 * \tparam T Type to be checked
 * \vprivate Product Private
 */
template <typename T>
class is_shared_pointer : public std::false_type {};

/*!
 * \copydoc is_shared_pointer
 * \vprivate Product Private
 */
template <typename T>
class is_shared_pointer<std::shared_ptr<T>> : public std::true_type {};

/*!
 * \brief Checks if the given Type is a unique_ptr or a unique_ptr using the ara::crypto::CustomDeleter
 * \tparam T Type to be checked
 * \vprivate Product Private
 */
template <typename T>
class is_unique_pointer : public std::false_type {};

/*!
 * \copydoc is_unique_pointer
 * \vprivate Product Private
 */
template <typename T>
class is_unique_pointer<std::unique_ptr<T>> : public std::true_type {};

/*!
 * \copydoc is_unique_pointer
 * \vprivate Product Private
 */
template <typename T>
class is_unique_pointer<std::unique_ptr<T, ara::crypto::CustomDeleter>> : public std::true_type {};

/*!
 * \brief Removes unique_ptr from the given type
 * \tparam T Type to remove unique_ptr from
 * \vprivate Product Private
 */
template <class T>
class remove_uptr {
 public:
  using type = T; /*!< Internal typedef for the trait */
};

/*!
 * \copydoc remove_uptr
 * \vprivate Product Private
 */
template <class T>
class remove_uptr<std::unique_ptr<T, ara::crypto::CustomDeleter>> {
 public:
  using type = T; /*!< Internal typedef for the trait */
};

/*!
 * \copydoc remove_uptr
 * \vprivate Product Private
 */
template <class T>
class remove_uptr<std::unique_ptr<T const, ara::crypto::CustomDeleter>> {
 public:
  using type = T; /*!< Internal typedef for the trait */
};

/*!
 * \brief Checks if a given type is an amsr::core::StringView.
 * \tparam T Type to be checked.
 * \vprivate Product Private
 */
template <typename T>
struct IsStringView : public std::false_type {};

/*!
 * \copydoc crypto::common::IsStringView
 * \vprivate Product Private
 */
template <>
struct IsStringView<amsr::core::StringView> : public std::true_type {};

/*!
 * \brief Checks if a given type is an amsr::core::Result<T>.
 * \tparam T Type to be checked.
 * \vprivate Product Private
 */
template <typename T>
struct IsResult : public std::false_type {};

/*!
 * \copydoc crypto::common::IsResult
 * \vprivate Product Private
 */
template <typename T>
struct IsResult<amsr::core::Result<T>> : public std::true_type {};

/*!
 * \brief Checks if a given type is an amsr::core::Optional<T>.
 * \tparam T Type to be checked.
 * \vprivate Product Private
 */
template <typename T>
struct IsOptional : public std::false_type {};

/*!
 * \copydoc crypto::common::IsOptional
 * \vprivate Product Private
 */
template <typename T>
struct IsOptional<amsr::core::Optional<T>> : public std::true_type {};

// VECTOR NC VectorC++-V11.0.3: MD_CRYPTO_VectorC++-V11-0-3_public_data_members
/*!
 * \brief Extracts the value type from an amsr::core::Result<T>.
 * \tparam T Type to be checked.
 * \vprivate Product Private
 */
template <typename T>
struct RemoveResult {
  /*! Type alias for the contained type of this trait */
  using type = T;
};

// VECTOR NC VectorC++-V11.0.3: MD_CRYPTO_VectorC++-V11-0-3_public_data_members
/*!
 * \copydoc crypto::common::RemoveResult
 * \vprivate Product Private
 */
template <typename T>
struct RemoveResult<amsr::core::Result<T>> {
  /*! Type alias for the contained type of this trait */
  using type = T;
};

// VECTOR NC VectorC++-V11.0.3: MD_CRYPTO_VectorC++-V11-0-3_public_data_members
/*!
 * \brief Extracts the value type from an amsr::core::Optional<T>.
 * \tparam T Type to be checked.
 * \vprivate Product Private
 */
template <typename T>
struct RemoveOptional {
  /*! Type alias for the contained type of this trait */
  using type = T;
};

// VECTOR NC VectorC++-V11.0.3: MD_CRYPTO_VectorC++-V11-0-3_public_data_members
/*!
 * \copydoc crypto::common::RemoveOptional
 * \vprivate Product Private
 */
template <typename T>
struct RemoveOptional<amsr::core::Optional<T>> {
  /*! Type alias for the contained type of this trait */
  using type = T;
};

/*!
 * \brief Checks if a given type is an ara::core::Vector<T>.
 * \tparam T Type to be checked.
 * \vprivate Product Private
 */
template <typename T>
struct IsVector : public std::false_type {};

/*!
 * \copydoc crypto::common::IsVector
 * \vprivate Product Private
 */
template <typename T>
struct IsVector<ara::core::Vector<T>> : public std::true_type {};

// VECTOR NC VectorC++-V11.0.3: MD_CRYPTO_VectorC++-V11-0-3_public_data_members
/*!
 * \brief Extracts the value type from an ara::core::Vector<T>.
 * \tparam T Type to be checked.
 * \vprivate Product Private
 */
template <typename T>
struct RemoveVector {
  /*! Type alias for the contained type of this trait */
  using type = T;
};

// VECTOR NC VectorC++-V11.0.3: MD_CRYPTO_VectorC++-V11-0-3_public_data_members
/*!
 * \copydoc crypto::common::RemoveVector
 * \vprivate Product Private
 */
template <typename T>
struct RemoveVector<ara::core::Vector<T>> {
  /*! Type alias for the contained type of this trait */
  using type = T;
};

/*!
 * \brief Removes unique_ptr and shared_ptr from the given type
 * \tparam T Type to remove unique_ptr and shared_ptr from
 * \vprivate Product Private
 */
template <class T>
using classname_without_smart_ptr = typename std::remove_const<
    typename vac::language::remove_shared_pointer<typename crypto::common::remove_uptr<T>::type>::type>::type;

/*!
 * \brief This struct is a helper to shorten the enable_if statement. It extracts the ClassName from the type
 *        which might be a pointer/reference/const/shared_ptr.
 * \tparam ArgumentType Typename to be processed.
 * \vprivate Product Private
 */
template <class ArgumentType>
using raw_class_name =
    typename std::remove_const<typename vac::language::remove_shared_pointer<typename crypto::common::remove_uptr<
        typename std::remove_reference<typename std::remove_pointer<ArgumentType>::type>::type>::type>::type>::type;

/*!
 * \brief Checks of the given Type implements the crypto::common::Identifiable interface
 * \tparam T Type to be checked
 * \vprivate Product Private
 */
template <typename T>
class is_identifiable : public std::is_base_of<::crypto::common::Identifiable, crypto::common::raw_class_name<T>> {};

/*!
 * \brief Checks if the given type is an rvalue reference to a unique_ptr to an identifiable.
 * \tparam T Type to be checked.
 * \vprivate Product Private
 */
template <typename T>
struct is_identifiable_uptr_rvalue_reference
    : public std::integral_constant<
          bool, std::is_rvalue_reference<T>::value &&
                    crypto::common::is_unique_pointer<typename std::remove_reference<T>::type>::value &&
                    std::is_base_of<
                        crypto::common::Identifiable,
                        typename crypto::common::remove_uptr<typename std::remove_reference<T>::type>::type>::value> {};

/*! Helper trait to implement conjunction */
template <bool...>
struct BoolPack {};

/*!
 * \brief Type trait that yields true if all types are the same.
 * \tparam Ts Types to compare
 */
template <class... Ts>
using Conjunction = std::is_same<BoolPack<true, Ts::value...>, BoolPack<Ts::value..., true>>;

}  // namespace common
}  // namespace crypto

#endif  // LIB_LIBCOMMON_INCLUDE_CRYPTO_COMMON_TYPE_TRAITS_H_
