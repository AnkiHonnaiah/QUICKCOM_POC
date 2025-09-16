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
/*!        \file  ara/crypto/cryp/key.h
 *        \brief  Generalized Key object interface.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBARA_INCLUDE_ARA_CRYPTO_CRYP_KEY_H_
#define LIB_LIBARA_INCLUDE_ARA_CRYPTO_CRYP_KEY_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>

#include "ara/crypto/common/base_id_types.h"
#include "ara/crypto/common/std_api.h"
#include "ara/crypto/cryp/key_material.h"
#include "ara/crypto/cryp/key_type.h"
#include "ara/crypto/cryp/keyed_context.h"
#include "ara/crypto/cryp/restricted_use_object.h"

namespace ara {
namespace crypto {
namespace cryp {

// VECTOR NC AutosarC++17_10-A10.1.1: MD_CRYPTO_AutosarC++17_10-A10.1.1_multiple_inheritance_false_positive
/*!
 * \brief Generalized Key object interface.
 * \details An implementation can filter allowed key values at generation/derivation time in order
 *          to prevent production of "weak" and "semi-weak" keys patterns specific to concrete algorithm.
 * \vpublic
 */
class Key : public KeyMaterial, public RestrictedUseObject {
 public:
  /*!
   * \brief Unique smart pointer of the interface.
   * \vpublic
   */
  using Uptrc = std::unique_ptr<Key const, CustomDeleter>;

  /*!
   * \brief Check compatibility of this key with an algorithm specified by an ID.
   * \param[in] alg_id Target Algorithm ID.
   * \return true if the key is compatible with the algorithm specified by the alg_id and false otherwise.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual bool IsCompatible(AlgId alg_id) const noexcept = 0;

  /*!
   * \brief Check compatibility of this key with a crypto transformation configured in the keyed context.
   * \param[in] context Target keyed context.
   * \return true if the key is compatible with the transformation of the context and false otherwise.
   * \details This method compares not only the Crypto Primitive IDs, but also the \a COUID of domain parameters
   *       objects associated with the key object and with provided context! If the COUIDs differ then this
   *       method returns \c false.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual bool IsCompatible(KeyedContext const& context) const noexcept = 0;

  /*!
   * \brief Get actual type of this key object.
   * \return Identifier of the top-level interface type of the key.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual KeyType GetKeyType() const noexcept = 0;

  /*!
   * \brief Get publicity of the key.
   * \return \c true if the key is public and false if the key is private or secret.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual bool IsPublic() const noexcept = 0;

 protected:
  /*!
   * \brief Downcast and move unique smart pointer of the base \c Key interface to concrete derived key type.
   * \param[in] key  unique smart pointer to the constant \c Key interface
   * \return unique smart pointer to downcasted constant interface of specified derived key type
   * \error SecurityErrorDomain::kBadObjectType  if an actual type of the \c key is not the specified
   *                derived key type
   * \vpublic
   */
  template <class DerivedKey>
  static auto Downcast(Key::Uptrc&& key) noexcept -> ara::core::Result<typename DerivedKey::Uptrc> {
    // VECTOR NC AutosarC++17_10-A2.8.3: MD_CRYPTO_AutosarC++17_10-A2.8.3_false_positive
    using DstValType = typename DerivedKey::Uptrc;
    // VECTOR NC AutosarC++17_10-A2.8.3: MD_CRYPTO_AutosarC++17_10-A2.8.3_false_positive
    using DstResType = ara::core::Result<DstValType>;
    DstResType result{nullptr};
    if (key) {
      ara::core::ErrorCode error{key->CheckObjectType(DerivedKey::sObjectType)};
      if (error != SecurityErrc::kNoError) {
        result.EmplaceError(error);
      } else {
        // VECTOR NC AutosarC++17_10-M5.2.3: MD_CRYPTO_AutosarC++17_10-M5.2.3_down_cast
        result.EmplaceValue(DstValType{static_cast<DerivedKey const*>(key.release())});
      }
    }
    return result;
  }
};

}  // namespace cryp
}  // namespace crypto
}  // namespace ara

#endif  // LIB_LIBARA_INCLUDE_ARA_CRYPTO_CRYP_KEY_H_
