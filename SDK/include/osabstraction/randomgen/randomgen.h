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
/*!        \file  libosabstraction-posix/include/osabstraction/randomgen/randomgen.h
 *        \brief  Defines random numbers generation API for libosabstraction user
 *
 *      \details
 *
 *********************************************************************************************************************/

#ifndef LIB_COMMON_LIBOSABSTRACTION_POSIX_INCLUDE_OSABSTRACTION_RANDOMGEN_RANDOMGEN_H_
#define LIB_COMMON_LIBOSABSTRACTION_POSIX_INCLUDE_OSABSTRACTION_RANDOMGEN_RANDOMGEN_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/core/result.h"
#include "amsr/core/span.h"

#include "osabstraction/randomgen/internal/randomgen_internal.h"

namespace osabstraction {
namespace randomgen {

/*!
 * \brief           Generates a random seed into a buffer.
 *
 * \details         Blocks until enough random bytes are available to fill the buffer.
 *
 * \note a. This API is intended for use cases that require very high quality randomness such as one-time pad.
 *          In order to achieve that the implementation is supposed to use entropy pool
 *          and not any kind of PRNG (Pseudo-Random Number Generator).
 *          As a consequence the implementation is expected to block when entropy pool is empty
 *          until additional environmental noise is gathered.
 *       b. Due to implicit instantiation rules user can call the function as a normal one without specifying template
 *          parameter, for example:
 *          std::array<uint8_t, 10> arr;
 *          amsr::core::Span<uint8_t> buffer{arr};
 *          osabstraction::randomgen::GetRandSeed(buffer);
 *
 * \param[in,out]   buffer  The buffer for the random seed bytes. The buffer size must be greater than zero.
 *
 * \return          An empty result.
 *
 * \error           OsabErrc::kSystemEnvironmentError   Failed to read from random source. The support data contains the
 *                                                      POSIX error code.
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      FALSE
 *
 * \trace           DSGN-Osab-RandomNumberGeneration
 *
 * \vprivate        Vector product internal API
 * \spec
 *   requires true;
 * \endspec
 */
auto GetRandSeed(::amsr::core::Span<std::uint8_t> buffer) noexcept -> ::amsr::core::Result<void>;

/*!
 * \brief           Generates a random seed into a buffer.
 *
 * \details         Does not block. Succeeds only if there are enough random bytes available to fill the buffer.
 *
 * \param[in,out]   buffer  The buffer for the random seed bytes. The buffer size must be greater than zero.
 *
 * \return          An empty result.
 *
 * \error           OsabErrc::kSystemEnvironmentError   Failed to read from random source. The support data contains the
 *                                                      POSIX error code.
 * \error           OsabErrc::kBusy                     Not enough entropy left in random source to provide the required
 *                                                      amount of random seed bytes. Entropy is accumulated over time,
 *                                                      retry later.
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      FALSE
 *
 * \trace           DSGN-Osab-RandomNumberGeneration
 *
 * \vprivate        Vector product internal API
 * \spec
 *   requires true;
 * \endspec
 */
auto GetRandSeedNonBlocking(::amsr::core::Span<std::uint8_t> buffer) noexcept -> ::amsr::core::Result<void>;

}  // namespace randomgen
}  // namespace osabstraction

#endif  // LIB_COMMON_LIBOSABSTRACTION_POSIX_INCLUDE_OSABSTRACTION_RANDOMGEN_RANDOMGEN_H_
