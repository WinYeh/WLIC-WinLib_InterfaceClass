/*******************************************************************************
 * CLAUDE.H - Project Documentation & AI Agent Instructions
 *
 * WinLib Interface Class (WLIC) Project
 * VEX Robotics Customizable Brain Screen Interface Framework
 *
 * This file contains comprehensive documentation about the project architecture,
 * design patterns, and instructions for Claude AI agents working with this code.
 ******************************************************************************/

#ifndef CLAUDE_H
#define CLAUDE_H

/*******************************************************************************
 * PROJECT OVERVIEW
 ******************************************************************************/

/*
 * PROJECT NAME: WinLib Interface Class (WLIC)
 * PURPOSE: A C++ framework for building customizable touch-based graphical
 *          interfaces for VEX robotics brain screens
 *
 * KEY CAPABILITIES:
 * - Hierarchical navigation between multiple interface screens
 * - Touch detection and button press handling
 * - Background update tasks for dynamic displays
 * - Run-Length Encoded (RLE) image rendering
 * - Callback-based action system
 * - State management for interface activation/deactivation
 */

/*******************************************************************************
 * PROJECT STRUCTURE
 ******************************************************************************/

/*
 * FILE ORGANIZATION:
 *
 * /include/
 *   ├── WLIC.h              - Main interface class header and function declarations
 *   ├── vex.h               - VEX library includes and macros
 *   └── robot-config.h      - Robot hardware configuration
 *
 * /src/
 *   ├── WLIC.cpp            - Interface class implementation (~700 lines)
 *   ├── main.cpp            - VEX competition template (autonomous/driver control)
 *   └── robot-config.cpp    - Hardware initialization
 *
 * /vex/
 *   └── [VEX SDK files]     - VEX V5 library files
 *
 * /.vscode/
 *   └── [VSCode config]     - Project settings for VEX extension
 */

/*******************************************************************************
 * CORE ARCHITECTURE
 ******************************************************************************/

/*
 * 1. INTERFACE CLASS (include/WLIC.h:8-67)
 * ==========================================
 *
 * The Interface class is the foundation of the GUI framework.
 *
 * KEY MEMBERS:
 * ------------
 * - int index                                    // Button state tracker
 *                                                //   -2: inactive
 *                                                //   -1: active, waiting for input
 *                                                //   >=0: specific button pressed
 *
 * - const std::vector<std::vector<double>>* button_coord
 *                                                // Button boundaries
 *                                                // Format: {x1, x2, y1, y2}
 *
 * - std::vector<Interface*>* linked_Interface    // Child interfaces for navigation
 *
 * - void (*actions)(int)                         // Callback when button pressed
 *
 * - void (*display)()                            // Function to render interface
 *
 * - int (*updateInterface)()                     // Background update function
 *
 * - task* updateItf_Task                         // Pointer to background task
 *
 * - static int isActive                          // Prevents simultaneous activations
 *
 *
 * CONSTRUCTOR PATTERNS:
 * ---------------------
 * The class provides 10 constructors for different use cases:
 *
 * 1. Basic display only:
 *    Interface(void (*display)())
 *
 * 2. Display + background updates:
 *    Interface(void (*display)(), int (*updateInterface)())
 *
 * 3. Interactive with buttons:
 *    Interface(button_coord, actions, display)
 *
 * 4. Interactive + updates:
 *    Interface(button_coord, actions, display, updateInterface)
 *
 * 5. Navigation interface:
 *    Interface(button_coord, linked_Interface, display)
 *
 * 6. Full-featured:
 *    Interface(button_coord, linked_Interface, actions, display, updateInterface)
 *
 * Choose the constructor based on required features.
 */

