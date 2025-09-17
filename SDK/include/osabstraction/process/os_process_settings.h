/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2024 by Vector Informatik GmbH.                                              All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file  libosabstraction-posix/include/osabstraction/process/os_process_settings.h
 *        \brief  Process API - Abstraction of Process management for POSIX - Settings for starting a process.
 *         \unit  osabstraction::Process_Linux
 *         \unit  osabstraction::Process_QNX
 *   \complexity  The osabstraction::Process_QNX unit slightly exceeds following metrics:
 *                - SUM, tCYC, mTCYC: Reason is that process creation requires different steps to apply the configured
 *                  settings for the newly created process. These are sequential steps which can be fully tested
 *                  without need of splitting the process creation functionality into different units.
 *                - mIMU: Cannot be avoided, process creation requires many interaction with the process settings unit.
 *
 *********************************************************************************************************************/

#ifndef LIB_COMMON_LIBOSABSTRACTION_POSIX_INCLUDE_OSABSTRACTION_PROCESS_OS_PROCESS_SETTINGS_H_
#define LIB_COMMON_LIBOSABSTRACTION_POSIX_INCLUDE_OSABSTRACTION_PROCESS_OS_PROCESS_SETTINGS_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <utility>
#include "osabstraction/process/ability_configuration.h"
#include "osabstraction/process/internal/ability.h"
#include "osabstraction/process/internal/resource_limit.h"
#include "osabstraction/process/process_types.h"
#include "osabstraction/thread/thread_types.h"
#include "vac/container/c_string_view.h"
// VECTOR Next Line AutosarC++17_10-M18.7.1: MD_OSA_M18.7.1_SignalHandling
#include "amsr/core/optional.h"
#include "amsr/core/result.h"
#include "amsr/signal/internal/signal_notification_entry.h"
#include "ara/core/vector.h"

namespace osabstraction {
namespace process {

// VECTOR Next Construct Metric-OO.WMC.One: MD_OSA_Metric-OO.WMC.One_OsProcessSettings
/*!
 * \brief   Settings to create a process.
 *
 * \details One object of this type contains information to create one process.
 *          The following information is contained:
 *          - command line arguments that shall be passed to the process
 *          - environment variables that shall be set for the process
 *          - scheduling settings that shall be set for the process
 *          - CPU cores on which the process shall be able to run
 *          - user ID that shall be set for the process
 *          - primary group ID that shall be set for the process
 *          - secondary group IDs that shall be set for the process
 *          - inherit secondary groups flag that shall be set for the process
 *          Each information is optional (from the point of view of this class, the consumer of the information may
 *          require some or all information mandatorily), this is realized using amsr::core::Optional.
 *          Contained information must be set using the Set* member function and can be acquired using the Get* member
 *          function for the respective information.
 *          The information contained in an object of this class are consumed by the CreateProcess member function of
 *          the OsProcess class.
 */
class OsProcessSettings final {  // VCA_OSA_MOLE_1298
 public:
  /*!
   * \brief           Reserves memory for environment variables and the argument vector.
   *
   * \context         InitPhase
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          FALSE
   *
   * \trace           DSGN-Osab-Process-ProcessCreation-Linux
   * \trace           DSGN-Osab-Process-ProcessCreation-QNX
   * \trace           DSGN-Osab-Process-ProcessCreationOptionalSettings
   *
   * \spec
   *   requires true;
   * \endspec
   */
  /*!
   * \internal
   * - Reserve memory for the pointers to the c string representations of the command line arguments that shall be
   *   passed to the process.
   * - Reserve memory for the pointers to the c string representations of the environment variables that shall be set
   *   for the process.
   * \endinternal
   */
  // VECTOR Next Construct AutosarC++17_10-A15.4.2: MD_OSA_A15.4.2_Allocation
  OsProcessSettings() noexcept {
    // additional storage will be reserved if arguments are added
    argv_.reserve(kMinArgvSize);  // VCA_OSA_STL_CALL_OBJ_PASS_VALUE

    // additional storage will be reserved if environment variables are added
    envp_.reserve(kMinEnvpSize);  // VCA_OSA_STL_CALL_OBJ_PASS_VALUE
  }

