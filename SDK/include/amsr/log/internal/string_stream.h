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
 *      \brief      Contains a growable string stream.
 *      \details
 *
 *      \unit       LogAndTrace::LogSinks::StringStream
 *
 *********************************************************************************************************************/
#ifndef LIB_AMSR_LOG_INCLUDE_AMSR_LOG_INTERNAL_STRING_STREAM_H_
#define LIB_AMSR_LOG_INCLUDE_AMSR_LOG_INTERNAL_STRING_STREAM_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <algorithm>
#include <cassert>
#include <limits>
#include <vector>

#include "amsr/core/vector.h"
#include "amsr/iostream/stringstream/output_string_stream.h"
#include "amsr/iostream/types.h"
#include "amsr/log/internal/abort.h"
#include "amsr/log/internal/empty_stream.h"
#include "ara/core/vector.h"
#include "vac/container/c_string_view.h"

namespace amsr {
namespace log {
namespace internal {
/*! \brief A string stream that is able to grow once the buffer is too small. */
// VECTOR NC Metric-OO.WMC.One: MD_LOG_Metric-OO.WMC.One_external_API
// VECTOR NC Compiler-#611: MD_LOG_Compiler-#611_no_total_override_needed_for_impl
// VCA_LOGAPI_MOLE_1298
class StringStream final : public EmptyStream {
 private:
  /*! \brief The underlying growable storage. */
  std::vector<char> storage_;
  /*! \brief The stream responsible for writing. */
  ::amsr::stream::stringstream::OutputStringStream ss_;

 public:
  /*!
   * \brief Constructs a new String Stream object
   *
   * \param capacity The initial capacity of the underlying storage.
   *
   * \pre         -
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \context     ANY
   * \spec
   *   requires true;
   * \endspec
   */
  // VECTOR NC AutosarC++17_10-A7.2.1: MD_LOG_A7.2.1_faulty_enum_assignment_false_positive
  explicit StringStream(std::size_t capacity) noexcept
      : EmptyStream(),
        storage_(std::min(static_cast<std::size_t>(64), capacity), '\0'),
        ss_{core::Span<char>{this->Data(), this->Capacity()}} {}

  /*!
   * \brief Clears the stream
   * \note Does not clear the underlying buffer which could leak information!
   *
   * \pre         -
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \context     ANY
   * \spec
   *   requires true;
   * \endspec
   */
  void Clear() noexcept { this->ReseatStream(); }

  /*!
   * \brief Returns a view into the currently stored buffer.
   *
   * \return  A view into the currently stored buffer.
   *
   * \pre         -
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \context     ANY
   * \spec
   *   requires true;
   * \endspec
   */
  auto AsStringView() const noexcept -> core::StringView { return this->ss_.AsStringView(); }

  /*!
   * \brief Returns a nullterminated view into the currently stored buffer.
   *
   * \return  A nullterminated view into the currently stored buffer.
   *
   * \pre         -
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \context     ANY
   * \spec
   *   requires true;
   * \endspec
   */
  auto GetAsCString() noexcept -> vac::container::CStringView {
    std::size_t const pos{this->Size()};
    // VCA_LOGAPI_VALID_OBJECT
    this->storage_[pos] = '\0';
    return vac::container::CStringView::FromLiteral(this->Data(), pos);
  }

  /*! \copydoc amsr::stream::OutputStream::Write(core::StringView) */
  auto Write(core::StringView data) noexcept -> Result<std::int64_t> final {
    this->GrowIfNeeded(data.size());
    return this->ss_.Write(data);
  }

  /*! \copydoc amsr::stream::OutputStream::Write(vac::container::CStringView) */
  auto Write(vac::container::CStringView data) noexcept -> Result<std::int64_t> final {
    this->GrowIfNeeded(data.size());
    // VCA_LOGAPI_CHECKED_WRITES
    return this->ss_.Write(data);
  }

  /*! \copydoc amsr::stream::OutputStream::Write(core::Span<char const>) */
  auto Write(core::Span<char const> data) noexcept -> Result<std::int64_t> final {
    this->GrowIfNeeded(data.size());
    return this->ss_.Write(data);
  }

