#include "message.h"
#include <iostream>
Message :: Message(int senderID,int destinationID,int messageID,MessageType msgType,double payload){
    this-> senderID = senderID;
    this-> destinationID = destinationID;
    this-> messageID = messageID;
    this-> msgType = msgType;
    this-> payload = payload;
}    

void Message:: print() const{
    std::cout << "Sender ID: " << senderID << "\n";
    std::cout << "Destination ID: " << destinationID << "\n";
    std::cout << "Message ID: " << messageID << "\n";
    switch(msgType){
    case MessageType::TEMPERATURE:
        std::cout<<"TEMPERATURE\n";
        break;
    case MessageType::STATUS_PING:
        std::cout<<"STATUS_PING\n";
        break;
    case MessageType::ERROR:
        std::cout<<"ERROR\n";
        break;
    case MessageType::RELAY:
        std::cout<<"RELAY\n";
        break;
    default:
        std::cout<<"Invalid Message Type\n";
        break;
    }
    std::cout << "Payload: " << payload << "\n";
    std::cout<<"---------------\n";
}

