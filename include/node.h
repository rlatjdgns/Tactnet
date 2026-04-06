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
Node* neighbors[2];
int neighbor_count;

public:
Node(int node_ID);

//receive messages between nodes
bool receive_message(Message m);

//send messages between nodes
bool send_message(Message m, Node* receiver);

bool add_neighbor(Node* neighbor_node);

bool broadcast(Message m);

void print_node();
};

#endif 