/*
 * 2. STATE MACHINE & NAVIGATION (src/WLIC.cpp:127-218)
 * =====================================================
 *
 * The interface system operates as a state machine:
 *
 * WORKFLOW:
 * ---------
 *
 * Home.setIndex(-1)        // Activate interface
 *         ↓
 * Home.Display()           // Render interface, start update task
 *         ↓
 * [Main Loop]
 *   Home.activate()        // Check for touches
 *         ↓
 *   [User touches screen]
 *         ↓
 *   InterfaceChooser()     // Detect which button (sets index)
 *         ↓
 *   [If linked interface exists]
 *         ↓
 *   Child.setIndex(-1)     // Activate child
 *   stopUpdateTask()       // Stop parent updates
 *   Brain.Screen.clearScreen()
 *   Child.Display()        // Show child interface
 *   actions(index)         // Execute callback
 *         ↓
 *   Interface::reset()     // Wait for release, reset state
 *
 *
 * KEY FUNCTIONS:
 * --------------
 *
 * int InterfaceChooser()
 *   - Iterates through button_coord vector
 *   - Checks if touch coordinates (brain_x, brain_y) fall within bounds
 *   - Sets index to matched button number
 *   - Returns: button index or -1 if no match
 *
 * void Display()
 *   - Calls display() function pointer to render graphics
 *   - Launches updateInterface() as background task if provided
 *
 * void activate()
 *   - Only runs if index == -1 (interface is active)
 *   - Checks isActive flag to prevent conflicts
 *   - Calls InterfaceChooser() to detect button press
 *   - Handles navigation to linked interfaces
 *   - Executes action callbacks
 *   - Sets isActive flag
 *
 * static void reset()
 *   - Waits for screen press to be released
 *   - Stores final touch position in brain_x, brain_y
 *   - Resets isActive flag to allow next activation
 *
 * void stopUpdateTask()
 *   - Stops background update task
 *   - Deletes task pointer
 *   - Prevents memory leaks when navigating away
 */

/*
 * 3. DISPLAY SYSTEM (src/WLIC.cpp:269-544)
 * =========================================
 *
 * The project includes pre-built display functions using RLE image compression.
 *
 * AVAILABLE DISPLAYS:
 * -------------------
 * - displayHome()         // Main menu (lines 269-300)
 * - displayHeading()      // Compass/heading display (lines 309-366)
 * - displayPowerflow()    // Motor/sensor status (lines 366-395)
 * - displayAuton()        // Autonomous selection (lines 395-423)
 * - displayAutonRed()     // Red alliance auton (lines 423-453)
 * - displayAutonBlue()    // Blue alliance auton (lines 453-482)
 * - displayLogo()         // Team logo (lines 482-512)
 * - displayUtil()         // Utilities menu (lines 512-544)
 *
 *
 * IMAGE FORMAT - RUN-LENGTH ENCODING:
 * ------------------------------------
 * Images are stored as:
 * 1. Color palette array (const char* imageColors[])
 * 2. RLE compressed pixel data (const int imageData[])
 *
 * RLE Format:
 *   Positive number N, color C -> Draw N pixels of color C
 *   Negative number -N         -> Skip N pixels (transparent)
 *
 * Example:
 *   { 5, 10, -3, 8, 15 }
 *   = Draw 5 pixels of color[10], skip 3, draw 8 pixels of color[15]
 *
 * Decoding loop (standard pattern):
 *   int x = 0, y = 0;
 *   for(int i = 0; i < dataSize; i += 2) {
 *       int count = imageData[i];
 *       int colorIndex = imageData[i + 1];
 *       if(count > 0) {
 *           Brain.Screen.setPenColor(imageColors[colorIndex]);
 *           for(int j = 0; j < count; ++j) {
 *               Brain.Screen.drawPixel(x++, y);
 *               if(x >= 480) { x = 0; y++; }
 *           }
 *       } else {
 *           x += count;
 *           while(x >= 480) { x -= 480; y++; }
 *       }
 *   }
 *
 *
 * IMAGE CONVERTER:
 * ----------------
 * Use VEX Image Converter to generate RLE code from drawings:
 * https://suhjae.github.io/vex-image/
 *
 * This tool converts images to the RLE format used by WLIC.
 */

