// VECTOR SL AutosarC++17_10-A2.9.1: MD_CRC_AutosarC++17_10-A2.9.1
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
/*!        \file  crc.h
 *        \brief  Functions for Crc Value calculations.
 *
 *      \details  Calculation of Crc values using the implementation from MICROSAR.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBCRC_INCLUDE_CRC_CRC_H_
#define LIB_LIBCRC_INCLUDE_CRC_CRC_H_

/*********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstdint>
#include <utility>
#include "ara/core/result.h"
#include "ara/core/span.h"

namespace crc {

/*!
 * \brief Data type used for input into Crc functions
 * \vpublic
 */
using CrcData = ara::core::Span<std::uint8_t const>;

/*!
 * \brief Template class to realize strong typing for return types of crc functions.
 * \vpublic
 */
template <typename T, typename Parameter>
class CrcValueType {
 public:
  /*!
   * \brief Default constructor from value
   * \param[in] value CRC-value
   *
   * \context ANY
   *
   * \pre -
   *
   * \threadsafe TRUE If \c value is not changed by another thread.
   * \reentrant TRUE
   * \synchronous TRUE
   *
   * \spec requires true; \endspec
   * \vpublic
   */
  explicit CrcValueType(T const& value) : value_(value) {}

  /*!
   * \brief Get the CrcValue
   * \return CrcValue associated with this object
   *
   * \context ANY
   *
   * \pre -
   *
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   *
   * \spec requires true; \endspec
   * \vpublic
   */
  T const& get() const { return value_; }

 private:
  /*!
   * \brief CrcValue stored by this object
   */
  T value_;
};

/*!
 * \brief Return value of Crc-8 Routine
 * \vpublic
 */
using Crc8Value = CrcValueType<std::uint8_t, struct Crc8ValueParameter>;

/*!
 * \brief Return value of Crc-8h2f Routine
 * \vpublic
 */
using Crc8h2fValue = CrcValueType<std::uint8_t, struct Crc8h2fValueParameter>;

/*!
 * \brief Return value of Crc-16 Routine
 * \vpublic
 */
using Crc16Value = CrcValueType<std::uint16_t, struct Crc16ValueParameter>;

/*!
 * \brief Return value of Crc-32 Routine
 * \vpublic
 */
using Crc32Value = CrcValueType<std::uint32_t, struct Crc32ValueParameter>;

/*!
 * \brief Return value of Crc-32reflect Routine
 * \vpublic
 */
using Crc32reflectValue = CrcValueType<std::uint32_t, struct Crc32reflectValueParameter>;

/*!
 * \brief Return value of Crc-32p4 Routine
 * \vpublic
 */
using Crc32p4Value = CrcValueType<std::uint32_t, struct Crc32p4ValueParameter>;

/*!
 * \brief Return value of Crc-64 Routine
 * \vpublic
 */
using Crc64Value = CrcValueType<std::uint64_t, struct Crc64ValueParameter>;

/*!
 * \brief Calculates the 8-bit CRC value for the passed data
 * \param[in] data Input data for the CRC calculation
 * \return Calculated 8-bit CRC value
 *
 * \context ANY
 *
 * \pre -
 *
 * \threadsafe TRUE If \c data is not changed by another thread.
 * \reentrant TRUE
 * \synchronous TRUE
 *
 * \spec requires true; \endspec
 * \trace DSGN-CRC-Crc8
 * \vpublic
 */
auto CalculateCrc8(CrcData const& data) noexcept -> Crc8Value;

/*!
 * \brief Calculates the 8-bit CRC value for the passed data
 * \param[in] data Input data for the CRC calculation
 * \param[in] start_value Start value when the algorithm starts i.e. the output of a previous call
 * in a sequence of calls
 * \return Calculated 8-bit CRC value
 *
 * \context ANY
 *
 * \pre -
 *
 * \threadsafe TRUE If \c data is not changed by another thread.
 * \reentrant TRUE
 * \synchronous TRUE
 *
 * \spec requires true; \endspec
 * \trace DSGN-CRC-Crc8
 * \vpublic
 */
