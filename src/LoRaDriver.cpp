#include "LoRaDriver.h"
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <sstream>
#include <iostream>

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
    tty.c_cc[VTIME] = 10;  
    tty.c_cc[VMIN] = 0; 
    tcsetattr(this->fd, TCSANOW, &tty);
    ::sleep(1);

    return true;
}

bool LoRaDriver::send(int destination, std::string message){
    std::string AT_message = "AT+SEND=" + std::to_string(destination) + "," + std::to_string(message.length()) + "," + message + "\r\n";
    write(this->fd, AT_message.c_str(), AT_message.length());
    ::usleep(500000);

    std::string response_string = "";
    char c;
    int attempts = 0;
    while(attempts < 100){
        int n = ::read(this->fd, &c, 1);
        if(n > 0){
            response_string += c;
            if(c == '\n') break;
        }
    attempts++;
    }
    std::cout << "Response: '" << response_string << "'\n";
    if(response_string.find("+OK") != std::string::npos){
        return true;
    } 
    return false;
}

ReceivedMessage LoRaDriver::receive(){
    char buffer[256];
    int totalBytes = 0;
    char c;
    
    while(totalBytes < 255){
        int n = read(this->fd, &c, 1);
        if(n <= 0) break;
        buffer[totalBytes++] = c;
        if(c == '\n') break;
    }
    
    if(totalBytes <= 0){
        ReceivedMessage empty{};
        return empty;
    }
    
    std::string str = std::string(buffer, totalBytes);
    
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