# TACTNET — Tactical Mesh Network


## Project Description

TACTNET is a 3-node LoRa mesh network where each node runs custom bare-metal C++ drivers for UART radio communication and I2C sensor reading. Node 1 broadcasts BME280 sensor data every 5 seconds via the scheduler, Node 2 stores and relays to Node 3 while sending back its own readings, and Node 3 forwards acknowledgments to Node 1. All transmissions are AES128 encrypted, nodes detect failures via timestamp-based keepalive, and a Flask dashboard visualizes live sensor data from all three nodes.

## Features

- Custom UART driver for REYAX RYLR998 LoRa module
- Custom I2C driver for Bosch BME280 environmental sensor
- Priority-based task scheduler with interval tracking
- Mesh relay routing — Node 1 → Node 2 → Node 3
- AES128 hardware encryption via RYLR998 AT+CPIN
- Timestamp-based failover detection (30 second timeout)
- Environmental alert system — temperature and humidity thresholds
- Live Flask dashboard with auto-refresh and event log

## Hardware

| Component | Quantity |
|-----------|----------|
| Raspberry Pi Zero 2W | 3 |
| REYAX RYLR998 LoRa 915MHz | 3 |
| Bosch BME280 (I2C, 3.3V) | 3 |
| SanDisk 32GB MicroSD | 3 |

## Wiring

See `docs/wiring_diagram.png`

**RYLR998 → Pi (UART, /dev/serial0, 115200 baud):**
- VDD → Pin 1 (3.3V) — GND → Pin 6
- RXD → Pin 8 (GPIO14 TX) — TXD → Pin 10 (GPIO15 RX)

**BME280 → Pi (I2C, /dev/i2c-1, 0x76):**
- VCC → Pin 17 (3.3V) — GND → Pin 9
- SDA → Pin 3 (GPIO2) — SCL → Pin 5 (GPIO3)

## Build

```bash
git clone https://github.com/rlatjdgns/Tactnet.git
cd Tactnet
g++ -std=c++17 src/message.cpp src/node.cpp src/task.cpp src/scheduler.cpp \
    src/LoRaDriver.cpp src/bme280_driver.cpp main.cpp -Iinclude -o tactnet
```

## Run

```bash
./tactnet 1   # Node 1 — broadcaster
./tactnet 2   # Node 2 — relay
./tactnet 3   # Node 3 — end node
```

**Dashboard:**
```bash
cd dashboard && python3 app.py
# http://tactnet-node1.local:5000
```

## Architecture

See `docs/architecture_diagram.png`

## Project Structure

```
Tactnet/
├── include/      # header files
├── src/          # driver and class implementations  
├── dashboard/    # Flask server and HTML template
├── docs/         # wiring and architecture diagrams
└── main.cpp      # node entry point
```
