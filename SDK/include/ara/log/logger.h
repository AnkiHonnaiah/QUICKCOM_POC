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
/**     \file       ara/log/logger.h
 *      \brief
 *      \details
 *
 *********************************************************************************************************************/
#ifndef LIB_ARA_LOGGING_INCLUDE_ARA_LOG_LOGGER_H_
#define LIB_ARA_LOGGING_INCLUDE_ARA_LOG_LOGGER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/log/logger.h"  // IWYU pragma: export
#include "ara/log/logstream.h"

namespace ara {
namespace log {
/*!
 * \copydoc amsr::log::Logger
 */
using Logger = amsr::log::Logger;

/*!
 * \brief Logs decimal numbers in hexadecimal format.
 *
 * Negatives are represented in 2's complement.
 *
 * \param[in] value Decimal number to be converted into hexadecimal number system
 * \return LogHex8 type that has a built-in stream handler
 *
 * \details As defined in SWS_LOG_00016, SWS_LOG_00022
 * \spec
 *   requires true;
 * \endspec
 */
constexpr auto HexFormat(std::uint8_t value) noexcept -> LogHex8 { return amsr::log::HexFormat(value); }

/*!
 * \brief Logs decimal numbers in hexadecimal format.
 *
 * \param value Decimal number to be converted into hexadecimal number system
 * \return LogHex8 type that has a built-in stream handler
 *
 * \details As defined in SWS_LOG_00016, SWS_LOG_00023
 * \spec
 *   requires true;
 * \endspec
 */
constexpr auto HexFormat(std::int8_t value) noexcept -> LogHex8 { return amsr::log::HexFormat(value); }

/*!
 * \brief Logs decimal numbers in hexadecimal format.
 *
 * \param[in] value Decimal number to be converted into hexadecimal number system
 * \return LogHex16 type that has a built-in stream handler
 *
 * \details As defined in SWS_LOG_00016, SWS_LOG_00024
 * \spec
 *   requires true;
 * \endspec
 */
constexpr auto HexFormat(std::uint16_t value) noexcept -> LogHex16 { return amsr::log::HexFormat(value); }

/*!
 * \brief Logs decimal numbers in hexadecimal format.
 *
 * \param[in] value Decimal number to be converted into hexadecimal number system
 * \return LogHex16 type that has a built-in stream handler
 *
 * \details As defined in SWS_LOG_00016, SWS_LOG_00025
 * \spec
 *   requires true;
 * \endspec
 */
constexpr auto HexFormat(std::int16_t value) noexcept -> LogHex16 { return amsr::log::HexFormat(value); }

/*!
 * \brief Logs decimal numbers in hexadecimal format.
 *
 * \param[in] value Decimal number to be converted into hexadecimal number system
 * \return LogHex32 type that has a built-in stream handler
 *
 * \details As defined in SWS_LOG_00016, SWS_LOG_00026
 * \spec
 *   requires true;
 * \endspec
 */
constexpr auto HexFormat(std::uint32_t value) noexcept -> LogHex32 { return amsr::log::HexFormat(value); }

/*!
 * \brief Logs decimal numbers in hexadecimal format.
 *
 * \param[in] value Decimal number to be converted into hexadecimal number system
 * \return LogHex32 type that has a built-in stream handler
 *
 * \details As defined in SWS_LOG_00016, SWS_LOG_00027
 * \spec
 *   requires true;
 * \endspec
 */
constexpr auto HexFormat(std::int32_t value) noexcept -> LogHex32 { return amsr::log::HexFormat(value); }

/*!
 * \brief Logs decimal numbers in hexadecimal format.
 *
 * \param[in] value Decimal number to be converted into hexadecimal number system
 * \return LogHex64 type that has a built-in stream handler
 *
 * \details As defined in SWS_LOG_00016, SWS_LOG_00028
 * \spec
 *   requires true;
 * \endspec
 */
constexpr auto HexFormat(std::uint64_t value) noexcept -> LogHex64 { return amsr::log::HexFormat(value); }

/*!
 * \brief Logs decimal numbers in hexadecimal format.
 *
 * \param[in] value Decimal number to be converted into hexadecimal number system
 * \return LogHex64 type that has a built-in stream handler
 *
 * \details As defined in SWS_LOG_00016, SWS_LOG_00029
 * \spec
 *   requires true;
 * \endspec
 */
constexpr auto HexFormat(std::int64_t value) noexcept -> LogHex64 { return amsr::log::HexFormat(value); }

/*!
 * \brief Logs decimal numbers in binary format.
 *
 * Negatives are represented in 2's complement.

 * \param[in] value Decimal number to be converted into a binary value
 * \return LogBin8 type that has a built-in stream handler
 *
 * \details As defined in SWS_LOG_00017, SWS_LOG_00030
 * \spec
 *   requires true;
 * \endspec
 */
constexpr auto BinFormat(std::uint8_t value) noexcept -> LogBin8 { return amsr::log::BinFormat(value); }

/*!
 * \brief Logs decimal numbers in binary format.
 *
 * \param[in] value Decimal number to be converted into a binary value
 * \return LogBin8 type that has a built-in stream handler
 *
 * \details As defined in SWS_LOG_00017, SWS_LOG_00031
 * \spec
 *   requires true;
 * \endspec
 */
constexpr auto BinFormat(std::int8_t value) noexcept -> LogBin8 { return amsr::log::BinFormat(value); }

/*!
 * \brief Logs decimal numbers in binary format.
 *
 * \param[in] value Decimal number to be converted into a binary value
 * \return LogBin16 type that has a built-in stream handler
 *
 * \details As defined in SWS_LOG_00017, SWS_LOG_00032
 * \spec
 *   requires true;
 * \endspec
 */
constexpr auto BinFormat(std::uint16_t value) noexcept -> LogBin16 { return amsr::log::BinFormat(value); }

/*!
 * \brief Logs decimal numbers in binary format.
 *
 * \param[in] value Decimal number to be represented in 16 digits
 * \return LogBin16 type that has a built-in stream handler
 *
 * \details As defined in SWS_LOG_00017, SWS_LOG_00033
 * \spec
 *   requires true;
 * \endspec
 */
constexpr auto BinFormat(std::int16_t value) noexcept -> LogBin16 { return amsr::log::BinFormat(value); }

/*!
 * \brief Logs decimal numbers in binary format.
 *
 * \param[in] value Decimal number to be converted into a binary value
 * \return LogBin32 type that has a built-in stream handler
 *
 * \details As defined in SWS_LOG_00017, SWS_LOG_00034
 * \spec
 *   requires true;
 * \endspec
 */
constexpr auto BinFormat(std::uint32_t value) noexcept -> LogBin32 { return amsr::log::BinFormat(value); }

/*!
 * \brief Logs decimal numbers in binary format.
 *
 * \param[in] value Decimal number to be converted into a binary value
 * \return LogBin32 type that has a built-in stream handler
 *
 * \details As defined in SWS_LOG_00017, SWS_LOG_00035
 * \spec
 *   requires true;
 * \endspec
 */
constexpr auto BinFormat(std::int32_t value) noexcept -> LogBin32 { return amsr::log::BinFormat(value); }

/*!
 * \brief Logs decimal numbers in binary format.
 *
 * \param[in] value Decimal number to be converted into a binary value
 * \return LogBin64 type that has a built-in stream handler
 *
 * \details As defined in SWS_LOG_00017, SWS_LOG_00036
 * \spec
 *   requires true;
 * \endspec
 */
constexpr auto BinFormat(std::uint64_t value) noexcept -> LogBin64 { return amsr::log::BinFormat(value); }

/*!
 * \brief Logs decimal numbers in binary format.
 *
 * \param[in] value Decimal number to be converted into a binary value
 * \return LogBin64 type that has a built-in stream handler
 *
 * \details As defined in SWS_LOG_00017, SWS_LOG_00037
 * \spec
 *   requires true;
 * \endspec
 */
constexpr auto BinFormat(std::int64_t value) noexcept -> LogBin64 { return amsr::log::BinFormat(value); }

/*!
 * \brief Logs raw binary data by providing a buffer
 * \tparam T The type of the contents of value.
 *           T can take an arbitrary type.
 *           T shall not be a pointer.
 * \param value Reference to binary data
 * \return LogRawBuffer type that has a built-in stream handler
 *
 * \details As defined in SWS_LOG_00038, SWS_LOG_00014
 * \spec
 *   requires true;
 * \endspec
 */
template <typename T>
constexpr auto RawBuffer(T const& value) noexcept -> LogRawBuffer {
  // VECTOR NC AutosarC++17_10-A5.2.4: MD_LOG_AutosarC++17_10-A5.2.4_external_API
  return {reinterpret_cast<char const*>(&value), sizeof(T)};  // NOLINT(cppcoreguidelines-pro-type-reinterpret-cast)
}

}  // namespace log
}  // namespace ara

#endif  // LIB_ARA_LOGGING_INCLUDE_ARA_LOG_LOGGER_H_
