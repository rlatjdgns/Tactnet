#ifndef LORADRIVER_H
#define LORADRIVER_H
#include <string>

struct ReceivedMessage {
    int senderAddress;
    std::string payload;
    int rssi;
    int snr;
};

class LoRaDriver{
private:
int address;
int fd;


public:
LoRaDriver(int address);
void set_encryption();
bool begin();
bool send(int destination, std::string message);
ReceivedMessage receive();
void disconnect();
};


#endif