  /*!
   * \brief Deleted Copy constructor.
   */
  OsProcessSettings(OsProcessSettings const&) noexcept = delete;

  /*!
   * \brief Copy assignment.
   */
  OsProcessSettings& operator=(OsProcessSettings const&) & noexcept = delete;

  /*!
   * \brief Move constructor.
   *
   * \steady    FALSE
   * \spec
   *   requires true;
   * \endspec
   */
  OsProcessSettings(OsProcessSettings&&) noexcept = default;

  /*!
   * \brief Deleted Move assignement.
   */
  OsProcessSettings& operator=(OsProcessSettings&& other) & noexcept = delete;

  /*!
   * \brief Default destructor.
   * \steady    FALSE
   *
   * \spec
   *   requires true;
   * \endspec
   */
  ~OsProcessSettings() noexcept = default;  // VCA_OSA_EXT_CALL_OBJ_DTOR, VCA_OSA_STL_CALL_OBJ_DTOR

  /*!
   * \brief           Sets command line arguments that shall be passed to the process.
   *
   * \details         Not calling this function on a settings object or passing an empty list of arguments means that no
   *                  arguments (other than the executable name) should be passed to a process that is created with
   *                  these settings.
   *
   * \param[in]       args                      List of command line arguments which shall be passed to the process.
   *
   * \context         InitPhase
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          FALSE
   *
   * \trace           DSGN-Osab-Process-ProcessCreationOptionalSettings
   *
   * \vprivate        Vector product internal API
   *
   * \spec
   *   requires true;
   * \endspec
   */
  void SetArguments(Arguments args) noexcept;

  /*!
   * \brief           Adds an environment variable that shall be set for the process.
   *
   * \details         Not calling this function on a settings object means that no environment variables should be set
   *                  for a process that is created with these settings.
   *
   * \param[in]       envvar_name               Name of the environment variable which shall be added, must not contain
   *                                            an equals character '='.
   * \param[in]       envvar_value              Value of the environment variable which shall be added.
   *
   * \note            Should an environment variable with that name have been set before, the value will be overwritten
   *                  by the value that is passed later.
   *
   * \context         InitPhase
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          FALSE
   *
   * \trace           DSGN-Osab-Process-ProcessCreationOptionalSettings
   *
   * \vprivate        Vector product internal API
   *
   * \spec
   *   requires true;
   * \endspec
   */
  void AddEnvironmentVar(EnvironmentVar envvar_name, EnvironmentVar const& envvar_value) noexcept;

  /*!
   * \brief                 Reads the environment variables and sets them for the process.
   *
   * \context               InitPhase
   *
   * \pre                   -
   *
   * \reentrant             FALSE
   * \synchronous           TRUE
   * \threadsafe            FALSE
   *
   */
  void InheritProcessEnvironmentVariables() noexcept;

  /*!
   * \brief   Type that states the result of ReplaceEnvironmentVar.
   */
  enum class ReplaceResult : std::uint8_t {
    kOk = 0x0u,        /*!<  Operation successful. */
    kNotFound = 0x01u, /*!<  The environment variable name was not found. */
    kTooLong = 0x02u   /*!<  The environment variable value is too long. */
  };

  /*!
   * \brief           Replaces the value of an existing environment variable.
   *
   * \param[in]       envvar_name               Name of the environment variable whose value shall be replaced.
   * \param[in]       envvar_value              Value to set for the environment variable.
   *
   * \return          OsProcessSettings::ReplaceResult::kNotFound if a variable named envvar_name does not exist
   *                  OsProcessSettings::ReplaceResult::kTooLong  if envvar_value is too long.
   *                  OsProcessSettings::ReplaceResult::kOk       if the value of the variable has been replaced.
   *
   * \context         ANY|!InitPhase
   *
   * \pre            -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          FALSE
   *
   * \trace           DSGN-Osab-Process-ProcessCreationOptionalSettings
   *
   * \vprivate        Vector product internal API
   *
   * \spec
   *   requires true;
   * \endspec
   */
  ReplaceResult ReplaceEnvironmentVar(vac::container::CStringView envvar_name,
                                      EnvironmentVar const& envvar_value) noexcept;

