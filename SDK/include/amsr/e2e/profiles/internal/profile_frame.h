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
/*!        \file
 *        \brief  ProfileFrame declaration.
 *
 *      \details  Contains the declarations for the ProfileFrame class.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBE2E_INCLUDE_AMSR_E2E_PROFILES_INTERNAL_PROFILE_FRAME_H_
#define LIB_LIBE2E_INCLUDE_AMSR_E2E_PROFILES_INTERNAL_PROFILE_FRAME_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstdint>
#include <memory>

#include "amsr/core/span.h"
#include "vac/testing/test_adapter.h"

#include "amsr/e2e/profiles/end2end_event_protection_props.h"
#include "amsr/e2e/profiles/profile.h"

namespace amsr {
namespace e2e {
namespace profiles {
namespace internal {
/*!
 * \brief Basic frame that concrete profiles can derive from.
 * \unit amsr::e2e::profiles::internal::ProfileFrame
 * \vprivate Vector component internal base class
 */
class ProfileFrame {
 public:
  /*!
   * \brief Alias for amsr::core::Span<std::uint8_t const>
   */
  using ConstBufferView = amsr::core::Span<std::uint8_t const>;

  /*!
   * \brief Alias for amsr::core::Span<std::uint8_t>
   */
  using BufferView = amsr::core::Span<std::uint8_t>;

  /*!
   * \brief Constructor for profile.
   * \param[in] profile Profile to choose for the Profile Checker object to be created.
   * \param[in] props the E2E Event Protection Properties
   *
   * The following constraints apply (proprietary profile has no constraints)
   * \code
   *
   * | Applicable Profiles        | Constraint                                                                        |
   * |----------------------------|-----------------------------------------------------------------------------------|
   * | All                        | props.kDataId <= std::numeric_limits<decltype(E2E_PXXConfigType::DataID)>::max()  |
   * | 1 [props.kDataIDMode == 3] | props.kDataId <= 4095                                                             |
   * | 1 [props.kDataIDMode != 3] | props.kDataId <= 65535                                                            |
   * | 4,5,6,7,22,44              | (props.kBitOffset % 8) == 0                                                       |
   * | 4,6,7,44                   | ((props.kBitOffset >> 3) + GetHeaderSize(Profile)) <= (props.kMinDataLength >> 3) |
   * | 5,22                       | ((props.kBitOffset >> 3) + GetHeaderSize(Profile)) <= (props.kDataLength >> 3)    |
   * | 4,6                        | props.kMinDataLength <= props.kMaxDataLength <= 32768                             |
   * | 7                          | props.kMinDataLength <= props.kMaxDataLength <= 33554432                          |
   * | 44                         | props.kMinDataLength <= props.kMaxDataLength <= 524280                            |
   * | 1,5,22                     | (props.kDataLength % 8) == 0                                                      |
   * | 1,22                       | props.kDataLength <= 256                                                          |
   * | 5                          | props.kDataLength <= 32768                                                        |
   * | 1                          | (props.kCounterOffset % 4) == 0                                                   |
   * | 1                          | props.kCounterOffset <= (props.kDataLength - 4)                                   |
   * | 1                          | (props.kCRCOffset % 8) == 0                                                       |
   * | 1                          | props.kCRCOffset <= (props.kDataLength - 8)                                       |
   * | 1 [props.kDataIDMode == 3] | (props.kDataIDNibbleOffset % 4) == 0                                              |
   * | 1 [props.kDataIDMode == 3] | props.kDataIDNibbleOffset <= (props.kDataLength - 4)                              |
   *
   *\endcode
   *
   * \param[in] max_delta_counter Maximum Delta Counter
   *
   * The following constraints apply (proprietary profile has no constraints):
   *
   * \code
   *
   * | Applicable Prof | Constraint                                                                                   |
   * |-----------------|----------------------------------------------------------------------------------------------|
   * | All             | max_delta_counter <=std::numeric_limits<decltype(E2E_PXXConfigType::MaxDeltaCounter)>::max() |
   * | 1               | max_delta_counter <= 14                                                                      |
   * | 22              | max_delta_counter <= 15                                                                      |
   *
   * \endcode
   *
   * \context ANY
   *
   * \pre -
   *
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \steady FALSE
   */
  ProfileFrame(Profile profile, End2EndEventProtectionProps const& props, std::uint32_t max_delta_counter) noexcept;

