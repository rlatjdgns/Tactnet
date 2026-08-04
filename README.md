# TACTNET — Tactical Mesh Network

## Motivation

During my service in the Republic of Korea Army, I maintained TICN C4I tactical communications infrastructure, a military mesh network used for real-time data exchange across distributed nodes. That experience gave me a concrete understanding of how mesh routing, failover detection, and encrypted radio communication work at the operational level, but not at the implementation level. TACTNET is a project that aims to bridge that gap by rebuilding those technical concepts with commodity hardware, drivers and algorithms. 

## Features

- Custom UART driver for REYAX RYLR998 LoRa module
- Custom I2C driver for Bosch BME280 environmental sensor
- Priority-based task scheduler with interval tracking
- Mesh relay routing: Node 1 → Node 2 → Node 3
- AES128 link-layer encryption, provisioned on the RYLR998 via AT+CPIN
- Timestamp-based failover detection (30 second timeout)
- Environmental alert system: temperature and humidity thresholds
- Live Flask dashboard with auto refresh and event log

## System Overview

TACTNET is a 3-node LoRa mesh network where each node runs custom C++ drivers written directly against the Linux device interfaces (termios on /dev/serial0 for the radio and /dev/i2c-1 for the sensor) with no third-party HAL or sensor libraries. Node 1 (Main) broadcasts BME280 sensor data every 5 seconds via the scheduler, Node 2 (Relay) stores and relays to Node 3 while sending back its own readings, and Node 3 (End) forwards acknowledgments to Node 1. All transmissions are AES128 encrypted, nodes detect failures via timestamp based keepalive, and a Flask dashboard visualizes live sensor data from all three nodes.

## Demo

[![TACTNET Demo](https://img.youtube.com/vi/-h0wWapHzws/0.jpg)](https://www.youtube.com/watch?v=-h0wWapHzws)

## Hardware

| Component | Quantity |
|-----------|----------|
| Raspberry Pi Zero 2W | 3 |
| REYAX RYLR998 LoRa 915MHz | 3 |
| Bosch BME280 (I2C, 3.3V) | 3 |
| SanDisk 32GB MicroSD | 3 |

## Wiring

**RYLR998 → Pi (UART, /dev/serial0, 115200 baud):**
- VDD → Pin 1 (3.3V)
- GND → Pin 6 (Ground)
- RXD → Pin 8 (GPIO14 TX)
- TXD → Pin 10 (GPIO15 RX)

**BME280 → Pi (I2C, /dev/i2c-1, 0x76):**
- VCC → Pin 1 (3.3V)
- GND → Pin 9 (Ground)
- SCL → Pin 5  (GPIO3)
- SDA → Pin 3 (GPIO2)

## Hardware Architecture

<img width = "500" alt = "TACTNET hardware architecture" src = "docs/architecture diagram.png">

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
## Key Technical Challenges

**1. `send()` reported success even for the failed transmission**
- **Symptom**: Node 2 and Node 3 both marked offline while being powered on.
-**How it was found**: Looking at raw response byte and found output came in two separate reads: `Response raw: '+'` and `Response raw: '+'`. `read()` was returning partial data, so response_string.find("+OK") failed against a string containing only '+.'  
- **The fix**: read one byte at a time until \n instead of one bulk read().

**2. Adding Node 3 silently broke Node 2, which had been working**
- **Symptom**: Network operational with Node 1 & 2 but broke when Node 3 was added. 
- **How it was found**: Printing the literal AT command before writing it. With one neighbor the message fit, while with two, broadcast() sent twice and both failed. 
- **The fix**: Compact `toString()` into `1|3|0|S|Payload: ...` instead of spelled out field names like 

**3. Half-duplex collisions**
- **Symptom**: Nodes 1 and 2 each transmitted correctly in isolation, but running both simultaneously broke communication in both directions
- **How it was found**: RYLR 998 is half-duplex and cannot receive while transmitting, so independent schedules on three nodes result overlap.  
- **The fix**: Reverted Nodes 2 and 3 to reactive transmission. They reply only immediately after receiving, making the schedule implicitly collision-free without a shared clock.

## Limitations & Future Improvements

- **Half-duplex radio** — the RYLR998 cannot transmit and receive simultaneously, requiring reactive timing coordination between nodes. 
- **Static routing** — relay paths are hardcoded in `main.cpp`. A dynamic routing protocol would allow nodes to discover and adapt routes automatically.
- **No persistent storage** — sensor readings are held in a 10-message circular buffer in RAM. Adding file logging would preserve historical data across reboots and enable trend analysis.
- **Single point of failure** — Node 1 is the sole broadcaster and dashboard data source. Distributing the broadcasting role across all nodes would make the network more resilient. However, Half-duplex  will have to be resolved for nodes to transmit and receive simultaneously when broadcasting. 
