/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2020 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!       \file   message_helper_functions.h
 *        \brief  Helper functions used internally by the handshake messages classes in the TLS library
 *
 *********************************************************************************************************************/
#ifndef LIB_LIBTLS_INCLUDE_TLS_INTERNAL_MESSAGES_MESSAGE_HELPER_FUNCTIONS_H_
#define LIB_LIBTLS_INCLUDE_TLS_INTERNAL_MESSAGES_MESSAGE_HELPER_FUNCTIONS_H_

#include <cstdint>
#include "ara/core/vector.h"
#include "tls/internal/common/common_general_types.h"
#include "tls/internal/common/helper_functions.h"

namespace amsr {
namespace tls {
namespace internal {
namespace messages {
namespace utility {

/*!
 * \brief Deserializes one byte from the buffer into uint8 variable
 * \details Checks that the buffer contains uint8 field at the buffer cursor. if yes it writes the data to the
 * output_variable. otherwise, it reports an error to the cxt
 * \param[in,out] cxt TlsCommFlowContext used for tracing internal logic flow and error reporting
 * \param[in] buffer The uint8 vector which includes the data to be deserialized
 * \param[in] index The current index
 * \param[in] last_data8 true if the pointed data must be at the end of the buffer. otherwise, false
 * \return The deserialized value packed in a std::uint8_t
 * \context ANY
 * \pre -
 * \reentrant FALSE
 * \error TlsCommErrorCode::kTlsErrDeserialize is reported if deserialization fails due to incomplete message
 * \trace amsr::tls::internal::messages::HandshakeMessage::Deserialize
 * \vprivate This method is component private.
 */
std::uint8_t DeserializeTlsData8(common::TlsCommFlowContext& cxt, ara::core::Vector<uint8_t> const& buffer,
                                 common::Index const index, bool const last_data8) noexcept;

/*!
 * \brief Deserializes two byte from the buffer into uint16 variable
 * \details Checks that the buffer contains two uint8 fields at the buffer cursor. if yes it writes the data to the
 * output_variable. otherwise, it reports an error to the cxt
 * \param[in,out] cxt TlsCommFlowContext used for tracing internal logic flow and error reporting
 * \param[in] buffer The uint8 vector which includes the data to be deserialized
 * \param[in] index The current index
 * \param[in] last_data16 true if the pointed data must be at the end of the buffer. otherwise, false
 * \return The deserialized value packed in a std::uint16_t
 * \context ANY
 * \pre -
 * \reentrant FALSE
 * \error TlsCommErrorCode::kTlsErrDeserialize is reported if deserialization fails due to incomplete message
 * \trace amsr::tls::internal::messages::HandshakeMessage::Deserialize
 * \vprivate This method is component private.
 */
std::uint16_t DeserializeTlsData16(common::TlsCommFlowContext& cxt, ara::core::Vector<uint8_t> const& buffer,
                                   common::Index const index, bool const last_data16) noexcept;

/*!
 * \brief         Deserializes three/four bytes from the buffer into uint32 variable.
 * \details       Checks that the buffer contains the required "bytes" of uint8 fields at the buffer cursor.
 *                If yes it writes the data to the output_variable. otherwise, it reports an error to the cxt.
 * \param[in,out] cxt TlsCommFlowContext used for tracing internal logic flow and error reporting.
 * \param[in]     buffer The uint8 vector which includes the data to be deserialized
 * \param[in]     index The current index
 * \param[in]     size_field_bytes The number of bytes to be deserialized into uint32, max value is 4.
 * \return        The deserialized value packed in a std::uint32_t
 * \context       ANY
 * \pre -
 * \reentrant     FALSE
 * \error         TlsCommErrorCode::kTlsErrDeserialize is reported if deserialization fails due to incomplete message
 * \trace         amsr::tls::internal::messages::HandshakeMessage::Deserialize
 * \vprivate      This method is component private.
 */
std::uint32_t DeserializeTlsData32(common::TlsCommFlowContext& cxt, ara::core::Vector<uint8_t> const& buffer,
                                   common::Index const index, std::size_t const size_field_bytes) noexcept;

/*!
 * \brief Deserializes n bytes from a uint8 buffer into another uint8 vector type buffer
 * \details Checks that the buffer contains n uint8 fields at the buffer cursor. if yes it writes the data to the
 * output_buffer. otherwise, it reports an error to the cxt
 * \param[in,out] cxt TlsCommFlowContext used for tracing internal logic flow and error reporting
 * \param[in] buffer The uint8 vector which includes the data to be deserialized.
 * \param[in] index The current index
 * \param[in] nr_of_bytes number of bytes to be read from the buffer
 * \param[in] last_field_in_range true if the pointed data must be at the end of the buffer. otherwise, false
 * \return A copy of the deserialized field
 * \context ANY
 * \pre -
 * \reentrant FALSE
 * \error TlsCommErrorCode::kTlsErrDeserialize is reported if deserialization fails due to incomplete message
 * \trace amsr::tls::internal::messages::HandshakeMessage::Deserialize
 * \vprivate This method is component private.
 */
ara::core::Vector<std::uint8_t> DeserializeTlsStaticField8(common::TlsCommFlowContext& cxt,
                                                           ara::core::Vector<uint8_t> const& buffer,
                                                           common::Index const index,
                                                           common::NrOfBytes const nr_of_bytes,
                                                           bool const last_field_in_range) noexcept;

/*!
 * \brief Deserializes a dynamic data field from a uint8 buffer into another uint8 vector type buffer
 * \details Performs a range check on the buffer argument with the specified index and size_field_bytes values. An error
 * is reported if the range is out of bounds, otherwise the data can be deserialized safely
 * \param[in,out] cxt TlsCommFlowContext used for tracing internal logic flow and error reporting
 * \param[in] buffer The uint8 vector which includes the data to be deserialized
 * \param[in] index The current index
 * \param[in] size_field_bytes size of the dynamic data size field in bytes
 * \param[in] last_dynamic8 true if the pointed data must be at the end of the buffer. otherwise, false
 * \return A copy of the deserialized field
 * \context ANY
 * \pre -
 * \reentrant FALSE
 * \error TlsCommErrorCode::kTlsErrDeserialize is reported if deserialization fails due to incomplete message
 * \trace amsr::tls::internal::messages::HandshakeMessage::Deserialize
 * \vprivate This method is component private.
 */
ara::core::Vector<std::uint8_t> DeserializeTlsDynamicField8(common::TlsCommFlowContext& cxt,
                                                            ara::core::Vector<uint8_t> const& buffer,
                                                            common::Index const index,
                                                            common::SizeFieldBytes const size_field_bytes,
                                                            bool const last_dynamic8) noexcept;

/*!
 * \brief Performs a boundary check for a given data field and verifies if the data field itself contains a complete
 * message.
 * \param[in,out] cxt TlsCommFlowContext used for tracing internal logic flow and error reporting
 * \param[in] buffer_size The size of the buffer
 * \param[in] index The current index to the buffer
 * \param[in] size_field_bytes The number of bytes making up the size value for the dynamic field
 * \param[in] field_size The size of the dynamic field
 * \param[in] max_field_size The maximum value of the size of the dynamic field
 * \param[in] max_check true if the pointed data must be at the end of the buffer. otherwise, false
 * \context ANY
 * \pre -
 * \reentrant FALSE
 * \error TlsCommErrorCode::kTlsErrDeserialize is reported if deserialization fails due to incomplete message or a too
 * large field size
 * \trace amsr::tls::internal::messages::HandshakeMessage::Deserialize
 * \vprivate This method is component private.
 */
void CheckFieldSizeOfSerializedBuffer(common::TlsCommFlowContext& cxt, common::BufferSize const buffer_size,
                                      common::Index const index, common::SizeFieldBytes const size_field_bytes,
                                      common::FieldSize const field_size, common::MaxFieldSize const max_field_size,
                                      bool const max_check) noexcept;

/*!
 * \brief Deserializes a dynamic data field from a uint8 buffer into another uint8 vector type buffer with maximum data
 * size check
 * \details Checks that the buffer contains the dynamic data size field and the according data field itself at the
 * buffer cursor, it also checks that the size of the data field is not more than the maximum param value, if checks
 * succeed it writes the data to the output_buffer. otherwise, it reports an error to the cxt
 * \param[in,out] cxt TlsCommFlowContext used for tracing internal logic flow and error reporting
 * \param[in] buffer The uint8 vector which includes the data to be deserialized
 * \param[in] index The curent index
 * \param[in] size_field_bytes size of the dynamic data size field in bytes
 * \param[in] max_field_size maximum size of data to be read
 * \param[in] last_field8_max_check true if the pointed data must be at the end of the buffer. otherwise, false
 * \return A copy of the deserialized field
 * \context ANY
 * \pre -
 * \reentrant FALSE
 * \error TlsCommErrorCode::kTlsErrDeserialize is reported if deserialization fails due to incomplete message
 * \trace amsr::tls::internal::messages::HandshakeMessage::Deserialize
 * \vprivate This method is component private.
 */
ara::core::Vector<std::uint8_t> DeserializeTlsDynamicField8WithMaxCheck(common::TlsCommFlowContext& cxt,
                                                                        ara::core::Vector<uint8_t> const& buffer,
                                                                        common::Index const index,
                                                                        common::SizeFieldBytes const size_field_bytes,
                                                                        common::MaxFieldSize const max_field_size,
                                                                        bool const last_field8_max_check) noexcept;

/*!
 * \brief Deserializes a dynamic data field from a uint8 buffer into uint16 vector type buffer with maximum data size
 * check
 * \details Checks that the buffer contains the dynamic data size field and the according data field itself at the
 * buffer cursor, it also checks that the size of the data field is not more than the maximum param value, if checks
 * succeed it writes the data to the output_buffer. otherwise, it reports an error to the cxt
 * \param[in,out] cxt TlsCommFlowContext used for tracing internal logic flow and error reporting
 * \param[in] buffer The uint8 vector which includes the data to be deserialized
 * \param[in] index The current index
 * \param[in] size_field_bytes size of the dynamic data size field in bytes
 * \param[in] max_field_size maximum size of data to be read
 * \param[in] last_field16_max_check true if the pointed data must be at the end of the buffer. otherwise, false
 * \return A copy of the deserialized field
 * \context ANY
 * \pre -
 * \reentrant FALSE
 * \error TlsCommErrorCode::kTlsErrDeserialize is reported if deserialization fails due to incomplete message
 * \trace amsr::tls::internal::messages::HandshakeMessage::Deserialize
 * \vprivate This method is component private.
 */
ara::core::Vector<std::uint16_t> DeserializeTlsDynamicField16WithMaxCheck(common::TlsCommFlowContext& cxt,
                                                                          ara::core::Vector<std::uint8_t> const& buffer,
                                                                          common::Index const index,
                                                                          common::SizeFieldBytes const size_field_bytes,
                                                                          common::MaxFieldSize const max_field_size,
                                                                          bool const last_field16_max_check) noexcept;

}  // namespace utility
}  // namespace messages
}  // namespace internal
}  // namespace tls
}  // namespace amsr

#endif  // LIB_LIBTLS_INCLUDE_TLS_INTERNAL_MESSAGES_MESSAGE_HELPER_FUNCTIONS_H_
