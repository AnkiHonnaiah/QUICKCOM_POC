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
/*!        \file  skeleton_field_config.h
 *        \brief  Configuration for SkeletonField.
 *
 *      \details  Configuration required to template SkeletonField class.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_FIELDS_SKELETON_FIELD_CONFIG_H_
#define LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_FIELDS_SKELETON_FIELD_CONFIG_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <type_traits>
#include "vac/container/c_string_view.h"

namespace amsr {
namespace socal {
namespace internal {
namespace fields {

/*!
 * \brief This class contains the parameters' definitions for SkeletonFieldConfig.
 */
class SkeletonFieldParams final {
 public:
  /*!
   * \brief   Parametrize HasNotifier option of a field.
   * \details This is a handy way to extend the template-parameter-list. If Active is set to true, the user must ensure
   *          to provide a struct like this:
   *
   *          ''' HasNotifier<true, SkeletonEvent<SkeletonFrontend, std::uint32_t, SkeletonBackendInterface, ...>> '''
   *
   *          If no notifier is necessary, the user is possible to set the type like this:
   *
   *          ''' HasNotifier<false> '''
   *
   *          Without the need to give a second template-parameter, because it defaults to void.
   * \tparam Active  True when HasNotification is set to true in the arxml. False otherwise.
   * \tparam Event   Event type when notifier is active. Default to be void otherwise.
   */
  template <bool Active, typename Event = typename std::enable_if<!Active, void>::type>
  struct HasNotifier {
    /*!
     * \brief This enum is used to store parameters set in the template parameters.
     */
    enum {
      /*!
       * \brief Contains the value of HasNotifier from the generator model.
       */
      active = Active
    };

    /*!
     * \brief Either this is the concrete Event type or void, if HasNotifier is set to false.
     */
    using type = Event;
  };

  /*!
   * \brief Parametrize HasGetter option of a field.
   * \tparam Active  True when this value is set to true in the arxml. False otherwise.
   */
  template <bool Active>
  struct HasGetter {
    /*!
     * \brief This enum is used to store parameters set in the template parameters.
     */
    enum {
      /*!
       * \brief Contains the value of HasGetter from the generator model.
       */
      active = Active
    };
  };

  /*!
   * \brief Parametrize HasSetter option of a field.
   * \tparam Active  True when this value is set to true in the arxml. False otherwise.
   */
  template <bool Active>
  struct HasSetter {
    /*!
     * \brief This enum is used to store parameters set in the template parameters.
     */
    enum {
      /*!
       * \brief Contains the value of HasSetter from the generator model.
       */
      active = Active
    };
  };
};

// clang-format off
/*!
 * \brief Parameter-set for one field
 *
 * \tparam SkeletonFrontend Type of the service-specific skeleton frontend class.
 * \tparam Notification The configured notification type (contains info if HasNotifier is set to true / false)
 * \tparam Getter The configured getter type (contains info if HasGetter is set to true / false)
 * \tparam Setter The configured setter type (contains info if HasSetter is set to true / false)
 * \tparam FieldName The configured field name.
 *
 * Example usage:
 *
 * '''
 * using NotifierConfig = HasNotifier<true, SkeletonEvent<SkeletonFrontend, std::uint8_t, SkeletonBackendInterface,
 * ...>>; using GetterConfig = HasGetter<false>; using SetterConfig = HasSetter<false>; using MyFieldConfig =
 * FieldParameters<SkeletonFrontend, NotifierConfig, GetterConfig, SetterConfig>;
 * '''
 */
// clang-format on
template <typename SkeletonFrontend, typename Notification, typename Getter, typename Setter, typename FieldName>
struct SkeletonFieldConfig {
  /*!
   * \brief Type alias for the service-specific skeleton front end.
   */
  using Skeleton = SkeletonFrontend;

  /*!
   * \brief Type alias for a pointer to a service-specific skeleton frontend.
   */
  using SkeletonPtr = typename std::add_pointer<Skeleton>::type;

  /*!
   * \brief Type alias for the field name.
   */
  static constexpr vac::container::CStringView Name{FieldName::Name};

  /*!
   * \brief Store the switches from the template parameterization to be able to use it in other structures.
   */
  enum {
    /*!
     * \brief Switch for HasNotification from the generator model.
     */
    HasNotification = Notification::active,
    /*!
     * \brief Switch for HasGetter from the generator model.
     */
    HasGetter = Getter::active,
    /*!
     * \brief Switch for HasSetter from the generator model.
     */
    HasSetter = Setter::active
  };

  /*!
   * \brief   This is used if the option HasNotification is set to false.
   * \details Provides a way to initialize the members with some empty structure in the field template class, because we
   *          can not enable_if member initialization in the constructor.
   *
   * \unit Socal::Skeleton::SkeletonField
   */
  class DummySkeletonEvent {
   public:
    /*!
     * \brief Delete default constructor.
     */
    DummySkeletonEvent() = delete;
    /*!
     * \brief Define destructor.
     * \steady FALSE
     */
    virtual ~DummySkeletonEvent() noexcept = default;  // VCA_SOCAL_COMPILER_GENERATED_DESTRUCTOR
    /*!
     * \brief Delete copy constructor.
     */
    DummySkeletonEvent(DummySkeletonEvent const&) = delete;
    /*!
     * \brief Delete move constructor.
     */
    DummySkeletonEvent(DummySkeletonEvent&&) = delete;
    /*!
     * \brief Delete copy assignment.
     */
    DummySkeletonEvent& operator=(DummySkeletonEvent const&) & = delete;
    /*!
     * \brief Delete move assignment.
     */
    DummySkeletonEvent& operator=(DummySkeletonEvent&&) & = delete;
    /*!
     * \brief Constructor is used to call it in the constructor of the skeleton field itself for bypassing if an option
     *        is not active.
     * \spec SkeletonPtr != nullptr; \endspec
     * \steady FALSE
     */
    explicit DummySkeletonEvent(SkeletonPtr) noexcept {}
  };

  /*!
   * \brief With the std::conditional we choose between a concrete Event or a "null" type for initialization.
   */
  using NotificationType =
      typename std::conditional<HasNotification, typename Notification::type, DummySkeletonEvent>::type;
};

}  // namespace fields
}  // namespace internal
}  // namespace socal
}  // namespace amsr

#endif  // LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_FIELDS_SKELETON_FIELD_CONFIG_H_