  /*!
   * \brief           Adds an ability that shall be set for the process, during process creation by
   *                  OsProcess::CreateProcess() API.
   *
   * \details         Not calling this function on a settings object means that no additional abilities
   *                  to the OS default ones or the inherited ones should be set for a process that is
   *                  screated with these settings.
   *                  If the ability does not already exist it will be created and will only be set to the
   *                  process that is created with this settings.
   *                  If the ability already exists (i.e. QNX native ability) the provided parameters will be
   *                  added to the already existing ones.
   *
   * \param[in]       name               Name of the ability which shall be configured.
   * \param[in]       configs            Configured settings for passed ability.
   * \param[in]       inherit            Inheritance setting of passed ability.
   * \param[in]       lock               Lock setting of passed ability.
   *
   * \note            Only implemented on QNX.
   * \return          Empty result if operation is supported by the OS.
   * \error           osabstraction::OsabErrc::kApiError       Creating the given ability is not supported (Linux).
   * \context         InitPhase
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          FALSE
   *
   * \trace           DSGN-Osab-Process-ProcessCreationOptionalSettings
   * \trace           DSGN-Osab-Process-ProcessStartupQNXAbility
   *
   * \vprivate        Vector product internal API
   *
   * \spec
   *   requires true;
   * \endspec
   */
  ::amsr::core::Result<void> AddStartupAbility(AbilityName const& name,
                                               ara::core::Vector<AbilityConfigurationEntry> const& configs,
                                               AbilityInheritance const& inherit, AbilityLock const& lock);

  /*!
   * \brief           Adds an ability that shall be set for the process on demand, after process creation, by
   *                  OsProcess::SetRunningAbilities() API.
   *
   * \details         Not calling this function on a settings object means that calling OsProcess::SetRunningAbilities()
   *                  with this OsProcessSettings object will fail.
   *                  If the ability does not already exist it will be created and will only be updated for the
   *                  process on which the OsProcess::SetRunningAbilities() will be called.
   *                  If the ability already exists (i.e. QNX native ability) the provided parameters will be
   *                  added to the already existing ones.
   *
   * \param[in]       name               Name of the ability which shall be configured.
   * \param[in]       configs            Configured settings for passed ability.
   * \param[in]       inherit            Inheritance setting of passed ability.
   * \param[in]       lock               Lock setting of passed ability.
   *
   * \note            Only implemented on QNX.
   * \return          Empty result if operation is supported by the OS.
   * \error           osabstraction::OsabErrc::kApiError       Creating the given ability is not supported (Linux).
   * \context         ANY|!InitPhase
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          TRUE
   *
   * \trace           DSGN-Osab-Process-ProcessRunningQNXAbility
   *
   * \vprivate        Vector product internal API
   */
  ::amsr::core::Result<void> AddRunningAbility(AbilityName const& name,
                                               ara::core::Vector<AbilityConfigurationEntry> const& configs,
                                               AbilityInheritance const& inherit, AbilityLock const& lock);

  /*!
   * \brief           Adds a resource limit that shall be set for the process.
   * \param[in]       name               Name of the resource limit which shall be set.
   * \param[in]       value              Value of the resource limit which shall be set.
   * \return          Empty result if operation is supported by the OS.
   * \error           osabstraction::OsabErrc::kApiError   feature not supported by the OS.
   * \context         InitPhase
   * \pre             -
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          FALSE
   * \trace           DSGN-Osab-Process-ProcessCreationOptionalSettings
   * \trace           DSGN-Osab-Process-ProcessResourceLimit
   * \vprivate        Vector product internal API
   * \spec
   *   requires true;
   * \endspec
   */
  ::amsr::core::Result<void> AddResourceLimit(ResourceLimitName const& name, ResourceLimitValue const& value);

