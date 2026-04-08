#ifndef MESSAGE_H
#define MESSAGE_H
#include <string>


enum class MessageType {
    TEMPERATURE,
    STATUS_PING,
    ERROR,
    RELAY
};

class Message{
private:
    int senderID; 
    int destinationID;
    int messageID;
    MessageType msgType;
    double payload;

public:
Message(int senderID,int destinationID,int messageID, MessageType msgType,double payload);
Message() = default;
void print() const;
std::string toString() const;

};

#endif 
