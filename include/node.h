#ifndef NODE_H
#define NODE_H

#include "message.h"
#include "scheduler.h"
#include "LoRaDriver.h"
class Node{
private:

int node_ID;
Message messages[10];
int message_count;
int neighbor_addresses[2];
int failure_count[2];
bool neighbor_online[2];
int neighbor_count;
int broadcast_count;
Scheduler scheduler;
LoRaDriver lora;

public:
Node(int node_ID);

int get_node_ID();

bool receive_message(Message m);

bool receive();

bool add_neighbor(int address);

bool broadcast(Message m);

void add_task(Task t);

void run_task();

void print_node();
};

#endif 
