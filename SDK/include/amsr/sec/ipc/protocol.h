// VECTOR SL AutosarC++17_10-A2.9.1: MD_CRYPTO_AutosarC++17_10-A2.9.1
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
/*!        \file  protocol.h
 *        \brief  Abstraction of the cryptostack IPC protocol.
 *
 *      \details  Enums and constants which are needed for the IPC of the cryptostack. They are used to construct
 *                messages (see MessageBuilder and BasicMessage) which contain an operation for the receiving process.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBCOMMON_INCLUDE_AMSR_SEC_IPC_PROTOCOL_H_
#define LIB_LIBCOMMON_INCLUDE_AMSR_SEC_IPC_PROTOCOL_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <chrono>
#include <cstdint>

namespace amsr {
namespace sec {
namespace ipc {

static constexpr std::uint32_t const kIpcPort{42};   /*!< Default IPC port */
static constexpr std::uint32_t const kIpcDomain{69}; /*!< Default IPC domain */

static constexpr std::chrono::nanoseconds const kReactorTimeout{
    std::chrono::nanoseconds::max()}; /*!< Timeout value for Reactor::HandleEvents*/

/*!
 * \brief ProxyId identifies an unique connection between a proxy object on client side and a skeleton object on server
 *        side. Therefor the memory address of the proxy is used and so as type the uintptr_t is defined as ProxyId
 * \vprivate Component Private
 */
using ProxyId = std::uint64_t;

static_assert(sizeof(ProxyId) >= sizeof(std::uintptr_t), "Size of pointers is larger than 64bit.");
static_assert(sizeof(ProxyId) >= sizeof(void*), "Size of amsr::sec::ipc::ProxyId is smaller than size of void*");

/*!
 * \brief Base type of all task enums
 * \vprivate Component Private
 */
using TaskId_t = std::uint32_t;

/*!
 * \brief Enum of generic tasks regarding IPC.
 * \vprivate Component Private
 */
enum class Task : TaskId_t {
  Unknown,   /*!< Unknown message type. */
  Error,     /*!< For reporting an exception or error to the other party. */
  Return,    /*!< For returning function call results. */
  Destroy,   /*!< For destroying an object i.e. calling its destructor. */
  Handshake, /*!< For IPC handshake operations. */
  Invocation /*!< For invoking remote method calls. */
};

// VECTOR NC AutosarC++17_10-M2.10.6: MD_CRYPTO_AutosarC++17_10-M2.10.6_enum_definition
/*!
 * \brief Detail task containing all steps to handle the handshake between client and server
 * \vprivate Component Private
 */
enum class Handshake : TaskId_t {
  RegisterCryptoProviderProxy,
  RegisterKeyStorageProviderProxy,
  RegisterX509ProviderProxy
};

/*!
 * \brief Enum for all methods provided by the CryptoProvider class ara::crypto::cryp::CryptoProvider
 * \vprivate Component Private
 */
enum class ara_crypto_cryp_CryptoProvider : TaskId_t {
  GenerateSymmetricKey,
  GeneratePrivateKey,
  CreateHashFunctionCtx,
  CreateAuthnStreamCipherCtx,
  CreateMessageAuthnCodeCtx,
  CreateKeyDerivationFunctionCtx,
  CreateSignerPrivateCtx,
  CreateVerifierPublicCtx,
  ConvertToAlgId,
  ConvertToAlgName,
  GenerateSeed,
  ExportPublicObject,
  ExportSecuredObject_Object,
  ExportSecuredObject_Container,
  ImportPublicObject,
  ImportRawObject,
  ImportRawObject_DomainParam,
  ImportSecuredObject,
  LoadObject,
  GetProviderVersion,
  GetProviderName,
  GetProviderUid,
  GetProviderType,
  DefaultRng,
  SetDefaultRng,
  CreateRandomGeneratorCtx,
  AllocDomainParameters,
  KnownDomainParameters,
  AllocVolatileContainer_Capacity,
  AllocVolatileContainer_List,
  CreateX509RequestSignerCtx,
  ReserveContexts_Map,
  ReserveContexts_Quantity,
  ReserveObjects_Map,
  ReserveObjects_Quantity,
  EnterRealTimeMode,
  LeaveRealTimeMode,
  GetSerializedSize,
  GetStorageSize,
  AllocPasswordCache,
  HashPassword,
  CreateSymmetricBlockCipherCtx,
  CreateSymmetricKeyWrapperCtx,
  CreateStreamCipherCtx,
  CreateKeyDiversifierCtx,
  CreateEncryptorPublicCtx,
  CreateDecryptorPrivateCtx,
  CreateKeyEncapsulatorPublicCtx,
  CreateKeyDecapsulatorPrivateCtx,
  CreateSigEncodePrivateCtx,
  CreateMsgRecoveryPublicCtx,
  CreateKeyAgreementPrivateCtx,
  GetPrimitiveCategory
};

