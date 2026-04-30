#include "LoRaDriver.h"
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <sstream>
#include <iostream>

LoRaDriver::LoRaDriver(int address) : address(address), fd(-1){}

bool LoRaDriver::begin(){
    this-> fd = open("/dev/serial0", O_RDWR);
    if(fd==-1)return false;

    // configure UART: 115200 baud, 1 second read timeout
    struct termios tty;
    tcgetattr(this->fd, &tty);
    cfsetspeed(&tty, B115200);
    tty.c_cflag |= (CLOCAL | CREAD);
    tty.c_cc[VTIME] = 10;  
    tty.c_cc[VMIN] = 0; 
    tcsetattr(this->fd, TCSANOW, &tty);
    
    ::sleep(1); // Time for module to stablizie 
    set_encryption();
    return true;
}

void LoRaDriver::set_encryption(){
    // AES128 key on RYLR998 module
    std::string cmd = "AT+CPIN=4B9F2A7E1C3D8B5F6E0A4C2D9F7B3E1A\r\n";
    write(this->fd, cmd.c_str(), cmd.length());

    // verify key was set
    std::string verify = "AT+CPIN?\r\n";
    write(fd, verify.c_str(), verify.length());
    ::usleep(200000);
    char response[64];
    ::read(fd, response, sizeof(response));
}



bool LoRaDriver::send(int destination, std::string message){
    // Build and send AT+SEND command
    std::string AT_message = "AT+SEND=" + std::to_string(destination) + "," + std::to_string(message.length()) + "," + message + "\r\n";
    write(this->fd, AT_message.c_str(), AT_message.length());
    
    ::usleep(500000);
    std::string response_string = "";
    char c;
    int attempts = 0;
    for (int i = 0; i < 100; i++) {
        int n = ::read(fd, &c, 1);
        if (n > 0) {
            response_string += c;
            if (c == '\n') break;
        }
    }
    return true;
}

ReceivedMessage LoRaDriver::receive(){
    // Reading incoming data byte by byte until \n
    char buffer[256];
    int totalBytes = 0;
    char c;
    while(totalBytes < 255){
        int n = read(this->fd, &c, 1);
        if(n <= 0) break;
        buffer[totalBytes++] = c;
        if(c == '\n') break;
    }
    
    if(totalBytes <= 0)return ReceivedMessage{};
    
    std::string str(buffer, totalBytes);
    
    if(str.find("+RCV=")!=0)return ReceivedMessage{};
    
    str = str.substr(5,str.length()-2);
    
    // Parsing sender,len,payload,rssi,snr
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