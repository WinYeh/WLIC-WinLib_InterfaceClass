/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       display.h                                                 */
/*    Author:       Win Yeh                                                   */
/*    Created:      2025                                                      */
/*    Description:  WLIC Display Functions - Interface declarations          */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#pragma once

/*-----------------------------------------------------------------------------
 * GLOBAL VARIABLES
 *---------------------------------------------------------------------------*/

/**
 * @brief Global status message string
 * @note Used to display system status or messages across interfaces
 */
extern std::string status;

/*-----------------------------------------------------------------------------
 * STATUS AND PERFORMANCE FUNCTIONS
 *---------------------------------------------------------------------------*/

/**
 * @brief Set the global status message
 * @param stat The status string to display
 */
void setStatus(std::string stat);

/**
 * @brief Background task that prints performance metrics on screen
 * @return int Always returns 0 when complete
 * @note Called as an update function for interfaces requiring real-time stats
 */
int printPerformance();

/*-----------------------------------------------------------------------------
 * HOME INTERFACE
 *---------------------------------------------------------------------------*/

/**
 * @brief Render the Home interface (main menu)
 * @note Displays the primary landing screen with navigation options
 */
void displayHome();

/*-----------------------------------------------------------------------------
 * HEADING INTERFACE
 *---------------------------------------------------------------------------*/

/**
 * @brief Draw directional indicator based on robot heading
 * @param heading The current heading angle in degrees (0-360)
 * @note Draws a compass/arrow showing robot orientation
 */
void drawdirection(double heading);

/**
 * @brief Background task that continuously updates heading display
 * @return int Always returns 0 when complete
 * @note Updates heading values from inertial sensor in real-time
 */
int drawHeading();

/**
 * @brief Render the Heading interface (compass display)
 * @note Shows robot orientation with visual compass indicator
 */
void displayHeading();

/*-----------------------------------------------------------------------------
 * PORTS INTERFACE
 *---------------------------------------------------------------------------*/

/**
 * @brief Render the Ports interface (motor and sensor status)
 * @note Displays connection status and information for all V5 ports
 */
void displayPorts();

/*-----------------------------------------------------------------------------
 * AUTONOMOUS SELECTION INTERFACES
 *---------------------------------------------------------------------------*/

/**
 * @brief Render the Autonomous Selection interface (main)
 * @note Displays options to select red or blue alliance autonomous routines
 */
void displayAuton();

/**
 * @brief Render the Red Alliance autonomous selection interface
 * @note Shows available autonomous routines for red alliance
 */
void displayAutonRed();

/**
 * @brief Render the Blue Alliance autonomous selection interface
 * @note Shows available autonomous routines for blue alliance
 */
void displayAutonBlue();

/*-----------------------------------------------------------------------------
 * UTILITY INTERFACES
 *---------------------------------------------------------------------------*/

/**
 * @brief Render the Logo interface (team branding)
 * @note Displays team logo and branding information
 */
void displayLogo();

/**
 * @brief Render the Utilities interface
 * @note Displays additional tools and settings options
 */
void displayUtil();

/*-----------------------------------------------------------------------------
 * MAIN MONITOR FUNCTION
 *---------------------------------------------------------------------------*/

/**
 * @brief Initialize and launch the WLIC interface system
 * @return int Always returns 0 when complete
 * @note Sets up all interfaces, coordinates, linked interfaces, and starts
 *       the main interface loop. Called from pre_auton() in main.cpp
 */
int Monitor();