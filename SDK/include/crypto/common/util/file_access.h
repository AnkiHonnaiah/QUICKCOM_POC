/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2023 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file  file_access.h
 *        \brief  Utilities for file access.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBCOMMON_INCLUDE_CRYPTO_COMMON_UTIL_FILE_ACCESS_H_
#define LIB_LIBCOMMON_INCLUDE_CRYPTO_COMMON_UTIL_FILE_ACCESS_H_

/*********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <fcntl.h>
#include <ftw.h>
#include <libgen.h>
#include <sys/stat.h>
#include <unistd.h>
#include <climits>

#include <algorithm>
#include <cctype>
#include <limits>
#include <vector>

#include "amsr/core/result.h"
#include "amsr/core/span.h"
#include "amsr/core/string.h"
#include "amsr/filesystem/path.h"
#include "amsr/iostream/filestream/input_file_stream.h"
#include "amsr/iostream/filestream/open_mode.h"
#include "amsr/iostream/filestream/output_file_stream.h"
#include "amsr/iostream/stateful_output_stream.h"
#include "ara/crypto/common/exceptions.h"
#include "osabstraction/process/processinterface.h"

namespace crypto {
namespace common {
namespace util {

// VECTOR NC Metric-OO.WMC.One: MD_CRYPTO_Metric-OO.WMC.One
// VECTOR NC AutosarC++17_10-M3.4.1: MD_CRYPTO_AutosarC++17_10-M3.4.1_global
/*!
 * \brief Utilities for file access.
 * \vprivate Product Private
 */
class FileAccess final {
 public:
  /*!
   * \brief Generic char point type.
   * \vprivate Product Private
   */
  using FileContentView = amsr::core::Span<std::uint8_t const>;