  /*!
   * \brief           Sets scheduling policy and priority that shall be used for the process.
   *
   * \details         Not calling this function on a settings object means default scheduling priority and policy should
   *                  be used when a process with these settings is created.
   *
   * \param[in]       scheduling_policy         Scheduling policy of the process.
   * \param[in]       scheduling_priority       Scheduling priority of the process.
   *
   * \context         InitPhase
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          FALSE
   *
   * \trace           DSGN-Osab-Process-ProcessCreationOptionalSettings
   *
   * \vprivate        Vector product internal API
   *
   * \spec
   *   requires true;
   * \endspec
   */
  void SetSchedulingSettings(osabstraction::thread::SchedulingPolicies scheduling_policy,
                             osabstraction::thread::SchedulingPriority scheduling_priority) {
    SchedulingSettings const sched_settings{std::move(scheduling_policy), std::move(scheduling_priority)};
    scheduling_settings_.emplace(sched_settings);
  }

  /*!
   * \brief           Sets nice value that shall be used for the process.
   *
   * \details         -Linux Not calling this function on a settings object means default nice value (0) should
   *                         be used when a process with these settings is created.
   *                  -QNX   Setting nice value will cause CreateProcess() to fail, because it is not supported.
   *
   * \param[in]       nice_value         Nice value of the process.
   *
   * \context         InitPhase
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          FALSE
   *
   * \trace           DSGN-Osab-Process-ProcessCreationOptionalSettings
   *
   * \vprivate        Vector product internal API
   *
   * \spec
   *   requires true;
   * \endspec
   */
  void SetNiceValue(osabstraction::thread::NiceValue nice_value) { nice_value_.emplace(nice_value); }

  /*!
   * \brief           Sets the CPU affinity that shall be used for the process.
   *
   * \details         Not calling this function on a settings object means the default CPU affinity should be used when
   *                  a process with these settings is created.
   *
   * \param[in]       cpu_affinity              CPU affinity mask for the process.
   *
   * \context         InitPhase
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          FALSE
   *
   * \trace           DSGN-Osab-Process-ProcessCreationOptionalSettings
   *
   * \vprivate        Vector product internal API
   *
   * \spec
   *   requires true;
   * \endspec
   */
  // VCA_OSA_EXT_CALL_OBJ_MOVE
  void SetCpuAffinity(CpuAffinity cpu_affinity) { cpu_affinity_.emplace(std::move(cpu_affinity)); }

  /*!
   * \brief           Sets the user ID that shall be used for the process.
   *
   * \details         Not calling this function on a settings object means the default user id should be used when a
   *                  process with these settings is created.
   *
   * \param[in]       user_id                   User ID for the process.
   *
   * \context         InitPhase
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          FALSE
   *
   * \trace           DSGN-Osab-Process-ProcessCreationOptionalSettings
   *
   * \vprivate        Vector product internal API
   *
   * \spec
   *   requires true;
   * \endspec
   */
  void SetUserId(UserId user_id) { user_id_.emplace(std::move(user_id)); }

  /*!
   * \brief           Sets the primary group ID that shall be used for the process.
   *
   * \details         Not calling this function on a settings object means the default group id should be used when a
   *                  process with these settings is created.
   *
   * \param[in]       group_id                  Primary group ID for the process.
   *
   * \context         InitPhase
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          FALSE
   *
   * \trace           DSGN-Osab-Process-ProcessCreationOptionalSettings
   *
   * \vprivate        Vector product internal API
   *
   * \spec
   *   requires true;
   * \endspec
   */
  void SetPrimaryGroupId(GroupId group_id) { group_id_.emplace(std::move(group_id)); }

