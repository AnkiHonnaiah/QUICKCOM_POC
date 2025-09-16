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
/*!        \file   libcommon/include/crypto/common/uid_util.h
 *          \brief  Generate an UUID according to RFC4122
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBCOMMON_INCLUDE_CRYPTO_COMMON_GENERATE_UUID_H_
#define LIB_LIBCOMMON_INCLUDE_CRYPTO_COMMON_GENERATE_UUID_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/crypto/cryp/random_generator_ctx.h"
#include "crypto/common/data/data_container.h"
#include "crypto/common/keys/key_storage_provider.h"

namespace crypto {
namespace common {

/*!
 * \brief Generate an UUID
 * \vprivate Product Private
 */
class UidUtil final {
 public:
  /*!
   * \brief Default constructor
   * \vprivate Product Private
   */
  UidUtil() = default;

  /*!
   * \brief deleted copy constructor
   * \vprivate Product Private
   */
  UidUtil(UidUtil&) noexcept = delete;

  /*!
   * \brief deleted move constructor
   * \vprivate Product Private
   */
  UidUtil(UidUtil&&) noexcept = delete;

  /*!
   * \brief deleted copy assign operator
   * \vprivate Product Private
   */
  auto operator=(UidUtil const&) noexcept -> UidUtil& = delete;
  /*!
   * \brief deleted move assign operator
   * \vprivate Product Private
   */
  auto operator=(UidUtil&&) noexcept -> UidUtil& = delete;

  /*!
   * \brief Destructor
   * \vprivate Product Private
   */
  ~UidUtil() noexcept = default;

  /*!
   * \brief Generate an UUID according to RFC4122
   * \param[in] rng_ctx Random number generator context
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error ara::crypto::SecurityErrc::kRuntimeFault if the max UUID value is reached.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  static auto GenerateUuid(ara::crypto::cryp::RandomGeneratorCtx::Sptr rng_ctx) noexcept
      -> ara::core::Result<ara::crypto::Uuid>;

  /*!
   * \brief Generates a version stamp for the mVersionStamp field of a COUID
   * \details Creates a Sequential value of a steady timer or simple counter, representing version of a Crypto Object.
   * \param[in,out] last_version_stamp reference to the last used version stamp.
   * \return the new version stamp
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous FALSE
   * \vprivate Product Private
   */
  static auto GetVersionStamp(std::uint64_t& last_version_stamp) noexcept -> std::uint64_t;
};

}  // namespace common
}  // namespace crypto

#endif  // LIB_LIBCOMMON_INCLUDE_CRYPTO_COMMON_GENERATE_UUID_H_