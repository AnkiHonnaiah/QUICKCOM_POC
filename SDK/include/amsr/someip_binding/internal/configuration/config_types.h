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
/*!        \file  config_types.h
 *        \brief  Types for the basic configuration structs.
 *        \unit   SomeIpBinding::SomeIpBinding::Configuration::JsonParser::JsonConfigTypes
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_BINDING_INCLUDE_AMSR_SOMEIP_BINDING_INTERNAL_CONFIGURATION_CONFIG_TYPES_H_
#define LIB_SOMEIP_BINDING_INCLUDE_AMSR_SOMEIP_BINDING_INTERNAL_CONFIGURATION_CONFIG_TYPES_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include <string>
#include "amsr/core/abort.h"
#include "amsr/core/optional.h"
#include "amsr/core/string.h"
#include "amsr/core/string_view.h"
#include "amsr/e2e/profiles/end2end_event_protection_props.h"
#include "amsr/e2e/profiles/profile.h"
#include "amsr/someip_binding_xf/internal/e2e/client_side_transformer_interface.h"
#include "someip-protocol/internal/someip_posix_types.h"
#include "vac/container/string_literals.h"

namespace amsr {
namespace someip_binding {
namespace internal {
namespace configuration {

// VECTOR NC AutosarC++17_10-M7.3.6, VectorC++-V5.0.1: MD_SOMEIPBINDING_AutosarC++17_10-M7.3.6_using_directive
// VECTOR NC AutosarC++17_10-M7.3.4, VectorC++-V5.0.1: MD_SOMEIPBINDING_AutosarC++17_10-M7.3.4_using_directive
using namespace ::vac::container::literals;  // NOLINT(build/namespaces)

/*!
 * \brief Struct to hold the service identifier integer.
 */
struct ServiceIdentifier {
  /*!
   * \brief The actual integer holding the service identifier.
   */
  ::amsr::someip_protocol::internal::ServiceId service_identifier{0};
};

/*!
 * \brief Comparator for service identifier instances
 */
