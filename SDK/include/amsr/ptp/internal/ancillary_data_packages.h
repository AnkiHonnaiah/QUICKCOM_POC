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
 *        \brief  This collection of ancillary data packages used together with sockets.
 *
 *      \details  The classes define a type safe wrapper around ancillary data. All options provide an interface to
 *                the operating system that consists of the requested protocol level and protocol type as well as
 *                a view to a buffer to store the ancillary data.
 *
 *         \unit  amsr::ptp::OsInterface
 *
 *********************************************************************************************************************/

#ifndef LIB_PTP_IMPL_LIBOSABSTRACTION_LINUX_INCLUDE_AMSR_PTP_INTERNAL_ANCILLARY_DATA_PACKAGES_H_
#define LIB_PTP_IMPL_LIBOSABSTRACTION_LINUX_INCLUDE_AMSR_PTP_INTERNAL_ANCILLARY_DATA_PACKAGES_H_

#include <ctime>  // Include before linux/errqueue.h, otherwise linux/errqueue.h may miss definition of struct timespec

#include <linux/errqueue.h>
#include <sys/socket.h>
#include <cerrno>
#include <chrono>

#include "amsr/ptp/internal/types.h"

namespace amsr {
namespace ptp {
namespace internal {

// VECTOR Next Construct AutosarC++17_10-M3.4.1: MD_OSA_M3.4.1_MinimizeIdentifierVisibility
/*!
 * \brief Implements an ancillary data package to get a software timestamp.
 *
 * The package accepts data created by the SO_TIMESTAMPING option with SOF_TIMESTAMPING_SOFTWARE flag set.
 */
class AncillaryDataPkgSoftwareTimestamp final {
 public:
  /*!
   * \brief Returns stored timestamp in nanoseconds
   *
   * \return          Stored timestamp in nanoseconds.
   *
   * \context         ANY
   *
   * \pre             HasReceivedTimestamp() == true
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   *
   * \vprivate        Vector component internal API
   */
  auto GetTimestampNS() const noexcept -> std::chrono::nanoseconds {
    return std::chrono::nanoseconds{time_stamps_.ts[0].tv_nsec} + std::chrono::seconds{time_stamps_.ts[0].tv_sec};
  }

  /*!
   * \brief Returns an ancillary data request that may be passed to the operating system interface layer.
   *
   * \return          Ancillary data requestd.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   *
   * \vprivate        Vector component internal API
   */
  auto GetRequest() noexcept -> AncillaryDataRequest {
    return AncillaryDataRequest{GetLevel(), GetType(), GetDataView(), &used_bytes_};
  }

  /*!
   * \brief Checks if a data package has been received.
   *
   * \return True if a timestamp has been written to the data package. False if no time stamp has been received.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   *
   * \vprivate        Vector component internal API
   */
  auto HasReceivedTimestamp() const noexcept -> bool { return (this->used_bytes_ == sizeof(struct scm_timestamping)); }

 private:
  /*!
   * \brief Returns the ancillary data's socket level.
   *
   * \return The ancillary data's socket level.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   *
   * \vprivate        Vector component internal API
   */
  static constexpr auto GetLevel() -> SockOptLevel { return SockOptLevel{SOL_SOCKET}; }

  /*!
   * \brief Returns the ancillary data's protocol specific type.
   *
   * \return The ancillary data's protocol specific type.
   *
   * \context         ANY
   *
   * \pre             -
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   *
   * \vprivate        Vector component internal API
   */
  static constexpr auto GetType() -> ControlMessageType { return ControlMessageType{SCM_TIMESTAMPING}; }

  /*!
   * \brief Returns a mutable view to the internal data.
   *
   * \details The buffer view shall be used to store ancillary data which is usually returned by sendmsg() or recvmsg().
   *
   * \return          Mutable view to the internal data.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   *
   * \vprivate        Vector component internal API
   *
   */
  auto GetDataView() noexcept -> AncillaryDataView { return AncillaryDataView{&time_stamps_, sizeof(time_stamps_)}; }

  /*!
   * \brief The internal OS data type to store time stamps.
   *
   * \details
   * ts[0] holds software time stamps.
   * ts[1] is currently not used.
   * ts[2] holds hardware time stamps.
   */
  struct scm_timestamping time_stamps_ {};

  /*!
   * \brief Stores the number of used bytes in an ancillary data request.
   */
  std::size_t used_bytes_{};
};

// VECTOR Next Construct AutosarC++17_10-M3.4.1: MD_OSA_M3.4.1_MinimizeIdentifierVisibility
/*!
 * \brief Implements an ancillary data package to get a hardware timestamp.
 *
 * The package accepts data created by the SO_TIMESTAMPING option with SOF_TIMESTAMPING_RAW_HARDWARE flag set.
 */
class AncillaryDataPkgHardwareTimestamp final {
 public:
  /*!
   * \brief Returns stored timestamp in nanoseconds
   *
   * \return          Stored timestamp in nanoseconds.
   *
   * \context         ANY
   *
   * \pre             HasReceivedTimestamp() == true
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   *
   * \vprivate        Vector component internal API
   */
  std::chrono::nanoseconds GetTimestampNS() const noexcept {
    return std::chrono::nanoseconds{time_stamps_.ts[2].tv_nsec} + std::chrono::seconds{time_stamps_.ts[2].tv_sec};
  }

