/**********************************************************************************************************************
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
/*!        \file  null_compression.h
 *        \brief  This file contains the class declaration of the null compression suite, NullCompression
 *********************************************************************************************************************/
#ifndef LIB_LIBTLS_INCLUDE_TLS_INTERNAL_RECORD_PROTOCOL_COMPRESSION_SUITES_NULL_COMPRESSION_H_
#define LIB_LIBTLS_INCLUDE_TLS_INTERNAL_RECORD_PROTOCOL_COMPRESSION_SUITES_NULL_COMPRESSION_H_

#include "tls/internal/record_protocol/compression_suites/compression_suite_interface.h"
#include "tls/internal/record_protocol/record_protocol_common_types.h"

namespace amsr {
namespace tls {
namespace internal {
namespace record_protocol {
namespace compression_suites {

/*!
 * \brief Null Compression Suite.
 * \vprivate This class is component private.
 */
class NullCompression final : public CompressionSuiteInterface {
 public:
  /*! \brief Default constructor. */
  NullCompression() = default;

  NullCompression(NullCompression const&) = delete;

  NullCompression(NullCompression&&) = delete;

  /*! \brief Default destructor. */
  ~NullCompression() final = default;

  NullCompression& operator=(NullCompression const&) = delete;

  NullCompression& operator=(NullCompression&&) = delete;

  // VECTOR NC AutosarC++17_10-M7.1.2: MD_libseccom_M7.1.2_ParamCanBeDeclaredConst
  /*!
   * \copydoc ::amsr::tls::internal::record_protocol::compression_suites::CompressionSuiteInterface::Init
   * \note Calling this method on this compression suite has no internal side effect and the TlsCommFlowContext argument
   *       is ignored.
   */
  void Init(common::TlsCommFlowContext& cxt, SecurityParameters sp) const noexcept final;
  // VECTOR NC AutosarC++17_10-M7.1.2: MD_libseccom_M7.1.2_ParamCanBeDeclaredConst
  /*!
   * \copydoc ::amsr::tls::internal::record_protocol::compression_suites::CompressionSuiteInterface::Compress
   * \vprivate This method is component private.
   */
  CompressedTextUptr Compress(PlainTextUptr&& pt, SecurityParameters const& sp) const noexcept final;
  // VECTOR NC AutosarC++17_10-M7.1.2: MD_libseccom_M7.1.2_ParamCanBeDeclaredConst
  /*!
   * \copydoc ::amsr::tls::internal::record_protocol::compression_suites::CompressionSuiteInterface::Decompress
   * \vprivate This method is component private.
   */
  PlainTextUptr Decompress(CompressedTextUptr&& ct, SecurityParameters const& sp) const noexcept final;
};

}  // namespace compression_suites
}  // namespace record_protocol
}  // namespace internal
}  // namespace tls
}  // namespace amsr

#endif  // LIB_LIBTLS_INCLUDE_TLS_INTERNAL_RECORD_PROTOCOL_COMPRESSION_SUITES_NULL_COMPRESSION_H_