class ServiceIdentifierComparator {
 public:
  /*!
   * \brief The functor operator to perform the comparison.
   * \param[in] lhs The first service identifier to be compared.
   * \param[in] rhs The second service identifier to be compared.
   * \return True if the inner service ID of lhs is less than that of rhs.
   * \pre -
   * \context ANY
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  bool operator()(ServiceIdentifier const& lhs, ServiceIdentifier const& rhs) const noexcept {
    return lhs.service_identifier < rhs.service_identifier;
  }
};

/*!
 * \brief Struct to hold the instance identifier integer.
 */
struct InstanceIdentifier {
  /*!
   * \brief The actual integer holding the instance identifier.
   */
  ::amsr::someip_protocol::internal::InstanceId instance_identifier{0};
};

/*!
 * \brief Struct to hold the instance specifier string.
 */
// VCA_SOMEIPBINDING_ACCESSING_MEMBERS_OF_REFERENCE_CLASS_ATTRIBUTES
struct InstanceSpecifier {
  /*!
   * \brief The actual string holding the instance specifier.
   */
  amsr::core::String instance_specifier;
};

/*!
 * \brief Struct to hold the major version integer.
 */
struct MajorVersion {
  /*!
   * \brief The actual integer holding the major version.
   */
  ::amsr::someip_protocol::internal::MajorVersion major_version{0};
};

/*!
 * \brief Struct to hold the minor version integer.
 */
struct MinorVersion {
  /*!
   * \brief The actual integer holding the minor version.
   */
  ::amsr::someip_protocol::internal::MinorVersion minor_version{0};
};

/*!
 * \brief Struct to hold the is_generic flag.
 */
struct IsGeneric {
  /*!
   * \brief The actual bool holding the is_generic flag.
   */
  bool is_generic{false};
};

/*!
 * \brief Struct to hold service shortname path.
 */
struct ShortnamePath {
  /*!
   * \brief The actual string holding the shortname path.
   */
  amsr::core::String shortname_path;
};

/*!
 * \brief Comparator for service shortname paths.
 */
class ShortnamePathEqualComparator final {
 public:
  /*!
   * \brief Equal compare operator for Shortname path.
   * \param[in] lhs  First Shortname path to compare.
   * \param[in] rhs Second Shortname path to compare.
   * \return true if first object value equals the second object value, otherwise false.
   *
   * \pre -
   * \context ANY
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  bool operator()(ShortnamePath const& lhs, ShortnamePath const& rhs) const noexcept {
    return lhs.shortname_path == rhs.shortname_path;
  }
};

/*!
 * \brief Type definition for the shortname path hash.
 */
class ShortnamePathHash {
 public:
  /*!
   * \brief Hash function operator for shortname path .
   * \param[in] shortname_path  shortname path  to hash.
   * \return Hashed shortname path .
   *
   * \pre       -
   * \context   ANY
   * \spec
   *   requires true;
   * \endspec
   */
  size_t operator()(ShortnamePath const& shortname_path) const {
    // VCA_SPC_15_SOMEIPBINDING_STL_TYPE_FUNCTION
    return std::hash<std::string>()(shortname_path.shortname_path.c_str());
  }
};

/*!
 * \brief Forward declare ShortnameComparator.
 */
class ShortnameComparator;

/*!
 * \brief Forward declare ShortnameHash.
 */
class ShortnameHash;

/*!
 * \brief Struct to hold service shortname.
 */
// VCA_SOMEIPBINDING_ACCESSING_MEMBERS_OF_REFERENCE_CLASS_ATTRIBUTES
struct Shortname {
  /*!
   * \brief The actual string holding the shortname path.
   */
  amsr::core::String shortname;
  /*!
   * \brief The type alias for comparator.
   */
  using Comparator = ShortnameComparator;
  /*!
   * \brief The type alias for hashing method for unordered maps.
   */
  using HashingMethod = ShortnameHash;
};
/*!
 * \brief Comparator for shortname
 */
class ShortnameComparator final {
 public:
  /*!
   * \brief The functor operator to perform the comparison.
   * \param[in] lhs The first shortname to be compared.
   * \param[in] rhs The second shortname to be compared.
   * \return True if the underlying string is less than rhs lexicographically
   * \pre -
   * \context ANY
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  bool operator()(Shortname const& lhs, Shortname const& rhs) const noexcept { return lhs.shortname < rhs.shortname; }
};

/*!
 * \brief Equal compare operator for Shortname.
 * \param[in] left  First Shortname to compare.
 * \param[in] right Second Shortname to compare.
 * \return true if first object value equals the second object value, otherwise false.
 *
 * \pre       -
 * \context   ANY
 * \reentrant FALSE
 * \spec
 *   requires true;
 * \endspec
 */
bool operator==(Shortname const& left, Shortname const& right) noexcept;

/*!
 * \brief Type definition for the shortname hash.
 */
class ShortnameHash {
 public:
  /*!
   * \brief Hash function operator for Shortname.
   * \param[in] shortname  Shortname to hash.
   * \return Hashed shortname.
   *
   * \pre       -
   * \context   ANY
   * \spec
   *   requires true;
   * \endspec
   */
  auto operator()(Shortname const& shortname) const -> size_t {
    // VCA_SPC_15_SOMEIPBINDING_STL_FUNCTION
    return std::hash<std::string>()(shortname.shortname.c_str());
  }
};

/*!
 * \brief Struct indicating valid event entities for given event shortname.
 */
struct ValidEventConfigurationEntity {
  /*!
   * \brief The shortname of the respective event.
   */
  ::vac::container::CStringView const kShortname;
  /*!
   * \brief Bool to indicate is signal based serialized for the respective event.
   */
  bool const kIsSignalBased;
};

/*!
 * \brief Struct indicating valid field entities for given field shortname.
 */
struct ValidFieldConfigurationEntity {
  /*!
   * \brief The shortname of the respective field.
   */
  ::vac::container::CStringView const kShortname;
  /*!
   * \brief Bool to indicate existance of a Notifier for the respective field.
   */
  bool const kHasNotifier;
  /*!
   * \brief Bool to indicate existance of a Setter for the respective field.
   */
  bool const kHasSetter;
  /*!
   * \brief Bool to indicate existance of a Getter for the respective field.
   */
  bool const kHasGetter;
};
/*!
 * \brief Struct indicating valid method entities for given method shortname.
 */
struct ValidMethodConfigurationEntity {
  /*!
   * \brief The shortname of the respective method.
   */
  ::vac::container::CStringView const kShortname;
};

/*!
 * \brief Class to grant template independent access to the validation data.
 */
class ConfigValidationDataAccess final {
 public:
  /*!
   * \brief Constructor.
   *
   * \param       kValidEventConfigurationEntityPtr        Pointer to array of event configuration entities elements.
   * \param       kValidEventConfigurationEntityArraySize  Size of the array of event configuration entities elements.
   * \param       kValidFieldConfigurationEntityPtr        Pointer to array of field configuration entities elements.
   * \param       kValidFieldConfigurationEntityArraySize  Size of the array of field configuration entities elements.
   * \param       kValidMethodConfigurationEntityPtr       Pointer to array of method configuration entities elements.
   * \param       kValidMethodConfigurationEntityArraySize Size of the array of method configuration entities
   *                                                         elements.
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   */
  constexpr ConfigValidationDataAccess(ValidEventConfigurationEntity const* const kValidEventConfigurationEntityPtr,
                                       size_t const kValidEventConfigurationEntityArraySize,
                                       ValidFieldConfigurationEntity const* const kValidFieldConfigurationEntityPtr,
                                       size_t const kValidFieldConfigurationEntityArraySize,
                                       ValidMethodConfigurationEntity const* const kValidMethodConfigurationEntityPtr,
                                       size_t const kValidMethodConfigurationEntityArraySize)
      : kValidEventConfigurationEntityPtr_{kValidEventConfigurationEntityPtr},
        kValidEventConfigurationEntityArraySize_{kValidEventConfigurationEntityArraySize},
        kValidFieldConfigurationEntityPtr_{kValidFieldConfigurationEntityPtr},
        kValidFieldConfigurationEntityArraySize_{kValidFieldConfigurationEntityArraySize},
        kValidMethodConfigurationEntityPtr_{kValidMethodConfigurationEntityPtr},
        kValidMethodConfigurationEntityArraySize_{kValidMethodConfigurationEntityArraySize} {}

