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
/*!        \file  common/cryp/domain_parameters.h
 *        \brief  Interface for all DomainParameters used in crypto provider.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIB_COMMON_INCLUDE_CRYPTO_COMMMON_CRYP_DOMAIN_PARAMETERS_H_
#define LIB_LIB_COMMON_INCLUDE_CRYPTO_COMMMON_CRYP_DOMAIN_PARAMETERS_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <map>
#include <vector>

#include "ara/crypto/cryp/domain_parameters.h"

namespace crypto {
namespace common {
namespace cryp {

/*!
 * \brief Interface for all DomainParameters used in crypto provider
 * \vprivate Product Private
 */
class DomainParameters : public ara::crypto::cryp::DomainParameters {
 public:
  /*!
   * \brief Checks if the parameter for the index is available
   * \param[in] index index of the requested parameter
   * \return true if the parameter exists, false if not
   * \vprivate Product Private
   */
  virtual bool HasParameter(std::size_t index) const = 0;

  /*!
   * \brief Getter for a single parameter value
   * \param[in] index of the requested parameter
   * \return Value of the requested parameter
   * \vprivate Product Private
   */
  virtual ara::core::Result<ara::crypto::ReadOnlyMemRegion> GetParameter(std::size_t index) const = 0;
};

}  // namespace cryp
}  // namespace common
}  // namespace crypto

#endif  // LIB_LIB_COMMON_INCLUDE_CRYPTO_COMMMON_CRYP_DOMAIN_PARAMETERS_H_
