#ifndef LORADRIVER_H
#define LORADRIVER_H
#include <string>

class LoRaDriver{
private:
int address;
int fd;

public:
LoRaDriver(int address);
bool begin();
bool send(int destination, std::string message);
std::string recieve();
void disconnect();
};


#endif