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
    std::cout << "Message "<< messageID << " received from Node" << senderID << "\n";
    std::cout<<"-----------------------------------\n";
    switch(msgType){
    case MessageType::SENSORREADING:
        std::cout<<"Type: SENSORREADING\n";
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
    std::string result = "";
    result += "Sender ID: " + std::to_string(senderID) + "|";
    result += "Destination ID: " + std::to_string(destinationID)+"|";
    result += "Message ID: " + std::to_string(messageID)+"|";
    switch(msgType){
    case MessageType::SENSORREADING:
        result += "Message Type: Sensor Reading|";
        break;
    case MessageType::STATUS_PING:
        result += "Message Type: Status Ping|";
        break;
    case MessageType::ERROR:
        result += "Message Type: Error|";
        break;
    case MessageType::RELAY:
        result += "Message Type: Relay|";
        break;
    default:
        result += "Message Type: Invalid";
        break;
    }

    result += "Payload: " + payload;
    return result;
};
