AI's answer for breaking down the CAN setup in the 2026 repo.
Note: we for 2027 we will be using the same inverter CAN frames, but the BMS may be being reconfigured

Yes, there are custom *application-level* frame payloads, but the active firmware uses ordinary classic CAN frames, not a custom physical or CAN frame format. The main VCU CAN setup is Teensy 4.1 `CAN1` at **500 kbit/s**, shared by the BAMOCAR inverter and BMS.

**Live VCU Bus**

In `main.cpp`, `Can1` is a `FlexCAN_T4<CAN1, RX_SIZE_256, TX_SIZE_16>` controller, initialized with `Can1.begin()` and `Can1.setBaudRate(500000)`. The code does not configure CAN filters or explicitly request extended IDs or CAN FD. It drains received frames and dispatches them in `bamocar.cpp`.

The code relies on a physical CAN transceiver and correctly wired, terminated bus; those are outside the software setup. The test-project documentation calls out 120-ohm termination. The active VCU code itself does not document specific CAN pin assignments.

**BAMOCAR Protocol**

The inverter protocol uses IDs `0x201` for commands from the Teensy and `0x181` for inverter replies. The names `BAMOCAR_RX_ID` and `BAMOCAR_TX_ID` are from the BAMOCAR’s perspective, which can be easy to misread. The IDs and register definitions are in `bamocar.h` and `config.h`.

These are 3-byte command frames. The first byte selects a BAMOCAR register or command; the payload conventions are implemented directly in the driver:

- `0x3D` requests a register: byte 1 is the register, byte 2 is the reporting interval in milliseconds; zero requests a one-shot reply.
- `0x90` sends a signed torque value as two little-endian bytes.
- `0x51` controls drive state; `0x8E` clears errors; `0xD0` configures the inverter’s CAN timeout in milliseconds, little-endian.
- Cyclic telemetry requests cover status, errors, speed, and current every 100 ms, and motor/inverter temperatures every 500 ms.

Responses use the same register-first pattern on `0x181`. The VCU decodes status, speed, current, temperatures, DC bus voltage, and error word. The main loop requests status while waiting for the inverter, configures telemetry and a 2000 ms inverter timeout once it responds, and reconnects if no inverter frames arrive for 500 ms. While online, it sends a torque command every 20 ms, sending zero torque when drive is disabled.

**BMS Protocol**

The BMS is a separate, receive-only broadcast protocol on that same bus. Its frame map is described in `Bms.h`, and the base ID is hard-coded as `0x500` in `config.h`. That base must match the BMS firmware’s `can_base_id` setting.

The BMS frames use standard 11-bit IDs and 8-byte payloads:

| IDs | Contents | Rate |
|---|---|---:|
| `0x500` | Pack status: battery voltages, current, state, flags | 100 ms |
| `0x501` | Active and latched fault bitfields | 100 ms |
| `0x510`–`0x522` | Cell voltages, four 16-bit mV values per frame | 100 ms |
| `0x530`–`0x542` | Temperatures, four signed °C × 10 values per frame | 500 ms |

Values are little-endian. The code recognizes `0xFFFF` as an invalid cell value and `0x8000` as an invalid temperature. It retains running min/max cell and temperature values rather than storing every reading; parsing is in `Bms.cpp`.

The current VCU reports BMS status and faults, but its drive torque path is not gated on BMS state or faults. The BMS watchdog marks telemetry stale; it does not currently change the VCU state machine.

**Logging and Diagnostics**

CAN TX and RX frames are sent to the logger with timestamp, direction, ID, length, and payload bytes. The record format is implemented in `Logger.cpp`.

**Other CAN-Related Code**

- `TEENSY_COMMAND_MOTOR` is an older Teensy BAMOCAR implementation using the same bus speed and IDs. Treat the current `TEENSY` firmware as authoritative; test projects may differ in startup sequence and behavior.
- `DUE_SEND_TORQUE` is an Arduino Due interactive BAMOCAR sender, also at 500 kbit/s.
- `PRO_M_MOCK_MC` is an MCP2515-based Pro Micro monitor. Its docs describe bitrate, crystal, pins, and optional filters; its sample sender transmits a generic test frame on `0x123`, not a car protocol frame.
- `VECTOR_MOTOR_CTRL_DASHBOARD` is a CANoe simulation. Its DBC defines a one-byte status message at `0x300`; the CAPL script reacts to BAMOCAR commands on `0x201`.
- The ESP32 project is Wi-Fi-only; the UNO project forwards serial data to a web dashboard; the flash-dashboard project is a UART bridge. None implements CAN communication.

The 2027 architecture is still skeletal: FrontNode initializes CAN1 at 500 kbit/s but has no frame logic; RearNode and `shared/CAN.cpp` contain placeholders, and the 2027 VCU source and Raspberry Pi folders are empty. There is not yet a defined 2027 custom frame map.