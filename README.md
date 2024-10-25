# M3D-Go-PIO

**M3D-Go-PIO** is an intuitive and versatile library for programming the **M3D Go** modular surface robot autonomously, without relying on Scratch. Designed for use with PlatformIO, this library allows users to develop code for the M3D Go robot, making coding visual, interactive, and simple—ideal for educational use or robotics enthusiasts.

## About M3D Go

The **M3D Go** is a modular, education-focused surface robot with sensors, displays, and BLE support, designed to make coding and robotics accessible to learners of all levels. With its pre-flashed Scratch-compatible core, M3D Go can be used immediately with our custom Scratch deployment, but it also supports fully autonomous programming with the M3D-Go-PIO library for advanced projects.

---

## Why PlatformIO over Arduino?

PlatformIO, compatible with the Arduino ecosystem, offers an advanced, feature-rich experience over the standard Arduino IDE. It’s faster, more robust, and supports a vast ecosystem of extensions, including seamless library management and support for multiple environments. **If you’re considering using Arduino, PlatformIO is an ideal choice for development on the M3D Go!**

---

## Getting Started with M3D Go

There are two paths for getting started with M3D Go: **Scratch** for beginners and **PlatformIO/Arduino** for advanced, autonomous programming. Choose the path that best suits your learning needs!

### Scratch vs. PlatformIO: Which Path to Choose?

|Feature|Scratch ([scratch.markhor3d.com](https://scratch.markhor3d.com))|PlatformIO / Arduino|
| --- | --- | --- |
|**Difficulty**|Beginner-friendly, visual coding|Intermediate to advanced|
|**Programming Style**|Drag-and-drop blocks|Text-based programming|
|**Robot Control**|Limited to pre-defined Scratch functions|Full autonomous control|
|**Ideal for**|Basic robotics and coding concepts|Autonomous robotics and sensor-driven projects|
|**Use Cases**|Class projects, beginners|Advanced projects, competitions|

Both options are available, so you can start with Scratch and progress to PlatformIO when ready!

---

### Path 1: Scratch

The M3D Go comes with a factory-loaded Scratch-compatible core, enabling you to get started with Scratch right out of the box!

1. **Visit the Scratch Site**: Go to our custom-deployed Scratch site at [scratch.markhor3d.com](https://scratch.markhor3d.com) for full integration with M3D Go.
2. **Connect the Robot**: Follow on-screen instructions to connect M3D Go to the Scratch environment.
3. **Explore and Code**: Begin coding with Scratch blocks to create interactive projects with M3D Go, from basic movements to sensor-based actions.

*Note*: If needed, the Scratch core can be rebuilt using the `go_ScratchCore` example. Detailed Scratch instructions can be found in our separate Scratch Documentation.

---

### Path 2: PlatformIO / Arduino Development

This path is ideal for users wanting to program the M3D Go robot autonomously. Follow these steps for setup and coding:

#### Step 1: Install VS Code and PlatformIO

1. **Download and Install VS Code**:
  * [VS Code Download](https://code.visualstudio.com/)
2. **Install PlatformIO Extension**:
  * Open VS Code, go to Extensions (`Ctrl+Shift+X`), search for “PlatformIO IDE,” and install it.

> **Note**: PlatformIO can take a considerable amount of time to download, install, and set up for the first time. Be patient, as it’s worth the wait for a robust coding environment.

#### Step 2: Set Up Your Project

1. **Include the M3D-Go-PIO Library**:
  * To use the published library, add the following line to your `platformio.ini` file:

ini

Copy code

```
lib_deps = markhor3d/M3D-Go-PIO@^1.0.9
```

  * Ensure `platformio.ini` specifies the **controller** as `esp32doit-devkit-v1`.
2. **Open Project in PlatformIO**:
  * Open PlatformIO in VS Code and select “Open Project.”
  * Navigate to your project folder and open it.

#### Step 3: Connect M3D Go and Add Add-Ons

1. **Connect M3D Go**: Ensure M3D Go is connected to your computer via USB.
2. **Attach Any Add-Ons**: Add necessary components such as OLED displays, sensors, and other accessories.

#### Step 4: Code Your Project

1. **Start Coding**: Use the M3D-Go-PIO library functions to program your robot autonomously. Begin with provided examples to understand core functions like movement, sensor reading, and display control.
2. **Build and Upload Code**: Click on the PlatformIO “Build” button (checkmark icon) to compile, then click “Upload” (right-arrow icon) to upload the code to M3D Go.
3. **Monitor Output**: Use the Serial Monitor to debug and view output directly from M3D Go.

---

## Examples Overview

Below are the included example files, what they demonstrate, and how to use them.

|Example File|Demonstrates|Description|
| --- | --- | --- |
|`go_ScratchCore`|Scratch Core Setup|Builds the latest Scratch core on M3D Go for use in the Scratch environment.|
|`go_hello_world`|Display Text|Displays "Hello, world!" text on the OLED screen.|
|`go_basic_movements`|Basic Movements|Moves the bot forward, backward, and in spins with delays.|
|`go_turns`|Basic Turning|Demonstrates left/right turning with specified motor powers.|
|`go_acceleration`|Movement with Acceleration|Demonstrates movement control with gradual acceleration and deceleration.|
|`hinge_basic`|Basic Hinge Control|Controls hinge movement and displays the current angle.|
|`line_basic`|Line Sensor Detection|Detects and displays the bot’s position relative to a line (on, left, right, or off).|
|`line_follower`|Simple Line Following|Makes the bot follow a line, stopping if it leaves the line.|
|`line_follower_debug`|Line Following with Debug Display|Shows line-following status on the OLED screen.|
|`range_basic`|Range Finder Display|Displays distance from an object in real-time on the OLED screen.|
|`range_follower`|Distance Maintenance|Moves the bot forward or backward to maintain a set distance from an object (50mm).|
|`remote_buttons`|Remote-Controlled Movement|Provides BLE-based button control for basic bot movements.|
|`remote_notifications`|Display Notifications on BLE Client|Demonstrates displaying various notification types (Normal, Exclamation, Warning, Error).|
|`remote_plot`|Plotting Data on Remote|Continuously sends range finder values to be plotted on the BLE client screen.|
|`remote_user_input`|User Input Prompt via BLE|Prompts the user for a float input via BLE and displays it on the OLED screen.|

**To Run an Example**:

1. Add the example code to your `src/main.cpp` file.
2. Set the `platformio.ini` file controller to `esp32doit-devkit-v1`.
3. Compile and upload the code to M3D Go.

---

## Visual, Interactive, and Simple Coding with M3D Go

M3D Go is designed to make coding visual, interactive, and straightforward:

* **Visual Feedback**: With the OLED display and BLE remote features, students receive real-time feedback, helping them understand the code’s effect immediately.
* **Interactive Programming**: Students can control M3D Go’s movement, display data, and receive sensor readings interactively. This hands-on experience makes learning more engaging.
* **Simplified Library Functions**: The M3D-Go-PIO library abstracts complex robotics functions into simple, intuitive commands, so students can focus on learning the essentials without being overwhelmed by details.

---

## Purchase M3D Go

M3D Go and its accessories can be purchased from our official site:
**[www.markhor3d.com](https://www.markhor3d.com)**

For inquiries, reach out to our support team:
📧 **Email**: info@markhor3d.com
🌐 **Website**: [www.markhor3d.com](https://www.markhor3d.com)

---

## Contributing

We welcome contributions from the community to improve M3D-Go-PIO! If you have ideas or improvements, feel free to submit a pull request or open an issue on this GitHub repository.

**Thank you for choosing M3D Go as part of your learning journey in robotics and coding!**