  /*!
   * \brief  Default move constructor.
   * \spec
   *   requires true;
   * \endspec
   */
  ConfigValidationDataAccess(ConfigValidationDataAccess&&) noexcept = default;
  /*!
   * \brief  Default copy constructor.
   * \spec
   *   requires true;
   * \endspec
   */
  ConfigValidationDataAccess(ConfigValidationDataAccess const&) = default;
  ConfigValidationDataAccess& operator=(ConfigValidationDataAccess const&) = delete;
  ConfigValidationDataAccess& operator=(ConfigValidationDataAccess&&) = delete;

  /*!
   * \brief  Default destructor.
   * \spec
   *   requires true;
   * \endspec
   */
  ~ConfigValidationDataAccess() = default;

  /*!
   * \brief Getter for the event shortname at given index position.
   * \param[in] index The index.
   * \return The event shortname.
   * \pre -
   * \context ANY
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  ValidEventConfigurationEntity const& GetValidEventConfigurationEntityAt(size_t const index) const noexcept {
    if (index >= kValidEventConfigurationEntityArraySize_) {
      amsr::core::Abort("Index of event validation data is out of bounds.");
    }
    // VECTOR NC AutosarC++17_10-M5.0.15: MD_SOMEIPBINDING_AutosarC++17_10-M5.0.15_Array_access_via_pointer
    // VECTOR NC AutosarC++17_10-M5.0.16: MD_SOMEIPBINDING_AutosarC++17_10-M5.0.16_Array_access_via_pointer
    // VCA_SOMEIPBINDING_RETURNED_VALUE_IS_VALID
    return kValidEventConfigurationEntityPtr_[index];
  }

  /*!
   * \brief Getter for the method shortname at given index position.
   * \param[in] index The index.
   * \return The method shortname.
   * \pre -
   * \context ANY
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  ::vac::container::CStringView const& GetMethodShortnameAt(size_t const index) const noexcept {
    if (index >= kValidMethodConfigurationEntityArraySize_) {
      amsr::core::Abort("Index of method validation data is out of bounds.");
    }
    // VECTOR NC AutosarC++17_10-M5.0.15: MD_SOMEIPBINDING_AutosarC++17_10-M5.0.15_Array_access_via_pointer
    // VECTOR NC AutosarC++17_10-M5.0.16: MD_SOMEIPBINDING_AutosarC++17_10-M5.0.16_Array_access_via_pointer
    // VCA_SOMEIPBINDING_RETURNED_VALUE_IS_VALID
    return kValidMethodConfigurationEntityPtr_[index].kShortname;
  }
  /*!
   * \brief Returns the number of event shortnames.
   * \return Number of event shortnames.
   * \pre -
   * \context ANY
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  size_t GetEventCount() const noexcept { return kValidEventConfigurationEntityArraySize_; }
  /*!
   * \brief Getter for the field configuration entities at given index position.
   * \param[in] index The index.
   * \return The field configuration entities.
   * \pre -
   * \context ANY
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  ValidFieldConfigurationEntity const& GetValidFieldConfigurationEntityAt(size_t const index) const noexcept {
    if (index >= kValidFieldConfigurationEntityArraySize_) {
      amsr::core::Abort("Index of field validation data is out of bounds.");
    }
    // VECTOR NC AutosarC++17_10-M5.0.15: MD_SOMEIPBINDING_AutosarC++17_10-M5.0.15_Array_access_via_pointer
    // VECTOR NC AutosarC++17_10-M5.0.16: MD_SOMEIPBINDING_AutosarC++17_10-M5.0.16_Array_access_via_pointer
    // VCA_SOMEIPBINDING_RETURNED_VALUE_IS_VALID
    return kValidFieldConfigurationEntityPtr_[index];
  }
  /*!
   * \brief Returns the number of field configuration entities elements.
   * \return Number of field configuration entities elements.
   * \pre -
   * \context ANY
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  size_t GetFieldCount() const noexcept { return kValidFieldConfigurationEntityArraySize_; }
  /*!
   * \brief Returns the number of method configuration entities elements.
   * \return Number of method configuration entities elements.
   * \pre -
   * \context ANY
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  size_t GetMethodCount() const noexcept { return kValidMethodConfigurationEntityArraySize_; }

 private:
  /*!
   * \brief The pointer to the valid event configuration entity array.
   */
  ValidEventConfigurationEntity const* const kValidEventConfigurationEntityPtr_;
  /*!
   * \brief The size of the valid event configuration entity array.
   */
  size_t const kValidEventConfigurationEntityArraySize_;
  /*!
   * \brief The pointer to the valid field configuration entity array.
   */
  ValidFieldConfigurationEntity const* const kValidFieldConfigurationEntityPtr_;
  /*!
   * \brief The size of the valid field configuration entity array.
   */
  size_t const kValidFieldConfigurationEntityArraySize_;
  /*!
   * \brief The pointer to the valid method configuration entities.
   */
  ValidMethodConfigurationEntity const* const kValidMethodConfigurationEntityPtr_;
  /*!
   * \brief The size of the valid method configuration entities.
   */
  size_t const kValidMethodConfigurationEntityArraySize_;
};

/*!
 * \brief Struct indicating valid service entities for given service shortname path.
 */
struct ValidServiceConfigurationEntity {
  /*!
   * \brief The shortname path of the respective service.
   */
  ::vac::container::CStringView const kShortnamePath;
  /*!
   * \brief Validation data for the respective service.
   */
  ConfigValidationDataAccess const kConfigValidationDataAccess;
};

/*!
 * \brief Class to grant template independent access to the service config validation data.
 */
class ServiceConfigValidationDataAccess final {
 public:
  /*!
   * \brief Constructor.
   *
   * \param       kValidEntitiesPtr       Pointer to array of service configuration entities elements.
   * \param       kValidEntitiesArraySize Size of the array of service configuration entities elements.
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \spec
   *   requires kValidEntitiesPtr != nullptr;
   * \endspec
   */
  constexpr ServiceConfigValidationDataAccess(ValidServiceConfigurationEntity const* const kValidEntitiesPtr,
                                              size_t const kValidEntitiesArraySize)
      : kValidEntitiesPtr_{kValidEntitiesPtr}, kValidEntitiesArraySize_{kValidEntitiesArraySize} {}