  /*!
   * \brief           Sets the secondary group IDs that shall be used for the process.
   *
   * \details         Not calling this function on a settings object means the default group ids should be used when a
   *                  process with these settings is created. Passing an empty group id list as input into this function
   *                  means no secondary groups should be set when a process with these settings is created.
   *
   * \param[in]       groups                    Secondary group IDs for the process.
   *
   * \context         InitPhase
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          FALSE
   *
   * \trace           DSGN-Osab-Process-ProcessCreationOptionalSettings
   *
   * \vprivate        Vector product internal API
   *
   * \spec
   *   requires true;
   * \endspec
   */
  // VCA_OSA_EXT_CALL_OBJ_MOVE
  void SetSecondaryGroups(GroupIdList groups) { groups_.emplace(std::move(groups)); }

  /*!
   * \brief           Sets the inherit secondary groups flag.
   *
   * \details         Not calling this function on a settings object means the default valuse of inherit secondary
   * groups(false) will be used when the process secondary groups are set.
   *
   * \param[in]       is_inherit_secondary_groups  true if parent secondary groups to be inherited when the groups_
   * field contains no value, otherwise false.
   *
   * \context         InitPhase
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          FALSE
   *
   * \trace           DSGN-Osab-Process-ProcessCreationOptionalSettings
   *
   * \vprivate        Vector product internal API
   *
   * \spec
   *   requires true;
   * \endspec
   */
  void SetInheritSecondaryGroups(bool is_inherit_secondary_groups) {
    is_inherit_secondary_groups_ = is_inherit_secondary_groups;
  }

  /*!
   * \brief           Sets the resource group in which the process shall be started.
   *
   * \details         Currently has only effect for linux implementation. Dummy implementation for QNX.
   *
   * \param[in]       resource_group_name   The resource group's name.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          FALSE
   *
   * \trace           DSGN-Osab-Process-ProcessCreationOptionalSettings
   * \trace           DSGN-Osab-Process-ResourceGroup-Linux
   *
   * \vprivate        Vector product internal API
   *
   * \spec
   *   requires true;
   * \endspec
   */
  void SetResourceGroup(ResourceGroupName resource_group_name) {
    // VCA_OSA_EXT_CALL_CTOR, VCA_OSA_CALL_PASS_VALUE
    ::amsr::core::Result<ResourceGroup> resource_group_result{ResourceGroup::Create(std::move(resource_group_name))};
    if (resource_group_result.HasValue()) {  // COV_OSAB_SET_RESOURCE_GROUP
      resource_group_ = std::move(resource_group_result.Value());
    }
  }

  /*!
   * \brief           Sets the security policy that shall be applied to the process.
   *
   * \details         - Not calling this function on a settings object means the security policy ID of the calling
   *                    process should be inherited to the created process.
   *                  - The specified security policy ID may be ignored if the OS does not enforce security policies
   *                    when the process is created.
   *
   * \param[in]       secpol_id   Security policy ID.
   *
   * \return          Empty result if the operation is supported by the OS.
   *
   * \error           osabstraction::OsabErrc::kApiError   feature is not supported by the OS.
   *
   * \context         InitPhase
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          FALSE
   *
   * \trace           DSGN-Osab-Process-ProcessCreationOptionalSettings
   * \trace           DSGN-Osab-Process-ProcessQNXSecurityPolicies
   *
   * \vprivate        Vector product internal API
   *
   * \spec
   *   requires true;
   * \endspec
   */
  ::amsr::core::Result<void> SetSecurityPolicy(SecurityPolicyId secpol_id);

  /*!
   * \brief           Get the arguments that shall be passed to the new Process.
   *
   * \details         Returns the arguments, program_name is the first one followed by the arguments that have been set
   *                  with SetArguments.
   *
   * \param[in]       program_name              Program name that shall be passed to the new Process as argv[0].
   *
   * \return          Arguments that shall be passed to the process as an array of pointers to c strings with a nullptr
   *                  after the last valid pointer.
   *
   * \context         ANY|!InitPhase
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          FALSE
   *
   * \trace           DSGN-Osab-Process-ProcessCreationOptionalSettings
   *
   * \vprivate        Vector component internal API
   *
   * \spec
   *   requires true;
   * \endspec
   */
  char const* const* GenerateArgv(Argument const& program_name);