auto CalculateCrc8(CrcData const& data, Crc8Value const& start_value) noexcept -> Crc8Value;

/*!
 * \brief Calculates the 8-bit CRC8h2f value for the passed data
 * \param[in] data Input data for the CRC calculation
 * \return Calculated 8-bit CRCh2f value
 *
 * \context ANY
 *
 * \pre -
 *
 * \threadsafe TRUE If \c data is not changed by another thread.
 * \reentrant TRUE
 * \synchronous TRUE
 *
 * \spec requires true; \endspec
 * \trace DSGN-CRC-Crc8h2f
 * \vpublic
 */
auto CalculateCrc8h2f(CrcData const& data) noexcept -> Crc8h2fValue;

/*!
 * \brief Calculates the 8-bit CRC8h2f value for the passed data
 * \param[in] data Input data for the CRC calculation
 * \param[in] start_value Start value when the algorithm starts i.e. the output of a previous call
 * in a sequence of calls
 * \return Calculated 8-bit CRCh2f value
 *
 * \context ANY
 *
 * \pre -
 *
 * \threadsafe TRUE If \c data is not changed by another thread.
 * \reentrant TRUE
 * \synchronous TRUE
 *
 * \spec requires true; \endspec
 * \trace DSGN-CRC-Crc8h2f
 * \vpublic
 */
auto CalculateCrc8h2f(CrcData const& data, Crc8h2fValue const& start_value) noexcept -> Crc8h2fValue;

/*!
 * \brief Calculates the 16-bit CRC value for the passed data
 * \param[in] data Input data for the CRC calculation
 * \return Calculated 16-bit CRC value
 *
 * \context ANY
 *
 * \pre -
 *
 * \threadsafe TRUE If \c data is not changed by another thread.
 * \reentrant TRUE
 * \synchronous TRUE
 *
 * \spec requires true; \endspec
 * \trace DSGN-CRC-Crc16
 * \vpublic
 */
auto CalculateCrc16(CrcData const& data) noexcept -> Crc16Value;

/*!
 * \brief Calculates the 16-bit CRC value for the passed data
 * \param[in] data Input data for the CRC calculation
 * \param[in] start_value Start value when the algorithm starts i.e. the output of a previous call
 * in a sequence of calls
 * \return Calculated 16-bit CRC value
 *
 * \context ANY
 *
 * \pre -
 *
 * \threadsafe TRUE If \c data is not changed by another thread.
 * \reentrant TRUE
 * \synchronous TRUE
 *
 * \spec requires true; \endspec
 * \trace DSGN-CRC-Crc16
 * \vpublic
 */
auto CalculateCrc16(CrcData const& data, Crc16Value const& start_value) noexcept -> Crc16Value;

/*!
 * \brief Calculates the 32-bit CRC value for the passed data
 * \param[in] data Input data for the CRC calculation
 * \return Calculated 32-bit CRC value
 *
 * \context ANY
 *
 * \pre -
 *
 * \threadsafe TRUE If \c data is not changed by another thread.
 * \reentrant TRUE
 * \synchronous TRUE
 *
 * \spec requires true; \endspec
 * \trace DSGN-CRC-Crc32
 * \vpublic
 */
auto CalculateCrc32(CrcData const& data) noexcept -> Crc32Value;

/*!
 * \brief Calculates the 32-bit CRC value for the passed data
 * \param[in] data Input data for the CRC calculation
 * \param[in] start_value Start value when the algorithm starts i.e. the output of a previous call
 * in a sequence of calls
 * \return Calculated 32-bit CRC value
 *
 * \context ANY
 *
 * \pre -
 *
 * \threadsafe TRUE If \c data is not changed by another thread.
 * \reentrant TRUE
 * \synchronous TRUE
 *
 * \spec requires true; \endspec
 * \trace DSGN-CRC-Crc32
 * \vpublic
 */
auto CalculateCrc32(CrcData const& data, Crc32Value const& start_value) noexcept -> Crc32Value;

