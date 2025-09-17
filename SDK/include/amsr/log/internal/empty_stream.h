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
/**     \file
 *      \brief      Contains an empty output stream.
 *      \details
 *
 *      \unit       LogAndTrace::LogSinks::EmptyStream
 *
 *********************************************************************************************************************/
#ifndef LIB_AMSR_LOG_INCLUDE_AMSR_LOG_INTERNAL_EMPTY_STREAM_H_
#define LIB_AMSR_LOG_INCLUDE_AMSR_LOG_INTERNAL_EMPTY_STREAM_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/core/abort.h"
#include "amsr/core/span.h"
#include "amsr/iostream/output_stream.h"
#include "amsr/log/internal/log_error_domain.h"
#include "vac/container/c_string_view.h"

namespace amsr {
namespace log {
namespace internal {
/*!
 * \brief A stream that does not implement any operations.
 *
 * \details Aborts on any called function.
 */
// VECTOR NC Metric-OO.WMC.One: MD_LOG_Metric-OO.WMC.One_external_API
// VECTOR Disable AutosarC++17_10-M5.2.12: MD_LOG_AutosarC++17_10-M5.2.12
// VCA_LOGAPI_MOLE_1298
class EmptyStream : public stream::OutputStream {
 public:
  /*! \copydoc amsr::stream::OutputStream::Tell() */
  [[noreturn]] auto Tell() const noexcept -> Result<std::uint64_t> override { core::Abort(__func__); }

  /*! \copydoc amsr::stream::OutputStream::Seek(std::int64_t, stream::SeekDirection) */
  [[noreturn]] auto Seek(std::int64_t, stream::SeekDirection) noexcept -> Result<std::uint64_t> override {
    core::Abort(__func__);
  }

  /*! \copydoc amsr::stream::OutputStream::Write(char) */
  [[noreturn]] auto Write(char) noexcept -> Result<std::int64_t> override { core::Abort(__func__); }

  /*! \copydoc amsr::stream::OutputStream::Write(bool const&) */
  [[noreturn]] auto Write(bool const&) noexcept -> Result<std::int64_t> override { core::Abort(__func__); }

  /*! \copydoc amsr::stream::OutputStream::Write(std::int8_t const&) */
  [[noreturn]] auto Write(std::int8_t const&) noexcept -> Result<std::int64_t> override { core::Abort(__func__); }

  /*! \copydoc amsr::stream::OutputStream::Write(std::int8_t const&, amsr::stream::Base) */
  [[noreturn]] auto Write(std::int8_t const&, stream::Base) noexcept -> Result<std::int64_t> override {
    core::Abort(__func__);
  }

  /*! \copydoc amsr::stream::OutputStream::Write(std::int16_t const&) */
  [[noreturn]] auto Write(std::int16_t const&) noexcept -> Result<std::int64_t> override { core::Abort(__func__); }

  /*! \copydoc amsr::stream::OutputStream::Write(std::int16_t const&, amsr::stream::Base) */
  [[noreturn]] auto Write(std::int16_t const&, stream::Base) noexcept -> Result<std::int64_t> override {
    core::Abort(__func__);
  }

  /*! \copydoc amsr::stream::OutputStream::Write(std::int32_t const&) */
  [[noreturn]] auto Write(std::int32_t const&) noexcept -> Result<std::int64_t> override { core::Abort(__func__); }

  /*! \copydoc amsr::stream::OutputStream::Write(std::int32_t const&, amsr::stream::Base) */
  [[noreturn]] auto Write(std::int32_t const&, stream::Base) noexcept -> Result<std::int64_t> override {
    core::Abort(__func__);
  }

  /*! \copydoc amsr::stream::OutputStream::Write(std::int64_t const&) */
  [[noreturn]] auto Write(std::int64_t const&) noexcept -> Result<std::int64_t> override { core::Abort(__func__); }

  /*! \copydoc amsr::stream::OutputStream::Write(std::int64_t const&, amsr::stream::Base) */
  [[noreturn]] auto Write(std::int64_t const&, stream::Base) noexcept -> Result<std::int64_t> override {
    core::Abort(__func__);
  }

  /*! \copydoc amsr::stream::OutputStream::Write(std::uint8_t const&) */
  [[noreturn]] auto Write(std::uint8_t const&) noexcept -> Result<std::int64_t> override { core::Abort(__func__); }

  /*! \copydoc amsr::stream::OutputStream::Write(std::uint8_t const&, amsr::stream::Base) */
  [[noreturn]] auto Write(std::uint8_t const&, stream::Base) noexcept -> Result<std::int64_t> override {
    core::Abort(__func__);
  }

  /*! \copydoc amsr::stream::OutputStream::Write(std::uint16_t const&) */
  [[noreturn]] auto Write(std::uint16_t const&) noexcept -> Result<std::int64_t> override { core::Abort(__func__); }

  /*! \copydoc amsr::stream::OutputStream::Write(std::uint16_t const&, amsr::stream::Base) */
  [[noreturn]] auto Write(std::uint16_t const&, stream::Base) noexcept -> Result<std::int64_t> override {
    core::Abort(__func__);
  }

