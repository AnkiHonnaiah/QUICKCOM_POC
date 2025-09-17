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
/*!        \file  ara/crypto/cryp/password_cache.h
 *        \brief  Password secure cache context interface.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBARA_INCLUDE_ARA_CRYPTO_CRYP_PASSWORD_CACHE_H_
#define LIB_LIBARA_INCLUDE_ARA_CRYPTO_CRYP_PASSWORD_CACHE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>

#include "ara/core/string_view.h"
#include "ara/crypto/common/std_api.h"
#include "ara/crypto/cryp/hash_function_ctx.h"
#include "ara/crypto/cryp/key_material.h"
#include "ara/crypto/cryp/password_hash.h"

namespace ara {
namespace crypto {
namespace cryp {

// VECTOR NC AutosarC++17_10-A10.1.1: MD_CRYPTO_AutosarC++17_10-A10.1.1_multiple_inheritance_false_positive
/*!
 * \brief Password secure cache context interface.
 * \vpublic
 */
class PasswordCache : public KeyMaterial, public CustomDisposable {
 public:
  /*!
   * \brief Unique smart pointer of the interface.
   * \vpublic
   */
  using Uptr = std::unique_ptr<PasswordCache, CustomDeleter>;

  /*!
   * \brief Get maximal password length.
   * \return Maximal supported password length (or buffer size) in characters.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual std::size_t GetMaximalLength() const noexcept = 0;

  /*!
   * \brief Get required password length.
   * \return Minimal required password length in characters.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual std::size_t GetRequiredLength() const noexcept = 0;

  // VECTOR NC AutosarC++17_10-M9.3.3: MD_CRYPTO_AutosarC++17_10-M9.3.3_AUTOSAR_API
  /*!
   * \brief Get required password complexity.
   * \return Minimal required password complexity (0 == no requirements).
   * \details Each symbol category in requirements means +1 to the complexity
   *          (f.e.: lower/upper cases, numbers, special symbols).
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual std::uint32_t GetRequiredComplexity() const noexcept = 0;

  // VECTOR NC AutosarC++17_10-M9.3.3: MD_CRYPTO_AutosarC++17_10-M9.3.3_method_can_be_declared_const
  /*!
   * \brief Securely clear the password cache.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual void Clear() noexcept = 0;

  /*!
   * \brief Get actual password length.
   * \return Actual password length in characters.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual std::size_t GetLength() const noexcept = 0;

  /*!
   * \brief Get actual password complexity.
   * \return Actual password complexity level.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual std::uint32_t GetComplexity() const noexcept = 0;

  // VECTOR NC AutosarC++17_10-M9.3.3: MD_CRYPTO_AutosarC++17_10-M9.3.3_AUTOSAR_API
  /*!
   * \brief Reset password context to new value.
   * \param[in] password A new value of the password.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error SecurityErrorDomain::kInvalidInputSize if the new password has a length greater than
   *                                               GetMaximalLength() of this password cache instance.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual ara::core::Result<void> Reset(ara::core::StringView password) noexcept = 0;

  /*!
   * \brief Compare password context with provided value for equality.
   * \param[in] password An external value for comparison.
   * \return true if internally stored and provided passwords are equal.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual bool Compare(ara::core::StringView password) const noexcept = 0;

  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
  /*!
   * \brief Calculate secure hash of the password randomized by a salt.
   * \param[in] hash A hash-function context that should be used for the digest calculation.
   * \param[in] reserved_index An optional index of reserved Object slot that should be used for
   *                          this allocation or default marker, which says to allocate on the heap.
   * \return Unique smart pointer to created password hash object.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error SecurityErrorDomain::kIncompleteArgState if the hash_ctx context is not initialized
   *                                                 by required domain parameters.
   * \error SecurityErrorDomain::kBusyResource if the slot specified by reserved_index is busy yet.
   * \error SecurityErrorDomain::kUnreservedResource if the slot specified by reserved_index was not allocated.
   * \error SecurityErrorDomain::kInsufficientResource if capacity of slot specified by reserved Index
   *                                                   is not enough for placing of the target object
   * \error SecurityErrorDomain::kBadAlloc if (reserved_index == kAllocObjectOnHeap),
   *                                       but allocation memory on the heap is impossible.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual ara::core::Result<PasswordHash::Uptr> SecureHash(
      HashFunctionCtx& hash, ReservedObjectIndex reserved_index = kAllocObjectOnHeap) const noexcept = 0;

  /*!
   * \brief Verifies a password stored in this context to compliance to the provided hash value.
   * \details Before returning from this method the hash_ctx context should be cleaned (from intermediate results)!
   * \param[in] hash_ctx A hash context that should be used for verification.
   * \param[in] password_hash A password hash object containing a reference value.
   * \return true if a password stored in this context matches to provided hash value.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error SecurityErrorDomain::kIncompatibleArguments if the hash algorithms in the hash-function context
   *                                                    hash_ctx and in the PasswordHash object differ.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual ara::core::Result<bool> Verify(HashFunctionCtx& hash_ctx,
                                         PasswordHash const& password_hash) const noexcept = 0;

  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
  // VECTOR NC AutosarC++17_10-M9.3.3: MD_CRYPTO_AutosarC++17_10-M9.3.3_AUTOSAR_API
  /*!
   * \brief Optional method for asking a password in secure manner by a top-most GUI window.
   * \details Implementation of this method must automatically obtain authentic name of the
   *          consumer application (from the Execution Manager) and display it in the window title!
   * \details The password request window must have two buttons: "Ok" and "Cancel".
   * \details It is recommended to call this method in a dedicated thread due to prevent blocking the main thread.
   * \param[in] prompt A prompt message that should be displayed to user.
   * \param[in] repeat If value of this flag is true then password must be entered twice (for confirmation).
   * \return true if password was entered by a user and
   *         false if it was canceled by the user or if the method is not supported at all.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual bool AskUser(ara::core::StringView prompt, bool repeat = false) noexcept = 0;
};

}  // namespace cryp
}  // namespace crypto
}  // namespace ara

#endif  // LIB_LIBARA_INCLUDE_ARA_CRYPTO_CRYP_PASSWORD_CACHE_H_
