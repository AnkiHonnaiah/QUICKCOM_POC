/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *
 *                Copyright (c) 2022 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *
 *               \file    warningdisplay_processor.h
 *              \brief    -
 *
 *            \details    -
 *
 *********************************************************************************************************************/
#ifndef SRC_WARNINGDISPLAY_WARNINGDISPLAY_PROCESSOR_H
#define SRC_WARNINGDISPLAY_WARNINGDISPLAY_PROCESSOR_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "rport_speedlimit_service_client.h"
#include "datatypes/impl_type_overlimitenum.h"

namespace oneapplication {
namespace warningdisplay {

class WarningDisplayProcessing final {
 public:
  static WarningDisplayProcessing& GetInst();

  void ChangeSpeedLimitState(datatypes::OverLimitEnum limit_state);


 private:
  /*!
   * \brief The default constructor. It is private to secure that it is only called internally.
   */
  WarningDisplayProcessing() = default;

  /*!
  * \brief  Holds the logging context for the lightcoordinator unit
  */
  static ara::log::Logger& GetLogger();

  void WarningMessageControl() noexcept;

 private:

  datatypes::OverLimitEnum internal_limit_state_{datatypes::OverLimitEnum::NOT_OVER};
};

}  // namespace warningdisplay
}  // namespace oneapplication

#endif //SRC_WARNINGDISPLAY_WARNINGDISPLAY_PROCESSOR_H