/*
 * 4. MONITOR FUNCTION (src/WLIC.cpp:544-700)
 * ===========================================
 *
 * The Monitor() function is the main program entry point that sets up
 * the complete interface hierarchy and runs the event loop.
 *
 * INTERFACE HIERARCHY:
 * --------------------
 *
 * Home (Main Menu)
 * ├── Heading (Compass Display)
 * │   └── [Shows robot orientation]
 * ├── Powerflow (Motor Status)
 * │   └── [Shows motor voltages/status]
 * ├── AutonSelect (Autonomous Selector)
 * │   ├── AutonRed (Red Alliance)
 * │   └── AutonBlue (Blue Alliance)
 * └── Util (Utilities)
 *     ├── Home (Back to Main)
 *     └── Logo (Team Logo)
 *
 *
 * SETUP PATTERN:
 * --------------
 * For each interface:
 *
 * 1. Define button coordinates:
 *    std::vector<std::vector<double>> coords = {
 *        { x1, x2, y1, y2 },  // Button 0
 *        { x1, x2, y1, y2 },  // Button 1
 *        ...
 *    };
 *
 * 2. Create linked interface vector (if needed):
 *    std::vector<Interface*> linkedItf = {
 *        &ChildInterface1,
 *        &ChildInterface2
 *    };
 *
 * 3. Instantiate Interface:
 *    InterfaceName = Interface(&coords, &linkedItf, displayFunc);
 *
 * 4. Activate root interface:
 *    Home.setIndex(-1);
 *    Home.Display();
 *
 * 5. Main event loop:
 *    while(1) {
 *        Home.activate();
 *        Child1.activate();
 *        Child2.activate();
 *        // ... activate all interfaces
 *        Interface::reset();
 *        wait(100, msec);
 *    }
 *
 *
 * COORDINATE SYSTEM:
 * ------------------
 * VEX V5 Brain Screen: 480x240 pixels
 * Origin (0,0) is top-left corner
 * X increases right, Y increases down
 *
 * Button format: { x_min, x_max, y_min, y_max }
 * Example: { 340, 480, 20, 60 } creates button from (340,20) to (480,60)
 */

/*******************************************************************************
 * HELPER FUNCTIONS & UTILITIES
 ******************************************************************************/

/*
 * STATUS SYSTEM:
 * --------------
 *
 * extern std::string status;              // Global status string
 * void setStatus(std::string stat);       // Update status
 * int printPerformance();                 // Display performance metrics
 *                                         // (Used as updateInterface for Home)
 *
 *
 * HEADING DISPLAY:
 * ----------------
 *
 * void drawdirection(double angle);       // Draw compass arrow
 * void setTargetHeading(double targ);     // Set target heading
 * void displayHeading();                  // Show compass interface
 *
 *
 * GLOBAL TOUCH COORDINATES:
 * -------------------------
 *
 * extern int brain_x, brain_y;            // Last touch coordinates
 *                                         // Updated in Interface::reset()
 */

/*******************************************************************************
 * VEX LIBRARY INTEGRATION
 ******************************************************************************/

/*
 * KEY VEX OBJECTS & FUNCTIONS:
 * ----------------------------
 *
 * Brain.Screen Methods:
 *   .clearScreen()                        // Clear display
 *   .setPenColor(color)                   // Set drawing color
 *   .setFillColor(color)                  // Set fill color
 *   .drawPixel(x, y)                      // Draw single pixel
 *   .drawLine(x1, y1, x2, y2)            // Draw line
 *   .drawRectangle(x, y, w, h)           // Draw rectangle
 *   .drawCircle(x, y, r)                 // Draw circle
 *   .setCursor(row, col)                 // Set text cursor (12x20 character grid)
 *   .print(format, ...)                  // Printf-style text output
 *   .pressing()                           // Returns true if screen touched
 *   .xPosition()                          // Get touch X coordinate
 *   .yPosition()                          // Get touch Y coordinate
 *
 * Colors:
 *   Hex strings: "#RRGGBB" or named: red, blue, green, yellow, etc.
 *
 * Task Management:
 *   task myTask(functionName, parameter); // Create background task
 *   myTask->stop();                       // Stop task
 *   delete myTask;                        // Clean up
 *
 * Timing:
 *   wait(time, msec);                     // Blocking delay
 *   waitUntil(condition);                 // Wait for condition (custom macro)
 */

