#include "message.h"
#include <iostream>
#include <sstream>
#include <iomanip>

Message :: Message(int senderID,int destinationID,int messageID,MessageType msgType,std::string payload){
    this-> senderID = senderID;
    this-> destinationID = destinationID;
    this-> messageID = messageID;
    this-> msgType = msgType;
    this-> payload = payload;
}    
int Message::get_sender_ID(){return senderID;}

std::string Message::get_payload(){return payload;}

void Message:: print() const{
    std::cout<<"-----------------------------------\n";
    std::cout << "*  Message "<< messageID << " received from Node " << senderID << " *\n";
    std::cout<<"-----------------------------------\n";
    std::stringstream ss(payload);
    std::string token;
    std::string values[3];
    int i = 0;
    while(std::getline(ss, token, '|') && i < 3){
        values[i++] = token;
    }
    switch(msgType){
    case MessageType::SENSORREADING:
        std::cout<<"Type: Sensor Reading\n";
        {
        std::cout << "      Temperature: " << std::fixed << std::setprecision(2) << std::stof(values[0]) << " °C\n";
        std::cout << "      Humidity: "    << std::fixed << std::setprecision(2) << std::stof(values[1]) << " %\n";
        std::cout << "      Pressure: "    << std::fixed << std::setprecision(2) << std::stof(values[2]) << " hPa\n";
        }
        break;
    case MessageType::STATUS_PING:
        std::cout << "Type: Status Ping (Node " << senderID << " readings)\n";
        std::cout << "      Temperature: " << std::fixed << std::setprecision(2) << std::stof(values[0]) << " °C\n";
        std::cout << "      Humidity: "    << std::fixed << std::setprecision(2) << std::stof(values[1]) << " %\n";
        std::cout << "      Pressure: "    << std::fixed << std::setprecision(2) << std::stof(values[2]) << " hPa\n";
        break;
    case MessageType::ERROR:
        std::cout<<"*ERROR*\n";
        if(std::stof(values[0])>35){
            std::cout<<"Temperature too hot: "<<values[0]<<"\n";
        }
        else if(std::stof(values[0])<10){
            std::cout<<"Temperature too cold: "<<values[0]<<"\n";
        }
        else if(std::stof(values[1])>95){
            std::cout<<"Humidity too high: "<< values[1]<<"\n";
        }    
        std::cout << "Payload: " << payload << "\n";
        break;
    default:
        std::cout<<"Unknown Message Type\n";
        std::cout << "Payload: " << payload << "\n";
        break;
    }
    std::cout<<"-----------------------------------\n";
}

std::string Message::toString() const{
    std::string typeStr;
    switch(msgType){
    case MessageType::SENSORREADING: typeStr = "S"; break;
    case MessageType::STATUS_PING: typeStr = "P"; break;
    case MessageType::ERROR: typeStr = "E"; break;
    default: typeStr = "?"; break;
    }
    return std::to_string(senderID) + "|" + std::to_string(destinationID) + "|" + std::to_string(messageID) + "|" + typeStr + "|Payload: " + payload;
}
