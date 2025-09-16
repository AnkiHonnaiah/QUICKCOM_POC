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
/*!        \file   someip_binding_lite/sample_ptr.h
 *         \brief  Sample pointer type.
 *         \unit   SomeIpBinding::SomeIpBindingLite::Communication::Events::SomeIpBindingLiteSamplePtr
 *
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_BINDING_LITE_INCLUDE_AMSR_SOMEIP_BINDING_LITE_SAMPLE_PTR_H_
#define LIB_SOMEIP_BINDING_LITE_INCLUDE_AMSR_SOMEIP_BINDING_LITE_SAMPLE_PTR_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstddef>
#include <cstdint>

namespace amsr {
namespace someip_binding_lite {

/*!
 * \brief Sample pointer type.
 * \vpublic
 */
class SamplePtr final {
 public:
  /*!
   * \brief Constructor.
   * \vprivate Vector product internal API.
   * \param[in] data_pointer Data pointer to store internally.
   * \pre -
   * \context     ANY
   * \reentrant   FALSE
   * \threadsafe  FALSE
   * \synchronous TRUE
   * \steady      TRUE
   * \internal
   * - Construct from the passed-in argument.
   * \endinternal
   */
  explicit SamplePtr(void const* data_pointer) noexcept : data_pointer_{data_pointer} {}

  /*!
   * \brief Move constructor.
   * \details Needed for compatibility with \p ara::core::Result.
   * \vpublic
   * \pre -
   * \context     ANY
   * \reentrant   FALSE
   * \threadsafe  FALSE
   * \synchronous TRUE
   * \steady      TRUE
   */
  SamplePtr(SamplePtr&&) noexcept = default;

  /*!
   * \brief Destructor.
   * \pre -
   * \context     ANY
   * \reentrant   FALSE
   * \threadsafe  FALSE
   * \synchronous TRUE
   * \steady      TRUE
   */
  ~SamplePtr() noexcept = default;

  SamplePtr() = delete;
  SamplePtr(SamplePtr const&) = delete;
  SamplePtr& operator=(SamplePtr const&) = delete;
  SamplePtr& operator=(SamplePtr&&) = delete;

  /*!
   * \brief Get the stored pointer.
   * \vpublic
   * \return Pointer to an array of bytes.
   * \pre -
   * \context     ANY
   * \reentrant   FALSE
   * \threadsafe  FALSE
   * \synchronous TRUE
   * \steady      TRUE
   * \internal
   * - Cast and return the internal pointer.
   * \endinternal
   */
  std::uint8_t const* Get() const noexcept {  // COV_SOMEIPBINDING_TOOLING_ISSUE
    // VECTOR Next Line AutosarC++17_10-M5.2.8:MD_SOMEIPBINDING_AutosarC++17_10-M5.2.8_conv_from_voidp
    return static_cast<std::uint8_t const*>(data_pointer_);
  }

  /*!
   * \brief Check if the internal pointer is null or not.
   * \vpublic
   * \return "true" if the internal pointer is not a null pointer and "false" otherwise.
   * \pre -
   * \context     ANY
   * \reentrant   FALSE
   * \threadsafe  FALSE
   * \synchronous TRUE
   * \steady      TRUE
   * \internal
   * - Return status of the internal pointer being null or not.
   * \endinternal
   */
  explicit operator bool() const noexcept { return (data_pointer_ != nullptr); }

 private:
  /*!
   * \brief Internal pointer.
   */
  void const* data_pointer_;
};

}  // namespace someip_binding_lite
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_LITE_INCLUDE_AMSR_SOMEIP_BINDING_LITE_SAMPLE_PTR_H_

// COV_JUSTIFICATION_BEGIN
// \ID COV_SOMEIPBINDING_TOOLING_ISSUE
//   \ACCEPT  XX
//   \REASON  The justified function is reported as being not covered
//            in TC:Standard_Embedded.CodeCoverageUnit, but it
//            is covered by UT__SomeIpBindingLite__SamplePtr.Get_Pointer.
//            The issue is tracked with https://jira.vi.vector.int/browse/TAR-20619.
// COV_JUSTIFICATION_END
