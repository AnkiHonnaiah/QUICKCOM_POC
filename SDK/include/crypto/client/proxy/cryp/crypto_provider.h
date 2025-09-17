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
 *
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file  proxy/cryp/crypto_provider.h
 *        \brief  Proxy implementation for ara::crypto::cryp::CryptoProvider.
 *
 *      \details  Client side implementation of the ara::crypto::cryp::CryptoProvider interface that
 *                transparently communicates with a server side implementation that performs the real cryptography.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBCLIENT_INCLUDE_CRYPTO_CLIENT_PROXY_CRYP_CRYPTO_PROVIDER_H_
#define LIB_LIBCLIENT_INCLUDE_CRYPTO_CLIENT_PROXY_CRYP_CRYPTO_PROVIDER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include <utility>

#include "amsr/sec/ipc/message_builder.h"
#include "amsr/sec/ipc/message_deserializer.h"
#include "amsr/sec/ipc/message_serializer.h"
#include "amsr/sec/ipc/transporter.h"
#include "ara/core/string.h"
#include "ara/core/string_view.h"
#include "ara/crypto/common/mem_region.h"
#include "ara/crypto/common/provider_info.h"
#include "ara/crypto/common/std_api.h"
#include "ara/crypto/common/trusted_container.h"
#include "ara/crypto/cryp/crypto_provider.h"
#include "crypto/client/base_proxy.h"

namespace crypto {
namespace client {
namespace proxy {
namespace cryp {

// VECTOR NC AutosarC++17_10-A10.1.1: MD_CRYPTO_AutosarC++17_10-A10.1.1_multiple_inheritance_false_positive
// VECTOR NC Metric-OO.WMC.One, Metric-OO.RFC: MD_CRYPTO_Metric-OO.WMC.One, MD_CRYPTO_Metric-OO.RFC
/*!
 * \brief Client side proxy implementation for ara::crypto::cryp::CryptoProvider
 * \vprivate Component Private
 * \trace DSGN-Crypto-ProcessIsolation, DSGN-Crypto-Skeleton_Proxy
 */
class CryptoProvider : public BaseProxy, public ara::crypto::cryp::CryptoProvider {
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
  // VECTOR NC VectorC++-V12.1.1: MD_CRYPTO_VectorC++-V12.1.1_base_class_initialization_false_positive
  /*!
   * \brief Constructs instance of class.
   * \param[in] transporter           Transporter interface for communication between client and server objects
   * \param[in] message_builder       Builder for messages to be sent to the server
   * \param[in] message_serializer    Serializer for serializing messages to send them via the transporter interface
   * \param[in] message_deserializer  Deserializer for deserializing responses received via the transporter interface
   * \vprivate Component Private
   */
  CryptoProvider(amsr::sec::ipc::Transporter& transporter, amsr::sec::ipc::MessageBuilder message_builder,
                 amsr::sec::ipc::MessageSerializer message_serializer,
                 amsr::sec::ipc::MessageDeserializer message_deserializer) noexcept
      : BaseProxy(transporter, message_builder_, message_serializer_, message_deserializer_),
        ara::crypto::cryp::CryptoProvider(),
        logger_{common::util::kClientLoggingContext, ""_sv},
        message_builder_{message_builder},
        message_serializer_{std::move(message_serializer)},
        message_deserializer_{std::move(message_deserializer)} {}

  /*!
   * \brief Constructor
   * \param[in] transporter Transporter interface for communication between client and server objects
   * \vprivate Component Private
   */
  explicit CryptoProvider(amsr::sec::ipc::Transporter& transporter) noexcept
      : CryptoProvider(transporter, amsr::sec::ipc::MessageBuilder{}, amsr::sec::ipc::MessageSerializer{},
                       amsr::sec::ipc::MessageDeserializer{}) {}

  /*!
   * \brief Default destructor
   * \vprivate Component private
   */
  ~CryptoProvider() noexcept override = default;

  // VECTOR NC AutosarC++17_10-A10.2.1: MD_CRYPTO_AutosarC++17_10-A10.2.1_hiding_functions
  /*!
   * \brief Deleted copy assignment operator
   * \return -
   * \vprivate Component Private
   */
  CryptoProvider& operator=(CryptoProvider const& /*other*/) = delete;

