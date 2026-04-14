#include "LoRaDriver.h"
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <string>


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

std::string LoRaDriver::recieve(){
    char buffer[256];
    int bytesRead = read(this->fd, buffer, sizeof(buffer));
    return std::string(buffer, bytesRead);
}

void LoRaDriver::disconnect() {
    close(this->fd);
    this->fd = -1;
}