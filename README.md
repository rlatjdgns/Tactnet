# Tactnet

## Overview
TACTNET is a personal project with three nodes built on Raspberry Pi Zero 2W, communicating sensor readings via LoRa. This project was motivated by my signaling work experience with the Republic of Korea Army (ROKA), where I maintained the TICN network of the battalion — managing signaling devices and the SLR antenna to sustain military server connectivity with upper command. That experience inspired me to replicate a similar system on my own: TACTNET.
Each node runs bare-metal C++ software with a priority-based task scheduler and handles mesh routing autonomously. In case of network failure, the active nodes detect the dropout, exclude the offline node, and re-establish the network among themselves.

## Tech Stack
Hardware:
* 3x Raspberry Pi Zero 2W
* 3x LoRa SX1276 915MHz
* 1x BME280 temperature/humidity/pressure sensor

Software:
* C++ (custom task scheduler, mesh routing)
* Python Flask (monitoring dashboard — coming soon)
* Linux (Raspberry Pi OS Lite)
