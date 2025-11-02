/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       Win Yeh                                                   */
/*    Created:      2025                                                      */
/*    Description:  WinLib Interface Class (WLIC) - Competition Template      */
/*                                                                            */
/*----------------------------------------------------------------------------*/

/*==============================================================================
 * WELCOME TO WINLIB INTERFACE CLASS (WLIC)
 *==============================================================================
 *
 * Thank you for downloading this project from GitHub!
 *
 * WLIC is a powerful C++ framework for building customizable touch-based
 * graphical interfaces on VEX V5 Brain screens. This framework provides:
 *
 *   • Hierarchical navigation with parent-child interface relationships
 *   • Automatic touch detection with coordinate-based hit testing
 *   • Background update tasks for dynamic real-time displays
 *   • Run-Length Encoded (RLE) image rendering support
 *   • Callback system for custom actions on button presses
 *   • Clean state management for interface activation/deactivation
 *
 * GETTING STARTED:
 * ----------------
 * 1. Review the built-in interfaces in src/display.cpp for examples
 * 2. See API_Reference.md for complete documentation
 * 3. Customize the interfaces to match your team's needs
 * 4. Build and download to your VEX V5 Brain
 *
 * DISCLAIMER:
 * -----------
 * This software is provided "as is" without warranty of any kind, express or
 * implied. The author(s) shall not be held liable for any damages, issues, or
 * malfunctions that may arise from the use of this project. Use at your own
 * risk. Always test your code thoroughly before competition.
 *
 * Licensed under the MIT License - see LICENSE file for details.
 *
 *============================================================================*/

#include "vex.h"
using namespace vex;
competition Competition;

/*---------------------------------------------------------------------------*/
/*                          Pre-Autonomous Functions                         */
/*                                                                           */
/*  This function runs BEFORE autonomous and BEFORE driver control.          */
/*---------------------------------------------------------------------------*/
void pre_auton(void)
{
  // Launch the WLIC interface system for autonomous selection and monitoring
  Monitor();

  // ADD YOUR PRE-AUTONOMOUS CODE HERE
}

/*---------------------------------------------------------------------------*/
/*                              Autonomous Task                              */
/*                                                                           */
/*  This function runs during the 15-second autonomous period in VEX         */
/*  competitions. It executes the pre-programmed autonomous routine.         */
/*---------------------------------------------------------------------------*/
void autonomous(void)
{
  // ADD YOUR AUTONOMOUS CODE HERE
}

/*---------------------------------------------------------------------------*/
/*                           Driver Control Task                             */
/*                                                                           */
/*  This function runs during the driver-controlled period (1:45 in VEX      */
/*  competitions). It contains the code that responds to controller input.   */
/*---------------------------------------------------------------------------*/
void usercontrol(void)
{
  // ADD YOUR DRIVER CONTROL CODE HERE
}

/*---------------------------------------------------------------------------*/
/*                              Main Program                                 */
/*                                                                           */
/*  The main() function is the entry point of the program. It sets up the    */
/*  competition system and registers callback functions.                     */
/*                                                                           */
/*  DO NOT MODIFY THIS FUNCTION unless you understand the VEX competition    */
/*  template structure.                                                      */
/*                                                                           */
/*  The VEX competition system automatically calls autonomous() or           */
/*  usercontrol() based on competition switch state or Brain screen.         */
/*---------------------------------------------------------------------------*/
int main()
{
  // Register the autonomous and driver control callback functions
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  // Run pre-autonomous initialization
  pre_auton();

  // Infinite loop to prevent main() from exiting
  // Competition callbacks (autonomous/usercontrol) run independently
  while (true) {
    wait(100, msec);
  }
}