/*!
 * \brief Enum for all methods provided by the class ara::crypto::keys::KeyStorageProvider
 * \vprivate Component Private
 */
enum class ara_crypto_keys_KeyStorageProvider : TaskId_t {
  FindSlot,
  FindObject,
  IsEmpty,
  OpenAsUser,
  OpenAsOwner,
  SaveCopy,
  Clear,
  GetPrototypedProps,
  GetContentProps,
  GetDefaultCryptoProviderUid,
  GetOwner,
  GetUsers,
  GetProviderVersion,
  GetProviderName,
  GetProviderUid,
  GetProviderType
};

/*!
 * \brief Enum for all methods provided by the CryptoProvider class ara::crypto::cryp::MessageAuthnCodeCtx
 * \vprivate Component Private
 */
enum class ara_crypto_cryp_MessageAuthnCodeCtx : TaskId_t {
  GetDigestSize,
  Update_keymaterial,
  Update_memreg,
  Update_byte,
  StartUpdateFinish,
  Finish,
  Finish_NoSignature,
  GetDigest,
  Compare,
  Check,
  IsKeyedContext,
  IsInitialized,
  Reset,
  GetParametersUid,
  GetPrimitiveName,
  GetPrimitiveId,
  GetCategory,
  IsKeyBitLengthSupported,
  GetMinKeyBitLength,
  GetMaxKeyBitLength,
  GetActualKeyBitLength,
  SetKey,
  IsDirectTransform,
  GetIvSize,
  GetBlockSize,
  IsValidIvSize,
  Start_MemRegion,
  Start_SecretSeed,
  GetActualIvBitLength
};

/*!
 * \brief Enum for all methods provided by the CryptoProvider class ara::crypto::cryp::AuthnStreamCipherCtx
 * \vprivate Component Private
 */
enum class ara_crypto_cryp_AuthnStreamCipherCtx : TaskId_t {
  GetMaxAssociatedDataSize,
  GetDigestSize,
  Start_Iv,
  Start_Seed,
  Update_keymaterial,
  Update_memreg,
  Update_byte,
  Finish,
  Finish_NoSignature,
  GetDigest,
  Compare,
  Check,
  IsKeyedContext,
  IsInitialized,
  Reset,
  GetParametersUid,
  GetPrimitiveName,
  GetPrimitiveId,
  GetCategory,
  IsKeyBitLengthSupported,
  GetMinKeyBitLength,
  GetMaxKeyBitLength,
  GetActualKeyBitLength,
  GetIvSize,
  GetBlockSize,
  IsBytewiseMode,
  IsSeekableMode,
  IsValidIvSize,
  Seek,
  ProcessBlocks_in_out,
  ProcessBlocks_inplace,
  ProcessBytes,
  FinishBytes,
  SetKey,
  IsDirectTransform,
  CountBytesInCache,
  GetActualIvBitLength
};

/*!
 * \brief Enum for all methods provided by the CryptoProvider class ara::crypto::cryp::StreamCipherCtx
 * \vprivate Component Private
 */
enum class ara_crypto_cryp_StreamCipherCtx : TaskId_t {
  Start_Iv,
  Start_Seed,
  IsKeyedContext,
  IsInitialized,
  GetParametersUid,
  Reset,
  GetPrimitiveName,
  GetPrimitiveId,
  GetCategory,
  MyProvider,
  IsKeyBitLengthSupported,
  GetMinKeyBitLength,
  GetMaxKeyBitLength,
  GetActualKeyBitLength,
  GetIvSize,
  GetBlockSize,
  IsBytewiseMode,
  IsSeekableMode,
  IsValidIvSize,
  Seek,
  ProcessBlocks_in_out,
  ProcessBlocks_inplace,
  ProcessBytes,
  FinishBytes,
  SetKey,
  IsDirectTransform,
  CountBytesInCache,
  GetActualIvBitLength
};