  /*!
   * \brief Checks whether a file exists.
   * \param[in] file_path Absolute path to the file to be checked.
   * \return Whether the file exists.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  static bool FileExists(amsr::core::StringView file_path) noexcept {
    bool result{false};
    amsr::stream::filestream::InputFileStream const file{};
    static_cast<void>(file.Open(file_path));

    if (file.IsOpen()) {
      result = true;
      static_cast<void>(file.Close());
    }

    return result;
  }

  /*!
   * \brief Moves a file.
   * \param[in] from Absolute source path to be moved from.
   * \param[in] to Absolute destination path to be moved to.
   * \return True on success, otherwise false.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  static bool MoveFile(amsr::core::StringView from, amsr::core::StringView to) noexcept {
    bool result{false};

    // make sure destination does not exist, otherwise behavior of rename is undefined
    if (!FileExists(to)) {
      // VECTOR NL AutosarC++17_10-M27.0.1: MD_CRYPTO_AutosarC++17_10-M27.0.1_no_alternative
      result = std::rename(from.data(), to.data()) == 0;
    }

    return result;
  }

  /*!
   * \brief Attempts to write a file with given content.
   * \param[in] file_path Absolute path to the file to be written.
   * \param[in] content File content to be written.
   * \return Whether the operation succeeded.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  static bool WriteFile(amsr::core::StringView file_path, FileContentView content) noexcept {
    bool result{false};
    amsr::stream::filestream::OutputFileStream file_stream{};
    amsr::stream::StatefulOutputStream stateful_stream{file_stream};
    // VECTOR NC AutosarC++17_10-A4.5.1: MD_CRYPTO_AutosarC++17_10-A4.5.1_BitmaskType
    static_cast<void>(file_stream.Open(file_path, amsr::stream::filestream::OpenMode::kTruncate |
                                                      amsr::stream::filestream::OpenMode::kWrite |
                                                      amsr::stream::filestream::OpenMode::kCreate));

    if (file_stream.IsOpen()) {
      assert(content.size() <= static_cast<std::uint64_t>(std::numeric_limits<std::streamsize>::max()));
      ara::core::Vector<char> vec{content.begin(), content.end()};
      amsr::core::Span<char> const buf{vec};
      stateful_stream.Write(buf);

      bool const sync_result{file_stream.Sync().HasValue()};
      bool const close_result{file_stream.Close().HasValue()};
      bool const stream_state_valid{stateful_stream.GetState().HasValue()};

      result = sync_result && close_result && stream_state_valid;
    }

    return result;
  }

  /*!
   * \brief Removes a file.
   * \param[in] file_path Complete path to the file to be removed.
   * \return Whether the file was removed.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  static bool RemoveFile(amsr::core::StringView file_path) noexcept {
    bool result{false};

    if (FileExists(file_path)) {
      // VECTOR NL AutosarC++17_10-M27.0.1: MD_CRYPTO_AutosarC++17_10-M27.0.1_no_alternative
      result = std::remove(file_path.data()) == 0;
      sync();
    }

    return result;
  }

  /*!
   * \brief Checks whether given path represents an existing directory-
   * \param[in] directory_path Directory path to check.
   * \return True if the path leads to an existing directory, otherwise false.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  static bool IsDirectory(amsr::core::StringView directory_path) noexcept {
    bool result{false};
    struct stat dir_stat {};

    result = stat(directory_path.data(), &dir_stat) == 0;
    // VECTOR NC AutosarC++17_10-M2.13.2: MD_CRYPTO_AutosarC++17_10-M2.13.2_conflict_with_c_api
    // VECTOR NL AutosarC++17_10-M5.0.21: MD_CRYPTO_AutosarC++17_10-M5.0.21_conflict_with_c_api
    result = (result) && (S_ISDIR(dir_stat.st_mode) != 0);

    return result;
  }

  /*!
   * \brief Creates a directory.
   * \param[in] directory_path Complete path to the directory to be created.
   * \return Whether the directory was created.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  static bool CreateDirectory(amsr::core::StringView directory_path) noexcept {
    bool result{IsDirectory(directory_path)};

    if (!result) {
      // VECTOR NC AutosarC++17_10-M2.13.2: MD_CRYPTO_AutosarC++17_10-M2.13.2_conflict_with_c_api
      // VECTOR NL AutosarC++17_10-M5.0.21: MD_CRYPTO_AutosarC++17_10-M5.0.21_conflict_with_c_api
      result = mkdir(directory_path.data(), S_IRWXU | S_IRWXG | S_IROTH) == 0;
    }

    return result;
  }

  /*!
   * \brief Removes a directory and all of its contents.
   * \param[in] directory_path Complete path to the directory to be removed.
   * \return Whether the directory was removed.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  static bool RemoveDirectory(amsr::core::StringView directory_path) noexcept {
    bool result{false};

    if (IsDirectory(directory_path)) {
      // VECTOR NC AutosarC++17_10-A8.5.2: MD_CRYPTO_AutosarC++17_10-A8.5.2_auto_from_ret_type
      // VECTOR NL AutosarC++17_10-M7.1.2: MD_CRYPTO_AutosarC++17_10-M7.1.2_parameters_should_stay_as_they_are
      auto const fw_cb = [](char const* path, struct stat const*, std::int32_t, struct FTW*) {
        // VECTOR NL AutosarC++17_10-M27.0.1: MD_CRYPTO_AutosarC++17_10-M27.0.1_no_alternative
        return std::remove(path);
      };
      // VECTOR NC AutosarC++17_10-A4.5.1: MD_CRYPTO_AutosarC++17_10-A4.5.1_BitmaskType
      // VECTOR NL AutosarC++17_10-M5.0.21: MD_CRYPTO_AutosarC++17_10-M5.0.21_conflict_with_c_api
      result = nftw(directory_path.data(), fw_cb, 32, FTW_DEPTH | FTW_PHYS) == 0;
    }
    sync();
    return result;
  }

  /*!
   * \brief Tests whether the given POSIX path is absolute.
   * \param[in] path POSIX file system path.
   * \return True if the path is absolute.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  static bool IsAbsolutePath(amsr::core::StringView path) noexcept {
    // VECTOR NC AutosarC++17_10-M5.14.1: MD_CRYPTO_AutosarC++17_10-M5.14.1_no_side_effect
    return ((ara::core::String{path}).rfind("~", 0) == 0) || (amsr::filesystem::Path{path}).IsAbsolute();
  }

  /*!
   * \brief Tests whether the given POSIX path is relative.
   * \param[in] path POSIX file system path.
   * \return True if the path is absolute.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  static bool IsRelativePath(amsr::core::StringView path) noexcept { return !IsAbsolutePath(path); }

  /*!
   * \brief Returns the current working directory.
   * \return Current working directory.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error ara::crypto::SecurityErrc::kInvalidArgument If path cannot be normalized
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  static amsr::core::Result<amsr::core::String> WorkingDirectory() noexcept {
    amsr::core::Result<amsr::core::String> result{ara::crypto::SecurityErrc::kResourceFault};
    std::array<char, PATH_MAX> cwd_buffer{};
    char const* const cwd{getcwd(cwd_buffer.data(), cwd_buffer.size())};

    if (cwd != nullptr) {
      amsr::core::Result<amsr::core::String> normalized_path{
          amsr::filesystem::Path{amsr::core::StringView{cwd_buffer.data()}}.Normalize().CStr()};

      if (!normalized_path) {
        result.EmplaceError(normalized_path.Error());
      } else {
        result.EmplaceValue(normalized_path.Value());
      }
    }

    return result;
  }

  // VECTOR NC AutosarC++17_10-A15.4.2: MD_CRYPTO_AutosarC++17_10-A15.4.2_noexcept_unsupported_in_used_api
  // VECTOR NC AutosarC++17_10-A15.5.3: MD_CRYPTO_AutosarC++17_10-A15.5.3_noexcept_unsupported_in_used_api
  /*!
   * \brief Joins two POSIX paths.
   * \param[in] base_path POSIX file system path.
   * \param[in] rel_path Relative POSIX file system path.
   * \return Joined path.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error ara::crypto::SecurityErrc::kInvalidArgument If \c base_path is not a path or if \c rel_path is not a
   * relative path.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  static amsr::core::Result<amsr::core::String> JoinPaths(amsr::core::StringView base_path,
                                                          amsr::core::StringView rel_path) noexcept {
    amsr::core::Result<amsr::core::String> result{ara::crypto::SecurityErrc::kInvalidArgument};

    // VECTOR NC AutosarC++17_10-M5.14.1: MD_CRYPTO_AutosarC++17_10-M5.14.1_no_side_effect
    if (FileAccess::IsRelativePath(rel_path)) {
      amsr::filesystem::Path const base{amsr::filesystem::Path{amsr::core::String(base_path)}};
      amsr::filesystem::Path const rel{amsr::filesystem::Path{amsr::core::String(rel_path)}};
      amsr::filesystem::Path const joined_normalized{(base / rel).Normalize()};
      result.EmplaceValue(joined_normalized.CStr());
    }

    return result;
  }

  /*!
   * \brief Makes a POSIX path absolute.
   * \param[in] path POSIX file system path.
   * \return Absolute path.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  static amsr::core::Result<amsr::core::String> AbsolutePath(amsr::core::StringView path) noexcept {
    amsr::core::Result<amsr::core::String> result{ara::crypto::SecurityErrc::kInvalidArgument};

    amsr::core::String const abs_path{path};

    if (FileAccess::IsRelativePath(abs_path)) {
      amsr::core::Result<amsr::core::String> wd{FileAccess::WorkingDirectory()};

      if (!wd) {
        result.EmplaceError(wd.Error());
      } else {
        amsr::core::Result<amsr::core::String> jp{FileAccess::JoinPaths(wd.Value(), abs_path)};

        if (!jp) {
          result.EmplaceError(jp.Error());
        } else {
          result.EmplaceValue(jp.Value());
        }
      }
    } else {
      amsr::core::String normalized{amsr::filesystem::Path{abs_path}.Normalize().CStr()};

      // Strip last character if it is a forward slash
      if ((normalized.length() > 1) && (normalized[normalized.length() - 1] == '/')) {
        normalized = normalized.substr(0, normalized.length() - 1);
      }

      result.EmplaceValue(normalized);
    }

    return result;
  }

  /*!
   * \brief Finds the last common position between absolute path and reference path.
   * \param[in] abs_path Absolute POSIX file system path.
   * \param[in] abs_ref_path Absolute reference POSIX file system path.
   * \return Last common position number.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  static std::size_t FindLastCommonPos(amsr::core::StringView abs_path, amsr::core::StringView abs_ref_path) noexcept {
    std::size_t const min_len{std::min(abs_path.length(), abs_ref_path.length())};
    bool diff_enc{false};
    std::size_t last_common_pos{0};  // here initialization required to mask the warning [-Werror=maybe-uninitialized]

    for (std::size_t i{0}; i < min_len; ++i) {
      if (abs_path[i] != abs_ref_path[i]) {
        diff_enc = true;
        break;
      } else if (abs_path[i] == '/') {
        last_common_pos = i;
      } else {
        // this else clause is required by AutosarC++17_10-M6.4.2
      }
    }

    if (!diff_enc) {
      if ((abs_path.length() > min_len) && (abs_path[min_len] == '/')) {
        last_common_pos = min_len;
      } else if ((abs_ref_path.length() > min_len) && (abs_ref_path[min_len] == '/')) {
        last_common_pos = min_len;
      } else {
        last_common_pos = min_len;
      }
    }

    return last_common_pos;
  }

  /*!
   * \brief Makes a POSIX path relative.
   * \param[in] abs_path Absolute POSIX file system path.
   * \param[in] abs_ref_path Absolute reference POSIX file system path.
   * \return Relative path in string format.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  static amsr::core::String MakeRelative(amsr::core::StringView abs_path,
                                         amsr::core::StringView abs_ref_path) noexcept {
    amsr::core::String rel_path;
    std::size_t const last_common_pos{FindLastCommonPos(abs_path, abs_ref_path)};

    for (std::size_t i{last_common_pos}; i < abs_ref_path.length(); ++i) {
      if (abs_ref_path[i] == '/') {
        rel_path += "../";
      }
    }

    if (abs_path.size() > (last_common_pos + 1)) {
      rel_path += abs_path.substr(last_common_pos + 1);
    }

    if ((rel_path.length() > 0) && (rel_path[rel_path.length() - 1] == '/')) {
      rel_path = rel_path.substr(0, rel_path.length() - 1);
    }

    return rel_path;
  }

  // VECTOR NC AutosarC++17_10-A15.4.2: MD_CRYPTO_AutosarC++17_10-A15.4.2_noexcept_unsupported_in_used_api
  // VECTOR NC AutosarC++17_10-A15.5.3: MD_CRYPTO_AutosarC++17_10-A15.5.3_noexcept_unsupported_in_used_api
  /*!
   * \brief Makes a POSIX path relative.
   * \param[in] path POSIX file system path.
   * \param[in] ref_path Reference POSIX file system path.
   * \return Relative path.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  static amsr::core::Result<amsr::core::String> RelativePath(amsr::core::StringView path,
                                                             amsr::core::StringView ref_path) noexcept {
    amsr::core::Result<amsr::core::String> result{ara::crypto::SecurityErrc::kInvalidArgument};

    amsr::core::Result<amsr::core::String> abs_path_result{AbsolutePath(path)};

    if (!abs_path_result) {
      result.EmplaceError(abs_path_result.Error());
    } else {
      amsr::core::Result<amsr::core::String> abs_ref_path_result{AbsolutePath(ref_path)};

      if (!abs_ref_path_result) {
        result.EmplaceError(abs_ref_path_result.Error());
      } else {
        amsr::core::StringView const abs_path{abs_path_result.Value()};
        amsr::core::StringView const abs_ref_path{abs_ref_path_result.Value()};

        if (((abs_path[0] == '~') || (abs_ref_path[0] == '~')) && ((abs_path[0] != '~') || (abs_ref_path[0] != '~'))) {
          result.EmplaceError(ara::crypto::SecurityErrc::kIncompatibleArguments);
        } else {
          result.EmplaceValue(MakeRelative(abs_path, abs_ref_path));
        }
      }
    }

    return result;
  }

  /*!
   * \brief Removes the last segment from a POSIX path.
   * \param[in] path POSIX path.
   * \return Stripped POSIX path.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  static amsr::core::String StripLastSegment(amsr::core::StringView path) noexcept {
    std::size_t const pos{path.find_last_of('/')};
    amsr::core::String result;

    if (pos == amsr::core::String::npos) {
      result = "";
    } else {
      result = path.substr(0, pos + 1);
    }

    return result;
  }
};

}  // namespace util
}  // namespace common
}  // namespace crypto

#endif  //  LIB_LIBCOMMON_INCLUDE_CRYPTO_COMMON_UTIL_FILE_ACCESS_H_
