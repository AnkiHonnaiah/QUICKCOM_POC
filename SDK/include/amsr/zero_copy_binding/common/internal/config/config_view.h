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
 *         \brief  A wrapper to hold the binding's configuration and provide reading access to specified parts of it.
 *
 *********************************************************************************************************************/

#ifndef LIB_ZERO_COPY_BINDING_COMMON_INCLUDE_AMSR_ZERO_COPY_BINDING_COMMON_INTERNAL_CONFIG_CONFIG_VIEW_H_
#define LIB_ZERO_COPY_BINDING_COMMON_INCLUDE_AMSR_ZERO_COPY_BINDING_COMMON_INTERNAL_CONFIG_CONFIG_VIEW_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/core/optional.h"
#include "amsr/zero_copy_binding/common/internal/config/zero_copy_binding_config.h"
#include "ara/core/vector.h"

namespace amsr {
namespace zero_copy_binding {
namespace common {
namespace internal {

namespace config {

/*!
 * \brief Wrapper class of the ZeroCopyBindingConfig data with reading access to the specified information.
 * \unit  ZeroCopyBinding::ZeroCopyBindingCommon::Configuration
 */
// VECTOR NC AutosarC++17_10-A12.0.1: MD_ZEROCOPYBINDING_A12.0.1_noMoveConstructorDeclaration
// VECTOR NC VectorC++-V12.0.1: MD_ZEROCOPYBINDING_V12.0.1_noMoveConstructorDeclaration
class ConfigView final {
  /*!
   * \brief Index types of the ::ara::core::Vector template instances used by the config struct.
   */
  using ServiceVectorIndexType = ::ara::core::Vector<Service>::size_type;
  using EventVectorIndexType = ::ara::core::Vector<Event>::size_type;
  using RequiredServiceInstanceVectorIndexType = ::ara::core::Vector<RequiredServiceInstance>::size_type;
  using RequiredInstanceSpecificEventVectorIndexType =
      ::ara::core::Vector<RequiredInstanceSpecificEventConfig>::size_type;
  using ProvidedServiceInstanceVectorIndexType = ::ara::core::Vector<ProvidedServiceInstance>::size_type;
  using ProvidedInstanceSpecificEventVectorIndexType =
      ::ara::core::Vector<ProvidedInstanceSpecificEventConfig>::size_type;

 public:
  /*!
   * \brief       Constructor of ConfigView.
   * \param[in]   zero_copy_binding_config  A reference to the binding's config struct.
   *                                        The reference must be valid for this object's entire lifetime.
   * \context     ANY
   * \pre         -
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Component private
   */
  explicit ConfigView(ZeroCopyBindingConfig const& zero_copy_binding_config) noexcept;

  /*!
   * \brief Default destructor.
   */
  ~ConfigView() noexcept = default;

  ConfigView() = delete;
  ConfigView(ConfigView const&) noexcept = default;
  // Commented out, since otherwise constructing a ConfigView from a temporary object won't work (will work in C++-17):
  // ConfigView(ConfigView&&) = delete;
  auto operator=(ConfigView const&) -> ConfigView& = delete;
  auto operator=(ConfigView&&) -> ConfigView& = delete;

  // Setter functions for the different indices.

  /*!
   * \brief       Construct a new ConfigView with specified service index.
   * \param[in]   service_idx  The service index that shall be set.
   * \return      An instance of the new, more specific ConfigView.
   * \context     ANY
   * \pre         The service index must not have already been set and has to be a valid index in the stored list of
   *              services.
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Component private
   */
  auto WithServiceIdx(ServiceVectorIndexType service_idx) const noexcept -> ConfigView;

  /*!
   * \brief       Construct a new ConfigView with specified event index.
   * \param[in]   event_idx  The event index that shall be set.
   * \return      An instance of the new, more specific ConfigView.
   * \context     ANY
   * \pre         The service index has to be set. Further, the event index must not have already been set and has to be
   *              a valid index in the stored list of services.
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Component private
   */
  auto WithEventIdx(EventVectorIndexType event_idx) const noexcept -> ConfigView;

  /*!
   * \brief       Construct a new ConfigView with specified required service index.
   * \param[in]   required_service_instance_idx  The required service index that shall be set.
   * \return      An instance of the new, more specific ConfigView.
   * \context     ANY
   * \pre         The service index has to be set. The required service index must not have already been set and has to
   *              be a valid index in the stored list of services.
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Component private
   */
  auto WithRequiredIdx(RequiredServiceInstanceVectorIndexType required_service_instance_idx) const noexcept
      -> ConfigView;

  /*!
   * \brief       Construct a new ConfigView with specified required event index.
   * \param[in]   rq_instance_specific_event_idx  The required event index that shall be set.
   * \return      An instance of the new, more specific ConfigView.
   * \context     ANY
   * \pre         The required service index has to be set. The required event index must not have already been set and
   *              has to be a valid index in the stored list of services.
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Component private
   */
  auto WithRequiredEventIdx(RequiredInstanceSpecificEventVectorIndexType rq_instance_specific_event_idx) const noexcept
      -> ConfigView;

