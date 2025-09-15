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
 *        \brief  File descriptor abstraction to clean up properly on destruction.
 *        \unit   osabstraction::FileDescriptor
 *
 *********************************************************************************************************************/

#ifndef LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_OSABSTRACTION_IO_FILE_DESCRIPTOR_H_
#define LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_OSABSTRACTION_IO_FILE_DESCRIPTOR_H_

#include "osabstraction/io/native_types.h"

namespace osabstraction {
namespace io {

/*!
 * \brief Class to store file descriptor.
 * \vprivate Vector product internal API
 */
class FileDescriptor final {
 public:
  /*!
   * \brief Constructor.
   *
   * \param[in]   handle         native handle
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   *
   * \vprivate        Vector product internal API
   */
  explicit FileDescriptor(NativeHandle handle = kInvalidNativeHandle) noexcept : handle_{handle} {}

  /*!
   * \brief Destructor.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   *
   * \vprivate        Vector product internal API
   */
  ~FileDescriptor() noexcept;

  /*!
   * \brief Deleted copy constructor.
   */
  FileDescriptor(FileDescriptor const& other) noexcept = delete;

  /*!
   * \brief Deleted assignement operator.
   */
  FileDescriptor& operator=(FileDescriptor const& other) noexcept = delete;

  // VECTOR Next Construct AutosarC++17_10-A12.8.1: MD_OSA_A12.8.1_MoveConstructor
  /*!
   * \brief Define move constructor.
   *
   * \param[in,out]   other    FileDescriptor object to move from.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   *
   * \vprivate        Vector product internal API
   */
  FileDescriptor(FileDescriptor&& other) noexcept : handle_{other.handle_} { other.handle_ = kInvalidNativeHandle; }

  /*!
   * \brief Define move operator.
   *
   * \param[in,out]   other    FileDescriptor object to move from.
   *
   * \return          This instance.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   *
   * \vprivate        Vector product internal API
   */
  FileDescriptor& operator=(FileDescriptor&& other) & noexcept;

  /*!
   * \brief           Returns handle to file descriptor.
   *
   * \return          Handle to file descriptor.
   *
   * \error           -
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   *
   * \vprivate        Vector product internal API
   */
  NativeHandle Get() const noexcept;

  /*!
   * \brief           Returns whether the file descriptor is valid.
   *
   * \return          True when file descriptor is valid, false otherwise.
   *
   * \error           -
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   *
   * \vprivate        Vector product internal API
   */
  bool IsValid() const noexcept;

 private:
  /*!
   * \brief Handle to file descriptor.
   */
  NativeHandle handle_{kInvalidNativeHandle};
};

}  // namespace io
}  // namespace osabstraction

#endif  // LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_OSABSTRACTION_IO_FILE_DESCRIPTOR_H_
