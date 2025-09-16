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
/*!        \file  ara/crypto/x509/ocsp_request.h
 *        \brief  On-line Certificate Status Protocol Request.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBARA_INCLUDE_ARA_CRYPTO_X509_OCSP_REQUEST_H_
#define LIB_LIBARA_INCLUDE_ARA_CRYPTO_X509_OCSP_REQUEST_H_

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
 * \brief On-line Certificate Status Protocol Request.
 * \vpublic
 */
class OcspRequest : public ara::crypto::Serializable,
                    public ara::crypto::CustomDisposable {  // Attention! Deviation: Added interface CustomDisposable
                                                            // (CustomDeleter requires interface)
 public:
  /*!
   * \brief Shared smart pointer of the interface.
   * \vpublic
   */
  using Uptr = std::unique_ptr<OcspRequest, ara::crypto::CustomDeleter>;

  /*!
   * \brief Shared smart pointer of the interface.
   * \vpublic
   */
  using Uptrc = std::unique_ptr<OcspRequest const, ara::crypto::CustomDeleter>;

  /*!
   * \brief Get version of the OCSP request format.
   * \return OCSP request format version.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual std::uint32_t Version() const noexcept = 0;
};

}  // namespace x509
}  // namespace crypto
}  // namespace ara

#endif  // LIB_LIBARA_INCLUDE_ARA_CRYPTO_X509_OCSP_REQUEST_H_
