#ifndef MESSAGE_H
#define MESSAGE_H
#include <string>

enum class MessageType {
    SENSORREADING,
    STATUS_PING,
    ERROR
};

class Message{
private:
    int senderID; 
    int destinationID;
    int messageID;
    MessageType msgType;
    std::string payload;
public:
Message(int senderID,int destinationID,int messageID, MessageType msgType,std::string payload);
Message() = default;
int get_sender_ID();
std::string get_payload();
void print() const;
std::string toString() const;
};

#endif 