  /*!
   * \brief Default destructor.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \steady FALSE
   */
  virtual ~ProfileFrame() noexcept;

  // Explicitly delete all unused default/move/copy constructors and operators. */
  ProfileFrame() = delete;
  ProfileFrame(ProfileFrame const&) = delete;
  ProfileFrame(ProfileFrame&&) = delete;
  ProfileFrame& operator=(ProfileFrame const&) = delete;
  ProfileFrame& operator=(ProfileFrame&&) = delete;

  /*!
   * \brief Get the header size in bytes to pre-allocate the correct buffer size.
   * \return The header size from the specification.
   * \context ANY
   * \pre -
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous TRUE
   * \steady      TRUE
   */
  std::uint_fast8_t GetHeaderSize() const noexcept;

  /*!
   * \brief Get the header size in bytes to pre-allocate the correct buffer size.
   * \param[in] profile the profile to determine the header size for
   * \return The header size from the specification if a valid literal for profile has been provided,
   *         an invalid arbitrary value otherwise.
   * \context ANY
   * \pre -
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous TRUE
   * \steady      TRUE
   */
  static std::uint_fast8_t constexpr GetHeaderSize(Profile profile) noexcept {
    // Size of the E2E headers.
    // The layout of the header for each profile looks like this:
    // Profile 01 (element 0): No header
    // Profile 04 (element 1): | Length (2 bytes) | Counter (2 bytes) | DataId (4 bytes) | CRC (4 bytes) |
    // Profile 05 (element 2): | CRC (2 bytes) | Counter (1 byte) |
    // Profile 06 (element 3): | CRC (2 bytes) | Length (2 bytes) | Counter (1 bytes) |
    // Profile 07 (element 4): | CRC (8 bytes) | Length (4 bytes) | Counter (4 bytes) | DataId (4 bytes) |
    // Profile 22 (element 5): | CRC (1 byte) | Counter (1 byte) |
    // Profile 44 (element 6): | Length (2 bytes) | Counter (2 bytes) | DataId (4 bytes) | CRC (4 bytes) |
    // Proprietary (element 7): <OEM-specific layout>
    //
    constexpr std::array<std::uint_fast8_t, 8> const kHeaderSizes{0U, 12U, 3U, 5U, 20U, 2U, 12U, 0U};
    return kHeaderSizes[static_cast<std::size_t>(profile) & 0x07U];
  }

 protected:
  // Forward declaration because this header must not include any C header file
  class ProfileFrameImpl;

  /*!
   * \brief Convert std::size_t into uint16.
   * \param[in] size the size to convert to uint16.
   * \return same value as size on success.
   * \context ANY
   * \pre value of size fits into uint16.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \steady TRUE
   */
  static std::uint16_t ConvertSizeToUInt16(std::size_t const size) noexcept;

  /*!
   * \brief Convert std::size_t into uint32.
   * \param[in] size the size to convert to uint32.
   * \return same value as size on success.
   * \context ANY
   * \pre value of size fits into uint32.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \steady TRUE
   */
  static std::uint32_t ConvertSizeToUInt32(std::size_t const size) noexcept;

  /*!
   * \brief Getter method for p_impl_.
   * \return Pointer to the implementation class implementation.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \steady TRUE
   */
  ProfileFrameImpl* GetPImpl() noexcept;

