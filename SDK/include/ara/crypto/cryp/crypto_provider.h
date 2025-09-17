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
/*!        \file  ara/crypto/cryp/crypto_provider.h
 *        \brief  Crypto Provider is a "factory" interface of all supported Crypto Primitives
 *                and a "trusted environment" for internal communications between them.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBARA_INCLUDE_ARA_CRYPTO_CRYP_CRYPTO_PROVIDER_H_
#define LIB_LIBARA_INCLUDE_ARA_CRYPTO_CRYP_CRYPTO_PROVIDER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstdint>
#include <memory>
#include <utility>
#include <vector>

#include "ara/core/string.h"
#include "ara/core/string_view.h"
#include "ara/crypto/common/mem_region.h"
#include "ara/crypto/common/provider_info.h"
#include "ara/crypto/common/serializable.h"
#include "ara/crypto/common/std_api.h"
#include "ara/crypto/common/trusted_container.h"
#include "ara/crypto/cryp/authn_stream_cipher_ctx.h"
#include "ara/crypto/cryp/crypto_primitive_id.h"
#include "ara/crypto/cryp/decryptor_private_ctx.h"
#include "ara/crypto/cryp/domain_parameters.h"
#include "ara/crypto/cryp/encryptor_public_ctx.h"
#include "ara/crypto/cryp/hash_function_ctx.h"
#include "ara/crypto/cryp/key_agreement_private_ctx.h"
#include "ara/crypto/cryp/key_decapsulator_private_ctx.h"
#include "ara/crypto/cryp/key_derivation_function_ctx.h"
#include "ara/crypto/cryp/key_diversifier_ctx.h"
#include "ara/crypto/cryp/key_encapsulator_public_ctx.h"
#include "ara/crypto/cryp/memory_pool.h"
#include "ara/crypto/cryp/message_authn_code_ctx.h"
#include "ara/crypto/cryp/msg_recovery_public_ctx.h"
#include "ara/crypto/cryp/password_cache.h"
#include "ara/crypto/cryp/random_generator_ctx.h"
#include "ara/crypto/cryp/sig_encode_private_ctx.h"
#include "ara/crypto/cryp/signer_private_ctx.h"
#include "ara/crypto/cryp/stream_cipher_ctx.h"
#include "ara/crypto/cryp/symmetric_block_cipher_ctx.h"
#include "ara/crypto/cryp/symmetric_key_wrapper_ctx.h"
#include "ara/crypto/cryp/verifier_public_ctx.h"
#include "ara/crypto/cryp/x509_request_signer_ctx.h"

namespace ara {
namespace crypto {
namespace cryp {

// VECTOR NC Metric-OO.WMC.One: MD_CRYPTO_Metric-OO.WMC.One
// VECTOR NC AutosarC++17_10-A10.1.1: MD_CRYPTO_AutosarC++17_10-A10.1.1_multiple_inheritance_false_positive
/*!
 * \brief Crypto Provider is a "factory" interface of all supported Crypto Primitives
 *        and a "trusted environment" for internal communications between them.
 * \details All Crypto Primitives should have an actual reference to their parent Crypto Provider.
 * \details A Crypto Provider can be destroyed only after destroying of all its daughterly Crypto Primitives.
 * \details Each method of this interface that creates a Crypto Primitive instance is non-constant,
 *          because any such creation increases a references counter of the Crypto Primitive.
 * \details Any user of this interface should create shared pointers
 *          to it only by calls of the method shared_from_this()!
 * \vpublic
 */
class CryptoProvider : public std::enable_shared_from_this<CryptoProvider>, public ProviderInfo {
 protected:
  /*!
   * \brief Default copy constructor
   * \vprivate Component Private
   */
  CryptoProvider(CryptoProvider const& /*other*/) = default;

  /*!
   * \brief Default move constructor
   * \vprivate Component Private
   */
  CryptoProvider(CryptoProvider&& /*other*/) noexcept = default;

 public:
  /*!
   * \brief Default constructor
   * \vprivate Component Private
   */
  CryptoProvider() = default;

  /*!
   * \brief Destructor.
   * \vprivate Component Private
   */
  ~CryptoProvider() noexcept override = default;

  // VECTOR NC AutosarC++17_10-A10.2.1: MD_CRYPTO_AutosarC++17_10-A10.2.1_hiding_functions
  /*!
   * \brief Deleted move assignment constructor
   * \vprivate Component Private
   */
  CryptoProvider& operator=(CryptoProvider&& /*other*/) = delete;

  // VECTOR NC AutosarC++17_10-A10.2.1: MD_CRYPTO_AutosarC++17_10-A10.2.1_hiding_functions
  /*!
   * \brief Deleted copy assignment constructor
   * \vprivate Component Private
   */
  CryptoProvider& operator=(CryptoProvider const& /*other*/) = delete;

  /*!
   * \brief Shared smart pointer of the interface.
   * \vpublic
   */
  using Sptr = std::shared_ptr<CryptoProvider>;

  /*!
   * \brief Enumeration of all types of crypto objects.
   * \vpublic
   */
  using ObjectType = CryptoObject::Type;

  /*!
   * \brief A short alias for Algorithm ID type definition.
   * \vpublic
   */
  using AlgId = CryptoPrimitiveId::AlgId;

  /*!
   * \brief A list of Crypto Contexts that should be contained by
   *        a single shared memory slot in different time moments.
   * \details This vector indirectly specifies a minimal required capacity of a single reserved
   *          Context slot via a list of Contexts' IDs that must be hosted by the slot.
   * \details alg_id Specify target Crypto Primitive via it's algorithm ID.
   * \details bool Specify direct ( true) or reverse (false) transformation.
   * \details This vector is used for calculation of minimal required capacity of the reserved Context slot.
   * \details If at least one element of AlgId has value 0 (ALGID_UNDEFINED),
   *         then maximal supported Context size should be reserved.
   * \vpublic
   */
  using ContainedContextsList = ara::core::Vector<std::pair<AlgId, bool>>;

  /*!
   * \brief A list of Crypto Objects that should be contained by
   *        a single shared memory slot in different time moments.
   * \details This vector indirectly specifies a minimal required capacity of a single reserved
   *          Object slot via a list of Objects' IDs that must be hosted by the slot.
   * \details AlgId Specify target Crypto Primitive via it's algorithm ID.
   * \details ObjectType Specify concrete object type.
   * \details This vector is used for calculation of minimal required capacity of the reserved Object slot.
   * \details If some AlgId element of the list has value 0 (ALGID_UNDEFINED),
   *          then maximal supported size of correspondent ObjectType should be reserved for this element.
   * \details If at least one element ObjectType of the list has value 0 (ObjectType::UNKNOWN),
   *          then maximal supported object size should be reserved.
   * \vpublic
   */
  using ContainedObjectsList = ara::core::Vector<std::pair<AlgId, ObjectType>>;

  /*!
   * \brief This vector specifies a whole mapping of minimally required capacities to the Context slots' indexes.
   * \vpublic
   */
  using ContextReservationMap = ara::core::Vector<ContainedContextsList>;

  /*!
   * \brief This vector specifies a whole mapping of minimally required capacities to the Object slots' indexes.
   * \vpublic
   */
  using ObjectReservationMap = ara::core::Vector<ContainedObjectsList>;

