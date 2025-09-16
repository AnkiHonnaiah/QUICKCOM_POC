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
 *        \brief  Defines OS independent names for signals.
 *         \unit  osabstraction::Signal_Linux
 *         \unit  osabstraction::Signal_QNX
 *   \complexity  The Signal_QNX unit slightly exceeds following metrics:
 *                - SUM, mTCYC: Due to SignalManager initialization, which requires multiple steps to be performed.
 *                  Functionality can be fully tested and makes no sense to split it into different units.
 *                - mIMC: Due to calls to OS APIs, cannot be avoided.
 *
 *********************************************************************************************************************/

#ifndef LIB_COMMON_LIBOSABSTRACTION_POSIX_INCLUDE_AMSR_SIGNAL_INTERNAL_SIGNAL_DEFINITIONS_H_
#define LIB_COMMON_LIBOSABSTRACTION_POSIX_INCLUDE_AMSR_SIGNAL_INTERNAL_SIGNAL_DEFINITIONS_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
// VECTOR Next Line AutosarC++17_10-M18.7.1: MD_OSA_M18.7.1_SignalHandling
#include <csignal>

namespace amsr {
namespace signal {
namespace internal {

/*!
 * \brief    POSIX signal data type.
 * \vprivate Vector product internal API
 */
using SignalIdentifier = int;  // VECTOR Same Line AutosarC++17_10-A3.9.1: MD_OSA_A3.9.1_PosixApi

// VECTOR Next Construct AutosarC++17_10-M18.7.1: MD_OSA_M18.7.1_SignalHandling
// VECTOR Next Construct AutosarC++17_10-M3.4.1: MD_OSA_M3.4.1_MinimizeIdentifierVisibility
/*!
 * \brief    Invalid signal identifier value.
 */
static constexpr SignalIdentifier kInvalidSignalIdentifier{0};

// VECTOR Next Construct AutosarC++17_10-M18.7.1: MD_OSA_M18.7.1_SignalHandling
// VECTOR Next Construct AutosarC++17_10-M3.4.1: MD_OSA_M3.4.1_MinimizeIdentifierVisibility
/*!
 * \brief    Maximum signal identifier value.
 */
static SignalIdentifier const kMaximumSignalIdentifier{SIGRTMAX};

// VECTOR Next Construct AutosarC++17_10-M18.7.1: MD_OSA_M18.7.1_SignalHandling
// VECTOR Next Construct AutosarC++17_10-M3.4.1: MD_OSA_M3.4.1_MinimizeIdentifierVisibility
/*!
 * \brief    POSIX signal SIGILL
 * \vprivate Vector product internal API
 */
static constexpr SignalIdentifier kSigIllIdentifier{SIGILL};

// VECTOR Next Construct AutosarC++17_10-M18.7.1: MD_OSA_M18.7.1_SignalHandling
// VECTOR Next Construct AutosarC++17_10-M3.4.1: MD_OSA_M3.4.1_MinimizeIdentifierVisibility
/*!
 * \brief    POSIX signal SIGBUS
 * \vprivate Vector product internal API
 */
static constexpr SignalIdentifier kSigBusIdentifier{SIGBUS};

// VECTOR Next Construct AutosarC++17_10-M18.7.1: MD_OSA_M18.7.1_SignalHandling
// VECTOR Next Construct AutosarC++17_10-M3.4.1: MD_OSA_M3.4.1_MinimizeIdentifierVisibility
/*!
 * \brief    POSIX signal SIGFPE
 * \vprivate Vector product internal API
 */
static constexpr SignalIdentifier kSigFpeIdentifier{SIGFPE};

// VECTOR Next Construct AutosarC++17_10-M18.7.1: MD_OSA_M18.7.1_SignalHandling
// VECTOR Next Construct AutosarC++17_10-M3.4.1: MD_OSA_M3.4.1_MinimizeIdentifierVisibility
/*!
 * \brief    POSIX signal SIGKILL
 * \vprivate Vector product internal API
 */
static constexpr SignalIdentifier kSigKillIdentifier{SIGKILL};

// VECTOR Next Construct AutosarC++17_10-M18.7.1: MD_OSA_M18.7.1_SignalHandling
// VECTOR Next Construct AutosarC++17_10-M3.4.1: MD_OSA_M3.4.1_MinimizeIdentifierVisibility
/*!
 * \brief    POSIX signal SIGUSR1
 * \vprivate Vector product internal API
 */
static constexpr SignalIdentifier kSigUsr1Identifier{SIGUSR1};

// VECTOR Next Construct AutosarC++17_10-M18.7.1: MD_OSA_M18.7.1_SignalHandling
// VECTOR Next Construct AutosarC++17_10-M3.4.1: MD_OSA_M3.4.1_MinimizeIdentifierVisibility
/*!
 * \brief    POSIX signal SIGSEGV
 * \vprivate Vector product internal API
 */
static constexpr SignalIdentifier kSigSegvIdentifier{SIGSEGV};

// VECTOR Next Construct AutosarC++17_10-M18.7.1: MD_OSA_M18.7.1_SignalHandling
// VECTOR Next Construct AutosarC++17_10-M3.4.1: MD_OSA_M3.4.1_MinimizeIdentifierVisibility
/*!
 * \brief    POSIX signal SIGCHLD
 * \vprivate Vector product internal API
 */
static constexpr SignalIdentifier kSigChldIdentifier{SIGCHLD};

// VECTOR Next Construct AutosarC++17_10-M18.7.1: MD_OSA_M18.7.1_SignalHandling
// VECTOR Next Construct AutosarC++17_10-M3.4.1: MD_OSA_M3.4.1_MinimizeIdentifierVisibility
/*!
 * \brief    Signal reserved for SignalManager internal use.
 * \vprivate Vector component internal API
 */
static constexpr SignalIdentifier kReservedSignalIdentifier{kSigUsr1Identifier};

// VECTOR Next Construct AutosarC++17_10-M18.7.1: MD_OSA_M18.7.1_SignalHandling
// VECTOR Next Construct AutosarC++17_10-M3.4.1: MD_OSA_M3.4.1_MinimizeIdentifierVisibility
/*!
 * \brief    POSIX signal SIGTERM code
 * \vprivate Vector product internal API
 */
static constexpr SignalIdentifier kSigTermIdentifier{SIGTERM};

// VECTOR Next Construct AutosarC++17_10-M18.7.1: MD_OSA_M18.7.1_SignalHandling
// VECTOR Next Construct AutosarC++17_10-M3.4.1: MD_OSA_M3.4.1_MinimizeIdentifierVisibility
/*!
 * \brief    POSIX signal SIGINT code
 * \vprivate Vector product internal API
 */
static constexpr SignalIdentifier kSigIntIdentifier{SIGINT};

// VECTOR Next Construct AutosarC++17_10-M18.7.1: MD_OSA_M18.7.1_SignalHandling
// VECTOR Next Construct AutosarC++17_10-M3.4.1: MD_OSA_M3.4.1_MinimizeIdentifierVisibility
/*!
 * \brief    POSIX signal SIGABRT code
 * \vprivate Vector product internal API
 */
static constexpr SignalIdentifier kSigAbrtIdentifier{SIGABRT};

// VECTOR Next Construct AutosarC++17_10-M18.7.1: MD_OSA_M18.7.1_SignalHandling
// VECTOR Next Construct AutosarC++17_10-M3.4.1: MD_OSA_M3.4.1_MinimizeIdentifierVisibility
/*!
 * \brief    POSIX signal SIGCONT code
 * \vprivate Vector product internal API
 */
static constexpr SignalIdentifier kSigContIdentifier{SIGCONT};

}  // namespace internal
}  // namespace signal
}  // namespace amsr

#endif  // LIB_COMMON_LIBOSABSTRACTION_POSIX_INCLUDE_AMSR_SIGNAL_INTERNAL_SIGNAL_DEFINITIONS_H_
