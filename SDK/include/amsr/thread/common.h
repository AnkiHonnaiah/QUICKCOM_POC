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
 *        \brief  Common data in the thread namespace.
 *
 *********************************************************************************************************************/
#ifndef LIB_VATHREAD_INCLUDE_AMSR_THREAD_COMMON_H_
#define LIB_VATHREAD_INCLUDE_AMSR_THREAD_COMMON_H_

#include <array>
#include <climits>

namespace amsr {
namespace thread {

/*!
 * \brief Provide a maximum length for the name of a thread.
 * \details The given size defines the number of usable characters excluding zero termination.
 * \vpublic
 */
constexpr std::size_t kMaxSizeThreadName{15};

/*!
 * \brief Default thread stack size in bytes.
 * \details A stack size of 0 lets the implementation determine a default minimum thread stack size.
 * \vpublic
 */
constexpr std::size_t kDefaultThreadStackSize{0};

/*!
 * \brief Alias for the storage of the thread name.
 * \vpublic
 */
using ThreadNameStorage = std::array<char, kMaxSizeThreadName + 1>;

}  // namespace thread
}  // namespace amsr

#endif  // LIB_VATHREAD_INCLUDE_AMSR_THREAD_COMMON_H_
