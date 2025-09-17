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
/**        \file  instance_specifier_lookup_table.h
 *        \brief  Define a lookup table class that relates instance specifiers to their corresponding
 *                instance identifiers.
 *
 *********************************************************************************************************************/
#ifndef LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_INSTANCE_SPECIFIER_LOOKUP_TABLE_H_
#define LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_INSTANCE_SPECIFIER_LOOKUP_TABLE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <map>
#include <memory>
#include <set>
#include <string>
#include <utility>
#include "amsr/core/instance_specifier.h"
#include "amsr/core/result.h"
#include "amsr/core/string_view.h"
#include "amsr/socal/internal/instance_specifier_lookup_table_key.h"
#include "amsr/socal/internal/logging/ara_com_logger.h"
#include "ara/com/instance_identifier.h"

namespace amsr {
namespace socal {
namespace internal {

/*!
 * \brief Alias for ::ara::com::InstanceIdentifier stored in the look up table.
 */
using InstanceSpecifierLookupTableEntry = ::ara::com::InstanceIdentifier;

/*!
 * \brief   Type alias for a container of InstanceSpecifierLookupTableEntry objects.
 * \details Sorted container required to allow implementation of operator< (e.g. FindServiceHandle::operator<).
 *
 * \trace SPEC-4980258
 */
using InstanceSpecifierLookupTableEntryContainer = std::set<InstanceSpecifierLookupTableEntry>;
// clang-format off
/*!
 * \brief   Manages the instance specifiers and relates them to their corresponding instance identifiers.
 * \details The lookup table stores the mapping of an InstanceSpecifier (key) along with a ServiceShortnamePath
 *          to 1..n mapping entries (each consisting of InstanceIdentifier).
 *
 * A lookup in the table can be done by
 * - InstanceSpecifier -> Return list of all mapped entries (InstanceSpecifierLookupTableEntry)
 * - InstanceIdentifier -> Return the InstanceSpecifierLookupTableEntry of the searched-for InstanceIdentifier
 *
 * Lookup table layout (map):
 *   key                     values (1..n)
 *   (InstanceSpecifier)     (InstanceSpecifierLookupTableEntry storing
 *                            InstanceIdentifier, ServiceShortnamePath)
 * +-------------------------------------------------------------------------------------------------------------+
 * |  {"exec/rootSwc/rport0", "/SWC/ServiceInterface/MyServiceA" }  | [ "Ipc:A:B:C",                             |
 * |                                                                |   "SomeIp:X:Y"                             |
 * |                                                                | ]                                          |
 * +-------------------------------------------------------------------------------------------------------------+
 * |  {"exec/rootSwc/rport1", "/SWC/ServiceInterface/MyServiceB" }  | [ "Ipc:X:Y:Z" ]                            |
 * +-------------------------------------------------------------------------------------------------------------+
 * \unit Socal::InstanceSpecifierLookupTable::InstanceSpecifierLookupTable
 */
// clang-format on
// VCA_SOCAL_COMPILER_GENERATED_DESTRUCTOR
class InstanceSpecifierLookupTable {
 public:
  /*!
   * \brief Constructor
   * \param[in] logger  Reference to the logger to be used for logging.
   * \pre     -
   * \context Init
   * \steady FALSE
   * \spec requires true; \endspec
   */
  explicit InstanceSpecifierLookupTable(::amsr::socal::internal::logging::AraComLogger const& logger) noexcept;

  /*!
   * \brief Add a new InstanceSpecifier entry to the lookup table.
   * \param[in] instance_specifier      The instance specifier which will be the key for the new entry.
   * \param[in] service_shortname_path  ShortNamePath of the mapped service interface.
   * \pre     -
   * \context Init
   * \steady FALSE
   * \spec requires true; \endspec
   */
  void AddInstanceSpecifier(::amsr::core::InstanceSpecifier const& instance_specifier,
                            ServiceShortNamePath const& service_shortname_path) noexcept;