  /*!
   * \brief       Construct a new ConfigView with specified provided service index.
   * \param[in]   provided_service_instance_idx  The provided service index that shall be set.
   * \return      An instance of the new, more specific ConfigView.
   * \context     ANY
   * \pre         The service index has to be set. The provided service index must not have already been set and has to
   *              be a valid index in the stored list of services.
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Component private
   */
  auto WithProvidedIdx(ProvidedServiceInstanceVectorIndexType provided_service_instance_idx) const noexcept
      -> ConfigView;

  /*!
   * \brief       Construct a new ConfigView with specified provided event index.
   * \param[in]   pv_instance_specific_event_idx  The provided event index that shall be set.
   * \return      An instance of the new, more specific ConfigView.
   * \context     ANY
   * \pre         The provided service index has to be set. The provided event index must not have already been set and
   *              has to be a valid index in the stored list of services.
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Component private
   */
  auto WithProvidedEventIdx(ProvidedInstanceSpecificEventVectorIndexType pv_instance_specific_event_idx) const noexcept
      -> ConfigView;

  // Getter functions for the different config structs.

  /*!
   * \brief       Provide access to the previously specified service configuration.
   * \return      A reference to the configuration struct associated with the specified service.
   * \context     ANY
   * \pre         The returned reference shall only be used during the ConfigView's lifetime.
   * \pre         The service index has to be set.
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Component private
   */
  auto AsServiceConfig() const noexcept -> Service const&;

  /*!
   * \brief       Provide access to the previously specified event configuration.
   * \return      A reference to the configuration struct associated with the specified event.
   * \context     ANY
   * \pre         The returned reference shall only be used during the ConfigView's lifetime.
   * \pre         The event index has to be set.
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Component private
   */
  auto AsEventConfig() const noexcept -> Event const&;

  /*!
   * \brief       Provide access to the previously specified required service configuration.
   * \return      A reference to the configuration struct associated with the specified required service instance.
   * \context     ANY
   * \pre         The returned reference shall only be used during the ConfigView's lifetime.
   * \pre         The required service index has to be set.
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Component private
   */
  auto AsRequiredConfig() const noexcept -> RequiredServiceInstance const&;

  /*!
   * \brief       Provide access to the previously specified required event configuration.
   * \return      A reference to the configuration struct associated with the specified required event.
   * \context     ANY
   * \pre         The returned reference shall only be used during the ConfigView's lifetime.
   * \pre         The required event index has to be set.
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Component private
   */
  auto AsRequiredEventConfig() const noexcept -> RequiredInstanceSpecificEventConfig const&;

  /*!
   * \brief       Provide access to the previously specified provided service configuration.
   * \return      A reference to the configuration struct associated with the specified provided service instance.
   * \context     ANY
   * \pre         The returned reference shall only be used during the ConfigView's lifetime.
   * \pre         The provided service index has to be set.
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Component private
   */
  auto AsProvidedConfig() const noexcept -> ProvidedServiceInstance const&;

  /*!
   * \brief       Provide access to the previously specified provided event configuration.
   * \return      A reference to the configuration struct associated with the specified provided event.
   * \context     ANY
   * \pre         The returned reference shall only be used during the ConfigView's lifetime.
   * \pre         The provided event index has to be set.
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Component private
   */
  auto AsProvidedEventConfig() const noexcept -> ProvidedInstanceSpecificEventConfig const&;

  /*!
   * \brief       Provide access to the whole configuration.
   * \return      The stored reference to the configuration struct.
   * \context     ANY
   * \pre         -
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Component private
   */
  auto AccessConfiguration() const noexcept -> ZeroCopyBindingConfig const&;

 private:
  /*!
   * \brief A reference to the binding's configuration struct.
   */
  ZeroCopyBindingConfig const& zero_copy_binding_config_;

  /*!
   * \brief The indices corresponding to the different config structs in the hierarchy.
   */
  ::amsr::core::Optional<ServiceVectorIndexType> service_idx_{};
  ::amsr::core::Optional<EventVectorIndexType> event_idx_{};
  ::amsr::core::Optional<RequiredServiceInstanceVectorIndexType> required_service_instance_idx_{};
  ::amsr::core::Optional<RequiredInstanceSpecificEventVectorIndexType> rq_instance_specific_event_idx_{};
  ::amsr::core::Optional<ProvidedServiceInstanceVectorIndexType> provided_service_instance_idx_{};
  ::amsr::core::Optional<ProvidedInstanceSpecificEventVectorIndexType> pv_instance_specific_event_idx_{};
};

}  // namespace config

}  // namespace internal
}  // namespace common
}  // namespace zero_copy_binding
}  // namespace amsr

#endif  // LIB_ZERO_COPY_BINDING_COMMON_INCLUDE_AMSR_ZERO_COPY_BINDING_COMMON_INTERNAL_CONFIG_CONFIG_VIEW_H_
