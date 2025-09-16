/*!********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2022 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!       \file   record_size_limit.h
 *        \brief  Parser for record size limit extension type.
 *********************************************************************************************************************/
#ifndef LIB_LIBTLS_INCLUDE_TLS_INTERNAL_MESSAGES_EXTENSIONS_RECORD_SIZE_LIMIT_H_
#define LIB_LIBTLS_INCLUDE_TLS_INTERNAL_MESSAGES_EXTENSIONS_RECORD_SIZE_LIMIT_H_

#include <cstdint>

#include "amsr/core/result.h"
#include "amsr/core/span.h"
#include "ara/core/vector.h"

namespace amsr {
namespace tls {
namespace internal {
namespace messages {
namespace extensions {

/*!
 * \brief Record size limit extension type serializer and deserializer.
 */
class RecordSizeLimitExtension final {
 public:
  /*!
   * \brief Deserialize an instance of the class from a serialized data buffer.
   * \param[in] buffer The buffer to read from.
   * \return Result with the class instance or an error.
   * \error tls::TlsCommErrorCode::kTlsErrDeserialize if the buffers size is not exactly 2 bytes.
   * \error tls::TlsCommErrorCode::kTlsErrInvalidArgument if the extracted value is not within the range of [64, 32768].
   */
  static amsr::core::Result<RecordSizeLimitExtension> Deserialize(
      amsr::core::Span<std::uint8_t const> const &buffer) noexcept;

  /*!
   * \brief Create an instance of the class.
   * \param[in] record_size_limit Record size limit to create from.
   * \return Result with the class instance or an error.
   * \error tls::TlsCommErrorCode::kTlsErrInvalidArgument if the value is not within the range of [64, 32768].
   */
  static amsr::core::Result<RecordSizeLimitExtension> Create(std::uint16_t record_size_limit) noexcept;

  /*!
   * \brief Destructor.
   */
  ~RecordSizeLimitExtension() noexcept = default;

  /*!
   * \brief Not copy constructible.
   */
  RecordSizeLimitExtension(RecordSizeLimitExtension const &) = delete;

  /*!
   * \brief Move constructor.
   */
  RecordSizeLimitExtension(RecordSizeLimitExtension &&) noexcept = default;

  /*!
   * \brief Not copy assignable.
   */
  RecordSizeLimitExtension &operator=(RecordSizeLimitExtension const &) = delete;

  /*!
   * \brief Not move assignable.
   */
  RecordSizeLimitExtension &operator=(RecordSizeLimitExtension &&) = delete;

  /*!
   * \brief Serialize the member variables into a buffer.
   * \param[out] buffer The buffer to write into.
   */
  void Serialize(ara::core::Vector<std::uint8_t> &buffer) const noexcept;

  /*!
   * \brief Gets the value for the record size limit.
   * \return The record size limit.
   */
  std::uint16_t GetValue() const noexcept;

 private:
  /*!
   * \brief Constructor.
   * \param[in] record_size_limit Record size limit to construct from.
   */
  explicit RecordSizeLimitExtension(std::uint16_t record_size_limit) noexcept;
  /*!
   * \brief Checks that the buffer's size has the expected size of the extension data.
   * \param[in] buffer A reference to the buffer.
   * \return Empty result or an error if the size was not as expected.
   * \error tls::TlsCommErrorCode::kTlsErrDeserialize if the buffers size is not exactly 2 bytes.
   */
  static amsr::core::Result<void> CheckBufferSize(amsr::core::Span<std::uint8_t const> const &buffer) noexcept;
  /*!
   * \brief Checks that the value is withing the accepted boundaries.
   * \param[in] value The deserialized value.
   * \return Empty result or an error if the value was not as expected.
   * \error tls::TlsCommErrorCode::kTlsErrInvalidArgument if the value is not within the range of [64, 32768].
   */
  static amsr::core::Result<void> CheckValue(std::uint16_t value) noexcept;

  /*!
   * \brief Record size limit.
   */
  std::uint16_t const record_size_limit_;
};

}  // namespace extensions
}  // namespace messages
}  // namespace internal
}  // namespace tls
}  // namespace amsr

#endif  // LIB_LIBTLS_INCLUDE_TLS_INTERNAL_MESSAGES_EXTENSIONS_RECORD_SIZE_LIMIT_H_
