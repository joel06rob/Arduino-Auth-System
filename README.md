# Arduino Auth System
## Overview

Arduino Auth System, built using the Arduino Uno Q is a hybrid embedded authentication platform that combines microcontroller firmware, a Python host service, and a real-time web interface.

The system demonstrates hardware-backed authentication where a connected device can generate and display a secure one-time code, which the user must enter via a web interface to unlock the system.

The Arduino acts as the embedded control unit responsible for state management, serial communication and LED matrix output, while Python simultaneously runs on the device providing a web server and the generation of secure authentication codes. The web interface communicates in real time with the device using WebSockets.

Demo Video:

---

## Why I Built This

I built this as part of my transition from software engineering into embedded systems development.

Rather than creating another standalone microcontroller project, I wanted to explore the Arduino Uno Q's capabilites and explore how firmware can interact with higher-level software through well-defined interfaces. This allowed me to gain experience designing a simple multi-layer architecture involving:
- Javascript frontend
- Python application logic
- Embedded C++ firmware
- Hardware state management

---

## Key Features

- Hardware-backed authentication workflow
- Secure one-time, 3 digit code generation using Python's `secrets` library
- Real-time WebSocket communication between browser and device
- Device State management
- Communication between MPU and MCU using Arduino's Bridge API which abstracts the underlying transport layer
- Custom bitmap rendering system for displaying authentication challenge digits
- Seperation of presentation, application logic and firmware responsibilities

---

## System Architecture

---

## Design Decisions

### Seperation of Responsibilities
The system is split into three distinct layers: Browser frontend, Python application logic, and embedded firmware.

This seperation was intentional to keep logic clean.

- **Browser** is responsible only for user interaction and displaying system state.
- **Python** acts as the application controller, handling authentication logic, code generation and validation.
- **Arduino Firmware** is responsible for the hardware-specific tasks such as LED matrix rendering and maintaining display state.

This design improves maintainability because each layer can be modified independantly without affecting the others. For example, authentication logic could be replaced/extended without requiring changes to firmware.


### Embedded State Machine Design
The firmware uses a simple state machine (`IDLE`, `SHOWING_CODE`, `UNLOCKED`) to manage display behaviour.

All visual output is determined by the current system state via the `updateDisplay()` function.

- This design was chosen as it helps prevent inconsisten display behaviour by keeping a simple state at all time.
- It also makes it easy to extend the system with new states later (i.e. `ERROR`) without modifying existing logic.

### Bridge API (RPC-Based Communication)
Communication between Python and the Arduono firmware is handled using the Arduino Bridge API, which provides an RPC-style interface.

Instead of manually implementing serial communication protocols, the Bridge system allows functions to be exposed on the firmware and called directly from Python and vise-versa.

The reason for this design choice is:

- It abstracts away low-level communication details such as serial framing and parsing.
- It allows function-level communication (i.e. `processAuthCode`, `unlockAuth`).
- It reduces development complexity whilst maintaining bidirectional communication between the layers.

---

## Challenges Encountered

### Rendering digits
Initially, the design choice was to store the full 13x8 bitmap frames for the digits. However I then realised that reusable digit bitmaps were much more efficient and cleaner.

### Synchronising communication between layers
...

### Display buffer
...

--- 

## Lessons Learned

---

## Future Improvements

- Improve bitmap rendering by packing digit and icon data into hexadecimal representations to reduce memory usage and improve efficiency.
- 
