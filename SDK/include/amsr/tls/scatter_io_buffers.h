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
 *         \brief  Scatter IO buffer class with an input scatter buffer and one output buffer.
 *         \unit   SecCom::LibTLS::TlsProvider
 *
 *********************************************************************************************************************/
#ifndef LIB_TLS_PROVIDER_COMMON_INCLUDE_AMSR_TLS_SCATTER_IO_BUFFERS_H_
#define LIB_TLS_PROVIDER_COMMON_INCLUDE_AMSR_TLS_SCATTER_IO_BUFFERS_H_

#include <cstdint>

#include "ara/core/span.h"

namespace amsr {
namespace tls {

/*!
 * \vprivate Product private.
 * \brief Scatter IO buffer class with an input scatter buffer and one output buffer.
 * \details This type does not support creating a contiguous buffer from two consecutive
 *          scatter buffers when the requested size allows that. The limitation is due
 *          to the decision that no memory allocation shall be done by this type.
 */
class ScatterIOBuffers final {
 public:
  /*!
   * \brief Constructor.
   * \param[in] in_scatter_buffer Input scatter of buffers to read from.
   * \param[out] out_buffer Output buffer to write to.
   * \context App
   * \pre -
   * \reentrant FALSE
   * \threadsafe FALSE
   */
  ScatterIOBuffers(ara::core::Span<ara::core::Span<std::uint8_t const>> in_scatter_buffer,
                   ara::core::Span<std::uint8_t> out_buffer) noexcept;

  /*!
   * \brief Destructor.
   * \context App
   * \pre -
   * \reentrant FALSE
   * \threadsafe FALSE
   */
  ~ScatterIOBuffers() noexcept = default;

  /*!
   * \brief Move constructor.
   * \details Needed for compatibility with \p ara::core::Result.
   * \context App
   * \pre -
   * \reentrant FALSE
   * \threadsafe FALSE
   */
  ScatterIOBuffers(ScatterIOBuffers&&) noexcept = default;

  /*!
   * \brief Copy constructor.
   * \context App
   * \pre -
   * \reentrant FALSE
   * \threadsafe FALSE
   */
  ScatterIOBuffers(ScatterIOBuffers const&) noexcept = default;

  /*!
   * \brief Copy assignment operator.
   * \return A reference to the type.
   * \context App
   * \pre -
   * \reentrant FALSE
   * \threadsafe FALSE
   */
  ScatterIOBuffers& operator=(ScatterIOBuffers const&) & noexcept = default;

  /*!
   * \brief Move assignment operator.
   * \return A reference to the type.
   * \context App
   * \pre -
   * \reentrant FALSE
   * \threadsafe FALSE
   */
  ScatterIOBuffers& operator=(ScatterIOBuffers&&) & noexcept = default;

  /*!
   * \brief Get the input scatter buffer's view and update the read size of the scatter.
   * \details The read size of the input scatter of buffers is incremented automatically
   *          (bound by the size of the scatter).
   * \param[in] size_bytes Size of the view in bytes.
   * \return A view of the input buffer.
   * \context App
   * \pre -
   * \reentrant FALSE
   * \threadsafe FALSE
   */
  ara::core::Span<std::uint8_t const> GetInputBuffer(std::size_t const size_bytes) noexcept;

  /*!
   * \brief Get the output buffer's view and update the write size.
   * \details The write size of the output buffer is incremented automatically
   *          (bound by the size of the buffer).
   * \param[in] size_bytes Size of the view in bytes.
   * \return A view of the output buffer.
   * \context App
   * \pre -
   * \reentrant FALSE
   * \threadsafe FALSE
   */
  ara::core::Span<std::uint8_t> GetOutputBuffer(std::size_t const size_bytes) noexcept;

  /*!
   * \brief Set the output buffer's view and reset the write size to zero.
   * \details It does not affect read size of the input scatter of buffers.
   * \param[in] buffer_view New view of the buffer.
   * \context App
   * \pre -
   * \reentrant FALSE
   * \threadsafe FALSE
   */
  void SetOutputBuffer(ara::core::Span<std::uint8_t> const buffer_view) noexcept;

  /*!
   * \brief Get the output buffer's view to the point it has been written to.
   * \details It does not affect read size of the input scatter of buffers and write size of the output buffer.
   * \return A view of the output buffer.
   * \context App
   * \pre -
   * \reentrant FALSE
   * \threadsafe FALSE
   */
  ara::core::Span<std::uint8_t> GetWrittenOutputBuffer() const noexcept;

  /*!
   * \brief Get the output buffer's view for the non-written part.
   * \details It does not affect read size of the input scatter of buffers and write size of the output buffer.
   * \return A view of the output buffer.
   * \context App
   * \pre -
   * \reentrant FALSE
   * \threadsafe FALSE
   */
  ara::core::Span<std::uint8_t> GetUnwrittenOutputBuffer() noexcept;

  /*!
   * \brief Get size of written part of the output buffer.
   * \return Size of written part of the output buffer.
   * \context App
   * \pre -
   * \reentrant FALSE
   * \threadsafe FALSE
   */
  std::size_t GetOutputBufferWriteSize() const noexcept;

  /*!
   * \brief Set size of written part of the output buffer.
   * \param[in] new_size New size.
   * \return "true" if the size is updated and "false" otherwise.
   * \context App
   * \pre -
   * \reentrant FALSE
   * \threadsafe FALSE
   */
  bool SetOutputBufferWriteSize(std::size_t const new_size) noexcept;

  /*!
   * \brief Get the status of the emptiness of the input scatter buffer.
   * \return  "true" if all the buffers in the input scatter have been consumed completely (read from)
   *          and "false" otherwise.
   * \context App
   * \pre -
   * \reentrant FALSE
   * \threadsafe FALSE
   */
  bool IsInputBufferEmpty() const noexcept;

  /*!
   * \brief Get the status of the fullness of the output buffer.
   * \return "true" if the output buffer has been consumed completely (written to) and "false" otherwise.
   * \context App
   * \pre -
   * \reentrant FALSE
   * \threadsafe FALSE
   */
  bool IsOutputBufferFull() const noexcept;

 private:
  /*!
   * \brief Input scatter buffer.
   */
  ara::core::Span<ara::core::Span<std::uint8_t const>> in_scatter_buffer_{};

  /*!
   * \brief Output buffer.
   */
  ara::core::Span<std::uint8_t> out_buffer_{};

  /*!
   * \brief Index of the input buffer to use from the scatter.
   */
  std::size_t in_buffer_span_index_{0U};

  /*!
   * \brief Amount of data that has been read from the current input buffer in the scatter.
   */
  std::size_t size_input_cur_buff_read_{0U};

  /*!
   * \brief Amount of data that has been written to the output buffer.
   */
  std::size_t size_output_buff_written_{0U};
};

}  // namespace tls
}  // namespace amsr

#endif  // LIB_TLS_PROVIDER_COMMON_INCLUDE_AMSR_TLS_SCATTER_IO_BUFFERS_H_
