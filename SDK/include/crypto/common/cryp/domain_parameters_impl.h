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
 *        \brief  Abstract base implementation for all DomainParameters used in CryptoProvider.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBCOMMON_INCLUDE_CRYPTO_COMMON_CRYP_DOMAIN_PARAMETERS_IMPL_H_
#define LIB_LIBCOMMON_INCLUDE_CRYPTO_COMMON_CRYP_DOMAIN_PARAMETERS_IMPL_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <map>
#include <utility>
#include <vector>

#include "ara/core/string.h"

#include "crypto/common/cryp/domain_parameters.h"
#include "crypto/common/trusted_container.h"
#include "crypto/common/util/logger.h"

namespace crypto {
namespace common {
namespace cryp {

// VECTOR NC AutosarC++17_10-M7.3.6: MD_CRYPTO_AutosarC++17_10-M7.3.6_sv
/*! Introduce string literal for StringView creation */
using vac::container::literals::operator"" _sv;

// VECTOR NC Metric-OO.RFC: MD_CRYPTO_Metric-OO.RFC
/*!
 * \brief Abstract base implementation for all DomainParameters used in crypto provider
 * \vprivate Product Private
 */
class DomainParametersImpl : public cryp::DomainParameters {
 public:
  // VECTOR NC VectorC++-V3.9.2: MD_CRYPTO_VectorC++-V3.9.2_built_in_types_as_function_parameters
  // VECTOR NC VectorC++-V12.1.1: MD_CRYPTO_Vector++VectorC++-V12.1.1_base_class_initialization_false_positive
  /*!
   * \brief Constructor
   * \param[in] crypto_provider CryptoProvider owning this object
   * \param[in] allowed_usage Allowed usage for this object
   * \param[in] session session flag
   * \param[in] exportable exportable flag
   * \param[in] logger Logging instance to be used by this object
   * \param[in] couid COUID of this DomainParameters object
   * \vprivate Product Private
   */
  DomainParametersImpl(ara::crypto::cryp::CryptoProvider& crypto_provider,
                       ara::crypto::cryp::RestrictedUseObject::Usage allowed_usage, bool session, bool exportable,
                       ara::log::Logger& logger,
                       // VECTOR NL AutosarC++17_10-M8.5.2: MD_CRYPTO_AutosarC++17_10-M8.5.2_default_constructor
                       ara::crypto::CryptoObjectUid couid = ara::crypto::CryptoObjectUid{}) noexcept
      : crypto::common::Identifiable{},
        cryp::DomainParameters{},
        required_parameter_names_{},
        couid_{couid},
        crypto_provider_{crypto_provider},
        allowed_usage_{allowed_usage},
        session_{session},
        exportable_{exportable},
        complete_{false},
        domain_parameters_{},
        logger_{logger} {}

  /*!
   * \brief Default destructor
   * \vprivate Product Private  // Generated function stub
  ara::core::Abort("Function not yet implemented!");
   */
  ~DomainParametersImpl() noexcept override = default;

  /*!
   * \brief Default copy constructor
   * \vprivate Product Private
   */
  DomainParametersImpl(DomainParametersImpl const& /*other*/) = delete;

  /*!
   * \brief Default move constructor
   * \vprivate Product Private
   */
  DomainParametersImpl(DomainParametersImpl&& /*other*/) noexcept = delete;

  // VECTOR NC AutosarC++17_10-A10.2.1: MD_CRYPTO_AutosarC++17_10-A10.2.1_hiding_functions
  /*!
   * \brief Deleted copy assignment operator
   * \return -
   * \vprivate Product Private
   */
  DomainParametersImpl& operator=(DomainParametersImpl const& /*other*/) = delete;

  // VECTOR NC AutosarC++17_10-A10.2.1: MD_CRYPTO_AutosarC++17_10-A10.2.1_hiding_functions
  /*!
   * \brief Deleted move assignment operator
   * \return -
   * \vprivate Product Private
   */
  DomainParametersImpl& operator=(DomainParametersImpl&& /*other*/) noexcept = delete;

