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
/*!        \file  file_io.h
 *        \brief  Defines the interface of the crypto config IO extension point
 *
 *********************************************************************************************************************/
#ifndef LIB_LIBSERVER_INCLUDE_CRYPTO_SERVER_FILE_IO_H_
#define LIB_LIBSERVER_INCLUDE_CRYPTO_SERVER_FILE_IO_H_
#include "amsr/core/string_view.h"
#include "amsr/iostream/filestream/open_mode.h"
#include "amsr/iostream/input_stream.h"
#include "ara/core/vector.h"

namespace crypto {
namespace server {
namespace file_io {
/*!
 * \brief Opens, reads and potentially decrypts file.
 * \param[in] file_path path of the file to open
 * \return InputStream with the file contents in plain text.
 * \context ANY
 * \pre -
 * \reentrant FALSE
 * \vprivate Product Private
 * \trace CREQ-Crypto-ConfigFilesIo
 */
auto ReadFromFile(ara::core::StringView file_path) noexcept -> ara::core::Result<ara::core::Vector<char>>;

/*!
 * \brief Opens, potentially encrypts and writes data to a file.
 * \param[in] data InputStream containing the data to be written
 * \param[in] file_path path to the output file.
 * \param[in] open_mode Additional open mode for the file (in addition to kWrite)
 * \return InputStream with the file contents in plain text.
 * \context ANY
 * \pre -
 * \reentrant FALSE
 * \vprivate Product Private
 * \trace CREQ-Crypto-ConfigFilesIo
 */
auto WriteToFile(amsr::stream::InputStream& data, ara::core::StringView file_path,
                 amsr::stream::filestream::OpenModeType open_mode) noexcept -> ara::core::Result<void>;

}  // namespace file_io
}  // namespace server
}  // namespace crypto

#endif  // LIB_LIBSERVER_INCLUDE_CRYPTO_SERVER_FILE_IO_H_
