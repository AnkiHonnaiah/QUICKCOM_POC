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
/*!        \file  restricted_use_object.h
 *        \brief  Restricted use object interface.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBARA_INCLUDE_ARA_CRYPTO_CRYP_RESTRICTED_USE_OBJECT_H_
#define LIB_LIBARA_INCLUDE_ARA_CRYPTO_CRYP_RESTRICTED_USE_OBJECT_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/crypto/common/base_id_types.h"
#include "ara/crypto/common/std_api.h"

#include "crypto_object.h"

namespace ara {
namespace crypto {
namespace cryp {

/*!
 * \brief A common interface for all objects supporting the usage restriction.
 * \vpublic
 */
class RestrictedUseObject : public CryptoObject {
 public:
  /*!
   * \brief Alias to the container type for bit-flags of allowed usages of the object.
   * \vpublic
   */
  using Usage = AllowedUsageFlags;

  /*!
   * \brief Get allowed usages of this object.
   * \return A combination of bit-flags that specifies allowed applications of the object.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual Usage GetAllowedUsage() const noexcept = 0;
};

}  // namespace cryp
}  // namespace crypto
}  // namespace ara

#endif  // LIB_LIBARA_INCLUDE_ARA_CRYPTO_CRYP_RESTRICTED_USE_OBJECT_H_
