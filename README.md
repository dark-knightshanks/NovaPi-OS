<div align="center">

#  NovaPi

**Bare-metal OS on Raspberry Pi with HDMI and a Pong game from scratch.**

![Language](https://img.shields.io/badge/Language-C-blue?style=flat-square)
![Platform](https://img.shields.io/badge/Platform-Raspberry%20Pi%204B-red?style=flat-square)
![Bare Metal](https://img.shields.io/badge/Type-Bare%20Metal-green?style=flat-square)
![Architecture](https://img.shields.io/badge/Arch-AArch64-orange?style=flat-square)

</div>

---

##  About the Project
The project deals with creating a bare-metal operating system and hardware abstraction layer for
Raspberry Pi 4B. The operating system will implement basic communication protocols, interrupt
controllers and ultimately HDMI support. Using these as the base this project will aim at creating a
simple pong game using the HDMI Support of our kernel.

### What is Bare Metal Programming?
A bare metal program runs directly on the processor with no operating
system underneath it. There is no kernel managing memory, no drivers
abstracting hardware, no scheduler. Every peripheral you want to use
has to be initialized and driven by your own code, by writing directly
to the hardware registers documented in the datasheet.

### Why Bare Metal on Raspberry Pi?
<img width="383" height="269" alt="image" src="https://github.com/user-attachments/assets/af28ee35-05fd-4a5a-9cce-4cebd5d30456" />




The Raspberry Pi 4B is built around an ARM Cortex-A72 processor with
a set of memory-mapped peripherals-GPIO, UART, timers, a GPU mailbox
interface. Normally all of this is abstracted away by Linux. This
project skips Linux entirely and talks to the hardware directly, which
gives a much clearer picture of what the hardware is actually doing
and how the software relates to it.

## Communication Protocols Explored

Building NovaPi involved working with three standard hardware
communication protocols. Not all of them ended up in the final Pong
game, but each was implemented and tested as part of understanding
how the Raspberry Pi 4B talks to peripherals.

**UART (Universal Asynchronous Receiver-Transmitter)** is what the
final project uses for keyboard input and debug output. It is a simple
two-wire protocol-no clock line, both sides just agree on a baud
rate (115200 here). A USB-to-TTL adapter bridges the Pi's PL011 UART
to the laptop, which is how keypresses reach the game.

<img width="376" height="196" alt="image" src="https://github.com/user-attachments/assets/8f7fe13a-93c4-4efe-8939-d2d5dd0b3438" />


**SPI (Serial Peripheral Interface)** is a four-wire synchronous
protocol with a dedicated clock line driven by the master. It is
significantly faster than UART and is commonly used for displays and
sensors. The SPI driver was implemented and tested on the Pi as part
of exploring peripheral communication options.

<img width="320" height="340" alt="image" src="https://github.com/user-attachments/assets/fad28c31-baca-40bb-ad13-54031f085dce" />


**I2C (Inter-Integrated Circuit)** uses just two wires — SDA (data)
and SCL (clock)-and supports multiple devices on the same bus
through unique device addresses. The I2C driver was written and used
to communicate with the OLED display during development.

<img width="263" height="350" alt="image" src="https://github.com/user-attachments/assets/79609134-f986-4821-a62f-31065c793a72" />


---

##  Demo

>


https://github.com/user-attachments/assets/7bd8c48b-310d-4eec-8977-28338f951de3


---
## Why NovaPi Matters

Most embedded projects run on top of an OS or a framework that hides
the hardware. NovaPi does none of that. Every driver — UART, GPIO,
framebuffer, timers-is written from scratch and talks directly to
the hardware registers. The end result is a playable game, but the
real outcome is a solid understanding of how a computer actually boots,
how a CPU talks to peripherals, and how a display gets its pixels.

---

##  Hardware

| Component | Purpose |
|-----------|---------|
| Raspberry Pi 4B | Main board |
| MicroSD Card | Holds boot image |
| HDMI Monitor | Game display |
| Micro HDMI to HDMI Cable | Connect Pi to monitor |
| USB-C Power Cable | Power the Raspberry Pi 4B |
| USB-to-TTL Adapter | Keyboard input via UART |
| Jumper Wires | UART connections |


---

### UART Wiring

```
USB-to-TTL  →  Raspberry Pi 4B
GND         →  Pin 6  (GND)
TXD         →  Pin 10 (GPIO 15)
RXD         →  Pin 8  (GPIO 14)
VCC         →  NOT CONNECTED
```
<img width="616" height="363" alt="image" src="https://github.com/user-attachments/assets/d14ff8ca-8360-4cc2-bc8a-c2b5ad883683" />

---

##  Project Structure

```
NovaPi/
├── include/
│ ├── peripherals/
│ │ ├── entry.h
│ │ ├── gpio.h
│ │ ├── i2c.h
│ │ ├── irq.h
│ │ ├── mailbox.h
│ │ ├── mini_uart.h
│ │ ├── mm.h
│ │ ├── oled.h
│ │ ├── pl011.h
│ │ ├── pong.h
│ │ ├── printf.h
│ │ ├── spi.h
│ │ ├── sysregs.h
│ │ ├── timer.h
│ │ ├── utils.h
│ │ └── video.h
├── src/
│ ├── boot.S                 # AArch64 boot stub
│ ├── entry.S                # Exception vector table
│ ├── irq.S                  # IRQ handler (assembly)
│ ├── PL011.c                # PL011 UART driver
│ ├── fontDATA.c             # Bitmap font data
│ ├── gpio.c                 # GPIO driver
│ ├── i2c.c                  # I2C driver
│ ├── irq.c                  # IRQ handler (C)
│ ├── kernel_main.c          # Entry point + game loop
│ ├── mailbox.c              # GPU mailbox interface
│ ├── mini_uart.c            # Mini UART driver
│ ├── mm.S                   # Memory management
│ ├── oled.c                 # OLED display driver
│ ├── pong.c                 # Pong game logic
│ ├── printf.c               # printf implementation
│ ├── spi.c                  # SPI driver
│ ├── timer.c                # System timer
│ ├── utils.S                # Utility functions
│ └── video.c                # Framebuffer + drawing
├── config.txt               # RPi firmware config
├── linker.ld                # Linker script
└── Makefile
```

---

##  Driver Documentation

### GPIO (`gpio.c`)
Configures GPIO pins using `GPFSEL` registers (3 bits per pin for function
select) and `GPIO_PUP_PDN_CNTRL` registers for pull-up/down control.

```c
set_gpfsel(pin, mode);       // Set pin function (0=input, 4=ALT0, etc.)
set_gpio_pull(pin, mode);    // 0=none, 1=pull-up, 2=pull-down
```

### UART / PL011 (`PL011.c`)
Initializes the PL011 UART at 115200 baud on GPIO 14/15 (ALT0).
Used for serial debug output and reading keyboard input.

```c
uart_init();        // Initialize PL011 UART
uart_recv();        // Blocking read one character
pl011_can_recv();   // Non-blocking: returns 1 if data available
pl011_putc(c);      // Send one character
```

### Framebuffer / Video (`video.c`, `mailbox.c`)
Requests a 640×480 32bpp framebuffer from the GPU via the mailbox
property interface. Exposes drawing primitives used by the game.

```c
mail_framebuffer(640, 480, 32);          // Request framebuffer
draw_pixels(x, y, color);               // Draw single pixel
draw_rectangle(x, y, h, w, color);      // Draw filled rectangle
draw_circle(x, y, radius, color);       // Draw filled circle
video_draw_string(str, x, y);           // Render bitmap text
```

### Timer (`timer.c`)
Uses the ARM system counter for simple busy-wait delays.

```c
delay(n);   // Spin-wait for ~n cycles
```

### Pong Game (`pong.c`)
Implements game state, collision detection, AI opponent, and
dirty-rectangle rendering (only redraws changed areas each frame).

```c
PingPong_Init();                  // Reset game state
PingPong_Update(left, right);     // Advance game by one tick
PingPong_Draw();                  // Render current frame
```

---
##  Getting Started

### Prerequisites

- Basic knowledge of C programming
- Understanding of computer architecture (registers, memory-mapped I/O)
- Linux environment (Ubuntu recommended)

**Helpful learning links/Resources:**
- [RasPi OS](https://github.com/s-matyukevich/raspberry-pi-os)
- [RasPi OS Blog](https://s-matyukevich.github.io/raspberry-pi-os/)
- [BCM2711 Datasheet (RPi 4B peripheral register reference)](https://datasheets.raspberrypi.com/bcm2711/bcm2711-peripherals.pdf)
- [Raspberry Pi Firmware Mailbox Property Interface](https://github.com/raspberrypi/firmware/wiki/Mailbox-property-interface)
- [Raspberry Pi Bare Metal](https://youtube.com/playlist?list=PLVxiWMqQvhg9FCteL7I0aohj1_YiUx1x8&si=8HMmJpgDHByHFYfw)
- [C Programming Series](https://www.youtube.com/playlist?list=PLvv0ScY6vfd8M-Mi_Vyrg7KgISTW3Sklt)
- [Raspberry Pi C/C++ Baremetal Programming](https://www.youtube.com/watch?v=mshVdGlGwBs)
- [Assembly Language Programming with ARM ](https://www.youtube.com/watch?v=gfmRrPjnEw4)
---

### Installation

Install the AArch64 cross-compiler:

```bash
sudo apt install gcc-aarch64-linux-gnu make
```
---
### Usage

**Clone the repository:**
```
git clone https://github.com/YOUR_USERNAME/NovaPi.git
cd NovaPi
```
**Build:**
```
make clean
make
```
**Copy to SD card:**

```
sudo cp kernel8.img /media/$USER/bootfs/
sync
sudo umount /media/$USER/bootfs
```
**Connect UART and open serial monitor:**

```
sudo minicom -b 115200 -o -D /dev/ttyUSB0
```
**Insert SD card into Pi, connect HDMI monitor, power on.**
---
##  How It Works

1. `boot.S` sets up the AArch64 stack and jumps to `kernel_main()`
2. PL011 UART is initialized for serial I/O
3. GPU mailbox allocates a 640×480 framebuffer
4. Game loop runs: read UART → update state → draw frame

---

##  Controls

| Key | Action |
|-----|--------|
| `A` | Move paddle left |
| `D` | Move paddle right |

---

## Contributors:
- [**Rishiraj Rajgor**](https://github.com/dark-knightshanks)
- [**Moksh Panicker**](https://github.com/moxybaba)

------

## Mentors:
- [**Shaunak Datar**](https://github.com/ShaunakKDatar)
- [**Aaditya Joil**](https://github.com/JaytidaA)
- [**Prithvi Tambewagh**](https://github.com/rkt-1597)

---


