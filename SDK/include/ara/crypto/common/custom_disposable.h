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
/*!        \file  ara/crypto/common/custom_disposable.h
 *        \brief  A basic interface of custom disposable objects.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBARA_INCLUDE_ARA_CRYPTO_COMMON_CUSTOM_DISPOSABLE_H_
#define LIB_LIBARA_INCLUDE_ARA_CRYPTO_COMMON_CUSTOM_DISPOSABLE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/crypto/common/std_api.h"
#include "crypto/common/identifiable.h"

namespace ara {
namespace crypto {

/*!
 * \brief A basic interface of custom disposable objects.
 * \vpublic
 */
// VECTOR NC AutosarC++17_10-M10.1.1: MD_CRYPTO_AutosarC++17_10-M10.1.1_diamond
class CustomDisposable : virtual public ::crypto::common::Identifiable {
 public:
  /*!
   * \brief Release allocated memory and other resources.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual void Release() noexcept = 0;

 protected:
  /*!
   * \brief Default constructor.
   * \vprivate Component Private
   */
  CustomDisposable() = default;

  /*!
   * \brief Hides destructor client side code.
   * \vprivate Component Private
   */
  ~CustomDisposable() noexcept override = default;

  /*!
   * \brief Default copy constructor
   * \vprivate Component Private
   */
  CustomDisposable(CustomDisposable const& /*other*/) = default;

  /*!
   * \brief Default move constructor
   * \vprivate Component Private
   */
  CustomDisposable(CustomDisposable&& /*other*/) noexcept = default;

  // VECTOR NC AutosarC++17_10-A10.2.1: MD_CRYPTO_AutosarC++17_10-A10.2.1_hiding_functions
  /*!
   * \brief Deleted move assignment constructor
   * \vprivate Component Private
   */
  CustomDisposable& operator=(CustomDisposable&& /*other*/) & noexcept = delete;

  // VECTOR NC AutosarC++17_10-A10.2.1: MD_CRYPTO_AutosarC++17_10-A10.2.1_hiding_functions
  /*!
   * \brief Default copy assignment constructor
   * \vprivate Component Private
   */
  CustomDisposable& operator=(CustomDisposable const& /*other*/) & = default;

  /*!
   * \brief Hides delete operator from client side code.
   * \param[in] p pointer
   * \vprivate Component Private
   */
  // VECTOR NC AutosarC++17_10-A15.5.1: MD_CRYPTO_AutosarC++17_10-A15.5.1_noexcept_if_appropriate
  static void operator delete(void* p);

  /*!
   * \brief Hides delete operator from client side code.
   * \param[in] p pointer
   * \vprivate Component Private
   */
  // VECTOR NC AutosarC++17_10-A15.5.1: MD_CRYPTO_AutosarC++17_10-A15.5.1_noexcept_if_appropriate
  static void operator delete[](void* p);
};

// VECTOR NC AutosarC++17_10-A15.5.1: MD_CRYPTO_AutosarC++17_10-A15.5.1_noexcept_if_appropriate
// VECTOR NL AutosarC++17_10-M0.3.1: MD_CRYPTO_AutosarC++17_10_M0.3.1_operator_delete
inline void CustomDisposable::operator delete(void* p) { ::operator delete(p); }

// VECTOR NC AutosarC++17_10-A15.5.1: MD_CRYPTO_AutosarC++17_10-A15.5.1_noexcept_if_appropriate
inline void CustomDisposable::operator delete[](void* p) { ::operator delete[](p); }

// VECTOR NC VectorC++-V11.0.3: MD_CRYPTO_VectorC++-V11-0-3_public_data_members
/*!
 * \brief A custom deleter definition.
 * \vprivate Component Private
 */
struct CustomDeleter final {
  /*!
   * \brief Constructor of the Custom Deleter.
   * \vprivate Component Private
   */
  constexpr CustomDeleter() noexcept = default;

  /*!
   * \brief Since the ara::crypto API uses several unique_ptr to const objects (uptrc), this deleter is required to
   *        handle also const pointers. Here a const_cast is necessary, because the method Release() in interface
   *        CustomDisposable is not const qualified.
   * \param[in] ptr A pointer to an instance of the const CustomDisposable interface.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  void operator()(CustomDisposable const* ptr) const noexcept {
    // VECTOR NL AutosarC++17_10-A5.2.3: MD_CRYPTO_AutosarC++17_10-A5.2.3_const_cast
    CustomDisposable* const non_const_ptr{const_cast<CustomDisposable*>(ptr)};
    // VECTOR NL AutosarC++17_10-M0.1.2: MD_CRYPTO_AutosarC++17_10-M0.1.2_branch_dead_false_positive
    if (non_const_ptr != nullptr) {
      non_const_ptr->Release();
    }
  }
};

}  // namespace crypto
}  // namespace ara

#endif  // LIB_LIBARA_INCLUDE_ARA_CRYPTO_COMMON_CUSTOM_DISPOSABLE_H_