  /*!
   * \brief Converts a common name of crypto algorithm to a correspondent vendor specific binary algorithm ID.
   * \param[in] primitive_name A unified name of the crypto primitive
   *                          (see "Crypto Primitives Naming Convention" for more details).
   * \return Vendor specific binary algorithm ID or kAlgIdUndefined if
   *         a primitive with provided name is not supported.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual AlgId ConvertToAlgId(ara::core::StringView primitive_name) const noexcept = 0;

  /*!
   * \brief Converts a vendor specific binary algorithm ID to a correspondent common name of the crypto algorithm.
   * \param[in] alg_id A vendor specific binary algorithm ID.
   * \return A common name of the crypto algorithm (see "!_Naming_Convention.txt" for more details).
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error SecurityErrorDomain::kUnknownIdentifier  if \c alg_id argument has an unsupported value.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual ara::core::Result<ara::core::String> ConvertToAlgName(AlgId alg_id) const noexcept = 0;

  /*!
   * \brief Get Crypto Primitive Category of specified algorithm.
   * \param[in] alg_id  the vendor specific binary algorithm ID.
   * \return the crypto primitive category of the crypto algorithm.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error SecurityErrorDomain::kUnknownIdentifier  if \c alg_id argument has an unsupported value.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual ara::core::Result<CryptoPrimitiveId::Category> GetPrimitiveCategory(AlgId alg_id) const noexcept = 0;

  /*!
   * \brief Reserves memory for simultaneous hosting of all Contexts specified by the map.
   * \param[in] reservation_map Contexts reservation map that defines minimal required size for
   *                           each reserved Context slot.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error SecurityErrorDomain::kUnknownIdentifier  if \c reservation_map includes unknown algorithm identifiers.
   * \error SecurityErrorDomain::kBadAlloc  if the requested reservation cannot be executed.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual ara::core::Result<void> ReserveContexts(ContextReservationMap const& reservation_map) noexcept = 0;

  /*!
   * \brief Reserves memory for simultaneous hosting of all Objects specified by the map.
   * \param[in] reservation_map Objects reservation map that defines minimal
   *                           required size for each reserved Object slot.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error SecurityErrorDomain::kInvalidArgument  if unknown or unsupported combination of object type and algorithm ID
   * presents in the \c reservation_map
   * \error SecurityErrorDomain::kBadAlloc  if the requested reservation cannot be executed.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual ara::core::Result<void> ReserveObjects(ObjectReservationMap const& reservation_map) noexcept = 0;

  /*!
   * \brief Reserves memory for simultaneous hosting of specified quantity of any type Contexts,
   *        i.e. maximal capacity will be reserved for each Context.
   * \param[in] quantity Number of Contexts for reservation.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error SecurityErrorDomain::kBadAlloc  if the requested reservation cannot be executed.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual ara::core::Result<void> ReserveContexts(std::size_t quantity) noexcept = 0;

  /*!
   * \brief Reserves memory for simultaneous hosting of specified quantity of any type Objects,
   *        i.e. maximal capacity will be reserved for each Object.
   * \param[in] quantity Number of Objects for reservation.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error SecurityErrorDomain::kBadAlloc  if the requested reservation cannot be executed.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual ara::core::Result<void> ReserveObjects(std::size_t quantity) noexcept = 0;

  /*!
   * \brief Switches the memory management engine of the current execution thread to the "Real-Time" mode.
   * \details After entering to the "Real-Time" mode, any allocations of Contexts or Objects on the heap are prohibited.
   * \details In the "Real-Time" mode only reserved Objects and Contexts can be used
   *          (see reserveContexts() and reserveObjects() methods and
   * \details Indexes of reserved Objects and Contexts slots are presented in the API
   *          by the types ReservedObjectIndex and ReservedContextIndex respectively.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual void EnterRealTimeMode() noexcept = 0;

  /*!
   * \brief Switches the memory management engine of the current execution thread to the "Non-Real-Time" mode.
   * \details After leaving the "Real-Time" mode, any allocations of Contexts or Objects on the heap are allowed.
   * \details In the "Non-Real-Time" mode the reserved Objects and Contexts can be used too
   *          (see reserveContexts() and reserveObjects() methods).
   * \details Indexes of reserved Objects and Contexts slots are presented in the API
   *          by the types ReservedObjectIndex and ReservedContextIndex respectively.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual void LeaveRealTimeMode() noexcept = 0;

  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
  /*!
   * \brief Allocates an empty object of domain parameters for specified algorithm.
   * \param[in] alg_id An identifier of the algorithm for which the domain parameters are intended.
   * \param[in] is_session Defines the "session" (or "temporary") attribute for the target domain parameters (if true).
   * \param[in] is_exportable Defines the exportability attribute for the target key context.
   * \param[in] reserved_index An optional index of reserved Object slot that should be used for this allocation or
   *                          default marker, which says to allocate on the heap.
   * \return Shared smart pointer to the allocated domain parameter object.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error SecurityErrorDomain::kUnknownIdentifier  if \c alg_id has an incorrect value
   * \error SecurityErrorDomain::kBusyResource  if the slot specified by \c reserved_index is busy yet
   * \error SecurityErrorDomain::kUnreservedResource  if the slot specified by \c reserved_index
   *                was not allocated
   * \error SecurityErrorDomain::kInsufficientResource  if size of the slot specified by \c reserved_index
   *                is not enough for placing of the target object
   * \error SecurityErrorDomain::kBadAlloc  if (reserved_index == kAllocObjectOnHeap),
   *                but allocation memory on the heap is impossible
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  // VECTOR NC VectorC++-V3.9.2: MD_CRYPTO_VectorC++-V3.9.2_built_in_types_as_function_parameters
  virtual ara::core::Result<DomainParameters::Sptr> AllocDomainParameters(
      AlgId alg_id, bool is_session = false, bool is_exportable = false,
      ReservedObjectIndex reserved_index = kAllocObjectOnHeap) noexcept = 0;

  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
  /*!
   * \brief Loads a known domain parameters by their OID/Name.
   * \param[in] oid_name OID/Name of required domain parameters (names are case-insensitive).
   * \param[in] reserved_index An optional index of reserved Object slot that should be used for this allocation or
   *                          default marker, which says to allocate on the heap.
   * \return Shared smart pointer to the allocated domain parameter object.
   * \details Crypto Provider can share a single instance of named (i.e. constant)
   *          domain parameters between a few consumers.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error SecurityErrorDomain::kUnknownIdentifier  if the \c oid_name argument has an unsupported value
   * \error SecurityErrorDomain::kBusyResource  if the slot specified by \c reserved_index is busy yet
   * \error SecurityErrorDomain::kUnreservedResource  if the slot specified by \c reserved_index
   *                was not allocated
   * \error SecurityErrorDomain::kInsufficientResource  if size of the slot specified by \c reserved_index
   *                is not enough for placing of the target object
   * \error SecurityErrorDomain::kBadAlloc  if (reserved_index == kAllocObjectOnHeap),
   *                but allocation memory on the heap is impossible
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual ara::core::Result<DomainParameters::Sptrc> KnownDomainParameters(
      ara::core::StringView oid_name, ReservedObjectIndex reserved_index = kAllocObjectOnHeap) noexcept = 0;

  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
  /*!
   * \brief Allocate a new symmetric key object and fill it by a new randomly generated value.
   * \details If (params != nullptr) then the domain parameters object
   *          must be in the completed state (see DomainParameters)!
   * \details If (params != nullptr) then at least the parameters' COUID
   *          must be saved to the dependency field of the generated key object.
   * \details Any serializable (i.e. savable/non-session or exportable) key must generate own COUID!
   * \details By default Crypto Provider should use an internal instance
   *          of a best from all supported RNG (ideally TRNG).
   * \param[in] alg_id Identifier of target symmetric crypto algorithm.
   * \param[in] allowed_usage Flags that define a list of allowed transformations' types
   *                         in which the target key can be used (see constants in scope of KeyMaterial).
   * \param[in] is_session Defines the "session" (or "temporary") attribute for the target key (if true).
   * \param[in] is_exportable Defines the exportability attribute for the target key (if true).
   * \param[in] params An optional pointer to Domain Parameters required for full specification
   *                   of the symmetric transformation (e.g. variable S-boxes of GOST28147-89).
   * \param[in] reserved_index An optional index of reserved Object slot that should be used
   *                          for this allocation or default marker, which says to allocate on the heap.
   * \return Smart unique pointer to the created symmetric key object.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error SecurityErrorDomain::kUnknownIdentifier  if \c alg_id has an unsupported value
   * \error SecurityErrorDomain::kEmptyContainer  if the domain parameters are required,
   *                but (params == nullptr) was passed
   * \error SecurityErrorDomain::kIncompatibleObject  if (params != nullptr),
   *                but provided domain parameters object has inappropriate type
   * \error SecurityErrorDomain::kIncompleteArgState  if (params != nullptr),
   *                but provided domain parameters object has an incomplete state
   * \error SecurityErrorDomain::kBusyResource  if the slot specified by \c reserved_index is busy yet
   * \error SecurityErrorDomain::kUnreservedResource  if the slot specified by \c reserved_index
   *                was not allocated
   * \error SecurityErrorDomain::kInsufficientResource  if size of the slot specified by \c reserved_index
   *                is not enough for placing of the target object
   * \error SecurityErrorDomain::kBadAlloc  if (reserved_index == kAllocObjectOnHeap),
   *                but allocation memory on the heap is impossible
   * \error SecurityErrorDomain::kIncompatibleArguments  if \c allowed_usage argument is incompatible with
   *                target algorithm \c alg_id (note: it is an optional error condition for this method)
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  // VECTOR NC VectorC++-V3.9.2: MD_CRYPTO_VectorC++-V3.9.2_built_in_types_as_function_parameters
  virtual ara::core::Result<SymmetricKey::Uptrc> GenerateSymmetricKey(
      AlgId alg_id, RestrictedUseObject::Usage allowed_usage, bool is_session = true, bool is_exportable = false,
      DomainParameters::Sptrc params = nullptr, ReservedObjectIndex reserved_index = kAllocObjectOnHeap) noexcept = 0;

  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
  /*!
   * \brief Allocate a new private key context of correspondent type and generates the key value randomly.
   * \details A common COUID should be shared for both private and public keys.
   * \details Any serializable (i.e. savable/non-session or exportable) key must generate own COUID!
   * \details If (params != nullptr) then the domain parameters object
   *          must be in the completed state (see DomainParameters)!
   * \param[in] alg_id Identifier of target public-private key crypto algorithm.
   * \param[in] allowed_usage Flags that define a list of allowed transformations' types
   *                         in which the target key can be used (see constants in scope of KeyMaterial).
   * \param[in] is_session Defines the "session" (or "temporary") attribute for the target key (if true).
   * \param[in] is_exportable Defines the exportability attribute for the target key (if true).
   * \param[in] params An optional pointer to Domain Parameters required for full specification of the transformation.
   * \param[in] reserved_index An optional index of reserved Object slot that should be used
   *                          for this allocation or default marker, which says to allocate on the heap.
   * \return Smart unique pointer to the created private key object.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error SecurityErrorDomain::kUnknownIdentifier  if \c alg_id has an unsupported value
   * \error SecurityErrorDomain::kEmptyContainer  if the domain parameters are required,
   *                but (params == nullptr) was passed
   * \error SecurityErrorDomain::kIncompatibleObject  if (params != nullptr),
   *                but provided domain parameters object has inappropriate type
   * \error SecurityErrorDomain::kIncompleteArgState  if (params != nullptr),
   *                but provided domain parameters object has an incomplete state
   * \error SecurityErrorDomain::kBusyResource  if the slot specified by \c reserved_index is busy yet
   * \error SecurityErrorDomain::kUnreservedResource  if the slot specified by \c reserved_index
   *                was not allocated
   * \error SecurityErrorDomain::kInsufficientResource  if size of the slot specified by \c reserved_index
   *                is not enough for placing of the target object
   * \error SecurityErrorDomain::kBadAlloc  if (reserved_index == kAllocObjectOnHeap),
   *                but allocation memory on the heap is impossible
   * \error SecurityErrorDomain::kIncompatibleArguments  if \c allowed_usage argument is incompatible with
   *                target algorithm \c alg_id (note: it is an optional error condition for this method)
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  // VECTOR NC VectorC++-V3.9.2: MD_CRYPTO_VectorC++-V3.9.2_built_in_types_as_function_parameters
  virtual ara::core::Result<PrivateKey::Uptrc> GeneratePrivateKey(
      AlgId alg_id, RestrictedUseObject::Usage allowed_usage, bool is_session = false, bool is_exportable = false,
      DomainParameters::Sptrc params = nullptr, ReservedObjectIndex reserved_index = kAllocObjectOnHeap) noexcept = 0;

  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
  /*!
   * \brief Generate a random Secret Seed object of requested algorithm.
   * \param[in] alg_id  the identifier of target crypto algorithm
   * \param[in] allowed_usage  the lags that define a list of allowed transformations' types in which the target seed
   *            can be used (see constants in scope of \c KeyMaterial)
   * \param[in] is_session  the "session" (or "temporary") attribute of the target seed (if \c true)
   * \param[in] is_exportable  the exportability attribute of the target seed (if \c true)
   * \param[in] reserved_index  the optional index of reserved Object slot that should be used for this allocation or
   *            default marker, which says to allocate on the heap
   * \return unique smart pointer to generated \c SecretSeed object
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error SecurityErrorDomain::kUnknownIdentifier  if \c alg_id has an unsupported value
   * \error SecurityErrorDomain::kBusyResource  if the slot specified by \c reserved_index is busy yet
   * \error SecurityErrorDomain::kUnreservedResource  if the slot specified by \c reserved_index
   *                was not allocated
   * \error SecurityErrorDomain::kInsufficientResource  if size of the slot specified by \c reserved_index
   *                is not enough for placing of the target object
   * \error SecurityErrorDomain::kBadAlloc  if (reserved_index == kAllocObjectOnHeap),
   *                but allocation memory on the heap is impossible
   * \error SecurityErrorDomain::kIncompatibleArguments  if \c allowed_usage argument is incompatible with
   *                target algorithm \c alg_id (note: it is an optional error condition for this method)
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  // VECTOR NC VectorC++-V3.9.2: MD_CRYPTO_VectorC++-V3.9.2_built_in_types_as_function_parameters
  virtual ara::core::Result<SecretSeed::Uptrc> GenerateSeed(
      AlgId alg_id, SecretSeed::Usage allowed_usage, bool is_session = true, bool is_exportable = false,
      ReservedObjectIndex reserved_index = kAllocObjectOnHeap) noexcept = 0;

  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
  /*!
   * \brief Return required buffer size for serialization of an object in specific format.
   * \param[in] object_type  the type of the target object
   * \param[in] alg_id  the Crypto Provider algorithm ID of the target object
   * \param[in] format_id  the Crypto Provider specific identifier of the output format
   * \return size required for storing of the object serialized in the specified format
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error SecurityErrorDomain::kUnknownIdentifier  if any argument has an unsupported value
   * \error SecurityErrorDomain::kIncompatibleArguments  if any pair of the arguments are incompatible
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual ara::core::Result<std::size_t> GetSerializedSize(
      ObjectType object_type, AlgId alg_id,
      Serializable::FormatId format_id = Serializable::kFormatDefault) const noexcept = 0;

  /*!
   * \brief Return required capacity of a key slot for saving of the object.
   * \param[in] object_type  the type of the target object
   * \param[in] alg_id  a Crypto Provider algorithm ID of the target object
   * \return size required for storing of the object in the Key Storage
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error SecurityErrorDomain::kUnknownIdentifier  if any argument has an unsupported value
   * \error SecurityErrorDomain::kIncompatibleArguments  if the arguments are incompatible
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual ara::core::Result<std::size_t> GetStorageSize(ObjectType object_type, AlgId alg_id) const noexcept = 0;

  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
  /*!
   * \brief Allocate a Volatile (virtual) Trusted Container according to directly specified capacity.
   * \param[in] capacity  the capacity required for this volatile trusted container (in bytes)
   * \return unique smart pointer to an allocated volatile trusted container
   * \note The Volatile Trusted Container can be used for execution of the import operations.
   * \note Current process obtains the "Owner" rights for allocated Container.
   * \note If (capacity == 0) then the capacity of the container will be selected automatically according to
   *       a maximal size of supported crypto objects.
   * \note A few volatile (temporary) containers can coexist at same time without any affecting each-other.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error SecurityErrorDomain::kBadAlloc  if the requested allocation cannot be executed
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual ara::core::Result<TrustedContainer::Uptr> AllocVolatileContainer(std::size_t capacity = 0) noexcept = 0;

  /*!
   * \brief Allocate a Volatile (virtual) Trusted Container according to indirect specification of a minimal
   *           required capacity for hosting of any listed object.
   * \param[in] objects_list  the list of objects that can be stored to this volatile trusted container
   * \return unique smart pointer to an allocated volatile trusted container
   * \note The Volatile Trusted Container can be used for execution of the import operations.
   * \note Current process obtains the "Owner" rights for allocated Container.
   * \note Real container capacity is calculated as a maximal storage size of all listed objects.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error SecurityErrorDomain::kInvalidArgument  if unsupported combination of object type and
   *                algorithm ID presents in the list
   * \error SecurityErrorDomain::kBadAlloc  if the requested allocation cannot be executed
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual ara::core::Result<TrustedContainer::Uptr> AllocVolatileContainer(
      ContainedObjectsList const& objects_list) noexcept = 0;

  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
  /*!
   * \brief Export a crypto object in a secure manner.
   * \param[in] object  the crypto object for export
   * \param[in] transport_context  the symmetric key wrap context initialized by a transport key (allowed usage:
   *            \c kAllowKeyExporting)
   * \param[out] serialized  the output buffer for the serialized object
   * \return actual capacity required for the serialized data
   * \note if (serialized.empty() == true) then the method returns required size only, but content of the
   *       \c transport_context stays unchanged!
   * \note Only an exportable and completed object (i.e. that have a GUID) can be exported!
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error SecurityErrorDomain::kInsufficientCapacity  if size of the \c serialized buffer is not enough
   *                for saving the output data
   * \error SecurityErrorDomain::kIncompleteArgState  if the \c transport_context is not initialized
   * \error SecurityErrorDomain::kIncompatibleObject  if a key loaded to the \c transport_context doesn't
   *                have required attributes (note: it is an optional error condition for this method)
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual ara::core::Result<std::size_t> ExportSecuredObject(
      CryptoObject const& object, SymmetricKeyWrapperCtx& transport_context,
      WritableMemRegion serialized = WritableMemRegion()) noexcept = 0;

  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
  /*!
   * \brief Export securely an object directly from a trusted container (i.e. without an intermediate creation of
   *           a crypto object).
   * \param[in] container  the trusted container that contains an object for export
   * \param[in] transport_context  the symmetric key wrap context initialized by a transport key (allowed usage:
   *            \c kAllowKeyExporting)
   * \param[out] serialized  the output buffer for the serialized object
   * \return actual capacity required for the serialized data
   * \note if (serialized == nullptr) then the method returns required size only, but content of
   *       the \c transport_context stays unchanged.
   * \note This method can be used for re-exporting of just imported object but on another transport key.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error SecurityErrorDomain::kEmptyContainer  if the \c container is empty
   * \error SecurityErrorDomain::kInsufficientCapacity  if size of the \c serialized buffer is not enough
   *                for saving the output data
   * \error SecurityErrorDomain::kIncompleteArgState  if the \c transport_context is not initialized
   * \error SecurityErrorDomain::kIncompatibleObject  if a key loaded to the \c transport_context doesn't
   *                have required attributes (note: it is an optional error condition for this method)
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual ara::core::Result<std::size_t> ExportSecuredObject(
      TrustedContainer const& container, SymmetricKeyWrapperCtx& transport_context,
      WritableMemRegion serialized = WritableMemRegion()) noexcept = 0;

  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
  /*!
   * \brief Import securely serialized object to a temporary (volatile) trusted container for following
   *           processing (without allocation of a crypto object context).
   * \param[out] container  the prealocated volatile trusted container for storing of the imported object
   * \param[in] serialized  the memory region that contains a securely serialized object that should be imported to
   *            the trusted container
   * \param[in] transport_context  the symmetric key wrap context initialized by a transport key (allowed usage:
   *            \c kAllowKeyImporting)
   * \param[in] is_exportable  the exportability attribute of the target object (this value for public keys and
   *            public domain parameters should be ignored, because they are always exportable)
   * \param[in] expected_object  the expected object type (default value \c ObjectType::kUnknown means without check)
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error SecurityErrorDomain::kUnexpectedValue  if the \c serialized contains incorrect data
   * \error SecurityErrorDomain::kBadObjectType  if (expected_object != ObjectType::kUnknown),
   *                but the actual object type differs from the expected one
   * \error SecurityErrorDomain::kIncompleteArgState  if the \c transport_context is not initialized
   * \error SecurityErrorDomain::kIncompatibleObject  if a key loaded to the \c transport_context doesn't
   *                have required attributes (note: it is an optional error condition for this method)
   * \error SecurityErrorDomain::kInsufficientCapacity  if capacity of the \c container is not enough to
   *                save the deserialized object
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual ara::core::Result<void> ImportSecuredObject(TrustedContainer& container, ReadOnlyMemRegion serialized,
                                                      SymmetricKeyWrapperCtx& transport_context,
                                                      bool is_exportable = false,
                                                      ObjectType expected_object = ObjectType::kUnknown) noexcept = 0;

  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
  /*!
   * \brief Export publicly an object from a trusted container (i.e. without an intermediate creation of a crypto
   *           object context).
   * \param[in] container  the trusted container that contains an object for export
   * \param[out] serialized  the output buffer for the serialized object
   * \param[in] format_id  the Crypto Provider specific identifier of the output format
   * \return actual capacity required for the serialized data
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error SecurityErrorDomain::kEmptyContainer  if the container is empty
   * \error SecurityErrorDomain::kUnexpectedValue  if the container contains a secret crypto object
   * \error SecurityErrorDomain::kInsufficientCapacity  if (serialized.empty() == false),
   *                but its capacity is not enough for storing result
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual ara::core::Result<std::size_t> ExportPublicObject(
      TrustedContainer const& container, WritableMemRegion serialized = WritableMemRegion(),
      Serializable::FormatId format_id = Serializable::kFormatDefault) noexcept = 0;

  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
  /*!
   * \brief Import publicly serialized object to a temporary (volatile) trusted container for following
   *           processing (without allocation of a crypto object context).
   * \param[out] container  the prealocated volatile trusted container for storing of the imported object
   * \param[in] serialized  the memory region that contains a securely serialized object that should be imported to
   *            the trusted container
   * \param[in] expected_object  the expected object type (default value \c ObjectType::kUnknown means without check)
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \note If (expected_object != ObjectType::kUnknown) and an actual object type differs from the expected
   *       one then this method fails.
   * \note If the \c serialized contains incorrect data then this method fails
   * \error SecurityErrorDomain::kUnexpectedValue  if the \c serialized contains incorrect data
   * \error SecurityErrorDomain::kBadObjectType  if (expected_object != ObjectType::kUnknown),
   *                but the actual object type differs from the expected one
   * \error SecurityErrorDomain::kInsufficientCapacity  if capacity of the \c container is not enough to
   *                save the deserialized object
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual ara::core::Result<void> ImportPublicObject(TrustedContainer& container, ReadOnlyMemRegion serialized,
                                                     ObjectType expected_object = ObjectType::kUnknown) noexcept = 0;

  // VECTOR NC AutosarC++17_10-M9.3.3: MD_CRYPTO_AutosarC++17_10-M9.3.3_method_can_be_declared_const
  /*!
   * \brief Import publicly serialized raw object to a trusted container for following processing.
   * \param[out] container  the preallocated trusted container for storing of the imported object
   * \param[in] serialized  the memory region that contains serialized raw data for the object
   * \param[in] alg_id algorithm id of the provided key material
   * \param[in] allowed_usage Flags that define a list of allowed transformations' types
   *                         in which the target key can be used (see constants in scope of KeyMaterial).
   * \context ANY
   * \pre -
   * \error SecurityErrorDomain::kUnexpectedValue  if \c serialized contains incorrect data
   * \error SecurityErrorDomain::kInsufficientCapacity  if capacity of the \c container is not enough to
   *                save the deserialized object
   * \error SecurityErrorDomain::kUnsupported  if raw import is not available for the specified \c alg_id.
   * \reentrant TRUE
   * \synchronous TRUE
   * \vpublic
   */
  virtual ara::core::Result<void> ImportRawObject(
      ara::crypto::TrustedContainer& container, ara::crypto::ReadOnlyMemRegion serialized, AlgId alg_id,
      ara::crypto::cryp::RestrictedUseObject::Usage allowed_usage) noexcept = 0;

