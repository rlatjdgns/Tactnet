#ifndef LORADRIVER_H
#define LORADRIVER_H

#include <string>

// Parsed incoming LoRa packet from RYLR998 +RCV response
struct ReceivedMessage {
    int senderAddress;
    std::string payload;
    int rssi;
    int snr;   
};

class LoRaDriver {
private:
    int address;  
    int fd;  

public:
    LoRaDriver(int address);
    bool begin();
    void set_encryption();
    bool send(int destination, std::string message);
    ReceivedMessage receive();
    void disconnect();
};

#endif