/*!
 * \brief Enum for all methods provided by the CryptoProvider class ara::crypto::cryp::HashFunctionCtx
 * \vprivate Component Private
 */
enum class ara_crypto_cryp_HashFunctionCtx : TaskId_t {
  GetDigestSize,
  IsKeyedContext,
  IsInitialized,
  Reset,
  GetPrimitiveName,
  GetPrimitiveId,
  GetCategory,
  Update_KeyMaterialRef,
  Update_ReadOnlyMemRegion,
  Update_Byte,
  Finish,
  Finish_NoSignature,
  GetDigest,
  Compare,
  Check,
  GetParametersUid,
  GetIvSize,
  GetBlockSize,
  IsValidIvSize,
  Start_MemRegion,
  Start_SecretSeed,
  GetActualIvBitLength
};

/*!
 * \brief Enum for all methods provided by the CryptoProvider class ara::crypto::cryp::KeyDerivationFunctionCtx
 * \vprivate Component Private
 */
enum class ara_crypto_cryp_KeyDerivationFunctionCtx : TaskId_t {
  IsKeyedContext,
  IsInitialized,
  Reset,
  GetParametersUid,
  GetPrimitiveName,
  GetPrimitiveId,
  GetCategory,
  ConfigIterations,
  DeriveKey_memregion,
  DeriveKey_secretseed,
  DeriveSeed_memregion,
  DeriveSeed_secretseed
};

/*!
 * \brief Enum for all methods provided by the CryptoProvider class ara::crypto::cryp::RandomGeneratorCtx
 * \vprivate Component Private
 */
enum class ara_crypto_cryp_RandomGeneratorCtx : TaskId_t {
  GetActualKeyBitLength,
  Reset,
  GetParametersUid,
  Generate,
  GetCategory,
  GetPrimitiveId,
  GetPrimitiveName,
  IsDirectTransform,
  IsInitialized,
  IsKeyBitLengthSupported,
  IsKeyedContext,
  GetMaxKeyBitLength,
  GetMinKeyBitLength,
  AddEntropy,
  SetKey,
  GetIvSize,
  GetBlockSize,
  IsValidIvSize,
  Start_MemRegion,
  Start_SecretSeed,
  GetActualIvBitLength
};

/*!
 * \brief Enum for all methods provided by the CryptoProvider class ara::crypto::cryp::SecretSeed
 * \vprivate Component Private
 */
enum class ara_crypto_cryp_SecretSeed : TaskId_t {
  GetObjectType,
  IsSession,
  IsExportable,
  GetObjectId,
  HasDependence,
  StorageSize,
  Save,
  GetPrimitiveName,
  GetPrimitiveId,
  GetCategory,
  GetActualKeyBitLength,
  GetAllowedUsage,
  Clone,
  JumpFrom,
  Next,
  Jump,
  OperatorXorAssign_SecretSeed,
  OperatorXorAssign_MemRegion
};

/*!
 * \brief Enum for all methods provided by the CryptoProvider class ara::crypto::cryp::Signature
 * \vprivate Component Private
 */
enum class ara_crypto_cryp_Signature : TaskId_t {
  GetObjectType,
  IsSession,
  IsExportable,
  GetObjectId,
  HasDependence,
  StorageSize,
  Save,
  GetPrimitiveName,
  GetPrimitiveId,
  GetCategory,
  ExportPublicly,
  GetHashAlgId
};

/*!
 * \brief Enum for all methods provided by the CryptoProvider class ara::crypto::cryp::SymmetricKey
 * \vprivate Component Private
 */
enum class ara_crypto_cryp_SymmetricKey : TaskId_t {
  GetAllowedUsage,
  IsCompatible_algid,
  IsCompatible_keyedctx,
  GetKeyType,
  IsPublic,
  IsExportable,
  GetActualKeyBitLength,
  GetObjectType,
  IsSession,
  GetObjectId,
  HasDependence,
  StorageSize,
  Save,
  GetPrimitiveName,
  GetPrimitiveId,
  GetCategory
};