  /*!
   * \brief   Add a new mapping entry between a InstanceSpecifier and a InstanceIdentifier.
   * \details This API must be called multiple times to register all known mappings. Example to initialize mapping of
   *          one InstanceSpecifier to two InstanceIdentifiers.
   *          MapInstanceSpecifierToInstanceId(InstanceSpecifier("exec/rootSwc/rport0"), InstanceIdentifier("Ipc:5"));
   *          MapInstanceSpecifierToInstanceId(InstanceSpecifier("exec/rootSwc/rport0"), InstanceIdentifier("Ipc:78"));
   * \param[in] instance_specifier   The instance specifier which is the key for the new entry.
   * \param[in] instance_identifier  The instance identifier which will be the value for the new entry. Expected
   *                                 format: "<binding_name>:<binding specific instance ID>"
   * \return True if the entry is added successfully, false if the entry is already present in the lookup table.
   * \pre         -
   * \context     BindingInit
   * \threadsafe  FALSE
   * \synchronous TRUE
   * \steady FALSE
   * \spec requires true; \endspec
   */
  bool MapInstanceSpecifierToInstanceId(::amsr::core::InstanceSpecifier const& instance_specifier,
                                        ::ara::com::InstanceIdentifier const& instance_identifier) noexcept;

  /*!
   * \brief Removes the mapping between the given InstanceSpecifier and InstanceIdentifier.
   * \param[in] instance_specifier   The instance specifier.
   * \param[in] instance_identifier  The instance identifier to be deleted.
   * \return True if the entry is found and deleted, false otherwise.
   * \pre     -
   * \context BindingShutdown
   * \steady FALSE
   * \spec requires true; \endspec
   */
  bool RemoveInstanceSpecifierEntry(::amsr::core::InstanceSpecifier const& instance_specifier,
                                    ::ara::com::InstanceIdentifier const& instance_identifier) noexcept;

  /*!
   * \brief Returns all InstanceIdentifiers which match the provided instance specifier.
   * \param[in] instance_specifier      The provided InstanceSpecifier which is used to look up the corresponding
   *                                    service InstanceIdentifiers.
   * \param[in] service_shortname_path  Optional service shortname path defines the service of the calling proxy or
   *                                    skeleton. If the provided InstanceSpecifier is not pointing to a PortPrototype
   *                                    referencing the same service, the InstanceSpecifier is treated as invalid.
   *                                    Default: An empty shortname path which disabled the check. Optionality of this
   *                                    parameter required for ara::com::runtime::ResolveInstanceIDs() use-case where a
   *                                    lookup without a concrete ServiceShortNamePath is done.
   * \return A container of matching entries (InstanceSpecifierLookupTableEntry). The list can be empty in case no
   *         matching entry exists.
   * \error ara::com::ComErrc::kUnknownInstanceSpecifier  If the InstanceSpecifier was not found in the lookup table.
   * \pre         -
   * \context     App
   * \synchronous TRUE
   * \steady FALSE
   * \spec requires true; \endspec
   *
   * \trace SPEC-8053578
   */
  ::amsr::core::Result<InstanceSpecifierLookupTableEntryContainer> Resolve(
      ::amsr::core::InstanceSpecifier const& instance_specifier,
      ServiceShortNamePath const& service_shortname_path = ::amsr::core::StringView{}) const noexcept;

  /*!
   * \brief Returns the entry (InstanceSpecifierLookupTableEntry) for the searched-for InstanceIdentifier.
   * \param[in] instance_identifier     An InstanceIdentifier which contains the binding type and the binding specific
   *                                    instance ID in string representation. Expected format: "<Binding
   *                                    type/prefix>:<binding specific instance ID>"
   * \param[in] service_shortname_path  Optional service shortname path defines the service of the calling proxy or
   *                                    skeleton.
   * \return An entry (InstanceSpecifierLookupTableEntry) containing the InstanceIdentifier.
   * \error ara::com::ComErrc::kUnknownInstanceIdentifier  If the InstanceIdentifier was not found in the lookup table.
   * \pre         -
   * \context     App
   * \synchronous TRUE
   * \steady FALSE
   * \spec requires true; \endspec
   */
  ::amsr::core::Result<InstanceSpecifierLookupTableEntry> Resolve(
      ::ara::com::InstanceIdentifier const& instance_identifier,
      ServiceShortNamePath const& service_shortname_path = ::amsr::core::StringView{}) const noexcept;