/*!
 * \brief Calculates the 32-bit CRC32reflect value for the passed data
 * \param[in] data Input data for the CRC calculation
 * \return Calculated 32-bit CRC value
 *
 * \context ANY
 *
 * \pre -
 *
 * \threadsafe TRUE If \c data is not changed by another thread.
 * \reentrant TRUE
 * \synchronous TRUE
 *
 * \spec requires true; \endspec
 * \trace DSGN-CRC-Crc32reflect
 * \vpublic
 */
auto CalculateCrc32reflect(CrcData const& data) noexcept -> Crc32reflectValue;

/*!
 * \brief Calculates the 32-bit CRC32reflect value for the passed data
 * \param[in] data Input data for the CRC calculation
 * \param[in] start_value Start value when the algorithm starts i.e. the output of a previous call
 * in a sequence of calls
 * \return Calculated 32-bit CRC value
 *
 * \context ANY
 *
 * \pre -
 *
 * \threadsafe TRUE If \c data is not changed by another thread.
 * \reentrant TRUE
 * \synchronous TRUE
 *
 * \spec requires true; \endspec
 * \trace DSGN-CRC-Crc32reflect
 * \vpublic
 */
auto CalculateCrc32reflect(CrcData const& data, Crc32reflectValue const& start_value) noexcept -> Crc32reflectValue;

/*!
 * \brief Calculates the 32-bit CRC32p4 value for the passed data
 * \param[in] data Input data for the CRC calculation
 * \return Calculated 32-bit CRC value
 *
 * \context ANY
 *
 * \pre -
 *
 * \threadsafe TRUE If \c data is not changed by another thread.
 * \reentrant TRUE
 * \synchronous TRUE
 *
 * \spec requires true; \endspec
 * \trace DSGN-CRC-Crc32p4
 * \vpublic
 */
auto CalculateCrc32p4(CrcData const& data) noexcept -> Crc32p4Value;

/*!
 * \brief Calculates the 32-bit CRC32p4 value for the passed data
 * \param[in] data Input data for the CRC calculation
 * \param[in] start_value Start value when the algorithm starts i.e. the output of a previous call
 * in a sequence of calls
 * \return Calculated 32-bit CRC value
 *
 * \context ANY
 *
 * \pre -
 *
 * \threadsafe TRUE If \c data is not changed by another thread.
 * \reentrant TRUE
 * \synchronous TRUE
 *
 * \spec requires true; \endspec
 * \trace DSGN-CRC-Crc32p4
 * \vpublic
 */
auto CalculateCrc32p4(CrcData const& data, Crc32p4Value const& start_value) noexcept -> Crc32p4Value;

/*!
 * \brief Calculates the 64-bit CRC value for the passed data
 * \param[in] data Input data for the CRC calculation
 * \return Calculated 64-bit CRC value
 *
 * \context ANY
 *
 * \pre -
 *
 * \threadsafe TRUE If \c data is not changed by another thread.
 * \reentrant TRUE
 * \synchronous TRUE
 *
 * \spec requires true; \endspec
 * \trace DSGN-CRC-Crc64
 * \vpublic
 */
auto CalculateCrc64(CrcData const& data) noexcept -> Crc64Value;

/*!
 * \brief Calculates the 64-bit CRC value for the passed data
 * \param[in] data Input data for the CRC calculation
 * \param[in] start_value Start value when the algorithm starts i.e. the output of a previous call
 * in a sequence of calls
 * \return Calculated 64-bit CRC value
 *
 * \context ANY
 *
 * \pre -
 *
 * \threadsafe TRUE If \c data is not changed by another thread.
 * \reentrant TRUE
 * \synchronous TRUE
 *
 * \spec requires true; \endspec
 * \trace DSGN-CRC-Crc64
 * \vpublic
 */
auto CalculateCrc64(CrcData const& data, Crc64Value const& start_value) noexcept -> Crc64Value;

}  // namespace crc

#endif  // LIB_LIBCRC_INCLUDE_CRC_CRC_H_