  /*! \copydoc amsr::stream::OutputStream::Write(bool const&) */
  auto Write(bool const& data) noexcept -> Result<std::int64_t> final {
    return Repeater([this, data]() { return this->ss_.Write(data); });
  }

  /*! \copydoc amsr::stream::OutputStream::Write(std::int32_t const&) */
  auto Write(std::int32_t const& data) noexcept -> Result<std::int64_t> final {
    return Repeater([this, data]() { return this->ss_.Write(data); });
  }

  /*! \copydoc amsr::stream::OutputStream::Write(std::uint64_t const&) */
  auto Write(std::uint64_t const& data) noexcept -> Result<std::int64_t> final {
    return Repeater([this, data]() { return this->ss_.Write(data); });
  }

  /*! \copydoc amsr::stream::OutputStream::Write(std::uint64_t const&, stream::Base) */
  auto Write(std::uint64_t const& data, stream::Base base) noexcept -> Result<std::int64_t> final {
    return Repeater([this, data, base]() {
      // VECTOR NC AutosarC++17_10-A7.2.1: MD_LOG_AutosarC++17_10-A7.2.1_FalsePositive
      return this->ss_.Write(data, base);
    });
  }

  /*! \copydoc amsr::stream::OutputStream::Write(std::int64_t const&) */
  auto Write(std::int64_t const& data) noexcept -> Result<std::int64_t> final {
    return Repeater([this, data]() { return this->ss_.Write(data); });
  }

  /*! \copydoc amsr::stream::OutputStream::Write(float const&) */
  auto Write(float const& data) noexcept -> Result<std::int64_t> final {
    return Repeater([this, data]() { return this->ss_.Write(data); });
  }

  /*! \copydoc amsr::stream::OutputStream::Write(double const&) */
  auto Write(double const& data) noexcept -> Result<std::int64_t> final {
    return Repeater([this, data]() { return this->ss_.Write(data); });
  }

  /*! \copydoc amsr::stream::OutputStream::Write(core::Span<std::uint8_t>, stream::Base) */
  auto Write(core::Span<std::uint8_t const> data, stream::Base base) noexcept -> Result<std::int64_t> final {
    return Repeater([this, data, base]() {
      // VECTOR NC AutosarC++17_10-A7.2.1: MD_LOG_AutosarC++17_10-A7.2.1_FalsePositive
      return this->ss_.Write(data, base);
    });
  }

  /*! \copydoc amsr::stream::OutputStream::Put(char) */
  auto Put(char data) noexcept -> Result<void> final {
    this->GrowIfNeeded(1);
    return this->ss_.Put(data);
  }

  /*! \copydoc amsr::stream::OutputStream::Tell() */
  auto Tell() const noexcept -> Result<std::uint64_t> final { return this->ss_.Tell(); }

 private:
  /*!
   * \brief Gets the currently used size.
   *
   * \return The currently used size.
   *
   * \pre         -
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \context     ANY
   * \spec
   *   requires true;
   * \endspec
   */
  auto Size() const noexcept -> std::size_t {
    using vac::container::literals::operator""_sv;

    Result<std::uint64_t> tell{this->Tell()};
    AssertDebug(tell.HasValue(), "Getting the current position should not fail"_sv);
    return static_cast<std::size_t>(tell.Value());
  }

  /*!
   * \brief Gets the currently used size.
   *
   * \return The currently used size.
   *
   * \pre         -
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \context     ANY
   * \spec
   *   requires true;
   * \endspec
   */
  auto SSize() const noexcept -> std::int64_t {
    using vac::container::literals::operator""_sv;

    std::size_t const size{this->Size()};
    return static_cast<std::int64_t>(size);
  }

  /*!
   * \brief Gets the currently available capacity.
   *
   * \return The currently available capacity.
   *
   * \pre         -
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \context     ANY
   * \spec
   *   requires true;
   * \endspec
   */
  auto Capacity() const noexcept -> std::size_t { return this->storage_.size() - 1; }

  /*!
   * \brief Gets the currently available capacity.
   *
   * \return The currently available capacity.
   *
   * \pre         -
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \context     ANY
   * \spec
   *   requires true;
   * \endspec
   */
  auto SCapacity() const noexcept -> std::int64_t {
    using vac::container::literals::operator""_sv;

    std::size_t const cap{this->Capacity()};
    return static_cast<std::int64_t>(cap);
  }

