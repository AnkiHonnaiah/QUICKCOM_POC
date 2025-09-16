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
/*!        \file  amsr/asn1/structure/att_cert_validity_period.h
 *        \brief  Representation of parsed att cert validity period.
 *
 *********************************************************************************************************************/

#ifndef LIB_VAASN1_INCLUDE_AMSR_ASN1_STRUCTURE_ATT_CERT_VALIDITY_PERIOD_H_
#define LIB_VAASN1_INCLUDE_AMSR_ASN1_STRUCTURE_ATT_CERT_VALIDITY_PERIOD_H_

/**********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/
#include "amsr/asn1/structure/asn1_primitive_types.h"
#include "amsr/core/result.h"
#include "ara/core/vector.h"

namespace amsr {
namespace asn1 {
namespace structure {

/*!
 * \brief AttCertValidityPeriod Structure.
 */
class AttCertValidityPeriod {
 public:
  /*!
   * \brief Equality operator
   * \param[in] other Other value
   * \return Equivalence (not identity) of values
   */
  auto operator==(AttCertValidityPeriod const& other) const -> bool {
    return std::tie(NotBeforeTime(), NotAfterTime()) == std::tie(other.NotBeforeTime(), other.NotAfterTime());
  }

  /*!
   * \brief Inequality operator
   * \param[in] other Other value
   * \return Negated equivalence (not identity) of values
   */
  auto operator!=(AttCertValidityPeriod const& other) const -> bool { return !(*this == other); }

  /*!
   * \brief Read-only access interface for property NotBeforeTime.
   * \return Value of NotBeforeTime.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto NotBeforeTime() const noexcept -> GeneralizedTime const& { return not_before_time_; }
  /*!
   * \brief access interface for property NotBeforeTime.
   * \return Value of NotBeforeTime.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto NotBeforeTime() noexcept -> GeneralizedTime& { return not_before_time_; }

  /*!
   * \brief Read-only access interface for property NotAfterTime.
   * \return Value of NotAfterTime.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto NotAfterTime() const noexcept -> GeneralizedTime const& { return not_after_time_; }

  /*!
   * \brief access interface for property NotAfterTime.
   * \return Value of NotAfterTime.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto NotAfterTime() noexcept -> GeneralizedTime& { return not_after_time_; }

  /*!
   * \brief Method to encode AttCertValidityPeriod class.
   * \return encoded data.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto Encode() const noexcept -> ara::core::Vector<std::uint8_t>;

 private:
  GeneralizedTime not_before_time_{}; /*!< Representation of Generalized Time. */
  GeneralizedTime not_after_time_{};  /*!< Representation of Generalized Time. */
};

}  // namespace structure
}  // namespace asn1
}  // namespace amsr

#endif  // LIB_VAASN1_INCLUDE_AMSR_ASN1_STRUCTURE_ATT_CERT_VALIDITY_PERIOD_H_
