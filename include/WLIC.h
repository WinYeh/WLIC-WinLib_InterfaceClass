/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       WLIC.h                                                    */
/*    Author:       Win Yeh                                                   */
/*    Created:      2025                                                      */
/*    Description:  WinLib Interface Class - Core header file                 */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#pragma once

/*-----------------------------------------------------------------------------
 * GLOBAL VARIABLES
 *---------------------------------------------------------------------------*/

/**
 * @brief Global X coordinate of last touch position on Brain screen
 * @note Updated by Interface::reset() after a screen press is released
 */
extern int brain_x;

/**
 * @brief Global Y coordinate of last touch position on Brain screen
 * @note Updated by Interface::reset() after a screen press is released
 */
extern int brain_y;

/*  To convert the drawing of an Interface into code, please go to
    VEX Image Converter: https://suhjae.github.io/vex-image/  */

/*-----------------------------------------------------------------------------
 * INTERFACE CLASS
 *---------------------------------------------------------------------------*/

/**
 * @class Interface
 * @brief Core class for building touch-based graphical interfaces on VEX V5 Brain
 *
 * The Interface class provides a complete framework for creating interactive
 * menu systems with hierarchical navigation, touch detection, and dynamic
 * displays. It supports multiple configuration options through constructor
 * overloads for different use cases.
 *
 * Key Features:
 * - State-based navigation system with parent-child relationships
 * - Automatic touch detection using button coordinate matching
 * - Background update tasks for real-time dynamic displays
 * - Action callbacks for custom button press handlers
 * - Flexible constructor overloads for various interface types
 *
 * @note See API_Reference.md for complete documentation and examples
 */
class Interface
{
    protected:
        /**
         * @brief Interface state tracker
         *
         * Values:
         *  -2 = Inactive (interface not displayed)
         *  -1 = Active (interface displayed, waiting for input)
         * >=0 = Button index that was pressed
         */
        int index;

        /**
         * @brief Pointer to button coordinate definitions
         *
         * Each button is defined by {x_min, x_max, y_min, y_max}
         * Touch detection matches coordinates against these regions
         *
         * @note VEX V5 screen is 480x240 pixels, origin at top-left
         */
        const std::vector<std::vector<double>>* button_coord;

        /**
         * @brief Pointer to vector of child interface pointers
         *
         * Enables hierarchical navigation. When button N is pressed,
         * linked_Interface[N] becomes the active interface.
         *
         * @note Used for building multi-level menu systems
         */
        std::vector<Interface*>* linked_Interface;

        /**
         * @brief Function pointer for action callbacks
         *
         * Called when a button is pressed. The button index is passed
         * as a parameter, allowing custom handling for each button.
         *
         * @param buttonIndex The index of the button that was pressed
         */
        void (*actions)(int);

        /**
         * @brief Function pointer for rendering the interface
         *
         * Called by Display() to draw the interface graphics on the
         * Brain screen. Should contain all drawing code (text, shapes,
         * images, etc.)
         */
        void (*display)();

        /**
         * @brief Function pointer for background update tasks
         *
         * Continuously called while interface is active (index == -1).
         * Used for dynamic displays like sensor readings, timers, etc.
         *
         * @return Should return 0 when complete
         */
        int (*updateInterface)();

        /**
         * @brief Pointer to the background update task
         *
         * Stores the VEX task object for the update function. Managed
         * automatically by Display() and stopUpdateTask().
         */
        task* updateItf_Task = nullptr;

        /**
         * @brief Static flag preventing simultaneous interface activations
         *
         * Ensures only one interface responds to touch input at a time.
         * Set by activate(), cleared by reset().
         */
        static int isActive;
    public:
        /*---------------------------------------------------------------------
         * CONSTRUCTORS
         *-------------------------------------------------------------------*/

        /**
         * @brief Default constructor - Creates an empty, inactive interface
         *
         * Creates an interface with no functionality. Useful as a placeholder
         * that will be initialized later.
         */
        Interface ();

        /**
         * @brief Display-only constructor
         *
         * Creates a static interface that displays graphics but has no
         * interactive elements.
         *
         * @param display Function to render the interface graphics
         */
        Interface (void (*display)());

        /**
         * @brief Display with background updates constructor
         *
         * Creates an interface with dynamic content that updates continuously
         * while displayed (e.g., sensor values, timers).
         *
         * @param display Function to render the interface graphics
         * @param updateInterface Function for background update tasks
         */
        Interface (void (*display)(),
                   int (*updateInterface)());

        /**
         * @brief Interactive interface with actions constructor
         *
         * Creates an interface with buttons that trigger custom actions
         * when pressed. No navigation to other interfaces.
         *
         * @param button_coord Pointer to button coordinate definitions
         * @param actions Callback function for button press handling
         * @param display Function to render the interface graphics
         */
        Interface (std::vector<std::vector<double>>* button_coord,
                   void (*actions)(int),
                   void (*display)());

        /**
         * @brief Interactive interface with actions and updates constructor
         *
         * Combines button actions with background updates for interactive
         * interfaces that also display dynamic content.
         *
         * @param button_coord Pointer to button coordinate definitions
         * @param actions Callback function for button press handling
         * @param display Function to render the interface graphics
         * @param updateInterface Function for background update tasks
         */
        Interface (std::vector<std::vector<double>>* button_coord,
                   void (*actions)(int),
                   void (*display)(),
                   int (*updateInterface)());

