# TACTNET — Tactical Mesh Network

## Motivation

During my service in the Republic of Korea Army, I maintained TICN C4I tactical communications infrastructure, a military mesh network used for real-time data exchange across distributed nodes. That experience gave me a concrete understanding of how mesh routing, failover detection, and encrypted radio communication work at the operational level, but not at the implementation level. TACTNET is a project that aims to bridge that gap by rebuilding those technical concepts with commodity hardware, drivers and algorithms. 

## Features

- Custom UART driver for REYAX RYLR998 LoRa module
- Custom I2C driver for Bosch BME280 environmental sensor
- Priority-based task scheduler with interval tracking
- Mesh relay routing: Node 1 → Node 2 → Node 3
- AES128 hardware encryption via RYLR998 AT+CPIN
- Timestamp-based failover detection (30 second timeout)
- Environmental alert system: temperature and humidity thresholds
- Live Flask dashboard with auto refresh and event log

## System Overview

TACTNET is a 3-node LoRa mesh network where each node runs custom bare-metal C++ drivers for UART radio communication and I2C sensor reading. Node 1 (Main) broadcasts BME280 sensor data every 5 seconds via the scheduler, Node 2 (Relay) stores and relays to Node 3 while sending back its own readings, and Node 3 (End) forwards acknowledgments to Node 1. All transmissions are AES128 encrypted, nodes detect failures via timestamp based keepalive, and a Flask dashboard visualizes live sensor data from all three nodes.

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
- VDD <Pin 1 (3.3V)> → GND <Pin 6>
- RXD <Pin 8 (GPIO14 TX)> → TXD <Pin 10 (GPIO15 RX)>

**BME280 → Pi (I2C, /dev/i2c-1, 0x76):**
- VCC <Pin 17 (3.3V)> → GND <Pin 9>
- SDA <Pin 3 (GPIO2)> → SCL <Pin 5 (GPIO3)>

## Hardware Architecture

See `docs/architecture_diagram.png`

## Software Architecture

**LoRaDriver** — UART driver for RYLR998. Configures `/dev/serial0` via `termios`, sends AT commands, reads responses byte by byte, and provisions AES128 key via `AT+CPIN`.

**BME280Driver** — I2C driver. Reads 24 bytes of factory calibration from chip registers and applies Bosch compensation formulas to produce calibrated temperature, humidity, and pressure.

**Message** — Data unit with sender ID, destination ID, MessageType enum (SENSORREADING, STATUS_PING, ERROR), and pipe-delimited payload. 

**Task / Scheduler** — Priority-based scheduler. Each task tracks `last_executed` timestamp. 

**Node** — Owns both LoRaDriver and BME280Driver. Manages neighbor addresses, circular message buffer (10 messages), relay routing, and timestamp-based failover.

**Flask Dashboard** — Python Flask server reads `/tmp/tactnet_data.json` written by the C++ program and serves a live dashboard at port 5000.

## Project Structure

```
Tactnet/
├── include/      # header files
├── src/          # driver and class implementations  
├── dashboard/    # Flask server and HTML template
├── docs/         # wiring and architecture diagrams
└── main.cpp      # node entry point
```
## DEMO

