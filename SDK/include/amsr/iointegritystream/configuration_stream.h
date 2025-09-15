
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
/*!       \file         configuration_stream.h
 *        \brief        Provides a configuration stream
 *        \unit         amsr::iointegritystream::ConfigurationStream
 *
 *        \complexity   The complexity is increased by many inter module calls which are necessary for the correct
 *                      functionality of this unit and does not introduce any risk.
 *********************************************************************************************************************/

#ifndef LIB_IOINTEGRITYSTREAM_INCLUDE_AMSR_IOINTEGRITYSTREAM_CONFIGURATION_STREAM_H_
#define LIB_IOINTEGRITYSTREAM_INCLUDE_AMSR_IOINTEGRITYSTREAM_CONFIGURATION_STREAM_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/core/result.h"
#include "amsr/core/string_view.h"
#include "amsr/core/variant.h"
#include "amsr/iointegritystream/integrity_file_stream.h"
#include "amsr/iostream/filestream/input_file_stream.h"
#include "vac/container/c_string_view.h"

namespace amsr {
namespace iointegritystream {

// VECTOR NC AutosarC++17_10-M3.4.1: MD_IoIntegrityStream_M3.4.1_typeCanBeDeclaredLocally
/*! \brief Environment variable name for "AMSR_DISABLE_INTEGRITY_CHECK". */
static constexpr vac::container::CStringView kAmsrDisableIntegrityCheck{
    vac::container::CStringView::FromLiteral("AMSR_DISABLE_INTEGRITY_CHECK", 28)};

// VECTOR NC AutosarC++17_10-M3.4.1: MD_IoIntegrityStream_M3.4.1_typeCanBeDeclaredLocally
/*! \brief Environment variable name for "AMSR_PROCESS_SHORT_NAME_PATH". */
static constexpr vac::container::CStringView kAmsrProcessShortNamePath{
    vac::container::CStringView::FromLiteral("AMSR_PROCESS_SHORT_NAME_PATH", 28)};

/*!
 * \brief Class to open a file for reading.
 * \details The opening behavior for the underlying filestream is controlled by an environment variable called
 * "AMSR_DISABLE_INTEGRITY_CHECK". When set to "1", a regular InputFileStream is opened, otherwise the file is opened
 * using the integrity protection mechanism (NOTE: IntegrityValidationStrategy is not configurable, default strategy
 * will be used). The target identifier can be extracted from the environment variable "AMSR_PROCESS_SHORT_NAME_PATH".
 * \vprivate Vector product internal API.
 */
class ConfigurationStream final {
 public:
  /*!
   * \brief Static object constructor.
   * \details Uses the process short name path as its target identifier. The target identifier is extracted from the
   * associated environment variable "AMSR_PROCESS_SHORT_NAME_PATH".
   * \param[in] validation_file_path The path to the validation file.
   * \return A instance of ConfigurationStream.
   * \error osabstraction::OsabErrc::kDoesNotExist Environment variable "AMSR_PROCESS_SHORT_NAME_PATH" does not exist.
   * \error amsr::stream::StreamErrc::kInvalidAccess Integrity of validation file is invalid.
   * \error amsr::stream::StreamErrc::* Underlying stream operations Open/Seek/Read fail.
   * \error amsr::json::JsonErrc::kUserValidationFailed Unknown/unexpected value has been encountered.
   * \error amsr::json::JsonErrc::kInvalidJson Parsing has failed due to invalid JSON data.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \context ANY
   * \pre -
   * \vprivate Vector product internal API.
   *
   * \spec
   * requires true;
   * \endspec
   *
   * \trace DSGN-IoIntegrity-ConfigurationStream
   */
  static amsr::core::Result<ConfigurationStream> Create(amsr::core::StringView validation_file_path) noexcept;

  /*!
   * \brief Static object constructor.
   * \param[in] validation_file_path The path to the validation file.
   * \param[in] target_id The target identifier to use.
   * \return A instance of ConfigurationStream.
   * \error amsr::stream::StreamErrc::kInvalidAccess Integrity of validation file is invalid.
   * \error amsr::stream::StreamErrc::* Underlying stream operations Open/Seek/Read fail.
   * \error amsr::json::JsonErrc::kUserValidationFailed Unknown/unexpected value has been encountered.
   * \error amsr::json::JsonErrc::kInvalidJson Parsing has failed due to invalid JSON data.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \context ANY
   * \pre -
   * \vprivate Vector product internal API.
   *
   * \spec
   * requires true;
   * \endspec
   *
   * \trace DSGN-IoIntegrity-ConfigurationStream
   */
  static amsr::core::Result<ConfigurationStream> Create(amsr::core::StringView validation_file_path,
                                                        amsr::core::StringView target_id) noexcept;

  ConfigurationStream(ConfigurationStream const&) noexcept = delete;
  ConfigurationStream& operator=(ConfigurationStream const&) noexcept = delete;

  /*!
   * \brief Default move constructor.
   * \vprivate Vector product internal API.
   *
   * \spec
   * requires true;
   * \endspec
   */
  ConfigurationStream(ConfigurationStream&&) noexcept = default;  // VCA_IOIS_MOVE_CONSTRUCTOR

  /*!
   * \brief Default move assignment operator.
   * \return Moved ConfigurationStream.
   * \vprivate Vector product internal API.
   *
   * \spec
   * requires true;
   * \endspec
   */
  ConfigurationStream& operator=(ConfigurationStream&&) & noexcept = default;  // VCA_IOIS_MOVE_CONSTRUCTOR