  /*!
   * \brief Release allocated memory and other resources.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  void Release() noexcept final { delete this; };

  /*!
   * \brief Gets a reference to Crypto Provider of this primitive.
   * \return A reference to Crypto Provider instance that provides this primitive.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  ara::crypto::cryp::CryptoProvider& MyProvider() const noexcept final;

  /*!
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
   * \threadsafe FALSE
   * \error SecurityErrorDomain::kUnknownIdentifier  if the \c index argument is incorrect.
   * \error SecurityErrorDomain::kUnexpectedValue  if the \c value argument is incorrect.
   * \error SecurityErrorDomain::kLogicFault  if the domain parameters' set is already completed.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  ara::core::Result<void> SetParameter(std::size_t index, ara::crypto::ReadOnlyMemRegion value) noexcept final;

  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
  // VECTOR NC AutosarC++17_10-M7.1.2: MD_CRYPTO_AutosarC++17_10-M7.1.2_autosar_api
  /*!
   * \brief Saves itself to provided trusted container.
   * \details An object can be saved to a container only if the container is empty and has enough capacity.
   * \details The save operation is allowed to an empty container only, i.e. any updates of objects are prohibited!
   * \details Only a non-session/non-temporary and completed object (i.e. that have a COUID) can be saved!
   * \details Only a single instance of an object is allowed in the persistent storage! Any object is uniquely
   *          identified by the combination of its COUID and type.
   * \param[in] container A target trusted container.
   * \param[in] referenced A pointer to another trusted container that keeps referenced crypto object.
   * \details Method not yet implemented.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error SecurityErrorDomain::kAccessViolation  if the target or the referenced non-volatile containers
   *         were opened "for usage", i.e. if (!container.HasOwnership() || !referenced->HasOwnership())
   * \error SecurityErrorDomain::kIncompatibleObject  if the object is "session", but the container
   * \error SecurityErrorDomain::kContentRestrictions  if the object doesn't satisfy the slot restrictions \c
   *         keys::KeySlotPrototypeProps)
   * \error SecurityErrorDomain::kInsufficientCapacity  if the capacity of the target container is not enough, i.e. if
   *         (container.Capacity() < this->StorageSize())
   * \error SecurityErrorDomain::kBadObjectReference
   *   - if (referenced != nullptr), but this crypto object doesn't support referencing to other objects;
   *   - if (referenced != nullptr), but an object in referenced container cannot be referenced;
   *   - if (referenced != nullptr), but this crypto object and referenced one have incompatible for
   *     referencing types and/or algorithms;
   *   - if (referenced != nullptr) and \c referenced is volatile, but \c container is non-volatile
   * \error SecurityErrorDomain::kEmptyContainer
   *   - if (referenced != nullptr), but the referenced trusted container is empty
   * \error ara::crypto::SecurityErrc::kUnsupported Method not yet implemented.
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  ara::core::Result<void> Save(ara::crypto::TrustedContainer& container,
                               ara::crypto::TrustedContainer* referenced = nullptr) const noexcept final;

  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
  /*!
   * \brief Serialize itself publicly.
   * \param[out] output  The preallocated output buffer (it can be empty if only the required
   *             size of the output buffer is interested).
   * \param[in] format_id  The Crypto Provider specific identifier of the output format.
   * \return Size required for storing of the output object.
   * \details Method not yet implemented.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error SecurityErrorDomain::kInsufficientCapacity
   *         if (output.empty() == false), but it's capacity is less than required
   * \error SecurityErrorDomain::kUnknownIdentifier  if an unknown format ID was specified
   * \error SecurityErrorDomain::kUnsupportedFormat
   *         if the specified format ID is not supported for this object type
   * \reentrant TRUE
   * \synchronous TRUE
   * \error ra::crypto::SecurityErrc::kUnsupported Method not yet implemented,  Returning default value.
   * \vprivate Product Private
   */
  ara::core::Result<std::size_t> ExportPublicly(
      // VECTOR NL AutosarC++17_10-M8.3.1: MD_CRYPTO_AutosarC++17_10-M8.3.1_namespace
      ara::crypto::WritableMemRegion output = ara::crypto::WritableMemRegion(),
      // VECTOR NL AutosarC++17_10-M8.3.1: MD_CRYPTO_AutosarC++17_10-M8.3.1_namespace
      FormatId format_id = ara::crypto::Serializable::kFormatDefault) const noexcept final;

