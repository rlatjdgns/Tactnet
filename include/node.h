//Header Guard
#ifndef NODE_H
#define NODE_H

// Dependency: Node stores and transmits Message objects
#include "message.h"
#include "scheduler.h"
#include "LoRaDriver.h"
class Node{
private:
//fields
int node_ID;
Message messages[10];
int message_count;
int neighbor_addresses[2];
int neighbor_count;
Scheduler scheduler;
LoRaDriver lora;

public:
Node(int node_ID);

int get_node_ID();

bool receive_message(Message m);

bool add_neighbor(int address);

bool broadcast(Message m);

void add_task(Task t);

void run_task();

void print_node();
};

#endif 
