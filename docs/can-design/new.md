**Messages**
***CAN1 (Sensor network):***
- 0x10 - APPS pcts - 2B 
  - [Left][Right]
- 0x20 - Front BPS, Front WS, Front Sus - 7B 
  - [BPS][Left WS1][Left WS2][Right WS1][Right WS2][Left Sus][Right Sus]
- 0x28 - Rear BPS, Rear WS, Rear Sus - 7B 
  - [BPS][Left WS1][Left WS2][Right WS1][Right WS2][Left Sus][Right Sus]
- 0x30 - IMU accel - 6B
  - [x1][x2][y1][y2][z1][z2]
- 0x31 - IMU gyro + temp - 6B
  - [x1][x2][y1][y2][z1][z2][temp1][temp2]

***CAN2 (Powertrain) [BMS ids tbc]:***
Frame breakdown tbc
- 0x181 - Inverter replies
- 0x201 - Inverter commands
- 0x500 - BMS pack status: voltage current state flags
- 0x501 - BMS active and latched faults
- 0x510-0x522 - Cell voltages, 4 x 16-bit mV values per frame
- 0x530-0x542 - Cell temps, 4 signed °C × 10 values per frame

***CAN3 (Pi):***
Frame breakdown tbd, we need:
- wheel speeds (rpm)
- suspensions
- brake pressures
- all temperature readings
- car speed (kph)
- all IMU readings
- GPS data
- BMS readings
- Motor torque

CAN frame bit structures (8-byte)
[0][1-11][12][13-18][19-82][83-98][99-100][101-107][108-110]
SOF  ID  RTR  CTRL   DATA    CRC     ACK    EOF       IFS

SOF - start of frame
RTR - remote transmission request
CTRL - control
CRC - cyclic redundency check
ACK - acknowledgment
EOF - end of frame
IFS - intermission

CAN_message_t structure:
typedef struct CAN_message_t {
  uint32_t id = 0;          // 11-bit or 29-bit identifier
  uint16_t timestamp = 0;   // Free-running timer timestamp (captured on RX)
  struct {
    uint8_t extended:1;     // 0 = Standard ID (11-bit), 1 = Extended ID (29-bit)
    uint8_t remote:1;       // 0 = Data frame, 1 = Remote transmission request (RTR)
    uint8_t overrun:1;      // 1 = Mailbox overrun occurred (RX only)
    uint8_t reserved:5;     // Reserved bits for internal use
  } flags;
  uint8_t len = 8;          // Data length code (0 to 8 bytes)
  uint8_t buf[8] = { 0 };   // 8-byte data payload buffer
  uint8_t mb = 0;           // Hardware mailbox index used for RX or TX
  uint8_t bus = 0;          // Originating bus index (e.g., 1 for CAN1, 2 for CAN2)
} CAN_message_t;