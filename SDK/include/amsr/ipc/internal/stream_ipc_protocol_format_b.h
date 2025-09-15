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
 *        \brief  Defines format B header of IPC protocol of SafeIPC.
 *        \unit   osabstraction::ipc::IpcUtils_LinuxPikeOs
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_COMMON_LIBOSABSTRACTION_LINUX_INCLUDE_AMSR_IPC_INTERNAL_STREAM_IPC_PROTOCOL_FORMAT_B_H_
#define LIB_COMMON_LIBOSABSTRACTION_LINUX_INCLUDE_AMSR_IPC_INTERNAL_STREAM_IPC_PROTOCOL_FORMAT_B_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstddef>
#include <cstdint>
#include "amsr/core/result.h"
#include "amsr/core/span.h"
#include "amsr/memory/memory_types.h"
#include "osabstraction/osab_error_domain.h"

namespace amsr {
namespace ipc {
namespace internal {
namespace stream_ipc_protocol {

/*!
 * \brief    Format B header that is added by SafeIPC to an IPC message after the common header if the common header
 *           indicates message format B.
 * \details  The format B header os OS specific.
 */
class FormatBHeader {
 public:
  /*!
   * \brief    Size of the format B header in bytes.
   */
  static constexpr std::size_t kSize{sizeof(std::uint8_t) + sizeof(std::uint8_t)};

  /*!
   * \brief    Type of memory exchange handle that is transferred with this IPC message.
   * \details  Currently only one type defined.
   */
  enum class MemoryExchangeHandleType : std::uint8_t {
    /*!
     * \brief  The memory exchange handle refers to shared memory.
     */
    kSharedMemoryExchangeHandle = 1
  };

  /*!
   * \brief    Access mode with which that memory exchange handle shall be opened by the receiver.
   */
  enum class SharedAccessMode : std::uint8_t {

    /*!
     * \brief   Read-only memory exchange handle.
     */
    kReadOnly = 1,
    /*!
     * \brief   Read-write memory exchange handle.
     */
    kReadWrite = 2
  };

  // VECTOR NC AutosarC++17_10-A12.1.5: MD_OSA_A12.1.5_CtorInvalidatedMembers
  /*!
   * \brief           Creates a FormatBHeader object for an IPC message.
   *
   * \details         Initializes all fields of the format B header.
   *
   * \param[in]       shared_access_mode   Access mode with which that memory exchange handle shall be opened by the
   *                                       receiver.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  /*!
   * \internal
   *  - Initialize all fields.
   * \endinternal
   */
  explicit FormatBHeader(SharedAccessMode shared_access_mode) noexcept
      : memory_exchange_handle_type_{static_cast<std::uint8_t>(MemoryExchangeHandleType::kSharedMemoryExchangeHandle)},
        shared_access_mode_{static_cast<std::uint8_t>(shared_access_mode)} {}

  /*!
   * \brief           Creates an invalid FormatBHeader object for an IPC message.
   *
   * \details         The created header is not valid because all fields of the header are not initialized to valid
   *                  values.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  /*!
   * \internal
   *  - Default initialize the fields.
   * \endinternal
   */
  FormatBHeader() noexcept : memory_exchange_handle_type_{}, shared_access_mode_{} {}

  /*!
   * \brief           Creates a FormatBHeader object from a received format B header.
   *
   * \details         The received format B header is deserialized and its contents stored in this object.
   *
   * \param[in]       data_view   View on the received data that represents the format B header.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  /*!
   * \internal
   *  - Deserialize the received message header and initialize all fields.
   * \endinternal
   */
  explicit FormatBHeader(::amsr::core::Span<std::uint8_t const, kSize> data_view) noexcept
      : memory_exchange_handle_type_{}, shared_access_mode_{} {
    std::size_t offset{0};

    memory_exchange_handle_type_ = data_view[offset];
    offset += sizeof(memory_exchange_handle_type_);

    shared_access_mode_ = data_view[offset];
  }

