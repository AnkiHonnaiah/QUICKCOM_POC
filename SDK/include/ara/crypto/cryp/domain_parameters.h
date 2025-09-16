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
/*!        \file  ara/crypto/cryp/domain_parameters.h
 *        \brief  Generic Domain Parameters interface.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBARA_INCLUDE_ARA_CRYPTO_CRYP_DOMAIN_PARAMETERS_H_
#define LIB_LIBARA_INCLUDE_ARA_CRYPTO_CRYP_DOMAIN_PARAMETERS_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>

#include "ara/core/string.h"
#include "ara/core/string_view.h"
#include "ara/crypto/common/mem_region.h"
#include "ara/crypto/common/serializable.h"
#include "ara/crypto/common/std_api.h"
#include "ara/crypto/cryp/restricted_use_object.h"

namespace ara {
namespace crypto {
namespace cryp {

// VECTOR Next Construct AutosarC++17_10-A10.1.1 : MD_CRYPTO_AutosarC++17_10-A10.1.1_multiple_for_proxies_unavoidable
/*!
 * \brief Generic Domain Parameters interface.
 * \details Any user of this interface should create shared pointers
 *          to it only by calls of the method shared_from_this()!
 * \vpublic
 */
class DomainParameters : public std::enable_shared_from_this<DomainParameters>,
                         public RestrictedUseObject,
                         public Serializable {
 public:
  /*!
   * \brief Static mapping of this interface to specific value of \c CryptoObjectType enumeration.
   * \vpublic
   */
  static CryptoObjectType const sObjectType{CryptoObjectType::kDomainParameters};

  /*!
   * \brief Shared smart pointer of the interface.
   * \vpublic
   */
  using Sptr = std::shared_ptr<DomainParameters>;

  /*!
   * \brief Shared smart pointer of the constant interface.
   * \vpublic
   */
  using Sptrc = std::shared_ptr<DomainParameters const>;

  /*!
   * \brief Unique smart pointer of the interface.
   * \vpublic
   */
  using Uptr = std::unique_ptr<DomainParameters, CustomDeleter>;

  /*!
   * \brief Unique pointer of the constant interface.
   * \vpublic
   */
  using Uptrc = std::unique_ptr<DomainParameters const, CustomDeleter>;

  /*!
   * \brief Get secrecy attribute of these parameters set.
   * \return true if the set of these parameters is secret.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual bool IsSecret() const noexcept = 0;

  /*!
   * \brief Returns number of supported parameters.
   * \return Number of supported parameters.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual std::size_t GetParametersCount() const noexcept = 0;

  /*!
   * \brief Returns maximal length between all names of this domain parameters.
   * \return Maximal length between all parameters' names.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual std::size_t GetMaxParameterNameLength() const noexcept = 0;

  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
  /*!
   * \brief Returns the parameter name by its index.
   * \details A list of supported parameters is specific for Crypto Primitive
   *          implementation and should be defined by a supplier.
   * \param[in] index Index of a parameter, name of which is requested.
   *                  It should be less than GetParametersCount().
   * \param[out] name An optional pointer to a string where the parameter name should be saved.
   * \return Actual length of the parameter name.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error SecurityErrorDomain::kUnknownIdentifier  if the \c index argument is incorrect.
   * \error SecurityErrorDomain::kInsufficientCapacity if (name != nullptr), but
   *                the \c name->capacity() is not enough to store the parameter name.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual ara::core::Result<std::size_t> GetParameterName(std::size_t index,
                                                          ara::core::String* name = nullptr) const noexcept = 0;

  /*!
   * \brief Gets expected size of specific parameter of the Crypto Primitive.
   * \param[in] index Index of a parameter that should be set. It should be less than GetParametersCount().
   * \return Expected (presize or maximal) size of the parameter defined by the index.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error SecurityErrorDomain::kUnknownIdentifier  if the \c index argument is incorrect.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual ara::core::Result<std::size_t> ExpectedParameterSize(std::size_t index) const noexcept = 0;

  /*!
   *
   * \brief Sets a value to specific parameter of the Crypto Primitive.
   * \details All Crypto Primitives that supports custom parameters
   *          also should have a correct default set of parameters.
   * \details A list of supported parameters is specific for Crypto Primitive
   *          implementation and should be defined by a supplier.
   * \details All named domain parameters (for which getUniqueName() returns non-empty string)
   *          are already created in the completed state and don't need a call of this method!
   * \param[in] index Index of a parameter, which should be set. It should be less than GetParametersCount().
   * \param[in] value New value of the parameter that should be set.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error SecurityErrorDomain::kUnknownIdentifier  if the \c index argument is incorrect.
   * \error SecurityErrorDomain::kUnexpectedValue  if the \c value argument is incorrect.
   * \error SecurityErrorDomain::kLogicFault  if the domain parameters' set is already completed.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual ara::core::Result<void> SetParameter(std::size_t index, ReadOnlyMemRegion value) noexcept = 0;

  /*!
   * \brief Set a value to a specific parameter of the domain parameters object.
   * \details All Crypto Primitives that supports custom parameters
   *          also should have a correct default set of parameters.
   * \details A list of supported parameters is specific for each DomainParameters
   *          implementation and should be defined by a supplier.
   * \details All named domain parameters (for which getUniqueName() returns non-empty string)
   *          are already created in the completed state and don't need a call of this method!
   * \param[in] index Index of a parameter, which should be set. It should be less than GetParametersCount().
   * \param[in] mem Writable memRegion to hold the serialized value.
   * \param[in] value New value of the parameter that should be set.
   * \tparam T type of the to serialize value.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error SecurityErrorDomain::kUnknownIdentifier  if the \c index argument is incorrect.
   * \error SecurityErrorDomain::kUnexpectedValue  if the \c value argument is incorrect.
   * \error SecurityErrorDomain::kLogicFault  if the domain parameters' set is already completed.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  template <typename T>
  ara::core::Result<void> SetParameter(std::size_t index, ara::crypto::WritableMemRegion mem, T value) noexcept {
    ara::core::Result<void> result{ara::crypto::SecurityErrc::kInsufficientCapacity};
    if (mem.size() >= sizeof(T)) {
      std::memcpy(mem.data(), &value, sizeof(T));
      result = SetParameter(index, mem.subspan(0, sizeof(T)));
    }
    return result;
  }

  /*!
   * \brief Checks completeness and consistency of this parameters set.
   * \details Until the set of domain parameters is incomplete the object COUID should not be set (i.e. equal to zero)!
   * \return true if this set of domain parameters is completed and false otherwise.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual bool IsCompleted() const noexcept = 0;

  /*!
   * \brief Checks completeness and consistency of this parameters set
   *        and generates the COUID if the parameters are completed at first time.
   * \param[in] allowed_usage The allowed usages for the domain parameters.
   * \details Until the set of domain parameters is incomplete the object COUID should not be set (i.e. equal to zero)!
   * \details If the set of domain parameters is already completed then all
   *          following calls of the method setParameter() will fail!
   * \details All named domain parameters (for which getUniqueName() returns non-empty string)
   *          are already created in the completed state and have assigned COUID!
   * \details Internal presence of a non-zero COUID is a marker of completed domain parameters,
   *          therefore the real check for completeness is required only if the COUID is zeros.
   * \return "true" if this set of domain parameters is completed and "false" otherwise.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual bool Complete(Usage allowed_usage) noexcept = 0;

  /*!
   * \brief Returns a unique "well-known" name of the parameters set, if it is assigned (f.e. OID/Name).
   * \details If this method returns non-empty string (i.e. all named domain parameters)
   *         then it is already in the completed state and has assigned COUID!
   * \return Unique name of the Domain Parameters set or the empty string if a unique name is not assigned.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual ara::core::StringView GetUniqueName() const noexcept = 0;
};

}  // namespace cryp
}  // namespace crypto
}  // namespace ara

#endif  // LIB_LIBARA_INCLUDE_ARA_CRYPTO_CRYP_DOMAIN_PARAMETERS_H_
