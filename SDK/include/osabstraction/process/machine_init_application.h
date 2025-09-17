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
 *        \brief  Implements a service to execute MachineInit Application on a POSIX operating system.
 *        \unit   osabstraction::Process_Linux
 *        \unit   osabstraction::Process_QNX
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_COMMON_LIBOSABSTRACTION_POSIX_INCLUDE_OSABSTRACTION_PROCESS_MACHINE_INIT_APPLICATION_H_
#define LIB_COMMON_LIBOSABSTRACTION_POSIX_INCLUDE_OSABSTRACTION_PROCESS_MACHINE_INIT_APPLICATION_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstdint>
#include <system_error>
#include "vac/container/c_string_view.h"

namespace osabstraction {
namespace process {
namespace machine_init_application {

/*!
 * \brief Stores the result of the MachineInit Execution.
 *
 * \details
 * The operator bool() overloads provides an easy way to check if the MachineInit Application has been executed
 * successfully. The result is positive if the MachineInit application returned with  EXIT_SUCCESS.
 *
 * A non existing file shall be ignored and thus is treated as positive result.
 *
 * \vprivate Vector product internal API
 */
class Result final {
 public:
  /*! \brief Alias for a status code returned by waitpid(). */
  using Status = std::int32_t;

  /*!
   * \brief Constructs a MachineInit Application result.
   *
   * \param[in] file_exists             True if the MachineInit Application exits. Otherwise false.
   * \param[in] process_creation_result Result of the process creation API.
   * \param[in] process_exit_status     Status returned by the MachineInit Application.
   *
   * \vprivate Vector product internal API
   *
   * \spec
   *   requires true;
   * \endspec
   */
  Result(bool file_exists, std::error_code process_creation_result, Status process_exit_status)
      : file_exists_{file_exists},
        process_creation_result_{process_creation_result},
        process_exit_status_{process_exit_status} {}

  /*!
   * \brief Checks whether the operation succeeded or failed.
   *
   * \details A non existing file shall be ignored and thus is treated as positive result.
   *
   * \return True if the MachineInit Application returned successfully. Otherwise false.
   *
   * \vprivate Vector product internal API
   *
   * \spec
   *   requires true;
   * \endspec
   */
  explicit operator bool() const noexcept;

  /*!
   * \brief Returns whether the passed MachineInitPath exists or not.
   *
   * \details
   * The default path is treated as passed path in this function.
   *
   * \return True if the path exists otherwise false.
   *
   * \context     InitPhase
   *
   * \pre         -
   *
   * \reentrant   FALSE
   * \synchronous TRUE
   * \threadsafe  FALSE
   *
   * \trace       DSGN-Osab-Process-MachineInitApp
   *
   * \vprivate Vector product internal API
   *
   * \spec
   *   requires true;
   * \endspec
   *
   */
  bool DoesPathExist() const noexcept { return file_exists_; }

  /*!
   * \brief Default copy constructor.
   *
   * \vprivate Vector product internal API
   *
   * \spec
   *   requires true;
   * \endspec
   */
  Result(Result const&) noexcept = default;

  /*!
   * \brief Default move constructor.
   *
   * \vprivate Vector product internal API
   *
   * \spec
   *   requires true;
   * \endspec
   */
  Result(Result&&) noexcept = default;

  /*!
   * \brief Default assignment operator.
   * \return Reference to this object.
   *
   * \vprivate Vector product internal API
   *
   * \spec
   *   requires true;
   * \endspec
   */
  Result& operator=(Result const&) & noexcept = default;

  /*!
   * \brief Default move operator.
   * \return Reference to this object.
   *
   * \vprivate Vector product internal API
   *
   * \spec
   *   requires true;
   * \endspec
   */
  Result& operator=(Result&&) & noexcept = default;

  /*!
   * \brief Default destructor.
   *
   * \vprivate Vector product internal API
   *
   * \spec
   *   requires true;
   * \endspec
   */
  ~Result() = default;

  Result() = delete; /*! \brief no default constructor supported. */

 private:
  bool file_exists_;                        /*!< Stores if the path to the MachineInit Application exists or not. */
  std::error_code process_creation_result_; /*!< The return code of OsProcess::CreateProcess. */
  Status process_exit_status_;              /*!< The exit code returned by the MachineInit Application. */
};

/*!
 * \brief Executes the MachineInit Application.
 *
 * \details
 * Creates the MachineInit Application with the current process properties and waits for its termination. It then
 * returns the result to the caller.
 *
 * If an empty path is provided then the service will fall back to an operating system specific default path. Also
 * note, that a non existing file is ignored and the returned result is positive.
 *
 * \return      Result containing detailed information of the operation.
 *
 * \context     InitPhase
 *
 * \pre         -
 *
 * \reentrant   FALSE
 * \synchronous TRUE
 * \threadsafe  FALSE
 * \steady      FALSE
 *
 * \param[in]   path_to_application   Path to Machine Init Application.
 *
 * \trace       DSGN-Osab-Process-MachineInitApp
 *
 * \vprivate Vector product internal API
 *
 * \spec
 *   requires true;
 * \endspec
 */
Result RunMachineInitApplication(vac::container::CStringView const& path_to_application);

}  // namespace machine_init_application
}  // namespace process
}  // namespace osabstraction

#endif  // LIB_COMMON_LIBOSABSTRACTION_POSIX_INCLUDE_OSABSTRACTION_PROCESS_MACHINE_INIT_APPLICATION_H_