  /*!
   * \brief           Serializes a FormatBHeader object into a data buffer.
   *
   * \param[out]      data_view   View on the memory where the contents of the FormatBHeader shall be serialized to.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  /*!
   * \internal
   *  - Serialize the variables of this object into the provided data buffer.
   * \endinternal
   */
  void Serialize(::amsr::core::Span<std::uint8_t, kSize> data_view) const noexcept {
    std::size_t offset{0};

    data_view[offset] = memory_exchange_handle_type_;
    offset += sizeof(memory_exchange_handle_type_);

    data_view[offset] = shared_access_mode_;
  };

  /*!
   * \brief           Gets the shared access mode of this header.
   *
   * \return          Shared access mode of this header.
   *
   * \context         ANY
   *
   * \pre             The shared access mode of this header is valid.
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  auto GetSharedAccessMode() const noexcept -> SharedAccessMode {
    return static_cast<SharedAccessMode>(shared_access_mode_);
  }

  /*!
   * \brief           Validates if the fields of this FormatBHeader are valid.
   *
   * \details         Should be used to check if a received format B header is corrupted or not.
   *
   * \return          Empty value if the format B header is valid, an error otherwise.
   *
   * \error           osabstraction::OsabErrc::kProtocolError   The format B header is not valid.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  /*!
   * \internal
   *  - Validate that
   *    - The type of memory exchange handle is shared memory exchange handle.
   *    - The shared access mode is one of the known values.
   *  - If the validation succeeded
   *    - Return an empty value.
   *  - Otherwise return a kProtocol error.
   * \endinternal
   */
  auto Validate() const noexcept -> ::amsr::core::Result<void> {
    ::amsr::core::Result<void> result{};

    if ((memory_exchange_handle_type_ !=
         static_cast<std::uint8_t>(MemoryExchangeHandleType::kSharedMemoryExchangeHandle)) ||
        ((shared_access_mode_ != static_cast<std::uint8_t>(SharedAccessMode::kReadOnly)) &&
         (shared_access_mode_ != static_cast<std::uint8_t>(SharedAccessMode::kReadWrite)))) {
      result.EmplaceError(osabstraction::OsabErrc::kProtocolError, "IPC message header corrupted (format B header).");
    }

    return result;
  }

 private:
  /*!
   * \brief    Memory exchange handle type field of the format B header.
   * \details  Specifies the type of memory exchange handle that is attached to the IPC message.
   */
  std::uint8_t memory_exchange_handle_type_;

  /*!
   * \brief    Shared access mode field of the format B header.
   * \details  Specifies the access mode with which the memory exchange handle attached to this IPC message shall be
   *           opened.
   */
  std::uint8_t shared_access_mode_;
};

/*!
 * \brief           Convert a FormatBHeader::SharedAccessMode to an amsr::memory::AccessMode.
 *
 * \param[in]       mode   FormatBHeader::SharedAccessMode to convert.
 *
 * \return          The converted value.
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      TRUE
 */
inline auto ConvertAccessMode(FormatBHeader::SharedAccessMode mode) noexcept -> amsr::memory::AccessMode {
  return (mode == FormatBHeader::SharedAccessMode::kReadOnly) ? amsr::memory::AccessMode::kReadOnly
                                                              : amsr::memory::AccessMode::kReadWrite;
}

/*!
 * \brief           Convert an amsr::memory::AccessMode to a FormatBHeader::SharedAccessMode.
 *
 * \param[in]       mode   amsr::memory::AccessMode to convert.
 *
 * \return          The converted value.
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      TRUE
 */
inline auto ConvertAccessMode(amsr::memory::AccessMode mode) noexcept -> FormatBHeader::SharedAccessMode {
  return (mode == amsr::memory::AccessMode::kReadOnly) ? FormatBHeader::SharedAccessMode::kReadOnly
                                                       : FormatBHeader::SharedAccessMode::kReadWrite;
}

}  // namespace stream_ipc_protocol
}  // namespace internal
}  // namespace ipc
}  // namespace amsr

#endif  // LIB_COMMON_LIBOSABSTRACTION_LINUX_INCLUDE_AMSR_IPC_INTERNAL_STREAM_IPC_PROTOCOL_FORMAT_B_H_