  /*!
   * \brief Returns the InstanceSpecifier for the searched-for InstanceIdentifier.
   * \param[in] instance_identifier     An InstanceIdentifier which contains the binding type and the binding specific
   *                                    instance ID in string representation. Expected format: "<Binding
   *                                    type/prefix>:<binding specific instance ID>"
   * \param[in] service_shortname_path  The service shortname path defines the service of the calling proxy or skeleton.
   * \return InstanceSpecifier for the binding specific instance identifier.
   * \pre         The instance identifier must be known.
   * \context     App
   * \synchronous TRUE
   * \steady FALSE
   * \spec requires true; \endspec
   */
  ::amsr::core::InstanceSpecifier ResolveInstanceSpecifier(
      ::ara::com::InstanceIdentifier const& instance_identifier,
      ServiceShortNamePath const& service_shortname_path) const noexcept;

  /*!
   * \brief Clears the contents of the look up table.
   * \pre     -
   * \context Shutdown
   * \steady FALSE
   * \spec requires true; \endspec
   */
  void Clear() noexcept;

  /*!
   * \brief Constructs an instance specifier lookup table entry container from the provided instance specifier and
   *        shortname path.
   * \param[in] instance_specifier      The instance specifier from which the entry should be constructed.
   * \param[in] service_shortname_path  The service shortname path defines the service of the calling proxy or skeleton.
   * \return The matching instance specifier lookup table entry container.
   * \pre         The instance specifier must be known.
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady FALSE
   * \spec requires true; \endspec
   */
  InstanceSpecifierLookupTableEntryContainer ConstructEntryContainerFromSpecifier(
      ::amsr::core::InstanceSpecifier const& instance_specifier,
      ServiceShortNamePath const& service_shortname_path) const noexcept;

  /*!
   * \brief Constructs an instance specifier lookup table entry container from the provided instance identifier and
   *        shortname path.
   * \param[in] instance_id             The instance identifier from which the entry container should be constructed.
   * \param[in] service_shortname_path  The service shortname path defines the service of the calling proxy or skeleton.
   * \return The matching instance specifier lookup table entry container.
   * \pre         -
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady FALSE
   * \spec requires true; \endspec
   */
  InstanceSpecifierLookupTableEntryContainer ConstructEntryContainerFromIdentifier(
      ::ara::com::InstanceIdentifier const& instance_id,
      ServiceShortNamePath const& service_shortname_path) const noexcept;

  /*!
   * \brief Constructs an instance specifier lookup table entry container from the provided instance identifier
   *        container and shortname path.
   * \param[in] instances               The InstanceIdentifierContainer from which the entry should be constructed.
   * \param[in] service_shortname_path  The service shortname path defines the service of the calling proxy or skeleton.
   * \return The matching instance specifier lookup table entry container.
   * \pre         The InstanceIdentifierContainer shall not be empty.
   * \pre         All the InstanceIdentifier must be unique.
   * \pre         All the InstanceIdentifier belong to the same ProvidedPortPrototype / ServiceInterface.
   * \pre         The instance identifier must be known.
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady FALSE
   * \spec requires true; \endspec
   *
   * \trace SPEC-8053493
   */
  InstanceSpecifierLookupTableEntryContainer ConstructEntryContainerFromIdentifierContainer(
      ::ara::com::InstanceIdentifierContainer const& instances,
      ServiceShortNamePath const& service_shortname_path) const noexcept;

 private:
  /*!
   * \brief Constructs an instance specifier lookup table entry from the provided instance identifier and shortname
   *        path.
   * \param[in] instance_id             The instance identifier from which the entry should be constructed.
   * \param[in] service_shortname_path  The service shortname path defines the service of the calling proxy or skeleton.
   * \return The matching instance specifier lookup table entry.
   * \pre         The instance identifier must be known.
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady FALSE
   * \spec requires true; \endspec
   */
  InstanceSpecifierLookupTableEntry ConstructEntryFromIdentifier(
      ::ara::com::InstanceIdentifier const& instance_id,
      ServiceShortNamePath const& service_shortname_path) const noexcept;

  /*!
   * \brief Type for a map that handles the correlation between InstanceSpecifier and InstanceIdentifier.
   */
  using InstanceSpecifierMap = std::map<InstanceSpecifierLookupTableKey, InstanceSpecifierLookupTableEntryContainer>;

  /*!
   * \brief logger.
   */
  ::amsr::socal::internal::logging::AraComLogger logger_;

  /*!
   * \brief Container storing mapping between InstanceSpecifier and the related InstanceIdentifiers.
   */
  InstanceSpecifierMap lookup_table_;
};

}  // namespace internal
}  // namespace socal
}  // namespace amsr

#endif  // LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_INSTANCE_SPECIFIER_LOOKUP_TABLE_H_
