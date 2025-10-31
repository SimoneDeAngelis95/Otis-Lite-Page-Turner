# Otis Lite

**Otis Lite** is the essential version of the **Otis** pedal — a compact and focused device built for one purpose:  
**turning pages on a tablet or computer** during live performances or practice sessions.  

Designed for **musicians** who read digital sheet music, Otis Lite gives you smooth, hands-free control over page turns, keeping your focus where it matters — on the music.

---

## Overview

Otis Lite is powered by a **Raspberry Pi Pico 2**, a modern dual-core microcontroller that provides excellent performance and stability.  
The pedal features **three physical buttons**, each paired with an **LED indicator** for clear visual feedback.

### Main Features

- **Two main buttons** for page turning (left and right arrows by default).  
- **One mode button** to switch the arrow direction mode:
  - **Horizontal mode** → buttons send **←** and **→**
  - **Vertical mode** → buttons send **↑** (page up) and **↓** (page down)

A dedicated LED indicates the active mode, so you always know which direction is currently enabled.

---

## Hardware

- **Microcontroller:** Raspberry Pi Pico 2  
- **3 momentary buttons**, each with an LED  
- **LED feedback** for visual confirmation  
- **USB-only connection** — no Bluetooth, no wireless dependencies  
- **Powered via USB (5V)**

Otis Lite is recognized by your device as a **standard USB keyboard (HID)**, so it works out of the box with most sheet music and PDF reader apps — no drivers required.

---

## Pinout (default configuration)

| Component           | Pico Pin | Function                                |
| ------------------- | -------- | --------------------------------------- |
| Mode Button         | GP13     | Switch direction mode                   |
| Left Button         | GP15     | Page back                               |
| Right Button        | GP14     | Page forward                            |
| Left LED            | GP16     | Feedback for left button                |
| Right LED           | GP17     | Feedback for right button               |
| Vertical Mode LED   | GP18     | Indicates current mode is vertical mode |
| Horizontal Mode LED | GP19     | Indicates current mode is vertical mode |

---

## Firmware

The firmware is written in **C++** using the **Raspberry Pi Pico SDK**.  
Otis Lite behaves as a **USB HID keyboard**, sending arrow key codes to the connected device.

---

## License

This project is released under the **GNU General Public License v3 (GPL-3.0)**.  
You are free to use, modify, and redistribute it under the same license terms.
