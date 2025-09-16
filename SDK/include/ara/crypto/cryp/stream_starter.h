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
/*!        \file  ara/crypto/cryp/stream_starter.h
 *        \brief  Basic StreamStarter interface definition.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBARA_INCLUDE_ARA_CRYPTO_CRYP_STREAM_STARTER_H_
#define LIB_LIBARA_INCLUDE_ARA_CRYPTO_CRYP_STREAM_STARTER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>

#include "ara/crypto/common/mem_region.h"
#include "ara/crypto/common/std_api.h"
#include "ara/crypto/cryp/secret_seed.h"

namespace ara {
namespace crypto {
namespace cryp {

/*!
 * \brief Starter interface of a stream processing, common for all primitives that supports the streamable processing
 *        approach.
 * \vpublic
 */
class StreamStarter {
 protected:
  // Attention! Deviation: Protected destructor made non-virtual to satisfy AutosarC++17_10-A12.4.1
  /*!
   * \brief Destructor.
   * \vpublic
   */
  ~StreamStarter() noexcept = default;

  // Attention! Deviation: Not mentioned in the AUTOSAR SWS, should be declared to satisfy AutosarC++17_10-A12.0.1
  /*!
   * \brief Constructor.
   * \vpublic
   */
  StreamStarter() noexcept = default;

  // Attention! Deviation: Not mentioned in the AUTOSAR SWS, should be declared to satisfy AutosarC++17_10-A12.0.1
  /*!
   * \brief Copy constructor.
   * \vpublic
   */
  StreamStarter(StreamStarter const& /*other*/) noexcept = default;

  // Attention! Deviation: Not mentioned in the AUTOSAR SWS, should be declared to satisfy AutosarC++17_10-A12.0.1
  /*!
   * \brief Copy assignment operator.
   * \return A reference to the object
   * \vpublic
   */
  StreamStarter& operator=(StreamStarter const& /*other*/) & noexcept = default;

  // Attention! Deviation: Not mentioned in the AUTOSAR SWS, should be declared to satisfy AutosarC++17_10-A12.0.1
  /*!
   * \brief Move constructor.
   * \vpublic
   */
  StreamStarter(StreamStarter&& /*other*/) noexcept = default;

  // Attention! Deviation: Not mentioned in the AUTOSAR SWS, should be declared to satisfy AutosarC++17_10-A12.0.1
  /*!
   * \brief Move assignment.
   * \return A reference to the object
   * \vpublic
   */
  StreamStarter& operator=(StreamStarter&& /*other*/) & noexcept = default;

 public:
  /*!
   * \brief Get default expected size of the Initialization Vector (IV) or nonce.
   * \return default expected size of IV in bytes
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual std::size_t GetIvSize() const noexcept = 0;

  /*!
   * \brief Get block (or internal buffer) size of the base algorithm.
   * \return size of the block in bytes
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual std::size_t GetBlockSize() const noexcept = 0;

  // VECTOR NC AutosarC++17_10-A0.1.5: MD_CRYPTO_AutosarC++17_10-A0.1.5_pure_virtual
  /*!
   * \brief Verify validity of specific Initialization Vector (IV) length.
   * \param[in] iv_size the length of the IV in bytes
   * \return \c true if provided IV length is supported by the algorithm and \c false otherwise
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual bool IsValidIvSize(std::size_t iv_size) const noexcept = 0;

  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
  /*!
   * \brief Initialize the context for a new data stream processing or generation (depending from the primitive).
   * \param[in] iv an optional Initialization Vector (IV) or "nonce" value
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error SecurityErrorDomain::kUninitializedContext if the context was not initialized by required domain parameters
   *        or key object
   * \error SecurityErrorDomain::kInvalidInputSize if the size of provided IV is not supported (i.e. if it is not
   *        enough for the initialization)
   * \error SecurityErrorDomain::kUnsupported if the base algorithm (or its current implementation) principally doesn't
   *        support the IV variation, but provided IV value is not empty, i.e. if (iv.empty() == false)
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual ara::core::Result<void> Start(ReadOnlyMemRegion iv = ReadOnlyMemRegion()) noexcept = 0;

  /*!
   * \brief Initialize the context for a new data stream processing or generation (depending from the primitive).
   * \param[in] iv the Initialization Vector (IV) or "nonce" object
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error SecurityErrorDomain::kUninitializedContext if the context was not initialized by required domain parameters
   *        or key object
   * \error SecurityErrorDomain::kInvalidInputSize if the size of provided IV is not supported (i.e. if it is not enough
   *        for the initialization)
   * \error SecurityErrorDomain::kUnsupported if the base algorithm (or its current implementation) principally doesn't
   *        support the IV variation
   * \error SecurityErrorDomain::kUsageViolation if this transformation type is prohibited by the "allowed usage"
   *        restrictions of the provided \c SecretSeed object
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual ara::core::Result<void> Start(SecretSeed const& iv) noexcept = 0;

  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
  // VECTOR NC AutosarC++17_10-M7.1.2: MD_CRYPTO_AutosarC++17_10-M7.1.2_autosar_api
  // VECTOR NC AutosarC++17_10-A0.1.5: MD_CRYPTO_AutosarC++17_10-A0.1.5_pure_virtual
  /*!
   * \brief Get actual bit-length of an IV loaded to the context.
   * \param[out] iv_uid optional pointer to a buffer for saving an COUID of a IV object now loaded to the context
   * \return actual length of the IV (now set to the algorithm context) in bits
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual std::size_t GetActualIvBitLength(CryptoObjectUid* iv_uid = nullptr) const noexcept = 0;
};

}  // namespace cryp
}  // namespace crypto
}  // namespace ara

#endif  // LIB_LIBARA_INCLUDE_ARA_CRYPTO_CRYP_STREAM_STARTER_H_