/*!
 * \brief Enum for all methods provided by the CryptoProvider class ara::crypto::cryp::PublicKey
 * \vprivate Component Private
 */
enum class ara_crypto_cryp_PublicKey : TaskId_t {
  GetActualKeyBitLength,
  GetAllowedUsage,
  CheckKey,
  ExportPublicly,
  GetCategory,
  GetKeyType,
  GetObjectId,
  GetObjectType,
  GetPrimitiveId,
  GetPrimitiveName,
  HasDependence,
  HashPublicKey,
  IsCompatible_Algid,
  IsCompatible_KeyedContext,
  IsExportable,
  IsPublic,
  IsSession,
  Save,
  StorageSize
};

/*!
 * \brief Enum for all methods provided by the CryptoProvider class ara::crypto::cryp::PrivateKey
 * \vprivate Component Private
 */
enum class ara_crypto_cryp_PrivateKey : TaskId_t {
  GetActualKeyBitLength,
  GetAllowedUsage,
  CheckKey,
  GetCategory,
  GetKeyType,
  GetObjectId,
  GetObjectType,
  GetPrimitiveId,
  GetPrimitiveName,
  HasDependence,
  IsCompatible_AlgId,
  IsCompatible_KeyedContext,
  IsExportable,
  IsPublic,
  IsSession,
  Save,
  StorageSize,
  GetPublicKey
};

/*!
 * \brief Enum for all methods provided by the CryptoProvider class ara::crypto::cryp::SignerPrivateCtx
 * \vprivate Component Private
 */
enum class ara_crypto_cryp_SignerPrivateCtx : TaskId_t {
  IsKeyedContext,
  IsInitialized,
  Reset,
  GetParametersUid,
  GetPrimitiveName,
  GetPrimitiveId,
  GetCategory,
  IsKeyBitLengthSupported,
  GetMinKeyBitLength,
  GetMaxKeyBitLength,
  GetActualKeyBitLength,
  SetKey,
  GetRequiredHashSize,
  GetRequiredHashAlgId,
  GetSignatureSize,
  Sign_hash,
  Sign_memregion
};

/*!
 * \brief Enum for all methods provided by the CryptoProvider class ara::crypto::cryp::VerifierPublicCtx
 * \vprivate Component Private
 */
enum class ara_crypto_cryp_VerifierPublicCtx : TaskId_t {
  IsKeyedContext,
  IsInitialized,
  Reset,
  GetParametersUid,
  GetPrimitiveName,
  GetPrimitiveId,
  GetCategory,
  IsKeyBitLengthSupported,
  GetMinKeyBitLength,
  GetMaxKeyBitLength,
  GetActualKeyBitLength,
  SetKey,
  GetRequiredHashSize,
  GetRequiredHashAlgId,
  GetSignatureSize,
  Verify_Signature,
  Verify_MemRegion
};

/*!
 * \brief Enum for all methods provided by the CryptoProvider class ara::crypto::TrustedContainer
 * \vprivate Component Private
 */
enum class ara_crypto_TrustedContainer : TaskId_t {
  GetObjectId,
  GetDependenceId,
  Capacity,
  IsVolatile,
  IsObjectSession,
  IsObjectExportable,
  ObjectSize,
  TypeRestriction,
  AllowedUsage,
  GetReferencesCounter,
  HasOwnership,
  GetLogicalContainerUid
};

/*!
 * \brief Enum for all methods provided by the CryptoProvider class ara::crypto::cryp::DomainParameters
 * \vprivate Component Private
 */
enum class ara_crypto_cryp_DomainParameters : TaskId_t {
  GetObjectType,
  IsSession,
  IsExportable,
  GetObjectId,
  HasDependence,
  StorageSize,
  Save,
  IsSecret,
  GetParametersCount,
  GetMaxParameterNameLength,
  GetParameterName,
  ExpectedParameterSize,
  SetParameter,
  Complete,
  IsCompleted,
  GetUniqueName,
  GetPrimitiveName,
  GetPrimitiveId,
  GetCategory,
  ExportPublicly,
  GetAllowedUsage
};

/*!
 * \brief Enum for all methods provided by the CryptoProvider class ara::crypto::cryp::X509AlgorithmId
 * \vprivate Component Private
 */