  ServiceConfigValidationDataAccess(ServiceConfigValidationDataAccess&&) noexcept = delete;
  ServiceConfigValidationDataAccess(ServiceConfigValidationDataAccess const&) = delete;
  ServiceConfigValidationDataAccess& operator=(ServiceConfigValidationDataAccess const&) = delete;
  ServiceConfigValidationDataAccess& operator=(ServiceConfigValidationDataAccess&&) = delete;

  /*!
   * \brief Default destructor.
   * \spec
   *   requires true;
   * \endspec
   */
  ~ServiceConfigValidationDataAccess() = default;

  /*!
   * \brief Getter for the validated service config object at given index position.
   * \param[in] index The index.
   * \return The validated service config object.
   * \pre -
   * \context ANY
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  ValidServiceConfigurationEntity const& GetValidConfigurationEntity(size_t const index) const noexcept {
    if (index >= kValidEntitiesArraySize_) {
      amsr::core::Abort("Index of service validation data is out of bounds.");
    }
    // VECTOR NC AutosarC++17_10-M5.0.15: MD_SOMEIPBINDING_AutosarC++17_10-M5.0.15_Array_access_via_pointer
    // VECTOR NC AutosarC++17_10-M5.0.16: MD_SOMEIPBINDING_AutosarC++17_10-M5.0.16_Array_access_via_pointer
    // VCA_SOMEIPBINDING_RETURNED_VALUE_IS_VALID
    return kValidEntitiesPtr_[index];
  }

  /*!
   * \brief Returns the number of service configuration entity elements.
   * \return Number of service configuration entity elements.
   * \pre -
   * \context ANY
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  size_t GetServicesCount() const noexcept { return kValidEntitiesArraySize_; }

 private:
  /*!
   * \brief The pointer to the valid service configuration entity array.
   */
  ValidServiceConfigurationEntity const* const kValidEntitiesPtr_;
  /*!
   * \brief The size of the valid service configuration entity array.
   */
  size_t const kValidEntitiesArraySize_;
};

/*!
 * \brief Struct holding E2E parametrization data for proxy event xf.
 */
struct ProxyEventXfE2EParametrizationData {
  /*!
   * \brief The client side E2E transformer.
   */
  std::unique_ptr<::amsr::someip_binding_xf::internal::e2e::ClientSideTransformerInterface> e2e_transformer_uptr;
  /*!
   * \brief The E2E profile.
   */
  ::amsr::e2e::profiles::Profile e2e_profile{::amsr::e2e::profiles::Profile::Proprietary};
  /*!
   * \brief Flag indicating E2E check disabled.
   */
  bool is_e2e_check_disabled{false};