  /*!
   * \brief Returns an ancillary data request that may be passed to the operating system interface layer.
   *
   * \return          Ancillary data request.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   *
   * \vprivate        Vector component internal API
   */
  auto GetRequest() noexcept -> AncillaryDataRequest {
    return AncillaryDataRequest{GetLevel(), GetType(), GetDataView(), &used_bytes_};
  }

  /*!
   * \brief Checks if a data packages has been received.
   *
   * \return True if a timestamp has been written to the data package. False if no time stamp has been received.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   *
   * \vprivate        Vector component internal API
   */
  auto HasReceivedTimestamp() const noexcept -> bool { return (this->used_bytes_ == sizeof(struct scm_timestamping)); }

 private:
  /*!
   * \brief Returns the ancillary data's socket level.
   *
   * \return The ancillary data's socket level.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   *
   * \vprivate        Vector component internal API
   */
  static constexpr auto GetLevel() -> SockOptLevel { return SockOptLevel{SOL_SOCKET}; }

  /*!
   * \brief Returns the ancillary data's protocol specific type.
   *
   * \return The ancillary data's protocol specific type.
   *
   * \context         ANY
   *
   * \pre             -
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   *
   * \vprivate        Vector component internal API
   */
  static constexpr auto GetType() -> ControlMessageType { return ControlMessageType{SCM_TIMESTAMPING}; }

  /*!
   * \brief Returns a mutable view to the internal data.
   *
   * \details The buffer view shall be used to store ancillary data which is usually returned by sendmsg() or recvmsg().
   *
   * \return          Ancillary data view of the internal data.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   *
   * \vprivate        Vector component internal API
   */
  auto GetDataView() noexcept -> AncillaryDataView { return AncillaryDataView{&time_stamps_, sizeof(time_stamps_)}; }

  /*!
   * \brief The internal OS data type to store time stamps.
   *
   * \details
   * ts[0] holds software time stamps.
   * ts[1] is currently not used.
   * ts[2] holds hardware time stamps.
   */
  struct scm_timestamping time_stamps_ {};

  /*!
   * \brief Stores the number of used bytes in an ancillary data request.
   */
  std::size_t used_bytes_{};
};

// VECTOR Next Construct AutosarC++17_10-M3.4.1: MD_OSA_M3.4.1_MinimizeIdentifierVisibility
/*!
 * \brief   Implements an ancillary data package to get an send timestamp socket error.
 * \details Only works for packet sockets.
 */
class AncillaryDataPkgSocketErrorTxTimestamp final {
 public:
  /*!
   * \brief Returns ID of the received send timestamp.
   *
   * \return          ID of the received send timestamp.
   *
   * \context         ANY
   *
   * \pre             HasTxTimestampSocketError() == true
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   *
   * \vprivate        Vector component internal API
   */
  SendTimestampToken GetTimestampId() const noexcept { return SendTimestampToken{socket_error_.ee_data}; }

  /*!
   * \brief Returns an ancillary data request that may be passed to the operating system interface layer.
   *
   * \return          Ancillary data request.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   *
   * \vprivate        Vector component internal API
   */
  auto GetRequest() noexcept -> AncillaryDataRequest {
    return AncillaryDataRequest{GetLevel(), GetType(), GetDataView(), &used_bytes_};
  }

  /*!
   * \brief Checks if a timestamp socket error has been received.
   *
   * \return True if a timestamp socket error has been received, false otherwise.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   *
   * \vprivate        Vector component internal API
   */
  auto HasTxTimestampSocketError() const noexcept -> bool {
    return (this->used_bytes_ == sizeof(struct sock_extended_err)) && (this->socket_error_.ee_errno == ENOMSG) &&
           (this->socket_error_.ee_origin == SO_EE_ORIGIN_TIMESTAMPING);
  }

 private:
  /*!
   * \brief Returns the ancillary data's socket level.
   *
   * \return The ancillary data's socket level.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   *
   * \vprivate        Vector component internal API
   */
  static constexpr auto GetLevel() -> SockOptLevel { return SockOptLevel{SOL_PACKET}; }

  /*!
   * \brief Returns the ancillary data's protocol specific type.
   *
   * \return The ancillary data's protocol specific type.
   *
   * \context         ANY
   *
   * \pre             -
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   *
   * \vprivate        Vector component internal API
   */
  static constexpr auto GetType() -> ControlMessageType { return ControlMessageType{PACKET_TX_TIMESTAMP}; }

  /*!
   * \brief Returns a mutable view to the internal data.
   *
   * \details The buffer view shall be used to store ancillary data which is usually returned by sendmsg() or recvmsg().
   *
   * \return          Mutable view to the internal data.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   *
   * \vprivate        Vector component internal API
   *
   */
  auto GetDataView() noexcept -> AncillaryDataView { return AncillaryDataView{&socket_error_, sizeof(socket_error_)}; }

  /*!
   * \brief The received socket error.
   */
  struct sock_extended_err socket_error_ {};

  /*!
   * \brief Stores the number of used bytes in an ancillary data request.
   */
  std::size_t used_bytes_{};
};

}  // namespace internal
}  // namespace ptp
}  // namespace amsr

#endif  // LIB_PTP_IMPL_LIBOSABSTRACTION_LINUX_INCLUDE_AMSR_PTP_INTERNAL_ANCILLARY_DATA_PACKAGES_H_