/*******************************************************************************
 * INSTRUCTIONS FOR CLAUDE AI AGENTS
 ******************************************************************************/

/*
 * WHEN WORKING WITH THIS PROJECT:
 * ================================
 *
 * 1. UNDERSTANDING THE CODEBASE:
 *    - Start with WLIC.h to understand the Interface class structure
 *    - Review Monitor() function to see how interfaces are connected
 *    - Study existing display functions to understand RLE image format
 *    - Check main.cpp for competition integration points
 *
 *
 * 2. CREATING NEW INTERFACES:
 *    - Define a display function (void functionName())
 *    - Use RLE format for images (get code from VEX Image Converter)
 *    - Define button coordinate vector if interactive
 *    - Create Interface object with appropriate constructor
 *    - Add to Monitor() function's interface hierarchy
 *    - Add activate() call to main event loop
 *
 *
 * 3. MODIFYING EXISTING INTERFACES:
 *    - Locate the display function in WLIC.cpp
 *    - Modify drawing code or RLE data
 *    - Update button coordinates if changing layout
 *    - Ensure linked interfaces are correctly updated
 *
 *
 * 4. DEBUGGING TIPS:
 *    - Check that button coordinates don't overlap
 *    - Verify all interfaces in hierarchy have activate() called
 *    - Ensure updateInterface tasks are properly stopped
 *    - Use Brain.Screen.print() for debug output
 *    - Check index values: -2 (inactive), -1 (active), >=0 (button)
 *
 *
 * 5. COMMON TASKS:
 *
 *    A. Add a new menu screen:
 *       - Create display function with graphics
 *       - Define button coordinates
 *       - Create Interface object
 *       - Link to parent interface
 *       - Add activate() to Monitor() loop
 *
 *    B. Add button to existing screen:
 *       - Add coordinate entry to button_coord vector
 *       - Update linked_Interface vector (if navigation button)
 *       - Update action callback to handle new button index
 *       - Redraw display function to show new button
 *
 *    C. Implement dynamic display:
 *       - Create updateInterface() function (return int)
 *       - Use while(index == -1) loop inside function
 *       - Update display elements as needed
 *       - Return 0 when done
 *       - Pass function to Interface constructor
 *
 *    D. Add custom actions:
 *       - Create void callback(int index) function
 *       - Use switch/if to handle different button indices
 *       - Pass callback to Interface constructor
 *       - Callback executes after navigation (if any)
 *
 *
 * 6. CODE STYLE GUIDELINES:
 *    - Follow existing naming conventions (PascalCase for interfaces)
 *    - Use descriptive variable names (home_coords, Home_linkedItf)
 *    - Comment button coordinate meanings
 *    - Keep display functions focused on rendering only
 *    - Separate logic from display code
 *    - Use const for coordinate and color data
 *
 *
 * 7. PERFORMANCE CONSIDERATIONS:
 *    - RLE images are more efficient than pixel-by-pixel drawing
 *    - Stop update tasks when navigating away (prevents CPU waste)
 *    - Use reasonable wait() delays in main loop (100ms typical)
 *    - Limit updateInterface() frequency to avoid screen flicker
 *    - Cache calculations in update functions when possible
 *
 *
 * 8. TESTING CHECKLIST:
 *    - [ ] All buttons respond to touches
 *    - [ ] Navigation works in both directions
 *    - [ ] No memory leaks (tasks properly stopped/deleted)
 *    - [ ] Display renders correctly
 *    - [ ] No overlapping button regions
 *    - [ ] Update tasks don't interfere with navigation
 *    - [ ] Status updates appear correctly
 *    - [ ] Screen clears between interface transitions
 *
 *
 * 9. INTEGRATION WITH ROBOT CODE:
 *    - main.cpp contains competition template
 *    - pre_auton() runs before match (call Monitor() here for selection)
 *    - autonomous() runs during autonomous period
 *    - usercontrol() runs during driver control
 *    - Interface selections can set global variables for auton choice
 *
 *
 * 10. FILE MODIFICATION GUIDELINES:
 *     - WLIC.h: Add new function declarations, update Interface class
 *     - WLIC.cpp: Implement display functions, modify Monitor()
 *     - main.cpp: Add competition-specific logic
 *     - robot-config.h/cpp: Define robot hardware
 *     - Keep interface code in WLIC files
 *     - Keep robot-specific code in main.cpp
 */

