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
/*!        \file  ara/crypto/common/provider_info.h
 *        \brief  A common interface for obtaining an identification information of a Provider.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBARA_INCLUDE_ARA_CRYPTO_COMMON_PROVIDER_INFO_H_
#define LIB_LIBARA_INCLUDE_ARA_CRYPTO_COMMON_PROVIDER_INFO_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/crypto/common/base_id_types.h"
#include "ara/crypto/common/guid.h"
#include "ara/crypto/common/std_api.h"
#include "crypto/common/identifiable.h"

namespace ara {
namespace crypto {

// VECTOR NC AutosarC++17_10-M10.1.1: MD_CRYPTO_AutosarC++17_10-M10.1.1_diamond
// VECTOR NC AutosarC++17_10-M10.1.2: MD_CRYPTO_AutosarC++17_10-M10.1.2_diamond
// VECTOR NC AutosarC++17_10-A12.1.5: MD_CRYPTO_AutosarC++17_10-A12.1.5_diamond
/*!
 * \brief A common interface for obtaining an identification information of a Provider.
 * \vpublic
 */
class ProviderInfo : public virtual ::crypto::common::Identifiable {
 protected:
  /*!
   * \brief Default constructor.
   * \vprivate Component Private
   */
  ProviderInfo() = default;

  // Attention! Deviation: Protected destructor made non-virtual to satisfy AutosarC++17_10-A12.4.1
  /*!
   * \brief Destructor.
   * \vprivate Component Private
   */
  ~ProviderInfo() noexcept override = default;

  /*!
   * \brief Default copy constructor
   * \vprivate Component Private
   */
  ProviderInfo(ProviderInfo const& /*other*/) = default;

  /*!
   * \brief Default move constructor
   * \vprivate Component Private
   */
  ProviderInfo(ProviderInfo&& /*other*/) noexcept = default;

 public:
  // VECTOR NC VectorC++-V11.0.3: MD_CRYPTO_VectorC++-V11-0-3_public_data_members
  /*!
   * \brief The Provider's Version Structure.
   * \vpublic
   */
  struct Version {
    std::uint64_t buildTime;  /*!< Build time stamp (the number of seconds since the UNIX Epoch, 1 January 1970) */
    std::uint_fast16_t major; /*!< Major version number */
    std::uint_fast16_t minor; /*!< Minor version number */
    std::uint_fast16_t patch; /*!< Patch number */

    bool isRelease; /*!< Release flag: it is a "Release" version if true and "Pre-release" if false */

    // VECTOR NC AutosarC++17_10-M11.0.3: MD_CRYPTO_AutosarC++17_10-M11.0.3_method_in_struct
    /*!
     * \brief Encode the Provider's Version to a single 64-bit unsigned integer.
     * \return 64-bit encoded version number correspondent to the structure content
     * \context ANY
     * \pre -
     * \threadsafe TRUE
     * \reentrant FALSE
     * \synchronous TRUE
     * \vpublic
     */
    std::uint64_t Encode() const noexcept;

    // VECTOR NC AutosarC++17_10-M11.0.3: MD_CRYPTO_AutosarC++17_10-M11.0.3_method_in_struct
    /*!
     * \brief Any valid version at least must have (buildTime > 0).
     * \return true if the version is "nil" and false otherwise
     * \context ANY
     * \pre -
     * \threadsafe TRUE
     * \reentrant FALSE
     * \synchronous TRUE
     * \vpublic
     */
    bool IsNil() const noexcept;
  };

  // VECTOR NC AutosarC++17_10-A10.2.1: MD_CRYPTO_AutosarC++17_10-A10.2.1_hiding_functions
  /*!
   * \brief Delete move assignment constructor
   * \vprivate Component Private
   */
  ProviderInfo& operator=(ProviderInfo&& /*other*/) = delete;

  // VECTOR NC AutosarC++17_10-A10.2.1: MD_CRYPTO_AutosarC++17_10-A10.2.1_hiding_functions
  /*!
   * \brief Deleted copy assignment constructor
   * \vprivate Component Private
   */
  ProviderInfo& operator=(ProviderInfo const& /*other*/) = delete;

