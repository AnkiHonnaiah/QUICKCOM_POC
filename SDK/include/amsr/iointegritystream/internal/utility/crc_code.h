/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2024 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file        crc_code.h
 *         \brief       ---
 *         \unit        amsr::iointegritystream::internal::utility::CrcCode
 *
 *         \complexity  The complexity is increased by many inter module calls which are necessary for the correct
 *                      functionality of this unit and does not introduce any risk.
 *********************************************************************************************************************/
#ifndef LIB_IOINTEGRITYSTREAM_INCLUDE_AMSR_IOINTEGRITYSTREAM_INTERNAL_UTILITY_CRC_CODE_H_
#define LIB_IOINTEGRITYSTREAM_INCLUDE_AMSR_IOINTEGRITYSTREAM_INTERNAL_UTILITY_CRC_CODE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/core/span.h"
#include "amsr/iointegritystream/strategy_types.h"
#include "crc/crc.h"

namespace amsr {
namespace iointegritystream {
namespace internal {
namespace utility {

/*!
 * \brief Wraps the CRC functionality for iteratively updating a CRC code.
 * \trace DSGN-IoIntegrity-Reader, DSGN-IoIntegrity-Writer, DSGN-IoIntegrity-Reader-Writer
 */
class CrcCode final {
 public:
  /*!
   * \brief Constructs the object.
   * \param[in] alg Algorithm to use (32- or 64-bit).
   * \param[in] start_value Start value for the CRC code.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   */
  explicit CrcCode(CRCAlgorithm alg, std::uint64_t start_value = 0) noexcept
      : alg_{alg}, crc_32_{static_cast<std::uint32_t>(start_value)}, crc_64_{start_value} {}

  /*!
   * \brief Constructs the object.
   * \tparam T Data element type to use.
   * \param[in] alg Algorithm to use (32- or 64-bit).
   * \param[in] data Start value for the CRC code.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   */
  explicit CrcCode(CRCAlgorithm alg, amsr::core::Span<char const> data) noexcept : CrcCode(std::move(alg)) {
    Update(data);
  }

  /*!
   * \brief Deleted default constructor.
   * \details The algorithm needs to be provided.
   */
  CrcCode() noexcept = delete;

  /*!
   * \brief Constructs the object.
   * \param[in] other Object to copy.
   * \return Copy of this.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   */
  CrcCode(CrcCode const& other) noexcept = default;

  /*!
   * \brief Constructs the object.
   * \param[in,out] other Object to move.
   * \return Reference to this.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   */
  CrcCode(CrcCode&& other) noexcept = default;

  /*!
   * \brief Assigns the object.
   * \param[in] other Object to copy.
   * \return Copy of this.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   */
  CrcCode& operator=(CrcCode const& other) & noexcept = default;

  /*!
   * \brief Assigns the object.
   * \param[in,out] other Object to move.
   * \return Reference to this.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   */
  CrcCode& operator=(CrcCode&& other) & noexcept = default;

  /*!
   * \brief Deletes the object.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   */
  ~CrcCode() noexcept = default;

  /*!
   * \brief Updates the object with the additional data provided.
   * \tparam T Type of data.
   * \param[in] data Data to be represented by the CRC code.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   */
  template <typename T, std::enable_if_t<sizeof(T) == sizeof(std::uint8_t), std::int32_t> = 0>
  inline void Update(amsr::core::Span<T const> data) noexcept {
    // VECTOR NL AutosarC++17_10-A5.2.4: MD_IoIntegrityStream_A5.2.4_reinterpretCastCharPtr
    crc::CrcData const crc_data{reinterpret_cast<std::uint8_t const*>(data.data()), data.size()};

    if (alg_ == CRCAlgorithm::CRC32) {
      crc_32_ = crc::CalculateCrc32(crc_data, crc_32_);
    } else {
      crc_64_ = crc::CalculateCrc64(crc_data, crc_64_);
    }
  }

  /*!
   * \brief Get the current state of the CRC code.
   * \return The current state of the CRC code.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   */
  inline std::uint64_t Get() const noexcept {
    std::uint64_t result{};

    if (alg_ == CRCAlgorithm::CRC32) {
      result = static_cast<std::uint64_t>(crc_32_.get());
    } else {
      result = crc_64_.get();
    }

    return result;
  }

 private:
  /*! \brief Algorithm used. */
  CRCAlgorithm alg_;
  /*! \brief 32-bit CRC code. */
  crc::Crc32Value crc_32_;
  /*! \brief 64-bit CRC code. */
  crc::Crc64Value crc_64_;
};

}  // namespace utility
}  // namespace internal
}  // namespace iointegritystream
}  // namespace amsr

#endif  // LIB_IOINTEGRITYSTREAM_INCLUDE_AMSR_IOINTEGRITYSTREAM_INTERNAL_UTILITY_CRC_CODE_H_