  /*!
   * \brief           Get the environment variables that shall be set for the new Process.
   *
   * \details         Each environment variable is created as a c string that contains the name of the environment
   *                  variable followed by an equals character followed by the value of the environment variable.
   *
   * \return          Environment variables that shall be set for the process as an array of pointers to c strings with
   *                  a nullptr after the last valid pointer.
   *
   * \context         ANY|!InitPhase
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          FALSE
   *
   * \trace           DSGN-Osab-Process-ProcessCreationOptionalSettings
   *
   * \vprivate        Vector component internal API
   *
   * \spec
   *   requires true;
   * \endspec
   */
  char const* const* GenerateEnvp();

  /*!
   * \brief           Get the SchedulingSettings.
   *
   * \return          Scheduling policy and priority of the process. Empty if scheduling settings have not been set.
   *
   * \context         ANY|!InitPhase
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          FALSE
   *
   * \trace           DSGN-Osab-Process-ProcessCreationOptionalSettings
   *
   * \vprivate        Vector component internal API
   *
   * \spec
   *   requires true;
   * \endspec
   */
  OptionalSchedulingSettings const& GetSchedulingSettings() const { return scheduling_settings_; }

  /*!
   * \brief           Get the optional nice value.
   *
   * \return          Nice value of the process. Empty if nice value has not been set.
   *
   * \context         ANY|!InitPhase
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          FALSE
   *
   * \trace           DSGN-Osab-Process-ProcessCreationOptionalSettings
   *
   * \vprivate        Vector component internal API
   *
   * \spec
   *   requires true;
   * \endspec
   */
  osabstraction::thread::OptionalNiceValue const& GetNiceValue() const { return nice_value_; }

  /*!
   * \brief           Get the CPU affinity.
   *
   * \return          CPU affinity of the process. Empty if CPU affinity has not been set.
   *
   * \context         ANY|!InitPhase
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          FALSE
   *
   * \trace           DSGN-Osab-Process-ProcessCreationOptionalSettings
   *
   * \vprivate        Vector component internal API
   *
   * \spec
   *   requires true;
   * \endspec
   */
  OptionalCpuAffinity const& GetCpuAffinity() const { return cpu_affinity_; }

  /*!
   * \brief           Get the resource group.
   *
   * \return          Resource group of the process. Empty if resource group has not been set.
   *
   * \context         ANY|!InitPhase
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          FALSE
   *
   * \trace           DSGN-Osab-Process-ProcessCreationOptionalSettings
   *
   * \vprivate        Vector component internal API
   *
   * \spec
   *   requires true;
   * \endspec
   */
  OptionalResourceGroup const& GetResourceGroup() const { return resource_group_; }

  /*!
   * \brief           Get the user ID for the process.
   *
   * \return          User ID of the process. Empty if user ID has not been set.
   *
   * \context         ANY|!InitPhase
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          FALSE
   *
   * \trace           DSGN-Osab-Process-ProcessCreationOptionalSettings
   *
   * \vprivate        Vector component internal API
   *
   * \spec
   *   requires true;
   * \endspec
   */
  OptionalUserId const& GetUserId() const { return user_id_; }

  /*!
   * \brief           Get the primary group ID for the process.
   *
   * \return          Primary group ID of the process. Empty if primary group ID has not been set.
   *
   * \context         ANY|!InitPhase
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          FALSE
   *
   * \trace           DSGN-Osab-Process-ProcessCreationOptionalSettings
   *
   * \vprivate        Vector component internal API
   *
   * \spec
   *   requires true;
   * \endspec
   */
  OptionalGroupId const& GetPrimaryGroupId() const { return group_id_; }

