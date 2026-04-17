#include "message.h"
#include <iostream>
#include <sstream>
Message :: Message(int senderID,int destinationID,int messageID,MessageType msgType,std::string payload){
    this-> senderID = senderID;
    this-> destinationID = destinationID;
    this-> messageID = messageID;
    this-> msgType = msgType;
    this-> payload = payload;
}    

void Message:: print() const{
    std::cout << "  Message "<< messageID << " received from Node " << senderID << "\n";
    std::cout<<"-----------------------------------\n";
    switch(msgType){
    case MessageType::SENSORREADING:
        std::cout<<"Type: Sensor Reading\n";
        {
        std::stringstream ss(payload);
        std::string token;
        std::string values[3];
        int i = 0;
        while(std::getline(ss, token, '|') && i < 3){
            values[i++] = token;
        }
        std::cout << "      Temperature: " << values[0] << " °C\n";
        std::cout << "      Humidity: " << values[1] << " %\n";
        std::cout << "      Pressure: " << values[2] << " hPa\n";
        }

        break;
    case MessageType::STATUS_PING:
        std::cout<<"STATUS_PING\n";
        std::cout << "Payload: " << payload << "\n";
        break;
    case MessageType::ERROR:
        std::cout<<"ERROR\n";
        std::cout << "Payload: " << payload << "\n";
        break;
    case MessageType::RELAY:
        std::cout<<"RELAY\n";
        std::cout << "Payload: " << payload << "\n";
        break;
    default:
        std::cout<<"Invalid Message Type\n";
        std::cout << "Payload: " << payload << "\n";
        break;
    }
    std::cout<<"-----------------------------------\n";
}

std::string Message::toString() const{
    std::string typeStr;
    switch(msgType){
    case MessageType::SENSORREADING:
        typeStr = "SENSOR";
        break;
    case MessageType::STATUS_PING:
        typeStr = "STATUS";
        break;
    case MessageType::ERROR:
        typeStr = "ERROR";
        break;
    case MessageType::RELAY:
        typeStr = "RELAY";
        break;
    default:
        typeStr = "INVALID";
        break;
    }
    return std::to_string(senderID) + "|" +
           std::to_string(destinationID) + "|" +
           std::to_string(messageID) + "|" +
           typeStr + "|" +
           payload;
}