  /*!
   * \brief Returns maximal length between all names of this domain parameters.
   * \return Maximal length between all parameters' names.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  std::size_t GetMaxParameterNameLength() const noexcept final;

  /*!
   * \brief Returns the "session" (or "temporary") attribute of the object.
   * \details A temporary object cannot be saved to a non-volatile trusted container!
   * \details A temporary object will be securely destroyed together with this interface instance!
   * \details A non-session object must have an assigned COUID (see GetObjectId()).
   * \return true if the object is temporary (i.e. its life time is limited by the current session only).
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  bool IsSession() const noexcept final;

  /*!
   * \brief Returns the "exportable" attribute of the object.
   * \details An exportable object must have an assigned COUID (see GetObjectId()).
   * \return true if the object is exportable (i.e. if it can be exported
   *         outside the trusted environment of the Crypto Provider).
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  bool IsExportable() const noexcept final;

  /*!
   * \brief Returns a storage size of the object.
   * \return Size in bytes of the objects required for its storage.
   * \details Method not yet implemented.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  std::size_t StorageSize() const noexcept final;

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
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  ara::core::Result<std::size_t> GetParameterName(std::size_t index, ara::core::String* name) const noexcept final;

  /*!
   * \brief Returns number of supported parameters.
   * \return Number of supported parameters.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  std::size_t GetParametersCount() const noexcept final;

  /*!
   * \brief Gets expected size of specific parameter of the Crypto Primitive.
   * \param[in] index Index of a parameter that should be set. It should be less than GetParametersCount().
   * \return Expected (presize or maximal) size of the parameter defined by the index.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  ara::core::Result<std::size_t> ExpectedParameterSize(std::size_t index) const noexcept final;

  /*!
   * \brief Checks completeness and consistency of this parameters set.
   * \details Until the set of domain parameters is incomplete the object COUID should not be set (i.e. equal to zero)!
   * \return true if this set of domain parameters is completed and false otherwise.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  bool IsCompleted() const noexcept final;

  // Attention! Deviation: On successful completion, the COUID should be generated. Currently, this is not the case.
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
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  bool Complete(Usage allowed_usage) noexcept final;

  /*!
   * \brief Returns the object's UID if it is assigned to the object.
   * \details An object that has no an assigned COUID cannot be (securely)
   *          serialized / exported or saved to a non-volatile container.
   * \details An object should not have a COUID if it is session and non-exportable
   *          simultaneously or if it is incomplete yet (last is applicable for domain parameters only).
   * \details A few related objects of different types can share a single COUID (e.g. private and public keys),
   *          but a combination of COUID and object type must be unique always!
   * \param[out] object_id Optional pointer to a buffer for the object's UID saving.
   * \return true if the object has assigned COUID.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  bool GetObjectId(ara::crypto::CryptoObjectUid* object_id) const noexcept final;

  /*!
   * \brief Checks whether the supported parameter exists at particular index.
   * \param[in] index Index to check the parameter existence.
   * \return \c true if parameter available.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  bool HasParameter(std::size_t index) const final;

  /*!
   * \brief Returns the supported parameter at particular index.
   * \param[in] index At this index, parameter required.
   * \return Parameter.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error ara::crypto::SecurityErrc::kInvalidArgument Requested parameter is not available.
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  ara::core::Result<ara::crypto::ReadOnlyMemRegion> GetParameter(std::size_t index) const final;

  /*!
   * \brief Get allowed usages of this object.
   * \return A combination of bit-flags that specifies allowed applications of the object.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  ara::crypto::cryp::RestrictedUseObject::Usage GetAllowedUsage() const noexcept final;

 protected:
  // VECTOR NC AutosarC++17_10-M11.0.1: MD_CRYPTO_AutosarC++17_10-M11.0.1_member_in_non_class_types_unavoidable
  // VECTOR NC VectorC++-V11.0.2: MD_CRYPTO_VectorC++-V11-0-2
  std::map<std::size_t, std::pair<ara::core::String, size_t>>
      required_parameter_names_; /*!< Map of required parameters and their names / expected size */
  // VECTOR NC VectorC++-V11.0.2: MD_CRYPTO_VectorC++-V11-0-2
  ara::crypto::CryptoObjectUid couid_; /*!< COUID of this CryptoObject */

 private:
  ara::crypto::cryp::CryptoProvider& crypto_provider_;          /*!< CryptoProvider owning this CryptoObject */
  ara::crypto::cryp::RestrictedUseObject::Usage allowed_usage_; /*!< Allowed usage */
  bool session_;                                                /*!< Session flag */
  bool exportable_;                                             /*!< Exportable flag */
  // VECTOR NC AutosarC++17_10-A12.1.3: MD_CRYPTO_AutosarC++17_10-A12.1.3
  bool complete_; /*!< Indicates if the domain parameters have been completed */
  std::map<std::size_t, ara::core::Vector<std::uint8_t>>
      domain_parameters_;    /*!< List containing copies of the real parameters */
  ara::log::Logger& logger_; /*!< Logging instance */
};

}  // namespace cryp
}  // namespace common
}  // namespace crypto

#endif  // LIB_LIBCOMMON_INCLUDE_CRYPTO_COMMON_CRYP_DOMAIN_PARAMETERS_IMPL_H_