 private:
  /*!
   * \brief Perform Profile 1 specific run-time assertions.
   * \param[in] props the E2E Event Protection Properties
   * \param[in] max_delta_counter Maximum Delta Counter
   * \pre -
   * \context ANY
   * \reentrant  FALSE
   * \synchronous TRUE
   * \steady TRUE
   */
  static void DoProfile01RuntimeAssertions(End2EndEventProtectionProps const& props,
                                           std::uint32_t max_delta_counter) noexcept;

  /*!
   * \brief Perform Profile 4 specific run-time assertions.
   * \param[in] props the E2E Event Protection Properties
   * \param[in] max_delta_counter Maximum Delta Counter
   * \pre -
   * \context ANY
   * \reentrant  FALSE
   * \synchronous TRUE
   * \steady TRUE
   */
  static void DoProfile04RuntimeAssertions(End2EndEventProtectionProps const& props,
                                           std::uint32_t max_delta_counter) noexcept;

  /*!
   * \brief Perform Profile 5 specific run-time assertions.
   * \param[in] props the E2E Event Protection Properties
   * \param[in] max_delta_counter Maximum Delta Counter
   * \pre -
   * \context ANY
   * \reentrant  FALSE
   * \synchronous TRUE
   * \steady TRUE
   */
  static void DoProfile05RuntimeAssertions(End2EndEventProtectionProps const& props,
                                           std::uint32_t max_delta_counter) noexcept;

  /*!
   * \brief Perform Profile 6 specific run-time assertions.
   * \param[in] props the E2E Event Protection Properties
   * \param[in] max_delta_counter Maximum Delta Counter
   * \pre -
   * \context ANY
   * \reentrant  FALSE
   * \synchronous TRUE
   * \steady TRUE
   */
  static void DoProfile06RuntimeAssertions(End2EndEventProtectionProps const& props,
                                           std::uint32_t max_delta_counter) noexcept;

  /*!
   * \brief Perform Profile 7 specific run-time assertions.
   * \param[in] props the E2E Event Protection Properties
   * \pre -
   * \context ANY
   * \reentrant  FALSE
   * \synchronous TRUE
   * \steady TRUE
   */
  static void DoProfile07RuntimeAssertions(End2EndEventProtectionProps const& props, std::uint32_t) noexcept;

  /*!
   * \brief Perform Profile 22 specific run-time assertions.
   * \param[in] props the E2E Event Protection Properties
   * \param[in] max_delta_counter Maximum Delta Counter
   * \pre -
   * \context ANY
   * \reentrant  FALSE
   * \synchronous TRUE
   * \steady TRUE
   */
  static void DoProfile22RuntimeAssertions(End2EndEventProtectionProps const& props,
                                           std::uint32_t max_delta_counter) noexcept;

  /*!
   * \brief Perform Profile 44 specific run-time assertions.
   * \param[in] props the E2E Event Protection Properties
   * \param[in] max_delta_counter Maximum Delta Counter
   * \pre -
   * \context ANY
   * \reentrant  FALSE
   * \synchronous TRUE
   * \steady TRUE
   */
  static void DoProfile44RuntimeAssertions(End2EndEventProtectionProps const& props,
                                           std::uint32_t max_delta_counter) noexcept;

  /*!
   * \brief Pimpl Idiom used here because this header must not include any C header file.
   */
  std::unique_ptr<ProfileFrameImpl> p_impl_;

  /*!
   * \brief Friend declaration for test purposes.
   */
  FRIEND_TEST(UT_, ProfileFrame);
  /*!
   * \brief Friend declaration for test purposes of static member functions.
   */
  FRIEND_TEST(UT_, ProfileFrame__StaticMembers);
};

}  // namespace internal
}  // namespace profiles
}  // namespace e2e
}  // namespace amsr

#endif  // LIB_LIBE2E_INCLUDE_AMSR_E2E_PROFILES_INTERNAL_PROFILE_FRAME_H_