  /*!
   * \brief Returns an encoded version of the Provider.
   * \details The QWORD value returned by this method includes two DWORD-length fields:
   *          - "Version structure" (32 bits) - most significant DWORD
   *          - "Build date & time" (32 bits) - least significant DWORD (LSDW)
   *          The "Version structure" includes four fields (each of them corresponds to one byte):
   *          - Major version (8 bits) - most significant byte (MSB) of the Version DWORD
   *          - Minor version (8 bits)
   *          - Patch version (8 bits)
   *          - Revision version (8 bits) - least significant byte (LSB) of the Version DWORD
   *          The "Build date & time" is a number of minutes since 1st of January 1970 00:00.
   *          So generally the Version QWORD has following structure:
   *
   *          |  MSB  | 8 bits | 8 bits |  LSB  |      LSDW (32 bits)     |
   *          | :---: | :----: | :----: | :---: | :---------------------: |
   *          | Major | Minor  | Patch  | Revis |    Build date & time    |
   * \return Encoded version that has following structure:
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual std::uint64_t GetProviderVersion() const noexcept = 0;

  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
  /*!
   * \brief Returns a human readable name of the Provider.
   * \details Life-time of the returned string is not less than the Provider instance life-time.
   * \return A pointer to null-terminated string with Provider Name.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual char const* GetProviderName() const noexcept = 0;

  /*!
   * \brief Returns the Provider's Globally Unique Identifier (GUID).
   * \param[out] provider_uid An output buffer for the Provider's GUID.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual void GetProviderUid(Guid& provider_uid) const noexcept = 0;

  /*!
   * \brief Get type of the Provider.
   * \return Type of the Provider.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual ProviderType GetProviderType() const noexcept = 0;

  /*!
   * \brief Decode the encoded "version number" of the Provider.
   * \param[in] version_number the encoded "version number" of the Provider
   * \return parsed structure of the Provider's version
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  static Version DecodeVersionNumber(std::uint64_t version_number) noexcept;
};

inline std::uint64_t ProviderInfo::Version::Encode() const noexcept {
  std::uint64_t result{(buildTime / 60) & static_cast<std::uint64_t>(0x7fffffff)};
  std::uint64_t const release{isRelease ? static_cast<std::uint64_t>(1) : static_cast<std::uint64_t>(0)};
  result |= release << static_cast<std::uint64_t>(31);
  result |= static_cast<std::uint64_t>(patch & static_cast<std::uint_fast16_t>(0xfff))
            << static_cast<std::uint64_t>(32);
  result |= static_cast<std::uint64_t>(minor & static_cast<std::uint_fast16_t>(0x3ff))
            << static_cast<std::uint64_t>(44);
  result |= static_cast<std::uint64_t>(major & static_cast<std::uint_fast16_t>(0x3ff))
            << static_cast<std::uint64_t>(54);
  return result;
}

inline bool ProviderInfo::Version::IsNil() const noexcept { return 0ULL == Encode(); }

inline ProviderInfo::Version ProviderInfo::DecodeVersionNumber(std::uint64_t version_number) noexcept {
  Version result{};
  result.buildTime = (version_number & static_cast<std::uint64_t>(0x7fffffff)) * 60;
  result.isRelease = (version_number & static_cast<std::uint64_t>(0x80000000)) != 0;
  result.patch = static_cast<std::uint_fast16_t>((version_number >> static_cast<std::uint64_t>(32)) &
                                                 static_cast<std::uint_fast16_t>(0xfff));
  result.minor = static_cast<std::uint_fast16_t>((version_number >> static_cast<std::uint64_t>(44)) &
                                                 static_cast<std::uint_fast16_t>(0x3ff));
  result.major = static_cast<std::uint_fast16_t>((version_number >> static_cast<std::uint64_t>(54)) &
                                                 static_cast<std::uint_fast16_t>(0x3ff));
  return result;
}

/*!
 * \brief Checks for equality of two versions.
 * \param[in] lhs Left-hand side version.
 * \param[in] rhs Right-hand side version.
 * \return True iff both versions are equal.
 * \context ANY
 * \pre -
 * \threadsafe TRUE
 * \reentrant FALSE
 * \synchronous TRUE
 * \vpublic
 */
inline bool operator==(ProviderInfo::Version const& lhs, ProviderInfo::Version const& rhs) noexcept {
  return lhs.Encode() == rhs.Encode();
}

/*!
 * \brief Checks if one version is smaller than the other.
 * \param[in] lhs Left-hand side version.
 * \param[in] rhs Right-hand side version.
 * \return True iff the left-hand side version is smaller than the right-hand side version.
 * \context ANY
 * \pre -
 * \threadsafe TRUE
 * \reentrant FALSE
 * \synchronous TRUE
 * \vpublic
 */
inline bool operator<(ProviderInfo::Version const& lhs, ProviderInfo::Version const& rhs) noexcept {
  return lhs.Encode() < rhs.Encode();
}

/*!
 * \brief Checks if one version is greater than the other.
 * \param[in] lhs Left-hand side version.
 * \param[in] rhs Right-hand side version.
 * \return True iff the left-hand side version is greater than the right-hand side version.
 * \context ANY
 * \pre -
 * \threadsafe TRUE
 * \reentrant FALSE
 * \synchronous TRUE
 * \vpublic
 */
inline bool operator>(ProviderInfo::Version const& lhs, ProviderInfo::Version const& rhs) noexcept {
  return lhs.Encode() > rhs.Encode();
}

/*!
 * \brief Checks for unequality of two versions.
 * \param[in] lhs Left-hand side version.
 * \param[in] rhs Right-hand side version.
 * \return True iff both versions are not equal.
 * \context ANY
 * \pre -
 * \threadsafe TRUE
 * \reentrant FALSE
 * \synchronous TRUE
 * \vpublic
 */
inline bool operator!=(ProviderInfo::Version const& lhs, ProviderInfo::Version const& rhs) noexcept {
  return !(lhs == rhs);
}

/*!
 * \brief Checks if one version is smaller or equal to the other.
 * \param[in] lhs Left-hand side version.
 * \param[in] rhs Right-hand side version.
 * \return True iff the left-hand side version is smaller or equal to the right-hand side version.
 * \context ANY
 * \pre -
 * \threadsafe TRUE
 * \reentrant FALSE
 * \synchronous TRUE
 * \vpublic
 */
inline bool operator<=(ProviderInfo::Version const& lhs, ProviderInfo::Version const& rhs) noexcept {
  return !(lhs > rhs);
}

/*!
 * \brief Checks if one version is greater or equal to the other.
 * \param[in] lhs Left-hand side version.
 * \param[in] rhs Right-hand side version.
 * \return True iff the left-hand side version is greater or equal to the right-hand side version.
 * \context ANY
 * \pre -
 * \threadsafe TRUE
 * \reentrant FALSE
 * \synchronous TRUE
 * \vpublic
 */
inline bool operator>=(ProviderInfo::Version const& lhs, ProviderInfo::Version const& rhs) noexcept {
  return !(lhs < rhs);
}

}  // namespace crypto
}  // namespace ara

#endif  // LIB_LIBARA_INCLUDE_ARA_CRYPTO_COMMON_PROVIDER_INFO_H_
