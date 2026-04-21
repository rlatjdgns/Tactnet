#ifndef NODE_H
#define NODE_H

#include "message.h"
#include "scheduler.h"
#include "LoRaDriver.h"
#include <ctime>
class Node{
private:

int node_ID;
Message messages[10];
int message_count;
int neighbor_addresses[2];
int neighbor_count;
Scheduler scheduler;
LoRaDriver lora;
time_t last_heard[2];
bool neighbor_online[2];

public:
Node(int node_ID);

int get_node_ID();

bool receive_message(Message m);

bool receive();

bool add_neighbor(int address);

bool broadcast(Message m);

void add_task(Task t);

void run_task();

void check_neighbors();

void print_node();
};

#endif 