  // VECTOR NC AutosarC++17_10-A10.2.1: MD_CRYPTO_AutosarC++17_10-A10.2.1_hiding_functions
  /*!
   * \brief Deleted move assignment operator
   * \return -
   * \vprivate Component Private
   */
  CryptoProvider& operator=(CryptoProvider&& /*other*/) = delete;

  /*!
   * \brief Converts a common name of crypto algorithm to a correspondent vendor specific binary algorithm ID.
   * \param[in] primitive_name A unified name of the crypto primitive
   *                          (see "Crypto Primitives Naming Convention" for more details).
   * \return Vendor specific binary algorithm ID or kAlgIdUndefined if
   *         a primitive with provided name is not supported.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  AlgId ConvertToAlgId(ara::core::StringView primitive_name) const noexcept override;

  /*!
   * \brief Converts a vendor specific binary algorithm ID to a correspondent common name of the crypto algorithm.
   * \param[in] alg_id A vendor specific binary algorithm ID.
   * \return A common name of the crypto algorithm.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error ara::crypto::SecurityErrc::kUnknownIdentifier If the algorithm ID is unknown.
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ara::core::Result<ara::core::String> ConvertToAlgName(AlgId alg_id) const noexcept override;

  /*!
   * \brief Reserves memory for simultaneous hosting of all Contexts specified by the map.
   * \param[in] reservation_map Contexts reservation map that defines minimal required size for
   *                           each reserved Context slot.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error ara::crypto::SecurityErrc::kUnsupported Method not supported.
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ara::core::Result<void> ReserveContexts(ContextReservationMap const& reservation_map) noexcept override;

  /*!
   * \brief Reserves memory for simultaneous hosting of all Objects specified by the map.
   * \param[in] reservation_map Objects reservation map that defines minimal
   *                           required size for each reserved Object slot.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error ara::crypto::SecurityErrc::kUnsupported Method not supported.
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ara::core::Result<void> ReserveObjects(ObjectReservationMap const& reservation_map) noexcept override;

  /*!
   * \brief Reserves memory for simultaneous hosting of specified quantity of any type Contexts,
   *        i.e. maximal capacity will be reserved for each Context.
   * \param[in] quantity Number of Contexts for reservation.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error ara::crypto::SecurityErrc::kUnsupported Method not supported.
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ara::core::Result<void> ReserveContexts(std::size_t quantity) noexcept override;

  /*!
   * \brief Reserves memory for simultaneous hosting of specified quantity of any type Objects,
   *        i.e. maximal capacity will be reserved for each Object.
   * \param[in] quantity Number of Objects for reservation.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error ara::crypto::SecurityErrc::kUnsupported Method not supported.
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ara::core::Result<void> ReserveObjects(std::size_t quantity) noexcept override;

  /*!
   * \brief Switches the memory management engine of the current execution thread to the "Real-Time" mode.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error ara::crypto::SecurityErrc::kUnsupported Method not supported.
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  void EnterRealTimeMode() noexcept override;

  /*!
   * \brief Switches the memory management engine of the current execution thread to the "Non-Real-Time" mode.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error ara::crypto::SecurityErrc::kUnsupported Method not supported.
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  void LeaveRealTimeMode() noexcept override;

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
   * \error ara::crypto::SecurityErrc::kUnknownIdentifier The given algorithm ID could not be used to allocate domain
   * parameters.
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ara::core::Result<ara::crypto::cryp::DomainParameters::Sptr> AllocDomainParameters(
      AlgId alg_id, bool is_session, bool is_exportable,
      ara::crypto::cryp::ReservedObjectIndex reserved_index) noexcept override;

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
   * \error ara::crypto::SecurityErrc::kUnreservedResource If (reserved_index != ara::crypto::cryp::kAllocObjectOnHeap)
                                                           Symmetric keys can only be generated on the heap.
   * \error ara::crypto::SecurityErrc::kUnknownIdentifier The symmetric key could not be generated for the given
   algorithm ID.
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ara::core::Result<ara::crypto::cryp::SymmetricKey::Uptrc> GenerateSymmetricKey(
      AlgId alg_id, ara::crypto::cryp::RestrictedUseObject::Usage allowed_usage, bool is_session, bool is_exportable,
      ara::crypto::cryp::DomainParameters::Sptrc params,
      ara::crypto::cryp::ReservedObjectIndex reserved_index) noexcept override;

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
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ara::core::Result<ara::crypto::cryp::DomainParameters::Sptrc> KnownDomainParameters(
      ara::core::StringView oid_name, ara::crypto::cryp::ReservedObjectIndex reserved_index) noexcept override;

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
   * \error ara::crypto::SecurityErrc::kUnreservedResource If (reserved_index != ara::crypto::cryp::kAllocObjectOnHeap)
                                                           Private keys can only be generated on the heap.
   * \error ara::crypto::SecurityErrc::kUnknownIdentifier The private key could not be generated for the given algorithm
   ID.
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ara::core::Result<ara::crypto::cryp::PrivateKey::Uptrc> GeneratePrivateKey(
      AlgId alg_id, ara::crypto::cryp::RestrictedUseObject::Usage allowed_usage, bool is_session, bool is_exportable,
      ara::crypto::cryp::DomainParameters::Sptrc params,
      ara::crypto::cryp::ReservedObjectIndex reserved_index) noexcept override;

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
   * \error ara::crypto::SecurityErrc::kUnsupported Method not supported.
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ara::core::Result<ara::crypto::cryp::SecretSeed::Uptrc> GenerateSeed(
      AlgId alg_id, ara::crypto::cryp::SecretSeed::Usage allowed_usage, bool is_session, bool is_exportable,
      ara::crypto::cryp::ReservedObjectIndex reserved_index) noexcept override;

  /*!
   * \brief Return required buffer size for serialization of an object in specific format.
   * \param[in] object_type  the type of the target object
   * \param[in] alg_id  the Crypto Provider algorithm ID of the target object
   * \param[in] format_id  the Crypto Provider specific identifier of the output format
   * \return size required for storing of the object serialized in the specified format
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error ara::crypto::SecurityErrc::kUnsupported Method not supported.
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ara::core::Result<std::size_t> GetSerializedSize(
      ObjectType object_type, AlgId alg_id, ara::crypto::Serializable::FormatId format_id) const noexcept override;

  /*!
   * \brief Return required capacity of a key slot for saving of the object.
   * \param[in] object_type  the type of the target object
   * \param[in] alg_id  a Crypto Provider algorithm ID of the target object
   * \return size required for storing of the object in the Key Storage
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error ara::crypto::SecurityErrc::kUnsupported Method not supported.
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ara::core::Result<std::size_t> GetStorageSize(ObjectType object_type, AlgId alg_id) const noexcept override;

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
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ara::core::Result<ara::crypto::TrustedContainer::Uptr> AllocVolatileContainer(std::size_t capacity) noexcept override;

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
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ara::core::Result<ara::crypto::TrustedContainer::Uptr> AllocVolatileContainer(
      ContainedObjectsList const& objects_list) noexcept override;

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
   * \error ara::crypto::SecurityErrc::kUnsupported Method not supported.
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ara::core::Result<std::size_t> ExportSecuredObject(ara::crypto::cryp::CryptoObject const& object,
                                                     ara::crypto::cryp::SymmetricKeyWrapperCtx& transport_context,
                                                     ara::crypto::WritableMemRegion serialized) noexcept override;

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
   * \error ara::crypto::SecurityErrc::kUnsupported Method not supported.
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ara::core::Result<std::size_t> ExportSecuredObject(ara::crypto::TrustedContainer const& container,
                                                     ara::crypto::cryp::SymmetricKeyWrapperCtx& transport_context,
                                                     ara::crypto::WritableMemRegion serialized) noexcept override;

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
   * \error ara::crypto::SecurityErrc::kUnsupported Method not supported.
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ara::core::Result<void> ImportSecuredObject(ara::crypto::TrustedContainer& container,
                                              ara::crypto::ReadOnlyMemRegion serialized,
                                              ara::crypto::cryp::SymmetricKeyWrapperCtx& transport_context,
                                              bool is_exportable, ObjectType expected_object) noexcept override;

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
   * \error ara::crypto::SecurityErrc::kAccessViolation The provided trusted container is not exportable.
   * \error ara::crypto::SecurityErrc::kIncompatibleArguments The provided trusted container is invalid or not
   * compatible.
   * \error ara::crypto::SecurityErrc::kInsufficientCapacity The provided buffer is too small to hold
   * serialized data.
   * \error ara::crypto::SecurityErrc::kRuntimeFault Unable to determine serialized size or unable to
   * export trusted container.
   * \reentrant TRUE
   * \synchronous TRUE
   * \trace CREQ-Crypto-ExportCryptographicMaterial
   * \vprivate Component Private
   */
  ara::core::Result<std::size_t> ExportPublicObject(ara::crypto::TrustedContainer const& container,
                                                    ara::crypto::WritableMemRegion serialized,
                                                    ara::crypto::Serializable::FormatId format_id) noexcept override;

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
   * \error ara::crypto::SecurityErrc::kAccessViolation The provided trusted container is not exportable.
   * \error ara::crypto::SecurityErrc::kIncompatibleArguments Trusted container is not a volatile container or has the
   * wrong type. \error ara::crypto::SecurityErrc::kUnexpectedValue Unable to import the object. \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ara::core::Result<void> ImportPublicObject(ara::crypto::TrustedContainer& container,
                                             ara::crypto::ReadOnlyMemRegion serialized,
                                             ObjectType expected_object) noexcept override;

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
   * \trace CREQ-Crypto-ImportCryptographicMaterial
   * \vprivate Component Private
   */
  ara::core::Result<void> ImportRawObject(
      ara::crypto::TrustedContainer& container, ara::crypto::ReadOnlyMemRegion serialized, AlgId alg_id,
      ara::crypto::cryp::RestrictedUseObject::Usage allowed_usage) noexcept override;

