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
 *               \file   main.cpp
 *              \brief    -
 *
 *            \details    -
 *
 *********************************************************************************************************************/

#include "warningdisplay_controller.h"


/*!
 * \brief Entry Point of the process.
 * \return Zero when successfully executed.
 */
int main() {
  int return_value = 0;

  oneapplication::warningdisplay::WarningDisplayController warningdisplay_control{};

  warningdisplay_control.Run();

  return return_value;
}
