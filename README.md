# WinLib Interface Class (WLIC)

A powerful C++ framework for building customizable touch-based graphical interfaces on VEX V5 Brain screens.

![VEX V5 Brain](https://img.shields.io/badge/VEX-V5%20Brain-orange)
![C++](https://img.shields.io/badge/Language-C%2B%2B-blue)
![License](https://img.shields.io/badge/License-MIT-green)

## 📋 Overview

WinLib Interface Class (WLIC) is a flexible GUI framework designed for VEX Robotics teams who want to create professional, interactive interfaces on their robot's brain screen. Build custom menus, navigation systems, and dynamic displays with ease.

### Key Features

✅ **Hierarchical Navigation** - Create multi-level menu systems with parent-child relationships
✅ **Touch Detection** - Automatic button press detection with coordinate-based hit testing
✅ **Background Updates** - Run continuous update tasks for dynamic displays
✅ **RLE Image Support** - Efficient Run-Length Encoded image rendering
✅ **Callback System** - Execute custom actions on button presses
✅ **State Management** - Clean interface activation and deactivation

## 🚀 Quick Start

### Prerequisites

- VEX V5 Brain
- VEXcode Pro V5 (C++)
- VEX SDK (V5_20240802_15_00_00 or newer)

### Installation

1. **Clone this repository:**
   ```bash
   git clone https://github.com/YOUR_USERNAME/WinLib_InterfaceClass.git
   cd WinLib_InterfaceClass
   ```

2. **Open in VEXcode:**
   - Open VEXcode Pro V5
   - File → Open Project
   - Select the project folder

3. **Build and Download:**
   - Click the Build button
   - Connect your V5 Brain
   - Download to robot

### Basic Usage

```cpp
#include "WLIC.h"

// Define your display function
void displayMyMenu() {
    Brain.Screen.clearScreen();
    Brain.Screen.printAt(10, 20, "My Custom Menu");
    Brain.Screen.drawRectangle(20, 50, 200, 40);  // Button
}

// Define button coordinates {x_min, x_max, y_min, y_max}
std::vector<std::vector<double>> menu_coords = {
    { 20, 220, 50, 90 }    // Button 0
};

// Create the interface
Interface MyMenu(&menu_coords, nullptr, displayMyMenu);

// In your main loop
MyMenu.setIndex(-1);       // Activate
MyMenu.Display();          // Show
while(1) {
    MyMenu.activate();     // Check for touches
    Interface::reset();
    wait(100, msec);
}
```

## 📖 Documentation

### Interface Class

The `Interface` class is the core of the framework. It supports various combinations of features:

#### Constructor Options

```cpp
// Display only
Interface(void (*display)());

// Interactive with buttons
Interface(button_coord, actions, display);

// Navigation between interfaces
Interface(button_coord, linked_Interface, display);

// Full-featured
Interface(button_coord, linked_Interface, actions, display, updateInterface);
```

#### Key Methods

- `setIndex(int)` - Activate interface (-1) or set button state
- `Display()` - Render the interface and start update tasks
- `activate()` - Main state machine, handles touch detection
- `InterfaceChooser()` - Detect which button was pressed
- `static reset()` - Reset state after screen release

### Button Coordinates

Buttons are defined as rectangular regions:

```cpp
std::vector<std::vector<double>> coords = {
    { x_min, x_max, y_min, y_max }
};
```

**VEX V5 Screen:** 480×240 pixels, origin at top-left (0,0)

Example:
```cpp
{ 340, 480, 20, 60 }  // Button from (340,20) to (480,60)
```

### Navigation System

Link interfaces to create hierarchical menus:

```cpp
std::vector<Interface*> linkedItf = {
    &ChildInterface1,
    &ChildInterface2
};

ParentInterface = Interface(&coords, &linkedItf, displayFunc);
```

Button index determines which child interface to navigate to.

### Image Rendering

WLIC supports Run-Length Encoded (RLE) images for efficient graphics. Convert your images using:

🔗 [VEX Image Converter](https://suhjae.github.io/vex-image/)

## 🏗️ Project Structure

```
WinLib_InterfaceClass/
├── include/
│   ├── WLIC.h              # Interface class header
│   ├── vex.h               # VEX includes and macros
│   ├── robot-config.h      # Robot hardware configuration
│   └── claude.h            # Comprehensive documentation
├── src/
│   ├── WLIC.cpp            # Interface implementation
│   ├── main.cpp            # Competition template
│   └── robot-config.cpp    # Hardware initialization
├── vex/                    # VEX SDK files
├── .vscode/                # VSCode configuration
├── makefile                # Build configuration
├── README.md               # This file
├── LICENSE                 # MIT License
└── .gitignore              # Git ignore rules
```

## 📚 Examples

### Pre-Built Interfaces

The project includes several example interfaces:

- **Home** - Main menu
- **Heading** - Compass/orientation display
- **Powerflow** - Motor and sensor status
- **Auton Selector** - Autonomous routine selection
- **Utilities** - Settings and configuration

See `src/WLIC.cpp` for implementation details.

### Creating a Custom Interface

1. **Define display function:**
   ```cpp
   void displaySettings() {
       Brain.Screen.clearScreen();
       Brain.Screen.printAt(10, 20, "Settings");
       // Draw your interface
   }
   ```

2. **Define button coordinates:**
   ```cpp
   std::vector<std::vector<double>> settings_coords = {
       { 20, 220, 50, 90 },   // Brightness
       { 20, 220, 100, 140 }  // Volume
   };
   ```

3. **Create Interface object:**
   ```cpp
   Interface Settings(&settings_coords, nullptr, displaySettings);
   ```

4. **Add to main loop:**
   ```cpp
   while(1) {
       Settings.activate();
       Interface::reset();
       wait(100, msec);
   }
   ```

## 🛠️ Advanced Features

### Background Update Tasks

Create dynamic displays that update automatically:

```cpp
int updateDisplay() {
    while(this->index == -1) {
        // Update display elements
        Brain.Screen.printAt(10, 20, "Time: %d", Brain.Timer.time());
        wait(100, msec);
    }
    return 0;
}

Interface MyInterface(display, updateDisplay);
```

### Action Callbacks

Execute code when buttons are pressed:

```cpp
void handleActions(int buttonIndex) {
    switch(buttonIndex) {
        case 0:
            // Button 0 pressed
            break;
        case 1:
            // Button 1 pressed
            break;
    }
}

Interface MyInterface(coords, handleActions, display);
```

## 🐛 Troubleshooting

### Interface doesn't respond to touches
- Verify button coordinates are correct
- Check that `activate()` is called in main loop
- Ensure `Interface::reset()` is called after activate

### Screen doesn't clear when navigating
- Make sure linked interfaces are properly set up
- Verify child interface is in the `activate()` loop

### Multiple interfaces activate simultaneously
- Only one interface should have `index == -1`
- Ensure `Interface::reset()` is called regularly

See [claude.h](claude.h) for comprehensive troubleshooting guide.

## 📄 API Reference

For complete API documentation, see [claude.h](claude.h).

### Quick Reference

**Interface Creation:**
```cpp
Interface(display)                              // Basic
Interface(coords, actions, display)             // Interactive
Interface(coords, linked, display)              // Navigation
Interface(coords, linked, actions, display, update)  // Full
```

**Core Methods:**
```cpp
void setIndex(int)           // Activate interface
void Display()               // Render and start updates
void activate()              // Handle touch input
int InterfaceChooser()       // Detect button press
static void reset()          // Reset state
```

## 🤝 Contributing

Contributions are welcome! Here's how you can help:

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/AmazingFeature`)
3. Commit your changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

### Coding Guidelines

- Follow existing code style (see [claude.h](claude.h))
- Comment button coordinates and complex logic
- Test on actual VEX V5 hardware when possible
- Update documentation for new features

## 📝 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## 👥 Authors

- **Win Yeh** - *Initial work*

## 🙏 Acknowledgments

- VEX Robotics for the V5 platform
- [VEX Image Converter](https://suhjae.github.io/vex-image/) by suhjae
- VEX community for feedback and testing

## 📞 Support

- **Issues:** Report bugs via [GitHub Issues](https://github.com/YOUR_USERNAME/WinLib_InterfaceClass/issues)
- **Documentation:** See [claude.h](claude.h) for detailed documentation
- **VEX Forums:** Discuss on [VEX Forum](https://www.vexforum.com/)

## 🗺️ Roadmap

- [ ] Auton Class implementation for autonomous routine management
- [ ] Animation support for transitions
- [ ] Touch gesture support (swipe, pinch)
- [ ] Additional pre-built interface templates
- [ ] GUI designer tool

## 📊 Project Status

**Status:** Active Development
**Version:** 1.0.0
**Last Updated:** October 2025

---

Made with ❤️ for VEX Robotics teams worldwide