  /*!
   * \brief Contains the position of the e2e update bit in the message sample, if it is configured. Param is only used
   * for signal based events.
   */
  amsr::core::Optional<std::size_t> e2e_update_bit_position{amsr::core::nullopt};

  /*!
   * \brief Contains the offset of the e2e protected area in the message sample. Param is only used for signal based
   * events.
   */
  std::size_t e2e_protected_offset{0};

  /*!
   * \brief Contains the length for the e2e protected area in the message sample. Param is only used for signal based
   * events.
   */
  std::size_t e2e_protected_length{0};

  /*!
   * \brief Indicates if e2e protection for this signal based event is configured with the above mentioned paramters
   * (update bit, e2e offset, e2e protected length) or in a legacy way, which does not use these paramters. Param is
   * only used for signal based events.
   */
  bool has_signal_based_e2e_range{false};
};

/*!
 * \brief Struct holding E2E parametrization data for skeleton event xf.
 */
struct SkeletonEventXfE2EParametrizationData {
  /*!
   * \brief The E2E event protection props.
   */
  ::amsr::e2e::profiles::End2EndEventProtectionProps e2e_event_protection_props{};
  /*!
   * \brief The E2E profile.
   */
  ::amsr::e2e::profiles::Profile e2e_profile{::amsr::e2e::profiles::Profile::Proprietary};
};

}  // namespace configuration
}  // namespace internal
}  // namespace someip_binding
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_INCLUDE_AMSR_SOMEIP_BINDING_INTERNAL_CONFIGURATION_CONFIG_TYPES_H_
