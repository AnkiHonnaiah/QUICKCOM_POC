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
/*!        \file  record_protocol_framer.h
 *        \brief  Handles the framing of incoming Record Protocol data into records.
 *
 *********************************************************************************************************************/
#ifndef LIB_LIBTLS_INCLUDE_TLS_INTERNAL_RECORD_PROTOCOL_RECORD_PROTOCOL_FRAMER_H_
#define LIB_LIBTLS_INCLUDE_TLS_INTERNAL_RECORD_PROTOCOL_RECORD_PROTOCOL_FRAMER_H_

#include <cstdint>
#include "ara/core/vector.h"
#include "tls/internal/common/common_general_types.h"
#include "tls/internal/record_protocol/record_protocol_common_types.h"
#include "tls/public/tls_comm_general_types.h"

namespace amsr {
namespace tls {
namespace internal {
namespace record_protocol {

/*!
 * \brief Using a vector to represent a (D)TLS Record Protocol record.
 */
using Record = ara::core::Vector<uint8_t>;

/*!
 * \brief Sequentially extracts ((D)TLS Record Protocol) records from an incoming "stream" of bytes.
 * \details New incoming data is added through AddData(). Calling GetRecord() will then return the
 * records one by one.
 *  \vprivate This class is component private.
 */
class RecordProtocolFramer final {
 public:
  /*!
   * \brief Construct a RecordFramer object.
   * \param[in] header_size The size of the (D)TLS record header size.
   */
  explicit RecordProtocolFramer(uint8_t header_size) : header_size_(header_size) {}

  /*! \brief Destroy the RecordProtocolFramer object */
  ~RecordProtocolFramer() = default;

  /*! \brief Not copy constructible. */
  RecordProtocolFramer(RecordProtocolFramer const &) = delete;

  /*! \brief Not move constructible. */
  RecordProtocolFramer(RecordProtocolFramer &&) = delete;

  /*! \brief Not copy assignable.  */
  RecordProtocolFramer &operator=(RecordProtocolFramer const &) = delete;

  /*! \brief Not move assignable. */
  RecordProtocolFramer &operator=(RecordProtocolFramer &&) = delete;

  /*!
   * \brief Adds incoming data to this RecordProtocolFramer.
   * \param[in] data Incoming (D)TLS data.
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \vprivate This method is component private.
   */
  void AddData(ReadOnlyBufferView data) noexcept;

  /*!
   * \brief Gets the next complete record from this RecordProtocolFramer. Returns false if none is available.
   * \param[out] record The record read.
   * \return True if a record was extracted. False if no complete record is available.
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \vprivate This method is component private.
   */
  bool GetRecord(Record &record) noexcept;

 private:
  /*!
   * \brief Construct a RecordFramer object. Force the user to use the explicit constructor.
   */
  RecordProtocolFramer() = default;

  /*!
   * \brief Indicates if one or more complete records are available to be read.
   * \return true if there is at least one complete record that can be read using GetRecord().
   * \context ANY
   * \pre -
   * \reentrant FALSE
   */
  bool IsRecordAvailable() noexcept;

  /*!
   * \brief Calculates the expected length of the (D)TLS Record Protocol record payload. (That is the part of the
   * record following the header).
   * \details Calculation is done by looking into the size field in the (D)TLS header
   * \return The expected length of the payload following the header
   * \context ANY
   * \pre The internal buffer must contain a complete (D)TLS header. If not values outside the buffer will be read.
   * \reentrant FALSE
   */
  uint16_t GetExpectedLengthOfPayload() noexcept;

  /*!
   * \brief Type used for internal buffer
   */
  using buffer_type = ara::core::Vector<uint8_t>;

  /*!
   * \brief Internal buffer for the data to process. The buffer will be filled by AddData() and emptied by GetRecord()
   */
  buffer_type buffer_{};
  /*!
   * \brief The length of the current (D)TLS record when the record protocol header has been subtracted.
   */
  buffer_type::size_type payload_length_{0U};
  /*!
   * \brief Size of the (D)TLS header..
   */
  buffer_type::size_type const header_size_{0U};
};

}  // namespace record_protocol
}  // namespace internal
}  // namespace tls
}  // namespace amsr

#endif  // LIB_LIBTLS_INCLUDE_TLS_INTERNAL_RECORD_PROTOCOL_RECORD_PROTOCOL_FRAMER_H_