enum class ara_crypto_cryp_X509AlgorithmId : TaskId_t {
  GetPrimitiveName,
  GetPrimitiveId,
  GetCategory,
  ExportPublicly,
  GetRequiredHashSize,
  GetRequiredHashAlgId,
  GetSignatureSize,
  HasDomainParameters,
  GetDomainParameters,
  IsSameParameters
};

/*!
 * \brief Enum for all methods provided by the CryptoProvider class ara::crypto::cryp::X509CertRequest
 * \vprivate Component Private
 */
enum class ara_crypto_cryp_X509CertRequest : TaskId_t {
  GetObjectType,
  IsSession,
  IsExportable,
  GetObjectId,
  HasDependence,
  StorageSize,
  Save,
  GetPrimitiveName,
  GetPrimitiveId,
  GetCategory,
  ExportPublicly,
  Verify,
  Version,
  Signature,
  SubjectPublicKeyInfo
};

/*!
 * \brief Enum for all methods provided by the CryptoProvider class ara::crypto::cryp::X509PublicKeyInfo
 * \vprivate Component Private
 */
enum class ara_crypto_cryp_X509PublicKeyInfo : TaskId_t {
  GetPrimitiveName,
  GetPrimitiveId,
  GetCategory,
  ExportPublicly,
  GetRequiredHashSize,
  GetRequiredHashAlgId,
  GetSignatureSize,
  HasDomainParameters,
  GetDomainParameters,
  IsSameParameters,
  GetPublicKey,
  IsSameKey
};

/*!
 * \brief Enum for all methods provided by the CryptoProvider class ara::crypto::cryp::X509RequestSignerCtx
 * \vprivate Component Private
 */
enum class ara_crypto_cryp_X509RequestSignerCtx : TaskId_t {
  IsKeyedContext,
  IsInitialized,
  Reset,
  GetParametersUid,
  GetPrimitiveName,
  GetPrimitiveId,
  GetCategory,
  IsKeyBitLengthSupported,
  GetMinKeyBitLength,
  GetMaxKeyBitLength,
  GetActualKeyBitLength,
  SetKey,
  GetRequiredHashSize,
  GetRequiredHashAlgId,
  GetSignatureSize,
  CreateCertRequest
};

/*!
 * \brief Enum for all methods provided by the CryptoProvider class ara::crypto::cryp::X509Signature
 * \vprivate Component Private
 */
enum class ara_crypto_cryp_X509Signature : TaskId_t {
  GetPrimitiveName,
  GetPrimitiveId,
  GetCategory,
  ExportPublicly,
  GetRequiredHashSize,
  GetRequiredHashAlgId,
  GetSignatureSize,
  HasDomainParameters,
  GetDomainParameters,
  IsSameParameters,
  GetSignature
};

/*!
 * \brief Enum for all methods provided by the class ara::crypto::cryp::KeyAgreementPrivateCtx
 * \vprivate Component Private
 */
enum class ara_crypto_cryp_KeyAgreementPrivateCtx : TaskId_t {
  AgreeSeed,
  AgreeKey,
  SetKey,
  IsKeyBitLengthSupported,
  GetMinKeyBitLength,
  GetMaxKeyBitLength,
  GetActualKeyBitLength,
  IsKeyedContext,
  IsInitialized,
  Reset,
  GetParametersUid,
  GetPrimitiveName,
  GetPrimitiveId,
  GetCategory,
  MyProvider,
  Release
};

/*!
 * \brief Tags for serialization of pointers used for argument pointers and return values
 * \vprivate Component Private
 */
enum class SerializationTags : std::uint8_t {
  tag_nullptr = 0,          /*!< Tag to indicate in a serialized message, that this element is a nullpointer */
  tag_normal_pointer = 1,   /*!< Tag to indicate in a serialized message, that this element is not a nullpointer */
  tag_skeleton_created = 2, /*!< Tag to indicate the successful creation of the skeleton object */
  tag_skeleton_not_created =
      3 /*!< Tag to indicate that the creation of the skeleton object failed (= driver returned NULL) */
};

// VECTOR NC AutosarC++17_10-M3.4.1: MD_CRYPTO_AutosarC++17_10-M3.4.1_global
/*!
 * \brief Enum of generic tasks regarding IPC.
 * \vprivate Component Private
 */
