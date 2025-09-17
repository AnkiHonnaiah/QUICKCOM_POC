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
/*!     \file     overload_protection_filter_object.h
 *      \brief    Configuration object for 'filter' for events.
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_OVERLOAD_PROTECTION_FILTER_OBJECT_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_OVERLOAD_PROTECTION_FILTER_OBJECT_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <chrono>

#include "amsr/someip_daemon_core/configuration/cfg_element.h"
#include "amsr/someip_daemon_core/configuration/configuration_types.h"

namespace amsr {
namespace someip_daemon_core {
namespace configuration {
namespace model {

/*!
 * \brief This is an intermediate representation of a 'overload_protection_filter' object which is filled by a parser.
 * \details The validator then takes this object and validates if mandatory elements are set or not.
 */
class OverloadProtectionFilterConfigObject final {
 public:
  /*!
   * \brief Minimum interval to filter the events element.
   */
  using MinimumIntervalElement = CfgElement<std::chrono::nanoseconds>;

  /*!
   * \brief Constructor sets the references to the elements of the structure to write to.
   * \param[in] filter Reference to the 'OverloadProtectionFilter' structure to write to.
   * \pre -
   * \context Init
   * \reentrant FALSE
   * \steady FALSE
   */
  explicit OverloadProtectionFilterConfigObject(ConfigurationTypesAndDefs::OverloadProtectionFilter& filter) noexcept
      : min_interval_ns_{filter.min_interval_ns} {}

  /*!
   * \brief Default destructor.
   * \steady FALSE
   */
  ~OverloadProtectionFilterConfigObject() = default;

  /*!
   * \brief Deleted copy constructor.
   */
  OverloadProtectionFilterConfigObject(OverloadProtectionFilterConfigObject const&) = delete;

  /*!
   * \brief Deleted move constructor.
   */
  OverloadProtectionFilterConfigObject(OverloadProtectionFilterConfigObject&&) = delete;

  /*!
   * \brief Deleted copy assignment constructor.
   */
  auto operator=(OverloadProtectionFilterConfigObject const&) & -> OverloadProtectionFilterConfigObject& = delete;

  /*!
   * \brief Deleted move assignment operator.
   */
  auto operator=(OverloadProtectionFilterConfigObject&&) & -> OverloadProtectionFilterConfigObject& = delete;

  /*!
   * \brief Set the minimum interval for events reception.
   * \param[in] min_interval_ns minimum interval for events reception.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   *
   * \steady FALSE
   */
  void SetMinimumInterval(std::chrono::nanoseconds const& min_interval_ns) noexcept {
    min_interval_ns_.SetElement(min_interval_ns);
  }

  /*!
   * \brief Getter for the minimum interval for events reception.
   * \return Reference to the minimum interval element of this intermediate object.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  MinimumIntervalElement const& GetMinimumInterval() const noexcept { return min_interval_ns_; }

 private:
  /*!
   * \brief The minimum interval between successive events for overload protection filtering.
   * \details After an event went through, any event that follows before this minimum time has elapsed will be dropped.
   *
   */
  MinimumIntervalElement min_interval_ns_;
};

}  // namespace model
}  // namespace configuration
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_OVERLOAD_PROTECTION_FILTER_OBJECT_H_
