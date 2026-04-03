//Header Guard
#ifndef NODE_H
#define NODE_H

// Dependency: Node stores and transmits Message objects
#include "message.h"

class Node{
private:
//fields
int node_ID;
Message messages[10];
int message_count;

public:
Node(int node_ID);

//receive messages between nodes
void receive_message(Message m);

//send messages between nodes
void send_message(Message m);

void print_node();
};

#endif 
