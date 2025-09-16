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
/*!        \file  compression_suite_interface.h
 *        \brief  This file contains the base interface for compression suites
 *
 *********************************************************************************************************************/
#ifndef LIB_LIBTLS_INCLUDE_TLS_INTERNAL_RECORD_PROTOCOL_COMPRESSION_SUITES_COMPRESSION_SUITE_INTERFACE_H_
#define LIB_LIBTLS_INCLUDE_TLS_INTERNAL_RECORD_PROTOCOL_COMPRESSION_SUITES_COMPRESSION_SUITE_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "tls/internal/record_protocol/record_protocol_common_types.h"

namespace amsr {
namespace tls {
namespace internal {
namespace record_protocol {
namespace compression_suites {

/*!
 * \brief The interface for compression suites
 * \vprivate This class is component private.
 */
class CompressionSuiteInterface {
 public:
  /*! \brief Virtual destructor */
  virtual ~CompressionSuiteInterface() = default;
  /*! \brief Default constructor */
  CompressionSuiteInterface() = default;
  /*! \brief Not copy constructible. */
  CompressionSuiteInterface(CompressionSuiteInterface const &) = delete;
  /*! \brief Not move constructible. */
  CompressionSuiteInterface(CompressionSuiteInterface &&) = delete;
  /*! \brief Not copy assignable. */
  void operator=(CompressionSuiteInterface const &) = delete;
  /*! \brief Not move assignable. */
  void operator=(CompressionSuiteInterface &&) = delete;

  // VECTOR NC AutosarC++17_10-M7.1.2: MD_libseccom_M7.1.2_ParamCanBeDeclaredConst
  /*!
   * \brief Initialize the compression suite
   * \param[in, out] cxt TlsCommFlowContext used for error reporting
   * \param[in] sp Current security parameters
   * \pre -
   * \context InitPhase
   * \reentrant FALSE
   * \synchronous TRUE
   */
  virtual void Init(common::TlsCommFlowContext &cxt, SecurityParameters sp) const noexcept = 0;

  // VECTOR NC AutosarC++17_10-M7.1.2: MD_libseccom_M7.1.2_ParamCanBeDeclaredConst
  /*!
   * \brief Compresses the passed plain text argument and returns a unique pointer to the new compressed text
   * \param[in] pt Unique pointer to a PlainText object
   * \param[in] sp Current security parameters
   * \return A unique pointer to a new CompressedText or nullptr if pt is nullptr
   * \pre Init must be called once first otherwise this function may fail
   * \context ANY|!InitPhase
   * \reentrant FALSE
   * \synchronous TRUE
   * \trace SPEC-6493110
   */
  virtual CompressedTextUptr Compress(PlainTextUptr &&pt, SecurityParameters const &sp) const noexcept = 0;

  // VECTOR NC AutosarC++17_10-M7.1.2: MD_libseccom_M7.1.2_ParamCanBeDeclaredConst
  /*!
   * \brief Decompresses the passed compressed text and return a unique pointer to the new plain text
   * \param[in] ct The compressed text to be decompressed
   * \param[in] sp Current security parameters
   * \return A unique pointer to a new PlainText or nullptr if ct is nullptr
   * \pre Init must be called once first otherwise this function may fail
   * \context ANY|!InitPhase
   * \reentrant FALSE
   * \synchronous TRUE
   */
  virtual PlainTextUptr Decompress(CompressedTextUptr &&ct, SecurityParameters const &sp) const noexcept = 0;
};

}  // namespace compression_suites
}  // namespace record_protocol
}  // namespace internal
}  // namespace tls
}  // namespace amsr

#endif  // LIB_LIBTLS_INCLUDE_TLS_INTERNAL_RECORD_PROTOCOL_COMPRESSION_SUITES_COMPRESSION_SUITE_INTERFACE_H_