  /*!
   * \brief Grows the storage in case it's needed for the size.
   *
   * \param size The size that needs to fit into the stream.
   *
   * \pre         -
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \context     ANY
   * \spec
   *   requires true;
   * \endspec
   */
  void GrowIfNeeded(std::size_t size) noexcept {
    if (this->NeedsGrowth(size)) {
      Grow(size);
    }
  }

  /*!
   * \brief Checks if the requested size can be stored without growing the storage.
   *
   * \param size The size to write
   * \return     True in case the size can be stored, false in case growing is needed.
   *
   * \pre         -
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \context     ANY
   * \spec
   *   requires true;
   * \endspec
   */
  auto NeedsGrowth(std::size_t size) const noexcept -> bool {
    char const* const cur{std::next(this->Data(), this->SSize())};
    char const* const end{std::next(this->Data(), this->SCapacity())};
    return static_cast<std::size_t>(std::distance(cur, end)) < size;
  }

  /*!
   * \brief Reinitializes the stream to the new buffer.
   *
   * \pre         -
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \context     ANY
   * \spec
   *   requires true;
   * \endspec
   */
  void ReseatStream() noexcept {
    this->ss_ = ::amsr::stream::stringstream::OutputStringStream{core::Span<char>{this->Data(), this->Capacity()}};
  }

  /*!
   * \brief       Grows the underlying storage & reinitializes the stream.
   * \note        Calling Grow with any argument will always result in growth.
   *              Storage will grow with the value of min_increase when it equals
   *              at least half the size of the initial capacity. Else the stream will
   *              always grow with half the size of the initial capacity.
   *
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \context     ANY
   * \spec
   *   requires true;
   * \endspec
   */
  // VECTOR NC AutosarC++17_10-A15.5.3: MD_LOG_AutosarC++17_10-A15.5.3
  // VECTOR NC AutosarC++17_10-A15.4.2: MD_LOG_AutosarC++17_10-A15.4.2
  void Grow(std::size_t min_increase = 1) noexcept {
    using vac::container::literals::operator""_sv;

    // Store the currently used size.
    std::int64_t const position{this->SSize()};
    // Increase the size of the storage
    std::size_t const capacity{this->storage_.size()};
    std::size_t const growth{std::max(capacity + min_increase, (capacity * 3) / 2)};
    // VCA_LOGAPI_CHECKED_CONTRACT
    this->storage_.resize(growth);

    // Insert the new buffer into the stream
    this->ReseatStream();
    // Jump to the previous location.
    Result<std::uint64_t> const seek_result{this->ss_.Seek(position, ::amsr::stream::SeekDirection::kBeg).HasValue()};
    AssertDebug(seek_result.HasValue(), "Seeking to the previous position may not fail."_sv);
  }

  /*!
   * \brief Tries to execute the command once & grows & repeats on failure.
   *
   * \tparam Fn The type of function to execute.
   * \param fn  The function to execute.
   * \return    The result of the function.
   *
   * \pre         -
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \context     ANY
   * \spec
   *   requires true;
   * \endspec
   */
  template <typename Fn>
  auto Repeater(Fn&& fn) noexcept -> Result<std::int64_t> {
    Result<std::int64_t> result{fn()};
    if (not result) {
      this->Grow(20);
      result = fn();
    }
    return result;
  }

  /*!
   * \brief Gets a mutable data pointer to the underlying storage.
   *
   * \return a mutable data pointer to the underlying storage.
   *
   * \pre         -
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \context     ANY
   * \spec
   *   requires true;
   * \endspec
   */
  auto Data() & noexcept -> char* { return this->storage_.data(); }

  /*!
   * \brief Gets a constant data pointer to the underlying storage.
   *
   * \return a constant data pointer to the underlying storage.
   *
   * \pre         -
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \context     ANY
   * \spec
   *   requires true;
   * \endspec
   */
  auto Data() const& noexcept -> char const* { return this->storage_.data(); }
};
}  // namespace internal
}  // namespace log
}  // namespace amsr

#endif  // LIB_AMSR_LOG_INCLUDE_AMSR_LOG_INTERNAL_STRING_STREAM_H_
