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
/*! \file
 *  \brief Version IpcServiceDiscovery.
 *  \unit IpcServiceDiscovery::Common::Version
 *
 *********************************************************************************************************************/
#ifndef LIB_IPC_SERVICE_DISCOVERY_COMMON_INCLUDE_AMSR_IPC_SERVICE_DISCOVERY_COMMON_INTERNAL_VERSION_VERSION_H_
#define LIB_IPC_SERVICE_DISCOVERY_COMMON_INCLUDE_AMSR_IPC_SERVICE_DISCOVERY_COMMON_INTERNAL_VERSION_VERSION_H_

#include <cstdint>
#include <utility>
#include "ara/log/logstream.h"

namespace amsr {
namespace ipc_service_discovery {
namespace common {
namespace internal {

/*! Version of IpcServiceDiscovery. */
class Version {
 public:
  /*!
   * \brief Major version type.
   */
  using MajorVersion = std::uint32_t;

  /*!
   * \brief Minor version type.
   */
  using MinorVersion = std::uint32_t;

  // VECTOR NC VectorC++-V11.0.2: MD_IPCSERVICEDISCOVERY_V11.0.2_inAClassAllMutableDataMembersShallBePrivate
  MajorVersion major_version{};
  // VECTOR NC VectorC++-V11.0.2: MD_IPCSERVICEDISCOVERY_V11.0.2_inAClassAllMutableDataMembersShallBePrivate
  MinorVersion minor_version{};

  /*!
   * \brief Match the Version against another.
   * \param[in] other Version to compare against.
   * \return True if matching, false if different.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component internal.
   */
  constexpr auto IsMatching(Version const& rhs) const noexcept -> bool { return major_version == rhs.major_version; }

  /*!
   * \brief Compare the Version against another for equality.
   * \param[in] other Version to compare against.
   * \return True if equal, false if different.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component internal.
   */
  constexpr auto operator==(Version const& rhs) const noexcept -> bool {
    return std::tie(major_version, minor_version) == std::tie(rhs.major_version, rhs.minor_version);
  }
};

/*!
 * \brief Log streaming operator for type Version.
 * \param[in] log_stream A reference to the LogStream.
 * \param[in] version A const reference to the Version.
 * \return The reference to the LogStream.
 * \context ANY
 * \pre -
 * \threadsafe FALSE
 * \reentrant FALSE
 * \synchronous TRUE
 * \vprivate Component Private
 */
// clang-format off
inline auto operator<<(::ara::log::LogStream& log_stream,
                       Version const& version) noexcept -> ::ara::log::LogStream& /* COV_IpcSD_utility */ {
  // clang-format on
  log_stream << "Version: ";
  log_stream << "[";
  log_stream << "MajorVersion: " << version.major_version << ", ";
  log_stream << "MinorVersion: " << version.minor_version;
  return log_stream << "]";
}

}  // namespace internal
}  // namespace common
}  // namespace ipc_service_discovery
}  // namespace amsr

#endif  // LIB_IPC_SERVICE_DISCOVERY_COMMON_INCLUDE_AMSR_IPC_SERVICE_DISCOVERY_COMMON_INTERNAL_VERSION_VERSION_H_

// COV_JUSTIFICATION_BEGIN
// \ID COV_IpcSD_utility
//   \ACCEPT  XX
//   \REASON  FunctionCoverage gap is reported for the API.
//            Since the API is a trivial logging function and tested by unit tests
//            no component test function coverage needs to be achieved for this function.
// COV_JUSTIFICATION_END
