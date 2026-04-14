#include "LoRaDriver.h"
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <sstream>

LoRaDriver::LoRaDriver(int address){
    this->address = address;
    this->fd = -1;
}

bool LoRaDriver::begin(){
    this-> fd = open("/dev/serial0", O_RDWR);
    if(fd==-1){
        return false;
    }

    struct termios tty;
    tcgetattr(this->fd, &tty);
    cfsetspeed(&tty, B115200);
    tty.c_cflag |= (CLOCAL | CREAD);
    tcsetattr(this->fd, TCSANOW, &tty);

    return true;
}

bool LoRaDriver::send(int destination, std::string message){
    std::string AT_message = "AT+SEND="+ std::to_string(destination) +","+std::to_string(message.length())+","+ message+"\r\n";
    write(this->fd, AT_message.c_str(), AT_message.length());
    return true;
}

ReceivedMessage LoRaDriver::recieve(){
    char buffer[256];
    int bytesRead = read(this->fd, buffer, sizeof(buffer));
    if(bytesRead <= 0){
    ReceivedMessage empty{};
    return empty;
    }
    std::string str = std::string(buffer, bytesRead);
    if(str.find("+RCV=")!=0){
        ReceivedMessage empty{};
        return empty;
    }
    str = str.substr(5,str.length()-2);
    
    std::vector<std::string> tokens;
    
    int start = 0;
    int end = str.find(',');

    while(end != std::string::npos) { 
        tokens.push_back(str.substr(start, end - start)); 
        start = end + 1;  
        end = str.find(',', start); 
    }
    tokens.push_back(str.substr(start)); 

    ReceivedMessage msg;
    msg.senderAddress = std::stoi(tokens[0]);
    msg.payload = tokens[2];
    msg.rssi = std::stoi(tokens[3]);
    msg.snr = std::stoi(tokens[4]);

    return msg;
}

void LoRaDriver::disconnect() {
    close(this->fd);
    this->fd = -1;
}