  // VECTOR NC AutosarC++17_10-M9.3.3: MD_CRYPTO_AutosarC++17_10-M9.3.3_method_can_be_declared_const
  /*!
   * \brief Import publicly serialized raw object to a trusted container for following processing.
   * \details Uses Domain Parameters for full specification of the object.
   * \param[out] container  the preallocated trusted container for storing of the imported object
   * \param[in] serialized  the memory region that contains serialized raw data for the object
   * \param[in] alg_id algorithm id of the provided key material
   * \param[in] allowed_usage Flags that define a list of allowed transformations' types
   *                         in which the target key can be used (see constants in scope of KeyMaterial).
   * \param[in] params A pointer to Domain Parameters required for full specification of the object.
   * \context ANY
   * \pre -
   * \error SecurityErrorDomain::kUnexpectedValue  if \c serialized contains incorrect data
   * \error SecurityErrorDomain::kInsufficientCapacity  if capacity of the \c container is not enough to
   *                save the deserialized object
   * \error SecurityErrorDomain::kUnsupported  if raw import is not available for the specified \c alg_id.
   * \reentrant TRUE
   * \synchronous TRUE
   * \vpublic
   */
  // VECTOR NC AutosarC++17_10-M9.3.3: MD_CRYPTO_AutosarC++17_10-M9.3.3_AUTOSAR_API
  virtual ara::core::Result<void> ImportRawObject(ara::crypto::TrustedContainer& container,
                                                  ara::crypto::ReadOnlyMemRegion serialized, AlgId alg_id,
                                                  ara::crypto::cryp::RestrictedUseObject::Usage allowed_usage,
                                                  ara::crypto::cryp::DomainParameters::Sptrc params) noexcept = 0;

  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
  /*!
   * \brief Load any crypto object from a "trusted container".
   * \param[in] container  the trusted container that contains the crypto object for loading
   * \param[in] reserved_index  the optional index of reserved Object slot that should be used for this allocation
   * or default marker, which says to allocate on the heap
   * \return unique smart pointer to the created object
   * \note This method is one of the "binding" methods between a Crypto Provider and the Key Storage Provider.
   *       Also this method may implement the Policy Enforcement Point (PEP) for access control via
   *       Identity and Access Management (IAM).
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error SecurityErrorDomain::kEmptyContainer if the container is empty
   * \error SecurityErrorDomain::kUnexpectedValue if the container content is damaged
   * \error SecurityErrorDomain::kBusyResource  if the slot specified by \c reserved_index is busy yet
   * \error SecurityErrorDomain::kUnreservedResource  if the slot specified by \c reserved_index was not allocated
   * \error SecurityErrorDomain::kInsufficientResource  if size of the slot specified by \c reserved_index is not
   * enough for placing of the target object
   * \error SecurityErrorDomain::kBadAlloc  if (reserved_index == kAllocObjectOnHeap), but allocation memory
   * on the heap is impossible
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual ara::core::Result<CryptoObject::Uptrc> LoadObject(
      TrustedContainer const& container, ReservedObjectIndex reserved_index = kAllocObjectOnHeap) noexcept = 0;

  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
  /*!
   * \brief Load concrete crypto object (of specified type) from a "trusted container".
   * \tparam ExpectedObject  the expected type of concrete object
   * \param[in] container  the trusted container that contains the crypto object for loading
   * \param[in] reserved_index  the optional index of reserved Object slot that should be used for this allocation or
   *            default marker, which says to allocate on the heap
   * \return unique smart pointer to the created object
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error SecurityErrorDomain::kEmptyContainer  if the container is empty
   * \error SecurityErrorDomain::kUnexpectedValue  if the container content is damaged
   * \error SecurityErrorDomain::kBusyResource  if the slot specified by \c reserved_index is busy yet
   * \error SecurityErrorDomain::kUnreservedResource  if the slot specified by \c reserved_index
   *                was not allocated
   * \error SecurityErrorDomain::kInsufficientResource  if size of the slot specified by \c reserved_index
   *                is not enough for placing of the target object
   * \error SecurityErrorDomain::kBadAlloc  if (reserved_index == kAllocObjectOnHeap),
   *                but allocation memory on the heap is impossible
   * \error SecurityErrorDomain::kBadObjectType  if an actual type of the container content differs from
   *                the expected type of concrete object
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  /*!
   * \internal
   * - Check if the crypto object matchs the expected type
   * - Retrieve the crypto object from the given container
   * - If retrieving the crypto object succeeds
   *   - Cast the crypto object pointer to a pointer to the expected type
   *   - Return a unique pointer result of the crypto object
   * - Else, fail with error
   * \endinternal
   */
  template <typename ExpectedObject>
  auto LoadConcreteObject(TrustedContainer const& container,
                          ReservedObjectIndex reserved_index = kAllocObjectOnHeap) noexcept
      -> ara::core::Result<typename ExpectedObject::Uptrc> {
    ara::core::Result<typename ExpectedObject::Uptrc> result{ara::crypto::SecurityErrc::kRuntimeFault};

    CryptoObjectType const actual_type{container.GetObjectId()};
    if (ExpectedObject::sObjectType != actual_type) {
      result.EmplaceError(ara::crypto::SecurityErrc::kBadObjectType);
    } else {
      ara::core::Result<CryptoObject::Uptrc> object_result{LoadObject(container, reserved_index)};

      if (!object_result.HasValue()) {
        result.EmplaceError(object_result.Error());
      } else {
        result.EmplaceValue(
            // VECTOR NC AutosarC++17_10-M5.2.3: MD_CRYPTO_AutosarC++17_10-M5.2.3_down_cast
            typename ExpectedObject::Uptrc{static_cast<ExpectedObject const*>(object_result.Value().release())});
      }
    }

    return result;
  }

  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
  /*!
   * \brief Allocate new Password Cache context.
   * \param[in] maximal_length  the maximal supported length of a target password (in characters)
   * \param[in] required_length  the minimal required length of a target password (in characters)
   * \param[in] required_complexity  the minimal required complexity of a target password (0 means "no requirements")
   * \param[in] reserved_index  an optional index of reserved Context slot that should be used for this allocation or
   * default marker, which says to allocate on the heap
   * \return smart unique pointer to the allocated password context
   * \note The complexity is measured by a number symbols' categories (e.g.: lower/upper cases, numbers, special
   *       symbols).
   * \note A maximal supported value of the argument \c maximal_length can be restricted by an implementation.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error SecurityErrorDomain::kBusyResource  if the slot specified by \c reserved_index is busy yet
   * \error SecurityErrorDomain::kUnreservedResource  if the slot specified by \c reserved_index
   *                was not allocated
   * \error SecurityErrorDomain::kInsufficientResource  if size of the slot specified by \c reserved_index
   *                is not enough for placing of the target object
   * \error SecurityErrorDomain::kBadAlloc  if (reserved_index == kAllocObjectOnHeap),
   *                but allocation memory on the heap is impossible
   * \error SecurityErrorDomain::kInvalidArgument  if any of arguments has unsupported value
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual ara::core::Result<PasswordCache::Uptr> AllocPasswordCache(
      std::size_t maximal_length, std::size_t required_length, std::uint32_t required_complexity,
      ReservedContextIndex reserved_index = kAllocContextOnHeap) noexcept = 0;

  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
  /*!
   * \brief Create a password hash object.
   * \param[in] hash_ctx  the hash-function context that should be used in this context
   * \param[in] password  the original password that should be hashed
   * \param[in] is_session  the "session" (or "temporary") attribute for the password hash object (if \c true)
   * \param[in] is_exportable  the exportability attribute for the password hash object (if \c true)
   * \param[in] reserved_index  an optional index of reserved Object slot that should be used for this allocation or
   *            default marker, which says to allocate on the heap
   * \return unique smart pointer to the allocated password hash context or nullptr if the container doesn't have
   *          an object of this type
   * \note An internal hash-function context required by the password hash context should be preallocated by this
   *       method too.
   * \note Any serializable (i.e. savable/non-session or exportable) password hash object must generate own COUID!
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error SecurityErrorDomain::kIncompleteArgState  if the configuring of the hash-function context
   *                (i.e. \c hash_ctx) is not finished yet (e.g. domain parameters are required, but not set yet)
   * \error SecurityErrorDomain::kBusyResource  if the slot specified by \c reserved_index is busy yet
   * \error SecurityErrorDomain::kUnreservedResource  if the slot specified by \c reserved_index
   *                was not allocated
   * \error SecurityErrorDomain::kInsufficientResource  if size of the slot specified by \c reserved_index
   *                is not enough for placing of the target object
   * \error SecurityErrorDomain::kBadAlloc  if (reserved_index == kAllocObjectOnHeap),
   *                but allocation memory on the heap is impossible
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  // VECTOR NC VectorC++-V3.9.2: MD_CRYPTO_VectorC++-V3.9.2_built_in_types_as_function_parameters
  virtual ara::core::Result<PasswordHash::Uptr> HashPassword(
      HashFunctionCtx& hash_ctx, PasswordCache const& password, bool is_session = false, bool is_exportable = false,
      ReservedObjectIndex reserved_index = kAllocObjectOnHeap) noexcept = 0;

  /*!
   * \brief Return a shared pointer to instance of default Random Number Generator (RNG) used by the Crypto
   *           Provider internaly.
   * \return shared smart pointer to default Random Number Generator
   * \note The default RNG should be the best one (i.e. a most secure) from all supported RNG (ideally TRNG).
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error SecurityErrorDomain::kRuntimeFault if the DefaultRng could not created.
   * \error SecurityErrorDomain::kInsufficientResource if the system does not have enough entropy to seed the default
   * RNG.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual ara::core::Result<RandomGeneratorCtx::Sptr> DefaultRng() noexcept = 0;

  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
  /*!
   * \brief Set default Random Number Generator (RNG) instance.
   * \param[in] rng  the shared smart pointer to an instance of the RNG context
   * \note If (rng == nullptr) then internal pointer of default RNG should be returned to original
   *       Crypto Proider-specific default RNG instance.
   * \note Reconfiguration executed by this method affects on the Crypto Provider instance in current process only!
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error SecurityErrorDomain::kIncompleteArgState  if (rng != nullptr), but provided
   *                RNG instance is not initialized yet
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual ara::core::Result<void> SetDefaultRng(RandomGeneratorCtx::Sptr rng = nullptr) noexcept = 0;

  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
  /*!
   * \brief Create a Random Number Generator (RNG) context.
   * \param[in] alg_id  identifier of target RNG algorithm
   * \param[in] reserved_index  an optional index of reserved Context slot that should be used for this allocation or
   *            default marker, which says to allocate on the heap
   * \return shared smart pointer to the created RNG context
   * \note A fully Deterministic RNG should be used only for debugging purposes, but any RNG used "in the field"
   *       should support an internal entropy source (not controllable by application).
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error SecurityErrorDomain::kUnknownIdentifier  if \c alg_id argument has an unsupported value
   * \error SecurityErrorDomain::kBusyResource  if the slot specified by \c reserved_index is busy yet
   * \error SecurityErrorDomain::kUnreservedResource  if the slot specified by \c reserved_index
   *                was not allocated
   * \error SecurityErrorDomain::kInsufficientResource  if size of the slot specified by \c reserved_index
   *                is not enough for placing of the target context
   * \error SecurityErrorDomain::kBadAlloc  if (reserved_index == kAllocObjectOnHeap),
   *                but allocation memory on the heap is impossible
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual ara::core::Result<RandomGeneratorCtx::Sptr> CreateRandomGeneratorCtx(
      AlgId alg_id, ReservedContextIndex reserved_index = kAllocContextOnHeap) noexcept = 0;

  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
  /*!
   * \brief Create a symmetric block cipher context.
   * \param[in] alg_id  identifier of the target crypto algorithm
   * \param[in] reserved_index  an optional index of reserved Context slot that should be used for this allocation or
   *            default marker, which says to allocate on the heap
   * \return unique smart pointer to the created context
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error SecurityErrorDomain::kUnknownIdentifier  if \c alg_id argument has an unsupported value
   * \error SecurityErrorDomain::kBusyResource  if the slot specified by \c reserved_index is busy yet
   * \error SecurityErrorDomain::kUnreservedResource  if the slot specified by \c reserved_index
   *                was not allocated
   * \error SecurityErrorDomain::kInsufficientResource  if size of the slot specified by \c reserved_index
   *                is not enough for placing of the target context
   * \error SecurityErrorDomain::kBadAlloc  if (reserved_index == kAllocObjectOnHeap),
   *                but allocation memory on the heap is impossible
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual ara::core::Result<SymmetricBlockCipherCtx::Uptr> CreateSymmetricBlockCipherCtx(
      AlgId alg_id, ReservedContextIndex reserved_index = kAllocContextOnHeap) noexcept = 0;

  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
  /*!
   * \brief Create a symmetric key-wrap algorithm context.
   * \param[in] alg_id  identifier of the target crypto algorithm
   * \param[in] reserved_index  an optional index of reserved Context slot that should be used for this allocation or
   *            default marker, which says to allocate on the heap
   * \return unique smart pointer to the created context
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error SecurityErrorDomain::kInvalidArgument  if \c alg_id argument specifies a crypto algorithm
   *                different from symmetric key-wrapping
   * \error SecurityErrorDomain::kUnknownIdentifier  if \c alg_id argument has an unsupported value
   * \error SecurityErrorDomain::kBusyResource  if the slot specified by \c reserved_index is busy yet
   * \error SecurityErrorDomain::kUnreservedResource  if the slot specified by \c reserved_index
   *                was not allocated
   * \error SecurityErrorDomain::kInsufficientResource  if size of the slot specified by \c reserved_index
   *                is not enough for placing of the target context
   * \error SecurityErrorDomain::kBadAlloc  if (reserved_index == kAllocObjectOnHeap),
   *                but allocation memory on the heap is impossible
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual ara::core::Result<SymmetricKeyWrapperCtx::Uptr> CreateSymmetricKeyWrapperCtx(
      AlgId alg_id, ReservedContextIndex reserved_index = kAllocContextOnHeap) noexcept = 0;

  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
  /*!
   * \brief Create a symmetric stream cipher context.
   * \param[in] alg_id  identifier of the target crypto algorithm
   * \param[in] reserved_index  an optional index of reserved Context slot that should be used for this allocation or
   *            default marker, which says to allocate on the heap
   * \return unique smart pointer to the created context
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error SecurityErrorDomain::kInvalidArgument  if \c alg_id argument specifies a crypto algorithm
   *                different from symmetric stream cipher
   * \error SecurityErrorDomain::kUnknownIdentifier  if \c alg_id argument has an unsupported value
   * \error SecurityErrorDomain::kBusyResource  if the slot specified by \c reserved_index is busy yet
   * \error SecurityErrorDomain::kUnreservedResource  if the slot specified by \c reserved_index
   *                was not allocated
   * \error SecurityErrorDomain::kInsufficientResource  if size of the slot specified by \c reserved_index
   *                is not enough for placing of the target context
   * \error SecurityErrorDomain::kBadAlloc  if (reserved_index == kAllocObjectOnHeap),
   *                but allocation memory on the heap is impossible
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual ara::core::Result<StreamCipherCtx::Uptr> CreateStreamCipherCtx(
      AlgId alg_id, ReservedContextIndex reserved_index = kAllocContextOnHeap) noexcept = 0;

  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
  /*!
   * \brief Create a symmetric authenticated stream cipher context.
   * \param[in] alg_id  identifier of the target crypto algorithm
   * \param[in] reserved_index  an optional index of reserved Context slot that should be used for this allocation or
   *            default marker, which says to allocate on the heap
   * \return unique smart pointer to the created context
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error SecurityErrorDomain::kInvalidArgument  if \c alg_id argument specifies a crypto algorithm
   *                different from symmetric authenticated stream cipher
   * \error SecurityErrorDomain::kInvalidArgument
   * \error SecurityErrorDomain::kUnknownIdentifier  if \c alg_id argument has an unsupported value
   * \error SecurityErrorDomain::kBusyResource  if the slot specified by \c reserved_index is busy yet
   * \error SecurityErrorDomain::kUnreservedResource  if the slot specified by \c reserved_index
   *                was not allocated
   * \error SecurityErrorDomain::kInsufficientResource  if size of the slot specified by \c reserved_index
   *                is not enough for placing of the target context
   * \error SecurityErrorDomain::kBadAlloc  if (reserved_index == kAllocObjectOnHeap),
   *                but allocation memory on the heap is impossible
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual ara::core::Result<AuthnStreamCipherCtx::Uptr> CreateAuthnStreamCipherCtx(
      AlgId alg_id, ReservedContextIndex reserved_index = kAllocContextOnHeap) noexcept = 0;

  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
  /*!
   * \brief Create a symmetric message authentication code context.
   * \param[in] alg_id  identifier of the target crypto algorithm
   * \param[in] reserved_index  an optional index of reserved Context slot that should be used for this allocation or
   *            default marker, which says to allocate on the heap
   * \return unique smart pointer to the created context
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error SecurityErrorDomain::kInvalidArgument  if \c alg_id argument specifies a crypto algorithm
   *                different from symmetric message authentication code
   * \error SecurityErrorDomain::kUnknownIdentifier  if \c alg_id argument has an unsupported value
   * \error SecurityErrorDomain::kBusyResource  if the slot specified by \c reserved_index is busy yet
   * \error SecurityErrorDomain::kUnreservedResource  if the slot specified by \c reserved_index
   *                was not allocated
   * \error SecurityErrorDomain::kInsufficientResource  if size of the slot specified by \c reserved_index
   *                is not enough for placing of the target context
   * \error SecurityErrorDomain::kBadAlloc  if (reserved_index == kAllocObjectOnHeap),
   *                but allocation memory on the heap is impossible
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual ara::core::Result<MessageAuthnCodeCtx::Uptr> CreateMessageAuthnCodeCtx(
      AlgId alg_id, ReservedContextIndex reserved_index = kAllocContextOnHeap) noexcept = 0;

  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
  /*!
   * \brief Create a hash function context.
   * \param[in] alg_id  identifier of the target crypto algorithm
   * \param[in] reserved_index  an optional index of reserved Context slot that should be used for this allocation or
   *            default marker, which says to allocate on the heap
   * \return unique smart pointer to the created context
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error SecurityErrorDomain::kInvalidArgument  if \c alg_id argument specifies a crypto algorithm
   *                different from hash function
   * \error SecurityErrorDomain::kUnknownIdentifier  if \c alg_id argument has an unsupported value
   * \error SecurityErrorDomain::kBusyResource  if the slot specified by \c reserved_index is busy yet
   * \error SecurityErrorDomain::kUnreservedResource  if the slot specified by \c reserved_index
   *                was not allocated
   * \error SecurityErrorDomain::kInsufficientResource  if size of the slot specified by \c reserved_index
   *                is not enough for placing of the target context
   * \error SecurityErrorDomain::kBadAlloc  if (reserved_index == kAllocObjectOnHeap),
   *                but allocation memory on the heap is impossible
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual ara::core::Result<HashFunctionCtx::Uptr> CreateHashFunctionCtx(
      AlgId alg_id, ReservedContextIndex reserved_index = kAllocContextOnHeap) noexcept = 0;

  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
  /*!
   * \brief Create a key derivation function context.
   * \param[in] alg_id  identifier of the target crypto algorithm
   * \param[in] reserved_index  an optional index of reserved Context slot that should be used for this allocation or
   *            default marker, which says to allocate on the heap
   * \return unique smart pointer to the created context
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error SecurityErrorDomain::kInvalidArgument  if \c alg_id argument specifies a crypto algorithm
   *                different from key derivation function
   * \error SecurityErrorDomain::kUnknownIdentifier  if \c alg_id argument has an unsupported value
   * \error SecurityErrorDomain::kBusyResource  if the slot specified by \c reserved_index is busy yet
   * \error SecurityErrorDomain::kUnreservedResource  if the slot specified by \c reserved_index
   *                was not allocated
   * \error SecurityErrorDomain::kInsufficientResource  if size of the slot specified by \c reserved_index
   *                is not enough for placing of the target context
   * \error SecurityErrorDomain::kBadAlloc  if (reserved_index == kAllocObjectOnHeap),
   *                but allocation memory on the heap is impossible
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual ara::core::Result<KeyDerivationFunctionCtx::Uptr> CreateKeyDerivationFunctionCtx(
      AlgId alg_id, ReservedContextIndex reserved_index = kAllocContextOnHeap) noexcept = 0;

  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
  /*!
   * \brief Create a symmetric key diversification context.
   * \param[in] master_alg_id  the crypto algorithm identifier of master-keys
   * \param[in] slave_key_length  the length of target slave-keys (derived from the master) in bits
   * \param[in] reserved_index  an optional index of reserved Context slot that should be used for this allocation or
   *            default marker, which says to allocate on the heap
   * \return unique smart pointer to the created context
   * \note slave_alg_id can have partial specification (only algorithm family and key length are required, but the mode
   *       and padding are optional).
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error SecurityErrorDomain::kInvalidArgument  if \c alg_id argument specifies a crypto algorithm
   *                different from symmetric key diversification
   * \error SecurityErrorDomain::kIncompatibleArguments  if requested \c slave_key_length value is unsupported
   *                for specified \c master_alg_id
   * \error SecurityErrorDomain::kUnknownIdentifier  if \c alg_id argument has an unsupported value
   * \error SecurityErrorDomain::kBusyResource  if the slot specified by \c reserved_index is busy yet
   * \error SecurityErrorDomain::kUnreservedResource  if the slot specified by \c reserved_index
   *                was not allocated
   * \error SecurityErrorDomain::kInsufficientResource  if size of the slot specified by \c reserved_index
   *                is not enough for placing of the target context
   * \error SecurityErrorDomain::kBadAlloc  if (reserved_index == kAllocObjectOnHeap),
   *                but allocation memory on the heap is impossible
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual ara::core::Result<KeyDiversifierCtx::Uptr> CreateKeyDiversifierCtx(
      AlgId master_alg_id, std::size_t slave_key_length,
      ReservedContextIndex reserved_index = kAllocContextOnHeap) noexcept = 0;

  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
  /*!
   * \brief Create an encryption public key context.
   * \param[in] alg_id  identifier of the target asymmetric encryption/decryption algorithm
   * \param[in] reserved_index  an optional index of reserved Context slot that should be used for this allocation or
   *            default marker, which says to allocate on the heap
   * \return unique smart pointer to the created context
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error SecurityErrorDomain::kInvalidArgument  if \c alg_id argument specifies a crypto algorithm
   *                different from asymmetric encryption/decryption
   * \error SecurityErrorDomain::kUnknownIdentifier  if \c alg_id argument has an unsupported value
   * \error SecurityErrorDomain::kBusyResource  if the slot specified by \c reserved_index is busy yet
   * \error SecurityErrorDomain::kUnreservedResource  if the slot specified by \c reserved_index
   *                was not allocated
   * \error SecurityErrorDomain::kInsufficientResource  if size of the slot specified by \c reserved_index
   *                is not enough for placing of the target context
   * \error SecurityErrorDomain::kBadAlloc  if (reserved_index == kAllocObjectOnHeap),
   *                but allocation memory on the heap is impossible
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual ara::core::Result<EncryptorPublicCtx::Uptr> CreateEncryptorPublicCtx(
      AlgId alg_id, ReservedContextIndex reserved_index = kAllocContextOnHeap) noexcept = 0;

  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
  /*!
   * \brief Create a decryption private key context.
   * \param[in] alg_id  identifier of the target asymmetric encryption/decryption algorithm
   * \param[in] reserved_index  an optional index of reserved Context slot that should be used for this allocation or
   *            default marker, which says to allocate on the heap
   * \return unique smart pointer to the created context
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error SecurityErrorDomain::kInvalidArgument  if \c alg_id argument specifies a crypto algorithm
   *                different from asymmetric encryption/decryption
   * \error SecurityErrorDomain::kUnknownIdentifier  if \c alg_id argument has an unsupported value
   * \error SecurityErrorDomain::kBusyResource  if the slot specified by \c reserved_index is busy yet
   * \error SecurityErrorDomain::kUnreservedResource  if the slot specified by \c reserved_index
   *                was not allocated
   * \error SecurityErrorDomain::kInsufficientResource  if size of the slot specified by \c reserved_index
   *                is not enough for placing of the target context
   * \error SecurityErrorDomain::kBadAlloc  if (reserved_index == kAllocObjectOnHeap),
   *                but allocation memory on the heap is impossible
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual ara::core::Result<DecryptorPrivateCtx::Uptr> CreateDecryptorPrivateCtx(
      AlgId alg_id, ReservedContextIndex reserved_index = kAllocContextOnHeap) noexcept = 0;

  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
  /*!
   * \brief Create a key-encapsulator public key context of a Key Encapsulation Mechanism (KEM).
   * \param[in] alg_id  identifier of the target KEM crypto algorithm
   * \param[in] reserved_index  an optional index of reserved Context slot that should be used for this allocation or
   *            default marker, which says to allocate on the heap
   * \return unique smart pointer to the created context
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error SecurityErrorDomain::kInvalidArgument  if \c alg_id argument specifies a crypto algorithm
   *                different from asymmetric KEM
   * \error SecurityErrorDomain::kUnknownIdentifier  if \c alg_id argument has an unsupported value
   * \error SecurityErrorDomain::kBusyResource  if the slot specified by \c reserved_index is busy yet
   * \error SecurityErrorDomain::kUnreservedResource  if the slot specified by \c reserved_index
   *                was not allocated
   * \error SecurityErrorDomain::kInsufficientResource  if size of the slot specified by \c reserved_index
   *                is not enough for placing of the target context
   * \error SecurityErrorDomain::kBadAlloc  if (reserved_index == kAllocObjectOnHeap),
   *                but allocation memory on the heap is impossible
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual ara::core::Result<KeyEncapsulatorPublicCtx::Uptr> CreateKeyEncapsulatorPublicCtx(
      AlgId alg_id, ReservedContextIndex reserved_index = kAllocContextOnHeap) noexcept = 0;

  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
  /*!
   * \brief Create a key-decapsulator private key context of a Key Encapsulation Mechanism (KEM).
   * \param[in] alg_id  identifier of the target KEM crypto algorithm
   * \param[in] reserved_index  an optional index of reserved Context slot that should be used for this allocation or
   *            default marker, which says to allocate on the heap
   * \return unique smart pointer to the created context
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error SecurityErrorDomain::kInvalidArgument  if \c alg_id argument specifies a crypto algorithm
   *                different from asymmetric KEM
   * \error SecurityErrorDomain::kUnknownIdentifier  if \c alg_id argument has an unsupported value
   * \error SecurityErrorDomain::kBusyResource  if the slot specified by \c reserved_index is busy yet
   * \error SecurityErrorDomain::kUnreservedResource  if the slot specified by \c reserved_index
   *                was not allocated
   * \error SecurityErrorDomain::kInsufficientResource  if size of the slot specified by \c reserved_index
   *                is not enough for placing of the target context
   * \error SecurityErrorDomain::kBadAlloc  if (reserved_index == kAllocObjectOnHeap),
   *                but allocation memory on the heap is impossible
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual ara::core::Result<KeyDecapsulatorPrivateCtx::Uptr> CreateKeyDecapsulatorPrivateCtx(
      AlgId alg_id, ReservedContextIndex reserved_index = kAllocContextOnHeap) noexcept = 0;

  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
  /*!
   * \brief Create a signature encoding private key context.
   * \param[in] alg_id  identifier of the target asymmetric crypto algorithm
   * \param[in] reserved_index  an optional index of reserved Context slot that should be used for this allocation or
   *            default marker, which says to allocate on the heap
   * \return unique smart pointer to the created context
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error SecurityErrorDomain::kInvalidArgument  if \c alg_id argument specifies a crypto algorithm
   *                different from asymmetric signature encoding with message recovery
   * \error SecurityErrorDomain::kUnknownIdentifier  if \c alg_id argument has an unsupported value
   * \error SecurityErrorDomain::kBusyResource  if the slot specified by \c reserved_index is busy yet
   * \error SecurityErrorDomain::kUnreservedResource  if the slot specified by \c reserved_index
   *                was not allocated
   * \error SecurityErrorDomain::kInsufficientResource  if size of the slot specified by \c reserved_index
   *                is not enough for placing of the target context
   * \error SecurityErrorDomain::kBadAlloc  if (reserved_index == kAllocObjectOnHeap),
   *                but allocation memory on the heap is impossible
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual ara::core::Result<SigEncodePrivateCtx::Uptr> CreateSigEncodePrivateCtx(
      AlgId alg_id, ReservedContextIndex reserved_index = kAllocContextOnHeap) noexcept = 0;

  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
  /*!
   * \brief Create a message recovery public key context.
   * \param[in] alg_id  identifier of the target asymmetric crypto algorithm
   * \param[in] reserved_index  an optional index of reserved Context slot that should be used for this allocation or
   *            default marker, which says to allocate on the heap
   * \return unique smart pointer to the created context
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error SecurityErrorDomain::kInvalidArgument  if \c alg_id argument specifies a crypto algorithm
   *                different from asymmetric signature encoding with message recovery
   * \error SecurityErrorDomain::kUnknownIdentifier  if \c alg_id argument has an unsupported value
   * \error SecurityErrorDomain::kBusyResource  if the slot specified by \c reserved_index is busy yet
   * \error SecurityErrorDomain::kUnreservedResource  if the slot specified by \c reserved_index
   *                was not allocated
   * \error SecurityErrorDomain::kInsufficientResource  if size of the slot specified by \c reserved_index
   *                is not enough for placing of the target context
   * \error SecurityErrorDomain::kBadAlloc  if (reserved_index == kAllocObjectOnHeap),
   *                but allocation memory on the heap is impossible
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual ara::core::Result<MsgRecoveryPublicCtx::Uptr> CreateMsgRecoveryPublicCtx(
      AlgId alg_id, ReservedContextIndex reserved_index = kAllocContextOnHeap) noexcept = 0;

  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
  /*!
   * \brief Create a signature private key context.
   * \param[in] alg_id  identifier of the target signature crypto algorithm
   * \param[in] reserved_index  an optional index of reserved Context slot that should be used for this allocation or
   *            default marker, which says to allocate on the heap
   * \return unique smart pointer to the created context
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error SecurityErrorDomain::kInvalidArgument  if \c alg_id argument specifies a crypto algorithm
   *                different from private key signature
   * \error SecurityErrorDomain::kUnknownIdentifier  if \c alg_id argument has an unsupported value
   * \error SecurityErrorDomain::kBusyResource  if the slot specified by \c reserved_index is busy yet
   * \error SecurityErrorDomain::kUnreservedResource  if the slot specified by \c reserved_index
   *                was not allocated
   * \error SecurityErrorDomain::kInsufficientResource  if size of the slot specified by \c reserved_index
   *                is not enough for placing of the target context
   * \error SecurityErrorDomain::kBadAlloc  if (reserved_index == kAllocObjectOnHeap),
   *                but allocation memory on the heap is impossible
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual ara::core::Result<SignerPrivateCtx::Uptr> CreateSignerPrivateCtx(
      AlgId alg_id, ReservedContextIndex reserved_index = kAllocContextOnHeap) noexcept = 0;

  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
  /*!
   * \brief Create a signature verification public key context.
   * \param[in] alg_id  identifier of the target signature crypto algorithm
   * \param[in] reserved_index  an optional index of reserved Context slot that should be used for this allocation or
   *            default marker, which says to allocate on the heap
   * \return unique smart pointer to the created context
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error SecurityErrorDomain::kInvalidArgument  if \c alg_id argument specifies a crypto algorithm
   *                different from public key signature verification
   * \error SecurityErrorDomain::kUnknownIdentifier  if \c alg_id argument has an unsupported value
   * \error SecurityErrorDomain::kBusyResource  if the slot specified by \c reserved_index is busy yet
   * \error SecurityErrorDomain::kUnreservedResource  if the slot specified by \c reserved_index
   *                was not allocated
   * \error SecurityErrorDomain::kInsufficientResource  if size of the slot specified by \c reserved_index
   *                is not enough for placing of the target context
   * \error SecurityErrorDomain::kBadAlloc  if (reserved_index == kAllocObjectOnHeap),
   *                but allocation memory on the heap is impossible
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual ara::core::Result<VerifierPublicCtx::Uptr> CreateVerifierPublicCtx(
      AlgId alg_id, ReservedContextIndex reserved_index = kAllocContextOnHeap) noexcept = 0;

  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
  /*!
   * \brief Create a key-agreement private key context.
   * \param[in] alg_id  identifier of the target key-agreement crypto algorithm
   * \param[in] reserved_index  an optional index of reserved Context slot that should be used for this allocation or
   *            default marker, which says to allocate on the heap
   * \return unique smart pointer to the created context
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error SecurityErrorDomain::kInvalidArgument  if \c alg_id argument specifies a crypto algorithm
   *                different from key-agreement
   * \error SecurityErrorDomain::kUnknownIdentifier  if \c alg_id argument has an unsupported value
   * \error SecurityErrorDomain::kBusyResource  if the slot specified by \c reserved_index is busy yet
   * \error SecurityErrorDomain::kUnreservedResource  if the slot specified by \c reserved_index
   *                was not allocated
   * \error SecurityErrorDomain::kInsufficientResource  if size of the slot specified by \c reserved_index
   *                is not enough for placing of the target context
   * \error SecurityErrorDomain::kBadAlloc  if (reserved_index == kAllocObjectOnHeap),
   *                but allocation memory on the heap is impossible
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual ara::core::Result<KeyAgreementPrivateCtx::Uptr> CreateKeyAgreementPrivateCtx(
      AlgId alg_id, ReservedContextIndex reserved_index = kAllocContextOnHeap) noexcept = 0;

  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
  /*!
   * \brief Create an X.509 certificate request signer context.
   * \param[in] alg_id  identifier of the target signature crypto algorithm that should be used for hashing and
   *            signature of certification requests produced by it
   * \param[in] reserved_index  an optional index of reserved Context slot that should be used for this allocation or
   *            default marker, which says to allocate on the heap
   * \return unique smart pointer to the created context
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error SecurityErrorDomain::kInvalidArgument  if \c alg_id argument specifies a crypto algorithm
   *                different from private key signature or doesn't include hash algorithm specification
   * \error SecurityErrorDomain::kUnknownIdentifier  if \c alg_id argument has an unsupported value
   * \error SecurityErrorDomain::kBusyResource  if the slot specified by \c reserved_index is busy yet
   * \error SecurityErrorDomain::kUnreservedResource  if the slot specified by \c reserved_index
   *                was not allocated
   * \error SecurityErrorDomain::kInsufficientResource  if size of the slot specified by \c reserved_index
   *                is not enough for placing of the target context
   * \error SecurityErrorDomain::kBadAlloc  if (reserved_index == kAllocObjectOnHeap),
   *                but allocation memory on the heap is impossible
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual ara::core::Result<X509RequestSignerCtx::Uptr> CreateX509RequestSignerCtx(
      AlgId alg_id, ReservedContextIndex reserved_index = kAllocContextOnHeap) noexcept = 0;
};

}  // namespace cryp
}  // namespace crypto
}  // namespace ara

#endif  // LIB_LIBARA_INCLUDE_ARA_CRYPTO_CRYP_CRYPTO_PROVIDER_H_
