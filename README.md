# 2027
This is the repository for the new comms architecture for the 2027 car

A high-level overview of the physical architecture can be found [here](https://app.mural.co/t/ucdfs1596/m/ucdfs1596/1788373118517/72c41017fdac1014d38ab57ffd399d8fdada9648)

The architecture uses 3 TEENSY 4.1 microcontrollers and a Raspberry Pi 5

The repos for the microcontrollers uses the PlatformIO framework for easy building and uploading of firmware to the TEENSY 4.1 boards. 

There is a separate PlatformIO project for each TEENSY: the front node, the rear node, and the central VCU (see architecture above).

## Getting Started

### Installation

1. Install Visual Studio Code
2. Install the PlatformIO IDE extension
3. Clone this repository:
   ```bash
   git clone https://github.com/UCDFS/2027.git
   ```

### Building the Project

1. Open the project folder in Visual Studio Code
2. PlatformIO will automatically detect the project and install dependencies
3. Build the project using one of these methods:
   - Click the checkmark (✓) icon in the PlatformIO toolbar
   - Use the command palette: `PlatformIO: Build`
   - Run in terminal: `pio run`

### Uploading to Teensy 4.1

1. Connect your Teensy 4.1 board via USB
2. Upload using one of these methods:
   - Click the right arrow (→) icon in the PlatformIO toolbar
   - Use the command palette: `PlatformIO: Upload`
   - Run in terminal: `pio run --target upload`
