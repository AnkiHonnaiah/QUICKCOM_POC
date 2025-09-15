/*!********************************************************************************************************************
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
/*!        \file
 *         \brief  TLS provider interface class.
 *         \unit   SecCom::LibTLS::TlsProvider
 *
 *********************************************************************************************************************/
#ifndef LIB_TLS_PROVIDER_COMMON_INCLUDE_AMSR_TLS_PROVIDER_INTERFACE_H_
#define LIB_TLS_PROVIDER_COMMON_INCLUDE_AMSR_TLS_PROVIDER_INTERFACE_H_

#include <utility>

#include "amsr/tls/data_types.h"
#include "amsr/tls/scatter_io_buffers.h"
#include "ara/core/result.h"

namespace amsr {
namespace tls {

/*!
 * \vprivate Product private.
 * \brief Interface for a secure communication provider.
 */
class ProviderInterface {
 public:
  /*!
   * \brief Constructor.
   */
  ProviderInterface() noexcept = default;

  /*!
   * \brief Destructor.
   */
  virtual ~ProviderInterface() noexcept = default;

  ProviderInterface(ProviderInterface&&) noexcept = delete;
  ProviderInterface(ProviderInterface const&) = delete;
  ProviderInterface& operator=(ProviderInterface const&) = delete;
  ProviderInterface& operator=(ProviderInterface&&) = delete;

  /*!
   * \brief Start the connection procedure.
   * \param[in] output_buffer_view
   * \return \p ara::core::Result
   */
  virtual ara::core::Result<ara::core::Span<std::uint8_t>> Connect(
      ara::core::Span<std::uint8_t> output_buffer_view) noexcept = 0;

  /*!
   * \brief Disconnect.
   * \param[in] output_buffer_view
   * \return \p ara::core::Result
   */
  virtual ara::core::Result<ara::core::Span<std::uint8_t>> Disconnect(
      ara::core::Span<std::uint8_t> output_buffer_view) noexcept = 0;

  /*!
   * \brief Continue an ongoing handshake procedure.
   * \param[in] scatter_io_buffers
   * \return \p ara::core::Result
   */
  virtual ara::core::Result<std::pair<ScatterIOBuffers, ProviderState>> ContinueConnection(
      ScatterIOBuffers scatter_io_buffers) noexcept = 0;

  /*!
   * \brief Secure (encrypt) the provided data.
   * \param[in] scatter_io_buffers
   * \return \p ara::core::Result
   */
  virtual ara::core::Result<ScatterIOBuffers> Secure(ScatterIOBuffers scatter_io_buffers) noexcept = 0;

  /*!
   * \brief Verify (decrypt) the provided data.
   * \param[in] scatter_io_buffers
   * \return \p ara::core::Result
   */
  virtual ara::core::Result<std::pair<ScatterIOBuffers, ProviderState>> Verify(
      ScatterIOBuffers scatter_io_buffers) noexcept = 0;

  /*!
   * \brief Check if the provider is connected or not.
   * \return Status.
   */
  virtual bool IsConnected() const noexcept = 0;

  /*!
   * \brief Get the maximum record size.
   * \return Size.
   */
  virtual std::size_t GetMaximumRecordSize() const noexcept = 0;
};

}  // namespace tls
}  // namespace amsr

#endif  // LIB_TLS_PROVIDER_COMMON_INCLUDE_AMSR_TLS_PROVIDER_INTERFACE_H_