  /*!
   * \brief           Get the secondary group IDs for the process.
   *
   * \return          Secondary group IDs of the process. Empty if secondary group IDs have not been set.
   *
   * \context         ANY|!InitPhase
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          FALSE
   *
   * \trace           DSGN-Osab-Process-ProcessCreationOptionalSettings
   *
   * \vprivate        Vector component internal API
   *
   * \spec
   *   requires true;
   * \endspec
   */
  OptionalGroupIdList const& GetSecondaryGroups() const { return groups_; }

  /*!
   * \brief           Get the inherit secondary groups flag for the process.
   *
   * \return          Inherit secondary groups flag of the process.
   *
   * \context         ANY|!InitPhase
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          FALSE
   *
   * \trace           DSGN-Osab-Process-ProcessCreationOptionalSettings
   *
   * \vprivate        Vector component internal API
   *
   * \spec
   *   requires true;
   * \endspec
   */
  bool IsInheritSecondaryGroups() const { return is_inherit_secondary_groups_; }

  /*!
   * \brief           Get the startup abilities for the process.
   *
   * \return          List of startup abilities. Empty if startup abilities have not been added.
   *
   * \context         ANY|!InitPhase
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          FALSE
   *
   * \trace           DSGN-Osab-Process-ProcessCreationOptionalSettings
   * \trace           DSGN-Osab-Process-ProcessStartupQNXAbility
   *
   * \vprivate        Vector component internal API
   *
   * \spec
   *   requires true;
   * \endspec
   */
  ara::core::Vector<internal::Ability> const& GetStartupAbilities() const { return startup_abilities_; }

  /*!
   * \brief           Get the running abilities for the process.
   *
   * \return          List of running abilities. Empty if running abilities have not been added.
   *
   * \context         ANY|!InitPhase
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          FALSE
   *
   * \trace           DSGN-Osab-Process-ProcessRunningQNXAbility
   *
   * \vprivate        Vector component internal API
   */
  ara::core::Vector<internal::Ability> const& GetRunningAbilities() const { return running_abilities_; }

  /*!
   * \brief           Get the resource limits for the process.
   *
   * \return          List of resource limits. Empty if resource limits have not been added.
   *
   * \context         ANY|!InitPhase
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          FALSE
   *
   * \trace           DSGN-Osab-Process-ProcessCreationOptionalSettings
   *
   * \vprivate        Vector component internal API
   *
   * \spec
   *   requires true;
   * \endspec
   */
  ara::core::Vector<internal::ResourceLimit> const& GetResourceLimits() const { return resource_limits_; }

  /*!
   * \brief           Get the security policy that shall be applied to the process.
   *
   * \details         This function is supposed to be called internally only by OS's
   *                  that support security policies.
   *
   * \return          Security policy ID. No value if there isn't security policy set.
   *
   * \context         ANY|!InitPhase
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          FALSE
   *
   * \trace           DSGN-Osab-Process-ProcessCreationOptionalSettings
   * \trace           DSGN-Osab-Process-ProcessQNXSecurityPolicies
   *
   * \vprivate        Vector component internal API
   *
   * \spec
   *   requires true;
   * \endspec
   */
  OptionalSecurityPolicyId const& GetSecurityPolicy() const { return secpol_id_; }

  /*!
   * \brief           Get the signal list that shall be unmasked for the process.
   *
   * \details         This function is called to get the signals to be unmasked.
   *
   * \return          Vector containing signals to be unmasked.
   *
   * \context         ANY|!InitPhase
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          FALSE
   *
   * \trace           DSGN-Osab-Process-ProcessCreationOptionalSettings
   *
   * \vprivate        Vector component internal API
   */
  ara::core::Vector<amsr::signal::SupportedSignal> const& GetSignalsToUnMask() const { return signals_to_unmask_; }