/*******************************************************************************
 * EXAMPLE: CREATING A NEW INTERFACE
 ******************************************************************************/

/*
 * EXAMPLE: Adding a "Settings" interface
 * =======================================
 *
 * 1. Add display function to WLIC.cpp:
 *
 *    void displaySettings() {
 *        Brain.Screen.setPenColor(white);
 *        Brain.Screen.setFillColor(black);
 *        Brain.Screen.printAt(10, 20, "Settings");
 *
 *        // Draw buttons
 *        Brain.Screen.drawRectangle(20, 50, 200, 40);
 *        Brain.Screen.printAt(30, 70, "Brightness");
 *
 *        Brain.Screen.drawRectangle(20, 100, 200, 40);
 *        Brain.Screen.printAt(30, 120, "Volume");
 *
 *        // Back button
 *        Brain.Screen.drawRectangle(340, 180, 140, 60);
 *        Brain.Screen.printAt(360, 210, "Back");
 *    }
 *
 * 2. Add declaration to WLIC.h:
 *
 *    void displaySettings();
 *
 * 3. Add to Monitor() function in WLIC.cpp:
 *
 *    // Settings interface
 *    Interface Settings;
 *
 *    std::vector<std::vector<double>> settings_coords = {
 *        { 20, 220, 50, 90 },     // 0 Brightness
 *        { 20, 220, 100, 140 },   // 1 Volume
 *        { 340, 480, 180, 240 }   // 2 Back
 *    };
 *    std::vector<Interface*> Settings_linkedItf = {
 *        nullptr,  // Brightness (no navigation)
 *        nullptr,  // Volume (no navigation)
 *        &Home     // Back to home
 *    };
 *    Settings = Interface(&settings_coords, &Settings_linkedItf, displaySettings);
 *
 * 4. Link from Util interface:
 *
 *    // Modify Util_linkedItf:
 *    std::vector<Interface*> Util_linkedItf = {
 *        &Home,
 *        &Logo,
 *        &Settings  // Add settings link
 *    };
 *
 * 5. Add to main loop:
 *
 *    while(1) {
 *        Home.activate();
 *        Settings.activate();  // Add this line
 *        // ... other activations
 *        Interface::reset();
 *        wait(100, msec);
 *    }
 *
 * Done! Settings interface is now accessible from Util menu.
 */

/*******************************************************************************
 * TROUBLESHOOTING GUIDE
 ******************************************************************************/

