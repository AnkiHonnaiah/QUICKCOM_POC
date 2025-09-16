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
/*!        \file  proxy_logger_util.h
 *        \brief  Logger utility class for proxy communication types.
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_PROXY_LOGGER_UTIL_H_
#define LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_PROXY_LOGGER_UTIL_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <string>

#include "amsr/core/string_view.h"
#include "ara/com/instance_identifier.h"
#include "ara/com/types_common.h"

namespace amsr {
namespace socal {
namespace internal {

/*!
 * \brief Logger utility class for proxy communication types.
 *
 * \unit Socal::Proxy::ProxyLoggerUtil
 */
class ProxyLoggerUtil {
 public:
  /*!
   * \brief Generates a unique logger prefix for Proxy communication types.
   * \param[in] prefix        The prefix to be added at the beginning.
   * \param[in] entity_name   The name of the entity to be added to the prefix.
   * \param[in] service_name  The service name to be added to the log prefix.
   * \param[in] instance_id   The instance identifier to be added to the log prefix.
   * \return The logger prefix constructed with the given data.
   * \pre         -
   * \context     App
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady FALSE
   * \spec requires true; \endspec
   */
  static std::string ConstructPrefix(std::string const& prefix, std::string const& entity_name,
                                     std::string const& service_name,
                                     ::ara::com::InstanceIdentifier const& instance_id) noexcept;
};

}  // namespace internal
}  // namespace socal
}  // namespace amsr

#endif  // LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_PROXY_LOGGER_UTIL_H_