enum class ara_crypto_x509_X509Provider : TaskId_t {
  CreateEmptyDn,
  BuildDn,
  DecodeDn,
  ParseCert,
  ParseAttributeCert,
  CountCertsInChain,
  ParseCertChain,
  ParseCertChain_Vector,
  VerifyCertByCrl,
  VerifyCertChainByCrl,
  VerifyAttributeCert,
  ImportCrl,
  Import,
  LoadCertificate,
  Remove,
  RemoveCertificate,
  SaveCertSignRequest,
  SetPendingStatus,
  SetAsRootOfTrust,
  CreateOcspRequest,
  CreateOcspRequest_Vector,
  ParseOcspResponse,
  CheckCertStatus,
  CheckCertStatus_Vector,
  FindCertByDn,
  FindCertByKeyIds,
  FindCertBySn,
  FindCertSignRequest,
  CleanupVolatileStorage,
  GetProviderVersion,
  GetProviderName,
  GetProviderType,
  GetProviderUid,
  FindCertByDnWildcard,
  VerifyCertExt,
  VerifyCertChainExt,
  VerifyAttributeCertExt,
};

// VECTOR NC AutosarC++17_10-M3.4.1: MD_CRYPTO_AutosarC++17_10-M3.4.1_global
/*!
 * \brief Enum of generic tasks regarding IPC.
 * \vprivate Component Private
 */
enum class ara_crypto_x509_X509DN : TaskId_t {
  GetDnString,
  SetDn,
  GetAttribute,
  GetAttribute_Idx,
  SetAttribute,
  SetAttribute_Idx,
  ExportPublicly,
  GetDigest
};

// VECTOR NC AutosarC++17_10-M3.4.1: MD_CRYPTO_AutosarC++17_10-M3.4.1_global
/*!
 * \brief Enum of generic tasks regarding IPC.
 * \vprivate Component Private
 */
enum class ara_crypto_x509_Certificate : TaskId_t {
  X509Version,
  IsRoot,
  IssuerDn,
  StartTime,
  EndTime,
  SerialNumber,
  AuthorityKeyId,
  SubjectKeyId,
  VerifyMe,
  GetFingerprint,
  GetStatus,
  GetSignatureAlgorithm,
  SubjectPubKey,
  SubjectDn,
  IsCa,
  GetPathLimit,
  GetConstraints,
  ExportPublicly
};

// VECTOR NC AutosarC++17_10-M3.4.1: MD_CRYPTO_AutosarC++17_10-M3.4.1_global
/*!
 * \brief Enum of generic tasks regarding IPC.
 * \vprivate Component Private
 */
enum class ara_crypto_x509_AttributeCertificate : TaskId_t {
  GetStatus,
  GetAttributeCertSerialNumber,
  GetSignatureAlgorithm,
  StartTime,
  EndTime,
  AuthorityKeyId,
  ExportPublicly
};

// VECTOR NC AutosarC++17_10-M3.4.1: MD_CRYPTO_AutosarC++17_10-M3.4.1_global
/*!
 * \brief Enum of generic tasks regarding IPC.
 * \vprivate Component Private
 */
enum class ara_crypto_x509_CertSignRequest : TaskId_t {
  Verify,
  ChallengePassword,
  SubjectPubKey,
  SubjectDn,
  IsCa,
  GetPathLimit,
  GetConstraints,
  GetSignatureAlgorithm,
  ExportPublicly,
  GetDigest
};

// VECTOR NC AutosarC++17_10-M3.4.1: MD_CRYPTO_AutosarC++17_10-M3.4.1_global
/*!
 * \brief Enum of generic tasks regarding IPC.
 * \vprivate Component Private
 */
enum class ara_crypto_x509_OcspResponse : TaskId_t { ExportPublicly, Version };

/*!
 * \brief Enum of generic tasks regarding IPC.
 * \vprivate Component Private
 */
enum class ara_crypto_x509_OcspRequest : TaskId_t { ExportPublicly, Version };

}  // namespace ipc
}  // namespace sec
}  // namespace amsr

#endif  // LIB_LIBCOMMON_INCLUDE_AMSR_SEC_IPC_PROTOCOL_H_