  /*!
   * \brief Import publicly serialized raw object to a trusted container for following processing.
   * \details Uses Domain Parameters for full specification of the object.
   * \param[out] container  the preallocated trusted container for storing of the imported object
   * \param[in] serialized  the memory region that contains serialized raw data for the object
   * \param[in] alg_id algorithm id of the provided key material
   * \param[in] allowed_usage Flags that define a list of allowed transformations' types
   *                         in which the target key can be used (see constants in scope of KeyMaterial).
   * \param[in] params An pointer to Domain Parameters required for full specification of the object.
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
  ara::core::Result<void> ImportRawObject(ara::crypto::TrustedContainer& container,
                                          ara::crypto::ReadOnlyMemRegion serialized, AlgId alg_id,
                                          ara::crypto::cryp::RestrictedUseObject::Usage allowed_usage,
                                          ara::crypto::cryp::DomainParameters::Sptrc params) noexcept override;

  /*!
   * \brief Load any crypto object from a "trusted container".
   * \param[in] container  the trusted container that contains the crypto object for loading
   * \param[in] reserved_index  the optional index of reserved Object slot that should be used for this allocation
   * or default marker, which says to allocate on the heap
   * \return unique smart pointer to the created object
   * \error SecurityErrorDomain::kEmptyContainer  if the container is empty
   * \error SecurityErrorDomain::kUnexpectedValue if the container content is damaged
   * \error SecurityErrorDomain::kBusyResource  if the slot specified by \c reserved_index is busy yet
   * \error SecurityErrorDomain::kUnreservedResource  if the slot specified by \c reserved_index was not allocated
   * \error SecurityErrorDomain::kInsufficientResource  if size of the slot specified by \c reserved_index is not enough
   * for placing of the target object
   * \error SecurityErrorDomain::kBadAlloc  if (reserved_index == kAllocObjectOnHeap), but allocation memory on the heap
   * is impossible
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error ara::crypto::SecurityErrc::kUnreservedResource If (reserved_index != ara::crypto::cryp::kAllocContextOnHeap)
                                                           Only heap storage supported.
   * \error ara::crypto::SecurityErrc::kIncompatibleArguments The provided trusted container is invalid or not
   compatible.
   * \error ara::crypto::SecurityErrc::kUnknownIdentifier Could not load key from trusted container
   *                                                      or crypto object type not supported.
   * \error ara::crypto::SecurityErrc::kEmptyContainer The container's algorithm ID is unknown.
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ara::core::Result<ara::crypto::cryp::CryptoObject::Uptrc> LoadObject(
      ara::crypto::TrustedContainer const& container,
      ara::crypto::cryp::ReservedObjectIndex reserved_index) noexcept override;

  /*!
   * \brief Allocate new Password Cache context.
   * \param[in] maximal_length  the maximal supported length of a target password (in characters)
   * \param[in] required_length  the minimal required length of a target password (in characters)
   * \param[in] required_complexity  the minimal required complexity of a target password (0 means "no
   * requirements") \param[in] reserved_index  an optional index of reserved Context slot that should be used for
   * this allocation or default marker, which says to allocate on the heap \return smart unique pointer to the
   * allocated password context
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error ara::crypto::SecurityErrc::kUnsupported Method not supported.
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ara::core::Result<ara::crypto::cryp::PasswordCache::Uptr> AllocPasswordCache(
      std::size_t maximal_length, std::size_t required_length, std::uint32_t required_complexity,
      ara::crypto::cryp::ReservedContextIndex reserved_index) noexcept override;

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
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error ara::crypto::SecurityErrc::kUnsupported Method not supported.
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ara::core::Result<ara::crypto::cryp::PasswordHash::Uptr> HashPassword(
      ara::crypto::cryp::HashFunctionCtx& hash_ctx, ara::crypto::cryp::PasswordCache const& password, bool is_session,
      bool is_exportable, ara::crypto::cryp::ReservedObjectIndex reserved_index) noexcept override;

  /*!
   * \brief Return a shared pointer to instance of default Random Number Generator (RNG) used by the Crypto
   *           Provider internaly.
   * \return shared smart pointer to default Random Number Generator
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error ara::crypto::SecurityErrc::kBadObjectReference Unable to load default RNG.
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ara::core::Result<ara::crypto::cryp::RandomGeneratorCtx::Sptr> DefaultRng() noexcept override;

  /*!
   * \brief Set default Random Number Generator (RNG) instance.
   * \param[in] rng  the shared smart pointer to an instance of the RNG context
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error ara::crypto::SecurityErrc::kBadObjectReference Unable to load default RNG.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ara::core::Result<void> SetDefaultRng(ara::crypto::cryp::RandomGeneratorCtx::Sptr rng) noexcept override;

  /*!
   * \brief Create a Random Number Generator (RNG) context.
   * \param[in] alg_id  identifier of target RNG algorithm
   * \param[in] reserved_index  an optional index of reserved Context slot that should be used for this allocation or
   *            default marker, which says to allocate on the heap
   * \return shared smart pointer to the created RNG context
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error ara::crypto::SecurityErrc::kRuntimeFault Initialization or algorithm ID derivation failed.
   * \error ara::crypto::SecurityErrc::kInsufficientResource Unable to seed the default RNG.
   * \error ara::crypto::SecurityErrc::kUnknownIdentifier Unable to generate random generator context for the given
   * algorithm ID.
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ara::core::Result<ara::crypto::cryp::RandomGeneratorCtx::Sptr> CreateRandomGeneratorCtx(
      AlgId alg_id, ara::crypto::cryp::ReservedContextIndex reserved_index) noexcept override;

  /*!
   * \brief Create a symmetric block cipher context.
   * \param[in] alg_id  identifier of the target crypto algorithm
   * \param[in] reserved_index  an optional index of reserved Context slot that should be used for this allocation or
   *            default marker, which says to allocate on the heap
   * \return unique smart pointer to the created context
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error ara::crypto::SecurityErrc::kUnsupported Method not supported.
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ara::core::Result<ara::crypto::cryp::SymmetricBlockCipherCtx::Uptr> CreateSymmetricBlockCipherCtx(
      AlgId alg_id, ara::crypto::cryp::ReservedContextIndex reserved_index) noexcept override;

  /*!
   * \brief Create a symmetric key-wrap algorithm context.
   * \param[in] alg_id  identifier of the target crypto algorithm
   * \param[in] reserved_index  an optional index of reserved Context slot that should be used for this allocation or
   *            default marker, which says to allocate on the heap
   * \return unique smart pointer to the created context
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error ara::crypto::SecurityErrc::kUnsupported Method not supported.
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ara::core::Result<ara::crypto::cryp::SymmetricKeyWrapperCtx::Uptr> CreateSymmetricKeyWrapperCtx(
      AlgId alg_id, ara::crypto::cryp::ReservedContextIndex reserved_index) noexcept override;

  /*!
   * \brief Create a symmetric stream cipher context.
   * \param[in] alg_id  identifier of the target crypto algorithm
   * \param[in] reserved_index  an optional index of reserved Context slot that should be used for this allocation or
   *            default marker, which says to allocate on the heap
   * \return unique smart pointer to the created context
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error ara::crypto::SecurityErrc::kUnsupported Method not supported.
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ara::core::Result<ara::crypto::cryp::StreamCipherCtx::Uptr> CreateStreamCipherCtx(
      AlgId alg_id, ara::crypto::cryp::ReservedContextIndex reserved_index) noexcept override;

  /*!
   * \brief Create a symmetric authenticated stream cipher context.
   * \param[in] alg_id  identifier of the target crypto algorithm
   * \param[in] reserved_index  an optional index of reserved Context slot that should be used for this allocation or
   *            default marker, which says to allocate on the heap
   * \return unique smart pointer to the created context
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error ara::crypto::SecurityErrc::kUnreservedResource Authenticated stream cipher contexts can only be generated on
   * the heap.
   * \error ara::crypto::SecurityErrc::kRuntimeFault Initialization or algorithm ID derivation failed.
   * \error ara::crypto::SecurityErrc::kUnknownIdentifier Unable to create authenticated stream cipher context for
   * the given algorithm ID.
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ara::core::Result<ara::crypto::cryp::AuthnStreamCipherCtx::Uptr> CreateAuthnStreamCipherCtx(
      AlgId alg_id, ara::crypto::cryp::ReservedContextIndex reserved_index) noexcept override;

  /*!
   * \brief Create a symmetric message authentication code context.
   * \param[in] alg_id  identifier of the target crypto algorithm
   * \param[in] reserved_index  an optional index of reserved Context slot that should be used for this allocation or
   *            default marker, which says to allocate on the heap
   * \return unique smart pointer to the created context
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error ara::crypto::SecurityErrc::kUnreservedResource Message authentication code contexts can only be generated on
   * the heap. \error ara::crypto::SecurityErrc::kRuntimeFault Initialization failed. \error
   * ara::crypto::SecurityErrc::kUnknownIdentifier Unable to create message authentication code context for the given
   * algorithm ID.
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ara::core::Result<ara::crypto::cryp::MessageAuthnCodeCtx::Uptr> CreateMessageAuthnCodeCtx(
      AlgId alg_id, ara::crypto::cryp::ReservedContextIndex reserved_index) noexcept override;

  /*!
   * \brief Create a hash function context.
   * \param[in] alg_id  identifier of the target crypto algorithm
   * \param[in] reserved_index  an optional index of reserved Context slot that should be used for this allocation or
   *            default marker, which says to allocate on the heap
   * \return unique smart pointer to the created context
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error ara::crypto::SecurityErrc::kUnreservedResource Hash function contexts can only be generated on the heap.
   * \error ara::crypto::SecurityErrc::kRuntimeFault Initialization failed.
   * \error ara::crypto::SecurityErrc::kUnknownIdentifier Unable to create hash function context for the given algorithm
   * ID.
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ara::core::Result<ara::crypto::cryp::HashFunctionCtx::Uptr> CreateHashFunctionCtx(
      AlgId alg_id, ara::crypto::cryp::ReservedContextIndex reserved_index) noexcept override;

  /*!
   * \brief Create a key derivation function context.
   * \param[in] alg_id  identifier of the target crypto algorithm
   * \param[in] reserved_index  an optional index of reserved Context slot that should be used for this allocation or
   *            default marker, which says to allocate on the heap
   * \return unique smart pointer to the created context
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error ara::crypto::SecurityErrc::kUnreservedResource Key derivation function contexts can only be generated on the
   * heap. \error ara::crypto::SecurityErrc::kRuntimeFault Initialization failed. \error
   * ara::crypto::SecurityErrc::kUnknownIdentifier Unable to key derivation function context for the given algorithm ID.
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ara::core::Result<ara::crypto::cryp::KeyDerivationFunctionCtx::Uptr> CreateKeyDerivationFunctionCtx(
      AlgId alg_id, ara::crypto::cryp::ReservedContextIndex reserved_index) noexcept override;

  /*!
   * \brief Create a symmetric key diversification context.
   * \param[in] master_alg_id  the crypto algorithm identifier of master-keys
   * \param[in] slave_key_length  the length of target slave-keys (derived from the master) in bits
   * \param[in] reserved_index  an optional index of reserved Context slot that should be used for this allocation or
   *            default marker, which says to allocate on the heap
   * \return unique smart pointer to the created context
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error ara::crypto::SecurityErrc::kUnsupported Method not supported.
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ara::core::Result<ara::crypto::cryp::KeyDiversifierCtx::Uptr> CreateKeyDiversifierCtx(
      AlgId master_alg_id, std::size_t slave_key_length,
      ara::crypto::cryp::ReservedContextIndex reserved_index) noexcept override;

  /*!
   * \brief Create an encryption public key context.
   * \param[in] alg_id  identifier of the target asymmetric encryption/decryption algorithm
   * \param[in] reserved_index  an optional index of reserved Context slot that should be used for this allocation or
   *            default marker, which says to allocate on the heap
   * \return unique smart pointer to the created context
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error ara::crypto::SecurityErrc::kUnsupported Method not supported.
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ara::core::Result<ara::crypto::cryp::EncryptorPublicCtx::Uptr> CreateEncryptorPublicCtx(
      AlgId alg_id, ara::crypto::cryp::ReservedContextIndex reserved_index) noexcept override;

  /*!
   * \brief Create a decryption private key context.
   * \param[in] alg_id  identifier of the target asymmetric encryption/decryption algorithm
   * \param[in] reserved_index  an optional index of reserved Context slot that should be used for this allocation or
   *            default marker, which says to allocate on the heap
   * \return unique smart pointer to the created context
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error ara::crypto::SecurityErrc::kUnsupported Method not supported.
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ara::core::Result<ara::crypto::cryp::DecryptorPrivateCtx::Uptr> CreateDecryptorPrivateCtx(
      AlgId alg_id, ara::crypto::cryp::ReservedContextIndex reserved_index) noexcept override;

  /*!
   * \brief Create a key-encapsulator public key context of a Key Encapsulation Mechanism (KEM).
   * \param[in] alg_id  identifier of the target KEM crypto algorithm
   * \param[in] reserved_index  an optional index of reserved Context slot that should be used for this allocation or
   *            default marker, which says to allocate on the heap
   * \return unique smart pointer to the created context
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error ara::crypto::SecurityErrc::kUnsupported Method not supported.
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ara::core::Result<ara::crypto::cryp::KeyEncapsulatorPublicCtx::Uptr> CreateKeyEncapsulatorPublicCtx(
      AlgId alg_id, ara::crypto::cryp::ReservedContextIndex reserved_index) noexcept override;

  /*!
   * \brief Create a key-decapsulator private key context of a Key Encapsulation Mechanism (KEM).
   * \param[in] alg_id  identifier of the target KEM crypto algorithm
   * \param[in] reserved_index  an optional index of reserved Context slot that should be used for this allocation or
   *            default marker, which says to allocate on the heap
   * \return unique smart pointer to the created context
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error ara::crypto::SecurityErrc::kUnsupported Method not supported.
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ara::core::Result<ara::crypto::cryp::KeyDecapsulatorPrivateCtx::Uptr> CreateKeyDecapsulatorPrivateCtx(
      AlgId alg_id, ara::crypto::cryp::ReservedContextIndex reserved_index) noexcept override;

  /*!
   * \brief Create a signature encoding private key context.
   * \param[in] alg_id  identifier of the target asymmetric crypto algorithm
   * \param[in] reserved_index  an optional index of reserved Context slot that should be used for this allocation or
   *            default marker, which says to allocate on the heap
   * \return unique smart pointer to the created context
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error ara::crypto::SecurityErrc::kUnsupported Method not supported.
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ara::core::Result<ara::crypto::cryp::SigEncodePrivateCtx::Uptr> CreateSigEncodePrivateCtx(
      AlgId alg_id, ara::crypto::cryp::ReservedContextIndex reserved_index) noexcept override;

  /*!
   * \brief Create a message recovery public key context.
   * \param[in] alg_id  identifier of the target asymmetric crypto algorithm
   * \param[in] reserved_index  an optional index of reserved Context slot that should be used for this allocation or
   *            default marker, which says to allocate on the heap
   * \return unique smart pointer to the created context
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error ara::crypto::SecurityErrc::kUnsupported Method not supported.
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ara::core::Result<ara::crypto::cryp::MsgRecoveryPublicCtx::Uptr> CreateMsgRecoveryPublicCtx(
      AlgId alg_id, ara::crypto::cryp::ReservedContextIndex reserved_index) noexcept override;

  /*!
   * \brief Create a signature private key context.
   * \param[in] alg_id  identifier of the target signature crypto algorithm
   * \param[in] reserved_index  an optional index of reserved Context slot that should be used for this allocation or
   *            default marker, which says to allocate on the heap
   * \return unique smart pointer to the created context
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error ara::crypto::SecurityErrc::kUnreservedResource Signer contexts can only be generated on the heap.
   * \error ara::crypto::SecurityErrc::kRuntimeFault Failed to create hash context.
   * \error ara::crypto::SecurityErrc::kUnknownIdentifier Unable to create signer context for the given algorithm ID.
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ara::core::Result<ara::crypto::cryp::SignerPrivateCtx::Uptr> CreateSignerPrivateCtx(
      AlgId alg_id, ara::crypto::cryp::ReservedContextIndex reserved_index) noexcept override;

  /*!
   * \brief Create a signature verification public key context.
   * \param[in] alg_id  identifier of the target signature crypto algorithm
   * \param[in] reserved_index  an optional index of reserved Context slot that should be used for this allocation or
   *            default marker, which says to allocate on the heap
   * \return unique smart pointer to the created context
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error ara::crypto::SecurityErrc::kUnreservedResource Verifier contexts can only be generated on the heap.
   * \error ara::crypto::SecurityErrc::kRuntimeFault Failed to create hash context.
   * \error ara::crypto::SecurityErrc::kUnknownIdentifier Unable to create verifier context for the given algorithm ID.
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ara::core::Result<ara::crypto::cryp::VerifierPublicCtx::Uptr> CreateVerifierPublicCtx(
      AlgId alg_id, ara::crypto::cryp::ReservedContextIndex reserved_index) noexcept override;

  /*!
   * \brief Create a key-agreement private key context.
   * \param[in] alg_id  identifier of the target key-agreement crypto algorithm
   * \param[in] reserved_index  an optional index of reserved Context slot that should be used for this allocation or
   *            default marker, which says to allocate on the heap
   * \return unique smart pointer to the created context
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error ara::crypto::SecurityErrc::kUnsupported Method not supported.
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ara::core::Result<ara::crypto::cryp::KeyAgreementPrivateCtx::Uptr> CreateKeyAgreementPrivateCtx(
      AlgId alg_id, ara::crypto::cryp::ReservedContextIndex reserved_index) noexcept override;

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
   * \error ara::crypto::SecurityErrc::kInvalidArgument Algorithm ID invalid.
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ara::core::Result<ara::crypto::cryp::X509RequestSignerCtx::Uptr> CreateX509RequestSignerCtx(
      AlgId alg_id, ara::crypto::cryp::ReservedContextIndex reserved_index) noexcept override;

  /*!
   * \brief Get the provider version.
   * \return The version of the provider.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  std::uint64_t GetProviderVersion() const noexcept override;

  /*!
   * \brief Returns a human readable name of the Provider.
   * \details Life-time of the returned string is not less than the Provider instance life-time.
   * \return A pointer to null-terminated string with Provider Name.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  char const* GetProviderName() const noexcept override;

  /*!
   * \brief Get type of the Provider.
   * \return Type of the Provider.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ara::crypto::ProviderType GetProviderType() const noexcept override;

  /*!
   * \brief Returns the Provider's Globally Unique Identifier (GUID).
   * \param[out] provider_uid An output buffer for the Provider's GUID.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  void GetProviderUid(ara::crypto::Guid& provider_uid) const noexcept override;

  /*!
   * \brief Get Crypto Primitive Category of specified algorithm.
   * \param[in] alg_id  the vendor specific binary algorithm ID.
   * \return the crypto primitive category of the crypto algorithm.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error ara::crypto::SecurityErrc::kUnknownIdentifier Unable to categorize the given algorithm ID.
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ara::core::Result<ara::crypto::cryp::CryptoPrimitiveId::Category> GetPrimitiveCategory(
      AlgId alg_id) const noexcept override;

 private:
  common::util::Logger logger_;                              /*!< Logging instance */
  amsr::sec::ipc::MessageBuilder message_builder_;           /*!< Message builder for communication */
  amsr::sec::ipc::MessageSerializer message_serializer_;     /*!< Message serializer for communication */
  amsr::sec::ipc::MessageDeserializer message_deserializer_; /*!< Message deserializer for communication */
};
}  // namespace cryp
}  // namespace proxy
}  // namespace client
}  // namespace crypto
#endif  // LIB_LIBCLIENT_INCLUDE_CRYPTO_CLIENT_PROXY_CRYP_CRYPTO_PROVIDER_H_
