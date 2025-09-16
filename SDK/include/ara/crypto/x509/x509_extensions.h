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
/*!        \file  ara/crypto/x509/x509_extensions.h
 *        \brief  Interface of X.509 Extensions.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBARA_INCLUDE_ARA_CRYPTO_X509_X509_EXTENSIONS_H_
#define LIB_LIBARA_INCLUDE_ARA_CRYPTO_X509_X509_EXTENSIONS_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>

#include "ara/crypto/common/custom_disposable.h"
#include "ara/crypto/common/serializable.h"
#include "ara/crypto/common/std_api.h"

namespace ara {
namespace crypto {
namespace x509 {

// VECTOR NC AutosarC++17_10-A10.1.1: MD_CRYPTO_AutosarC++17_10-A10.1.1_multiple_inheritance_false_positive
/*!
 * \brief Interface of X.509 Extensions.
 * \vpublic
 */
class X509Extensions : public ara::crypto::Serializable,
                       public ara::crypto::CustomDisposable {  // Attention! Deviation: Added interface CustomDisposable
                                                               // (CustomDeleter requires interface)
 public:
  /*!
   * \brief Shared smart pointer of the interface.
   * \vpublic
   */
  using Sptr = std::unique_ptr<X509Extensions, ara::crypto::CustomDeleter>;

  /*!
   * \brief Count number of elements in the sequence.
   * \return number of elements in the sequence.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual std::size_t Count() const noexcept = 0;
};

}  // namespace x509
}  // namespace crypto
}  // namespace ara

#endif  // LIB_LIBARA_INCLUDE_ARA_CRYPTO_X509_X509_EXTENSIONS_H_