        /**
         * @brief Navigation interface constructor
         *
         * Creates an interface with buttons that navigate to child interfaces.
         * Each button links to a specific child interface by index.
         *
         * @param button_coord Pointer to button coordinate definitions
         * @param linked_Interface Pointer to vector of child interface pointers
         * @param display Function to render the interface graphics
         */
        Interface (std::vector<std::vector<double>>* button_coord,
                   std::vector<Interface*>* linked_Interface,
                   void (*display)());

        /**
         * @brief Navigation interface with actions constructor
         *
         * Creates an interface that both navigates to children AND triggers
         * custom actions on button press.
         *
         * @param button_coord Pointer to button coordinate definitions
         * @param linked_Interface Pointer to vector of child interface pointers
         * @param actions Callback function for button press handling
         * @param display Function to render the interface graphics
         */
        Interface (std::vector<std::vector<double>>* button_coord,
                   std::vector<Interface*>* linked_Interface,
                   void (*actions)(int),
                   void (*display)());

        /**
         * @brief Navigation interface with updates constructor
         *
         * Creates a navigation interface with background updates for
         * dynamic content that changes while displayed.
         *
         * @param button_coord Pointer to button coordinate definitions
         * @param linked_Interface Pointer to vector of child interface pointers
         * @param display Function to render the interface graphics
         * @param updateInterface Function for background update tasks
         */
        Interface (std::vector<std::vector<double>>* button_coord,
                   std::vector<Interface*>* linked_Interface,
                   void (*display)(),
                   int (*updateInterface)());

        /**
         * @brief Full-featured constructor (all features enabled)
         *
         * Creates an interface with all capabilities: navigation, actions,
         * and background updates. The most flexible configuration.
         *
         * @param button_coord Pointer to button coordinate definitions
         * @param linked_Interface Pointer to vector of child interface pointers
         * @param actions Callback function for button press handling
         * @param display Function to render the interface graphics
         * @param updateInterface Function for background update tasks
         */
        Interface (std::vector<std::vector<double>>* button_coord,
                   std::vector<Interface*>* linked_Interface,
                   void (*actions)(int),
                   void (*display)(),
                   int (*updateInterface)()); 

        /*---------------------------------------------------------------------
         * PUBLIC METHODS
         *-------------------------------------------------------------------*/

        /**
         * @brief Get the current interface state
         *
         * @return int Current index value
         *         -2 = Inactive (not displayed)
         *         -1 = Active (displayed, waiting for input)
         *        >=0 = Button index that was pressed
         */
        int getIndex();

        /**
         * @brief Set the interface state
         *
         * Used to manually control interface activation. Set to -1 to
         * activate an interface, -2 to deactivate.
         *
         * @param i New index value to set
         */
        void setIndex(int i);

        /**
         * @brief Detect which button was pressed based on touch coordinates
         *
         * Checks if global touch coordinates (brain_x, brain_y) fall within
         * any defined button regions. Updates the index to match the button.
         *
         * @return int Button index that was pressed, or -1 if no match found
         *
         * @note Automatically called by activate() during touch detection
         */
        int InterfaceChooser();

        /**
         * @brief Render the interface on the Brain screen
         *
         * Calls the display() function pointer to draw the interface graphics.
         * If an updateInterface() function is provided, launches it as a
         * background task.
         *
         * @note Call this once when switching to a new interface
         */
        void Display();

        /**
         * @brief Background update task wrapper function
         *
         * Internal method that runs the updateInterface() function in a loop
         * while the interface is active (index == -1). Stops when interface
         * is deactivated.
         *
         * @return int Always returns 0 when loop exits
         *
         * @note Called automatically by Display(), not intended for direct use
         */
        int UpdateInterface();

        /**
         * @brief Static helper to launch update task
         *
         * Required by VEX task system to launch UpdateInterface() as a task.
         * Converts void* parameter back to Interface pointer.
         *
         * @param obj Pointer to Interface object (cast as void*)
         * @return int Always returns 0
         *
         * @note Internal use only - called by Display()
         */
        static int launchUpdateItf(void* obj);

        /**
         * @brief Stop and delete the background update task
         *
         * Safely terminates the update task if one is running. Automatically
         * called when navigating away from an interface.
         *
         * @note Safe to call even if no task is running
         */
        void stopUpdateTask();

        /**
         * @brief Execute the action callback function
         *
         * Calls the actions() function pointer with the current button index.
         * Triggered automatically by activate() when a button is pressed.
         *
         * @note Only called if actions() function pointer is not null
         */
        void Action();

        /**
         * @brief Main interface state machine
         *
         * Handles the complete interface lifecycle:
         * 1. Check if this interface is active (index == -1)
         * 2. Detect button presses via InterfaceChooser()
         * 3. Navigate to child interfaces if linked_Interface exists
         * 4. Execute action callbacks
         * 5. Manage interface state transitions
         *
         * @note Must be called regularly in main loop for all interfaces
         *
         * @code
         * while(1) {
         *     Interface1.activate();
         *     Interface2.activate();
         *     Interface::reset();
         *     wait(100, msec);
         * }
         * @endcode
         */
        void activate();

        /**
         * @brief Reset the interface system after touch release
         *
         * Static method that should be called after all activate() calls
         * in the main loop. Updates global touch coordinates (brain_x, brain_y)
         * and resets the isActive flag to allow new touch detection.
         *
         * @note Must be called once per loop iteration after all activations
         *
         * @code
         * Interface::reset();  // Called as static method
         * @endcode
         */
        static void reset();
};

/* ---------- Auton Class coming soon.... ------------ */
