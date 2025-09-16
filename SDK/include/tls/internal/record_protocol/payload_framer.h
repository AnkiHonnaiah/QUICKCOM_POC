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
/*!        \file  payload_framer.h
 *        \brief  Payload data to extract
 *
 *********************************************************************************************************************/
#ifndef LIB_LIBTLS_INCLUDE_TLS_INTERNAL_RECORD_PROTOCOL_PAYLOAD_FRAMER_H_
#define LIB_LIBTLS_INCLUDE_TLS_INTERNAL_RECORD_PROTOCOL_PAYLOAD_FRAMER_H_

/*********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "tls/internal/record_protocol/record_protocol_common_types.h"
#include "tls/public/tls_comm_general_types.h"

namespace amsr {
namespace tls {
namespace internal {
namespace record_protocol {

/*!
 * \brief Extracts a payload considering overhead from a supplied source buffer.
 */
class PayloadFramer final {
 public:
  /*!
   * \brief Default constructor
   */
  PayloadFramer() noexcept = default;

  /*! \brief Not copy constructible */
  PayloadFramer(PayloadFramer const&) = delete;
  /*! \brief Not move constructible */
  PayloadFramer(PayloadFramer&&) = delete;
  /*! \brief Not copy assignable */
  PayloadFramer& operator=(PayloadFramer const&) = delete;
  /*! \brief Not move assignable */
  PayloadFramer& operator=(PayloadFramer&&) = delete;

  /*! \brief Default destructor */
  ~PayloadFramer() noexcept = default;

  /*!
   * \brief Sets the data this PayloadFramer should use as source. Also sets the overhead (see documentation of
   * SendFragmenter::SetDataAndRecordSize) and the wanted size of the output record.
   * \param[in] data A view of the source data that this PayloadFramer should use to create the Payload objects when
   * GetNextPayload() is called.
   * \param[in] record_size The size limit of the plaintext without taking record expansion into account.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \synchronous TRUE
   */
  void SetDataAndRecordSize(ReadOnlyBufferView const data, std::size_t const record_size) noexcept;

  /*!
   * \brief Sets the data this PayloadFramer should use as source.
   * \param[in] data A view of the source data that this PayloadFramer should use to create the Payload objects when
   * GetNextPayload() is called.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \synchronous TRUE
   */
  void SetData(ReadOnlyBufferView const data) noexcept;

  /*!
   * \brief Returns a Payload object from the source data supplied in the SetDataAndRecordSize() call.
   * \return The next Payload object from the source buffer.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \synchronous TRUE
   */
  Payload GetNextPayload() noexcept;

 private:
  /*! \brief A view of the source data used to construct the Payload objects*/
  ReadOnlyBufferView data_{};

  /*! \brief The size of the fragment (Payload) */
  std::size_t fragment_size_{0U};

  /*!
   * \brief An index of which Payload is the current one. Updated in each call of GetNextPayload() and reset in
   * SetDataAndRecordSize()
   */
  std::size_t index_{0U};
};

}  // namespace record_protocol
}  // namespace internal
}  // namespace tls
}  // namespace amsr

#endif  // LIB_LIBTLS_INCLUDE_TLS_INTERNAL_RECORD_PROTOCOL_PAYLOAD_FRAMER_H_