  /*! \copydoc amsr::stream::OutputStream::Write(std::uint32_t const&) */
  [[noreturn]] auto Write(std::uint32_t const&) noexcept -> Result<std::int64_t> override { core::Abort(__func__); }

  /*! \copydoc amsr::stream::OutputStream::Write(std::uint32_t const&, amsr::stream::Base) */
  [[noreturn]] auto Write(std::uint32_t const&, stream::Base) noexcept -> Result<std::int64_t> override {
    core::Abort(__func__);
  }

  /*! \copydoc amsr::stream::OutputStream::Write(std::uint64_t const&) */
  [[noreturn]] auto Write(std::uint64_t const&) noexcept -> Result<std::int64_t> override { core::Abort(__func__); }

  /*! \copydoc amsr::stream::OutputStream::Write(std::uint64_t const&, amsr::stream::Base) */
  [[noreturn]] auto Write(std::uint64_t const&, stream::Base) noexcept -> Result<std::int64_t> override {
    core::Abort(__func__);
  }

  /*! \copydoc amsr::stream::OutputStream::Write(float const&) */
  [[noreturn]] auto Write(float const&) noexcept -> Result<std::int64_t> override { core::Abort(__func__); }

  /*! \copydoc amsr::stream::OutputStream::Write(double const&) */
  [[noreturn]] auto Write(double const&) noexcept -> Result<std::int64_t> override { core::Abort(__func__); }

  /*! \copydoc amsr::stream::OutputStream::Write(core::StringView) */
  [[noreturn]] auto Write(core::StringView) noexcept -> Result<std::int64_t> override { core::Abort(__func__); }

  /*! \copydoc amsr::stream::OutputStream::Write(vac::container::CStringView) */
  [[noreturn]] auto Write(vac::container::CStringView) noexcept -> Result<std::int64_t> override {
    core::Abort(__func__);
  }

  /*! \copydoc amsr::stream::OutputStream::Write(core::Span<char const>) */
  [[noreturn]] auto Write(core::Span<char const>) noexcept -> Result<std::int64_t> override { core::Abort(__func__); }

  /*! \copydoc amsr::stream::OutputStream::Write(core::Span<vac::language::byte const>) */
  [[noreturn]] auto Write(core::Span<vac::language::byte const>) noexcept -> Result<std::int64_t> override {
    core::Abort(__func__);
  }

  /*! \copydoc amsr::stream::OutputStream::Write(std::uint64_t,
   vac::language::FunctionRef<std::uint64_t(core::Span<char>)> */
  [[noreturn]] auto Write(std::uint64_t, vac::language::FunctionRef<std::uint64_t(core::Span<char>)>) noexcept
      -> Result<std::uint64_t> override {
    core::Abort(__func__);
  }

  /*! \copydoc amsr::stream::OutputStream::Write(vac::language::FunctionRef<std::uint64_t(core::Span<char>)> */
  [[noreturn]] auto Write(vac::language::FunctionRef<std::uint64_t(core::Span<char>)>) noexcept
      -> Result<std::uint64_t> override {
    core::Abort(__func__);
  }

  /*! \copydoc amsr::stream::OutputStream::Write(core::Span<std::uint8_t>, stream::Base)
   */
  [[noreturn]] auto Write(core::Span<std::uint8_t const>, stream::Base) noexcept -> Result<std::int64_t> override {
    core::Abort(__func__);
  }

  /*! \copydoc amsr::stream::OutputStream::Put(char) */
  [[noreturn]] auto Put(char) noexcept -> Result<void> override { core::Abort(__func__); }

  /*! \copydoc amsr::stream::OutputStream::Sync() */
  [[noreturn]] auto Sync() noexcept -> Result<void> override { core::Abort(__func__); }

  /*! \copydoc amsr::stream::OutputStream::Flush() */
  [[noreturn]] auto Flush() noexcept -> Result<void> override { core::Abort(__func__); }

  /*! \copydoc amsr::stream::OutputStream::GetRemainingBufferSize() */
  [[noreturn]] auto GetRemainingBufferSize() const noexcept -> core::Span<char>::index_type override {
    core::Abort(__func__);
  }

  /*! \copydoc amsr::stream::OutputStream::IsOpen() */
  [[noreturn]] auto IsOpen() const noexcept -> bool override { core::Abort(__func__); }

  // VECTOR NC AutosarC++17_10-A0.1.3: MD_LOG_AutosarC++17_10-A0.1.3_external_API
  /*!
   * \brief Forbidden char const* overload for Write API. Use StringView version instead.
   * \spec
   *   requires true;
   * \endspec
   */
  [[noreturn]] auto Write(char const*) noexcept -> Result<std::int64_t> final {
    core::Abort("EmptyStream::Write: char const* Overload is not to be used.");
  }
};
// VECTOR Enable AutosarC++17_10-M5.2.12
}  // namespace internal
}  // namespace log
}  // namespace amsr

#endif  // LIB_AMSR_LOG_INCLUDE_AMSR_LOG_INTERNAL_EMPTY_STREAM_H_
