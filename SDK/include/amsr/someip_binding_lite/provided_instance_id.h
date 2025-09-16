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
/*!        \file   someip_binding_lite/provided_instance_id.h
 *         \brief  Provided instance ID type.
 *         \unit   SomeIpBinding::SomeIpBindingLite::SomeIpBindingLiteDataTypes
 *
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_BINDING_LITE_INCLUDE_AMSR_SOMEIP_BINDING_LITE_PROVIDED_INSTANCE_ID_H_
#define LIB_SOMEIP_BINDING_LITE_INCLUDE_AMSR_SOMEIP_BINDING_LITE_PROVIDED_INSTANCE_ID_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstdint>

#include "amsr/someip_binding_lite/data_types.h"
#include "amsr/someip_binding_lite/internal/constants.h"
#include "amsr/someip_binding_lite/internal/data_types.h"
#include "ara/core/abort.h"

namespace amsr {
namespace someip_binding_lite {

/*!
 * \brief Provided instance ID.
 * \vpublic
 */
class ProvidedInstanceId final {
 public:
  /*!
   * \brief Constructor.
   * \vpublic
   * \param[in] instance_id The ID of the provided instance.
   * \pre \p instance_id shall not have the special value to represent all IDs.
   * \context     ANY
   * \reentrant   FALSE
   * \threadsafe  FALSE
   * \synchronous TRUE
   * \steady      FALSE
   * \internal
   * - Construct from the passed-in argument.
   * \endinternal
   */
  explicit ProvidedInstanceId(std::uint16_t const instance_id) noexcept : instance_id_{instance_id} {
    if (instance_id_ == internal::kInstanceIdAll) {
      ara::core::Abort(
          "ProvidedInstanceId::ProvidedInstanceId: "
          "Instance ID representing all IDs is not allowed.");
    }
  }

  /*!
   * \brief Destructor.
   * \steady FALSE
   */
  ~ProvidedInstanceId() noexcept = default;

  /*!
   * \brief Copy constructible.
   * \details Needed for compatibility with \p std::map.
   * \steady FALSE
   */
  ProvidedInstanceId(ProvidedInstanceId const&) noexcept = default;

  /*!
   * \brief Move constructible.
   * \details It is used only for emplace or erase list of provided
   *          instance ids in the RequiredServiceInstanceImpl class.
   * \steady FALSE
   */
  ProvidedInstanceId(ProvidedInstanceId&&) noexcept = default;

  ProvidedInstanceId& operator=(ProvidedInstanceId const&) = delete;

  /*!
   * \brief Move assignable.
   * \details It is used only for emplace or erase list of provided
   *          instance ids in the RequiredServiceInstanceImpl class.
   * \return Reference to the type.
   *
   * \steady      FALSE
   */
  ProvidedInstanceId& operator=(ProvidedInstanceId&&) & noexcept = default;

  /*!
   * \brief Comparison operator.
   * \details Needed for searching in \p std::map.
   * \vpublic
   * \param[in] other The other instance to compare against.
   * \return "true" if the instances are equal and "false" otherwise.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \threadsafe FALSE
   * \synchronous TRUE
   * \steady      FALSE
   * \internal
   * - Compare the passed-in instance ID with the internal ID and return the status.
   * \endinternal
   */
  bool operator==(ProvidedInstanceId const& other) const noexcept {  // COV_SOMEIPBINDING_TOOLING_ISSUE
    return ((*this).instance_id_ == other.instance_id_);             // COV_SOMEIPBINDING_TOOLING_ISSUE
  }

  /*!
   * \brief Get the internal instance ID.
   * \vpublic
   * \return The internal instance ID.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \threadsafe FALSE
   * \synchronous TRUE
   * \steady      FALSE
   * \internal
   * - Return the internal instance ID.
   * \endinternal
   */
  InstanceId Value() const noexcept { return instance_id_; }

 private:
  /*!
   * \brief Internal instance ID.
   */
  std::uint16_t instance_id_;
};

}  // namespace someip_binding_lite
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_LITE_INCLUDE_AMSR_SOMEIP_BINDING_LITE_PROVIDED_INSTANCE_ID_H_

// COV_JUSTIFICATION_BEGIN
// \ID COV_SOMEIPBINDING_TOOLING_ISSUE
//   \ACCEPT  XX
//   \REASON  The justified gap is reported as being not covered
//            in TC:Standard_Embedded.CodeCoverageUnit, but it
//            is covered by UT__SomeIpBindingLite__ProvidedInstanceId.Equality and
//            UT__SomeIpBindingLite__ProvidedInstanceId.Inequality.
//            The issue is tracked with https://jira.vi.vector.int/browse/TAR-20619.
// COV_JUSTIFICATION_END
