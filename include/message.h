#ifndef MESSAGE_H
#define MESSAGE_H

class Message{
private:
    int senderID; 
    int destinationID;
    int messageID;
    int messageType;
    double payload;

public:
Message(int senderID,int destinationID,int messageID,int messageType,double payload);

void print();
};

#endif 
