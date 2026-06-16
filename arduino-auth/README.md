# Arduino Auth System
## Overview

Arduino Auth System, built using the Arduino Uno Q is a hybrid embedded authentication platform that combines microcontroller firmware, a Python host service, and a real-time web interface.

The system demonstrates hardware-backed authentication where a connected device can generate and display a secure one-time code, which the user must enter via a web interface to unlock the system.

The Arduino acts as the embedded control unit responsible for state management, serial communication and LED matrix output, while Python simultaneously runs on the device providing a web server and the generation of secure authentication codes. The web interface communicates in real time with the device using WebSockets.

Demo Video:

---

## Why I Built This

---

## Key Features

- Hardware-backed authentication workflow
- Secure one-time code generation using Python's `secrets` library
- Real-time WebSocket communication between browser and device
- Device State management
- Serial communication between MPU and MCU
- LED matrix output for authentication challenge display
- ...

---

## System Architecture

---

## Design Decisions

### Seperation of Responsibilities

### Serial Communication

### Embedded State Machine Design

---

## Challenges Encountered

--- 

## Lessons Learned

---


