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
/*!        \file  validation_writer.h
 *        \brief  Writes validation data to file
 *
 *********************************************************************************************************************/
#ifndef LIB_IOINTEGRITYSTREAM_INCLUDE_AMSR_IOINTEGRITYSTREAM_INTERNAL_JSON_VALIDATION_VALIDATION_WRITER_H_
#define LIB_IOINTEGRITYSTREAM_INCLUDE_AMSR_IOINTEGRITYSTREAM_INTERNAL_JSON_VALIDATION_VALIDATION_WRITER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/core/optional.h"
#include "amsr/core/result.h"
#include "amsr/core/string_view.h"
#include "amsr/iointegritystream/internal/common_types.h"
#include "amsr/iostream/filestream/file_permission.h"

namespace amsr {
namespace iointegritystream {
namespace internal {
namespace json_validation {

/*!
 * \brief Write validation information to file with specified permission.
 * \details Format the validation data to json format and add CRC32 checksum to the end of the file.
 * \param[in] file_out The file to write data into.
 * \param[in] data The data to write into file.
 * \param[in] opt_permission The optional permission mode when we write to validation file.
 * \return Result of whether updating the validation file was successful or not.
 * \error amsr::json::JsonErrc::kUserValidationFailed If json validation failed.
 * \error amsr::json::JsonErrc::kInvalidJson If json buffer cannot contain all the data.
 * \error amsr::json::JsonErrc::StreamFailure If validation file not including checksum is larger than 4MiB.
 * \error amsr::stream::StreamErrc::* Underlying stream operations Open/Write fail.
 * \context ANY
 * \pre -
 * \threadsafe FALSE
 * \reentrant FALSE
 *
 * \trace DSGN-IoIntegrity-Writer, DSGN-IoIntegrity-Reader-Writer, DSGN-IoIntegrity-Update-Validation-File
 */
amsr::core::Result<void> WriteValidationData(
    amsr::core::StringView file_out, ValidationFileData const& data,
    amsr::core::Optional<amsr::stream::filestream::FilePermission> opt_permission) noexcept;

}  // namespace json_validation
}  // namespace internal
}  // namespace iointegritystream
}  // namespace amsr

#endif  // LIB_IOINTEGRITYSTREAM_INCLUDE_AMSR_IOINTEGRITYSTREAM_INTERNAL_JSON_VALIDATION_VALIDATION_WRITER_H_