  /*!
   * \brief           Add the given signal to the signal list to be unblocked.
   *
   * \details         Update the signal list to be unblocked
   *
   * \param[in]       signal   The signal to be added.
   *
   * \context         InitPhase
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          FALSE
   *
   * \trace           DSGN-Osab-Process-ProcessCreationOptionalSettings
   *
   * \vprivate        Vector product internal API
   *
   * \spec
   *   requires true;
   * \endspec
   */
  // VCA_OSA_STL_CALL_OBJ_PASS_PTR_RW
  void AddSignalToUnMask(amsr::signal::SupportedSignal signal) { signals_to_unmask_.emplace_back(signal); }

 private:
  /*! \brief   Minimal size of argv_ to hold argv[0] and a nullptr. */
  static constexpr size_t kMinArgvSize{2};

  /*! \brief   Minimal size of envp_ to hold a nullptr. */
  static constexpr size_t kMinEnvpSize{1};

  /*!
   * \brief           Command line arguments which shall be passed to the process.
   */
  Arguments args_{};

  /*!
   * \brief           Command line arguments as char* which shall be passed to the process.
   */
  ara::core::Vector<char const*> argv_{};

  /*!
   * \brief           Environment variables which shall be passed to the process.
   *
   * \details         key:   environment variable name
   *                  value: environment variable name followed by equals character followed by environment variable
   *                         value
   *
   * \note            The value contains the information in the described format because this is the format that is
   *                  required when the information is used to start a process. This way, no further memory allocation
   *                  is required to build a string in the required format when a process is started.
   */
  EnvironmentVars env_vars_{};

  /*!
   * \brief           Environment variables as char * which shall be passed to the process.
   */
  ara::core::Vector<char const*> envp_{};

  /*!
   * \brief           The scheduling settings for the application under process.
   */
  OptionalSchedulingSettings scheduling_settings_{};

  /*!
   * \brief           The nice value for the application under process.
   * \details         Nice value is not supported on QNX. Not empty value will lead to error during
   *                  CreateProcess().
   */
  osabstraction::thread::OptionalNiceValue nice_value_{};

  /*!
   * \brief           The CPU core control for core mask setting of the process.
   */
  OptionalCpuAffinity cpu_affinity_{};

  /*!
   * \brief           The user ID of the process.
   */
  OptionalUserId user_id_{};

  /*!
   * \brief           The primary group ID of the process.
   */
  OptionalGroupId group_id_{};

  /*!
   * \brief           The resource limits of the process.
   */
  ara::core::Vector<internal::ResourceLimit> resource_limits_{};

  /*!
   * \brief           The secondary group IDs of the process.
   */
  OptionalGroupIdList groups_{};

  /*!
   * \brief           The resource group that the process shall be started in.
   */
  OptionalResourceGroup resource_group_{};

  /*!
   * \brief           The abilities of the process, which are set by OsProcess::CreateProcess().
   */
  ara::core::Vector<internal::Ability> startup_abilities_{};

  /*!
   * \brief           The abilities of the process, which are set by OsProcess::SetRunningAbilities();
   */
  ara::core::Vector<internal::Ability> running_abilities_{};

  /*!
   * \brief           The security policy of the process.
   */
  OptionalSecurityPolicyId secpol_id_{};

  /*!
   * \brief           The list of signals to be unmasked.
   */
  ara::core::Vector<amsr::signal::SupportedSignal> signals_to_unmask_{};

  /*!
   * \brief           Indicates whether the parent secondary groups needs to be inherited when the groups_ has no value.
   */
  bool is_inherit_secondary_groups_{false};
};

}  // namespace process
}  // namespace osabstraction

#endif  // LIB_COMMON_LIBOSABSTRACTION_POSIX_INCLUDE_OSABSTRACTION_PROCESS_OS_PROCESS_SETTINGS_H_

// COV_JUSTIFICATION_BEGIN
//  \ID COV_OSAB_SET_RESOURCE_GROUP
//  \ACCEPT TX
//  \REASON In POSIX it will never enter the else condition because POSIX implementation of Create() will always return
//          a value. Only on PikeOs Create() can return an error.
// COV_JUSTIFICATION_END
