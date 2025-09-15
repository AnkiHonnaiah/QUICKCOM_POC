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
/*!        \file  record_protocol_deserializer.h
 *        \brief  This file contains the class declaration of the RecordProtocolDeserializer
 *
 *********************************************************************************************************************/
#ifndef LIB_LIBTLS_INCLUDE_TLS_INTERNAL_RECORD_PROTOCOL_RECORD_PROTOCOL_DESERIALIZER_H_
#define LIB_LIBTLS_INCLUDE_TLS_INTERNAL_RECORD_PROTOCOL_RECORD_PROTOCOL_DESERIALIZER_H_

#include <memory>
#include <utility>
#include "tls/internal/record_protocol/record_protocol_common_types.h"
#include "tls/internal/record_protocol/record_protocol_containers.h"
#include "tls/internal/record_protocol/record_protocol_validator.h"

namespace amsr {
namespace tls {
namespace internal {
namespace record_protocol {

/*! \brief Using a std::pair with ReturnTypeValidator and CipherTextUptr as return type for DeserializeAndValidate */
using ReturnTypeDeserializer = std::pair<ReturnTypeValidator, CipherTextUptr>;

/*!
 * \brief The RecordProtocolDeserializer takes a raw bytestream containing one Record and deserializes it to a
 * CipherTextTls or CipherTextDtls. The deserializer validates the CipherText object before returning it.
 * \vprivate This class is component private.
 */
class RecordProtocolDeserializer final {
 public:
  /*! \brief Default constructor */
  RecordProtocolDeserializer() = default;
  /*! \brief Destroy the object */
  ~RecordProtocolDeserializer() = default;
  /*! \brief Not copy constructible. */
  RecordProtocolDeserializer(RecordProtocolDeserializer const &) = delete;
  /*! \brief Not move constructible. */
  RecordProtocolDeserializer(RecordProtocolDeserializer &&) = delete;
  /*! \brief Not copy assignable.  */
  RecordProtocolDeserializer &operator=(RecordProtocolDeserializer const &) = delete;
  /*! \brief Not move assignable. */
  RecordProtocolDeserializer &operator=(RecordProtocolDeserializer &&) = delete;
  /*!
   * \brief Deserialize a TLS record into a CipherText and validate it
   * \details The deserialized CipherText is validated before it is returned, you must check the result of the
   * validation prior to using the CipherText
   * \param[in, out] record A reference to a record to be deserialized
   * \param[in] rpContext A const reference to the current RecordProtocolContext
   * \return A pair containing the result of the Validator and the deserialized CipherText
   * \context RecordProtocol
   * \pre -
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate This method is component private.
   */
  static ReturnTypeDeserializer DeserializeAndValidateTLS(Record const &record,
                                                          RecordProtocolContext const &rpContext) noexcept;
  /*!
   * \brief Deserialize a DTLS record into a CipherText and validate it
   * \details The deserialized CipherText is validated before it is returned, you must check the result of the
   * validation prior to using the CipherText
   * \param[in, out] record A reference to a record to be deserialized
   * \param[in] rpContext A const reference to the current RecordProtocolContext
   * \param[in, out] received_epoch Received epoch in record
   * \return A pair containing the result of the Validator and the deserialized CipherText
   * \context RecordProtocol
   * \pre -
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate This method is component private.
   */
  static ReturnTypeDeserializer DeserializeAndValidateDTLS(Record const &record, RecordProtocolContext const &rpContext,
                                                           std::uint16_t &received_epoch) noexcept;

  /*!
   * \brief Deserializes the passed Record to a CipherTextDtls
   * \details During deserialization payload data from the Record is moved to the new CipherTextDtls
   * \param[in, out] record A reference to the Record
   * \return A CipherTextDtls instance wrapped by a std::unique_ptr
   * \context RecordProtocol
   * \pre -
   * \reentrant FALSE
   * \synchronous TRUE
   * \trace SPEC-6562097
   */
  static std::unique_ptr<CipherTextDtls> DeserializeToCipherTextDtls(Record const &record) noexcept;

 private:
  /*!
   * \brief Deserializes the passed Record to a CipherTextTls
   * \details During deserialization payload data from the Record is moved to the new CipherTextTls
   * \param[in, out] record A reference to the Record
   * \param[in] rpContext A const reference to the current RecordProtocolContext
   * \return A CipherTextTls instance wrapped by a std::unique_ptr
   * \context RecordProtocol
   * \pre -
   * \reentrant FALSE
   * \synchronous TRUE
   * \trace SPEC-6493112
   */
  static std::unique_ptr<CipherTextTls> DeserializeToCipherTextTls(Record const &record,
                                                                   RecordProtocolContext const &rpContext) noexcept;
};

}  // namespace record_protocol
}  // namespace internal
}  // namespace tls
}  // namespace amsr

#endif  // LIB_LIBTLS_INCLUDE_TLS_INTERNAL_RECORD_PROTOCOL_RECORD_PROTOCOL_DESERIALIZER_H_