  /*!
   * \brief Default destructor.
   * \details Associated file will be closed when ConfigurationStream instance is destructed.
   * \vprivate Vector product internal API.
   *
   * \spec
   * requires true;
   * \endspec
   */
  ~ConfigurationStream() noexcept = default;  // VCA_IOIS_DEFAULT_DESTRUCTOR

  /*!
   * \brief Open a file for reading.
   * \param[in] file_path File to open.
   * \return A reference to an InputStream.
   * \error amsr::stream::StreamErrc::kInvalidAccess Stream already opened or integrity of protected file is invalid.
   * \error amsr::stream::StreamErrc::kInvalidArgument \c file_path is not protected by the current integrity stream.
   * \error amsr::stream::StreamErrc::* Underlying stream operations Open/Tell/Seek/Read fail.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \context ANY
   * \pre -
   * \vprivate Vector product internal API.
   *
   * \spec
   * requires true;
   * \endspec
   */
  amsr::core::Result<amsr::stream::InputStream&> Open(amsr::core::StringView file_path) noexcept;

  /*!
   * \brief Close the opened file.
   * \return Result of type void
   * \error amsr::stream::StreamErrc::* Underlying stream operation Close fails.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \context ANY
   * \pre -
   * \vprivate Vector product internal API.
   *
   * \spec
   * requires true;
   * \endspec
   */
  amsr::core::Result<void> Close() noexcept;

 private:
  /*!
   * \brief Alias for type to use as StreamVariant.
   */
  using StreamVariant = amsr::core::Variant<IntegrityFileStream, amsr::stream::filestream::InputFileStream>;

  /*!
   * \brief Check if integrity protection is disabled.
   * \return A boolean value that indicates if the integrity is disabled or not.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   */
  static bool IsIntegrityCheckDisabled() noexcept;

  /*!
   * \brief Check and return the value of the environment variable AMSR_PROCESS_SHORT_NAME_PATH if it exist.
   * \return The assigned variable string.
   * \error osabstraction::OsabErrc::kDoesNotExist Environment variable "AMSR_PROCESS_SHORT_NAME_PATH" does not exist.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   */
  static amsr::core::Result<amsr::core::StringView> GetTargetId() noexcept;

  /*!
   * \brief Create a ConfigurationStream object from a result type containing the target ID.
   * \param[in] validation_file_path The path to the validation file.
   * \param[in] target_id_result Result type containing the target ID string.
   * \return A instance of ConfigurationStream.
   * \error osabstraction::OsabErrc::kDoesNotExist Environment variable "AMSR_PROCESS_SHORT_NAME_PATH" does not exist.
   * \error amsr::stream::StreamErrc::kInvalidAccess Integrity of validation file is invalid.
   * \error amsr::stream::StreamErrc::* Underlying stream operations Open/Seek/Read fail.
   * \error amsr::json::JsonErrc::kUserValidationFailed Unknown/unexpected value has been encountered.
   * \error amsr::json::JsonErrc::kInvalidJson Parsing has failed due to invalid JSON data.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   */
  static amsr::core::Result<ConfigurationStream> CreateConfigurationStream(
      amsr::core::StringView validation_file_path, amsr::core::Result<core::StringView> target_id_result) noexcept;
  /*!
   * \brief Explicit constructor that creates an ConfigurationStream object.
   * \param[in] input_fs The input file stream instance to use.
   * \pre -
   * \context ANY
   * \vprivate Vector product internal API.
   */
  explicit ConfigurationStream(stream::filestream::InputFileStream&& input_fs) noexcept;

  /*!
   * \brief Explicit constructor that creates an ConfigurationStream object.
   * \param[in] integrity_fs The integrity file stream instance to use.
   * \pre -
   * \context ANY
   * \vprivate Vector product internal API.
   */
  explicit ConfigurationStream(IntegrityFileStream&& integrity_fs) noexcept;

  /*!
   * \brief Variant storing the underlying file stream instance.
   * \vprivate Vector product internal API.
   */
  StreamVariant fs_variant_;

  // VECTOR NC AutosarC++17_10-A11.3.1: MD_IoIntegrityStream_A11.3.1_friend_class
  /*!
   * \brief Friend declaration for testing
   */
  FRIEND_TEST(UT__ConfigurationStream, IntegrityCheckOff__WithTargetParam);
  // VECTOR NC AutosarC++17_10-A11.3.1: MD_IoIntegrityStream_A11.3.1_friend_class
  /*!
   * \brief Friend declaration for testing
   */
  FRIEND_TEST(UT__ConfigurationStream, IntegrityCheckOff__WithoutTargetParam);
  // VECTOR NC AutosarC++17_10-A11.3.1: MD_IoIntegrityStream_A11.3.1_friend_class
  /*!
   * \brief Friend declaration for testing
   */
  FRIEND_TEST(UT__ConfigurationStream, IntegrityCheckOn__WithTargetParam__EvnVarNotSet);

  // VECTOR NC AutosarC++17_10-A11.3.1: MD_IoIntegrityStream_A11.3.1_friend_class
  /*!
   * \brief Friend declaration for testing
   */
  FRIEND_TEST(UT__ConfigurationStream, IntegrityCheckOn__WithTargetParam__EvnVarInvalid);

  // VECTOR NC AutosarC++17_10-A11.3.1: MD_IoIntegrityStream_A11.3.1_friend_class
  /*!
   * \brief Friend declaration for testing
   */
  FRIEND_TEST(UT__ConfigurationStream, IntegrityCheckOn__WithoutTargetParam__ProcessShortNameValid);
};

}  // namespace iointegritystream
}  // namespace amsr

#endif  // LIB_IOINTEGRITYSTREAM_INCLUDE_AMSR_IOINTEGRITYSTREAM_CONFIGURATION_STREAM_H_