/*
 * COMMON ISSUES & SOLUTIONS:
 * ==========================
 *
 * PROBLEM: Interface doesn't respond to touches
 * SOLUTION:
 *   - Check button_coord values are correct
 *   - Verify activate() is called in main loop
 *   - Ensure Interface::reset() is called
 *   - Check index is set to -1 initially
 *
 * PROBLEM: Screen doesn't clear when navigating
 * SOLUTION:
 *   - Brain.Screen.clearScreen() is called in activate()
 *   - Make sure linked interface exists
 *
 * PROBLEM: Multiple interfaces activate simultaneously
 * SOLUTION:
 *   - The static isActive flag should prevent this
 *   - Check that Interface::reset() is called
 *   - Verify only one interface has index == -1
 *
 * PROBLEM: Update task doesn't stop
 * SOLUTION:
 *   - stopUpdateTask() is called in activate()
 *   - Check while loop condition: while(index == -1 && updateItf_Task)
 *   - Ensure index changes when navigating away
 *
 * PROBLEM: Memory issues/crashes
 * SOLUTION:
 *   - Tasks must be deleted after stopping
 *   - Check for null pointers before dereferencing
 *   - Ensure vectors are not out of bounds
 *
 * PROBLEM: Wrong button activates
 * SOLUTION:
 *   - Check button coordinate overlaps
 *   - Verify linked_Interface vector order matches button indices
 *   - Ensure brain_x, brain_y are being set correctly
 *
 * PROBLEM: Image displays incorrectly
 * SOLUTION:
 *   - Verify RLE data format is correct
 *   - Check color palette array size
 *   - Ensure imageData size is correct
 *   - Verify x wrapping logic: if(x >= 480) { x = 0; y++; }
 */

/*******************************************************************************
 * API QUICK REFERENCE
 ******************************************************************************/

/*
 * INTERFACE CLASS API:
 * ====================
 *
 * Constructors:
 *   Interface()                                           // Empty interface
 *   Interface(display)                                    // Display only
 *   Interface(display, updateInterface)                   // Display + updates
 *   Interface(button_coord, actions, display)             // Interactive
 *   Interface(button_coord, actions, display, update)     // Interactive + updates
 *   Interface(button_coord, linked, display)              // Navigation
 *   Interface(button_coord, linked, actions, display)     // Full interactive
 *   Interface(button_coord, linked, display, update)      // Navigation + updates
 *   Interface(button_coord, linked, actions, display, update) // All features
 *
 * Methods:
 *   int getIndex()                    // Get current index
 *   void setIndex(int)                // Set index (-1 to activate)
 *   int InterfaceChooser()            // Detect button press
 *   void Display()                    // Render interface
 *   int UpdateInterface()             // Background update loop
 *   void stopUpdateTask()             // Stop background task
 *   void Action()                     // Execute action callback
 *   void activate()                   // Main state machine
 *   static void reset()               // Reset after screen release
 *
 *
 * DISPLAY FUNCTIONS:
 * ==================
 *
 *   void displayHome();               // Main menu
 *   void displayHeading();            // Compass display
 *   void displayPowerflow();          // Motor status
 *   void displayAuton();              // Autonomous selector
 *   void displayAutonRed();           // Red alliance auton
 *   void displayAutonBlue();          // Blue alliance auton
 *   void displayLogo();               // Team logo
 *   void displayUtil();               // Utilities menu
 *
 *
 * UTILITY FUNCTIONS:
 * ==================
 *
 *   void setStatus(std::string);      // Update status string
 *   int printPerformance();           // Display performance metrics
 *   void drawdirection(double);       // Draw compass arrow
 *   void setTargetHeading(double);    // Set target heading
 */

/*******************************************************************************
 * VERSION HISTORY & NOTES
 ******************************************************************************/

/*
 * PROJECT STATUS: Active Development
 * LAST UPDATED: 2025-10-23
 *
 * FUTURE ENHANCEMENTS (Commented in code):
 * -----------------------------------------
 * - Auton Class implementation (WLIC.h:68, WLIC.cpp:223-231)
 *   Plans for autonomous routine management class
 *
 * KNOWN LIMITATIONS:
 * ------------------
 * - Single touch only (no multi-touch support)
 * - No touch gestures (swipe, pinch, etc.)
 * - Fixed 480x240 screen resolution
 * - No built-in animation support
 * - Background tasks must be manually managed
 *
 * COMPATIBILITY:
 * --------------
 * - VEX V5 Brain
 * - VEXcode Pro V5 (C++)
 * - VEX SDK version: v5_vcs.h included
 */

/*******************************************************************************
 * END OF DOCUMENTATION
 ******************************************************************************/

#endif // CLAUDE_H
