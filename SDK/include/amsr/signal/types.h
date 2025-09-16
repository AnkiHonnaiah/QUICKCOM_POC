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
/**        \file
 *        \brief  public signal handling types.
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_SIGNAL_TYPES_H_
#define LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_SIGNAL_TYPES_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/signal/internal/signal_definitions.h"
#include "vac/language/unique_function.h"

namespace amsr {
namespace signal {

/*!
 * \brief Amount of supported signals.
 */
static constexpr std::size_t kNumberOfSupportedSignals{3};

/*!
 * \brief           SignalManager supported signals
 * \trace           DSGN-Osab-SignalManagerSupportedSignals
 * \vprivate        Vector product internal API
 */
enum class SupportedSignal : internal::SignalIdentifier {
  kSigTerm = internal::kSigTermIdentifier, /*!< \brief The SIGTERM enum value */
  kSigInt = internal::kSigIntIdentifier,   /*!< \brief The SIGINT enum value */
  kSigChld = internal::kSigChldIdentifier, /*!< \brief The SIGCHLD enum value */
};

/*!
 * \brief           SignalManager notification type
 * \vprivate        Vector product internal API
 */
enum class SignalNotificationType : bool {
  kOneShot,   /*!< \brief Only the first pending signal will be notified. */
  kContinuous /*!< \brief Every time the signal is pending will be notified. */
};

// forward declaration to avoid cycles in the include graph
class SupportedSignalInfo;

/*!
 * \brief    User callback for signal notification.
 * \details  Used by the SignalManager and defined by the user to receive notifications upon registered signal
 *           reception.
 * \vprivate Vector product internal API
 */
using SignalCallback = vac::language::UniqueFunction<void(SupportedSignalInfo const&)>;

}  // namespace signal
}  // namespace amsr

#endif  // LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_SIGNAL_TYPES_H_
