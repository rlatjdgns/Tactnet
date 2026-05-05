# TACTNET — Tactical Mesh Network


## Motivation

Environmental monitoring systems are expensive, fragile, and dependent on centralized infrastructure. We wanted to challenge that. What if a mesh network could be built from scratch — every driver, every routing algorithm, every scheduler — using nothing but bare-metal C++ and commodity hardware? This project was born out of that question, and out of firsthand experience maintaining TICN tactical communications infrastructure during military service in the Republic of Korea Army.

By fusing LoRa radio, I2C sensor drivers, and a custom priority-based scheduler, I built a self-healing mesh network that detects node failures, routes messages through relay nodes, encrypts all transmissions, and visualizes live sensor data on a web dashboard. No libraries. No shortcuts. Just hardware and code.

## Project Description


## Live Dashboard

[dashboard screenshot here]

## Architecture Diagram

[diagram here]

## Wiring Diagram

[wiring diagram here]

## Hardware Components

**Sensor Node**
- Raspberry Pi Zero 2W × 3
- SanDisk 32GB MicroSD × 3

**Radio Communication**
- REYAX RYLR998 LoRa Module 915MHz × 3 (UART, AES128, SX1278)

**Environmental Sensing**
- Bosch BME280 × 3 (I2C, temperature / humidity / pressure)

**Prototyping**
- Breadboard and jumper wires

## Software Architecture

**LoRaDriver** — bare-metal UART driver for RYLR998. Configures `/dev/serial0` via `termios`, sends AT commands, reads responses byte-by-byte, and handles AES128 key provisioning.

**BME280Driver** — bare-metal I2C driver. Opens `/dev/i2c-1`, reads 24 bytes of factory calibration data from chip registers, and applies Bosch's official compensation formulas to produce calibrated readings.

**Message** — fundamental data unit with sender ID, destination ID, message type enum (SENSORREADING, STATUS_PING, ERROR), and pipe-delimited payload. Serializes to compact format for LoRa transmission.

**Task / Scheduler** — priority-based task scheduler. Tasks track their own `last_executed` timestamp. Scheduler sorts by priority using bubble sort and executes tasks whose interval has elapsed.

**Node** — core class owning both LoRaDriver and BME280Driver. Manages neighbor addresses, circular message buffer, relay routing, and timestamp-based failover detection.

**Flask Dashboard** — Python Flask server reads `/tmp/tactnet_data.json` written by the C++ program and serves a live auto-refreshing dashboard showing all node sensor data, online/offline status, and environmental alerts.

## Build and Run

**Prerequisites (each Pi):**
```bash
sudo apt install -y g++ git python3-pip
pip3 install pyserial --break-system-packages
sudo raspi-config  # enable I2C and UART
```

**Configure LoRa address (run once per node):**
```bash
python3 -c "
import serial, time
s = serial.Serial('/dev/serial0', 115200, timeout=2)
s.write(b'AT+ADDRESS=1\r\n')  # change to 1, 2, or 3
time.sleep(0.5)
print(s.read_all())
s.close()
"
```

**Build:**
```bash
git clone https://github.com/rlatjdgns/Tactnet.git
cd Tactnet
g++ -std=c++17 src/message.cpp src/node.cpp src/task.cpp src/scheduler.cpp \
    src/LoRaDriver.cpp src/bme280_driver.cpp main.cpp -Iinclude -o tactnet
```

**Run:**
```bash
./tactnet 1 
./tactnet 2   
./tactnet 3   
```

**Dashboard:**
```bash
pip3 install flask --break-system-packages
cd dashboard && python3 app.py
# open http://tactnet-node1.local:5000
```

## Project Structure

```
Tactnet/
├── include/          # header files
├── src/              # driver and class implementations
├── dashboard/        # Flask web server and HTML template
├── docs/             # wiring diagram, architecture diagram
└── main.cpp          # node entry point (argv selects node behavior)
```