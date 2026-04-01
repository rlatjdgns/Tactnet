#include "message.h"
#include <iostream>
Message :: Message(int senderID,int destinationID,int messageID,int messageType,double payload){
    this-> senderID = senderID;
    this-> destinationID = destinationID;
    this-> messageID = messageID;
    this-> messageType = messageType;
    this-> payload = payload;
}    

void Message:: print(){
    std::cout << "Sender ID: " << senderID << "\n";
    std::cout << "destination ID: " << destinationID << "\n";
    std::cout << "message ID: " << messageID << "\n";
    std::cout << "messageType: " << messageType << "\n";
    std::cout << "payload: " << payload << "\n";
}

